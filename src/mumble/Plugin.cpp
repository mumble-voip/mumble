// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Plugin.h"
#include "API.h"
#include "Version.h"

#include <QMutexLocker>
#include <QWriteLocker>

#include <cstring>


// initialize the static ID counter
plugin_id_t Plugin::s_nextID = 1;
QMutex Plugin::s_idLock;

void assertPluginLoaded(const Plugin *plugin) {
	// don't throw and exception in release build
	if (!plugin->isLoaded()) {
#ifdef QT_DEBUG
		throw std::runtime_error("Attempting to access plugin but it is not loaded!");
#else
		qWarning("Plugin assertion failed: Assumed plugin with ID %d to be loaded but it wasn't!", plugin->getID());
#endif
	}
}

Plugin::Plugin(QString path, bool isBuiltIn, QObject *p)
	: QObject(p), m_lib(path), m_pluginPath(path), m_pluginIsLoaded(false), m_pluginLock(QReadWriteLock::NonRecursive),
	  m_pluginFnc(), m_isBuiltIn(isBuiltIn), m_positionalDataIsEnabled(true), m_positionalDataIsActive(false),
	  m_mayMonitorKeyboard(false) {
	// See if the plugin is loadable in the first place unless it is a built-in plugin
	m_pluginIsValid = isBuiltIn || m_lib.load();

	if (!m_pluginIsValid) {
		// throw an exception to indicate that the plugin isn't valid
		throw PluginError(
			QString::fromLatin1("Unable to load the specified library (%1)").arg(m_lib.errorString()).toStdString());
	}

	// acquire id-lock in order to assign an ID to this plugin
	QMutexLocker lock(&Plugin::s_idLock);
	m_pluginID = Plugin::s_nextID;
	Plugin::s_nextID++;
}

Plugin::~Plugin() {
	if (isLoaded()) {
		shutdown();
	}
	if (m_lib.isLoaded()) {
		m_lib.unload();
	}
}

QString Plugin::extractWrappedString(MumbleStringWrapper wrapper) const {
	QString wrappedString = QString::fromUtf8(wrapper.data, static_cast< int >(wrapper.size));

	if (wrapper.needsReleasing) {
		releaseResource(static_cast< const void * >(wrapper.data));
	}

	return wrappedString;
}

bool Plugin::doInitialize() {
	resolveFunctionPointers();

	return m_pluginIsValid;
}

void Plugin::resolveFunctionPointers() {
	if (isValid()) {
		// The corresponding library was loaded -> try to locate all API functions and provide defaults for
		// the missing ones

		QWriteLocker lock(&m_pluginLock);

		// resolve the mandatory functions first
		m_pluginFnc.init = reinterpret_cast< decltype(MumblePluginFunctions::init) >(m_lib.resolve("mumble_init"));
		m_pluginFnc.shutdown =
			reinterpret_cast< decltype(MumblePluginFunctions::shutdown) >(m_lib.resolve("mumble_shutdown"));
		m_pluginFnc.getName =
			reinterpret_cast< decltype(MumblePluginFunctions::getName) >(m_lib.resolve("mumble_getName"));
		m_pluginFnc.getAPIVersion =
			reinterpret_cast< decltype(MumblePluginFunctions::getAPIVersion) >(m_lib.resolve("mumble_getAPIVersion"));
		m_pluginFnc.registerAPIFunctions = reinterpret_cast< decltype(MumblePluginFunctions::registerAPIFunctions) >(
			m_lib.resolve("mumble_registerAPIFunctions"));
		m_pluginFnc.releaseResource = reinterpret_cast< decltype(MumblePluginFunctions::releaseResource) >(
			m_lib.resolve("mumble_releaseResource"));

		// validate that all those functions are available in the loaded lib
		m_pluginIsValid = m_pluginFnc.init && m_pluginFnc.shutdown && m_pluginFnc.getName && m_pluginFnc.getAPIVersion
						  && m_pluginFnc.registerAPIFunctions && m_pluginFnc.releaseResource;

		if (!m_pluginIsValid) {
			// Don't bother trying to resolve any other functions
#ifdef MUMBLE_PLUGIN_DEBUG
#	define CHECK_AND_LOG(name)                                                                         \
		if (!m_pluginFnc.name) {                                                                        \
			qDebug("\t\"%s\" is missing the %s() function", qPrintable(m_pluginPath), "mumble_" #name); \
		}
			CHECK_AND_LOG(init);
			CHECK_AND_LOG(shutdown);
			CHECK_AND_LOG(getName);
			CHECK_AND_LOG(getAPIVersion);
			CHECK_AND_LOG(registerAPIFunctions);
			CHECK_AND_LOG(releaseResource);
#	undef CHECK_AND_LOG
#endif

			return;
		}

		m_pluginFnc.getPluginFunctionsVersion =
			reinterpret_cast< decltype(MumblePluginFunctions::getPluginFunctionsVersion) >(
				m_lib.resolve("mumble_getPluginFunctionsVersion"));

		const mumble_version_t pluginFunctionsVersion = getPluginFunctionsVersion();

		// The mandatory functions are there, now see if any optional functions are implemented as well
		m_pluginFnc.setMumbleInfo =
			reinterpret_cast< decltype(MumblePluginFunctions::setMumbleInfo) >(m_lib.resolve("mumble_setMumbleInfo"));
		m_pluginFnc.getVersion =
			reinterpret_cast< decltype(MumblePluginFunctions::getVersion) >(m_lib.resolve("mumble_getVersion"));
		m_pluginFnc.getAuthor =
			reinterpret_cast< decltype(MumblePluginFunctions::getAuthor) >(m_lib.resolve("mumble_getAuthor"));
		m_pluginFnc.getDescription =
			reinterpret_cast< decltype(MumblePluginFunctions::getDescription) >(m_lib.resolve("mumble_getDescription"));
		m_pluginFnc.getFeatures =
			reinterpret_cast< decltype(MumblePluginFunctions::getFeatures) >(m_lib.resolve("mumble_getFeatures"));
		m_pluginFnc.deactivateFeatures = reinterpret_cast< decltype(MumblePluginFunctions::deactivateFeatures) >(
			m_lib.resolve("mumble_deactivateFeatures"));
		m_pluginFnc.initPositionalData = reinterpret_cast< decltype(MumblePluginFunctions::initPositionalData) >(
			m_lib.resolve("mumble_initPositionalData"));
		m_pluginFnc.fetchPositionalData = reinterpret_cast< decltype(MumblePluginFunctions::fetchPositionalData) >(
			m_lib.resolve("mumble_fetchPositionalData"));
		m_pluginFnc.shutdownPositionalData =
			reinterpret_cast< decltype(MumblePluginFunctions::shutdownPositionalData) >(
				m_lib.resolve("mumble_shutdownPositionalData"));
		m_pluginFnc.onServerConnected = reinterpret_cast< decltype(MumblePluginFunctions::onServerConnected) >(
			m_lib.resolve("mumble_onServerConnected"));
		m_pluginFnc.onServerDisconnected = reinterpret_cast< decltype(MumblePluginFunctions::onServerDisconnected) >(
			m_lib.resolve("mumble_onServerDisconnected"));
		m_pluginFnc.onChannelEntered = reinterpret_cast< decltype(MumblePluginFunctions::onChannelEntered) >(
			m_lib.resolve("mumble_onChannelEntered"));
		m_pluginFnc.onChannelExited = reinterpret_cast< decltype(MumblePluginFunctions::onChannelExited) >(
			m_lib.resolve("mumble_onChannelExited"));
		m_pluginFnc.onUserTalkingStateChanged =
			reinterpret_cast< decltype(MumblePluginFunctions::onUserTalkingStateChanged) >(
				m_lib.resolve("mumble_onUserTalkingStateChanged"));
		m_pluginFnc.onReceiveData =
			reinterpret_cast< decltype(MumblePluginFunctions::onReceiveData) >(m_lib.resolve("mumble_onReceiveData"));
		m_pluginFnc.onAudioInput =
			reinterpret_cast< decltype(MumblePluginFunctions::onAudioInput) >(m_lib.resolve("mumble_onAudioInput"));
		m_pluginFnc.onAudioSourceFetched = reinterpret_cast< decltype(MumblePluginFunctions::onAudioSourceFetched) >(
			m_lib.resolve("mumble_onAudioSourceFetched"));
		m_pluginFnc.onAudioOutputAboutToPlay =
			reinterpret_cast< decltype(MumblePluginFunctions::onAudioOutputAboutToPlay) >(
				m_lib.resolve("mumble_onAudioOutputAboutToPlay"));
		m_pluginFnc.onServerSynchronized = reinterpret_cast< decltype(MumblePluginFunctions::onServerSynchronized) >(
			m_lib.resolve("mumble_onServerSynchronized"));
		m_pluginFnc.onUserAdded =
			reinterpret_cast< decltype(MumblePluginFunctions::onUserAdded) >(m_lib.resolve("mumble_onUserAdded"));
		m_pluginFnc.onUserRemoved =
			reinterpret_cast< decltype(MumblePluginFunctions::onUserRemoved) >(m_lib.resolve("mumble_onUserRemoved"));
		m_pluginFnc.onChannelAdded =
			reinterpret_cast< decltype(MumblePluginFunctions::onChannelAdded) >(m_lib.resolve("mumble_onChannelAdded"));
		m_pluginFnc.onChannelRemoved = reinterpret_cast< decltype(MumblePluginFunctions::onChannelRemoved) >(
			m_lib.resolve("mumble_onChannelRemoved"));
		m_pluginFnc.onChannelRenamed = reinterpret_cast< decltype(MumblePluginFunctions::onChannelRenamed) >(
			m_lib.resolve("mumble_onChannelRenamed"));
		m_pluginFnc.onKeyEvent =
			reinterpret_cast< decltype(MumblePluginFunctions::onKeyEvent) >(m_lib.resolve("mumble_onKeyEvent"));
		m_pluginFnc.hasUpdate =
			reinterpret_cast< decltype(MumblePluginFunctions::hasUpdate) >(m_lib.resolve("mumble_hasUpdate"));
		m_pluginFnc.getUpdateDownloadURL = reinterpret_cast< decltype(MumblePluginFunctions::getUpdateDownloadURL) >(
			m_lib.resolve("mumble_getUpdateDownloadURL"));

		if (pluginFunctionsVersion >= mumble_version_t({ 1, 1, 0 })) {
			// Functions introduced with plugin functions scheme v1.1.0
			m_pluginFnc.getPositionalDataContextPrefix =
				reinterpret_cast< decltype(MumblePluginFunctions::getPositionalDataContextPrefix) >(
					m_lib.resolve("mumble_getPositionalDataContextPrefix"));
		}

#ifdef MUMBLE_PLUGIN_DEBUG
#	define CHECK_AND_LOG(name)        \
		qDebug("\t"                    \
			   "mumble_" #name ": %s", \
			   (m_pluginFnc.name == nullptr ? "no" : "yes"))
		qDebug(">>>> Found optional functions for plugin \"%s\"", qUtf8Printable(m_pluginPath));
		CHECK_AND_LOG(setMumbleInfo);
		CHECK_AND_LOG(getVersion);
		CHECK_AND_LOG(getAuthor);
		CHECK_AND_LOG(getDescription);
		CHECK_AND_LOG(getFeatures);
		CHECK_AND_LOG(deactivateFeatures);
		CHECK_AND_LOG(initPositionalData);
		CHECK_AND_LOG(fetchPositionalData);
		CHECK_AND_LOG(shutdownPositionalData);
		CHECK_AND_LOG(getPositionalDataContextPrefix);
		CHECK_AND_LOG(onServerConnected);
		CHECK_AND_LOG(onServerDisconnected);
		CHECK_AND_LOG(onChannelEntered);
		CHECK_AND_LOG(onChannelExited);
		CHECK_AND_LOG(onUserTalkingStateChanged);
		CHECK_AND_LOG(onReceiveData);
		CHECK_AND_LOG(onAudioInput);
		CHECK_AND_LOG(onAudioSourceFetched);
		CHECK_AND_LOG(onAudioOutputAboutToPlay);
		CHECK_AND_LOG(onServerSynchronized);
		CHECK_AND_LOG(onUserAdded);
		CHECK_AND_LOG(onUserRemoved);
		CHECK_AND_LOG(onChannelAdded);
		CHECK_AND_LOG(onChannelRemoved);
		CHECK_AND_LOG(onChannelRenamed);
		CHECK_AND_LOG(onKeyEvent);
		CHECK_AND_LOG(hasUpdate);
		CHECK_AND_LOG(getUpdateDownloadURL);
		qDebug("<<<<");
#endif

		// If positional audio is to be supported, all three functions (init, fetch, shutdown) have to be implemented
		// For PA it is all or nothing
		if (!(m_pluginFnc.initPositionalData && m_pluginFnc.fetchPositionalData && m_pluginFnc.shutdownPositionalData)
			&& (m_pluginFnc.initPositionalData || m_pluginFnc.fetchPositionalData
				|| m_pluginFnc.shutdownPositionalData)) {
			m_pluginFnc.initPositionalData     = nullptr;
			m_pluginFnc.fetchPositionalData    = nullptr;
			m_pluginFnc.shutdownPositionalData = nullptr;
#ifdef MUMBLE_PLUGIN_DEBUG
			qDebug("\t\"%s\" has only partially implemented positional data functions -> deactivating all of them",
				   qPrintable(m_pluginPath));
#endif
		}
	}
}

bool Plugin::isValid() const {
	PluginReadLocker lock(&m_pluginLock);

	return m_pluginIsValid;
}

bool Plugin::isLoaded() const {
	PluginReadLocker lock(&m_pluginLock);

	return m_pluginIsLoaded;
}

plugin_id_t Plugin::getID() const {
	PluginReadLocker lock(&m_pluginLock);

	return m_pluginID;
}

bool Plugin::isBuiltInPlugin() const {
	PluginReadLocker lock(&m_pluginLock);

	return m_isBuiltIn;
}

QString Plugin::getFilePath() const {
	PluginReadLocker lock(&m_pluginLock);

	return m_pluginPath;
}

bool Plugin::isPositionalDataEnabled() const {
	PluginReadLocker lock(&m_pluginLock);

	return m_positionalDataIsEnabled;
}

void Plugin::enablePositionalData(bool enable) {
	QWriteLocker lock(&m_pluginLock);

	m_positionalDataIsEnabled = enable;
}

bool Plugin::isPositionalDataActive() const {
	PluginReadLocker lock(&m_pluginLock);

	return m_positionalDataIsActive;
}

void Plugin::allowKeyboardMonitoring(bool allow) {
	QWriteLocker lock(&m_pluginLock);

	m_mayMonitorKeyboard = allow;
}

bool Plugin::isKeyboardMonitoringAllowed() const {
	PluginReadLocker lock(&m_pluginLock);

	return m_mayMonitorKeyboard;
}

mumble_error_t Plugin::init() {
	{
		QReadLocker lock(&m_pluginLock);

		if (m_pluginIsLoaded) {
			return MUMBLE_STATUS_OK;
		}
	}

	//////////////////////////////
	// Step 1: Introduce ourselves (inform the plugin about Mumble's (API) version

	// Get Mumble version
	Version::component_t mumbleMajor, mumbleMinor, mumblePatch;
	Version::getComponents(mumbleMajor, mumbleMinor, mumblePatch);

	// Require API version 1.0.0 as the minimal supported one
	setMumbleInfo({ mumbleMajor, mumbleMinor, mumblePatch }, MUMBLE_PLUGIN_API_VERSION, { 1, 0, 0 });


	//////////////////////////////
	// Step 2: Provide the API functions to the plugin
	const mumble_version_t apiVersion = getAPIVersion();
	if (apiVersion >= mumble_version_t({ 1, 0, 0 }) && apiVersion < mumble_version_t({ 1, 2, 0 })) {
		MumbleAPI_v_1_0_x api = API::getMumbleAPI_v_1_0_x();
		registerAPIFunctions(&api);
	} else if (apiVersion >= mumble_version_t({ 1, 2, 0 }) && apiVersion < mumble_version_t({ 1, 3, 0 })) {
		MumbleAPI_v_1_2_x api = API::getMumbleAPI_v_1_2_x();
		registerAPIFunctions(&api);
	} else {
		// The API version could not be obtained -> this is an invalid plugin that shouldn't have been loaded in the
		// first place
		qWarning("Unable to obtain requested MumbleAPI version");
		return MUMBLE_EC_INVALID_API_VERSION;
	}


	//////////////////////////////
	// Step 3: Actually try to load the plugin

	mumble_error_t retStatus;
	if (m_pluginFnc.init) {
		retStatus = m_pluginFnc.init(m_pluginID);
	} else {
		retStatus = MUMBLE_EC_GENERIC_ERROR;
	}

	{
		QWriteLocker lock(&m_pluginLock);
		m_pluginIsLoaded = retStatus == MUMBLE_STATUS_OK;
	}

	return retStatus;
}

void Plugin::shutdown() {
	bool posDataActive;
	{
		QReadLocker rLock(&m_pluginLock);
		if (!m_pluginIsLoaded) {
			return;
		}

		posDataActive = m_positionalDataIsActive;
	}

	if (posDataActive) {
		shutdownPositionalData();
	}

	if (m_pluginFnc.shutdown) {
		m_pluginFnc.shutdown();
	}

	{
		QWriteLocker lock(&m_pluginLock);

		m_pluginIsLoaded = false;
	}
}

QString Plugin::getName() const {
	if (m_pluginFnc.getName) {
		return extractWrappedString(m_pluginFnc.getName());
	} else {
		return QString::fromLatin1("Unknown plugin");
	}
}

mumble_version_t Plugin::getAPIVersion() const {
	if (m_pluginFnc.getAPIVersion) {
		return m_pluginFnc.getAPIVersion();
	} else {
		return MUMBLE_VERSION_UNKNOWN;
	}
}

mumble_version_t Plugin::getPluginFunctionsVersion() const {
	if (m_pluginFnc.getPluginFunctionsVersion) {
		return m_pluginFnc.getPluginFunctionsVersion();
	} else {
		return mumble_version_t({ 1, 0, 0 });
	}
}

void Plugin::registerAPIFunctions(void *api) const {
	if (m_pluginFnc.registerAPIFunctions) {
		m_pluginFnc.registerAPIFunctions(api);
	}
}

void Plugin::releaseResource(const void *pointer) const {
	if (m_pluginFnc.releaseResource) {
		m_pluginFnc.releaseResource(pointer);
	}
}

void Plugin::setMumbleInfo(mumble_version_t mumbleVersion, mumble_version_t mumbleAPIVersion,
						   mumble_version_t minimalExpectedAPIVersion) const {
	if (m_pluginFnc.setMumbleInfo) {
		m_pluginFnc.setMumbleInfo(mumbleVersion, mumbleAPIVersion, minimalExpectedAPIVersion);
	}
}

mumble_version_t Plugin::getVersion() const {
	if (m_pluginFnc.getVersion) {
		return m_pluginFnc.getVersion();
	} else {
		return MUMBLE_VERSION_UNKNOWN;
	}
}

QString Plugin::getAuthor() const {
	if (m_pluginFnc.getAuthor) {
		return extractWrappedString(m_pluginFnc.getAuthor());
	} else {
		return QString::fromLatin1("Unknown");
	}
}

QString Plugin::getDescription() const {
	if (m_pluginFnc.getDescription) {
		return extractWrappedString(m_pluginFnc.getDescription());
	} else {
		return QString::fromLatin1("No description provided");
	}
}

uint32_t Plugin::getFeatures() const {
	if (m_pluginFnc.getFeatures) {
		return m_pluginFnc.getFeatures();
	} else {
		return MUMBLE_FEATURE_NONE;
	}
}

uint32_t Plugin::deactivateFeatures(uint32_t features) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.deactivateFeatures) {
		return m_pluginFnc.deactivateFeatures(features);
	} else {
		return features;
	}
}

bool Plugin::showAboutDialog(QWidget *parent) const {
	assertPluginLoaded(this);

	Q_UNUSED(parent);
	return false;
}

bool Plugin::showConfigDialog(QWidget *parent) const {
	assertPluginLoaded(this);

	Q_UNUSED(parent);
	return false;
}

uint8_t Plugin::initPositionalData(const char *const *programNames, const uint64_t *programPIDs, size_t programCount) {
	assertPluginLoaded(this);

	if (m_pluginFnc.initPositionalData) {
		uint8_t returnCode = m_pluginFnc.initPositionalData(programNames, programPIDs, programCount);

		{
			QWriteLocker lock(&m_pluginLock);
			m_positionalDataIsActive = returnCode == MUMBLE_PDEC_OK;
		}

		return returnCode;
	} else {
		return MUMBLE_PDEC_ERROR_PERM;
	}
}

bool Plugin::fetchPositionalData(Position3D &avatarPos, Vector3D &avatarDir, Vector3D &avatarAxis,
								 Position3D &cameraPos, Vector3D &cameraDir, Vector3D &cameraAxis, QString &context,
								 QString &identity) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.fetchPositionalData) {
		const char *contextPtr  = "";
		const char *identityPtr = "";

		bool retStatus = m_pluginFnc.fetchPositionalData(
			static_cast< float * >(avatarPos), static_cast< float * >(avatarDir), static_cast< float * >(avatarAxis),
			static_cast< float * >(cameraPos), static_cast< float * >(cameraDir), static_cast< float * >(cameraAxis),
			&contextPtr, &identityPtr);

		context  = QString::fromUtf8(contextPtr);
		identity = QString::fromUtf8(identityPtr);

		return retStatus;
	} else {
		avatarPos.toZero();
		avatarDir.toZero();
		avatarAxis.toZero();
		cameraPos.toZero();
		cameraDir.toZero();
		cameraAxis.toZero();
		context  = QString();
		identity = QString();

		return false;
	}
}

void Plugin::shutdownPositionalData() {
	assertPluginLoaded(this);

	if (m_pluginFnc.shutdownPositionalData) {
		m_positionalDataIsActive = false;

		m_pluginFnc.shutdownPositionalData();
	}
}

QString Plugin::getPositionalDataContextPrefix() const {
	assertPluginLoaded(this);

	if (m_pluginFnc.getPositionalDataContextPrefix) {
		return extractWrappedString(m_pluginFnc.getPositionalDataContextPrefix());
	} else {
		// Use plugin's name as default context prefix
		return getName();
	}
}

void Plugin::onServerConnected(mumble_connection_t connection) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onServerConnected) {
		m_pluginFnc.onServerConnected(connection);
	}
}

void Plugin::onServerDisconnected(mumble_connection_t connection) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onServerDisconnected) {
		m_pluginFnc.onServerDisconnected(connection);
	}
}

void Plugin::onChannelEntered(mumble_connection_t connection, mumble_userid_t userID,
							  mumble_channelid_t previousChannelID, mumble_channelid_t newChannelID) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onChannelEntered) {
		m_pluginFnc.onChannelEntered(connection, userID, previousChannelID, newChannelID);
	}
}

void Plugin::onChannelExited(mumble_connection_t connection, mumble_userid_t userID,
							 mumble_channelid_t channelID) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onChannelExited) {
		m_pluginFnc.onChannelExited(connection, userID, channelID);
	}
}

void Plugin::onUserTalkingStateChanged(mumble_connection_t connection, mumble_userid_t userID,
									   mumble_talking_state_t talkingState) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onUserTalkingStateChanged) {
		m_pluginFnc.onUserTalkingStateChanged(connection, userID, talkingState);
	}
}

bool Plugin::onReceiveData(mumble_connection_t connection, mumble_userid_t sender, const uint8_t *data,
						   size_t dataLength, const char *dataID) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onReceiveData) {
		return m_pluginFnc.onReceiveData(connection, sender, data, dataLength, dataID);
	} else {
		return false;
	}
}

bool Plugin::onAudioInput(short *inputPCM, uint32_t sampleCount, uint16_t channelCount, uint32_t sampleRate,
						  bool isSpeech) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onAudioInput) {
		return m_pluginFnc.onAudioInput(inputPCM, sampleCount, channelCount, sampleRate, isSpeech);
	} else {
		return false;
	}
}

bool Plugin::onAudioSourceFetched(float *outputPCM, uint32_t sampleCount, uint16_t channelCount, uint32_t sampleRate,
								  bool isSpeech, mumble_userid_t userID) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onAudioSourceFetched) {
		return m_pluginFnc.onAudioSourceFetched(outputPCM, sampleCount, channelCount, sampleRate, isSpeech, userID);
	} else {
		return false;
	}
}

bool Plugin::onAudioOutputAboutToPlay(float *outputPCM, uint32_t sampleCount, uint16_t channelCount,
									  uint32_t sampleRate) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onAudioOutputAboutToPlay) {
		return m_pluginFnc.onAudioOutputAboutToPlay(outputPCM, sampleCount, channelCount, sampleRate);
	} else {
		return false;
	}
}

void Plugin::onServerSynchronized(mumble_connection_t connection) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onServerSynchronized) {
		m_pluginFnc.onServerSynchronized(connection);
	}
}

void Plugin::onUserAdded(mumble_connection_t connection, mumble_userid_t userID) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onUserAdded) {
		m_pluginFnc.onUserAdded(connection, userID);
	}
}

void Plugin::onUserRemoved(mumble_connection_t connection, mumble_userid_t userID) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onUserRemoved) {
		m_pluginFnc.onUserRemoved(connection, userID);
	}
}

void Plugin::onChannelAdded(mumble_connection_t connection, mumble_channelid_t channelID) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onChannelAdded) {
		m_pluginFnc.onChannelAdded(connection, channelID);
	}
}

void Plugin::onChannelRemoved(mumble_connection_t connection, mumble_channelid_t channelID) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onChannelRemoved) {
		m_pluginFnc.onChannelRemoved(connection, channelID);
	}
}

void Plugin::onChannelRenamed(mumble_connection_t connection, mumble_channelid_t channelID) const {
	assertPluginLoaded(this);

	if (m_pluginFnc.onChannelRenamed) {
		m_pluginFnc.onChannelRenamed(connection, channelID);
	}
}

void Plugin::onKeyEvent(mumble_keycode_t keyCode, bool wasPress) const {
	assertPluginLoaded(this);

	if (!m_mayMonitorKeyboard) {
		// Keyboard monitoring is forbidden for this plugin
		return;
	}

	if (m_pluginFnc.onKeyEvent) {
		m_pluginFnc.onKeyEvent(static_cast< std::uint32_t >(keyCode), wasPress);
	}
}

bool Plugin::hasUpdate() const {
	if (m_pluginFnc.hasUpdate) {
		return m_pluginFnc.hasUpdate();
	} else {
		// A plugin that doesn't implement this function is assumed to never know about
		// any potential updates
		return false;
	}
}

QUrl Plugin::getUpdateDownloadURL() const {
	if (m_pluginFnc.getUpdateDownloadURL) {
		return QUrl(extractWrappedString(m_pluginFnc.getUpdateDownloadURL()));
	} else {
		// Return an empty URL as a fallback
		return QUrl();
	}
}

bool Plugin::providesAboutDialog() const {
	return false;
}

bool Plugin::providesConfigDialog() const {
	return false;
}



/////////////////// Implementation of the PluginReadLocker /////////////////////////
PluginReadLocker::PluginReadLocker(QReadWriteLock *lock) : m_lock(lock), m_unlocked(false) {
	relock();
}

void PluginReadLocker::unlock() {
	if (!m_lock) {
		// do nothgin for nullptr
		return;
	}

	m_unlocked = true;

	m_lock->unlock();
}

void PluginReadLocker::relock() {
	if (!m_lock) {
		// do nothing for a nullptr
		return;
	}

	// First try to lock for read-access
	if (!m_lock->tryLockForRead()) {
		// if that fails, we'll try to lock for write-access
		// That will only succeed in the case that the current thread holds the write-access to this lock already which
		// caused the previous attempt to lock for reading to fail (by design of the QtReadWriteLock). As we are in the
		// thread with the write-access, it means that this threads has asked for read-access on top of it which we will
		// grant (in contrast of QtReadLocker) because if you have the permission to change something you surely should
		// have permission to read it. This assumes that the thread won't try to read data it temporarily has corrupted.
		if (!m_lock->tryLockForWrite()) {
			// If we couldn't lock for write at this point, it means another thread has write-access granted by the lock
			// so we'll have to wait in order to gain regular read-access as would be with QtReadLocker
			m_lock->lockForRead();
		}
	}

	m_unlocked = false;
}

PluginReadLocker::~PluginReadLocker() {
	if (m_lock && !m_unlocked) {
		// unlock the lock if it isn't nullptr
		m_lock->unlock();
	}
}
