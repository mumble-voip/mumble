/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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
#include "DBus.h"

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
	if (! QSqlDatabase::isDriverAvailable(g_sp.qsDBDriver)) {
		qFatal("Database driver %s not available", qPrintable(g_sp.qsDBDriver));
	}
	QSqlDatabase db = QSqlDatabase::addDatabase(g_sp.qsDBDriver);
	QStringList datapaths;
	int i;

	bool found = false;

	if (g_sp.qsDBDriver == "QSQLITE") {
		if (! g_sp.qsDatabase.isEmpty()) {
			db.setDatabaseName(g_sp.qsDatabase);
			found = db.open();
		} else {
			datapaths << QCoreApplication::instance()->applicationDirPath();
			datapaths << QDir::currentPath();
			datapaths << QDir::homePath();

			for (i = 0; (i < datapaths.size()) && ! found; i++) {
				if (!datapaths[i].isEmpty()) {
					QFile f(datapaths[i] + "/murmur.sqlite");
					if (f.exists()) {
						db.setDatabaseName(f.fileName());
						found = db.open();
					}
				}
			}

			if (! found) {
				for (i = 0; (i < datapaths.size()) && ! found; i++) {
					if (!datapaths[i].isEmpty()) {
						QFile f(datapaths[i] + "/murmur.sqlite");
						db.setDatabaseName(f.fileName());
						found = db.open();
					}
				}
			}
		}
		if (found) {
			QFileInfo fi(db.databaseName());
			qDebug("Openend SQLite database %s", qPrintable(fi.absoluteFilePath()));
		}
	} else {
		db.setDatabaseName(g_sp.qsDatabase);
		db.setHostName(g_sp.qsDBHostName);
		db.setPort(g_sp.iDBPort);
		db.setUserName(g_sp.qsDBUserName);
		db.setPassword(g_sp.qsDBPassword);
		found = db.open();
	}

	if (! found) {
		QSqlError e = db.lastError();
		qFatal("ServerDB: Failed initialization: %s",qPrintable(e.text()));
	}

	QSqlQuery query;

	if (g_sp.qsDBDriver == "QSQLITE") {
		query.exec(QString::fromLatin1("CREATE TABLE %1players (player_id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, email TEXT, pw TEXT)").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("ALTER TABLE %1players ADD COLUMN lastchannel INTEGER").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("ALTER TABLE %1players ADD COLUMN texture BLOB").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE UNIQUE INDEX %1players_name ON %1players (name)").arg(g_sp.qsDBPrefix));

		query.exec(QString::fromLatin1("CREATE TABLE %1player_auth (player_auth_id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, pw TEXT, email TEXT, authcode TEXT)").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE UNIQUE INDEX %1player_auth_name ON %1player_auth(name)").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE UNIQUE INDEX %1player_auth_code ON %1player_auth(authcode)").arg(g_sp.qsDBPrefix));

		query.exec(QString::fromLatin1("CREATE TABLE %1channels (channel_id INTEGER PRIMARY KEY AUTOINCREMENT, parent_id INTEGER, name TEXT, inheritACL INTEGER)").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE TRIGGER %1channels_parent_del AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1channels WHERE parent_id = old.channel_id; UPDATE %1players SET lastchannel=0 WHERE lastchannel = old.channel_id; END;").arg(g_sp.qsDBPrefix));

		query.exec(QString::fromLatin1("CREATE TABLE %1groups (group_id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, channel_id INTEGER, inherit INTEGER, inheritable INTEGER)").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE UNIQUE INDEX %1groups_name_channels ON %1groups(name, channel_id)").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE TRIGGER %1groups_del_channel AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1groups WHERE channel_id = old.channel_id; END;").arg(g_sp.qsDBPrefix));

		query.exec(QString::fromLatin1("CREATE TABLE %1group_members (group_members_id INTEGER PRIMARY KEY AUTOINCREMENT, group_id INTEGER, player_id INTEGER, addit INTEGER)").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE TRIGGER %1groups_members_del_group AFTER DELETE ON %1groups FOR EACH ROW BEGIN DELETE FROM %1group_members WHERE group_id = old.group_id; END;").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE TRIGGER %1groups_members_del_player AFTER DELETE ON %1players FOR EACH ROW BEGIN DELETE FROM %1group_members WHERE player_id = old.player_id; END;").arg(g_sp.qsDBPrefix));

		query.exec(QString::fromLatin1("CREATE TABLE %1acl (acl_id INTEGER PRIMARY KEY AUTOINCREMENT, channel_id INTEGER, priority INTEGER, player_id INTEGER, group_name TEXT, apply_here INTEGER, apply_sub INTEGER, grantpriv INTEGER, revokepriv INTEGER)").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE UNIQUE INDEX %1acl_channel_pri ON %1acl(channel_id, priority)").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE TRIGGER %1acl_del_channel AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1acl WHERE channel_id = old.channel_id; END;").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE TRIGGER %1acl_del_player AFTER DELETE ON %1players FOR EACH ROW BEGIN DELETE FROM %1acl WHERE player_id = old.player_id; END;").arg(g_sp.qsDBPrefix));

		query.exec(QString::fromLatin1("CREATE TABLE %1channel_links (channel_links_id INTEGER PRIMARY KEY AUTOINCREMENT, channel_id INTEGER, link_id INTEGER)").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("CREATE TRIGGER %1channel_links_del_channel AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1channel_links WHERE channel_id = old.channel_id OR link_id = old.channel_id; END;").arg(g_sp.qsDBPrefix));
		query.exec(QString::fromLatin1("DELETE FROM %1channel_links").arg(g_sp.qsDBPrefix));

		query.exec(QString::fromLatin1("CREATE TABLE %1bans (ban_id INTEGER PRIMARY KEY AUTOINCREMENT, base INTEGER, mask INTEGER)").arg(g_sp.qsDBPrefix));
	}

	query.exec(QString::fromLatin1("INSERT INTO %1channels (channel_id, parent_id, name) VALUES (0, NULL, 'Root')").arg(g_sp.qsDBPrefix));
	query.exec(QString::fromLatin1("INSERT INTO %1players (player_id, name, email, pw) VALUES (0, 'SuperUser', '', '')").arg(g_sp.qsDBPrefix));

	// Work around BUG in mysql, because apparantly insert with explicit id 0 means "aha, use the auto_increment and make it '1'"
	query.exec(QString::fromLatin1("UPDATE %1players SET player_id = 0 WHERE name LIKE 'SuperUser'").arg(g_sp.qsDBPrefix));
	query.exec(QString::fromLatin1("UPDATE %1channels SET channel_id = 0 WHERE name LIKE 'Root' AND parent_id IS NULL").arg(g_sp.qsDBPrefix));

	// Update from old method for sqlite. Root is now the only channel with a 'NULL' parent. This allows foreign keys in mysql.
	query.exec(QString::fromLatin1("UPDATE %1channels SET parent_id = NULL WHERE name LIKE 'Root' AND parent_id = -1").arg(g_sp.qsDBPrefix));

	query.exec(QString::fromLatin1("SELECT COUNT(*) FROM %1acl").arg(g_sp.qsDBPrefix));
	if (query.next()) {
		int c = query.value(0).toInt();
		if (c == 0) {
			query.exec(QString::fromLatin1("INSERT INTO %1acl (channel_id, priority, player_id, group_name, apply_here, apply_sub, grantpriv, revokepriv) VALUES (0, 1, NULL, 'auth', 1, 0, 64, 0)").arg(g_sp.qsDBPrefix));
			query.exec(QString::fromLatin1("INSERT INTO %1acl (channel_id, priority, player_id, group_name, apply_here, apply_sub, grantpriv, revokepriv) VALUES (0, 2, NULL, 'admin', 1, 1, 1, 0)").arg(g_sp.qsDBPrefix));
		}
	}

	query.exec(QString::fromLatin1("SELECT COUNT(*) FROM %1groups").arg(g_sp.qsDBPrefix));
	if (query.next()) {
		int c = query.value(0).toInt();
		if (c == 0) {
			query.exec(QString::fromLatin1("INSERT INTO %1groups (group_id, name, channel_id, inherit, inheritable) VALUES (0, 'admin', 0, 1, 1)").arg(g_sp.qsDBPrefix));
			query.exec(QString::fromLatin1("INSERT INTO %1group_members (group_id, player_id, addit) VALUES (0, 0, 1)").arg(g_sp.qsDBPrefix));
		}
	}

	query.exec(QString::fromLatin1("VACUUM"));
}

bool ServerDB::hasUsers() {
	TransactionHolder th;

	QSqlQuery query;
	query.prepare(QString::fromLatin1("SELECT count(*) FROM %1players").arg(g_sp.qsDBPrefix));
	query.exec();
	if (query.next()) {
		if (query.value(0).toInt() > 0)
			return true;
	}
	return false;
}

// -1 Wrong PW
// -2 Anonymous

int ServerDB::authenticate(QString &name, const QString &pw) {

	int res = dbus->authenticate(name, pw);
	if (res != -2) {
		if (res != -1) {
			TransactionHolder th;
			QSqlQuery query;

			query.prepare(QString::fromLatin1("INSERT INTO %1players (player_id, name) VALUES (?,?)").arg(g_sp.qsDBPrefix));
			query.addBindValue(res);
			query.addBindValue(name);
			query.exec();
		}
		return res;
	}

	TransactionHolder th;
	QSqlQuery query;

	query.prepare(QString::fromLatin1("SELECT player_id,name,pw FROM %1players WHERE name like ?").arg(g_sp.qsDBPrefix));
	query.addBindValue(name);
	query.exec();
	if (query.next()) {
		res = -1;
		QString storedpw = query.value(2).toString();
		if (! storedpw.isEmpty()) {
			QCryptographicHash hash(QCryptographicHash::Sha1);
			hash.addData(pw.toUtf8());
			if (storedpw == QString::fromLatin1(hash.result().toHex())) {
				name = query.value(1).toString();
				res = query.value(0).toInt();
			} else if (storedpw == pw) {
				name = query.value(1).toString();
				res = query.value(0).toInt();
				setPW(res, pw);
			}
		}
	}
	return res;
}

void ServerDB::setPW(int id, const QString &pw) {
	TransactionHolder th;

	QCryptographicHash hash(QCryptographicHash::Sha1);

	hash.addData(pw.toUtf8());

	QSqlQuery query;
	query.prepare(QString::fromLatin1("UPDATE %1players SET pw=? WHERE player_id=?").arg(g_sp.qsDBPrefix));
	query.addBindValue(QString::fromLatin1(hash.result().toHex()));
	query.addBindValue(id);
	query.exec();
}

QString ServerDB::getUserName(int id) {
	QString name = dbus->mapIdToName(id);
	if (! name.isEmpty())
		return name;

	TransactionHolder th;
	QSqlQuery query;
	query.prepare(QString::fromLatin1("SELECT name FROM %1players WHERE player_id = ?").arg(g_sp.qsDBPrefix));
	query.addBindValue(id);
	query.exec();
	if (query.next()) {
		name = query.value(0).toString();
	}
	return name;
}

int ServerDB::getUserID(const QString &name) {
	int id = dbus->mapNameToId(name);

	if (id != -2)
		return id;
	TransactionHolder th;

	QSqlQuery query;
	query.prepare(QString::fromLatin1("SELECT player_id FROM %1players WHERE name like ?").arg(g_sp.qsDBPrefix));
	query.addBindValue(name);
	query.exec();
	if (query.next()) {
		id = query.value(0).toInt();
	}
	return id;
}

QByteArray ServerDB::getUserTexture(int id) {
	QByteArray qba=dbus->mapIdToTexture(id);
	if (! qba.isNull()) {
		return qba;
	}

	TransactionHolder th;

	QSqlQuery query;
	query.prepare(QString::fromLatin1("SELECT texture FROM %1players WHERE player_id = ?").arg(g_sp.qsDBPrefix));
	query.addBindValue(id);
	query.exec();
	if (query.next()) {
		qba = query.value(0).toByteArray();
	}
	return qba;
}

void ServerDB::addLink(Channel *c, Channel *l) {
	TransactionHolder th;

	QSqlQuery query;
	query.prepare(QString::fromLatin1("INSERT INTO %1channel_links (channel_id, link_id) VALUES (?,?)").arg(g_sp.qsDBPrefix));
	query.addBindValue(c->iId);
	query.addBindValue(l->iId);
	query.exec();

	query.prepare(QString::fromLatin1("INSERT INTO %1channel_links (channel_id, link_id) VALUES (?,?)").arg(g_sp.qsDBPrefix));
	query.addBindValue(l->iId);
	query.addBindValue(c->iId);
	query.exec();
}

void ServerDB::removeLink(Channel *c, Channel *l) {
	TransactionHolder th;

	QSqlQuery query;

	if (l) {
		query.prepare(QString::fromLatin1("DELETE FROM %1channel_links WHERE channel_id = ? AND link_id = ?").arg(g_sp.qsDBPrefix));
		query.addBindValue(c->iId);
		query.addBindValue(l->iId);
		query.exec();

		query.prepare(QString::fromLatin1("DELETE FROM %1channel_links WHERE channel_id = ? AND link_id = ?").arg(g_sp.qsDBPrefix));
		query.addBindValue(l->iId);
		query.addBindValue(c->iId);
		query.exec();
	} else {
		query.prepare(QString::fromLatin1("DELETE FROM %1channel_links WHERE channel_id = ? OR link_id = ?").arg(g_sp.qsDBPrefix));
		query.addBindValue(c->iId);
		query.addBindValue(c->iId);
		query.exec();
	}
}

Channel *ServerDB::addChannel(Channel *parent, const QString &name) {
	TransactionHolder th;

	QSqlQuery query;
	query.prepare(QString::fromLatin1("INSERT INTO %1channels (parent_id, name) VALUES (?,?)").arg(g_sp.qsDBPrefix));
	query.addBindValue(parent->iId);
	query.addBindValue(name);
	query.exec();
	int id = query.lastInsertId().toInt();
	return Channel::add(id, name, parent);
}

void ServerDB::removeChannel(const Channel *c) {
	TransactionHolder th;

	QSqlQuery query;
	query.prepare(QString::fromLatin1("DELETE FROM %1channels WHERE channel_id = ?").arg(g_sp.qsDBPrefix));
	query.addBindValue(c->iId);
	query.exec();
}

void ServerDB::updateChannel(const Channel *c) {
	TransactionHolder th;
	Group *g;
	ChanACL *acl;

	QSqlQuery query;
	query.prepare(QString::fromLatin1("UPDATE %1channels SET parent_id = ?, inheritACL = ? WHERE channel_id = ?").arg(g_sp.qsDBPrefix));
	query.addBindValue(c->cParent ? c->cParent->iId : QVariant());
	query.addBindValue(c->bInheritACL ? 1 : 0);
	query.addBindValue(c->iId);
	query.exec();

	query.prepare(QString::fromLatin1("DELETE FROM %1groups WHERE channel_id = ?").arg(g_sp.qsDBPrefix));
	query.addBindValue(c->iId);
	query.exec();

	query.prepare(QString::fromLatin1("DELETE FROM %1acl WHERE channel_id = ?").arg(g_sp.qsDBPrefix));
	query.addBindValue(c->iId);
	query.exec();

	foreach(g, c->qhGroups) {
		query.prepare(QString::fromLatin1("INSERT INTO %1groups (name, channel_id, inherit, inheritable) VALUES (?,?,?,?)").arg(g_sp.qsDBPrefix));
		query.addBindValue(g->qsName);
		query.addBindValue(g->c->iId);
		query.addBindValue(g->bInherit ? 1 : 0);
		query.addBindValue(g->bInheritable ? 1 : 0);
		query.exec();

		int id = query.lastInsertId().toInt();
		int pid;

		foreach(pid, g->qsAdd) {
			query.prepare(QString::fromLatin1("INSERT INTO %1group_members (group_id, player_id, addit) VALUES (?, ?, ?)").arg(g_sp.qsDBPrefix));
			query.addBindValue(id);
			query.addBindValue(pid);
			query.addBindValue(1);
			query.exec();
		}
		foreach(pid, g->qsRemove) {
			query.prepare(QString::fromLatin1("INSERT INTO %1group_members (group_id, player_id, addit) VALUES (?, ?, ?)").arg(g_sp.qsDBPrefix));
			query.addBindValue(id);
			query.addBindValue(pid);
			query.addBindValue(0);
			query.exec();
		}
	}

	int pri = 5;

	foreach(acl, c->qlACL) {
		query.prepare(QString::fromLatin1("INSERT INTO %1acl (channel_id, priority, player_id, group_name, apply_here, apply_sub, grantpriv, revokepriv) VALUES (?,?,?,?,?,?,?,?)").arg(g_sp.qsDBPrefix));
		query.addBindValue(acl->c->iId);
		query.addBindValue(pri++);

		query.addBindValue((acl->iPlayerId == -1) ? QVariant() : acl->iPlayerId);
		query.addBindValue((acl->qsGroup.isEmpty()) ? QVariant() : acl->qsGroup);
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
	query.prepare(QString::fromLatin1("SELECT group_id, name, inherit, inheritable FROM %1groups WHERE channel_id = ?").arg(g_sp.qsDBPrefix));
	query.addBindValue(cid);
	query.exec();
	while (query.next()) {
		int gid = query.value(0).toInt();
		QString name = query.value(1).toString();
		Group *g = new Group(c, name);
		g->bInherit = query.value(2).toBool();
		g->bInheritable = query.value(3).toBool();

		QSqlQuery mem;
		mem.prepare("SELECT player_id, addit FROM %1group_members WHERE group_id = ?");
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

	query.prepare(QString::fromLatin1("SELECT player_id, group_name, apply_here, apply_sub, grantpriv, revokepriv FROM %1acl WHERE channel_id = ? ORDER BY priority").arg(g_sp.qsDBPrefix));
	query.addBindValue(cid);
	query.exec();
	while (query.next()) {
		ChanACL *acl = new ChanACL(c);
		acl->iPlayerId = query.value(0).isNull() ? -1 : query.value(0).toInt();
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
		if (parentid == -1) {
			query.prepare(QString::fromLatin1("SELECT channel_id, name, inheritACL FROM %1channels WHERE parent_id IS NULL ORDER BY name").arg(g_sp.qsDBPrefix));
		} else {
			query.prepare(QString::fromLatin1("SELECT channel_id, name, inheritACL FROM %1channels WHERE parent_id=? ORDER BY name").arg(g_sp.qsDBPrefix));
			query.addBindValue(parentid);
		}
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

void ServerDB::setLastChannel(const Player *p) {

	if (p->iId < 0)
		return;

		TransactionHolder th;
		QSqlQuery query;

		query.prepare(QString::fromLatin1("UPDATE %1players SET lastchannel=? WHERE player_id = ?").arg(g_sp.qsDBPrefix));
		query.addBindValue(p->cChannel->iId);
		query.addBindValue(p->iId);
		query.exec();
}

int ServerDB::readLastChannel(Player *p) {

	Channel *c = Channel::get(0);

	if (p->iId >= 0) {
		TransactionHolder th;
		QSqlQuery query;

		query.prepare(QString::fromLatin1("SELECT lastchannel FROM %1players WHERE player_id = ?").arg(g_sp.qsDBPrefix));
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

void ServerDB::dumpChannel(const Channel *c) {
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

QList<ServerDB::qpBan> ServerDB::getBans() {
	TransactionHolder th;
	QList<qpBan> bans;

	QSqlQuery query;
	query.prepare(QString::fromLatin1("SELECT base,mask FROM %1bans").arg(g_sp.qsDBPrefix));
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
	query.prepare(QString::fromLatin1("DELETE FROM %1bans").arg(g_sp.qsDBPrefix));
	query.exec();
	foreach(ban, bans) {
		query.prepare(QString::fromLatin1("INSERT INTO %1bans (base,mask) VALUES (?,?)").arg(g_sp.qsDBPrefix));
		query.addBindValue(ban.first);
		query.addBindValue(ban.second);
		query.exec();
	}
}
