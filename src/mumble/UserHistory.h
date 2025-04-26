#ifndef MUMBLE_USERHISTORY_H_
#define MUMBLE_USERHISTORY_H_

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QDateTime>

struct UserHistoryEntry {
    QString username;
    QString hash;
    QString comment;
    QDateTime lastSeen;
    unsigned int userId;
    QString channelName;
    bool isListener;
};

class UserHistory : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(UserHistory)

public:
    static UserHistory& instance();
    
    void addUser(const QString& username, const QString& hash, const QString& comment, 
                unsigned int userId, const QString& channelName, bool isListener);
    void updateUser(const QString& hash, const QString& channelName, bool isListener);
    UserHistoryEntry* findUser(const QString& hash);
    QList<UserHistoryEntry> getAllUsers() const;
    void clear();

private:
    UserHistory();
    ~UserHistory();
    
    QHash<QString, UserHistoryEntry> m_userHistory;
};

#endif // MUMBLE_USERHISTORY_H_ 