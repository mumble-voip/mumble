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

#include "ServerDB.h"
#include "Player.h"
#include "Channel.h"
#include "Group.h"
#include "ACL.h"
#include "Server.h"
#include "Connection.h"

#define SQLDUMP(x) qWarning("%s", qPrintable(x.lastError().text())

class TransactionHolder {
	protected:
		bool bAbort;
	public:
		TransactionHolder() {
			QSqlDatabase::database().transaction();
			bAbort = false;
		}

		void abort() {
			bAbort = true;
		}

		~TransactionHolder() {
			if (bAbort) {
				QSqlDatabase::database().rollback();
			} else {
				QSqlDatabase::database().commit();
			}
		}
};

ServerDB::ServerDB() {
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	QSettings qs;
	QStringList datapaths;
	int i;

	bool found = false;

	if (! g_sp.qsDatabase.isEmpty()) {
		db.setDatabaseName(g_sp.qsDatabase);
		found = db.open();
	} else {

		datapaths << qs.value("DBPath").toString();
		datapaths << QCoreApplication::instance()->applicationDirPath();
		datapaths << QDir::currentPath();
		datapaths << QDir::homePath();

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

	query.exec("CREATE TABLE channels (channel_id INTEGER PRIMARY KEY AUTOINCREMENT, parent_id INTEGER, name TEXT, inheritACL INTEGER)");
	query.exec("CREATE TRIGGER channels_parent_del AFTER DELETE ON channels FOR EACH ROW BEGIN DELETE FROM channels WHERE parent_id = old.channel_id; UPDATE players SET lastchannel=0 WHERE lastchannel = old.channel_id; END;");

	query.exec("CREATE TABLE groups (group_id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, channel_id INTEGER, inherit INTEGER, inheritable INTEGER)");
	query.exec("CREATE UNIQUE INDEX groups_name_channels ON groups(name, channel_id)");
	query.exec("CREATE TRIGGER groups_del_channel AFTER DELETE ON channels FOR EACH ROW BEGIN DELETE FROM groups WHERE channel_id = old.channel_id; END;");

	query.exec("CREATE TABLE group_members (group_members_id INTEGER PRIMARY KEY AUTOINCREMENT, group_id INTEGER, player_id INTEGER, addit INTEGER)");
	query.exec("CREATE TRIGGER groups_members_del_group AFTER DELETE ON groups FOR EACH ROW BEGIN DELETE FROM group_members WHERE group_id = old.group_id; END;");
	query.exec("CREATE TRIGGER groups_members_del_player AFTER DELETE ON players FOR EACH ROW BEGIN DELETE FROM group_members WHERE player_id = old.player_id; END;");

	query.exec("CREATE TABLE acl (acl_id INTEGER PRIMARY KEY AUTOINCREMENT, channel_id INTEGER, priority INTEGER, player_id INTEGER, group_name TEXT, apply_here INTEGER, apply_sub INTEGER, grant INTEGER, revoke INTEGER)");
	query.exec("CREATE UNIQUE INDEX acl_channel_pri ON acl(channel_id, priority)");
	query.exec("CREATE TRIGGER acl_del_channel AFTER DELETE ON channels FOR EACH ROW BEGIN DELETE FROM acl WHERE channel_id = old.channel_id; END;");
	query.exec("CREATE TRIGGER acl_del_player AFTER DELETE ON players FOR EACH ROW BEGIN DELETE FROM acl WHERE player_id = old.player_id; END;");

	query.exec("CREATE TABLE connections (con_id INTEGER PRIMARY KEY, player_id INTEGER, channel_id INTEGER, player_name TEXT, ip TEXT, port INTEGER)");
	query.exec("CREATE UNIQUE INDEX connections_player_name ON connections(player_name)");
	query.exec("DELETE FROM connections");

	query.exec("CREATE TABLE channel_links (channel_links_id INTEGER PRIMARY KEY AUTOINCREMENT, channel_id INTEGER, link_id INTEGER)");
	query.exec("CREATE TRIGGER channel_links_del_channel AFTER DELETE ON channels FOR EACH ROW BEGIN DELETE FROM channel_links WHERE channel_id = old.channel_id OR link_id = old.channel_id; END;");
	query.exec("DELETE FROM channel_links");

	query.exec("CREATE TABLE commands (command_id INTEGER PRIMARY KEY AUTOINCREMENT, command TEXT, arg1 TEXT, arg2 TEXT, arg3 TEXT, arg4 TEXT, arg5 TEXT, arg6 TEXT, arg7 TEXT, arg8 TEXT, arg9 TEXT)");
	query.exec("DELETE FROM commands");

	query.exec("CREATE TABLE bans (ban_id INTEGER PRIMARY KEY AUTOINCREMENT, base INTEGER, mask INTEGER)");

	query.exec("INSERT INTO channels (channel_id, parent_id, name) VALUES (0, -1, 'Root')");
	query.exec("INSERT INTO players (player_id, name, email, pw) VALUES (0, 'SuperUser', '', '')");

	query.exec("SELECT COUNT(*) FROM acl");
	if (query.next()) {
		int c = query.value(0).toInt();
		if (c == 0) {
			query.exec("INSERT INTO acl (channel_id, priority, player_id, group_name, apply_here, apply_sub, grant, revoke) VALUES (0, 1, -1, 'auth', 1, 0, 64, 0)");
			query.exec("INSERT INTO acl (channel_id, priority, player_id, group_name, apply_here, apply_sub, grant, revoke) VALUES (0, 2, -1, 'admin', 1, 1, 1, 0)");
		}
	}

	query.exec("SELECT COUNT(*) FROM groups");
	if (query.next()) {
		int c = query.value(0).toInt();
		if (c == 0) {
			query.exec("INSERT INTO groups (group_id, name, channel_id, inherit, inheritable) VALUES (0, 'admin', 0, 1, 1)");
			query.exec("INSERT INTO group_members (group_id, player_id, addit) VALUES (0, 0, 1)");
		}
	}

	query.exec("VACUUM");
}

bool ServerDB::hasUsers() {
	TransactionHolder th;

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
	TransactionHolder th;

	int res = -2;
	QSqlQuery query;
	query.prepare("SELECT player_id,name,pw FROM players WHERE name like ?");
	query.addBindValue(name);
	query.exec();
	if (query.next()) {
		res = -1;
		QString storedpw = query.value(2).toString();
		if (! storedpw.isEmpty() && (storedpw == pw)) {
			name = query.value(1).toString();
			res = query.value(0).toInt();
		}
	}
	return res;
}

void ServerDB::setPW(int id, QString pw) {
	TransactionHolder th;

	QSqlQuery query;
	query.prepare("UPDATE players SET pw=? WHERE player_id = ?");
	query.addBindValue(pw);
	query.addBindValue(id);
	query.exec();
}

QString ServerDB::getUserName(int id) {
	TransactionHolder th;
	QString name;

	QSqlQuery query;
	query.prepare("SELECT name FROM players WHERE player_id = ?");
	query.addBindValue(id);
	query.exec();
	if (query.next()) {
		name = query.value(0).toString();
	}
	return name;
}

int ServerDB::getUserID(QString name) {
	TransactionHolder th;
	int id = -1;

	QSqlQuery query;
	query.prepare("SELECT player_id FROM players WHERE name like ?");
	query.addBindValue(name);
	query.exec();
	if (query.next()) {
		id = query.value(0).toInt();
	}
	return id;
}

void ServerDB::addLink(Channel *c, Channel *l) {
	TransactionHolder th;

	QSqlQuery query;
	query.prepare("INSERT INTO channel_links (channel_id, link_id) VALUES (?,?)");
	query.addBindValue(c->iId);
	query.addBindValue(l->iId);
	query.exec();

	query.prepare("INSERT INTO channel_links (channel_id, link_id) VALUES (?,?)");
	query.addBindValue(l->iId);
	query.addBindValue(c->iId);
	query.exec();
}

void ServerDB::removeLink(Channel *c, Channel *l) {
	TransactionHolder th;

	QSqlQuery query;

	if (l) {
		query.prepare("DELETE FROM channel_links WHERE channel_id = ? AND link_id = ?");
		query.addBindValue(c->iId);
		query.addBindValue(l->iId);
		query.exec();

		query.prepare("DELETE FROM channel_links WHERE channel_id = ? AND link_id = ?");
		query.addBindValue(l->iId);
		query.addBindValue(c->iId);
		query.exec();
	} else {
		query.prepare("DELETE FROM channel_links WHERE channel_id = ? OR link_id = ?");
		query.addBindValue(c->iId);
		query.addBindValue(c->iId);
		query.exec();
	}
}

Channel *ServerDB::addChannel(Channel *parent, QString name) {
	TransactionHolder th;

	QSqlQuery query;
	query.prepare("INSERT INTO channels (parent_id, name) VALUES (?,?)");
	query.addBindValue(parent->iId);
	query.addBindValue(name);
	query.exec();
	int id = query.lastInsertId().toInt();
	return Channel::add(id, name, parent);
}

void ServerDB::removeChannel(Channel *c) {
	TransactionHolder th;

	QSqlQuery query;
	query.prepare("DELETE FROM channels WHERE channel_id = ?");
	query.addBindValue(c->iId);
	query.exec();
}

void ServerDB::updateChannel(Channel *c) {
	TransactionHolder th;
	Group *g;
	ChanACL *acl;

	QSqlQuery query;
	query.prepare("UPDATE channels SET parent_id = ?, inheritACL = ? WHERE channel_id = ?");
	query.addBindValue(c->cParent ? c->cParent->iId : -1);
	query.addBindValue(c->bInheritACL ? 1 : 0);
	query.addBindValue(c->iId);
	query.exec();

	query.prepare("DELETE FROM groups WHERE channel_id = ?");
	query.addBindValue(c->iId);
	query.exec();

	query.prepare("DELETE FROM acl WHERE channel_id = ?");
	query.addBindValue(c->iId);
	query.exec();

	foreach(g, c->qhGroups) {
		query.prepare("INSERT INTO groups (name, channel_id, inherit, inheritable) VALUES (?,?,?,?)");
		query.addBindValue(g->qsName);
		query.addBindValue(g->c->iId);
		query.addBindValue(g->bInherit ? 1 : 0);
		query.addBindValue(g->bInheritable ? 1 : 0);
		query.exec();

		int id = query.lastInsertId().toInt();
		int pid;

		foreach(pid, g->qsAdd) {
			query.prepare("INSERT INTO group_members (group_id, player_id, addit) VALUES (?, ?, ?)");
			query.addBindValue(id);
			query.addBindValue(pid);
			query.addBindValue(1);
			query.exec();
		}
		foreach(pid, g->qsRemove) {
			query.prepare("INSERT INTO group_members (group_id, player_id, addit) VALUES (?, ?, ?)");
			query.addBindValue(id);
			query.addBindValue(pid);
			query.addBindValue(0);
			query.exec();
		}
	}

	int pri = 5;

	foreach(acl, c->qlACL) {
		query.prepare("INSERT INTO acl (channel_id, priority, player_id, group_name, apply_here, apply_sub, grant, revoke) VALUES (?,?,?,?,?,?,?,?)");
		query.addBindValue(acl->c->iId);
		query.addBindValue(pri++);
		query.addBindValue(acl->iPlayerId);
		query.addBindValue(acl->qsGroup);
		query.addBindValue(acl->bApplyHere ? 1 : 0);
		query.addBindValue(acl->bApplySubs ? 1 : 0);
		query.addBindValue(static_cast<int>(acl->pAllow));
		query.addBindValue(static_cast<int>(acl->pDeny));
		query.exec();
	}
}

void ServerDB::readChannelPrivs(Channel *c) {
	TransactionHolder th;

	int cid = c->iId;

	QSqlQuery query;
	query.prepare("SELECT group_id, name, inherit, inheritable FROM groups WHERE channel_id = ?");
	query.addBindValue(cid);
	query.exec();
	while (query.next()) {
		int gid = query.value(0).toInt();
		QString name = query.value(1).toString();
		Group *g = new Group(c, name);
		g->bInherit = query.value(2).toBool();
		g->bInheritable = query.value(3).toBool();

		QSqlQuery mem;
		mem.prepare("SELECT player_id, addit FROM group_members WHERE group_id = ?");
		mem.addBindValue(gid);
		mem.exec();
		while (mem.next()) {
			int uid = mem.value(0).toInt();
			if (mem.value(1).toBool())
				g->qsAdd << uid;
			else
				g->qsRemove << uid;
		}
	}

	query.prepare("SELECT player_id, group_name, apply_here, apply_sub, grant, revoke FROM acl WHERE channel_id = ? ORDER BY priority");
	query.addBindValue(cid);
	query.exec();
	while (query.next()) {
		ChanACL *acl = new ChanACL(c);
		acl->iPlayerId = query.value(0).toInt();
		acl->qsGroup = query.value(1).toString();
		acl->bApplyHere = query.value(2).toBool();
		acl->bApplySubs = query.value(3).toBool();
		acl->pAllow = static_cast<ChanACL::Permissions>(query.value(4).toInt());
		acl->pDeny = static_cast<ChanACL::Permissions>(query.value(5).toInt());
	}
}

void ServerDB::readChannels(Channel *p) {

	QList<Channel *> kids;
	Channel *c;
	QSqlQuery query;
	int parentid = -1;

	if (p) {
		parentid = p->iId;
		readChannelPrivs(Channel::get(parentid));
	}

	{
		TransactionHolder th;
		query.prepare("SELECT channel_id, name, inheritACL FROM channels WHERE parent_id=? ORDER BY name");
		query.addBindValue(parentid);
		query.exec();
		while (query.next()) {
			c = Channel::add(query.value(0).toInt(), query.value(1).toString(), p);
			c->bInheritACL = query.value(2).toBool();
			kids << c;
		}
	}

	foreach(c, kids)
		readChannels(c);
}

void ServerDB::setLastChannel(Player *p) {
	TransactionHolder th;

	if (p->iId < 0)
		return;

	QSqlQuery query;

	query.prepare("UPDATE players SET lastchannel=? WHERE player_id = ?");
	query.addBindValue(p->cChannel->iId);
	query.addBindValue(p->iId);
	query.exec();
	
	query.prepare("UPDATE connections SET channel_id=? WHERE con_id = ?");
	query.addBindValue(p->cChannel->iId);
	query.addBindValue(p->sId);
	query.exec();
}

int ServerDB::readLastChannel(Player *p) {

	Channel *c = Channel::get(0);

	if (p->iId >= 0) {
		TransactionHolder th;
		QSqlQuery query;

		query.prepare("SELECT lastchannel FROM players WHERE player_id = ?");
		query.addBindValue(p->iId);
		query.exec();

		if (query.next()) {
			int id = query.value(0).toInt();
			Channel *chan = Channel::get(id);
			if (chan)
				c = chan;
		}
	}
	return c->iId;
}

void ServerDB::conLoggedOn(Player *p, Connection *con) {
	TransactionHolder th;
	QSqlQuery query;

	query.prepare("INSERT INTO connections (con_id, player_id, channel_id, player_name, ip, port) VALUES (?,?,?,?,?,?)");
	query.addBindValue(p->sId);
	query.addBindValue(p->iId);
	query.addBindValue(p->cChannel->iId);
	query.addBindValue(p->qsName);
	query.addBindValue(con->peerAddress().toString());
	query.addBindValue(con->peerPort());
	query.exec();
}

void ServerDB::conLoggedOff(Player *p) {
	TransactionHolder th;
	QSqlQuery query;

	query.prepare("DELETE FROM connections WHERE con_id = ?");
	query.addBindValue(p->sId);
	query.exec();
}

void ServerDB::conChangedChannel(Player *p) {
	TransactionHolder th;
	QSqlQuery query;

	query.prepare("UPDATE connections SET player_name=? WHERE con_id = ?");
	query.addBindValue(p->qsName);
	query.addBindValue(p->sId);
	query.exec();
}

void ServerDB::conChangedName(Player *p) {
	TransactionHolder th;
	QSqlQuery query;

	query.prepare("UPDATE connections SET channel_id=? WHERE con_id = ?");
	query.addBindValue(p->cChannel->iId);
	query.addBindValue(p->sId);
	query.exec();
}

void ServerDB::dumpChannel(Channel *c) {
	Group *g;
	ChanACL *acl;
	int pid;

	if (c == NULL) {
		c = Channel::get(0);
	}

	qWarning("Channel %s (ACLInherit %d)", qPrintable(c->qsName), c->bInheritACL);
	foreach(g, c->qhGroups) {
		qWarning("Group %s (Inh %d  Able %d)", qPrintable(g->qsName), g->bInherit, g->bInheritable);
		foreach(pid, g->qsAdd)
			qWarning("Add %d", pid);
		foreach(pid, g->qsRemove)
			qWarning("Remove %d", pid);
	}
	foreach(acl, c->qlACL) {
		int allow = static_cast<int>(acl->pAllow);
		int deny = static_cast<int>(acl->pDeny);
		qWarning("ChanACL Here %d Sub %d Allow %04x Deny %04x ID %d Group %s", acl->bApplyHere, acl->bApplySubs, allow, deny, acl->iPlayerId, qPrintable(acl->qsGroup));
	}
	qWarning(" ");

	foreach(c, c->qlChannels) {
		dumpChannel(c);
	}
}

QList<ServerDB::qpCommand> ServerDB::getCommands() {
	TransactionHolder th;
	QList<qpCommand> commands;

	QSqlQuery query;
	query.prepare("SELECT command,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9 FROM commands ORDER BY command_id");
	query.exec();
	while (query.next()) {
		qpCommand cmd;
		cmd.first = query.value(0).toString();
		for(int i=1;i<10;i++)
			cmd.second.append(query.value(i));
		commands << cmd;
	}
	if (commands.count() != 0) {
		query.prepare("DELETE FROM commands");
		query.exec();
	}
	return commands;
}

QList<ServerDB::qpBan> ServerDB::getBans() {
	TransactionHolder th;
	QList<qpBan> bans;

	QSqlQuery query;
	query.prepare("SELECT base,mask FROM bans");
	query.exec();
	while (query.next()) {
		qpBan ban;
		ban.first = query.value(0).toUInt();
		ban.second = query.value(1).toInt();
		bans << ban;
	}
	return bans;
}

void ServerDB::setBans(QList<ServerDB::qpBan> bans) {
	TransactionHolder th;
	qpBan ban;

	QSqlQuery query;
	query.prepare("DELETE FROM bans");
	query.exec();
	foreach(ban, bans) {
		query.prepare("INSERT INTO bans (base,mask) VALUES (?,?)");
		query.addBindValue(ban.first);
		query.addBindValue(ban.second);
		query.exec();
	}
}
