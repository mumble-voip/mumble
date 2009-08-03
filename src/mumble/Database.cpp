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

#include "Database.h"
#include "Global.h"

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
		QMessageBox::critical(NULL, tr("Mumble"), tr("Mumble failed to initialize a database in any\nof the possible locations."), QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
		qFatal("Database: Failed initialization");
	}

	QSqlQuery query;

	// FIXME: Oops. These were unique. Leave them in for people using the snapshots for a few releases.
	query.exec(QLatin1String("DROP INDEX IF EXISTS `tokens_host_port`"));
	query.exec(QLatin1String("DROP TABLE IF EXISTS `shortcut_host_port`"));

	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `servers` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `name` TEXT, `hostname` TEXT, `port` INTEGER DEFAULT 64738, `username` TEXT, `password` TEXT)"));
	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `comments` (`comment` BLOB, `seen` DATE)"));
	query.exec(QLatin1String("CREATE UNIQUIE INDEX IF NOT EXISTS `comments_comment` ON `comments`(`comment`)"));
	query.exec(QLatin1String("CREATE INDEX IF NOT EXISTS `comments_seen` ON `comments`(`seen`)"));
	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `tokens` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `hostname` TEXT, `port` INTEGER, `token` TEXT)"));
	query.exec(QLatin1String("CREATE INDEX IF NOT EXISTS `tokens_host_port` ON `tokens`(`hostname`,`port`)"));
	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `shortcut` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `hostname` TEXT, `port` INTEGER, `shortcut` BLOB, `target` BLOB, `suppress` INTEGER)"));
	query.exec(QLatin1String("CREATE INDEX IF NOT EXISTS `shortcut_host_port` ON `shortcut`(`hostname`,`port`)"));
	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `cert` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `hostname` TEXT, `port` INTEGER, `digest` TEXT)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `cert_host_port` ON `cert`(`hostname`,`port`)"));
	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `udp` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `hostname` TEXT, `port` INTEGER)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `udp_host_port` ON `udp`(`hostname`,`port`)"));
	query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS `friends` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `name` TEXT, `hash` TEXT)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `friends_name` ON `friends`(`name`)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX IF NOT EXISTS `friends_hash` ON `friends`(`hash`)"));

	query.exec(QLatin1String("DELETE FROM `comments` WHERE `seen` < datetime('now', '-1 years')"));
}

bool Database::seenComment(const QString &comment) {
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT COUNT(*) FROM `comments` WHERE `comment` = ?"));
	query.addBindValue(QCryptographicHash::hash(comment.toUtf8(), QCryptographicHash::Sha1));
	query.exec();
	if (query.next()) {
		return (query.value(0).toInt() > 0);
	}
	return false;
}

void Database::setSeenComment(const QString &comment) {
	QSqlQuery query;

	query.prepare(QLatin1String("REPLACE INTO `comments` (`comment`, `seen`) VALUES (?, datetime('now'))"));
	query.addBindValue(QCryptographicHash::hash(comment.toUtf8(), QCryptographicHash::Sha1));
	query.exec();
}

QStringList Database::getTokens(const QString &hostname, unsigned short port) {
	QList<QString> qsl;
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT `token` FROM `tokens` WHERE `hostname` = ? AND `port` = ?"));
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.exec();
	while (query.next()) {
		qsl << query.value(0).toString();
	}
	return qsl;
}

void Database::setTokens(const QString &hostname, unsigned short port, QStringList &tokens) {
	QSqlQuery query;

	query.prepare(QLatin1String("DELETE FROM `tokens` WHERE `hostname` = ? AND `port` = ?"));
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.exec();

	query.prepare(QLatin1String("INSERT INTO `tokens` (`hostname`, `port`, `token`) VALUES (?,?,?)"));
	foreach(const QString &qs, tokens) {
		query.addBindValue(hostname);
		query.addBindValue(port);
		query.addBindValue(qs);
		query.exec();
	}
}

QList<Shortcut> Database::getShortcuts(const QString &hostname, unsigned short port) {
	QList<Shortcut> ql;
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT `shortcut`,`target`,`suppress` FROM `shortcut` WHERE `hostname` = ? AND `port` = ?"));
	query.addBindValue(hostname);
	query.addBindValue(port);
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

bool Database::setShortcuts(const QString &hostname, unsigned short port, QList<Shortcut> &shortcuts) {
	QSqlQuery query;
	bool updated = false;

	query.prepare(QLatin1String("DELETE FROM `shortcut` WHERE `hostname` = ? AND `port` = ?"));
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.exec();

	const QList<Shortcut> scs = shortcuts;

	query.prepare(QLatin1String("INSERT INTO `shortcut` (`hostname`, `port`, `shortcut`, `target`, `suppress`) VALUES (?,?,?,?,?)"));
	foreach(const Shortcut &sc, scs) {
		if (sc.isServerSpecific()) {
			shortcuts.removeAll(sc);
			updated = true;

			query.addBindValue(hostname);
			query.addBindValue(port);

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

bool Database::getUdp(const QString &hostname, unsigned short port) {
	QSqlQuery query;
	query.prepare(QLatin1String("SELECT COUNT(*) FROM `udp` WHERE `hostname` = ? AND `port` = ?"));
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.exec();
	if (query.next()) {
		return (query.value(0).toInt() == 0);
	}
	return true;
}

void Database::setUdp(const QString &hostname, unsigned short port, bool udp) {
	QSqlQuery query;
	if (! udp)
		query.prepare(QLatin1String("REPLACE INTO `udp` (`hostname`,`port`) VALUES (?,?)"));
	else
		query.prepare(QLatin1String("DELETE FROM `udp` WHERE `hostname` = ? AND `port` = ?"));
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.exec();
}

bool Database::fuzzyMatch(QString &user, QString &pw, QString &hostname, unsigned short port) {
	QSqlQuery query;
	if (! user.isEmpty()) {
		query.prepare(QLatin1String("SELECT `username`, `password`, `hostname` FROM `servers` WHERE `username` LIKE ? AND `hostname` LIKE ? AND `port`=?"));
		query.addBindValue(user);
	} else {
		query.prepare(QLatin1String("SELECT `username`, `password`, `hostname` FROM `servers` WHERE `hostname` LIKE ? AND `port`=?"));
	}
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.exec();
	if (query.next()) {
		user = query.value(0).toString();
		if (pw.isEmpty())
			pw = query.value(1).toString();
		hostname = query.value(2).toString();
		return true;
	} else {
		return false;
	}
}
