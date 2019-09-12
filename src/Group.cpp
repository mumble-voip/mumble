// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Group.h"

#include "Channel.h"
#include "User.h"
#ifdef MURMUR
#include "ServerUser.h"

#include <QtCore/QStack>
#endif

Group::Group(Channel *assoc, const QString &name) {
	c = assoc;
	bInherit = true;
	bInheritable = true;
	qsName = name;
	if (c)
		c->qhGroups[name] = this;
}

#ifdef MURMUR

QSet<int> Group::members() {
	QStack<Group *> s;
	QSet<int> m;
	Channel *p;
	Group *g;
	int i;

	p = c;
	while (p) {
		g = p->qhGroups.value(qsName);
		if (g) {
			if ((p != c) && ! g->bInheritable)
				break;
			s.push(g);
			if (! g->bInherit)
				break;
		}
		p = p->cParent;
	}

	while (! s.isEmpty()) {
		g = s.pop();
		foreach(i, g->qsAdd)
			m.insert(i);
		foreach(i, g->qsRemove)
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
				return NULL;
		}
		p = p->cParent;
	}
	return NULL;
}

QSet<QString> Group::groupNames(Channel *chan) {
	QStack<Channel *> s;
	QSet<QString> m;
	Channel *c = chan;
	Group *g;

	while (c) {
		s.push(c);
		c = c->cParent;
	}

	while (! s.isEmpty()) {
		c = s.pop();
		foreach(g, c->qhGroups) {
			if ((chan != c) && (! g->bInheritable))
				m.remove(g->qsName);
			else
				m.insert(g->qsName);
		}
	}

	return m;
}

#define RET_FALSE (invert ? true : false)
#define RET_TRUE (invert ? false : true)

bool Group::isMember(Channel *curChan, Channel *aclChan, QString name, ServerUser *pl) {
	Channel *p;
	Channel *c;
	Group *g;

	bool m = false;
	bool invert = false;
	bool token = false;
	bool hash = false;
	c = curChan;

	while (true) {
		if (name.isEmpty())
			return false;

		if (name.startsWith(QChar::fromLatin1('!'))) {
			invert = true;
			name = name.remove(0,1);
			continue;
		}

		if (name.startsWith(QChar::fromLatin1('~'))) {
			c = aclChan;
			name = name.remove(0,1);
			continue;
		}

		if (name.startsWith(QChar::fromLatin1('#'))) {
			token = true;
			name = name.remove(0,1);
			continue;
		}
		if (name.startsWith(QChar::fromLatin1('$'))) {
			hash = true;
			name = name.remove(0,1);
			continue;
		}

		break;
	}

	if (token)
		m = pl->qslAccessTokens.contains(name, Qt::CaseInsensitive);
	else if (hash)
		m = pl->qsHash == name;
	else if (name == QLatin1String("none"))
		m = false;
	else if (name == QLatin1String("all"))
		m = true;
	else if (name == QLatin1String("auth"))
		m = (pl->iId >= 0);
	else if (name == QLatin1String("strong"))
		m = pl->bVerified;
	else if (name == QLatin1String("in"))
		m = (pl->cChannel == c);
	else if (name == QLatin1String("out"))
		m = !(pl->cChannel == c);
	else if (name == QLatin1String("sub")
			|| name.startsWith(QLatin1String("sub,"))) {
		
		name = name.remove(0,4);
		int mindesc = 1;
		int maxdesc = 1000;
		int minpath = 0;
		QStringList args = name.split(QLatin1String(","));
		if (args.count() >= 3) {
			maxdesc = args[2].isEmpty() ? maxdesc : args[2].toInt();
		}
		if (args.count() >= 2) {
			mindesc = args[1].isEmpty() ? mindesc : args[1].toInt();
		}
		if (args.count() >= 1) {
			minpath = args[0].isEmpty() ? minpath : args[0].toInt();
		}

		Channel *home = pl->cChannel;
		QList<Channel *> playerChain;
		QList<Channel *> groupChain;

		p = home;
		while (p) {
			playerChain.prepend(p);
			p = p->cParent;
		}

		p = curChan;
		while (p) {
			groupChain.prepend(p);
			p = p->cParent;
		}

		int cofs = groupChain.indexOf(c);
		Q_ASSERT(cofs != -1);

		cofs += minpath;

		if (cofs >= groupChain.count()) {
			return RET_FALSE;
		} else if (cofs < 0) {
			cofs = 0;
		}

		Channel *needed = groupChain[cofs];
		if (playerChain.indexOf(needed) == -1) {
			return RET_FALSE;
		}

		int mindepth = cofs + mindesc;
		int maxdepth = cofs + maxdesc;

		int pdepth = playerChain.count() - 1;

		m = (pdepth >= mindepth) && (pdepth <= maxdepth);
	} else {
		QStack<Group *> s;

		p = c;

		while (p) {
			g = p->qhGroups.value(name);

			if (g) {
				if ((p != c) && ! g->bInheritable)
					break;
				s.push(g);
				if (! g->bInherit)
					break;
			}

			p = p->cParent;
		}

		while (! s.isEmpty()) {
			g = s.pop();
			if (g->qsAdd.contains(pl->iId) || g->qsTemporary.contains(pl->iId) || g->qsTemporary.contains(- static_cast<int>(pl->uiSession)))
				m = true;
			if (g->qsRemove.contains(pl->iId))
				m = false;
		}
	}
	return invert ? !m : m;
}

#endif
