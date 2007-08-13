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

#include "ConnectDialog.h"
#include "Global.h"

QList<PublicInfo> ConnectDialog::qlPublicServers;

ConnectDialog::ConnectDialog(QWidget *p) : QDialog(p) {

	qhList = new QHttp(QLatin1String("mumble.hive.no"), 80, this);
	qhList->setObjectName(QLatin1String("Request"));

	setupUi(this);

	bPublicInit = false;
	bDirty = false;

	qstmServers = new QSqlTableModel(this);
	qstmServers->setTable(QLatin1String("servers"));
	qstmServers->setSort(1, Qt::AscendingOrder);
	if (! qstmServers->select()) {
		qWarning("ConnectDialog: Failed to reselect table");
	}
	qstmServers->setEditStrategy(QSqlTableModel::OnManualSubmit);

	qlwServers->setModel(qstmServers);
	qlwServers->setModelColumn(1);

	QItemSelectionModel *selectionModel = qlwServers->selectionModel();

	connect(selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(onSelection_Changed(const QModelIndex &, const QModelIndex &)));
	connect(qleName, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));
	connect(qleServer, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));
	connect(qleUsername, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));
	connect(qlePassword, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));
	connect(qlePort, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));

	qlePort->setValidator(new QIntValidator(1, 65535, qlePort));

	QModelIndex idx = qstmServers->index(g.qs->value(QLatin1String("ServerRow"),-1).toInt(),0);
	if (idx.isValid()) {
		qlwServers->setCurrentIndex(idx);
	}


	if (qstmServers->rowCount() < 1) {
		qtwTab->setCurrentIndex(1);
	}
}

void ConnectDialog::accept() {
	if (qtwTab->currentIndex() == 1) {
		int row = qtwServers->currentRow();
		if (row == -1)
			return;

		bool ok;
		QString defUserName = g.qs->value(QLatin1String("defUserName")).toString();
		defUserName = QInputDialog::getText(this, tr("Connecting to %1").arg(qtwServers->item(row, 0)->text()), tr("Enter username"), QLineEdit::Normal, defUserName, &ok);
		if (! ok)
			return;

		g.qs->setValue(QLatin1String("defUserName"), defUserName);
		qsUsername = defUserName;
		qsPassword = QString();
		QStringList a = qtwServers->item(row, 1)->text().split(QLatin1Char(':'));
		qsServer = a.at(0);
		iPort = a.at(1).toInt();
	} else {
		if (bDirty) {
			QSqlRecord r;
			r = toRecord();
			if (! r.isEmpty()) {
				if (qlwServers->currentIndex().row() >= 0)
					qstmServers->setRecord(qlwServers->currentIndex().row(), r);
				else
					qstmServers->insertRecord(-1, r);
				qstmServers->submitAll();
			}
		}

		qsServer = qleServer->text();
		qsUsername = qleUsername->text();
		qsPassword = qlePassword->text();
		iPort = qlePort->text().toInt();

		int row = qlwServers->currentIndex().row();

		g.qs->setValue(QLatin1String("ServerRow"), row >= 0 ? row : 0);
	}
	QDialog::accept();
}

QSqlRecord ConnectDialog::toRecord() const {
	QSqlRecord r = qstmServers->record();

	QString name = qleName->text();
	QString host = qleServer->text();
	QString user = qleUsername->text();
	QString pw = qlePassword->text();
	int port = qlePort->text().toInt();

	if (name.isEmpty() || host.isEmpty() || user.isEmpty() || (port == 0))
		return r;
	r.setValue(QLatin1String("name"), name);
	r.setValue(QLatin1String("hostname"), host);
	r.setValue(QLatin1String("username"), user);
	r.setValue(QLatin1String("password"), pw);
	r.setValue(QLatin1String("port"), port);
	return r;
}

void ConnectDialog::initList() {
	if (bPublicInit || (qlPublicServers.count() > 0))
		return;

	bPublicInit = true;

	qhList->get(QString::fromLatin1("/list.cgi?version=%1").arg(QLatin1String(MUMBLE_RELEASE)));
}

void ConnectDialog::fillList() {
	for (int i=0;i<qtwServers->rowCount();i++)
		qtwServers->removeRow(0);

	qtwServers->setSortingEnabled(false);

	foreach(PublicInfo pi, qlPublicServers) {
		qtwServers->insertRow(0);
		qtwServers->setItem(0, 0, new QTableWidgetItem(pi.name));
		qtwServers->setItem(0, 1, new QTableWidgetItem(QString::fromLatin1("%1:%2").arg(pi.ip).arg(pi.port)));
		qtwServers->setItem(0, 2, new QTableWidgetItem(pi.url.toString()));
	}
	qtwServers->resizeColumnsToContents();
	qtwServers->setSortingEnabled(true);
}

void ConnectDialog::on_qpbURL_clicked() {
	int row=qtwServers->currentRow();
	if (row == -1)
		return;

	QDesktopServices::openUrl(QUrl(qtwServers->item(row, 2)->text()));
}

void ConnectDialog::on_qpbCopy_clicked() {
	int row=qtwServers->currentRow();
	if (row == -1)
		return;

	QStringList a = qtwServers->item(row, 1)->text().split(QLatin1Char(':'));

	qleName->setText(qtwServers->item(row, 0)->text());
	qleServer->setText(a.at(0));
	qleUsername->setText(g.qs->value(QLatin1String("defUserName")).toString());
	qlePassword->setText(QString());
	qlePort->setText(a.at(1));

	qtwTab->setCurrentIndex(0);

	on_qpbAdd_clicked();
}

void ConnectDialog::on_Request_done(bool err) {
	if (err) {
		QMessageBox::warning(this, tr("Mumble"), tr("Failed to fetch server list"), QMessageBox::Ok);
		return;
	}

	QDomDocument doc;
	doc.setContent(qhList->readAll());

	qlPublicServers.clear();

	QDomElement root=doc.documentElement();
	QDomNode n = root.firstChild();
	while (!n.isNull()) {
		QDomElement e = n.toElement();
		if (!e.isNull()) {
			if (e.tagName() == QLatin1String("server")) {
				PublicInfo pi;
				pi.name = e.attribute(QLatin1String("name"));
				pi.url = e.attribute(QLatin1String("url"));
				pi.ip = e.attribute(QLatin1String("ip"));
				pi.port = e.attribute(QLatin1String("port")).toInt();
				qlPublicServers << pi;
			}
		}
		n = n.nextSibling();
	}

	fillList();
}

void ConnectDialog::onSelection_Changed(const QModelIndex &index, const QModelIndex &previndex) {
	QSqlRecord r;

	if (bDirty) {
		bDirty = false;
		r = toRecord();
		if (! r.isEmpty()) {
			if (previndex.row() >= 0)
				qstmServers->setRecord(previndex.row(), r);
			else
				qstmServers->insertRecord(-1, r);
			qstmServers->submitAll();
		}
	}
	if (index.isValid()) {
		r = qstmServers->record(index.row());
		qleName->setText(r.value(QLatin1String("name")).toString());
		qleServer->setText(r.value(QLatin1String("hostname")).toString());
		qleUsername->setText(r.value(QLatin1String("username")).toString());
		qlePassword->setText(r.value(QLatin1String("password")).toString());
		qlePort->setText(r.value(QLatin1String("port")).toString());
		bDirty = false;
	}
}

void ConnectDialog::on_qpbAdd_clicked() {
	if (bDirty) {
		bDirty = false;
		QSqlRecord r = toRecord();
		if (! r.isEmpty()) {
			qstmServers->setRecord(qlwServers->currentIndex().row(), r);
			qstmServers->submitAll();
		}
	}

	qleName->setText(tr("-Unnamed entry-"));
	qleServer->setText(QString());
	qleUsername->setText(g.qs->value(QLatin1String("defUserName")).toString());
	qlePassword->setText(QString());
	qlePort->setText(QLatin1String("64738"));

	bDirty = false;
}

void ConnectDialog::on_qpbRemove_clicked() {
	int row = qlwServers->currentIndex().row();
	if (row < 0)
		return;
	if (!qstmServers->removeRows(row, 1, QModelIndex())) {
		qWarning("Failed row remove: %s", qPrintable(qstmServers->lastError().text()));
	}
	qstmServers->submitAll();
	qstmServers->select();
}

void ConnectDialog::onDirty(const QString &) {
	bDirty = true;
}

void ConnectDialog::on_qtwTab_currentChanged(int idx) {
	if (idx != 1)
		return;

	initList();
	fillList();
}
