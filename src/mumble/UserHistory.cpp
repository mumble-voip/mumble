#include "UserHistory.h"

UserHistory& UserHistory::instance() {
    static UserHistory instance;
    return instance;
}

UserHistory::UserHistory() {
}

UserHistory::~UserHistory() {
}

void UserHistory::addUser(const QString& username, const QString& hash, const QString& comment,
                         unsigned int userId, const QString& channelName, bool isListener) {
    UserHistoryEntry entry;
    entry.username = username;
    entry.hash = hash;
    entry.comment = comment;
    entry.lastSeen = QDateTime::currentDateTime();
    entry.userId = userId;
    entry.channelName = channelName;
    entry.isListener = isListener;
    
    m_userHistory[hash] = entry;
}

void UserHistory::updateUser(const QString& hash, const QString& channelName, bool isListener) {
    if (m_userHistory.contains(hash)) {
        m_userHistory[hash].lastSeen = QDateTime::currentDateTime();
        m_userHistory[hash].channelName = channelName;
        m_userHistory[hash].isListener = isListener;
    }
}

UserHistoryEntry* UserHistory::findUser(const QString& hash) {
    if (m_userHistory.contains(hash)) {
        return &m_userHistory[hash];
    }
    return nullptr;
}

QList<UserHistoryEntry> UserHistory::getAllUsers() const {
    return m_userHistory.values();
}

void UserHistory::clear() {
    m_userHistory.clear();
} 