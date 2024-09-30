// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Database.h"

#include "MumbleApplication.h"
#include "Net.h"
#include "Utils.h"
#include "Version.h"
#include "Global.h"
#include "GlobalShortcutTypes.h"

#include <QSettings>
#include <QtCore/QStandardPaths>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtWidgets/QMessageBox>

static void logSQLError(const QSqlQuery &query) {
	const QSqlError error(query.lastQuery());
	qWarning() << "SQL Query failed" << query.lastQuery();
	qWarning() << error.nativeErrorCode() << query.lastError().text();
}

static bool execQueryAndLogFailure(QSqlQuery &query) {
	if (!query.exec()) {
		logSQLError(query);
		return false;
	}
	return true;
}

static bool execQueryAndLogFailure(QSqlQuery &query, const QString &queryString) {
	if (!query.exec(queryString)) {
		logSQLError(query);
		return false;
	}
	return true;
}

bool Database::findOrCreateDatabase() {
	QSettings qs;
	QStringList datapaths;

	datapaths << Global::get().qdBasePath.absolutePath();
	datapaths << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
	datapaths << QDir::homePath() + QLatin1String("/.config/Mumble");
#endif
	datapaths << QDir::homePath();
	datapaths << QDir::currentPath();
	datapaths << qApp->applicationDirPath();
	datapaths << qs.value(QLatin1String("InstPath")).toString();
	datapaths.removeAll(QLatin1String(""));
	datapaths.removeDuplicates();

	// Try to find an existing database
	foreach (const QString &datapath, datapaths) {
		// Try the legacy path first, and use it if it exists.
		// If it doesn't, use the new, non-hidden version.
		QFile legacyDatabaseFile(datapath + QLatin1String("/.mumble.sqlite"));
		if (legacyDatabaseFile.exists()) {
			db.setDatabaseName(legacyDatabaseFile.fileName());
			if (db.open()) {
				return true;
			}
		}
		QFile databaseFile(datapath + QLatin1String("/mumble.sqlite"));
		if (databaseFile.exists()) {
			db.setDatabaseName(databaseFile.fileName());
			if (db.open()) {
				return true;
			}
		}
	}

	// There is no existing database, so we create one
	foreach (const QString &datapath, datapaths) {
		QDir::root().mkpath(datapath);
		QFile f(datapath + QLatin1String("/mumble.sqlite"));
		db.setDatabaseName(f.fileName());
		if (db.open()) {
			return true;
		}
	}
	return false;
}

Database::Database(const QString &dbname) {
	db = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"), dbname);
	if (!Global::get().s.qsDatabaseLocation.isEmpty()) {
		QFile configuredLocation(Global::get().s.qsDatabaseLocation);
		if (configuredLocation.exists()) {
			db.setDatabaseName(Global::get().s.qsDatabaseLocation);
			db.open();
		} else {
			int result = QMessageBox::critical(nullptr, QLatin1String("Mumble"),
											   tr("The database file '%1' set in the configuration file does not "
												  "exist. Do you want to create a new database file at this location?")
												   .arg(Global::get().s.qsDatabaseLocation),
											   QMessageBox::Yes | QMessageBox::No);
			if (result == QMessageBox::Yes) {
				db.setDatabaseName(Global::get().s.qsDatabaseLocation);
				db.open();
			} else {
				qFatal("Database: File not found");
			}
		}
	}
	if (!db.isOpen()) {
		if (findOrCreateDatabase()) {
			Global::get().s.qsDatabaseLocation = db.databaseName();
		} else {
			QMessageBox::critical(nullptr, QLatin1String("Mumble"),
								  tr("Mumble failed to initialize a database in any of the possible locations."),
								  QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
			qFatal("Database: Failed initialization");
		}
	}

	QFileInfo fi(db.databaseName());

	if (!fi.isWritable()) {
		QMessageBox::critical(nullptr, QLatin1String("Mumble"),
							  tr("The database '%1' is read-only. Mumble cannot store server settings (i.e. SSL "
								 "certificates) until you fix this problem.")
								  .arg(fi.filePath().toHtmlEscaped()),
							  QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
		qWarning("Database: Database is read-only");
	}

	{
		QFile f(db.databaseName());
		f.setPermissions(f.permissions()
						 & ~(QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup | QFile::ReadOther
							 | QFile::WriteOther | QFile::ExeOther));
	}

	QSqlQuery query(db);

	execQueryAndLogFailure(query,
						   QLatin1String("CREATE TABLE IF NOT EXISTS `servers` (`id` INTEGER PRIMARY KEY "
										 "AUTOINCREMENT, `name` TEXT, `hostname` TEXT, `port` INTEGER DEFAULT " MUMTEXT(
											 DEFAULT_MUMBLE_PORT) ", `username` TEXT, `password` TEXT)"));
	query.exec(QLatin1String(
		"ALTER TABLE `servers` ADD COLUMN `url` TEXT")); // Upgrade path, failing this query is not noteworthy

	execQueryAndLogFailure(
		query, QLatin1String("CREATE TABLE IF NOT EXISTS `comments` (`who` TEXT, `comment` BLOB, `seen` DATE)"));
	execQueryAndLogFailure(
		query, QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `comments_comment` ON `comments`(`who`, `comment`)"));
	execQueryAndLogFailure(query, QLatin1String("CREATE INDEX IF NOT EXISTS `comments_seen` ON `comments`(`seen`)"));

	execQueryAndLogFailure(query,
						   QLatin1String("CREATE TABLE IF NOT EXISTS `blobs` (`hash` TEXT, `data` BLOB, `seen` DATE)"));
	execQueryAndLogFailure(query, QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `blobs_hash` ON `blobs`(`hash`)"));
	execQueryAndLogFailure(query, QLatin1String("CREATE INDEX IF NOT EXISTS `blobs_seen` ON `blobs`(`seen`)"));

	execQueryAndLogFailure(query, QLatin1String("CREATE TABLE IF NOT EXISTS `tokens` (`id` INTEGER PRIMARY KEY "
												"AUTOINCREMENT, `digest` BLOB, `token` TEXT)"));
	execQueryAndLogFailure(query, QLatin1String("CREATE INDEX IF NOT EXISTS `tokens_host_port` ON `tokens`(`digest`)"));

	execQueryAndLogFailure(
		query, QLatin1String("CREATE TABLE IF NOT EXISTS `shortcut` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `digest` "
							 "BLOB, `type` INTEGER, `shortcut` BLOB, `target` BLOB, `suppress` INTEGER)"));
	query.exec(QLatin1String(
		"ALTER TABLE `shortcut` ADD COLUMN `type` INTEGER")); // Upgrade path, failing this query is not noteworthy
	execQueryAndLogFailure(query,
						   QLatin1String("CREATE INDEX IF NOT EXISTS `shortcut_host_port` ON `shortcut`(`digest`)"));

	execQueryAndLogFailure(
		query,
		QLatin1String("CREATE TABLE IF NOT EXISTS `udp` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `digest` BLOB)"));
	execQueryAndLogFailure(query,
						   QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `udp_host_port` ON `udp`(`digest`)"));

	execQueryAndLogFailure(query, QLatin1String("CREATE TABLE IF NOT EXISTS `cert` (`id` INTEGER PRIMARY KEY "
												"AUTOINCREMENT, `hostname` TEXT, `port` INTEGER, `digest` TEXT)"));
	execQueryAndLogFailure(
		query, QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `cert_host_port` ON `cert`(`hostname`,`port`)"));

	execQueryAndLogFailure(
		query,
		QLatin1String(
			"CREATE TABLE IF NOT EXISTS `friends` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `name` TEXT, `hash` TEXT)"));
	execQueryAndLogFailure(query,
						   QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `friends_name` ON `friends`(`name`)"));
	execQueryAndLogFailure(query,
						   QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `friends_hash` ON `friends`(`hash`)"));

	execQueryAndLogFailure(
		query,
		QLatin1String("CREATE TABLE IF NOT EXISTS `ignored` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `hash` TEXT)"));
	execQueryAndLogFailure(query,
						   QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `ignored_hash` ON `ignored`(`hash`)"));

	execQueryAndLogFailure(
		query, QLatin1String(
				   "CREATE TABLE IF NOT EXISTS `ignored_tts` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `hash` TEXT)"));
	execQueryAndLogFailure(
		query, QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `ignored_tts_hash` ON `ignored_tts`(`hash`)"));

	execQueryAndLogFailure(
		query,
		QLatin1String("CREATE TABLE IF NOT EXISTS `muted` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `hash` TEXT)"));
	execQueryAndLogFailure(query, QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `muted_hash` ON `muted`(`hash`)"));
	execQueryAndLogFailure(query, QLatin1String("CREATE TABLE IF NOT EXISTS `volume` (`id` INTEGER PRIMARY KEY "
												"AUTOINCREMENT, `hash` TEXT, `volume` FLOAT)"));
	execQueryAndLogFailure(query, QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `volume_hash` ON `volume`(`hash`)"));
	execQueryAndLogFailure(query, QLatin1String("CREATE TABLE IF NOT EXISTS `nicknames` (`id` INTEGER PRIMARY KEY "
												"AUTOINCREMENT, `hash` TEXT, `nickname` TEXT)"));
	execQueryAndLogFailure(query,
						   QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `nicknames_hash` ON `nicknames`(`hash`)"));

	// Note: A previous snapshot version created a table called 'hidden'
	execQueryAndLogFailure(
		query, QLatin1String("CREATE TABLE IF NOT EXISTS `filtered_channels` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
							 "`server_cert_digest` TEXT NOT NULL, `channel_id` INTEGER NOT NULL)"));
	execQueryAndLogFailure(query, QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `filtered_channels_entry` ON "
												"`filtered_channels`(`server_cert_digest`, `channel_id`)"));
	query.exec(QLatin1String("ALTER TABLE `filtered_channels` ADD COLUMN `filter_mode` INTEGER DEFAULT ")
			   + QString::number(
				   static_cast< int >(ChannelFilterMode::HIDE))); // Upgrade path, failing this query is not noteworthy

	execQueryAndLogFailure(query, QLatin1String("CREATE TABLE IF NOT EXISTS `pingcache` (`id` INTEGER PRIMARY KEY "
												"AUTOINCREMENT, `hostname` TEXT, `port` INTEGER, `ping` INTEGER)"));
	execQueryAndLogFailure(
		query,
		QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `pingcache_host_port` ON `pingcache`(`hostname`,`port`)"));

	execQueryAndLogFailure(query, QLatin1String("DELETE FROM `comments` WHERE `seen` < datetime('now', '-1 years')"));
	execQueryAndLogFailure(query, QLatin1String("DELETE FROM `blobs` WHERE `seen` < datetime('now', '-1 months')"));

	execQueryAndLogFailure(query, QLatin1String("VACUUM"));

	execQueryAndLogFailure(query, QLatin1String("PRAGMA synchronous = NORMAL"));
#ifdef Q_OS_WIN
	// Windows can not handle TRUNCATE with multiple connections to the DB. Thus less performant DELETE.
	execQueryAndLogFailure(query, QLatin1String("PRAGMA journal_mode = DELETE"));
#else
	execQueryAndLogFailure(query, QLatin1String("PRAGMA journal_mode = TRUNCATE"));
#endif

	execQueryAndLogFailure(query, QLatin1String("SELECT sqlite_version()"));
	while (query.next())
		qWarning() << "Database SQLite:" << query.value(0).toString();
}

Database::~Database() {
	QSqlQuery query(db);
	execQueryAndLogFailure(query, QLatin1String("PRAGMA journal_mode = DELETE"));
	execQueryAndLogFailure(query, QLatin1String("VACUUM"));
}

QList< FavoriteServer > Database::getFavorites() {
	QSqlQuery query(db);
	QList< FavoriteServer > ql;

	query.prepare(QLatin1String(
		"SELECT `name`, `hostname`, `port`, `username`, `password`, `url` FROM `servers` ORDER BY `name`"));
	execQueryAndLogFailure(query);

	while (query.next()) {
		FavoriteServer fs;
		fs.qsName     = query.value(0).toString();
		fs.qsHostname = query.value(1).toString();
		fs.usPort     = static_cast< unsigned short >(query.value(2).toUInt());
		fs.qsUsername = query.value(3).toString();
		fs.qsPassword = query.value(4).toString();
		fs.qsUrl      = query.value(5).toString();
		ql << fs;
	}
	return ql;
}

void Database::setFavorites(const QList< FavoriteServer > &servers) {
	QSqlQuery query(db);
	db.transaction();

	query.prepare(QLatin1String("DELETE FROM `servers`"));
	execQueryAndLogFailure(query);

	query.prepare(QLatin1String(
		"REPLACE INTO `servers` (`name`, `hostname`, `port`, `username`, `password`, `url`) VALUES (?,?,?,?,?,?)"));
	foreach (const FavoriteServer &s, servers) {
		query.addBindValue(s.qsName);
		query.addBindValue(s.qsHostname);
		query.addBindValue(s.usPort);
		query.addBindValue(s.qsUsername);
		query.addBindValue(s.qsPassword);
		query.addBindValue(s.qsUrl);
		execQueryAndLogFailure(query);
	}

	db.commit();
}

bool Database::isLocalIgnored(const QString &hash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `hash` FROM `ignored` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
	return query.next();
}

void Database::setLocalIgnored(const QString &hash, bool ignored) {
	QSqlQuery query(db);

	if (ignored)
		query.prepare(QLatin1String("INSERT INTO `ignored` (`hash`) VALUES (?)"));
	else
		query.prepare(QLatin1String("DELETE FROM `ignored` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
}

bool Database::isLocalIgnoredTTS(const QString &hash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `hash` FROM `ignored_tts` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
	return query.next();
}

void Database::setLocalIgnoredTTS(const QString &hash, bool ignoredTTS) {
	QSqlQuery query(db);

	if (ignoredTTS)
		query.prepare(QLatin1String("INSERT INTO `ignored_tts` (`hash`) VALUES (?)"));
	else
		query.prepare(QLatin1String("DELETE FROM `ignored_tts` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
}

bool Database::isLocalMuted(const QString &hash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `hash` FROM `muted` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
	return query.next();
}

void Database::setUserLocalVolume(const QString &hash, float volume) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("INSERT OR REPLACE INTO `volume` (`hash`, `volume`) VALUES (?,?)"));
	query.addBindValue(hash);
	query.addBindValue(QString::number(volume));
	execQueryAndLogFailure(query);
}

float Database::getUserLocalVolume(const QString &hash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `volume` FROM `volume` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
	if (query.first()) {
		return query.value(0).toString().toFloat();
	}
	return 1.0f;
}

void Database::setUserLocalNickname(const QString &hash, const QString &nickname) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("INSERT OR REPLACE INTO `nicknames` (`hash`, `nickname`) VALUES (?,?)"));
	query.addBindValue(hash);
	query.addBindValue(nickname);
	execQueryAndLogFailure(query);
}

QString Database::getUserLocalNickname(const QString &hash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `nickname` FROM `nicknames` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
	if (query.first()) {
		return query.value(0).toString();
	}
	return QString();
}

void Database::setLocalMuted(const QString &hash, bool muted) {
	QSqlQuery query(db);

	if (muted)
		query.prepare(QLatin1String("INSERT INTO `muted` (`hash`) VALUES (?)"));
	else
		query.prepare(QLatin1String("DELETE FROM `muted` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
}

void Database::clearLocalMuted() {
	QSqlQuery query(db);
	query.prepare(QLatin1String("DELETE FROM `muted`"));
	execQueryAndLogFailure(query);
}

ChannelFilterMode Database::getChannelFilterMode(const QByteArray &server_cert_digest, const unsigned int channel_id) {
	QSqlQuery query(db);

	query.prepare(QLatin1String(
		"SELECT `filter_mode` FROM `filtered_channels` WHERE `server_cert_digest` = ? AND `channel_id` = ?"));
	query.addBindValue(server_cert_digest);
	query.addBindValue(channel_id);
	execQueryAndLogFailure(query);

	if (query.first()) {
		return static_cast< ChannelFilterMode >(query.value(0).toInt());
	}

	return ChannelFilterMode::NORMAL;
}

void Database::setChannelFilterMode(const QByteArray &server_cert_digest, const unsigned int channel_id,
									const ChannelFilterMode filterMode) {
	QSqlQuery query(db);

	switch (filterMode) {
		case ChannelFilterMode::NORMAL:
			query.prepare(
				QLatin1String("DELETE FROM `filtered_channels` WHERE `server_cert_digest` = ? AND `channel_id` = ?"));
			break;
		case ChannelFilterMode::PIN:
		case ChannelFilterMode::HIDE:
			query.prepare(QLatin1String("INSERT OR REPLACE INTO `filtered_channels` (`server_cert_digest`, "
										"`channel_id`, `filter_mode`) VALUES (?, ?, ?)"));
			query.bindValue(2, static_cast< int >(filterMode));
			break;
	}

	query.bindValue(0, server_cert_digest);
	query.bindValue(1, channel_id);

	execQueryAndLogFailure(query);
}

QMap< UnresolvedServerAddress, unsigned int > Database::getPingCache() {
	QSqlQuery query(db);
	QMap< UnresolvedServerAddress, unsigned int > map;

	query.prepare(QLatin1String("SELECT `hostname`, `port`, `ping` FROM `pingcache`"));
	execQueryAndLogFailure(query);
	while (query.next()) {
		map.insert(
			UnresolvedServerAddress(query.value(0).toString(), static_cast< unsigned short >(query.value(1).toUInt())),
			query.value(2).toUInt());
	}
	return map;
}

void Database::setPingCache(const QMap< UnresolvedServerAddress, unsigned int > &map) {
	QSqlQuery query(db);
	QMap< UnresolvedServerAddress, unsigned int >::const_iterator i;

	db.transaction();

	query.prepare(QLatin1String("DELETE FROM `pingcache`"));
	execQueryAndLogFailure(query);

	query.prepare(QLatin1String("REPLACE INTO `pingcache` (`hostname`, `port`, `ping`) VALUES (?,?,?)"));
	for (i = map.constBegin(); i != map.constEnd(); ++i) {
		query.addBindValue(i.key().hostname);
		query.addBindValue(i.key().port);
		query.addBindValue(i.value());
		execQueryAndLogFailure(query);
	}

	db.commit();
}

bool Database::seenComment(const QString &hash, const QByteArray &commenthash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT COUNT(*) FROM `comments` WHERE `who` = ? AND `comment` = ?"));
	query.addBindValue(hash);
	query.addBindValue(commenthash);
	execQueryAndLogFailure(query);
	if (query.next()) {
		if (query.value(0).toInt() > 0) {
			query.prepare(
				QLatin1String("UPDATE `comments` SET `seen` = datetime('now') WHERE `who` = ? AND `comment` = ?"));
			query.addBindValue(hash);
			query.addBindValue(commenthash);
			execQueryAndLogFailure(query);
			return true;
		}
	}
	return false;
}

void Database::setSeenComment(const QString &hash, const QByteArray &commenthash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("REPLACE INTO `comments` (`who`, `comment`, `seen`) VALUES (?, ?, datetime('now'))"));
	query.addBindValue(hash);
	query.addBindValue(commenthash);
	execQueryAndLogFailure(query);
}

QByteArray Database::blob(const QByteArray &hash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `data` FROM `blobs` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
	if (query.next()) {
		QByteArray qba = query.value(0).toByteArray();

		query.prepare(QLatin1String("UPDATE `blobs` SET `seen` = datetime('now') WHERE `hash` = ?"));
		query.addBindValue(hash);
		execQueryAndLogFailure(query);

		return qba;
	}
	return QByteArray();
}

void Database::setBlob(const QByteArray &hash, const QByteArray &data) {
	if (hash.isEmpty() || data.isEmpty())
		return;

	QSqlQuery query(db);

	query.prepare(QLatin1String("REPLACE INTO `blobs` (`hash`, `data`, `seen`) VALUES (?, ?, datetime('now'))"));
	query.addBindValue(hash);
	query.addBindValue(data);
	execQueryAndLogFailure(query);
}

QStringList Database::getTokens(const QByteArray &digest) {
	QList< QString > qsl;
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `token` FROM `tokens` WHERE `digest` = ?"));
	query.addBindValue(digest);
	execQueryAndLogFailure(query);
	while (query.next()) {
		qsl << query.value(0).toString();
	}
	return qsl;
}

void Database::setTokens(const QByteArray &digest, QStringList &tokens) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("DELETE FROM `tokens` WHERE `digest` = ?"));
	query.addBindValue(digest);
	execQueryAndLogFailure(query);

	query.prepare(QLatin1String("INSERT INTO `tokens` (`digest`, `token`) VALUES (?,?)"));
	foreach (const QString &qs, tokens) {
		query.addBindValue(digest);
		query.addBindValue(qs);
		execQueryAndLogFailure(query);
	}
}

QList< Shortcut > Database::getShortcuts(const QByteArray &digest) {
	QList< Shortcut > ql;
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `type`, `shortcut`,`target`,`suppress` FROM `shortcut` WHERE `digest` = ?"));
	query.addBindValue(digest);
	execQueryAndLogFailure(query);
	while (query.next()) {
		Shortcut sc;

		QVariant type = query.value(0);

		if (type.isNull()) {
			// The shortcut's type was originally not explicitly stored, because the assumption was that the only
			// server-specific shortcuts (which are the ones we're dealing with here) are those configuring whispers or
			// shouts. Thus, if the field is not set, we assume that we're loading a shortcut from that era, which means
			// that we'll assume it to be a whisper/shout shortcut as well.
			sc.iIndex = GlobalShortcutType::Whisper_Shout;
		} else {
			sc.iIndex = type.toInt();
		}

		QByteArray a = query.value(1).toByteArray();

		{
			QDataStream s(&a, QIODevice::ReadOnly);
			s.setVersion(QDataStream::Qt_4_0);
			s >> sc.qlButtons;
		}

		a = query.value(2).toByteArray();

		{
			QDataStream s(&a, QIODevice::ReadOnly);
			s.setVersion(QDataStream::Qt_4_0);
			s >> sc.qvData;
		}

		sc.bSuppress = query.value(3).toBool();
		ql << sc;
	}
	return ql;
}

void Database::setShortcuts(const QByteArray &digest, const QList< Shortcut > &shortcuts) {
	QSqlQuery query(db);

	if (!db.transaction()) {
		const QSqlError error(QSqlDatabase::database().lastError());
		qWarning() << "Database: Unable to start transaction for saving shortcuts" << error.nativeErrorCode()
				   << error.text();
		qWarning() << "-> We'll rather not save them at all than risk potentially losing all previous shortcuts";

		return;
	}

	query.prepare(QLatin1String("DELETE FROM `shortcut` WHERE `digest` = ?"));
	query.addBindValue(digest);
	execQueryAndLogFailure(query);

	query.prepare(QLatin1String(
		"INSERT INTO `shortcut` (`digest`, `type`, `shortcut`, `target`, `suppress`) VALUES (?,?,?,?,?)"));
	for (const Shortcut &sc : shortcuts) {
		if (sc.isServerSpecific()) {
			query.addBindValue(digest);

			query.addBindValue(sc.iIndex);

			QByteArray a;
			{
				QDataStream s(&a, QIODevice::WriteOnly);
				s.setVersion(QDataStream::Qt_4_0);
				s << sc.qlButtons;
			}
			query.addBindValue(a);

			a.clear();
			{
				QDataStream s(&a, QIODevice::WriteOnly);
				s.setVersion(QDataStream::Qt_4_0);
				s << sc.qvData;
			}
			query.addBindValue(a);

			query.addBindValue(sc.bSuppress);
			execQueryAndLogFailure(query);
		}
	}

	db.commit();
}

const QMap< QString, QString > Database::getFriends() {
	QMap< QString, QString > qm;
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `name`, `hash` FROM `friends`"));
	execQueryAndLogFailure(query);
	while (query.next())
		qm.insert(query.value(0).toString(), query.value(1).toString());
	return qm;
}

const QString Database::getFriend(const QString &hash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `name` FROM `friends` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
	if (query.next())
		return query.value(0).toString();
	return QString();
}

void Database::addFriend(const QString &name, const QString &hash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("REPLACE INTO `friends` (`name`, `hash`) VALUES (?,?)"));
	query.addBindValue(name);
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
}

void Database::removeFriend(const QString &hash) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("DELETE FROM `friends` WHERE `hash` = ?"));
	query.addBindValue(hash);
	execQueryAndLogFailure(query);
}

const QString Database::getDigest(const QString &hostname, unsigned short port) {
	QSqlQuery query(db);

	query.prepare(QLatin1String("SELECT `digest` FROM `cert` WHERE `hostname` = ? AND `port` = ?"));
	query.addBindValue(hostname);
	query.addBindValue(port);
	execQueryAndLogFailure(query);
	if (query.next()) {
		return query.value(0).toString();
	}
	return QString();
}

void Database::setDigest(const QString &hostname, unsigned short port, const QString &digest) {
	QSqlQuery query(db);
	query.prepare(QLatin1String("REPLACE INTO `cert` (`hostname`,`port`,`digest`) VALUES (?,?,?)"));
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.addBindValue(digest);
	execQueryAndLogFailure(query);
}

void Database::setPassword(const QString &hostname, unsigned short port, const QString &uname, const QString &pw) {
	QSqlQuery query(db);
	query.prepare(
		QLatin1String("UPDATE `servers` SET `password` = ? WHERE `hostname` = ? AND `port` = ? AND `username` = ?"));
	query.addBindValue(pw);
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.addBindValue(uname);
	execQueryAndLogFailure(query);
}

bool Database::getUdp(const QByteArray &digest) {
	QSqlQuery query(db);
	query.prepare(QLatin1String("SELECT COUNT(*) FROM `udp` WHERE `digest` = ? "));
	query.addBindValue(digest);
	execQueryAndLogFailure(query);
	if (query.next()) {
		return (query.value(0).toInt() == 0);
	}
	return true;
}

void Database::setUdp(const QByteArray &digest, bool udp) {
	QSqlQuery query(db);
	if (!udp)
		query.prepare(QLatin1String("REPLACE INTO `udp` (`digest`) VALUES (?)"));
	else
		query.prepare(QLatin1String("DELETE FROM `udp` WHERE `digest` = ?"));
	query.addBindValue(digest);
	execQueryAndLogFailure(query);
}


bool Database::fuzzyMatch(QString &name, QString &user, QString &pw, QString &hostname, unsigned short port) {
	QSqlQuery query(db);
	if (!user.isEmpty()) {
		query.prepare(QLatin1String("SELECT `username`, `password`, `hostname`, `name` FROM `servers` WHERE `username` "
									"LIKE ? AND `hostname` LIKE ? AND `port`=?"));
		query.addBindValue(user);
	} else {
		query.prepare(QLatin1String(
			"SELECT `username`, `password`, `hostname`, `name` FROM `servers` WHERE `hostname` LIKE ? AND `port`=?"));
	}
	query.addBindValue(hostname);
	query.addBindValue(port);
	execQueryAndLogFailure(query);
	if (query.next()) {
		user = query.value(0).toString();
		if (pw.isEmpty())
			pw = query.value(1).toString();
		hostname = query.value(2).toString();
		if (name.isEmpty())
			name = query.value(3).toString();
		return true;
	} else {
		return false;
	}
}
