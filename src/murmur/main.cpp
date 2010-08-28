/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "murmur_pch.h"

#ifdef Q_OS_WIN
#include "Tray.h"
#endif

#include "Server.h"
#include "ServerDB.h"
#include "DBus.h"
#include "Meta.h"
#include "Version.h"
#include "SSL.h"

#ifdef Q_OS_UNIX
#include "UnixMurmur.h"
#endif

QFile *qfLog = NULL;

static bool bVerbose = false;
#ifdef QT_NO_DEBUG
static bool detach = true;
#else
static bool detach = false;
#endif

Meta *meta = NULL;

static LogEmitter le;

static QStringList qlErrors;

static void murmurMessageOutput(QtMsgType type, const char *msg) {
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
				qlErrors << QLatin1String(msg);
			qlErrors << QString();
			m = qlErrors.join(QLatin1String("\n"));
			fprintf(stderr, "%s", qPrintable(m));
		}
#else
		::MessageBoxA(NULL, qPrintable(m), "Murmur", MB_OK | MB_ICONWARNING);
#endif
		exit(0);
	}
}

#ifdef USE_ICE
void IceParse(int &argc, char *argv[]);
void IceStart();
void IceStop();
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
	setenv("AVAHI_COMPAT_NOWARN", "1", 1);
#endif
	QCoreApplication a(argc, argv);
	UnixMurmur unixhandler;
	unixhandler.initialcap();
#endif
	a.setApplicationName("Murmur");
	a.setOrganizationName("Mumble");
	a.setOrganizationDomain("mumble.sourceforge.net");

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

#ifdef Q_OS_WIN
	{
		size_t reqSize;
		_wgetenv_s(&reqSize, NULL, 0, L"PATH");
		if (reqSize > 0) {
			STACKVAR(wchar_t, buff, reqSize+1);
			_wgetenv_s(&reqSize, buff, reqSize, L"PATH");
			QString path = QString::fromLatin1("%1;%2").arg(QDir::toNativeSeparators(a.applicationDirPath())).arg(QString::fromWCharArray(buff));
			STACKVAR(wchar_t, buffout, path.length() + 1);
			path.toWCharArray(buffout);
			_wputenv_s(L"PATH", buffout);
		}
	}
#endif

	argc = a.argc();
	argv = a.argv();

	QString inifile;
	QString supw;
	bool wipeSsl = false;
	int sunum = 1;
#ifndef Q_OS_WIN
	bool readPw = false;
#endif

	qInstallMsgHandler(murmurMessageOutput);

#ifdef Q_OS_WIN
	Tray tray(NULL, &le);
#endif

	for (int i=1;i<argc;i++) {
		bool bLast = false;
		QString arg = QString(argv[i]).toLower();
		if ((arg == "-supw") && (i+1 < argc)) {
			i++;
			supw = argv[i];
			detach = false;
			if (i+1 < argc) {
				i++;
				sunum = QString::fromLatin1(argv[i]).toInt();
			}
			bLast = true;
#ifdef Q_OS_UNIX
		} else if ((arg == "-readsupw")) {
			detach = false;
			readPw = true;
			if (i+1 < argc) {
				i++;
				sunum = QString::fromLatin1(argv[i]).toInt();
			}
			bLast = true;
#endif
		} else if ((arg == "-ini") && (i+1 < argc)) {
			i++;
			inifile=argv[i];
		} else if ((arg == "-wipessl")) {
			wipeSsl = true;
		} else if ((arg == "-fg")) {
			detach = false;
		} else if ((arg == "-v")) {
			bVerbose = true;
		} else if ((arg == "-version") || (arg == "--version")) {
			detach = false;
			qFatal("%s -- %s", argv[0], MUMBLE_RELEASE);
		} else if ((arg == "-h") || (arg == "-help") || (arg == "--help")) {
			detach = false;
			qFatal("Usage: %s [-ini <inifile>] [-supw <password>]\n"
			       "  -ini <inifile>   Specify ini file to use.\n"
			       "  -supw <pw> [srv] Set password for 'SuperUser' account on server srv.\n"
#ifdef Q_OS_UNIX
			       "  -readsupw [srv]  Reads password for server srv from standard input.\n"
#endif
			       "  -v               Add verbose output.\n"
			       "  -fg              Don't detach from console [Unix-like systems only].\n"
			       "  -wipessl         Remove SSL certificates from database.\n"
			       "  -version         Show version information.\n"
			       "If no inifile is provided, murmur will search for one in \n"
			       "default locations.", argv[0]);
#ifdef Q_OS_UNIX
		} else if (arg == "-limits") {
			Meta::mp.read(inifile);
			unixhandler.setuid();
			unixhandler.finalcap();
			LimitTest::testLimits(a);
#endif
		} else {
			detach = false;
			qFatal("Unknown argument %s", argv[i]);
		}
		if (bLast && (i+1 != argc)) {
			detach = false;
			qFatal("Password arguments must be last.");
		}
	}


	Meta::mp.read(inifile);
	MumbleSSL::addSystemCA();

#ifdef Q_OS_UNIX
	unixhandler.setuid();
#endif
	ServerDB db;

	meta = new Meta();

#ifdef Q_OS_UNIX
	if (readPw) {
		char password[256];
		char *p;

		printf("Password: ");
		fflush(NULL);
		fgets(password, 255, stdin);
		p = strchr(password, '\r');
		if (p)
			*p = 0;
		p = strchr(password, '\n');
		if (p)
			*p = 0;

		supw = QLatin1String(password);
	}
#endif

	if (! supw.isEmpty()) {
		if (supw.isEmpty())
			qFatal("Superuser password can not be empty");
		ServerDB::setSUPW(sunum, supw);
		qFatal("Superuser password set on server %d", sunum);
	}

	if (wipeSsl) {
		qWarning("Removing all per-server SSL certificates from the database.");
		foreach(int sid, ServerDB::getAllServers()) {
			ServerDB::setConf(sid, "key");
			ServerDB::setConf(sid, "certificate");
			ServerDB::setConf(sid, "passphrase");
		}
	}

	if (detach && ! Meta::mp.qsLogfile.isEmpty()) {
		qfLog = new QFile(Meta::mp.qsLogfile);
		if (! qfLog->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
			delete qfLog;
			qfLog = NULL;
#ifdef Q_OS_UNIX
			fprintf(stderr, "murmurd: failed to open logfile %s: no logging will be done\n",qPrintable(Meta::mp.qsLogfile));
#else
			qWarning("Failed to open logfile %s. Will not detach.",qPrintable(Meta::mp.qsLogfile));
			detach = false;
#endif
		} else {
			qfLog->setTextModeEnabled(true);
			QFileInfo qfi(*qfLog);
			Meta::mp.qsLogfile = qfi.absoluteFilePath();
		}
	} else {
		detach = false;
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

		chdir("/");
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
			MurmurDBus::qdbc = QDBusConnection::sessionBus();
		else if (Meta::mp.qsDBus == "system")
			MurmurDBus::qdbc = QDBusConnection::systemBus();
		else {
			// QtDBus is not quite finished yet.
			qWarning("Warning: Peer-to-peer session support is currently nonworking.");
			MurmurDBus::qdbc = QDBusConnection::connectToBus(Meta::mp.qsDBus, "mainbus");
			if (! MurmurDBus::qdbc.isConnected()) {
				QDBusServer *qdbs = new QDBusServer(Meta::mp.qsDBus, &a);
				qWarning("%s",qPrintable(qdbs->lastError().name()));
				qWarning("%d",qdbs->isConnected());
				qWarning("%s",qPrintable(qdbs->address()));
				MurmurDBus::qdbc = QDBusConnection::connectToBus(Meta::mp.qsDBus, "mainbus");
			}
		}
		if (! MurmurDBus::qdbc.isConnected()) {
			qWarning("Failed to connect to D-Bus %s",qPrintable(Meta::mp.qsDBus));
		}
	}
	new MetaDBus(meta);
	if (MurmurDBus::qdbc.isConnected()) {
		if (! MurmurDBus::qdbc.registerObject("/", meta) || ! MurmurDBus::qdbc.registerService(Meta::mp.qsDBusService)) {
			QDBusError e=MurmurDBus::qdbc.lastError();
			qWarning("Failed to register on DBus: %s %s", qPrintable(e.name()), qPrintable(e.message()));
		} else {
			qWarning("DBus registration succeeded");
		}
	}
#endif

#ifdef USE_ICE
	IceStart();
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

#ifdef USE_ICE
	IceStop();
#endif

	delete qfLog;
	qfLog = NULL;

	delete meta;

	qInstallMsgHandler(NULL);

#ifdef Q_OS_UNIX
	if (! Meta::mp.qsPid.isEmpty()) {
		QFile pid(Meta::mp.qsPid);
		pid.remove();
	}
#endif
	return res;
}
