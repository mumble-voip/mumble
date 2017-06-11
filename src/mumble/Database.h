// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DATABASE_H_
#define MUMBLE_MUMBLE_DATABASE_H_

#include "Settings.h"
#include "UnresolvedServerAddress.h"

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
		~Database() Q_DECL_OVERRIDE;
		static QList<FavoriteServer> getFavorites();
		static void setFavorites(const QList<FavoriteServer> &servers);
		static void setPassword(const QString &host, unsigned short port, const QString &user, const QString &pw);
		static bool fuzzyMatch(QString &name, QString &user, QString &pw, QString &host, unsigned short port);

		static bool isLocalIgnored(const QString &hash);
		static void setLocalIgnored(const QString &hash, bool ignored);

		static bool isLocalMuted(const QString &hash);
		static void setLocalMuted(const QString &hash, bool muted);

		static float getUserLocalVolume(const QString &hash);
		static void setUserLocalVolume(const QString &hash, float volume);

		static bool isChannelFiltered(const QByteArray &server_cert_digest, const int channel_id);
		static void setChannelFiltered(const QByteArray &server_cert_digest, const int channel_id, bool hidden);

		static QMap<UnresolvedServerAddress, unsigned int> getPingCache();
		static void setPingCache(const QMap<UnresolvedServerAddress, unsigned int> &cache);

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

#endif
