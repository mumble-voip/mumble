#ifndef TS3_FUNCTIONS_H
#define TS3_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "clientlib_publicdefinitions.h"
#include "public_definitions.h"
#include "plugin_definitions.h"

/* Functions exported to plugin from main binary */
struct TS3Functions {
	unsigned int (*getClientLibVersion)(char** result);
	unsigned int (*getClientLibVersionNumber)(uint64* result);
	unsigned int (*spawnNewServerConnectionHandler)(int port, uint64* result);
	unsigned int (*destroyServerConnectionHandler)(uint64 serverConnectionHandlerID);

	/* Error handling */
	unsigned int (*getErrorMessage)(unsigned int errorCode, char** error);

	/* Memory management */
	unsigned int (*freeMemory)(void* pointer);

	/* Logging */
	unsigned int (*logMessage)(const char* logMessage, enum LogLevel severity, const char* channel, uint64 logID);

	/* Sound */
	unsigned int (*getPlaybackDeviceList)(const char* modeID, char**** result);
	unsigned int (*getPlaybackModeList)(char*** result);
	unsigned int (*getCaptureDeviceList)(const char* modeID, char**** result);
	unsigned int (*getCaptureModeList)(char*** result);
	unsigned int (*getDefaultPlaybackDevice)(const char* modeID, char*** result);
	unsigned int (*getDefaultPlayBackMode)(char** result);
	unsigned int (*getDefaultCaptureDevice)(const char* modeID, char*** result);
	unsigned int (*getDefaultCaptureMode)(char** result);
	unsigned int (*openPlaybackDevice)(uint64 serverConnectionHandlerID, const char* modeID, const char* playbackDevice);
	unsigned int (*openCaptureDevice)(uint64 serverConnectionHandlerID, const char* modeID, const char* captureDevice);
	unsigned int (*getCurrentPlaybackDeviceName)(uint64 serverConnectionHandlerID, char** result, int* isDefault);
	unsigned int (*getCurrentPlayBackMode)(uint64 serverConnectionHandlerID, char** result);
	unsigned int (*getCurrentCaptureDeviceName)(uint64 serverConnectionHandlerID, char** result, int* isDefault);
	unsigned int (*getCurrentCaptureMode)(uint64 serverConnectionHandlerID, char** result);
	unsigned int (*initiateGracefulPlaybackShutdown)(uint64 serverConnectionHandlerID);
	unsigned int (*closePlaybackDevice)(uint64 serverConnectionHandlerID);
	unsigned int (*closeCaptureDevice)(uint64 serverConnectionHandlerID);
	unsigned int (*activateCaptureDevice)(uint64 serverConnectionHandlerID);
	unsigned int (*playWaveFileHandle)(uint64 serverConnectionHandlerID, const char* path, int loop, uint64* waveHandle);
	unsigned int (*pauseWaveFileHandle)(uint64 serverConnectionHandlerID, uint64 waveHandle, int pause);
	unsigned int (*closeWaveFileHandle)(uint64 serverConnectionHandlerID, uint64 waveHandle);
	unsigned int (*playWaveFile)(uint64 serverConnectionHandlerID, const char* path);
	unsigned int (*registerCustomDevice)(const char* deviceID, const char* deviceDisplayName, int capFrequency, int capChannels, int playFrequency, int playChannels);
	unsigned int (*unregisterCustomDevice)(const char* deviceID);
	unsigned int (*processCustomCaptureData)(const char* deviceName, const short* buffer, int samples);
	unsigned int (*acquireCustomPlaybackData)(const char* deviceName, short* buffer, int samples);

	/* Preprocessor */
	unsigned int (*getPreProcessorInfoValueFloat)(uint64 serverConnectionHandlerID, const char* ident, float* result);
	unsigned int (*getPreProcessorConfigValue)(uint64 serverConnectionHandlerID, const char* ident, char** result);
	unsigned int (*setPreProcessorConfigValue)(uint64 serverConnectionHandlerID, const char* ident, const char* value);

	/* Encoder */
	unsigned int (*getEncodeConfigValue)(uint64 serverConnectionHandlerID, const char* ident, char** result);

	/* Playback */
	unsigned int (*getPlaybackConfigValueAsFloat)(uint64 serverConnectionHandlerID, const char* ident, float* result);
	unsigned int (*setPlaybackConfigValue)(uint64 serverConnectionHandlerID, const char* ident, const char* value);
	unsigned int (*setClientVolumeModifier)(uint64 serverConnectionHandlerID, anyID clientID, float value);

	/* Recording */
	unsigned int (*startVoiceRecording)(uint64 serverConnectionHandlerID);
	unsigned int (*stopVoiceRecording)(uint64 serverConnectionHandlerID);

	/* 3d sound positioning */
	unsigned int (*systemset3DListenerAttributes) (uint64 serverConnectionHandlerID, const TS3_VECTOR* position, const TS3_VECTOR* forward, const TS3_VECTOR* up);
	unsigned int (*set3DWaveAttributes) (uint64 serverConnectionHandlerID, uint64 waveHandle, const TS3_VECTOR* position);
	unsigned int (*systemset3DSettings) (uint64 serverConnectionHandlerID, float distanceFactor, float rolloffScale);
	unsigned int (*channelset3DAttributes) (uint64 serverConnectionHandlerID, anyID clientID, const TS3_VECTOR* position);

	/* Interaction with the server */
	unsigned int (*startConnection)(uint64 serverConnectionHandlerID, const char* identity, const char* ip, unsigned int port, const char* nickname,
	                                const char** defaultChannelArray, const char* defaultChannelPassword, const char* serverPassword);
	unsigned int (*stopConnection)(uint64 serverConnectionHandlerID, const char* quitMessage);
	unsigned int (*requestClientMove)(uint64 serverConnectionHandlerID, anyID clientID, uint64 newChannelID, const char* password, const char* returnCode);
	unsigned int (*requestClientVariables)(uint64 serverConnectionHandlerID, anyID clientID, const char* returnCode);
	unsigned int (*requestClientKickFromChannel)(uint64 serverConnectionHandlerID, anyID clientID, const char* kickReason, const char* returnCode);
	unsigned int (*requestClientKickFromServer)(uint64 serverConnectionHandlerID, anyID clientID, const char* kickReason, const char* returnCode);
	unsigned int (*requestChannelDelete)(uint64 serverConnectionHandlerID, uint64 channelID, int force, const char* returnCode);
	unsigned int (*requestChannelMove)(uint64 serverConnectionHandlerID, uint64 channelID, uint64 newChannelParentID, uint64 newChannelOrder, const char* returnCode);
	unsigned int (*requestSendPrivateTextMsg)(uint64 serverConnectionHandlerID, const char* message, anyID targetClientID, const char* returnCode);
	unsigned int (*requestSendChannelTextMsg)(uint64 serverConnectionHandlerID, const char* message, uint64 targetChannelID, const char* returnCode);
	unsigned int (*requestSendServerTextMsg)(uint64 serverConnectionHandlerID, const char* message, const char* returnCode);
	unsigned int (*requestConnectionInfo)(uint64 serverConnectionHandlerID, anyID clientID, const char* returnCode);
	unsigned int (*requestClientSetWhisperList)(uint64 serverConnectionHandlerID, anyID clientID, const uint64* targetChannelIDArray, const anyID* targetClientIDArray, const char* returnCode);
	unsigned int (*requestChannelSubscribe)(uint64 serverConnectionHandlerID, const uint64* channelIDArray, const char* returnCode);
	unsigned int (*requestChannelSubscribeAll)(uint64 serverConnectionHandlerID, const char* returnCode);
	unsigned int (*requestChannelUnsubscribe)(uint64 serverConnectionHandlerID, const uint64* channelIDArray, const char* returnCode);
	unsigned int (*requestChannelUnsubscribeAll)(uint64 serverConnectionHandlerID, const char* returnCode);
	unsigned int (*requestChannelDescription)(uint64 serverConnectionHandlerID, uint64 channelID, const char* returnCode);
	unsigned int (*requestMuteClients)(uint64 serverConnectionHandlerID, const anyID* clientIDArray, const char* returnCode);
	unsigned int (*requestUnmuteClients)(uint64 serverConnectionHandlerID, const anyID* clientIDArray, const char* returnCode);
	unsigned int (*requestClientPoke)(uint64 serverConnectionHandlerID, anyID clientID, const char* message, const char* returnCode);
	unsigned int (*requestClientIDs)(uint64 serverConnectionHandlerID, const char* clientUniqueIdentifier, const char* returnCode);
	unsigned int (*clientChatClosed)(uint64 serverConnectionHandlerID, const char* clientUniqueIdentifier, anyID clientID, const char* returnCode);
	unsigned int (*clientChatComposing)(uint64 serverConnectionHandlerID, anyID clientID, const char* returnCode);
	unsigned int (*requestServerTemporaryPasswordAdd)(uint64 serverConnectionHandlerID, const char* password, const char* description, uint64 duration, uint64 targetChannelID, const char* targetChannelPW, const char* returnCode);
	unsigned int (*requestServerTemporaryPasswordDel)(uint64 serverConnectionHandlerID, const char* password, const char* returnCode);
	unsigned int (*requestServerTemporaryPasswordList)(uint64 serverConnectionHandlerID, const char* returnCode);

	/* Access clientlib information */

	/* Query own client ID */
	unsigned int (*getClientID)(uint64 serverConnectionHandlerID, anyID* result);

	/* Client info */
	unsigned int (*getClientSelfVariableAsInt)(uint64 serverConnectionHandlerID, size_t flag, int* result);
	unsigned int (*getClientSelfVariableAsString)(uint64 serverConnectionHandlerID, size_t flag, char** result);
	unsigned int (*setClientSelfVariableAsInt)(uint64 serverConnectionHandlerID, size_t flag, int value);
	unsigned int (*setClientSelfVariableAsString)(uint64 serverConnectionHandlerID, size_t flag, const char* value);
	unsigned int (*flushClientSelfUpdates)(uint64 serverConnectionHandlerID, const char* returnCode);
	unsigned int (*getClientVariableAsInt)(uint64 serverConnectionHandlerID, anyID clientID, size_t flag, int* result);
	unsigned int (*getClientVariableAsUInt64)(uint64 serverConnectionHandlerID, anyID clientID, size_t flag, uint64* result);
	unsigned int (*getClientVariableAsString)(uint64 serverConnectionHandlerID, anyID clientID, size_t flag, char** result);
	unsigned int (*getClientList)(uint64 serverConnectionHandlerID, anyID** result); 
	unsigned int (*getChannelOfClient)(uint64 serverConnectionHandlerID, anyID clientID, uint64* result);

	/* Channel info */
	unsigned int (*getChannelVariableAsInt)(uint64 serverConnectionHandlerID, uint64 channelID, size_t flag, int* result);
	unsigned int (*getChannelVariableAsUInt64)(uint64 serverConnectionHandlerID, uint64 channelID, size_t flag, uint64* result);
	unsigned int (*getChannelVariableAsString)(uint64 serverConnectionHandlerID, uint64 channelID, size_t flag, char** result);
	unsigned int (*getChannelIDFromChannelNames)(uint64 serverConnectionHandlerID, char** channelNameArray, uint64* result);
	unsigned int (*setChannelVariableAsInt)(uint64 serverConnectionHandlerID, uint64 channelID, size_t flag, int value);
	unsigned int (*setChannelVariableAsUInt64)(uint64 serverConnectionHandlerID, uint64 channelID, size_t flag, uint64 value);
	unsigned int (*setChannelVariableAsString)(uint64 serverConnectionHandlerID, uint64 channelID, size_t flag, const char* value);
	unsigned int (*flushChannelUpdates)(uint64 serverConnectionHandlerID, uint64 channelID, const char* returnCode);
	unsigned int (*flushChannelCreation)(uint64 serverConnectionHandlerID, uint64 channelParentID, const char* returnCode);
	unsigned int (*getChannelList)(uint64 serverConnectionHandlerID, uint64** result);
	unsigned int (*getChannelClientList)(uint64 serverConnectionHandlerID, uint64 channelID,  anyID** result);
	unsigned int (*getParentChannelOfChannel)(uint64 serverConnectionHandlerID, uint64 channelID, uint64* result);

	/* Server info */
	unsigned int (*getServerConnectionHandlerList)(uint64** result);
	unsigned int (*getServerVariableAsInt)(uint64 serverConnectionHandlerID, size_t flag, int* result);
	unsigned int (*getServerVariableAsUInt64)(uint64 serverConnectionHandlerID, size_t flag, uint64* result);
	unsigned int (*getServerVariableAsString)(uint64 serverConnectionHandlerID, size_t flag, char** result);
	unsigned int (*requestServerVariables)(uint64 serverConnectionHandlerID);

	/* Connection info */
	unsigned int (*getConnectionStatus)(uint64 serverConnectionHandlerID, int* result);
	unsigned int (*getConnectionVariableAsUInt64)(uint64 serverConnectionHandlerID, anyID clientID, size_t flag, uint64* result);
	unsigned int (*getConnectionVariableAsDouble)(uint64 serverConnectionHandlerID, anyID clientID, size_t flag, double* result);
	unsigned int (*getConnectionVariableAsString)(uint64 serverConnectionHandlerID, anyID clientID, size_t flag, char** result);
	unsigned int (*cleanUpConnectionInfo)(uint64 serverConnectionHandlerID, anyID clientID);

	/* Client related */
	unsigned int (*requestClientDBIDfromUID)(uint64 serverConnectionHandlerID, const char* clientUniqueIdentifier, const char* returnCode);
	unsigned int (*requestClientNamefromUID)(uint64 serverConnectionHandlerID, const char* clientUniqueIdentifier, const char* returnCode);
	unsigned int (*requestClientNamefromDBID)(uint64 serverConnectionHandlerID, uint64 clientDatabaseID, const char* returnCode);
	unsigned int (*requestClientEditDescription)(uint64 serverConnectionHandlerID, anyID clientID, const char* clientDescription, const char* returnCode);
	unsigned int (*requestClientSetIsTalker)(uint64 serverConnectionHandlerID, anyID clientID, int isTalker, const char* returnCode);
	unsigned int (*requestIsTalker)(uint64 serverConnectionHandlerID, int isTalkerRequest, const char* isTalkerRequestMessage, const char* returnCode);

	/* Plugin related */
	unsigned int (*requestSendClientQueryCommand)(uint64 serverConnectionHandlerID, const char* command, const char* returnCode);

	/* Filetransfer */
	unsigned int (*getTransferFileName)(anyID transferID, char** result);
	unsigned int (*getTransferFilePath)(anyID transferID, char** result);
	unsigned int (*getTransferFileSize)(anyID transferID, uint64* result);
	unsigned int (*getTransferFileSizeDone)(anyID transferID, uint64* result);
	unsigned int (*isTransferSender)(anyID transferID, int* result);  /* 1 == upload, 0 == download */
	unsigned int (*getTransferStatus)(anyID transferID, int* result);
	unsigned int (*getCurrentTransferSpeed)(anyID transferID, float* result);
	unsigned int (*getAverageTransferSpeed)(anyID transferID, float* result);
	unsigned int (*getTransferRunTime)(anyID transferID, uint64* result);
	unsigned int (*sendFile)(uint64 serverConnectionHandlerID, uint64 channelID, const char* channelPW, const char* file, int overwrite, int resume, const char* sourceDirectory, anyID* result, const char* returnCode);
	unsigned int (*requestFile)(uint64 serverConnectionHandlerID, uint64 channelID, const char* channelPW, const char* file, int overwrite, int resume, const char* destinationDirectory, anyID* result, const char* returnCode);
	unsigned int (*haltTransfer)(uint64 serverConnectionHandlerID, anyID transferID, int deleteUnfinishedFile, const char* returnCode);
	unsigned int (*requestFileList)(uint64 serverConnectionHandlerID, uint64 channelID, const char* channelPW, const char* path, const char* returnCode);
	unsigned int (*requestFileInfo)(uint64 serverConnectionHandlerID, uint64 channelID, const char* channelPW, const char* file, const char* returnCode);
	unsigned int (*requestDeleteFile)(uint64 serverConnectionHandlerID, uint64 channelID, const char* channelPW, const char** file, const char* returnCode);
	unsigned int (*requestCreateDirectory)(uint64 serverConnectionHandlerID, uint64 channelID, const char* channelPW, const char* directoryPath, const char* returnCode);
	unsigned int (*requestRenameFile)(uint64 serverConnectionHandlerID, uint64 fromChannelID, const char* channelPW, uint64 toChannelID, const char* toChannelPW, const char* oldFile, const char* newFile, const char* returnCode);

	/* Offline message management */
	unsigned int (*requestMessageAdd)(uint64 serverConnectionHandlerID, const char* toClientUID, const char* subject, const char* message, const char* returnCode);
	unsigned int (*requestMessageDel)(uint64 serverConnectionHandlerID, uint64 messageID, const char* returnCode);
	unsigned int (*requestMessageGet)(uint64 serverConnectionHandlerID, uint64 messageID, const char* returnCode);
	unsigned int (*requestMessageList)(uint64 serverConnectionHandlerID, const char* returnCode);
	unsigned int (*requestMessageUpdateFlag)(uint64 serverConnectionHandlerID, uint64 messageID, int flag, const char* returnCode);

	/* Interacting with the server - confirming passwords */
	unsigned int (*verifyServerPassword)(uint64 serverConnectionHandlerID, const char* serverPassword, const char* returnCode);
	unsigned int (*verifyChannelPassword)(uint64 serverConnectionHandlerID, uint64 channelID, const char* channelPassword, const char* returnCode);

	/* Interacting with the server - banning */
	unsigned int (*banclient)(uint64 serverConnectionHandlerID, anyID clientID, uint64 timeInSeconds, const char* banReason, const char* returnCode);
	unsigned int (*banadd)(uint64 serverConnectionHandlerID, const char* ipRegExp, const char* nameRegexp, const char* uniqueIdentity, uint64 timeInSeconds, const char* banReason, const char* returnCode);
	unsigned int (*banclientdbid)(uint64 serverConnectionHandlerID, uint64 clientDBID, uint64 timeInSeconds, const char* banReason, const char* returnCode);
	unsigned int (*bandel)(uint64 serverConnectionHandlerID, uint64 banID, const char* returnCode);
	unsigned int (*bandelall)(uint64 serverConnectionHandlerID, const char* returnCode);
	unsigned int (*requestBanList)(uint64 serverConnectionHandlerID, const char* returnCode);

	/* Interacting with the server - complain */
	unsigned int (*requestComplainAdd)(uint64 serverConnectionHandlerID, uint64 targetClientDatabaseID, const char* complainReason, const char* returnCode);
	unsigned int (*requestComplainDel)(uint64 serverConnectionHandlerID, uint64 targetClientDatabaseID, uint64 fromClientDatabaseID, const char* returnCode);
	unsigned int (*requestComplainDelAll)(uint64 serverConnectionHandlerID, uint64 targetClientDatabaseID, const char* returnCode);
	unsigned int (*requestComplainList)(uint64 serverConnectionHandlerID, uint64 targetClientDatabaseID, const char* returnCode);

	/* Permissions */
	unsigned int (*requestServerGroupList)(uint64 serverConnectionHandlerID, const char* returnCode);
	unsigned int (*requestServerGroupAdd)(uint64 serverConnectionHandlerID, const char* groupName, int groupType, const char* returnCode);
	unsigned int (*requestServerGroupDel)(uint64 serverConnectionHandlerID, uint64 serverGroupID, int force, const char* returnCode);
	unsigned int (*requestServerGroupAddClient)(uint64 serverConnectionHandlerID, uint64 serverGroupID, uint64 clientDatabaseID, const char* returnCode);
	unsigned int (*requestServerGroupDelClient)(uint64 serverConnectionHandlerID, uint64 serverGroupID, uint64 clientDatabaseID, const char* returnCode);
	unsigned int (*requestServerGroupsByClientID)(uint64 serverConnectionHandlerID, uint64 clientDatabaseID, const char* returnCode);
	unsigned int (*requestServerGroupAddPerm)(uint64 serverConnectionHandlerID, uint64 serverGroupID, int continueonerror, const unsigned int* permissionIDArray, const int* permissionValueArray, const int* permissionNegatedArray, const int* permissionSkipArray, int arraySize, const char* returnCode);
	unsigned int (*requestServerGroupDelPerm)(uint64 serverConnectionHandlerID, uint64 serverGroupID, int continueOnError, const unsigned int* permissionIDArray, int arraySize, const char* returnCode);
	unsigned int (*requestServerGroupPermList)(uint64 serverConnectionHandlerID, uint64 serverGroupID, const char* returnCode);
	unsigned int (*requestServerGroupClientList)(uint64 serverConnectionHandlerID, uint64 serverGroupID, int withNames, const char* returnCode);
	unsigned int (*requestChannelGroupList)(uint64 serverConnectionHandlerID, const char* returnCode);
	unsigned int (*requestChannelGroupAdd)(uint64 serverConnectionHandlerID, const char* groupName, int groupType, const char* returnCode);
	unsigned int (*requestChannelGroupDel)(uint64 serverConnectionHandlerID, uint64 channelGroupID, int force, const char* returnCode);
	unsigned int (*requestChannelGroupAddPerm)(uint64 serverConnectionHandlerID, uint64 channelGroupID, int continueonerror, const unsigned int* permissionIDArray, const int* permissionValueArray, int arraySize, const char* returnCode);
	unsigned int (*requestChannelGroupDelPerm)(uint64 serverConnectionHandlerID, uint64 channelGroupID, int continueOnError, const unsigned int* permissionIDArray, int arraySize, const char* returnCode);
	unsigned int (*requestChannelGroupPermList)(uint64 serverConnectionHandlerID, uint64 channelGroupID, const char* returnCode);
	unsigned int (*requestSetClientChannelGroup)(uint64 serverConnectionHandlerID, const uint64* channelGroupIDArray, const uint64* channelIDArray, const uint64* clientDatabaseIDArray, int arraySize, const char* returnCode);
	unsigned int (*requestChannelAddPerm)(uint64 serverConnectionHandlerID, uint64 channelID, const unsigned int* permissionIDArray, const int* permissionValueArray, int arraySize, const char* returnCode);
	unsigned int (*requestChannelDelPerm)(uint64 serverConnectionHandlerID, uint64 channelID, const unsigned int* permissionIDArray, int arraySize, const char* returnCode);
	unsigned int (*requestChannelPermList)(uint64 serverConnectionHandlerID, uint64 channelID, const char* returnCode);
	unsigned int (*requestClientAddPerm)(uint64 serverConnectionHandlerID, uint64 clientDatabaseID, const unsigned int* permissionIDArray, const int* permissionValueArray, const int* permissionSkipArray, int arraySize, const char* returnCode);
	unsigned int (*requestClientDelPerm)(uint64 serverConnectionHandlerID, uint64 clientDatabaseID, const unsigned int* permissionIDArray, int arraySize, const char* returnCode);
	unsigned int (*requestClientPermList)(uint64 serverConnectionHandlerID, uint64 clientDatabaseID, const char* returnCode);
	unsigned int (*requestChannelClientAddPerm)(uint64 serverConnectionHandlerID, uint64 channelID, uint64 clientDatabaseID, const unsigned int* permissionIDArray, const int* permissionValueArray, int arraySize, const char* returnCode);
	unsigned int (*requestChannelClientDelPerm)(uint64 serverConnectionHandlerID, uint64 channelID, uint64 clientDatabaseID, const unsigned int* permissionIDArray, int arraySize, const char* returnCode);
	unsigned int (*requestChannelClientPermList)(uint64 serverConnectionHandlerID, uint64 channelID, uint64 clientDatabaseID, const char* returnCode);
	unsigned int (*privilegeKeyUse)(uint64 serverConnectionHandler, const char* tokenKey, const char* returnCode);
	unsigned int (*requestPermissionList)(uint64 serverConnectionHandler, const char* returnCode);
	unsigned int (*requestPermissionOverview)(uint64 serverConnectionHandler, uint64 clientDBID, uint64 channelID, const char* returnCode);

	/* Helper Functions */
	unsigned int (*clientPropertyStringToFlag)(const char* clientPropertyString, size_t* resultFlag);
	unsigned int (*channelPropertyStringToFlag)(const char* channelPropertyString, size_t* resultFlag);
	unsigned int (*serverPropertyStringToFlag)(const char* serverPropertyString, size_t* resultFlag);

	/* Client functions */
	void         (*getAppPath)(char* path, size_t maxLen);
	void         (*getResourcesPath)(char* path, size_t maxLen);
	void         (*getConfigPath)(char* path, size_t maxLen);
	void         (*getPluginPath)(char* path, size_t maxLen);
	uint64       (*getCurrentServerConnectionHandlerID)();
	void         (*printMessage)(uint64 serverConnectionHandlerID, const char* message, enum PluginMessageTarget messageTarget);
	void         (*printMessageToCurrentTab)(const char* message);
	void         (*urlsToBB)(const char* text, char* result, size_t maxLen);
	void         (*sendPluginCommand)(uint64 serverConnectionHandlerID, const char* pluginID, const char* command, int targetMode, const anyID* targetIDs, const char* returnCode);
	void         (*getDirectories)(const char* path, char* result, size_t maxLen);
	unsigned int (*getServerConnectInfo)(uint64 scHandlerID, char* host, unsigned short* port, char* password, size_t maxLen);
	unsigned int (*getChannelConnectInfo)(uint64 scHandlerID, uint64 channelID, char* path, char* password, size_t maxLen);
	void         (*createReturnCode)(const char* pluginID, char* returnCode, size_t maxLen);
	unsigned int (*requestInfoUpdate)(uint64 scHandlerID, enum PluginItemType itemType, uint64 itemID);
	uint64       (*getServerVersion)(uint64 scHandlerID);
	unsigned int (*isWhispering)(uint64 scHandlerID, anyID clientID, int* result);
	unsigned int (*isReceivingWhisper)(uint64 scHandlerID, anyID clientID, int* result);
	unsigned int (*getAvatar)(uint64 scHandlerID, anyID clientID, char* result, size_t maxLen);
	void         (*setPluginMenuEnabled)(const char* pluginID, int menuID, int enabled);
	void         (*showHotkeySetup)();
	void         (*requestHotkeyInputDialog)(const char* pluginID, const char* keyword, int isDown, void* qParentWindow);
	unsigned int (*getHotkeyFromKeyword)(const char* pluginID, const char** keywords, char** hotkeys, size_t arrayLen, size_t hotkeyBufSize);
	unsigned int (*getClientDisplayName)(uint64 scHandlerID, anyID clientID, char* result, size_t maxLen);
	unsigned int (*getBookmarkList)(struct PluginBookmarkList** list);
	unsigned int (*getProfileList)(enum PluginGuiProfile profile, int* defaultProfileIdx, char*** result);
	unsigned int (*guiConnect)(enum PluginConnectTab connectTab, const char* serverLabel, const char* serverAddress, const char* serverPassword, const char* nickname, const char* channel, const char* channelPassword, const char* captureProfile, const char* playbackProfile, const char* hotkeyProfile, const char* soundProfile, const char* userIdentity, const char* oneTimeKey, const char* phoneticName, uint64* scHandlerID);
	unsigned int (*guiConnectBookmark)(enum PluginConnectTab connectTab, const char* bookmarkuuid, uint64* scHandlerID);
	unsigned int (*createBookmark)(const char* bookmarkuuid, const char* serverLabel, const char* serverAddress, const char* serverPassword, const char* nickname, const char* channel, const char* channelPassword, const char* captureProfile, const char* playbackProfile, const char* hotkeyProfile, const char* soundProfile, const char* uniqueUserId, const char* oneTimeKey, const char* phoneticName);
	unsigned int (*getPermissionIDByName)(uint64 serverConnectionHandlerID, const char* permissionName, unsigned int* result);
	unsigned int (*getClientNeededPermission)(uint64 serverConnectionHandlerID, const char* permissionName, int* result);
};

#ifdef __cplusplus
}
#endif

#endif
