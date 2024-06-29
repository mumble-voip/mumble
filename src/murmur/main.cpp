// Copyright 2008-2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_DBUS
#	include "DBus.h"
#endif

#include "EnvUtils.h"
#include "License.h"
#include "LogEmitter.h"
#include "Meta.h"
#include "SSL.h"
#include "Server.h"
#include "ServerDB.h"
#include "Version.h"

#include <boost/optional/optional_io.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <csignal>
#include <iostream>

#ifdef Q_OS_WIN
#	include "About.h"
#	include "Tray.h"

#	include <QtWidgets/QApplication>
#else
#	include "UnixMurmur.h"

#	include <QtCore/QCoreApplication>
#endif

#include <QtCore/QTextCodec>

#ifdef USE_DBUS
#	include <QtDBus/QDBusError>
#	include <QtDBus/QDBusServer>
#endif

#include <openssl/crypto.h>

#ifdef Q_OS_WIN
#	include <intrin.h>
#else
#	include <fcntl.h>
#	include <sys/syslog.h>
#endif

#include <CLI/CLI.hpp>

QFile *qfLog = nullptr;

static bool bVerbose = false;
#ifdef QT_NO_DEBUG
static bool detach = true;
#else
static bool detach = false;
#endif

#ifdef Q_OS_UNIX
static UnixMurmur *unixMurmur = nullptr;
#endif

Meta *meta = nullptr;

static LogEmitter le;

static QStringList qlErrors;

static void murmurMessageOutputQString(QtMsgType type, const QString &msg) {
#ifdef Q_OS_UNIX
	if (unixMurmur->logToSyslog) {
		int level;
		switch (type) {
			case QtDebugMsg:
				level = LOG_DEBUG;
				break;
			case QtWarningMsg:
				level = LOG_WARNING;
				break;
			case QtCriticalMsg:
				level = LOG_CRIT;
				break;
			case QtFatalMsg:
			default:
				level = LOG_ALERT;
				break;
		}
		syslog(level, "%s", qPrintable(msg));
		if (type == QtFatalMsg) {
			exit(1);
		}
		return;
	}
#endif

	char c;
	switch (type) {
		case QtDebugMsg:
			if (!bVerbose)
				return;
			c = 'D';
			break;
		case QtWarningMsg:
			c = 'W';
			break;
		case QtCriticalMsg:
			c = 'C';
			break;
		case QtFatalMsg:
			c = 'F';
			break;
		default:
			c = 'X';
	}
	QString m = QString::fromLatin1("<%1>%2 %3")
					.arg(QChar::fromLatin1(c))
					.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
					.arg(msg);

	if (!qfLog || !qfLog->isOpen()) {
#ifdef Q_OS_UNIX
		if (!detach)
			fprintf(stderr, "%s\n", qPrintable(m));
		else
			qlErrors << m;
#else
		qlErrors << m;
#	ifndef QT_NO_DEBUG
		fprintf(stderr, "%s\n", qPrintable(m));
#	endif
#endif
	} else {
		if (!qlErrors.isEmpty()) {
			foreach (const QString &e, qlErrors) {
				qfLog->write(e.toUtf8());
				qfLog->write("\n");
			}
			qlErrors.clear();
		}
		qfLog->write(m.toUtf8());
		qfLog->write("\n");
		qfLog->flush();
	}
	le.addLogEntry(m);
	if (type == QtFatalMsg) {
#ifdef Q_OS_UNIX
		if (detach) {
			if (qlErrors.isEmpty())
				qlErrors << msg;
			qlErrors << QString();
			m = qlErrors.join(QLatin1String("\n"));
			fprintf(stderr, "%s", qPrintable(m));
		}
#else
		::MessageBoxA(nullptr, qPrintable(m), "Murmur", MB_OK | MB_ICONWARNING);
#endif
		exit(1);
	}
}

static void murmurMessageOutputWithContext(QtMsgType type, const QMessageLogContext &ctx, const QString &msg) {
	Q_UNUSED(ctx);
	murmurMessageOutputQString(type, msg);
}

#ifdef USE_ICE
void IceParse(int &argc, char *argv[]);
void IceStart();
void IceStop();
#endif

void cleanup(int signum) {
	qWarning("Killing running servers");

	meta->killAll();

	qWarning("Shutting down");

#ifdef USE_DBUS
	delete MurmurDBus::qdbc;
	MurmurDBus::qdbc = nullptr;
#endif

#ifdef USE_ICE
	IceStop();
#endif

	delete qfLog;
	qfLog = nullptr;

	delete meta;

	qInstallMessageHandler(nullptr);

#ifdef Q_OS_UNIX
	if (!Meta::mp.qsPid.isEmpty()) {
		QFile pid(Meta::mp.qsPid);
		pid.remove();
	}
#endif
	exit(signum);
}

class CLIOptions {
public:
	bool quit = false;
	boost::optional< std::string > ini_file;
	boost::tuple< std::string, boost::optional< int > > supw_srv;
	boost::optional< int > disable_su_srv;
	bool verbose_logging = false;
	bool cli_detach      = detach;
	bool wipe_ssl        = false;
	bool wipe_logs       = false;
	bool log_groups      = false;
	bool log_acls        = false;

	bool print_authors            = false;
	bool print_license            = false;
	bool print_3rd_party_licenses = false;

#ifdef Q_OS_UNIX
	bool limits = false;
	boost::optional< int > read_supw_srv;
#endif

	static constexpr const char *const CLI_ABOUT_SECTION          = "About";
	static constexpr const char *const CLI_LOGGING_SECTION        = "Logging";
	static constexpr const char *const CLI_ADMINISTRATION_SECTION = "Administration";
	static constexpr const char *const CLI_CONFIGURATION_SECTION  = "Configuration";
	static constexpr const char *const CLI_TESTING_SECTION        = "Testing";
};

CLIOptions parseCLI(int argc, char **argv) {
	CLIOptions options;

	CLI::App app;
	app.set_version_flag("--version", Version::getRelease().toStdString());

	app.add_option_no_stream("-i,--ini", options.ini_file, "Specify ini file to use.")
		->option_text("<inifile>")
		->expected(1, 2)
		->check(CLI::ExistingFile)
		->group(CLIOptions::CLI_CONFIGURATION_SECTION);

	app.add_option("-w,--supw", options.supw_srv, "Set password for 'SuperUser' account on server srv.")
		->option_text("<pw> [srv]")
		->allow_extra_args()
		->expected(0, 1)
		->group(CLIOptions::CLI_ADMINISTRATION_SECTION);

#ifdef Q_OS_UNIX
	app.add_option_no_stream("-r,--readsupw", options.read_supw_srv,
							 "Reads password for server srv from standard input.")
		->option_text("[srv]")
		->default_val(1)
		->expected(0, 1)
		->group(CLIOptions::CLI_ADMINISTRATION_SECTION);

	app.add_flag("-l,--limits", options.limits,
				 "Tests and shows how many file descriptors and threads can be created.\n"
				 "The purpose of this option is to test how many clients Mumble server can handle.\n"
				 "Mumble server will exit after this test.")
		->group(CLIOptions::CLI_TESTING_SECTION);
#endif

	app.add_option_no_stream("-d,--disablesu", options.disable_su_srv,
							 "Disable password for 'SuperUser' account on server srv.")
		->option_text("[srv]")
		->expected(0, 1)
		->group(CLIOptions::CLI_ADMINISTRATION_SECTION);
	app.add_flag("-s,--wipessl", options.wipe_ssl, "Remove SSL certificates from database.")
		->group(CLIOptions::CLI_ADMINISTRATION_SECTION);

	app.add_flag("-v,--verbose", options.verbose_logging, "Use verbose logging (include debug-logs).")
		->group(CLIOptions::CLI_LOGGING_SECTION);
	app.add_flag("!-f,!--force-fg", options.cli_detach,
#ifdef Q_OS_UNIX
				 "Don't detach from console."
#else
				 "Don't write to the log file."
#endif
				 )
		->group(CLIOptions::CLI_LOGGING_SECTION);

	app.add_flag("-p,--wipelogs", options.wipe_logs, "Remove all log entries from database.")
		->group(CLIOptions::CLI_LOGGING_SECTION);
	app.add_flag("-g,--loggroups", options.log_groups, "Turns on logging for group changes for all servers.")
		->group(CLIOptions::CLI_LOGGING_SECTION);
	app.add_flag("-a,--logacls", options.log_acls, "Turns on logging for ACL changes for all servers.")
		->group(CLIOptions::CLI_LOGGING_SECTION);


	app.add_flag("--authors", options.print_authors, "Show Mumble server's authors.")
		->group(CLIOptions::CLI_ABOUT_SECTION);
	app.add_flag("--license", options.print_license, "Show Mumble server's license.")
		->group(CLIOptions::CLI_ABOUT_SECTION);
	app.add_flag("--3rd-party-licenses", options.print_3rd_party_licenses,
				 "Show licenses for third-party software used by Mumble server.")
		->group(CLIOptions::CLI_ABOUT_SECTION);


	app.footer("If no inifile is provided, Mumble server will search for one in\ndefault locations.");

	try {
		(app).parse(argc, argv);
	} catch (const CLI::ParseError &e) {
		std::stringstream info_stream, error_stream;
		app.exit(e, info_stream, error_stream);

		if (e.get_exit_code() != static_cast< int >(CLI::ExitCodes::Success)) {
			qFatal("%s", error_stream.str().c_str());
		} else {
			qInfo("%s", info_stream.str().c_str());
		}
		options.quit = true;
	}

	return options;
}

int main(int argc, char **argv) {
	// Check for SSE and MMX, but only in the windows binaries
#ifdef Q_OS_WIN
	int cpuinfo[4];
	__cpuid(cpuinfo, 1);

#	define MMXSSE 0x02800000
	if ((cpuinfo[3] & MMXSSE) != MMXSSE) {
		::MessageBoxA(nullptr, "Mumble requires a SSE capable processor (Pentium 3 / Ahtlon-XP)", "Mumble",
					  MB_OK | MB_ICONERROR);
		exit(0);
	}

	SetDllDirectory(L"");

#endif
	int res;

#ifdef USE_ICE
	IceParse(argc, argv);
#endif

#ifdef Q_OS_WIN
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);

	QIcon icon;
	icon.addFile(QLatin1String(":/murmur.16x16.png"));
	icon.addFile(QLatin1String(":/murmur.32x32.png"));
	icon.addFile(QLatin1String(":/murmur.64x64.png"));
	a.setWindowIcon(icon);
#else
#	ifndef Q_OS_MAC
	EnvUtils::setenv(QLatin1String("AVAHI_COMPAT_NOWARN"), QLatin1String("1"));
#	endif
	QCoreApplication a(argc, argv);
	UnixMurmur unixhandler;
	unixMurmur = &unixhandler;
	unixhandler.initialcap();
#endif
	a.setApplicationName("Murmur");
	a.setOrganizationName("Mumble");
	a.setOrganizationDomain("mumble.sourceforge.net");

	MumbleSSL::initialize();

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

#ifdef Q_OS_WIN
	// By default, windbus expects the path to dbus-daemon to be in PATH, and the path
	// should contain bin\\, and the path to the config is hardcoded as ..\etc

	{
		QString path = EnvUtils::getenv(QLatin1String("PATH"));
		if (path.isEmpty()) {
			qWarning() << "Failed to get PATH. Not adding application directory to PATH. DBus bindings may not work.";
		} else {
			QString newPath =
				QString::fromLatin1("%1;%2").arg(QDir::toNativeSeparators(a.applicationDirPath())).arg(path);
			if (!EnvUtils::setenv(QLatin1String("PATH"), newPath)) {
				qWarning() << "Failed to set PATH. DBus bindings may not work.";
			}
		}
	}
#endif


#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
	// For Qt >= 5.10 we use QRandomNumberGenerator that is seeded automatically
	qsrand(QDateTime::currentDateTime().toTime_t());
#endif

	qInstallMessageHandler(murmurMessageOutputWithContext);

	CLIOptions cli_options = parseCLI(argc, argv);

	if (cli_options.quit)
		return 0;

	if (cli_options.print_license) {
#ifdef Q_OS_WIN
		AboutDialog ad(nullptr, AboutDialogOptionsShowLicense);
		ad.exec();
		return 0;
#else
		qInfo("%s\n", qPrintable(License::license()));
		return 0;
#endif
	} else if (cli_options.print_authors) {
#ifdef Q_OS_WIN
		AboutDialog ad(nullptr, AboutDialogOptionsShowAuthors);
		ad.exec();
		return 0;
#else
		qInfo("%s\n", "For a list of authors, please see https://github.com/mumble-voip/mumble/graphs/contributors");
		return 0;
#endif
	} else if (cli_options.print_3rd_party_licenses) {
#ifdef Q_OS_WIN
		AboutDialog ad(nullptr, AboutDialogOptionsShowThirdPartyLicenses);
		ad.exec();
		return 0;
#else
		qInfo("%s", qPrintable(License::printableThirdPartyLicenseInfo()));
		return 0;
#endif
	}

	detach          = cli_options.cli_detach;
	QString inifile = QString::fromStdString(cli_options.ini_file.get_value_or(""));
	QString supw;
	bool disableSu = false;
	bool wipeSsl   = cli_options.wipe_ssl;
	bool wipeLogs  = cli_options.wipe_logs;
	int sunum      = 1;
#ifdef Q_OS_UNIX
	bool readPw = false;
#endif
	bool logGroups = cli_options.log_groups;
	bool logACL    = cli_options.log_acls;

	bVerbose = cli_options.verbose_logging;

	if (cli_options.disable_su_srv.has_value()) {
		detach    = false;
		disableSu = true;
		sunum     = cli_options.disable_su_srv.get();
	}

	if (!cli_options.supw_srv.get< 0 >().empty()) {
		supw  = QString::fromStdString(cli_options.supw_srv.get< 0 >());
		sunum = cli_options.supw_srv.get< 1 >().get_value_or(1);
#ifdef Q_OS_LINUX
	} else if (cli_options.read_supw_srv.has_value()) {
		// Note that it is essential to set detach = false here. If this is ever to be changed, the code part
		// handling the readPw = true part has to be moved up so that it is executed before fork is called on Unix
		// systems.

		detach = false;
		readPw = true;
		sunum  = cli_options.read_supw_srv.get();
	}

	if (cli_options.limits) {
		detach = false;
		Meta::mp.read(inifile);
		unixhandler.setuid();
		unixhandler.finalcap();
		LimitTest::testLimits(a);
#endif
	}
#ifdef Q_OS_WIN
	Tray tray(nullptr, &le);
#endif

	if (QSslSocket::supportsSsl()) {
		qInfo("SSL: OpenSSL version is '%s'", SSLeay_version(SSLEAY_VERSION));
	} else {
		qFatal("SSL: this version of Murmur is built against Qt without SSL Support. Aborting.");
	}

#ifdef Q_OS_UNIX
	inifile = unixhandler.trySystemIniFiles(inifile);
#endif

	Meta::mp.read(inifile);

	// Activating the logging of ACLs and groups via commandLine overwrites whatever is set in the ini file
	if (logGroups) {
		Meta::mp.bLogGroupChanges = logGroups;
	}
	if (logACL) {
		Meta::mp.bLogACLChanges = logACL;
	}

	// need to open log file early so log dir can be root owned:
	// http://article.gmane.org/gmane.comp.security.oss.general/4404
#ifdef Q_OS_UNIX
	unixhandler.logToSyslog = Meta::mp.qsLogfile == QLatin1String("syslog");
	if (detach && !Meta::mp.qsLogfile.isEmpty() && !unixhandler.logToSyslog) {
#else
	if (detach && !Meta::mp.qsLogfile.isEmpty()) {
#endif
		qfLog = new QFile(Meta::mp.qsLogfile);
		if (!qfLog->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
			delete qfLog;
			qfLog = nullptr;
#ifdef Q_OS_UNIX
			fprintf(stderr, "murmurd: failed to open logfile %s: no logging will be done\n",
					qPrintable(Meta::mp.qsLogfile));
#else
			qWarning("Failed to open logfile %s. No logging will be performed.", qPrintable(Meta::mp.qsLogfile));
#endif
		} else {
			qfLog->setTextModeEnabled(true);
			QFileInfo qfi(*qfLog);
			Meta::mp.qsLogfile = qfi.absoluteFilePath();
#ifdef Q_OS_UNIX
			if (Meta::mp.uiUid != 0 && fchown(qfLog->handle(), Meta::mp.uiUid, Meta::mp.uiGid) == -1) {
				qFatal("can't change log file owner to %d %d:%d - %s", qfLog->handle(), Meta::mp.uiUid, Meta::mp.uiGid,
					   strerror(errno));
			}
#endif
		}
#ifdef Q_OS_UNIX
	} else if (detach && unixhandler.logToSyslog) {
		openlog("murmurd", LOG_PID, LOG_DAEMON);
		syslog(LOG_DEBUG, "murmurd syslog adapter up and running");
#endif
	} else {
		detach = false;
	}

#ifdef Q_OS_UNIX
	unixhandler.setuid();
#endif

#ifdef Q_OS_UNIX
	// It is really important that these fork calls come before creating the
	// ServerDB object because sqlite uses POSIX locks on Unix systems (see
	// https://sqlite.org/lockingv3.html#:~:text=SQLite%20uses%20POSIX%20advisory%20locks,then%20database%20corruption%20can%20result.)
	// POSIX locks are automatically released if a process calls close() on any of
	// its open file descriptors for that file. If the ServerDB object, which
	// opens the sqlite database, is created before the fork, then the child will
	// inherit all open file descriptors and then close them on exit, releasing
	// all these POSIX locks. If another process (i.e. not murmur) makes any
	// connections to the database, it will think nobody else is connected
	// (because nothing is locked) and database corruption can (and likely will!)
	// ensue. This is particularly nasty if you have WAL mode enabled, because the
	// WAL file is deleted when the last connection to the database closes.
	if (detach) {
		if (fork() != 0) {
			_exit(0);
		}
		setsid();
		if (fork() != 0) {
			_exit(0);
		}

		if (!Meta::mp.qsPid.isEmpty()) {
			QFile pid(Meta::mp.qsPid);
			if (pid.open(QIODevice::WriteOnly)) {
				QFileInfo fi(pid);
				Meta::mp.qsPid = fi.absoluteFilePath();

				QTextStream out(&pid);
				out << getpid();
				pid.close();
			}
		}

		if (chdir("/") != 0)
			fprintf(stderr, "Failed to chdir to /");
		int fd;

		fd = open("/dev/null", O_RDONLY);
		dup2(fd, 0);
		close(fd);

		fd = open("/dev/null", O_WRONLY);
		dup2(fd, 1);
		close(fd);

		fd = open("/dev/null", O_WRONLY);
		dup2(fd, 2);
		close(fd);
	}
	unixhandler.finalcap();
#endif

	MumbleSSL::addSystemCA();

	ServerDB db;

	meta = new Meta();

#ifdef Q_OS_UNIX
	// It doesn't matter that this code comes after the forking because detach is
	// set to false when readPw is set to true.
	if (readPw) {
		char password[256];
		char *p;

		printf("Password: ");
		fflush(nullptr);
		if (fgets(password, 255, stdin) != password)
			qFatal("No password provided");
		p = strchr(password, '\r');
		if (p)
			*p = 0;
		p = strchr(password, '\n');
		if (p)
			*p = 0;

		supw = QLatin1String(password);
	}
#endif

	if (!supw.isNull()) {
		if (supw.isEmpty()) {
			qFatal("Superuser password can not be empty");
		}
		ServerDB::setSUPW(sunum, supw);
		qInfo("Superuser password set on server %d", sunum);
		return 0;
	}

	if (disableSu) {
		ServerDB::disableSU(sunum);
		qInfo("SuperUser password disabled on server %d", sunum);
		return 0;
	}

	if (wipeSsl) {
		qWarning("Removing all per-server SSL certificates from the database.");
		foreach (int sid, ServerDB::getAllServers()) {
			ServerDB::setConf(sid, "key");
			ServerDB::setConf(sid, "certificate");
			ServerDB::setConf(sid, "passphrase");
			ServerDB::setConf(sid, "sslDHParams");
		}
	}

	if (wipeLogs) {
		qWarning("Removing all log entries from the database.");
		ServerDB::wipeLogs();
	}

#ifdef USE_DBUS
	MurmurDBus::registerTypes();

	if (!Meta::mp.qsDBus.isEmpty()) {
		if (Meta::mp.qsDBus == "session")
			MurmurDBus::qdbc = new QDBusConnection(QDBusConnection::sessionBus());
		else if (Meta::mp.qsDBus == "system")
			MurmurDBus::qdbc = new QDBusConnection(QDBusConnection::systemBus());
		else {
			// QtDBus is not quite finished yet.
			qWarning("Warning: Peer-to-peer session support is currently nonworking.");
			MurmurDBus::qdbc = new QDBusConnection(QDBusConnection::connectToBus(Meta::mp.qsDBus, "mainbus"));
			if (!MurmurDBus::qdbc->isConnected()) {
				QDBusServer *qdbs = new QDBusServer(Meta::mp.qsDBus, &a);
				qWarning("%s", qPrintable(qdbs->lastError().name()));
				qWarning("%d", qdbs->isConnected());
				qWarning("%s", qPrintable(qdbs->address()));
				MurmurDBus::qdbc = new QDBusConnection(QDBusConnection::connectToBus(Meta::mp.qsDBus, "mainbus"));
			}
		}
		if (!MurmurDBus::qdbc->isConnected()) {
			qWarning("Failed to connect to D-Bus %s", qPrintable(Meta::mp.qsDBus));
		} else {
			new MetaDBus(meta);
			if (MurmurDBus::qdbc->isConnected()) {
				if (!MurmurDBus::qdbc->registerObject("/", meta)
					|| !MurmurDBus::qdbc->registerService(Meta::mp.qsDBusService)) {
					QDBusError e = MurmurDBus::qdbc->lastError();
					qWarning("Failed to register on DBus: %s %s", qPrintable(e.name()), qPrintable(e.message()));
				} else {
					qWarning("DBus registration succeeded");
				}
			}
		}
	}
#endif

#ifdef USE_ICE
	IceStart();
#endif

	meta->getOSInfo();

	qWarning("Murmur %s running on %s: %s: Booting servers", qPrintable(Version::toString(Version::get())),
			 qPrintable(meta->qsOS), qPrintable(meta->qsOSVersion));

	meta->bootAll();

	signal(SIGTERM, cleanup);
	signal(SIGINT, cleanup);

	res = a.exec();

	cleanup(0);

	return res;
}
