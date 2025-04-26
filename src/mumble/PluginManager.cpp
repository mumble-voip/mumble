// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <limits>

#include "LegacyPlugin.h"
#include "PluginManager.h"
#include <QByteArray>
#include <QChar>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QHashIterator>
#include <QKeyEvent>
#include <QMutexLocker>
#include <QReadLocker>
#include <QTimer>
#include <QVector>
#include <QWriteLocker>

#include "API.h"
#include "Log.h"
#include "PluginInstaller.h"
#include "PluginUpdater.h"
#include "ProcessResolver.h"
#include "ServerHandler.h"
#include "Global.h"

#ifdef USE_MANUAL_PLUGIN
#	include "ManualPlugin.h"
#endif

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#ifdef Q_OS_WIN
#	include <tlhelp32.h>
#	include <string>
#endif

#ifdef Q_OS_LINUX
#	include <QtCore/QStringList>
#endif

PluginManager::PluginManager(QSet< QString > *additionalSearchPaths, QObject *p)
	: QObject(p), m_pluginCollectionLock(QReadWriteLock::NonRecursive), m_pluginHashMap(), m_positionalData(),
	  m_positionalDataCheckTimer(), m_sentDataMutex(), m_sentData(),
	  m_activePosDataPluginLock(QReadWriteLock::NonRecursive), m_activePositionalDataPlugin(), m_updater() {
	qRegisterMetaType< mumble_plugin_id_t >("mumble_plugin_id_t");

	std::vector< QString > pluginPaths;

	// Setup search-paths
	if (additionalSearchPaths) {
		pluginPaths.insert(pluginPaths.end(), additionalSearchPaths->begin(), additionalSearchPaths->end());
	}

#ifdef Q_OS_MAC
	// Path to plugins inside AppBundle
	pluginPaths.push_back(QString::fromLatin1("%1/../Plugins").arg(qApp->applicationDirPath()));
#endif

#ifdef MUMBLE_PLUGIN_PATH
	// Path to where plugins are/will be installed on the system
	pluginPaths.push_back(QString::fromLatin1(MUMTEXT(MUMBLE_PLUGIN_PATH)));
#endif

	// Path to "plugins" dir right next to the executable's location. This is the case for when Mumble
	// is run after compilation without having installed it anywhere special
	pluginPaths.push_back(
		QString::fromLatin1("%1/plugins").arg(MumbleApplication::instance()->applicationVersionRootPath()));

	// Path to where the plugin installer will write plugins
	pluginPaths.push_back(PluginInstaller::getInstallDir());

	for (const QString &currentPath : pluginPaths) {
		// Transform currentPath to an absolute, canonical path and only then add it to m_pluginSearchPaths in order
		// to ensure that each path is contained only once.
		QDir dir(currentPath);

		if (dir.exists()) {
			m_pluginSearchPaths.insert(dir.canonicalPath());
		}
	}

#ifdef Q_OS_WIN
	// According to MS KB Q131065, we need this to OpenProcess()

	m_hToken = nullptr;

	if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &m_hToken)) {
		if (GetLastError() == ERROR_NO_TOKEN) {
			ImpersonateSelf(SecurityImpersonation);
			OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &m_hToken);
		}
	}

	TOKEN_PRIVILEGES tp;
	LUID luid;
	m_cbPrevious = sizeof(TOKEN_PRIVILEGES);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Luid       = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(m_hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &m_tpPrevious, &m_cbPrevious);
#endif

	// Synchronize the positional data in a regular interval
	// By making this the parent of the created timer, we don't have to delete it explicitly
	QTimer *serverSyncTimer = new QTimer(this);
	QObject::connect(serverSyncTimer, &QTimer::timeout, this, &PluginManager::on_syncPositionalData);
	serverSyncTimer->start(POSITIONAL_SERVER_SYNC_INTERVAL);

	// Install this manager as a global eventFilter in order to get notified about all keypresses
	if (QCoreApplication::instance()) {
		QCoreApplication::instance()->installEventFilter(this);
	}

	// Set up the timer for regularly checking for available positional data plugins
	m_positionalDataCheckTimer.setInterval(POSITIONAL_DATA_CHECK_INTERVAL);
	m_positionalDataCheckTimer.start();
	QObject::connect(&m_positionalDataCheckTimer, &QTimer::timeout, this,
					 &PluginManager::checkForAvailablePositionalDataPlugin);

	QObject::connect(&m_updater, &PluginUpdater::updatesAvailable, this, &PluginManager::on_updatesAvailable);
	QObject::connect(this, &PluginManager::keyEvent, this, &PluginManager::on_keyEvent);
	QObject::connect(this, &PluginManager::pluginLostLink, this, &PluginManager::reportLostLink);
	QObject::connect(this, &PluginManager::pluginLinked, this, &PluginManager::reportPluginLinked);
	QObject::connect(this, &PluginManager::pluginEncounteredPermanentError, this, &PluginManager::reportPermanentError);
}

PluginManager::~PluginManager() {
	clearPlugins();

#ifdef Q_OS_WIN
	AdjustTokenPrivileges(m_hToken, FALSE, &m_tpPrevious, m_cbPrevious, NULL, NULL);
	CloseHandle(m_hToken);
#endif
}

bool PluginManager::eventFilter(QObject *target, QEvent *event) {
	if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
		static QVector< QKeyEvent * > processedEvents;

		QKeyEvent *kEvent = static_cast< QKeyEvent * >(event);

		// We have to keep track of which events we have processed already as
		// the same event might be sent to multiple targets and since this is
		// installed as a global event filter, we get notified about each of
		// them. However we want to process each event only once.
		if (!kEvent->isAutoRepeat() && !processedEvents.contains(kEvent)) {
			// Fire event
			emit keyEvent(static_cast< unsigned int >(kEvent->key()), kEvent->modifiers(),
						  kEvent->type() == QEvent::KeyPress);

			processedEvents << kEvent;

			if (processedEvents.size() == 1) {
				// Make sure to clear the list of processed events after each iteration
				// of the event loop (we don't want to let the vector grow to infinity
				// over time. Firing the timer only when the size of processedEvents is
				// exactly 1, we avoid adding multiple timers in a single iteration.
				QTimer::singleShot(0, []() { processedEvents.clear(); });
			}
		}
	}

	// standard event processing
	return QObject::eventFilter(target, event);
}

void PluginManager::unloadPlugins() {
	QReadLocker lock(&m_pluginCollectionLock);

	auto it = m_pluginHashMap.begin();

	while (it != m_pluginHashMap.end()) {
		unloadPlugin(*it.value());
		it++;
	}
}

void PluginManager::clearPlugins() {
	// Unload plugins so that they aren't implicitly unloaded once they go out of scope after having been
	// removed from the pluginHashMap.
	// This could lead to one of the plugins making an API call in its shutdown function which then would try
	// to verify the plugin's ID. For that it'll ask this PluginManager for a plugin with that ID. To check
	// that it will have to acquire a read-lock for the pluginHashMap which is impossible after we acquire the
	// write-lock in this function leading to a deadlock.
	unloadPlugins();

	QWriteLocker lock(&m_pluginCollectionLock);

	// Clear the list itself
	m_pluginHashMap.clear();
}

bool PluginManager::selectActivePositionalDataPlugin() {
	QReadLocker pluginLock(&m_pluginCollectionLock);
	QWriteLocker activePluginLock(&m_activePosDataPluginLock);

	if (!Global::get().s.bTransmitPosition) {
		// According to the settings the position shall not be transmitted meaning that we don't have to select any
		// plugin for positional data
		m_activePositionalDataPlugin = nullptr;

		return false;
	}

	const ProcessResolver procRes(true);
	const ProcessResolver::ProcessMap &map = procRes.getProcessMap();

	// We require 2 separate arrays holding the names and the PIDs -> create them from the given map
	std::vector< uint64_t > pids;
	std::vector< const char * > names;
	pids.reserve(procRes.amountOfProcesses());
	names.reserve(procRes.amountOfProcesses());
	for (const std::pair< const uint64_t, std::unique_ptr< char[] > > &currentEntry : map) {
		pids.push_back(currentEntry.first);
		names.push_back(currentEntry.second.get());
	}

	auto it = m_pluginHashMap.begin();

	// We assume that there is only one (enabled) plugin for the currently played game so we don't have to remember
	// which plugin was active last
	while (it != m_pluginHashMap.end()) {
		plugin_ptr_t currentPlugin = it.value();

		if (currentPlugin->isPositionalDataEnabled() && currentPlugin->isLoaded()) {
			switch (currentPlugin->initPositionalData(names.data(), pids.data(), procRes.amountOfProcesses())) {
				case MUMBLE_PDEC_OK:
					// the plugin is ready to provide positional data
					m_activePositionalDataPlugin = currentPlugin;

					emit pluginLinked(currentPlugin->getID());

					return true;

				case MUMBLE_PDEC_ERROR_PERM:
					// the plugin encountered a permanent error -> disable it
					emit pluginEncounteredPermanentError(currentPlugin->getID());

					currentPlugin->enablePositionalData(false);
					break;

				case MUMBLE_PDEC_ERROR_TEMP:
					// The plugin encountered a temporary error -> skip it for now (that is: do nothing)
					break;
			}
		}

		it++;
	}

	m_activePositionalDataPlugin = nullptr;

	return false;
}

#define LOG_FOUND(plugin, path, legacyStr)                                                                       \
	qDebug("Found %splugin '%s' at \"%s\"", legacyStr, qUtf8Printable(plugin->getName()), qUtf8Printable(path)); \
	qDebug() << "Its description:" << qUtf8Printable(plugin->getDescription())
#define LOG_FOUND_PLUGIN(plugin, path) LOG_FOUND(plugin, path, "")
#define LOG_FOUND_LEGACY_PLUGIN(plugin, path) LOG_FOUND(plugin, path, "legacy ")
#define LOG_FOUND_BUILTIN(plugin) LOG_FOUND(plugin, QString::fromLatin1("<builtin>"), "built-in ")
void PluginManager::rescanPlugins() {
	clearPlugins();

	{
		QWriteLocker lock(&m_pluginCollectionLock);

		// iterate over all files in the respective directories and try to construct a plugin from them
		for (const auto &currentPath : m_pluginSearchPaths) {
			QFileInfoList currentList = QDir(currentPath).entryInfoList();

			for (int k = 0; k < currentList.size(); k++) {
				QFileInfo currentInfo = currentList[k];

				if (!QLibrary::isLibrary(currentInfo.absoluteFilePath())) {
					// consider only files that actually could be libraries
					continue;
				}

				try {
					plugin_ptr_t p(Plugin::createNew< Plugin >(currentInfo.absoluteFilePath()));

#ifdef MUMBLE_PLUGIN_DEBUG
					LOG_FOUND_PLUGIN(p, currentInfo.absoluteFilePath());
#endif

					// if this code block is reached, the plugin was instantiated successfully so we can add it to the
					// map
					m_pluginHashMap.insert(p->getID(), p);
				} catch (const PluginError &e) {
					// If an exception is thrown, this library does not represent a proper plugin
					// Check if it might be a legacy plugin instead
					try {
						legacy_plugin_ptr_t lp(Plugin::createNew< LegacyPlugin >(currentInfo.absoluteFilePath()));

#ifdef MUMBLE_PLUGIN_DEBUG
						LOG_FOUND_LEGACY_PLUGIN(lp, currentInfo.absoluteFilePath());
#endif
						m_pluginHashMap.insert(lp->getID(), lp);
					} catch (const PluginError &el) {
						Q_UNUSED(el);

						// At the time this function is running the MainWindow is not necessarily created yet, so we
						// can't use the normal Log::log function
						Log::logOrDefer(Log::Warning, tr("Non-plugin found in plugin directory: \"%1\" (%2)")
														  .arg(currentInfo.absoluteFilePath())
														  .arg(QString::fromUtf8(e.what())));
					}
				}
			}
		}

		// handle built-in plugins
#ifdef USE_MANUAL_PLUGIN
		try {
			std::shared_ptr< ManualPlugin > mp(Plugin::createNew< ManualPlugin >());

			m_pluginHashMap.insert(mp->getID(), mp);
#	ifdef MUMBLE_PLUGIN_DEBUG
			LOG_FOUND_BUILTIN(mp);
#	endif
		} catch (const PluginError &e) {
			// At the time this function is running the MainWindow is not necessarily created yet, so we can't use
			// the normal Log::log function
			Log::logOrDefer(Log::Warning, tr("Failed at loading manual plugin: %1").arg(QString::fromUtf8(e.what())));
		}
#endif
	}

	QReadLocker readLock(&m_pluginCollectionLock);

	// load plugins based on settings
	// iterate over all plugins that have saved settings
	auto it = Global::get().s.qhPluginSettings.constBegin();
	while (it != Global::get().s.qhPluginSettings.constEnd()) {
		// for this we need a way to get a plugin based on the filepath
		const QString pluginKey     = it.key();
		const PluginSetting setting = it.value();

		// iterate over all loaded plugins to see if the current setting is applicable
		auto pluginIt = m_pluginHashMap.begin();
		while (pluginIt != m_pluginHashMap.end()) {
			plugin_ptr_t plugin = pluginIt.value();
			QString pluginHash  = QLatin1String(
                QCryptographicHash::hash(plugin->getFilePath().toUtf8(), QCryptographicHash::Sha1).toHex());
			if (pluginKey == pluginHash) {
				if (setting.enabled) {
					loadPlugin(plugin->getID());

					const uint32_t features = plugin->getFeatures();

					if (!setting.positionalDataEnabled && (features & MUMBLE_FEATURE_POSITIONAL)) {
						// try to deactivate the feature if the setting says so
						plugin->deactivateFeatures(MUMBLE_FEATURE_POSITIONAL);
					}
				}

				// positional data is a special feature that has to be enabled/disabled in the Plugin wrapper class
				// additionally to telling the plugin library that the feature shall be deactivated
				plugin->enablePositionalData(setting.positionalDataEnabled);

				plugin->allowKeyboardMonitoring(setting.allowKeyboardMonitoring);

				break;
			}

			pluginIt++;
		}

		it++;
	}
}

const_plugin_ptr_t PluginManager::getPlugin(plugin_id_t pluginID) const {
	QReadLocker lock(&m_pluginCollectionLock);

	return m_pluginHashMap.value(pluginID);
}

void PluginManager::checkForPluginUpdates() {
	m_updater.checkForUpdates();
}

bool PluginManager::fetchPositionalData() {
	if (Global::get().bPosTest) {
		// This is for testing-purposes only so the "fetched" position doesn't have any real meaning
		m_positionalData.reset();

		m_positionalData.m_playerDir.z  = 1.0f;
		m_positionalData.m_playerAxis.y = 1.0f;
		m_positionalData.m_cameraDir.z  = 1.0f;
		m_positionalData.m_cameraAxis.y = 1.0f;

		return true;
	}

	QReadLocker activePluginLock(&m_activePosDataPluginLock);

	if (!m_activePositionalDataPlugin) {
		// It appears as if there is currently no plugin capable of delivering positional audio
		// Set positional data to zero-values
		m_positionalData.reset();

		return false;
	}

	QWriteLocker posDataLock(&m_positionalData.m_lock);

	bool retStatus = m_activePositionalDataPlugin->fetchPositionalData(
		m_positionalData.m_playerPos, m_positionalData.m_playerDir, m_positionalData.m_playerAxis,
		m_positionalData.m_cameraPos, m_positionalData.m_cameraDir, m_positionalData.m_cameraAxis,
		m_positionalData.m_context, m_positionalData.m_identity);

	if (!m_positionalData.m_context.isEmpty()) {
		m_positionalData.m_context =
			m_activePositionalDataPlugin->getPositionalDataContextPrefix() + QChar::Null + m_positionalData.m_context;
	}

	if (!retStatus) {
		// We won't be making changes to the positional data anymore, so we can drop the lock
		posDataLock.unlock();

		// Shut the currently active plugin down and set a new one (if available)
		m_activePositionalDataPlugin->shutdownPositionalData();

		emit pluginLostLink(m_activePositionalDataPlugin->getID());

		// unlock the read-lock in order to allow selectActivePositionaldataPlugin to gain a write-lock
		activePluginLock.unlock();

		selectActivePositionalDataPlugin();
	} else {
		// If the return-status doesn't indicate an error, we can assume that positional data is available
		// The remaining problematic case is, if the player is exactly at position (0,0,0) as this is used as an
		// indicator for the absence of positional data in the mix() function in AudioOutput.cpp Thus we have to make
		// sure that this position is never set if positional data is actually available. We solve this problem by
		// shifting the player a minimal amount on the z-axis
		if (m_positionalData.m_playerPos == Position3D(0.0f, 0.0f, 0.0f)) {
			m_positionalData.m_playerPos = { 0.0f, 0.0f, std::numeric_limits< float >::min() };
		}
		if (m_positionalData.m_cameraPos == Position3D(0.0f, 0.0f, 0.0f)) {
			m_positionalData.m_cameraPos = { 0.0f, 0.0f, std::numeric_limits< float >::min() };
		}
	}

	return retStatus;
}

void PluginManager::unlinkPositionalData() {
	QWriteLocker lock(&m_activePosDataPluginLock);

	if (m_activePositionalDataPlugin) {
		m_activePositionalDataPlugin->shutdownPositionalData();

		emit pluginLostLink(m_activePositionalDataPlugin->getID());

		// Set the pointer to nullptr
		m_activePositionalDataPlugin = nullptr;
	}
}

bool PluginManager::isPositionalDataAvailable() const {
	QReadLocker lock(&m_activePosDataPluginLock);

	return m_activePositionalDataPlugin != nullptr;
}

const PositionalData &PluginManager::getPositionalData() const {
	return m_positionalData;
}

void PluginManager::enablePositionalDataFor(plugin_id_t pluginID, bool enable) const {
	QReadLocker lock(&m_pluginCollectionLock);

	plugin_ptr_t plugin = m_pluginHashMap.value(pluginID);

	if (plugin) {
		plugin->enablePositionalData(enable);
	}
}

const QVector< const_plugin_ptr_t > PluginManager::getPlugins(bool sorted) const {
	QReadLocker lock(&m_pluginCollectionLock);

	QVector< const_plugin_ptr_t > pluginList;

	auto it = m_pluginHashMap.constBegin();
	if (sorted) {
		QList< plugin_id_t > ids = m_pluginHashMap.keys();

		// sort keys so that the corresponding Plugins are in alphabetical order based on their name
		std::sort(ids.begin(), ids.end(), [this](plugin_id_t first, plugin_id_t second) {
			return QString::compare(m_pluginHashMap.value(first)->getName(), m_pluginHashMap.value(second)->getName(),
									Qt::CaseInsensitive)
				   < 0;
		});

		foreach (plugin_id_t currentID, ids) { pluginList.append(m_pluginHashMap.value(currentID)); }
	} else {
		while (it != m_pluginHashMap.constEnd()) {
			pluginList.append(it.value());

			it++;
		}
	}

	return pluginList;
}

bool PluginManager::loadPlugin(plugin_id_t pluginID) const {
	QReadLocker lock(&m_pluginCollectionLock);

	plugin_ptr_t plugin = m_pluginHashMap.value(pluginID);

	if (plugin) {
		if (plugin->isLoaded()) {
			// Don't attempt to load a plugin if it already is loaded.
			// This can happen if the user clicks the apply button in the settings
			// before hitting ok.
			return true;
		}

		return plugin->init() == MUMBLE_STATUS_OK;
	}

	return false;
}

void PluginManager::unloadPlugin(plugin_id_t pluginID) {
	plugin_ptr_t plugin;
	{
		QReadLocker lock(&m_pluginCollectionLock);

		plugin = m_pluginHashMap.value(pluginID);
	}

	if (plugin) {
		unloadPlugin(*plugin);
	}
}

void PluginManager::unloadPlugin(Plugin &plugin) {
	if (plugin.isLoaded()) {
		// Only shut down loaded plugins

		bool isActivePosDataPlugin = false;
		{
			QWriteLocker lock(&m_activePosDataPluginLock);
			isActivePosDataPlugin = &plugin == m_activePositionalDataPlugin.get();
		}

		if (isActivePosDataPlugin) {
			unlinkPositionalData();
		}

		plugin.shutdown();
	}
}

bool PluginManager::clearPlugin(plugin_id_t pluginID) {
	// We have to unload the plugin before we take the write lock. The reasoning being that if
	// the plugin makes an API call in its shutdown callback, that'll lead to this manager being
	// asked for whether a plugin with such an ID exists. The function performing this check will
	// take a read lock which is not possible if we hold a write lock here already (deadlock).
	unloadPlugin(pluginID);

	QWriteLocker lock(&m_pluginCollectionLock);

	// Remove the plugin from the list of known plugins
	plugin_ptr_t plugin = m_pluginHashMap.take(pluginID);

	return plugin != nullptr;
}

uint32_t PluginManager::deactivateFeaturesFor(plugin_id_t pluginID, uint32_t features) const {
	QReadLocker lock(&m_pluginCollectionLock);

	plugin_ptr_t plugin = m_pluginHashMap.value(pluginID);

	if (plugin) {
		return plugin->deactivateFeatures(features);
	}

	return MUMBLE_FEATURE_NONE;
}

void PluginManager::allowKeyboardMonitoringFor(plugin_id_t pluginID, bool allow) const {
	QReadLocker lock(&m_pluginCollectionLock);

	plugin_ptr_t plugin = m_pluginHashMap.value(pluginID);

	if (plugin) {
		plugin->allowKeyboardMonitoring(allow);
	}
}

bool PluginManager::pluginExists(plugin_id_t pluginID) const {
	QReadLocker lock(&m_pluginCollectionLock);

	return m_pluginHashMap.contains(pluginID);
}

void PluginManager::foreachPlugin(std::function< void(Plugin &) > pluginProcessor) const {
	QReadLocker lock(&m_pluginCollectionLock);

	auto it = m_pluginHashMap.constBegin();

	while (it != m_pluginHashMap.constEnd()) {
		pluginProcessor(*it.value());

		it++;
	}
}

void PluginManager::on_serverConnected() const {
	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug("PluginManager: Connected to a server with connection ID %d", connectionID);
#endif

	foreachPlugin([connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onServerConnected(connectionID);
		}
	});
}

void PluginManager::on_serverDisconnected() const {
	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug("PluginManager: Disconnected from a server with connection ID %d", connectionID);
#endif

	foreachPlugin([connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onServerDisconnected(connectionID);
		}
	});
}

void PluginManager::on_channelEntered(const Channel *newChannel, const Channel *prevChannel, const User *user) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: User" << user->qsName << "entered channel" << newChannel->qsName
			 << "- ID:" << newChannel->iId;
#endif

	if (!Global::get().sh) {
		// if there is no server-handler, there is no (real) channel to enter
		return;
	}

	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

	foreachPlugin([user, newChannel, prevChannel, connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onChannelEntered(connectionID, user->uiSession,
									prevChannel ? static_cast< int >(prevChannel->iId) : -1,
									static_cast< int >(newChannel->iId));
		}
	});
}

void PluginManager::on_channelExited(const Channel *channel, const User *user) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: User" << user->qsName << "left channel" << channel->qsName << "- ID:" << channel->iId;
#endif

	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

	foreachPlugin([user, channel, connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onChannelExited(connectionID, user->uiSession, static_cast< int >(channel->iId));
		}
	});
}

QString getTalkingStateStr(Settings::TalkState ts) {
	switch (ts) {
		case Settings::TalkState::Passive:
			return QString::fromLatin1("Passive");
		case Settings::TalkState::Talking:
			return QString::fromLatin1("Talking");
		case Settings::TalkState::Whispering:
			return QString::fromLatin1("Whispering");
		case Settings::TalkState::Shouting:
			return QString::fromLatin1("Shouting");
		case Settings::TalkState::MutedTalking:
			return QString::fromLatin1("MutedTalking");
	}

	return QString::fromLatin1("Unknown");
}

void PluginManager::on_userTalkingStateChanged() const {
	const ClientUser *user = qobject_cast< ClientUser * >(QObject::sender());
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	if (user) {
		qDebug() << "PluginManager: User" << user->qsName << "changed talking state to"
				 << getTalkingStateStr(user->tsState);
	} else {
		qCritical() << "PluginManager: Unable to identify ClientUser";
	}
#endif

	if (user) {
		// Convert Mumble's talking state to the TalkingState used in the API
		mumble_talking_state_t ts = MUMBLE_TS_INVALID;

		switch (user->tsState) {
			case Settings::TalkState::Passive:
				ts = MUMBLE_TS_PASSIVE;
				break;
			case Settings::TalkState::Talking:
				ts = MUMBLE_TS_TALKING;
				break;
			case Settings::TalkState::Whispering:
				ts = MUMBLE_TS_WHISPERING;
				break;
			case Settings::TalkState::Shouting:
				ts = MUMBLE_TS_SHOUTING;
				break;
			case Settings::TalkState::MutedTalking:
				ts = MUMBLE_TS_TALKING_MUTED;
				break;
		}

		if (ts == MUMBLE_TS_INVALID) {
			qWarning("PluginManager.cpp: Invalid talking state encountered");
			// An error occurred
			return;
		}

		const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

		foreachPlugin([user, ts, connectionID](Plugin &plugin) {
			if (plugin.isLoaded()) {
				plugin.onUserTalkingStateChanged(connectionID, user->uiSession, ts);
			}
		});
	}
}

void PluginManager::on_audioInput(short *inputPCM, unsigned int sampleCount, unsigned int channelCount,
								  unsigned int sampleRate, bool isSpeech) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: AudioInput with" << channelCount << "channels and" << sampleCount
			 << "samples per channel. IsSpeech:" << isSpeech;
#endif

	foreachPlugin([inputPCM, sampleCount, channelCount, sampleRate, isSpeech](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onAudioInput(inputPCM, sampleCount, static_cast< std::uint16_t >(channelCount), sampleRate,
								isSpeech);
		}
	});
}

void PluginManager::on_audioSourceFetched(float *outputPCM, unsigned int sampleCount, unsigned int channelCount,
										  unsigned int sampleRate, bool isSpeech, const ClientUser *user) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: AudioSource with" << channelCount << "channels and" << sampleCount
			 << "samples per channel fetched. IsSpeech:" << isSpeech;
	if (user != nullptr) {
		qDebug() << "Sender-ID:" << user->uiSession;
	}
#endif

	foreachPlugin([outputPCM, sampleCount, channelCount, sampleRate, isSpeech, user](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onAudioSourceFetched(outputPCM, sampleCount, static_cast< std::uint16_t >(channelCount), sampleRate,
										isSpeech, user ? user->uiSession : static_cast< unsigned int >(-1));
		}
	});
}

void PluginManager::on_audioOutputAboutToPlay(float *outputPCM, unsigned int sampleCount, unsigned int channelCount,
											  unsigned int sampleRate, bool *modifiedAudio) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: AudioOutput with" << channelCount << "channels and" << sampleCount
			 << "samples per channel";
#endif
	foreachPlugin([outputPCM, sampleCount, channelCount, sampleRate, modifiedAudio](Plugin &plugin) {
		if (plugin.isLoaded()) {
			if (plugin.onAudioOutputAboutToPlay(outputPCM, sampleCount, static_cast< std::uint16_t >(channelCount),
												sampleRate)) {
				*modifiedAudio = true;
			}
		}
	});
}

void PluginManager::on_receiveData(const ClientUser *sender, const uint8_t *data, size_t dataLength,
								   const char *dataID) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: Data with ID" << dataID << "and length" << dataLength
			 << "received. Sender-ID:" << sender->uiSession;
#endif

	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

	foreachPlugin([sender, data, dataLength, dataID, connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onReceiveData(connectionID, sender->uiSession, data, dataLength, dataID);
		}
	});
}

void PluginManager::on_serverSynchronized() const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: Server synchronized";
#endif

	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

	foreachPlugin([connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onServerSynchronized(connectionID);
		}
	});
}

void PluginManager::on_userAdded(mumble_userid_t userID) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: Added user with ID" << userID;
#endif

	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

	foreachPlugin([userID, connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onUserAdded(connectionID, userID);
		};
	});
}

void PluginManager::on_userRemoved(mumble_userid_t userID) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: Removed user with ID" << userID;
#endif

	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

	foreachPlugin([userID, connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onUserRemoved(connectionID, userID);
		};
	});
}

void PluginManager::on_channelAdded(mumble_channelid_t channelID) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: Added channel with ID" << channelID;
#endif

	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

	foreachPlugin([channelID, connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onChannelAdded(connectionID, channelID);
		};
	});
}

void PluginManager::on_channelRemoved(mumble_channelid_t channelID) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: Removed channel with ID" << channelID;
#endif

	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

	foreachPlugin([channelID, connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onChannelRemoved(connectionID, channelID);
		};
	});
}

void PluginManager::on_channelRenamed(int channelID) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: Renamed channel with ID" << channelID;
#endif

	const mumble_connection_t connectionID = Global::get().sh->getConnectionID();

	foreachPlugin([channelID, connectionID](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onChannelRenamed(connectionID, channelID);
		};
	});
}

void PluginManager::on_keyEvent(unsigned int key, Qt::KeyboardModifiers modifiers, bool isPress) const {
#ifdef MUMBLE_PLUGIN_CALLBACK_DEBUG
	qDebug() << "PluginManager: Key event detected: keyCode =" << key << "modifiers:" << modifiers
			 << "isPress =" << isPress;
#else
	Q_UNUSED(modifiers);
#endif

	// Convert from Qt encoding to our own encoding
	mumble_keycode_t keyCode = API::qtKeyCodeToAPIKeyCode(key);

	foreachPlugin([keyCode, isPress](Plugin &plugin) {
		if (plugin.isLoaded()) {
			plugin.onKeyEvent(keyCode, isPress);
		}
	});
}

void PluginManager::on_syncPositionalData() {
	// fetch positional data
	if (fetchPositionalData()) {
		// Sync the gathered data (context + identity) with the server
		if (!Global::get().uiSession) {
			// For some reason the local session ID is not set -> clear all data sent to the server in order to
			// guarantee a re-send once the session is restored and there is data available
			QMutexLocker mLock(&m_sentDataMutex);

			m_sentData.context.clear();
			m_sentData.identity.clear();
		} else {
			// Check if the identity and/or the context has changed and if it did, send that new info to the server
			QMutexLocker mLock(&m_sentDataMutex);
			QReadLocker rLock(&m_positionalData.m_lock);

			if (m_sentData.context != m_positionalData.m_context
				|| m_sentData.identity != m_positionalData.m_identity) {
				MumbleProto::UserState mpus;
				mpus.set_session(Global::get().uiSession);

				if (m_sentData.context != m_positionalData.m_context) {
					m_sentData.context = m_positionalData.m_context;
					mpus.set_plugin_context(m_sentData.context.toUtf8().constData(),
											static_cast< std::size_t >(m_sentData.context.size()));
				}
				if (m_sentData.identity != m_positionalData.m_identity) {
					m_sentData.identity = m_positionalData.m_identity;
					mpus.set_plugin_identity(m_sentData.identity.toUtf8().constData());
				}

				if (Global::get().sh) {
					// send the message if the serverHandler is available
					Global::get().sh->sendMessage(mpus);
				}
			}
		}
	} else {
		QMutexLocker mLock(&m_sentDataMutex);

		if (!m_sentData.identity.isEmpty() || !m_sentData.context.isEmpty()) {
			// The server has been sent non-empty identity and/or context but we are now no longer able to fetch
			// positional data. That means that the respective plugin has been unlinked and thus we want to clear the
			// identity and context set on the server.
			MumbleProto::UserState mpus;
			mpus.set_plugin_context("");
			mpus.set_plugin_identity("");

			if (Global::get().sh) {
				Global::get().sh->sendMessage(mpus);
			}

			m_sentData.identity.clear();
			m_sentData.context.clear();
		}
	}
}

void PluginManager::on_updatesAvailable() {
	if (Global::get().s.bPluginAutoUpdate) {
		m_updater.update();
	} else {
		m_updater.promptAndUpdate();
	}
}

void PluginManager::checkForAvailablePositionalDataPlugin() {
	bool performSearch = false;
	{
		QReadLocker activePluginLock(&m_activePosDataPluginLock);

		performSearch = !m_activePositionalDataPlugin;
	}

	if (performSearch) {
		selectActivePositionalDataPlugin();
	}
}

void PluginManager::reportLostLink(mumble_plugin_id_t pluginID) {
	// We are calling GUI code, so we must only execute this function from the GUI (main) thread - which we assume is
	// where the plugin manager object is living in.
	assert(this->thread() == QThread::currentThread());

	const_plugin_ptr_t plugin = getPlugin(pluginID);

	// Need to check for the presence of Global::get().l in case we are currently
	// shutting down Mumble in which case the Log might already have been deleted.
	if (plugin && Global::get().l) {
		Global::get().l->log(Log::Information,
							 PluginManager::tr("%1 lost link").arg(plugin->getName().toHtmlEscaped()));
	}
}

void PluginManager::reportPluginLinked(mumble_plugin_id_t pluginID) {
	// We are calling GUI code, so we must only execute this function from the GUI (main) thread - which we assume is
	// where the plugin manager object is living in.
	assert(this->thread() == QThread::currentThread());

	const_plugin_ptr_t plugin = getPlugin(pluginID);

	if (plugin) {
		Global::get().l->log(Log::Information, tr("%1 linked").arg(plugin->getName().toHtmlEscaped()));
	}
}

void PluginManager::reportPermanentError(mumble_plugin_id_t pluginID) {
	// We are calling GUI code, so we must only execute this function from the GUI (main) thread - which we assume is
	// where the plugin manager object is living in.
	assert(this->thread() == QThread::currentThread());

	const_plugin_ptr_t plugin = getPlugin(pluginID);

	if (plugin) {
		Global::get().l->log(
			Log::Warning,
			tr("Plugin \"%1\" encountered a permanent error in positional data gathering").arg(plugin->getName()));
	}
}
