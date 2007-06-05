/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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

class Player;
class Connection;
class Message;
class Channel;

typedef QPair<QHostAddress, quint16> Peer;

// Unfortunately, this needs to be "large enough" to hold
// enough frames to account for both short-term and
// long-term "maladjustments".

#define N_BANDWIDTH_SLOTS 360

struct BandwidthRecord {
	int iRecNum;
	int iSum;
	unsigned char a_iBW[N_BANDWIDTH_SLOTS];

	BandwidthRecord();
	void addFrame(int size);
	int bytesPerSec();
};

class Server : public QObject {
	Q_OBJECT;
	protected:
		QQueue<int> qqIds;
		QTcpServer *qtsServer;
		QUdpSocket *qusUdp;
		QTimer *qtTimer;
	protected slots:
		void newClient();
		void connectionClosed(QString);
		void message(QByteArray &, Connection *cCon = NULL);
		void udpReady();
		void checkCommands();
	public:
		QHash<short, Connection *> qmConnections;
		QHash<Connection *, Player *> qmPlayers;
		QHash<Connection *, BandwidthRecord *> qmBandwidth;
		QHash<Connection *, Peer> qhPeers;
		QHash<Peer, Connection *> qhPeerConnections;

		QHash<int, QString> qhUserNameCache;
		QHash<QString, int> qhUserIDCache;

		QList<QPair<quint32, int> > qlBans;

		void sendAll(Message *);
		void sendExcept(Message *, Connection *);
		void sendMessage(short, Message *);
		void sendMessage(Connection *, Message *);

		void log(QString s, Connection *c = NULL);

		void removeChannel(Channel *c, Player *src, Channel *dest = NULL);
		void playerEnterChannel(Player *p, Channel *c, bool quiet = false);

		Server();
};

struct ServerParams {
	int iPort;
	int iCommandFrequency;
	int iMaxBandwidth;
	int iMaxUsers;
	QString qsPassword;
	QString qsWelcomeText;
	QString qsDatabase;
	bool bTestloop;
	ServerParams();
	void read(QString fname = QString("murmur.ini"));
};

extern ServerParams g_sp;
extern Server *g_sServer;

#else
class Server;
#endif
