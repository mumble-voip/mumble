// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Group.h"

#include "Channel.h"
#include "User.h"
#ifdef MURMUR
#	include "ServerUser.h"

#	include <QtCore/QStack>
#endif

const Qt::CaseSensitivity Group::accessTokenCaseSensitivity = Qt::CaseInsensitive;

Group::Group(Channel *assoc, const QString &name) {
	c            = assoc;
	bInherit     = true;
	bInheritable = true;
	qsName       = name;
	if (c)
		c->qhGroups[name] = this;
}

#ifdef MURMUR

QSet< int > Group::members() {
	QStack< Group * > s;
	QSet< int > m;
	Channel *p;
	Group *g;
	int i;

	p = c;
	while (p) {
		g = p->qhGroups.value(qsName);
		if (g) {
			if ((p != c) && !g->bInheritable)
				break;
			s.push(g);
			if (!g->bInherit)
				break;
		}
		p = p->cParent;
	}

	while (!s.isEmpty()) {
		g = s.pop();
		foreach (i, g->qsAdd)
			m.insert(i);
		foreach (i, g->qsRemove)
			m.remove(i);
	}

	return m;
}

Group *Group::getGroup(Channel *chan, QString name) {
	Channel *p = chan;
	while (p) {
		Group *g = p->qhGroups.value(name);
		if (g) {
			if (chan == p)
				return g;
			else if (g->bInheritable)
				return g;
			else
				return nullptr;
		}
		p = p->cParent;
	}
	return nullptr;
}

QSet< QString > Group::groupNames(Channel *chan) {
	QStack< Channel * > s;
	QSet< QString > m;
	Channel *c = chan;
	Group *g;

	while (c) {
		s.push(c);
		c = c->cParent;
	}

	while (!s.isEmpty()) {
		c = s.pop();
		foreach (g, c->qhGroups) {
			if ((chan != c) && (!g->bInheritable))
				m.remove(g->qsName);
			else
				m.insert(g->qsName);
		}
	}

	return m;
}

#	define RET_FALSE (invert ? true : false)
#	define RET_TRUE (invert ? false : true)

bool Group::appliesToUser(const Channel &currentChannel, const Channel &aclChannel, QString groupSpecification,
						  const ServerUser &user) {
	bool matches                  = false;
	bool invert                   = false;
	bool isAccessToken            = false;
	bool isCertHash               = false;
	const Channel *contextChannel = &currentChannel;

	while (!groupSpecification.isEmpty()) {
		if (groupSpecification.startsWith(QChar::fromLatin1('!'))) {
			invert             = true;
			groupSpecification = groupSpecification.remove(0, 1);
			continue;
		}

		if (groupSpecification.startsWith(QChar::fromLatin1('~'))) {
			contextChannel     = &aclChannel;
			groupSpecification = groupSpecification.remove(0, 1);
			continue;
		}

		if (groupSpecification.startsWith(QChar::fromLatin1('#'))) {
			isAccessToken      = true;
			groupSpecification = groupSpecification.remove(0, 1);
			continue;
		}
		if (groupSpecification.startsWith(QChar::fromLatin1('$'))) {
			isCertHash         = true;
			groupSpecification = groupSpecification.remove(0, 1);
			continue;
		}

		break;
	}

	if (groupSpecification.isEmpty()) {
		return false;
	}


	// First, all special cases that aren't even groups and meta groups (groups that don't actually exist as groups but
	// have a special meaning based on their name
	if (isAccessToken)
		matches = user.qslAccessTokens.contains(groupSpecification, Group::accessTokenCaseSensitivity);
	else if (isCertHash)
		matches = user.qsHash == groupSpecification;
	else if (groupSpecification == QLatin1String("none"))
		matches = false;
	else if (groupSpecification == QLatin1String("all"))
		matches = true;
	else if (groupSpecification == QLatin1String("auth"))
		matches = (user.iId >= 0);
	else if (groupSpecification == QLatin1String("strong"))
		matches = user.bVerified;
	else if (groupSpecification == QLatin1String("in"))
		matches = (user.cChannel == contextChannel);
	else if (groupSpecification == QLatin1String("out"))
		matches = !(user.cChannel == contextChannel);
	else if (groupSpecification == QLatin1String("sub") || groupSpecification.startsWith(QLatin1String("sub,"))) {
		groupSpecification = groupSpecification.remove(0, 4);

		int requiredChannelOffset = 0;
		int minDescendantLevel    = 1;
		int maxDescendantLevel    = 1000;

		// Parse arguments, if any
		QStringList args = groupSpecification.split(QLatin1String(","));
		if (args.count() >= 1 && !args[0].isEmpty()) {
			requiredChannelOffset = args[0].toInt();
		}
		if (args.count() >= 2 && !args[1].isEmpty()) {
			minDescendantLevel = args[1].toInt();
		}
		if (args.count() >= 3 && !args[2].isEmpty()) {
			maxDescendantLevel = args[2].toInt();
		}

		// Assemble channel hierarchy from root channel to the channel the player is currently in
		QList< const Channel * > homeChannelHierarchy;
		const Channel *channel = user.cChannel;
		while (channel) {
			homeChannelHierarchy.prepend(channel);
			channel = channel->cParent;
		}

		// Assemble channel hierarchy from root channel to the channel the ACL containing this specification is
		// evaluated for
		QList< const Channel * > currentChannelHierarchy;
		channel = &currentChannel;
		while (channel) {
			currentChannelHierarchy.prepend(channel);
			channel = channel->cParent;
		}

		auto requiredChannelIndex = currentChannelHierarchy.indexOf(contextChannel);
		Q_ASSERT(requiredChannelIndex != -1);

		requiredChannelIndex += requiredChannelOffset;

		if (requiredChannelIndex >= currentChannelHierarchy.count()) {
			return RET_FALSE;
		} else if (requiredChannelIndex < 0) {
			requiredChannelIndex = 0;
		}

		const Channel *requiredChannel = currentChannelHierarchy[requiredChannelIndex];
		if (homeChannelHierarchy.indexOf(requiredChannel) == -1) {
			return RET_FALSE;
		}

		const auto minDepth = requiredChannelIndex + minDescendantLevel;
		const auto maxDepth = requiredChannelIndex + maxDescendantLevel;

		const auto totalDepth = homeChannelHierarchy.count() - 1;

		matches = (totalDepth >= minDepth) && (totalDepth <= maxDepth);
	} else {
		// The group specification is an actual group name
		QStack< const Group * > groupStack;

		const Channel *channel = contextChannel;

		while (channel) {
			const Group *group = channel->qhGroups.value(groupSpecification);

			if (group) {
				if ((channel != contextChannel) && !group->bInheritable)
					break;
				groupStack.push(group);
				if (!group->bInherit)
					break;
			}

			channel = channel->cParent;
		}

		while (!groupStack.isEmpty()) {
			const Group *group = groupStack.pop();
			if (group->qsAdd.contains(user.iId) || group->qsTemporary.contains(user.iId)
				|| group->qsTemporary.contains(-static_cast< int >(user.uiSession)))
				matches = true;
			if (group->qsRemove.contains(user.iId))
				matches = false;
		}
	}
	return invert ? !matches : matches;
}

#endif
