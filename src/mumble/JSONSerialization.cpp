// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "JSONSerialization.h"
#include "Cert.h"
#include "SettingsMacros.h"


template< typename T, bool isEnum > struct SaveValueConverter {
	static const T &getValue(const T &value) { return value; }

	static void loadValue(const nlohmann::json &json, T &variable) { json.get_to(variable); }
};

template< typename T > using has_enum_conversion_function = decltype(enumToString(std::declval< T >()));

template< typename T > struct SaveValueConverter< T, true > {
	static const char *getValue(const T &enumeration) {
#if BOOST_VERSION >= 106700
		// Use the "detection idiom" to provide a proper error message in case there is no conversion function for the
		// given enum.
		// boost::is_detected is only available since Boost 1.67 and since we only use it to create a better error
		// message, we can simply not make use of it, if earlier versions are used.
		static_assert(boost::is_detected< has_enum_conversion_function, T >::value,
					  "No viable enum-to-name function found");
#endif

		return enumToString(enumeration);
	}

	static void loadValue(const nlohmann::json &json, T &variable) {
		variable = stringToEnum< T >(json.get< std::string >());
	}
};

template< typename T > void save(nlohmann::json &json, const SettingsKey &key, const T &value) {
	json[key] = SaveValueConverter< T, std::is_enum< T >::value >::getValue(value);
}

template< typename T > void save(nlohmann::json &json, const char *category, const SettingsKey &key, const T &value) {
	if (!json.contains(category)) {
		json[category] = nlohmann::json::object();
	}

	save(json[category], key, value);
}

template< typename T, bool isArray > struct CopyHelper {
	static void copy(T &dest, const T &source) { dest = source; }
};

template< typename T > struct CopyHelper< T, true > {
	template< size_t N > static constexpr std::size_t size(const T (&)[N]) { return N; }

	static constexpr std::size_t size(const T &array) { return sizeof(array) / sizeof(*array); }

	static void copy(T &dest, const T &source) {
		static_assert(size(dest) == size(dest), "Can only copy arrays of equal size");

		std::copy(source, source + size(source), dest);
	}
};

template< typename T >
void load(const nlohmann::json &json, const SettingsKey &key, T &variable, const T &defaultValue,
		  bool useDefault = true) {
	nlohmann::json valueEntry = key.selectFrom(json);

	if (!valueEntry.is_null()) {
		SaveValueConverter< T, std::is_enum< T >::value >::loadValue(valueEntry, variable);
	} else if (useDefault) {
		CopyHelper< T, std::is_array< T >::value >::copy(variable, defaultValue);
	}
}

template< typename T >
void load(const nlohmann::json &json, const char *category, const SettingsKey &key, T &variable, const T &defaultValue,
		  bool useDefault = true) {
	if (!json.contains(category)) {
		if (useDefault) {
			CopyHelper< T, std::is_array< T >::value >::copy(variable, defaultValue);
		}

		return;
	}

	const nlohmann::json &categoryJSON = json.at(category);

	load(categoryJSON, key, variable, defaultValue, useDefault);
}



void to_json(nlohmann::json &j, const Settings &settings) {
	j[SettingsKeys::SETTINGS_VERSION_KEY] = 1;

	const Settings defaultValues;

#define PROCESS(category, key, variable)                          \
	if (settings.variable != defaultValues.variable) {            \
		save(j, #category, SettingsKeys::key, settings.variable); \
	}

	PROCESS_ALL_SETTINGS

#undef PROCESS

	if (settings.qlShortcuts != defaultValues.qlShortcuts) {
		// We only remove server specific shortcuts since they are saved in the DB.
		// We should consider moving those out of the DB into the JSON settings.
		QList< Shortcut > genericShortcuts;
		for (const Shortcut &shortcut : settings.qlShortcuts) {
			if (!shortcut.isServerSpecific()) {
				genericShortcuts.push_back(shortcut);
			}
		}

		j["shortcuts"]["defined"] = genericShortcuts;
	}

	if (settings.qmMessages != defaultValues.qmMessages) {
		j["messages"]["traits"] = settings.qmMessages;
	}
	if (settings.qmMessageSounds != defaultValues.qmMessageSounds) {
		j["messages"]["sounds"] = settings.qmMessageSounds;
	}

	if (settings.qmLCDDevices != defaultValues.qmLCDDevices) {
		j["lcd"]["devices"] = settings.qmLCDDevices;
	}

	if (settings.qhPluginSettings != defaultValues.qhPluginSettings) {
		j["plugins"] = settings.qhPluginSettings;
	}

	if (settings.os != defaultValues.os) {
		j["overlay"] = settings.os;
	}

	j[SettingsKeys::CERTIFICATE_KEY] = CertWizard::exportCert(settings.kpCertificate);

	// Save whether Mumble has quit regularly (in contrast to having crashed). This flag is set right before saving the
	// settings because Mumble is shut down (regularly). In contrast, when saving the settings because the user has made
	// changes in the UI, this will be false. Thus when loading the settings again and seeing this flag is false, it
	// means Mumble never got to perform its regular shutdown routine, meaning that it is likely that it has crashed
	// before.
	j[SettingsKeys::MUMBLE_QUIT_NORMALLY_KEY] = settings.mumbleQuitNormally;
}

void migrateSettings(nlohmann::json &json, int settingsVersion) {
	// Perform conversions required to transform the given JSON into the format applicable to be read out by the most
	// recent standards

	// Check if the old ask_on_quit key exists and the new one does not exist within the json file
	if (json.contains("ask_on_quit")
		&& (!json.contains(static_cast< const char * >(SettingsKeys::QUIT_BEHAVIOR_KEY)))) {
		if (!json.at("ask_on_quit").get< bool >()) {
			json[SettingsKeys::QUIT_BEHAVIOR_KEY] = QuitBehavior::ALWAYS_QUIT;
		} else {
			json[SettingsKeys::QUIT_BEHAVIOR_KEY] = QuitBehavior::ALWAYS_ASK;
		}
	}

	if (json.contains("play_transmit_cue")
		&& (!json.contains(static_cast< const char * >(SettingsKeys::TRANSMIT_CUE_WHEN_PTT_KEY)))) {
		json[SettingsKeys::TRANSMIT_CUE_WHEN_PTT_KEY] = json.at("play_transmit_cue").get< bool >();
	}

	(void) settingsVersion;
}

void from_json(const nlohmann::json &j, Settings &settings) {
	int settingsVersion = j.at(SettingsKeys::SETTINGS_VERSION_KEY).get< int >();

	// Copy since we might have to make modifications
	nlohmann::json json = j;

	migrateSettings(json, settingsVersion);

#define PROCESS(category, key, variable) \
	load(json, #category, SettingsKeys::key, settings.variable, settings.variable, true);

	PROCESS_ALL_SETTINGS

#undef PROCESS

	if (json.contains("shortcuts") && json.at("shortcuts").contains("defined")) {
		settings.qlShortcuts = json.at("shortcuts").at("defined");
	}

	if (json.contains("messages") && json.at("messages").contains("traits")) {
		settings.qmMessages = json.at("messages").at("traits");
	}
	if (json.contains("messages") && json.at("messages").contains("sounds")) {
		settings.qmMessageSounds = json.at("messages").at("sounds");
	}

	if (json.contains("lcd") && json.at("lcd").contains("devices")) {
		settings.qmLCDDevices = json.at("lcd").at("devices");
	}

	if (json.contains("plugins")) {
		settings.qhPluginSettings = json.at("plugins");
	}

	if (json.contains("overlay")) {
		settings.os = json.at("overlay");
	}

	if (json.contains(static_cast< const char * >(SettingsKeys::CERTIFICATE_KEY))) {
		settings.kpCertificate = CertWizard::importCert(json.at(SettingsKeys::CERTIFICATE_KEY));
	}

	if (json.contains(static_cast< const char * >(SettingsKeys::MUMBLE_QUIT_NORMALLY_KEY))) {
		settings.mumbleQuitNormally = json.at(SettingsKeys::MUMBLE_QUIT_NORMALLY_KEY);
	}

#ifndef USE_RENAMENOISE
	if (settings.noiseCancelMode == Settings::NoiseCancelRNN || settings.noiseCancelMode == Settings::NoiseCancelBoth) {
		// Use Speex instead as this Mumble build was built without support for ReNameNoise
		settings.noiseCancelMode = Settings::NoiseCancelSpeex;
	}
#endif
}

void to_json(nlohmann::json &j, const OverlaySettings &settings) {
#define PROCESS(category, key, variable) save(j, SettingsKeys::key, settings.variable);

	PROCESS_ALL_OVERLAY_SETTINGS

#undef PROCESS
}

void from_json(const nlohmann::json &j, OverlaySettings &settings) {
#define PROCESS(category, key, variable) load(j, SettingsKeys::key, settings.variable, settings.variable, true);

	PROCESS_ALL_OVERLAY_SETTINGS

#undef PROCESS
}



void to_json(nlohmann::json &j, const QString &string) {
	j = string.toStdString();
}

void from_json(const nlohmann::json &j, QString &string) {
	string = QString::fromStdString(j.get< std::string >());
}

void to_json(nlohmann::json &j, const QVariant &variant) {
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_5_0);

	stream << variant;
	j = data.toBase64().toStdString();
}

void from_json(const nlohmann::json &j, QVariant &variant) {
	QByteArray data = QByteArray::fromBase64(QByteArray::fromStdString(j.get< std::string >()));
	QDataStream stream(&data, QIODevice::ReadOnly);
	stream.setVersion(QDataStream::Qt_5_0);

	stream >> variant;
}

void to_json(nlohmann::json &j, const QPoint &point) {
	j = nlohmann::json::object({
		{ "x", point.x() },
		{ "y", point.y() },
	});
}

void from_json(const nlohmann::json &j, QPoint &point) {
	point.setX(j.at("x").get< int >());
	point.setY(j.at("y").get< int >());
}

void to_json(nlohmann::json &j, const ShortcutTarget &target) {
	bool link       = false;
	bool subchannel = false;

	if (target.bCurrentSelection) {
		j["current_selection"] = target.bCurrentSelection;
		link                   = true;
		subchannel             = true;
	} else if (target.bUsers) {
		j["users"] = target.qlUsers;
	} else {
		j["channel_id"] = target.iChannel;
		j["group"]      = target.qsGroup;
		link            = true;
		subchannel      = true;
	}

	if (link) {
		j["include_links"] = target.bLinks;
	}
	if (subchannel) {
		j["include_subchannel"] = target.bChildren;
	}
}

void from_json(const nlohmann::json &j, ShortcutTarget &target) {
	bool link       = false;
	bool subchannel = false;

	if (j.contains("current_selection")) {
		j.at("current_selection").get_to(target.bCurrentSelection);
		link       = true;
		subchannel = true;
	} else if (j.contains("users")) {
		j.at("users").get_to(target.qlUsers);
		target.bUsers = true;
	} else {
		j.at("channel_id").get_to(target.iChannel);
		j.at("group").get_to(target.qsGroup);
		link       = true;
		subchannel = true;
	}

	if (link) {
		j.at("link").get_to(target.bLinks);
	}
	if (subchannel) {
		j.at("include_subchannel").get_to(target.bChildren);
	}
}

void to_json(nlohmann::json &j, const Shortcut &shortcut) {
	j["suppress"] = shortcut.bSuppress;
	j["index"]    = shortcut.iIndex;
	j["buttons"]  = shortcut.qlButtons;
	j["data"]     = shortcut.qvData;
}

void from_json(const nlohmann::json &j, Shortcut &shortcut) {
	j.at("suppress").get_to(shortcut.bSuppress);
	j.at("index").get_to(shortcut.iIndex);
	j.at("buttons").get_to(shortcut.qlButtons);
	j.at("data").get_to(shortcut.qvData);
}

void to_json(nlohmann::json &j, const PluginSetting &setting) {
	j["path"]                        = setting.path;
	j["enabled"]                     = setting.enabled;
	j["positional_data_enabled"]     = setting.positionalDataEnabled;
	j["keyboard_monitoring_allowed"] = setting.allowKeyboardMonitoring;
}

void from_json(const nlohmann::json &j, PluginSetting &setting) {
	j.at("path").get_to(setting.path);
	j.at("enabled").get_to(setting.enabled);
	j.at("positional_data_enabled").get_to(setting.positionalDataEnabled);
	j.at("keyboard_monitoring_allowed").get_to(setting.allowKeyboardMonitoring);
}

void to_json(nlohmann::json &j, const QByteArray &byteArray) {
	j = byteArray.toBase64().toStdString();
}

void from_json(const nlohmann::json &j, QByteArray &byteArray) {
	byteArray = QByteArray::fromBase64(QByteArray::fromStdString(j.get< std::string >()));
}

void to_json(nlohmann::json &j, const QColor &color) {
	j["red"]   = color.red();
	j["green"] = color.green();
	j["blue"]  = color.blue();
	j["alpha"] = color.alpha();
}

void from_json(const nlohmann::json &j, QColor &color) {
	color.setRed(j.at("red").get< decltype(color.red()) >());
	color.setGreen(j.at("green").get< decltype(color.green()) >());
	color.setBlue(j.at("blue").get< decltype(color.blue()) >());
	color.setAlpha(j.at("alpha").get< decltype(color.alpha()) >());
}

void to_json(nlohmann::json &j, const QFont &font) {
	j = font.toString();
}

void from_json(const nlohmann::json &j, QFont &font) {
	font.fromString(j.get< QString >());
}

void to_json(nlohmann::json &j, const QRect &rect) {
	j["x"]      = rect.x();
	j["y"]      = rect.y();
	j["width"]  = rect.width();
	j["height"] = rect.height();
}

void from_json(const nlohmann::json &j, QRect &rect) {
	rect.setX(j.at("x").get< int >());
	rect.setY(j.at("y").get< int >());
	rect.setWidth(j.at("width").get< int >());
	rect.setHeight(j.at("height").get< int >());
}

void to_json(nlohmann::json &j, const QRectF &rect) {
	j["x"]      = rect.x();
	j["y"]      = rect.y();
	j["width"]  = rect.width();
	j["height"] = rect.height();
}

void from_json(const nlohmann::json &j, QRectF &rect) {
	rect.setX(j.at("x").get< qreal >());
	rect.setY(j.at("y").get< qreal >());
	rect.setWidth(j.at("width").get< qreal >());
	rect.setHeight(j.at("height").get< qreal >());
}

void to_json(nlohmann::json &j, const QSize &size) {
	j["width"]  = size.width();
	j["height"] = size.height();
}

void from_json(const nlohmann::json &j, QSize &size) {
	size.setWidth(j.at("width").get< int >());
	size.setHeight(j.at("height").get< int >());
}

void to_json(nlohmann::json &j, const QSizeF &size) {
	j["width"]  = size.width();
	j["height"] = size.height();
}

void from_json(const nlohmann::json &j, QSizeF &size) {
	size.setWidth(j.at("width").get< qreal >());
	size.setHeight(j.at("height").get< qreal >());
}
