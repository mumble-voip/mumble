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

ServerView::ServerView(QWidget *p) : QTreeWidget(p) {
}

QMimeData *ServerView::mimeData(const QList<QTreeWidgetItem *> items) const {
	if (items.isEmpty())
		return NULL;

	ServerItem *si = static_cast<ServerItem *>(items.first());
	return si->toMimeData();
}

QStringList ServerView::mimeTypes() const {
	QStringList qsl;
	qsl << QStringList(QLatin1String("text/uri-list"));
	qsl << QStringList(QLatin1String("text/plain"));
	return qsl;
}

Qt::DropActions ServerView::supportedDropActions() const {
	return Qt::CopyAction | Qt::LinkAction;
}

bool ServerView::dropMimeData(QTreeWidgetItem *, int, const QMimeData *mime, Qt::DropAction) {
	ServerItem *si = ServerItem::fromMimeData(mime);
	if (! si)
		return false;

	qobject_cast<ConnectDialog *>(parent())->qlItems << si;
	addTopLevelItem(si);

	return true;
}

void ServerItem::initAccumulator() {
	boost::array<double, 3> probs = {0.75, 0.80, 0.95 };

	asQuantile = new asQuantileType(boost::accumulators::tag::extended_p_square::probabilities = probs);
	dPing = 0.0;
	uiPing = 0;
	uiUsers = 0;
	uiMaxUsers = 0;
	uiBandwidth = 0;
	uiSent = 0;
	uiVersion = 0;

	// Without this, columncount is wrong.
	setData(0, Qt::DisplayRole, QVariant());
	setData(1, Qt::DisplayRole, QVariant());
	setData(2, Qt::DisplayRole, QVariant());
	setData(3, Qt::DisplayRole, QVariant());
}

ServerItem::ServerItem(const FavoriteServer &fs) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	itType = FavoriteType;
	qsName = fs.qsName;
	qsHostname = fs.qsHostname;
	usPort = fs.usPort;

	qsUsername = fs.qsUsername;
	qsPassword = fs.qsPassword;

	qsUrl = fs.qsUrl;

	if (qsHostname.startsWith(QLatin1Char('@')))
		brRecord = BonjourRecord(qsHostname.mid(1), QLatin1String("_mumble._tcp."), QLatin1String("local."));

	initAccumulator();
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

	initAccumulator();
	setDatas();
}

ServerItem::ServerItem(const QString &name, const QString &host, unsigned short port, const QString &username) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	itType = FavoriteType;
	qsName = name;
	qsHostname = host;
	usPort = port;
	qsUsername = username;

	initAccumulator();
	setDatas();
}

ServerItem::ServerItem(const BonjourRecord &br) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	itType = LANType;
	qsName = br.serviceName;
	qsHostname = QLatin1Char('@') + qsName;
	brRecord = br;
	usPort = 0;

	initAccumulator();
	setDatas();
}

ServerItem *ServerItem::fromMimeData(const QMimeData *mime, QWidget *p) {
	if (mime->hasFormat(QLatin1String("OriginatedInMumble")))
		return NULL;

	QUrl url;
	if (mime->hasUrls() && ! mime->urls().isEmpty())
		url = mime->urls().at(0);
	else if (mime->hasText())
		url = QUrl::fromEncoded(mime->text().toUtf8());

	QString qsFile = url.toLocalFile();
	if (! qsFile.isEmpty()) {
		QFile f(qsFile);
		if (f.open(QIODevice::ReadOnly)) {
			QByteArray qba = f.readAll();
			f.close();

			url = QUrl::fromEncoded(qba, QUrl::StrictMode);
			if (! url.isValid()) {
				QSettings qs(qsFile, QSettings::IniFormat);
				url = QUrl::fromEncoded(qs.value(QLatin1String("InternetShortcut/URL")).toByteArray(), QUrl::StrictMode);
			}
		}
	}

	if (! url.isValid() || (url.scheme() != QLatin1String("mumble")))
		return NULL;

	if (url.userName().isEmpty()) {
		if (g.s.qsUsername.isEmpty()) {
			bool ok;
			QString defUserName = QInputDialog::getText(p, ConnectDialog::tr("Adding host %1").arg(url.host()), ConnectDialog::tr("Enter username"), QLineEdit::Normal, g.s.qsUsername, &ok).trimmed();
			if (! ok)
				return NULL;
			if (defUserName.isEmpty())
				return NULL;
			g.s.qsUsername = defUserName;
		}
		url.setUserName(g.s.qsUsername);
	}

	if (! url.hasQueryItem(QLatin1String("title")))
		url.addQueryItem(QLatin1String("title"), url.host());

	ServerItem *si = new ServerItem(url.queryItemValue(QLatin1String("title")), url.host(), url.port(), url.userName());
	if (! url.password().isEmpty())
		si->qsPassword = url.password();

	if (url.hasQueryItem(QLatin1String("url")))
		si->qsUrl = url.queryItemValue(QLatin1String("url"));

	return si;
}

QVariant ServerItem::data(int column, int role) const {
	if (role == Qt::DisplayRole) {
		switch (column) {
			case 0:
				return qsName;
			case 1:
				if (itType == FavoriteType)
					return ConnectDialog::tr("Favorite");
				else if (itType == LANType)
					return ConnectDialog::tr("LAN");
				else
					return qsCountry;
			case 2:
				return (dPing > 0.0) ? QString::number(uiPing) : QVariant();
			case 3:
				return uiUsers ? QString::fromLatin1("%1/%2").arg(uiUsers).arg(uiMaxUsers) : QVariant();
		}
	} else if (role == Qt::DecorationRole) {
		if (column == 1) {
			if (itType == FavoriteType)
				return loadIcon(QLatin1String(":/emblems/emblem-favorite.svg"));
			else if (itType == LANType)
				return loadIcon(QLatin1String(":/places/network-workgroup.svg"));
			else if (! qsCountryCode.isEmpty())
				return loadIcon(QString::fromLatin1(":/flags/%1.png").arg(qsCountryCode));
		}
	} else if (role == Qt::ToolTipRole) {
		QStringList qsl;
		foreach(const QHostAddress &qha, qlAddresses)
			qsl << qha.toString();

		double ploss = 100.0;
		quint32 uiRecv = boost::accumulators::count(* asQuantile);

		if (uiSent > 0)
			ploss = (uiSent - uiRecv) * 100. / uiSent;

		QString qs;
		qs +=
		    QLatin1String("<table>") +
		    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Name"), qsName) +
		    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Hostname"), qsHostname);

		if (! qsBonjourHost.isEmpty())
			qs += QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Bonjour name"), qsBonjourHost);

		qs +=
		    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Port")).arg(usPort) +
		    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Addresses"), qsl.join(QLatin1String(", ")));

		if (! qsUrl.isEmpty())
			qs += QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Website"), qsUrl);

		if (uiSent > 0) {
			qs += QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Packet loss"), QString::fromLatin1("%1% (%2/%3)").arg(ploss, 0, 'f', 1).arg(uiRecv).arg(uiSent));
			if (uiRecv > 0) {
				qs +=
				    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Ping (80%)"), ConnectDialog::tr("%1 ms").
				            arg(boost::accumulators::extended_p_square(* asQuantile)[1] / 1000., 0, 'f', 2)) +
				    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Ping (95%)"), ConnectDialog::tr("%1 ms").
				            arg(boost::accumulators::extended_p_square(* asQuantile)[2] / 1000., 0, 'f', 2)) +
				    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Bandwidth"), ConnectDialog::tr("%1 kbit/s").arg(uiBandwidth / 1000)) +
				    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Users"), QString::fromLatin1("%1/%2").arg(uiUsers).arg(uiMaxUsers)) +
				    QString::fromLatin1("<tr><th align=left>%1</th><td>%2.%3.%4</td></tr>").arg(ConnectDialog::tr("Version")).arg(uiVersion >> 16).arg((uiVersion >> 8) & 0xFF).arg(uiVersion & 0xFF);
			}
		}
		qs += QLatin1String("</table>");
		return qs;
	}
	return QTreeWidgetItem::data(column, role);
}

void ServerItem::hideCheck() {
	bool hide = false;
	if (itType == PublicType) {
		if (g.s.ssFilter == Settings::ShowFavorite)
			hide = true;
		else if (g.s.ssFilter == Settings::ShowReachable)
			hide = (uiPing == 0);
		else if (g.s.ssFilter == Settings::ShowPopulated)
			hide = (uiUsers == 0);
	}
	if (hide != isHidden())
		setHidden(hide);
}

void ServerItem::setDatas() {
	emitDataChanged();
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

QMimeData *ServerItem::toMimeData() const {
	QUrl url;
	url.setScheme(QLatin1String("mumble"));
	url.setHost(qsHostname);
	url.setPort(usPort);
	url.addQueryItem(QLatin1String("title"), qsName);
	if (! qsUrl.isEmpty())
		url.addQueryItem(QLatin1String("url"), qsUrl);
	url.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));

	QString qs = QLatin1String(url.toEncoded());

	QMimeData *mime = new QMimeData;

#ifdef Q_OS_WIN
	// FIXME: Even with all this, it still won't drop on the desktop.

	QString contents = QString::fromLatin1("[InternetShortcut]\r\nURL=%1\r\n").arg(qs);
	QString urlname = QString::fromLatin1("%1.url").arg(qsName);

	FILEGROUPDESCRIPTORA fgda;
	ZeroMemory(&fgda, sizeof(fgda));
	fgda.cItems = 1;
	fgda.fgd[0].dwFlags = FD_LINKUI | FD_FILESIZE;
	fgda.fgd[0].nFileSizeLow=contents.length();
	strcpy_s(fgda.fgd[0].cFileName, MAX_PATH, urlname.toLocal8Bit().constData());
	mime->setData(QLatin1String("FileGroupDescriptor"), QByteArray(reinterpret_cast<const char *>(&fgda), sizeof(fgda)));

	FILEGROUPDESCRIPTORW fgdw;
	ZeroMemory(&fgdw, sizeof(fgdw));
	fgdw.cItems = 1;
	fgdw.fgd[0].dwFlags = FD_LINKUI | FD_FILESIZE;
	fgdw.fgd[0].nFileSizeLow=contents.length();
	wcscpy_s(fgdw.fgd[0].cFileName, MAX_PATH, urlname.utf16());
	mime->setData(QLatin1String("FileGroupDescriptorW"), QByteArray(reinterpret_cast<const char *>(&fgdw), sizeof(fgdw)));

	mime->setData(QString::fromUtf16(CFSTR_FILECONTENTS), contents.toLocal8Bit());

	DWORD context[4];
	context[0] = 0;
	context[1] = 1;
	context[2] = 0;
	context[3] = 0;
	mime->setData(QLatin1String("DragContext"), QByteArray(reinterpret_cast<const char *>(&context[0]), sizeof(context)));

	DWORD dropaction;
	dropaction = DROPEFFECT_LINK;
	mime->setData(QString::fromUtf16(CFSTR_PREFERREDDROPEFFECT), QByteArray(reinterpret_cast<const char *>(&dropaction), sizeof(dropaction)));
#endif
	QList<QUrl> urls;
	urls << url;
	mime->setUrls(urls);

	mime->setText(qs);
	mime->setHtml(QString::fromLatin1("<a href=\"%1\">%2</a>").arg(qs).arg(qsName));

	mime->setData(QLatin1String("OriginatedInMumble"), QByteArray());

	return mime;
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

	if (column == 0) {
		QString a = text(column).toLower();
		QString b = other.text(column).toLower();

		QRegExp re(QLatin1String("[^0-9a-z]"));
		a.remove(re);
		b.remove(re);
		return a < b;
	}

	if (column == 1) {
		return qsCountry < other.qsCountry;
	}

	if (column == 2) {
		double a = dPing ? dPing : UINT_MAX;
		double b = other.dPing ? other.dPing : UINT_MAX;
		return a < b;
	}
	if (column == 3)
		return uiUsers < other.uiUsers;
	return false;
}

QIcon ServerItem::loadIcon(const QString &name) {
	if (! qmIcons.contains(name))
		qmIcons.insert(name, QIcon(name));
	return qmIcons.value(name);
}

ConnectDialogEdit::ConnectDialogEdit(QWidget *p, const QString &name, const QString &host, const QString &user, unsigned short port) : QDialog(p) {
	setupUi(this);
	qlePort->setValidator(new QIntValidator(1, 65535, qlePort));

	qleName->setText(name);
	qleServer->setText(host);
	qleUsername->setText(user.isEmpty() ? g.s.qsUsername : user);
	qlePort->setText(QString::number(port));

	usPort = 0;
	bOk = true;

	connect(qleName, SIGNAL(textChanged(const QString &)), this, SLOT(validate()));
	connect(qleServer, SIGNAL(textChanged(const QString &)), this, SLOT(validate()));
	connect(qlePort, SIGNAL(textChanged(const QString &)), this, SLOT(validate()));
	connect(qleUsername, SIGNAL(textChanged(const QString &)), this, SLOT(validate()));

	validate();
}

void ConnectDialogEdit::validate() {
	qsName = qleName->text().simplified();
	qsHostname = qleServer->text().simplified();
	usPort = qlePort->text().toUShort();
	qsUsername = qleUsername->text().simplified();

	bOk = ! qsName.isEmpty() && ! qsHostname.isEmpty() && ! qsUsername.isEmpty() && usPort;
	qdbbButtonBox->button(QDialogButtonBox::Ok)->setEnabled(bOk);
}

void ConnectDialogEdit::accept() {
	validate();
	if (bOk)
		QDialog::accept();
}

QList<PublicInfo> ConnectDialog::qlPublicServers;
Timer ConnectDialog::tPublicServers;

ConnectDialog::ConnectDialog(QWidget *p) : QDialog(p) {
	setupUi(this);
#ifdef Q_OS_MAC
	setWindowFlags(Qt::Sheet);
#endif
	bPublicInit = false;

	if (tPublicServers.elapsed() >= 60 * 24 * 1000000ULL) {
		qlPublicServers.clear();
	}

	qdbbButtonBox->button(QDialogButtonBox::Ok)->setText(tr("Connect"));

	QPushButton *qpb = new QPushButton(tr("Add New..."), this);
	qpb->setDefault(false);
	qpb->setAutoDefault(false);
	connect(qpb, SIGNAL(clicked()), qaFavoriteAddNew, SIGNAL(triggered()));
	qdbbButtonBox->addButton(qpb, QDialogButtonBox::ActionRole);

	qtwServers->sortItems(2, Qt::AscendingOrder);
	qtwServers->header()->setResizeMode(0, QHeaderView::Stretch);
	qtwServers->header()->setResizeMode(1, QHeaderView::ResizeToContents);
	qtwServers->header()->setResizeMode(2, QHeaderView::ResizeToContents);
	qtwServers->header()->setResizeMode(3, QHeaderView::ResizeToContents);

	qaShowAll->setChecked(false);
	qaShowReachable->setChecked(false);
	qaShowPopulated->setChecked(false);
	qaShowFavorite->setChecked(false);

	switch (g.s.ssFilter) {
		case Settings::ShowFavorite:
			qaShowFavorite->setChecked(true);
			break;
		case Settings::ShowPopulated:
			qaShowPopulated->setChecked(true);
			break;
		case Settings::ShowReachable:
			qaShowReachable->setChecked(true);
			break;
		default:
			qaShowAll->setChecked(true);
			break;
	}

	qagFilters = new QActionGroup(this);
	qagFilters->addAction(qaShowAll);
	qagFilters->addAction(qaShowReachable);
	qagFilters->addAction(qaShowPopulated);
	qagFilters->addAction(qaShowFavorite);

	connect(qagFilters, SIGNAL(triggered(QAction *)), this, SLOT(onFiltersTriggered(QAction *)));

	qmPopup = new QMenu(this);
	qmFilters = new QMenu(tr("Filters"), this);
	qmFilters->addAction(qaShowAll);
	qmFilters->addAction(qaShowReachable);
	qmFilters->addAction(qaShowPopulated);
	qmFilters->addAction(qaShowFavorite);

	QList<QTreeWidgetItem *> ql;
	QList<FavoriteServer> favorites = Database::getFavorites();

	foreach(const FavoriteServer &fs, favorites) {
		ServerItem *si = new ServerItem(fs);
		ql << si;
		qlItems << si;
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

	if (g.s.ssFilter != Settings::ShowFavorite)
		initList();
	fillList();
	pingList();

	new QShortcut(QKeySequence(QKeySequence::Copy), this, SLOT(on_qaFavoriteCopy_triggered()));
	new QShortcut(QKeySequence(QKeySequence::Paste), this, SLOT(on_qaFavoritePaste_triggered()));

	qtwServers->setCurrentItem(NULL);
	bLastFound = false;
}

ConnectDialog::~ConnectDialog() {
	ServerItem::qmIcons.clear();

	QList<FavoriteServer> ql;

	foreach(ServerItem *si, qlItems) {
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

	qsPassword = si->qsPassword;
	qsServer = si->qsHostname;
	if (qsServer.startsWith(QLatin1Char('@'))) {
		qsServer = si->qsBonjourHost;
		if (qsServer.isEmpty())
			return;
	}
	usPort = si->usPort;

	if (si->qsUsername.isEmpty()) {
		bool ok;
		QString defUserName = QInputDialog::getText(this, tr("Connecting to %1").arg(si->qsName), tr("Enter username"), QLineEdit::Normal, g.s.qsUsername, &ok).trimmed();
		if (! ok)
			return;
		g.s.qsUsername = si->qsUsername = defUserName;
	}

	qsUsername = si->qsUsername;

	g.s.qsLastServer = si->qsName;

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

	ConnectDialogEdit *cde = new ConnectDialogEdit(this, name, host, user, port);

	if (cde->exec() == QDialog::Accepted) {
		ServerItem *si = new ServerItem(cde->qsName, cde->qsHostname, cde->usPort, cde->qsUsername);
		qlItems << si;
		qtwServers->addTopLevelItem(si);
	}
	delete cde;
}

void ConnectDialog::on_qaFavoriteEdit_triggered() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || (si->itType != ServerItem::FavoriteType))
		return;

	ConnectDialogEdit *cde = new ConnectDialogEdit(this, si->qsName, si->qsHostname, si->qsUsername, si->usPort);

	if (cde->exec() == QDialog::Accepted) {
		si->qsName = cde->qsName;
		si->qsHostname = cde->qsHostname;
		si->qsUsername = cde->qsUsername;
		si->usPort = cde->usPort;
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

void ConnectDialog::on_qaFavoriteCopy_triggered() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si)
		return;

	QApplication::clipboard()->setMimeData(si->toMimeData());
}

void ConnectDialog::on_qaFavoritePaste_triggered() {
	ServerItem *si = ServerItem::fromMimeData(QApplication::clipboard()->mimeData());
	if (! si)
		return;

	qlItems << si;
	qtwServers->addTopLevelItem(si);
}

void ConnectDialog::on_qaUrl_triggered() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || si->qsUrl.isEmpty())
		return;

	QDesktopServices::openUrl(QUrl(si->qsUrl));
}

void ConnectDialog::onFiltersTriggered(QAction *act) {
	if (act == qaShowAll)
		g.s.ssFilter = Settings::ShowAll;
	else if (act == qaShowReachable)
		g.s.ssFilter = Settings::ShowReachable;
	else if (act == qaShowPopulated)
		g.s.ssFilter = Settings::ShowPopulated;
	else
		g.s.ssFilter = Settings::ShowFavorite;

	if (g.s.ssFilter != Settings::ShowFavorite)
		initList();

	foreach(ServerItem *si, qlItems)
		si->hideCheck();
}

void ConnectDialog::on_qtwServers_customContextMenuRequested(const QPoint &mpos) {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->itemAt(mpos));

	qmPopup->clear();

	if (si && (si->itType == ServerItem::FavoriteType)) {
		qmPopup->addAction(qaFavoriteEdit);
		qmPopup->addAction(qaFavoriteRemove);
	} else if (si) {
		qmPopup->addAction(qaFavoriteAdd);
	}
	if (si && ! si->qsUrl.isEmpty())
		qmPopup->addAction(qaUrl);

	if (! qmPopup->isEmpty())
		qmPopup->addSeparator();
	qmPopup->addMenu(qmFilters);

	qmPopup->popup(qtwServers->viewport()->mapToGlobal(mpos), NULL);
}

void ConnectDialog::on_qtwServers_itemDoubleClicked(QTreeWidgetItem *item, int) {
	qtwServers->setCurrentItem(item);
	accept();
}

void ConnectDialog::on_qtwServers_currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *) {
	ServerItem *si = static_cast<ServerItem *>(item);

	bool bOk = (si && ! si->qlAddresses.isEmpty());
	qdbbButtonBox->button(QDialogButtonBox::Ok)->setEnabled(bOk);

	bLastFound = true;
}

void ConnectDialog::initList() {
	if (bPublicInit || (qlPublicServers.count() > 0))
		return;

	bPublicInit = true;

	QUrl url;
	url.setScheme(QLatin1String("http"));
	url.setHost(g.qsRegionalHost);
	url.setPath(QLatin1String("/list2.cgi"));
	url.addQueryItem(QLatin1String("version"), QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)));

	QNetworkRequest req(url);
	QNetworkReply *rep = g.nam->get(req);
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));
}

#ifdef USE_BONJOUR
void ConnectDialog::onResolved(BonjourRecord record, QString host, int port) {
	foreach(ServerItem *si, qlItems) {
		if (si->brRecord == record) {
			si->usPort = static_cast<unsigned short>(port);
			si->qsBonjourHost = host;
			QHostInfo::lookupHost(host, this, SLOT(lookedUp(QHostInfo)));
			qmLookups.insert(host, si);
			return;
		}
	}
}

void ConnectDialog::onUpdateLanList(const QList<BonjourRecord> &list) {
	QList<QTreeWidgetItem *> ql;

	foreach(const BonjourRecord &record, list) {
		bool found = false;
		foreach(ServerItem *si, qlItems) {
			if (si->brRecord == record) {
				found = true;
				break;
			}
		}
		if (! found) {
			ServerItem *si = new ServerItem(record);
			qlItems << si;
			ql << si;
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
	QList<QTreeWidgetItem *> ql;
	QList<QTreeWidgetItem *> qlNew;

	foreach(const PublicInfo &pi, qlPublicServers) {
		bool found = false;
		foreach(ServerItem *si, qlItems) {
			if ((pi.qsIp == si->qsHostname) && (pi.usPort == si->usPort)) {
				found = true;

				si->qsCountry = pi.qsCountry;
				si->qsCountryCode = pi.qsCountryCode;
				si->qsUrl = pi.quUrl.toString();
				si->setDatas();
			}
		}
		if (! found)
			ql << new ServerItem(pi);
	}

	while (! ql.isEmpty()) {
		ServerItem *si = static_cast<ServerItem *>(ql.takeAt(qrand() % ql.count()));
		qlNew << si;
		qlItems << si;
	}

	qtwServers->addTopLevelItems(qlNew);
	foreach(QTreeWidgetItem *qtwi, qlNew)
		static_cast<ServerItem *>(qtwi)->hideCheck();
}

void ConnectDialog::pingList() {
	iPingIndex = -1;
	qtPingTick->start(50);
	timeTick();
}

void ConnectDialog::timeTick() {
	if (! bLastFound && ! g.s.qsLastServer.isEmpty()) {
		QList<QTreeWidgetItem *> items = qtwServers->findItems(g.s.qsLastServer, Qt::MatchExactly);
		if (!items.isEmpty()) {
			bLastFound = true;
			qtwServers->setCurrentItem(items.at(0));
		}
	}

	ServerItem *current = static_cast<ServerItem *>(qtwServers->currentItem());
	ServerItem *hover = static_cast<ServerItem *>(qtwServers->itemAt(qtwServers->viewport()->mapFromGlobal(QCursor::pos())));

	ServerItem *si = NULL;
	if (tCurrent.elapsed() >= 1000000ULL)
		si = current;
	if (! si && (tHover.elapsed() >= 1000000ULL))
		si = hover;
	if (!si) {
		if (qlItems.isEmpty())
			return;

		if (++iPingIndex >= qlItems.count()) {
			if (tRestart.isElapsed(1000000ULL))
				iPingIndex = 0;
			else
				return;
		}

		si = qlItems.at(iPingIndex);

		if (! si)
			return;
	}

	if (si == current)
		tCurrent.restart();
	if (si == hover)
		tHover.restart();

	if (si->qlAddresses.isEmpty()) {
		QHostAddress qha(si->qsHostname);
		if (qha.isNull()) {
			if (qhDNSCache.contains(si->qsHostname.toLower())) {
				si->qlAddresses << qhDNSCache.value(si->qsHostname.toLower());
			} else {
				if (!qmLookups.contains(si->qsHostname)) {
					QHostInfo::lookupHost(si->qsHostname, this, SLOT(lookedUp(QHostInfo)));
					qmLookups.insert(si->qsHostname, si);
				}
				return;
			}
		} else {
			si->qlAddresses << qha;
		}

		if (si == qtwServers->currentItem())
			on_qtwServers_currentItemChanged(si, si);
	}

	foreach(const QHostAddress &host, si->qlAddresses)
		sendPing(si, host, si->usPort);
}

void ConnectDialog::lookedUp(QHostInfo info) {
	if (! qmLookups.contains(info.hostName()))
		return;

	ServerItem *si = qmLookups.value(info.hostName());
	qmLookups.remove(info.hostName());

	if (info.error() != QHostInfo::NoError)
		return;

	si->qlAddresses = info.addresses();
	qhDNSCache.insert(info.hostName().toLower(), info.addresses());

	if (si == qtwServers->currentItem())
		on_qtwServers_currentItemChanged(si, si);

	foreach(const QHostAddress &host, si->qlAddresses)
		sendPing(si, host, si->usPort);
}

void ConnectDialog::sendPing(ServerItem *si, const QHostAddress &host, unsigned short port) {
	char blob[16];

	qpAddress addr(host, port);

	quint64 uiRand;
	if (qhPingRand.contains(addr)) {
		uiRand = qhPingRand.value(addr);
	} else {
		uiRand = (static_cast<quint64>(qrand()) << 32) | static_cast<quint64>(qrand());
		qhPingRand.insert(addr, uiRand);
	}

	memset(blob, 0, sizeof(blob));
	* reinterpret_cast<quint64 *>(blob+8) = tPing.elapsed() ^ uiRand;

	if (bIPv4 && host.protocol() == QAbstractSocket::IPv4Protocol)
		qusSocket4->writeDatagram(blob+4, 12, host, port);
	else if (bIPv6 && host.protocol() == QAbstractSocket::IPv6Protocol)
		qusSocket6->writeDatagram(blob+4, 12, host, port);
	else
		return;

	QSet<ServerItem *> &qs = qhPings[addr];

	qs.insert(si);

	foreach(si, qs)
		++ si->uiSent;
}

void ConnectDialog::udpReply() {
	QUdpSocket *sock = qobject_cast<QUdpSocket *>(sender());
	while (sock->hasPendingDatagrams()) {
		char blob[64];

		QHostAddress host;
		unsigned short port;

		int len = sock->readDatagram(blob+4, 24, &host, &port);
		if (len == 24) {
			qpAddress address(host, port);
			if (qhPings.contains(address)) {
				quint32 *ping = reinterpret_cast<quint32 *>(blob+4);
				quint64 *ts = reinterpret_cast<quint64 *>(blob+8);

				quint64 elapsed = tPing.elapsed() - (*ts ^ qhPingRand.value(address));

				foreach(ServerItem *si, qhPings.value(address)) {
					si->uiVersion = qFromBigEndian(ping[0]);
					si->uiUsers = qFromBigEndian(ping[3]);
					si->uiMaxUsers = qFromBigEndian(ping[4]);
					si->uiBandwidth = qFromBigEndian(ping[5]);

					(* si->asQuantile)(static_cast<double>(elapsed));
					si->dPing = boost::accumulators::extended_p_square(* si->asQuantile)[0];
					si->uiPing = lroundf(si->dPing / 1000.);

					si->setDatas();
					si->hideCheck();
				}
			}
		}
	}
}

void ConnectDialog::finished() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());
	if (rep->error() != QNetworkReply::NoError) {
		QUrl url = rep->request().url();
		if (url.host() == g.qsRegionalHost) {
			url.setHost(QLatin1String("mumble.info"));
			QNetworkRequest req(url);
			QNetworkReply *nrep = g.nam->get(req);
			connect(nrep, SIGNAL(finished()), this, SLOT(finished()));

			rep->deleteLater();
			return;
		}

		QMessageBox::warning(this, tr("Mumble"), tr("Failed to fetch server list"), QMessageBox::Ok);
		return;
	}

	QDomDocument doc;
	doc.setContent(rep->readAll());

	qlPublicServers.clear();

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
