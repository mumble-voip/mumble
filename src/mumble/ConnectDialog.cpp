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
int ConnectDialog::iPingIndex = -1;

ConnectDialog::ConnectDialog(QWidget *p) : QDialog(p) {
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
	qtwServers->header()->setResizeMode(2, QHeaderView::ResizeToContents);
	qtwServers->header()->setResizeMode(3, QHeaderView::ResizeToContents);
	qtwServers->header()->setResizeMode(4, QHeaderView::ResizeToContents);
	qtwServers->header()->setResizeMode(5, QHeaderView::Stretch);

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
	connect(qlePort, SIGNAL(textEdited(const QString &)), this, SLOT(onDirty(const QString &)));
#ifdef USE_BONJOUR
	bCopyOnResolve = false;
	bResolving = false;
	connect(qtwLanServers, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_qpbLanBrowserConnect_clicked()));
#endif

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

	qtwTab->setTabEnabled(2, false);

#ifdef USE_BONJOUR
	// Make sure the we got the objects we need, then wire them up
	if (g.bc->bsbBrowser == NULL || g.bc->bsrResolver == NULL) {
		qtwTab->setTabToolTip(2, tr("Bonjour runtime not installed."));
	} else {
		qtwTab->setTabToolTip(2, tr("No LAN servers found."));
		connect(g.bc->bsbBrowser, SIGNAL(error(DNSServiceErrorType)),
		        this, SLOT(onLanBrowseError(DNSServiceErrorType)));
		connect(g.bc->bsbBrowser, SIGNAL(currentBonjourRecordsChanged(const QList<BonjourRecord> &)),
		        this, SLOT(onUpdateLanList(const QList<BonjourRecord> &)));
		connect(g.bc->bsrResolver, SIGNAL(error(DNSServiceErrorType)),
		        this, SLOT(onLanResolveError(DNSServiceErrorType)));
		connect(g.bc->bsrResolver, SIGNAL(bonjourRecordResolved(const QHostInfo &, int)),
		        this, SLOT(accept(const QHostInfo &, int)));
		onUpdateLanList(g.bc->bsbBrowser->currentRecords());
	}
#else
	qtwTab->setTabToolTip(2, tr("Bonjour support disabled during compilation."));
#endif

	qtPingTick = new QTimer(this);
	connect(qtPingTick, SIGNAL(timeout()), this, SLOT(timeTick()));

	qusSocket4 = new QUdpSocket(this);
	qusSocket6 = new QUdpSocket(this);
	bIPv4 = qusSocket4->bind(QHostAddress(QHostAddress::Any), 0);
	bIPv6 = qusSocket6->bind(QHostAddress(QHostAddress::AnyIPv6), 0);
	connect(qusSocket4, SIGNAL(readyRead()), this, SLOT(udpReply()));
	connect(qusSocket6, SIGNAL(readyRead()), this, SLOT(udpReply()));

	for(int i=0;i<qlPublicServers.count(); ++i) {
		PublicInfo &info = qlPublicServers[i];
		info.tsiItem = NULL;
	}

	fillList();
	if (qstmServers->rowCount() < 1) {
		on_qpbAdd_clicked();
		qtwTab->setCurrentIndex(qtwTab->isTabEnabled(2) ? 2 : 1);
	} else {
		qtwTab->setCurrentIndex(0);
	}
}

void ConnectDialog::accept() {
	if (qtwTab->currentIndex() == 1) {
		QTreeWidgetItem *item = qtwServers->currentItem();
		if (! item)
			return;

		bool ok;
		QString defUserName = QInputDialog::getText(this, tr("Connecting to %1").arg(item->text(0)), tr("Enter username"), QLineEdit::Normal, g.s.qsUsername, &ok).trimmed();
		if (! ok)
			return;

		g.s.qsUsername = defUserName;
		qsUsername = defUserName;
		qsPassword = QString();
		QStringList a = item->text(1).split(QLatin1Char(':'));
		qsServer = a.at(0);
		usPort = a.at(1).toUShort();
	} else {
		qsServer = qleServer->text().trimmed();
		qsUsername = qleUsername->text().trimmed();
		usPort = qlePort->text().trimmed().toUShort();

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
	int port = qlePort->text().toInt();

	if (name.isEmpty() || host.isEmpty() || user.isEmpty() || (port == 0)) {
		return QSqlRecord();
	}
	r.setValue(QLatin1String("name"), name);
	r.setValue(QLatin1String("hostname"), host);
	r.setValue(QLatin1String("port"), port);
	r.setValue(QLatin1String("username"), user);
	r.setValue(QLatin1String("password"), qsPassword);
	return r;
}

void ConnectDialog::initList() {
	if (bPublicInit || (qlPublicServers.count() > 0))
		return;

	bPublicInit = true;

	qhList = new QHttp(QLatin1String("mumble.hive.no"), 80, this);
	qhList->setObjectName(QLatin1String("Request"));

	QUrl url(QLatin1String("http://mumble.hive.no/list2.cgi"));
	url.addQueryItem(QLatin1String("version"), QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)));

	QNetworkRequest req(url);
	QNetworkReply *rep = g.nam->get(req);
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));
}

#ifdef USE_BONJOUR
void ConnectDialog::accept(const QHostInfo &host, int port) {
	if (!bResolving) return;

	QTreeWidgetItem *item = qtwLanServers->currentItem();
	if (! item)
		return;

	qpbLanBrowserConnect->setEnabled(true);
	qpbLanBrowserCopy->setEnabled(true);
	qtwLanServers->setEnabled(true);
	qtwTab->setTabEnabled(0, true);
	qtwTab->setTabEnabled(1, true);

	const QList<QHostAddress> &addrs = host.addresses();
	if (addrs.isEmpty()) return;

	QHostAddress addr(addrs.first());

	bResolving = false;
	if (bCopyOnResolve) {
		bCopyOnResolve = false;
		qlwServers->setCurrentIndex(QModelIndex());

		QStringList a = item->text(1).split(QLatin1Char(':'));

		qleName->setText(item->text(0));
		qleServer->setText(addr.toString());
		if (g.s.qsUsername.isEmpty())
			qleUsername->setText(tr("Unknown"));
		else
			qleUsername->setText(g.s.qsUsername);
		qlePort->setText(QVariant(port).toString());

		qtwTab->setCurrentIndex(0);

		bDirty = true;
		on_qpbAdd_clicked();
	} else {
		bool ok;
		QString defUserName = QInputDialog::getText(this, tr("Connecting to %1").arg(item->text(0)), tr("Enter username"), QLineEdit::Normal, g.s.qsUsername, &ok).trimmed();
		if (! ok)
			return;
		g.s.qsUsername = defUserName;
		qsUsername = defUserName;
		qsPassword = QString();
		qsServer = addr.toString();
		usPort = static_cast<unsigned short>(port);

		QDialog::accept();
	}
}

void ConnectDialog::onUpdateLanList(const QList<BonjourRecord> &list) {
	qtwLanServers->clear();

	foreach(BonjourRecord record, list) {

		QVariant hrecord;
		hrecord.setValue(record);
		QTreeWidgetItem *tmp = new QTreeWidgetItem(qtwLanServers, QStringList() << record.serviceName);
		tmp->setData(0, Qt::UserRole, hrecord);
	}

	if (! list.isEmpty()) {
		qtwTab->setTabEnabled(2, true);
		qtwTab->setTabToolTip(2, QString());
	}
}

void ConnectDialog::onLanBrowseError(DNSServiceErrorType err) {
	qWarning()<<"Bonjour reported browser error "<< err;
}

void ConnectDialog::onLanResolveError(DNSServiceErrorType err) {
	if (bResolving) {
		qpbLanBrowserConnect->setEnabled(true);
		qpbLanBrowserCopy->setEnabled(true);
		qpbLanBrowserCopy->setEnabled(true);
		qtwLanServers->setEnabled(true);
		qtwTab->setTabEnabled(0, true);
		qtwTab->setTabEnabled(1, true);
		bResolving = false; // Make sure we don't get stuck due to failed resolving
	}
	qWarning()<<"Bonjour reported resolver error "<< err;
}

void ConnectDialog::on_qpbLanBrowserConnect_clicked() {
	if (bResolving) return;
	QTreeWidgetItem *item = qtwLanServers->currentItem();
	if (! item)
		return;

	QVariant hrecord = item->data(0, Qt::UserRole);
	g.bc->bsrResolver->resolveBonjourRecord(hrecord.value<BonjourRecord>());
	bResolving = true;
	qpbLanBrowserConnect->setDisabled(true);
	qpbLanBrowserCopy->setDisabled(true);
	qtwLanServers->setDisabled(true);
	qtwTab->setTabEnabled(0, false);
	qtwTab->setTabEnabled(1, false);
}


void ConnectDialog::on_qpbLanBrowserCopy_clicked() {
	if (bResolving) return;
	bCopyOnResolve = true;
	QTreeWidgetItem *item = qtwLanServers->currentItem();
	if (! item)
		return;

	QVariant hrecord = item->data(0, Qt::UserRole);
	g.bc->bsrResolver->resolveBonjourRecord(hrecord.value<BonjourRecord>());
	bResolving = true;
	qpbLanBrowserConnect->setDisabled(true);
	qpbLanBrowserCopy->setDisabled(true);
	qtwLanServers->setDisabled(true);
	qtwTab->setTabEnabled(0, false);
	qtwTab->setTabEnabled(1, false);
}
#endif

void ConnectDialog::fillList() {
	QList<QTreeWidgetItem *> ql;

	for(int i=0;i<qlPublicServers.count(); ++i) {
		PublicInfo &pi = qlPublicServers[i];

		QStringList qsl;
		qsl << pi.qsName;
		qsl << QString::fromLatin1("%1:%2").arg(pi.qsIp).arg(pi.usPort);
		qsl << pi.qsCountry;
		qsl << (pi.uiPing ? QString::number(pi.uiPing / 1000) : QString());
		qsl << (pi.uiUsers ? QString::number(pi.uiUsers) : QString());
		qsl << pi.quUrl.toString();

		TextSortedItem *tsi = new TextSortedItem(NULL, qsl);
		if (! pi.qsCountryCode.isEmpty())
			tsi->setIcon(2, QIcon(QString::fromLatin1(":/flags/%1.png").arg(pi.qsCountryCode)));

		pi.tsiItem = tsi;

		ql << tsi;
	}
	qtwServers->clear();
	qtwServers->addTopLevelItems(ql);
}

void ConnectDialog::pingList() {
	iPingIndex = 0;
	qtPingTick->start(50);
}

void ConnectDialog::timeTick() {
	if (qlPublicServers.isEmpty())
		return;

	if (++iPingIndex >= qlPublicServers.count())
		iPingIndex = 0;

	const PublicInfo &pi = qlPublicServers.at(iPingIndex);

	QHostAddress qha(pi.qsIp);
	if (! qha.isNull())
		sendPing(iPingIndex, qha, pi.usPort);
	else
		qmLookups.insert(QHostInfo::lookupHost(pi.qsIp, this, SLOT(lookedUp(QHostInfo))), iPingIndex);
}

void ConnectDialog::lookedUp(QHostInfo info) {
	int id = info.lookupId();
	if (! qmLookups.contains(id))
		return;
	int index = qmLookups.value(id);
	qmLookups.remove(id);

	if (info.error() != QHostInfo::NoError)
		return;

	if (index >= qlPublicServers.count())
		return;

	PublicInfo &pi = qlPublicServers[index];

	foreach(const QHostAddress &host, info.addresses())
		sendPing(index, host, pi.usPort);
}

void ConnectDialog::sendPing(int index, const QHostAddress &host, unsigned short port) {
	char blob[16];

	memset(blob, 0, sizeof(blob));
	* reinterpret_cast<quint64 *>(blob+8) = tPing.elapsed();

	qmActivePings.insert(qpAddress(host, port), index);

	if (bIPv4 && host.protocol() == QAbstractSocket::IPv4Protocol)
		qusSocket4->writeDatagram(blob+4, 12, host, port);
	else if (bIPv6 && host.protocol() == QAbstractSocket::IPv6Protocol)
		qusSocket6->writeDatagram(blob+4, 12, host, port);
}

void ConnectDialog::udpReply() {
	QUdpSocket *sock = qobject_cast<QUdpSocket *>(sender());
	while (sock->hasPendingDatagrams()) {
		char blob[20];

		QHostAddress host;
		unsigned short port;

		int len = sock->readDatagram(blob+4, 16, &host, &port);
		if (len == 16) {
			qpAddress address(host, port);
			if (qmActivePings.contains(address)) {
				int index = qmActivePings.value(address);
				qmActivePings.remove(address);

				if (index >= qlPublicServers.count())
					continue;

				PublicInfo &pi = qlPublicServers[index];

				quint32 *ping = reinterpret_cast<quint32 *>(blob+4);
				quint64 *ts = reinterpret_cast<quint64 *>(blob+8);

				pi.uiPing = tPing.elapsed() - *ts;
				pi.uiUsers = qFromBigEndian(ping[3]);

				pi.tsiItem->setText(3, QString::number(pi.uiPing / 1000));
				pi.tsiItem->setText(4, pi.uiUsers ? QString::number(pi.uiUsers) : QString());
			}
		}
	}
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
	qlePort->setText(a.at(1));

	qtwTab->setCurrentIndex(0);

	bDirty = true;
	on_qpbAdd_clicked();
}

void ConnectDialog::finished() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());
	if (rep->error() != QNetworkReply::NoError) {
		QMessageBox::warning(this, tr("Mumble"), tr("Failed to fetch server list"), QMessageBox::Ok);
		return;
	}

	QDomDocument doc;
	doc.setContent(rep->readAll());

	qlPublicServers.clear();
	qmLookups.clear();
	qmActivePings.clear();

	QDomElement root=doc.documentElement();
	QDomNode n = root.firstChild();
	while (!n.isNull()) {
		QDomElement e = n.toElement();
		if (!e.isNull()) {
			if (e.tagName() == QLatin1String("server")) {
				PublicInfo pi;
				pi.qsName = e.attribute(QLatin1String("name"));
				pi.quUrl = e.attribute(QLatin1String("url"));
				pi.qsIp = e.attribute(QLatin1String("ip"));
				pi.usPort = e.attribute(QLatin1String("port")).toUShort();
				pi.qsCountry = e.attribute(QLatin1String("country"));
				pi.qsCountryCode = e.attribute(QLatin1String("country_code")).toLower();

				pi.uiPing = 0;
				pi.uiUsers = 0;
				pi.tsiItem = NULL;

				qlPublicServers << pi;
			}
		}
		n = n.nextSibling();
	}

	tPublicServers.restart();

	fillList();

	rep->deleteLater();
}

void ConnectDialog::onSelection_Changed(const QModelIndex &index, const QModelIndex &) {
	if (index.isValid()) {
		QSqlRecord r = qstmServers->record(index.row());
		qleName->setText(r.value(QLatin1String("name")).toString());
		qleServer->setText(r.value(QLatin1String("hostname")).toString());
		qleUsername->setText(r.value(QLatin1String("username")).toString());
		qlePort->setText(r.value(QLatin1String("port")).toString());
		bDirty = false;
		qsPassword = r.value(QLatin1String("password")).toString();
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
	qsPassword = pw;
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

	if (idx == 1) {
		initList();
		pingList();
	}
}
