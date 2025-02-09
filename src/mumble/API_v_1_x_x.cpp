// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "API.h"
#include "AudioOutput.h"
#include "AudioOutputToken.h"
#include "Channel.h"
#include "ClientUser.h"
#include "Database.h"
#include "Log.h"
#include "MainWindow.h"
#include "MumbleConstants.h"
#include "PluginManager.h"
#include "ServerHandler.h"
#include "Settings.h"
#include "UserModel.h"
#include "Version.h"
#include "Global.h"

#define MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS
#include "MumblePlugin.h"
#undef MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS

#include <QVariant>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QReadLocker>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <chrono>
#include <cstdint>
#include <cstring>
#include <string>

#define EXIT_WITH(code)           \
	if (promise) {                \
		promise->set_value(code); \
	}                             \
	return;

#define VERIFY_PLUGIN_ID(id)                              \
	if (!Global::get().pluginManager->pluginExists(id)) { \
		EXIT_WITH(MUMBLE_EC_INVALID_PLUGIN_ID);           \
	}

// Right now there can only be one connection managed by the current ServerHandler
#define VERIFY_CONNECTION(connection)                                             \
	if (!Global::get().sh || Global::get().sh->getConnectionID() != connection) { \
		EXIT_WITH(MUMBLE_EC_CONNECTION_NOT_FOUND);                                \
	}

// Right now whether or not a connection has finished synchronizing is indicated by Global::get().uiSession. If it is
// zero, synchronization is not done yet (or there is no connection to begin with). The connection parameter in the
// macro is only present in case it will be needed in the future
#define ENSURE_CONNECTION_SYNCHRONIZED(connection)      \
	if (Global::get().uiSession == 0) {                 \
		EXIT_WITH(MUMBLE_EC_CONNECTION_UNSYNCHRONIZED); \
	}

#define UNUSED(var) (void) var;

namespace API {

void APIPromise::set_value(mumble_error_t value) {
	m_promise.set_value(value);
}

std::future< mumble_error_t > APIPromise::get_future() {
	return m_promise.get_future();
}

APIPromise::lock_guard_t APIPromise::lock() {
	return APIPromise::lock_guard_t(m_lock);
}

bool APIPromise::isCancelled() const {
	APIPromise::lock_guard_t guard(m_lock);

	return m_cancelled;
}

void APIPromise::cancel() {
	APIPromise::lock_guard_t guard(m_lock);

	m_cancelled = true;
}

MumbleAPICurator::~MumbleAPICurator() {
	// free all remaining resources using the stored deleters
	for (const auto &current : m_entries) {
		const Entry &entry = current.second;

		// Delete leaked resource
		entry.m_deleter(current.first);

		// Print an error about the leaked resource
		printf("[ERROR]: Plugin with ID %d leaked memory from a call to API function \"%s\"\n", entry.m_pluginID,
			   entry.m_sourceFunctionName);
	}
}
// Some common delete-functions
void defaultDeleter(const void *ptr) {
	// We use const-cast in order to circumvent the shortcoming of the free() signature only taking
	// in void * and not const void *. Delete on the other hand is allowed on const pointers which is
	// why this is an okay thing to do.
	// See also https://stackoverflow.com/questions/2819535/unable-to-free-const-pointers-in-c
	free(const_cast< void * >(ptr));
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// API IMPLEMENTATION //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

// This macro registers type, type * and type ** to Qt's metatype system
// and also their const variants (except const value as that doesn't make sense)
#define REGISTER_METATYPE(type)                             \
	qRegisterMetaType< type >(#type);                       \
	qRegisterMetaType< type * >(#type " *");                \
	qRegisterMetaType< type ** >(#type " **");              \
	qRegisterMetaType< const type * >("const " #type " *"); \
	qRegisterMetaType< const type ** >("const " #type " **");

MumbleAPI::MumbleAPI() {
	// Move this object to the main thread
	moveToThread(qApp->thread());

	// Register all API types to Qt's metatype system
	REGISTER_METATYPE(bool);
	REGISTER_METATYPE(char);
	REGISTER_METATYPE(double);
	REGISTER_METATYPE(int);
	REGISTER_METATYPE(int64_t);
	REGISTER_METATYPE(mumble_channelid_t);
	REGISTER_METATYPE(mumble_connection_t);
	REGISTER_METATYPE(mumble_plugin_id_t);
	REGISTER_METATYPE(mumble_settings_key_t);
	REGISTER_METATYPE(mumble_transmission_mode_t);
	REGISTER_METATYPE(mumble_userid_t);
	REGISTER_METATYPE(mumble_userid_t);
	REGISTER_METATYPE(std::size_t);
	REGISTER_METATYPE(uint8_t);

	// Define additional types that can't be defined using macro REGISTER_METATYPE
	qRegisterMetaType< std::shared_ptr< api_promise_t > >("std::shared_ptr< api_promise_t >");
	qRegisterMetaType< std::shared_ptr< API::api_promise_t > >("std::shared_ptr< API::api_promise_t >");
	qRegisterMetaType< const void * >("const void *");
	qRegisterMetaType< const void ** >("const void **");
	qRegisterMetaType< void * >("void *");
	qRegisterMetaType< void ** >("void **");
}

#undef REFGISTER_METATYPE

MumbleAPI &MumbleAPI::get() {
	static MumbleAPI api;

	return api;
}

void MumbleAPI::freeMemory_v_1_0_x(mumble_plugin_id_t callerID, const void *ptr,
								   std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "freeMemory_v_1_0_x", Qt::QueuedConnection, Q_ARG(mumble_plugin_id_t, callerID),
								  Q_ARG(const void *, ptr), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	// Don't verify plugin ID here to avoid memory leaks
	UNUSED(callerID);

	auto it = m_curator.m_entries.find(ptr);
	if (it != m_curator.m_entries.cend()) {
		MumbleAPICurator::Entry &entry = (*it).second;

		// call the deleter to delete the resource
		entry.m_deleter(ptr);

		// Remove pointer from curator
		m_curator.m_entries.erase(it);

		EXIT_WITH(MUMBLE_STATUS_OK);
	} else {
		EXIT_WITH(MUMBLE_EC_POINTER_NOT_FOUND);
	}
}

void MumbleAPI::getActiveServerConnection_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t *connection,
												  std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getActiveServerConnection_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t *, connection),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	if (Global::get().sh) {
		*connection = Global::get().sh->getConnectionID();

		EXIT_WITH(MUMBLE_STATUS_OK);
	} else {
		EXIT_WITH(MUMBLE_EC_NO_ACTIVE_CONNECTION);
	}
}

void MumbleAPI::isConnectionSynchronized_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
												 bool *synchronized, std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "isConnectionSynchronized_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(bool *, synchronized), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);
	VERIFY_CONNECTION(connection);

	// Right now there can only be one connection and if Global::get().uiSession is zero, then the synchronization has
	// not finished yet (or there is no connection to begin with)
	*synchronized = Global::get().uiSession != 0;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getLocalUserID_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
									   mumble_userid_t *userID, std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getLocalUserID_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_userid_t *, userID), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	*userID = Global::get().uiSession;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getUserName_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID,
									const char **name, std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getUserName_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_userid_t, userID), Q_ARG(const char **, name),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	const ClientUser *user = ClientUser::get(userID);

	if (user) {
		// +1 for NULL terminator
		std::size_t size = static_cast< std::size_t >(user->qsName.toUtf8().size() + 1);

		char *nameArray = reinterpret_cast< char * >(malloc(size * sizeof(char)));

		std::strcpy(nameArray, user->qsName.toUtf8().data());

		// save the allocated pointer and how to delete it
		m_curator.m_entries.insert({ nameArray, { defaultDeleter, callerID, "getUserName" } });

		*name = nameArray;

		EXIT_WITH(MUMBLE_STATUS_OK);
	} else {
		EXIT_WITH(MUMBLE_EC_USER_NOT_FOUND);
	}
}

void MumbleAPI::getChannelName_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
									   mumble_channelid_t channelID, const char **name,
									   std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getChannelName_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_channelid_t, channelID), Q_ARG(const char **, name),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	const Channel *channel = Channel::get(static_cast< unsigned int >(channelID));

	if (channel) {
		// +1 for NULL terminator
		std::size_t size = static_cast< std::size_t >(channel->qsName.toUtf8().size() + 1);

		char *nameArray = reinterpret_cast< char * >(malloc(size * sizeof(char)));

		std::strcpy(nameArray, channel->qsName.toUtf8().data());

		// save the allocated pointer and how to delete it
		m_curator.m_entries.insert({ nameArray, { defaultDeleter, callerID, "getChannelName" } });

		*name = nameArray;

		EXIT_WITH(MUMBLE_STATUS_OK);
	} else {
		EXIT_WITH(MUMBLE_EC_CHANNEL_NOT_FOUND);
	}
}

void MumbleAPI::getAllUsers_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
									mumble_userid_t **users, std::size_t *userCount,
									std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getAllUsers_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_userid_t **, users), Q_ARG(std::size_t *, userCount),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	QReadLocker userLock(&ClientUser::c_qrwlUsers);

	std::size_t amount = static_cast< std::size_t >(ClientUser::c_qmUsers.size());

	auto it = ClientUser::c_qmUsers.constBegin();

	mumble_userid_t *userIDs = reinterpret_cast< mumble_userid_t * >(malloc(sizeof(mumble_userid_t) * amount));

	unsigned int index = 0;
	while (it != ClientUser::c_qmUsers.constEnd()) {
		userIDs[index] = it.key();

		it++;
		index++;
	}

	m_curator.m_entries.insert({ userIDs, { defaultDeleter, callerID, "getAllUsers" } });

	*users     = userIDs;
	*userCount = amount;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getAllChannels_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
									   mumble_channelid_t **channels, std::size_t *channelCount,
									   std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getAllChannels_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_channelid_t **, channels), Q_ARG(std::size_t *, channelCount),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	QReadLocker channelLock(&Channel::c_qrwlChannels);

	std::size_t amount = static_cast< std::size_t >(Channel::c_qhChannels.size());

	auto it = Channel::c_qhChannels.constBegin();

	mumble_channelid_t *channelIDs =
		reinterpret_cast< mumble_channelid_t * >(malloc(sizeof(mumble_channelid_t) * amount));

	unsigned int index = 0;
	while (it != Channel::c_qhChannels.constEnd()) {
		channelIDs[index] = static_cast< mumble_channelid_t >(it.key());

		it++;
		index++;
	}

	m_curator.m_entries.insert({ channelIDs, { defaultDeleter, callerID, "getAllChannels" } });

	*channels     = channelIDs;
	*channelCount = amount;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getChannelOfUser_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
										 mumble_userid_t userID, mumble_channelid_t *channelID,
										 std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getChannelOfUser_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_userid_t, userID), Q_ARG(mumble_channelid_t *, channelID),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	const ClientUser *user = ClientUser::get(userID);

	if (!user) {
		EXIT_WITH(MUMBLE_EC_USER_NOT_FOUND);
	}

	if (user->cChannel) {
		*channelID = static_cast< mumble_channelid_t >(user->cChannel->iId);

		EXIT_WITH(MUMBLE_STATUS_OK);
	} else {
		EXIT_WITH(MUMBLE_EC_GENERIC_ERROR);
	}
}

void MumbleAPI::getUsersInChannel_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
										  mumble_channelid_t channelID, mumble_userid_t **users, std::size_t *userCount,
										  std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getUsersInChannel_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_channelid_t, channelID), Q_ARG(mumble_userid_t **, users),
								  Q_ARG(std::size_t *, userCount), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	const Channel *channel = Channel::get(static_cast< unsigned int >(channelID));

	if (!channel) {
		EXIT_WITH(MUMBLE_EC_CHANNEL_NOT_FOUND);
	}

	std::size_t amount = static_cast< std::size_t >(channel->qlUsers.size());

	mumble_userid_t *userIDs = reinterpret_cast< mumble_userid_t * >(malloc(sizeof(mumble_userid_t) * amount));

	int index = 0;
	foreach (const User *currentUser, channel->qlUsers) {
		userIDs[index] = currentUser->uiSession;

		index++;
	}

	m_curator.m_entries.insert({ userIDs, { defaultDeleter, callerID, "getUsersInChannel" } });

	*users     = userIDs;
	*userCount = amount;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getLocalUserTransmissionMode_v_1_0_x(mumble_plugin_id_t callerID,
													 mumble_transmission_mode_t *transmissionMode,
													 std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(
			this, "getLocalUserTransmissionMode_v_1_0_x", Qt::QueuedConnection, Q_ARG(mumble_plugin_id_t, callerID),
			Q_ARG(mumble_transmission_mode_t *, transmissionMode), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	switch (Global::get().s.atTransmit) {
		case Settings::AudioTransmit::Continuous:
			*transmissionMode = MUMBLE_TM_CONTINOUS;
			EXIT_WITH(MUMBLE_STATUS_OK);
		case Settings::AudioTransmit::VAD:
			*transmissionMode = MUMBLE_TM_VOICE_ACTIVATION;
			EXIT_WITH(MUMBLE_STATUS_OK);
		case Settings::AudioTransmit::PushToTalk:
			*transmissionMode = MUMBLE_TM_PUSH_TO_TALK;
			EXIT_WITH(MUMBLE_STATUS_OK);
	}

	// Unable to resolve transmission mode
	EXIT_WITH(MUMBLE_EC_GENERIC_ERROR);
}

void MumbleAPI::isUserLocallyMuted_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
										   mumble_userid_t userID, bool *muted,
										   std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "isUserLocallyMuted_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_userid_t, userID), Q_ARG(bool *, muted),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	const ClientUser *user = ClientUser::get(userID);

	if (!user) {
		EXIT_WITH(MUMBLE_EC_USER_NOT_FOUND);
	}

	*muted = user->bLocalMute;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::isLocalUserMuted_v_1_0_x(mumble_plugin_id_t callerID, bool *muted,
										 std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "isLocalUserMuted_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(bool *, muted),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	*muted = Global::get().s.bMute;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::isLocalUserDeafened_v_1_0_x(mumble_plugin_id_t callerID, bool *deafened,
											std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "isLocalUserDeafened_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(bool *, deafened),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	*deafened = Global::get().s.bDeaf;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getUserHash_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID,
									const char **hash, std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getUserHash_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_userid_t, userID), Q_ARG(const char **, hash),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	const ClientUser *user = ClientUser::get(userID);

	if (!user) {
		EXIT_WITH(MUMBLE_EC_USER_NOT_FOUND);
	}

	// The user's hash is already in hexadecimal representation, so we don't have to worry about null-bytes in it
	// +1 for NULL terminator
	std::size_t size = static_cast< std::size_t >(user->qsHash.toUtf8().size() + 1);

	char *hashArray = reinterpret_cast< char * >(malloc(size * sizeof(char)));

	std::strcpy(hashArray, user->qsHash.toUtf8().data());

	m_curator.m_entries.insert({ hashArray, { defaultDeleter, callerID, "getUserHash" } });

	*hash = hashArray;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getServerHash_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, const char **hash,
									  std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getServerHash_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(const char **, hash), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	// Use hexadecimal representation in order for the String to be properly printable and for it to be C-encodable
	QByteArray hashHex = Global::get().sh->qbaDigest.toHex();
	QString strHash    = QString::fromLatin1(hashHex);

	// +1 for NULL terminator
	std::size_t size = static_cast< std::size_t >(strHash.toUtf8().size() + 1);

	char *hashArray = reinterpret_cast< char * >(malloc(size * sizeof(char)));

	std::strcpy(hashArray, strHash.toUtf8().data());

	m_curator.m_entries.insert({ hashArray, { defaultDeleter, callerID, "getServerHash" } });

	*hash = hashArray;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::requestLocalUserTransmissionMode_v_1_0_x(mumble_plugin_id_t callerID,
														 mumble_transmission_mode_t transmissionMode,
														 std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(
			this, "requestLocalUserTransmissionMode_v_1_0_x", Qt::QueuedConnection, Q_ARG(mumble_plugin_id_t, callerID),
			Q_ARG(mumble_transmission_mode_t, transmissionMode), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	Settings::AudioTransmit mode    = Settings::PushToTalk;
	bool identifiedTransmissionMode = false;

	switch (transmissionMode) {
		case MUMBLE_TM_CONTINOUS:
			mode                       = Settings::Continuous;
			identifiedTransmissionMode = true;
			break;
		case MUMBLE_TM_VOICE_ACTIVATION:
			mode                       = Settings::VAD;
			identifiedTransmissionMode = true;
			break;
		case MUMBLE_TM_PUSH_TO_TALK:
			mode                       = Settings::PushToTalk;
			identifiedTransmissionMode = true;
			break;
	}

	if (identifiedTransmissionMode) {
		if (!Global::get().mw) {
			EXIT_WITH(MUMBLE_EC_INTERNAL_ERROR);
		}

		Global::get().mw->setTransmissionMode(mode);

		EXIT_WITH(MUMBLE_STATUS_OK);
	} else {
		EXIT_WITH(MUMBLE_EC_UNKNOWN_TRANSMISSION_MODE);
	}
}

void MumbleAPI::getUserComment_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
									   mumble_userid_t userID, const char **comment,
									   std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getUserComment_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_userid_t, userID), Q_ARG(const char **, comment),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	ClientUser *user = ClientUser::get(userID);

	if (!user) {
		EXIT_WITH(MUMBLE_EC_USER_NOT_FOUND);
	}

	if (user->qsComment.isEmpty() && !user->qbaCommentHash.isEmpty()) {
		user->qsComment = QString::fromUtf8(Global::get().db->blob(user->qbaCommentHash));

		if (user->qsComment.isEmpty()) {
			// The user's comment hasn't been synchronized to this client yet
			EXIT_WITH(MUMBLE_EC_UNSYNCHRONIZED_BLOB);
		}
	}

	// +1 for NULL terminator
	std::size_t size = static_cast< std::size_t >(user->qsComment.toUtf8().size() + 1);

	char *nameArray = reinterpret_cast< char * >(malloc(size * sizeof(char)));

	std::strcpy(nameArray, user->qsComment.toUtf8().data());

	m_curator.m_entries.insert({ nameArray, { defaultDeleter, callerID, "getUserComment" } });

	*comment = nameArray;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getChannelDescription_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
											  mumble_channelid_t channelID, const char **description,
											  std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getChannelDescription_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_channelid_t, channelID), Q_ARG(const char **, description),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	Channel *channel = Channel::get(static_cast< unsigned int >(channelID));

	if (!channel) {
		EXIT_WITH(MUMBLE_EC_CHANNEL_NOT_FOUND);
	}

	if (channel->qsDesc.isEmpty() && !channel->qbaDescHash.isEmpty()) {
		channel->qsDesc = QString::fromUtf8(Global::get().db->blob(channel->qbaDescHash));

		if (channel->qsDesc.isEmpty()) {
			// The channel's description hasn't been synchronized to this client yet
			EXIT_WITH(MUMBLE_EC_UNSYNCHRONIZED_BLOB);
		}
	}

	// +1 for NULL terminator
	std::size_t size = static_cast< std::size_t >(channel->qsDesc.toUtf8().size() + 1);

	char *nameArray = reinterpret_cast< char * >(malloc(size * sizeof(char)));

	std::strcpy(nameArray, channel->qsDesc.toUtf8().data());

	m_curator.m_entries.insert({ nameArray, { defaultDeleter, callerID, "getChannelDescription" } });

	*description = nameArray;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::requestUserMove_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
										mumble_userid_t userID, mumble_channelid_t channelID, const char *password,
										std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "requestUserMove_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_userid_t, userID), Q_ARG(mumble_channelid_t, channelID),
								  Q_ARG(const char *, password), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	const ClientUser *user = ClientUser::get(userID);

	if (!user) {
		EXIT_WITH(MUMBLE_EC_USER_NOT_FOUND);
	}

	const Channel *channel = Channel::get(static_cast< unsigned int >(channelID));

	if (!channel) {
		EXIT_WITH(MUMBLE_EC_CHANNEL_NOT_FOUND);
	}

	if (channel != user->cChannel) {
		// send move-request to the server only if the user is not in the channel already
		QStringList passwordList;
		if (password) {
			passwordList << QString::fromUtf8(password);
		}

		Global::get().sh->joinChannel(user->uiSession, static_cast< unsigned int >(channel->iId), passwordList);
	}

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::requestMicrophoneActivationOverwrite_v_1_0_x(mumble_plugin_id_t callerID, bool activate,
															 std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "requestMicrophoneActivationOverwrite_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(bool, activate),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	PluginData::get().overwriteMicrophoneActivation.store(activate);

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::requestLocalMute_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
										 mumble_userid_t userID, bool muted, std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "requestLocalMute_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(mumble_userid_t, userID), Q_ARG(bool, muted),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	if (userID == Global::get().uiSession) {
		// Can't locally mute the local user
		EXIT_WITH(MUMBLE_EC_INVALID_MUTE_TARGET);
	}

	ClientUser *user = ClientUser::get(userID);

	if (!user) {
		EXIT_WITH(MUMBLE_EC_USER_NOT_FOUND);
	}

	user->setLocalMute(muted);

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::requestLocalUserMute_v_1_0_x(mumble_plugin_id_t callerID, bool muted,
											 std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "requestLocalUserMute_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(bool, muted),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	if (!Global::get().mw) {
		EXIT_WITH(MUMBLE_EC_INTERNAL_ERROR);
	}

	Global::get().mw->setAudioMute(muted);

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::requestLocalUserDeaf_v_1_0_x(mumble_plugin_id_t callerID, bool deafened,
											 std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "requestLocalUserDeaf_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(bool, deafened),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	if (!Global::get().mw) {
		EXIT_WITH(MUMBLE_EC_INTERNAL_ERROR);
	}

	Global::get().mw->setAudioDeaf(deafened);

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::requestSetLocalUserComment_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
												   const char *comment, std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "requestSetLocalUserComment_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(const char *, comment), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	ClientUser *localUser = ClientUser::get(Global::get().uiSession);

	if (!localUser) {
		EXIT_WITH(MUMBLE_EC_USER_NOT_FOUND);
	}

	if (!Global::get().mw || !Global::get().mw->pmModel) {
		EXIT_WITH(MUMBLE_EC_INTERNAL_ERROR);
	}

	Global::get().mw->pmModel->setComment(localUser, QString::fromUtf8(comment));

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::findUserByName_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
									   const char *userName, mumble_userid_t *userID,
									   std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "findUserByName_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(const char *, userName), Q_ARG(mumble_userid_t *, userID),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	const QString qsUserName = QString::fromUtf8(userName);

	QReadLocker userLock(&ClientUser::c_qrwlUsers);

	auto it = ClientUser::c_qmUsers.constBegin();
	while (it != ClientUser::c_qmUsers.constEnd()) {
		if (it.value()->qsName == qsUserName) {
			*userID = it.key();

			EXIT_WITH(MUMBLE_STATUS_OK);
		}

		it++;
	}

	EXIT_WITH(MUMBLE_EC_USER_NOT_FOUND);
}

void MumbleAPI::findChannelByName_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
										  const char *channelName, mumble_channelid_t *channelID,
										  std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "findChannelByName_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_connection_t, connection),
								  Q_ARG(const char *, channelName), Q_ARG(mumble_channelid_t *, channelID),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	const QString qsChannelName = QString::fromUtf8(channelName);

	QReadLocker channelLock(&Channel::c_qrwlChannels);

	auto it = Channel::c_qhChannels.constBegin();
	while (it != Channel::c_qhChannels.constEnd()) {
		if (it.value()->qsName == qsChannelName) {
			*channelID = static_cast< mumble_channelid_t >(it.key());

			EXIT_WITH(MUMBLE_STATUS_OK);
		}

		it++;
	}

	EXIT_WITH(MUMBLE_EC_CHANNEL_NOT_FOUND);
}

QVariant getMumbleSettingHelper(mumble_settings_key_t key) {
	QVariant value;

	// All values are explicitly cast to the target type of their associated API. For instance there is not API to
	// get float values but there is one for doubles. Therefore floats have to be cast to doubles in order for the
	// type checking to work out.
	switch (key) {
		case MUMBLE_SK_AUDIO_INPUT_VOICE_HOLD:
			value = static_cast< int >(Global::get().s.iVoiceHold);
			break;
		case MUMBLE_SK_AUDIO_INPUT_VAD_SILENCE_THRESHOLD:
			value = static_cast< double >(Global::get().s.fVADmin);
			break;
		case MUMBLE_SK_AUDIO_INPUT_VAD_SPEECH_THRESHOLD:
			value = static_cast< double >(Global::get().s.fVADmax);
			break;
		case MUMBLE_SK_AUDIO_OUTPUT_PA_MINIMUM_DISTANCE:
			value = static_cast< double >(Global::get().s.fAudioMinDistance);
			break;
		case MUMBLE_SK_AUDIO_OUTPUT_PA_MAXIMUM_DISTANCE:
			value = static_cast< double >(Global::get().s.fAudioMaxDistance);
			break;
		case MUMBLE_SK_AUDIO_OUTPUT_PA_BLOOM:
			value = static_cast< double >(Global::get().s.fAudioBloom);
			break;
		case MUMBLE_SK_AUDIO_OUTPUT_PA_MINIMUM_VOLUME:
			value = static_cast< double >(Global::get().s.fAudioMaxDistVolume);
			break;
		case MUMBLE_SK_INVALID:
			// There is no setting associated with this key
			break;
	}

	return value;
}

// IS_TYPE actually only checks if the QVariant can be converted to the needed type since that's all that we really care
// about at the end of the day.
#define IS_TYPE(var, varType) static_cast< QMetaType::Type >(var.typeId()) == varType
#define IS_NOT_TYPE(var, varType) static_cast< QMetaType::Type >(var.typeId()) != varType

void MumbleAPI::getMumbleSetting_bool_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, bool *outValue,
											  std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getMumbleSetting_bool_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_settings_key_t, key),
								  Q_ARG(bool *, outValue), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	QVariant value = getMumbleSettingHelper(key);

	if (!value.isValid()) {
		// We also return that for MUMBLE_SK_INVALID
		EXIT_WITH(MUMBLE_EC_UNKNOWN_SETTINGS_KEY);
	}

	if (IS_NOT_TYPE(value, QMetaType::Bool)) {
		EXIT_WITH(MUMBLE_EC_WRONG_SETTINGS_TYPE);
	}

	*outValue = value.toBool();

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getMumbleSetting_int_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, int64_t *outValue,
											 std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getMumbleSetting_int_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_settings_key_t, key),
								  Q_ARG(int64_t *, outValue), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	QVariant value = getMumbleSettingHelper(key);

	if (!value.isValid()) {
		// We also return that for MUMBLE_SK_INVALID
		EXIT_WITH(MUMBLE_EC_UNKNOWN_SETTINGS_KEY);
	}

	if (IS_NOT_TYPE(value, QMetaType::Int)) {
		EXIT_WITH(MUMBLE_EC_WRONG_SETTINGS_TYPE);
	}

	*outValue = value.toInt();

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getMumbleSetting_double_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key,
												double *outValue, std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getMumbleSetting_double_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_settings_key_t, key),
								  Q_ARG(double *, outValue), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	QVariant value = getMumbleSettingHelper(key);

	if (!value.isValid()) {
		// We also return that for MUMBLE_SK_INVALID
		EXIT_WITH(MUMBLE_EC_UNKNOWN_SETTINGS_KEY);
	}

	if (IS_NOT_TYPE(value, QMetaType::Double)) {
		EXIT_WITH(MUMBLE_EC_WRONG_SETTINGS_TYPE);
	}

	*outValue = value.toDouble();

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::getMumbleSetting_string_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key,
												const char **outValue, std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "getMumbleSetting_string_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_settings_key_t, key),
								  Q_ARG(const char **, outValue), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	QVariant value = getMumbleSettingHelper(key);

	if (!value.isValid()) {
		// We also return that for MUMBLE_SK_INVALID
		EXIT_WITH(MUMBLE_EC_UNKNOWN_SETTINGS_KEY);
	}

	if (IS_NOT_TYPE(value, QMetaType::QString)) {
		EXIT_WITH(MUMBLE_EC_WRONG_SETTINGS_TYPE);
	}

	const QString stringValue = value.toString();

	// +1 for NULL terminator
	std::size_t size = static_cast< std::size_t >(stringValue.toUtf8().size() + 1);

	char *valueArray = reinterpret_cast< char * >(malloc(size * sizeof(char)));

	std::strcpy(valueArray, stringValue.toUtf8().data());

	m_curator.m_entries.insert({ valueArray, { defaultDeleter, callerID, "getMumbleSetting_string" } });

	*outValue = valueArray;

	EXIT_WITH(MUMBLE_STATUS_OK);
}

mumble_error_t setMumbleSettingHelper(mumble_settings_key_t key, QVariant value) {
	switch (key) {
		case MUMBLE_SK_AUDIO_INPUT_VOICE_HOLD:
			if (IS_TYPE(value, QMetaType::Int)) {
				Global::get().s.iVoiceHold = value.toInt();

				return MUMBLE_STATUS_OK;
			} else {
				return MUMBLE_EC_WRONG_SETTINGS_TYPE;
			}
		case MUMBLE_SK_AUDIO_INPUT_VAD_SILENCE_THRESHOLD:
			if (IS_TYPE(value, QMetaType::Double)) {
				Global::get().s.fVADmin = static_cast< float >(value.toDouble());

				return MUMBLE_STATUS_OK;
			} else {
				return MUMBLE_EC_WRONG_SETTINGS_TYPE;
			}
		case MUMBLE_SK_AUDIO_INPUT_VAD_SPEECH_THRESHOLD:
			if (IS_TYPE(value, QMetaType::Double)) {
				Global::get().s.fVADmax = static_cast< float >(value.toDouble());

				return MUMBLE_STATUS_OK;
			} else {
				return MUMBLE_EC_WRONG_SETTINGS_TYPE;
			}
		case MUMBLE_SK_AUDIO_OUTPUT_PA_MINIMUM_DISTANCE:
			if (IS_TYPE(value, QMetaType::Double)) {
				Global::get().s.fAudioMinDistance = static_cast< float >(value.toDouble());

				return MUMBLE_STATUS_OK;
			} else {
				return MUMBLE_EC_WRONG_SETTINGS_TYPE;
			}
		case MUMBLE_SK_AUDIO_OUTPUT_PA_MAXIMUM_DISTANCE:
			if (IS_TYPE(value, QMetaType::Double)) {
				Global::get().s.fAudioMaxDistance = static_cast< float >(value.toDouble());

				return MUMBLE_STATUS_OK;
			} else {
				return MUMBLE_EC_WRONG_SETTINGS_TYPE;
			}
		case MUMBLE_SK_AUDIO_OUTPUT_PA_BLOOM:
			if (IS_TYPE(value, QMetaType::Double)) {
				Global::get().s.fAudioBloom = static_cast< float >(value.toDouble());

				return MUMBLE_STATUS_OK;
			} else {
				return MUMBLE_EC_WRONG_SETTINGS_TYPE;
			}
		case MUMBLE_SK_AUDIO_OUTPUT_PA_MINIMUM_VOLUME:
			if (IS_TYPE(value, QMetaType::Double)) {
				Global::get().s.fAudioMaxDistVolume = static_cast< float >(value.toDouble());

				return MUMBLE_STATUS_OK;
			} else {
				return MUMBLE_EC_WRONG_SETTINGS_TYPE;
			}
		case MUMBLE_SK_INVALID:
			// Do nothing
			break;
	}

	return MUMBLE_EC_UNKNOWN_SETTINGS_KEY;
}

void MumbleAPI::setMumbleSetting_bool_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, bool value,
											  std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "setMumbleSetting_bool_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_settings_key_t, key),
								  Q_ARG(bool, value), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	mumble_error_t exitCode = setMumbleSettingHelper(key, value);
	EXIT_WITH(exitCode);
}

void MumbleAPI::setMumbleSetting_int_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, int64_t value,
											 std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "setMumbleSetting_int_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_settings_key_t, key),
								  Q_ARG(int64_t, value), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	mumble_error_t exitCode = setMumbleSettingHelper(key, QVariant::fromValue(value));
	EXIT_WITH(exitCode);
}

void MumbleAPI::setMumbleSetting_double_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, double value,
												std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "setMumbleSetting_double_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_settings_key_t, key),
								  Q_ARG(double, value), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	mumble_error_t exitCode = setMumbleSettingHelper(key, value);
	EXIT_WITH(exitCode);
}

void MumbleAPI::setMumbleSetting_string_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key,
												const char *value, std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "setMumbleSetting_string_v_1_0_x", Qt::QueuedConnection,
								  Q_ARG(mumble_plugin_id_t, callerID), Q_ARG(mumble_settings_key_t, key),
								  Q_ARG(const char *, value), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	mumble_error_t exitCode = setMumbleSettingHelper(key, QString::fromUtf8(value));
	EXIT_WITH(exitCode);
}
#undef IS_TYPE
#undef IS_NOT_TYPE

void MumbleAPI::sendData_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
								 const mumble_userid_t *users, std::size_t userCount, const uint8_t *data,
								 std::size_t dataLength, const char *dataID, std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "sendData_v_1_0_x", Qt::QueuedConnection, Q_ARG(mumble_plugin_id_t, callerID),
								  Q_ARG(mumble_connection_t, connection), Q_ARG(const mumble_userid_t *, users),
								  Q_ARG(std::size_t, userCount), Q_ARG(const uint8_t *, data),
								  Q_ARG(std::size_t, dataLength), Q_ARG(const char *, dataID),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	VERIFY_CONNECTION(connection);
	ENSURE_CONNECTION_SYNCHRONIZED(connection);

	if (dataLength > Mumble::Plugins::PluginMessage::MAX_DATA_LENGTH) {
		EXIT_WITH(MUMBLE_EC_DATA_TOO_BIG);
	}
	if (std::strlen(dataID) > Mumble::Plugins::PluginMessage::MAX_DATA_ID_LENGTH) {
		EXIT_WITH(MUMBLE_EC_DATA_ID_TOO_LONG);
	}

	MumbleProto::PluginDataTransmission mpdt;
	mpdt.set_sendersession(Global::get().uiSession);

	for (std::size_t i = 0; i < userCount; i++) {
		const ClientUser *user = ClientUser::get(users[i]);

		if (user) {
			mpdt.add_receiversessions(users[i]);
		} else {
			EXIT_WITH(MUMBLE_EC_USER_NOT_FOUND);
		}
	}

	mpdt.set_data(data, dataLength);
	mpdt.set_dataid(dataID);

	if (Global::get().sh) {
		if (Global::get().sh->m_version < Version::fromComponents(1, 4, 0)) {
			// The sendMessage call relies on the server relaying the message to the respective receiver. This
			// functionality was added to the server protocol in version 1.4.0, so an older server will not know what to
			// do with the received message.
			EXIT_WITH(MUMBLE_EC_OPERATION_UNSUPPORTED_BY_SERVER);
		}

		Global::get().sh->sendMessage(mpdt);

		EXIT_WITH(MUMBLE_STATUS_OK);
	} else {
		EXIT_WITH(MUMBLE_EC_CONNECTION_NOT_FOUND);
	}
}

void MumbleAPI::log_v_1_0_x(mumble_plugin_id_t callerID, const char *message,
							std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "log_v_1_0_x", Qt::QueuedConnection, Q_ARG(mumble_plugin_id_t, callerID),
								  Q_ARG(const char *, message), Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	// We verify the plugin manually as we need a handle to it later
	const_plugin_ptr_t plugin = Global::get().pluginManager->getPlugin(callerID);
	if (!plugin) {
		EXIT_WITH(MUMBLE_EC_INVALID_PLUGIN_ID);
	}

	QString msg = QString::fromLatin1("<b>%1:</b> %2")
					  .arg(plugin->getName().toHtmlEscaped())
					  .arg(QString::fromUtf8(message).toHtmlEscaped());

	// Use static method that handles the case in which the Log object doesn't exist yet
	Log::logOrDefer(Log::PluginMessage, msg);

	EXIT_WITH(MUMBLE_STATUS_OK);
}

void MumbleAPI::playSample_v_1_0_x(mumble_plugin_id_t callerID, const char *samplePath,
								   std::shared_ptr< api_promise_t > promise) {
	playSample_v_1_2_x(callerID, samplePath, 1.0f, promise);
}

void MumbleAPI::playSample_v_1_2_x(mumble_plugin_id_t callerID, const char *samplePath, float volume,
								   std::shared_ptr< api_promise_t > promise) {
	if (QThread::currentThread() != thread()) {
		// Invoke in main thread
		QMetaObject::invokeMethod(this, "playSample_v_1_2_x", Qt::QueuedConnection, Q_ARG(mumble_plugin_id_t, callerID),
								  Q_ARG(const char *, samplePath), Q_ARG(float, volume),
								  Q_ARG(std::shared_ptr< api_promise_t >, promise));

		return;
	}

	api_promise_t::lock_guard_t guard = promise->lock();
	if (promise->isCancelled()) {
		return;
	}

	VERIFY_PLUGIN_ID(callerID);

	if (!Global::get().ao) {
		EXIT_WITH(MUMBLE_EC_AUDIO_NOT_AVAILABLE);
	}

	if (Global::get().ao->playSample(QString::fromUtf8(samplePath), volume, false)) {
		EXIT_WITH(MUMBLE_STATUS_OK);
	} else {
		EXIT_WITH(MUMBLE_EC_INVALID_SAMPLE);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////// C FUNCTION WRAPPERS FOR USE IN API STRUCT ///////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#define C_WRAPPER(funcName)                                                                                    \
	mumble_error_t MUMBLE_PLUGIN_CALLING_CONVENTION funcName(TYPED_ARGS) {                                     \
		auto promise        = std::make_shared< api_promise_t >();                                             \
		api_future_t future = promise->get_future();                                                           \
                                                                                                               \
		MumbleAPI::get().funcName(ARG_NAMES, promise);                                                         \
                                                                                                               \
		if (future.wait_for(std::chrono::milliseconds(800)) != std::future_status::ready) {                    \
			/* The call to cancel may block until the operation is finished, if and only if the operation      \
			 has already started and is thus in progress.*/                                                    \
			promise->cancel();                                                                                 \
                                                                                                               \
			/* If the cancel-operation above blocked, this means that the operation has now finished in which  \
			 case this will fail and we continue as if nothing had happened.                                   \
			 If however it did not block the operation will immediately abort once it starts, meaning that the \
			 check below will succeed. */                                                                      \
			if (future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {                  \
				promise->set_value(MUMBLE_EC_API_REQUEST_TIMEOUT);                                             \
			}                                                                                                  \
		}                                                                                                      \
                                                                                                               \
		return future.get();                                                                                   \
	}

#define TYPED_ARGS mumble_plugin_id_t callerID, const void *ptr
#define ARG_NAMES callerID, ptr
C_WRAPPER(freeMemory_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_connection_t *connection
#define ARG_NAMES callerID, connection
C_WRAPPER(getActiveServerConnection_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_connection_t connection, bool *synchronized
#define ARG_NAMES callerID, connection, synchronized
C_WRAPPER(isConnectionSynchronized_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t *userID
#define ARG_NAMES callerID, connection, userID
C_WRAPPER(getLocalUserID_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS \
	mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, const char **name
#define ARG_NAMES callerID, connection, userID, name
C_WRAPPER(getUserName_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS \
	mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_channelid_t channelID, const char **name
#define ARG_NAMES callerID, connection, channelID, name
C_WRAPPER(getChannelName_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS \
	mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t **users, std::size_t *userCount
#define ARG_NAMES callerID, connection, users, userCount
C_WRAPPER(getAllUsers_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS                                                                              \
	mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_channelid_t **channels, \
		std::size_t *channelCount
#define ARG_NAMES callerID, connection, channels, channelCount
C_WRAPPER(getAllChannels_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS \
	mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, mumble_channelid_t *channel
#define ARG_NAMES callerID, connection, userID, channel
C_WRAPPER(getChannelOfUser_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS                                                                             \
	mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_channelid_t channelID, \
		mumble_userid_t **userList, std::size_t *userCount
#define ARG_NAMES callerID, connection, channelID, userList, userCount
C_WRAPPER(getUsersInChannel_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_transmission_mode_t *transmissionMode
#define ARG_NAMES callerID, transmissionMode
C_WRAPPER(getLocalUserTransmissionMode_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, bool *muted
#define ARG_NAMES callerID, connection, userID, muted
C_WRAPPER(isUserLocallyMuted_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, bool *muted
#define ARG_NAMES callerID, muted
C_WRAPPER(isLocalUserMuted_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, bool *deafened
#define ARG_NAMES callerID, deafened
C_WRAPPER(isLocalUserDeafened_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS \
	mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, const char **hash
#define ARG_NAMES callerID, connection, userID, hash
C_WRAPPER(getUserHash_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_connection_t connection, const char **hash
#define ARG_NAMES callerID, connection, hash
C_WRAPPER(getServerHash_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_transmission_mode_t transmissionMode
#define ARG_NAMES callerID, transmissionMode
C_WRAPPER(requestLocalUserTransmissionMode_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS \
	mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, const char **comment
#define ARG_NAMES callerID, connection, userID, comment
C_WRAPPER(getUserComment_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS \
	mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_channelid_t channelID, const char **description
#define ARG_NAMES callerID, connection, channelID, description
C_WRAPPER(getChannelDescription_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS                                                                                                     \
	mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, mumble_channelid_t channelID, \
		const char *password
#define ARG_NAMES callerID, connection, userID, channelID, password
C_WRAPPER(requestUserMove_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, bool activate
#define ARG_NAMES callerID, activate
C_WRAPPER(requestMicrophoneActivationOverwrite_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, bool muted
#define ARG_NAMES callerID, connection, userID, muted
C_WRAPPER(requestLocalMute_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, bool muted
#define ARG_NAMES callerID, muted
C_WRAPPER(requestLocalUserMute_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, bool deafened
#define ARG_NAMES callerID, deafened
C_WRAPPER(requestLocalUserDeaf_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_connection_t connection, const char *comment
#define ARG_NAMES callerID, connection, comment
C_WRAPPER(requestSetLocalUserComment_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS \
	mumble_plugin_id_t callerID, mumble_connection_t connection, const char *userName, mumble_userid_t *userID
#define ARG_NAMES callerID, connection, userName, userID
C_WRAPPER(findUserByName_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS \
	mumble_plugin_id_t callerID, mumble_connection_t connection, const char *channelName, mumble_channelid_t *channelID
#define ARG_NAMES callerID, connection, channelName, channelID
C_WRAPPER(findChannelByName_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_settings_key_t key, bool *outValue
#define ARG_NAMES callerID, key, outValue
C_WRAPPER(getMumbleSetting_bool_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_settings_key_t key, int64_t *outValue
#define ARG_NAMES callerID, key, outValue
C_WRAPPER(getMumbleSetting_int_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_settings_key_t key, double *outValue
#define ARG_NAMES callerID, key, outValue
C_WRAPPER(getMumbleSetting_double_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_settings_key_t key, const char **outValue
#define ARG_NAMES callerID, key, outValue
C_WRAPPER(getMumbleSetting_string_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_settings_key_t key, bool value
#define ARG_NAMES callerID, key, value
C_WRAPPER(setMumbleSetting_bool_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_settings_key_t key, int64_t value
#define ARG_NAMES callerID, key, value
C_WRAPPER(setMumbleSetting_int_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_settings_key_t key, double value
#define ARG_NAMES callerID, key, value
C_WRAPPER(setMumbleSetting_double_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, mumble_settings_key_t key, const char *value
#define ARG_NAMES callerID, key, value
C_WRAPPER(setMumbleSetting_string_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS                                                                                                    \
	mumble_plugin_id_t callerID, mumble_connection_t connection, const mumble_userid_t *users, std::size_t userCount, \
		const uint8_t *data, std::size_t dataLength, const char *dataID
#define ARG_NAMES callerID, connection, users, userCount, data, dataLength, dataID
C_WRAPPER(sendData_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, const char *message
#define ARG_NAMES callerID, message
C_WRAPPER(log_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, const char *samplePath
#define ARG_NAMES callerID, samplePath
C_WRAPPER(playSample_v_1_0_x)
#undef TYPED_ARGS
#undef ARG_NAMES

#define TYPED_ARGS mumble_plugin_id_t callerID, const char *samplePath, float volume
#define ARG_NAMES callerID, samplePath, volume
C_WRAPPER(playSample_v_1_2_x)
#undef TYPED_ARGS
#undef ARG_NAMES


#undef C_WRAPPER


/////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// GETTER FOR API STRUCTS /////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

MumbleAPI_v_1_0_x getMumbleAPI_v_1_0_x() {
	return { freeMemory_v_1_0_x,
			 getActiveServerConnection_v_1_0_x,
			 isConnectionSynchronized_v_1_0_x,
			 getLocalUserID_v_1_0_x,
			 getUserName_v_1_0_x,
			 getChannelName_v_1_0_x,
			 getAllUsers_v_1_0_x,
			 getAllChannels_v_1_0_x,
			 getChannelOfUser_v_1_0_x,
			 getUsersInChannel_v_1_0_x,
			 getLocalUserTransmissionMode_v_1_0_x,
			 isUserLocallyMuted_v_1_0_x,
			 isLocalUserMuted_v_1_0_x,
			 isLocalUserDeafened_v_1_0_x,
			 getUserHash_v_1_0_x,
			 getServerHash_v_1_0_x,
			 getUserComment_v_1_0_x,
			 getChannelDescription_v_1_0_x,
			 requestLocalUserTransmissionMode_v_1_0_x,
			 requestUserMove_v_1_0_x,
			 requestMicrophoneActivationOverwrite_v_1_0_x,
			 requestLocalMute_v_1_0_x,
			 requestLocalUserMute_v_1_0_x,
			 requestLocalUserDeaf_v_1_0_x,
			 requestSetLocalUserComment_v_1_0_x,
			 findUserByName_v_1_0_x,
			 findChannelByName_v_1_0_x,
			 getMumbleSetting_bool_v_1_0_x,
			 getMumbleSetting_int_v_1_0_x,
			 getMumbleSetting_double_v_1_0_x,
			 getMumbleSetting_string_v_1_0_x,
			 setMumbleSetting_bool_v_1_0_x,
			 setMumbleSetting_int_v_1_0_x,
			 setMumbleSetting_double_v_1_0_x,
			 setMumbleSetting_string_v_1_0_x,
			 sendData_v_1_0_x,
			 log_v_1_0_x,
			 playSample_v_1_0_x };
}

MumbleAPI_v_1_2_x getMumbleAPI_v_1_2_x() {
	return { freeMemory_v_1_0_x,
			 getActiveServerConnection_v_1_0_x,
			 isConnectionSynchronized_v_1_0_x,
			 getLocalUserID_v_1_0_x,
			 getUserName_v_1_0_x,
			 getChannelName_v_1_0_x,
			 getAllUsers_v_1_0_x,
			 getAllChannels_v_1_0_x,
			 getChannelOfUser_v_1_0_x,
			 getUsersInChannel_v_1_0_x,
			 getLocalUserTransmissionMode_v_1_0_x,
			 isUserLocallyMuted_v_1_0_x,
			 isLocalUserMuted_v_1_0_x,
			 isLocalUserDeafened_v_1_0_x,
			 getUserHash_v_1_0_x,
			 getServerHash_v_1_0_x,
			 getUserComment_v_1_0_x,
			 getChannelDescription_v_1_0_x,
			 requestLocalUserTransmissionMode_v_1_0_x,
			 requestUserMove_v_1_0_x,
			 requestMicrophoneActivationOverwrite_v_1_0_x,
			 requestLocalMute_v_1_0_x,
			 requestLocalUserMute_v_1_0_x,
			 requestLocalUserDeaf_v_1_0_x,
			 requestSetLocalUserComment_v_1_0_x,
			 findUserByName_v_1_0_x,
			 findChannelByName_v_1_0_x,
			 getMumbleSetting_bool_v_1_0_x,
			 getMumbleSetting_int_v_1_0_x,
			 getMumbleSetting_double_v_1_0_x,
			 getMumbleSetting_string_v_1_0_x,
			 setMumbleSetting_bool_v_1_0_x,
			 setMumbleSetting_int_v_1_0_x,
			 setMumbleSetting_double_v_1_0_x,
			 setMumbleSetting_string_v_1_0_x,
			 sendData_v_1_0_x,
			 log_v_1_0_x,
			 playSample_v_1_2_x };
}

#define MAP(qtName, apiName) \
	case Qt::Key_##qtName:   \
		return MUMBLE_KC_##apiName

mumble_keycode_t qtKeyCodeToAPIKeyCode(unsigned int keyCode) {
	switch (keyCode) {
		MAP(Escape, ESCAPE);
		MAP(Tab, TAB);
		MAP(Backspace, BACKSPACE);
		case Qt::Key_Return:
			// Fallthrough
		case Qt::Key_Enter:
			return MUMBLE_KC_ENTER;
			MAP(Delete, DELETE);
			MAP(Print, PRINT);
			MAP(Home, HOME);
			MAP(End, END);
			MAP(Up, UP);
			MAP(Down, DOWN);
			MAP(Left, LEFT);
			MAP(Right, RIGHT);
			MAP(PageUp, PAGE_UP);
			MAP(PageDown, PAGE_DOWN);
			MAP(Shift, SHIFT);
			MAP(Control, CONTROL);
			MAP(Meta, META);
			MAP(Alt, ALT);
			MAP(AltGr, ALT_GR);
			MAP(CapsLock, CAPSLOCK);
			MAP(NumLock, NUMLOCK);
			MAP(ScrollLock, SCROLLLOCK);
			MAP(F1, F1);
			MAP(F2, F2);
			MAP(F3, F3);
			MAP(F4, F4);
			MAP(F5, F5);
			MAP(F6, F6);
			MAP(F7, F7);
			MAP(F8, F8);
			MAP(F9, F9);
			MAP(F10, F10);
			MAP(F11, F11);
			MAP(F12, F12);
			MAP(F13, F13);
			MAP(F14, F14);
			MAP(F15, F15);
			MAP(F16, F16);
			MAP(F17, F17);
			MAP(F18, F18);
			MAP(F19, F19);
		case Qt::Key_Super_L:
			// Fallthrough
		case Qt::Key_Super_R:
			return MUMBLE_KC_SUPER;
			MAP(Space, SPACE);
			MAP(Exclam, EXCLAMATION_MARK);
			MAP(QuoteDbl, DOUBLE_QUOTE);
			MAP(NumberSign, HASHTAG);
			MAP(Dollar, DOLLAR);
			MAP(Percent, PERCENT);
			MAP(Ampersand, AMPERSAND);
			MAP(Apostrophe, SINGLE_QUOTE);
			MAP(ParenLeft, OPEN_PARENTHESIS);
			MAP(ParenRight, CLOSE_PARENTHESIS);
			MAP(Asterisk, ASTERISK);
			MAP(Plus, PLUS);
			MAP(Comma, COMMA);
			MAP(Minus, MINUS);
			MAP(Period, PERIOD);
			MAP(Slash, SLASH);
			MAP(0, 0);
			MAP(1, 1);
			MAP(2, 2);
			MAP(3, 3);
			MAP(4, 4);
			MAP(5, 5);
			MAP(6, 6);
			MAP(7, 7);
			MAP(8, 8);
			MAP(9, 9);
			MAP(Colon, COLON);
			MAP(Semicolon, SEMICOLON);
			MAP(Less, LESS_THAN);
			MAP(Equal, EQUALS);
			MAP(Greater, GREATER_THAN);
			MAP(Question, QUESTION_MARK);
			MAP(At, AT_SYMBOL);
			MAP(A, A);
			MAP(B, B);
			MAP(C, C);
			MAP(D, D);
			MAP(E, E);
			MAP(F, F);
			MAP(G, G);
			MAP(H, H);
			MAP(I, I);
			MAP(J, J);
			MAP(K, K);
			MAP(L, L);
			MAP(M, M);
			MAP(N, N);
			MAP(O, O);
			MAP(P, P);
			MAP(Q, Q);
			MAP(R, R);
			MAP(S, S);
			MAP(T, T);
			MAP(U, U);
			MAP(V, V);
			MAP(W, W);
			MAP(X, X);
			MAP(Y, Y);
			MAP(Z, Z);
			MAP(BracketLeft, OPEN_BRACKET);
			MAP(BracketRight, CLOSE_BRACKET);
			MAP(Backslash, BACKSLASH);
			MAP(AsciiCircum, CIRCUMFLEX);
			MAP(Underscore, UNDERSCORE);
			MAP(BraceLeft, OPEN_BRACE);
			MAP(BraceRight, CLOSE_BRACE);
			MAP(Bar, VERTICAL_BAR);
			MAP(AsciiTilde, TILDE);
			MAP(degree, DEGREE_SIGN);
	}

	return MUMBLE_KC_INVALID;
}

#undef MAP


// Implementation of PluginData
PluginData::PluginData() : overwriteMicrophoneActivation(false) {
}

PluginData::~PluginData() {
}

PluginData &PluginData::get() {
	static PluginData *instance = new PluginData();

	return *instance;
}
} // namespace API

#undef EXIT_WITH
#undef VERIFY_PLUGIN_ID
#undef VERIFY_CONNECTION
#undef ENSURE_CONNECTION_SYNCHRONIZED
#undef UNUSED
