/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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
#include <windows.h>
#include "Tray.h"
#endif
#ifdef Q_OS_UNIX
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif


#include "Server.h"
#include "ServerDB.h"
#include "DBus.h"
#include "Meta.h"

QFile *logfile;

static bool bVerbose = false;
#ifdef QT_NO_DEBUG
bool detach = true;
#else
bool detach = false;
#endif

Meta meta;

LogEmitter le;

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
		case QtFatalMsg:
			c='F';
			break;
		default:
			c='X';
	}
	QString m= QString::fromLatin1("<%1>%2 %3").arg(QChar::fromLatin1(c)).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(msg);

	if (! logfile || ! logfile->isOpen()) {
#ifdef Q_OS_UNIX
		fprintf(stderr, "%s\n", qPrintable(m));
#else
#ifdef QT_NO_DEBUG
		::MessageBoxA(NULL, msg, "Murmur", MB_OK | MB_ICONWARNING);
#else
		fprintf(stderr, "%s\n", qPrintable(m));
#endif
#endif
	} else {
		logfile->write(m.toUtf8());
		logfile->write("\n");
		logfile->flush();
	}
	le.addLogEntry(m);
	if (type == QtFatalMsg)
		exit(0);
}

int main(int argc, char **argv) {
	// Check for SSE and MMX, but only in the windows binaries
#ifdef Q_OS_WIN
#define cpuid(func,ax,bx,cx,dx) __asm__ __volatile__ ("cpuid": "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func));
#define MMXSSE 0x02800000
	unsigned int ax, bx, cx, dx;
	cpuid(1,ax,bx,cx,dx);
	if ((dx & MMXSSE) != MMXSSE) {
		::MessageBoxA(NULL, "Mumble requires a SSE capable processor (Pentium 3 / Ahtlon-XP)", "Mumble", MB_OK | MB_ICONERROR);
		exit(0);
	}
#endif
	int res;

#ifdef Q_OS_WIN
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);

	QIcon icon;
	icon.addFile(QLatin1String(":/mumble.png.2"));
	icon.addFile(QLatin1String(":/mumble.png.1"));
	icon.addFile(QLatin1String(":/mumble.png.0"));
	a.setWindowIcon(icon);
#else
	QCoreApplication a(argc, argv);
#endif
	a.setApplicationName("Murmur");
	a.setOrganizationName("Mumble");
	a.setOrganizationDomain("mumble.sourceforge.net");

	argc = a.argc();
	argv = a.argv();

	QString inifile;
	QString supw;
	int sunum = 1;

	qInstallMsgHandler(murmurMessageOutput);

#ifdef Q_OS_WIN
	Tray tray(NULL, &le);
#endif

	for (int i=1;i<argc;i++) {
		QString arg = QString(argv[i]).toLower();
		if ((arg == "-supw") && (i+1 < argc)) {
			i++;
			supw = argv[i];
			if (i < argc) {
				i++;
				sunum = QString::fromLatin1(argv[i]).toInt();
			}
		} else if ((arg == "-ini") && (i+1 < argc)) {
			i++;
			inifile=argv[i];
		} else if ((arg == "-fg")) {
			detach = false;
		} else if ((arg == "-v")) {
			bVerbose = true;
		} else if ((arg == "-h") || (arg == "--help")) {
			i++;
			qFatal("Usage: %s [-ini <inifile>] [-supw <password>]\n"
			       "  -ini <inifile>   Specify ini file to use.\n"
			       "  -supw <pw> [srv] Set password for 'SuperUser' account on server srv.\n"
			       "  -v               Add verbose output.\n"
			       "  -fg              Don't detach from console [Linux only].\n"
			       "If no inifile is provided, murmur will search for one in \n"
			       "default locations.",argv[0]);
		} else {
			qFatal("Unknown argument %s", argv[i]);
		}
	}

	Meta::mp.read(inifile);
	ServerDB db;

	if (! supw.isEmpty()) {
		ServerDB::setSUPW(sunum, supw);
		qFatal("Superuser password set on server %d", sunum);
	}

	if (detach && ! Meta::mp.qsLogfile.isEmpty()) {
		logfile = new QFile(Meta::mp.qsLogfile);
		if (! logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
			delete logfile;
			logfile = NULL;
			qWarning("Failed to open logfile %s. Will not detach.",qPrintable(Meta::mp.qsLogfile));
			detach = false;
		} else {
			logfile->setTextModeEnabled(true);
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

	MurmurDBus::registerTypes();
#endif

	if (! Meta::mp.qsDBus.isEmpty()) {
		qWarning("Oh Happy Day!");
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
		new MetaDBus(&meta);
	}
	if (MurmurDBus::qdbc.isConnected()) {
		MurmurDBus::qdbc.registerObject("/", &meta);
		MurmurDBus::qdbc.registerService("net.sourceforge.mumble.murmur");
	}

	meta.bootAll();

	res=a.exec();

	qWarning("Killing running servers");

	meta.killAll();

	qWarning("Shutting down");

	if (logfile)
		delete logfile;

	qInstallMsgHandler(NULL);

	return res;
}
