/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#include <QApplication>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QStringList>
#include <QSettings>
#include <QFile>
#include <QDir>

Database *g_db;

Database::Database() {
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	QSettings qs;
	QStringList datapaths;
	int i;

	datapaths << qs.value("InstPath").toString();
	datapaths << QDir::homePath();
	datapaths << qApp->applicationDirPath();
	datapaths << QDir::currentPath();
	bool found = false;

	for(i = 0; (i < datapaths.size()) && ! found; i++) {
		if (!datapaths[i].isEmpty()) {
			QFile f(datapaths[i] + "/mumble.sqlite");
			if (f.exists()) {
				db.setDatabaseName(f.fileName());
				found = db.open();
			}
		}
	}

	if (! found) {
		for(i = 0; (i < datapaths.size()) && ! found; i++) {
			if (!datapaths[i].isEmpty()) {
				QFile f(datapaths[i] + "/mumble.sqlite");
				db.setDatabaseName(f.fileName());
				found = db.open();
			}
		}
	}

	if (! found) {
		QMessageBox::critical(NULL, "Database failed", "Mumble failed to initialize a database in any\nof the possible locations.", QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton);
		qFatal("Database: Failed initialization");
	}
}
