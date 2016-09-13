// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_DATABASE_H_
#define MUMBLE_MURMUR_DATABASE_H_

#include <QtCore/QVariant>

#include "Timer.h"

class Channel;
class User;
class Connection;
class QSqlDatabase;
class QSqlQuery;

class ServerDB {
	public:
		enum ChannelInfo { Channel_Description, Channel_Position, Channel_Max_Users };
		enum UserInfo { User_Name, User_Email, User_Comment, User_Hash, User_Password, User_LastActive, User_KDFIterations };
		ServerDB();
		~ServerDB();
		typedef QPair<unsigned int, QString> LogRecord;
		static Timer tLogClean;
		static QSqlDatabase *db;
		static QString qsUpgradeSuffix;
		static void setSUPW(int iServNum, const QString &pw);
		static void disableSU(int srvnum);
		static QList<int> getBootServers();
		static QList<int> getAllServers();
		static int addServer();
		static void deleteServer(int server_id);
		static bool serverExists(int num);
		static QMap<QString, QString> getAllConf(int server_id);
		static QVariant getConf(int server_id, const QString &key, QVariant def = QVariant());
		static void setConf(int server_id, const QString &key, const QVariant &value = QVariant());
		static QList<LogRecord> getLog(int server_id, unsigned int offs_min, unsigned int offs_max);
		static QString getLegacySHA1Hash(const QString &password);
		static int getLogLen(int server_id);
		static void wipeLogs();
		static bool prepare(QSqlQuery &, const QString &, bool fatal = true, bool warn = true);
		static bool query(QSqlQuery &, const QString &, bool fatal = true, bool warn = true);
		static bool exec(QSqlQuery &, const QString &str = QString(), bool fatal= true, bool warn = true);
		static bool execBatch(QSqlQuery &, const QString &str = QString(), bool fatal= true);
		// No copy; private declaration without implementation
		ServerDB(const ServerDB &);
		
	private:
		static void loadOrSetupMetaPKBDF2IterationsCount(QSqlQuery &query);
		static void writeSUPW(int srvnum, const QString &pwHash, const QString &saltHash, const QVariant &kdfIterations);
};

#endif
