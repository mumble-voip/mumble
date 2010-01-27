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

#include "ConnectDialog.h"
#include "Global.h"
#include "ServerHandler.h"
#include "Channel.h"

TextSortedItem::TextSortedItem(QTreeWidget *p, const QStringList &strings) : QTreeWidgetItem(p, strings) {
}

bool TextSortedItem::operator <(const QTreeWidgetItem &other) const {
	const QTreeWidget *w = treeWidget();

	int column = w ? w ->sortColumn() : 0;

	QString a = text(column).toLower();
	QString b = other.text(column).toLower();

	QRegExp re(QLatin1String("[^0-9a-z]"));
	a.remove(re);
	b.remove(re);
	return a < b;
}

QList<PublicInfo> ConnectDialog::qlPublicServers;
Timer ConnectDialog::tPublicServers;

ConnectDialog::ConnectDialog(QWidget *p) : QDialog(p) {

	qhList = new QHttp(QLatin1String("mumble.hive.no"), 80, this);
	qhList->setObjectName(QLatin1String("Request"));

	setupUi(this);
#ifdef Q_OS_MAC
	setWindowFlags(Qt::Sheet);
#endif

	bPublicInit = false;
	bDirty = false;

	if (tPublicServers.elapsed() >= 60 * 24 * 1000000ULL) {
		qlPublicServers.clear();
	}

	qtwServers->sortItems(0, Qt::AscendingOrder);

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

	if (g.sh && g.sh->isRunning()) {
		fillEmpty();
	} else {
		QModelIndex idx = qstmServers->index(g.s.iServerRow, 0);
		if (idx.isValid()) {
			qlwServers->setCurrentIndex(idx);
		} else if (qstmServers->rowCount() > 0) {
			qlwServers->setCurrentIndex(qstmServers->index(0,0));
		}
	}

	fillList();
	qtwTab->setCurrentIndex(0);

	if (qstmServers->rowCount() < 1) {
		on_qpbAdd_clicked();
		qtwTab->setCurrentIndex(1);
	}
}

void ConnectDialog::accept() {
	if (qtwTab->currentIndex() == 1) {
		QTreeWidgetItem *item = qtwServers->currentItem();
		if (! item)
			return;

		bool ok;
		QString defUserName = QInputDialog::getText(this, tr("Connecting to %1").arg(item->text(0)), tr("Enter username"), QLineEdit::Normal, g.s.qsUsername, &ok);
		if (! ok)
			return;

		g.s.qsUsername = defUserName;
		qsUsername = defUserName;
		qsPassword = QString();
		QStringList a = item->text(1).split(QLatin1Char(':'));
		qsServer = a.at(0);
		usPort = a.at(1).toUShort();
	} else {
		qsServer = qleServer->text();
		qsUsername = qleUsername->text();
		qsPassword = qlePassword->text();
		usPort = qlePort->text().toUShort();

		int row = qlwServers->currentIndex().row();
		g.s.iServerRow = (row >= 0) ? row : 0;
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

	if (name.isEmpty() || host.isEmpty() || user.isEmpty() || (port == 0)) {
		return QSqlRecord();
	}
	r.setValue(QLatin1String("name"), name);
	r.setValue(QLatin1String("hostname"), host);
	r.setValue(QLatin1String("port"), port);
	r.setValue(QLatin1String("username"), user);
	r.setValue(QLatin1String("password"), pw);
	return r;
}

void ConnectDialog::initList() {
	if (bPublicInit || (qlPublicServers.count() > 0))
		return;

	bPublicInit = true;

	qhList->get(QString::fromLatin1("/list.cgi?qcompress=1&version=%1").arg(QLatin1String(MUMBLE_RELEASE)));
}

void ConnectDialog::fillList() {
	qtwServers->clear();

	QList<QTreeWidgetItem *> ql;

	foreach(PublicInfo pi, qlPublicServers) {
		QStringList qsl;
		qsl << pi.name;
		qsl << QString::fromLatin1("%1:%2").arg(pi.ip).arg(pi.port);
		qsl << pi.url.toString();
		ql << new TextSortedItem(NULL,qsl);
	}
	qtwServers->addTopLevelItems(ql);
}

void ConnectDialog::on_qpbURL_clicked() {
	QTreeWidgetItem *item = qtwServers->currentItem();
	if (! item)
		return;
	QDesktopServices::openUrl(QUrl(item->text(2)));
}

void ConnectDialog::on_qpbCopy_clicked() {
	QTreeWidgetItem *item = qtwServers->currentItem();
	if (! item)
		return;

	qlwServers->setCurrentIndex(QModelIndex());

	QStringList a = item->text(1).split(QLatin1Char(':'));

	qleName->setText(item->text(0));
	qleServer->setText(a.at(0));
	if (g.s.qsUsername.isEmpty())
		qleUsername->setText(tr("Unknown"));
	else
		qleUsername->setText(g.s.qsUsername);
	qlePassword->setText(QString());
	qlePort->setText(a.at(1));

	qtwTab->setCurrentIndex(0);

	bDirty = true;
	on_qpbAdd_clicked();
}

void ConnectDialog::on_Request_done(bool err) {
	if (err) {
		QMessageBox::warning(this, QLatin1String("Mumble"), tr("Failed to fetch server list"), QMessageBox::Ok);
		return;
	}

	QDomDocument doc;
	doc.setContent(qUncompress(qhList->readAll()));

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
				pi.port = e.attribute(QLatin1String("port")).toUShort();
				qlPublicServers << pi;
			}
		}
		n = n.nextSibling();
	}

	tPublicServers.restart();

	fillList();
}

void ConnectDialog::onSelection_Changed(const QModelIndex &index, const QModelIndex &) {
	if (index.isValid()) {
		QSqlRecord r = qstmServers->record(index.row());
		qleName->setText(r.value(QLatin1String("name")).toString());
		qleServer->setText(r.value(QLatin1String("hostname")).toString());
		qleUsername->setText(r.value(QLatin1String("username")).toString());
		qlePassword->setText(r.value(QLatin1String("password")).toString());
		qlePort->setText(r.value(QLatin1String("port")).toString());
		bDirty = false;
		qpbRemove->setEnabled(true);
		qpbAdd->setEnabled(true);
		qpbAdd->setText(tr("New"));
	} else {
		qpbRemove->setEnabled(false);
	}
}

void ConnectDialog::on_qpbAdd_clicked() {
	if (bDirty) {
		bDirty = false;
		QSqlRecord r = toRecord();
		if (! r.isEmpty()) {
			const QModelIndex &previndex = qlwServers->currentIndex();
			if (previndex.row() >= 0) {
				// FIXME: setRecord is bugged in 4.4.1
				// qstmServers->setRecord(previndex.row(), r);
				qstmServers->removeRows(previndex.row(), 1, QModelIndex());
				qstmServers->insertRecord(-1, r);
			} else {
				qstmServers->insertRecord(-1, r);
			}
			qstmServers->submitAll();

			qpbAdd->setText(tr("New"));

			QSqlRecord rec;
			int nrec = qstmServers->rowCount();

			QStringList qslFields;
			qslFields << QLatin1String("name");
			qslFields << QLatin1String("hostname");
			qslFields << QLatin1String("username");
			qslFields << QLatin1String("password");
			qslFields << QLatin1String("port");

			for (int i=0; i < nrec;++i) {
				rec = qstmServers->record(i);
				bool match = true;
				foreach(const QString &s, qslFields) {
					if (rec.value(s) != r.value(s))
						match = false;
				}
				if (match) {
					qlwServers->setCurrentIndex(qstmServers->index(i, 0));
					break;
				}
			}

			return;
		} else {
			return;
		}
	}

	fillEmpty();
}

void ConnectDialog::fillEmpty() {
	qlwServers->setCurrentIndex(QModelIndex());

	QString host, user, pw;
	QString name = tr("-Unnamed entry-");
	unsigned short port = 64738;
	bool ready = false;

	if (g.sh && g.sh->isRunning()) {
		g.sh->getConnectionInfo(host, port, user, pw);
		Channel *c = Channel::get(0);
		if (c) {
			if (c->qsName != QLatin1String("Root")) {
				name = c->qsName;
				ready = true;
			}
		}
	} else {
		user = g.s.qsUsername;
		pw = QString();
		host = QLatin1String("host");
	}
	qleName->setText(name);
	qleServer->setText(host);
	qleUsername->setText(user);
	qlePassword->setText(pw);
	qlePort->setText(QString::number(port));

	qpbAdd->setText(tr("Add"));
	qpbAdd->setEnabled(ready);

	bDirty = ready;
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

	fillEmpty();
}

void ConnectDialog::onDirty(const QString &) {
	const QModelIndex &previndex = qlwServers->currentIndex();

	QSqlRecord r = toRecord();
	qpbAdd->setEnabled(! r.isEmpty());
	if (previndex.row() >= 0)
		qpbAdd->setText(tr("Update"));
	else
		qpbAdd->setText(tr("Add"));
	bDirty = true;
}

void ConnectDialog::on_qtwTab_currentChanged(int idx) {
	if (idx != 1)
		return;

	initList();
}
