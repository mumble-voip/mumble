// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_OVERLAY
#	include "Overlay.h"
#endif
#include "AudioInput.h"
#include "AudioOutput.h"
#include "AudioWizard.h"
#include "Cert.h"
#include "Database.h"
#include "DeveloperConsole.h"
#ifdef Q_OS_WIN
#	include "GlobalShortcut_win.h"
#endif
#include "LCD.h"
#include "Log.h"
#include "LogEmitter.h"
#include "MainWindow.h"
#include "ServerHandler.h"
#ifdef USE_ZEROCONF
#	include "Zeroconf.h"
#endif
#ifdef USE_DBUS
#	include "DBus.h"
#endif
#ifdef USE_VLD
#	include "vld.h"
#endif
#include "ApplicationPalette.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "ClientUser.h"
#include "CrashReporter.h"
#include "EnvUtils.h"
#include "License.h"
#include "MumbleApplication.h"
#include "NetworkConfig.h"
#include "PluginInstaller.h"
#include "PluginManager.h"
#include "QtWidgetUtils.h"
#include "SSL.h"
#include "SocketRPC.h"
#include "TalkingUI.h"
#include "Themes.h"
#include "Translations.h"
#include "UserLockFile.h"
#include "VersionCheck.h"
#include "Global.h"

#include <QLocale>
#include <QScreen>
#include <QtCore/QProcess>
#include <QtGui/QDesktopServices>
#include <QtWidgets/QMessageBox>

#ifdef USE_DBUS
#	include <QtDBus/QDBusInterface>
#endif

#ifdef Q_OS_WIN
#	include <shellapi.h>
#endif


#ifdef BOOST_NO_EXCEPTIONS
namespace boost {
void throw_exception(std::exception const &) {
	qFatal("Boost exception caught!");
}
} // namespace boost
#endif

extern void os_init();
extern char *os_lang;

QPoint getTalkingUIPosition() {
	QPoint talkingUIPos = QPoint(0, 0);
	if (Global::get().s.qpTalkingUI_Position != Settings::UNSPECIFIED_POSITION
		&& Mumble::QtUtils::positionIsOnScreen(Global::get().s.qpTalkingUI_Position)) {
		// Restore last position
		talkingUIPos = Global::get().s.qpTalkingUI_Position;
	} else {
		// Place the TalkingUI next to the MainWindow by default
		const QPoint mainWindowPos = Global::get().mw->pos();
		const int horizontalBuffer = 10;
		const QPoint defaultPos =
			QPoint(mainWindowPos.x() + Global::get().mw->size().width() + horizontalBuffer, mainWindowPos.y());

		if (Mumble::QtUtils::positionIsOnScreen(defaultPos)) {
			talkingUIPos = defaultPos;
		}
	}

	// We have to ask the TalkingUI to adjust its size in order to get a proper
	// size from it (instead of a random default one).
	Global::get().talkingUI->adjustSize();
	const QSize talkingUISize = Global::get().talkingUI->size();

	// The screen should always be found at this point as we have chosen to pos to be on a screen
	const QScreen *screen  = Mumble::QtUtils::screenAt(talkingUIPos);
	const QRect screenGeom = screen ? screen->availableGeometry() : QRect(0, 0, 0, 0);

	// Check whether the TalkingUI fits on the screen in x-direction
	if (!Mumble::QtUtils::positionIsOnScreen(talkingUIPos + QPoint(talkingUISize.width(), 0))) {
		int overlap = talkingUIPos.x() + talkingUISize.width() - screenGeom.x() - screenGeom.width();

		// Correct the x coordinate but don't move it below 0
		talkingUIPos.setX(std::max(talkingUIPos.x() - overlap, 0));
	}
	// Check whether the TalkingUI fits on the screen in y-direction
	if (!Mumble::QtUtils::positionIsOnScreen(talkingUIPos + QPoint(0, talkingUISize.height()))) {
		int overlap = talkingUIPos.y() + talkingUISize.height() - screenGeom.y() - screenGeom.height();

		// Correct the x coordinate but don't move it below 0
		talkingUIPos.setY(std::max(talkingUIPos.x() - overlap, 0));
	}

	return talkingUIPos;
}

#ifdef Q_OS_WIN
// from os_early_win.cpp
extern int os_early_init();
// from os_win.cpp
extern HWND mumble_mw_hwnd;
#endif // Q_OS_WIN

#if defined(Q_OS_WIN) && !defined(MUMBLEAPP_DLL)
extern "C" __declspec(dllexport) int main(int argc, char **argv) {
#else
int main(int argc, char **argv) {
#endif
	int res = 0;

#if defined(Q_OS_WIN)
	int ret = os_early_init();
	if (ret != 0) {
		return ret;
	}
#endif

#if defined(Q_OS_WIN)
	SetDllDirectory(L"");
#else
#	ifndef Q_OS_MAC
	EnvUtils::setenv(QLatin1String("AVAHI_COMPAT_NOWARN"), QLatin1String("1"));
#	endif
#endif

	// Initialize application object.
	MumbleApplication a(argc, argv);
	a.setApplicationName(QLatin1String("Mumble"));
	a.setOrganizationName(QLatin1String("Mumble"));
	a.setOrganizationDomain(QLatin1String("mumble.sourceforge.net"));
	a.setQuitOnLastWindowClosed(false);

#if QT_VERSION >= 0x050100
	a.setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

#ifdef Q_OS_WIN
	a.installNativeEventFilter(&a);
#endif

	MumbleSSL::initialize();

#ifdef USE_SBCELT
	{
		QDir d(a.applicationVersionRootPath());
		QString helper = d.absoluteFilePath(QString::fromLatin1("sbcelt-helper"));
		EnvUtils::setenv(QLatin1String("SBCELT_HELPER_BINARY"), helper.toUtf8().constData());
	}
#endif

	// This argument has to be parsed first, since it's value is needed to create the global struct,
	// which other switches are modifying. If it is parsed first, the order of the arguments does not matter.
	QStringList args = a.arguments();
	const int index  = std::max(args.lastIndexOf(QLatin1String("-c")), args.lastIndexOf(QLatin1String("--config")));
	if (index >= 0) {
		if (index + 1 < args.count()) {
			QFile inifile(args.at(index + 1));
			if (inifile.exists() && inifile.permissions().testFlag(QFile::WriteUser)) {
				Global::g_global_struct = new Global(args.at(index + 1));
			} else {
				printf("%s", qPrintable(MainWindow::tr("Configuration file %1 does not exist or is not writable.\n")
											.arg(args.at(index + 1))));
				return 1;
			}
		} else {
			qCritical("Missing argument for --config!");
			return 1;
		}
	} else {
		Global::g_global_struct = new Global();
	}

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
	// For Qt >= 5.10 we use QRandomNumberGenerator that is seeded automatically
	qsrand(QDateTime::currentDateTime().toTime_t());
#endif

	Global::get().le = QSharedPointer< LogEmitter >(new LogEmitter());
	Global::get().c  = new DeveloperConsole();

	os_init();

	bool bAllowMultiple       = false;
	bool suppressIdentity     = false;
	bool customJackClientName = false;
	bool bRpcMode             = false;
	bool printTranslationDirs = false;
	QString rpcCommand;
	QUrl url;
	QDir qdCert(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
	QStringList extraTranslationDirs;
	QString localeOverwrite;

	QStringList pluginsToBeInstalled;
	if (a.arguments().count() > 1) {
		for (int i = 1; i < args.count(); ++i) {
			if (args.at(i) == QLatin1String("-h") || args.at(i) == QLatin1String("--help")
#if defined(Q_OS_WIN)
				|| args.at(i) == QLatin1String("/?")
#endif
			) {
				QString helpMessage =
					MainWindow::tr("Usage: mumble [options] [<url> | <plugin_list>]\n"
								   "\n"
								   "<url> specifies a URL to connect to after startup instead of showing\n"
								   "the connection window, and has the following form:\n"
								   "mumble://[<username>[:<password>]@]<host>[:<port>][/<channel>[/"
								   "<subchannel>...]][?version=<x.y.z>]\n"
								   "\n"
								   "<plugin_list> is a list of plugin files that shall be installed"
								   "\n"
								   "The version query parameter has to be set in order to invoke the\n"
								   "correct client version. It currently defaults to 1.2.0.\n"
								   "\n"
								   "Valid options are:\n"
								   "  -h, --help    Show this help text and exit.\n"
								   "  -m, --multiple\n"
								   "                Allow multiple instances of the client to be started.\n"
								   "  -c, --config\n"
								   "                Specify an alternative configuration file.\n"
								   "                If you use this to run multiple instances of Mumble at once,\n"
								   "                make sure to set an alternative 'database' value in the config.\n"
								   "  --default-certificate-dir <dir>\n"
								   "                Specify an alternative default certificate path.\n"
								   "                This path is only used if there is no certificate loaded\n"
								   "                from the settings.\n"
								   "  -n, --noidentity\n"
								   "                Suppress loading of identity files (i.e., certificates.)\n"
								   "  -jn, --jackname <arg>\n"
								   "                Set custom Jack client name.\n"
								   "  --license\n"
								   "                Show the Mumble license.\n"
								   "  --authors\n"
								   "                Show the Mumble authors.\n"
								   "  --third-party-licenses\n"
								   "                Show licenses for third-party software used by Mumble.\n"
								   "  --window-title-ext <arg>\n"
								   "                Sets a custom window title extension.\n"
								   "  --dump-input-streams\n"
								   "                Dump PCM streams at various parts of the input chain\n"
								   "                (useful for debugging purposes)\n"
								   "                - raw microphone input\n"
								   "                - speaker readback for echo cancelling\n"
								   "                - processed microphone input\n"
								   "  --print-echocancel-queue\n"
								   "                Print on stdout the echo cancellation queue state\n"
								   "                (useful for debugging purposes)\n"
								   "  --translation-dir <dir>\n"
								   "                Specifies an additional translation fir <dir> in which\n"
								   "                Mumble will search for translation files that overwrite\n"
								   "                the bundled ones\n"
								   "                Directories added this way have higher priority than\n"
								   "                the default locations used otherwise\n"
								   "  --print-translation-dirs\n"
								   "                Print out the paths in which Mumble will search for\n"
								   "                translation files that overwrite the bundled ones.\n"
								   "                (Useful for translators testing their translations)\n"
								   "  --locale <locale>\n"
								   "                Overwrite the locale in Mumble's settings with a\n"
								   "                locale that corresponds to the given locale string.\n"
								   "                If the format is invalid, Mumble will error.\n"
								   "                Otherwise the locale will be permanently saved to\n"
								   "                Mumble's settings."
								   "\n");
				QString rpcHelpBanner = MainWindow::tr("Remote controlling Mumble:\n"
													   "\n");
				QString rpcHelpMessage =
					MainWindow::tr("Usage: mumble rpc <action> [options]\n"
								   "\n"
								   "It is possible to remote control a running instance of Mumble by using\n"
								   "the 'mumble rpc' command.\n"
								   "\n"
								   "Valid actions are:\n"
								   "  mute\n"
								   "                Mute self\n"
								   "  unmute\n"
								   "                Unmute self\n"
								   "  togglemute\n"
								   "                Toggle self-mute status\n"
								   "  deaf\n"
								   "                Deafen self\n"
								   "  undeaf\n"
								   "                Undeafen self\n"
								   "  toggledeaf\n"
								   "                Toggle self-deafen status\n"
								   "  starttalking\n"
								   "                Start talking\n"
								   "  stoptalking\n"
								   "                Stop talking\n"
								   "\n");

				QString helpOutput = helpMessage + rpcHelpBanner + rpcHelpMessage;
				if (bRpcMode) {
					helpOutput = rpcHelpMessage;
				}

#if defined(Q_OS_WIN)
				QMessageBox::information(nullptr, MainWindow::tr("Invocation"), helpOutput);
#else
				printf("%s", qPrintable(helpOutput));
#endif
				return 1;
			} else if (args.at(i) == QLatin1String("-m") || args.at(i) == QLatin1String("--multiple")) {
				bAllowMultiple = true;
			} else if (args.at(i) == QLatin1String("-n") || args.at(i) == QLatin1String("--noidentity")) {
				suppressIdentity                  = true;
				Global::get().s.bSuppressIdentity = true;
			} else if (args.at(i) == QLatin1String("-jn") || args.at(i) == QLatin1String("--jackname")) {
				if (i + 1 < args.count()) {
					Global::get().s.qsJackClientName = QString(args.at(i + 1));
					customJackClientName             = true;
					++i;
				} else {
					qCritical("Missing argument for --jackname!");
					return 1;
				}
			} else if (args.at(i) == QLatin1String("--window-title-ext")) {
				if (i + 1 < args.count()) {
					Global::get().windowTitlePostfix = QString(args.at(i + 1));
					++i;
				} else {
					qCritical("Missing argument for --window-title-ext!");
					return 1;
				}
			} else if (args.at(i) == QLatin1String("-license") || args.at(i) == QLatin1String("--license")) {
				printf("%s\n", qPrintable(License::license()));
				return 0;
			} else if (args.at(i) == QLatin1String("-authors") || args.at(i) == QLatin1String("--authors")) {
				printf("%s\n", qPrintable(License::authors()));
				return 0;
			} else if (args.at(i) == QLatin1String("-third-party-licenses")
					   || args.at(i) == QLatin1String("--third-party-licenses")) {
				printf("%s", qPrintable(License::printableThirdPartyLicenseInfo()));
				return 0;
			} else if (args.at(i) == QLatin1String("rpc")) {
				bRpcMode = true;
				if (args.count() - 1 > i) {
					rpcCommand = QString(args.at(i + 1));
				} else {
					QString rpcError = MainWindow::tr("Error: No RPC command specified");
#if defined(Q_OS_WIN)
					QMessageBox::information(nullptr, MainWindow::tr("RPC"), rpcError);
#else
					printf("%s\n", qPrintable(rpcError));
#endif
					return 1;
				}
			} else if (args.at(i) == QLatin1String("--dump-input-streams")) {
				Global::get().bDebugDumpInput = true;
			} else if (args.at(i) == QLatin1String("--print-echocancel-queue")) {
				Global::get().bDebugPrintQueue = true;
			} else if (args.at(i) == QLatin1String("-c") || args.at(i) == QLatin1String("--config")) {
				//	We already parsed these arguments above, so just skip over them here
				++i;
			} else if (args.at(i) == QLatin1String("--default-certificate-dir")) {
				if (i + 1 < args.count()) {
					qdCert = QDir(args.at(i + 1));
					// I suppose we should really be checking whether the directory is writable here too,
					// but there are some subtleties with doing that:
					// (doc.qt.io/qt-5/qfile.html#platform-specific-issues)
					// so we can just let things fail down below when this directory is used.
					if (!qdCert.exists()) {
						printf("%s", qPrintable(MainWindow::tr("Directory %1 does not exist.\n").arg(args.at(i + 1))));
						return 1;
					}
					++i;
				} else {
					qCritical("Missing argument for --default-certificate-dir!");
					return 1;
				}
			} else if (args.at(i) == "--print-translation-dirs") {
				printTranslationDirs = true;
			} else if (args.at(i) == "--translation-dir") {
				if (i + 1 < args.count()) {
					extraTranslationDirs.append(args.at(i + 1));
					i++;
				} else {
					qCritical("Missing argument for --translation-dir!");
					return 1;
				}
			} else if (args.at(i) == "--locale") {
				if (i + 1 < args.count()) {
					localeOverwrite = args.at(i + 1);
					i++;
				} else {
					qCritical("Missing argument for --locale!");
					return 1;
				}
			} else {
				if (PluginInstaller::canBePluginFile(args.at(i))) {
					pluginsToBeInstalled << args.at(i);
				} else {
					if (!bRpcMode) {
						QUrl u = QUrl::fromEncoded(args.at(i).toUtf8());
						if (u.isValid() && (u.scheme() == QLatin1String("mumble"))) {
							url = u;
						} else {
							QFile f(args.at(i));
							if (f.exists()) {
								url = QUrl::fromLocalFile(f.fileName());
							}
						}
					}
				}
			}
		}
	}

	if (printTranslationDirs) {
		QString infoString = QObject::tr("The directories in which Mumble searches for extra translation files are:\n");

		int counter = 1;
		for (const QString &currentTranslationDir :
			 Mumble::Translations::getTranslationDirectories(a, extraTranslationDirs)) {
			infoString += QString::fromLatin1("%1. ").arg(counter) + currentTranslationDir + "\n";
			counter++;
		}

#if defined(Q_OS_WIN)
		QMessageBox::information(nullptr, QObject::tr("Invocation"), infoString);
#else
		printf("%s", qUtf8Printable(infoString));
#endif

		return 0;
	}

#ifdef USE_DBUS
#	ifdef Q_OS_WIN
	// By default, windbus expects the path to dbus-daemon to be in PATH, and the path
	// should contain bin\\, and the path to the config is hardcoded as ..\etc

	{
		size_t reqSize;
		if (_wgetenv_s(&reqSize, nullptr, 0, L"PATH") != 0) {
			qWarning() << "Failed to get PATH. Not adding application directory to PATH. DBus bindings may not work.";
		} else if (reqSize > 0) {
			STACKVAR(wchar_t, buff, reqSize + 1);
			if (_wgetenv_s(&reqSize, buff, reqSize, L"PATH") != 0) {
				qWarning()
					<< "Failed to get PATH. Not adding application directory to PATH. DBus bindings may not work.";
			} else {
				QString path =
					QString::fromLatin1("%1;%2")
						.arg(QDir::toNativeSeparators(MumbleApplication::instance()->applicationVersionRootPath()))
						.arg(QString::fromWCharArray(buff));
				STACKVAR(wchar_t, buffout, path.length() + 1);
				path.toWCharArray(buffout);
				if (_wputenv_s(L"PATH", buffout) != 0) {
					qWarning() << "Failed to set PATH. DBus bindings may not work.";
				}
			}
		}
	}
#	endif
#endif

	if (bRpcMode) {
		bool sent = false;
		QMap< QString, QVariant > param;
		param.insert(rpcCommand, rpcCommand);
		sent = SocketRPC::send(QLatin1String("Mumble"), QLatin1String("self"), param);
		if (sent) {
			return 0;
		} else {
			return 1;
		}
	}

	if (!bAllowMultiple) {
		if (url.isValid()) {
#ifndef USE_DBUS
			QMap< QString, QVariant > param;
			param.insert(QLatin1String("href"), url);
#endif
			bool sent = false;
#ifdef USE_DBUS
			QDBusInterface qdbi(QLatin1String("net.sourceforge.mumble.mumble"), QLatin1String("/"),
								QLatin1String("net.sourceforge.mumble.Mumble"));

			QDBusMessage reply = qdbi.call(QLatin1String("openUrl"), QLatin1String(url.toEncoded()));
			sent               = (reply.type() == QDBusMessage::ReplyMessage);
#else
			sent = SocketRPC::send(QLatin1String("Mumble"), QLatin1String("url"), param);
#endif
			if (sent)
				return 0;
		} else {
			bool sent = false;
#ifdef USE_DBUS
			QDBusInterface qdbi(QLatin1String("net.sourceforge.mumble.mumble"), QLatin1String("/"),
								QLatin1String("net.sourceforge.mumble.Mumble"));

			QDBusMessage reply = qdbi.call(QLatin1String("focus"));
			sent               = (reply.type() == QDBusMessage::ReplyMessage);
#else
			sent = SocketRPC::send(QLatin1String("Mumble"), QLatin1String("focus"));
#endif
			if (sent)
				return 0;
		}
	}

#ifdef Q_OS_WIN
	// The code above this block is somewhat racy, in that it might not
	// be possible to do RPC/DBus if two processes start at almost the
	// same time.
	//
	// In order to be completely sure we don't open multiple copies of
	// Mumble, we open a lock file. The file is opened without any sharing
	// modes enabled. This gives us exclusive access to the file.
	// If another Mumble instance attempts to open the file, it will fail,
	// and that instance will know to terminate itself.
	UserLockFile userLockFile(Global::get().qdBasePath.filePath(QLatin1String("mumble.lock")));
	if (!bAllowMultiple) {
		if (!userLockFile.acquire()) {
			qWarning("Another process has already acquired the lock file at '%s'. Terminating...",
					 qPrintable(userLockFile.path()));
			return 1;
		}
	}
#endif

	// Load preferences
	Global::get().s.load();

	// Check whether we need to enable accessibility features
#ifdef Q_OS_WIN
	// Only windows for now. Could not find any information on how to query this for osx or linux
	{
		HIGHCONTRAST hc;
		hc.cbSize = sizeof(HIGHCONTRAST);
		SystemParametersInfo(SPI_GETHIGHCONTRAST, sizeof(HIGHCONTRAST), &hc, 0);

		if (hc.dwFlags & HCF_HIGHCONTRASTON)
			Global::get().s.bHighContrast = true;
	}
#endif

	DeferInit::run_initializers();

	ApplicationPalette applicationPalette;

	Themes::apply();

	QLocale systemLocale = QLocale::system();

#ifdef Q_OS_MAC
	if (os_lang) {
		const QLocale macOSLocale = QLocale(QString::fromLatin1(os_lang));

		if (macOSLocale != QLocale::c()) {
			qWarning("Using Mac OS X system language as locale name");
			systemLocale = macOSLocale;
		}
	}
#endif

	QLocale settingsLocale;

	if (localeOverwrite.isEmpty()) {
		settingsLocale = QLocale(Global::get().s.qsLanguage);
		if (settingsLocale == QLocale::c()) {
			settingsLocale = systemLocale;
		}
	} else {
		// Manually specified locale overwrite
		settingsLocale = QLocale(localeOverwrite);

		if (settingsLocale == QLocale::c()) {
			qFatal("Invalid locale specification \"%s\"", qUtf8Printable(localeOverwrite));
			return 1;
		}

		// The locale is valid -> save it to the settings
		Global::get().s.qsLanguage = settingsLocale.nativeLanguageName();
	}

	if (!pluginsToBeInstalled.isEmpty()) {
		foreach (QString currentPlugin, pluginsToBeInstalled) {
			try {
				PluginInstaller installer(currentPlugin);
				installer.exec();
			} catch (const PluginInstallException &e) {
				qCritical() << qUtf8Printable(e.getMessage());
			}
		}

		return 0;
	}

	qWarning("Locale is \"%s\" (System: \"%s\")", qUtf8Printable(settingsLocale.name()),
			 qUtf8Printable(systemLocale.name()));

	Mumble::Translations::LifetimeGuard translationGuard =
		Mumble::Translations::installTranslators(settingsLocale, a, extraTranslationDirs);

	// Initialize proxy settings
	NetworkConfig::SetupProxy();

	Global::get().nam = new QNetworkAccessManager();

#ifndef NO_CRASH_REPORT
	CrashReporter *cr = new CrashReporter();
	cr->run();
	delete cr;
#endif

	// Initialize database
	Global::get().db = new Database(QLatin1String("main"));

#ifdef USE_ZEROCONF
	// Initialize zeroconf
	Global::get().zeroconf = new Zeroconf();
#endif

	// PluginManager
	Global::get().pluginManager = new PluginManager();
	Global::get().pluginManager->rescanPlugins();

#ifdef USE_OVERLAY
	Global::get().o = new Overlay();
	Global::get().o->setActive(Global::get().s.os.bEnable);
#endif

	Global::get().lcd = new LCD();

	// Process any waiting events before initializing our MainWindow.
	// The mumble:// URL support for Mac OS X happens through AppleEvents,
	// so we need to loop a little before we begin.
	a.processEvents();

	// Main Window
	Global::get().mw = new MainWindow(nullptr);
	Global::get().mw->show();

	Global::get().talkingUI = new TalkingUI();

	// Set TalkingUI's position
	Global::get().talkingUI->move(getTalkingUIPosition());

	// By setting the TalkingUI's position **before** making it visible tends to more reliably include the
	// window's frame to be included in the positioning calculation on X11 (at least using KDE Plasma)
	Global::get().talkingUI->setVisible(Global::get().s.bShowTalkingUI);

	QObject::connect(Global::get().mw, &MainWindow::userAddedChannelListener, Global::get().talkingUI,
					 &TalkingUI::on_channelListenerAdded);
	QObject::connect(Global::get().mw, &MainWindow::userRemovedChannelListener, Global::get().talkingUI,
					 &TalkingUI::on_channelListenerRemoved);
	QObject::connect(Global::get().channelListenerManager.get(), &ChannelListenerManager::localVolumeAdjustmentsChanged,
					 Global::get().talkingUI, &TalkingUI::on_channelListenerLocalVolumeAdjustmentChanged);

	QObject::connect(Global::get().mw, &MainWindow::serverSynchronized, Global::get().talkingUI,
					 &TalkingUI::on_serverSynchronized);

	// Initialize logger
	// Log::log() needs the MainWindow to already exist. Thus creating the Log instance
	// before the MainWindow one, does not make sense. if you need logging before this
	// point, use Log::logOrDefer()
	Global::get().l = new Log();
	Global::get().l->processDeferredLogs();

#ifdef Q_OS_WIN
	// Set mumble_mw_hwnd in os_win.cpp.
	// Used in ASIOInput and GlobalShortcut_win by APIs that require a HWND.
	mumble_mw_hwnd = reinterpret_cast< HWND >(Global::get().mw->winId());
#endif

#ifdef USE_DBUS
	new MumbleDBus(Global::get().mw);
	QDBusConnection::sessionBus().registerObject(QLatin1String("/"), Global::get().mw);
	QDBusConnection::sessionBus().registerService(QLatin1String("net.sourceforge.mumble.mumble"));
#endif

	SocketRPC *srpc = new SocketRPC(QLatin1String("Mumble"));

	Global::get().l->log(Log::Information, MainWindow::tr("Welcome to Mumble."));

	Audio::start();

	a.setQuitOnLastWindowClosed(false);

	// Configuration updates
	bool runaudiowizard = false;
	switch (Global::get().s.uiUpdateCounter) {
		case 0:
			// Previous version was an pre 1.2.3 release or this is the first run
			runaudiowizard = true;
			// Fallthrough
		case 1:
			// Previous versions used old idle action style, convert it
			if (Global::get().s.iIdleTime == 5 * 60) { // New default
				Global::get().s.iaeIdleAction = Settings::Nothing;
			} else {
				Global::get().s.iIdleTime     = 60 * qRound(Global::get().s.iIdleTime / 60.); // Round to minutes
				Global::get().s.iaeIdleAction = Settings::Deafen;                             // Old behavior
			}
			// Fallthrough
#ifdef Q_OS_WIN
		case 2: {
			auto &shortcuts = Global::get().s.qlShortcuts;
			const auto migratedShortcuts = GlobalShortcutWin::migrateSettings(shortcuts);
			if (shortcuts.size() > migratedShortcuts.size()) {
				const auto num = shortcuts.size() - migratedShortcuts.size();
				QMessageBox::warning(nullptr, QObject::tr("Shortcuts migration incomplete"),
									 QObject::tr("Unfortunately %1 shortcut(s) could not be migrated.\nYou can register them again.").arg(num));
			}

			shortcuts = migratedShortcuts;
		}
#endif
	}

	if (runaudiowizard) {
		AudioWizard *aw = new AudioWizard(Global::get().mw);
		aw->exec();
		delete aw;
	}

	Global::get().s.uiUpdateCounter = 3;

	if (!CertWizard::validateCert(Global::get().s.kpCertificate)) {
		QFile qf(qdCert.absoluteFilePath(QLatin1String("MumbleAutomaticCertificateBackup.p12")));
		if (qf.open(QIODevice::ReadOnly | QIODevice::Unbuffered)) {
			Settings::KeyPair kp = CertWizard::importCert(qf.readAll());
			qf.close();
			if (CertWizard::validateCert(kp))
				Global::get().s.kpCertificate = kp;
		}
		if (!CertWizard::validateCert(Global::get().s.kpCertificate)) {
			CertWizard *cw = new CertWizard(Global::get().mw);
			cw->exec();
			delete cw;

			if (!CertWizard::validateCert(Global::get().s.kpCertificate)) {
				Global::get().s.kpCertificate = CertWizard::generateNewCert();
				if (qf.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered)) {
					qf.write(CertWizard::exportCert(Global::get().s.kpCertificate));
					qf.close();
				}
			}
		}
	}

	if (QDateTime::currentDateTime().daysTo(Global::get().s.kpCertificate.first.first().expiryDate()) < 14)
		Global::get().l->log(
			Log::Warning,
			CertWizard::tr("<b>Certificate Expiry:</b> Your certificate is about to expire. You need to renew it, "
						   "or you will no longer be able to connect to servers you are registered on."));

#ifdef QT_NO_DEBUG
	// Only perform the version-check for non-debug builds
	if (Global::get().s.bUpdateCheck) {
		// Use different settings for the version checks depending on whether this is a snapshot build
		// or a normal release build
#	ifndef SNAPSHOT_BUILD
		// release build
		new VersionCheck(true, Global::get().mw);
#	else
		// snapshot build
		new VersionCheck(false, Global::get().mw, true);
#	endif
	}

	if (Global::get().s.bPluginCheck) {
		Global::get().pluginManager->checkForPluginUpdates();
	}
#else  // QT_NO_DEBUG
	Global::get().mw->msgBox(MainWindow::tr("Skipping version check in debug mode."));
#endif // QT_NO_DEBUG

	if (url.isValid()) {
		OpenURLEvent *oue = new OpenURLEvent(url);
		qApp->postEvent(Global::get().mw, oue);
#ifdef Q_OS_MAC
	} else if (!a.quLaunchURL.isEmpty()) {
		OpenURLEvent *oue = new OpenURLEvent(a.quLaunchURL);
		qApp->postEvent(Global::get().mw, oue);
#endif
	} else {
		Global::get().mw->on_qaServerConnect_triggered(true);
	}

	if (!Global::get().bQuit)
		res = a.exec();

	Global::get().s.save();

	url.clear();

	ServerHandlerPtr sh = Global::get().sh;
	if (sh) {
		if (sh->isRunning()) {
			url = sh->getServerURL();
			sh->disconnect();
		}

		// Wait for the ServerHandler thread to exit before proceeding shutting down. This is so that
		// all events that the ServerHandler might emit are enqueued into Qt's event loop before we
		// ask it to pocess all of them below.

		// We iteratively probe whether the ServerHandler thread has finished yet. If it did
		// not, we execute pending events in the main loop. This is because the ServerHandler
		// could be stuck waiting for a function to complete in the main loop (e.g. a plugin
		// uses the API in the disconnect callback).
		// We assume that this entire process is done in way under a second.
		int iterations = 0;
		while (!sh->wait(10)) {
			QCoreApplication::processEvents();
			iterations++;

			if (iterations > 200) {
				qFatal("ServerHandler does not exit as expected");
			}
		}
	}

	QCoreApplication::processEvents();

	// Only start deleting items once all pending events have been processed (Audio::stop deletes the audio
	// input and output)
	Audio::stop();

	delete srpc;

	delete Global::get().talkingUI;
	// Delete the MainWindow before the ServerHandler gets reset in order to allow all callbacks
	// trggered by this deletion to still access the ServerHandler (atm all these callbacks are in PluginManager.cpp)
	delete Global::get().mw;
	Global::get().mw = nullptr; // Make it clear to any destruction code, that MainWindow no longer exists

	Global::get().sh.reset();

	while (sh && !sh.unique())
		QThread::yieldCurrentThread();
	sh.reset();

	delete Global::get().nam;
	delete Global::get().lcd;

	delete Global::get().db;
	delete Global::get().l;
	Global::get().l = nullptr; // Make it clear to any destruction code that Log no longer exists

	delete Global::get().pluginManager;

#ifdef USE_ZEROCONF
	delete Global::get().zeroconf;
#endif

#ifdef USE_OVERLAY
	delete Global::get().o;
#endif

	delete Global::get().c;
	Global::get().le.clear();

	DeferInit::run_destroyers();

	delete Global::g_global_struct;
	Global::g_global_struct = nullptr;

#ifndef QT_NO_DEBUG
#	if (GOOGLE_PROTOBUF_VERSION >= 2001000)
	// Release global protobuf memory allocations.
	google::protobuf::ShutdownProtobufLibrary();
#	endif
#endif

#ifdef Q_OS_WIN
	// Release the userLockFile.
	//
	// It is important that we release it before we attempt to
	// restart Mumble (if requested). If we do not release it
	// before that, the new instance might not be able to start
	// correctly.
	userLockFile.release();
#endif

	// Tear down OpenSSL state.
	MumbleSSL::destroy();

	// At this point termination of our process is immenent. We can safely
	// launch another version of Mumble. The reason we do an actual
	// restart instead of re-creating our data structures is that making
	// sure we won't leave state is quite tricky. Mumble has quite a
	// few spots which might not consider seeing to basic initializations.
	// Until we invest the time to verify this, rather be safe (and a bit slower)
	// than sorry (and crash/bug out). Also take care to reconnect if possible.
	if (res == MUMBLE_EXIT_CODE_RESTART) {
		QStringList arguments;

		if (bAllowMultiple)
			arguments << QLatin1String("--multiple");
		if (suppressIdentity)
			arguments << QLatin1String("--noidentity");
		if (customJackClientName)
			arguments << QLatin1String("--jackname ") + Global::get().s.qsJackClientName;
		if (!url.isEmpty())
			arguments << url.toString();

		qWarning() << "Triggering restart of Mumble with arguments: " << arguments;

#ifdef Q_OS_WIN
		// Work around bug related to QTBUG-7645. Mumble has uiaccess=true set
		// on windows which makes normal CreateProcess calls (like Qt uses in
		// startDetached) fail unless they specifically enable additional priviledges.
		// Note that we do not actually require user interaction by UAC nor full admin
		// rights but only the right token on launch. Here we use ShellExecuteEx
		// which handles this transparently for us.
		const std::wstring applicationFilePath = qApp->applicationFilePath().toStdWString();
		const std::wstring argumentsString     = arguments.join(QLatin1String(" ")).toStdWString();

		SHELLEXECUTEINFO si;
		ZeroMemory(&si, sizeof(SHELLEXECUTEINFO));
		si.cbSize       = sizeof(SHELLEXECUTEINFO);
		si.lpFile       = applicationFilePath.data();
		si.lpParameters = argumentsString.data();

		bool ok = (ShellExecuteEx(&si) == TRUE);
#else
		bool ok = QProcess::startDetached(qApp->applicationFilePath(), arguments);
#endif
		if (!ok) {
			QMessageBox::warning(nullptr, QApplication::tr("Failed to restart mumble"),
								 QApplication::tr("Mumble failed to restart itself. Please restart it manually."));
			return 1;
		}
		return 0;
	}
	return res;
}

#if defined(Q_OS_WIN) && defined(MUMBLEAPP_DLL)
extern "C" __declspec(dllexport) int MumbleMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdArg, int cmdShow) {
	Q_UNUSED(instance)
	Q_UNUSED(prevInstance)
	Q_UNUSED(cmdArg)
	Q_UNUSED(cmdShow)

	int argc;
	wchar_t **argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (!argvW) {
		return -1;
	}

	QVector< QByteArray > argvS;
	argvS.reserve(argc);

	QVector< char * > argvV(argc, nullptr);
	for (int i = 0; i < argc; ++i) {
		argvS.append(QString::fromWCharArray(argvW[i]).toLocal8Bit());
		argvV[i] = argvS.back().data();
	}

	LocalFree(argvW);

	return main(argc, argvV.data());
}
#endif
