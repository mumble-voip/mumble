#pragma once
#include <string>
#include "common.hpp"
#include <Windows.h>
#include "SignalSlot.hpp"
/*
Shared Mem layout
offset 0: SharedMemoryData
offset 128: Synchronous Request [512b]
offset 640: Synchronous Answer [512b]
offset 1152: Asynchronous Messages array of SharedMemString[500]
*/

#define SHAREDMEM_ASYNCMSG_COUNT 300
#define SHAREDMEM_MAX_STRINGSIZE (sizeof(SharedMemString) -4)
#define SHAREDMEM_BUFSIZE (128+sizeof(SharedMemString)+sizeof(SharedMemString)+(sizeof(SharedMemString) * SHAREDMEM_ASYNCMSG_COUNT)) //Header+SyncReq+SyncAnsw+AsyncMessages

namespace SharedMemoryHandlerInternal {
    struct SharedMemString {
        uint32_t length{ 0 };
        char data[2044]{ 0 };
        SharedMemString& operator=(const std::string& other) {
            length = static_cast<uint32_t>(other.length());
            if (length == 0 || length > SHAREDMEM_MAX_STRINGSIZE) {
                length = 0;
                return *this;
            }
            memcpy(data, other.c_str(), length);
            return *this;
        }
        bool assignTo(std::string& other) const {
            if (length == 0 || length > SHAREDMEM_MAX_STRINGSIZE)
                return false;
            other.resize(length);
            other.assign(data, length);
            return true;
        }
        bool assignToAndClear(std::string& other) {
            if (length == 0 || length > SHAREDMEM_MAX_STRINGSIZE)
                return false;
            other.resize(length);
            other.assign(data, length);
            length = 0;
            return true;
        }
    };
    class SharedMemoryData {
    public:
        explicit SharedMemoryData(uint32_t _size) :sharedMemSize(_size) {}
        bool getAsyncRequest(std::string& req);
        bool getAsyncRequestMultiple(std::vector<std::string>& reqs);
        bool getSyncRequest(std::string& req);
        void setSyncResponse(const std::string& response);
        bool hasAsyncRequest() const;
        bool hasSyncRequest() const;
        std::chrono::system_clock::time_point getLastGameTick() const { return lastGameTick; }
        void setLastPluginTick() { lastPluginTick = std::chrono::system_clock::now(); }
        void setLastGameTick(std::chrono::system_clock::time_point time) { lastGameTick = time; }
        void setConfigNeedsRefresh(bool needs) { configNeedsRefresh = needs; }
        void onShutdown() { lastPluginTick = std::chrono::system_clock::time_point(0us); }
    private:
        uint32_t sharedMemSize{ 0 };
        volatile uint16_t nextFreeAsyncMessage{ 0 };
        std::chrono::system_clock::time_point lastGameTick;
        std::chrono::system_clock::time_point lastPluginTick;
        bool configNeedsRefresh{ false };
    };
    static_assert(sizeof(SharedMemoryData) < 128, "SharedMemoryData is bigger than space allocated to it in SHAMEM");
    class MutexLock {
        HANDLE hMutex;
        bool m_isLocked = false;
    public:
        explicit MutexLock(HANDLE _mutex) : hMutex(_mutex) { lock(); }
        ~MutexLock() {
            unlock();
        }
        bool isLocked() const { return m_isLocked; }
        void unlock() {
            if (!m_isLocked) return;
            ReleaseMutex(hMutex);
            m_isLocked = false;
        }
        void lock() {
            if (m_isLocked) return;
            const auto dwWaitResult = WaitForSingleObject(
                hMutex,    // handle to mutex
                INFINITE);  // no time-out interval
            if (dwWaitResult == WAIT_OBJECT_0) {
                m_isLocked = true;
                return;
            }
            m_isLocked = false;
        };
    };
}

class SharedMemoryHandler {
public:
    SharedMemoryHandler();
    ~SharedMemoryHandler();
    bool getData(std::string& data, std::chrono::milliseconds timeout);
    bool getDataMultiple(std::vector<std::string>& data, std::chrono::milliseconds timeout);
    bool sendData(const std::string& data);
    bool sendData(const char* data, size_t length);
    bool hasRequests();
    bool answerSyncRequest(const std::string& answer);
    bool waitForRequest(std::chrono::milliseconds timeout);
    bool getSyncRequest(std::string& request);
    bool getAsyncRequest(std::string& request);
    bool getAsyncRequestMultiple(std::vector<std::string>& requests);
    bool isConnected();
    void setConfigNeedsRefresh(bool param1) const;
    std::chrono::system_clock::time_point lastConnectedEvent;
    void setGameDisconnected();
    Signal<void()> onConnected;
    Signal<void()> onDisconnected;

private:
    bool createMemRegion();
    bool createMemMap();
    bool isReady();
    HANDLE hMapFile = nullptr;
    HANDLE hEventRequest = nullptr;
    HANDLE hEventResponse = nullptr;//Signals that a SYNC response is ready
    HANDLE hMutex = nullptr;
    HANDLE pMapView = nullptr;
    bool wasConnected = false;
};

