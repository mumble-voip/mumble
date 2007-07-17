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

#include "Database.h"
#include "Global.h"

Database::Database() {
	QSqlDatabase db = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"));
	QSettings qs;
	QStringList datapaths;
	int i;

	datapaths << qs.value(QLatin1String("InstPath")).toString();
	datapaths << QDir::homePath();
	datapaths << qApp->applicationDirPath();
	datapaths << QDir::currentPath();
	bool found = false;

	for(i = 0; (i < datapaths.size()) && ! found; i++) {
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
		for(i = 0; (i < datapaths.size()) && ! found; i++) {
			if (!datapaths[i].isEmpty()) {
				QFile f(datapaths[i] + QLatin1String("/.mumble.sqlite"));
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
	// query.exec("DROP TABLE servers");
	query.exec(QLatin1String("CREATE TABLE servers (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, hostname TEXT, port INTEGER DEFAULT 64738, username TEXT, password TEXT)"));
	query.exec(QLatin1String("ALTER TABLE servers ADD COLUMN udp INTEGER DEFAULT 1"));

	query.exec(QLatin1String("CREATE TABLE cert (id INTEGER PRIMARY KEY AUTOINCREMENT, hostname TEXT, port INTEGER, digest TEST)"));
	query.exec(QLatin1String("CREATE UNIQUE INDEX cert_host_port ON cert(hostname,port)"));
}

const QString Database::getDigest(const QString &hostname, unsigned short port) {
    QSqlQuery query;

    query.prepare("SELECT digest FROM cert WHERE hostname = ? AND port = ?");
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
    query.prepare("REPLACE INTO cert (hostname,port,digest) VALUES (?,?,?)");
    query.addBindValue(hostname);
    query.addBindValue(port);
    query.addBindValue(digest);
    query.exec();
}
