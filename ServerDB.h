/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _DATABASE_H
#define _DATABASE_H

class Channel;
class Player;
class Connection;

class ServerDB : public QObject {
	Q_OBJECT
	public:
		typedef QPair<QString, QList<QVariant> > qpCommand;
		typedef QPair<quint32, int> qpBan;
		ServerDB();
		static int authenticate(QString &name, QString pw);
		static bool hasUsers();
		static Channel *addChannel(Channel *c, QString name);
		static void removeChannel(Channel *c);
		static void readChannels(Channel *p = NULL);
		static void updateChannel(Channel *c);
		static void readChannelPrivs(Channel *c);
		static void setLastChannel(Player *p);
		static int readLastChannel(Player *p);
		static void dumpChannel(Channel *c);
		static int getUserID(QString name);
		static QString getUserName(int id);
		static void setPW(int id, QString pw);
		static void conChangedName(Player *p);
		static void conChangedChannel(Player *p);
		static void conLoggedOn(Player *p, Connection *con);
		static void conLoggedOff(Player *p);
		static void addLink(Channel *c, Channel *l);
		static void removeLink(Channel *c, Channel *l);
		static QList<qpCommand> getCommands();
		static QList<qpBan> getBans();
		static void setBans(QList<qpBan> bans);
};

#endif
