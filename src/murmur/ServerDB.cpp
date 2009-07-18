/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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
#include "User.h"
#include "Channel.h"
#include "Group.h"
#include "ACL.h"
#include "Server.h"
#include "Meta.h"
#include "Connection.h"
#include "DBus.h"

#define SQLDO(x) ServerDB::exec(query, QLatin1String(x), true)
#define SQLMAY(x) ServerDB::exec(query, QLatin1String(x), false, false)
#define SQLPREP(x) ServerDB::prepare(query, QLatin1String(x))
#define SQLEXEC() ServerDB::exec(query)
#define SQLEXECBATCH() ServerDB::execBatch(query)
#define SOFTEXEC() ServerDB::exec(query, QString(), false)

class TransactionHolder {
	public:
		QSqlQuery *qsqQuery;
		TransactionHolder() {
			ServerDB::db->transaction();
			qsqQuery = new QSqlQuery();
		}

		~TransactionHolder() {
			qsqQuery->clear();
			delete qsqQuery;
			ServerDB::db->commit();
		}
};

QSqlDatabase *ServerDB::db;
Timer ServerDB::tLogClean;

ServerDB::ServerDB() {
	if (! QSqlDatabase::isDriverAvailable(Meta::mp.qsDBDriver)) {
		qFatal("ServerDB: Database driver %s not available", qPrintable(Meta::mp.qsDBDriver));
	}
	db = new QSqlDatabase(QSqlDatabase::addDatabase(Meta::mp.qsDBDriver));
	QStringList datapaths;
	int i;

	bool found = false;

	if (Meta::mp.qsDBDriver == "QSQLITE") {
		if (! Meta::mp.qsDatabase.isEmpty()) {
			db->setDatabaseName(Meta::mp.qsDatabase);
			found = db->open();
		} else {
			datapaths << Meta::mp.qdBasePath.absolutePath();
			datapaths << QDir::currentPath();
			datapaths << QCoreApplication::instance()->applicationDirPath();
			datapaths << QDir::homePath();

			for (i = 0; (i < datapaths.size()) && ! found; i++) {
				if (!datapaths[i].isEmpty()) {
					QFile f(datapaths[i] + "/murmur.sqlite");
					if (f.exists()) {
						db->setDatabaseName(f.fileName());
						found = db->open();
					}
				}
			}

			if (! found) {
				for (i = 0; (i < datapaths.size()) && ! found; i++) {
					if (!datapaths[i].isEmpty()) {
						QFile f(datapaths[i] + "/murmur.sqlite");
						db->setDatabaseName(f.fileName());
						found = db->open();
					}
				}
			}
		}
		if (found) {
			QFileInfo fi(db->databaseName());
			qWarning("ServerDB: Openend SQLite database %s", qPrintable(fi.absoluteFilePath()));
			if (! fi.isWritable())
				qFatal("ServerDB: Database is not writeable");
		}
	} else {
		db->setDatabaseName(Meta::mp.qsDatabase);
		db->setHostName(Meta::mp.qsDBHostName);
		db->setPort(Meta::mp.iDBPort);
		db->setUserName(Meta::mp.qsDBServerUserName);
		db->setPassword(Meta::mp.qsDBPassword);
		db->setConnectOptions(Meta::mp.qsDBOpts);
		found = db->open();
	}

	if (! found) {
		QSqlError e = db->lastError();
		qFatal("ServerDB: Failed initialization: %s",qPrintable(e.text()));
	}

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;

	int version = 0;

	if (Meta::mp.qsDBDriver == "QSQLITE")
		SQLDO("CREATE TABLE IF NOT EXISTS `%1meta` (`keystring` TEXT PRIMARY KEY, `value` TEXT)");
	else
		SQLDO("CREATE TABLE IF NOT EXISTS `%1meta`(`keystring` varchar(255) PRIMARY KEY, `value` varchar(255)) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");

	SQLDO("SELECT `value` FROM `%1meta` WHERE `keystring` = 'version'");
	if (query.next())
		version = query.value(0).toInt();

	if (version < 4) {
		if (version > 0) {
			qWarning("Renaming old tables...");
			SQLDO("ALTER TABLE `%1servers` RENAME TO `%1servers_old`");
			if (version < 2)
				SQLMAY("ALTER TABLE `%1log` RENAME TO `%1slog`");
			SQLDO("ALTER TABLE `%1slog` RENAME TO `%1slog_old`");
			SQLDO("ALTER TABLE `%1config` RENAME TO `%1config_old`");
			SQLDO("ALTER TABLE `%1channels` RENAME TO `%1channels_old`");
			SQLDO("ALTER TABLE `%1players` RENAME TO `%1players_old`");
			SQLDO("ALTER TABLE `%1groups` RENAME TO `%1groups_old`");
			SQLDO("ALTER TABLE `%1group_members` RENAME TO `%1group_members_old`");
			SQLDO("ALTER TABLE `%1acl` RENAME TO `%1acl_old`");
			SQLDO("ALTER TABLE `%1channel_links` RENAME TO `%1channel_links_old`");
			SQLDO("ALTER TABLE `%1bans` RENAME TO `%1bans_old`");
		}

		qWarning("Generating new tables...");
		if (Meta::mp.qsDBDriver == "QSQLITE") {
			if (version > 0) {
				SQLDO("DROP TRIGGER IF EXISTS `%1log_timestamp`");
				SQLDO("DROP TRIGGER IF EXISTS `%1log_server_del`");
				SQLDO("DROP TRIGGER IF EXISTS `%1slog_timestamp`");
				SQLDO("DROP TRIGGER IF EXISTS `%1slog_server_del`");
				SQLDO("DROP TRIGGER IF EXISTS `%1config_server_del`");
				SQLDO("DROP TRIGGER IF EXISTS `%1channels_parent_del`");
				SQLDO("DROP TRIGGER IF EXISTS `%1channels_server_del`");
				SQLDO("DROP TRIGGER IF EXISTS `%1players_server_del`");
				SQLDO("DROP TRIGGER IF EXISTS `%1players_update_timestamp`");
				SQLDO("DROP TRIGGER IF EXISTS `%1groups_del_channel`");
				SQLDO("DROP TRIGGER IF EXISTS `%1groups_members_del_group`");
				SQLDO("DROP TRIGGER IF EXISTS `%1groups_members_del_player`");
				SQLDO("DROP TRIGGER IF EXISTS `%1acl_del_channel`");
				SQLDO("DROP TRIGGER IF EXISTS `%1acl_del_player`");
				SQLDO("DROP TRIGGER IF EXISTS `%1channel_links_del_channel`");
				SQLDO("DROP TRIGGER IF EXISTS `%1bans_del_server`");

				SQLDO("DROP INDEX IF EXISTS `%1log_time`");
				SQLDO("DROP INDEX IF EXISTS `%1slog_time`");
				SQLDO("DROP INDEX IF EXISTS `%1config_key`");
				SQLDO("DROP INDEX IF EXISTS `%1channel_id`");
				SQLDO("DROP INDEX IF EXISTS `%1channel_info_id`");
				SQLDO("DROP INDEX IF EXISTS `%1players_name`");
				SQLDO("DROP INDEX IF EXISTS `%1players_id`");
				SQLDO("DROP INDEX IF EXISTS `%1groups_name_channels`");
				SQLDO("DROP INDEX IF EXISTS `%1acl_channel_pri`");
			}

			SQLDO("CREATE TABLE `%1servers` (`server_id` INTEGER PRIMARY KEY AUTOINCREMENT)");

			SQLDO("CREATE TABLE `%1slog`(`server_id` INTEGER NOT NULL, `msg` TEXT, `msgtime` DATE)");
			SQLDO("CREATE INDEX `%1slog_time` ON `%1slog`(`msgtime`)");
			SQLDO("CREATE TRIGGER `%1slog_timestamp` AFTER INSERT ON `%1slog` FOR EACH ROW BEGIN UPDATE `%1slog` SET `msgtime` = datetime('now') WHERE rowid = new.rowid; END;");
			SQLDO("CREATE TRIGGER `%1slog_server_del` AFTER DELETE ON `%1servers` FOR EACH ROW BEGIN DELETE FROM `%1slog` WHERE `server_id` = old.`server_id`; END;");

			SQLDO("CREATE TABLE `%1config` (`server_id` INTEGER NOT NULL, `key` TEXT, `value` TEXT)");
			SQLDO("CREATE UNIQUE INDEX `%1config_key` ON `%1config`(`server_id`, `key`)");
			SQLDO("CREATE TRIGGER `%1config_server_del` AFTER DELETE ON `%1servers` FOR EACH ROW BEGIN DELETE FROM `%1config` WHERE `server_id` = old.`server_id`; END;");

			SQLDO("CREATE TABLE `%1channels` (`server_id` INTEGER NOT NULL, `channel_id` INTEGER NOT NULL, `parent_id` INTEGER, `name` TEXT, `inheritacl` INTEGER)");
			SQLDO("CREATE UNIQUE INDEX `%1channel_id` ON `%1channels`(`server_id`, `channel_id`)");
			SQLDO("CREATE TRIGGER `%1channels_parent_del` AFTER DELETE ON `%1channels` FOR EACH ROW BEGIN DELETE FROM `%1channels` WHERE `parent_id` = old.`channel_id` AND `server_id` = old.`server_id`; UPDATE `%1users` SET `lastchannel`=0 WHERE `lastchannel` = old.`channel_id` AND `server_id` = old.`server_id`; END;");
			SQLDO("CREATE TRIGGER `%1channels_server_del` AFTER DELETE ON `%1servers` FOR EACH ROW BEGIN DELETE FROM `%1channels` WHERE `server_id` = old.`server_id`; END;");

			SQLDO("CREATE TABLE `%1channel_info` (`server_id` INTEGER NOT NULL, `channel_id` INTEGER NOT NULL, `key` TEXT, `value` TEXT)");
			SQLDO("CREATE UNIQUE INDEX `%1channel_info_id` ON `%1channel_info`(`server_id`, `channel_id`, `key`)");
			SQLDO("CREATE TRIGGER `%1channel_info_del_channel` AFTER DELETE on `%1channels` FOR EACH ROW BEGIN DELETE FROM `%1channel_info` WHERE `channel_id` = old.`channel_id` AND `server_id` = old.`server_id`; END;");

			SQLDO("CREATE TABLE `%1users` (`server_id` INTEGER NOT NULL, `user_id` INTEGER NOT NULL, `name` TEXT NOT NULL, `pw` TEXT, `lastchannel` INTEGER, `texture` BLOB, `last_active` DATE)");
			SQLDO("CREATE UNIQUE INDEX `%1users_name` ON `%1users` (`server_id`,`name`)");
			SQLDO("CREATE UNIQUE INDEX `%1users_id` ON `%1users` (`server_id`, `user_id`)");
			SQLDO("CREATE TRIGGER `%1users_server_del` AFTER DELETE ON `%1servers` FOR EACH ROW BEGIN DELETE FROM `%1users` WHERE `server_id` = old.`server_id`; END;");
			SQLDO("CREATE TRIGGER `%1users_update_timestamp` AFTER UPDATE OF `lastchannel` ON `%1users` FOR EACH ROW BEGIN UPDATE `%1users` SET `last_active` = datetime('now') WHERE `user_id` = old.`user_id` AND `server_id` = old.`server_id`; END;");

			SQLDO("CREATE TABLE `%1user_info` (`server_id` INTEGER NOT NULL, `user_id` INTEGER NOT NULL, `key` TEXT, `value` TEXT)");
			SQLDO("CREATE UNIQUE INDEX `%1user_info_id` ON `%1user_info`(`server_id`, `user_id`, `key`)");
			SQLDO("CREATE TRIGGER `%1user_info_del_user` AFTER DELETE on `%1users` FOR EACH ROW BEGIN DELETE FROM `%1user_info` WHERE `user_id` = old.`user_id` AND `server_id` = old.`server_id`; END;");

			SQLDO("CREATE TABLE `%1groups` (`group_id` INTEGER PRIMARY KEY AUTOINCREMENT, `server_id` INTEGER NOT NULL, `name` TEXT, `channel_id` INTEGER NOT NULL, `inherit` INTEGER, `inheritable` INTEGER)");
			SQLDO("CREATE UNIQUE INDEX `%1groups_name_channels` ON `%1groups`(`server_id`, `channel_id`, `name`)");
			SQLDO("CREATE TRIGGER `%1groups_del_channel` AFTER DELETE ON `%1channels` FOR EACH ROW BEGIN DELETE FROM `%1groups` WHERE `channel_id` = old.`channel_id` AND `server_id` = old.`server_id`; END;");

			SQLDO("CREATE TABLE `%1group_members` (`group_id` INTEGER NOT NULL, `server_id` INTEGER NOT NULL, `user_id` INTEGER NOT NULL, `addit` INTEGER)");
			SQLDO("CREATE TRIGGER `%1groups_members_del_group` AFTER DELETE ON `%1groups` FOR EACH ROW BEGIN DELETE FROM `%1group_members` WHERE `group_id` = old.`group_id`; END;");
			SQLDO("CREATE TRIGGER `%1groups_members_del_user` AFTER DELETE on `%1users` FOR EACH ROW BEGIN DELETE FROM `%1group_members` WHERE `user_id` = old.`user_id` AND `server_id` = old.`server_id`; END;");

			SQLDO("CREATE TABLE `%1acl` (`server_id` INTEGER NOT NULL, `channel_id` INTEGER NOT NULL, `priority` INTEGER, `user_id` INTEGER, `group_name` TEXT, `apply_here` INTEGER, `apply_sub` INTEGER, `grantpriv` INTEGER, `revokepriv` INTEGER)");
			SQLDO("CREATE UNIQUE INDEX `%1acl_channel_pri` ON `%1acl`(`server_id`, `channel_id`, `priority`)");
			SQLDO("CREATE TRIGGER `%1acl_del_channel` AFTER DELETE ON `%1channels` FOR EACH ROW BEGIN DELETE FROM `%1acl` WHERE `channel_id` = old.`channel_id` AND `server_id` = old.`server_id`; END;");
			SQLDO("CREATE TRIGGER `%1acl_del_user` AFTER DELETE ON `%1users` FOR EACH ROW BEGIN DELETE FROM `%1acl` WHERE `user_id` = old.`user_id` AND `server_id` = old.`server_id`; END;");

			SQLDO("CREATE TABLE `%1channel_links` (`server_id` INTEGER NOT NULL, `channel_id` INTEGER NOT NULL, `link_id` INTEGER NOT NULL)");
			SQLDO("CREATE TRIGGER `%1channel_links_del_channel` AFTER DELETE ON `%1channels` FOR EACH ROW BEGIN DELETE FROM `%1channel_links` WHERE `server_id` = old.`server_id` AND (`channel_id` = old.`channel_id` OR `link_id` = old.`channel_id`); END;");
			SQLDO("DELETE FROM `%1channel_links`");

			SQLDO("CREATE TABLE `%1bans` (`server_id` INTEGER NOT NULL, `base` BLOB, `mask` INTEGER, `name` TEXT, `hash` TEXT, `reason` TEXT, `start` DATE, `duration` INTEGER)");
			SQLDO("CREATE TRIGGER `%1bans_del_server` AFTER DELETE ON `%1servers` FOR EACH ROW BEGIN DELETE FROM `%1bans` WHERE `server_id` = old.`server_id`; END;");
		} else {
			SQLDO("CREATE TABLE `%1servers`(`server_id` INTEGER PRIMARY KEY AUTO_INCREMENT) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");

			SQLDO("CREATE TABLE `%1slog`(`server_id` INTEGER NOT NULL, `msg` TEXT, `msgtime` TIMESTAMP) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE INDEX `%1slog_time` ON `%1slog`(`msgtime`)");
			SQLDO("ALTER TABLE `%1slog` ADD CONSTRAINT `%1slog_server_del` FOREIGN KEY (`server_id`) REFERENCES `%1servers`(`server_id`) ON DELETE CASCADE");

			SQLDO("CREATE TABLE `%1config` (`server_id` INTEGER NOT NULL, `key` varchar(255), `value` TEXT) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE UNIQUE INDEX `%1config_key` ON `%1config`(`server_id`, `key`)");
			SQLDO("ALTER TABLE `%1config` ADD CONSTRAINT `%1config_server_del` FOREIGN KEY (`server_id`) REFERENCES `%1servers`(`server_id`) ON DELETE CASCADE");

			SQLDO("CREATE TABLE `%1channels` (`server_id` INTEGER NOT NULL, `channel_id` INTEGER NOT NULL, `parent_id` INTEGER, `name` varchar(255), `inheritacl` INTEGER) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE UNIQUE INDEX `%1channel_id` ON `%1channels`(`server_id`, `channel_id`)");
			SQLDO("ALTER TABLE `%1channels` ADD CONSTRAINT `%1channels_parent_del` FOREIGN KEY (`server_id`, `parent_id`) REFERENCES `%1channels`(`server_id`,`channel_id`) ON DELETE CASCADE");
			SQLDO("ALTER TABLE `%1channels` ADD CONSTRAINT `%1channels_server_del` FOREIGN KEY (`server_id`) REFERENCES `%1servers`(`server_id`) ON DELETE CASCADE");

			SQLDO("CREATE TABLE `%1channel_info` (`server_id` INTEGER NOT NULL, `channel_id` INTEGER NOT NULL, `key` varchar(255), `value` varchar(255))");
			SQLDO("CREATE UNIQUE INDEX `%1channel_info_id` ON `%1channel_info`(`server_id`, `channel_id`, `key`)");
			SQLDO("ALTER TABLE `%1channel_info` ADD CONSTRAINT `%1channel_info_del_channel` FOREIGN KEY (`server_id`, `channel_id`) REFERENCES `%1channels`(`server_id`,`channel_id`) ON DELETE CASCADE");

			SQLDO("CREATE TABLE `%1users` (`server_id` INTEGER NOT NULL, `user_id` INTEGER NOT NULL, `name` varchar(255), `pw` varchar(128), `lastchannel` INTEGER, `texture` LONGBLOB, `last_active` TIMESTAMP) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE INDEX `%1users_channel` ON `%1users`(`server_id`, `lastchannel`)");
			SQLDO("CREATE UNIQUE INDEX `%1users_name` ON `%1users` (`server_id`,`name`)");
			SQLDO("CREATE UNIQUE INDEX `%1users_id` ON `%1users` (`server_id`, `user_id`)");
			SQLDO("ALTER TABLE `%1users` ADD CONSTRAINT `%1users_server_del` FOREIGN KEY (`server_id`) REFERENCES `%1servers`(`server_id`) ON DELETE CASCADE");

			SQLDO("CREATE TABLE `%1user_info` (`server_id` INTEGER NOT NULL, `user_id` INTEGER NOT NULL, `key` varchar(255), `value` varchar(255))");
			SQLDO("CREATE UNIQUE INDEX `%1user_info_id` ON `%1user_info`(`server_id`, `user_id`, `key`)");
			SQLDO("ALTER TABLE `%1user_info` ADD CONSTRAINT `%1user_info_del_user` FOREIGN KEY (`server_id`, `user_id`) REFERENCES `%1users`(`server_id`,`user_id`) ON DELETE CASCADE");

			SQLDO("CREATE TABLE `%1groups` (`group_id` INTEGER PRIMARY KEY AUTO_INCREMENT, `server_id` INTEGER NOT NULL, `name` varchar(255), `channel_id` INTEGER NOT NULL, `inherit` INTEGER, `inheritable` INTEGER) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE UNIQUE INDEX `%1groups_name_channels` ON `%1groups`(`server_id`, `channel_id`, `name`)");
			SQLDO("ALTER TABLE `%1groups` ADD CONSTRAINT `%1groups_del_channel` FOREIGN KEY (`server_id`, `channel_id`) REFERENCES `%1channels`(`server_id`, `channel_id`) ON DELETE CASCADE");

			SQLDO("CREATE TABLE `%1group_members` (`group_id` INTEGER NOT NULL, `server_id` INTEGER NOT NULL, `user_id` INTEGER NOT NULL, `addit` INTEGER) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE INDEX `%1group_members_users` ON `%1group_members`(`server_id`, `user_id`)");
			SQLDO("ALTER TABLE `%1group_members` ADD CONSTRAINT `%1group_members_del_group` FOREIGN KEY (`group_id`) REFERENCES `%1groups`(`group_id`) ON DELETE CASCADE");
			SQLDO("ALTER TABLE `%1group_members` ADD CONSTRAINT `%1group_members_del_user` FOREIGN KEY (`server_id`, `user_id`) REFERENCES `%1users`(`server_id`,`user_id`) ON DELETE CASCADE");

			SQLDO("CREATE TABLE `%1acl` (`server_id` INTEGER NOT NULL, `channel_id` INTEGER NOT NULL, `priority` INTEGER, `user_id` INTEGER, `group_name` varchar(255), `apply_here` INTEGER, `apply_sub` INTEGER, `grantpriv` INTEGER, `revokepriv` INTEGER) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("CREATE UNIQUE INDEX `%1acl_channel_pri` ON `%1acl`(`server_id`, `channel_id`, `priority`)");
			SQLDO("CREATE INDEX `%1acl_user` ON `%1acl`(`server_id`, `user_id`)");
			SQLDO("ALTER TABLE `%1acl` ADD CONSTRAINT `%1acl_del_channel` FOREIGN KEY (`server_id`, `channel_id`) REFERENCES `%1channels`(`server_id`, `channel_id`) ON DELETE CASCADE");
			SQLDO("ALTER TABLE `%1acl` ADD CONSTRAINT `%1acl_del_user` FOREIGN KEY (`server_id`, `user_id`) REFERENCES `%1users`(`server_id`, `user_id`) ON DELETE CASCADE");

			SQLDO("CREATE TABLE `%1channel_links` (`server_id` INTEGER NOT NULL, `channel_id` INTEGER NOT NULL, `link_id` INTEGER NOT NULL) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("ALTER TABLE `%1channel_links` ADD CONSTRAINT `%1channel_links_del_channel` FOREIGN KEY(`server_id`, `channel_id`) REFERENCES `%1channels`(`server_id`, `channel_id`) ON DELETE CASCADE");
			SQLDO("DELETE FROM `%1channel_links`");

			SQLDO("CREATE TABLE `%1bans` (`server_id` INTEGER NOT NULL, `base` BINARY(16), `mask` INTEGER, `name` varchar(255), `hash` CHAR(40), `reason` TEXT, `start` DATETIME, `duration` INTEGER) Type=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci");
			SQLDO("ALTER TABLE `%1bans` ADD CONSTRAINT `%1bans_del_server` FOREIGN KEY(`server_id`) REFERENCES `%1servers`(`server_id`) ON DELETE CASCADE");
		}
		if (version == 0) {
			SQLDO("INSERT INTO `%1servers` (`server_id`) VALUES(1)");
			SQLDO("INSERT INTO `%1meta` (`keystring`, `value`) VALUES('version','4')");
		} else {
			qWarning("Importing old data...");

			SQLDO("INSERT INTO `%1servers` (`server_id`) SELECT `server_id` FROM `%1servers_old`");
			SQLDO("INSERT INTO `%1slog` (`server_id`, `msg`, `msgtime`) SELECT `server_id`, `msg`, `msgtime` FROM `%1slog_old`");
			SQLDO("INSERT INTO `%1config` (`server_id`, `key`, `value`) SELECT `server_id`, `keystring`, `value` FROM `%1config_old`");
			SQLDO("INSERT INTO `%1channels` (`server_id`, `channel_id`, `parent_id`, `name`, `inheritacl`) SELECT `server_id`, `channel_id`, `parent_id`, `name`, `inheritacl` FROM `%1channels_old`");
			SQLDO("INSERT INTO `%1users` (`server_id`, `user_id`, `name`, `pw`, `lastchannel`, `texture`, `last_active`) SELECT `server_id`, `player_id`, `name`, `pw`, `lastchannel`, `texture`, `last_active` FROM `%1players_old`");
			SQLDO("INSERT INTO `%1groups` (`group_id`, `server_id`, `name`, `channel_id`, `inherit`, `inheritable`) SELECT `group_id`, `server_id`, `name`, `channel_id`, `inherit`, `inheritable` FROM `%1groups_old`");
			SQLDO("INSERT INTO `%1group_members` (`group_id`, `server_id`, `user_id`, `addit`) SELECT `group_id`, `server_id`, `player_id`, `addit` FROM `%1group_members_old`");
			SQLDO("INSERT INTO `%1acl` (`server_id`, `channel_id`, `priority`, `user_id`, `group_name`, `apply_here`, `apply_sub`, `grantpriv`, `revokepriv`) SELECT `server_id`, `channel_id`, `priority`, `player_id`, `group_name`, `apply_here`, `apply_sub`, `grantpriv`, `revokepriv` FROM `%1acl_old`");
			SQLDO("INSERT INTO `%1channel_links` (`server_id`, `channel_id`, `link_id`) SELECT `server_id`, `channel_id`, `link_id` FROM `%1channel_links_old`");

			QList<QList<QVariant> > ql;
			SQLPREP("SELECT `server_id`, `base`, `mask` FROM `%1bans_old`");
			SQLEXEC();
			while (query.next()) {
				QList<QVariant> l;
				l << query.value(0);
				l << query.value(1);
				l << query.value(2);
				ql << l;
			}
			SQLPREP("INSERT INTO `%1bans` (`server_id`, `base`, `mask`) VALUES (?, ?, ?)");
			foreach(const QList<QVariant> &l, ql) {

				quint32 addr = htonl(l.at(1).toUInt());
				const char *ptr = reinterpret_cast<const char *>(&addr);

				QByteArray qba(16, 0);
				qba[10] = static_cast<char>(-1);
				qba[11] = static_cast<char>(-1);
				qba[12] = ptr[0];
				qba[13] = ptr[1];
				qba[14] = ptr[2];
				qba[15] = ptr[3];

				query.addBindValue(l.at(0));
				query.addBindValue(qba);
				query.addBindValue(l.at(2).toInt() + 96);
				SQLEXEC();
			}

			SQLDO("INSERT INTO `%1user_info` SELECT `server_id`,`player_id`,'email',email FROM `%1players_old` WHERE `email` IS NOT NULL");

			if (version == 3) {
				SQLDO("INSERT INTO `%1channel_info` SELECT `server_id`,`channel_id`,'description',`description` FROM `%1channels_old` WHERE `description` IS NOT NULL");
			}

			qWarning("Removing old tables...");
			SQLDO("DROP TABLE IF EXISTS `%1slog_old`");
			SQLDO("DROP TABLE IF EXISTS `%1config_old`");
			SQLDO("DROP TABLE IF EXISTS `%1channels_old`");
			SQLDO("DROP TABLE IF EXISTS `%1players_old`");
			SQLDO("DROP TABLE IF EXISTS `%1groups_old`");
			SQLDO("DROP TABLE IF EXISTS `%1group_members_old`");
			SQLDO("DROP TABLE IF EXISTS `%1acl_old`");
			SQLDO("DROP TABLE IF EXISTS `%1channel_links_old`");
			SQLDO("DROP TABLE IF EXISTS `%1bans_old`");

			SQLDO("UPDATE `%1meta` SET `value` = '4' WHERE `keystring` = 'version'");
		}
	}
	query.clear();
}

ServerDB::~ServerDB() {
	db->close();
	delete db;
	db = NULL;
}

bool ServerDB::prepare(QSqlQuery &query, const QString &str, bool fatal, bool warn) {
	if (! db->isValid()) {
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
		db->close();
		if (! db->open()) {
			qFatal("Lost connection to SQL Database: Reconnect: %s", qPrintable(db->lastError().text()));
		}
		query = QSqlQuery();
		if (query.prepare(q)) {
			qWarning("SQL Connection lost, reconnection OK");
			return true;
		}

		if (fatal) {
			*db = QSqlDatabase();
			qFatal("SQL Prepare Error [%s]: %s", qPrintable(q), qPrintable(query.lastError().text()));
		} else if (warn) {
			qDebug("SQL Prepare Error [%s]: %s", qPrintable(q), qPrintable(query.lastError().text()));
		}
		return false;
	}
}

bool ServerDB::exec(QSqlQuery &query, const QString &str, bool fatal, bool warn) {
	if (! str.isEmpty())
		prepare(query, str, fatal, warn);
	if (query.exec()) {
		return true;
	} else {

		if (fatal) {
			*db = QSqlDatabase();
			qFatal("SQL Error [%s]: %s", qPrintable(query.lastQuery()), qPrintable(query.lastError().text()));
		} else if (warn) {
			qDebug("SQL Error [%s]: %s", qPrintable(query.lastQuery()), qPrintable(query.lastError().text()));
		}
		return false;
	}
}

bool ServerDB::execBatch(QSqlQuery &query, const QString &str, bool fatal) {
	if (! str.isEmpty())
		prepare(query, str, fatal);
	if (query.execBatch()) {
		return true;
	} else {

		if (fatal) {
			*db = QSqlDatabase();
			qFatal("SQL Error [%s]: %s", qPrintable(query.lastQuery()), qPrintable(query.lastError().text()));
		} else
			qDebug("SQL Error [%s]: %s", qPrintable(query.lastQuery()), qPrintable(query.lastError().text()));
		return false;
	}
}

void Server::initialize() {
	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("SELECT `channel_id` FROM `%1channels` WHERE `server_id` = ? AND `channel_id` = 0");
	query.addBindValue(iServerNum);
	SQLEXEC();
	if (! query.next()) {
		SQLPREP("INSERT INTO `%1channels` (`server_id`, `channel_id`, `parent_id`, `name`) VALUES (?, ?, ?, ?)");
		query.addBindValue(iServerNum);
		query.addBindValue(0);
		query.addBindValue(QVariant());
		query.addBindValue(QLatin1String("Root"));
		SQLEXEC();
		SQLPREP("UPDATE `%1channels` SET `channel_id` = 0 WHERE `server_id` = ? AND `name` = ? AND `parent_id` IS NULL");
		query.addBindValue(iServerNum);
		query.addBindValue(QLatin1String("Root"));
		SQLEXEC();
	}

	SQLPREP("SELECT `user_id` FROM `%1users` WHERE `server_id` = ? AND `user_id` = 0");
	query.addBindValue(iServerNum);
	SQLEXEC();
	if (! query.next()) {
		SQLPREP("INSERT INTO `%1users` (`server_id`, `user_id`, `name`) VALUES (?, ?, ?)");
		query.addBindValue(iServerNum);
		query.addBindValue(0);
		query.addBindValue(QLatin1String("SuperUser"));
		SQLEXEC();
		SQLPREP("UPDATE `%1users` SET `user_id` = 0 WHERE `server_id` = ? AND `name` = ?");
		query.addBindValue(iServerNum);
		query.addBindValue(QLatin1String("SuperUser"));
		SQLEXEC();
	}

	SQLPREP("SELECT COUNT(*) FROM `%1acl` WHERE `server_id`=?");
	query.addBindValue(iServerNum);
	SQLEXEC();
	if (query.next()) {
		int c = query.value(0).toInt();
		if (c == 0) {
			SQLPREP("INSERT INTO `%1acl` (`server_id`, `channel_id`, `priority`, `group_name`, `apply_here`, `apply_sub`, `grantpriv`) VALUES (?,?,?,?,?,?,?)");
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

	SQLPREP("SELECT COUNT(*) FROM `%1groups` WHERE `server_id`=? AND `channel_id`=?");
	query.addBindValue(iServerNum);
	query.addBindValue(0);
	SQLEXEC();
	if (query.next()) {
		int c = query.value(0).toInt();
		if (c == 0) {
			SQLPREP("INSERT INTO `%1groups`(`server_id`, `channel_id`, `name`, `inherit`, `inheritable`) VALUES (?,?,?,?,?)");
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

int Server::registerUser(const QMap<QString, QString> &info) {
	const QString &name = info.value("name");

	if (name.isEmpty())
		return -1;

	if (! validateUserName(name))
		return -1;

	if (getUserID(name) >= 0)
		return -1;

	qhUserIDCache.remove(name);

	int res = -2;
	emit registerUserSig(res, info);
	if (res != -2) {
		qhUserIDCache.remove(name);
	}
	if (res == -1)
		return res;

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	int id = 0;

	if (res < 0) {
		SQLPREP("SELECT MAX(`user_id`)+1 AS id FROM `%1users` WHERE `server_id`=? AND `user_id` < 1000000000");
		query.addBindValue(iServerNum);
		SQLEXEC();
		if (query.next())
			id = query.value(0).toInt();
	} else {
		id = res;
	}

	SQLPREP("REPLACE INTO `%1users` (`server_id`, `user_id`, `name`) VALUES (?,?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	query.addBindValue(name);
	SQLEXEC();
	qhUserNameCache.remove(id);

	setInfo(id, info);

	return id;
}

bool Server::unregisterUser(int id) {
	if (id <= 0)
		return false;

	QMap<QString, QString> info = getRegistration(id);

	if (info.isEmpty())
		return false;

	qhUserIDCache.remove(info.value("name"));
	qhUserNameCache.remove(id);

	int res = -2;
	emit unregisterUserSig(res, id);
	if (res == 0) {
		return false;
	}

	{
		QMutexLocker lock(&qmCache);

		foreach(Channel *c, qhChannels) {
			bool write = false;
			QList<ChanACL *> ql = c->qlACL;

			foreach(ChanACL *acl, ql) {
				if (acl->iUserId == id) {
					c->qlACL.removeAll(acl);
					write = true;
				}
			}
			foreach(Group *g, c->qhGroups) {
				bool addrem = g->qsAdd.remove(id);
				bool remrem = g->qsRemove.remove(id);
				write = write || addrem || remrem;
			}
			if (write)
				updateChannel(c);
		};
	}


	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("DELETE FROM `%1users` WHERE `server_id` = ? AND `user_id` = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();

	return true;
}

QMap<int, QString > Server::getRegisteredUsers(const QString &filter) {
	QMap<int, QString > m;

	emit getRegisteredUsersSig(filter, m);

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	if (filter.isEmpty()) {
		SQLPREP("SELECT `user_id`, `name` FROM `%1users` WHERE `server_id` = ?");
		query.addBindValue(iServerNum);
	} else {
		SQLPREP("SELECT `user_id`, `name` FROM `%1users` WHERE `server_id` = ? AND `name` LIKE ?");
		query.addBindValue(iServerNum);
		query.addBindValue(filter);
	}
	SQLEXEC();

	while (query.next()) {
		int id = query.value(0).toInt();
		QString name = query.value(1).toString();
		m.insert(id, name);
	}
	return m;
}

bool Server::isUserId(int id) {
	QMap<QString, QString> info;
	int res = -2;
	emit getRegistrationSig(res, id, info);
	if (res >= 0)
		return (res > 0);

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT `user_id` FROM `%1users` WHERE `server_id` = ? AND `user_id` = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();
	if (query.next()) {
		return true;
	}
	return false;
}

QMap<QString, QString> Server::getRegistration(int id) {
	QMap<QString, QString> info;
	int res = -2;
	emit getRegistrationSig(res, id, info);
	if (res >= 0)
		return info;

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT `name` FROM `%1users` WHERE `server_id` = ? AND `user_id` = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();
	if (query.next()) {
		info.insert("name", query.value(0).toString());

		SQLPREP("SELECT `key`, `value` FROM `%1user_info` WHERE `server_id` = ? AND `user_id` = ?");
		query.addBindValue(iServerNum);
		query.addBindValue(id);
		SQLEXEC();
		while (query.next()) {
			info.insert(query.value(0).toString(), query.value(1).toString());
		}
	}
	return info;
}

// -1 Wrong PW
// -2 Anonymous

int Server::authenticate(QString &name, const QString &pw, const QStringList &emails, const QString &certhash, bool bStrongCert) {
	int res = -2;

	emit authenticateSig(res, name, pw);

	if (res != -2) {
		// External authentication handled it. Ignore certificate completely.
		if (res != -1) {
			TransactionHolder th;
			QSqlQuery &query = *th.qsqQuery;

			int lchan=readLastChannel(res);

			SQLPREP("REPLACE INTO `%1users` (`server_id`, `user_id`, `name`, `lastchannel`) VALUES (?,?,?,?)");
			query.addBindValue(iServerNum);
			query.addBindValue(res);
			query.addBindValue(name);
			query.addBindValue(lchan);
			SQLEXEC();
		}
		if (res >= 0) {
			qhUserNameCache.remove(res);
			qhUserIDCache.remove(name);
		}
		return res;
	}

	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("SELECT `user_id`,`name`,`pw` FROM `%1users` WHERE `server_id` = ? AND `name` like ?");
	query.addBindValue(iServerNum);
	query.addBindValue(name);
	SQLEXEC();
	if (query.next()) {
		res = -1;
		QString storedpw = query.value(2).toString();
		QString hashedpw = QString::fromLatin1(QCryptographicHash::hash(pw.toUtf8(), QCryptographicHash::Sha1).toHex());

		if (! storedpw.isEmpty() && (storedpw == hashedpw)) {
			name = query.value(1).toString();
			res = query.value(0).toInt();
		}
	}

	// No password match. Try cert or email match, but only for non-SuperUser.
	if (!certhash.isEmpty() && (res < 0)) {
		SQLPREP("SELECT `user_id` FROM `%1user_info` WHERE `server_id` = ? AND `key` = ? AND `value` = ?");
		query.addBindValue(iServerNum);
		query.addBindValue(QLatin1String("certhash"));
		query.addBindValue(certhash);
		SQLEXEC();
		if (query.next()) {
			res = query.value(0).toInt();
		} else if (bStrongCert) {
			foreach(const QString &email, emails) {
				if (! email.isEmpty()) {
					query.addBindValue(iServerNum);
					query.addBindValue(QLatin1String("email"));
					query.addBindValue(email);
					SQLEXEC();
					if (query.next()) {
						res = query.value(0).toInt();
						break;
					}
				}
			}
		}
		if (res > 0) {
			SQLPREP("SELECT `name` FROM `%1users` WHERE `server_id` = ? AND `user_id` = ?");
			query.addBindValue(iServerNum);
			query.addBindValue(res);
			SQLEXEC();
			if (! query.next()) {
				res = -1;
			} else {
				name = query.value(0).toString();
			}
		}
	}
	if (! certhash.isEmpty() && (res > 0)) {
		SQLPREP("REPLACE INTO `%1user_info` (`server_id`, `user_id`, `key`, `value`) VALUES (?, ?, ?, ?)");
		query.addBindValue(iServerNum);
		query.addBindValue(res);
		query.addBindValue(QLatin1String("certhash"));
		query.addBindValue(certhash);
		SQLEXEC();
		if (! emails.isEmpty()) {
			query.addBindValue(iServerNum);
			query.addBindValue(res);
			query.addBindValue(QLatin1String("email"));
			query.addBindValue(emails.at(0));
			SQLEXEC();
		}
	}
	if (res >= 0) {
		qhUserNameCache.remove(res);
		qhUserIDCache.remove(name);
	}
	return res;
}

bool Server::setInfo(int id, const QMap<QString, QString> &setinfo) {
	int res = -2;

	QMap<QString, QString> info = setinfo;

	if (info.contains("name")) {
		qhUserIDCache.remove(qhUserNameCache.value(id));
		qhUserNameCache.remove(id);
		qhUserIDCache.remove(info.value("name"));
	}

	emit setInfoSig(res, id, info);
	if (res >= 0)
		return (res > 0);

	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

	if (info.contains("pw")) {
		const QString &pw = info.value("pw");
		QCryptographicHash hash(QCryptographicHash::Sha1);
		hash.addData(pw.toUtf8());

		SQLPREP("UPDATE `%1users` SET `pw`=? WHERE `server_id` = ? AND `user_id`=?");
		query.addBindValue(pw.isEmpty() ? QVariant() : QString::fromLatin1(hash.result().toHex()));
		query.addBindValue(iServerNum);
		query.addBindValue(id);
		SQLEXEC();
		info.remove("pw");
	}
	if (info.contains("name")) {
		const QString &name = info.value("name");
		SQLPREP("UPDATE `%1users` SET `name`=? WHERE `server_id` = ? AND `user_id`=?");
		query.addBindValue(name);
		query.addBindValue(iServerNum);
		query.addBindValue(id);
		SQLEXEC();
		info.remove("name");
	}
	if (! info.isEmpty()) {
		QMap<QString, QString>::const_iterator i;
		SQLPREP("REPLACE INTO `%1user_info` (`server_id`, `user_id`, `key`, `value`) VALUES (?,?,?,?)");

		QVariantList serverids, userids, keys, values;

		for (i=info.constBegin(); i != info.constEnd(); ++i) {
			serverids << iServerNum;
			userids << id;
			keys << i.key();
			values << i.value();
		}
		query.addBindValue(serverids);
		query.addBindValue(userids);
		query.addBindValue(keys);
		query.addBindValue(values);
		SQLEXECBATCH();
	}

	return true;
}

bool Server::setTexture(int id, const QByteArray &texture) {
	if (id <= 0)
		return false;

	QByteArray tex;
	if (! texture.isEmpty()) {
		qint32 l = 600*60*4;
		if (texture.size() < 4)
			return false;

		if (texture.size() != l) {
			const unsigned char *data = reinterpret_cast<const unsigned char *>(texture.constData());

			if (((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]) == l) {
				tex = qUncompress(texture);
			} else {
				QByteArray qba(4,0);
				qba[0] = static_cast<char>((l >> 24) & 0xFF);
				qba[1] = static_cast<char>((l >> 16) & 0xFF);
				qba[2] = static_cast<char>((l >> 8) & 0xFF);
				qba[3] = static_cast<char>((l >> 0) & 0xFF);
				qba.append(texture);
				tex = qUncompress(qba);
			}
		} else {
			tex = texture;
		}

		if (tex.size() != 600 * 60 * 4)
			return false;
	}

	if (! tex.isEmpty())
		tex = qCompress(tex);

	ServerUser *u = qhUsers.value(id);
	if (u)
		u->qbaTexture = tex;

	int res = -2;
	emit setTextureSig(res, id, tex);
	if (res >= 0)
		return (res > 0);

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("UPDATE `%1users` SET `texture`=? WHERE `server_id` = ? AND `user_id`=?");
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
	SQLPREP("UPDATE `%1users` SET `pw`=? WHERE `server_id` = ? AND `user_id`=?");
	query.addBindValue(QString::fromLatin1(hash.result().toHex()));
	query.addBindValue(srvnum);
	query.addBindValue(0);
	SQLEXEC();
}

QString Server::getUserName(int id) {
	if (qhUserNameCache.contains(id))
		return qhUserNameCache.value(id);
	QString name;
	emit idToNameSig(name, id);
	if (! name.isEmpty()) {
		qhUserIDCache.insert(name, id);
		qhUserNameCache.insert(id, name);
		return name;
	}

	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT `name` FROM `%1users` WHERE `server_id` = ? AND `user_id` = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();
	if (query.next()) {
		name = query.value(0).toString();
		qhUserIDCache.insert(name, id);
		qhUserNameCache.insert(id, name);
	}
	return name;
}

int Server::getUserID(const QString &name) {
	if (qhUserIDCache.contains(name))
		return qhUserIDCache.value(name);
	int id = -2;
	emit nameToIdSig(id, name);
	if (id != -2) {
		qhUserIDCache.insert(name, id);
		qhUserNameCache.insert(id, name);
		return id;
	}
	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT `user_id` FROM `%1users` WHERE `server_id` = ? AND `name` like ?");
	query.addBindValue(iServerNum);
	query.addBindValue(name);
	SQLEXEC();
	if (query.next()) {
		id = query.value(0).toInt();
		qhUserIDCache.insert(name, id);
		qhUserNameCache.insert(id, name);
	}
	return id;
}

QByteArray Server::getUserTexture(int id) {
	QByteArray qba;
	emit idToTextureSig(qba, id);
	if (! qba.isNull()) {
		return qba;
	}

	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT `texture` FROM `%1users` WHERE `server_id` = ? AND `user_id` = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	SQLEXEC();
	if (query.next()) {
		qba = query.value(0).toByteArray();
		if (! qba.isEmpty())
			if (qba.size() == 600 * 60 * 4)
				qba = qCompress(qba);
	}
	return qba;
}

void Server::addLink(Channel *c, Channel *l) {
	if (c->bTemporary || l->bTemporary)
		return;
	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("INSERT INTO `%1channel_links` (`server_id`, `channel_id`, `link_id`) VALUES (?,?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	query.addBindValue(l->iId);
	SQLEXEC();

	query.addBindValue(iServerNum);
	query.addBindValue(l->iId);
	query.addBindValue(c->iId);
	SQLEXEC();

	c->link(l);
}

void Server::removeLink(Channel *c, Channel *l) {
	if (c->bTemporary || l->bTemporary)
		return;
	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;

	if (l) {
		SQLPREP("DELETE FROM `%1channel_links` WHERE `server_id` = ? AND `channel_id` = ? AND `link_id` = ?");
		query.addBindValue(iServerNum);
		query.addBindValue(c->iId);
		query.addBindValue(l->iId);
		SQLEXEC();

		query.addBindValue(iServerNum);
		query.addBindValue(l->iId);
		query.addBindValue(c->iId);
		SQLEXEC();

		c->unlink(l);
	} else {
		SQLPREP("DELETE FROM `%1channel_links` WHERE `server_id` = ? AND (`channel_id` = ? OR `link_id` = ?)");
		query.addBindValue(iServerNum);
		query.addBindValue(c->iId);
		query.addBindValue(c->iId);
		SQLEXEC();

		c->unlink();
	}
}

Channel *Server::addChannel(Channel *p, const QString &name, bool temporary, int position) {
	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("SELECT MAX(`channel_id`)+1 AS id FROM `%1channels` WHERE `server_id`=?");
	query.addBindValue(iServerNum);
	SQLEXEC();
	int id = 0;
	if (query.next())
		id = query.value(0).toInt();

	// Temporary channels might "complicate" this somewhat.
	while (qhChannels.contains(id))
		++id;

	if (! temporary) {
		SQLPREP("INSERT INTO `%1channels` (`server_id`, `parent_id`, `channel_id`, `name`) VALUES (?,?,?,?)");
		query.addBindValue(iServerNum);
		query.addBindValue(p->iId);
		query.addBindValue(id);
		query.addBindValue(name);
		SQLEXEC();
	}

	// Add channel sorting information
	SQLPREP("INSERT INTO `%1channel_info` ( `server_id`, `channel_id`, `key`, `value`) VALUES(?,?,?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(id);
	query.addBindValue("position");
	query.addBindValue(QVariant(position).toString());

	Channel *c = new Channel(id, name, p);
	c->bTemporary = temporary;
	c->iPosition = position;
	qhChannels.insert(id, c);
	return c;
}

void Server::removeChannelDB(const Channel *c) {
	if (! c->bTemporary) {
		TransactionHolder th;

		QSqlQuery &query = *th.qsqQuery;
		SQLPREP("DELETE FROM `%1channels` WHERE `server_id` = ? AND `channel_id` = ?");
		query.addBindValue(iServerNum);
		query.addBindValue(c->iId);
		SQLEXEC();
	}
	qhChannels.remove(c->iId);
}

void Server::updateChannel(const Channel *c) {
	if (c->bTemporary)
		return;
	TransactionHolder th;
	Group *g;
	ChanACL *acl;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("UPDATE `%1channels` SET `name` = ?, `parent_id` = ?, `inheritacl` = ? WHERE `server_id` = ? AND `channel_id` = ?");
	query.addBindValue(c->qsName);
	query.addBindValue(c->cParent ? c->cParent->iId : QVariant());
	query.addBindValue(c->bInheritACL ? 1 : 0);
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	SQLEXEC();

	// Update channel description information
	SQLPREP("REPLACE INTO `%1channel_info` (`server_id`, `channel_id`, `key`, `value`) VALUES (?,?,?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	query.addBindValue("description");
	query.addBindValue(c->qsDesc);
	SQLEXEC();

	// Update channel position information
	SQLPREP("REPLACE INTO `%1channel_info` (`server_id`, `channel_id`, `key`, `value`) VALUES (?,?,?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	query.addBindValue("position");
	query.addBindValue(QVariant(c->iPosition).toString());
	SQLEXEC();

	SQLPREP("DELETE FROM `%1groups` WHERE `server_id` = ? AND `channel_id` = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	SQLEXEC();

	SQLPREP("DELETE FROM `%1acl` WHERE `server_id` = ? AND `channel_id` = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(c->iId);
	SQLEXEC();

	foreach(g, c->qhGroups) {
		SQLPREP("INSERT INTO `%1groups` (`server_id`, `channel_id`, `name`, `inherit`, `inheritable`) VALUES (?,?,?,?,?)");
		query.addBindValue(iServerNum);
		query.addBindValue(g->c->iId);
		query.addBindValue(g->qsName);
		query.addBindValue(g->bInherit ? 1 : 0);
		query.addBindValue(g->bInheritable ? 1 : 0);
		SQLEXEC();

		int id = query.lastInsertId().toInt();
		int pid;

		foreach(pid, g->qsAdd) {
			SQLPREP("INSERT INTO `%1group_members` (`group_id`, `server_id`, `user_id`, `addit`) VALUES (?, ?, ?, ?)");
			query.addBindValue(id);
			query.addBindValue(iServerNum);
			query.addBindValue(pid);
			query.addBindValue(1);
			SQLEXEC();
		}
		foreach(pid, g->qsRemove) {
			SQLPREP("INSERT INTO `%1group_members` (`group_id`, `server_id`, `user_id`, `addit`) VALUES (?, ?, ?, ?)");
			query.addBindValue(id);
			query.addBindValue(iServerNum);
			query.addBindValue(pid);
			query.addBindValue(0);
			SQLEXEC();
		}
	}

	int pri = 5;

	foreach(acl, c->qlACL) {
		SQLPREP("INSERT INTO `%1acl` (`server_id`, `channel_id`, `priority`, `user_id`, `group_name`, `apply_here`, `apply_sub`, `grantpriv`, `revokepriv`) VALUES (?,?,?,?,?,?,?,?,?)");
		query.addBindValue(iServerNum);
		query.addBindValue(acl->c->iId);
		query.addBindValue(pri++);

		query.addBindValue((acl->iUserId == -1) ? QVariant() : acl->iUserId);
		query.addBindValue((acl->qsGroup.isEmpty()) ? QVariant() : acl->qsGroup);
		query.addBindValue(acl->bApplyHere ? 1 : 0);
		query.addBindValue(acl->bApplySubs ? 1 : 0);
		query.addBindValue(static_cast<int>(acl->pAllow));
		query.addBindValue(static_cast<int>(acl->pDeny));
		SQLEXEC();
	}
}

/** Reads the channel privileges (group and acl) as well as the channel information key/value pairs from the database.
 * @param c Channel to fetch information for
 */
void Server::readChannelPrivs(Channel *c) {
	TransactionHolder th;

	int cid = c->iId;

	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("SELECT `key`, `value` FROM `%1channel_info` WHERE `server_id` = ? AND `channel_id` = ?");
	query.addBindValue(iServerNum);
	query.addBindValue(cid);
	SQLEXEC();
	while (query.next()) {
		const QString &key = query.value(0).toString();
		const QString &value = query.value(1).toString();
		if (key == "decription")
			c->qsDesc = value;
		else if (key == "position") {
			c->iPosition = QVariant(value).toInt(); // If the conversion fails it'll return the default value 0
		}
	}

	SQLPREP("SELECT `group_id`, `name`, `inherit`, `inheritable` FROM `%1groups` WHERE `server_id` = ? AND `channel_id` = ?");
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
		mem.prepare(QString::fromLatin1("SELECT user_id, addit FROM %1group_members WHERE group_id = ?").arg(Meta::mp.qsDBPrefix));
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

	SQLPREP("SELECT `user_id`, `group_name`, `apply_here`, `apply_sub`, `grantpriv`, `revokepriv` FROM `%1acl` WHERE `server_id` = ? AND `channel_id` = ? ORDER BY `priority`");
	query.addBindValue(iServerNum);
	query.addBindValue(cid);
	SQLEXEC();
	while (query.next()) {
		ChanACL *acl = new ChanACL(c);
		acl->iUserId = query.value(0).isNull() ? -1 : query.value(0).toInt();
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
			SQLPREP("SELECT `channel_id`, `name`, `inheritacl` FROM `%1channels` WHERE `server_id` = ? AND `parent_id` IS NULL ORDER BY `name`");
			query.addBindValue(iServerNum);
		} else {
			SQLPREP("SELECT `channel_id`, `name`, `inheritacl` FROM `%1channels` WHERE `server_id` = ? AND `parent_id`=? ORDER BY `name`");
			query.addBindValue(iServerNum);
			query.addBindValue(parentid);
		}
		SQLEXEC();

		while (query.next()) {
			c = new Channel(query.value(0).toInt(), query.value(1).toString(), p);
			if (! p)
				c->setParent(this);
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

	SQLPREP("SELECT `channel_id`, `link_id` FROM `%1channel_links` WHERE `server_id` = ?");
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

void Server::setLastChannel(const User *p) {
	if (p->iId < 0)
		return;

	if (p->cChannel->bTemporary)
		return;

	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("UPDATE `%1users` SET `lastchannel`=? WHERE `server_id` = ? AND `user_id` = ?");
	query.addBindValue(p->cChannel->iId);
	query.addBindValue(iServerNum);
	query.addBindValue(p->iId);
	SQLEXEC();
}

int Server::readLastChannel(int id) {
	if (id < 0)
		return 0;

	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("SELECT `lastchannel` FROM `%1users` WHERE `server_id` = ? AND `user_id` = ?");
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
	qWarning("Description: %s", qPrintable(c->qsDesc));
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
		qWarning("ChanACL Here %d Sub %d Allow %04x Deny %04x ID %d Group %s", acl->bApplyHere, acl->bApplySubs, allow, deny, acl->iUserId, qPrintable(acl->qsGroup));
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
	SQLPREP("SELECT `base`,`mask`,`name`,`hash`,`reason`,`start`,`duration` FROM `%1bans` WHERE `server_id` = ?");
	query.addBindValue(iServerNum);
	SQLEXEC();
	while (query.next()) {

		Ban ban;
		ban.haAddress = query.value(0).toByteArray();

		ban.iMask = query.value(1).toInt();
		ban.qsUsername = query.value(2).toString();
		ban.qsHash = query.value(3).toString();
		ban.qsReason = query.value(4).toString();
		ban.qdtStart = query.value(5).toDateTime();
		ban.iDuration = query.value(6).toInt();

		if (ban.isValid())
			qlBans << ban;
	}
}

void Server::saveBans() {
	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("DELETE FROM `%1bans` WHERE `server_id` = ? ");
	query.addBindValue(iServerNum);
	SQLEXEC();

	SQLPREP("INSERT INTO `%1bans` (`server_id`, `base`,`mask`,`name`,`hash`,`reason`,`start`,`duration`) VALUES (?,?,?,?,?,?,?,?)");
	foreach(const Ban &ban, qlBans) {
		query.addBindValue(iServerNum);
		query.addBindValue(ban.haAddress.toByteArray());
		query.addBindValue(ban.iMask);
		query.addBindValue(ban.qsUsername);
		query.addBindValue(ban.qsHash);
		query.addBindValue(ban.qsReason);
		query.addBindValue(ban.qdtStart);
		query.addBindValue(ban.iDuration);
		SQLEXEC();
	}
}

QVariant Server::getConf(const QString &key, QVariant def) {
	return ServerDB::getConf(iServerNum, key, def);
}

QVariant ServerDB::getConf(int server_id, const QString &key, QVariant def) {
	TransactionHolder th;

	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT `value` FROM `%1config` WHERE `server_id` = ? AND `key` = ?");
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
	SQLPREP("SELECT `key`, `value` FROM `%1config` WHERE `server_id` = ?");
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

void Server::dblog(const QString &str) {
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

	SQLPREP("INSERT INTO `%1slog` (`server_id`, `msg`) VALUES(?,?)");
	query.addBindValue(iServerNum);
	query.addBindValue(str);
	SQLEXEC();
}

QList<QPair<unsigned int, QString> > ServerDB::getLog(int server_id, unsigned int offs_min, unsigned int offs_max) {
	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;

	SQLPREP("SELECT `msgtime`, `msg` FROM `%1slog` WHERE `server_id` = ? ORDER BY `msgtime` DESC LIMIT ?, ?");
	query.addBindValue(server_id);
	query.addBindValue(offs_min);
	query.addBindValue(offs_max);
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
		SQLPREP("DELETE FROM `%1config` WHERE `server_id` = ? AND `key` = ?");
		query.addBindValue(server_id);
		query.addBindValue(key);
	} else {
		SQLPREP("REPLACE INTO `%1config` (`server_id`, `key`, `value`) VALUES (?,?,?)");
		query.addBindValue(server_id);
		query.addBindValue(key);
		query.addBindValue(value.toString());
	}
	SQLEXEC();
}


QList<int> ServerDB::getAllServers() {
	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT `server_id` FROM `%1servers`");
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
		SQLPREP("SELECT `value` FROM `%1config` WHERE `server_id` = ? AND `key` = ?");
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
	SQLPREP("SELECT `server_id` FROM `%1servers` WHERE `server_id` = ?");
	query.addBindValue(num);
	SQLEXEC();
	if (query.next())
		return true;
	return false;
}

int ServerDB::addServer() {
	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("SELECT MAX(`server_id`)+1 AS id FROM `%1servers`");
	SQLEXEC();
	int id = 0;
	if (query.next())
		id = query.value(0).toInt();
	SQLPREP("INSERT INTO `%1servers` (`server_id`) VALUES (?)");
	query.addBindValue(id);
	SQLEXEC();
	return id;
}

void ServerDB::deleteServer(int server_id) {
	TransactionHolder th;
	QSqlQuery &query = *th.qsqQuery;
	SQLPREP("DELETE FROM `%1servers` WHERE `server_id` = ?");
	query.addBindValue(server_id);
	SQLEXEC();
}
