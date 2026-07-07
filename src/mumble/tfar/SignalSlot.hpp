#pragma once
#include <vector>
#include <functional>

template<class Sig>
class Signal;

template<class ReturnType, class... Args>
class Signal<ReturnType(Args...)> {
private:
    typedef std::function<ReturnType(Args...)> Slot;

public:
    void connect(Slot slot) {
        m_slots.push_back(slot);
    }

    std::vector<ReturnType> operator() (Args... args) const {
        return emitSignal(args...);
    }
    std::vector<ReturnType> emitSignal(Args... args) const {
        std::vector<ReturnType> returnData;
        if (m_slots.empty())
            return returnData;
        for (auto &slot : m_slots) {
            returnData.push_back(slot(args...));
        }
        return returnData;
    }
    void removeAllSlots() {
        m_slots.clear();
    }
private:
    std::vector<Slot> m_slots{};
};


template<class... Args>
class Signal<void(Args...)> {
private:
    typedef std::function<void(Args...)> Slot;

public:
    void connect(Slot slot) {
        m_slots.push_back(slot);
    }

    void operator() (Args... args) const {
        return emitSignal(args...);
    }
    void emitSignal(Args... args) const {
        if (m_slots.empty())
            return;
        for (auto &slot : m_slots) {
            slot(args...);
        }
    }
    void removeAllSlots() {
        m_slots.clear();
    }
private:
    std::vector<Slot> m_slots{};
};