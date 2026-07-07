#pragma once

#include <Windows.h>
/*
could also use std::shared_lock and std::unique_lock from
#include <shared_mutex>
#include <mutex>
This would make it easier to switch from our lock types to std::mutex types and keep one pair of functions to lock stuff...
But i already created this new one so.... nah

*/

#include "ProfilerTracy.hpp"

template<class LOCK>
class LockGuard_exclusive {
    LOCK* m_lock = nullptr;
    CRITICAL_SECTION* m_criticalSection = nullptr;
    bool isLocked = false;

public:
    explicit LockGuard_exclusive(LOCK& _cs) : m_lock(&_cs), isLocked(true) {
        m_lock->lockExclusive();
    }

    // Compatibility with old TFAR code that still passes raw CRITICAL_SECTION.
    explicit LockGuard_exclusive(CRITICAL_SECTION& _cs) : m_criticalSection(&_cs), isLocked(true) {
        EnterCriticalSection(m_criticalSection);
    }

    explicit LockGuard_exclusive(CRITICAL_SECTION* _cs) : m_criticalSection(_cs), isLocked(_cs != nullptr) {
        if (m_criticalSection) {
            EnterCriticalSection(m_criticalSection);
        }
    }

    ~LockGuard_exclusive() {
        unlock();
    }

    void unlock() {
        if (!isLocked) {
            return;
        }

        if (m_lock) {
            m_lock->unlockExclusive();
        } else if (m_criticalSection) {
            LeaveCriticalSection(m_criticalSection);
        }

        isLocked = false;
    }
};


template<class LOCK>
class LockGuard_shared {
    LOCK* m_lock = nullptr;
    CRITICAL_SECTION* m_criticalSection = nullptr;
    bool isLocked = false;

public:
    explicit LockGuard_shared(LOCK& _cs) : m_lock(&_cs), isLocked(true) {
        m_lock->lockShared();
    }

    // Compatibility with old TFAR code that still passes raw CRITICAL_SECTION.
    explicit LockGuard_shared(CRITICAL_SECTION& _cs) : m_criticalSection(&_cs), isLocked(true) {
        EnterCriticalSection(m_criticalSection);
    }

    explicit LockGuard_shared(CRITICAL_SECTION* _cs) : m_criticalSection(_cs), isLocked(_cs != nullptr) {
        if (m_criticalSection) {
            EnterCriticalSection(m_criticalSection);
        }
    }

    ~LockGuard_shared() {
        unlock();
    }

    void unlock() {
        if (!isLocked) {
            return;
        }

        if (m_lock) {
            m_lock->unlockShared();
        } else if (m_criticalSection) {
            LeaveCriticalSection(m_criticalSection);
        }

        isLocked = false;
    }
};


class ReadWriteLock_impl {  //Consider SRW locks if you are reading at least 4:1 vs writing
    SRWLOCK m_lock{ SRWLOCK_INIT };
public:
    ReadWriteLock_impl() {}
    ReadWriteLock_impl(std::string_view) {}
    void lockExclusive() {
        AcquireSRWLockExclusive(&m_lock);
    }
    void lockShared() {
        AcquireSRWLockShared(&m_lock);
    }
    void unlockExclusive() {
        ReleaseSRWLockExclusive(&m_lock);
    }
    void unlockShared() {
        ReleaseSRWLockShared(&m_lock);
    }

    void lock() {
        lockExclusive();
    }
    void unlock() {
        unlockExclusive();
    }
    void lock_shared() {
        lockShared();
    }
    void unlock_shared() {
        unlockShared();
    }
};

class CriticalSectionLock_impl {  //Consider SRW locks if you are reading at least 4:1 vs writing
    CRITICAL_SECTION m_lock;
public:
    CriticalSectionLock_impl() { InitializeCriticalSection(&m_lock); }
    CriticalSectionLock_impl(std::string_view) { InitializeCriticalSection(&m_lock); }

    ~CriticalSectionLock_impl() {
        DeleteCriticalSection(&m_lock);
    }
    void lockExclusive() {
        EnterCriticalSection(&m_lock);
    }
    void lockShared() {
        EnterCriticalSection(&m_lock);
    }
    void unlockExclusive() {
        LeaveCriticalSection(&m_lock);
    }
    void unlockShared() {
        LeaveCriticalSection(&m_lock);
    }


    void lock() {
        lockExclusive();
    }
    void unlock() {
        unlockExclusive();
    }
    void lock_shared() {
        lockShared();
    }
    void unlock_shared() {
        unlockShared();
    }
};

#if ENABLE_TRACY_LOCK_PROFILING
class ReadWriteLock {
    ProfilerScope sc;
    tracy::SharedLockable<ReadWriteLock_impl> lock;

public:
    ReadWriteLock(std::string_view name) : sc(nullptr, name.data(), 0), lock((tracy::SourceLocationData*)& sc.data) {

    }


    void lockExclusive() {
        lock.lock();
    }
    void lockShared() {
        lock.lock_shared();
    }
    void unlockExclusive() {
        lock.unlock();
    }
    void unlockShared() {
        lock.unlock_shared();
    }

};

class CriticalSectionLock {
    ProfilerScope sc;
    tracy::SharedLockable<ReadWriteLock_impl> lock;
public:
    CriticalSectionLock(std::string_view name): sc(nullptr, name.data(), 0), lock((tracy::SourceLocationData*)&sc.data) {

    }


    void lockExclusive() {
        lock.lock();
    }
    void lockShared() {
        lock.lock_shared();
    }
    void unlockExclusive() {
        lock.unlock();
    }
    void unlockShared() {
        lock.unlock_shared();
    }
};

#else
using ReadWriteLock = ReadWriteLock_impl;
using CriticalSectionLock = CriticalSectionLock_impl;
#endif
