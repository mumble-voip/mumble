/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#if defined(Q_OS_WIN)
	// Check if running with local .ini file (Roaming mode)
	if (g.qs->fileName().endsWith(QLatin1String("mumble.ini"), Qt::CaseInsensitive))
		datapaths << qApp->applicationDirPath();
	else if (g.qs->fileName().endsWith(QLatin1String("mumble11x.ini"), Qt::CaseInsensitive))
		datapaths << qApp->applicationDirPath();

	size_t reqSize;
	_wgetenv_s(&reqSize, NULL, 0, L"APPDATA");
	if (reqSize > 0) {
		STACKVAR(wchar_t, buff, reqSize+1);
		_wgetenv_s(&reqSize, buff, reqSize, L"APPDATA");

		QDir appdir = QDir(QDir::fromNativeSeparators(QString::fromWCharArray(buff)));

		datapaths << appdir.absolutePath() + QLatin1String("/Mumble");
	}
#elif defined(Q_OS_MAC)
	datapaths << QDir::homePath() + QLatin1String("/Library/Preferences/Mumble/");
#else
	datapaths << QDir::homePath() + QLatin1String("/.config/Mumble");
#endif

	datapaths << QDir::homePath();
	datapaths << QDir::currentPath();
	datapaths << qApp->applicationDirPath();
	datapaths << qs.value(QLatin1String("InstPath")).toString();
	bool found = false;

	for (i = 0; (i < datapaths.size()) && ! found; i++) {
		if (!datapaths[i].isEmpty()) {
			{
				QFile f(datapaths[i] + QLatin1String("/mumble11x.sqlite"));
				if (f.exists()) {
					db.setDatabaseName(f.fileName());
					found = db.open();
				}
			}

			{
				QFile f(datapaths[i] + QLatin1String("/.mumble11x.sqlite"));
				if (f.exists()) {
					db.setDatabaseName(f.fileName());
					found = db.open();
				}
			}

			{
				QFile f(datapaths[i] + QLatin1String("/mumble.sqlite"));
				if (f.exists()) {
					QFile f2(datapaths[i] + QLatin1String("/mumble11x.sqlite"));
					if (QFile::copy(f.fileName(), f2.fileName())) {
						db.setDatabaseName(f2.fileName());
						found = db.open();
					}
				}
			}

			{
				QFile f(datapaths[i] + QLatin1String("/.mumble.sqlite"));
				if (f.exists()) {
					QFile f2(datapaths[i] + QLatin1String("/.mumble11x.sqlite"));
					if (QFile::copy(f.fileName(), f2.fileName())) {
						db.setDatabaseName(f2.fileName());
						found = db.open();
					}
				}
			}
		}
	}

	if (! found) {
		for (i = 0; (i < datapaths.size()) && ! found; i++) {
			if (!datapaths[i].isEmpty()) {
				QDir::root().mkpath(datapaths[i]);
#ifdef Q_OS_WIN
				QFile f(datapaths[i] + QLatin1String("/mumble11x.sqlite"));
#else
				QFile f(datapaths[i] + QLatin1String("/.mumble11x.sqlite"));
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

	QSqlQuery query;
	// query.exec("DROP TABLE servers");
	query.exec(QLatin1String("CREATE TABLE servers (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, hostname TEXT, port INTEGER DEFAULT 64738, username TEXT, password TEXT)"));
	query.exec(QLatin1String("CREATE TABLE cert (id INTEGER PRIMARY KEY AUTOINCREMENT, hostname TEXT, port INTEGER, digest TEXT)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX cert_host_port ON cert(hostname,port)"));
}

const QString Database::getDigest(const QString &hostname, unsigned short port) {
	QSqlQuery query;

	query.prepare(QLatin1String("SELECT digest FROM cert WHERE hostname = ? AND port = ?"));
	query.addBindValue(hostname);;
	query.addBindValue(port);
	query.exec();
	if (query.next()) {
		return query.value(0).toString();
	}
	return QString();
}

void Database::setDigest(const QString &hostname, unsigned short port, const QString &digest) {
	QSqlQuery query;
	query.prepare(QLatin1String("REPLACE INTO cert (hostname,port,digest) VALUES (?,?,?)"));
	query.addBindValue(hostname);
	query.addBindValue(port);
	query.addBindValue(digest);
	query.exec();
}

bool Database::fuzzyMatch(QString &user, QString &pw, QString &hostname, unsigned short port) {
	QSqlQuery query;
	if (! user.isEmpty()) {
		query.prepare(QLatin1String("SELECT username, password, hostname FROM servers WHERE username LIKE ? AND hostname LIKE ? AND port=?"));
		query.addBindValue(user);
	} else {
		query.prepare(QLatin1String("SELECT username, password, hostname FROM servers WHERE hostname LIKE ? AND port=?"));
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
