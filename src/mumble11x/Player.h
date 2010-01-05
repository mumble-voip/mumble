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

#ifndef _PLAYER_H
#define _PLAYER_H

#include "murmur_pch.h"

class Channel;

class Player {
	private:
		Q_DISABLE_COPY(Player)
	public:
		enum State { Connected, Authenticated };
		State sState;
		unsigned int uiSession;
		int iId;
		QString qsName;
		bool bMute, bDeaf, bSuppressed;
		bool bLocalMute;
		bool bSelfMute, bSelfDeaf;
		bool bTalking, bAltSpeak;
		Channel *cChannel;
		QString getFlagsString() const;

		Player();
		virtual ~Player() {};

		operator const QString() const;
};

class ClientPlayer : public QObject, public Player {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ClientPlayer)
	public:
		ClientPlayer(QObject *p = NULL);
		static QHash<unsigned int, ClientPlayer *> c_qmPlayers;
		static QReadWriteLock c_qrwlPlayers;
		static ClientPlayer *get(unsigned int);
		static ClientPlayer *add(unsigned int, QObject *p = NULL);
		static ClientPlayer *match(const ClientPlayer *p, bool matchname = false);
		static void remove(unsigned int);
		static void remove(ClientPlayer *);
	public slots:
		void setTalking(bool talking, bool altspeech);
		void setMute(bool mute);
		void setDeaf(bool deaf);
		void setLocalMute(bool mute);
		void setSelfMuteDeaf(bool mute, bool deaf);
	signals:
		void talkingChanged(bool talking);
		void muteDeafChanged();
};

#else
class Player;
#endif
