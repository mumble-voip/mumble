/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

QMap<short, Player *> g_qmPlayers;
QMutex g_qmPlayersMutex;

Player::Player() {
  sState = Player::Connected;
  sId = 0;
  bMute = bDeaf = false;
  bSelfMute = bSelfDeaf = false;
  bTalking = false;
}

Player *Player::get(short sId) {
	Player *p = NULL;
	g_qmPlayersMutex.lock();
	if (g_qmPlayers.contains(sId))
		p = g_qmPlayers[sId];
	g_qmPlayersMutex.unlock();
	return p;
}

Player *Player::add(short sId) {
	Player *p = new Player();
	p->sId = sId;
	g_qmPlayersMutex.lock();
	g_qmPlayers[sId] = p;
	g_qmPlayersMutex.unlock();
	return p;
}

void Player::remove(short sId) {
	g_qmPlayersMutex.lock();
	g_qmPlayers.remove(sId);
	g_qmPlayersMutex.unlock();
}

void Player::remove(Player *p) {
	g_qmPlayersMutex.lock();
	g_qmPlayers.remove(p->sId);
	g_qmPlayersMutex.unlock();
}

void Player::setTalking(bool talking) {
	if (bTalking == talking)
		return;
	bTalking = bTalking;
	emit talkingChanged(this, bTalking);
}
