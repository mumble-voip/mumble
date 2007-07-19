/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _DBUS_H
#define _DBUS_H

#include "murmur_pch.h"

#include "Player.h"
#include "Channel.h"
#include "Group.h"
#include "ACL.h"

struct PlayerInfo {
	unsigned int session;
	bool mute, deaf, suppressed;
	bool selfMute, selfDeaf;
	int channel;
	PlayerInfo() { };
	PlayerInfo(Player *);
};
Q_DECLARE_METATYPE(PlayerInfo);

struct PlayerInfoExtended : public PlayerInfo {
	int id;
	QString name;
	int onlinesecs;
	int bytespersec;
	PlayerInfoExtended() {};
	PlayerInfoExtended(Player *);
};
Q_DECLARE_METATYPE(PlayerInfoExtended);
Q_DECLARE_METATYPE(QList<PlayerInfoExtended>);

struct ChannelInfo {
	int id;
	QString name;
	int parent;
	QList<int> links;
	ChannelInfo() { };
	ChannelInfo(Channel *c);
};
Q_DECLARE_METATYPE(ChannelInfo);
Q_DECLARE_METATYPE(QList<ChannelInfo>);

struct GroupInfo {
	QString name;
	bool inherited, inherit, inheritable;
	QList<int> add, remove, members;
	GroupInfo() { };
	GroupInfo(Group *g);
};
Q_DECLARE_METATYPE(GroupInfo);
Q_DECLARE_METATYPE(QList<GroupInfo>);

struct ACLInfo {
	bool applyHere, applySubs, inherited;
	int playerid;
	QString group;
	unsigned int allow, deny;
	ACLInfo() { };
	ACLInfo(ChanACL *acl);
};
Q_DECLARE_METATYPE(ACLInfo);
Q_DECLARE_METATYPE(QList<ACLInfo>);

struct BanInfo {
	unsigned int address;
	int bits;
	BanInfo() { };
	BanInfo(QPair<quint32,int>);
};
Q_DECLARE_METATYPE(BanInfo);
Q_DECLARE_METATYPE(QList<BanInfo>);

class MurmurDBus : public QDBusAbstractAdaptor {
		Q_OBJECT
		Q_CLASSINFO("D-Bus Interface", "net.sourceforge.mumble.Murmur");
	protected:
		QString qsAuthService;
		QString qsAuthPath;
	public:
		QDBusConnection qdbc;

		MurmurDBus(QCoreApplication &application);

		void playerStateChanged(Player *p);
		void playerConnected(Player *p);
		void playerDisconnected(Player *p);

		void channelStateChanged(Channel *c);
		void channelCreated(Channel *c);
		void channelRemoved(Channel *c);
		static void registerTypes();

		int mapNameToId(const QString &name);
		QString mapIdToName(int id);
		QByteArray mapIdToTexture(int id);
		int authenticate(const QString &uname, const QString &pw);
	public slots:
		// Order of paremeters is IMPORTANT, or Qt will barf.
		// Needs to be:
		// First all input parameters (non-ref or const-ref)
		// Then const QDbusMessage ref
		// Then output paremeters (ref)
		// Unfortunately, this makes things look chaotic, but luckily it looks sane again when introspected.
		// make SURE arguments have sane names, the argument-name will be exported in introspection xml.

		void getPlayers(QList<PlayerInfoExtended> &player_list);
		void getChannels(QList<ChannelInfo> &channel_list);

		void getACL(int channel, const QDBusMessage &, QList<ACLInfo> &acls,QList<GroupInfo> &groups, bool &inherit);
		void setACL(int channel, const QList<ACLInfo> &acls, const QList<GroupInfo> &groups, bool inherit, const QDBusMessage &);

		void getBans(QList<BanInfo> &bans);
		void setBans(const QList<BanInfo> &bans, const QDBusMessage &);

		void kickPlayer(unsigned int session, const QString &reason, const QDBusMessage &);
		void getPlayerState(unsigned int session, const QDBusMessage &, PlayerInfo &state);
		void setPlayerState(const PlayerInfo &state, const QDBusMessage &);
		void setChannelState(const ChannelInfo &state, const QDBusMessage &);

		void removeChannel(int id, const QDBusMessage &);
		void addChannel(const QString &name, int parent, const QDBusMessage &, int &newid);

		void getPlayerNames(const QList<int> &ids, const QDBusMessage &, QList<QString> &names);
		void getPlayerIds(const QList<QString> &names, const QDBusMessage &, QList<int> &ids);

		void setAuthenticator(const QDBusObjectPath &path, const QDBusMessage &);
		void setTemporaryGroups(int channel, int playerid, const QStringList &groups, const QDBusMessage &);
	signals:
		void playerStateChanged(const PlayerInfo &state);
		void playerConnected(const PlayerInfo &state);
		void playerDisconnected(const PlayerInfo &state);

		void channelStateChanged(const ChannelInfo &state);
		void channelCreated(const ChannelInfo &state);
		void channelRemoved(const ChannelInfo &state);
};

extern MurmurDBus *dbus;

#else
class MurmurDBus;
#endif
