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

#include <QStack>
#include <QStringList>

#include "Group.h"
#include "Channel.h"
#include "Player.h"

Group::Group(Channel *assoc, QString name) {
	c = assoc;
	bInherit = true;
	bInheritable = true;
	qsName = name;
	c->qhGroups[name] = this;
}

QSet<int> Group::members() {
	QStack<Group *> s;
	QSet<int> m;
	Channel *p;
	Group *g;
	int i;

	p = c;
	while (p) {
		g = p->qhGroups.value(qsName);
		p = p->cParent;

		if (g) {
			if ((p != c) && ! g->bInheritable)
				break;
			s.push(g);
			if (! g->bInherit)
				break;
		}
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
		Group *g = chan->qhGroups.value(name);
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

bool Group::isMember(Channel *c, QString name, Player *pl) {
	Channel *p;
	Group *g;

	bool m = false;

	if (name.isEmpty())
		return false;

	if (name == "none")
		return false;

	if (name == "all")
		return true;

	if (name == "auth")
		return (pl->iId >= 0);

	if (name == "in") {
		if (pl->cChannel == c)
			return true;
		return false;
	}

	if (name == "out") {
		if (pl->cChannel == c)
			return false;
		return true;
	}

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
		if (g->qsAdd.contains(pl->iId))
			m = true;
		if (g->qsRemove.contains(pl->iId))
			m = false;
	}

	return m;
}
