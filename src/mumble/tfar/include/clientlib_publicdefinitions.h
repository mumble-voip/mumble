#ifndef CLIENTLIB_PUBLICDEFINITIONS_H
#define CLIENTLIB_PUBLICDEFINITIONS_H

enum Visibility {
	ENTER_VISIBILITY = 0,
	RETAIN_VISIBILITY,
	LEAVE_VISIBILITY
};

enum ConnectStatus {
	STATUS_DISCONNECTED = 0,       //There is no activity to the server, this is the default value
	STATUS_CONNECTING,             //We are trying to connect, we haven't got a clientID yet, we haven't been accepted by the server
	STATUS_CONNECTED,              //The server has accepted us, we can talk and hear and we got a clientID, but we don't have the channels and clients yet, we can get server infos (welcome msg etc.)
	STATUS_CONNECTION_ESTABLISHING,//we are CONNECTED and we are visible
	STATUS_CONNECTION_ESTABLISHED, //we are CONNECTED and we have the client and channels available
};

enum LocalTestMode {
    TEST_MODE_OFF = 0,
    TEST_MODE_VOICE_LOCAL_ONLY = 1,
    TEST_MODE_VOICE_LOCAL_AND_REMOTE = 2,
};

#endif //CLIENTLIB_PUBLICDEFINITIONS_H
