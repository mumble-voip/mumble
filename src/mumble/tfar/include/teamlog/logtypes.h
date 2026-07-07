#ifndef TEAMLOG_LOGTYPES_H
#define TEAMLOG_LOGTYPES_H

enum LogTypes {
	LogType_NONE          = 0x0000,
	LogType_FILE          = 0x0001,
	LogType_CONSOLE       = 0x0002,
	LogType_USERLOGGING   = 0x0004,
	LogType_NO_NETLOGGING = 0x0008,
	LogType_DATABASE      = 0x0010,
	LogType_SYSLOG        = 0x0020,
};

enum LogLevel {
	LogLevel_CRITICAL = 0, //these messages stop the program
	LogLevel_ERROR,        //everything that is really bad, but not so bad we need to shut down
	LogLevel_WARNING,      //everything that *might* be bad
	LogLevel_DEBUG,        //output that might help find a problem
	LogLevel_INFO,         //informational output, like "starting database version x.y.z"
	LogLevel_DEVEL         //developer only output (will not be displayed in release mode)
};

#endif //TEAMLOG_LOGTYPES_H
