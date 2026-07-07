#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <memory>
#include "teamlog/logtypes.h"
class ILogger {
protected:
    ~ILogger() = default;

public:
    virtual void log(const std::string& message) = 0;
    virtual void log(const std::string& message, LogLevel _loglevel) = 0;
    template<typename T>
    void operator<< (const T& data) {
        std::stringstream str;
        str << data;
        log(str.str());
    };
};

class FileLogger : public ILogger {
public:
    explicit FileLogger(const std::string& filePath);
    virtual ~FileLogger() = default;

    void log(const std::string& message) override;
    void log(const std::string& message, LogLevel _loglevel) override;
private:
    std::ofstream file;
};

class TeamspeakLogger : public ILogger {
public:
    explicit TeamspeakLogger(enum LogLevel defaultLoglevel);
    virtual ~TeamspeakLogger() = default;

    void log(const std::string& message) override;
    void log(const std::string& message, LogLevel _loglevel) override;
private:
    LogLevel level;
};

class DebugStringLogger : public ILogger {
public:
    explicit DebugStringLogger() = default;
    virtual ~DebugStringLogger() = default;

    void log(const std::string& message) override;
    void log(const std::string& message, LogLevel _loglevel) override;
};

class CircularLogger : public ILogger {
public:
    explicit CircularLogger(uint32_t _messageCount) : messageCount(_messageCount) {};
    virtual ~CircularLogger() = default;

    void log(const std::string& message) override;
    void log(const std::string& message, LogLevel _loglevel) override;

    std::vector <std::string> messages;
    uint32_t offset{0};
    uint32_t messageCount;
};

enum class LoggerTypes {
    profiler,
    gameCommands,
    pluginCommands,
    teamspeakClientlog,
    internalCircularLog
};

//this stuff is missing a lot of features.. Especially Loglevels. I just made this for debugging purposes
class Logger {
public:
    static void registerLogger(LoggerTypes type, std::shared_ptr<ILogger> logger);
    //#TODO deprecate log without logLevel and use DEBUG as default loglevel
    //#TODO log function that takes message as rvalue reference to avoid copy (&&)
    static void log(LoggerTypes type, const std::string& message);
    static void log(LoggerTypes type, const std::string& message, LogLevel _loglevel);
    static std::vector<std::shared_ptr<ILogger>> getLogger(LoggerTypes type);
private:
    Logger() = default;
    ~Logger() = default;
    void _registerLogger(LoggerTypes type, std::shared_ptr<ILogger> logger);
    void _log(LoggerTypes type, const std::string& message) const;
    void _log(LoggerTypes type, const std::string& message, LogLevel _loglevel) const;
    static Logger& getInstance() { static Logger log; return log; }
    std::map<LoggerTypes, std::vector<std::shared_ptr<ILogger>>> registeredLoggers;
};

