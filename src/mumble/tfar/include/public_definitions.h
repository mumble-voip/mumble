#ifndef PUBLIC_DEFINITIONS_H
#define PUBLIC_DEFINITIONS_H

#include "teamlog/logtypes.h"

//limited length, measured in characters
#define TS3_MAX_SIZE_CHANNEL_NAME 40
#define TS3_MAX_SIZE_VIRTUALSERVER_NAME 64
#define TS3_MAX_SIZE_CLIENT_NICKNAME 64
#define TS3_MIN_SIZE_CLIENT_NICKNAME 3
#define TS3_MAX_SIZE_REASON_MESSAGE 80

//limited length, measured in bytes (utf8 encoded)
#define TS3_MAX_SIZE_TEXTMESSAGE 1024
#define TS3_MAX_SIZE_CHANNEL_TOPIC 255
#define TS3_MAX_SIZE_CHANNEL_DESCRIPTION 8192
#define TS3_MAX_SIZE_VIRTUALSERVER_WELCOMEMESSAGE 1024

//minimum amount of seconds before a clientID that was in use can be assigned to a new client
#define TS3_MIN_SECONDS_CLIENTID_REUSE 300

// MUMBLE-TFAR: anyID widened from 16 to 32 bit — Mumble session IDs are uint32.
#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
	typedef unsigned __int32 anyID;
	typedef unsigned __int64 uint64;
	#ifdef BUILDING_DLL
		#define EXPORTDLL __declspec(dllexport)
	#else
		#define EXPORTDLL
	#endif
#else
	#include <stdint.h>
	typedef uint32_t anyID;
	typedef uint64_t uint64;
	#ifdef BUILDING_DLL
		#define EXPORTDLL __attribute__ ((visibility("default")))
	#else
		#define EXPORTDLL
	#endif
#endif

enum TalkStatus {
	STATUS_NOT_TALKING = 0,
	STATUS_TALKING = 1,
	STATUS_TALKING_WHILE_DISABLED = 2,
};

enum CodecType {
	CODEC_SPEEX_NARROWBAND = 0,   //mono,   16bit,  8kHz, bitrate dependent on the quality setting
	CODEC_SPEEX_WIDEBAND,         //mono,   16bit, 16kHz, bitrate dependent on the quality setting
	CODEC_SPEEX_ULTRAWIDEBAND,    //mono,   16bit, 32kHz, bitrate dependent on the quality setting
	CODEC_CELT_MONO,              //mono,   16bit, 48kHz, bitrate dependent on the quality setting
	CODEC_OPUS_VOICE,             //mono,   16bit, 48khz, bitrate dependent on the quality setting, optimized for voice
	CODEC_OPUS_MUSIC,             //stereo, 16bit, 48khz, bitrate dependent on the quality setting, optimized for music
};

enum CodecEncryptionMode {
	CODEC_ENCRYPTION_PER_CHANNEL = 0,
	CODEC_ENCRYPTION_FORCED_OFF,
	CODEC_ENCRYPTION_FORCED_ON,
};

enum TextMessageTargetMode {
	TextMessageTarget_CLIENT=1,
	TextMessageTarget_CHANNEL,
	TextMessageTarget_SERVER,
	TextMessageTarget_MAX
};

enum MuteInputStatus {
	MUTEINPUT_NONE = 0,
	MUTEINPUT_MUTED,
};

enum MuteOutputStatus {
	MUTEOUTPUT_NONE = 0,
	MUTEOUTPUT_MUTED,
};

enum HardwareInputStatus {
	HARDWAREINPUT_DISABLED = 0,
	HARDWAREINPUT_ENABLED,
};

enum HardwareOutputStatus {
	HARDWAREOUTPUT_DISABLED = 0,
	HARDWAREOUTPUT_ENABLED,
};

enum InputDeactivationStatus {
	INPUT_ACTIVE = 0,
	INPUT_DEACTIVATED = 1,
};

enum ReasonIdentifier {
	REASON_NONE                              = 0,  //no reason data
	REASON_MOVED                             = 1,  //{SectionInvoker}
	REASON_SUBSCRIPTION                      = 2,  //no reason data
	REASON_LOST_CONNECTION                   = 3,  //reasonmsg=reason
	REASON_KICK_CHANNEL                      = 4,  //{SectionInvoker} reasonmsg=reason               //{SectionInvoker} is only added server->client
	REASON_KICK_SERVER                       = 5,  //{SectionInvoker} reasonmsg=reason               //{SectionInvoker} is only added server->client
	REASON_KICK_SERVER_BAN                   = 6,  //{SectionInvoker} reasonmsg=reason bantime=time  //{SectionInvoker} is only added server->client
	REASON_SERVERSTOP                        = 7,  //reasonmsg=reason
	REASON_CLIENTDISCONNECT                  = 8,  //reasonmsg=reason
	REASON_CHANNELUPDATE                     = 9,  //no reason data
	REASON_CHANNELEDIT                       = 10, //{SectionInvoker}
	REASON_CLIENTDISCONNECT_SERVER_SHUTDOWN  = 11,  //reasonmsg=reason
};

enum ChannelProperties {
	CHANNEL_NAME = 0,                       //Available for all channels that are "in view", always up-to-date
	CHANNEL_TOPIC,                          //Available for all channels that are "in view", always up-to-date
	CHANNEL_DESCRIPTION,                    //Must be requested (=> requestChannelDescription)
	CHANNEL_PASSWORD,                       //not available client side
	CHANNEL_CODEC,                          //Available for all channels that are "in view", always up-to-date
	CHANNEL_CODEC_QUALITY,                  //Available for all channels that are "in view", always up-to-date
	CHANNEL_MAXCLIENTS,                     //Available for all channels that are "in view", always up-to-date
	CHANNEL_MAXFAMILYCLIENTS,               //Available for all channels that are "in view", always up-to-date
	CHANNEL_ORDER,                          //Available for all channels that are "in view", always up-to-date
	CHANNEL_FLAG_PERMANENT,                 //Available for all channels that are "in view", always up-to-date
	CHANNEL_FLAG_SEMI_PERMANENT,            //Available for all channels that are "in view", always up-to-date
	CHANNEL_FLAG_DEFAULT,                   //Available for all channels that are "in view", always up-to-date
	CHANNEL_FLAG_PASSWORD,                  //Available for all channels that are "in view", always up-to-date
	CHANNEL_CODEC_LATENCY_FACTOR,           //Available for all channels that are "in view", always up-to-date
	CHANNEL_CODEC_IS_UNENCRYPTED,           //Available for all channels that are "in view", always up-to-date
	CHANNEL_SECURITY_SALT,                  //Not available client side, not used in teamspeak, only SDK. Sets the options+salt for security hash.
	CHANNEL_DELETE_DELAY,                   //How many seconds to wait before deleting this channel
	CHANNEL_ENDMARKER,
};

enum ClientProperties {
	CLIENT_UNIQUE_IDENTIFIER = 0,           //automatically up-to-date for any client "in view", can be used to identify this particular client installation
	CLIENT_NICKNAME,                        //automatically up-to-date for any client "in view"
	CLIENT_VERSION,                         //for other clients than ourself, this needs to be requested (=> requestClientVariables)
	CLIENT_PLATFORM,                        //for other clients than ourself, this needs to be requested (=> requestClientVariables)
	CLIENT_FLAG_TALKING,                    //automatically up-to-date for any client that can be heard (in room / whisper)
	CLIENT_INPUT_MUTED,                     //automatically up-to-date for any client "in view", this clients microphone mute status
	CLIENT_OUTPUT_MUTED,                    //automatically up-to-date for any client "in view", this clients headphones/speakers/mic combined mute status
	CLIENT_OUTPUTONLY_MUTED,                //automatically up-to-date for any client "in view", this clients headphones/speakers only mute status
	CLIENT_INPUT_HARDWARE,                  //automatically up-to-date for any client "in view", this clients microphone hardware status (is the capture device opened?)
	CLIENT_OUTPUT_HARDWARE,                 //automatically up-to-date for any client "in view", this clients headphone/speakers hardware status (is the playback device opened?)
	CLIENT_INPUT_DEACTIVATED,               //only usable for ourself, not propagated to the network
	CLIENT_IDLE_TIME,                       //internal use
	CLIENT_DEFAULT_CHANNEL,                 //only usable for ourself, the default channel we used to connect on our last connection attempt
	CLIENT_DEFAULT_CHANNEL_PASSWORD,        //internal use
	CLIENT_SERVER_PASSWORD,                 //internal use
	CLIENT_META_DATA,                       //automatically up-to-date for any client "in view", not used by TeamSpeak, free storage for sdk users
	CLIENT_IS_MUTED,                        //only make sense on the client side locally, "1" if this client is currently muted by us, "0" if he is not
	CLIENT_IS_RECORDING,                    //automatically up-to-date for any client "in view"
	CLIENT_VOLUME_MODIFICATOR,              //internal use
	CLIENT_VERSION_SIGN,					//sign
	CLIENT_SECURITY_HASH,                   //SDK use, not used by teamspeak. Hash is provided by an outside source. A channel will use the security salt + other client data to calculate a hash, which must be the same as the one provided here.
	CLIENT_ENDMARKER,
};

enum VirtualServerProperties {
	VIRTUALSERVER_UNIQUE_IDENTIFIER = 0,             //available when connected, can be used to identify this particular server installation
	VIRTUALSERVER_NAME,                              //available and always up-to-date when connected
	VIRTUALSERVER_WELCOMEMESSAGE,                    //available when connected,  (=> requestServerVariables)
	VIRTUALSERVER_PLATFORM,                          //available when connected
	VIRTUALSERVER_VERSION,                           //available when connected
	VIRTUALSERVER_MAXCLIENTS,                        //only available on request (=> requestServerVariables), stores the maximum number of clients that may currently join the server
	VIRTUALSERVER_PASSWORD,                          //not available to clients, the server password
	VIRTUALSERVER_CLIENTS_ONLINE,                    //only available on request (=> requestServerVariables),
	VIRTUALSERVER_CHANNELS_ONLINE,                   //only available on request (=> requestServerVariables),
	VIRTUALSERVER_CREATED,                           //available when connected, stores the time when the server was created
	VIRTUALSERVER_UPTIME,                            //only available on request (=> requestServerVariables), the time since the server was started
	VIRTUALSERVER_CODEC_ENCRYPTION_MODE,             //available and always up-to-date when connected
	VIRTUALSERVER_ENDMARKER,
};

enum ConnectionProperties {
	CONNECTION_PING = 0,                                        //average latency for a round trip through and back this connection
	CONNECTION_PING_DEVIATION,                                  //standard deviation of the above average latency
	CONNECTION_CONNECTED_TIME,                                  //how long the connection exists already
	CONNECTION_IDLE_TIME,                                       //how long since the last action of this client
	CONNECTION_CLIENT_IP,                                       //IP of this client (as seen from the server side)
	CONNECTION_CLIENT_PORT,                                     //Port of this client (as seen from the server side)
	CONNECTION_SERVER_IP,                                       //IP of the server (seen from the client side) - only available on yourself, not for remote clients, not available server side
	CONNECTION_SERVER_PORT,                                     //Port of the server (seen from the client side) - only available on yourself, not for remote clients, not available server side
	CONNECTION_PACKETS_SENT_SPEECH,                             //how many Speech packets were sent through this connection
	CONNECTION_PACKETS_SENT_KEEPALIVE,
	CONNECTION_PACKETS_SENT_CONTROL,
	CONNECTION_PACKETS_SENT_TOTAL,                              //how many packets were sent totally (this is PACKETS_SENT_SPEECH + PACKETS_SENT_KEEPALIVE + PACKETS_SENT_CONTROL)
	CONNECTION_BYTES_SENT_SPEECH,
	CONNECTION_BYTES_SENT_KEEPALIVE,
	CONNECTION_BYTES_SENT_CONTROL,
	CONNECTION_BYTES_SENT_TOTAL,
	CONNECTION_PACKETS_RECEIVED_SPEECH,
	CONNECTION_PACKETS_RECEIVED_KEEPALIVE,
	CONNECTION_PACKETS_RECEIVED_CONTROL,
	CONNECTION_PACKETS_RECEIVED_TOTAL,
	CONNECTION_BYTES_RECEIVED_SPEECH,
	CONNECTION_BYTES_RECEIVED_KEEPALIVE,
	CONNECTION_BYTES_RECEIVED_CONTROL,
	CONNECTION_BYTES_RECEIVED_TOTAL,
	CONNECTION_PACKETLOSS_SPEECH,
	CONNECTION_PACKETLOSS_KEEPALIVE,
	CONNECTION_PACKETLOSS_CONTROL,
	CONNECTION_PACKETLOSS_TOTAL,                                //the probability with which a packet round trip failed because a packet was lost
	CONNECTION_SERVER2CLIENT_PACKETLOSS_SPEECH,                 //the probability with which a speech packet failed from the server to the client
	CONNECTION_SERVER2CLIENT_PACKETLOSS_KEEPALIVE,
	CONNECTION_SERVER2CLIENT_PACKETLOSS_CONTROL,
	CONNECTION_SERVER2CLIENT_PACKETLOSS_TOTAL,
	CONNECTION_CLIENT2SERVER_PACKETLOSS_SPEECH,
	CONNECTION_CLIENT2SERVER_PACKETLOSS_KEEPALIVE,
	CONNECTION_CLIENT2SERVER_PACKETLOSS_CONTROL,
	CONNECTION_CLIENT2SERVER_PACKETLOSS_TOTAL,
	CONNECTION_BANDWIDTH_SENT_LAST_SECOND_SPEECH,               //howmany bytes of speech packets we sent during the last second
	CONNECTION_BANDWIDTH_SENT_LAST_SECOND_KEEPALIVE,
	CONNECTION_BANDWIDTH_SENT_LAST_SECOND_CONTROL,
	CONNECTION_BANDWIDTH_SENT_LAST_SECOND_TOTAL,
	CONNECTION_BANDWIDTH_SENT_LAST_MINUTE_SPEECH,               //howmany bytes/s of speech packets we sent in average during the last minute
	CONNECTION_BANDWIDTH_SENT_LAST_MINUTE_KEEPALIVE,
	CONNECTION_BANDWIDTH_SENT_LAST_MINUTE_CONTROL,
	CONNECTION_BANDWIDTH_SENT_LAST_MINUTE_TOTAL,
	CONNECTION_BANDWIDTH_RECEIVED_LAST_SECOND_SPEECH,
	CONNECTION_BANDWIDTH_RECEIVED_LAST_SECOND_KEEPALIVE,
	CONNECTION_BANDWIDTH_RECEIVED_LAST_SECOND_CONTROL,
	CONNECTION_BANDWIDTH_RECEIVED_LAST_SECOND_TOTAL,
	CONNECTION_BANDWIDTH_RECEIVED_LAST_MINUTE_SPEECH,
	CONNECTION_BANDWIDTH_RECEIVED_LAST_MINUTE_KEEPALIVE,
	CONNECTION_BANDWIDTH_RECEIVED_LAST_MINUTE_CONTROL,
	CONNECTION_BANDWIDTH_RECEIVED_LAST_MINUTE_TOTAL,
	CONNECTION_ENDMARKER,
};

typedef struct {
    float x;        /* X co-ordinate in 3D space. */
    float y;        /* Y co-ordinate in 3D space. */
    float z;        /* Z co-ordinate in 3D space. */
} TS3_VECTOR;

enum GroupWhisperType {
	GROUPWHISPERTYPE_SERVERGROUP        = 0,
	GROUPWHISPERTYPE_CHANNELGROUP       = 1,
	GROUPWHISPERTYPE_CHANNELCOMMANDER   = 2,
	GROUPWHISPERTYPE_ALLCLIENTS         = 3,
	GROUPWHISPERTYPE_ENDMARKER,
};

enum GroupWhisperTargetMode {
	GROUPWHISPERTARGETMODE_ALL                   = 0,
	GROUPWHISPERTARGETMODE_CURRENTCHANNEL        = 1,
	GROUPWHISPERTARGETMODE_PARENTCHANNEL         = 2,
	GROUPWHISPERTARGETMODE_ALLPARENTCHANNELS     = 3,
	GROUPWHISPERTARGETMODE_CHANNELFAMILY         = 4,
	GROUPWHISPERTARGETMODE_ANCESTORCHANNELFAMILY = 5,
	GROUPWHISPERTARGETMODE_SUBCHANNELS           = 6,
	GROUPWHISPERTARGETMODE_ENDMARKER,
};

enum MonoSoundDestination{ 
  MONO_SOUND_DESTINATION_ALL                  =0, /* Send mono sound to all available speakers */
  MONO_SOUND_DESTINATION_FRONT_CENTER         =1, /* Send mono sound to front center speaker if available */
  MONO_SOUND_DESTINATION_FRONT_LEFT_AND_RIGHT =2  /* Send mono sound to front left/right speakers if available */
};

enum SecuritySaltOptions {
	SECURITY_SALT_CHECK_NICKNAME  = 1, /* put nickname into security hash */
	SECURITY_SALT_CHECK_META_DATA = 2  /* put (game)meta data into security hash */
};

/*this enum is used to disable client commands on the server*/
enum ClientCommand{
	CLIENT_COMMAND_requestConnectionInfo        =  0,
	CLIENT_COMMAND_requestClientMove            =  1,
	CLIENT_COMMAND_requestXXMuteClients         =  2,
	CLIENT_COMMAND_requestClientKickFromXXX     =  3,
	CLIENT_COMMAND_flushChannelCreation         =  4,
	CLIENT_COMMAND_flushChannelUpdates          =  5,
	CLIENT_COMMAND_requestChannelMove           =  6,
	CLIENT_COMMAND_requestChannelDelete         =  7,
	CLIENT_COMMAND_requestChannelDescription    =  8,
	CLIENT_COMMAND_requestChannelXXSubscribeXXX =  9,
	CLIENT_COMMAND_requestServerConnectionInfo  = 10,
	CLIENT_COMMAND_requestSendXXXTextMsg        = 11,
	CLIENT_COMMAND_filetransfers                = 12,
	CLIENT_COMMAND_ENDMARKER
};

/* Access Control List*/
enum ACLType{
	ACL_NONE       = 0,
	ACL_WHITE_LIST = 1,
	ACL_BLACK_LIST = 2
};

/* file transfer actions*/
enum FTAction{
	FT_INIT_SERVER  = 0,
	FT_INIT_CHANNEL = 1,
	FT_UPLOAD       = 2,
	FT_DOWNLOAD     = 3,
	FT_DELETE       = 4,
	FT_CREATEDIR    = 5,
	FT_RENAME       = 6,
	FT_FILELIST     = 7,
	FT_FILEINFO     = 8
};

/* file transfer status */
enum FileTransferState {
	FILETRANSFER_INITIALISING = 0,
	FILETRANSFER_ACTIVE,
	FILETRANSFER_FINISHED,
};

/* file transfer types */
enum {
	FileListType_Directory = 0,
	FileListType_File,
};

/* some structs to handle variables in callbacks */
#define MAX_VARIABLES_EXPORT_COUNT 64
struct VariablesExportItem{
	unsigned char itemIsValid;    /* This item has valid values. ignore this item if 0 */
	unsigned char proposedIsSet;  /* The value in proposed is set. if 0 ignore proposed */
	const char* current;          /* current value (stored in memory) */
	const char* proposed;         /* New value to change to (const, so no updates please) */
};

struct VariablesExport{
	struct VariablesExportItem items[MAX_VARIABLES_EXPORT_COUNT];
};

struct ClientMiniExport{
	anyID ID;
	uint64 channel;
	const char* ident;
	const char* nickname;
};

struct TransformFilePathExport{
	uint64 channel;
	const char* filename;
	int action;
	int transformedFileNameMaxSize;
	int channelPathMaxSize;
};

struct TransformFilePathExportReturns{
	char* transformedFileName;
	char* channelPath;
	int logFileAction;
};

struct FileTransferCallbackExport{
	anyID clientID;
	anyID transferID;
	anyID remoteTransferID;
	unsigned int status;
	const char* statusMessage;
	uint64 remotefileSize;
	uint64 bytes;
	int isSender;
};

/*define for file transfer bandwith limits*/
#define BANDWIDTH_LIMIT_UNLIMITED 0xFFFFFFFFFFFFFFFFll


/*defines for speaker locations used by some sound callbacks*/
#ifndef SPEAKER_FRONT_LEFT
#define SPEAKER_FRONT_LEFT              0x1
#define SPEAKER_FRONT_RIGHT             0x2
#define SPEAKER_FRONT_CENTER            0x4
#define SPEAKER_LOW_FREQUENCY           0x8
#define SPEAKER_BACK_LEFT               0x10
#define SPEAKER_BACK_RIGHT              0x20
#define SPEAKER_FRONT_LEFT_OF_CENTER    0x40
#define SPEAKER_FRONT_RIGHT_OF_CENTER   0x80
#define SPEAKER_BACK_CENTER             0x100
#define SPEAKER_SIDE_LEFT               0x200
#define SPEAKER_SIDE_RIGHT              0x400
#define SPEAKER_TOP_CENTER              0x800
#define SPEAKER_TOP_FRONT_LEFT          0x1000
#define SPEAKER_TOP_FRONT_CENTER        0x2000
#define SPEAKER_TOP_FRONT_RIGHT         0x4000
#define SPEAKER_TOP_BACK_LEFT           0x8000
#define SPEAKER_TOP_BACK_CENTER         0x10000
#define SPEAKER_TOP_BACK_RIGHT          0x20000
#endif
#define SPEAKER_HEADPHONES_LEFT         0x10000000
#define SPEAKER_HEADPHONES_RIGHT        0x20000000
#define SPEAKER_MONO                    0x40000000

#endif /*PUBLIC_DEFINITIONS_H*/
