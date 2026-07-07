#ifndef PUBLIC_RARE_DEFINITIONS_H
#define PUBLIC_RARE_DEFINITIONS_H

#include "public_definitions.h"

//limited length, measured in characters
#define TS3_MAX_SIZE_CLIENT_NICKNAME_NONSDK 30
#define TS3_MIN_SIZE_CLIENT_NICKNAME_NONSDK 3
#define TS3_MAX_SIZE_AWAY_MESSAGE 80
#define TS3_MAX_SIZE_GROUP_NAME 30
#define TS3_MAX_SIZE_TALK_REQUEST_MESSAGE 50
#define TS3_MAX_SIZE_COMPLAIN_MESSAGE 200
#define TS3_MAX_SIZE_CLIENT_DESCRIPTION 200
#define TS3_MAX_SIZE_HOST_MESSAGE 200
#define TS3_MAX_SIZE_HOSTBUTTON_TOOLTIP 50
#define TS3_MAX_SIZE_POKE_MESSAGE 100
#define TS3_MAX_SIZE_OFFLINE_MESSAGE 4096
#define TS3_MAX_SIZE_OFFLINE_MESSAGE_SUBJECT 200

//limited length, measured in bytes (utf8 encoded)
#define TS3_MAX_SIZE_PLUGIN_COMMAND 1024*8
#define TS3_MAX_SIZE_VIRTUALSERVER_HOSTBANNER_GFX_URL 2000


enum GroupShowNameTreeMode {
	GroupShowNameTreeMode_NONE= 0, //dont group show name
	GroupShowNameTreeMode_BEFORE,  //show group name before client name
	GroupShowNameTreeMode_BEHIND   //show group name behind client name
};

enum PluginTargetMode {
	PluginCommandTarget_CURRENT_CHANNEL=0,                  //send plugincmd to all clients in current channel
	PluginCommandTarget_SERVER,                             //send plugincmd to all clients on server
	PluginCommandTarget_CLIENT,                             //send plugincmd to all given client ids
	PluginCommandTarget_CURRENT_CHANNEL_SUBSCRIBED_CLIENTS, //send plugincmd to all subscribed clients in current channel
	PluginCommandTarget_MAX
};

enum {
	SERVER_BINDING_VIRTUALSERVER=0,
	SERVER_BINDING_SERVERQUERY  =1,
	SERVER_BINDING_FILETRANSFER =2,
};

enum HostMessageMode {
	HostMessageMode_NONE=0,               //dont display anything
	HostMessageMode_LOG,                  //display message inside log
	HostMessageMode_MODAL,                //display message inside a modal dialog
	HostMessageMode_MODALQUIT             //display message inside a modal dialog and quit/close server/connection
};

enum HostBannerMode {
	HostBannerMode_NO_ADJUST=0,           //Do not adjust
	HostBannerMode_ADJUST_IGNORE_ASPECT,  //Adjust but ignore aspect ratio
	HostBannerMode_ADJUST_KEEP_ASPECT,    //Adjust and keep aspect ratio
};

enum ClientType {
	ClientType_NORMAL = 0,
	ClientType_SERVERQUERY,
};

enum AwayStatus {
	AWAY_NONE = 0,
	AWAY_ZZZ,
};

enum CommandLinePropertiesRare {
#ifdef SERVER
	COMMANDLINE_CREATE_DEFAULT_VIRTUALSERVER= 0,    //create default virtualserver
	COMMANDLINE_MACHINE_ID,                         //machine id (starts only virtualserver with given machineID
	COMMANDLINE_DEFAULT_VOICE_PORT,
	COMMANDLINE_VOICE_IP,
	COMMANDLINE_THREADS_VOICE_UDP,
	COMMANDLINE_LICENSEPATH,
#ifndef SDK
	COMMANDLINE_FILETRANSFER_PORT,
	COMMANDLINE_FILETRANSFER_IP,
	COMMANDLINE_QUERY_PORT,
	COMMANDLINE_QUERY_IP,
	COMMANDLINE_QUERY_IP_WHITELIST,
	COMMANDLINE_QUERY_IP_BLACKLIST,
	COMMANDLINE_CLEAR_DATABASE,
	COMMANDLINE_SERVERADMIN_PASSWORD,
	COMMANDLINE_DBPLUGIN,
	COMMANDLINE_DBPLUGINPARAMETER,
	COMMANDLINE_DBSQLPATH,
	COMMANDLINE_DBSQLCREATEPATH,
	COMMANDLINE_DBCONNECTIONS,
	COMMANDLINE_LOGPATH,
	COMMANDLINE_CREATEINIFILE,
	COMMANDLINE_INIFILE,
	COMMANDLINE_LOGQUERYCOMMANDS,
	COMMANDLINE_DBCLIENTKEEPDAYS,
	COMMANDLINE_THREADS_QUERY_TCP,
	COMMANDLINE_NO_PERMISSION_UPDATE,
	COMMANDLINE_OPEN_WIN_CONSOLE,
	COMMANDLINE_NO_PASSWORD_DIALOG,
	COMMANDLINE_LOGAPPEND,
	COMMANDLINE_QUERY_SKIPBRUTEFORCECHECK,
#endif
#else
	COMMANDLINE_NOTHING=0,
#endif
	COMMANDLINE_ENDMARKER_RARE,
};

enum ServerInstancePropertiesRare {
	SERVERINSTANCE_DATABASE_VERSION= 0,
	SERVERINSTANCE_FILETRANSFER_PORT,
	SERVERINSTANCE_SERVER_ENTROPY,
	SERVERINSTANCE_MONTHLY_TIMESTAMP,
	SERVERINSTANCE_MAX_DOWNLOAD_TOTAL_BANDWIDTH,
	SERVERINSTANCE_MAX_UPLOAD_TOTAL_BANDWIDTH,
	SERVERINSTANCE_GUEST_SERVERQUERY_GROUP,
	SERVERINSTANCE_SERVERQUERY_FLOOD_COMMANDS,       //how many commands we can issue while in the SERVERINSTANCE_SERVERQUERY_FLOOD_TIME window
	SERVERINSTANCE_SERVERQUERY_FLOOD_TIME,           //time window in seconds for max command execution check
	SERVERINSTANCE_SERVERQUERY_BAN_TIME,             //how many seconds someone get banned if he floods
	SERVERINSTANCE_TEMPLATE_SERVERADMIN_GROUP,
	SERVERINSTANCE_TEMPLATE_SERVERDEFAULT_GROUP,
	SERVERINSTANCE_TEMPLATE_CHANNELADMIN_GROUP,
	SERVERINSTANCE_TEMPLATE_CHANNELDEFAULT_GROUP,
	SERVERINSTANCE_PERMISSIONS_VERSION,
	SERVERINSTANCE_PENDING_CONNECTIONS_PER_IP,
	SERVERINSTANCE_ENDMARKER_RARE,
};

enum VirtualServerPropertiesRare {
	VIRTUALSERVER_DUMMY_0= VIRTUALSERVER_ENDMARKER,
	VIRTUALSERVER_DUMMY_1,
	VIRTUALSERVER_DUMMY_2,
	VIRTUALSERVER_DUMMY_3,
	VIRTUALSERVER_DUMMY_4,
	VIRTUALSERVER_DUMMY_5,
	VIRTUALSERVER_DUMMY_6,
	VIRTUALSERVER_DUMMY_7,
	VIRTUALSERVER_DUMMY_8,
	VIRTUALSERVER_KEYPAIR,                                     //internal use
	VIRTUALSERVER_HOSTMESSAGE,                                 //available when connected, not updated while connected
	VIRTUALSERVER_HOSTMESSAGE_MODE,                            //available when connected, not updated while connected
	VIRTUALSERVER_FILEBASE,                                    //not available to clients, stores the folder used for file transfers
	VIRTUALSERVER_DEFAULT_SERVER_GROUP,                        //the client permissions server group that a new client gets assigned
	VIRTUALSERVER_DEFAULT_CHANNEL_GROUP,                       //the channel permissions group that a new client gets assigned when joining a channel
	VIRTUALSERVER_FLAG_PASSWORD,                               //only available on request (=> requestServerVariables) 
	VIRTUALSERVER_DEFAULT_CHANNEL_ADMIN_GROUP,                 //the channel permissions group that a client gets assigned when creating a channel
	VIRTUALSERVER_MAX_DOWNLOAD_TOTAL_BANDWIDTH,                //only available on request (=> requestServerVariables)
	VIRTUALSERVER_MAX_UPLOAD_TOTAL_BANDWIDTH,                  //only available on request (=> requestServerVariables)
	VIRTUALSERVER_HOSTBANNER_URL,                              //available when connected, always up-to-date 
	VIRTUALSERVER_HOSTBANNER_GFX_URL,                          //available when connected, always up-to-date
	VIRTUALSERVER_HOSTBANNER_GFX_INTERVAL,                     //available when connected, always up-to-date
	VIRTUALSERVER_COMPLAIN_AUTOBAN_COUNT,                      //only available on request (=> requestServerVariables)
	VIRTUALSERVER_COMPLAIN_AUTOBAN_TIME,                       //only available on request (=> requestServerVariables)
	VIRTUALSERVER_COMPLAIN_REMOVE_TIME,                        //only available on request (=> requestServerVariables)
	VIRTUALSERVER_MIN_CLIENTS_IN_CHANNEL_BEFORE_FORCED_SILENCE,//only available on request (=> requestServerVariables)
	VIRTUALSERVER_PRIORITY_SPEAKER_DIMM_MODIFICATOR,           //available when connected, always up-to-date
	VIRTUALSERVER_ID,                                          //available when connected
	VIRTUALSERVER_ANTIFLOOD_POINTS_TICK_REDUCE,                //only available on request (=> requestServerVariables)
	VIRTUALSERVER_ANTIFLOOD_POINTS_NEEDED_COMMAND_BLOCK,       //only available on request (=> requestServerVariables)
	VIRTUALSERVER_ANTIFLOOD_POINTS_NEEDED_IP_BLOCK,            //only available on request (=> requestServerVariables)
	VIRTUALSERVER_CLIENT_CONNECTIONS,                          //only available on request (=> requestServerVariables)
	VIRTUALSERVER_QUERY_CLIENT_CONNECTIONS,                    //only available on request (=> requestServerVariables)
	VIRTUALSERVER_HOSTBUTTON_TOOLTIP,                          //available when connected, always up-to-date 
	VIRTUALSERVER_HOSTBUTTON_URL,                              //available when connected, always up-to-date 
	VIRTUALSERVER_HOSTBUTTON_GFX_URL,                          //available when connected, always up-to-date 
	VIRTUALSERVER_QUERYCLIENTS_ONLINE,                         //only available on request (=> requestServerVariables)
	VIRTUALSERVER_DOWNLOAD_QUOTA,                              //only available on request (=> requestServerVariables)
	VIRTUALSERVER_UPLOAD_QUOTA,                                //only available on request (=> requestServerVariables)
	VIRTUALSERVER_MONTH_BYTES_DOWNLOADED,                      //only available on request (=> requestServerVariables)
	VIRTUALSERVER_MONTH_BYTES_UPLOADED,                        //only available on request (=> requestServerVariables)
	VIRTUALSERVER_TOTAL_BYTES_DOWNLOADED,                      //only available on request (=> requestServerVariables)
	VIRTUALSERVER_TOTAL_BYTES_UPLOADED,                        //only available on request (=> requestServerVariables)
	VIRTUALSERVER_PORT,                                        //only available on request (=> requestServerVariables)
	VIRTUALSERVER_AUTOSTART,                                   //only available on request (=> requestServerVariables)
	VIRTUALSERVER_MACHINE_ID,                                  //only available on request (=> requestServerVariables)
	VIRTUALSERVER_NEEDED_IDENTITY_SECURITY_LEVEL,              //only available on request (=> requestServerVariables)
	VIRTUALSERVER_LOG_CLIENT,                                  //only available on request (=> requestServerVariables)
	VIRTUALSERVER_LOG_QUERY,                                   //only available on request (=> requestServerVariables)
	VIRTUALSERVER_LOG_CHANNEL,                                 //only available on request (=> requestServerVariables)
	VIRTUALSERVER_LOG_PERMISSIONS,                             //only available on request (=> requestServerVariables)
	VIRTUALSERVER_LOG_SERVER,                                  //only available on request (=> requestServerVariables)
	VIRTUALSERVER_LOG_FILETRANSFER,                            //only available on request (=> requestServerVariables)
	VIRTUALSERVER_MIN_CLIENT_VERSION,                          //only available on request (=> requestServerVariables)
	VIRTUALSERVER_NAME_PHONETIC,                               //available when connected, always up-to-date
	VIRTUALSERVER_ICON_ID,                                     //available when connected, always up-to-date
	VIRTUALSERVER_RESERVED_SLOTS,                              //available when connected, always up-to-date
	VIRTUALSERVER_TOTAL_PACKETLOSS_SPEECH,                     //only available on request (=> requestServerVariables)
	VIRTUALSERVER_TOTAL_PACKETLOSS_KEEPALIVE,                  //only available on request (=> requestServerVariables)
	VIRTUALSERVER_TOTAL_PACKETLOSS_CONTROL,                    //only available on request (=> requestServerVariables)
	VIRTUALSERVER_TOTAL_PACKETLOSS_TOTAL,                      //only available on request (=> requestServerVariables)
	VIRTUALSERVER_TOTAL_PING,                                  //only available on request (=> requestServerVariables)
	VIRTUALSERVER_IP,                                          //internal use | contains only ONE binded ip
	VIRTUALSERVER_WEBLIST_ENABLED,                             //only available on request (=> requestServerVariables)
	VIRTUALSERVER_AUTOGENERATED_PRIVILEGEKEY,                  //internal use
	VIRTUALSERVER_ASK_FOR_PRIVILEGEKEY,                        //available when connected
	VIRTUALSERVER_HOSTBANNER_MODE,                             //available when connected, always up-to-date
	VIRTUALSERVER_CHANNEL_TEMP_DELETE_DELAY_DEFAULT,           //available when connected, always up-to-date
	VIRTUALSERVER_MIN_ANDROID_VERSION,                         //only available on request (=> requestServerVariables)
	VIRTUALSERVER_MIN_IOS_VERSION,                             //only available on request (=> requestServerVariables)
	VIRTUALSERVER_MIN_WINPHONE_VERSION,                           //only available on request (=> requestServerVariables)
	VIRTUALSERVER_ENDMARKER_RARE
};

enum ChannelPropertiesRare {
	CHANNEL_DUMMY_2= CHANNEL_ENDMARKER,
	CHANNEL_DUMMY_3,
	CHANNEL_DUMMY_4,
	CHANNEL_DUMMY_5,
	CHANNEL_DUMMY_6,
	CHANNEL_DUMMY_7,
	CHANNEL_FLAG_MAXCLIENTS_UNLIMITED,      //Available for all channels that are "in view", always up-to-date
	CHANNEL_FLAG_MAXFAMILYCLIENTS_UNLIMITED,//Available for all channels that are "in view", always up-to-date
	CHANNEL_FLAG_MAXFAMILYCLIENTS_INHERITED,//Available for all channels that are "in view", always up-to-date
	CHANNEL_FLAG_ARE_SUBSCRIBED,            //Only available client side, stores whether we are subscribed to this channel
	CHANNEL_FILEPATH,                       //not available client side, the folder used for file-transfers for this channel
	CHANNEL_NEEDED_TALK_POWER,              //Available for all channels that are "in view", always up-to-date
	CHANNEL_FORCED_SILENCE,                 //Available for all channels that are "in view", always up-to-date
	CHANNEL_NAME_PHONETIC,                  //Available for all channels that are "in view", always up-to-date
	CHANNEL_ICON_ID,                        //Available for all channels that are "in view", always up-to-date
	CHANNEL_FLAG_PRIVATE,                   //Available for all channels that are "in view", always up-to-date
	CHANNEL_ENDMARKER_RARE
};

enum ClientPropertiesRare {
	CLIENT_DUMMY_3= CLIENT_ENDMARKER,
	CLIENT_DUMMY_4,
	CLIENT_DUMMY_5,
	CLIENT_DUMMY_6,
	CLIENT_DUMMY_7,
	CLIENT_DUMMY_8,
	CLIENT_DUMMY_9,
	CLIENT_KEY_OFFSET,                      //internal use
	CLIENT_LAST_VAR_REQUEST,                //internal use
	CLIENT_LOGIN_NAME,                      //used for serverquery clients, makes no sense on normal clients currently
	CLIENT_LOGIN_PASSWORD,                  //used for serverquery clients, makes no sense on normal clients currently
	CLIENT_DATABASE_ID,                     //automatically up-to-date for any client "in view", only valid with PERMISSION feature, holds database client id
	CLIENT_CHANNEL_GROUP_ID,                //automatically up-to-date for any client "in view", only valid with PERMISSION feature, holds database client id
	CLIENT_SERVERGROUPS,                    //automatically up-to-date for any client "in view", only valid with PERMISSION feature, holds all servergroups client belongs too
	CLIENT_CREATED,                         //this needs to be requested (=> requestClientVariables), first time this client connected to this server
	CLIENT_LASTCONNECTED,                   //this needs to be requested (=> requestClientVariables), last time this client connected to this server
	CLIENT_TOTALCONNECTIONS,                //this needs to be requested (=> requestClientVariables), how many times this client connected to this server
	CLIENT_AWAY,                            //automatically up-to-date for any client "in view", this clients away status
	CLIENT_AWAY_MESSAGE,                    //automatically up-to-date for any client "in view", this clients away message
	CLIENT_TYPE,                            //automatically up-to-date for any client "in view", determines if this is a real client or a server-query connection
	CLIENT_FLAG_AVATAR,                     //automatically up-to-date for any client "in view", this client got an avatar
	CLIENT_TALK_POWER,                      //automatically up-to-date for any client "in view", only valid with PERMISSION feature, holds database client id
	CLIENT_TALK_REQUEST,                    //automatically up-to-date for any client "in view", only valid with PERMISSION feature, holds timestamp where client requested to talk
	CLIENT_TALK_REQUEST_MSG,                //automatically up-to-date for any client "in view", only valid with PERMISSION feature, holds matter for the request
	CLIENT_DESCRIPTION,                     //automatically up-to-date for any client "in view"
	CLIENT_IS_TALKER,                       //automatically up-to-date for any client "in view"
	CLIENT_MONTH_BYTES_UPLOADED,            //this needs to be requested (=> requestClientVariables)
	CLIENT_MONTH_BYTES_DOWNLOADED,          //this needs to be requested (=> requestClientVariables)
	CLIENT_TOTAL_BYTES_UPLOADED,            //this needs to be requested (=> requestClientVariables)
	CLIENT_TOTAL_BYTES_DOWNLOADED,          //this needs to be requested (=> requestClientVariables)
	CLIENT_IS_PRIORITY_SPEAKER,             //automatically up-to-date for any client "in view"
	CLIENT_UNREAD_MESSAGES,                 //automatically up-to-date for any client "in view"
	CLIENT_NICKNAME_PHONETIC,               //automatically up-to-date for any client "in view"
	CLIENT_NEEDED_SERVERQUERY_VIEW_POWER,   //automatically up-to-date for any client "in view"
	CLIENT_DEFAULT_TOKEN,                   //only usable for ourself, the default token we used to connect on our last connection attempt
	CLIENT_ICON_ID,                         //automatically up-to-date for any client "in view"
	CLIENT_IS_CHANNEL_COMMANDER,            //automatically up-to-date for any client "in view"
	CLIENT_COUNTRY,                         //automatically up-to-date for any client "in view"
	CLIENT_CHANNEL_GROUP_INHERITED_CHANNEL_ID, //automatically up-to-date for any client "in view", only valid with PERMISSION feature, contains channel_id where the channel_group_id is set from
	CLIENT_BADGES,                          //automatically up-to-date for any client "in view", stores icons for partner badges
	CLIENT_ENDMARKER_RARE,
};

enum ConnectionPropertiesRare {
	CONNECTION_DUMMY_0= CONNECTION_ENDMARKER,
	CONNECTION_DUMMY_1,
	CONNECTION_DUMMY_2,
	CONNECTION_DUMMY_3,
	CONNECTION_DUMMY_4,
	CONNECTION_DUMMY_5,
	CONNECTION_DUMMY_6,
	CONNECTION_DUMMY_7,
	CONNECTION_DUMMY_8,
	CONNECTION_DUMMY_9,
	CONNECTION_FILETRANSFER_BANDWIDTH_SENT,                     //how many bytes per second are currently being sent by file transfers
	CONNECTION_FILETRANSFER_BANDWIDTH_RECEIVED,                 //how many bytes per second are currently being received by file transfers
	CONNECTION_FILETRANSFER_BYTES_RECEIVED_TOTAL,               //how many bytes we received in total through file transfers
	CONNECTION_FILETRANSFER_BYTES_SENT_TOTAL,                   //how many bytes we sent in total through file transfers
	CONNECTION_ENDMARKER_RARE,
};

enum LicenseViolationType {
	NO_VIOLATION = 0,
	SLOT_VIOLATION,
	SLOT_SUSPICION
};

enum BBCodeTags {
	BBCodeTag_B           = 0x00000001,
	BBCodeTag_I           = 0x00000002,
	BBCodeTag_U           = 0x00000004,
	BBCodeTag_S           = 0x00000008,
	BBCodeTag_SUP         = 0x00000010,
	BBCodeTag_SUB         = 0x00000020,
	BBCodeTag_COLOR       = 0x00000040,
	BBCodeTag_SIZE        = 0x00000080,
	BBCodeTag_group_text  = 0x000000FF,

	BBCodeTag_LEFT        = 0x00001000,
	BBCodeTag_RIGHT       = 0x00002000,
	BBCodeTag_CENTER      = 0x00004000,
	BBCodeTag_group_align = 0x00007000,

	BBCodeTag_URL         = 0x00010000,
	BBCodeTag_IMAGE       = 0x00020000,
	BBCodeTag_HR          = 0x00040000,

	BBCodeTag_LIST        = 0x00100000,
	BBCodeTag_LISTITEM    = 0x00200000,
	BBCodeTag_group_list  = 0x00300000,

	BBCodeTag_TABLE       = 0x00400000,
	BBCodeTag_TR          = 0x00800000,
	BBCodeTag_TH          = 0x01000000,
	BBCodeTag_TD          = 0x02000000,
	BBCodeTag_group_table = 0x03C00000,

	BBCodeTag_def_simple     = BBCodeTag_B | BBCodeTag_I | BBCodeTag_U | BBCodeTag_S | BBCodeTag_SUP | BBCodeTag_SUB |BBCodeTag_COLOR | BBCodeTag_URL,
	BBCodeTag_def_simple_Img = BBCodeTag_def_simple | BBCodeTag_IMAGE,
	BBCodeTag_def_extended   = BBCodeTag_group_text | BBCodeTag_group_align | BBCodeTag_URL | BBCodeTag_IMAGE | BBCodeTag_HR | BBCodeTag_group_list | BBCodeTag_group_table,
};

typedef int(*ExtraBBCodeValidator)(void* userparam, const char* tag, const char* paramValue, int paramValueSize, const char* childValue, int childValueSize);
typedef const char* (*ExtraBBCodeParamTransform)(void* userparam, const char* tag, const char* paramValue);

#endif //PUBLIC_RARE_DEFINITIONS_H
