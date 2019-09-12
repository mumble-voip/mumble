// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_DBUS
# include "DBus.h"
#endif

#include "Server.h"
#include "ServerDB.h"
#include "Meta.h"
#include "Version.h"
#include "SSL.h"
#include "License.h"
#include "LogEmitter.h"
#include "EnvUtils.h"

#ifdef Q_OS_WIN
# include "About.h"
# include "Tray.h"

# include <QtWidgets/QApplication>
#else
# include "UnixMurmur.h"

# include <QtCore/QCoreApplication>
#endif

#include <QtCore/QTextCodec>

#ifdef USE_DBUS
# include <QtDBus/QDBusError>
# include <QtDBus/QDBusServer>
#endif

#include <openssl/crypto.h>

#ifdef Q_OS_WIN
# include <intrin.h>
#else
# include <fcntl.h>
# include <sys/syslog.h>
#endif

QFile *qfLog = NULL;

static bool bVerbose = false;
#ifdef QT_NO_DEBUG
static bool detach = true;
#else
static bool detach = false;
#endif

#ifdef Q_OS_UNIX
static UnixMurmur *unixMurmur = NULL;
#endif

Meta *meta = NULL;

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
			if (! bVerbose)
				return;
			c='D';
			break;
		case QtWarningMsg:
			c='W';
			break;
		case QtCriticalMsg:
			c='C';
			break;
		case QtFatalMsg:
			c='F';
			break;
		default:
			c='X';
	}
	QString m= QString::fromLatin1("<%1>%2 %3").arg(QChar::fromLatin1(c)).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(msg);

	if (! qfLog || ! qfLog->isOpen()) {
#ifdef Q_OS_UNIX
		if (! detach)
			fprintf(stderr, "%s\n", qPrintable(m));
		else
			qlErrors << m;
#else
		qlErrors << m;
#ifndef QT_NO_DEBUG
		fprintf(stderr, "%s\n", qPrintable(m));
#endif
#endif
	} else {
		if (! qlErrors.isEmpty()) {
			foreach(const QString &e, qlErrors) {
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
		::MessageBoxA(NULL, qPrintable(m), "Murmur", MB_OK | MB_ICONWARNING);
#endif
		exit(1);
	}
}

#if QT_VERSION >= 0x050000
static void murmurMessageOutputWithContext(QtMsgType type, const QMessageLogContext &ctx, const QString &msg) {
	Q_UNUSED(ctx);
	murmurMessageOutputQString(type, msg);
}
#else
static void murmurMessageOutput(QtMsgType type, const char *msg) {
	murmurMessageOutputQString(type, QString::fromUtf8(msg));
}
#endif

#ifdef USE_ICE
void IceParse(int &argc, char *argv[]);
void IceStart();
void IceStop();
#endif

#ifdef USE_GRPC
// From MurmurGRPCImpl.cpp.
void GRPCStart();
void GRPCStop();
#endif

int main(int argc, char **argv) {
	// Check for SSE and MMX, but only in the windows binaries
#ifdef Q_OS_WIN
	int cpuinfo[4];
	__cpuid(cpuinfo, 1);

#define MMXSSE 0x02800000
	if ((cpuinfo[3] & MMXSSE) != MMXSSE) {
		::MessageBoxA(NULL, "Mumble requires a SSE capable processor (Pentium 3 / Ahtlon-XP)", "Mumble", MB_OK | MB_ICONERROR);
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
#ifndef Q_OS_MAC
	EnvUtils::setenv(QLatin1String("AVAHI_COMPAT_NOWARN"), QLatin1String("1"));
#endif
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
#if QT_VERSION < 0x050000
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

#ifdef Q_OS_WIN
	// By default, windbus expects the path to dbus-daemon to be in PATH, and the path
	// should contain bin\\, and the path to the config is hardcoded as ..\etc

	{
		QString path = EnvUtils::getenv(QLatin1String("PATH"));
		if (path.isEmpty()) {
			qWarning() << "Failed to get PATH. Not adding application directory to PATH. DBus bindings may not work.";
		} else {
			QString newPath = QString::fromLatin1("%1;%2").arg(QDir::toNativeSeparators(a.applicationDirPath())).arg(path);
			if (!EnvUtils::setenv(QLatin1String("PATH"), newPath)) {
				qWarning() << "Failed to set PATH. DBus bindings may not work.";
			}
		}
	}
#endif

	QString inifile;
	QString supw;
	bool disableSu = false;
	bool wipeSsl = false;
	bool wipeLogs = false;
	int sunum = 1;
#ifdef Q_OS_UNIX
	bool readPw = false;
#endif

	qsrand(QDateTime::currentDateTime().toTime_t());
#if QT_VERSION >= 0x050000
	qInstallMessageHandler(murmurMessageOutputWithContext);
#else
	qInstallMsgHandler(murmurMessageOutput);
#endif

#ifdef Q_OS_WIN
	Tray tray(NULL, &le);
#endif

	QStringList args = a.arguments();
	for (int i = 1; i < args.size(); i++) {
		bool bLast = false;
		QString arg = args.at(i).toLower();
		if ((arg == "-supw")) {
			detach = false;
			if (i+1 < args.size()) {
				i++;
				supw = args.at(i);
				if (i+1 < args.size()) {
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
			detach = false;
			readPw = true;
			if (i+1 < args.size()) {
				i++;
				sunum = args.at(i).toInt();
			}
			bLast = true;
#endif
		} else if ((arg == "-disablesu")) {
		        detach = false;
		        disableSu = true;
		        if (i+1 < args.size()) {
		                i++;
		                sunum = args.at(i).toInt();
		        }
		        bLast = true;
		} else if ((arg == "-ini") && (i+1 < args.size())) {
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
			detach = false;
			qFatal("%s -- %s", qPrintable(args.at(0)), MUMBLE_RELEASE);
		} else if (args.at(i) == QLatin1String("-license") || args.at(i) == QLatin1String("--license")) {
#ifdef Q_OS_WIN
			AboutDialog ad(NULL, AboutDialogOptionsShowLicense);
			ad.exec();
			return 0;
#else
			qFatal("%s\n", qPrintable(License::license()));
#endif
		} else if (args.at(i) == QLatin1String("-authors") || args.at(i) == QLatin1String("--authors")) {
#ifdef Q_OS_WIN
			AboutDialog ad(NULL, AboutDialogOptionsShowAuthors);
			ad.exec();
			return 0;
#else
			qFatal("%s\n", qPrintable(License::authors()));
#endif
		} else if (args.at(i) == QLatin1String("-third-party-licenses") || args.at(i) == QLatin1String("--third-party-licenses")) {
#ifdef Q_OS_WIN
			AboutDialog ad(NULL, AboutDialogOptionsShowThirdPartyLicenses);
			ad.exec();
			return 0;
#else
			qFatal("%s", qPrintable(License::printableThirdPartyLicenseInfo()));
#endif
		} else if ((arg == "-h") || (arg == "-help") || (arg == "--help")) {
			detach = false;
			qFatal("Usage: %s [-ini <inifile>] [-supw <password>]\n"
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
			       "  -v                     Add verbose output.\n"
#ifdef Q_OS_UNIX
			       "  -fg                    Don't detach from console.\n"
#else
			       "  -fg                    Don't write to the log file.\n"
#endif
			       "  -wipessl               Remove SSL certificates from database.\n"
			       "  -wipelogs              Remove all log entries from database.\n"
			       "  -version               Show version information.\n"
			       "\n"
			       "  -license               Show Murmur's license.\n"
			       "  -authors               Show Murmur's authors.\n"
			       "  -third-party-licenses  Show licenses for third-party software used by Murmur.\n"
			       "\n"
			       "If no inifile is provided, murmur will search for one in \n"
			       "default locations.", qPrintable(args.at(0)));
#ifdef Q_OS_UNIX
		} else if (arg == "-limits") {
			detach = false;
			Meta::mp.read(inifile);
			unixhandler.setuid();
			unixhandler.finalcap();
			LimitTest::testLimits(a);
#endif
		} else {
			detach = false;
			qFatal("Unknown argument %s", qPrintable(args.at(i)));
		}
		if (bLast && (i+1 != args.size())) {
			detach = false;
			qFatal("Password arguments must be last.");
		}
	}

	if (QSslSocket::supportsSsl()) {
		qWarning("SSL: OpenSSL version is '%s'", SSLeay_version(SSLEAY_VERSION));
	} else {
		qFatal("SSL: this version of Murmur is built against Qt without SSL Support. Aborting.");
	}

#ifdef Q_OS_UNIX
	inifile = unixhandler.trySystemIniFiles(inifile);
#endif

	Meta::mp.read(inifile);

	// need to open log file early so log dir can be root owned:
	// http://article.gmane.org/gmane.comp.security.oss.general/4404
#ifdef Q_OS_UNIX
	unixhandler.logToSyslog = Meta::mp.qsLogfile == QLatin1String("syslog");
	if (detach && ! Meta::mp.qsLogfile.isEmpty() && !unixhandler.logToSyslog) {
#else
	if (detach && ! Meta::mp.qsLogfile.isEmpty()) {
#endif
		qfLog = new QFile(Meta::mp.qsLogfile);
		if (! qfLog->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
			delete qfLog;
			qfLog = NULL;
#ifdef Q_OS_UNIX
			fprintf(stderr, "murmurd: failed to open logfile %s: no logging will be done\n", qPrintable(Meta::mp.qsLogfile));
#else
			qWarning("Failed to open logfile %s. No logging will be performed.", qPrintable(Meta::mp.qsLogfile));
#endif
		} else {
			qfLog->setTextModeEnabled(true);
			QFileInfo qfi(*qfLog);
			Meta::mp.qsLogfile = qfi.absoluteFilePath();
#ifdef Q_OS_UNIX
			if (Meta::mp.uiUid != 0 && fchown(qfLog->handle(), Meta::mp.uiUid, Meta::mp.uiGid) == -1) {
				qFatal("can't change log file owner to %d %d:%d - %s", qfLog->handle(), Meta::mp.uiUid, Meta::mp.uiGid, strerror(errno));
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

	MumbleSSL::addSystemCA();

	ServerDB db;

	meta = new Meta();

#ifdef Q_OS_UNIX
	if (readPw) {
		char password[256];
		char *p;

		printf("Password: ");
		fflush(NULL);
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
		qFatal("Superuser password set on server %d", sunum);
	}

	if (disableSu) {
	        ServerDB::disableSU(sunum);
	        qFatal("SuperUser password disabled on server %d", sunum);
	}

	if (wipeSsl) {
		qWarning("Removing all per-server SSL certificates from the database.");
		foreach(int sid, ServerDB::getAllServers()) {
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

#ifdef Q_OS_UNIX
	if (detach) {
		if (fork() != 0) {
			_exit(0);
		}
		setsid();
		if (fork() != 0) {
			_exit(0);
		}

		if (! Meta::mp.qsPid.isEmpty()) {
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

#ifdef USE_DBUS
	MurmurDBus::registerTypes();

	if (! Meta::mp.qsDBus.isEmpty()) {
		if (Meta::mp.qsDBus == "session")
			MurmurDBus::qdbc = new QDBusConnection(QDBusConnection::sessionBus());
		else if (Meta::mp.qsDBus == "system")
			MurmurDBus::qdbc = new QDBusConnection(QDBusConnection::systemBus());
		else {
			// QtDBus is not quite finished yet.
			qWarning("Warning: Peer-to-peer session support is currently nonworking.");
			MurmurDBus::qdbc = new QDBusConnection(QDBusConnection::connectToBus(Meta::mp.qsDBus, "mainbus"));
			if (! MurmurDBus::qdbc->isConnected()) {
				QDBusServer *qdbs = new QDBusServer(Meta::mp.qsDBus, &a);
				qWarning("%s",qPrintable(qdbs->lastError().name()));
				qWarning("%d",qdbs->isConnected());
				qWarning("%s",qPrintable(qdbs->address()));
				MurmurDBus::qdbc = new QDBusConnection(QDBusConnection::connectToBus(Meta::mp.qsDBus, "mainbus"));
			}
		}
		if (! MurmurDBus::qdbc->isConnected()) {
			qWarning("Failed to connect to D-Bus %s",qPrintable(Meta::mp.qsDBus));
		} else {
			new MetaDBus(meta);
			if (MurmurDBus::qdbc->isConnected()) {
				if (! MurmurDBus::qdbc->registerObject("/", meta) || ! MurmurDBus::qdbc->registerService(Meta::mp.qsDBusService)) {
					QDBusError e=MurmurDBus::qdbc->lastError();
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

#ifdef USE_GRPC
	GRPCStart();
#else
	if (!meta->mp.qsGRPCAddress.isEmpty() || !meta->mp.qsGRPCCert.isEmpty() || !meta->mp.qsGRPCKey.isEmpty()) {
		qWarning("This version of Murmur was built without gRPC support. Ignoring 'grpc' option from configuration file.");
	}
#endif

	meta->getOSInfo();

	int major, minor, patch;
	QString strver;
	meta->getVersion(major, minor, patch, strver);

	qWarning("Murmur %d.%d.%d (%s) running on %s: %s: Booting servers", major, minor, patch, qPrintable(strver), qPrintable(meta->qsOS), qPrintable(meta->qsOSVersion));

	meta->bootAll();

	res=a.exec();

	qWarning("Killing running servers");

	meta->killAll();

	qWarning("Shutting down");

#ifdef USE_DBUS
	delete MurmurDBus::qdbc;
	MurmurDBus::qdbc = NULL;
#endif

#ifdef USE_ICE
	IceStop();
#endif

#ifdef USE_GRPC
	GRPCStop();
#endif

	delete qfLog;
	qfLog = NULL;

	delete meta;

#if QT_VERSION >= 0x050000
	qInstallMessageHandler(NULL);
#else
	qInstallMsgHandler(NULL);
#endif

#ifdef Q_OS_UNIX
	if (! Meta::mp.qsPid.isEmpty()) {
		QFile pid(Meta::mp.qsPid);
		pid.remove();
	}
#endif
	return res;
}
