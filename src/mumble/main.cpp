/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include "Overlay.h"
#include "MainWindow.h"
#include "ServerHandler.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "AudioWizard.h"
#include "Cert.h"
#include "Database.h"
#include "Log.h"
#include "Plugins.h"
#include "Global.h"
#include "LCD.h"
#ifdef USE_DBUS
#include "DBus.h"
#endif
#include "VersionCheck.h"
#include "NetworkConfig.h"

#ifdef BOOST_NO_EXCEPTIONS
namespace boost {
void throw_exception(std::exception const &) {
	qFatal("Boost exception caught!");
}
}
#endif

extern void os_init();
extern char *os_url;
extern char *os_lang;

int main(int argc, char **argv) {
	int res;

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
	os_init();
#endif

	QT_REQUIRE_VERSION(argc, argv, "4.4.1");

	// Initialize application object.
	QApplication a(argc, argv);
	a.setApplicationName(QLatin1String("Mumble"));
	a.setOrganizationName(QLatin1String("Mumble"));
	a.setOrganizationDomain(QLatin1String("mumble.sourceforge.net"));
	a.setQuitOnLastWindowClosed(false);

	Global::g_global_struct = new Global();

	QUrl url;
	if (a.arguments().count() > 1) {
		QUrl u = QUrl::fromEncoded(a.arguments().last().toUtf8());
		if (u.isValid() && (u.scheme() == QLatin1String("mumble")))
			url = u;
	}

#ifdef USE_DBUS
#ifdef Q_OS_WIN
	// By default, windbus expects the path to dbus-daemon to be in PATH, and the path
	// should contain bin\\, and the path to the config is hardcoded as ..\etc

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

#ifdef QT_NO_DEBUG
	QDBusInterface qdbi(QLatin1String("net.sourceforge.mumble.mumble"), QLatin1String("/"), QLatin1String("net.sourceforge.mumble.Mumble"));
	if (url.isValid()) {
		QDBusMessage reply=qdbi.call(QLatin1String("openUrl"), QLatin1String(url.toEncoded()));
		if (reply.type() == QDBusMessage::ReplyMessage) {
			return 0;
		}
	} else {
		QDBusMessage reply=qdbi.call(QLatin1String("focus"));
		if (reply.type() == QDBusMessage::ReplyMessage) {
			return 0;
		}
	}
#endif
#endif

	QFile inifile(QString::fromLatin1("%1/mumble.ini").arg(a.applicationDirPath()));
	if (inifile.exists() && inifile.permissions().testFlag(QFile::WriteUser))
		g.qs = new QSettings(inifile.fileName(), QSettings::IniFormat);
	else
		g.qs = new QSettings();

	// Load preferences
	g.s.load();

	DeferInit::run_initializers();

	if (! g.s.qsStyle.isEmpty()) {
		a.setStyle(g.s.qsStyle);
	}

	if (! g.s.qsSkin.isEmpty()) {
		QFile file(g.s.qsSkin);
		file.open(QFile::ReadOnly);
		QString styleSheet=QLatin1String(file.readAll());
		if (! styleSheet.isEmpty()) {
			QFileInfo fi(g.s.qsSkin);
			QDir::addSearchPath(QLatin1String("skin"), fi.path());
			a.setStyleSheet(styleSheet);
		}
	}

	QDir::addSearchPath(QLatin1String("skin"),QLatin1String(":/"));
	QDir::addSearchPath(QLatin1String("translation"), QLatin1String(":/"));

	QString qsSystemLocale = QLocale::system().name();

#ifdef Q_OS_MAC
	if (os_lang) {
		qWarning("Using Mac OS X system langauge as locale name");
		qsSystemLocale = QString(os_lang);
	}
#endif

	qWarning("Locale is %s", qPrintable(qsSystemLocale));

	QString locale = g.s.qsLanguage.isEmpty() ? qsSystemLocale : g.s.qsLanguage;

	QTranslator translator;
	if (translator.load(QLatin1String("translation:mumble_") + locale))
		a.installTranslator(&translator);

	QTranslator loctranslator;
	if (loctranslator.load(QLatin1String("mumble_") + locale, a.applicationDirPath()))
		a.installTranslator(&loctranslator);

	QTranslator qttranslator;
	if (qttranslator.load(QLibraryInfo::location(QLibraryInfo::TranslationsPath) + QLatin1String("/qt_") + locale))
		a.installTranslator(&qttranslator);
	else if (qttranslator.load(QLatin1String("translation:qt_") + locale))
		a.installTranslator(&qttranslator);

	// Initialize proxy settings
	NetworkConfig::SetupProxy();

	// Initialize logger
	g.l = new Log();

	// Initialize database
	g.db = new Database();

	// Initialize the serverhandler
	g.sh = new ServerHandler();
	g.sh->moveToThread(g.sh);

	g.o = new Overlay();
	g.o->setActive(g.s.bOverlayEnable);

	g.lcd = new LCD();

	g.nam = new QNetworkAccessManager();

	// Process any waiting events before initializing our MainWindow.
	// The mumble:// URL support for Mac OS X happens through AppleEvents,
	// so we need to loop a little before we begin.  If we were launched
	// through a URL, this should call a platform specific callback
	// (in os_macx.cpp) and point the `os_url' global to a valid URL.
	a.processEvents();

	// Main Window
	g.mw=new MainWindow(NULL);
	g.mw->show();

#ifdef USE_DBUS
	new MumbleDBus(g.mw);
	QDBusConnection::sessionBus().registerObject(QLatin1String("/"), g.mw);
	QDBusConnection::sessionBus().registerService(QLatin1String("net.sourceforge.mumble.mumble"));
#endif

	g.l->log(Log::Information, MainWindow::tr("Welcome to Mumble."));

	// Plugins
	g.p = new Plugins(NULL);
	g.p->rescanPlugins();

	// And the start the last chosen audio system.
	g.ai = AudioInputRegistrar::newFromChoice();
	if (g.ai)
		g.ai->start(QThread::HighestPriority);
	g.ao = AudioOutputRegistrar::newFromChoice();
	if (g.ao)
		g.ao->start(QThread::HighPriority);

	a.setQuitOnLastWindowClosed(false);

	if (g.s.bFirstTime) {
		if (QMessageBox::question(g.mw, MainWindow::tr("Mumble"), MainWindow::tr("This is the first time you're starting Mumble.<br />Would you like to go through the Audio Wizard to configure your soundcard?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
			AudioWizard *aw = new AudioWizard(g.mw);
			aw->exec();
			delete aw;
		}
		g.s.bFirstTime = false;
	}

	if (! CertWizard::validateCert(g.s.kpCertificate)) {
		CertWizard *cw = new CertWizard(g.mw);
		cw->exec();
		delete cw;
	}

	if (g.s.bUpdateCheck)
		new VersionCheck(true, g.mw);

	if (g.s.bPluginCheck)
		g.p->checkUpdates();

	if (url.isValid()) {
		g.mw->openUrl(url);
#ifdef Q_OS_MAC
	} else if (os_url) {
		g.mw->openUrl(QUrl::fromEncoded(QByteArray(os_url)));
#endif
	} else {
		g.mw->on_qaServerConnect_triggered();
	}

	res=a.exec();

	g.s.save();

	g.ao.reset();
	g.ai.reset();

	g.sh->disconnect();
	delete g.sh;
	delete g.mw;

	delete g.nam;
	delete g.lcd;

	delete g.db;
	delete g.p;
	delete g.l;


	delete g.o;

	delete g.qs;

	DeferInit::run_destroyers();

	delete Global::g_global_struct;
	Global::g_global_struct = NULL;
	return res;
}
