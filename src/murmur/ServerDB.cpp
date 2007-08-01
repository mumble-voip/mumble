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
#include "Meta.h"
#include "Connection.h"
#include "DBus.h"

#define SQLDUMP(x) qWarning("%s", qPrintable(x.lastError().text()))

#define SQLDO(x) query.exec(QString::fromLatin1(x).arg(Meta::mp.qsDBPrefix))
#define SQLPREP(x) query.prepare(QString::fromLatin1(x).arg(Meta::mp.qsDBPrefix))
#define SQLEXEC() if (!query.exec()) qFatal("SQL Error [%s]: %s", qPrintable(query.lastQuery()), qPrintable(query.lastError().text()))

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
	if (! QSqlDatabase::isDriverAvailable(Meta::mp.qsDBDriver)) {
		qFatal("Database driver %s not available", qPrintable(Meta::mp.qsDBDriver));
	}
	QSqlDatabase db = QSqlDatabase::addDatabase(Meta::mp.qsDBDriver);
	QStringList datapaths;
	int i;

	bool found = false;

	if (Meta::mp.qsDBDriver == "QSQLITE") {
		if (! Meta::mp.qsDatabase.isEmpty()) {
			db.setDatabaseName(Meta::mp.qsDatabase);
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
		db.setDatabaseName(Meta::mp.qsDatabase);
		db.setHostName(Meta::mp.qsDBHostName);
		db.setPort(Meta::mp.iDBPort);
		db.setUserName(Meta::mp.qsDBUserName);
		db.setPassword(Meta::mp.qsDBPassword);
		found = db.open();
	}

	if (! found) {
		QSqlError e = db.lastError();
		qFatal("ServerDB: Failed initialization: %s",qPrintable(e.text()));
	}

	QSqlQuery query;

	int version = 0;

	SQLDO("SELECT value FROM %1meta WHERE key = 'version'");
	if (query.next())
		version = query.value(0).toInt();

	if (version < 1) {
		if (Meta::mp.qsDBDriver == "QSQLITE") {
			bool migrate = false;
			if (SQLDO("SELECT count(*) FROM players")) {
				migrate = true;
				SQLDO("ALTER TABLE players RENAME TO playersold");
				SQLDO("DROP TABLE player_auth");
				SQLDO("ALTER TABLE channels RENAME TO channelsold");
				SQLDO("ALTER TABLE groups RENAME TO groupsold");
				SQLDO("ALTER TABLE group_members RENAME TO group_membersold");
				SQLDO("ALTER TABLE acl RENAME TO aclold");
				SQLDO("DROP TABLE channel_links");
				SQLDO("ALTER TABLE bans RENAME TO bansold");
			}
			SQLDO("CREATE TABLE %1meta (key TEXT PRIMARY KEY, value TEXT)");
			SQLDO("CREATE TABLE %1servers (server_id INTEGER PRIMARY KEY AUTOINCREMENT)");
			SQLDO("CREATE TABLE %1config (server_id INTEGER, key TEXT, value TEXT)");
			SQLDO("CREATE UNIQUE INDEX %1config_key ON %1config(server_id, key)");

			SQLDO("CREATE TABLE %1players (server_id INTEGER, player_id INTEGER, name TEXT, email TEXT, pw TEXT, lastchannel INTEGER, texture BLOB, last_active DATE)");
			SQLDO("CREATE UNIQUE INDEX %1players_name ON %1players (server_id,name)");
			SQLDO("CREATE TRIGGER %1players_server_del AFTER DELETE ON %1servers FOR EACH ROW BEGIN DELETE FROM %1players WHERE server_id = old.server_id; END;");
			SQLDO("CREATE TRIGGER %1players_update_timestamp AFTER UPDATE OF lastchannel ON %1players FOR EACH ROW BEGIN UPDATE %1players SET last_active = datetime('now') WHERE player_id = old.player_id AND server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1player_auth (player_auth_id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, pw TEXT, email TEXT, authcode TEXT)");
			SQLDO("CREATE UNIQUE INDEX %1player_auth_name ON %1player_auth(name)");
			SQLDO("CREATE UNIQUE INDEX %1player_auth_code ON %1player_auth(authcode)");

			SQLDO("CREATE TABLE %1channels (server_id INTEGER, channel_id INTEGER, parent_id INTEGER, name TEXT, inheritacl INTEGER)");
			SQLDO("CREATE UNIQUE INDEX %1channel_id ON %1channels(server_id, channel_id)");
			SQLDO("CREATE TRIGGER %1channels_parent_del AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1channels WHERE parent_id = old.channel_id AND server_id = old.server_id; UPDATE %1players SET lastchannel=0 WHERE lastchannel = old.channel_id AND server_id = old.server_id; END;");
			SQLDO("CREATE TRIGGER %1channels_server_del AFTER DELETE ON %1servers FOR EACH ROW BEGIN DELETE FROM %1channels WHERE server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1groups (group_id INTEGER PRIMARY KEY AUTOINCREMENT, server_id INTEGER, name TEXT, channel_id INTEGER, inherit INTEGER, inheritable INTEGER)");
			SQLDO("CREATE UNIQUE INDEX %1groups_name_channels ON %1groups(server_id, channel_id, name)");
			SQLDO("CREATE TRIGGER %1groups_del_channel AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1groups WHERE channel_id = old.channel_id AND server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1group_members (group_id INTEGER, player_id INTEGER, addit INTEGER)");
			SQLDO("CREATE TRIGGER %1groups_members_del_group AFTER DELETE ON %1groups FOR EACH ROW BEGIN DELETE FROM %1group_members WHERE group_id = old.group_id; END;");
			SQLDO("CREATE TRIGGER %1groups_members_del_player AFTER DELETE ON %1players FOR EACH ROW BEGIN DELETE FROM %1group_members WHERE player_id = old.player_id; END;");

			SQLDO("CREATE TABLE %1acl (server_id INTEGER, channel_id INTEGER, priority INTEGER, player_id INTEGER, group_name TEXT, apply_here INTEGER, apply_sub INTEGER, grantpriv INTEGER, revokepriv INTEGER)");
			SQLDO("CREATE UNIQUE INDEX %1acl_channel_pri ON %1acl(server_id, channel_id, priority)");
			SQLDO("CREATE TRIGGER %1acl_del_channel AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1acl WHERE channel_id = old.channel_id AND server_id = old.server_id; END;");
			SQLDO("CREATE TRIGGER %1acl_del_player AFTER DELETE ON %1players FOR EACH ROW BEGIN DELETE FROM %1acl WHERE player_id = old.player_id AND server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1channel_links (server_id INTEGER, channel_id INTEGER, link_id INTEGER)");
			SQLDO("CREATE TRIGGER %1channel_links_del_channel AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1channel_links WHERE server_id = old.server_id AND (channel_id = old.channel_id OR link_id = old.channel_id); END;");
			SQLDO("DELETE FROM %1channel_links");

			SQLDO("CREATE TABLE %1bans (server_id INTEGER, base INTEGER, mask INTEGER)");

			SQLDO("INSERT INTO %1servers (server_id) VALUES(1)");
			SQLDO("INSERT INTO %1meta (key, value) VALUES('version','1')");

			if (migrate) {
				qDebug("Migrating from single-server database to multi-server database");
				SQLDO("INSERT INTO %1players SELECT 1, player_id, name, email, pw, lastchannel, texture, null FROM playersold");
				SQLDO("INSERT INTO %1channels SELECT 1, channel_id, parent_id, name, inheritACL FROM channelsold");
				SQLDO("INSERT INTO %1groups SELECT group_id, 1, name, channel_id, inherit, inheritable FROM groupsold");
				SQLDO("INSERT INTO %1group_members SELECT group_id, player_id, addit FROM group_membersold");
				SQLDO("INSERT INTO %1acl SELECT 1, channel_id, priority, player_id, group_name, apply_here, apply_sub, grantpriv, revokepriv FROM aclold");
				SQLDO("INSERT INTO %1bans SELECT 1, base, mask FROM bansold");
				SQLDO("DROP TABLE playersold");
				SQLDO("DROP TABLE channelsold");
				SQLDO("DROP TABLE groupsold");
				SQLDO("DROP TABLE group_membersold");
				SQLDO("DROP TABLE aclold");
				SQLDO("DROP TABLE bansold");
			}
		} else {
			qFatal("SQL Schema is version %d, requires version 1", version);
		}
	}
}

void Server::initialize() {
	QSqlQuery query;

	SQLPREP("SELECT channel_id FROM %1channels WHERE server_id = ? AND channel_id = 0");
	query.addBindValue(iServerNum);
	SQLEXEC();
	if (! query.next()) {
		SQLPREP("INSERT INTO %1channels (server_id, channel_id, parent_id, name) VALUES (?, ?, ?, ?)");
		query.addBindValue(iServerNum);
		query.addBindValue(0);
		query.addBindValue(QVariant());
		query.addBindValue(QLatin1String("Root"));
		SQLEXEC();
		SQLPREP("UPDATE %1channels SET channel_id = 0 WHERE server_id = ? AND name = ? AND parent_id IS NULL");
		query.addBindValue(iServerNum);
		query.addBindValue(QLatin1String("Root"));
		SQLEXEC();
	}

	SQLPREP("SELECT player_id FROM %1players WHERE server_id = ? AND player_id = 0");
	query.addBindValue(iServerNum);
	SQLEXEC();
	if (! query.next()) {
		SQLPREP("INSERT INTO %1players (server_id, player_id, name) VALUES (?, ?, ?)");
		query.addBindValue(iServerNum);
		query.addBindValue(0);
		query.addBindValue(QLatin1String("SuperUser"));
		SQLEXEC();
		SQLPREP("UPDATE %1players SET player_id = 0 WHERE server_id = ? AND name = ?");
		query.addBindValue(iServerNum);
		query.addBindValue(QLatin1String("SuperUser"));
		SQLEXEC();
	}

	SQLPREP("SELECT COUNT(*) FROM %1acl WHERE server_id=?");
	query.addBindValue(iServerNum);
	SQLEXEC();
	if (query.next()) {
		int c = query.value(0).toInt();
		if (c == 0) {
			SQLPREP("INSERT INTO %1acl (server_id, channel_id, priority, group_name, apply_here, apply_sub, grantpriv) VALUES (?,?,?,?,?,?,?)");
			query.addBindValue(iServerNum);
			query.addBindValue(0);
			query.addBindValue(1);
			query.addBindValue(QLatin1String("auth"));
			query.addBindValue(1);
			query.addBindValue(0);
			query.addBindValue(64);
			SQLEXEC();

			query.addBindValue(iServerNum);
			query.addBindValue(0);
			query.addBindValue(2);
			query.addBindValue(QLatin1String("admin"));
			query.addBindValue(1);
			query.addBindValue(1);
			query.addBindValue(1);
			SQLEXEC();
		}
	}

	SQLPREP("SELECT COUNT(*) FROM %1groups WHERE server_id=? AND channel_id=?");
	query.addBindValue(iServerNum);
	query.addBindValue(0);
	SQLEXEC();
	if (query.next()) {
		int c = query.value(0).toInt();
		if (c == 0) {
			SQLPREP("INSERT INTO %1groups(server_id, channel_id, name, inherit, inheritable) VALUES (?,?,?,?,?)");
			query.addBindValue(iServerNum);
			query.addBindValue(0);
			query.addBindValue(QLatin1String("admin"));
			query.addBindValue(1);
			query.addBindValue(1);
			SQLEXEC();
		}
	}

	SQLDO("VACUUM");
}

// -1 Wrong PW
// -2 Anonymous

int Server::authenticate(QString &name, const QString &pw) {
	int res = dbus->authenticate(name, pw);
	if (res != -2) {
		if (res != -1) {
			TransactionHolder th;
			QSqlQuery query;

			SQLPREP("REPLACE INTO %1players (server_id, player_id, name) VALUES (?,?,?)");
			query.addBindValue(iServerNum);
			query.addBindValue(res);
			query.addBindValue(name);
			SQLEXEC();
		}
		return res;
	}

	TransactionHolder th;
	QSqlQuery query;

	SQLPREP("SELECT player_id,name,pw FROM %1players WHERE server_id = ? AND name like ?");
	query.addBindValue(iServerNum);
	query.addBindValue(name);
	SQLEXEC();
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

void Server::setPW(int id, const QString &pw) {
	TransactionHolder th;

	QCryptographicHash hash(QCryptographicHash::Sha1);

	hash.addData(pw.toUtf8());

	QSqlQuery query;
	SQLPREP("UPDATE %1players SET pw=? WHERE server_id = ? AND player_id=?");
	query.addBindValue(QString::fromLatin1(hash.result().toHex()));
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();
}

void ServerDB::setSUPW(int srvnum, const QString &pw) {
	TransactionHolder th;

	QCryptographicHash hash(QCryptographicHash::Sha1);

	hash.addData(pw.toUtf8());

	QSqlQuery query;
	SQLPREP("UPDATE %1players SET pw=? WHERE server_id = ? AND player_id=?");
	query.addBindValue(QString::fromLatin1(hash.result().toHex()));
	query.addBindValue(srvnum);
	query.addBindValue(0);
	SQLEXEC();
}

QString Server::getUserName(int id) {
	QString name = dbus->mapIdToName(id);
	if (! name.isEmpty())
		return name;

	TransactionHolder th;
	QSqlQuery query;
	SQLPREP("SELECT name FROM %1players WHERE server_id = ? AND player_id = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();
	if (query.next()) {
		name = query.value(0).toString();
	}
	return name;
}

int Server::getUserID(const QString &name) {
	int id = dbus->mapNameToId(name);

	if (id != -2)
		return id;
	TransactionHolder th;

	QSqlQuery query;
	SQLPREP("SELECT player_id FROM %1players WHERE server_id = ? AND name like ?");
	query.addBindValue(iServerNum);
	query.addBindValue(name);
	SQLEXEC();
	if (query.next()) {
		id = query.value(0).toInt();
	}
	return id;
}

QByteArray Server::getUserTexture(int id) {
	QByteArray qba=dbus->mapIdToTexture(id);
	if (! qba.isNull()) {
		return qba;
	}

	TransactionHolder th;

	QSqlQuery query;
	SQLPREP("SELECT texture FROM %1players WHERE server_id = ? AND player_id = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();
	if (query.next()) {
		qba = query.value(0).toByteArray();
	}
	return qba;
}

void Server::addLink(Channel *c, Channel *l) {
	TransactionHolder th;

	QSqlQuery query;
	SQLPREP("INSERT INTO %1channel_links (server_id, channel_id, link_id) VALUES (?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	query.addBindValue(l->iId);
	SQLEXEC();

	query.addBindValue(iServerNum);
	query.addBindValue(l->iId);
	query.addBindValue(c->iId);
	SQLEXEC();
}

void Server::removeLink(Channel *c, Channel *l) {
	TransactionHolder th;

	QSqlQuery query;

	if (l) {
		SQLPREP("DELETE FROM %1channel_links WHERE server_id = ? AND channel_id = ? AND link_id = ?");
		query.addBindValue(iServerNum);
		query.addBindValue(c->iId);
		query.addBindValue(l->iId);
		SQLEXEC();

		query.addBindValue(iServerNum);
		query.addBindValue(l->iId);
		query.addBindValue(c->iId);
		SQLEXEC();
	} else {
		SQLPREP("DELETE FROM %1channel_links WHERE server_id = ? AND (channel_id = ? OR link_id = ?)");
		query.addBindValue(iServerNum);
		query.addBindValue(c->iId);
		query.addBindValue(c->iId);
		SQLEXEC();
	}
}

Channel *Server::addChannel(Channel *p, const QString &name) {
	TransactionHolder th;

	QSqlQuery query;
	SQLPREP("INSERT INTO %1channels (server_id, parent_id, name) VALUES (?,?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(p->iId);
	query.addBindValue(name);
	SQLEXEC();
	int id = query.lastInsertId().toInt();
	Channel *c = new Channel(id, name, p);
	qhChannels.insert(id, c);
	return c;
}

void Server::removeChannel(const Channel *c) {
	TransactionHolder th;

	QSqlQuery query;
	SQLPREP("DELETE FROM %1channels WHERE server_id = ? AND channel_id = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	SQLEXEC();
}

void Server::updateChannel(const Channel *c) {
	TransactionHolder th;
	Group *g;
	ChanACL *acl;

	QSqlQuery query;
	SQLPREP("UPDATE %1channels SET parent_id = ?, inheritacl = ? WHERE server_id = ? AND channel_id = ?");
	query.addBindValue(c->cParent ? c->cParent->iId : QVariant());
	query.addBindValue(c->bInheritACL ? 1 : 0);
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	SQLEXEC();

	SQLPREP("DELETE FROM %1groups WHERE server_id = ? AND channel_id = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	SQLEXEC();

	SQLPREP("DELETE FROM %1acl WHERE server_id = ? AND channel_id = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	SQLEXEC();

	foreach(g, c->qhGroups) {
		SQLPREP("INSERT INTO %1groups (server_id, channel_id, name, inherit, inheritable) VALUES (?,?,?,?)");
		query.addBindValue(iServerNum);
		query.addBindValue(g->c->iId);
		query.addBindValue(g->qsName);
		query.addBindValue(g->bInherit ? 1 : 0);
		query.addBindValue(g->bInheritable ? 1 : 0);
		SQLEXEC();

		int id = query.lastInsertId().toInt();
		int pid;

		foreach(pid, g->qsAdd) {
			SQLPREP("INSERT INTO %1group_members (group_id, player_id, addit) VALUES (?, ?, ?)");
			query.addBindValue(id);
			query.addBindValue(pid);
			query.addBindValue(1);
			SQLEXEC();
		}
		foreach(pid, g->qsRemove) {
			SQLPREP("INSERT INTO %1group_members (group_id, player_id, addit) VALUES (?, ?, ?)");
			query.addBindValue(id);
			query.addBindValue(pid);
			query.addBindValue(0);
			SQLEXEC();
		}
	}

	int pri = 5;

	foreach(acl, c->qlACL) {
		SQLPREP("INSERT INTO %1acl (server_id, channel_id, priority, player_id, group_name, apply_here, apply_sub, grantpriv, revokepriv) VALUES (?,?,?,?,?,?,?,?)");
		query.addBindValue(iServerNum);
		query.addBindValue(acl->c->iId);
		query.addBindValue(pri++);

		query.addBindValue((acl->iPlayerId == -1) ? QVariant() : acl->iPlayerId);
		query.addBindValue((acl->qsGroup.isEmpty()) ? QVariant() : acl->qsGroup);
		query.addBindValue(acl->bApplyHere ? 1 : 0);
		query.addBindValue(acl->bApplySubs ? 1 : 0);
		query.addBindValue(static_cast<int>(acl->pAllow));
		query.addBindValue(static_cast<int>(acl->pDeny));
		SQLEXEC();
	}
}

void Server::readChannelPrivs(Channel *c) {
	TransactionHolder th;

	int cid = c->iId;

	QSqlQuery query;
	SQLPREP("SELECT group_id, name, inherit, inheritable FROM %1groups WHERE server_id = ? AND channel_id = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(cid);
	SQLEXEC();
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

	SQLPREP("SELECT player_id, group_name, apply_here, apply_sub, grantpriv, revokepriv FROM %1acl WHERE server_id = ? AND channel_id = ? ORDER BY priority");
	query.addBindValue(iServerNum);
	query.addBindValue(cid);
	SQLEXEC();
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

void Server::readChannels(Channel *p) {
	QList<Channel *> kids;
	Channel *c;
	QSqlQuery query;
	int parentid = -1;

	if (p) {
		parentid = p->iId;
		readChannelPrivs(qhChannels.value(parentid));
	}

	{
		TransactionHolder th;
		if (parentid == -1) {
			SQLPREP("SELECT channel_id, name, inheritacl FROM %1channels WHERE server_id = ? AND parent_id IS NULL ORDER BY name");
			query.addBindValue(iServerNum);
		} else {
			SQLPREP("SELECT channel_id, name, inheritacl FROM %1channels WHERE server_id = ? AND parent_id=? ORDER BY name");
			query.addBindValue(iServerNum);
			query.addBindValue(parentid);
		}
		SQLEXEC();
		while (query.next()) {
			c = new Channel(query.value(0).toInt(), query.value(1).toString(), p);
			qhChannels.insert(c->iId, c);
			c->bInheritACL = query.value(2).toBool();
			kids << c;
		}
	}

	foreach(c, kids)
	readChannels(c);
}

void Server::setLastChannel(const Player *p) {

	if (p->iId < 0)
		return;

	TransactionHolder th;
	QSqlQuery query;

	SQLPREP("UPDATE %1players SET lastchannel=? WHERE server_id = ? AND player_id = ?");
	query.addBindValue(p->cChannel->iId);
	query.addBindValue(iServerNum);
	query.addBindValue(p->iId);
	SQLEXEC();
}

int Server::readLastChannel(Player *p) {

	Channel *c = qhChannels.value(0);

	if (p->iId >= 0) {
		TransactionHolder th;
		QSqlQuery query;

		SQLPREP("SELECT lastchannel FROM %1players WHERE server_id = ? AND player_id = ?");
		query.addBindValue(iServerNum);
		query.addBindValue(p->iId);
		SQLEXEC();

		if (query.next()) {
			int id = query.value(0).toInt();
			Channel *chan = qhChannels.value(id);
			if (chan)
				c = chan;
		}
	}
	return c->iId;
}

void Server::dumpChannel(const Channel *c) {
	Group *g;
	ChanACL *acl;
	int pid;

	if (c == NULL) {
		c = qhChannels.value(0);
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

void Server::getBans() {
	TransactionHolder th;

	qlBans.clear();

	QSqlQuery query;
	SQLPREP("SELECT base,mask FROM %1bans WHERE server_id = ?");
	query.addBindValue(iServerNum);
	SQLEXEC();
	while (query.next()) {
		qpBan ban;
		ban.first = query.value(0).toUInt();
		ban.second = query.value(1).toInt();
		qlBans << ban;
	}
}

void Server::saveBans() {
	TransactionHolder th;
	qpBan ban;

	QSqlQuery query;
	SQLPREP("DELETE FROM %1bans WHERE server_id = ? ");
	query.addBindValue(iServerNum);
	SQLEXEC();
	foreach(ban, qlBans) {
		SQLPREP("INSERT INTO %1bans (server_id, base,mask) VALUES (?,?,?)");
		query.addBindValue(iServerNum);
		query.addBindValue(ban.first);
		query.addBindValue(ban.second);
		SQLEXEC();
	}
}

QVariant Server::getConf(const QString &key, QVariant def) {
	return ServerDB::getConf(iServerNum, key, def);
}

QVariant ServerDB::getConf(int server_id, const QString &key, QVariant def) {
	TransactionHolder th;

	QSqlQuery query;
	SQLPREP("SELECT value FROM %1config WHERE server_id = ? AND key = ?");
	query.addBindValue(server_id);
	query.addBindValue(key);
	SQLEXEC();
	if (query.next()) {
		return query.value(0);
	}
	return def;
}

void Server::setConf(const QString &key, const QVariant &value) {
	ServerDB::setConf(iServerNum, key, value);
}

void ServerDB::setConf(int server_id, const QString &key, const QVariant &value) {
	TransactionHolder th;

	QSqlQuery query;
	if (value.isNull()) {
		SQLPREP("DELETE FROM %1config WHERE server_id = ? AND key = ?");
		query.addBindValue(server_id);
		query.addBindValue(key);
	} else {
		SQLPREP("REPLACE INTO %1config (server_id, key, value) VALUES (?,?,?)");
		query.addBindValue(server_id);
		query.addBindValue(key);
		query.addBindValue(value.toString());
	}
	SQLEXEC();
}


QList<int> ServerDB::getAllServers() {
	TransactionHolder th;
	QSqlQuery query;
	SQLPREP("SELECT server_id FROM %1servers");
	SQLEXEC();

	QList<int> ql;
	while (query.next())
		ql << query.value(0).toInt();
	return ql;
}

QList<int> ServerDB::getBootServers() {
	QList<int> ql = getAllServers();

	TransactionHolder th;
	QSqlQuery query;

	QList<int> bootlist;
	foreach(int i, ql) {
		SQLPREP("SELECT value FROM %1config WHERE server_id = ? AND key = ?");
		query.addBindValue(i);
		query.addBindValue(QLatin1String("boot"));
		SQLEXEC();
		if (! query.next() || query.value(0).toBool())
			bootlist << i;
	}
	return bootlist;
}

bool ServerDB::serverExists(int num) {
	TransactionHolder th;
	QSqlQuery query;
	SQLPREP("SELECT server_id FROM %1servers WHERE server_id = ?");
	query.addBindValue(num);
	SQLEXEC();
	if (query.next())
		return true;
	return false;
}

int ServerDB::addServer() {
	TransactionHolder th;
	QSqlQuery query;
	SQLPREP("SELECT MAX(server_id)+1 AS id FROM %1servers");
	SQLEXEC();
	int id = 0;
	if (query.next())
		id = query.value(0).toInt();
	SQLPREP("INSERT INTO %1servers (server_id) VALUES (?)");
	query.addBindValue(id);
	SQLEXEC();
	return id;
}
