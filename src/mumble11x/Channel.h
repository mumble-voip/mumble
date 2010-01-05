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

#ifndef _CHANNEL_H
#define _CHANNEL_H

#include "murmur_pch.h"

class Player;
class Group;
class ChanACL;

class Channel : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(Channel)
	private:
		QSet<Channel *> qsUnseen;
	public:
		int iId;
		Channel *cParent;
		QString qsName;
		QString qsDesc;
		QList<Channel *> qlChannels;
		QList<Player *> qlPlayers;
		QHash<QString, Group *> qhGroups;
		QList<ChanACL *> qlACL;

		QSet<Channel *> qsPermLinks;
		QHash<Channel *, int> qhLinks;

		typedef QPair<Player *, Channel *> qpPlayerLink;
		QSet<qpPlayerLink> qsPlayerLinks;

		bool bInheritACL;

		static QHash<int, Channel *> c_qhChannels;
		static QReadWriteLock c_qrwlChannels;

		Channel(int id, const QString &name, const QString &desc, QObject *p = NULL);
		~Channel();

		static Channel *get(int);
		static Channel *add(int, const QString &, const QString & = QString());
		static void remove(Channel *);

		void addChannel(Channel *c);
		void removeChannel(Channel *c);
		void addPlayer(Player *p);
		void removePlayer(Player *p);

		bool isLinked(Channel *c);
		void link(Channel *c);
		void unlink(Channel *c);

		void playerLink(Channel *c, Player *p);
		void playerUnlink(Channel *c, Player *p);

		QSet<Channel *> allLinks();

		operator const QString() const;
};

#else
class Channel;
#endif
