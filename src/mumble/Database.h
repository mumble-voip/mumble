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

#include "mumble_pch.hpp"
#include "Settings.h"

struct FavoriteServer {
	QString qsName;
	QString qsUsername;
	QString qsPassword;
	QString qsHostname;
	QString qsUrl;
	unsigned short usPort;
};

class Database : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(Database)
	public:
		Database();
		~Database();
		static QList<FavoriteServer> getFavorites();
		static void setFavorites(const QList<FavoriteServer> &servers);
		static void setPassword(const QString &host, unsigned short port, const QString &user, const QString &pw);
		static bool fuzzyMatch(QString &name, QString &user, QString &pw, QString &host, unsigned short port);

		static bool isLocalMuted(const QString &hash);
		static void setLocalMuted(const QString &hash, bool muted);

		static QMap<QPair<QString, unsigned short>, unsigned int> getPingCache();
		static void setPingCache(const QMap<QPair<QString, unsigned short>, unsigned int> &cache);

		static bool seenComment(const QString &hash, const QByteArray &commenthash);
		static void setSeenComment(const QString &hash, const QByteArray &commenthash);

		static QByteArray blob(const QByteArray &hash);
		static void setBlob(const QByteArray &hash, const QByteArray &blob);

		static QStringList getTokens(const QByteArray &digest);
		static void setTokens(const QByteArray &digest, QStringList &tokens);

		static QList<Shortcut> getShortcuts(const QByteArray &digest);
		static bool setShortcuts(const QByteArray &digest, QList<Shortcut> &shortcuts);

		static void addFriend(const QString &name, const QString &hash);
		static void removeFriend(const QString &hash);
		static const QString getFriend(const QString &hash);
		static const QMap<QString, QString> getFriends();

		static const QString getDigest(const QString &hostname, unsigned short port);
		static void setDigest(const QString &hostname, unsigned short port, const QString &digest);

		static bool getUdp(const QByteArray &digest);
		static void setUdp(const QByteArray &digest, bool udp);
};

#else
class Database;
#endif
