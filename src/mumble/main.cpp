// Copyright The Mumble Developers. All rights reserved.
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
#include "Logger.h"
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
#include "Version.h"
#include "VersionCheck.h"
#include "Global.h"

#include "widgets/TrayIcon.h"

#include <CLI/CLI.hpp>

#include <QLocale>
#include <QScreen>
#include <QtCore/QProcess>
#include <QtGui/QDesktopServices>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTextBrowser>

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <optional>

#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/qt_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

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

using namespace mumble;

extern void os_init();
extern char *os_lang;

using QtLogSink = spdlog::sinks::qt_color_sink_st;

void initLog(QTextBrowser *textBox = nullptr) {
	// TODO: Ideally we should add a user option, perhaps along with a launch parameter, to set the log level.
	// However, the messages across the codebase are very inconsistent in terms of right now.
	// For example, there are a lot of debug messages that should be info instead.
	// Thus, for the time being let's show all messages regardless of their advertised level.
	log::init(spdlog::level::trace);

	// Set up file log
	using FileSink = spdlog::sinks::rotating_file_sink_st;
	// 5MB
	static constexpr std::size_t maxSize  = 5 * 1024 * 2024;
	static constexpr std::size_t maxFiles = 3;

#ifndef Q_OS_MACOS
	const auto filePath = Global::get().qdBasePath.filePath(QLatin1String("Console.txt")).toStdString();
	log::addSink(std::make_shared< FileSink >(filePath, maxSize, maxFiles));
#else
	std::string filePath = "/Library/Logs/Mumble.log";
	if (const char *homePath = std::getenv("HOME")) {
		filePath.insert(0, homePath);
		log::addSink(std::make_shared< FileSink >(filePath, maxSize, maxFiles));
	}
#endif

	if (textBox) {
		static constexpr int maxLines = 1000;
		log::addSink(std::make_shared< QtLogSink >(textBox, maxLines));
	}
}

void prepareLogForShutdown() {
	log::debug("Preparing to exit...");

	std::shared_ptr< spdlog::logger > logger = spdlog::get(log::MainLoggerName);

	if (!logger) {
		return;
	}

	auto &sinks = logger->sinks();

	// Remove Qt sinks because the widgets they write to are going to be deleted soon
	auto remove_qt_sinks = [](auto &container) {
		container.erase(
			std::remove_if(container.begin(), container.end(),
						   [](const spdlog::sink_ptr &sink) { return std::dynamic_pointer_cast< QtLogSink >(sink); }),
			container.end());
	};

	remove_qt_sinks(sinks);
	for (spdlog::sink_ptr &sink : sinks) {
		auto st_dist_sink = std::dynamic_pointer_cast< spdlog::sinks::dist_sink_st >(sink);
		auto mt_dist_sink = std::dynamic_pointer_cast< spdlog::sinks::dist_sink_mt >(sink);

		if (st_dist_sink) {
			remove_qt_sinks(st_dist_sink->sinks());
		}
		if (mt_dist_sink) {
			remove_qt_sinks(mt_dist_sink->sinks());
		}
	}
}

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


struct CLIOptions {
	bool allowMultiple            = false;
	bool suppressIdentity         = false;
	bool rpcMode                  = false;
	bool startHiddenInTray        = false;
	bool printTranslationDirs     = false;
	bool quit                     = false;
	bool showVersion              = false;
	bool showLicense              = false;
	bool showAuthors              = false;
	bool showThirdPartyLicenses   = false;
	bool dumpInputStreams         = false;
	bool printEchoCancelQueue     = false;
	bool skipSettingsBackupPrompt = false;

	std::optional< std::string > configFile;
	std::optional< std::string > jackClientName;
	std::optional< std::string > windowTitleExt;
	std::optional< std::string > hyperlink;
	std::optional< std::string > translationDir;
	std::optional< std::string > locale;
	std::optional< std::string > defaultCertDir;

	std::optional< std::string > rpcCommand;

	static constexpr const char *CLI_GENERAL_SECTION  = "General";
	static constexpr const char *CLI_LANGUAGE_SECTION = "Language";
	static constexpr const char *CLI_ABOUT_SECTION    = "About";
	static constexpr const char *CLI_DEBUG_SECTION    = "Debug";
	static constexpr const char *CLI_REMOTE_SECTION   = "Remote Control";

	static const std::set< std::string > knownRpcCommands;
};

const std::set< std::string > CLIOptions::knownRpcCommands = {
	"mute", "unmute", "togglemute", "deaf", "undeaf", "toggledeaf", "starttalking", "stoptalking",
};

CLIOptions parseCLI(int argc, char **argv) {
	CLIOptions options;

	CLI::App app("Mumble Client");
	app.set_version_flag("--version", "Mumble version " + Version::getRelease().toStdString());
	app.add_flag("--license", options.showLicense, "Show the Mumble license.")->group(CLIOptions::CLI_ABOUT_SECTION);
	app.add_flag("--authors", options.showAuthors, "Show the Mumble authors.")->group(CLIOptions::CLI_ABOUT_SECTION);
	app.add_flag("--third-party-licenses", options.showThirdPartyLicenses,
				 "Show licenses for third-party software used by Mumble.")
		->group(CLIOptions::CLI_ABOUT_SECTION);

	app.add_option("-c,--config", options.configFile,
				   "Specify an alternative configuration file. "
				   "If you use this to run multiple instances of Mumble at once, "
				   "make sure to use an alternative 'database' value in the config.")
		->option_text("<config>")
		->check(CLI::ExistingFile)
		->group(CLIOptions::CLI_GENERAL_SECTION);

	app.add_option("--default-certificate-dir", options.defaultCertDir,
				   "Specify an alternative default certificate path. "
				   "This path is only used if there is no certificate loaded "
				   "from the settings.")
		->option_text("<dir>")
		->check(CLI::ExistingDirectory)
		->group(CLIOptions::CLI_GENERAL_SECTION);

	app.add_flag("-m,--multiple", options.allowMultiple, "Allow multiple instances of the client to be started.")
		->group(CLIOptions::CLI_GENERAL_SECTION);

	app.add_flag("--no-identity", options.suppressIdentity, "Suppress loading of identity files (i.e., certificates).")
		->group(CLIOptions::CLI_GENERAL_SECTION);

	app.add_option("--jack-name", options.jackClientName, "Set custom Jack client name.")
		->option_text("<name>")
		->group(CLIOptions::CLI_GENERAL_SECTION);

	app.add_option("--window-title-ext", options.windowTitleExt, "Set a custom window title extension.")
		->option_text("<extension>")
		->group(CLIOptions::CLI_GENERAL_SECTION);

	app.add_flag("--skip-settings-backup-prompt", options.skipSettingsBackupPrompt,
				 "Don't show the settings recovery dialog on startup after a crash.")
		->group(CLIOptions::CLI_GENERAL_SECTION);



	app.add_flag("--hidden", options.startHiddenInTray, "Start Mumble hidden in the system tray.")
		->group(CLIOptions::CLI_GENERAL_SECTION);

	app.add_option("--locale", options.locale,
				   "Overwrite the locale in Mumble's settings with a "
				   "locale that corresponds to the given locale string. "
				   "If the format is invalid, Mumble will error. "
				   "Otherwise the locale will be permanently saved to "
				   "Mumble's settings.")
		->option_text("<locale>")
		->group(CLIOptions::CLI_LANGUAGE_SECTION);
	app.add_option("--translation-dir", options.translationDir,
				   "Specifies an additional translation directory <dir> "
				   "in which Mumble will search for translation files that "
				   "overwrite the bundled ones. "
				   "Directories added this way have higher priority than "
				   "the default locations used otherwise.")
		->option_text("<dir>")
		->check(CLI::ExistingDirectory)
		->group(CLIOptions::CLI_LANGUAGE_SECTION);
	app.add_flag("--print-translation-dirs", options.printTranslationDirs,
				 "Print out the paths in which Mumble will search for "
				 "translation files that overwrite the bundled ones. "
				 "(Useful for translators testing their translations)")
		->group(CLIOptions::CLI_LANGUAGE_SECTION);

	app.add_flag("--dump-input-streams", options.dumpInputStreams,
				 "Dump PCM streams at various parts of the input chain "
				 " - raw microphone input "
				 " - speaker readback for echo cancelling "
				 " - processed microphone input")
		->group(CLIOptions::CLI_DEBUG_SECTION);
	app.add_flag("--print-echocancel-queue", options.printEchoCancelQueue,
				 "Print on stdout the echo cancellation queue state")
		->group(CLIOptions::CLI_DEBUG_SECTION);

	app.add_option(
		   "hyperlink", options.hyperlink,
		   "<url> specifies a URL to connect to after startup instead of showing "
		   "the connection window, and has the following form:\n"
		   "mumble://[<username>[:<password>]@]<host>[:<port>][/<channel>[/"
		   "<subchannel>...]][?version=<x.y.z>]\n"
		   "\n"
		   "<plugin_list> is a list of plugin files that shall be installed"
		   "\n"
		   "The version query parameter has to be set in order to invoke the\n"
		   "correct client version. It currently defaults to 1.2.0.\n"
		   "\n"
		   "This allows setup of program associations with mumble to directly connect to servers or run with plugins.")
		->option_text("<url> | <plugin_list>")
		->group(CLIOptions::CLI_GENERAL_SECTION);

	CLI::App *rpc = app.add_subcommand("rpc", "Remote control a running instance of Mumble");
	rpc->add_option("action", options.rpcCommand, "Action to perform")
		->required()
		->check(CLI::IsMember(CLIOptions::knownRpcCommands, CLI::ignore_case));

	try {
		app.parse(argc, argv);
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

	a.setDesktopFileName("info.mumble.Mumble");

#ifdef Q_OS_WIN
	a.installNativeEventFilter(&a);
#endif

	MumbleSSL::initialize();
	CLIOptions options = parseCLI(argc, argv);

	if (options.quit) {
		return 0;
	}

	// This argument has to be parsed first, since it's value is needed to create the global struct,
	// which other switches are modifying. If it is parsed first, the order of the arguments does not matter.
	QString settingsFile;

	if (options.configFile) {
		QFile inifile(QString::fromStdString(*options.configFile));
		if (inifile.exists() && inifile.permissions().testFlag(QFile::WriteUser)) {
			Global::g_global_struct = new Global(inifile.fileName());
			settingsFile            = inifile.fileName();
		} else {
			printf("%s", qPrintable(MainWindow::tr("Configuration file %1 does not exist or is not writable.\n")
										.arg(inifile.fileName())));
			return 1;
		}
	} else {
		Global::g_global_struct = new Global();
	}

	auto logBox = new QTextBrowser();
	initLog(logBox);
	Global::get().c = new DeveloperConsole(logBox);

	os_init();

	QUrl url;
	QDir qdCert(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
	QStringList extraTranslationDirs;
	QString localeOverwrite;

	QStringList pluginsToBeInstalled;

	if (options.suppressIdentity) {
		Global::get().s.bSuppressIdentity = true;
	} else if (options.jackClientName) {
		Global::get().s.qsJackClientName = QString::fromStdString(*options.jackClientName);
	} else if (options.windowTitleExt) {
		Global::get().windowTitlePostfix = QString::fromStdString(*options.windowTitleExt);
	} else if (options.showLicense) {
		printf("%s\n", qPrintable(License::license()));
		return 0;
	} else if (options.showAuthors) {
		printf("%s\n", "For a list of authors, please see https://github.com/mumble-voip/mumble/graphs/contributors");
		return 0;
	} else if (options.showThirdPartyLicenses) {
		printf("%s", qPrintable(License::printableThirdPartyLicenseInfo()));
		return 0;
	} else if (options.rpcCommand) {
		options.rpcMode = true;
	} else if (options.dumpInputStreams) {
		Global::get().bDebugDumpInput = true;
	} else if (options.printEchoCancelQueue) {
		Global::get().bDebugPrintQueue = true;
	} else if (options.defaultCertDir) {
		qdCert = QDir(QString::fromStdString(*options.defaultCertDir));
		// I suppose we should really be checking whether the directory is writable here too,
		// but there are some subtleties with doing that:
		// (doc.qt.io/qt-5/qfile.html#platform-specific-issues)
		// so we can just let things fail down below when this directory is used.
		if (!qdCert.exists()) { // probably not reached since cli11 checks for existence
			printf("%s", qPrintable(MainWindow::tr("Directory %1 does not exist.\n").arg(qdCert.absolutePath())));
			return 1;
		}

	} else if (options.translationDir) {
		extraTranslationDirs.append(QString::fromStdString(*options.translationDir));
	} else if (options.locale) {
		localeOverwrite = QString::fromStdString(*options.locale);
	} else if (options.startHiddenInTray) {
#ifndef Q_OS_MAC
		qInfo("Starting hidden in system tray");
#else
		// When Qt addresses hide() on macOS to use native hiding, this can be fixed.
		qWarning("Can not start Mumble hidden in system tray on macOS");
#endif
	} else if (options.hyperlink) {
		if (PluginInstaller::canBePluginFile(QFileInfo(QString::fromStdString(*options.hyperlink)))) {
			pluginsToBeInstalled << QString::fromStdString(*options.hyperlink);
		} else {
			if (!options.rpcMode) {
				QString qHyperlink = QString::fromStdString(*options.hyperlink);
				QUrl u             = QUrl::fromEncoded(qHyperlink.toUtf8());
				if (u.isValid() && (u.scheme() == QLatin1String("mumble"))) {
					url = u;
				} else {
					QFile f(qHyperlink);
					if (f.exists()) {
						url = QUrl::fromLocalFile(f.fileName());
					}
				}
			}
		}
	}

	if (options.printTranslationDirs) {
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
			std::vector< wchar_t > buff;
			buff.resize(reqSize + 1);
			if (_wgetenv_s(&reqSize, buff, reqSize, L"PATH") != 0) {
				qWarning()
					<< "Failed to get PATH. Not adding application directory to PATH. DBus bindings may not work.";
			} else {
				QString path =
					QString::fromLatin1("%1;%2")
						.arg(QDir::toNativeSeparators(MumbleApplication::instance()->applicationVersionRootPath()))
						.arg(QString::fromWCharArray(buff));
				static std::vector< wchar_t > outBuffer;
				outBuffer.resize(path.length() + 1);
				wchar_t *buffout = outBuffer.data();
				path.toWCharArray(buffout);
				if (_wputenv_s(L"PATH", buffout) != 0) {
					qWarning() << "Failed to set PATH. DBus bindings may not work.";
				}
			}
		}
	}
#	endif
#endif

	if (options.rpcMode) {
		bool sent = false;
		QMap< QString, QVariant > param;
		QString rpcCommand = QString::fromStdString(*options.rpcCommand);
		param.insert(rpcCommand, rpcCommand);
		sent = SocketRPC::send(QLatin1String("Mumble"), QLatin1String("self"), param);
		if (sent) {
			return 0;
		} else {
			return 1;
		}
	}

	if (!options.allowMultiple) {
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
	if (!options.allowMultiple) {
		if (!userLockFile.acquire()) {
			qWarning("Another process has already acquired the lock file at '%s'. Terminating...",
					 qPrintable(userLockFile.path()));
			return 1;
		}
	}
#endif

	// Load preferences
	if (settingsFile.isEmpty()) {
		Global::get().s.load();
	} else {
		Global::get().s.load(settingsFile);
	}
	if (!Global::get().migratedDBPath.isEmpty()) {
		// We have migrated the DB to a new location. Make sure that the settings hold the correct (new) path and that
		// this path is written to disk immediately in order to minimize the risk of losing this information due to a
		// crash.
		Global::get().s.qsDatabaseLocation = Global::get().migratedDBPath;

		// Also update all plugin settings that might be affected by the migration
		Global::get().s.migratePluginSettings(Global::get().migratedPluginDirPath);

		Global::get().s.save();
	}

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
	Global::get().db->clearLocalMuted();

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
	if (!options.startHiddenInTray) {
		Global::get().mw->showRaiseWindow();
	}

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

	Global::get().trayIcon = new TrayIcon();

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

	if (!Global::get().s.audioWizardShown) {
		auto wizard = std::make_unique< AudioWizard >(Global::get().mw);
		wizard->exec();

		Global::get().s.audioWizardShown = true;
	}

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
	} else if (!options.startHiddenInTray || Global::get().s.bAutoConnect) {
		Global::get().mw->on_qaServerConnect_triggered(true);
	}

	if (!Global::get().bQuit)
		res = a.exec();

	// Indicate that this was a regular shutdown
	Global::get().s.mumbleQuitNormally = true;
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
		// ask it to process all of them below.

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

	prepareLogForShutdown();

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

		if (options.allowMultiple)
			arguments << QLatin1String("--multiple");
		if (options.suppressIdentity)
			arguments << QLatin1String("--noidentity");
		if (options.jackClientName)
			arguments << QLatin1String("--jackname ") + Global::get().s.qsJackClientName;
		if (!url.isEmpty())
			arguments << url.toString();

		qWarning() << "Triggering restart of Mumble with arguments: " << arguments;

#ifdef Q_OS_WIN
		// Work around bug related to QTBUG-7645. Mumble has uiaccess=true set
		// on windows which makes normal CreateProcess calls (like Qt uses in
		// startDetached) fail unless they specifically enable additional privileges.
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
