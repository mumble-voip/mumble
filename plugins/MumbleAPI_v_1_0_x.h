// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/// This header file contains the definition of Mumble's API

#ifndef EXTERNAL_MUMBLE_PLUGIN_API_H_
#define EXTERNAL_MUMBLE_PLUGIN_API_H_

#include "PluginComponents_v_1_0_x.h"
#include <stdint.h>


// API version
#define MUMBLE_PLUGIN_API_MAJOR_MACRO 1
#define MUMBLE_PLUGIN_API_MINOR_MACRO 0
#define MUMBLE_PLUGIN_API_PATCH_MACRO 2

const int32_t MUMBLE_PLUGIN_API_MAJOR            = MUMBLE_PLUGIN_API_MAJOR_MACRO;
const int32_t MUMBLE_PLUGIN_API_MINOR            = MUMBLE_PLUGIN_API_MINOR_MACRO;
const int32_t MUMBLE_PLUGIN_API_PATCH            = MUMBLE_PLUGIN_API_PATCH_MACRO;
const mumble_version_t MUMBLE_PLUGIN_API_VERSION = { MUMBLE_PLUGIN_API_MAJOR, MUMBLE_PLUGIN_API_MINOR,
													 MUMBLE_PLUGIN_API_PATCH };

// Create macro for casting the pointer to the API object to the proper struct.
// Note that this must only be used if the API uses MUMBLE_PLUGIN_API_VERSION of the API.
#define MUMBLE_CONCAT_HELPER(a, b) a##_##b
#define MUMBLE_CONCAT(a, b) MUMBLE_CONCAT_HELPER(a, b)
#define MUMBLE_API_STRUCT      \
	MUMBLE_CONCAT(MumbleAPI_v, \
				  MUMBLE_CONCAT(MUMBLE_PLUGIN_API_MAJOR_MACRO, MUMBLE_CONCAT(MUMBLE_PLUGIN_API_MINOR_MACRO, x)))
#define MUMBLE_API_CAST(ptrName) (*((struct MUMBLE_API_STRUCT *) ptrName))


struct MumbleAPI_v_1_0_x {
	/////////////////////////////////////////////////////////
	////////////////////// GENERAL NOTES ////////////////////
	/////////////////////////////////////////////////////////
	//
	// All functions that take in a connection as a paremeter may only be called **after** the connection
	// has finished synchronizing. The only exception from this is isConnectionSynchronized.
	//
	// Strings returned by the API are UTF-8 encoded
	// Strings passed to the API are expected to be UTF-8 encoded
	//
	// All API functions are synchronized and will be executed in Mumble's "main thread" from which most plugin
	// callbacks are called as well. Note however that an API call is BLOCKING if invoked from a different
	// thread. This means that they can cause deadlocks if used without caution. An example that will lead
	// to a deadlock is:
	// - plugin callback gets called from the main thread
	// - callback messages a separate thread to do something and waits for the action to have completed
	// - Separate thread calls an API function
	// - The function blocks and waits to be executed in the main thread which is currently blocked waiting
	// - deadlock


	// -------- Memory management --------

	/// Frees the given pointer.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param pointer The pointer to free
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *freeMemory)(mumble_plugin_id_t callerID, const void *pointer);



	// -------- Getter functions --------

	/// Gets the connection ID of the server the user is currently active on (the user's audio output is directed at).
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param[out] connection A pointer to the memory location the ID should be written to
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then it is valid to access
	/// the value of the provided pointer
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getActiveServerConnection)(mumble_plugin_id_t callerID,
																		 mumble_connection_t *connection);

	/// Checks whether the given connection has finished initializing yet.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param[out] A pointer to the boolean variable that'll hold the info whether the server has finished
	/// synchronization yet after this function has executed successfully.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *isConnectionSynchronized)(mumble_plugin_id_t callerID,
																		mumble_connection_t connection,
																		bool *synchronized);

	/// Fills in the information about the local user.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param[out] userID A pointer to the memory the user's ID shall be written to
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getLocalUserID)(mumble_plugin_id_t callerID,
															  mumble_connection_t connection, mumble_userid_t *userID);

	/// Fills in the information about the given user's name.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param userID The user's ID whose name should be obtained
	/// @param[out] userName A pointer to where the pointer to the allocated string (C-encoded) should be written to.
	/// The allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	/// allocated if this function returns STATUS_OK.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getUserName)(mumble_plugin_id_t callerID, mumble_connection_t connection,
														   mumble_userid_t userID, const char **userName);

	/// Fills in the information about the given channel's name.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param channelID The channel's ID whose name should be obtained
	/// @param[out] channelName A pointer to where the pointer to the allocated string (C-ecoded) should be written to.
	/// The allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	/// allocated if this function returns STATUS_OK.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getChannelName)(mumble_plugin_id_t callerID,
															  mumble_connection_t connection,
															  mumble_channelid_t channelID, const char **channelName);

	/// Gets an array of all users that are currently connected to the provided server. Passing a nullptr as any of the
	/// out-parameter will prevent that property to be set/allocated. If you are only interested in the user count you
	/// can thus pass nullptr as the users parameter and save time on allocating + freeing the channels-array while
	/// still getting the size out.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param[out] users A pointer to where the pointer of the allocated array shall be written. The
	/// allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	/// allocated if this function returns STATUS_OK.
	/// @param[out] userCount A pointer to where the size of the allocated user-array shall be written to
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getAllUsers)(mumble_plugin_id_t callerID, mumble_connection_t connection,
														   mumble_userid_t **users, size_t *userCount);

	/// Gets an array of all channels on the provided server. Passing a nullptr as any of the out-parameter will prevent
	/// that property to be set/allocated. If you are only interested in the channel count you can thus pass nullptr as
	/// the channels parameter and save time on allocating + freeing the channels-array while still getting the size
	/// out.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param[out] channels A pointer to where the pointer of the allocated array shall be written. The
	/// allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	/// allocated if this function returns STATUS_OK.
	/// @param[out] channelCount A pointer to where the size of the allocated channel-array shall be written to
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getAllChannels)(mumble_plugin_id_t callerID,
															  mumble_connection_t connection,
															  mumble_channelid_t **channels, size_t *channelCount);

	/// Gets the ID of the channel the given user is currently connected to.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param userID The ID of the user to search for
	/// @param[out] A pointer to where the ID of the channel shall be written
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getChannelOfUser)(mumble_plugin_id_t callerID,
																mumble_connection_t connection, mumble_userid_t userID,
																mumble_channelid_t *channel);

	/// Gets an array of all users in the specified channel.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param channelID The ID of the channel whose users shall be retrieved
	/// @param[out] userList A pointer to where the pointer of the allocated array shall be written. The allocated
	/// memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be allocated if
	/// this function returns STATUS_OK.
	/// @param[out] userCount A pointer to where the size of the allocated user-array shall be written to
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getUsersInChannel)(mumble_plugin_id_t callerID,
																 mumble_connection_t connection,
																 mumble_channelid_t channelID,
																 mumble_userid_t **userList, size_t *userCount);

	/// Gets the current transmission mode of the local user.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param[out] transmissionMode A pointer to where the transmission mode shall be written.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getLocalUserTransmissionMode)(
		mumble_plugin_id_t callerID, mumble_transmission_mode_t *transmissionMode);

	/// Checks whether the given user is currently locally muted.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param userID The ID of the user to check for
	/// @param[out] muted A pointer to where the local mute state of that user shall be written
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *isUserLocallyMuted)(mumble_plugin_id_t callerID,
																  mumble_connection_t connection,
																  mumble_userid_t userID, bool *muted);

	/// Checks whether the local user is currently muted.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param[out] muted A pointer to where the mute state of the local user shall be written
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *isLocalUserMuted)(mumble_plugin_id_t callerID, bool *muted);

	/// Checks whether the local user is currently deafened.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param[out] deafened A pointer to where the deaf state of the local user shall be written
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *isLocalUserDeafened)(mumble_plugin_id_t callerID, bool *deafened);

	/// Gets the hash of the given user (can be used to recognize users between restarts)
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param userID The ID of the user to search for
	/// @param[out] hash A pointer to where the pointer to the allocated string (C-encoded) should be written to. The
	/// allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	/// allocated if this function returns STATUS_OK.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getUserHash)(mumble_plugin_id_t callerID, mumble_connection_t connection,
														   mumble_userid_t userID, const char **hash);

	/// Gets the hash of the server for the given connection (can be used to recognize servers between restarts)
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection
	/// @param[out] hash A pointer to where the pointer to the allocated string (C-encoded) should be written to. The
	/// allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	/// allocated if this function returns STATUS_OK.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getServerHash)(mumble_plugin_id_t callerID,
															 mumble_connection_t connection, const char **hash);

	/// Gets the comment of the given user. Note that a user might have a comment configured that hasn't been
	/// synchronized to this client yet. In this case this function will return EC_UNSYNCHRONIZED_BLOB. As of now there
	/// is now way to request the synchronization to happen via the Plugin-API.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection
	/// @param userID the ID of the user whose comment should be obtained
	/// @param[out] comment A pointer to where the pointer to the allocated string (C-encoded) should be written to. The
	/// allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	/// allocated if this function returns STATUS_OK.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getUserComment)(mumble_plugin_id_t callerID,
															  mumble_connection_t connection, mumble_userid_t userID,
															  const char **comment);

	/// Gets the description of the given channel. Note that a channel might have a description configured that hasn't
	/// been synchronized to this client yet. In this case this function will return EC_UNSYNCHRONIZED_BLOB. As of now
	/// there is now way to request the synchronization to happen via the Plugin-API.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection
	/// @param channelID the ID of the channel whose comment should be obtained
	/// @param[out] description A pointer to where the pointer to the allocated string (C-encoded) should be written to.
	/// The allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will only be
	/// allocated if this function returns STATUS_OK.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getChannelDescription)(mumble_plugin_id_t callerID,
																	 mumble_connection_t connection,
																	 mumble_channelid_t channelID,
																	 const char **description);


	// -------- Request functions --------

	/// Requests Mumble to set the local user's transmission mode to the specified one. If you only need to temporarily
	/// set the transmission mode to continous, use requestMicrophoneActivationOverwrite instead as this saves you the
	/// work of restoring the previous state afterwards.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param transmissionMode The requested transmission mode
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *requestLocalUserTransmissionMode)(
		mumble_plugin_id_t callerID, mumble_transmission_mode_t transmissionMode);

	/// Requests Mumble to move the given user into the given channel
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param userID The ID of the user that shall be moved
	/// @param channelID The ID of the channel to move the user to
	/// @param password The password of the target channel (UTF-8 encoded as a C-string). Pass NULL if the target
	/// channel does not require a password for entering
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *requestUserMove)(mumble_plugin_id_t callerID,
															   mumble_connection_t connection, mumble_userid_t userID,
															   mumble_channelid_t channelID, const char *password);

	/// Requests Mumble to overwrite the microphone activation so that the microphone is always on (same as if the user
	/// had chosen the continous transmission mode). If a plugin requests this overwrite, it is responsible for
	/// deactivating the overwrite again once it is no longer required
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param activate Whether to activate the overwrite (false deactivates an existing overwrite)
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *requestMicrophoneActivationOvewrite)(mumble_plugin_id_t callerID,
																				   bool activate);

	/// Requests Mumble to set the local mute state of the given client. Note that this only affects the **local** mute
	/// state opposed to a server-mute (client is globally muted by the server) or the client's own mute-state (client
	/// has muted its microphone and thus isn't transmitting any audio). Furthermore it must be noted that muting the
	/// local user with this function does not work (it doesn't make sense). If you try to do so, this function will
	/// fail. In order to make this work, this function will also fail if the server has not finished synchronizing with
	/// the client yet. For muting the local user, use requestLocalUserMute instead.
	///
	/// @param callerID The ID of the plugin calling this function.
	/// @param connection The ID of the server-connection to use as a context
	/// @param userID The ID of the user that shall be muted
	/// @param muted Whether to locally mute the given client (opposed to unmuting it)
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *requestLocalMute)(mumble_plugin_id_t callerID,
																mumble_connection_t connection, mumble_userid_t userID,
																bool muted);

	/// Requests Mumble to set the mute state of the local user. In the UI this is referred to as "self-mute".
	///
	/// @param callerID The ID of the plugin calling this function.
	/// @param muted Whether to locally mute the local user (opposed to unmuting it)
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *requestLocalUserMute)(mumble_plugin_id_t callerID, bool muted);

	/// Requests Mumble to set the deaf state of the local user. In the UI this is referred to as "self-deaf".
	///
	/// @param callerID The ID of the plugin calling this function.
	/// @param deafened Whether to locally deafen the local user (opposed to undeafening it)
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *requestLocalUserDeaf)(mumble_plugin_id_t callerID, bool deafened);

	/// Sets the comment of the local user
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection
	/// @param comment The new comment to use (C-encoded). A subset of HTML formatting is supported.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer
	/// may be accessed
	mumble_error_t(PLUGIN_CALLING_CONVENTION *requestSetLocalUserComment)(mumble_plugin_id_t callerID,
																		  mumble_connection_t connection,
																		  const char *comment);



	// -------- Find functions --------

	/// Fills in the information about a user with the specified name, if such a user exists. The search is
	/// case-sensitive.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param userName The respective user's name
	/// @param[out] userID A pointer to the memory the user's ID shall be written to
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	/// be accessed.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *findUserByName)(mumble_plugin_id_t callerID,
															  mumble_connection_t connection, const char *userName,
															  mumble_userid_t *userID);

	/// Fills in the information about a channel with the specified name, if such a channel exists. The search is
	/// case-sensitive.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to use as a context
	/// @param channelName The respective channel's name
	/// @param[out] channelID A pointer to the memory the channel's ID shall be written to
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	/// be accessed.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *findChannelByName)(mumble_plugin_id_t callerID,
																 mumble_connection_t connection,
																 const char *channelName,
																 mumble_channelid_t *channelID);



	// -------- Settings --------

	/// Fills in the current value of the setting with the given key. Note that this function can only be used for
	/// settings whose value is a bool!
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param key The key to the desired setting
	/// @param[out] outValue A pointer to the memory the setting's value shall be written to.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	/// be accessed.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getMumbleSetting_bool)(mumble_plugin_id_t callerID,
																	 mumble_settings_key_t key, bool *outValue);

	/// Fills in the current value of the setting with the given key. Note that this function can only be used for
	/// settings whose value is an int!
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param key The key to the desired setting
	/// @param[out] outValue A pointer to the memory the setting's value shall be written to.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	/// be accessed.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getMumbleSetting_int)(mumble_plugin_id_t callerID,
																	mumble_settings_key_t key, int64_t *outValue);

	/// Fills in the current value of the setting with the given key. Note that this function can only be used for
	/// settings whose value is a double!
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param key The key to the desired setting
	/// @param[out] outValue A pointer to the memory the setting's value shall be written to.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	/// be accessed.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getMumbleSetting_double)(mumble_plugin_id_t callerID,
																	   mumble_settings_key_t key, double *outValue);

	/// Fills in the current value of the setting with the given key. Note that this function can only be used for
	/// settings whose value is a String!
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param key The key to the desired setting
	/// @param[out] outValue The memory address to which the pointer to the setting's value (the String) will be
	/// written. The allocated memory has to be freed by a call to freeMemory by the plugin eventually. The memory will
	/// only be allocated if this function returns STATUS_OK.
	/// @returns The error code. If everything went well, STATUS_OK will be returned. Only then the passed pointer may
	/// be accessed.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *getMumbleSetting_string)(mumble_plugin_id_t callerID,
																	   mumble_settings_key_t key,
																	   const char **outValue);


	/// Sets the value of the setting with the given key. Note that this function can only be used for settings whose
	/// value is a bool!
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param key The key to the desired setting
	/// @param value The value that should be set for the given setting
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *setMumbleSetting_bool)(mumble_plugin_id_t callerID,
																	 mumble_settings_key_t key, bool value);

	/// Sets the value of the setting with the given key. Note that this function can only be used for settings whose
	/// value is an int!
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param key The key to the desired setting
	/// @param value The value that should be set for the given setting
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *setMumbleSetting_int)(mumble_plugin_id_t callerID,
																	mumble_settings_key_t key, int64_t value);

	/// Sets the value of the setting with the given key. Note that this function can only be used for settings whose
	/// value is a double!
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param key The key to the desired setting
	/// @param value The value that should be set for the given setting
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *setMumbleSetting_double)(mumble_plugin_id_t callerID,
																	   mumble_settings_key_t key, double value);

	/// Sets the value of the setting with the given key. Note that this function can only be used for settings whose
	/// value is a string!
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param key The key to the desired setting
	/// @param value The value that should be set for the given setting
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *setMumbleSetting_string)(mumble_plugin_id_t callerID,
																	   mumble_settings_key_t key, const char *value);



	// -------- Miscellaneous --------

	/// Sends the provided data to the provided client(s). This kind of data can only be received by another plugin
	/// active on that client. The sent data can be seen by any active plugin on the receiving client. Therefore the
	/// sent data must not contain sensitive information or anything else that shouldn't be known by others.
	///
	/// NOTE: Messages sent via this API function are rate-limited by the server. If the rate-limit is hit, the message
	/// will be dropped without an error message. The rate-limiting is global (e.g. it doesn't matter which plugin sent
	/// the respective messages - they all count to the same limit).
	/// Therefore if you have multiple messages to send, you should consider sending them asynchronously one at a time
	/// with a little delay in between (~1 second).
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param connection The ID of the server-connection to send the data through (the server the given users are on)
	/// @param users An array of user IDs to send the data to
	/// @param userCount The size of the provided user-array
	/// @param data The data array that shall be sent. This can be an arbitrary sequence of bytes. Note that the size of
	/// is restricted to <= 1KB.
	/// @param dataLength The length of the data array
	/// @param dataID The ID of the sent data. This has to be used by the receiving plugin(s) to figure out what to do
	/// with the data. This has to be a C-encoded String. It is recommended that the ID starts with a plugin-specific
	/// prefix in order to avoid name clashes. Note that the size of this string is restricted to <= 100 bytes.
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *sendData)(mumble_plugin_id_t callerID, mumble_connection_t connection,
														const mumble_userid_t *users, size_t userCount,
														const uint8_t *data, size_t dataLength, const char *dataID);

	/// Logs the given message (typically to Mumble's console). All passed strings have to be UTF-8 encoded.
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param message The message to log
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *log)(mumble_plugin_id_t callerID, const char *message);

	/// Plays the provided sample. It uses libsndfile as a backend so the respective file format needs to be supported
	/// by it in order for this to work out (see http://www.mega-nerd.com/libsndfile/).
	///
	/// @param callerID The ID of the plugin calling this function
	/// @param samplePath The path to the sample that shall be played (UTF-8 encoded)
	/// @returns The error code. If everything went well, STATUS_OK will be returned.
	mumble_error_t(PLUGIN_CALLING_CONVENTION *playSample)(mumble_plugin_id_t callerID, const char *samplePath);
};

#endif // EXTERNAL_MUMBLE_PLUGIN_API_H_
