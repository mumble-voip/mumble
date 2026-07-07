#include "profilers.hpp"
#include <thread>
#include <sstream>
#include "Logger.hpp"

void profiler::log(const std::string& message) {
    //log_string(message, LogLevel_WARNING);
    //OutputDebugStringA(message.c_str());
    //OutputDebugStringA("\n");
    Logger::log(LoggerTypes::profiler, message);
}


#if ENABLE_API_PROFILER

static const float APIProfiler_ReportIntervalSecs = 1.0f;

float APIProfiler::s_ooFrequency = 0;
INT64 APIProfiler::s_reportInterval = 0;

//------------------------------------------------------------------
// Flush is called at the rate determined by APIProfiler_ReportIntervalSecs
//------------------------------------------------------------------
void APIProfiler::Flush(int64_t end) {
    // Auto-initialize globals based on timer frequency:
    if (s_reportInterval == 0) {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        s_ooFrequency = 1.0f / freq.QuadPart;
        LONG Barrier;
        _InterlockedOr(&Barrier, 0);
        s_reportInterval = static_cast<INT64>(freq.QuadPart * APIProfiler_ReportIntervalSecs);
    }

    // Avoid garbage timing on first call by initializing a new interval:
    if (m_threadInfo->lastReportTime == 0) {
        m_threadInfo->lastReportTime = m_start;
        return;
    }

    // Enough time has elapsed. Print statistics to console:
    const auto interval = (end - m_threadInfo->lastReportTime) * s_ooFrequency;
    const auto measured = m_threadInfo->accumulator * s_ooFrequency;
    char buffer[512];
    snprintf(buffer, "TID 0x%zu time spent in \"%s\": %.3f/%.3f ms %.1f%% %llux %.5f microsec per-call",
        std::hash<std::thread::id>()(std::this_thread::get_id()),
        m_threadInfo->name,
        measured * 1000,
        interval * 1000,
        100.f * measured / interval,
        m_threadInfo->hitCount,
        (measured * 1000 * 1000) / m_threadInfo->hitCount);
    profiler::log(std::string(buffer));

    // Reset statistics and begin next timing interval:
    m_threadInfo->lastReportTime = end;
    m_threadInfo->accumulator = 0;
    m_threadInfo->hitCount = 0;
}

#endif
