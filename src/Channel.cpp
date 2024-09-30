// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Channel.h"
#include "ACL.h"
#include "Group.h"
#include "User.h"

#include <QtCore/QStack>

#ifdef MUMBLE
#	include <queue>
#	include "PluginManager.h"
#	include "Global.h"
#	include "Database.h"
#	include "ServerHandler.h"

QHash< unsigned int, Channel * > Channel::c_qhChannels;
QReadWriteLock Channel::c_qrwlChannels;
#endif

Channel::Channel(unsigned int id, const QString &name, QObject *p) : QObject(p) {
	iId         = id;
	iPosition   = 0;
	qsName      = name;
	bInheritACL = true;
	uiMaxUsers  = 0;
	bTemporary  = false;
	cParent     = qobject_cast< Channel * >(p);
	if (cParent)
		cParent->addChannel(this);
#ifdef MUMBLE
	uiPermissions = 0;
	m_filterMode  = ChannelFilterMode::NORMAL;

	hasEnterRestrictions.store(false);
	localUserCanEnter.store(true);
#endif // MUMBLE
}

Channel::~Channel() {
	if (cParent)
		cParent->removeChannel(this);

	foreach (Channel *c, qlChannels)
		delete c;

	foreach (ChanACL *acl, qlACL)
		delete acl;
	foreach (Group *g, qhGroups)
		delete g;
	foreach (Channel *l, qhLinks.keys())
		unlink(l);

	Q_ASSERT(qlChannels.count() == 0);
	Q_ASSERT(children().count() == 0);
}

#ifdef MUMBLE
Channel *Channel::get(unsigned int id) {
	QReadLocker lock(&c_qrwlChannels);
	return c_qhChannels.value(id);
}

Channel *Channel::add(unsigned int id, const QString &name) {
	QWriteLocker lock(&c_qrwlChannels);

	if (c_qhChannels.contains(id))
		return nullptr;

	Channel *c = new Channel(id, name, nullptr);
	c_qhChannels.insert(id, c);

	// We have to use a direct connection here in order to make sure that the user object that gets passed to the
	// callback does not get invalidated or deleted while the callback is running.
	QObject::connect(c, &Channel::channelEntered, Global::get().pluginManager, &PluginManager::on_channelEntered,
					 Qt::DirectConnection);
	QObject::connect(c, &Channel::channelExited, Global::get().pluginManager, &PluginManager::on_channelExited,
					 Qt::DirectConnection);

	return c;
}

void Channel::remove(Channel *c) {
	QWriteLocker lock(&c_qrwlChannels);
	c_qhChannels.remove(c->iId);
}

void Channel::setFilterMode(ChannelFilterMode filterMode) {
	m_filterMode        = filterMode;
	ServerHandlerPtr sh = Global::get().sh;
	if (sh) {
		Global::get().db->setChannelFilterMode(sh->qbaDigest, iId, m_filterMode);
	}
}

void Channel::clearFilterMode() {
	if (m_filterMode != ChannelFilterMode::NORMAL) {
		setFilterMode(ChannelFilterMode::NORMAL);
	}
}

bool Channel::isFiltered() const {
	if (!Global::get().s.bFilterActive) {
		// Channel filtering is disabled
		return false;
	}

	const Channel *userChannel = nullptr;

	if (Global::get().uiSession != 0) {
		const ClientUser *user = ClientUser::get(Global::get().uiSession);
		if (user) {
			userChannel = user->cChannel;
		}
	}

	bool hasUser = false;
	bool hasHide = false;
	bool hasPin  = false;

	// Iterate tree down
	std::queue< const Channel * > channelSearch;
	channelSearch.push(this);

	while (!channelSearch.empty()) {
		const Channel *c = channelSearch.front();
		channelSearch.pop();

		// Never hide channel, if user resides in this channel or a subchannel
		if (userChannel && c == userChannel) {
			return false;
		}

		if (c->m_filterMode == ChannelFilterMode::PIN) {
			hasPin = true;
		}

		if (!c->qlUsers.isEmpty()) {
			hasUser = true;
		}

		for (const Channel *currentSubChannel : c->qlChannels) {
			channelSearch.push(currentSubChannel);
		}
	}

	// Iterate tree up
	const Channel *c = this;
	while (c) {
		if (c->m_filterMode == ChannelFilterMode::HIDE) {
			hasHide = true;
			break;
		}
		c = c->cParent;
	}

	if (hasPin) {
		return false;
	}

	if (hasHide) {
		return true;
	}

	return Global::get().s.bFilterHidesEmptyChannels && !hasUser;
}

#endif // MUMBLE

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
		foreach (Channel *c, qhLinks.keys())
			unlink(c);
	}
}

QSet< Channel * > Channel::allLinks() {
	QSet< Channel * > seen;
	seen.insert(this);
	if (qhLinks.isEmpty())
		return seen;

	QStack< Channel * > stack;
	stack.push(this);

	while (!stack.isEmpty()) {
		Channel *lnk = stack.pop();
		foreach (Channel *l, lnk->qhLinks.keys()) {
			if (!seen.contains(l)) {
				seen.insert(l);
				stack.push(l);
			}
		}
	}
	return seen;
}

QSet< Channel * > Channel::allChildren() {
	QSet< Channel * > seen;
	if (!qlChannels.isEmpty()) {
		QStack< Channel * > stack;
		stack.push(this);

		while (!stack.isEmpty()) {
			Channel *c = stack.pop();
			foreach (Channel *chld, c->qlChannels) {
				seen.insert(chld);
				if (!chld->qlChannels.isEmpty())
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
	c->cParent = nullptr;
	c->setParent(nullptr);
	qlChannels.removeAll(c);
}

void Channel::addUser(User *p) {
	Channel *prevChannel = p->cChannel;

	if (prevChannel)
		prevChannel->removeUser(p);
	p->cChannel = this;
	qlUsers << p;

	emit channelEntered(this, prevChannel, p);
}

void Channel::removeUser(User *p) {
	qlUsers.removeAll(p);

	emit channelExited(this, p);
}

Channel::operator QString() const {
	return QString::fromLatin1("%1[%2:%3%4]")
		.arg(qsName, QString::number(iId), QString::number(cParent ? static_cast< int >(cParent->iId) : -1),
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
	if (qlChannels.empty()) {
		return 0;
	}

	size_t result = 0;
	foreach (Channel *child, qlChannels) { result = qMax(result, child->getDepth() + 1); }

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
