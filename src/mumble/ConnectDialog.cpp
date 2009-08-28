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
#include "Database.h"

QMap<QString, QIcon> ServerItem::qmIcons;

ServerItem::ServerItem(const FavoriteServer &fs) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	itType = FavoriteType;
	qsName = fs.qsName;
	qsHostname = fs.qsHostname;
	usPort = fs.usPort;

	qsUsername = fs.qsUsername;
	qsPassword = fs.qsPassword;

	qsUrl = fs.qsUrl;

	uiPing = 0;
	uiUsers = 0;

	if (qsHostname.startsWith(QLatin1Char('@')))
		brRecord = BonjourRecord(qsHostname.mid(1), QLatin1String("_mumble._tcp."), QLatin1String("local."));

	setDatas();
}

ServerItem::ServerItem(const PublicInfo &pi) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	itType = PublicType;
	qsName = pi.qsName;
	qsHostname = pi.qsIp;
	usPort = pi.usPort;
	qsUrl = pi.quUrl.toString();
	qsCountry = pi.qsCountry;
	qsCountryCode = pi.qsCountryCode;

	uiPing = 0;
	uiUsers = 0;

	setDatas();
}

ServerItem::ServerItem(const QString &name, const QString &host, unsigned short port, const QString &username) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	itType = FavoriteType;
	qsName = name;
	qsHostname = host;
	usPort = port;
	qsUsername = username;

	uiPing = 0;
	uiUsers = 0;

	setDatas();
}

ServerItem::ServerItem(const BonjourRecord &br) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	itType = LANType;
	qsName = br.serviceName;
	qsHostname = QLatin1Char('@') + qsName;
	brRecord = br;
	uiPing = 0;
	uiUsers = 0;

	usPort = 0;

	setDatas();
}

void ServerItem::setDatas() {
	setText(0, qsName);
	if (itType == FavoriteType) {
		setText(1, ConnectDialog::tr("Favorite"));
		setIcon(1, loadIcon(QLatin1String(":/emblems/emblem-favorite.svg")));
	} else if (itType == LANType) {
		setText(1, ConnectDialog::tr("LAN"));
	} else {
		setText(1, qsCountry);
		if (! qsCountryCode.isEmpty())
			setIcon(1, loadIcon(QString::fromLatin1(":/flags/%1.png").arg(qsCountryCode)));
	}

	setText(2, uiPing ? QString::number(uiPing / 1000) : QString());
	setText(3, uiUsers ? QString::number(uiUsers) : QString());
}

FavoriteServer ServerItem::toFavoriteServer() const {
	FavoriteServer fs;
	fs.qsName = qsName;
	fs.qsHostname = qsHostname;
	fs.usPort = usPort;
	fs.qsUsername = qsUsername;
	fs.qsPassword = qsPassword;
	fs.qsUrl = qsUrl;
	return fs;
}

bool ServerItem::operator <(const QTreeWidgetItem &o) const {
	const ServerItem &other = static_cast<const ServerItem &>(o);
	const QTreeWidget *w = treeWidget();

	int column = w ? w->sortColumn() : 0;
	bool inverse = w ? (w->header()->sortIndicatorOrder() == Qt::DescendingOrder) : false;
	bool less;

	if (itType != other.itType) {
		if (itType == FavoriteType)
			less = true;
		else if ((itType == LANType) && (other.itType == PublicType))
			less = true;
		else
			less = false;
			return less ^ inverse;
	}

	if (column == 2) {
		quint32 a = uiPing ? uiPing : UINT_MAX;
		quint32 b = other.uiPing ? other.uiPing : UINT_MAX;
		return a < b;
	}
	if (column == 3)
		return uiUsers < other.uiUsers;

	QString a = text(column).toLower();
	QString b = other.text(column).toLower();

	QRegExp re(QLatin1String("[^0-9a-z]"));
	a.remove(re);
	b.remove(re);
	return a < b;
}

QIcon ServerItem::loadIcon(const QString &name) {
	if (! qmIcons.contains(name))
		qmIcons.insert(name, QIcon(name));
	return qmIcons.value(name);
}

QList<PublicInfo> ConnectDialog::qlPublicServers;
Timer ConnectDialog::tPublicServers;
int ConnectDialog::iPingIndex = -1;

ConnectDialogEdit::ConnectDialogEdit(QWidget *p) : QDialog(p) {
	setupUi(this);
	qlePort->setValidator(new QIntValidator(1, 65535, qlePort));
}

ConnectDialog::ConnectDialog(QWidget *p) : QDialog(p) {
	setupUi(this);
#ifdef Q_OS_MAC
	setWindowFlags(Qt::Sheet);
#endif
	bPublicInit = false;

	if (tPublicServers.elapsed() >= 60 * 24 * 1000000ULL) {
		qlPublicServers.clear();
	}

	qbbButtonBox->button(QDialogButtonBox::Ok)->setText(tr("Connect"));

	qtwServers->sortItems(0, Qt::AscendingOrder);
	qtwServers->header()->setResizeMode(0, QHeaderView::Stretch);
	qtwServers->header()->setResizeMode(1, QHeaderView::ResizeToContents);
	qtwServers->header()->setResizeMode(2, QHeaderView::ResizeToContents);
	qtwServers->header()->setResizeMode(3, QHeaderView::ResizeToContents);

	QList<QTreeWidgetItem *> ql;
	QList<FavoriteServer> favorites = Database::getFavorites();

	foreach(const FavoriteServer &fs, favorites) {
		ServerItem *si = new ServerItem(fs);
		ql << si;
#ifdef USE_BONJOUR
		if (! si->brRecord.serviceName.isEmpty())
			g.bc->bsrResolver->resolveBonjourRecord(si->brRecord);
#endif
	}

	qtwServers->addTopLevelItems(ql);

#ifdef USE_BONJOUR
	// Make sure the we got the objects we need, then wire them up
	if (g.bc->bsbBrowser && g.bc->bsrResolver) {
		connect(g.bc->bsbBrowser, SIGNAL(error(DNSServiceErrorType)),
		        this, SLOT(onLanBrowseError(DNSServiceErrorType)));
		connect(g.bc->bsbBrowser, SIGNAL(currentBonjourRecordsChanged(const QList<BonjourRecord> &)),
		        this, SLOT(onUpdateLanList(const QList<BonjourRecord> &)));
		connect(g.bc->bsrResolver, SIGNAL(error(BonjourRecord, DNSServiceErrorType)),
		        this, SLOT(onLanResolveError(BonjourRecord, DNSServiceErrorType)));
		connect(g.bc->bsrResolver, SIGNAL(bonjourRecordResolved(BonjourRecord, QString, int)),
		        this, SLOT(onResolved(BonjourRecord, QString, int)));
		onUpdateLanList(g.bc->bsbBrowser->currentRecords());
	}
#endif

	qtPingTick = new QTimer(this);
	connect(qtPingTick, SIGNAL(timeout()), this, SLOT(timeTick()));

	qusSocket4 = new QUdpSocket(this);
	qusSocket6 = new QUdpSocket(this);
	bIPv4 = qusSocket4->bind(QHostAddress(QHostAddress::Any), 0);
	bIPv6 = qusSocket6->bind(QHostAddress(QHostAddress::AnyIPv6), 0);
	connect(qusSocket4, SIGNAL(readyRead()), this, SLOT(udpReply()));
	connect(qusSocket6, SIGNAL(readyRead()), this, SLOT(udpReply()));

	qmPopup = new QMenu(this);

	initList();
	fillList();
	pingList();
}

ConnectDialog::~ConnectDialog() {
	ServerItem::qmIcons.clear();

	QList<QTreeWidgetItem *> qlOld = qtwServers->findItems(QString(), Qt::MatchStartsWith);
	QList<FavoriteServer> ql;

	foreach(QTreeWidgetItem *qtwi, qlOld) {
		ServerItem *si = static_cast<ServerItem *>(qtwi);
		if (si->itType != ServerItem::FavoriteType)
			continue;
		ql << si->toFavoriteServer();
	}
	Database::setFavorites(ql);
}

void ConnectDialog::accept() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || si->qlAddresses.isEmpty())
		return;

	if (si->qsUsername.isEmpty()) {
		bool ok;
		QString defUserName = QInputDialog::getText(this, tr("Connecting to %1").arg(si->qsName), tr("Enter username"), QLineEdit::Normal, g.s.qsUsername, &ok).trimmed();
		if (! ok)
			return;
		g.s.qsUsername = si->qsUsername = defUserName;
	}

	qsUsername = si->qsUsername;
	qsPassword = si->qsPassword;
	qsServer = si->qsHostname;
	if (qsServer.startsWith(QLatin1Char('@'))) {
		if (si->qlAddresses.isEmpty())
			return;
		qsServer = si->qlAddresses.at(0).toString();
	}
	usPort = si->usPort;

	QDialog::accept();
}

void ConnectDialog::on_qaFavoriteAdd_triggered() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || (si->itType == ServerItem::FavoriteType))
		return;

	si->itType = ServerItem::FavoriteType;
	si->setDatas();
}

void ConnectDialog::on_qaFavoriteAddNew_triggered() {
	ConnectDialogEdit *cde = new ConnectDialogEdit(this);

	QString host, user, pw;
	QString name;
	unsigned short port = 64738;

	if (g.sh && g.sh->isRunning()) {
		g.sh->getConnectionInfo(host, port, user, pw);
		Channel *c = Channel::get(0);
		if (c) {
			if (c->qsName != QLatin1String("Root"))
				name = c->qsName;
		}
	} else
		user = g.s.qsUsername;

	cde->qleName->setText(name);
	cde->qleServer->setText(host);
	cde->qleUsername->setText(user);
	cde->qlePort->setText(QString::number(port));

	if (cde->exec() == QDialog::Accepted) {
		name = cde->qleName->text();
		host = cde->qleServer->text();
		user = cde->qleUsername->text();
		port = cde->qlePort->text().toUShort();
		ServerItem *si = new ServerItem(name, host, port, user);
		qtwServers->addTopLevelItem(si);
	}
	delete cde;
}

void ConnectDialog::on_qaFavoriteEdit_triggered() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || (si->itType != ServerItem::FavoriteType))
		return;

	ConnectDialogEdit *cde = new ConnectDialogEdit(this);

	cde->qleName->setText(si->qsName);
	cde->qleServer->setText(si->qsHostname);
	cde->qleUsername->setText(si->qsUsername);
	cde->qlePort->setText(QString::number(si->usPort));

	if (cde->exec() == QDialog::Accepted) {
		si->qsName = cde->qleName->text();
		si->qsHostname = cde->qleServer->text();
		si->qsUsername = cde->qleUsername->text();
		si->usPort = cde->qlePort->text().toUShort();
		si->setDatas();
	}
	delete cde;
}

void ConnectDialog::on_qaFavoriteRemove_triggered() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || (si->itType != ServerItem::FavoriteType))
		return;

	si->itType = si->brRecord.serviceName.isEmpty() ? ServerItem::PublicType : ServerItem::LANType;
	si->setDatas();
}

void ConnectDialog::on_qaUrl_triggered() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || si->qsUrl.isEmpty())
		return;

	QDesktopServices::openUrl(QUrl(si->qsUrl));
}

void ConnectDialog::on_qtwServers_customContextMenuRequested (const QPoint &mpos) {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->itemAt(mpos));

	qmPopup->clear();

	if (si && (si->itType == ServerItem::FavoriteType)) {
		qmPopup->addAction(qaFavoriteEdit);
		qmPopup->addAction(qaFavoriteAddNew);
		qmPopup->addAction(qaFavoriteRemove);
	} else if (si) {
		qmPopup->addAction(qaFavoriteAdd);
		qmPopup->addAction(qaFavoriteAddNew);
	} else {
		qmPopup->addAction(qaFavoriteAddNew);
	}
	if (si && ! si->qsUrl.isEmpty())
		qmPopup->addAction(qaUrl);

	qmPopup->popup(qtwServers->viewport()->mapToGlobal(mpos), NULL);
}

void ConnectDialog::on_qtwServers_itemDoubleClicked(QTreeWidgetItem *item, int) {
	qtwServers->setCurrentItem(item);
	accept();
}

void ConnectDialog::initList() {
	if (bPublicInit || (qlPublicServers.count() > 0))
		return;

	bPublicInit = true;

	QUrl url(QLatin1String("http://mumble.hive.no/list2.cgi"));
	url.addQueryItem(QLatin1String("version"), QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)));

	QNetworkRequest req(url);
	QNetworkReply *rep = g.nam->get(req);
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));
}

#ifdef USE_BONJOUR
void ConnectDialog::onResolved(BonjourRecord record, QString host, int port) {
	QList<QTreeWidgetItem *> qlOld = qtwServers->findItems(QString(), Qt::MatchStartsWith);

	foreach(QTreeWidgetItem *qtwi, qlOld) {
		ServerItem *si = static_cast<ServerItem *>(qtwi);
		if (si->brRecord == record) {
			si->usPort = static_cast<unsigned short>(port);
			qmLookups.insert(QHostInfo::lookupHost(host, this, SLOT(lookedUp(QHostInfo))), si);
			return;
		}
	}
}

void ConnectDialog::onUpdateLanList(const QList<BonjourRecord> &list) {
	QList<QTreeWidgetItem *> qlOld = qtwServers->findItems(QString(), Qt::MatchStartsWith);
	QList<QTreeWidgetItem *> ql;

	foreach(const BonjourRecord &record, list) {
		bool found = false;
		foreach(QTreeWidgetItem *qtwi, qlOld) {
			ServerItem *si = static_cast<ServerItem *>(qtwi);

			if (si->brRecord == record) {
				found = true;
				break;
			}
		}
		if (! found) {
			ql << new ServerItem(record);
			g.bc->bsrResolver->resolveBonjourRecord(record);
		}
	}

	qtwServers->addTopLevelItems(ql);
}

void ConnectDialog::onLanBrowseError(DNSServiceErrorType err) {
	qWarning()<<"Bonjour reported browser error "<< err;
}

void ConnectDialog::onLanResolveError(BonjourRecord, DNSServiceErrorType err) {
	qWarning()<<"Bonjour reported resolver error "<< err;
}
#endif

void ConnectDialog::fillList() {
	QList<QTreeWidgetItem *> qlOld = qtwServers->findItems(QString(), Qt::MatchStartsWith);
	QList<QTreeWidgetItem *> ql;

	foreach(const PublicInfo &pi, qlPublicServers) {
		bool found = false;
		foreach(QTreeWidgetItem *qtwi, qlOld) {
			ServerItem *si = static_cast<ServerItem *>(qtwi);
			if ((pi.qsIp == si->qsHostname) && (pi.usPort == si->usPort)) {
				found = true;

				si->qsUrl = pi.quUrl.toString();
				si->setDatas();
			}
		}
		if (! found)
			ql << new ServerItem(pi);
	}

	qtwServers->addTopLevelItems(ql);
}

void ConnectDialog::pingList() {
	iPingIndex = 0;
	qtPingTick->start(50);
}

void ConnectDialog::timeTick() {
	QList<QTreeWidgetItem *> ql = qtwServers->findItems(QString(), Qt::MatchStartsWith);

	if (ql.isEmpty())
		return;

	if (++iPingIndex >= ql.count())
		iPingIndex = 0;

	ServerItem *si = static_cast<ServerItem *>(ql.at(iPingIndex));

	if (si->qlAddresses.isEmpty()) {
		QHostAddress qha(si->qsHostname);
		if (qha.isNull()) {
			qmLookups.insert(QHostInfo::lookupHost(si->qsHostname, this, SLOT(lookedUp(QHostInfo))), si);
			return;
		}
		si->qlAddresses << qha;
	}

	foreach(const QHostAddress &host, si->qlAddresses)
		sendPing(si, host, si->usPort);
}

void ConnectDialog::lookedUp(QHostInfo info) {
	int id = info.lookupId();
	if (! qmLookups.contains(id))
		return;
	ServerItem *si = qmLookups.value(id);
	qmLookups.remove(id);

	if (info.error() != QHostInfo::NoError)
		return;

	si->qlAddresses = info.addresses();

	foreach(const QHostAddress &host, si->qlAddresses)
		sendPing(si, host, si->usPort);
}

void ConnectDialog::sendPing(ServerItem *si, const QHostAddress &host, unsigned short port) {
	char blob[16];

	memset(blob, 0, sizeof(blob));
	* reinterpret_cast<quint64 *>(blob+8) = tPing.elapsed();

	qmActivePings.insert(qpAddress(host, port), si);

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
				ServerItem *si = qmActivePings.value(address);
				qmActivePings.remove(address);

				quint32 *ping = reinterpret_cast<quint32 *>(blob+4);
				quint64 *ts = reinterpret_cast<quint64 *>(blob+8);

				si->uiPing = tPing.elapsed() - *ts;
				si->uiUsers = qFromBigEndian(ping[3]);

				si->setDatas();
			}
		}
	}
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

				qlPublicServers << pi;
			}
		}
		n = n.nextSibling();
	}

	tPublicServers.restart();

	fillList();

	rep->deleteLater();
}
