/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include "ClientUser.h"
#include "Channel.h"

QHash<unsigned int, ClientUser *> ClientUser::c_qmUsers;
QReadWriteLock ClientUser::c_qrwlUsers;

ClientUser::ClientUser(QObject *p) : QObject(p) {
}

ClientUser *ClientUser::get(unsigned int uiSession) {
	QReadLocker lock(&c_qrwlUsers);
	ClientUser *p = c_qmUsers.value(uiSession);
	return p;
}

ClientUser *ClientUser::add(unsigned int uiSession, QObject *po) {
	QWriteLocker lock(&c_qrwlUsers);

	ClientUser *p = new ClientUser(po);
	p->uiSession = uiSession;
	c_qmUsers[uiSession] = p;
	return p;
}

ClientUser *ClientUser::match(const ClientUser *other, bool matchname) {
	QReadLocker lock(&c_qrwlUsers);

	ClientUser *p;
	foreach(p, c_qmUsers) {
		if (p == other)
			continue;
		if ((p->iId >= 0) && (p->iId == other->iId))
			return p;
		if (matchname && (p->qsName == other->qsName))
			return p;
	}
	return NULL;
}

void ClientUser::remove(unsigned int uiSession) {
	QWriteLocker lock(&c_qrwlUsers);
	User *p = c_qmUsers.take(uiSession);
	if (p && p->cChannel)
		p->cChannel->removeUser(p);
}

void ClientUser::remove(ClientUser *p) {
	remove(p->uiSession);
}

QString ClientUser::getFlagsString() const {
	QStringList flags;

	if (! qsFriendName.isEmpty())
		flags << ClientUser::tr("Friend");
	if (iId >= 0)
		flags << ClientUser::tr("Authenticated");
	if (bMute)
		flags << ClientUser::tr("Muted (server)");
	if (bDeaf)
		flags << ClientUser::tr("Deafened (server)");
	if (bLocalMute)
		flags << ClientUser::tr("Local Mute");
	if (bSelfMute)
		flags << ClientUser::tr("Muted (self)");
	if (bSelfDeaf)
		flags << ClientUser::tr("Deafened (self)");

	return flags.join(QLatin1String(", "));
}

void ClientUser::setTalking(bool talking, bool altspeech) {
	if ((bTalking == talking) && (bAltSpeak == altspeech))
		return;
	bTalking = talking;
	bAltSpeak = altspeech;
	emit talkingChanged(bTalking);
}

void ClientUser::setMute(bool mute) {
	if (bMute == mute)
		return;
	bMute = mute;
	if (! bMute)
		bDeaf = false;
	emit muteDeafChanged();
}

void ClientUser::setSuppress(bool suppress) {
	if (bSuppress == suppress)
		return;
	bSuppress = suppress;
	emit muteDeafChanged();
}

void ClientUser::setLocalMute(bool mute) {
	if (bLocalMute == mute)
		return;
	bLocalMute = mute;
	emit muteDeafChanged();
}

void ClientUser::setDeaf(bool deaf) {
	bDeaf = deaf;
	if (bDeaf)
		bMute = true;
	emit muteDeafChanged();
}

void ClientUser::setSelfMute(bool mute) {
	bSelfMute = mute;
	if (! mute)
		bSelfDeaf = false;
	emit muteDeafChanged();
}

void ClientUser::setSelfDeaf(bool deaf) {
	bSelfDeaf = deaf;
	if (deaf)
		bSelfMute = true;
	emit muteDeafChanged();
}

/* From Channel.h
 */
void Channel::addClientUser(ClientUser *p) {
	addUser(p);
	p->setParent(this);
}
