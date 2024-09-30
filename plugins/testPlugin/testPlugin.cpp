// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// Include the definitions of the plugin functions
// Not that this will also include ../PluginComponents.h
#include "../MumblePlugin.h"

#include <cstring>
#include <iostream>

// These are just some utility functions facilitating writing logs and the like
// The actual implementation of the plugin is further down
std::ostream &pLog() {
	std::cout << "TestPlugin: ";
	return std::cout;
}

template< typename T > void pluginLog(T log) {
	pLog() << log << std::endl;
}

std::ostream &operator<<(std::ostream &stream, const mumble_version_t version) {
	stream << "v" << version.major << "." << version.minor << "." << version.patch;
	return stream;
}


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////// PLUGIN IMPLEMENTATION ///////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

mumble_api_t mumAPI;
mumble_connection_t activeConnection;
mumble_plugin_id_t ownID;

//////////////////////////////////////////////////////////////
//////////////////// OBLIGATORY FUNCTIONS ////////////////////
//////////////////////////////////////////////////////////////
// All of the following function must be implemented in order for Mumble to load the plugin

mumble_error_t mumble_init(uint32_t id) {
	pluginLog("Initialized plugin");

	ownID = id;

	// Print the connection ID at initialization. If not connected to a server it should be -1.
	pLog() << "Plugin ID is " << id << std::endl;

	mumAPI.log(ownID, "Intitialized");

	// Little showcase for how to retrieve a setting from Mumble
	int64_t voiceHold;
	mumble_error_t error = mumAPI.getMumbleSetting_int(ownID, MUMBLE_SK_AUDIO_INPUT_VOICE_HOLD, &voiceHold);
	if (error == MUMBLE_STATUS_OK) {
		pLog() << "The voice hold is set to " << voiceHold << std::endl;
	} else {
		pluginLog("Failed to retrieve voice hold");
		pLog() << mumble_errorMessage(error) << std::endl;
	}

	// MUMBLE_STATUS_OK is a macro set to the appropriate status flag (ErrorCode)
	// If you need to return any other status have a look at the ErrorCode enum
	// inside PluginComponents.h and use one of its values
	return MUMBLE_STATUS_OK;
}

void mumble_shutdown() {
	pluginLog("Shutdown plugin");

	mumAPI.log(ownID, "Shutdown");
}

MumbleStringWrapper mumble_getName() {
	static const char *name = "TestPlugin";

	MumbleStringWrapper wrapper;
	wrapper.data = name;
	wrapper.size = strlen(name);
	// It's a static String and therefore doesn't need releasing
	wrapper.needsReleasing = false;

	return wrapper;
}

mumble_version_t mumble_getAPIVersion() {
	// MUMBLE_PLUGIN_API_VERSION will always contain the API version of the used header file (the one used to build
	// this plugin against). Thus you should always return that here in order to no have to worry about it.
	return MUMBLE_PLUGIN_API_VERSION;
}

void mumble_registerAPIFunctions(void *api) {
	// In this function the plugin is presented with a struct of function pointers that can be used
	// to interact with Mumble. Thus you should store it somewhere safe for later usage.

	// The pointer has to be cast to the respective API struct. You always have to cast to the same API version
	// as this plugin itself is using. Thus if this plugin is compiled using the API version 1.0.x (where x is an
	// arbitrary version) the pointer has to be cast to MumbleAPI_v_1_0_x (where x is a literal "x"). Furthermore the
	// struct HAS TO BE COPIED!!! Storing the pointer is not an option as it will become invalid quickly!

	// **If** you are using the same API version that is specified in the included header file (as you should), you
	// can simply use the MUMBLE_API_CAST to cast the pointer to the correct type and automatically dereferencing it.
	mumAPI = MUMBLE_API_CAST(api);

	pluginLog("Registered Mumble's API functions");
}

void mumble_releaseResource(const void *pointer) {
	std::cerr << "[ERROR]: Unexpected call to mumble_releaseResources" << std::endl;
	std::terminate();
	// This plugin doesn't use resources that are explicitly allocated (only static Strings are used). Therefore
	// we don't have to implement this function.
	//
	// If you allocated resources using malloc(), you're implementation for releasing that would be
	// free(const_cast<void *>(pointer));
	//
	// If however you allocated a resource using the new operator (C++ only), you have figure out the pointer's
	// original type and then invoke
	// delete static_cast<ActualType *>(pointer);

	// Mark as unused
	(void) pointer;
}


//////////////////////////////////////////////////////////////
///////////////////// OPTIONAL FUNCTIONS /////////////////////
//////////////////////////////////////////////////////////////
// The implementation of below functions is optional. If you don't need them, don't include them in your
// plugin

void mumble_setMumbleInfo(mumble_version_t mumbleVersion, mumble_version_t mumbleAPIVersion,
						  mumble_version_t minimumExpectedAPIVersion) {
	// this function will always be the first one to be called. Even before init()
	// In here you can get info about the Mumble version this plugin is about to run in.
	pLog() << "Mumble version: " << mumbleVersion << "; Mumble API-Version: " << mumbleAPIVersion
		   << "; Minimal expected API-Version: " << minimumExpectedAPIVersion << std::endl;
}

mumble_version_t mumble_getVersion() {
	// Mumble uses semantic versioning (see https://semver.org/)
	// { major, minor, patch }
	return { 1, 0, 0 };
}

MumbleStringWrapper mumble_getAuthor() {
	static const char *author = "MumbleDevelopers";

	MumbleStringWrapper wrapper;
	wrapper.data = author;
	wrapper.size = strlen(author);
	// It's a static String and therefore doesn't need releasing
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getDescription() {
	static const char *description = "This plugin is merely a reference implementation without any real functionality. "
									 "It shouldn't be included in the release build of Mumble.";

	MumbleStringWrapper wrapper;
	wrapper.data = description;
	wrapper.size = strlen(description);
	// It's a static String and therefore doesn't need releasing
	wrapper.needsReleasing = false;

	return wrapper;
}

uint32_t mumble_getFeatures() {
	// Tells Mumble whether this plugin delivers some known common functionality. See the PluginFeature enum in
	// PluginComponents.h for what is available.
	// If you want your plugin to deliver positional data, you'll want to return FEATURE_POSITIONAL
	return MUMBLE_FEATURE_NONE;
}

uint32_t mumble_deactivateFeatures(uint32_t features) {
	pLog() << "Asked to deactivate feature set " << features << std::endl;

	// All features that can't be deactivated should be returned
	return features;
}

uint8_t mumble_initPositionalData(const char *const *programNames, const uint64_t *programPIDs, size_t programCount) {
	std::ostream &stream = pLog() << "Got " << programCount << " programs to init positional data.";

	if (programCount > 0) {
		stream << " The first name is " << programNames[0] << " and has PID " << programPIDs[0];
	}

	stream << std::endl;

	// As this plugin doesn't provide PD, we return PDEC_ERROR_PERM to indicate that even in the future we won't do so
	// If your plugin is indeed delivering positional data but is only temporarily unable to do so, return
	// PDEC_ERROR_TEMP. and if you deliver PD and succeeded initializing return PDEC_OK.
	return MUMBLE_PDEC_ERROR_PERM;
}

#define SET_TO_ZERO(name) \
	name[0] = 0.0f;       \
	name[1] = 0.0f;       \
	name[2] = 0.0f
bool mumble_fetchPositionalData(float *avatarPos, float *avatarDir, float *avatarAxis, float *cameraPos,
								float *cameraDir, float *cameraAxis, const char **context, const char **identity) {
	pluginLog("Has been asked to deliver positional data");

	// If unable to provide positional data, this function should return false and reset all given values to 0 / empty
	// Strings
	SET_TO_ZERO(avatarPos);
	SET_TO_ZERO(avatarDir);
	SET_TO_ZERO(avatarAxis);
	SET_TO_ZERO(cameraPos);
	SET_TO_ZERO(cameraDir);
	SET_TO_ZERO(cameraAxis);
	*context  = "";
	*identity = "";

	// This function returns whether it can continue to deliver positional data
	return false;
}
#undef SET_TO_ZERO

void mumble_shutdownPositionalData() {
	pluginLog("Shutting down positional data");
}

void mumble_onServerConnected(mumble_connection_t connection) {
	activeConnection = connection;

	pLog() << "Established server-connection with ID " << connection << std::endl;

	// Use API function that'll block
	mumAPI.log(ownID, "Connected to a server");
}

void mumble_onServerDisconnected(mumble_connection_t connection) {
	activeConnection = -1;

	const char *serverHash;
	if (mumAPI.getServerHash(ownID, connection, &serverHash) == MUMBLE_STATUS_OK) {
		pLog() << "Disconnected from server-connection with ID " << connection << "(hash: " << serverHash << ")"
			   << std::endl;

		mumAPI.freeMemory(ownID, serverHash);
	} else {
		pluginLog("[ERROR]: mumble_onServerDisconnected - Unable to fetch server-hash");
	}
}

void mumble_onServerSynchronized(mumble_connection_t connection) {
	// The client has finished synchronizing with the server. Thus we can now obtain a list of all users on this server
	const char *serverHash;
	if (mumAPI.getServerHash(ownID, connection, &serverHash) == MUMBLE_STATUS_OK) {
		pLog() << "Server has finished synchronizing (ServerConnection: " << connection << "; hash: " << serverHash
			   << ")" << std::endl;

		mumAPI.freeMemory(ownID, serverHash);
	} else {
		pluginLog("[ERROR]: mumble_onServerSynchronized - Unable to fetch server-hash");
	}

	size_t userCount;
	mumble_userid_t *userIDs;

	if (mumAPI.getAllUsers(ownID, activeConnection, &userIDs, &userCount) != MUMBLE_STATUS_OK) {
		pluginLog("[ERROR]: Can't obtain user list");
		return;
	}

	mumble_userid_t localUserID;
	if (mumAPI.getLocalUserID(ownID, connection, &localUserID) != MUMBLE_STATUS_OK) {
		pluginLog("[ERROR]: Can't obtain ID of local user");
		return;
	}

	pLog() << "There are " << userCount << " users on this server. Their names are:" << std::endl;

	for (size_t i = 0; i < userCount; i++) {
		const char *userName;
		if (mumAPI.getUserName(ownID, connection, userIDs[i], &userName) != MUMBLE_STATUS_OK) {
			pLog() << "<Unable to fetch user name>" << std::endl;
			continue;
		}

		const char *userHash;
		if (mumAPI.getUserHash(ownID, connection, userIDs[i], &userHash) != MUMBLE_STATUS_OK) {
			pluginLog("<Unable to get user-hash>");
		}

		pLog() << "\t" << userName << " (" << userHash << ")" << std::endl;

		// Mute the user "MuteMe" if this is not the name of the local user (in which case it'd fail)
		if (userIDs[i] != localUserID && std::strcmp(userName, "MuteMe") == 0) {
			if (mumAPI.requestLocalMute(ownID, connection, userIDs[i], true) != MUMBLE_STATUS_OK) {
				pluginLog("[ERROR]: Failed at muting user \"MuteMe\"!");
			}
		}

		mumAPI.freeMemory(ownID, userName);
		mumAPI.freeMemory(ownID, userHash);
	}

	mumAPI.freeMemory(ownID, userIDs);

	size_t channelCount;
	mumble_channelid_t *channelIDs;

	if (mumAPI.getAllChannels(ownID, activeConnection, &channelIDs, &channelCount) != MUMBLE_STATUS_OK) {
		pluginLog("[ERROR]: Failed to fetch channel list!");
		return;
	}

	pLog() << "There are " << channelCount << " channels on this server" << std::endl;

	mumAPI.freeMemory(ownID, channelIDs);

	mumble_userid_t localUser;
	if (mumAPI.getLocalUserID(ownID, activeConnection, &localUser) != MUMBLE_STATUS_OK) {
		pluginLog("Failed to retrieve local user ID");
		return;
	}

	if (mumAPI.sendData(ownID, activeConnection, &localUser, 1, reinterpret_cast< const uint8_t * >("Just a test"), 12,
						"testMsg")
		== MUMBLE_STATUS_OK) {
		pluginLog("Successfully sent plugin message");

		// Try break the rate-limiter for plugin messages
		for (int i = 0; i < 40; i++) {
			std::string data = "Rate-limit message #" + std::to_string(i);

			mumAPI.sendData(ownID, activeConnection, &localUser, 1, reinterpret_cast< const uint8_t * >(data.c_str()),
							data.size(), "testMsg");
		}
	} else {
		pluginLog("Failed at sending message");
	}

	if (mumAPI.requestSetLocalUserComment(ownID, connection,
										  "This user has the TestPlugin enabled - <b>hand over a cookie!</b>")
		!= MUMBLE_STATUS_OK) {
		pluginLog("Failed at setting the local user's comment");
	}
}

void mumble_onChannelEntered(mumble_connection_t connection, mumble_userid_t userID,
							 mumble_channelid_t previousChannelID, mumble_channelid_t newChannelID) {
	std::ostream &stream = pLog() << "User with ID " << userID << " entered channel with ID " << newChannelID << ".";

	// negative ID means that there was no previous channel (e.g. because the user just connected)
	if (previousChannelID >= 0) {
		stream << " Came from channel with ID " << previousChannelID << ".";
	}

	stream << " (ServerConnection: " << connection << ")" << std::endl;
}

void mumble_onChannelExited(mumble_connection_t connection, mumble_userid_t userID, mumble_channelid_t channelID) {
	pLog() << "User with ID " << userID << " has left channel with ID " << channelID
		   << ". (ServerConnection: " << connection << ")" << std::endl;
}

void mumble_onUserTalkingStateChanged(mumble_connection_t connection, mumble_userid_t userID,
									  mumble_talking_state_t talkingState) {
	std::ostream &stream = pLog() << "User with ID " << userID << " changed his talking state to ";

	// The possible values are contained in the TalkingState enum inside PluginComponent.h
	switch (talkingState) {
		case MUMBLE_TS_INVALID:
			stream << "Invalid";
			break;
		case MUMBLE_TS_PASSIVE:
			stream << "Passive";
			break;
		case MUMBLE_TS_TALKING:
			stream << "Talking";
			break;
		case MUMBLE_TS_WHISPERING:
			stream << "Whispering";
			break;
		case MUMBLE_TS_SHOUTING:
			stream << "Shouting";
			break;
		default:
			stream << "Unknown (" << talkingState << ")";
	}

	stream << ". (ServerConnection: " << connection << ")" << std::endl;
}

bool mumble_onAudioInput(short *inputPCM, uint32_t sampleCount, uint16_t channelCount, uint32_t sampleRate,
						 bool isSpeech) {
	// pLog() << "Audio input with " << channelCount << " channels and " << sampleCount << " samples per channel
	// encountered. IsSpeech: "
	//	<< isSpeech << " Sample rate is " << sampleRate << "Hz" << std::endl;

	// mark variables as unused
	(void) inputPCM;
	(void) sampleCount;
	(void) channelCount;
	(void) sampleRate;
	(void) isSpeech;

	// This function returns whether it has modified the audio stream
	return false;
}

bool mumble_onAudioSourceFetched(float *outputPCM, uint32_t sampleCount, uint16_t channelCount, uint32_t sampleRate,
								 bool isSpeech, mumble_userid_t userID) {
	std::ostream &stream = pLog() << "Audio output source with " << channelCount << " channels and " << sampleCount
								  << " samples per channel "
								  << "(" << sampleRate << " Hz) fetched.";

	if (isSpeech) {
		stream << " The output is speech from user with ID " << userID << ".";
	}

	stream << std::endl;

	// Mark ouputPCM as unused
	(void) outputPCM;

	// This function returns whether it has modified the audio stream
	return false;
}

bool mumble_onAudioOutputAboutToPlay(float *outputPCM, uint32_t sampleCount, uint16_t channelCount,
									 uint32_t sampleRate) {
	// pLog() << "The resulting audio output has " << channelCount << " channels with " << sampleCount << " samples per
	// channel (" sampleRate << " Hz)" << std::endl;

	// mark variables as unused
	(void) outputPCM;
	(void) sampleCount;
	(void) channelCount;
	(void) sampleRate;

	// This function returns whether it has modified the audio stream
	return false;
}

bool mumble_onReceiveData(mumble_connection_t connection, mumble_userid_t sender, const uint8_t *data,
						  size_t dataLength, const char *dataID) {
	pLog() << "Received data with ID \"" << dataID << "\" from user with ID " << sender << ". Its length is "
		   << dataLength << ". (ServerConnection:" << connection << ")" << std::endl;

	if (std::strcmp(dataID, "testMsg") == 0) {
		// We know that data is only a normal C-encoded String, so the reinterpret_cast is safe
		pLog() << "The received data: " << reinterpret_cast< const char * >(data) << std::endl;
	}

	// This function returns whether it has processed the data (preventing further plugins from seeing it)
	return false;
}

void mumble_onUserAdded(mumble_connection_t connection, mumble_userid_t userID) {
	pLog() << "Added user with ID " << userID << " (ServerConnection: " << connection << ")" << std::endl;
}

void mumble_onUserRemoved(mumble_connection_t connection, mumble_userid_t userID) {
	pLog() << "Removed user with ID " << userID << " (ServerConnection: " << connection << ")" << std::endl;
}

void mumble_onChannelAdded(mumble_connection_t connection, mumble_channelid_t channelID) {
	pLog() << "Added channel with ID " << channelID << " (ServerConnection: " << connection << ")" << std::endl;
}

void mumble_onChannelRemoved(mumble_connection_t connection, mumble_channelid_t channelID) {
	pLog() << "Removed channel with ID " << channelID << " (ServerConnection: " << connection << ")" << std::endl;
}

void mumble_onChannelRenamed(mumble_connection_t connection, mumble_channelid_t channelID) {
	pLog() << "Renamed channel with ID " << channelID << " (ServerConnection: " << connection << ")" << std::endl;
}

void mumble_onKeyEvent(uint32_t keyCode, bool wasPress) {
	pLog() << "Encountered key " << (wasPress ? "press" : "release") << " of key with code " << keyCode << std::endl;
}

bool mumble_hasUpdate() {
	// This plugin never has an update
	return false;
}

MumbleStringWrapper mumble_getUpdateDownloadURL() {
	static const char *url = "https://i.dont.exist/testplugin.zip";

	MumbleStringWrapper wrapper;
	wrapper.data = url;
	wrapper.size = strlen(url);
	// It's a static String and therefore doesn't need releasing
	wrapper.needsReleasing = false;

	return wrapper;
}
