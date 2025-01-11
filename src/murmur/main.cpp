// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "EnvUtils.h"
#include "License.h"
#include "LogEmitter.h"
#include "Meta.h"
#include "SSL.h"
#include "Server.h"
#include "ServerDB.h"
#include "Version.h"

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

#include <openssl/crypto.h>

#ifdef Q_OS_WIN
#	include <intrin.h>
#else
#	include <fcntl.h>
#	include <sys/syslog.h>
#endif

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

	// Initialize meta parameter
	Meta::mp = std::make_unique< MetaParams >();

	MumbleSSL::initialize();

	QString inifile;
	QString supw;
	bool disableSu = false;
	bool wipeSsl   = false;
	bool wipeLogs  = false;
	int sunum      = 1;
#ifdef Q_OS_UNIX
	bool readPw = false;
#endif
	bool logGroups = false;
	bool logACL    = false;


	qInstallMessageHandler(murmurMessageOutputWithContext);

#ifdef Q_OS_WIN
	Tray tray(nullptr, &le);
#endif

	QStringList args = a.arguments();
	for (int i = 1; i < args.size(); i++) {
		bool bLast  = false;
		QString arg = args.at(i).toLower();
		if ((arg == "-supw")) {
			detach = false;
			if (i + 1 < args.size()) {
				i++;
				supw = args.at(i);
				if (i + 1 < args.size()) {
					i++;
					sunum = args.at(i).toInt();
				}
				bLast = true;
			} else {
#ifdef Q_OS_UNIX
				qFatal("-supw expects the password on the command line - maybe you meant -readsupw?");
#else
				qFatal("-supw expects the password on the command line");
#endif
			}
#ifdef Q_OS_UNIX
		} else if ((arg == "-readsupw")) {
			// Note that it is essential to set detach = false here. If this is ever to be changed, the code part
			// handling the readPw = true part has to be moved up so that it is executed before fork is called on Unix
			// systems.
			detach = false;
			readPw = true;
			if (i + 1 < args.size()) {
				i++;
				sunum = args.at(i).toInt();
			}
			bLast = true;
#endif
		} else if ((arg == "-disablesu")) {
			detach    = false;
			disableSu = true;
			if (i + 1 < args.size()) {
				i++;
				sunum = args.at(i).toInt();
			}
			bLast = true;
		} else if ((arg == "-ini") && (i + 1 < args.size())) {
			i++;
			inifile = args.at(i);
		} else if ((arg == "-wipessl")) {
			wipeSsl = true;
		} else if ((arg == "-wipelogs")) {
			wipeLogs = true;
		} else if ((arg == "-fg")) {
			detach = false;
		} else if ((arg == "-v")) {
			bVerbose = true;
		} else if ((arg == "-version") || (arg == "--version")) {
			// Print version and exit (print to regular std::cout to avoid adding any useless meta-information from
			// using e.g. qWarning
			std::cout << "Mumble server version " << Version::getRelease().toStdString() << std::endl;
			return 0;
		} else if (args.at(i) == QLatin1String("-license") || args.at(i) == QLatin1String("--license")) {
#ifdef Q_OS_WIN
			AboutDialog ad(nullptr, AboutDialogOptionsShowLicense);
			ad.exec();
			return 0;
#else
			qInfo("%s\n", qPrintable(License::license()));
			return 0;
#endif
		} else if (args.at(i) == QLatin1String("-authors") || args.at(i) == QLatin1String("--authors")) {
#ifdef Q_OS_WIN
			AboutDialog ad(nullptr, AboutDialogOptionsShowAuthors);
			ad.exec();
			return 0;
#else
			qInfo("%s\n",
				  "For a list of authors, please see https://github.com/mumble-voip/mumble/graphs/contributors");
			return 0;
#endif
		} else if (args.at(i) == QLatin1String("-third-party-licenses")
				   || args.at(i) == QLatin1String("--third-party-licenses")) {
#ifdef Q_OS_WIN
			AboutDialog ad(nullptr, AboutDialogOptionsShowThirdPartyLicenses);
			ad.exec();
			return 0;
#else
			qInfo("%s", qPrintable(License::printableThirdPartyLicenseInfo()));
			return 0;
#endif
		} else if ((arg == "-h") || (arg == "-help") || (arg == "--help")) {
			detach = false;
			qInfo("Usage: %s [-ini <inifile>] [-supw <password>]\n"
				  "  --version              Print version information and exit\n"
				  "  -ini <inifile>         Specify ini file to use.\n"
				  "  -supw <pw> [srv]       Set password for 'SuperUser' account on server srv.\n"
#ifdef Q_OS_UNIX
				  "  -readsupw [srv]        Reads password for server srv from standard input.\n"
#endif
				  "  -disablesu [srv]       Disable password for 'SuperUser' account on server srv.\n"
#ifdef Q_OS_UNIX
				  "  -limits                Tests and shows how many file descriptors and threads can be created.\n"
				  "                         The purpose of this option is to test how many clients Murmur can handle.\n"
				  "                         Murmur will exit after this test.\n"
#endif
				  "  -v                     Use verbose logging (include debug-logs).\n"
#ifdef Q_OS_UNIX
				  "  -fg                    Don't detach from console.\n"
#else
				  "  -fg                    Don't write to the log file.\n"
#endif
				  "  -wipessl               Remove SSL certificates from database.\n"
				  "  -wipelogs              Remove all log entries from database.\n"
				  "  -loggroups             Turns on logging for group changes for all servers.\n"
				  "  -logacls               Turns on logging for ACL changes for all servers.\n"
				  "  -version               Show version information.\n"
				  "\n"
				  "  -license               Show Murmur's license.\n"
				  "  -authors               Show Murmur's authors.\n"
				  "  -third-party-licenses  Show licenses for third-party software used by Murmur.\n"
				  "\n"
				  "If no inifile is provided, murmur will search for one in \n"
				  "default locations.",
				  qPrintable(args.at(0)));
			return 0;
#ifdef Q_OS_UNIX
		} else if (arg == "-limits") {
			detach = false;
			Meta::mp->read(inifile);
			unixhandler.setuid();
			unixhandler.finalcap();
			LimitTest::testLimits(a);
#endif
		} else if (arg == "-loggroups") {
			logGroups = true;
		} else if (arg == "-logacls") {
			logACL = true;
		} else {
			detach = false;
			qFatal("Unknown argument %s", qPrintable(args.at(i)));
		}
		if (bLast && (i + 1 != args.size())) {
			detach = false;
			qFatal("Password arguments must be last.");
		}
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
