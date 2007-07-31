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

#ifndef _SERVER_H
#define _SERVER_H

#include "murmur_pch.h"
#include "Message.h"
#include "Timer.h"
#include "Player.h"
#include "Connection.h"

class Channel;
class PacketDataStream;

typedef QPair<quint32, quint16> Peer;

// Unfortunately, this needs to be "large enough" to hold
// enough frames to account for both short-term and
// long-term "maladjustments".

#define N_BANDWIDTH_SLOTS 360

struct BandwidthRecord {
	int iRecNum;
	int iSum;
	Timer qtFirst;
	unsigned char a_iBW[N_BANDWIDTH_SLOTS];
	Timer a_qtWhen[N_BANDWIDTH_SLOTS];

	BandwidthRecord();
	void addFrame(int size);
	int bytesPerSec();
};

class LogEmitter : public QObject {
		Q_OBJECT
	signals:
		void newLogEntry(const QString &msg);
	public:
		LogEmitter(QObject *parent = NULL);
		void addLogEntry(const QString &msg);
};

class SslServer : public QTcpServer {
		Q_OBJECT;
	protected:
		QList<QSslSocket *> qlSockets;
		void incomingConnection(int);
	public:
		QSslSocket *nextPendingSSLConnection();
		SslServer(QObject *parent = NULL);
};

class Server;


// THIS is a no-go. Can't have multiple inheritance with QObjects.
// Though we don't use any of the signals from "Player"...
// Fug it.

class User : public Connection, public Player {
		Q_OBJECT
	protected:
		Server *s;
	public:
		BandwidthRecord bwr;
		quint32 uiAddress;
		quint16 usPort;
		QHostAddress qha;
		User(Server *parent, QSslSocket *socket);
};


class Server : public QThread, public MessageHandler {
		Q_OBJECT;
	protected:
		QQueue<int> qqIds;
		SslServer *qtsServer;
		QTimer *qtTimeout;
		int iServerNum;
	protected slots:
		void newClient();
		void connectionClosed(QString);
		void sslError(const QList<QSslError> &);
		void message(QByteArray &, Connection *cCon = NULL);
		void checkTimeout();
		void tcpTransmitData(QByteArray, unsigned int);
	public:
		QHash<unsigned int, User *> qhUsers;
		QHash<unsigned int, Channel *> qhChannels;
		QReadWriteLock qrwlUsers;

		ChanACL::ACLCache acCache;
		QMutex qmCache;

		bool hasPermission(Player *p, Channel *c, ChanACL::Perm perm);
		void clearACLCache();

		QHash<int, QByteArray> qhUserTextureCache;
		QHash<int, QString> qhUserNameCache;
		QHash<QString, int> qhUserIDCache;

		QList<QPair<quint32, int> > qlBans;

		void sendAll(Message *);
		void sendExcept(Message *, Connection *);
		void sendMessage(unsigned int, Message *);
		void sendMessage(Connection *, Message *);

		void log(QString s, Connection *c = NULL);

		void removeChannel(Channel *c, Player *src, Channel *dest = NULL);
		void playerEnterChannel(Player *u, Channel *c, bool quiet = false);

		void emitPacket(Message *msg);

		User *getUser(unsigned int);

		Server(int snum, QObject *parent = NULL);

		// Database / DBus functions. Implementation in ServerDB.cpp
		void initialize();
		typedef QPair<quint32, int> qpBan;
		int authenticate(QString &name, const QString &pw);
		Channel *addChannel(Channel *c, const QString &name);
		void removeChannel(const Channel *c);
		void readChannels(Channel *p = NULL);
		void updateChannel(const Channel *c);
		void readChannelPrivs(Channel *c);
		void setLastChannel(const Player *u);
		int readLastChannel(Player *p);
		void dumpChannel(const Channel *c);
		int getUserID(const QString &name);
		QString getUserName(int id);
		QByteArray getUserTexture(int id);
		void setPW(int id, const QString &pw);
		void addLink(Channel *c, Channel *l);
		void removeLink(Channel *c, Channel *l);
		void getBans();
		void saveBans();

		// From msgHandler. Implementation in Messages.cpp
		virtual void msgSpeex(Connection *, MessageSpeex *);
		virtual void msgServerAuthenticate(Connection *, MessageServerAuthenticate *);
		virtual void msgPing(Connection *, MessagePing *);
		virtual void msgServerReject(Connection *, MessageServerReject *);
		virtual void msgServerSync(Connection *, MessageServerSync *);
		virtual void msgServerJoin(Connection *, MessageServerJoin *);
		virtual void msgServerLeave(Connection *, MessageServerLeave *);
		virtual void msgPlayerMute(Connection *, MessagePlayerMute *);
		virtual void msgPlayerDeaf(Connection *, MessagePlayerDeaf *);
		virtual void msgPlayerSelfMuteDeaf(Connection *, MessagePlayerSelfMuteDeaf *);
		virtual void msgPlayerKick(Connection *, MessagePlayerKick *);
		virtual void msgPlayerBan(Connection *, MessagePlayerBan *);
		virtual void msgPlayerMove(Connection *, MessagePlayerMove *);
		virtual void msgPlayerRename(Connection *, MessagePlayerRename *);
		virtual void msgChannelAdd(Connection *, MessageChannelAdd *);
		virtual void msgChannelRemove(Connection *, MessageChannelRemove *);
		virtual void msgChannelMove(Connection *, MessageChannelMove *);
		virtual void msgChannelLink(Connection *, MessageChannelLink *);
		virtual void msgServerBanList(Connection *, MessageServerBanList *);
		virtual void msgTextMessage(Connection *, MessageTextMessage *);
		virtual void msgPermissionDenied(Connection *, MessagePermissionDenied *);
		virtual void msgEditACL(Connection *, MessageEditACL *);
		virtual void msgQueryUsers(Connection *, MessageQueryUsers *);
		virtual void msgTexture(Connection *, MessageTexture *);

		// UDP Handling
	protected:
		QUdpSocket *qusUdp;
		QHash<unsigned int, Peer> qhPeers;
		QHash<unsigned int, QHostAddress> qhHosts;
		void processMsg(PacketDataStream &pds, Connection *cCon);
		void sendMessage(unsigned int id, const char *data, int len, QByteArray &cache);
	signals:
		void tcpTransmit(QByteArray, unsigned int id);
	public:
		void fakeUdpPacket(Message *msg, Connection *source);
		void run();
};

struct ServerParams {
	QHostAddress qhaBind;
	int iPort;
	int iTimeout;
	int iMaxBandwidth;
	int iMaxUsers;
	QString qsPassword;
	QString qsWelcomeText;

	QString qsDatabase;

	QString qsDBDriver;
	QString qsDBUserName;
	QString qsDBPassword;
	QString qsDBHostName;
	QString qsDBPrefix;
	int iDBPort;

	QString qsDBus;

	QString qsLogfile;

	QString qsRegName;
	QString qsRegPassword;
	QString qsRegHost;
	QUrl qurlRegWeb;

	QString qsSSLCert;
	QString qsSSLKey;
	QString qsSSLStore;

	ServerParams();
	void read(QString fname = QString("murmur.ini"));
};

extern ServerParams g_sp;

#else
class Server;
#endif
