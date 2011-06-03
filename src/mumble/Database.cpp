/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#include "Database.h"
#include "Global.h"
#include "Message.h"
#include "Net.h"
#include "Version.h"

Database::Database() {
	QSqlDatabase db = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"));
	QSettings qs;
	QStringList datapaths;
	int i;

	datapaths << g.qdBasePath.absolutePath();
	datapaths << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#if defined(Q_OS_UNIX) && ! defined(Q_OS_MAC)
	datapaths << QDir::homePath() + QLatin1String("/.config/Mumble");
#endif
	datapaths << QDir::homePath();
	datapaths << QDir::currentPath();
	datapaths << qApp->applicationDirPath();
	datapaths << qs.value(QLatin1String("InstPath")).toString();
	bool found = false;

	for (i = 0; (i < datapaths.size()) && ! found; i++) {
		if (!datapaths[i].isEmpty()) {
			QFile f(datapaths[i] + QLatin1String("/mumble.sqlite"));
			if (f.exists()) {
				db.setDatabaseName(f.fileName());
				found = db.open();
			}

			QFile f2(datapaths[i] + QLatin1String("/.mumble.sqlite"));
			if (f2.exists()) {
				db.setDatabaseName(f2.fileName());
				found = db.open();
			}
		}
	}

	if (! found) {
		for (i = 0; (i < datapaths.size()) && ! found; i++) {
			if (!datapaths[i].isEmpty()) {
				QDir::root().mkpath(datapaths[i]);
#ifdef Q_OS_WIN
				QFile f(datapaths[i] + QLatin1String("/mumble.sqlite"));
#else
				QFile f(datapaths[i] + QLatin1String("/.mumble.sqlite"));
#endif
				db.setDatabaseName(f.fileName());
				found = db.open();
			}
		}
	}

	if (! found) {
		QMessageBox::critical(NULL, QLatin1String("Mumble"), tr("Mumble failed to initialize a database in any\nof the possible locations."), QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
		qFatal("Database: Failed initialization");
	}

	QFileInfo fi(db.databaseName());

	if (! fi.isWritable()) {
		QMessageBox::critical(NULL, QLatin1String("Mumble"), tr("The database '%1' is read-only. Mumble cannot store server settings (i.e. SSL certificates) until you fix this problem.").arg(fi.filePath()), QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
		qWarning("Database: Database is read-only");
	}

	{
		QFile f(db.databaseName());
		f.setPermissions(f.permissions() & ~(QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup | QFile::ReadOther | QFile::WriteOther | QFile::ExeOther));
	}

	QSqlQuery query;

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `servers` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `name` TEXT, `hostname` TEXT, `port` INTEGER DEFAULT " MUMTEXT(DEFAULT_MUMBLE_PORT) ", `username` TEXT, `password` TEXT)"));
	query.exec(QLatin1String("ALTER TABLE `servers` ADD COLUMN `url` TEXT"));

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `comments` (`who` TEXT, `comment` BLOB, `seen` DATE)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `comments_comment` ON `comments`(`who`, `comment`)"));
	query.exec(QLatin1String("CREATE INDEX IF NOT EXISTS `comments_seen` ON `comments`(`seen`)"));

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `blobs` (`hash` TEXT, `data` BLOB, `seen` DATE)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `blobs_hash` ON `blobs`(`hash`)"));
	query.exec(QLatin1String("CREATE INDEX IF NOT EXISTS `blobs_seen` ON `blobs`(`seen`)"));

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `tokens` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `digest` BLOB, `token` TEXT)"));
	query.exec(QLatin1String("CREATE INDEX IF NOT EXISTS `tokens_host_port` ON `tokens`(`digest`)"));

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `shortcut` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `digest` BLOB, `shortcut` BLOB, `target` BLOB, `suppress` INTEGER)"));
	query.exec(QLatin1String("CREATE INDEX IF NOT EXISTS `shortcut_host_port` ON `shortcut`(`digest`)"));

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `udp` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `digest` BLOB)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `udp_host_port` ON `udp`(`digest`)"));

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `cert` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `hostname` TEXT, `port` INTEGER, `digest` TEXT)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `cert_host_port` ON `cert`(`hostname`,`port`)"));

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `friends` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `name` TEXT, `hash` TEXT)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `friends_name` ON `friends`(`name`)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `friends_hash` ON `friends`(`hash`)"));

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `muted` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `hash` TEXT)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `muted_hash` ON `muted`(`hash`)"));

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `pingcache` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `hostname` TEXT, `port` INTEGER, `ping` INTEGER)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `pingcache_host_port` ON `pingcache`(`hostname`,`port`)"));

	query.exec(QLatin1String("DELETE FROM `comments` WHERE `seen` < datetime('now', '-1 years')"));
	query.exec(QLatin1String("DELETE FROM `blobs` WHERE `seen` < datetime('now', '-1 months')"));

	query.exec(QLatin1String("VACUUM"));

	query.exec(QLatin1String("PRAGMA synchronous = OFF"));
	query.exec(QLatin1String("PRAGMA journal_mode = TRUNCATE"));

	query.exec(QLatin1String("SELECT sqlite_version()"));
	while (query.next())
		qWarning() << "Database SQLite:" << query.value(0).toString();
}

Database::~Database() {
	QSqlQuery query;
	query.exec(QLatin1String("PRAGMA journal_mode = DELETE"));
	query.exec(QLatin1String("VACUUM"));
}

QList<FavoriteServer> Database::getFavorites() {
	QSqlQuery query;
	QList<FavoriteServer> ql;

	query.prepare(QLatin1String("SELECT `name`, `hostname`, `port`, `username`, `password`, `url` FROM `servers` ORDER BY `name`"));
	query.exec();
	while (query.next()) {
		FavoriteServer fs;
		fs.qsName = query.value(0).toString();
		fs.qsHostname = query.value(1).toString();
		fs.usPort = static_cast<unsigned short>(query.value(2).toUInt());
		fs.qsUsername = query.value(3).toString();
		fs.qsPassword = query.value(4).toString();
		fs.qsUrl = query.value(5).toString();
		ql << fs;
	}
	return ql;
}

void Database::setFavorites(const QList<FavoriteServer> &servers) {
	QSqlQuery query;
	QSqlDatabase::database().transaction();

	query.prepare(QLatin1String("DELETE FROM `servers`"));
	query.exec();

	query.prepare(QLatin1String("REPLACE INTO `servers` (`name`, `hostname`, `port`, `username`, `password`, `url`) VALUES (?,?,?,?,?,?)"));
	foreach(const FavoriteServer &s, servers) {
		query.addBindValue(s.qsName);
		query.addBindValue(s.qsHostname);
		query.addBindValue(s.usPort);
		query.addBindValue(s.qsUsername);
		query.addBindValue(s.qsPassword);
		query.addBindValue(s.qsUrl);
		query.exec();
	}

	QSqlDatabase::database().commit();
}

bool Database::isLocalMuted(const QString &hash) {
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT `hash` FROM `muted` WHERE `hash` = ?"));
	query.addBindValue(hash);
	query.exec();
	while (query.next()) {
		return true;
	}
	return false;
}

void Database::setLocalMuted(const QString &hash, bool muted) {
	QSqlQuery query;

	if (muted)
		query.prepare(QLatin1String("INSERT INTO `muted` (`hash`) VALUES (?)"));
	else
		query.prepare(QLatin1String("DELETE FROM `muted` WHERE `hash` = ?"));
	query.addBindValue(hash);
	query.exec();
}

QMap<QPair<QString, unsigned short>, unsigned int> Database::getPingCache() {
	QSqlQuery query;
	QMap<QPair<QString, unsigned short>, unsigned int> map;

	query.prepare(QLatin1String("SELECT `hostname`, `port`, `ping` FROM `pingcache`"));
	query.exec();
	while (query.next()) {
		map.insert(QPair<QString, unsigned short>(query.value(0).toString(), query.value(1).toUInt()), query.value(2).toUInt());
	}
	return map;
}

void Database::setPingCache(const QMap<QPair<QString, unsigned short>, unsigned int> &map) {
	QSqlQuery query;
	QMap<QPair<QString, unsigned short>, unsigned int>::const_iterator i;

	QSqlDatabase::database().transaction();

	query.prepare(QLatin1String("DELETE FROM `pingcache`"));
	query.exec();

	query.prepare(QLatin1String("REPLACE INTO `pingcache` (`hostname`, `port`, `ping`) VALUES (?,?,?)"));
	for (i = map.constBegin(); i != map.constEnd(); ++i) {
		query.addBindValue(i.key().first);
		query.addBindValue(i.key().second);
		query.addBindValue(i.value());
		query.exec();
	}

	QSqlDatabase::database().commit();
}

bool Database::seenComment(const QString &hash, const QByteArray &commenthash) {
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT COUNT(*) FROM `comments` WHERE `who` = ? AND `comment` = ?"));
	query.addBindValue(hash);
	query.addBindValue(commenthash);
	query.exec();
	if (query.next()) {
		if (query.value(0).toInt() > 0) {
			query.prepare(QLatin1String("UPDATE `comments` SET `seen` = datetime('now') WHERE `who` = ? AND `comment` = ?"));
			query.addBindValue(hash);
			query.addBindValue(commenthash);
			query.exec();
			return true;
		}
	}
	return false;
}

void Database::setSeenComment(const QString &hash, const QByteArray &commenthash) {
	QSqlQuery query;

	query.prepare(QLatin1String("REPLACE INTO `comments` (`who`, `comment`, `seen`) VALUES (?, ?, datetime('now'))"));
	query.addBindValue(hash);
	query.addBindValue(commenthash);
	query.exec();
}

QByteArray Database::blob(const QByteArray &hash) {
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT `data` FROM `blobs` WHERE `hash` = ?"));
	query.addBindValue(hash);
	query.exec();
	if (query.next()) {
		QByteArray qba = query.value(0).toByteArray();

		query.prepare(QLatin1String("UPDATE `blobs` SET `seen` = datetime('now') WHERE `hash` = ?"));
		query.addBindValue(hash);
		query.exec();

		return qba;
	}
	return QByteArray();
}

void Database::setBlob(const QByteArray &hash, const QByteArray &data) {
	if (hash.isEmpty() || data.isEmpty())
		return;

	QSqlQuery query;

	query.prepare(QLatin1String("REPLACE INTO `blobs` (`hash`, `data`, `seen`) VALUES (?, ?, datetime('now'))"));
	query.addBindValue(hash);
	query.addBindValue(data);
	query.exec();
}

QStringList Database::getTokens(const QByteArray &digest) {
	QList<QString> qsl;
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT `token` FROM `tokens` WHERE `digest` = ?"));
	query.addBindValue(digest);
	query.exec();
	while (query.next()) {
		qsl << query.value(0).toString();
	}
	return qsl;
}

void Database::setTokens(const QByteArray &digest, QStringList &tokens) {
	QSqlQuery query;

	query.prepare(QLatin1String("DELETE FROM `tokens` WHERE `digest` = ?"));
	query.addBindValue(digest);
	query.exec();

	query.prepare(QLatin1String("INSERT INTO `tokens` (`digest`, `token`) VALUES (?,?)"));
	foreach(const QString &qs, tokens) {
		query.addBindValue(digest);
		query.addBindValue(qs);
		query.exec();
	}
}

QList<Shortcut> Database::getShortcuts(const QByteArray &digest) {
	QList<Shortcut> ql;
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT `shortcut`,`target`,`suppress` FROM `shortcut` WHERE `digest` = ?"));
	query.addBindValue(digest);
	query.exec();
	while (query.next()) {
		Shortcut sc;

		QByteArray a = query.value(0).toByteArray();

		{
			QDataStream s(&a, QIODevice::ReadOnly);
			s.setVersion(QDataStream::Qt_4_0);
			s >> sc.qlButtons;
		}

		a = query.value(1).toByteArray();

		{
			QDataStream s(&a, QIODevice::ReadOnly);
			s.setVersion(QDataStream::Qt_4_0);
			s >> sc.qvData;
		}

		sc.bSuppress=query.value(2).toBool();
		ql << sc;
	}
	return ql;
}

bool Database::setShortcuts(const QByteArray &digest, QList<Shortcut> &shortcuts) {
	QSqlQuery query;
	bool updated = false;

	query.prepare(QLatin1String("DELETE FROM `shortcut` WHERE `digest` = ?"));
	query.addBindValue(digest);
	query.exec();

	const QList<Shortcut> scs = shortcuts;

	query.prepare(QLatin1String("INSERT INTO `shortcut` (`digest`, `shortcut`, `target`, `suppress`) VALUES (?,?,?,?)"));
	foreach(const Shortcut &sc, scs) {
		if (sc.isServerSpecific()) {
			shortcuts.removeAll(sc);
			updated = true;

			query.addBindValue(digest);

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
			query.exec();
		}
	}
	return updated;
}

const QMap<QString, QString> Database::getFriends() {
	QMap<QString, QString> qm;
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT `name`, `hash` FROM `friends`"));
	query.exec();
	while (query.next())
		qm.insert(query.value(0).toString(), query.value(1).toString());
	return qm;
}

const QString Database::getFriend(const QString &hash) {
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT `name` FROM `friends` WHERE `hash` = ?"));
	query.addBindValue(hash);
	query.exec();
	if (query.next())
		return query.value(0).toString();
	return QString();
}

void Database::addFriend(const QString &name, const QString &hash) {
	QSqlQuery query;

	query.prepare(QLatin1String("REPLACE INTO `friends` (`name`, `hash`) VALUES (?,?)"));
	query.addBindValue(name);
	query.addBindValue(hash);
	query.exec();
}

void Database::removeFriend(const QString &hash) {
	QSqlQuery query;

	query.prepare(QLatin1String("DELETE FROM `friends` WHERE `hash` = ?"));
	query.addBindValue(hash);
	query.exec();
}

const QString Database::getDigest(const QString &hostname, unsigned short port) {
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT `digest` FROM `cert` WHERE `hostname` = ? AND `port` = ?"));
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.exec();
	if (query.next()) {
		return query.value(0).toString();
	}
	return QString();
}

void Database::setDigest(const QString &hostname, unsigned short port, const QString &digest) {
	QSqlQuery query;
	query.prepare(QLatin1String("REPLACE INTO `cert` (`hostname`,`port`,`digest`) VALUES (?,?,?)"));
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.addBindValue(digest);
	query.exec();
}

void Database::setPassword(const QString &hostname, unsigned short port, const QString &uname, const QString &pw) {
	QSqlQuery query;
	query.prepare(QLatin1String("UPDATE `servers` SET `password` = ? WHERE `hostname` = ? AND `port` = ? AND `username` = ?"));
	query.addBindValue(pw);
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.addBindValue(uname);
	query.exec();
}

bool Database::getUdp(const QByteArray &digest) {
	QSqlQuery query;
	query.prepare(QLatin1String("SELECT COUNT(*) FROM `udp` WHERE `digest` = ? "));
	query.addBindValue(digest);
	query.exec();
	if (query.next()) {
		return (query.value(0).toInt() == 0);
	}
	return true;
}

void Database::setUdp(const QByteArray &digest, bool udp) {
	QSqlQuery query;
	if (! udp)
		query.prepare(QLatin1String("REPLACE INTO `udp` (`digest`) VALUES (?)"));
	else
		query.prepare(QLatin1String("DELETE FROM `udp` WHERE `digest` = ?"));
	query.addBindValue(digest);
	query.exec();
}

bool Database::fuzzyMatch(QString &name, QString &user, QString &pw, QString &hostname, unsigned short port) {
	QSqlQuery query;
	if (! user.isEmpty()) {
		query.prepare(QLatin1String("SELECT `username`, `password`, `hostname`, `name` FROM `servers` WHERE `username` LIKE ? AND `hostname` LIKE ? AND `port`=?"));
		query.addBindValue(user);
	} else {
		query.prepare(QLatin1String("SELECT `username`, `password`, `hostname`, `name` FROM `servers` WHERE `hostname` LIKE ? AND `port`=?"));
	}
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.exec();
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
