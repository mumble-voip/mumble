/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net>

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

#ifndef MUMBLE_MURMUR_SERVERUSER_H_
#define MUMBLE_MURMUR_SERVERUSER_H_

#include <QtCore/QStringList>

#ifdef Q_OS_UNIX
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif

#include "Connection.h"
#include "Net.h"
#include "Timer.h"
#include "User.h"

// Unfortunately, this needs to be "large enough" to hold
// enough frames to account for both short-term and
// long-term "maladjustments".

#define N_BANDWIDTH_SLOTS 360

struct BandwidthRecord {
	int iRecNum;
	int iSum;
	Timer tFirst;
	Timer tIdleControl;
	unsigned short a_iBW[N_BANDWIDTH_SLOTS];
	Timer a_qtWhen[N_BANDWIDTH_SLOTS];

	BandwidthRecord();
	bool addFrame(int size, int maxpersec);
	int onlineSeconds() const;
	int idleSeconds() const;
	void resetIdleSeconds();
	int bandwidth() const;
};

struct WhisperTarget {
	struct Channel {
		int iId;
		bool bChildren;
		bool bLinks;
		QString qsGroup;
	};
	QList<unsigned int> qlSessions;
	QList<WhisperTarget::Channel> qlChannels;
};

class Server;

class ServerUser : public Connection, public User {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ServerUser)
	protected:
		Server *s;
	public:
		enum State { Connected, Authenticated };
		State sState;
		operator const QString() const;

		float dUDPPingAvg, dUDPPingVar;
		float dTCPPingAvg, dTCPPingVar;
		quint64 uiUDPPackets, uiTCPPackets;

		unsigned int uiVersion;
		QString qsRelease;
		QString qsOS;
		QString qsOSVersion;
		QString qsOSDisplayableVersion;

		std::string ssContext;
		QString qsIdentity;

		bool bVerified;
		QStringList qslEmail;

		HostAddress haAddress;
		bool bUdp;

		QList<int> qlCodecs;
		bool bOpus;

		QStringList qslAccessTokens;

		QMap<int, WhisperTarget> qmTargets;
		typedef QPair<QSet<ServerUser *>, QSet<ServerUser *> > TargetCache;
		QMap<int, TargetCache> qmTargetCache;
		QMap<QString, QString> qmWhisperRedirect;

		int iLastPermissionCheck;
		QMap<int, unsigned int> qmPermissionSent;
#ifdef Q_OS_UNIX
		int sUdpSocket;
#else
		SOCKET sUdpSocket;
#endif
		BandwidthRecord bwr;
		struct sockaddr_storage saiUdpAddress;
		struct sockaddr_storage saiTcpLocalAddress;
		ServerUser(Server *parent, QSslSocket *socket);
};

#endif
