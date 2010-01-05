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

#include "Channel.h"
#include "Player.h"
#include "Group.h"
#include "ACL.h"

QHash<int, Channel *> Channel::c_qhChannels;
QReadWriteLock Channel::c_qrwlChannels;

uint qHash(const Channel::qpPlayerLink & pl) {
	return qHash(pl.first) + qHash(pl.second);
}

Channel::Channel(int id, const QString &name, const QString &desc, QObject *p) : QObject(p) {
	iId = id;
	qsName = name;
	qsDesc = desc;
	bInheritACL = true;
	cParent = qobject_cast<Channel *>(p);
	if (cParent)
		cParent->addChannel(this);
}

Channel::~Channel() {
	if (cParent)
		cParent->removeChannel(this);

	foreach(Channel *c, qlChannels)
		delete c;

	foreach(ChanACL *acl, qlACL)
		delete acl;
	foreach(Group *g, qhGroups)
		delete g;
	foreach(Channel *l, qhLinks.keys())
		unlink(l);

	Q_ASSERT(qlChannels.count() == 0);
	Q_ASSERT(children().count() == 0);
}

Channel *Channel::get(int id) {
	QReadLocker lock(&c_qrwlChannels);
	return c_qhChannels.value(id);
}

Channel *Channel::add(int id, const QString &name, const QString &desc) {
	QWriteLocker lock(&c_qrwlChannels);

	if (c_qhChannels.contains(id))
		return NULL;

	Channel *c = new Channel(id,name, desc, NULL);
	c_qhChannels.insert(id, c);
	return c;
}

void Channel::remove(Channel *c) {
	QWriteLocker lock(&c_qrwlChannels);
	c_qhChannels.remove(c->iId);
}

bool Channel::isLinked(Channel *l) {
	return qhLinks.contains(l);
}

void Channel::link(Channel *l) {
	if (qsPermLinks.contains(l))
		return;
	qsPermLinks.insert(l);
	qhLinks[l]++;
	l->qsPermLinks.insert(this);
	l->qhLinks[this]++;
}

void Channel::unlink(Channel *l) {
	if (l) {
		qsPermLinks.remove(l);
		qhLinks.remove(l);
		l->qsPermLinks.remove(this);
		l->qhLinks.remove(this);

		foreach(qpPlayerLink pl, qsPlayerLinks) {
			if (pl.second == l)
				qsPlayerLinks.remove(pl);
		}
		foreach(qpPlayerLink pl, l->qsPlayerLinks) {
			if (pl.second == l)
				qsPlayerLinks.remove(pl);
		}
	} else {
		foreach(Channel *c, qhLinks.keys())
			unlink(c);
	}
}

void Channel::playerLink(Channel *l, Player *p) {
	qpPlayerLink pl(p, l);
	if (qsPlayerLinks.contains(pl))
		return;

	qsPlayerLinks.insert(pl);
	l->qsPlayerLinks.insert(pl);
	qhLinks[l]++;
	l->qhLinks[this]++;
}

void Channel::playerUnlink(Channel *l, Player *p) {
	qpPlayerLink pl(p, l);
	if (! qsPlayerLinks.contains(pl))
		return;

	qsPlayerLinks.remove(pl);
	l->qsPlayerLinks.remove(qpPlayerLink(p, this));
	qhLinks[l]--;
	l->qhLinks[this]--;

	if (qhLinks[l] == 0)
		qhLinks.remove(l);
	if (l->qhLinks[this] == 0)
		l->qhLinks.remove(this);
}

QSet<Channel *> Channel::allLinks() {
	QSet<Channel *> seen;
	seen.insert(this);
	if (qhLinks.isEmpty())
		return seen;

	Channel *l, *lnk;
	QStack<Channel *> stack;
	stack.push(this);

	while (! stack.isEmpty()) {
		lnk = stack.pop();
		foreach(l, lnk->qhLinks.keys()) {
			if (! seen.contains(l)) {
				seen.insert(l);
				stack.push(l);
			}
		}
	}
	return seen;
}

void Channel::addChannel(Channel *c) {
	c->cParent = this;
	c->setParent(this);
	qlChannels << c;
}

void Channel::removeChannel(Channel *c) {
	c->cParent = NULL;
	c->setParent(NULL);
	qlChannels.removeAll(c);
}

void Channel::addPlayer(Player *p) {
	if (p->cChannel)
		p->cChannel->removePlayer(p);
	ClientPlayer *cp = dynamic_cast<ClientPlayer *>(p);
	if (cp)
		cp->setParent(this);
	p->cChannel = this;
	qlPlayers << p;
}

void Channel::removePlayer(Player *p) {
	foreach(qpPlayerLink pl, qsPlayerLinks) {
		if (pl.first == p) {
			qsPlayerLinks.remove(pl);
			pl.second->qsPlayerLinks.remove(qpPlayerLink(p, this));
		}
	}

	qlPlayers.removeAll(p);
}

Channel::operator const QString() const {
	return QString::fromLatin1("%1[%2:%3]").arg(qsName).arg(iId).arg(cParent ? cParent->iId : -1);
}
