/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _DATABASE_H
#define _DATABASE_H

#include "murmur_pch.h"
#include "Timer.h"

class Channel;
class User;
class Connection;

class ServerDB {
	public:
		enum ChannelInfo { Channel_Description, Channel_Position };
		enum UserInfo { User_Name, User_Email, User_Comment, User_Hash, User_Password, User_LastActive };
		ServerDB();
		~ServerDB();
		typedef QPair<unsigned int, QString> LogRecord;
		static Timer tLogClean;
		static QSqlDatabase *db;
		static QString qsUpgradeSuffix;
		static void setSUPW(int iServNum, const QString &pw);
		static QList<int> getBootServers();
		static QList<int> getAllServers();
		static int addServer();
		static void deleteServer(int server_id);
		static bool serverExists(int num);
		static QMap<QString, QString> getAllConf(int server_id);
		static QVariant getConf(int server_id, const QString &key, QVariant def = QVariant());
		static void setConf(int server_id, const QString &key, const QVariant &value = QVariant());
		static QList<LogRecord> getLog(int server_id, unsigned int offs_min, unsigned int offs_max);
		static int getLogLen(int server_id);
		static bool prepare(QSqlQuery &, const QString &, bool fatal = true, bool warn = true);
		static bool exec(QSqlQuery &, const QString &str = QString(), bool fatal= true, bool warn = true);
		static bool execBatch(QSqlQuery &, const QString &str = QString(), bool fatal= true);
};

#endif
