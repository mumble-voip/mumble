// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/// This header file specifies the Mumble plugin interface

#ifndef EXTERNAL_MUMBLE_PLUGIN_H_
#define EXTERNAL_MUMBLE_PLUGIN_H_

#include "MumbleAPI_v_1_0_x.h"
#include "PluginComponents_v_1_0_x.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__) && !defined(__MINGW32__) // GCC on Unix-like systems
#	define PLUGIN_EXPORT __attribute__((visibility("default")))
#elif defined(_MSC_VER)
#	define PLUGIN_EXPORT __declspec(dllexport)
#elif defined(__MINGW32__)
#	define PLUGIN_EXPORT __attribute__((dllexport))
#else
#	error No PLUGIN_EXPORT definition available
#endif


#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////// MANDATORY FUNCTIONS ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/// Gets called right after loading the plugin in order to let the plugin initialize.
///
/// Registers the ID of this plugin.
/// @param id The ID for this plugin. This is the ID Mumble will reference this plugin with
/// and by which this plugin can identify itself when communicating with Mumble.
/// @returns The status of the initialization. If everything went fine, return STATUS_OK
PLUGIN_EXPORT mumble_error_t PLUGIN_CALLING_CONVENTION mumble_init(mumble_plugin_id_t id);

/// Gets called when unloading the plugin in order to allow it to clean up after itself.
/// Note that it is still safe to call API functions from within this callback.
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_shutdown();

/// Gets the name of the plugin.
///
/// NOTE: This function may be called without the plugin being loaded
///
/// @returns A String-wrapper containing the requested name
PLUGIN_EXPORT struct MumbleStringWrapper PLUGIN_CALLING_CONVENTION mumble_getName();

/// Gets the Version of the plugin-API this plugin intends to use.
/// Mumble will decide whether this plugin is loadable or not based on the return value of this function.
///
/// NOTE: This function may be called without the plugin being loaded
///
/// @returns The respective API Version
PLUGIN_EXPORT mumble_version_t PLUGIN_CALLING_CONVENTION mumble_getAPIVersion();

/// Provides the MumbleAPI struct to the plugin. This struct contains function pointers that can be used
/// to interact with the Mumble client. It is up to the plugin to store this struct somewhere if it wants to make use
/// of it at some point.
///
/// NOTE: This function may be called without the plugin being loaded
///
/// @param api A pointer to the MumbleAPI struct. The API struct must be cast to the version corresponding to the
/// user API version. If your plugin is e.g. using the 1.0.x API, then you have to cast this pointer to
/// MumbleAPI_v_1_0_x. Note also that you **must not store this pointer**. It will become invalid. Therefore
/// you have to copy the struct in order to use it later on.
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_registerAPIFunctions(void *apiStruct);

/// Releases the resource pointed to by the given pointer. If the respective resource has been allocated before,
/// this would be the time to free/delete it.
/// The resources processed by this functions are only those that have been specifically allocated in order to return
/// them in one of the plugin functions to Mumble (e.g. the String returned by mumble_getName) and has nothing to do
/// with your plugin's internal resource management.
/// In short: Only resources passed from the plugin to Mumble via a return value may be processed by this function.
///
/// NOTE1: This function may be called without the plugin being loaded
///
/// NOTE2: that the pointer might be pointing to memory that had to be allocated without the plugin being loaded.
/// Therefore you should be very sure that there'll be another callback in which you want to free this memory,
/// should you decide to not do it here (which is hereby explicitly advised against).
///
/// NOTE3: The pointer is const as Mumble won't mess with the memory allocated by the plugin (no modifications).
/// Nontheless this function is explicitly responsible for freeing the respective memory parts. If the memory has
/// been allocated using malloc(), it needs to be freed using free() which requires a const-cast. If however the
/// memory has been created using the new operator you have to cast the pointer back to its original type and then
/// use the  delete operator on it (no const-cast necessary in this case).
/// See https://stackoverflow.com/questions/2819535/unable-to-free-const-pointers-in-c
/// and https://stackoverflow.com/questions/941832/is-it-safe-to-delete-a-void-pointer
///
/// @param pointer The pointer to the memory that needs free-ing
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_releaseResource(const void *pointer);



//////////////////////////////////////////////////////////////////////////////////
///////////////////////////// GENERAL FUNCTIONS //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/// Tells the plugin some basic information about the Mumble client loading it.
/// This function will be the first one that is being called on this plugin - even before it is decided whether to load
/// the plugin at all.
///
/// @param mumbleVersion The Version of the Mumble client
/// @param mumbleAPIVersion The Version of the plugin-API the Mumble client runs with
/// @param minimumExpectedAPIVersion The minimum Version the Mumble clients expects this plugin to meet in order to load
/// it
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_setMumbleInfo(mumble_version_t mumbleVersion,
																  mumble_version_t mumbleAPIVersion,
																  mumble_version_t minimumExpectedAPIVersion);

/// Gets the Version of this plugin
///
/// NOTE: This function may be called without the plugin being loaded
///
/// @returns The plugin's version
PLUGIN_EXPORT mumble_version_t PLUGIN_CALLING_CONVENTION mumble_getVersion();

/// Gets the name of the plugin author(s).
///
/// NOTE: This function may be called without the plugin being loaded
///
/// @returns A String-wrapper containing the requested author name(s)
PLUGIN_EXPORT struct MumbleStringWrapper PLUGIN_CALLING_CONVENTION mumble_getAuthor();

/// Gets the description of the plugin.
///
/// NOTE: This function may be called without the plugin being loaded
///
/// @returns A String-wrapper containing the requested description
PLUGIN_EXPORT struct MumbleStringWrapper PLUGIN_CALLING_CONVENTION mumble_getDescription();

/// Gets the feature set of this plugin. The feature set is described by bitwise or'ing the elements of the
/// Mumble_PluginFeature enum together.
///
/// NOTE: This function may be called without the plugin being loaded
///
/// @returns The feature set of this plugin
PLUGIN_EXPORT uint32_t PLUGIN_CALLING_CONVENTION mumble_getFeatures();

/// Requests this plugin to deactivate the given (sub)set of provided features.
/// If this is not possible, the features that can't be deactivated shall be returned by this function.
///
/// Example (check if FEATURE_POSITIONAL shall be deactivated):
/// @code
/// if (features & FEATURE_POSITIONAL) {
/// // positional shall be deactivated
/// };
/// @endcode
///
/// @param features The feature set that shall be deactivated
/// @returns The feature set that can't be disabled (bitwise or'ed). If all requested features can be disabled, return
/// FEATURE_NONE. If none of the requested features can be disabled return the unmodified features parameter.
PLUGIN_EXPORT uint32_t PLUGIN_CALLING_CONVENTION mumble_deactivateFeatures(uint32_t features);



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////// POSITIONAL DATA /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// If this plugin wants to provide positional data, ALL functions of this category
// have to be implemented

/// Indicates that Mumble wants to use this plugin to request positional data. Therefore it should check whether it is
/// currently able to do so and allocate memory that is needed for that process. As a parameter this function gets an
/// array of names and an array of PIDs. They are of same length and the PID at index i belongs to a program whose name
/// is listed at index i in the "name-array".
///
/// @param programNames An array of pointers to the program names
/// @param programPIDs An array of the corresponding program PIDs
/// @param programCount The length of programNames and programPIDs
/// @returns The error code. If everything went fine PDEC_OK shall be returned. In that case Mumble will start
/// frequently calling fetchPositionalData. If this returns anything but PDEC_OK, Mumble will assume that the plugin is
/// (currently) uncapable of providing positional data. In this case this function must not have allocated any memory
/// that needs to be cleaned up later on. Depending on the returned error code, Mumble might try to call this function
/// again at some point.
PLUGIN_EXPORT uint8_t PLUGIN_CALLING_CONVENTION mumble_initPositionalData(const char *const *programNames,
																		  const uint64_t *programPIDs,
																		  size_t programCount);

/// Retrieves the positional data. If no data can be fetched, set all float-vectors to 0 and return false.
///
/// @param[out] avatarPos A float-array of size 3 representing the cartesian position of the player/avatar in the ingame
/// world. One unit represents one meter of distance.
/// @param[out] avatarDir A float-array of size 3 representing the cartesian direction-vector of the player/avatar
/// ingame (where it is facing).
/// @param[out] avatarAxis A float-array of size 3 representing the vector pointing from the toes of the character to
/// its head. One unit represents one meter of distance.
/// @param[out] cameraPos A float-array of size 3 representing the cartesian position of the camera in the ingame world.
/// One unit represents one meter of distance.
/// @param[out] cameraDir A float-array of size 3 representing the cartesian direction-vector of the camera ingame
/// (where it is facing).
/// @param[out] cameraAxis A float-array of size 3 representing a vector from the bottom of the camera to its top. One
/// unit represents one meter of distance.
/// @param[out] context A pointer to where the pointer to a C-encoded string storing the context of the provided
/// positional data shall be written. This context should include information about the server (and team) the player is
/// on. Only players with identical context will be able to hear each other's audio. The returned pointer has to remain
/// valid until the next invokation of this function or until shutdownPositionalData is called.
/// @param[out] identity A pointer to where the pointer to a C-encoded string storing the identity of the player shall
/// be written. It can be polled by external scripts from the server and should uniquely identify the player in the
/// game. The pointer has to remain valid until the next invokation of this function or until shutdownPositionalData is
/// called.
/// @returns Whether this plugin can continue delivering positional data. If this function returns false,
/// shutdownPositionalData will be called.
PLUGIN_EXPORT bool PLUGIN_CALLING_CONVENTION mumble_fetchPositionalData(float *avatarPos, float *avatarDir,
																		float *avatarAxis, float *cameraPos,
																		float *cameraDir, float *cameraAxis,
																		const char **context, const char **identity);

/// Indicates that this plugin will not be asked for positional data any longer. Thus any memory allocated for this
/// purpose should be freed at this point.
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_shutdownPositionalData();



//////////////////////////////////////////////////////////////////////////////////
////////////////////// EVENTHANDLERS / CALLBACK FUNCTIONS ////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/// Called when connecting to a server.
/// Note that in most cases you'll want to use mumble_onServerSynchronized instead.
/// Note also that this callback will be called from a DIFFERENT THREAD!
///
/// @param connection The ID of the newly established server-connection
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onServerConnected(mumble_connection_t connection);

/// Called when disconnecting from a server.
/// Note that this callback is called from a DIFFERENT THREAD!
///
/// @param connection The ID of the server-connection that has been terminated
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onServerDisconnected(mumble_connection_t connection);

/// Called when the client has finished synchronizing with the server
///
/// @param connection The ID of the server-connection that has been terminated
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onServerSynchronized(mumble_connection_t connection);

/// Called whenever any user on the server enters a channel
/// This function will also be called when freshly connecting to a server as each user on that
/// server needs to be "added" to the respective channel as far as the local client is concerned.
///
/// @param connection The ID of the server-connection this event is connected to
/// @param userID The ID of the user this event has been triggered for
/// @param previousChannelID The ID of the chanel the user is coming from. Negative IDs indicate that there is no
/// previous channel (e.g. the user freshly connected to the server) or the channel isn't available because of any other
/// reason.
/// @param newChannelID The ID of the channel the user has entered. If the ID is negative, the new channel could not be
/// retrieved. This means that the ID is invalid.
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onChannelEntered(mumble_connection_t connection,
																	 mumble_userid_t userID,
																	 mumble_channelid_t previousChannelID,
																	 mumble_channelid_t newChannelID);

/// Called whenever a user leaves a channel.
/// This includes a client disconnecting from the server as this will also lead to the user not being in that channel
/// anymore.
///
/// @param connection The ID of the server-connection this event is connected to
/// @param userID The ID of the user that left the channel
/// @param channelID The ID of the channel the user left. If the ID is negative, the channel could not be retrieved.
/// This means that the ID is invalid.
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onChannelExited(mumble_connection_t connection,
																	mumble_userid_t userID,
																	mumble_channelid_t channelID);

/// Called when any user changes his/her talking state.
///
/// @param connection The ID of the server-connection this event is connected to
/// @param userID The ID of the user whose talking state has been changed
/// @param talkingState The new TalkingState the user has switched to.
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onUserTalkingStateChanged(mumble_connection_t connection,
																			  mumble_userid_t userID,
																			  mumble_talking_state_t talkingState);

/// Called whenever there is audio input.
/// Note that this callback will be called from the AUDIO THREAD.
/// Note also that blocking this callback will cause Mumble's audio processing to get suspended.
///
/// @param inputPCM A pointer to a short-array holding the pulse-code-modulation (PCM) representing the audio input. Its
/// length is sampleCount * channelCount. The PCM format for stereo input is [LRLRLR...] where L and R are samples of
/// the left and right channel respectively.
/// @param sampleCount The amount of sample points per channel
/// @param channelCount The amount of channels in the audio
/// @param sampleRate The used sample rate in Hz
/// @param isSpeech A boolean flag indicating whether Mumble considers the input as part of speech (instead of
/// background noise)
/// @returns Whether this callback has modified the audio input-array
PLUGIN_EXPORT bool PLUGIN_CALLING_CONVENTION mumble_onAudioInput(short *inputPCM, uint32_t sampleCount,
																 uint16_t channelCount, uint32_t sampleRate,
																 bool isSpeech);

/// Called whenever Mumble fetches data from an active audio source (could be a voice packet or a playing sample).
/// The provided audio buffer is the raw buffer without any processing applied to it yet.
/// Note that this callback will be called from the AUDIO THREAD.
/// Note also that blocking this callback will cause Mumble's audio processing to get suspended.
///
/// @param outputPCM A pointer to a float-array holding the pulse-code-modulation (PCM) representing the audio output.
/// Its length is sampleCount * channelCount. The PCM format for stereo output is [LRLRLR...] where L and R are samples
/// of the left and right channel respectively.
/// @param sampleCount The amount of sample points per channel
/// @param channelCount The amount of channels in the audio
/// @param sampleRate The used sample rate in Hz
/// @param isSpeech Whether this audio belongs to a received voice packet (and will thus (most likely) contain speech)
/// @param userID If isSpeech is true, this contains the ID of the user this voice packet belongs to. If isSpeech is
/// false, the content of this parameter is unspecified and should not be accessed
/// @returns Whether this callback has modified the audio output-array
PLUGIN_EXPORT bool PLUGIN_CALLING_CONVENTION mumble_onAudioSourceFetched(float *outputPCM, uint32_t sampleCount,
																		 uint16_t channelCount, uint32_t sampleRate,
																		 bool isSpeech, mumble_userid_t userID);

/// Called whenever the fully mixed and processed audio is about to be handed to the audio backend (about to be played).
/// Note that this happens immediately before Mumble clips the audio buffer.
/// Note that this callback will be called from the AUDIO THREAD.
/// Note also that blocking this callback will cause Mumble's audio processing to get suspended.
///
/// @param outputPCM A pointer to a float-array holding the pulse-code-modulation (PCM) representing the audio output.
/// Its length is sampleCount * channelCount. The PCM format for stereo output is [LRLRLR...] where L and R are samples
/// of the left and right channel respectively.
/// @param sampleCount The amount of sample points per channel
/// @param channelCount The amount of channels in the audio
/// @param sampleRate The used sample rate in Hz
/// @returns Whether this callback has modified the audio output-array
PLUGIN_EXPORT bool PLUGIN_CALLING_CONVENTION mumble_onAudioOutputAboutToPlay(float *outputPCM, uint32_t sampleCount,
																			 uint16_t channelCount,
																			 uint32_t sampleRate);

/// Called whenever data has been received that has been sent by a plugin. This data should only be processed by the
/// intended plugin. For this reason a dataID is provided that should be used to determine whether the data is intended
/// for this plugin or not. As soon as the data has been processed, no further plugins will be notified about it.
///
/// @param connection The ID of the server-connection the data is coming from
/// @param sender The ID of the user whose client's plugin has sent the data
/// @param data The sent data array. This can be an arbitrary sequence of bytes.
/// @param dataLength The length of the data array
/// @param dataID The ID of this data (C-encoded)
/// @return Whether the given data has been processed by this plugin
PLUGIN_EXPORT bool PLUGIN_CALLING_CONVENTION mumble_onReceiveData(mumble_connection_t connection,
																  mumble_userid_t sender, const uint8_t *data,
																  size_t dataLength, const char *dataID);

/// Called when a new user gets added to the user model. This is the case when that new user freshly connects to the
/// server the local user is on but also when the local user connects to a server other clients are already connected to
/// (in this case this method will be called for every client already on that server).
///
/// @param connection An object used to identify the current connection
/// @param userID The ID of the user that has been added

PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onUserAdded(mumble_connection_t connection, mumble_userid_t userID);

/// Called when a user gets removed from the user model. This is the case when that user disconnects from the server the
/// local user is on but also when the local user disconnects from a server other clients are connected to (in this case
/// this method will be called for every client on that server).
///
/// @param connection An object used to identify the current connection
/// @param userID The ID of the user that has been removed
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onUserRemoved(mumble_connection_t connection,
																  mumble_userid_t userID);

/// Called when a new channel gets added to the user model. This is the case when a new channel is created on the server
/// the local user is on but also when the local user connects to a server that contains channels other than the
/// root-channel (in this case this method will be called for ever non-root channel on that server).
///
/// @param connection An object used to identify the current connection
/// @param channelID The ID of the channel that has been added
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onChannelAdded(mumble_connection_t connection,
																   mumble_channelid_t channelID);

/// Called when a channel gets removed from the user model. This is the case when a channel is removed on the server the
/// local user is on but also when the local user disconnects from a server that contains channels other than the
/// root-channel (in this case this method will be called for ever non-root channel on that server).
///
/// @param connection An object used to identify the current connection
/// @param channelID The ID of the channel that has been removed
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onChannelRemoved(mumble_connection_t connection,
																	 mumble_channelid_t channelID);

/// Called when a channel gets renamed. This also applies when a new channel is created (thus assigning it an initial
/// name is also considered renaming).
///
/// @param connection An object used to identify the current connection
/// @param channelID The ID of the channel that has been renamed
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onChannelRenamed(mumble_connection_t connection,
																	 mumble_channelid_t channelID);

/// Called when a key has been pressed or released while Mumble has keyboard focus.
/// Note that this callback will only work if the user has explicitly given permission to monitor keyboard
/// events for this plugin. Thus if you want to use this callback, make sure your users know that they have to
/// enable that.
///
/// @param keyCode The key code of the respective key. The character codes are defined
/// via the Mumble_KeyCode enum. For printable 7-bit ASCII characters these codes conform
/// to the ASCII code-page with the only difference that case is not distinguished. Therefore
/// always the upper-case letter code will be used for letters.
/// @param wasPres Whether the respective key has been pressed (instead of released)
PLUGIN_EXPORT void PLUGIN_CALLING_CONVENTION mumble_onKeyEvent(uint32_t keyCode, bool wasPress);



//////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PLUGIN UPDATES ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/// This function is used to determine whether the plugin can find an update for itself that is available for download.
///
/// NOTE: This function may be called without the plugin being loaded
///
/// @return Whether the plugin was able to find an update for itself
PLUGIN_EXPORT bool PLUGIN_CALLING_CONVENTION mumble_hasUpdate();

/// This function is used to retrieve the URL for downloading the newer/updated version of this plugin.
///
/// NOTE: This function may be called without the plugin being loaded
///
/// @returns A String-wrapper containing the requested URL
PLUGIN_EXPORT struct MumbleStringWrapper PLUGIN_CALLING_CONVENTION mumble_getUpdateDownloadURL();


#ifdef __cplusplus
}
#endif


#endif
