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
        slots.push_back(slot);
    }

    std::vector<ReturnType> operator() (Args... args) const {
        return emit(args...);
    }
    std::vector<ReturnType> emit(Args... args) const {
        std::vector<ReturnType> returnData;
        if (slots.empty())
            return;
        for (auto &slot : slots) {
            returnData.push_back(slot(args...));
        }
        return returnData;
    }
    void removeAllSlots() {
        slots.clear();
    }
private:
    std::vector<Slot> slots{};
};


template<class... Args>
class Signal<void(Args...)> {
private:
    typedef std::function<void(Args...)> Slot;

public:
    void connect(Slot slot) {
        slots.push_back(slot);
    }

    void operator() (Args... args) const {
        return emit(args...);
    }
    void emit(Args... args) const {
        if (slots.empty())
            return;
        for (auto &slot : slots) {
            slot(args...);
        }
    }
    void removeAllSlots() {
        slots.clear();
    }
private:
    std::vector<Slot> slots{};
};