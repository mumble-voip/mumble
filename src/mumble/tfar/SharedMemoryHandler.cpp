#include "SharedMemoryHandler.hpp"
#include <thread>
#include "task_force_radio.hpp"
#include "settings.hpp"
#include "Logger.hpp"
using namespace SharedMemoryHandlerInternal;

bool SharedMemoryHandlerInternal::SharedMemoryData::getAsyncRequest(std::string& req) {
    ProfileFunction;
    setLastPluginTick();
    const auto asyncBase = reinterpret_cast<SharedMemString*>(reinterpret_cast<char*>(this) + 128 + sizeof(SharedMemString) * 2);
    if (nextFreeAsyncMessage == 0)
        return false;
    const auto position = (nextFreeAsyncMessage--) - 1;//Don't want next but last
    return asyncBase[position].assignToAndClear(req);
}

bool SharedMemoryData::getAsyncRequestMultiple(std::vector<std::string>& reqs) {
    ProfileFunction;
    setLastPluginTick();
    const auto asyncBase = reinterpret_cast<SharedMemString*>(reinterpret_cast<char*>(this) + 128 + sizeof(SharedMemString) * 2);
    if (nextFreeAsyncMessage == 0)
        return false;

    for (int position = 0; position < nextFreeAsyncMessage; ++position) { //Grab all messages in order
        reqs.emplace_back();
        auto& req = reqs.back();

        if (!asyncBase[position].assignToAndClear(req))
            reqs.pop_back();
    }

    nextFreeAsyncMessage = 0;

    return true;
}

bool SharedMemoryHandlerInternal::SharedMemoryData::getSyncRequest(std::string& req) {
    ProfileFunction;
    setLastPluginTick();
    auto syncReq = reinterpret_cast<SharedMemString*>(reinterpret_cast<char*>(this) + 128);
    return syncReq->assignToAndClear(req);
}

void SharedMemoryHandlerInternal::SharedMemoryData::setSyncResponse(const std::string& response) {
    ProfileFunction;
    setLastPluginTick();
    if (response.length() > SHAREDMEM_MAX_STRINGSIZE) {
        MessageBoxA(0, "TFAR Plugin Too big Sresponse", response.c_str(), 0);
        __debugbreak();//Response bigger than max allowed size
        return;
    }
    const auto syncResp = reinterpret_cast<SharedMemString*>(reinterpret_cast<char*>(this) + 128 + sizeof(SharedMemString));
    *syncResp = response;
}

bool SharedMemoryHandlerInternal::SharedMemoryData::hasAsyncRequest() const {
    return nextFreeAsyncMessage > 0;
}

bool SharedMemoryHandlerInternal::SharedMemoryData::hasSyncRequest() const {
    const auto syncResp = reinterpret_cast<const SharedMemString*>(reinterpret_cast<const char*>(this) + 128);
    return syncResp->length > 0;
}

std::string GetLastErrorString() {
    LPSTR messageBuffer = nullptr;
    const size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                       nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr);
    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);
    return message;
}

SharedMemoryHandler::SharedMemoryHandler() {
    //SECURITY_DESCRIPTOR SD;
    //InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);
    //SetSecurityDescriptorDacl(&SD, TRUE, NULL, FALSE);
    //SECURITY_ATTRIBUTES SA;
    //SA.nLength = sizeof(SA);
    //SA.lpSecurityDescriptor = &SD;
    //SA.bInheritHandle = TRUE;
    hEventRequest = CreateEvent(
        NULL,    // default security attribute 
        TRUE,    // manual-reset event 
        FALSE,    // initial state = signaled 
        L"Local\\TFARSHAMEM_EVTREQ"
    );
    if (!hEventRequest)
        Logger::log(LoggerTypes::teamspeakClientlog, "SHAMEM CreateEvent REQ " + GetLastErrorString(), LogLevel_ERROR);

    hEventResponse = CreateEvent(
        NULL,    // default security attribute 
        TRUE,    // manual-reset event 
        FALSE,    // initial state = signaled 
        L"Local\\TFARSHAMEM_EVTRESP"
    );
    if (!hEventResponse)
        Logger::log(LoggerTypes::teamspeakClientlog, "SHAMEM CreateEvent RESP " + GetLastErrorString(), LogLevel_ERROR);

    hMutex = CreateMutex(
        NULL,    // default security attribute 
        FALSE,
        L"Local\\TFARSHAMEM_MTX"
    );
    if (!hMutex)
        Logger::log(LoggerTypes::teamspeakClientlog, "SHAMEM CreateMutex " + GetLastErrorString(), LogLevel_ERROR);

    createMemMap();

    TFAR::getInstance().doDiagReport.connect([this](std::stringstream& diag) {
        diag << "SHAMEM:\n";
        diag << TS_INDENT << "hMapFile: " << hMapFile << "\n";
        diag << TS_INDENT << "hEventRequest: " << hEventRequest << "\n";
        diag << TS_INDENT << "hEventResponse: " << hEventResponse << "\n";
        diag << TS_INDENT << "hMutex: " << hMutex << "\n";
        diag << TS_INDENT << "pMapView: " << pMapView << "\n";
        diag << TS_INDENT << "wasConnected: " << wasConnected << "\n";
        diag << TS_INDENT << "isReady: " << isReady() << "\n";
        
        MutexLock lock(hMutex);
        SharedMemoryData* pData = static_cast<SharedMemoryData*>(pMapView);

        diag << TS_INDENT << TS_INDENT << "hasAsyncRequest: " << pData->hasAsyncRequest() << "\n";
        diag << TS_INDENT << TS_INDENT << "hasSyncRequest: " << pData->hasSyncRequest() << "\n";
        diag << TS_INDENT << TS_INDENT << "lastGameTick: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now()-pData->getLastGameTick()).count() << "us" << "\n";
    });

}


SharedMemoryHandler::~SharedMemoryHandler() {
    if (pMapView) {
        auto pData = static_cast<SharedMemoryData*>(pMapView);
        pData->onShutdown();
        UnmapViewOfFile(pMapView);
    }
    if (hMapFile) CloseHandle(hMapFile);
    if (hEventRequest) CloseHandle(hEventRequest);
    if (hMutex) CloseHandle(hMutex);
}


bool SharedMemoryHandler::getData(std::string& data, std::chrono::milliseconds timeout) {
    ProfileFunction;
    if (!waitForRequest(timeout))
        return false;
    if (!getSyncRequest(data))
        return getAsyncRequest(data);
    return true;
}

bool SharedMemoryHandler::getDataMultiple(std::vector<std::string>& data, std::chrono::milliseconds timeout) {
    ProfileFunction;
    data.clear();
    if (!waitForRequest(timeout))
        return false;
    std::string sync;
    if (getSyncRequest(sync)) {
        data.emplace_back(std::move(sync));
        return true;
    }
    return getAsyncRequestMultiple(data);
}

bool SharedMemoryHandler::sendData(const std::string& data) {
    ProfileFunction;
    return answerSyncRequest(data);
}

bool SharedMemoryHandler::sendData(const char* data, size_t length) {
    return sendData(std::string(data, length));
}

bool SharedMemoryHandler::hasRequests() {
    if (!isReady()) return false;
    MutexLock lock(hMutex);
    const auto pData = static_cast<SharedMemoryData*>(pMapView);
    return pData->hasAsyncRequest() || pData->hasSyncRequest();
}

bool SharedMemoryHandler::answerSyncRequest(const std::string& answer) {
    if (!isReady()) return false;
    MutexLock lock(hMutex);
    auto pData = static_cast<SharedMemoryData*>(pMapView);
    pData->setSyncResponse(answer);
    //pData->setSyncReady(true);
    SetEvent(hEventResponse);
    return true;
}
bool SharedMemoryHandler::waitForRequest(std::chrono::milliseconds timeout) {
    ProfileFunction;
    if (!isReady()) {
        std::this_thread::sleep_for(1ms);//we don't want a loop running on 100% cpu
        return false;
    }
    if (hasRequests())
        return true;
    const auto waited = WaitForSingleObject(hEventRequest, static_cast<DWORD>(timeout.count()));
    ResetEvent(hEventRequest);
    return waited == WAIT_OBJECT_0;
}

bool SharedMemoryHandler::getSyncRequest(std::string& request) {
    if (!isReady()) return false;
    MutexLock lock(hMutex);
    auto pData = static_cast<SharedMemoryData*>(pMapView);
    return pData->getSyncRequest(request);
}

bool SharedMemoryHandler::getAsyncRequest(std::string& request) {
    if (!isReady()) return false;
    MutexLock lock(hMutex);
    auto* pData = static_cast<SharedMemoryData*>(pMapView);
    return pData->getAsyncRequest(request);
}

bool SharedMemoryHandler::getAsyncRequestMultiple(std::vector<std::string>& requests) {
    if (!isReady()) return false;
    MutexLock lock(hMutex);
    auto* pData = static_cast<SharedMemoryData*>(pMapView);
    return pData->getAsyncRequestMultiple(requests);
}

bool SharedMemoryHandler::isConnected() {
    ProfileFunction;
    const auto timeout = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(TFAR::config.get<float>(Setting::pluginTimeout)));
    MutexLock lock(hMutex);
    auto pData = static_cast<SharedMemoryData*>(pMapView);
    pData->setLastPluginTick();
    const auto lastGameTick = pData->getLastGameTick();
    lock.unlock();
    const auto isCurrentlyConnected = (std::chrono::system_clock::now() - lastGameTick) < timeout;
    if ((std::chrono::system_clock::now() - lastConnectedEvent) < 500ms)
        return isCurrentlyConnected;
    if (wasConnected && !isCurrentlyConnected) {
        onDisconnected();
        lastConnectedEvent = std::chrono::system_clock::now();
    } else if (!wasConnected && isCurrentlyConnected) {
        onConnected();
        lastConnectedEvent = std::chrono::system_clock::now();
    }
    wasConnected = isCurrentlyConnected;
    return isCurrentlyConnected;
}

void SharedMemoryHandler::setConfigNeedsRefresh(bool param1) const {
    auto pData = static_cast<SharedMemoryData*>(pMapView);
    pData->setConfigNeedsRefresh(param1);
}

void SharedMemoryHandler::setGameDisconnected() {
    if (!isReady()) return;
    MutexLock lock(hMutex);
    auto pData = static_cast<SharedMemoryData*>(pMapView);
    pData->setLastGameTick(std::chrono::system_clock::time_point(0us));
}

bool SharedMemoryHandler::createMemRegion() {
    if (hMapFile)
        CloseHandle(hMapFile);
    //SECURITY_DESCRIPTOR SD;
    //InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);
    //SetSecurityDescriptorDacl(&SD, TRUE, NULL, FALSE);
    //SECURITY_ATTRIBUTES SA;
    //SA.nLength = sizeof(SA);
    //SA.lpSecurityDescriptor = &SD;
    //SA.bInheritHandle = TRUE;
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        SHAREDMEM_BUFSIZE,                // maximum object size (low-order DWORD)
        L"Local\\TFARSHAMEM");                 // name of mapping object
    if (!hMapFile) {
        Logger::log(LoggerTypes::teamspeakClientlog, "SHAMEM CreateFileMapping " + GetLastErrorString(), LogLevel_ERROR);
        return false;
    }
    return true;
}

bool SharedMemoryHandler::createMemMap() {
    if (!hMapFile) {
        if (!createMemRegion())
            return false;
    }
    if (pMapView)
        CloseHandle(pMapView);

    pMapView = MapViewOfFile(hMapFile,   // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0,
        0,
        SHAREDMEM_BUFSIZE);
    if (!pMapView) {
        Logger::log(LoggerTypes::teamspeakClientlog, "SHAMEM MapViewOfFile " + GetLastErrorString(), LogLevel_ERROR);
        CloseHandle(hMapFile);
        return false;
    }

    new(pMapView) SharedMemoryData(SHAREDMEM_BUFSIZE);

    return true;
}

bool SharedMemoryHandler::isReady() {
    if (!pMapView) {
        if (!createMemMap())
            return false;
    }
    return true;
}

