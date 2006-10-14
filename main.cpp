/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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

#ifdef Q_OS_WIN
#include "Overlay.h"
#endif

#include "MainWindow.h"
#include "ServerHandler.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "Database.h"
#include "Log.h"
#include "Plugins.h"
#include "Global.h"

#ifdef BOOST_NO_EXCEPTIONS
namespace boost {
	void throw_exception(std::exception const & e) {
		qFatal("Boost exception caught!");
	}
}
#endif

extern void os_init();

int main(int argc, char **argv)
{
	// Check for SSE and MMX, but only in the windows binaries

	int res;
#ifdef Q_OS_WIN
	os_init();
#endif

	QT_REQUIRE_VERSION(argc, argv, "4.1.0");

	// Initialize application object.
	QApplication a(argc, argv);
	a.setApplicationName("Mumble");
	a.setOrganizationName("Mumble");
	a.setOrganizationDomain("mumble.sourceforge.net");
	a.setQuitOnLastWindowClosed(false);

    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString(":/mumble_") + locale);
    a.installTranslator(&translator);

	g.qs = new QSettings();

	// Set application icon
	QIcon icon;
	icon.addFile(":/mumble.png.2");
	icon.addFile(":/mumble.png.1");
	icon.addFile(":/mumble.png.0");
	a.setWindowIcon(icon);

	// Load preferences
	g.s.load();

	// Initialize logger
	g.l = new Log();
	g.l->loadSettings();

	// Initialize database
	g.db = new Database();

	// Initialize the serverhandler
	g.sh = new ServerHandler();
	g.sh->moveToThread(g.sh);

#ifdef Q_OS_WIN
	g.o = new Overlay();
	g.o->setActive(g.s.bOverlayEnable);
#endif

	// Main Window
	g.mw=new MainWindow(NULL);
	g.mw->show();

	g.l->log(Log::Information, MainWindow::tr("Welcome to Mumble."));

	// Plugins
	g.p = new Plugins(NULL);
	g.p->rescanPlugins();

	// And the start the last chosen audio system.
	g.ai = AudioInputRegistrar::newFromChoice();
	g.ai->start(QThread::HighestPriority);
	g.ao = AudioOutputRegistrar::newFromChoice();

	a.setQuitOnLastWindowClosed(true);
	res=a.exec();

	g.s.save();
	g.l->saveSettings();

	g.ao.reset();
	g.ai.reset();

	g.sh->disconnect();

	// This causes QT to complain. Fatally. It's either a bug in
	// my code or the QT code.
	// delete g.sh;
	delete g.mw;

	delete g.db;
	delete g.l;

#ifdef Q_OS_WIN
	delete g.o;
#endif

	delete g.qs;

	return res;
}
