// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "LegacyPlugin.h"

#define MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS
#include "MumblePlugin.h"
#undef MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS

#include <cstdlib>
#include <map>
#include <string.h>
#include <wchar.h>

#include <QRegularExpression>

#include <utf8/cpp11.h>

/// A regular expression used to extract the version from the legacy plugin's description
static const QRegularExpression
	versionRegEx(QString::fromLatin1("(?:v)?(?:ersion)?[ \\t]*(\\d+)\\.(\\d+)(?:\\.(\\d+))?"),
				 QRegularExpression::CaseInsensitiveOption);


LegacyPlugin::LegacyPlugin(QString path, bool isBuiltIn, QObject *p)
	: Plugin(path, isBuiltIn, p), m_name(), m_description(), m_version(MUMBLE_VERSION_UNKNOWN), m_mumPlug(0),
	  m_mumPlug2(0), m_mumPlugQt(0) {
}

LegacyPlugin::~LegacyPlugin() {
}

bool LegacyPlugin::doInitialize() {
	if (Plugin::doInitialize()) {
		// initialization seems to have succeeded so far
		// This means that mumPlug is initialized

		m_name = QString::fromStdWString(m_mumPlug->shortname);
		// Although the MumblePlugin struct has a member called "description", the actual description seems to
		// always only be returned by the longdesc function (The description member is actually just the name with some
		// version info)
		m_description = QString::fromStdWString(m_mumPlug->longdesc());
		// The version field in the MumblePlugin2 struct is the positional-audio-plugin-API version and not the version
		// of the plugin itself. This information is not provided for legacy plugins.
		// Most of them however provide information about the version of the game they support. Thus we will try to
		// parse the description and extract this version using it for the plugin's version as well. Some plugins have
		// the version in the actual description field of the old API (see above comment why these aren't the same) so
		// we will use a combination of both to search for the version. If multiple version(-like) strings are found,
		// the last one will be used.
		QString matchContent = m_description + QChar::Null + QString::fromStdWString(m_mumPlug->description);
		QRegularExpressionMatchIterator matchIt = versionRegEx.globalMatch(matchContent);

		// Only consider the last match
		QRegularExpressionMatch match;
		while (matchIt.hasNext()) {
			match = matchIt.next();
		}

		if (match.hasMatch()) {
			// Store version
			m_version = { match.captured(1).toInt(), match.captured(2).toInt(), match.captured(3).toInt() };
		}

		return true;
	} else {
		// initialization has failed
		// pass on info about failed init
		return false;
	}
}

void LegacyPlugin::resolveFunctionPointers() {
	// We don't set any functions inside the apiFnc struct variable in order for the default
	// implementations in the Plugin class to mimic empty default implementations for all functions
	// not explicitly overwritten by this class

	if (isValid()) {
		// The corresponding library was loaded -> try to locate all API functions of the legacy plugin's spec
		// (for positional audio) and set defaults for the other ones in order to maintain compatibility with
		// the new plugin system

		QWriteLocker lock(&m_pluginLock);

		mumblePluginFunc pluginFunc     = reinterpret_cast< mumblePluginFunc >(m_lib.resolve("getMumblePlugin"));
		mumblePlugin2Func plugin2Func   = reinterpret_cast< mumblePlugin2Func >(m_lib.resolve("getMumblePlugin2"));
		mumblePluginQtFunc pluginQtFunc = reinterpret_cast< mumblePluginQtFunc >(m_lib.resolve("getMumblePluginQt"));

		if (pluginFunc) {
			m_mumPlug = pluginFunc();
		}
		if (plugin2Func) {
			m_mumPlug2 = plugin2Func();
		}
		if (pluginQtFunc) {
			m_mumPlugQt = pluginQtFunc();
		}

		// A legacy plugin is valid as long as there is a function to get the MumblePlugin struct from it
		// and the plugin has been compiled by the same compiler as this client (determined by the plugin's
		// "magic") and it isn't retracted
		bool suitableMagic = m_mumPlug && m_mumPlug->magic == MUMBLE_PLUGIN_MAGIC;
		bool retracted     = m_mumPlug && m_mumPlug->shortname == L"Retracted";
		m_pluginIsValid    = pluginFunc && suitableMagic && !retracted;

#ifdef MUMBLE_PLUGIN_DEBUG
		if (!m_pluginIsValid) {
			if (!pluginFunc) {
				qDebug("Plugin \"%s\" is missing the getMumblePlugin() function", qPrintable(m_pluginPath));
			} else if (!suitableMagic) {
				qDebug("Plugin \"%s\" was compiled with a different compiler (magic differs)",
					   qPrintable(m_pluginPath));
			} else {
				qDebug("Plugin \"%s\" is retracted", qPrintable(m_pluginPath));
			}
		}
#endif
	}
}

mumble_error_t LegacyPlugin::init() {
	{
		QWriteLocker lock(&m_pluginLock);

		m_pluginIsLoaded = true;
	}

	// No-op as legacy plugins never have anything to initialize
	// The only init function they care about is the one that inits positional audio
	return MUMBLE_STATUS_OK;
}

QString LegacyPlugin::getName() const {
	PluginReadLocker lock(&m_pluginLock);

	if (!m_name.isEmpty()) {
		return m_name;
	} else {
		return QString::fromLatin1("<Unknown Legacy Plugin>");
	}
}

QString LegacyPlugin::getDescription() const {
	PluginReadLocker lock(&m_pluginLock);

	if (!m_description.isEmpty()) {
		return m_description;
	} else {
		return QString::fromLatin1("<No description provided by the legacy plugin>");
	}
}

bool LegacyPlugin::showAboutDialog(QWidget *parent) const {
	if (m_mumPlugQt && m_mumPlugQt->about) {
		m_mumPlugQt->about(parent);

		return true;
	}
	if (m_mumPlug->about) {
		// the original implementation in Mumble would pass nullptr to the about-function in the mumPlug struct
		// so we'll mimic that behaviour for compatibility
		m_mumPlug->about(nullptr);

		return true;
	}

	return false;
}

bool LegacyPlugin::showConfigDialog(QWidget *parent) const {
	if (m_mumPlugQt && m_mumPlugQt->config) {
		m_mumPlugQt->config(parent);

		return true;
	}
	if (m_mumPlug->config) {
		// the original implementation in Mumble would pass nullptr to the about-function in the mumPlug struct
		// so we'll mimic that behaviour for compatibility
		m_mumPlug->config(nullptr);

		return true;
	}

	return false;
}

uint8_t LegacyPlugin::initPositionalData(const char *const *programNames, const uint64_t *programPIDs,
										 size_t programCount) {
	int retCode;

	if (m_mumPlug2) {
		// Create and populate a multimap holding the names and PIDs to pass to the tryLock-function
		std::multimap< std::wstring, unsigned long long int > pidMap;

		for (size_t i = 0; i < programCount; i++) {
			const std::string currentName = programNames[i];

			try {
				switch (sizeof(wchar_t)) {
					case 2: {
						const std::u16string str16 = utf8::utf8to16(currentName);
						pidMap.insert({ { str16.cbegin(), str16.cend() }, programPIDs[i] });
						break;
					}
					case 4: {
						const std::u32string str32 = utf8::utf8to32(currentName);
						pidMap.insert({ { str32.cbegin(), str32.cend() }, programPIDs[i] });
						break;
					}
					default:
						return MUMBLE_PDEC_ERROR_PERM;
				}
			} catch (const utf8::invalid_utf8 &) {
				continue;
			}
		}

		retCode = m_mumPlug2->trylock(pidMap);
	} else {
		// The default MumblePlugin doesn't take the name and PID arguments
		retCode = m_mumPlug->trylock();
	}

	// ensure that only expected return codes are being returned from this function
	// the legacy plugins return 1 on successful locking and 0 on failure
	if (retCode) {
		QWriteLocker wLock(&m_pluginLock);

		m_positionalDataIsActive = true;

		return MUMBLE_PDEC_OK;
	} else {
		// legacy plugins don't have the concept of indicating a permanent error
		// so we'll return a temporary error for them
		return MUMBLE_PDEC_ERROR_TEMP;
	}
}

bool LegacyPlugin::fetchPositionalData(Position3D &avatarPos, Vector3D &avatarDir, Vector3D &avatarAxis,
									   Position3D &cameraPos, Vector3D &cameraDir, Vector3D &cameraAxis,
									   QString &context, QString &identity) const {
	std::wstring identityWstr;
	std::string contextStr;

	int retCode = m_mumPlug->fetch(static_cast< float * >(avatarPos), static_cast< float * >(avatarDir),
								   static_cast< float * >(avatarAxis), static_cast< float * >(cameraPos),
								   static_cast< float * >(cameraDir), static_cast< float * >(cameraAxis), contextStr,
								   identityWstr);

	context  = QString::fromStdString(contextStr);
	identity = QString::fromStdWString(identityWstr);

	// The fetch-function should return if it is "still locked on" meaning that it can continue providing
	// positional audio
	return retCode == 1;
}

void LegacyPlugin::shutdownPositionalData() {
	QWriteLocker lock(&m_pluginLock);

	m_positionalDataIsActive = false;

	m_mumPlug->unlock();
}

uint32_t LegacyPlugin::getFeatures() const {
	return MUMBLE_FEATURE_POSITIONAL;
}

mumble_version_t LegacyPlugin::getVersion() const {
	return m_version;
}

bool LegacyPlugin::providesAboutDialog() const {
	return m_mumPlug->about || (m_mumPlugQt && m_mumPlugQt->about);
}

bool LegacyPlugin::providesConfigDialog() const {
	return m_mumPlug->config || (m_mumPlugQt && m_mumPlugQt->config);
}

mumble_version_t LegacyPlugin::getAPIVersion() const {
	// Legacy plugins are always on most recent API as they don't use it in any case -> no need to perform
	// backwards compatibility stuff
	return MUMBLE_PLUGIN_API_VERSION;
}
