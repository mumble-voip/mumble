// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Settings.h"
#include "AudioInput.h"
#include "Cert.h"
#include "EnumStringConversions.h"
#include "EnvUtils.h"
#include "JSONSerialization.h"
#include "Log.h"
#include "QtUtils.h"
#include "SSL.h"
#include "SettingsKeys.h"
#include "SettingsMacros.h"
#include "Global.h"

#if defined(Q_OS_WIN)
#	include "GlobalShortcut_win.h"
#endif

#include "../../overlay/overlay.h"

#include <QByteArray>
#include <QDataStream>
#include <QFileInfo>
#include <QImageReader>
#include <QMessageBox>
#include <QOperatingSystemVersion>
#include <QProcessEnvironment>
#include <QRegularExpression>
#include <QSettings>
#include <QStandardPaths>
#include <QSystemTrayIcon>

#include <boost/typeof/typeof.hpp>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <limits>
#include <memory>

#include <nlohmann/json.hpp>

constexpr const char *BACKUP_FILE_EXTENSION = ".back";
#ifdef Q_OS_WINDOWS
constexpr const char *REGISTRY_ID = ":::::REGISTRY:::::";
#endif

const QPoint Settings::UNSPECIFIED_POSITION =
	QPoint(std::numeric_limits< int >::min(), std::numeric_limits< int >::max());

bool Shortcut::isServerSpecific() const {
	if (qvData.canConvert< ShortcutTarget >()) {
		const ShortcutTarget &sc = qvariant_cast< ShortcutTarget >(qvData);
		return sc.isServerSpecific();
	}
	if (qvData.canConvert< ChannelTarget >()) {
		return true;
	}
	return false;
}

bool Shortcut::operator<(const Shortcut &other) const {
	return (iIndex < other.iIndex);
}

bool Shortcut::operator==(const Shortcut &other) const {
	return (iIndex == other.iIndex) && (qlButtons == other.qlButtons) && (qvData == other.qvData)
		   && (bSuppress == other.bSuppress);
}

bool ShortcutTarget::isServerSpecific() const {
	return !bCurrentSelection && !bUsers && iChannel >= 0;
}

bool ShortcutTarget::operator==(const ShortcutTarget &o) const {
	if ((bUsers != o.bUsers) || (bForceCenter != o.bForceCenter) || (bCurrentSelection != o.bCurrentSelection))
		return false;
	if (bUsers)
		return (qlUsers == o.qlUsers) && (qlSessions == o.qlSessions);
	else
		return (iChannel == o.iChannel) && (bLinks == o.bLinks) && (bChildren == o.bChildren) && (qsGroup == o.qsGroup);
}

std::size_t qHash(const ShortcutTarget &t) {
	std::size_t h = t.bForceCenter ? 0x55555555 : 0xaaaaaaaa;

	if (t.bCurrentSelection) {
		h ^= 0x20000000;
	}

	if (t.bUsers) {
		foreach (unsigned int u, t.qlSessions)
			h ^= u;
	} else {
		h ^= static_cast< unsigned int >(t.iChannel);
		if (t.bLinks)
			h ^= 0x80000000;
		if (t.bChildren)
			h ^= 0x40000000;
		h ^= qHash(t.qsGroup);
		h = ~h;
	}
	return h;
}

std::size_t qHash(const QList< ShortcutTarget > &l) {
	auto h = static_cast< std::size_t >(l.count());
	foreach (const ShortcutTarget &st, l)
		h ^= qHash(st);
	return h;
}

bool operator==(const PluginSetting &lhs, const PluginSetting &rhs) {
	return lhs.enabled == rhs.enabled && lhs.positionalDataEnabled == rhs.positionalDataEnabled
		   && lhs.allowKeyboardMonitoring == rhs.allowKeyboardMonitoring && lhs.path == rhs.path;
}

bool operator!=(const PluginSetting &lhs, const PluginSetting &rhs) {
	return !(lhs == rhs);
}

bool operator==(const OverlaySettings &lhs, const OverlaySettings &rhs) {
#define PROCESS(category, key, variable) \
	if (lhs.variable != rhs.variable) {  \
		return false;                    \
	}
	PROCESS_ALL_OVERLAY_SETTINGS
#undef PROCESS

	return true;
}

bool operator!=(const OverlaySettings &lhs, const OverlaySettings &rhs) {
	return !(lhs == rhs);
}


void Settings::save(const QString &path) const {
	// Our saving procedure is a 4-step process:
	// 1. Write the settings that are to be saved to a temporary file
	// 2. Check if a settings backup exists and if it does, delete it
	// 3. If an older settings file already exists, rename it to now be the backup
	// 4. Move the temporary file to the correct location to make sure it now serves as the proper settings file
	// All of this is an attempt at making the system as resistant to interruptions as possible.
	if (!path.endsWith(".json")) {
		throw std::runtime_error("Expected settings file to have \".json\" extension");
	}

	nlohmann::json settingsJSON = *this;

	QFile tmpFile(QString::fromLatin1("%1/mumble_settings.json.tmp")
					  .arg(QStandardPaths::writableLocation(QStandardPaths::TempLocation)));

	std::ofstream stream(Mumble::QtUtils::qstring_to_path(tmpFile.fileName()));
	stream << settingsJSON.dump(4) << std::endl;
	stream.close();

	if (stream.fail()) {
		qWarning("Failed at writing temporary settings file: %s", qUtf8Printable(tmpFile.fileName()));
	}

	QFile targetFile(path);
	QFile backupFile(path + BACKUP_FILE_EXTENSION);

	if (targetFile.exists()) {
		if (!createdSettingsBackup) {
			// Create a backup of the settings
			if (backupFile.exists()) {
				// We first have to delete our backup
				if (!backupFile.remove()) {
					qWarning("Failed at deleting settings backup file: %s", qUtf8Printable(backupFile.errorString()));
				}
			}

			// Turn the current settings file into the backup file
			if (!targetFile.rename(backupFile.fileName())) {
				qWarning("Failed at renaming settings file to backup file: %s",
						 qUtf8Printable(targetFile.errorString()));
			}

			createdSettingsBackup = true;
		} else {
			// The current instance of Mumble has already created a settings backup while it was running. Thus
			// performing another backup now would actually delete the backup we created before. Thus we only delete the
			// current settings file in order to overwrite it below.
			if (!targetFile.remove()) {
				qWarning("Failed at deleting settings file: %s", qUtf8Printable(targetFile.errorString()));
			}
		}
	} else {
		QFileInfo info(targetFile);
		if (!info.dir().exists()) {
			if (!info.dir().mkpath(".")) {
				qWarning("Failed to create directory for settings at %s", qUtf8Printable(info.dir().absolutePath()));
			}
		}
	}

	// Move the temp-file into place
	if (!tmpFile.rename(path)) {
		qWarning("Failed at moving settings from %s to %s - reason: %s", qUtf8Printable(tmpFile.fileName()),
				 qUtf8Printable(path), qUtf8Printable(tmpFile.errorString()));
	}
}

void Settings::save() const {
	if (settingsLocation.isEmpty()) {
		save(findSettingsLocation());
	} else {
		save(settingsLocation);
	}
}

void Settings::load(const QString &path) {
	if (path.endsWith(QLatin1String(BACKUP_FILE_EXTENSION))) {
		// Trim away the backup extension
		settingsLocation = path.left(path.size() - static_cast< int >(std::strlen(BACKUP_FILE_EXTENSION)));
	} else {
		settingsLocation = path;
	}

	std::ifstream stream(Mumble::QtUtils::qstring_to_path(path));

	nlohmann::json settingsJSON;
	try {
		stream >> settingsJSON;

		settingsJSON.get_to(*this);

		if (!mumbleQuitNormally) {
			// These settings were saved without Mumble quitting normally afterwards. In order to prevent loading
			// settings that are causing crashes, we check if we can load the backup instead.
			if (!path.endsWith(QLatin1String(BACKUP_FILE_EXTENSION))) {
				QString backupPath = path + BACKUP_FILE_EXTENSION;

				QFileInfo backupInfo(backupPath);
				if (backupInfo.exists() && backupInfo.isFile()) {
					QMessageBox msgBox;
					msgBox.setWindowTitle(QObject::tr("Potentially broken settings"));
					msgBox.setText(QObject::tr("Load backup settings?"));
					msgBox.setInformativeText(QObject::tr(
						"It seems that Mumble did not perform a normal shutdown. If you did not intentionally kill the "
						"application, this could mean that the used settings caused a crash. "
						"Do you want to load the setting's backup instead?"));
					msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
					msgBox.setDefaultButton(QMessageBox::No);
					msgBox.setIcon(QMessageBox::Question);

					if (msgBox.exec() == QMessageBox::Yes) {
						// Load the backup instead
						qWarning() << "Loading backup settings from" << backupPath;
						load(backupPath);
					}
				}
			} else {
				// This is already the backup we are loading
				QMessageBox msgBox;
				msgBox.setWindowTitle(QObject::tr("Potentially broken settings"));
				msgBox.setText(QObject::tr("The backed-up settings also seem to have been saved without Mumble exiting "
										   "normally (potentially indicating a crash)."));
				msgBox.setInformativeText(
					QObject::tr(
						"If you experience repeated crashes with these settings, you might have to manually delete the "
						"settings files at <pre>%1</pre> and <pre>%2</pre> in order to reset all settings to their "
						"default value.")
						.arg(path.left(path.size() - static_cast< int >(std::strlen(BACKUP_FILE_EXTENSION))))
						.arg(path));
				msgBox.setIcon(QMessageBox::Warning);
				msgBox.exec();
			}
		}
	} catch (const nlohmann::json::parse_error &e) {
		qWarning() << "Failed to load settings from" << path << "due to invalid format: " << e.what();

		if (!path.endsWith(QLatin1String(BACKUP_FILE_EXTENSION)) && QFileInfo(path + BACKUP_FILE_EXTENSION).exists()) {
			qWarning() << "Falling back to backup" << path + BACKUP_FILE_EXTENSION;
			load(path + BACKUP_FILE_EXTENSION);
		}
	}

	// Always reset this flag to false
	mumbleQuitNormally = false;
}

void Settings::load() {
	bool foundExisting = false;

	QString settingsPath = findSettingsLocation(false, &foundExisting);

	if (foundExisting) {
		// If we found a regular settings file, then use that and be done with it
		qInfo() << "Loading settings from" << settingsPath;
		load(settingsPath);
	} else if (QFileInfo(settingsPath + BACKUP_FILE_EXTENSION).exists()) {
		// Load backup settings instead
		qInfo() << "Loading backup settings from" << settingsPath + BACKUP_FILE_EXTENSION;
		load(settingsPath + BACKUP_FILE_EXTENSION);
	} else {
		// Otherwise check for a legacy settings file and if that is found, load settings from there
		QString legacySettingsPath = findSettingsLocation(true, &foundExisting);

		if (foundExisting) {
			qInfo() << "Loading settings from legacy settings file" << legacySettingsPath;
			legacyLoad(legacySettingsPath);
		}
#ifdef Q_OS_WIN
		else {
			// On Windows, we previously used the registry, so if we did not find an old config file (which would
			// probably be very archaic), we can check the registry
			qInfo() << "Loading legacy settings from the registry";
			legacyLoad(QLatin1String(REGISTRY_ID));
		}
#endif
	}
}


QDataStream &operator<<(QDataStream &qds, const ShortcutTarget &st) {
	// Start by the version of this setting. This is needed to make sure we can stay compatible
	// with older versions (aka don't break existing shortcuts when updating the implementation)
	qds << QString::fromLatin1("v2");

	qds << st.bCurrentSelection << st.bUsers << st.bForceCenter;

	if (st.bCurrentSelection) {
		return qds << st.bLinks << st.bChildren;
	} else if (st.bUsers) {
		return qds << st.qlUsers;
	} else {
		return qds << st.iChannel << st.qsGroup << st.bLinks << st.bChildren;
	}
}

QDataStream &operator>>(QDataStream &qds, ShortcutTarget &st) {
	// Check for presence of a leading version string
	QString versionString;
	QIODevice *device = qds.device();

	if (device) {
		// Qt's way of serializing the stream requires us to read a few characters into
		// the stream in order to get across some leading zeros and other meta stuff.
		char buf[16];

		// Init buf
		for (unsigned int i = 0; i < sizeof(buf); i++) {
			buf[i] = 0;
		}

		qint64 read = device->peek(buf, sizeof(buf));

		for (unsigned int i = 0; i < read; i++) {
			if (buf[i] >= 31) {
				if (buf[i] == 'v') {
					qds >> versionString;
				} else {
					break;
				}
			}
		}
	} else {
		qCritical("Settings: Unable to determine version of setting for ShortcutTarget");
	}

	if (versionString == QLatin1String("v2")) {
		qds >> st.bCurrentSelection;
	}

	qds >> st.bUsers >> st.bForceCenter;

	if (st.bCurrentSelection) {
		return qds >> st.bLinks >> st.bChildren;
	} else if (st.bUsers) {
		return qds >> st.qlUsers;
	} else {
		return qds >> st.iChannel >> st.qsGroup >> st.bLinks >> st.bChildren;
	}
}

QDataStream &operator<<(QDataStream &qds, const ChannelTarget &target) {
	return qds << target.channelID;
}

QDataStream &operator>>(QDataStream &qds, ChannelTarget &target) {
	return qds >> target.channelID;
}

bool operator==(const ChannelTarget &lhs, const ChannelTarget &rhs) {
	return lhs.channelID == rhs.channelID;
}

bool operator<(const ChannelTarget &lhs, const ChannelTarget &rhs) {
	return lhs.channelID < rhs.channelID;
}

std::size_t qHash(const ChannelTarget &target) {
	return qHash(target.channelID);
}

const QString Settings::cqsDefaultPushClickOn  = QLatin1String(":/on.ogg");
const QString Settings::cqsDefaultPushClickOff = QLatin1String(":/off.ogg");

const QString Settings::cqsDefaultMuteCue = QLatin1String(":/off.ogg");

OverlaySettings::OverlaySettings() {
#ifdef Q_OS_MACOS
	qsStyle = QLatin1String("Cleanlooks");
#endif

	qcUserName[Settings::Passive]      = QColor(170, 170, 170);
	qcUserName[Settings::MutedTalking] = QColor(170, 170, 170);
	qcUserName[Settings::Talking]      = QColor(255, 255, 255);
	qcUserName[Settings::Whispering]   = QColor(128, 255, 128);
	qcUserName[Settings::Shouting]     = QColor(255, 128, 255);

	setPreset();
	qfFps = qfUserName;
}

void OverlaySettings::setPreset(const OverlayPresets preset) {
	switch (preset) {
		case LargeSquareAvatar:
			uiColumns      = 2;
			fUserName      = 0.75f;
			fChannel       = 0.75f;
			fMutedDeafened = 0.5f;
			fAvatar        = 1.0f;

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
			qfUserName = QFont(QLatin1String("Verdana"), 20);
#else
			qfUserName = QFont(QLatin1String("Arial"), 20);
#endif
			qfChannel = qfUserName;

			fUser[Settings::Passive]      = 0.5f;
			fUser[Settings::MutedTalking] = 0.5f;
			fUser[Settings::Talking]      = (7.0f / 8.0f);
			fUser[Settings::Whispering]   = (7.0f / 8.0f);
			fUser[Settings::Shouting]     = (7.0f / 8.0f);

			qrfUserName      = QRectF(-0.0625f, 0.101563f - 0.0625f, 0.125f, 0.023438f);
			qrfChannel       = QRectF(-0.03125f, -0.0625f, 0.09375f, 0.015625f);
			qrfMutedDeafened = QRectF(-0.0625f, -0.0625f, 0.0625f, 0.0625f);
			qrfAvatar        = QRectF(-0.0625f, -0.0625f, 0.125f, 0.125f);

			fBoxPenWidth = (1.f / 256.0f);
			fBoxPad      = (1.f / 256.0f);

			bUserName      = true;
			bChannel       = true;
			bMutedDeafened = true;
			bAvatar        = true;
			bBox           = false;

			qaUserName      = Qt::AlignCenter;
			qaMutedDeafened = Qt::AlignLeft | Qt::AlignTop;
			qaAvatar        = Qt::AlignCenter;
			qaChannel       = Qt::AlignCenter;
			break;
		case AvatarAndName:
		default:
			uiColumns      = 1;
			fUserName      = 1.0f;
			fChannel       = (7.0f / 8.0f);
			fMutedDeafened = (7.0f / 8.0f);
			fAvatar        = 1.0f;

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
			qfUserName = QFont(QLatin1String("Verdana"), 20);
#else
			qfUserName = QFont(QLatin1String("Arial"), 20);
#endif
			qfChannel = qfUserName;

			fUser[Settings::Passive]      = 0.5f;
			fUser[Settings::MutedTalking] = 0.5f;
			fUser[Settings::Talking]      = (7.0f / 8.0f);
			fUser[Settings::Whispering]   = (7.0f / 8.0f);
			fUser[Settings::Shouting]     = (7.0f / 8.0f);

			qrfUserName      = QRectF(0.015625f, -0.015625f, 0.250f, 0.03125f);
			qrfChannel       = QRectF(0.03125f, -0.015625f, 0.1875f, 0.015625f);
			qrfMutedDeafened = QRectF(0.234375f, -0.015625f, 0.03125f, 0.03125f);
			qrfAvatar        = QRectF(-0.03125f, -0.015625f, 0.03125f, 0.03125f);

			fBoxPenWidth = 0.0f;
			fBoxPad      = (1.f / 256.0f);

			bUserName      = true;
			bChannel       = false;
			bMutedDeafened = true;
			bAvatar        = true;
			bBox           = true;

			qaUserName      = Qt::AlignLeft | Qt::AlignVCenter;
			qaMutedDeafened = Qt::AlignRight | Qt::AlignVCenter;
			qaAvatar        = Qt::AlignCenter;
			qaChannel       = Qt::AlignLeft | Qt::AlignTop;
			break;
	}
}

Settings::Settings() {
#ifndef NDEBUG
	verifySettingsKeys();
#endif

#if defined(Q_OS_WIN)
	GlobalShortcutWin::registerMetaTypes();
#endif
	qRegisterMetaType< ShortcutTarget >("ShortcutTarget");
	qRegisterMetaType< ChannelTarget >("ChannelTarget");
	qRegisterMetaType< QVariant >("QVariant");
	qRegisterMetaType< PluginSetting >("PluginSetting");
	qRegisterMetaType< Search::SearchDialog::UserAction >("SearchDialog::UserAction");
	qRegisterMetaType< Search::SearchDialog::ChannelAction >("SearchDialog::ChannelAction");
#ifdef Q_OS_MACOS
	// The echo cancellation feature on macOS is experimental and known to be able to cause problems
	// (e.g. muting the user instead of only cancelling echo - https://github.com/mumble-voip/mumble/issues/4912)
	// Therefore we disable it by default until the issues are fixed.
	echoOption = EchoCancelOptionID::DISABLED;
#endif
#ifdef Q_OS_WIN
	// Don't enable minimize to tray by default on Windows >= 7
	bHideInTray = QOperatingSystemVersion::current() < QOperatingSystemVersion::Windows7;
#else
	const bool isUnityDesktop =
		QProcessEnvironment::systemEnvironment().value(QLatin1String("XDG_CURRENT_DESKTOP")) == QLatin1String("Unity");
	bHideInTray = !isUnityDesktop && QSystemTrayIcon::isSystemTrayAvailable();
#endif
#ifdef NO_UPDATE_CHECK
	bUpdateCheck = false;
	bPluginCheck = false;
#endif
#if defined(AUDIO_TEST)
	lmLoopMode = Server;
#endif


#ifdef Q_OS_LINUX
	if (EnvUtils::waylandIsUsed()) {
		// Due to the issues we're currently having on Wayland, we disable shortcuts by default
		bShortcutEnable = false;
	}
#endif

	for (int i = Log::firstMsgType; i <= Log::lastMsgType; ++i) {
		qmMessages.insert(i,
						  Settings::LogConsole | Settings::LogBalloon | Settings::LogTTS | Settings::LogMessageLimit);
		qmMessageSounds.insert(i, QString());
	}

	qmMessageSounds[Log::CriticalError]          = QLatin1String(":/Critical.ogg");
	qmMessageSounds[Log::PermissionDenied]       = QLatin1String(":/PermissionDenied.ogg");
	qmMessageSounds[Log::SelfMute]               = QLatin1String(":/SelfMutedDeafened.ogg");
	qmMessageSounds[Log::SelfUnmute]             = QLatin1String(":/SelfMutedDeafened.ogg");
	qmMessageSounds[Log::SelfDeaf]               = QLatin1String(":/SelfMutedDeafened.ogg");
	qmMessageSounds[Log::SelfUndeaf]             = QLatin1String(":/SelfMutedDeafened.ogg");
	qmMessageSounds[Log::ServerConnected]        = QLatin1String(":/ServerConnected.ogg");
	qmMessageSounds[Log::ServerDisconnected]     = QLatin1String(":/ServerDisconnected.ogg");
	qmMessageSounds[Log::TextMessage]            = QLatin1String(":/TextMessage.ogg");
	qmMessageSounds[Log::PrivateTextMessage]     = qmMessageSounds[Log::TextMessage];
	qmMessageSounds[Log::ChannelJoin]            = QLatin1String(":/UserJoinedChannel.ogg");
	qmMessageSounds[Log::ChannelLeave]           = QLatin1String(":/UserLeftChannel.ogg");
	qmMessageSounds[Log::ChannelJoinConnect]     = qmMessageSounds[Log::ChannelJoin];
	qmMessageSounds[Log::ChannelLeaveDisconnect] = qmMessageSounds[Log::ChannelLeave];
	qmMessageSounds[Log::YouMutedOther]          = QLatin1String(":/UserMutedYouOrByYou.ogg");
	qmMessageSounds[Log::YouMuted]               = QLatin1String(":/UserMutedYouOrByYou.ogg");
	qmMessageSounds[Log::YouKicked]              = QLatin1String(":/UserKickedYouOrByYou.ogg");
	qmMessageSounds[Log::Recording]              = QLatin1String(":/RecordingStateChanged.ogg");

	qmMessages[Log::DebugInfo]       = Settings::LogConsole;
	qmMessages[Log::Warning]         = Settings::LogConsole | Settings::LogBalloon;
	qmMessages[Log::Information]     = Settings::LogConsole;
	qmMessages[Log::UserJoin]        = Settings::LogConsole;
	qmMessages[Log::UserLeave]       = Settings::LogConsole;
	qmMessages[Log::UserKicked]      = Settings::LogConsole;
	qmMessages[Log::OtherSelfMute]   = Settings::LogConsole;
	qmMessages[Log::OtherMutedOther] = Settings::LogConsole;
	qmMessages[Log::UserRenamed]     = Settings::LogConsole;
	qmMessages[Log::PluginMessage]   = Settings::LogConsole;
}

bool Settings::doEcho() const {
	if (AudioInputRegistrar::qmNew) {
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qsAudioInput);
		if (air) {
			if ((Global::get().s.echoOption != EchoCancelOptionID::DISABLED)
				&& air->canEcho(Global::get().s.echoOption, qsAudioOutput))
				return true;
		}
	}
	return false;
}

bool Settings::doPositionalAudio() const {
	return bPositionalAudio;
}

void OverlaySettings::savePresets(const QString &filename) {
	nlohmann::json settingsJSON = *this;

	settingsJSON.erase(SettingsKeys::OVERLAY_ENABLE_KEY);
	settingsJSON.erase(SettingsKeys::OVERLAY_WHITELIST_KEY);
	settingsJSON.erase(SettingsKeys::OVERLAY_WHITELIST_EXCLUDE_KEY);
	settingsJSON.erase(SettingsKeys::OVERLAY_BLACKLIST_KEY);
	settingsJSON.erase(SettingsKeys::OVERLAY_BLACKLIST_EXCLUDE_KEY);
	settingsJSON.erase(SettingsKeys::OVERLAY_LAUNCHERS_KEY);
	settingsJSON.erase(SettingsKeys::OVERLAY_LAUNCHERS_EXCLUDE_KEY);

	std::ofstream stream(Mumble::QtUtils::qstring_to_path(filename));

	stream << settingsJSON.dump(4) << std::endl;
}

void OverlaySettings::load(const QString &filename) {
	std::ifstream stream(Mumble::QtUtils::qstring_to_path(filename));

	nlohmann::json settingsJSON;
	try {
		stream >> settingsJSON;

		settingsJSON.get_to(*this);
	} catch (const nlohmann::json::parse_error &e) {
		qWarning() << "Failed to load overlay settings due to invalid format: " << e.what();
	}
}

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()


BOOST_TYPEOF_REGISTER_TYPE(Qt::Alignment)
BOOST_TYPEOF_REGISTER_TYPE(Settings::AudioTransmit)
BOOST_TYPEOF_REGISTER_TYPE(Settings::VADSource)
BOOST_TYPEOF_REGISTER_TYPE(Settings::LoopMode)
BOOST_TYPEOF_REGISTER_TYPE(Settings::OverlayShow)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ProxyType)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ChannelExpand)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ChannelDrag)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ServerShow)
BOOST_TYPEOF_REGISTER_TYPE(Settings::NoiseCancel)
BOOST_TYPEOF_REGISTER_TYPE(Settings::WindowLayout)
BOOST_TYPEOF_REGISTER_TYPE(Settings::AlwaysOnTopBehaviour)
BOOST_TYPEOF_REGISTER_TYPE(Settings::RecordingMode)
BOOST_TYPEOF_REGISTER_TYPE(QString)
BOOST_TYPEOF_REGISTER_TYPE(QByteArray)
BOOST_TYPEOF_REGISTER_TYPE(QColor)
BOOST_TYPEOF_REGISTER_TYPE(QVariant)
BOOST_TYPEOF_REGISTER_TYPE(QFont)
BOOST_TYPEOF_REGISTER_TYPE(EchoCancelOptionID)
BOOST_TYPEOF_REGISTER_TEMPLATE(QList, 1)


#define LOAD(var, name) var = qvariant_cast< BOOST_TYPEOF(var) >(settings_ptr->value(QLatin1String(name), var))
#define LOADENUM(var, name) \
	var = static_cast< BOOST_TYPEOF(var) >(settings_ptr->value(QLatin1String(name), var).toInt())
#define LOADFLAG(var, name) \
	var = static_cast< BOOST_TYPEOF(var) >(settings_ptr->value(QLatin1String(name), static_cast< int >(var)).toInt())
#define DEPRECATED(name) \
	do {                 \
	} while (0)

// Workaround for mumble-voip/mumble#2638.
//
// Qt previously expected to be able to write
// NUL bytes in strings in plists. This is no
// longer possible, which causes Qt to write
// incomplete strings to the preferences plist.
// These are of the form "@Variant(", and, for
// Mumble, typically happen for float values.
//
// We detect this bad value and avoid loading
// it. This causes such settings to fall back
// to their defaults, instead of being set to
// a zero value.
#ifdef Q_OS_MACOS
#	undef LOAD
#	define LOAD(var, name)                                                                              \
		do {                                                                                             \
			if (settings_ptr->value(QLatin1String(name)).toString() != QLatin1String("@Variant(")) {     \
				var = qvariant_cast< BOOST_TYPEOF(var) >(settings_ptr->value(QLatin1String(name), var)); \
			}                                                                                            \
		} while (0)
#endif

void OverlaySettings::legacyLoad(QSettings *settings_ptr) {
	LOAD(bEnable, "enable");

	LOADENUM(osShow, "show");
	LOAD(bAlwaysSelf, "alwaysself");
	LOAD(uiActiveTime, "activetime");
	LOADENUM(osSort, "sort");

	LOAD(fX, "x");
	LOAD(fY, "y");
	LOAD(fZoom, "zoom");
	LOAD(uiColumns, "columns");

	settings_ptr->beginReadArray(QLatin1String("states"));
	for (unsigned int i = 0; i < 4; ++i) {
		settings_ptr->setArrayIndex(static_cast< int >(i));
		LOAD(qcUserName[i], "color");
		LOAD(fUser[i], "opacity");
	}
	settings_ptr->endArray();

	LOAD(qfUserName, "userfont");
	LOAD(qfChannel, "channelfont");
	LOAD(qcChannel, "channelcolor");
	LOAD(qfFps, "fpsfont");
	LOAD(qcFps, "fpscolor");

	LOAD(fBoxPad, "padding");
	LOAD(fBoxPenWidth, "penwidth");
	LOAD(qcBoxPen, "pencolor");
	LOAD(qcBoxFill, "fillcolor");

	LOAD(bUserName, "usershow");
	LOAD(bChannel, "channelshow");
	LOAD(bMutedDeafened, "mutedshow");
	LOAD(bAvatar, "avatarshow");
	LOAD(bBox, "boxshow");
	LOAD(bFps, "fpsshow");
	LOAD(bTime, "timeshow");

	LOAD(fUserName, "useropacity");
	LOAD(fChannel, "channelopacity");
	LOAD(fMutedDeafened, "mutedopacity");
	LOAD(fAvatar, "avataropacity");
	LOAD(fFps, "fpsopacity");

	LOAD(qrfUserName, "userrect");
	LOAD(qrfChannel, "channelrect");
	LOAD(qrfMutedDeafened, "mutedrect");
	LOAD(qrfAvatar, "avatarrect");
	LOAD(qrfFps, "fpsrect");
	LOAD(qrfTime, "timerect");

	LOADFLAG(qaUserName, "useralign");
	LOADFLAG(qaChannel, "channelalign");
	LOADFLAG(qaMutedDeafened, "mutedalign");
	LOADFLAG(qaAvatar, "avataralign");

	LOADENUM(oemOverlayExcludeMode, "mode");
	LOAD(qslLaunchers, "launchers");
	LOAD(qslLaunchersExclude, "launchersexclude");
	LOAD(qslWhitelist, "whitelist");
	LOAD(qslWhitelistExclude, "whitelistexclude");
	LOAD(qslPaths, "paths");
	LOAD(qslPathsExclude, "pathsexclude");
	LOAD(qslBlacklist, "blacklist");
	LOAD(qslBlacklistExclude, "blacklistexclude");
}

void Settings::legacyLoad(const QString &path) {
	std::unique_ptr< QSettings > settings_ptr;
	QSettings::Format format = QSettings::IniFormat;

#ifdef Q_OS_WINDOWS
	if (path == QLatin1String(REGISTRY_ID)) {
		// Search the registry
		settings_ptr = std::make_unique< QSettings >();
	} else
#endif
		settings_ptr = std::make_unique< QSettings >(path.isEmpty() ? findSettingsLocation(true) : path, format);


	LOAD(qsDatabaseLocation, "databaselocation");

	LOAD(bMute, "audio/mute");
	LOAD(bDeaf, "audio/deaf");
	LOADENUM(atTransmit, "audio/transmit");
	LOAD(uiDoublePush, "audio/doublepush");
	LOAD(pttHold, "audio/ptthold");
	LOAD(audioCueEnabledPTT, "audio/pushclick");
	LOAD(qsTxAudioCueOn, "audio/pushclickon");
	LOAD(qsTxAudioCueOff, "audio/pushclickoff");
	LOAD(bTxMuteCue, "audio/mutecue");
	LOAD(qsTxMuteCue, "audio/mutecuepath");
	LOAD(iQuality, "audio/quality");
	LOAD(iMinLoudness, "audio/loudness");
	LOAD(fVolume, "audio/volume");
	LOAD(fOtherVolume, "audio/othervolume");
	LOAD(bAttenuateOthers, "audio/attenuateothers");
	LOAD(bAttenuateOthersOnTalk, "audio/attenuateothersontalk");
	LOAD(bAttenuateUsersOnPrioritySpeak, "audio/attenuateusersonpriorityspeak");
	LOAD(bOnlyAttenuateSameOutput, "audio/onlyattenuatesameoutput");
	LOAD(bAttenuateLoopbacks, "audio/attenuateloopbacks");
	LOADENUM(vsVAD, "audio/vadsource");
	LOAD(fVADmin, "audio/vadmin");
	LOAD(fVADmax, "audio/vadmax");

	int oldNoiseSuppress = 0;
	LOAD(oldNoiseSuppress, "audio/noisesupress");
	LOAD(iSpeexNoiseCancelStrength, "audio/speexNoiseCancelStrength");

	// Select the most negative of the 2 (one is expected to be zero as it is
	// unset). This is for compatibility as we have renamed the setting at some point.
	iSpeexNoiseCancelStrength = std::min(oldNoiseSuppress, iSpeexNoiseCancelStrength);

	LOADENUM(noiseCancelMode, "audio/noiseCancelMode");

#ifndef USE_RENAMENOISE
	if (noiseCancelMode == NoiseCancelRNN || noiseCancelMode == NoiseCancelBoth) {
		// Use Speex instead as this Mumble build was built without support for ReNameNoise
		noiseCancelMode = NoiseCancelSpeex;
	}
#endif

	LOAD(bAllowLowDelay, "audio/allowlowdelay");
	LOAD(uiAudioInputChannelMask, "audio/inputchannelmask");
	LOAD(iVoiceHold, "audio/voicehold");
	LOAD(iOutputDelay, "audio/outputdelay");

	// Idle auto actions
	LOAD(iIdleTime, "audio/idletime");
	LOADENUM(iaeIdleAction, "audio/idleaction");
	LOAD(bUndoIdleActionUponActivity, "audio/undoidleactionuponactivity");

	LOAD(fAudioMinDistance, "audio/mindistance");
	LOAD(fAudioMaxDistance, "audio/maxdistance");
	LOAD(fAudioMaxDistVolume, "audio/maxdistancevolume");
	LOAD(fAudioBloom, "audio/bloom");
	DEPRECATED("audio/echo");
	DEPRECATED("audio/echomulti");
	LOAD(bExclusiveInput, "audio/exclusiveinput");
	LOAD(bExclusiveOutput, "audio/exclusiveoutput");
	LOAD(bPositionalAudio, "audio/positional");
	LOAD(bPositionalHeadphone, "audio/headphone");
	LOAD(qsAudioInput, "audio/input");
	LOAD(qsAudioOutput, "audio/output");
	LOAD(bWhisperFriends, "audio/whisperfriends");
	LOAD(iMessageLimitUserThreshold, "audio/messagelimitusers");
	LOAD(bTransmitPosition, "audio/postransmit");

	if (settings_ptr->contains("audio/echooptionid")) {
		// Load the new echo cancel option instead
		LOADFLAG(echoOption, "audio/echooptionid");
	} else {
#ifndef Q_OS_MACOS
		// Compatibility layer for overtaking the old (now deprecated) settings
		// This block should only be called once at the first start of the new Mumble version
		// As echo cancellation was not available on macOS before, we don't have to run this compatibility
		// code on macOS (instead simply use the new default as set in the constructor).
		if (settings_ptr->contains("audio/echo")) {
			bool deprecatedEcho      = false;
			bool deprecatedEchoMulti = false;

			LOAD(deprecatedEcho, "audio/echo");
			LOAD(deprecatedEchoMulti, "audio/echomulti");

			if (deprecatedEcho) {
				if (deprecatedEchoMulti) {
					echoOption = EchoCancelOptionID::SPEEX_MULTICHANNEL;
				} else {
					echoOption = EchoCancelOptionID::SPEEX_MIXED;
				}
			} else {
				echoOption = EchoCancelOptionID::DISABLED;
			}
		}
#endif
	}

	LOAD(iJitterBufferSize, "net/jitterbuffer");
	LOAD(iFramesPerPacket, "net/framesperpacket");

	LOAD(bASIOEnable, "asio/enable");
	LOAD(qsASIOclass, "asio/class");
	LOAD(qlASIOmic, "asio/mic");
	LOAD(qlASIOspeaker, "asio/speaker");

	LOAD(qsWASAPIInput, "wasapi/input");
	LOAD(qsWASAPIOutput, "wasapi/output");
	LOAD(qsWASAPIRole, "wasapi/role");

	LOAD(qsALSAInput, "alsa/input");
	LOAD(qsALSAOutput, "alsa/output");

	LOAD(pipeWireInput, "pipewire/input");
	LOAD(pipeWireOutput, "pipewire/output");

	LOAD(qsPulseAudioInput, "pulseaudio/input");
	LOAD(qsPulseAudioOutput, "pulseaudio/output");

	LOAD(qsJackAudioOutput, "jack/output");
	LOAD(bJackStartServer, "jack/startserver");
	LOAD(bJackAutoConnect, "jack/autoconnect");

	LOAD(qsOSSInput, "oss/input");
	LOAD(qsOSSOutput, "oss/output");

	LOAD(qsCoreAudioInput, "coreaudio/input");
	LOAD(qsCoreAudioOutput, "coreaudio/output");

	LOAD(iPortAudioInput, "portaudio/input");
	LOAD(iPortAudioOutput, "portaudio/output");

	LOAD(bTTS, "tts/enable");
	LOAD(iTTSVolume, "tts/volume");
	LOAD(iTTSThreshold, "tts/threshold");
	LOAD(bTTSMessageReadBack, "tts/readback");
	LOAD(bTTSNoScope, "tts/noscope");
	LOAD(bTTSNoAuthor, "tts/noauthor");
	LOAD(qsTTSLanguage, "tts/language");

	// Network settings
	LOAD(bTCPCompat, "net/tcponly");
	LOAD(bQoS, "net/qos");
	LOAD(bReconnect, "net/reconnect");
	LOAD(bAutoConnect, "net/autoconnect");
	LOAD(bSuppressIdentity, "net/suppress");
	LOADENUM(ptProxyType, "net/proxytype");
	LOAD(qsProxyHost, "net/proxyhost");
	LOAD(usProxyPort, "net/proxyport");
	LOAD(qsProxyUsername, "net/proxyusername");
	LOAD(qsProxyPassword, "net/proxypassword");
	DEPRECATED("net/maximagesize");
	LOAD(iMaxImageWidth, "net/maximagewidth");
	LOAD(iMaxImageHeight, "net/maximageheight");
	LOAD(qsServicePrefix, "net/serviceprefix");
	LOAD(iMaxInFlightTCPPings, "net/maxinflighttcppings");
	LOAD(iPingIntervalMsec, "net/pingintervalmsec");
	LOAD(iConnectionTimeoutDurationMsec, "net/connectiontimeoutdurationmsec");
	LOAD(bUdpForceTcpAddr, "net/udpforcetcpaddr");

	// Network settings - SSL
	LOAD(qsSslCiphers, "net/sslciphers");

	// Privacy settings
	LOAD(bHideOS, "privacy/hideos");

	LOAD(qsLanguage, "ui/language");
	LOAD(themeName, "ui/theme");
	LOAD(themeStyleName, "ui/themestyle");
	LOADENUM(ceExpand, "ui/expand");
	LOADENUM(ceChannelDrag, "ui/drag");
	LOADENUM(ceUserDrag, "ui/userdrag");
	LOADENUM(aotbAlwaysOnTop, "ui/alwaysontop");
	LOAD(bEnableDeveloperMenu, "ui/developermenu");
	LOAD(bLockLayout, "ui/locklayout");
	LOAD(bMinimalView, "ui/minimalview");
	LOAD(bHideFrame, "ui/hideframe");
	LOAD(bUserTop, "ui/usertop");
	LOAD(qbaMainWindowGeometry, "ui/geometry");
	LOAD(qbaMainWindowState, "ui/state");
	LOAD(qbaMinimalViewGeometry, "ui/minimalviewgeometry");
	LOAD(qbaMinimalViewState, "ui/minimalviewstate");
	LOAD(qbaConfigGeometry, "ui/ConfigGeometry");
	LOADENUM(wlWindowLayout, "ui/WindowLayout");
	LOAD(qsUsername, "ui/username");
	LOAD(qsLastServer, "ui/server");
	LOADENUM(ssFilter, "ui/serverfilter");

	LOAD(bUpdateCheck, "ui/updatecheck");
	LOAD(bPluginCheck, "ui/plugincheck");
	LOAD(bPluginAutoUpdate, "ui/pluginAutoUpdate");

	LOAD(bHideInTray, "ui/hidetray");
	LOAD(bStateInTray, "ui/stateintray");
	LOAD(bUsage, "ui/usage");
	LOAD(bShowUserCount, "ui/showusercount");
	LOAD(bShowVolumeAdjustments, "ui/showVolumeAdjustments");
	LOAD(bShowNicknamesOnly, "ui/showNicknamesOnly");
	LOAD(bChatBarUseSelection, "ui/chatbaruseselection");
	LOAD(bFilterHidesEmptyChannels, "ui/filterhidesemptychannels");
	LOAD(bFilterActive, "ui/filteractive");
	LOAD(qsImagePath, "ui/imagepath");
	LOAD(bShowContextMenuInMenuBar, "ui/showcontextmenuinmenubar");
	LOAD(qbaConnectDialogGeometry, "ui/connect/geometry");
	LOAD(qbaConnectDialogHeader, "ui/connect/header");
	LOAD(bShowTransmitModeComboBox, "ui/transmitmodecombobox");
	LOAD(bHighContrast, "ui/HighContrast");
	LOAD(iMaxLogBlocks, "ui/MaxLogBlocks");
	LOAD(bLog24HourClock, "ui/24HourClock");
	LOAD(iChatMessageMargins, "ui/ChatMessageMargins");
	LOAD(bDisablePublicList, "ui/disablepubliclist");

	if (settings_ptr->contains("ui/askonquit")) {
		// Compatibility layer for overtaking the old (now deprecated) settings
		// This block should only be called once at the first start of the new Mumble version
		bool deprecatedAskOnQuit = true;
		LOAD(deprecatedAskOnQuit, "ui/askonquit");

		quitBehavior = deprecatedAskOnQuit ? QuitBehavior::ALWAYS_ASK : QuitBehavior::ALWAYS_QUIT;
	}

	// TalkingUI
	LOAD(qpTalkingUI_Position, "ui/talkingUIPosition");
	LOAD(bShowTalkingUI, "ui/showTalkingUI");
	LOAD(bTalkingUI_LocalUserStaysVisible, "ui/talkingUI_LocalUserStaysVisible");
	LOAD(bTalkingUI_AbbreviateChannelNames, "ui/talkingUI_AbbreviateChannelNames");
	LOAD(bTalkingUI_AbbreviateCurrentChannel, "ui/talkingUI_AbbreviateCurrentChannel");
	LOAD(bTalkingUI_ShowLocalListeners, "ui/talkingUI_ShowLocalListeners");
	LOAD(iTalkingUI_RelativeFontSize, "ui/talkingUI_RelativeFontSize");
	LOAD(iTalkingUI_SilentUserLifeTime, "ui/talkingUI_SilentUserLifeTime");
	LOAD(iTalkingUI_ChannelHierarchyDepth, "ui/talkingUI_ChannelHierarchieDepth");
	LOAD(iTalkingUI_MaxChannelNameLength, "ui/talkingUI_MaxChannelNameLength");
	LOAD(iTalkingUI_PrefixCharCount, "ui/talkingUI_PrefixCharCount");
	LOAD(iTalkingUI_PostfixCharCount, "ui/talkingUI_PostfixCharCount");
	LOAD(qsTalkingUI_AbbreviationReplacement, "ui/talkingUI_AbbreviationReplacement");

	// Load the old setting first in case it is set and then load the actual setting
	LOAD(qsHierarchyChannelSeparator, "ui/talkingUI_ChannelSeparator");
	LOAD(qsHierarchyChannelSeparator, "ui/hierarchy_channelSeparator");

	LOAD(manualPlugin_silentUserDisplaytime, "ui/manualPlugin_silentUserDisplaytime");

	// PTT Button window
	LOAD(bShowPTTButtonWindow, "ui/showpttbuttonwindow");
	LOAD(qbaPTTButtonWindowGeometry, "ui/pttbuttonwindowgeometry");

	// Recording
	LOAD(qsRecordingPath, "recording/path");
	LOAD(qsRecordingFile, "recording/file");
	LOADENUM(rmRecordingMode, "recording/mode");
	LOAD(iRecordingFormat, "recording/format");

	// Special configuration options not exposed to UI
	LOAD(disableConnectDialogEditing, "ui/disableconnectdialogediting");
	LOAD(bPingServersDialogViewed, "consent/pingserversdialogviewed");

	// OverlayPrivateWin
	LOAD(iOverlayWinHelperRestartCooldownMsec, "overlay_win/helper/restart_cooldown_msec");
	LOAD(bOverlayWinHelperX86Enable, "overlay_win/helper/x86/enable");
	LOAD(bOverlayWinHelperX64Enable, "overlay_win/helper/x64/enable");

	// LCD
	LOAD(iLCDUserViewMinColWidth, "lcd/userview/mincolwidth");
	LOAD(iLCDUserViewSplitterWidth, "lcd/userview/splitterwidth");

	QByteArray qba = qvariant_cast< QByteArray >(settings_ptr->value(QLatin1String("net/certificate")));
	if (!qba.isEmpty())
		kpCertificate = CertWizard::importCert(qba);

	LOAD(bShortcutEnable, "shortcut/enable");
	LOAD(bSuppressMacEventTapWarning, "shortcut/mac/suppresswarning");
	LOAD(bEnableEvdev, "shortcut/linux/evdev/enable");
	LOAD(bEnableXInput2, "shortcut/x11/xinput2/enable");
	LOAD(bEnableGKey, "shortcut/gkey");
	LOAD(bEnableXboxInput, "shortcut/windows/xbox/enable");
	LOAD(bEnableUIAccess, "shortcut/windows/uiaccess/enable");

	// Search options
	LOAD(searchForUsers, "search/search_for_users");
	LOAD(searchForChannels, "search/search_for_channels");
	LOAD(searchCaseSensitive, "search/search_case_sensitive");
	LOAD(searchAsRegex, "search/search_as_regex");
	LOAD(searchOptionsShown, "search/search_options_shown");
	LOADFLAG(searchUserAction, "search/search_user_action");
	LOADFLAG(searchChannelAction, "search/search_channel_action");
	LOAD(searchDialogPosition, "search/search_dialog_position");

	int nshorts = settings_ptr->beginReadArray(QLatin1String("shortcuts"));
	for (int i = 0; i < nshorts; i++) {
		settings_ptr->setArrayIndex(i);
		Shortcut s;

		s.iIndex = -2;

		LOAD(s.iIndex, "index");
		LOAD(s.qlButtons, "keys");
		LOAD(s.bSuppress, "suppress");
		s.qvData = settings_ptr->value(QLatin1String("data"));
		if (s.iIndex >= -1)
			qlShortcuts << s;
	}
	settings_ptr->endArray();

	settings_ptr->beginReadArray(QLatin1String("messages"));
	for (QMap< int, quint32 >::const_iterator it = qmMessages.constBegin(); it != qmMessages.constEnd(); ++it) {
		settings_ptr->setArrayIndex(it.key());
		LOAD(qmMessages[it.key()], "log");
	}
	settings_ptr->endArray();

	settings_ptr->beginReadArray(QLatin1String("messagesounds"));
	for (QMap< int, QString >::const_iterator it = qmMessageSounds.constBegin(); it != qmMessageSounds.constEnd();
		 ++it) {
		settings_ptr->setArrayIndex(it.key());
		LOAD(qmMessageSounds[it.key()], "logsound");
	}
	settings_ptr->endArray();

	settings_ptr->beginGroup(QLatin1String("lcd/devices"));
	foreach (const QString &d, settings_ptr->childKeys()) {
		qmLCDDevices.insert(d, settings_ptr->value(d, true).toBool());
	}
	settings_ptr->endGroup();

	// Plugins
	settings_ptr->beginGroup(QLatin1String("plugins"));
	foreach (const QString &pluginKey, settings_ptr->childGroups()) {
		QString pluginHash;

		if (pluginKey.contains(QLatin1String("_"))) {
			// The key contains the filename as well as the hash
			const auto index = pluginKey.lastIndexOf(QLatin1String("_"));
			pluginHash       = pluginKey.right(pluginKey.size() - index - 1);
		} else {
			pluginHash = pluginKey;
		}

		PluginSetting pluginSettings;
		pluginSettings.path = settings_ptr->value(pluginKey + QLatin1String("/path")).toString();
		pluginSettings.allowKeyboardMonitoring =
			settings_ptr->value(pluginKey + QLatin1String("/allowKeyboardMonitoring")).toBool();
		pluginSettings.enabled = settings_ptr->value(pluginKey + QLatin1String("/enabled")).toBool();
		pluginSettings.positionalDataEnabled =
			settings_ptr->value(pluginKey + QLatin1String("/positionalDataEnabled")).toBool();

		qhPluginSettings.insert(pluginHash, pluginSettings);
	}
	settings_ptr->endGroup();

	settings_ptr->beginGroup(QLatin1String("overlay"));
	os.legacyLoad(settings_ptr.get());
	settings_ptr->endGroup();


	// This field previously populated Settings::uiUpdateCounter, which no longer exists. We require it though, in order
	// to determine whether we have to perform some migration work.
	unsigned int configVersion = 0;
	LOAD(configVersion, "lastupdate");
	audioWizardShown = true;
	switch (configVersion) {
		case 0:
			// Previous version was pre 1.2.3 or Mumble is run for the first time
			audioWizardShown = false;
			// Fallthrough
		case 1:
			// Previous versions used old idle action style, convert it
			if (iIdleTime == 5 * 60) { // New default
				iaeIdleAction = Settings::Nothing;
			} else {
				iIdleTime =
					static_cast< unsigned int >(60 * qRound(Global::get().s.iIdleTime / 60.)); // Round to minutes
				iaeIdleAction = Settings::Deafen;                                              // Old behavior
			}
			// Fallthrough
#ifdef Q_OS_WIN
		case 2: {
			QList< Shortcut > &shortcuts              = qlShortcuts;
			const QList< Shortcut > migratedShortcuts = GlobalShortcutWin::migrateSettings(shortcuts);
			if (shortcuts.size() > migratedShortcuts.size()) {
				const uint32_t num = shortcuts.size() - migratedShortcuts.size();
				QMessageBox::warning(
					nullptr, QObject::tr("Shortcuts migration incomplete"),
					QObject::tr("Unfortunately %1 shortcut(s) could not be migrated.\nYou can register them again.")
						.arg(num));
			}

			shortcuts = migratedShortcuts;
		}
#endif
	}
}

void Settings::migratePluginSettings(const MigratedPath &path) {
	std::vector< PluginSetting > migratedSettings;

	auto it = qhPluginSettings.begin();

	while (it != qhPluginSettings.end()) {
		if (it.value().path.startsWith(path.oldPath)) {
			// Migrate the settings for this plugin
			it.value().path.replace(path.oldPath, path.newPath);

			// Move setting out of hash-map
			migratedSettings.push_back(std::move(it.value()));

			// Remove this entry
			it = qhPluginSettings.erase(it);
		} else {
			it++;
		}
	}

	// Back-insert all entries that have been migrated
	for (const PluginSetting &current : migratedSettings) {
		qhPluginSettings.insert(
			QLatin1String(QCryptographicHash::hash(current.path.toUtf8(), QCryptographicHash::Sha1).toHex()),
			std::move(current));
	}
}



QDataStream &operator>>(QDataStream &arch, PluginSetting &setting) {
	arch >> setting.enabled;
	arch >> setting.positionalDataEnabled;
	arch >> setting.allowKeyboardMonitoring;

	return arch;
}

QDataStream &operator<<(QDataStream &arch, const PluginSetting &setting) {
	arch << setting.enabled;
	arch << setting.positionalDataEnabled;
	arch << setting.allowKeyboardMonitoring;

	return arch;
}

#undef LOAD
#undef LOADENUM
#undef LOADFLAG
#undef DEPRECATED
#undef SAVE
#undef SAVEFLAG


void Settings::verifySettingsKeys() const {
	std::vector< const char * > categoryNames;
	const char *currentCategoryName;
#define PROCESS(category, key, variable) currentCategoryName = #category;
#define INTERMEDIATE_OPERATION categoryNames.push_back(currentCategoryName);
	PROCESS_ALL_SETTINGS_WITH_INTERMEDIATE_OPERATION
	PROCESS_ALL_OVERLAY_SETTINGS_WITH_INTERMEDIATE_OPERATION

	// Assert that all entries in categoryNames are unique
	std::sort(categoryNames.begin(), categoryNames.end());
	assert(std::unique(categoryNames.begin(), categoryNames.end(),
					   [](const char *lhs, const char *rhs) { return std::strcmp(lhs, rhs) == 0; })
		   == categoryNames.end());

#undef PROCESS
#undef INTERMEDIATE_OPERATION


	std::vector< std::string > keyNames;
#define PROCESS(category, key, variable) keyNames.push_back(#key);
#define INTERMEDIATE_OPERATION                                               \
	std::sort(keyNames.begin(), keyNames.end());                             \
	assert(std::unique(keyNames.begin(), keyNames.end()) == keyNames.end()); \
	keyNames.clear();
	PROCESS_ALL_SETTINGS_WITH_INTERMEDIATE_OPERATION
	PROCESS_ALL_OVERLAY_SETTINGS_WITH_INTERMEDIATE_OPERATION
#undef PROCESS
#undef INTERMEDIATE_OPERATION


	std::vector< std::string > variableNames;
#define PROCESS(category, key, variable) variableNames.push_back(#variable);
	PROCESS_ALL_SETTINGS
	std::sort(variableNames.begin(), variableNames.end());
	assert(std::unique(variableNames.begin(), variableNames.end()) == variableNames.end());
	variableNames.clear();

	PROCESS_ALL_OVERLAY_SETTINGS
	std::sort(variableNames.begin(), variableNames.end());
	assert(std::unique(variableNames.begin(), variableNames.end()) == variableNames.end());
#undef PROCESS
}


QString Settings::findSettingsLocation(bool legacy, bool *foundExistingFile) const {
	// In order to make sure we'll find (mostly legacy) settings files, even if they end up being in a slightly
	// different dir than we currently expect, we construct a search path list that we'll traverse while searching for
	// the settings file. In case we find a suitable settings file within the search path, then we'll continue to use
	// this path instead of creating a new one (in the location that we currently think is best to use).
	QStringList paths;
	paths << QCoreApplication::instance()->applicationDirPath();
	paths << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	paths << QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	paths << QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
	paths << QFileInfo(QSettings().fileName()).dir().absolutePath();


	QStringList settingsFileNames = legacy ? QStringList({ QStringLiteral("mumble.conf"), QStringLiteral("Mumble.conf"),
														   QStringLiteral("mumble.ini"), QStringLiteral("Mumble.ini") })
										   : QStringList({ QStringLiteral("mumble_settings.json") });

	QString chosenPath;

	for (const QString &settingsFileName : settingsFileNames) {
		for (const QString &currentPath : paths) {
			QFile settingsFile(QString::fromLatin1("%1/%2").arg(currentPath).arg(settingsFileName));

			if (settingsFile.exists() && settingsFile.permissions().testFlag(QFile::WriteUser)) {
				// Found existing settings file -> use that
				chosenPath = QFileInfo(settingsFile).absoluteFilePath();

				break;
			}
		}
	}

	if (foundExistingFile) {
		*foundExistingFile = !chosenPath.isEmpty();
	}

	if (chosenPath.isEmpty()) {
		// We were unable to find an existing settings file -> Fall back to a default location
		chosenPath = QString::fromLatin1("%1/%2")
						 .arg(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation))
						 .arg(settingsFileNames[0]);
		// Note: QStandardPaths::AppConfigLocation will return a directory of the style <root>/<org>/<application> where
		// <root> is the path to the general config related directory on the respective OS, <org> is the name of our
		// organization and <application> is our application's name. In our case (at the time of writing this) <org> =
		// <application> = Mumble, leading to a doubly nested "Mumble" directory. This should only be a cosmetic issue
		// though.
	}

	return chosenPath;
}
