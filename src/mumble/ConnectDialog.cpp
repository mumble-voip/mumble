// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ConnectDialog.h"

#ifdef USE_BONJOUR
#include "BonjourClient.h"
#include "BonjourServiceBrowser.h"
#include "BonjourServiceResolver.h"
#endif

#include "Channel.h"
#include "Database.h"
#include "ServerHandler.h"
#include "WebFetch.h"
#include "ServerResolver.h"
#include "Utils.h"

#include <QtCore/QMimeData>
#include <QtCore/QUrlQuery>
#include <QtCore/QtEndian>
#include <QtGui/QClipboard>
#include <QtGui/QDesktopServices>
#include <QtGui/QPainter>
#include <QtNetwork/QUdpSocket>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QShortcut>
#include <QtXml/QDomDocument>

#include <boost/accumulators/statistics/extended_p_square.hpp>
#include <boost/array.hpp>

#ifdef Q_OS_WIN
# include <shlobj.h>
#endif

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

QMap<QString, QIcon> ServerItem::qmIcons;
QList<PublicInfo> ConnectDialog::qlPublicServers;
QString ConnectDialog::qsUserCountry, ConnectDialog::qsUserCountryCode, ConnectDialog::qsUserContinentCode;
Timer ConnectDialog::tPublicServers;


PingStats::PingStats() {
	init();
}

PingStats::~PingStats() {
	delete asQuantile;
}

void PingStats::init() {
	boost::array<double, 3> probs = {{0.75, 0.80, 0.95 }};

	asQuantile = new asQuantileType(boost::accumulators::tag::extended_p_square::probabilities = probs);
	dPing = 0.0;
	uiPing = 0;
	uiPingSort = 0;
	uiUsers = 0;
	uiMaxUsers = 0;
	uiBandwidth = 0;
	uiSent = 0;
	uiRecv = 0;
	uiVersion = 0;
}

void PingStats::reset() {
	delete asQuantile;
	init();
}

ServerViewDelegate::ServerViewDelegate(QObject *p) : QStyledItemDelegate(p) {
}

ServerViewDelegate::~ServerViewDelegate() {
}

void ServerViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	// Allow a ServerItem's BackgroundRole to override the current theme's default color.
	QVariant bg = index.data(Qt::BackgroundRole);
	if (bg.isValid()) {
		painter->fillRect(option.rect, bg.value<QBrush>());
	}

	QStyledItemDelegate::paint(painter, option, index);
}

ServerView::ServerView(QWidget *p) : QTreeWidget(p) {
	siFavorite = new ServerItem(tr("Favorite"), ServerItem::FavoriteType);
	addTopLevelItem(siFavorite);
	siFavorite->setExpanded(true);
	siFavorite->setHidden(true);

#ifdef USE_BONJOUR
	siLAN = new ServerItem(tr("LAN"), ServerItem::LANType);
	addTopLevelItem(siLAN);
	siLAN->setExpanded(true);
	siLAN->setHidden(true);
#else
	siLAN = NULL;
#endif

	if (!g.s.disablePublicList) {
		siPublic = new ServerItem(tr("Public Internet"), ServerItem::PublicType);
		siPublic->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
		addTopLevelItem(siPublic);

		siPublic->setExpanded(false);

		// The continent code is empty when the server's IP address is not in the GeoIP database
		qmContinentNames.insert(QLatin1String(""), tr("Unknown"));

		qmContinentNames.insert(QLatin1String("af"), tr("Africa"));
		qmContinentNames.insert(QLatin1String("as"), tr("Asia"));
		qmContinentNames.insert(QLatin1String("na"), tr("North America"));
		qmContinentNames.insert(QLatin1String("sa"), tr("South America"));
		qmContinentNames.insert(QLatin1String("eu"), tr("Europe"));
		qmContinentNames.insert(QLatin1String("oc"), tr("Oceania"));
	} else {
		qWarning()<< "Public list disabled";

		siPublic = NULL;
	}
}

ServerView::~ServerView() {
	delete siFavorite;
	delete siLAN;
	delete siPublic;
}

QMimeData *ServerView::mimeData(const QList<QTreeWidgetItem *> mimeitems) const {
	if (mimeitems.isEmpty())
		return NULL;

	ServerItem *si = static_cast<ServerItem *>(mimeitems.first());
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

/* Extract and append (2), (3) etc to the end of a servers name if it is cloned. */
void ServerView::fixupName(ServerItem *si) {
	QString name = si->qsName;

	int tag = 1;

	QRegExp tmatch(QLatin1String("(.+)\\((\\d+)\\)"));
	tmatch.setMinimal(true);
	if (tmatch.exactMatch(name)) {
		name = tmatch.capturedTexts().at(1).trimmed();
		tag = tmatch.capturedTexts().at(2).toInt();
	}

	bool found;
	QString cmpname;
	do {
		found = false;
		if (tag > 1)
			cmpname = name + QString::fromLatin1(" (%1)").arg(tag);
		else
			cmpname = name;

		foreach(ServerItem *f, siFavorite->qlChildren)
			if (f->qsName == cmpname)
				found = true;

		++tag;
	} while (found);

	si->qsName = cmpname;
}

bool ServerView::dropMimeData(QTreeWidgetItem *, int, const QMimeData *mime, Qt::DropAction) {
	ServerItem *si = ServerItem::fromMimeData(mime);
	if (! si)
		return false;

	fixupName(si);

	qobject_cast<ConnectDialog *>(parent())->qlItems << si;
	siFavorite->addServerItem(si);

	qobject_cast<ConnectDialog *>(parent())->startDns(si);

	setCurrentItem(si);

	return true;
}

ServerItem *ServerView::getParent(const QString &continentcode, const QString &countrycode, const QString &countryname, const QString &usercontinent, const QString &usercountry) {
	ServerItem *continent = qmContinent.value(continentcode);
	if (!continent) {
		QString name = qmContinentNames.value(continentcode);
		if (name.isEmpty())
			name = continentcode;
		continent = new ServerItem(name, ServerItem::PublicType, continentcode);
		qmContinent.insert(continentcode, continent);
		siPublic->addServerItem(continent);

		if (!continentcode.isEmpty()) {
			if (continentcode == usercontinent) {
				continent->setExpanded(true);
				scrollToItem(continent, QAbstractItemView::PositionAtTop);
			}
		} else {
			continent->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
		}
	}

	// If the continent code is empty, we put the server directly into the "Unknown" continent
	if (continentcode.isEmpty()) {
		return continent;
	}

	ServerItem *country = qmCountry.value(countrycode);
	if (!country) {
		country = new ServerItem(countryname, ServerItem::PublicType, continentcode, countrycode);
		qmCountry.insert(countrycode, country);
		country->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

		continent->addServerItem(country);

		if (!countrycode.isEmpty() && countrycode == usercountry) {
			country->setExpanded(true);
			scrollToItem(country, QAbstractItemView::PositionAtTop);
		}
	}
	return country;
}


void ServerItem::init() {
	// Without this, columncount is wrong.
	setData(0, Qt::DisplayRole, QVariant());
	setData(1, Qt::DisplayRole, QVariant());
	setData(2, Qt::DisplayRole, QVariant());
	emitDataChanged();
}

ServerItem::ServerItem(const FavoriteServer &fs) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	siParent = NULL;
	bParent = false;

	itType = FavoriteType;
	qsName = fs.qsName;
	usPort = fs.usPort;

	qsUsername = fs.qsUsername;
	qsPassword = fs.qsPassword;

	qsUrl = fs.qsUrl;

	bCA = false;

	if (fs.qsHostname.startsWith(QLatin1Char('@'))) {
		qsBonjourHost = fs.qsHostname.mid(1);
		brRecord = BonjourRecord(qsBonjourHost, QLatin1String("_mumble._tcp."), QLatin1String("local."));
	} else {
		qsHostname = fs.qsHostname;
	}

	init();
}

ServerItem::ServerItem(const PublicInfo &pi) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	siParent = NULL;
	bParent = false;
	itType = PublicType;
	qsName = pi.qsName;
	qsHostname = pi.qsIp;
	usPort = pi.usPort;
	qsUrl = pi.quUrl.toString();
	qsCountry = pi.qsCountry;
	qsCountryCode = pi.qsCountryCode;
	qsContinentCode = pi.qsContinentCode;
	bCA = pi.bCA;

	init();
}

ServerItem::ServerItem(const QString &name, const QString &host, unsigned short port, const QString &username, const QString &password) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	siParent = NULL;
	bParent = false;
	itType = FavoriteType;
	qsName = name;
	usPort = port;
	qsUsername = username;
	qsPassword = password;

	bCA = false;

	if (host.startsWith(QLatin1Char('@'))) {
		qsBonjourHost = host.mid(1);
		brRecord = BonjourRecord(qsBonjourHost, QLatin1String("_mumble._tcp."), QLatin1String("local."));
	} else {
		qsHostname = host;
	}

	init();
}

ServerItem::ServerItem(const BonjourRecord &br) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	siParent = NULL;
	bParent = false;
	itType = LANType;
	qsName = br.serviceName;
	qsBonjourHost = qsName;
	brRecord = br;
	usPort = 0;
	bCA = false;

	init();
}

ServerItem::ServerItem(const QString &name, ItemType itype, const QString &continent, const QString &country) {
	siParent = NULL;
	bParent = true;
	qsName = name;
	itType = itype;
	if (itType == PublicType) {
		qsCountryCode = country;
		qsContinentCode = continent;
	}
	setFlags(flags() & ~Qt::ItemIsDragEnabled);
	bCA = false;

	init();
}

ServerItem::ServerItem(const ServerItem *si) {
	siParent = NULL;
	bParent = false;
	itType = FavoriteType;

	qsName = si->qsName;
	qsHostname = si->qsHostname;
	usPort = si->usPort;
	qsUsername = si->qsUsername;
	qsPassword = si->qsPassword;
	qsCountry = si->qsCountry;
	qsCountryCode = si->qsCountryCode;
	qsContinentCode = si->qsContinentCode;
	qsUrl = si->qsUrl;
	qsBonjourHost = si->qsBonjourHost;
	brRecord = si->brRecord;
	qlAddresses = si->qlAddresses;
	bCA = si->bCA;

	uiVersion = si->uiVersion;
	uiPing = si->uiPing;
	uiPingSort = si->uiPing;
	uiUsers = si->uiUsers;
	uiMaxUsers = si->uiMaxUsers;
	uiBandwidth = si->uiBandwidth;
	uiSent = si->uiSent;
	dPing = si->dPing;
	*asQuantile = * si->asQuantile;
}

ServerItem::~ServerItem() {
	if (siParent) {
		siParent->qlChildren.removeAll(this);
		if (siParent->bParent && siParent->qlChildren.isEmpty())
			siParent->setHidden(true);
	}

	// This is just for cleanup when exiting the dialog, it won't stop pending DNS for the children.
	foreach(ServerItem *si, qlChildren)
		delete si;
}

ServerItem *ServerItem::fromMimeData(const QMimeData *mime, bool default_name, QWidget *p, bool convertHttpUrls) {
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
		// Make sure we don't accidently read something big the user
		// happened to have in his clipboard. We only want to look
		// at small link files.
		if (f.open(QIODevice::ReadOnly) && f.size() < 10240) {
			QByteArray qba = f.readAll();
			f.close();

			url = QUrl::fromEncoded(qba, QUrl::StrictMode);
			if (! url.isValid()) {
				// Windows internet shortcut files (.url) are an ini with an URL value
				QSettings qs(qsFile, QSettings::IniFormat);
				url = QUrl::fromEncoded(qs.value(QLatin1String("InternetShortcut/URL")).toByteArray(), QUrl::StrictMode);
			}
		}
	}

	if (default_name) {
#if QT_VERSION >= 0x050000
		QUrlQuery query(url);
		if (! query.hasQueryItem(QLatin1String("title"))) {
			query.addQueryItem(QLatin1String("title"), url.host());
		}
#else
		if (! url.hasQueryItem(QLatin1String("title"))) {
			url.addQueryItem(QLatin1String("title"), url.host());
		}
#endif
	}

	if (! url.isValid()) {
		return NULL;
	}

	// An URL from text without a scheme will have the hostname text
	// in the QUrl scheme and no hostname. We do not want to use that.
	if (url.host().isEmpty()) {
		return NULL;
	}

	// Some communication programs automatically create http links from domains.
	// When a user sends another user a domain to connect to, and http is added wrongly,
	// we do our best to remove it again.
	if (convertHttpUrls && (
	    url.scheme() == QLatin1String("http")
	    || url.scheme() == QLatin1String("https"))) {
		url.setScheme(QLatin1String("mumble"));
	}

	return fromUrl(url, p);
}

ServerItem *ServerItem::fromUrl(QUrl url, QWidget *p) {
	if (! url.isValid() || (url.scheme() != QLatin1String("mumble"))) {
		return NULL;
	}

#if QT_VERSION >= 0x050000
	QUrlQuery query(url);
#endif

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

#if QT_VERSION >= 0x050000
	ServerItem *si = new ServerItem(query.queryItemValue(QLatin1String("title")), url.host(), static_cast<unsigned short>(url.port(DEFAULT_MUMBLE_PORT)), url.userName(), url.password());

	if (query.hasQueryItem(QLatin1String("url")))
		si->qsUrl = query.queryItemValue(QLatin1String("url"));
#else
	ServerItem *si = new ServerItem(url.queryItemValue(QLatin1String("title")), url.host(), static_cast<unsigned short>(url.port(DEFAULT_MUMBLE_PORT)), url.userName(), url.password());

	if (url.hasQueryItem(QLatin1String("url")))
		si->qsUrl = url.queryItemValue(QLatin1String("url"));
#endif

	return si;
}

QVariant ServerItem::data(int column, int role) const {
	if (bParent) {
		if (column == 0) {
			switch (role) {
				case Qt::DisplayRole:
					return qsName;
				case Qt::DecorationRole:
					if (itType == FavoriteType)
						return loadIcon(QLatin1String("skin:emblems/emblem-favorite.svg"));
					else if (itType == LANType)
						return loadIcon(QLatin1String("skin:places/network-workgroup.svg"));
					else if (! qsCountryCode.isEmpty()) {
						QString flag = QString::fromLatin1(":/flags/%1.svg").arg(qsCountryCode);
						if (!QFileInfo(flag).exists()) {
							flag = QLatin1String("skin:categories/applications-internet.svg");
						}
						return loadIcon(flag);
					}
					else
						return loadIcon(QLatin1String("skin:categories/applications-internet.svg"));
			}
		}
	} else {
		if (role == Qt::DisplayRole) {
			switch (column) {
				case 0:
					return qsName;
				case 1:
					return (dPing > 0.0) ? QString::number(uiPing) : QVariant();
				case 2:
					return uiUsers ? QString::fromLatin1("%1/%2 ").arg(uiUsers).arg(uiMaxUsers) : QVariant();
			}
		} else if (role == Qt::ToolTipRole) {
			QStringList qsl;
			foreach(const ServerAddress &addr, qlAddresses) {
				qsl << Qt::escape(addr.host.toString() + QLatin1String(":") + QString::number(static_cast<unsigned long>(addr.port)));
			}

			double ploss = 100.0;

			if (uiSent > 0)
				ploss = (uiSent - qMin(uiRecv, uiSent)) * 100. / uiSent;

			QString qs;
			qs +=
			    QLatin1String("<table>") +
			    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Servername"), Qt::escape(qsName)) +
			    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Hostname"), Qt::escape(qsHostname));

			if (! qsBonjourHost.isEmpty())
				qs += QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Bonjour name"), Qt::escape(qsBonjourHost));

			qs +=
			    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Port")).arg(usPort) +
			    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Addresses"), qsl.join(QLatin1String(", ")));

			if (! qsUrl.isEmpty())
				qs += QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Website"), Qt::escape(qsUrl));

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
					    QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>").arg(ConnectDialog::tr("Version")).arg(MumbleVersion::toString(uiVersion));
				}
			}
			qs += QLatin1String("</table>");
			return qs;
		} else if (role == Qt::BackgroundRole) {
			if (bCA) {
				QColor qc(Qt::green);
				qc.setAlpha(32);
				return qc;
			}
		}
	}
	return QTreeWidgetItem::data(column, role);
}

void ServerItem::addServerItem(ServerItem *childitem) {
	Q_ASSERT(childitem->siParent == NULL);

	childitem->siParent = this;
	qlChildren.append(childitem);
	childitem->hideCheck();

	if (bParent && (itType != PublicType) && isHidden())
		setHidden(false);
}

// If all child items are hidden, there is no child indicator, regardless of policy, so we have to add/remove instead.
void ServerItem::hideCheck() {
	bool hide = false;
	bool ishidden = (parent() == NULL);

	if (! bParent && (itType == PublicType)) {
		if (g.s.ssFilter == Settings::ShowReachable)
			hide = (dPing == 0.0);
		else if (g.s.ssFilter == Settings::ShowPopulated)
			hide = (uiUsers == 0);
	}
	if (hide != ishidden) {
		if (hide)
			siParent->removeChild(this);
		else
			siParent->addChild(this);
	}
}

void ServerItem::setDatas(double elapsed, quint32 users, quint32 maxusers) {
	if (elapsed == 0.0) {
		emitDataChanged();
		return;
	}

	(*asQuantile)(static_cast<double>(elapsed));
	dPing = boost::accumulators::extended_p_square(*asQuantile)[0];
	if (dPing == 0.0)
		dPing = elapsed;

	quint32 ping = static_cast<quint32>(lround(dPing / 1000.));
	uiRecv = static_cast<quint32>(boost::accumulators::count(* asQuantile));

	bool changed = (ping != uiPing) || (users != uiUsers) || (maxusers != uiMaxUsers);

	uiUsers = users;
	uiMaxUsers = maxusers;
	uiPing = ping;

	double grace = qMax(5000., 50. * uiPingSort);
	double diff = fabs(1000. * uiPingSort - dPing);

	if ((uiPingSort == 0) || ((uiSent >= 10) && (diff >= grace)))
		uiPingSort = ping;

	if (changed)
		emitDataChanged();
}

FavoriteServer ServerItem::toFavoriteServer() const {
	FavoriteServer fs;
	fs.qsName = qsName;
	if (! qsBonjourHost.isEmpty())
		fs.qsHostname = QLatin1Char('@') + qsBonjourHost;
	else
		fs.qsHostname = qsHostname;
	fs.usPort = usPort;
	fs.qsUsername = qsUsername;
	fs.qsPassword = qsPassword;
	fs.qsUrl = qsUrl;
	return fs;
}


/**
 * This function turns a ServerItem object into a QMimeData object holding a URL to the server.
 */
QMimeData *ServerItem::toMimeData() const {
	QMimeData *mime = ServerItem::toMimeData(qsName, qsHostname, usPort);

	if (itType == FavoriteType)
		mime->setData(QLatin1String("OriginatedInMumble"), QByteArray());

	return mime;
}

/**
 * This function creates a QMimeData object containing a URL to the server at host and port. name is passed in the
 * query string as "title", which is used for adding a server to favorites. channel may be omitted, but if specified it
 * should be in the format of "/path/to/channel".
 */
QMimeData *ServerItem::toMimeData(const QString &name, const QString &host, unsigned short port, const QString &channel) {
	QUrl url;
	url.setScheme(QLatin1String("mumble"));
	url.setHost(host);
	if (port != DEFAULT_MUMBLE_PORT)
		url.setPort(port);
	url.setPath(channel);

#if QT_VERSION >= 0x050000
	QUrlQuery query;
	query.addQueryItem(QLatin1String("title"), name);
	query.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));
	url.setQuery(query);
#else
	url.addQueryItem(QLatin1String("title"), name);
	url.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));
#endif

	QString qs = QLatin1String(url.toEncoded());

	QMimeData *mime = new QMimeData;

#ifdef Q_OS_WIN
	QString contents = QString::fromLatin1("[InternetShortcut]\r\nURL=%1\r\n").arg(qs);
	QString urlname = QString::fromLatin1("%1.url").arg(name);

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
	wcscpy_s(fgdw.fgd[0].cFileName, MAX_PATH, urlname.toStdWString().c_str());
	mime->setData(QLatin1String("FileGroupDescriptorW"), QByteArray(reinterpret_cast<const char *>(&fgdw), sizeof(fgdw)));

	mime->setData(QString::fromWCharArray(CFSTR_FILECONTENTS), contents.toLocal8Bit());

	DWORD context[4];
	context[0] = 0;
	context[1] = 1;
	context[2] = 0;
	context[3] = 0;
	mime->setData(QLatin1String("DragContext"), QByteArray(reinterpret_cast<const char *>(&context[0]), sizeof(context)));

	DWORD dropaction;
	dropaction = DROPEFFECT_LINK;
	mime->setData(QString::fromWCharArray(CFSTR_PREFERREDDROPEFFECT), QByteArray(reinterpret_cast<const char *>(&dropaction), sizeof(dropaction)));
#endif
	QList<QUrl> urls;
	urls << url;
	mime->setUrls(urls);

	mime->setText(qs);
	mime->setHtml(QString::fromLatin1("<a href=\"%1\">%2</a>").arg(qs).arg(Qt::escape(name)));

	return mime;
}

bool ServerItem::operator <(const QTreeWidgetItem &o) const {
	const ServerItem &other = static_cast<const ServerItem &>(o);
	const QTreeWidget *w = treeWidget();

	const int column = w ? w->sortColumn() : 0;

	if (itType != other.itType) {
		const bool inverse = w ? (w->header()->sortIndicatorOrder() == Qt::DescendingOrder) : false;
		bool less;

		if (itType == FavoriteType)
			less = true;
		else if ((itType == LANType) && (other.itType == PublicType))
			less = true;
		else
			less = false;
		return less ^ inverse;
	}

	if (bParent) {
		const bool inverse = w ? (w->header()->sortIndicatorOrder() == Qt::DescendingOrder) : false;
		return (qsName < other.qsName) ^ inverse;
	}

	if (column == 0) {
		QString a = qsName.toLower();
		QString b = other.qsName.toLower();

		QRegExp re(QLatin1String("[^0-9a-z]"));
		a.remove(re);
		b.remove(re);
		return a < b;
	} else if (column == 1) {
		quint32 a = uiPingSort ? uiPingSort : UINT_MAX;
		quint32 b = other.uiPingSort ? other.uiPingSort : UINT_MAX;
		return a < b;
	} else if (column == 2) {
		return uiUsers < other.uiUsers;
	}
	return false;
}

QIcon ServerItem::loadIcon(const QString &name) {
	if (! qmIcons.contains(name))
		qmIcons.insert(name, QIcon(name));
	return qmIcons.value(name);
}

ConnectDialogEdit::ConnectDialogEdit(QWidget *p, const QString &name, const QString &host, const QString &user, unsigned short port, const QString &password) : QDialog(p) {
	setupUi(this);
	init();

	bCustomLabel = ! name.simplified().isEmpty();

	qleName->setText(name);
	qleServer->setText(host);
	qleUsername->setText(user);
	qlePort->setText(QString::number(port));
	qlePassword->setText(password);

	validate();
}

ConnectDialogEdit::ConnectDialogEdit(QWidget *parent) : QDialog(parent) {
	setupUi(this);
	setWindowTitle(tr("Add Server"));
	init();

	if (!updateFromClipboard()) {
		// If connected to a server assume the user wants to add it
		if (g.sh && g.sh->isRunning()) {
			QString host, name, user, pw;
			unsigned short port = DEFAULT_MUMBLE_PORT;

			g.sh->getConnectionInfo(host, port, user, pw);
			Channel *c = Channel::get(0);
			if (c && c->qsName != QLatin1String("Root")) {
				name = c->qsName;
			}

			showNotice(tr("You are currently connected to a server.\nDo you want to fill the dialog with the connection data of this server?\nHost: %1 Port: %2").arg(host).arg(port));
			m_si = new ServerItem(name, host, port, user, pw);
		}
	}
	qleUsername->setText(g.s.qsUsername);
}

void ConnectDialogEdit::init() {
	m_si = NULL;
	usPort = 0;
	bOk = true;
	bCustomLabel = false;

	qwInlineNotice->hide();

	qlePort->setValidator(new QIntValidator(1, 65535, qlePort));
	qlePort->setText(QString::number(DEFAULT_MUMBLE_PORT));
	qlePassword->setEchoMode(QLineEdit::Password);

	connect(qleName, SIGNAL(textChanged(const QString &)), this, SLOT(validate()));
	connect(qleServer, SIGNAL(textChanged(const QString &)), this, SLOT(validate()));
	connect(qlePort, SIGNAL(textChanged(const QString &)), this, SLOT(validate()));
	connect(qleUsername, SIGNAL(textChanged(const QString &)), this, SLOT(validate()));
	connect(qlePassword, SIGNAL(textChanged(const QString &)), this, SLOT(validate()));

	validate();
}

ConnectDialogEdit::~ConnectDialogEdit() {
	delete m_si;
}

void ConnectDialogEdit::showNotice(const QString &text) {
	QLabel *label = qwInlineNotice->findChild<QLabel *>(QLatin1String("qlPasteNotice"));
	Q_ASSERT(label);
	label->setText(text);
	qwInlineNotice->show();
	adjustSize();
}

bool ConnectDialogEdit::updateFromClipboard() {
	delete m_si;
	m_si = ServerItem::fromMimeData(QApplication::clipboard()->mimeData(), false, NULL, true);
	bool hasServerData = m_si != NULL;
	if (hasServerData) {
		showNotice(tr("You have an URL in your clipboard.\nDo you want to fill the dialog with this data?\nHost: %1 Port: %2").arg(m_si->qsHostname).arg(m_si->usPort));
		return true;
	} else {
		qwInlineNotice->hide();
		adjustSize();
		return false;
	}
}

void ConnectDialogEdit::on_qbFill_clicked() {
	Q_ASSERT(m_si);

	qwInlineNotice->hide();
	adjustSize();

	qleName->setText(m_si->qsName);
	qleServer->setText(m_si->qsHostname);
	qleUsername->setText(m_si->qsUsername);
	qlePort->setText(QString::number(m_si->usPort));
	qlePassword->setText(m_si->qsPassword);

	delete m_si;
	m_si = NULL;
}

void ConnectDialogEdit::on_qbDiscard_clicked() {
	qwInlineNotice->hide();
	adjustSize();
}

void ConnectDialogEdit::on_qleName_textEdited(const QString& name) {
	if (bCustomLabel) {
		// If empty, then reset to automatic label.
		// NOTE(nik@jnstw.us): You may be tempted to set qleName to qleServer, but that results in the odd
		// UI behavior that clearing the field doesn't clear it; it'll immediately equal qleServer. Instead,
		// leave it empty and let it update the next time qleServer updates. Code in accept will default it
		// to qleServer if it isn't updated beforehand.
		if (name.simplified().isEmpty()) {
			bCustomLabel = false;
		}
	} else {
		// If manually edited, set to Custom
		bCustomLabel = true;
	}
}

void ConnectDialogEdit::on_qleServer_textEdited(const QString& server) {
	// If using automatic label, update it
	if (!bCustomLabel) {
		qleName->setText(server);
	}
}

void ConnectDialogEdit::validate() {
	qsName = qleName->text().simplified();
	qsHostname = qleServer->text().simplified();
	usPort = qlePort->text().toUShort();
	qsUsername = qleUsername->text().simplified();
	qsPassword = qlePassword->text();

	// For bonjour hosts disable the port field as it's auto-detected
	qlePort->setDisabled(!qsHostname.isEmpty() && qsHostname.startsWith(QLatin1Char('@')));

	// For SuperUser show password edit
	if (qsUsername.toLower() == QLatin1String("superuser")) {
		qliPassword->setVisible(true);
		qlePassword->setVisible(true);
		qcbShowPassword->setVisible(true);
		adjustSize();
	} else if (qsPassword.isEmpty()) {
		qliPassword->setVisible(false);
		qlePassword->setVisible(false);
		qcbShowPassword->setVisible(false);
		adjustSize();
	}

	bOk = ! qsHostname.isEmpty() && ! qsUsername.isEmpty() && usPort;
	qdbbButtonBox->button(QDialogButtonBox::Ok)->setEnabled(bOk);
}

void ConnectDialogEdit::accept() {
	validate();
	if (bOk) {
		QString server = qleServer->text().simplified();

		// If the user accidentally added a schema or path part, drop it now.
		// We can't do so during editing as that is quite jarring.
		const int schemaPos = server.indexOf(QLatin1String("://"));
		if (schemaPos != -1) {
			server.remove(0, schemaPos + 3);
		}

		const int pathPos = server.indexOf(QLatin1Char('/'));
		if (pathPos != -1) {
			server.resize(pathPos);
		}

		qleServer->setText(server);

		if (qleName->text().simplified().isEmpty() || !bCustomLabel) {
			qleName->setText(server);
		}

		QDialog::accept();
	}
}

void ConnectDialogEdit::on_qcbShowPassword_toggled(bool checked) {
	qlePassword->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

ConnectDialog::ConnectDialog(QWidget *p, bool autoconnect) : QDialog(p), bAutoConnect(autoconnect) {
	setupUi(this);
#ifdef Q_OS_MAC
	setWindowModality(Qt::WindowModal);
#endif
	bPublicInit = false;

	siAutoConnect = NULL;

	bAllowPing = g.s.ptProxyType == Settings::NoProxy;
	bAllowHostLookup = g.s.ptProxyType == Settings::NoProxy;
	bAllowBonjour = g.s.ptProxyType == Settings::NoProxy;
	bAllowFilters = g.s.ptProxyType == Settings::NoProxy;

	if (tPublicServers.elapsed() >= 60 * 24 * 1000000ULL) {
		qlPublicServers.clear();
	}

	qdbbButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	qdbbButtonBox->button(QDialogButtonBox::Ok)->setText(tr("&Connect"));

	QPushButton *qpbAdd = new QPushButton(tr("&Add New..."), this);
	qpbAdd->setDefault(false);
	qpbAdd->setAutoDefault(false);
	connect(qpbAdd, SIGNAL(clicked()), qaFavoriteAddNew, SIGNAL(triggered()));
	qdbbButtonBox->addButton(qpbAdd, QDialogButtonBox::ActionRole);

	
	qpbEdit = new QPushButton(tr("&Edit..."), this);
	qpbEdit->setEnabled(false);
	qpbEdit->setDefault(false);
	qpbEdit->setAutoDefault(false);
	connect(qpbEdit, SIGNAL(clicked()), qaFavoriteEdit, SIGNAL(triggered()));
	qdbbButtonBox->addButton(qpbEdit, QDialogButtonBox::ActionRole);
	
	qpbAdd->setHidden(g.s.disableConnectDialogEditing);
	qpbEdit->setHidden(g.s.disableConnectDialogEditing);

	qtwServers->setItemDelegate(new ServerViewDelegate());

	// Hide ping and user count if we are not allowed to ping.
	if (!bAllowPing) {
		qtwServers->setColumnCount(1);
	}

	qtwServers->sortItems(1, Qt::AscendingOrder);

#if QT_VERSION >= 0x050000
	qtwServers->header()->setSectionResizeMode(0, QHeaderView::Stretch);
	if (qtwServers->columnCount() >= 2) {
		qtwServers->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	}
	if (qtwServers->columnCount() >= 3) {
		qtwServers->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	}
#else
	qtwServers->header()->setResizeMode(0, QHeaderView::Stretch);
	if (qtwServers->columnCount() >= 2) {
		qtwServers->header()->setResizeMode(1, QHeaderView::ResizeToContents);
	}
	if (qtwServers->columnCount() >= 3) {
		qtwServers->header()->setResizeMode(2, QHeaderView::ResizeToContents);
	}
#endif

	connect(qtwServers->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(OnSortChanged(int, Qt::SortOrder)));

	qaShowAll->setChecked(false);
	qaShowReachable->setChecked(false);
	qaShowPopulated->setChecked(false);

	if (bAllowFilters) {
		switch (g.s.ssFilter) {
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
	} else {
		qaShowAll->setChecked(true);
	}

	qagFilters = new QActionGroup(this);
	qagFilters->addAction(qaShowAll);
	qagFilters->addAction(qaShowReachable);
	qagFilters->addAction(qaShowPopulated);

	connect(qagFilters, SIGNAL(triggered(QAction *)), this, SLOT(onFiltersTriggered(QAction *)));

	qmPopup = new QMenu(this);
	qmFilters = new QMenu(tr("&Filters"), this);
	qmFilters->addAction(qaShowAll);
	qmFilters->addAction(qaShowReachable);
	qmFilters->addAction(qaShowPopulated);

	if (!bAllowFilters) {
		qmFilters->setEnabled(false);
	}

	QList<QTreeWidgetItem *> ql;
	QList<FavoriteServer> favorites = g.db->getFavorites();

	foreach(const FavoriteServer &fs, favorites) {
		ServerItem *si = new ServerItem(fs);
		qlItems << si;
		startDns(si);
		qtwServers->siFavorite->addServerItem(si);
	}

#ifdef USE_BONJOUR
	// Make sure the we got the objects we need, then wire them up
	if (bAllowBonjour && g.bc->bsbBrowser && g.bc->bsrResolver) {
		connect(g.bc->bsbBrowser.data(), SIGNAL(error(DNSServiceErrorType)),
		        this, SLOT(onLanBrowseError(DNSServiceErrorType)));
		connect(g.bc->bsbBrowser.data(), SIGNAL(currentBonjourRecordsChanged(const QList<BonjourRecord> &)),
		        this, SLOT(onUpdateLanList(const QList<BonjourRecord> &)));
		connect(g.bc->bsrResolver.data(), SIGNAL(error(BonjourRecord, DNSServiceErrorType)),
		        this, SLOT(onLanResolveError(BonjourRecord, DNSServiceErrorType)));
		connect(g.bc->bsrResolver.data(), SIGNAL(bonjourRecordResolved(BonjourRecord, QString, int)),
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

	if (qtwServers->siFavorite->isHidden()
	    && (!qtwServers->siLAN || qtwServers->siLAN->isHidden())
	    && qtwServers->siPublic != NULL) {
		qtwServers->siPublic->setExpanded(true);
	}

	iPingIndex = -1;
	qtPingTick->start(50);

	new QShortcut(QKeySequence(QKeySequence::Copy), this, SLOT(on_qaFavoriteCopy_triggered()));
	new QShortcut(QKeySequence(QKeySequence::Paste), this, SLOT(on_qaFavoritePaste_triggered()));

	qtwServers->setCurrentItem(NULL);
	bLastFound = false;

	qmPingCache = g.db->getPingCache();

	if (! g.s.qbaConnectDialogGeometry.isEmpty())
		restoreGeometry(g.s.qbaConnectDialogGeometry);
	if (! g.s.qbaConnectDialogHeader.isEmpty())
		qtwServers->header()->restoreState(g.s.qbaConnectDialogHeader);
}

ConnectDialog::~ConnectDialog() {
	ServerItem::qmIcons.clear();

	QList<FavoriteServer> ql;
	qmPingCache.clear();

	foreach(ServerItem *si, qlItems) {
		if (si->uiPing)
			qmPingCache.insert(UnresolvedServerAddress(si->qsHostname, si->usPort), si->uiPing);

		if (si->itType != ServerItem::FavoriteType)
			continue;
		ql << si->toFavoriteServer();
	}
	g.db->setFavorites(ql);
	g.db->setPingCache(qmPingCache);

	g.s.qbaConnectDialogHeader = qtwServers->header()->saveState();
	g.s.qbaConnectDialogGeometry = saveGeometry();
}

void ConnectDialog::accept() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || (bAllowHostLookup && si->qlAddresses.isEmpty()) || si->qsHostname.isEmpty()) {
		qWarning() << "Invalid server";
		return;
	}

	qsPassword = si->qsPassword;
	qsServer = si->qsHostname;
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

void ConnectDialog::OnSortChanged(int logicalIndex, Qt::SortOrder) {
	if (logicalIndex != 2) {
		return;
	}

	foreach(ServerItem *si, qlItems) {
		if (si->uiPing && (si->uiPing != si->uiPingSort)) {
			si->uiPingSort = si->uiPing;
			si->setDatas();
		}
	}
}

void ConnectDialog::on_qaFavoriteAdd_triggered() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || (si->itType == ServerItem::FavoriteType))
		return;

	si = new ServerItem(si);
	qtwServers->fixupName(si);
	qlItems << si;
	qtwServers->siFavorite->addServerItem(si);
	qtwServers->setCurrentItem(si);
	startDns(si);
}

void ConnectDialog::on_qaFavoriteAddNew_triggered() {
	ConnectDialogEdit *cde = new ConnectDialogEdit(this);

	if (cde->exec() == QDialog::Accepted) {
		ServerItem *si = new ServerItem(cde->qsName, cde->qsHostname, cde->usPort, cde->qsUsername, cde->qsPassword);
		qlItems << si;
		qtwServers->siFavorite->addServerItem(si);
		qtwServers->setCurrentItem(si);
		startDns(si);
	}
	delete cde;
}

void ConnectDialog::on_qaFavoriteEdit_triggered() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || (si->itType != ServerItem::FavoriteType))
		return;

	QString host;
	if (! si->qsBonjourHost.isEmpty())
		host = QLatin1Char('@') + si->qsBonjourHost;
	else
		host = si->qsHostname;

	ConnectDialogEdit *cde = new ConnectDialogEdit(this, si->qsName, host, si->qsUsername, si->usPort, si->qsPassword);

	if (cde->exec() == QDialog::Accepted) {

		si->qsName = cde->qsName;
		si->qsUsername = cde->qsUsername;
		si->qsPassword = cde->qsPassword;
		if ((cde->qsHostname != host) || (cde->usPort != si->usPort)) {
			stopDns(si);

			si->qlAddresses.clear();
			si->reset();

			si->usPort = cde->usPort;

			if (cde->qsHostname.startsWith(QLatin1Char('@'))) {
				si->qsHostname = QString();
				si->qsBonjourHost = cde->qsHostname.mid(1);
				si->brRecord = BonjourRecord(si->qsBonjourHost, QLatin1String("_mumble._tcp."), QLatin1String("local."));
			} else {
				si->qsHostname = cde->qsHostname;
				si->qsBonjourHost = QString();
				si->brRecord = BonjourRecord();
			}
			startDns(si);
		}
		si->setDatas();
	}
	delete cde;
}

void ConnectDialog::on_qaFavoriteRemove_triggered() {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->currentItem());
	if (! si || (si->itType != ServerItem::FavoriteType))
		return;

	stopDns(si);
	qlItems.removeAll(si);
	delete si;
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
	qtwServers->siFavorite->addServerItem(si);
	qtwServers->setCurrentItem(si);
	startDns(si);
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

	foreach(ServerItem *si, qlItems)
		si->hideCheck();
}

void ConnectDialog::on_qtwServers_customContextMenuRequested(const QPoint &mpos) {
	ServerItem *si = static_cast<ServerItem *>(qtwServers->itemAt(mpos));
	qmPopup->clear();

	if (si != NULL && si->bParent) {
		si = NULL;
	}
	
	if (si != NULL) {

		if (!g.s.disableConnectDialogEditing) {
			if (si->itType == ServerItem::FavoriteType) {
				qmPopup->addAction(qaFavoriteEdit);
				qmPopup->addAction(qaFavoriteRemove);
			} else {
				qmPopup->addAction(qaFavoriteAdd);
			}
		}
		
		if (!si->qsUrl.isEmpty()) {
			qmPopup->addAction(qaUrl);
		}
	}
	
	if (! qmPopup->isEmpty()) {
		qmPopup->addSeparator();
	}
	
	qmPopup->addMenu(qmFilters);

	qmPopup->popup(qtwServers->viewport()->mapToGlobal(mpos), NULL);
}

void ConnectDialog::on_qtwServers_itemDoubleClicked(QTreeWidgetItem *item, int) {
	qtwServers->setCurrentItem(item);
	accept();
}

void ConnectDialog::on_qtwServers_currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *) {
	ServerItem *si = static_cast<ServerItem *>(item);

	if (si->siParent == qtwServers->siFavorite) {
		qpbEdit->setEnabled(true);
	} else {
		qpbEdit->setEnabled(false);
	}
	
	bool bOk = !si->qlAddresses.isEmpty();
	if (!bAllowHostLookup) {
		bOk = true;
	}
	qdbbButtonBox->button(QDialogButtonBox::Ok)->setEnabled(bOk);

	bLastFound = true;
}

void ConnectDialog::on_qtwServers_itemExpanded(QTreeWidgetItem *item) {
	if (qtwServers->siPublic != NULL && item == qtwServers->siPublic) {
		initList();
		fillList();
	}

	ServerItem *p = static_cast<ServerItem *>(item);

	foreach(ServerItem *si, p->qlChildren)
		startDns(si);
}

void ConnectDialog::initList() {
	if (bPublicInit || (qlPublicServers.count() > 0))
		return;

	bPublicInit = true;

	QUrl url;
	url.setPath(QLatin1String("/v1/list"));
#if QT_VERSION >= 0x050000
	QUrlQuery query;
	query.addQueryItem(QLatin1String("version"), QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)));
	url.setQuery(query);
#else
	url.addQueryItem(QLatin1String("version"), QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)));
#endif

	WebFetch::fetch(QLatin1String("publist"), url, this, SLOT(fetched(QByteArray,QUrl,QMap<QString,QString>)));
}

#ifdef USE_BONJOUR
void ConnectDialog::onResolved(BonjourRecord record, QString host, int port) {
	qlBonjourActive.removeAll(record);
	foreach(ServerItem *si, qlItems) {
		if (si->brRecord == record) {
			unsigned short usport = static_cast<unsigned short>(port);
			if ((host != si->qsHostname) || (usport != si->usPort)) {
				stopDns(si);
				si->usPort = static_cast<unsigned short>(port);
				si->qsHostname = host;
				startDns(si);
			}
		}
	}
}

void ConnectDialog::onUpdateLanList(const QList<BonjourRecord> &list) {
	QSet<ServerItem *> items;
	QSet<ServerItem *> old = qtwServers->siLAN->qlChildren.toSet();

	foreach(const BonjourRecord &record, list) {
		bool found = false;
		foreach(ServerItem *si, old) {
			if (si->brRecord == record) {
				items.insert(si);
				found = true;
				break;
			}
		}
		if (! found) {
			ServerItem *si = new ServerItem(record);
			qlItems << si;
			g.bc->bsrResolver->resolveBonjourRecord(record);
			startDns(si);
			qtwServers->siLAN->addServerItem(si);
		}
	}
	QSet<ServerItem *> remove = old.subtract(items);
	foreach(ServerItem *si, remove) {
		stopDns(si);
		qlItems.removeAll(si);
		delete si;
	}
}

void ConnectDialog::onLanBrowseError(DNSServiceErrorType err) {
	qWarning()<<"Bonjour reported browser error "<< err;
}

void ConnectDialog::onLanResolveError(BonjourRecord br, DNSServiceErrorType err) {
	qlBonjourActive.removeAll(br);
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
				si->qsCountry = pi.qsCountry;
				si->qsCountryCode = pi.qsCountryCode;
				si->qsContinentCode = pi.qsContinentCode;
				si->qsUrl = pi.quUrl.toString();
				si->bCA = pi.bCA;
				si->setDatas();

				if (si->itType == ServerItem::PublicType)
					found = true;
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

	foreach(QTreeWidgetItem *qtwi, qlNew) {
		ServerItem *si = static_cast<ServerItem *>(qtwi);
		ServerItem *p = qtwServers->getParent(si->qsContinentCode, si->qsCountryCode, si->qsCountry, qsUserContinentCode, qsUserCountryCode);
		p->addServerItem(si);

		if (p->isExpanded() && p->parent()->isExpanded())
			startDns(si);
	}
}

void ConnectDialog::timeTick() {
	if (! bLastFound && ! g.s.qsLastServer.isEmpty()) {
		QList<QTreeWidgetItem *> items = qtwServers->findItems(g.s.qsLastServer, Qt::MatchExactly | Qt::MatchRecursive);
		if (!items.isEmpty()) {
			bLastFound = true;
			qtwServers->setCurrentItem(items.at(0));
			if (g.s.bAutoConnect && bAutoConnect) {
				siAutoConnect = static_cast<ServerItem *>(items.at(0));
				if (! siAutoConnect->qlAddresses.isEmpty()) {
					accept();
					return;
				} else if (!bAllowHostLookup) {
					accept();
					return;
				}
			}
		}
	}

	if (bAllowHostLookup) {
		// Start DNS Lookup of first unknown hostname
		foreach(const UnresolvedServerAddress &unresolved, qlDNSLookup) {
			if (qsDNSActive.contains(unresolved)) {
				continue;
			}

			qlDNSLookup.removeAll(unresolved);
			qlDNSLookup.append(unresolved);

			qsDNSActive.insert(unresolved);
			ServerResolver *sr = new ServerResolver();
			QObject::connect(sr, SIGNAL(resolved()), this, SLOT(lookedUp()));
			sr->resolve(unresolved.hostname, unresolved.port);
			break;
		}
	}

	ServerItem *current = static_cast<ServerItem *>(qtwServers->currentItem());
	ServerItem *hover = static_cast<ServerItem *>(qtwServers->itemAt(qtwServers->viewport()->mapFromGlobal(QCursor::pos())));

	ServerItem *si = NULL;

	if (tCurrent.elapsed() >= 1000000ULL)
		si = current;
	if (! si && (tHover.elapsed() >= 1000000ULL))
		si = hover;

	if (si) {
		QString hostname = si->qsHostname.toLower();
		unsigned short port = si->usPort;
		UnresolvedServerAddress unresolved(hostname, port);

		if (si->qlAddresses.isEmpty()) {
			if (! hostname.isEmpty()) {
				qlDNSLookup.removeAll(unresolved);
				qlDNSLookup.prepend(unresolved);
			}
			si = NULL;
		}
	}

	if (!si) {
		if (qlItems.isEmpty())
			return;

		bool expanded;

		do {
			++iPingIndex;
			if (iPingIndex >= qlItems.count()) {
				if (tRestart.isElapsed(1000000ULL))
					iPingIndex = 0;
				else
					return;
			}
			si = qlItems.at(iPingIndex);

			ServerItem *p = si->siParent;
			expanded = true;
			while (p && expanded) {
				expanded = expanded && p->isExpanded();
				p = p->siParent;
			}
		} while (si->qlAddresses.isEmpty() || ! expanded);
	}

	if (si == current)
		tCurrent.restart();
	if (si == hover)
		tHover.restart();

	foreach(const ServerAddress &addr, si->qlAddresses) {
		sendPing(addr.host.toAddress(), addr.port);
	}
}


void ConnectDialog::startDns(ServerItem *si) {
	if (!bAllowHostLookup) {
		return;
	}

	QString hostname = si->qsHostname.toLower();
	unsigned short port = si->usPort;
	UnresolvedServerAddress unresolved(hostname, port);

	if (si->qlAddresses.isEmpty()) {
		// Determine if qsHostname is an IP address
		// or a hostname. If it is an IP address, we
		// can treat it as resolved as-is.
		QHostAddress qha(si->qsHostname);
		bool hostnameIsIPAddress = !qha.isNull();
		if (hostnameIsIPAddress) {
			si->qlAddresses.append(ServerAddress(HostAddress(qha), port));
		} else {
			si->qlAddresses = qhDNSCache.value(unresolved);
		}
	}

	if (qtwServers->currentItem() == si)
		qdbbButtonBox->button(QDialogButtonBox::Ok)->setEnabled(! si->qlAddresses.isEmpty());

	if (! si->qlAddresses.isEmpty()) {
		foreach(const ServerAddress &addr, si->qlAddresses) {
			qhPings[addr].insert(si);
		}
		return;
	}

#ifdef USE_BONJOUR
	if (bAllowBonjour && si->qsHostname.isEmpty() && ! si->brRecord.serviceName.isEmpty()) {
		if (! qlBonjourActive.contains(si->brRecord)) {
			g.bc->bsrResolver->resolveBonjourRecord(si->brRecord);
			qlBonjourActive.append(si->brRecord);
		}
		return;
	}
#endif

	if (! qhDNSWait.contains(unresolved)) {
		if (si->itType == ServerItem::PublicType)
			qlDNSLookup.append(unresolved);
		else
			qlDNSLookup.prepend(unresolved);
	}
	qhDNSWait[unresolved].insert(si);
}

void ConnectDialog::stopDns(ServerItem *si) {
	if (!bAllowHostLookup) {
		return;
	}

	foreach(const ServerAddress &addr, si->qlAddresses) {
		if (qhPings.contains(addr)) {
			qhPings[addr].remove(si);
			if (qhPings[addr].isEmpty()) {
				qhPings.remove(addr);
				qhPingRand.remove(addr);
			}
		}
	}

	QString hostname = si->qsHostname.toLower();
	unsigned short port = si->usPort;
	UnresolvedServerAddress unresolved(hostname, port);

	if (qhDNSWait.contains(unresolved)) {
		qhDNSWait[unresolved].remove(si);
		if (qhDNSWait[unresolved].isEmpty()) {
			qhDNSWait.remove(unresolved);
			qlDNSLookup.removeAll(unresolved);
		}
	}
}

void ConnectDialog::lookedUp() {
	ServerResolver *sr = qobject_cast<ServerResolver *>(QObject::sender());
	sr->deleteLater();

	QString hostname = sr->hostname().toLower();
	unsigned short port = sr->port();
	UnresolvedServerAddress unresolved(hostname, port);

	qsDNSActive.remove(unresolved);

	// An error occurred, or no records were found.
	if (sr->records().size() == 0) {
		return;
	}

	QSet<ServerAddress> qs;
	foreach (ServerResolverRecord record, sr->records()) {
		foreach(const HostAddress &ha, record.addresses()) {
			qs.insert(ServerAddress(ha, record.port()));
		}
	}

	QSet<ServerItem *> waiting = qhDNSWait[unresolved];
	foreach(ServerItem *si, waiting) {
		foreach (const ServerAddress &addr, qs) {
			qhPings[addr].insert(si);
		}

		si->qlAddresses = qs.toList();
	}

	qlDNSLookup.removeAll(unresolved);
	qhDNSCache.insert(unresolved, qs.toList());
	qhDNSWait.remove(unresolved);

	foreach(ServerItem *si, waiting) {
		if (si == qtwServers->currentItem()) {
			on_qtwServers_currentItemChanged(si, si);
			if (si == siAutoConnect)
				accept();
		}
	}

	if (bAllowPing) {
		foreach(const ServerAddress &addr, qs) {
			sendPing(addr.host.toAddress(), addr.port);
		}
	}
}

void ConnectDialog::sendPing(const QHostAddress &host, unsigned short port) {
	char blob[16];

	ServerAddress addr(HostAddress(host), port);

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

	const QSet<ServerItem *> &qs = qhPings.value(addr);

	foreach(ServerItem *si, qs)
		++ si->uiSent;
}

void ConnectDialog::udpReply() {
	QUdpSocket *sock = qobject_cast<QUdpSocket *>(sender());

	while (sock->hasPendingDatagrams()) {
		char blob[64];

		QHostAddress host;
		unsigned short port;

		qint64 len = sock->readDatagram(blob+4, 24, &host, &port);
		if (len == 24) {
			if (host.scopeId() == QLatin1String("0"))
				host.setScopeId(QLatin1String(""));

			ServerAddress address(HostAddress(host), port);

			if (qhPings.contains(address)) {
				quint32 *ping = reinterpret_cast<quint32 *>(blob+4);
				quint64 *ts = reinterpret_cast<quint64 *>(blob+8);

				quint64 elapsed = tPing.elapsed() - (*ts ^ qhPingRand.value(address));

				foreach(ServerItem *si, qhPings.value(address)) {
					si->uiVersion = qFromBigEndian(ping[0]);
					quint32 users = qFromBigEndian(ping[3]);
					quint32 maxusers = qFromBigEndian(ping[4]);
					si->uiBandwidth = qFromBigEndian(ping[5]);

					if (! si->uiPingSort)
						si->uiPingSort = qmPingCache.value(UnresolvedServerAddress(si->qsHostname, si->usPort));

					si->setDatas(static_cast<double>(elapsed), users, maxusers);
					si->hideCheck();
				}
			}
		}
	}
}

void ConnectDialog::fetched(QByteArray xmlData, QUrl, QMap<QString, QString> headers) {
	if (xmlData.isNull()) {
		QMessageBox::warning(this, QLatin1String("Mumble"), tr("Failed to fetch server list"), QMessageBox::Ok);
		return;
	}

	QDomDocument doc;
	doc.setContent(xmlData);

	qlPublicServers.clear();
	qsUserCountry = headers.value(QLatin1String("Geo-Country"));
	qsUserCountryCode = headers.value(QLatin1String("Geo-Country-Code")).toLower();
	qsUserContinentCode = headers.value(QLatin1String("Geo-Continent-Code")).toLower();

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
				pi.qsCountry = e.attribute(QLatin1String("country"), tr("Unknown"));
				pi.qsCountryCode = e.attribute(QLatin1String("country_code")).toLower();
				pi.qsContinentCode = e.attribute(QLatin1String("continent_code")).toLower();
				pi.bCA = e.attribute(QLatin1String("ca")).toInt() ? true : false;

				qlPublicServers << pi;
			}
		}
		n = n.nextSibling();
	}

	tPublicServers.restart();

	fillList();
}
