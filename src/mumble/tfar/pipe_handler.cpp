#include "pipe_handler.hpp"
#include "profilers.hpp"
#include <thread>


pipe_handler::pipe_handler() {
    waitForDataEvent = CreateEvent(
        NULL,    // default security attribute 
        TRUE,    // manual-reset event 
        TRUE,    // initial state = signaled 
        NULL);   // unnamed event object

}


pipe_handler::~pipe_handler() {
    CloseHandle(pipe);
    CloseHandle(waitForDataEvent);
}
extern volatile bool pipeConnected;
bool pipe_handler::getData(std::string& output, std::chrono::milliseconds timeout) {
    output.resize(4096);

    for (int it = 0; it < timeout.count(); it++) {
        if (pipe == INVALID_HANDLE_VALUE) openPipe();

        DWORD numBytesRead = 0;
        DWORD numBytesAvail = 0;

        if (PeekNamedPipe(pipe, NULL, 0, &numBytesRead, &numBytesAvail, NULL)) {
            if (numBytesAvail > 0) {

                BOOL result = ReadFile(
                    pipe,
                    &output[0], // the data from the pipe will be put here
                    4096, // number of bytes allocated
                    &numBytesRead, // this will store number of bytes actually read
                    NULL // not using overlapped IO
                );
                if (result) {
                    m_isConnected = true;
                    output.resize(numBytesRead);
                    return true;
                } else {
                    m_isConnected = false;
                    openPipe();
                    std::this_thread::sleep_for(50us);
                    return false;
                }
            }
        } else {
            m_isConnected = false;
            openPipe();
            std::this_thread::sleep_for(50us);
            return false;
        }
        std::this_thread::sleep_for(1ms);
    }

    return false;
}


bool pipe_handler::sendData(const std::string& data) const {
    return sendData(data.c_str(), data.length());
}


bool pipe_handler::sendData(const char* data, size_t length) const {
    DWORD written = 0;
    auto retn = WriteFile(pipe, data, (DWORD) length + 1, &written, NULL) != 0;//tricky conversion from BOOL to bool
    //FlushFileBuffers(pipe);
    return retn;
}

void pipe_handler::openPipe() {
    if (pipe != INVALID_HANDLE_VALUE) {
        CloseHandle(pipe);
        pipe = INVALID_HANDLE_VALUE;
    }
    SECURITY_DESCRIPTOR SD;
    InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&SD, TRUE, NULL, FALSE);
    SECURITY_ATTRIBUTES SA;
    SA.nLength = sizeof(SA);
    SA.lpSecurityDescriptor = &SD;
    SA.bInheritHandle = TRUE;
    pipe = CreateFile(
        PIPE_NAME,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        NULL
    );
    DWORD error = GetLastError();
    DWORD dwMode = PIPE_READMODE_MESSAGE;
    SetNamedPipeHandleState(
        pipe,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time
}
