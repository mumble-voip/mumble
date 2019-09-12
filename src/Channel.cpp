// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Channel.h"
#include "User.h"
#include "Group.h"
#include "ACL.h"

#include <QtCore/QStack>

#ifdef MUMBLE
QHash<int, Channel *> Channel::c_qhChannels;
QReadWriteLock Channel::c_qrwlChannels;
#endif

Channel::Channel(int id, const QString &name, QObject *p) : QObject(p) {
	iId = id;
	iPosition = 0;
	qsName = name;
	bInheritACL = true;
	uiMaxUsers = 0;
	bTemporary = false;
	cParent = qobject_cast<Channel *>(p);
	if (cParent)
		cParent->addChannel(this);
#ifdef MUMBLE
	uiPermissions = 0;
	bFiltered = false;
#endif
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

#ifdef MUMBLE
Channel *Channel::get(int id) {
	QReadLocker lock(&c_qrwlChannels);
	return c_qhChannels.value(id);
}

Channel *Channel::add(int id, const QString &name) {
	QWriteLocker lock(&c_qrwlChannels);

	if (c_qhChannels.contains(id))
		return NULL;

	Channel *c = new Channel(id, name, NULL);
	c_qhChannels.insert(id, c);
	return c;
}

void Channel::remove(Channel *c) {
	QWriteLocker lock(&c_qrwlChannels);
	c_qhChannels.remove(c->iId);
}
#endif

bool Channel::lessThan(const Channel *first, const Channel *second) {
	if ((first->iPosition != second->iPosition) && (first->cParent == second->cParent))
		return first->iPosition < second->iPosition;
	else
		return QString::localeAwareCompare(first->qsName, second->qsName) < 0;
}

bool Channel::isLinked(Channel *l) const {
	return ((l == this) || qhLinks.contains(l));
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
	} else {
		foreach(Channel *c, qhLinks.keys())
			unlink(c);
	}
}

QSet<Channel *> Channel::allLinks() {
	QSet<Channel *> seen;
	seen.insert(this);
	if (qhLinks.isEmpty())
		return seen;

	QStack<Channel *> stack;
	stack.push(this);

	while (! stack.isEmpty()) {
		Channel *lnk = stack.pop();
		foreach(Channel *l, lnk->qhLinks.keys()) {
			if (! seen.contains(l)) {
				seen.insert(l);
				stack.push(l);
			}
		}
	}
	return seen;
}

QSet<Channel *> Channel::allChildren() {
	QSet<Channel *> seen;
	if (! qlChannels.isEmpty()) {
		QStack<Channel *> stack;
		stack.push(this);

		while (! stack.isEmpty()) {
			Channel *c = stack.pop();
			foreach(Channel *chld, c->qlChannels) {
				seen.insert(chld);
				if (! chld->qlChannels.isEmpty())
					stack.append(chld);
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

void Channel::addUser(User *p) {
	if (p->cChannel)
		p->cChannel->removeUser(p);
	p->cChannel = this;
	qlUsers << p;
}

void Channel::removeUser(User *p) {
	qlUsers.removeAll(p);
}

Channel::operator QString() const {
	return QString::fromLatin1("%1[%2:%3%4]").arg(qsName,
	        QString::number(iId),
	        QString::number(cParent ? cParent->iId : -1),
	        bTemporary ? QLatin1String("*") : QLatin1String(""));
}

size_t Channel::getLevel() const {
	size_t i = 0;

	const Channel *c = this;
	while (c->cParent) {
		c = c->cParent;
		++i;
	}

	return i;
}

size_t Channel::getDepth() const {
	if(qlChannels.empty()) {
		return 0;
	}

	size_t result = 0;
	foreach(Channel *child, qlChannels) {
		result = qMax(result, child->getDepth() + 1);
	}

	return result;
}

QString Channel::getPath() const {
	QString out;

	const Channel *tmp = this;
	while (tmp->cParent) {
		// Skip the root channel.
		if (tmp->iId == 0) {
			break;
		}

		out.prepend(QString::fromLatin1("/"));
		out.prepend(tmp->qsName);

		tmp = tmp->cParent;
	}

	return out;
}
