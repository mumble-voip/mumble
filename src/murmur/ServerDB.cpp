/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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

#define SQLDO(x) ServerDB::exec(query, QLatin1String(x), false)
#define SQLPREP(x) ServerDB::prepare(query, QLatin1String(x))
#define SQLEXEC() ServerDB::exec(query)
#define SOFTEXEC() ServerDB::exec(query, QString(), false)

class TransactionHolder {
	public:
		QSqlQuery *qsqQuery;
		TransactionHolder() {
			ServerDB::db.transaction();
			qsqQuery = new QSqlQuery();
		}

		~TransactionHolder() {
			qsqQuery->clear();
			delete qsqQuery;
			ServerDB::db.commit();
		}
};

QSqlDatabase ServerDB::db;
Timer ServerDB::tLogClean;

ServerDB::ServerDB() {
	if (! QSqlDatabase::isDriverAvailable(Meta::mp.qsDBDriver)) {
		qFatal("ServerDB: Database driver %s not available", qPrintable(Meta::mp.qsDBDriver));
	}
	db = QSqlDatabase::addDatabase(Meta::mp.qsDBDriver);
	QStringList datapaths;
	int i;

	bool found = false;

	if (Meta::mp.qsDBDriver == "QSQLITE") {
		if (! Meta::mp.qsDatabase.isEmpty()) {
			db.setDatabaseName(Meta::mp.qsDatabase);
			found = db.open();
		} else {
			datapaths << Meta::mp.qdBasePath.absolutePath();
			datapaths << QDir::currentPath();
			datapaths << QCoreApplication::instance()->applicationDirPath();
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
			qWarning("ServerDB: Openend SQLite database %s", qPrintable(fi.absoluteFilePath()));
			if (! fi.isWritable())
				qFatal("ServerDB: Database is not writeable");
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

	SQLDO("SELECT value FROM %1meta WHERE keystring = 'version'");
	if (query.next())
		version = query.value(0).toInt();

	if (version < 1) {
		if ((Meta::mp.qsDBDriver != "QSQLITE") && (Meta::mp.qsDBDriver != "QMYSQL")) {
			qFatal("SQL Schema is version %d, requires version 1", version);
		}
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

		if (Meta::mp.qsDBDriver == "QSQLITE") {
			SQLDO("DROP TRIGGER IF EXISTS channels_parent_del");
			SQLDO("DROP TRIGGER IF EXISTS groups_del_channel");
			SQLDO("DROP TRIGGER IF EXISTS groups_members_del_group");
			SQLDO("DROP TRIGGER IF EXISTS acl_del_channel");
			SQLDO("DROP TRIGGER IF EXISTS acl_del_player");
			SQLDO("DROP TRIGGER IF EXISTS channel_links_del_channel");
			SQLDO("DROP INDEX IF EXISTS players_name");
			SQLDO("DROP INDEX IF EXISTS groups_name_channels");
			SQLDO("DROP INDEX IF EXISTS acl_channel_pri");
			SQLDO("CREATE TABLE %1meta (keystring TEXT PRIMARY KEY, value TEXT)");
			SQLDO("CREATE TABLE %1servers (server_id INTEGER PRIMARY KEY AUTOINCREMENT)");

			SQLDO("CREATE TABLE %1slog(server_id INTEGER NOT NULL, msg TEXT, msgtime DATE)");
			SQLDO("CREATE INDEX %1slog_time ON %1slog(msgtime)");
			SQLDO("CREATE TRIGGER %1slog_timestamp AFTER INSERT ON %1slog FOR EACH ROW BEGIN UPDATE %1slog SET msgtime = datetime('now') WHERE rowid = new.rowid; END;");
			SQLDO("CREATE TRIGGER %1slog_server_del AFTER DELETE ON %1servers FOR EACH ROW BEGIN DELETE FROM %1slog WHERE server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1config (server_id INTEGER NOT NULL, keystring TEXT, value TEXT)");
			SQLDO("CREATE UNIQUE INDEX %1config_key ON %1config(server_id, keystring)");
			SQLDO("CREATE TRIGGER %1config_server_del AFTER DELETE ON %1servers FOR EACH ROW BEGIN DELETE FROM %1config WHERE server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1channels (server_id INTEGER NOT NULL, channel_id INTEGER NOT NULL, parent_id INTEGER, name TEXT, inheritacl INTEGER)");
			SQLDO("CREATE UNIQUE INDEX %1channel_id ON %1channels(server_id, channel_id)");
			SQLDO("CREATE TRIGGER %1channels_parent_del AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1channels WHERE parent_id = old.channel_id AND server_id = old.server_id; UPDATE %1players SET lastchannel=0 WHERE lastchannel = old.channel_id AND server_id = old.server_id; END;");
			SQLDO("CREATE TRIGGER %1channels_server_del AFTER DELETE ON %1servers FOR EACH ROW BEGIN DELETE FROM %1channels WHERE server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1players (server_id INTEGER NOT NULL, player_id INTEGER NOT NULL, name TEXT NOT NULL, email TEXT, pw TEXT, lastchannel INTEGER, texture BLOB, last_active DATE)");
			SQLDO("CREATE UNIQUE INDEX %1players_name ON %1players (server_id,name)");
			SQLDO("CREATE UNIQUE INDEX %1players_id ON %1players (server_id, player_id)");
			SQLDO("CREATE TRIGGER %1players_server_del AFTER DELETE ON %1servers FOR EACH ROW BEGIN DELETE FROM %1players WHERE server_id = old.server_id; END;");
			SQLDO("CREATE TRIGGER %1players_update_timestamp AFTER UPDATE OF lastchannel ON %1players FOR EACH ROW BEGIN UPDATE %1players SET last_active = datetime('now') WHERE player_id = old.player_id AND server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1player_auth (player_auth_id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, pw TEXT, email TEXT, authcode TEXT)");
			SQLDO("CREATE UNIQUE INDEX %1player_auth_name ON %1player_auth(name)");
			SQLDO("CREATE UNIQUE INDEX %1player_auth_code ON %1player_auth(authcode)");

			SQLDO("CREATE TABLE %1groups (group_id INTEGER PRIMARY KEY AUTOINCREMENT, server_id INTEGER NOT NULL, name TEXT, channel_id INTEGER NOT NULL, inherit INTEGER, inheritable INTEGER)");
			SQLDO("CREATE UNIQUE INDEX %1groups_name_channels ON %1groups(server_id, channel_id, name)");
			SQLDO("CREATE TRIGGER %1groups_del_channel AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1groups WHERE channel_id = old.channel_id AND server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1group_members (group_id INTEGER NOT NULL, server_id INTEGER NOT NULL, player_id INTEGER NOT NULL, addit INTEGER)");
			SQLDO("CREATE TRIGGER %1groups_members_del_group AFTER DELETE ON %1groups FOR EACH ROW BEGIN DELETE FROM %1group_members WHERE group_id = old.group_id; END;");
			SQLDO("CREATE TRIGGER %1groups_members_del_player AFTER DELETE on %1players FOR EACH ROW BEGIN DELETE FROM %1group_members WHERE player_id = old.player_id AND server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1acl (server_id INTEGER NOT NULL, channel_id INTEGER NOT NULL, priority INTEGER, player_id INTEGER, group_name TEXT, apply_here INTEGER, apply_sub INTEGER, grantpriv INTEGER, revokepriv INTEGER)");
			SQLDO("CREATE UNIQUE INDEX %1acl_channel_pri ON %1acl(server_id, channel_id, priority)");
			SQLDO("CREATE TRIGGER %1acl_del_channel AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1acl WHERE channel_id = old.channel_id AND server_id = old.server_id; END;");
			SQLDO("CREATE TRIGGER %1acl_del_player AFTER DELETE ON %1players FOR EACH ROW BEGIN DELETE FROM %1acl WHERE player_id = old.player_id AND server_id = old.server_id; END;");

			SQLDO("CREATE TABLE %1channel_links (server_id INTEGER NOT NULL, channel_id INTEGER NOT NULL, link_id INTEGER NOT NULL)");
			SQLDO("CREATE TRIGGER %1channel_links_del_channel AFTER DELETE ON %1channels FOR EACH ROW BEGIN DELETE FROM %1channel_links WHERE server_id = old.server_id AND (channel_id = old.channel_id OR link_id = old.channel_id); END;");
			SQLDO("DELETE FROM %1channel_links");

			SQLDO("CREATE TABLE %1bans (server_id INTEGER NOT NULL, base INTEGER, mask INTEGER)");
			SQLDO("CREATE TRIGGER %1bans_del_server AFTER DELETE ON %1servers FOR EACH ROW BEGIN DELETE FROM %1bans WHERE server_id = old.server_id; END;");

			SQLDO("INSERT INTO %1servers (server_id) VALUES(1)");
			SQLDO("INSERT INTO %1meta (keystring, value) VALUES('version','2')");

			SQLDO("VACUUM");
		} else {
			SQLDO("CREATE TABLE %1meta(keystring varchar(255) PRIMARY KEY, value varchar(255)) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE TABLE %1servers(server_id INTEGER PRIMARY KEY AUTO_INCREMENT) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");

			SQLDO("CREATE TABLE %1slog(server_id INTEGER NOT NULL, msg TEXT, msgtime TIMESTAMP) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE INDEX %1slog_time ON %1slog(msgtime)");
			SQLDO("ALTER TABLE %1slog ADD CONSTRAINT %1slog_server_del FOREIGN KEY (server_id) REFERENCES %1servers(server_id) ON DELETE CASCADE");

			SQLDO("CREATE TABLE %1config (server_id INTEGER NOT NULL, keystring varchar(255), value TEXT) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE UNIQUE INDEX %1config_key ON %1config(server_id, keystring)");
			SQLDO("ALTER TABLE %1config ADD CONSTRAINT %1config_server_del FOREIGN KEY (server_id) REFERENCES %1servers(server_id) ON DELETE CASCADE");

			SQLDO("CREATE TABLE %1channels (server_id INTEGER NOT NULL, channel_id INTEGER NOT NULL, parent_id INTEGER, name varchar(255), inheritacl INTEGER) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE UNIQUE INDEX %1channel_id ON %1channels(server_id, channel_id)");
			SQLDO("ALTER TABLE %1channels ADD CONSTRAINT %1channels_parent_del FOREIGN KEY (server_id, parent_id) REFERENCES %1channels(server_id,channel_id) ON DELETE CASCADE");
			SQLDO("ALTER TABLE %1channels ADD CONSTRAINT %1channels_server_del FOREIGN KEY (server_id) REFERENCES %1servers(server_id) ON DELETE CASCADE");

			SQLDO("CREATE TABLE %1players (server_id INTEGER NOT NULL, player_id INTEGER NOT NULL, name varchar(255), email varchar(255), pw varchar(128), lastchannel INTEGER, texture LONGBLOB, last_active TIMESTAMP) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE INDEX %1players_channel ON %1players(server_id, lastchannel)");
			SQLDO("CREATE UNIQUE INDEX %1players_name ON %1players (server_id,name)");
			SQLDO("CREATE UNIQUE INDEX %1players_id ON %1players (server_id, player_id)");
			SQLDO("ALTER TABLE %1players ADD CONSTRAINT %1players_server_del FOREIGN KEY (server_id) REFERENCES %1servers(server_id) ON DELETE CASCADE");

			SQLDO("CREATE TABLE %1player_auth (player_auth_id INTEGER PRIMARY KEY AUTO_INCREMENT, name varchar(255), pw varchar(128), email varchar(255), authcode varchar(255)) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE UNIQUE INDEX %1player_auth_name ON %1player_auth(name)");
			SQLDO("CREATE UNIQUE INDEX %1player_auth_code ON %1player_auth(authcode)");

			SQLDO("CREATE TABLE %1groups (group_id INTEGER PRIMARY KEY AUTO_INCREMENT, server_id INTEGER NOT NULL, name varchar(255), channel_id INTEGER NOT NULL, inherit INTEGER, inheritable INTEGER) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE UNIQUE INDEX %1groups_name_channels ON %1groups(server_id, channel_id, name)");
			SQLDO("ALTER TABLE %1groups ADD CONSTRAINT %1groups_del_channel FOREIGN KEY (server_id, channel_id) REFERENCES %1channels(server_id, channel_id) ON DELETE CASCADE");

			SQLDO("CREATE TABLE %1group_members (group_id INTEGER NOT NULL, server_id INTEGER NOT NULL, player_id INTEGER NOT NULL, addit INTEGER) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE INDEX %1group_members_players ON %1group_members(server_id, player_id)");
			SQLDO("ALTER TABLE %1group_members ADD CONSTRAINT %1group_members_del_group FOREIGN KEY (group_id) REFERENCES %1groups(group_id) ON DELETE CASCADE");
			SQLDO("ALTER TABLE %1group_members ADD CONSTRAINT %1group_members_del_player FOREIGN KEY (server_id, player_id) REFERENCES %1players(server_id,player_id) ON DELETE CASCADE");

			SQLDO("CREATE TABLE %1acl (server_id INTEGER NOT NULL, channel_id INTEGER NOT NULL, priority INTEGER, player_id INTEGER, group_name varchar(255), apply_here INTEGER, apply_sub INTEGER, grantpriv INTEGER, revokepriv INTEGER) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE UNIQUE INDEX %1acl_channel_pri ON %1acl(server_id, channel_id, priority)");
			SQLDO("CREATE INDEX %1acl_player ON %1acl(server_id, player_id)");
			SQLDO("ALTER TABLE %1acl ADD CONSTRAINT %1acl_del_channel FOREIGN KEY (server_id, channel_id) REFERENCES %1channels(server_id, channel_id) ON DELETE CASCADE");
			SQLDO("ALTER TABLE %1acl ADD CONSTRAINT %1acl_del_player FOREIGN KEY (server_id, player_id) REFERENCES %1players(server_id, player_id) ON DELETE CASCADE");

			SQLDO("CREATE TABLE %1channel_links (server_id INTEGER NOT NULL, channel_id INTEGER NOT NULL, link_id INTEGER NOT NULL) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("ALTER TABLE %1channel_links ADD CONSTRAINT %1channel_links_del_channel FOREIGN KEY(server_id, channel_id) REFERENCES %1channels(server_id, channel_id) ON DELETE CASCADE");
			SQLDO("DELETE FROM %1channel_links");

			SQLDO("CREATE TABLE %1bans (server_id INTEGER NOT NULL, base INTEGER, mask INTEGER) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("ALTER TABLE %1bans ADD CONSTRAINT %1bans_del_server FOREIGN KEY(server_id) REFERENCES %1servers(server_id) ON DELETE CASCADE");

			SQLDO("INSERT INTO %1servers (server_id) VALUES(1)");
			SQLDO("INSERT INTO %1meta (keystring, value) VALUES('version','2')");
		}

		if (migrate) {
			qWarning("Migrating from single-server database to multi-server database");
			SQLDO("INSERT INTO %1channels SELECT 1, channel_id, parent_id, name, inheritACL FROM channelsold");
			SQLDO("INSERT INTO %1players SELECT 1, player_id, name, email, pw, lastchannel, texture, null FROM playersold");
			SQLDO("INSERT INTO %1groups SELECT group_id, 1, name, channel_id, inherit, inheritable FROM groupsold");
			SQLDO("INSERT INTO %1group_members SELECT group_id, 1, player_id, addit FROM group_membersold");
			SQLDO("INSERT INTO %1acl SELECT 1, channel_id, priority, player_id, group_name, apply_here, apply_sub, grantpriv, revokepriv FROM aclold");
			SQLDO("INSERT INTO %1bans SELECT 1, base, mask FROM bansold");
			SQLDO("DROP TABLE bansold");
			SQLDO("DROP TABLE group_membersold");
			SQLDO("DROP TABLE aclold");
			SQLDO("DROP TABLE groupsold");
			SQLDO("DROP TABLE playersold");
			SQLDO("DROP TABLE channelsold");
		}
	} else if (version < 2) {
		if (Meta::mp.qsDBDriver == "QSQLITE") {
			SQLDO("DROP TRIGGER %1log_timestamp");
			SQLDO("DROP TRIGGER %1log_server_del");

			SQLDO("ALTER TABLE %1log RENAME TO %1slog");

			SQLDO("CREATE TRIGGER %1slog_timestamp AFTER INSERT ON %1slog FOR EACH ROW BEGIN UPDATE %1slog SET msgtime = datetime('now') WHERE rowid = new.rowid; END;");
			SQLDO("CREATE TRIGGER %1slog_server_del AFTER DELETE ON %1servers FOR EACH ROW BEGIN DELETE FROM %1slog WHERE server_id = old.server_id; END;");

			SQLDO("UPDATE %1meta SET value='2' WHERE keystring='version'");
			SQLDO("CREATE UNIQUE INDEX %1players_id ON %1players (server_id, player_id)");
		} else {
			SQLDO("CREATE TABLE %1slog(server_id INTEGER, msg TEXT, msgtime TIMESTAMP) Type=InnoDB");
			SQLDO("CREATE INDEX %1slog_time ON %1slog(msgtime)");
			SQLDO("ALTER TABLE %1slog ADD CONSTRAINT %1slog_server_del FOREIGN KEY (server_id) REFERENCES %1servers(server_id) ON DELETE CASCADE");
			SQLDO("UPDATE %1meta SET value='2' WHERE keystring='version'");

			SQLDO("ALTER TABLE %1acl CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1bans CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1channel_links CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1channels CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1config CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1group_members CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1groups CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1meta CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1player_auth CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1players CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1servers CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
			SQLDO("ALTER TABLE %1slog CONVERT TO CHARACTER SET utf8 COLLATE utf8_unicode_ci");
		}
	}
	query.clear();
}

ServerDB::~ServerDB() {
	db.close();
}

bool ServerDB::prepare(QSqlQuery &query, const QString &str, bool fatal) {
	if (! db.isValid()) {
		qWarning("SQL [%s] rejected: Database is gone", qPrintable(str));
		return false;
	}
	QString q;
	if (str.contains(QLatin1String("%1")))
		q = str.arg(Meta::mp.qsDBPrefix);
	else
		q = str;

	if (query.prepare(q)) {
		return true;
	} else {
		db.close();
		if (! db.open()) {
			qFatal("Lost connection to SQL Database: Reconnect: %s", qPrintable(db.lastError().text()));
		}
		query = QSqlQuery();
		if (query.prepare(q)) {
			qWarning("SQL Connection lost, reconnection OK");
			return true;
		}

		if (fatal) {
			db = QSqlDatabase();
			qFatal("SQL Prepare Error [%s]: %s", qPrintable(q), qPrintable(query.lastError().text()));
		} else
			qDebug("SQL Prepare Error [%s]: %s", qPrintable(q), qPrintable(query.lastError().text()));
		return false;
	}
}

bool ServerDB::exec(QSqlQuery &query, const QString &str, bool fatal) {
	if (! str.isEmpty())
		prepare(query, str, fatal);
	if (query.exec()) {
		return true;
	} else {

		if (fatal) {
			db = QSqlDatabase();
			qFatal("SQL Error [%s]: %s", qPrintable(query.lastQuery()), qPrintable(query.lastError().text()));
		} else
			qDebug("SQL Error [%s]: %s", qPrintable(query.lastQuery()), qPrintable(query.lastError().text()));
		return false;
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
	query.clear();
}

int Server::registerPlayer(const QString &name) {
	if (name.isEmpty())
		return -1;

	if (getUserID(name) >= 0)
		return -1;

	qhUserIDCache.remove(name);

	int res = dbus->dbusRegisterPlayer(name);
	if (res != -2) {
		qhUserNameCache.remove(res);
		return res;
	}

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("SELECT MAX(player_id)+1 AS id FROM %1players WHERE server_id=? AND player_id < 1000000000");
	query.addBindValue(iServerNum);
	SQLEXEC();
	int id = 0;
	if (query.next())
		id = query.value(0).toInt();

	SQLPREP("INSERT INTO %1players (server_id, player_id, name) VALUES (?,?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	query.addBindValue(name);
	SQLEXEC();
	qhUserNameCache.remove(id);
	return id;
}

bool Server::unregisterPlayer(int id) {
	if (id <= 0)
		return false;

	QString oname, email;
	if (! getRegistration(id, oname, email))
		return false;

	qhUserIDCache.remove(oname);
	qhUserNameCache.remove(id);

	int res = dbus->dbusUnregisterPlayer(id);
	if (res >= 0) {
		return (res > 0);
	}

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("DELETE FROM %1players WHERE server_id = ? AND player_id = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();

	return true;
}

QMap<int, QPair<QString, QString> > Server::getRegisteredPlayers(const QString &filter) {
	QMap<int, QPair<QString, QString> > m;

	m = dbus->dbusGetRegisteredPlayers(filter);

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	if (filter.isEmpty()) {
		SQLPREP("SELECT player_id, name, email FROM %1players WHERE server_id = ?");
		query.addBindValue(iServerNum);
	} else {
		SQLPREP("SELECT player_id, name, email FROM %1players WHERE server_id = ? AND name LIKE ?");
		query.addBindValue(iServerNum);
		query.addBindValue(filter);
	}
	SQLEXEC();

	while (query.next()) {
		int id = query.value(0).toInt();
		QString name = query.value(1).toString();
		QString email = query.value(2).toString();
		m.insert(id, QPair<QString,QString>(name,email));
	}
	return m;
}

bool Server::getRegistration(int id, QString &name, QString &email) {
	int res = dbus->dbusGetRegistration(id, name, email);
	if (res >= 0)
		return (res > 0);

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT name, email FROM %1players WHERE server_id = ? AND player_id = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();
	if (query.next()) {
		name = query.value(0).toString();
		email = query.value(1).toString();
		return true;
	}
	return false;
}

// -1 Wrong PW
// -2 Anonymous

int Server::authenticate(QString &name, const QString &pw) {
	int res = dbus->authenticate(name, pw);
	if (res != -2) {
		if (res != -1) {
			TransactionHolder th;
			QSqlQuery &query = *th.qsqQuery;

			int lchan=readLastChannel(res);

			SQLPREP("REPLACE INTO %1players (server_id, player_id, name, lastchannel) VALUES (?,?,?,?)");
			query.addBindValue(iServerNum);
			query.addBindValue(res);
			query.addBindValue(name);
			query.addBindValue(lchan);
			SQLEXEC();
		}
		return res;
	}

	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

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

bool Server::setPW(int id, const QString &pw) {
	int res = dbus->dbusSetPW(id, pw);
	if (res >= 0)
		return (res > 0);

	TransactionHolder th;

	QCryptographicHash hash(QCryptographicHash::Sha1);

	hash.addData(pw.toUtf8());

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("UPDATE %1players SET pw=? WHERE server_id = ? AND player_id=?");
	query.addBindValue(pw.isEmpty() ? QVariant() : QString::fromLatin1(hash.result().toHex()));
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();

	return true;
}

bool Server::setEmail(int id, const QString &email) {
	if (id <= 0)
		return false;

	int res = dbus->dbusSetEmail(id, email);
	if (res >= 0)
		return (res > 0);

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("UPDATE %1players SET email=? WHERE server_id = ? AND player_id=?");
	query.addBindValue(email);
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();

	return true;
}

bool Server::setName(int id, const QString &name) {
	if (id <= 0)
		return false;

	int oid = getUserID(name);
	if (oid >= 0)
		return false;

	QString oname, email;
	if (! getRegistration(id, oname, email))
		return false;

	qhUserIDCache.remove(name);
	qhUserIDCache.remove(oname);
	qhUserNameCache.remove(id);

	int res = dbus->dbusSetName(id, name);
	if (res == 0)
		return false;

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("UPDATE %1players SET name=? WHERE server_id = ? AND player_id=?");
	query.addBindValue(name);
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();

	return true;
}

bool Server::setTexture(int id, const QByteArray &texture) {
	if (id <= 0)
		return false;

	QByteArray tex;
	if (texture.size() != 600 * 60 * 4)
		tex = qUncompress(texture);
	else
		tex = texture;

	if (tex.size() != 600 * 60 * 4)
		return false;

	tex = qCompress(tex);

	int res = dbus->dbusSetTexture(id, tex);
	if (res >= 0)
		return (res > 0);

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("UPDATE %1players SET texture=? WHERE server_id = ? AND player_id=?");
	query.addBindValue(tex, QSql::Binary | QSql::In);
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();

	return true;
}

void ServerDB::setSUPW(int srvnum, const QString &pw) {
	TransactionHolder th;

	QCryptographicHash hash(QCryptographicHash::Sha1);

	hash.addData(pw.toUtf8());

	QSqlQuery &query = *th.qsqQuery;
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
	QSqlQuery &query = *th.qsqQuery;
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

	QSqlQuery &query = *th.qsqQuery;
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

	QSqlQuery &query = *th.qsqQuery;
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

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("INSERT INTO %1channel_links (server_id, channel_id, link_id) VALUES (?,?,?)");
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

	QSqlQuery &query = *th.qsqQuery;

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

	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("SELECT MAX(channel_id)+1 AS id FROM %1channels WHERE server_id=?");
	query.addBindValue(iServerNum);
	SQLEXEC();
	int id = 0;
	if (query.next())
		id = query.value(0).toInt();


	SQLPREP("INSERT INTO %1channels (server_id, parent_id, channel_id, name) VALUES (?,?,?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(p->iId);
	query.addBindValue(id);
	query.addBindValue(name);
	SQLEXEC();
	Channel *c = new Channel(id, name, p);
	qhChannels.insert(id, c);
	return c;
}

void Server::removeChannel(const Channel *c) {
	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("DELETE FROM %1channels WHERE server_id = ? AND channel_id = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	SQLEXEC();
	qhChannels.remove(c->iId);
}

void Server::updateChannel(const Channel *c) {
	TransactionHolder th;
	Group *g;
	ChanACL *acl;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("UPDATE %1channels SET name = ?, parent_id = ?, inheritacl = ? WHERE server_id = ? AND channel_id = ?");
	query.addBindValue(c->qsName);
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
		SQLPREP("INSERT INTO %1groups (server_id, channel_id, name, inherit, inheritable) VALUES (?,?,?,?,?)");
		query.addBindValue(iServerNum);
		query.addBindValue(g->c->iId);
		query.addBindValue(g->qsName);
		query.addBindValue(g->bInherit ? 1 : 0);
		query.addBindValue(g->bInheritable ? 1 : 0);
		SQLEXEC();

		int id = query.lastInsertId().toInt();
		int pid;

		foreach(pid, g->qsAdd) {
			SQLPREP("INSERT INTO %1group_members (group_id, server_id, player_id, addit) VALUES (?, ?, ?, ?)");
			query.addBindValue(id);
			query.addBindValue(iServerNum);
			query.addBindValue(pid);
			query.addBindValue(1);
			SOFTEXEC();
		}
		foreach(pid, g->qsRemove) {
			SQLPREP("INSERT INTO %1group_members (group_id, server_id, player_id, addit) VALUES (?, ?, ?, ?)");
			query.addBindValue(id);
			query.addBindValue(iServerNum);
			query.addBindValue(pid);
			query.addBindValue(0);
			SOFTEXEC();
		}
	}

	int pri = 5;

	foreach(acl, c->qlACL) {
		SQLPREP("INSERT INTO %1acl (server_id, channel_id, priority, player_id, group_name, apply_here, apply_sub, grantpriv, revokepriv) VALUES (?,?,?,?,?,?,?,?,?)");
		query.addBindValue(iServerNum);
		query.addBindValue(acl->c->iId);
		query.addBindValue(pri++);

		query.addBindValue((acl->iPlayerId == -1) ? QVariant() : acl->iPlayerId);
		query.addBindValue((acl->qsGroup.isEmpty()) ? QVariant() : acl->qsGroup);
		query.addBindValue(acl->bApplyHere ? 1 : 0);
		query.addBindValue(acl->bApplySubs ? 1 : 0);
		query.addBindValue(static_cast<int>(acl->pAllow));
		query.addBindValue(static_cast<int>(acl->pDeny));
		SOFTEXEC();
	}
}

void Server::readChannelPrivs(Channel *c) {
	TransactionHolder th;

	int cid = c->iId;

	QSqlQuery &query = *th.qsqQuery;
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
		mem.prepare(QString::fromLatin1("SELECT player_id, addit FROM %1group_members WHERE group_id = ?").arg(Meta::mp.qsDBPrefix));
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
	
	query.clear();

	foreach(c, kids)
	readChannels(c);
}

void Server::readLinks() {
	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("SELECT channel_id, link_id FROM %1channel_links WHERE server_id = ?");
	query.addBindValue(iServerNum);
	SQLEXEC();

	while (query.next()) {
		int cid = query.value(0).toInt();
		int lid = query.value(1).toInt();

		Channel *c = qhChannels.value(cid);
		Channel *l = qhChannels.value(lid);
		if (c && l)
			c->link(l);
	}
}

void Server::setLastChannel(const Player *p) {

	if (p->iId < 0)
		return;

	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("UPDATE %1players SET lastchannel=? WHERE server_id = ? AND player_id = ?");
	query.addBindValue(p->cChannel->iId);
	query.addBindValue(iServerNum);
	query.addBindValue(p->iId);
	SOFTEXEC();
}

int Server::readLastChannel(int id) {
	if (id < 0)
		return 0;

	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("SELECT lastchannel FROM %1players WHERE server_id = ? AND player_id = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();

	if (query.next()) {
		int cid = query.value(0).toInt();
		if (qhChannels.contains(cid))
			return cid;
	}
	return 0;
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

	QSqlQuery &query = *th.qsqQuery;
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

	QSqlQuery &query = *th.qsqQuery;
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

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT value FROM %1config WHERE server_id = ? AND keystring = ?");
	query.addBindValue(server_id);
	query.addBindValue(key);
	SQLEXEC();
	if (query.next()) {
		return query.value(0);
	}
	return def;
}

QMap<QString, QString> ServerDB::getAllConf(int server_id) {
	TransactionHolder th;

	QMap<QString, QString> map;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT keystring, value FROM %1config WHERE server_id = ?");
	query.addBindValue(server_id);
	SQLEXEC();
	while (query.next()) {
		map.insert(query.value(0).toString(), query.value(1).toString());
	}
	return map;
}

void Server::setConf(const QString &key, const QVariant &value) {
	ServerDB::setConf(iServerNum, key, value);
}

void Server::dblog(const char *str) {
	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

	// Once per hour
	if (Meta::mp.iLogDays > 0) {
		if (ServerDB::tLogClean.isElapsed(3600ULL * 1000000ULL)) {
			QString qstr;
			if (Meta::mp.qsDBDriver == "QSQLITE") {
				qstr = QString::fromLatin1("msgtime < datetime('now','-%1 days')").arg(Meta::mp.iLogDays);
			} else {
				qstr = QString::fromLatin1("msgtime < now() - INTERVAL %1 day").arg(Meta::mp.iLogDays);
			}
			ServerDB::prepare(query, QString::fromLatin1("DELETE FROM %1slog WHERE ") + qstr);
			SQLEXEC();
		}
	}

	SQLPREP("INSERT INTO %1slog (server_id, msg) VALUES(?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(QLatin1String(str));
	SQLEXEC();
}

QList<QPair<unsigned int, QString> > ServerDB::getLog(int server_id, unsigned int sec_min, unsigned int sec_max) {
	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

	QString qstr;
	if (Meta::mp.qsDBDriver == "QSQLITE") {
		qstr = QString::fromLatin1("msgtime > datetime('now','-%1 seconds') AND msgtime < datetime('now','-%2 seconds')").arg(sec_max).arg(sec_min);
	} else {
		qstr = QString::fromLatin1("msgtime > now() - INTERVAL %1 second AND msgtime < now() - INTERVAL %2 second").arg(sec_max).arg(sec_min);
	}
	ServerDB::prepare(query, QString::fromLatin1("SELECT msgtime, msg FROM %1slog WHERE server_id = ? AND ") + qstr);
	query.addBindValue(server_id);
	SQLEXEC();

	QList<QPair<unsigned int, QString> > ql;
	while (query.next()) {
		QDateTime qdt = query.value(0).toDateTime();
		QString msg = query.value(1).toString();
		ql << QPair<unsigned int, QString>(qdt.toTime_t(), msg);
	}
	return ql;
}

void ServerDB::setConf(int server_id, const QString &key, const QVariant &value) {
	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	if (value.isNull() || value.toString().trimmed().isEmpty()) {
		SQLPREP("DELETE FROM %1config WHERE server_id = ? AND keystring = ?");
		query.addBindValue(server_id);
		query.addBindValue(key);
	} else {
		SQLPREP("REPLACE INTO %1config (server_id, keystring, value) VALUES (?,?,?)");
		query.addBindValue(server_id);
		query.addBindValue(key);
		query.addBindValue(value.toString());
	}
	SQLEXEC();
}


QList<int> ServerDB::getAllServers() {
	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;
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
	QSqlQuery &query = *th.qsqQuery;

	QList<int> bootlist;
	foreach(int i, ql) {
		SQLPREP("SELECT value FROM %1config WHERE server_id = ? AND keystring = ?");
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
	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT server_id FROM %1servers WHERE server_id = ?");
	query.addBindValue(num);
	SQLEXEC();
	if (query.next())
		return true;
	return false;
}

int ServerDB::addServer() {
	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;
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

void ServerDB::deleteServer(int server_id) {
	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("DELETE FROM %1servers WHERE server_id = ?");
	query.addBindValue(server_id);
	SQLEXEC();
}
