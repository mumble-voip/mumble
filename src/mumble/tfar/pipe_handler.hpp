#pragma once
#include <Windows.h>
#include "common.hpp"
class pipe_handler {
public:
    pipe_handler();
    ~pipe_handler();
    bool getData(std::string& output, std::chrono::milliseconds timeout);
    bool sendData(const std::string& data) const;
    bool sendData(const char* data, size_t length) const;
    bool isConnected() { return m_isConnected; }
private:
    void openPipe();
    HANDLE pipe = INVALID_HANDLE_VALUE;
    HANDLE waitForDataEvent = INVALID_HANDLE_VALUE;
    bool m_isConnected;
};

