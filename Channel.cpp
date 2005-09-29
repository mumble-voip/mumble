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

#include "Channel.h"
#include "Player.h"
#include "Group.h"
#include "ACL.h"

QHash<int, Channel *> Channel::c_qhChannels;
QReadWriteLock Channel::c_qrwlChannels;

Channel::Channel(int id, QString name, QObject *p) : QObject(p) {
	iId = id;
	iParent = 0;
	qsName = name;
	bInheritACL = true;
	cParent = qobject_cast<Channel *>(p);
	if (cParent)
		cParent->addChannel(this);
}

Channel::~Channel() {
	foreach(ChanACL *acl, qlACL)
		delete acl;
	foreach(Group *g, qhGroups)
		delete g;

	Q_ASSERT(qlChannels.count() == 0);
	Q_ASSERT(qlPlayers.count() == 0);
	Q_ASSERT(children().count() == 0);
}

Channel *Channel::get(int id) {
	QReadLocker lock(&c_qrwlChannels);
	return c_qhChannels.value(id);
}

Channel *Channel::add(int id, QString name, QObject *po) {
	QWriteLocker lock(&c_qrwlChannels);

	Channel *c = new Channel(id,name, po);
	c_qhChannels[id] = c;
	return c;
}

void Channel::remove(Channel *c) {
	QWriteLocker lock(&c_qrwlChannels);
	c_qhChannels.remove(c->iId);
}

void Channel::addChannel(Channel *c) {
	c->iParent = iId;
	c->cParent = this;
	c->setParent(this);
	qlChannels << c;
}

void Channel::removeChannel(Channel *c) {
	c->iParent = 0;
	c->cParent = NULL;
	c->setParent(NULL);
	qlChannels.removeAll(c);
}

void Channel::addPlayer(Player *p) {
	if (p->cChannel)
		p->cChannel->removePlayer(p);
	p->setParent(this);
	p->cChannel = this;
	qlPlayers << p;
}

void Channel::removePlayer(Player *p) {
	qlPlayers.removeAll(p);
}
