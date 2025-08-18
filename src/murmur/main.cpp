// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBWrapper.h"
#include "EnvUtils.h"
#include "ExceptionUtils.h"
#include "License.h"
#include "LogEmitter.h"
#include "Meta.h"
#include "SSL.h"
#include "ServerApplication.h"
#include "Version.h"

#include <QSslSocket>

#include <cassert>
#include <csignal>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#ifdef Q_OS_WIN
#	include "About.h"
#	include "Tray.h"
#else
#	include "UnixMurmur.h"
#endif

#include <openssl/crypto.h>

#ifdef Q_OS_WIN
#	include <intrin.h>
#else
#	include <fcntl.h>
#	include <sys/syslog.h>
#endif

#include <optional>
#include <tuple>

#include <CLI/CLI.hpp>

extern QFile *qfLog;

static bool bVerbose = false;
#ifdef QT_NO_DEBUG
static bool detach = true;
#else
static bool detach = false;
#endif

#ifdef Q_OS_UNIX
static UnixMurmur *unixMurmur = nullptr;
#endif

extern Meta *meta;

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
			for (const QString &e : qlErrors) {
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

#ifdef USE_ICE
	IceStop();
#endif

	delete qfLog;
	qfLog = nullptr;

	delete meta;

	qInstallMessageHandler(nullptr);

#ifdef Q_OS_UNIX
	if (!Meta::mp->qsPid.isEmpty()) {
		QFile pid(Meta::mp->qsPid);
		pid.remove();
	}
#endif
	exit(signum);
}

struct CLIOptions {
	bool quit = false;
	std::optional< std::string > iniFile;
	std::optional< std::string > dbDumpPath;
	std::optional< std::string > dbImportPath;
	std::tuple< std::string, std::optional< unsigned int > > supwSrv;
	std::optional< unsigned int > disableSuSrv;
	bool verboseLogging = false;
	bool cliDetach      = detach;
	bool wipeSsl        = false;
	bool wipeLogs       = false;
	bool logGroups      = false;
	bool logAcls        = false;

	bool printAuthors            = false;
	bool printLicense            = false;
	bool printThirdPartyLicenses = false;

#ifdef Q_OS_UNIX
	bool limits = false;
	std::optional< unsigned int > readSupwSrv;
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

	app.add_option_no_stream("-i,--ini", options.iniFile, "Specify ini file to use.")
		->option_text("<inifile>")
		->expected(1, 2)
		->check(CLI::ExistingFile)
		->group(CLIOptions::CLI_CONFIGURATION_SECTION);

	app.add_option("--set-su-pw", options.supwSrv, "Set password for 'SuperUser' account on server srv.")
		->option_text("<pw> [srv]")
		->allow_extra_args()
		->expected(0, 1)
		->group(CLIOptions::CLI_ADMINISTRATION_SECTION);

#ifdef Q_OS_UNIX
	app.add_option_no_stream("--read-su-pw", options.readSupwSrv, "Reads password for server srv from standard input.")
		->option_text("[srv]")
		->default_val(1)
		->expected(0, 1)
		->group(CLIOptions::CLI_ADMINISTRATION_SECTION);

	app.add_flag("--limits", options.limits,
				 "Tests and shows how many file descriptors and threads can be created. "
				 "The purpose of this option is to test how many clients Mumble server can handle. "
				 "Mumble server will exit after this test.")
		->group(CLIOptions::CLI_TESTING_SECTION);
#endif

	app.add_option_no_stream("--disable-su", options.disableSuSrv,
							 "Disable password for 'SuperUser' account on server srv.")
		->option_text("[srv]")
		->expected(0, 1)
		->group(CLIOptions::CLI_ADMINISTRATION_SECTION);
	app.add_flag("--wipe-ssl", options.wipeSsl, "Remove SSL certificates from database.")
		->group(CLIOptions::CLI_ADMINISTRATION_SECTION);
	app.add_flag("--db-json-dump", options.dbDumpPath,
				 "Requests a JSON dump of the database to be written to the given file")
		->option_text("[file]")
		->expected(0, 1)
		->group(CLIOptions::CLI_ADMINISTRATION_SECTION);
	app.add_flag("--db-json-import", options.dbImportPath,
				 "Reads in the provide JSON file and imports its contents into the database")
		->option_text("[file]")
		->expected(0, 1)
		->group(CLIOptions::CLI_ADMINISTRATION_SECTION);

	app.add_flag("-v,--verbose", options.verboseLogging, "Use verbose logging (include debug-logs).")
		->group(CLIOptions::CLI_LOGGING_SECTION);
	app.add_flag("--detach,!--no-detach,--background,!--foreground", options.cliDetach,
				 "Whether to run in detached/background mode. In this mode, the program will detach and run as an "
				 "independent process in the background. Furthermore, logs will be written to the database instead of "
				 "to the console.")
		->group(CLIOptions::CLI_LOGGING_SECTION);

	app.add_flag("--wipe-logs", options.wipeLogs, "Remove all log entries from database.")
		->group(CLIOptions::CLI_LOGGING_SECTION);
	app.add_flag("--log-groups", options.logGroups, "Turns on logging for group changes for all servers.")
		->group(CLIOptions::CLI_LOGGING_SECTION);
	app.add_flag("--log-acls", options.logAcls, "Turns on logging for ACL changes for all servers.")
		->group(CLIOptions::CLI_LOGGING_SECTION);


	app.add_flag("--authors", options.printAuthors, "Show Mumble server's authors.")
		->group(CLIOptions::CLI_ABOUT_SECTION);
	app.add_flag("--license", options.printLicense, "Show Mumble server's license.")
		->group(CLIOptions::CLI_ABOUT_SECTION);
	app.add_flag("--third-party-licenses", options.printThirdPartyLicenses,
				 "Show licenses for third-party software used by Mumble server.")
		->group(CLIOptions::CLI_ABOUT_SECTION);


	app.footer("If no inifile is provided, Mumble server will search for one in default locations.");

	try {
		(app).parse(argc, argv);
	} catch (const CLI::ParseError &e) {
		std::stringstream info_stream, error_stream;
		app.exit(e, info_stream, error_stream);

		if (e.get_exit_code() != static_cast< int >(CLI::ExitCodes::Success)) {
			qWarning("%s", error_stream.str().c_str());
		} else {
			qInfo("%s", info_stream.str().c_str());
		}
		options.quit = true;
	}

	return options;
}

int main(int argc, char **argv) {
	try {
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

		ServerApplication a(argc, argv);
#ifdef Q_OS_WIN
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
		UnixMurmur unixhandler;
		unixMurmur = &unixhandler;
		unixhandler.initialcap();
#endif
		a.setApplicationName("Murmur");
		a.setOrganizationName("Mumble");
		a.setOrganizationDomain("mumble.sourceforge.net");

		// Initialize meta parameter
		Meta::mp = std::make_unique< MetaParams >();

		MumbleSSL::initialize();

		qInstallMessageHandler(murmurMessageOutputWithContext);

#ifdef Q_OS_WIN
		Tray tray(nullptr, &le);
#endif
		CLIOptions cli_options = parseCLI(argc, argv);
		if (cli_options.quit)
			return 0;

		if (cli_options.printLicense) {
#ifdef Q_OS_WIN
			AboutDialog ad(nullptr, AboutDialogOptionsShowLicense);
			ad.exec();
			return 0;
#else
			qInfo("%s\n", qPrintable(License::license()));
			return 0;
#endif
		} else if (cli_options.printAuthors) {
#ifdef Q_OS_WIN
			AboutDialog ad(nullptr, AboutDialogOptionsShowAuthors);
			ad.exec();
			return 0;
#else
			qInfo("%s\n",
				  "For a list of authors, please see https://github.com/mumble-voip/mumble/graphs/contributors");
			return 0;
#endif
		} else if (cli_options.printThirdPartyLicenses) {
#ifdef Q_OS_WIN
			AboutDialog ad(nullptr, AboutDialogOptionsShowThirdPartyLicenses);
			ad.exec();
			return 0;
#else
			qInfo("%s", qPrintable(License::printableThirdPartyLicenseInfo()));
			return 0;
#endif
		}

		detach          = cli_options.cliDetach;
		QString inifile = QString::fromStdString(cli_options.iniFile.value_or(""));
		QString supw;
		bool disableSu     = false;
		bool wipeSsl       = cli_options.wipeSsl;
		bool wipeLogs      = cli_options.wipeLogs;
		unsigned int sunum = 0;
#ifdef Q_OS_UNIX
		bool readPw = false;
#endif
		bool logGroups = cli_options.logGroups;
		bool logACL    = cli_options.logAcls;

		bVerbose = cli_options.verboseLogging;

		if (cli_options.disableSuSrv) {
			detach    = false;
			disableSu = true;
			sunum     = *cli_options.disableSuSrv;
		}

		if (!std::get< 0 >(cli_options.supwSrv).empty()) {
			supw  = QString::fromStdString(std::get< 0 >(cli_options.supwSrv));
			sunum = std::get< 1 >(cli_options.supwSrv).value_or< unsigned int >(1);
#ifdef Q_OS_LINUX
		} else if (cli_options.readSupwSrv) {
			// Note that it is essential to set detach = false here. If this is ever to be changed, the code part
			// handling the readPw = true part has to be moved up so that it is executed before fork is called on Unix
			// systems.

			detach = false;
			readPw = true;
			sunum  = *cli_options.readSupwSrv;
		}

		if (cli_options.limits) {
			detach = false;
			Meta::mp->read(inifile);
			unixhandler.setuid();
			unixhandler.finalcap();
			LimitTest::testLimits(a);
#endif
		}

		if (QSslSocket::supportsSsl()) {
			qInfo("SSL: OpenSSL version is '%s'", SSLeay_version(SSLEAY_VERSION));
		} else {
			qFatal("SSL: this version of Murmur is built against Qt without SSL Support. Aborting.");
		}

#ifdef Q_OS_UNIX
		inifile = unixhandler.trySystemIniFiles(inifile);
#endif

		Meta::mp->read(inifile);

		if (cli_options.dbDumpPath) {
			DBWrapper wrapper(Meta::getConnectionParameter());

			std::ofstream file(*cli_options.dbDumpPath);
			file << wrapper.exportDBToJSON().dump(2);

			qInfo("Dumped JSON representation of database contents to '%s'", cli_options.dbDumpPath->c_str());

			return 0;
		}

		if (cli_options.dbImportPath) {
			qInfo("Importing contents of '%s' into database", cli_options.dbImportPath->c_str());
			DBWrapper wrapper(Meta::getConnectionParameter());

			std::ifstream file(*cli_options.dbImportPath);

			nlohmann::json json;
			file >> json;

			wrapper.importFromJSON(json, true);

			return 0;
		}

		// Activating the logging of ACLs and groups via commandLine overwrites whatever is set in the ini file
		if (logGroups) {
			Meta::mp->bLogGroupChanges = logGroups;
		}
		if (logACL) {
			Meta::mp->bLogACLChanges = logACL;
		}

		// need to open log file early so log dir can be root owned:
		// http://article.gmane.org/gmane.comp.security.oss.general/4404
#ifdef Q_OS_UNIX
		unixhandler.logToSyslog = Meta::mp->qsLogfile == QLatin1String("syslog");
		if (detach && !Meta::mp->qsLogfile.isEmpty() && !unixhandler.logToSyslog) {
#else
		if (detach && !Meta::mp->qsLogfile.isEmpty()) {
#endif
			qfLog = new QFile(Meta::mp->qsLogfile);
			if (!qfLog->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
				delete qfLog;
				qfLog = nullptr;
#ifdef Q_OS_UNIX
				fprintf(stderr, "murmurd: failed to open logfile %s: no logging will be done\n",
						qPrintable(Meta::mp->qsLogfile));
#else
				qWarning("Failed to open logfile %s. No logging will be performed.", qPrintable(Meta::mp->qsLogfile));
#endif
			} else {
				qfLog->setTextModeEnabled(true);
				QFileInfo qfi(*qfLog);
				Meta::mp->qsLogfile = qfi.absoluteFilePath();
#ifdef Q_OS_UNIX
				if (Meta::mp->uiUid != 0 && fchown(qfLog->handle(), Meta::mp->uiUid, Meta::mp->uiGid) == -1) {
					qFatal("can't change log file owner to %d %d:%d - %s", qfLog->handle(), Meta::mp->uiUid,
						   Meta::mp->uiGid, strerror(errno));
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
		// Meta object (which in turn creates a database connection) because sqlite
		// uses POSIX locks on Unix systems (see
		// https://sqlite.org/lockingv3.html#:~:text=SQLite%20uses%20POSIX%20advisory%20locks,then%20database%20corruption%20can%20result.)
		// POSIX locks are automatically released if a process calls close() on any of
		// its open file descriptors for that file. If the Meta object, which
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

			if (!Meta::mp->qsPid.isEmpty()) {
				QFile pid(Meta::mp->qsPid);
				if (pid.open(QIODevice::WriteOnly)) {
					QFileInfo fi(pid);
					Meta::mp->qsPid = fi.absoluteFilePath();

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

		meta = new Meta(Meta::getConnectionParameter());
		meta->initPBKDF2IterationCount();

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
			meta->dbWrapper.setSuperUserPassword(sunum, supw.toStdString());
			qInfo("Superuser password set on server %u", sunum);
			return 0;
		}

		if (disableSu) {
			meta->dbWrapper.disableSuperUser(sunum);

			qInfo("SuperUser password disabled on server %u", sunum);
			return 0;
		}

		if (wipeSsl) {
			qWarning("Removing all per-server SSL certificates from the database.");

			meta->dbWrapper.clearAllPerServerSLLConfigurations();
		}

		if (wipeLogs) {
			qWarning("Removing all log entries from the database.");

			meta->dbWrapper.clearAllServerLogs();
		}

#ifdef USE_ICE
		IceStart();
#endif

		meta->getOSInfo();

		qWarning("Murmur %s running on %s: %s: Booting servers", qPrintable(Version::toString(Version::get())),
				 qPrintable(meta->qsOS), qPrintable(meta->qsOSVersion));

		meta->bootAll(Meta::getConnectionParameter(), true);

		signal(SIGTERM, cleanup);
		signal(SIGINT, cleanup);

		res = a.exec();

		cleanup(res);

		return res;
	} catch (const std::exception &e) {
		std::cerr << "[ERROR]: Exiting due to unhandled exception:\n";
		mumble::printExceptionMessage(std::cerr, e, 2);
		return 1;
	} catch (...) {
		std::cerr << "[ERROR]: Caught unknown error (this is a bug, please report it)" << std::endl;
		return 2;
	}
}
