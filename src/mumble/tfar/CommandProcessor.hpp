#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include "common.hpp"
#include <atomic>

struct unitPositionPacket;

enum class gameCommand {
    TS_INFO,                //Synchronous
    POS,                    //Synchronous
    IS_SPEAKING,            //Synchronous
    IS_SPEAKING_BULK,      //Synchronous
    FREQ,                   //Async
    KILLED,                 //Async
    TRACK,                  //Async
    DFRAME,                 //Async
    SPEAKERS,               //Async
    TANGENT,                //Async //TANGENT or TANGENT_LR or TANGENT_DD
    RELEASE_ALL_TANGENTS,   //Async
    SETCFG,                 //Async
    MISSIONEND,             //Async
    AddRadioTower,
    DeleteRadioTower,
    RECV_FREQS,
    collectDebugInfo,       //Async
    unknown
};

class CommandProcessor {
public:
    CommandProcessor();
    ~CommandProcessor();
    void stopThread();
    void queueCommand(const std::string& command);
    std::string processCommand(const std::string& command);

    static gameCommand toGameCommand(std::string_view textCommand, size_t tokenCount);

    static inline std::atomic_bool vadEnabled = false;
private:
    void threadRun();
    void processAsynchronousCommand(const std::string& command) const;//Called inside thread

    void processSpeakers(std::vector<std::string_view>& tokens) const;
    void processUnitKilled(std::string &&name, TSServerID serverConnection) const;

    static void processUnitPosition(TSServerID serverConnection, unitPositionPacket& packet);
    static std::string ts_info(std::string_view command);

    static void process_tangent_off(PTTDelayArguments arguments);

    static void disableVoiceAndSendCommand(std::string_view commandToBroadcast, dataType::TSServerID currentServerConnectionHandlerID, bool pressed);

    static std::string convertNickname(std::string_view nickname);


    std::queue<std::string> commandQueue;
    std::unique_ptr<std::thread> myThread;
    std::condition_variable threadWorkCondition;
    std::mutex theadMutex;
    bool shouldRun = true;//don't need atomic here. believe me.
    bool threadRunning = false;
};

