#pragma once
#include <chrono>
#include "common.hpp"
#include <Windows.h>
#include <memory>
#include <utility>
#include "version.h"
#include <string>


namespace profiler {
    void log(const std::string& message);
}


#ifdef isCI
#define ENABLE_API_PROFILER 0     // Disabled for release builds
#define ENABLE_PLUGIN_LOGS 1
#else
//#define ENABLE_API_PROFILER 1     // Comment this line to disable the profiler
#define ENABLE_TRACY_PROFILER 1
#endif

#include "ProfilerTracy.hpp"


#if ENABLE_API_PROFILER



class speedTest {
public:
    explicit speedTest(std::string  name_, bool willPrintOnDestruct_ = true) :start(std::chrono::high_resolution_clock::now()), name(std::move(name_)), willPrintOnDestruct(willPrintOnDestruct_) {}
    ~speedTest() { if (willPrintOnDestruct) log(); }
    void log() const {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();
        profiler::log(name + " " + std::to_string(duration) + " microsecs");
    }
    void log(const std::string & text) {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();
        const auto message = name + "-" + text + " " + std::to_string(duration) + " microsecs";
        profiler::log(message);
        start += std::chrono::high_resolution_clock::now() - now; //compensate time for call to log func
    }
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }
    std::chrono::microseconds getCurrentElapsedTime() const {
        const auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now - start);
    }
    std::chrono::high_resolution_clock::time_point start;
    std::string name;
    bool willPrintOnDestruct;
};

//http://preshing.com/20111203/a-c-profiling-module-for-multithreaded-apis/









//------------------------------------------------------------------
// A class for local variables created on the stack by the API_PROFILER macro:
//------------------------------------------------------------------
class APIProfiler {
public:
    //------------------------------------------------------------------
    // A structure for each thread to store information about an API:
    //------------------------------------------------------------------
    struct ThreadInfo {
        int64_t lastReportTime;
        int64_t accumulator;   // total time spent in target module since the last report
        int64_t hitCount;      // number of times the target module was called since last report
        const char *name;    // the name of the target module
    };

private:
    int64_t m_start;
    ThreadInfo *m_threadInfo;

    static float s_ooFrequency;      // 1.0 divided by QueryPerformanceFrequency()
    static int64_t s_reportInterval;   // length of time between reports
    void Flush(int64_t end);

public:
    __forceinline APIProfiler(ThreadInfo *threadInfo) {
        LARGE_INTEGER start;
        QueryPerformanceCounter(&start);
        m_start = start.QuadPart;
        m_threadInfo = threadInfo;
    }

    __forceinline ~APIProfiler() {
        LARGE_INTEGER end;
        QueryPerformanceCounter(&end);
        m_threadInfo->accumulator += (end.QuadPart - m_start);
        m_threadInfo->hitCount++;
        if (end.QuadPart - m_threadInfo->lastReportTime > s_reportInterval)
            Flush(end.QuadPart);
    }
};

//----------------------
// Profiler is enabled
//----------------------
#define DECLARE_API_PROFILER(name) \
    extern __declspec(thread) APIProfiler::ThreadInfo __APIProfiler_##name;

#define DEFINE_API_PROFILER(name) \
    __declspec(thread) APIProfiler::ThreadInfo __APIProfiler_##name = { 0, 0, 0, #name };

#define TOKENPASTE2(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE2(x, y)
#define API_PROFILER(name) \
    APIProfiler TOKENPASTE(__APIProfiler_##name, __LINE__)(&__APIProfiler_##name)

#else

//----------------------
// Profiler is disabled
//----------------------
#define DECLARE_API_PROFILER(name)
#define DEFINE_API_PROFILER(name)
#define API_PROFILER(name)

class speedTest {
public:
    explicit speedTest(const std::string & name_, bool willPrintOnDestruct_ = true) {}
    ~speedTest() { return; }
    void log() const {
        return;
    }
    void log(const std::string & text) {
        return;
    }
    void reset() {
        return;
    }
    std::chrono::microseconds getCurrentElapsedTime() const {
        return 0us;
    }
};


#endif




