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

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QStringList>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>

#include "ServerDB.h"
#include "Channel.h"

ServerDB::ServerDB() {
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	QSettings qs;
	QStringList datapaths;
	int i;

	datapaths << qs.value("DBPath").toString();
	datapaths << QCoreApplication::instance()->applicationDirPath();
	datapaths << QDir::currentPath();
	datapaths << QDir::homePath();
	bool found = false;

	for(i = 0; (i < datapaths.size()) && ! found; i++) {
		if (!datapaths[i].isEmpty()) {
			QFile f(datapaths[i] + "/murmur.sqlite");
			if (f.exists()) {
				db.setDatabaseName(f.fileName());
				found = db.open();
			}
		}
	}

	if (! found) {
		for(i = 0; (i < datapaths.size()) && ! found; i++) {
			if (!datapaths[i].isEmpty()) {
				QFile f(datapaths[i] + "/murmur.sqlite");
				db.setDatabaseName(f.fileName());
				found = db.open();
			}
		}
	}

	if (! found) {
		qFatal("ServerDB: Failed initialization");
	} else {
		qWarning("ServerDB: Opened successfully");
	}

	QSqlQuery query;
	query.exec("CREATE TABLE players (player_id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, email TEXT, pw TEXT)");
	query.exec("ALTER TABLE players ADD COLUMN lastchannel INTEGER");
	query.exec("CREATE UNIQUE INDEX players_name ON players (name)");
	query.exec("CREATE TABLE player_auth (player_auth_id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, pw TEXT, email TEXT, authcode TEXT)");
	query.exec("CREATE UNIQUE INDEX player_auth_name ON player_auth(name)");
	query.exec("CREATE UNIQUE INDEX player_auth_code ON player_auth(authcode)");
	query.exec("CREATE TABLE channels (channel_id INTEGER PRIMARY KEY AUTOINCREMENT, parent_id INTEGER, name TEXT)");
	query.exec("CREATE TRIGGER channels_parent_del AFTER DELETE ON channel FOR EACH ROW BEGIN DELETE FROM channel WHERE parent_id = old.channel_id; UPDATE players SET lastchannel=0 WHERE lastchannel = old.channel_id; END;");
	query.exec("INSERT INTO channels (channel_id, parent_id, name) VALUES (0, -1, 'Root')");
}

bool ServerDB::hasUsers() {
	QSqlQuery query;
	query.prepare("SELECT count(*) FROM players");
	query.exec();
	if (query.next()) {
		if (query.value(0).toInt() > 0)
			return true;
	}
	return false;
}

int ServerDB::authenticate(QString &name, QString pw) {
	int res = -2;
	QSqlQuery query;
	query.prepare("SELECT player_id,name,pw FROM players WHERE name like ?");
	query.addBindValue(name);
	query.exec();
	if (query.next()) {
		res = -1;
		QString storedpw = query.value(2).toString();
		if (storedpw == pw) {
			name = query.value(1).toString();
			res = query.value(0).toInt();
		}
	}
	return res;
}

Channel *ServerDB::addChannel(Channel *parent, QString name) {
	QSqlQuery query;
	query.prepare("INSERT INTO channels (parent_id, name) VALUES (?,?)");
	query.addBindValue(parent->iId);
	query.addBindValue(name);
	query.exec();
	int id = query.lastInsertId().toInt();
	return Channel::add(id, name, parent);
}

void ServerDB::removeChannel(Channel *c) {
	QSqlQuery query;
	query.prepare("DELETE FROM channels WHERE channel_id = ?");
	query.addBindValue(c->iId);
	query.exec();
}

void ServerDB::updateChannel(Channel *c) {
	QSqlQuery query;
	query.prepare("UPDATE channels SET parent_id = ? WHERE channel_id = ?");
	query.addBindValue(c->iParent);
	query.addBindValue(c->iId);
	query.exec();
}

void ServerDB::readChannels(Channel *p) {
	QList<Channel *> kids;
	Channel *c;
	QSqlQuery query;
	int parentid = -1;

	if (p)
		parentid = p->iId;

	query.prepare("SELECT channel_id, name FROM channels WHERE parent_id=? ORDER BY name");
	query.addBindValue(parentid);
	query.exec();
	while (query.next()) {
		c = Channel::add(query.value(0).toInt(), query.value(1).toString(), p);
		kids << c;
	}

	foreach(c, kids)
		readChannels(c);
}

void ServerDB::setLastChannel(Player *p) {
	if (p->iId < 0)
		return;

	QSqlQuery query;

	query.prepare("UPDATE players SET lastchannel=? WHERE player_id = ?");
	query.addBindValue(p->cChannel->iId);
	query.addBindValue(p->iId);
	query.exec();
}

int ServerDB::readLastChannel(Player *p) {
	Channel *c = Channel::get(0);

	if (p->iId >= 0) {
		QSqlQuery query;

		query.prepare("SELECT lastchannel FROM players WHERE player_id = ?");
		query.addBindValue(p->iId);
		query.exec();

		if (query.next()) {
			int id = query.value(0).toInt();
			qWarning("Restored player %d to %d", p->iId, id);
			Channel *chan = Channel::get(id);
			if (chan)
				c = chan;
		}
	}
	c->addPlayer(p);
	return c->iId;
}
