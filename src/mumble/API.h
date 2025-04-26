// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_API_H_
#define MUMBLE_MUMBLE_API_H_

// In here Mumble API structs for all versions are defined
#include "MumbleAPI_structs.h"

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <QObject>

namespace API {

class APIPromise {
public:
	using lock_guard_t = std::unique_lock< std::recursive_mutex >;

	APIPromise() = default;

	void set_value(mumble_error_t value);

	std::future< mumble_error_t > get_future();

	lock_guard_t lock();

	bool isCancelled() const;
	void cancel();

protected:
	std::promise< mumble_error_t > m_promise;
	mutable std::recursive_mutex m_lock;
	bool m_cancelled = false;
};

using api_future_t  = std::future< mumble_error_t >;
using api_promise_t = APIPromise;

/// A "curator" that will keep track of allocated resources and how to delete them
struct MumbleAPICurator {
	struct Entry {
		/// The function used to delete the corresponding pointer
		std::function< void(const void *) > m_deleter;
		/// The ID of the plugin the resource pointed at was allocated for
		mumble_plugin_id_t m_pluginID;
		/// The name of the API function the resource pointed to was allocated in
		/// NOTE: This must only ever be a pointer to a String literal.
		const char *m_sourceFunctionName;
	};

	std::unordered_map< const void *, Entry > m_entries;

	~MumbleAPICurator();
};

/// This object contains the actual API implementation. It also takes care of synchronizing API calls
/// with Mumble's main thread so that plugins can call them from an arbitrary thread without causing
/// issues.
/// This class is a singleton as a way to be able to write C function wrappers for the member functions
/// that are needed for passing to the plugins.
class MumbleAPI : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(MumbleAPI)

public:
	static MumbleAPI &get();

public slots:
	// The description of the functions is provided in MumbleAPI.h

	// Note that every slot is synchronized and is therefore guaranteed to be executed in the main
	// thread. For the synchronization strategy see below.
	void freeMemory_v_1_0_x(mumble_plugin_id_t callerID, const void *ptr, std::shared_ptr< api_promise_t > promise);
	void getActiveServerConnection_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t *connection,
										   std::shared_ptr< api_promise_t > promise);
	void isConnectionSynchronized_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
										  bool *synchronized, std::shared_ptr< api_promise_t > promise);
	void getLocalUserID_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t *userID,
								std::shared_ptr< api_promise_t > promise);
	void getUserName_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID,
							 const char **name, std::shared_ptr< api_promise_t > promise);
	void getChannelName_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
								mumble_channelid_t channelID, const char **name,
								std::shared_ptr< api_promise_t > promise);
	void getAllUsers_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t **users,
							 std::size_t *userCount, std::shared_ptr< api_promise_t > promise);
	void getAllChannels_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
								mumble_channelid_t **channels, std::size_t *channelCount,
								std::shared_ptr< api_promise_t > promise);
	void getChannelOfUser_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID,
								  mumble_channelid_t *channelID, std::shared_ptr< api_promise_t > promise);
	void getUsersInChannel_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
								   mumble_channelid_t channelID, mumble_userid_t **users, std::size_t *userCount,
								   std::shared_ptr< api_promise_t > promise);
	void getLocalUserTransmissionMode_v_1_0_x(mumble_plugin_id_t callerID, mumble_transmission_mode_t *transmissionMode,
											  std::shared_ptr< api_promise_t > promise);
	void isUserLocallyMuted_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID,
									bool *muted, std::shared_ptr< api_promise_t > promise);
	void isLocalUserMuted_v_1_0_x(mumble_plugin_id_t callerID, bool *muted, std::shared_ptr< api_promise_t > promise);
	void isLocalUserDeafened_v_1_0_x(mumble_plugin_id_t callerID, bool *deafened,
									 std::shared_ptr< api_promise_t > promise);
	void getUserHash_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID,
							 const char **hash, std::shared_ptr< api_promise_t > promise);
	void getServerHash_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, const char **hash,
							   std::shared_ptr< api_promise_t > promise);
	void requestLocalUserTransmissionMode_v_1_0_x(mumble_plugin_id_t callerID,
												  mumble_transmission_mode_t transmissionMode,
												  std::shared_ptr< api_promise_t > promise);
	void getUserComment_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID,
								const char **comment, std::shared_ptr< api_promise_t > promise);
	void getChannelDescription_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
									   mumble_channelid_t channelID, const char **description,
									   std::shared_ptr< api_promise_t > promise);
	void requestUserMove_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID,
								 mumble_channelid_t channelID, const char *password,
								 std::shared_ptr< api_promise_t > promise);
	void requestMicrophoneActivationOverwrite_v_1_0_x(mumble_plugin_id_t callerID, bool activate,
													  std::shared_ptr< api_promise_t > promise);
	void requestLocalMute_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID,
								  bool muted, std::shared_ptr< api_promise_t > promise);
	void requestLocalUserMute_v_1_0_x(mumble_plugin_id_t callerID, bool muted,
									  std::shared_ptr< api_promise_t > promise);
	void requestLocalUserDeaf_v_1_0_x(mumble_plugin_id_t callerID, bool deafened,
									  std::shared_ptr< api_promise_t > promise);
	void requestSetLocalUserComment_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection,
											const char *comment, std::shared_ptr< api_promise_t > promise);
	void findUserByName_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, const char *userName,
								mumble_userid_t *userID, std::shared_ptr< api_promise_t > promise);
	void findChannelByName_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, const char *channelName,
								   mumble_channelid_t *channelID, std::shared_ptr< api_promise_t > promise);
	void getMumbleSetting_bool_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, bool *outValue,
									   std::shared_ptr< api_promise_t > promise);
	void getMumbleSetting_int_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, int64_t *outValue,
									  std::shared_ptr< api_promise_t > promise);
	void getMumbleSetting_double_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, double *outValue,
										 std::shared_ptr< api_promise_t > promise);
	void getMumbleSetting_string_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, const char **outValue,
										 std::shared_ptr< api_promise_t > promise);
	void setMumbleSetting_bool_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, bool value,
									   std::shared_ptr< api_promise_t > promise);
	void setMumbleSetting_int_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, int64_t value,
									  std::shared_ptr< api_promise_t > promise);
	void setMumbleSetting_double_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, double value,
										 std::shared_ptr< api_promise_t > promise);
	void setMumbleSetting_string_v_1_0_x(mumble_plugin_id_t callerID, mumble_settings_key_t key, const char *value,
										 std::shared_ptr< api_promise_t > promise);
	void sendData_v_1_0_x(mumble_plugin_id_t callerID, mumble_connection_t connection, const mumble_userid_t *users,
						  std::size_t userCount, const uint8_t *data, std::size_t dataLength, const char *dataID,
						  std::shared_ptr< api_promise_t > promise);
	void log_v_1_0_x(mumble_plugin_id_t callerID, const char *message, std::shared_ptr< api_promise_t > promise);
	void playSample_v_1_0_x(mumble_plugin_id_t callerID, const char *samplePath,
							std::shared_ptr< api_promise_t > promise);
	void playSample_v_1_2_x(mumble_plugin_id_t callerID, const char *samplePath, float volume,
							std::shared_ptr< api_promise_t > promise);


private:
	MumbleAPI();

	MumbleAPICurator m_curator;
};

/// @returns The Mumble API struct (v1.0.x)
MumbleAPI_v_1_0_x getMumbleAPI_v_1_0_x();

/// @returns The Mumble API struct (v1.2.x)
MumbleAPI_v_1_2_x getMumbleAPI_v_1_2_x();

/// Converts from the Qt key-encoding to the API's key encoding.
///
/// @param keyCode The Qt key-code that shall be converted
/// @returns The converted key code or KC_INVALID if conversion failed
mumble_keycode_t qtKeyCodeToAPIKeyCode(unsigned int keyCode);

/// A class holding non-permanent data set by plugins. Non-permanent means that this data
/// will not be stored between restarts.
/// All member field should be atomic in order to be thread-safe
class PluginData {
public:
	/// Constructor
	PluginData();
	/// Destructor
	~PluginData();

	/// A flag indicating whether a plugin has requested the microphone to be permanently on (mirroring the
	/// behaviour of the continuous transmission mode.
	std::atomic_bool overwriteMicrophoneActivation;

	/// @returns A reference to the PluginData singleton
	static PluginData &get();
}; // class PluginData
} // namespace API


// Declare the meta-types that we require in order for the API to work
Q_DECLARE_METATYPE(mumble_settings_key_t)
Q_DECLARE_METATYPE(mumble_settings_key_t *)
Q_DECLARE_METATYPE(mumble_transmission_mode_t)
Q_DECLARE_METATYPE(mumble_transmission_mode_t *)
Q_DECLARE_METATYPE(std::shared_ptr< API::api_promise_t >)

//////////////////////////////////////////////////////////////
///////////// SYNCHRONIZATION STRATEGY ///////////////////////
//////////////////////////////////////////////////////////////

/**
 * Every API function call checks whether it is being called from the main thread. If it is,
 * it continues executing as usual. If it is not however, it uses Qt's signal/slot mechanism
 * to schedule the respective function to be run in the main thread in the next iteration of
 * the event loop.
 * In order to synchronize with the calling thread, the return value (error code) of these
 * functions is "returned" as a promise. Thus by accessing the exit code via the corresponding
 * future, the calling thread is blocked until the function has been executed in the main thread
 * (and thereby set the exit code once it is done allowing the calling thread to unblock).
 */

#endif
