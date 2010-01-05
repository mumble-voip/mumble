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

#include "Player.h"
#include "Channel.h"

QHash<unsigned int, ClientPlayer *> ClientPlayer::c_qmPlayers;
QReadWriteLock ClientPlayer::c_qrwlPlayers;

Player::Player() {
	sState = Player::Connected;
	uiSession = 0;
	iId = -1;
	bMute = bDeaf = false;
	bSelfMute = bSelfDeaf = false;
	bTalking = bAltSpeak = false;
	bLocalMute = false;
	bSuppressed = false;
	cChannel = 0;
}

QString Player::getFlagsString() const {
	QStringList flags;

	if (iId >= 0)
		flags << ClientPlayer::tr("Authenticated");
	if (bMute)
		flags << ClientPlayer::tr("Muted (server)");
	if (bDeaf)
		flags << ClientPlayer::tr("Deafened (server)");
	if (bLocalMute)
		flags << ClientPlayer::tr("Local Mute");
	if (bSelfMute)
		flags << ClientPlayer::tr("Muted (self)");
	if (bSelfDeaf)
		flags << ClientPlayer::tr("Deafened (self)");

	return flags.join(QLatin1String(", "));
}

Player::operator const QString() const {
	return QString::fromLatin1("%1:%2(%3)").arg(qsName).arg(uiSession).arg(iId);
}

ClientPlayer::ClientPlayer(QObject *p) : QObject(p) {
}

ClientPlayer *ClientPlayer::get(unsigned int uiSession) {
	QReadLocker lock(&c_qrwlPlayers);
	ClientPlayer *p = c_qmPlayers.value(uiSession);
	return p;
}

ClientPlayer *ClientPlayer::add(unsigned int uiSession, QObject *po) {
	QWriteLocker lock(&c_qrwlPlayers);

	ClientPlayer *p = new ClientPlayer(po);
	p->uiSession = uiSession;
	c_qmPlayers[uiSession] = p;
	return p;
}

ClientPlayer *ClientPlayer::match(const ClientPlayer *other, bool matchname) {
	QReadLocker lock(&c_qrwlPlayers);

	ClientPlayer *p;
	foreach(p, c_qmPlayers) {
		if (p == other)
			continue;
		if ((p->iId >= 0) && (p->iId == other->iId))
			return p;
		if (matchname && (p->qsName == other->qsName))
			return p;
	}
	return NULL;
}

void ClientPlayer::remove(unsigned int uiSession) {
	QWriteLocker lock(&c_qrwlPlayers);
	Player *p = c_qmPlayers.take(uiSession);
	if (p && p->cChannel)
		p->cChannel->removePlayer(p);
}

void ClientPlayer::remove(ClientPlayer *p) {
	remove(p->uiSession);
}

void ClientPlayer::setTalking(bool talking, bool altspeech) {
	if ((bTalking == talking) && (bAltSpeak == altspeech))
		return;
	bTalking = talking;
	bAltSpeak = altspeech;
	emit talkingChanged(bTalking);
}

void ClientPlayer::setMute(bool mute) {
	if (bMute == mute)
		return;
	bMute = mute;
	if (! bMute && bDeaf)
		bDeaf = false;
	emit muteDeafChanged();
}

void ClientPlayer::setLocalMute(bool mute) {
	if (bLocalMute == mute)
		return;
	bLocalMute = mute;
	emit muteDeafChanged();
}

void ClientPlayer::setDeaf(bool deaf) {
	if (bDeaf == deaf)
		return;
	bDeaf = deaf;
	if (bDeaf && ! bMute)
		bMute = true;
	emit muteDeafChanged();
}

void ClientPlayer::setSelfMuteDeaf(bool mute, bool deaf) {
	bSelfDeaf = deaf;
	bSelfMute = mute;
	emit muteDeafChanged();
}
