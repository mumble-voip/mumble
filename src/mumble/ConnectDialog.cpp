// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ConnectDialog.h"

#ifdef USE_ZEROCONF
#	include "Zeroconf.h"
#endif

#include "Channel.h"
#include "Database.h"
#include "ServerHandler.h"
#include "ServerResolver.h"
#include "Utils.h"
#include "WebFetch.h"
#include "Global.h"

#include <QSettings>
#include <QShortcut>
#include <QtCore/QMimeData>
#include <QtCore/QRegularExpression>
#include <QtCore/QUrlQuery>
#include <QtCore/QtEndian>
#include <QtGui/QClipboard>
#include <QtGui/QDesktopServices>
#include <QtGui/QPainter>
#include <QtNetwork/QUdpSocket>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtXml/QDomDocument>

#include <boost/accumulators/statistics/extended_p_square.hpp>
#include <boost/array.hpp>

#ifdef Q_OS_WIN
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include <shlobj.h>
#endif

#include <QRandomGenerator>

#include <algorithm>

QMap< QString, QIcon > ServerItem::qmIcons;
QList< PublicInfo > ConnectDialog::qlPublicServers;
QString ConnectDialog::qsUserCountry, ConnectDialog::qsUserCountryCode, ConnectDialog::qsUserContinentCode;
Timer ConnectDialog::tPublicServers;


PingStats::PingStats() {
	init();
}

PingStats::~PingStats() {
	delete asQuantile;
}

void PingStats::init() {
	boost::array< double, 3 > probs = { { 0.75, 0.80, 0.95 } };

	asQuantile  = new asQuantileType(boost::accumulators::tag::extended_p_square::probabilities = probs);
	dPing       = 0.0;
	uiPing      = 0;
	uiPingSort  = 0;
	uiUsers     = 0;
	uiMaxUsers  = 0;
	uiBandwidth = 0;
	uiSent      = 0;
	uiRecv      = 0;
	m_version   = Version::UNKNOWN;
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
		painter->fillRect(option.rect, bg.value< QBrush >());
	}

	QStyledItemDelegate::paint(painter, option, index);
}

ServerView::ServerView(QWidget *p) : QTreeWidget(p) {
	siFavorite = new ServerItem(tr("Favorite"), ServerItem::FavoriteType);
	addTopLevelItem(siFavorite);
	siFavorite->setExpanded(true);
	siFavorite->setHidden(true);

#ifdef USE_ZEROCONF
	siLAN = new ServerItem(tr("LAN"), ServerItem::LANType);
	addTopLevelItem(siLAN);
	siLAN->setExpanded(true);
	siLAN->setHidden(true);
#else
	siLAN         = nullptr;
#endif

	if (!Global::get().s.bDisablePublicList) {
		siPublic = new ServerItem(tr("Public Internet"), ServerItem::PublicType);
		siPublic->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
		addTopLevelItem(siPublic);

		siPublic->setExpanded(false);
	} else {
		qWarning() << "Public list disabled";

		siPublic = nullptr;
	}
}

ServerView::~ServerView() {
	delete siFavorite;
	delete siLAN;
	delete siPublic;
}

QMimeData *ServerView::mimeData(const QList< QTreeWidgetItem * > &mimeitems) const {
	if (mimeitems.isEmpty())
		return nullptr;

	ServerItem *si = static_cast< ServerItem * >(mimeitems.first());
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

	const QRegularExpression regex(QRegularExpression::anchoredPattern(QLatin1String("(.+)\\((\\d+)\\)")),
								   QRegularExpression::InvertedGreedinessOption);
	const QRegularExpressionMatch match = regex.match(name);
	if (match.hasMatch()) {
		name = match.capturedTexts().at(1).trimmed();
		tag  = match.capturedTexts().at(2).toInt();
	}

	bool found;
	QString cmpname;
	do {
		found = false;
		if (tag > 1)
			cmpname = name + QString::fromLatin1(" (%1)").arg(tag);
		else
			cmpname = name;

		foreach (ServerItem *f, siFavorite->qlChildren)
			if (f->qsName == cmpname)
				found = true;

		++tag;
	} while (found);

	si->qsName = cmpname;
}

bool ServerView::dropMimeData(QTreeWidgetItem *, int, const QMimeData *mime, Qt::DropAction) {
	ServerItem *si = ServerItem::fromMimeData(mime);
	if (!si)
		return false;

	fixupName(si);

	qobject_cast< ConnectDialog * >(parent())->qlItems << si;
	siFavorite->addServerItem(si);

	qobject_cast< ConnectDialog * >(parent())->startDns(si);

	setCurrentItem(si);

	return true;
}

void ServerItem::init() {
	// Without this, columncount is wrong.
	setData(0, Qt::DisplayRole, QVariant());
	setData(1, Qt::DisplayRole, QVariant());
	setData(2, Qt::DisplayRole, QVariant());
	emitDataChanged();
}

ServerItem::ServerItem(const FavoriteServer &fs) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	siParent = nullptr;
	bParent  = false;

	itType = FavoriteType;
	qsName = fs.qsName;
	usPort = fs.usPort;

	qsUsername = fs.qsUsername;
	qsPassword = fs.qsPassword;

	qsUrl = fs.qsUrl;

	bCA = false;
#ifdef USE_ZEROCONF
	if (fs.qsHostname.startsWith(QLatin1Char('@'))) {
		zeroconfHost   = fs.qsHostname.mid(1);
		zeroconfRecord = BonjourRecord(zeroconfHost, QLatin1String("_mumble._tcp."), QLatin1String("local."));
	} else {
		qsHostname = fs.qsHostname;
	}
#else
	qsHostname    = fs.qsHostname;
#endif
	init();
}

ServerItem::ServerItem(const PublicInfo &pi) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	siParent        = nullptr;
	bParent         = false;
	itType          = PublicType;
	qsName          = pi.qsName;
	qsHostname      = pi.qsIp;
	usPort          = pi.usPort;
	qsUrl           = pi.quUrl.toString();
	qsCountry       = pi.qsCountry;
	qsCountryCode   = pi.qsCountryCode;
	qsContinentCode = pi.qsContinentCode;
	bCA             = pi.bCA;

	init();
}

ServerItem::ServerItem(const QString &name, const QString &host, unsigned short port, const QString &username,
					   const QString &password)
	: QTreeWidgetItem(QTreeWidgetItem::UserType) {
	siParent   = nullptr;
	bParent    = false;
	itType     = FavoriteType;
	qsName     = name;
	usPort     = port;
	qsUsername = username;
	qsPassword = password;

	bCA = false;
#ifdef USE_ZEROCONF
	if (host.startsWith(QLatin1Char('@'))) {
		zeroconfHost   = host.mid(1);
		zeroconfRecord = BonjourRecord(zeroconfHost, QLatin1String("_mumble._tcp."), QLatin1String("local."));
	} else {
		qsHostname = host;
	}
#else
	qsHostname    = host;
#endif
	init();
}

#ifdef USE_ZEROCONF
ServerItem::ServerItem(const BonjourRecord &br) : QTreeWidgetItem(QTreeWidgetItem::UserType) {
	siParent       = nullptr;
	bParent        = false;
	itType         = LANType;
	qsName         = br.serviceName;
	zeroconfHost   = qsName;
	zeroconfRecord = br;
	usPort         = 0;
	bCA            = false;

	init();
}
#endif

ServerItem::ServerItem(const QString &name, ItemType itype) {
	siParent = nullptr;
	bParent  = true;
	qsName   = name;
	itType   = itype;
	setFlags(flags() & ~Qt::ItemIsDragEnabled);
	bCA = false;

	init();
}

ServerItem::ServerItem(const ServerItem *si) {
	siParent = nullptr;
	bParent  = false;
	itType   = FavoriteType;

	qsName          = si->qsName;
	qsHostname      = si->qsHostname;
	usPort          = si->usPort;
	qsUsername      = si->qsUsername;
	qsPassword      = si->qsPassword;
	qsCountry       = si->qsCountry;
	qsCountryCode   = si->qsCountryCode;
	qsContinentCode = si->qsContinentCode;
	qsUrl           = si->qsUrl;
#ifdef USE_ZEROCONF
	zeroconfHost   = si->zeroconfHost;
	zeroconfRecord = si->zeroconfRecord;
#endif
	qlAddresses = si->qlAddresses;
	bCA         = si->bCA;

	m_version   = si->m_version;
	uiPing      = si->uiPing;
	uiPingSort  = si->uiPing;
	uiUsers     = si->uiUsers;
	uiMaxUsers  = si->uiMaxUsers;
	uiBandwidth = si->uiBandwidth;
	uiSent      = si->uiSent;
	dPing       = si->dPing;
	*asQuantile = *si->asQuantile;
}

ServerItem::~ServerItem() {
	if (siParent) {
		siParent->qlChildren.removeAll(this);
		if (siParent->bParent && siParent->qlChildren.isEmpty())
			siParent->setHidden(true);
	}

	// This is just for cleanup when exiting the dialog, it won't stop pending DNS for the children.
	foreach (ServerItem *si, qlChildren)
		delete si;
}

ServerItem *ServerItem::fromMimeData(const QMimeData *mime, bool default_name, QWidget *p, bool convertHttpUrls) {
	if (mime->hasFormat(QLatin1String("OriginatedInMumble")))
		return nullptr;

	QUrl url;
	if (mime->hasUrls() && !mime->urls().isEmpty())
		url = mime->urls().at(0);
	else if (mime->hasText())
		url = QUrl::fromEncoded(mime->text().toUtf8());

	QString qsFile = url.toLocalFile();
	if (!qsFile.isEmpty()) {
		QFile f(qsFile);
		// Make sure we don't accidentally read something big the user
		// happened to have in his clipboard. We only want to look
		// at small link files.
		if (f.open(QIODevice::ReadOnly) && f.size() < 10240) {
			QByteArray qba = f.readAll();
			f.close();

			url = QUrl::fromEncoded(qba, QUrl::StrictMode);
			if (!url.isValid()) {
				// Windows internet shortcut files (.url) are an ini with an URL value
				QSettings qs(qsFile, QSettings::IniFormat);
				url =
					QUrl::fromEncoded(qs.value(QLatin1String("InternetShortcut/URL")).toByteArray(), QUrl::StrictMode);
			}
		}
	}

	if (default_name) {
		QUrlQuery query(url);
		if (!query.hasQueryItem(QLatin1String("title"))) {
			query.addQueryItem(QLatin1String("title"), url.host());
		}
	}

	if (!url.isValid()) {
		return nullptr;
	}

	// An URL from text without a scheme will have the hostname text
	// in the QUrl scheme and no hostname. We do not want to use that.
	if (url.host().isEmpty()) {
		return nullptr;
	}

	// Some communication programs automatically create http links from domains.
	// When a user sends another user a domain to connect to, and http is added wrongly,
	// we do our best to remove it again.
	if (convertHttpUrls && (url.scheme() == QLatin1String("http") || url.scheme() == QLatin1String("https"))) {
		url.setScheme(QLatin1String("mumble"));
	}

	return fromUrl(url, p);
}

ServerItem *ServerItem::fromUrl(QUrl url, QWidget *p) {
	if (!url.isValid() || (url.scheme() != QLatin1String("mumble"))) {
		return nullptr;
	}

	QUrlQuery query(url);

	if (url.userName().isEmpty()) {
		if (Global::get().s.qsUsername.isEmpty()) {
			bool ok;
			QString defUserName = QInputDialog::getText(p, ConnectDialog::tr("Adding host %1").arg(url.host()),
														ConnectDialog::tr("Enter username"), QLineEdit::Normal,
														Global::get().s.qsUsername, &ok)
									  .trimmed();
			if (!ok)
				return nullptr;
			if (defUserName.isEmpty())
				return nullptr;
			Global::get().s.qsUsername = defUserName;
		}
		url.setUserName(Global::get().s.qsUsername);
	}

	ServerItem *si =
		new ServerItem(query.queryItemValue(QLatin1String("title")), url.host(),
					   static_cast< unsigned short >(url.port(DEFAULT_MUMBLE_PORT)), url.userName(), url.password());

	if (query.hasQueryItem(QLatin1String("url")))
		si->qsUrl = query.queryItemValue(QLatin1String("url"));

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
					else
						return loadIcon(QLatin1String("skin:categories/applications-internet.svg"));
			}
		}
	} else {
		if (role == Qt::DecorationRole && column == 0) {
			QString flag;
			if (!qsCountryCode.isEmpty()) {
				flag = QString::fromLatin1(":/flags/%1.svg").arg(qsCountryCode);
				if (!QFileInfo(flag).exists()) {
					flag = QLatin1String("skin:categories/applications-internet.svg");
				}
			} else {
				flag = QLatin1String("skin:categories/applications-internet.svg");
			}
			return loadIcon(flag);
		}
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
			QStringList ipv4List;
			QStringList ipv6List;
			foreach (const ServerAddress &addr, qlAddresses) {
				const QString address = addr.host.toString(false).toHtmlEscaped();
				if (addr.host.isV6()) {
					ipv6List << address;
				} else {
					ipv4List << address;
				}
			}
			QString ipv4 = "-";
			QString ipv6 = "-";
			if (!ipv4List.isEmpty()) {
				ipv4 = ipv4List.join(QLatin1String(", "));
			}
			if (!ipv6List.isEmpty()) {
				ipv6 = ipv6List.join(QLatin1String(", "));
			}

			double ploss = 100.0;

			if (uiSent > 0)
				ploss = (uiSent - std::min(uiRecv, uiSent)) * 100. / uiSent;

			QString qs;
			qs += QLatin1String("<table>")
				  + QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
						.arg(ConnectDialog::tr("Servername"), qsName.toHtmlEscaped())
				  + QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
						.arg(ConnectDialog::tr("Hostname"), qsHostname.toHtmlEscaped());
#ifdef USE_ZEROCONF
			if (!zeroconfHost.isEmpty())
				qs += QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
						  .arg(ConnectDialog::tr("Bonjour name"), zeroconfHost.toHtmlEscaped());
#endif
			qs += QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
					  .arg(ConnectDialog::tr("Port"))
					  .arg(usPort)
				  + QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
						.arg(ConnectDialog::tr("IPv4 address"), ipv4)
				  + QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
						.arg(ConnectDialog::tr("IPv6 address"), ipv6);

			if (!qsUrl.isEmpty())
				qs += QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
						  .arg(ConnectDialog::tr("Website"), qsUrl.toHtmlEscaped());

			if (uiSent > 0) {
				qs += QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
						  .arg(ConnectDialog::tr("Packet loss"), QString::fromLatin1("%1% (%2/%3)")
																	 .arg(ploss, 0, 'f', 1)
																	 .arg(uiSent - std::min(uiRecv, uiSent))
																	 .arg(uiSent));
				if (uiRecv > 0) {
					qs += QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
							  .arg(ConnectDialog::tr("Ping (80%)"),
								   ConnectDialog::tr("%1 ms").arg(
									   boost::accumulators::extended_p_square(*asQuantile)[1] / 1000., 0, 'f', 2))
						  + QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
								.arg(ConnectDialog::tr("Ping (95%)"),
									 ConnectDialog::tr("%1 ms").arg(
										 boost::accumulators::extended_p_square(*asQuantile)[2] / 1000., 0, 'f', 2))
						  + QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
								.arg(ConnectDialog::tr("Bandwidth"),
									 ConnectDialog::tr("%1 kbit/s").arg(uiBandwidth / 1000))
						  + QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
								.arg(ConnectDialog::tr("Users"),
									 QString::fromLatin1("%1/%2").arg(uiUsers).arg(uiMaxUsers))
						  + QString::fromLatin1("<tr><th align=left>%1</th><td>%2</td></tr>")
								.arg(ConnectDialog::tr("Version"))
								.arg(Version::toString(m_version));
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
		} else if (role == Qt::AccessibleTextRole) {
			return QString("%1 %2").arg(ConnectDialog::tr("Server")).arg(qsName);
		}
	}
	return QTreeWidgetItem::data(column, role);
}

void ServerItem::addServerItem(ServerItem *childitem) {
	Q_ASSERT(!childitem->siParent);

	childitem->siParent = this;
	qlChildren.append(childitem);
	addChild(childitem);
	// Public servers must initially be hidden for the search to work properly
	// They will be set to visible later on
	if (childitem->itType == PublicType) {
		childitem->setHidden(true);
	}

	if (bParent && (itType != PublicType) && isHidden()) {
		setHidden(false);
	}
}

void ServerItem::setDatas(double elapsed, quint32 users, quint32 maxusers) {
	if (elapsed == 0.0) {
		emitDataChanged();
		return;
	}

	(*asQuantile)(static_cast< double >(elapsed));
	dPing = boost::accumulators::extended_p_square(*asQuantile)[0];
	if (dPing == 0.0)
		dPing = elapsed;

	quint32 ping = static_cast< quint32 >(lround(dPing / 1000.));
	uiRecv       = static_cast< quint32 >(boost::accumulators::count(*asQuantile));

	bool changed = (ping != uiPing) || (users != uiUsers) || (maxusers != uiMaxUsers);

	uiUsers    = users;
	uiMaxUsers = maxusers;
	uiPing     = ping;

	double grace = qMax(5000., 50. * uiPingSort);
	double diff  = fabs(1000. * uiPingSort - dPing);

	if ((uiPingSort == 0) || ((uiSent >= 10) && (diff >= grace)))
		uiPingSort = ping;

	if (changed)
		emitDataChanged();
}

FavoriteServer ServerItem::toFavoriteServer() const {
	FavoriteServer fs;
	fs.qsName = qsName;
#ifdef USE_ZEROCONF
	if (!zeroconfHost.isEmpty())
		fs.qsHostname = QLatin1Char('@') + zeroconfHost;
	else
		fs.qsHostname = qsHostname;
#else
	fs.qsHostname = qsHostname;
#endif
	fs.usPort     = usPort;
	fs.qsUsername = qsUsername;
	fs.qsPassword = qsPassword;
	fs.qsUrl      = qsUrl;
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
QMimeData *ServerItem::toMimeData(const QString &name, const QString &host, unsigned short port,
								  const QString &channel) {
	QUrl url;
	url.setScheme(QLatin1String("mumble"));
	url.setHost(host);
	if (port != DEFAULT_MUMBLE_PORT)
		url.setPort(port);
	url.setPath(channel);

	QUrlQuery query;
	query.addQueryItem(QLatin1String("title"), name);
	query.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));
	url.setQuery(query);

	QString qs = QLatin1String(url.toEncoded());

	QMimeData *mime = new QMimeData;

#ifdef Q_OS_WIN
	QString contents = QString::fromLatin1("[InternetShortcut]\r\nURL=%1\r\n").arg(qs);
	QString urlname  = QString::fromLatin1("%1.url").arg(name);

	FILEGROUPDESCRIPTORA fgda;
	ZeroMemory(&fgda, sizeof(fgda));
	fgda.cItems              = 1;
	fgda.fgd[0].dwFlags      = FD_LINKUI | FD_FILESIZE;
	fgda.fgd[0].nFileSizeLow = contents.length();
	strcpy_s(fgda.fgd[0].cFileName, MAX_PATH, urlname.toLocal8Bit().constData());
	mime->setData(QLatin1String("FileGroupDescriptor"),
				  QByteArray(reinterpret_cast< const char * >(&fgda), sizeof(fgda)));

	FILEGROUPDESCRIPTORW fgdw;
	ZeroMemory(&fgdw, sizeof(fgdw));
	fgdw.cItems              = 1;
	fgdw.fgd[0].dwFlags      = FD_LINKUI | FD_FILESIZE;
	fgdw.fgd[0].nFileSizeLow = contents.length();
	wcscpy_s(fgdw.fgd[0].cFileName, MAX_PATH, urlname.toStdWString().c_str());
	mime->setData(QLatin1String("FileGroupDescriptorW"),
				  QByteArray(reinterpret_cast< const char * >(&fgdw), sizeof(fgdw)));

	mime->setData(QString::fromWCharArray(CFSTR_FILECONTENTS), contents.toLocal8Bit());

	DWORD context[4];
	context[0] = 0;
	context[1] = 1;
	context[2] = 0;
	context[3] = 0;
	mime->setData(QLatin1String("DragContext"),
				  QByteArray(reinterpret_cast< const char * >(&context[0]), sizeof(context)));

	DWORD dropaction;
	dropaction = DROPEFFECT_LINK;
	mime->setData(QString::fromWCharArray(CFSTR_PREFERREDDROPEFFECT),
				  QByteArray(reinterpret_cast< const char * >(&dropaction), sizeof(dropaction)));
#endif
	QList< QUrl > urls;
	urls << url;
	mime->setUrls(urls);

	mime->setText(qs);
	mime->setHtml(QString::fromLatin1("<a href=\"%1\">%2</a>").arg(qs).arg(name.toHtmlEscaped()));

	return mime;
}

bool ServerItem::operator<(const QTreeWidgetItem &o) const {
	const ServerItem &other = static_cast< const ServerItem & >(o);
	const QTreeWidget *w    = treeWidget();

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

		QRegularExpression re(QLatin1String("[^0-9a-z]"));
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
	if (!qmIcons.contains(name))
		qmIcons.insert(name, QIcon(name));
	return qmIcons.value(name);
}

ConnectDialogEdit::ConnectDialogEdit(QWidget *p, const QString &name, const QString &host, const QString &user,
									 unsigned short port, const QString &password)
	: QDialog(p) {
	setupUi(this);
	init();

	bCustomLabel = !name.simplified().isEmpty();

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
		if (Global::get().sh && Global::get().sh->isRunning()) {
			QString host, name, user, pw;
			unsigned short port = DEFAULT_MUMBLE_PORT;

			Global::get().sh->getConnectionInfo(host, port, user, pw);
			Channel *c = Channel::get(Channel::ROOT_ID);
			if (c && c->qsName != QLatin1String("Root")) {
				name = c->qsName;
			}

			showNotice(tr("You are currently connected to a server.\nDo you want to fill the dialog with the "
						  "connection data of this server?\nHost: %1 Port: %2")
						   .arg(host)
						   .arg(port));
			m_si = new ServerItem(name, host, port, user, pw);
		}
	}
	qleUsername->setText(Global::get().s.qsUsername);
}

void ConnectDialogEdit::init() {
	m_si         = nullptr;
	usPort       = 0;
	bOk          = true;
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
	QLabel *label = qwInlineNotice->findChild< QLabel * >(QLatin1String("qlPasteNotice"));
	Q_ASSERT(label);
	label->setText(text);
	qwInlineNotice->show();
	adjustSize();
}

bool ConnectDialogEdit::updateFromClipboard() {
	delete m_si;
	m_si = ServerItem::fromMimeData(QApplication::clipboard()->mimeData(), false, nullptr, true);
	if (m_si) {
		showNotice(
			tr("You have an URL in your clipboard.\nDo you want to fill the dialog with this data?\nHost: %1 Port: %2")
				.arg(m_si->qsHostname)
				.arg(m_si->usPort));
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
	m_si = nullptr;
}

void ConnectDialogEdit::on_qbDiscard_clicked() {
	qwInlineNotice->hide();
	adjustSize();
}

void ConnectDialogEdit::on_qleName_textEdited(const QString &name) {
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

void ConnectDialogEdit::on_qleServer_textEdited(const QString &server) {
	// If using automatic label, update it
	if (!bCustomLabel) {
		qleName->setText(server);
	}
}

void ConnectDialogEdit::validate() {
	qsName     = qleName->text().simplified();
	qsHostname = qleServer->text().simplified();
	usPort     = qlePort->text().toUShort();
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

	bOk = !qsHostname.isEmpty() && !qsUsername.isEmpty() && usPort;
	qdbbButtonBox->button(QDialogButtonBox::Ok)->setEnabled(bOk);
}

void ConnectDialogEdit::accept() {
	validate();
	if (bOk) {
		QString server = qleServer->text().simplified();

		// If the user accidentally added a schema or path part, drop it now.
		// We can't do so during editing as that is quite jarring.
		const auto schemaPos = server.indexOf(QLatin1String("://"));
		if (schemaPos != -1) {
			server.remove(0, schemaPos + 3);
		}

		const auto pathPos = server.indexOf(QLatin1Char('/'));
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

	siAutoConnect = nullptr;

	bAllowPing       = Global::get().s.ptProxyType == Settings::NoProxy;
	bAllowHostLookup = Global::get().s.ptProxyType == Settings::NoProxy;
	bAllowZeroconf   = Global::get().s.ptProxyType == Settings::NoProxy;
	bAllowFilters    = Global::get().s.ptProxyType == Settings::NoProxy;

	if (tPublicServers.elapsed() >= 60 * 24 * 1000000ULL) {
		qlPublicServers.clear();
	}

	qdbbButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	qdbbButtonBox->button(QDialogButtonBox::Ok)->setText(tr("C&onnect"));
	qdbbButtonBox->button(QDialogButtonBox::Ok)->setFocusPolicy(Qt::TabFocus);

	qdbbButtonBox->button(QDialogButtonBox::Cancel)->setFocusPolicy(Qt::TabFocus);
	qdbbButtonBox->button(QDialogButtonBox::Cancel)->setAutoDefault(false);

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

	qpbAdd->setHidden(Global::get().s.disableConnectDialogEditing);
	qpbEdit->setHidden(Global::get().s.disableConnectDialogEditing);

	qtwServers->setItemDelegate(new ServerViewDelegate());

	if (!Global::get().s.bDisablePublicList) {
		const QIcon qiFlag = ServerItem::loadIcon(QLatin1String("skin:categories/applications-internet.svg"));
		// Add continents and 'Unknown' to the location combobox
		qcbSearchLocation->addItem(qiFlag, tr("All"), QLatin1String("all"));
		qcbSearchLocation->addItem(qiFlag, tr("Africa"), QLatin1String("af"));
		qcbSearchLocation->addItem(qiFlag, tr("Asia"), QLatin1String("as"));
		qcbSearchLocation->addItem(qiFlag, tr("Europe"), QLatin1String("eu"));
		qcbSearchLocation->addItem(qiFlag, tr("North America"), QLatin1String("na"));
		qcbSearchLocation->addItem(qiFlag, tr("Oceania"), QLatin1String("oc"));
		qcbSearchLocation->addItem(qiFlag, tr("South America"), QLatin1String("sa"));
		qcbSearchLocation->addItem(qiFlag, tr("Unknown"), QLatin1String(""));
		addCountriesToSearchLocation();
	}
	qgbSearch->setVisible(false);

	// Hide ping and user count if we are not allowed to ping.
	if (!bAllowPing) {
		qtwServers->setColumnCount(1);
	}

	qtwServers->sortItems(1, Qt::AscendingOrder);

	qtwServers->header()->setSectionResizeMode(0, QHeaderView::Stretch);
	if (qtwServers->columnCount() >= 2) {
		qtwServers->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	}
	if (qtwServers->columnCount() >= 3) {
		qtwServers->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	}

	connect(qtwServers->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this,
			SLOT(OnSortChanged(int, Qt::SortOrder)));

	if (bAllowFilters) {
		switch (Global::get().s.ssFilter) {
			case Settings::ShowPopulated:
				qcbFilter->setCurrentText(tr("Show Populated"));
				break;
			case Settings::ShowAll:
				qcbFilter->setCurrentText(tr("Show All"));
				break;
			default:
				qcbFilter->setCurrentText(tr("Show Reachable"));
				break;
		}
	} else {
		qcbFilter->setEnabled(false);
	}

	qmPopup = new QMenu(this);

	QList< QTreeWidgetItem * > ql;
	QList< FavoriteServer > favorites = Global::get().db->getFavorites();

	foreach (const FavoriteServer &fs, favorites) {
		ServerItem *si = new ServerItem(fs);
		qlItems << si;
		startDns(si);
		qtwServers->siFavorite->addServerItem(si);
	}
#ifdef USE_ZEROCONF
	if (bAllowZeroconf && Global::get().zeroconf && Global::get().zeroconf->isOk()) {
		connect(Global::get().zeroconf, &Zeroconf::recordsChanged, this, &ConnectDialog::onUpdateLanList);
		connect(Global::get().zeroconf, &Zeroconf::recordResolved, this, &ConnectDialog::onResolved);
		connect(Global::get().zeroconf, &Zeroconf::resolveError, this, &ConnectDialog::onLanResolveError);
		onUpdateLanList(Global::get().zeroconf->currentRecords());

		Global::get().zeroconf->startBrowser(QLatin1String("_mumble._tcp"));
	}
#endif
	qtPingTick = new QTimer(this);
	connect(qtPingTick, SIGNAL(timeout()), this, SLOT(timeTick()));

	qusSocket4 = new QUdpSocket(this);
	qusSocket6 = new QUdpSocket(this);
	bIPv4      = qusSocket4->bind(QHostAddress(QHostAddress::Any), 0);
	bIPv6      = qusSocket6->bind(QHostAddress(QHostAddress::AnyIPv6), 0);
	connect(qusSocket4, SIGNAL(readyRead()), this, SLOT(udpReply()));
	connect(qusSocket6, SIGNAL(readyRead()), this, SLOT(udpReply()));

	if (qtwServers->siFavorite->isHidden() && (!qtwServers->siLAN || qtwServers->siLAN->isHidden())
		&& qtwServers->siPublic) {
		qtwServers->siPublic->setExpanded(true);
	}

	iPingIndex = -1;
	qtPingTick->start(50);

	new QShortcut(QKeySequence(QKeySequence::Copy), this, SLOT(on_qaFavoriteCopy_triggered()));
	new QShortcut(QKeySequence(QKeySequence::Paste), this, SLOT(on_qaFavoritePaste_triggered()));

	qtwServers->setCurrentItem(nullptr);
	bLastFound = false;

	qmPingCache = Global::get().db->getPingCache();

	if (!Global::get().s.qbaConnectDialogGeometry.isEmpty())
		restoreGeometry(Global::get().s.qbaConnectDialogGeometry);
	if (!Global::get().s.qbaConnectDialogHeader.isEmpty())
		qtwServers->header()->restoreState(Global::get().s.qbaConnectDialogHeader);

	setTabOrder(qtwServers, qleSearchServername);
	setTabOrder(qleSearchServername, qcbSearchLocation);
	setTabOrder(qcbSearchLocation, qcbFilter);
	setTabOrder(qcbFilter, qpbAdd);
	setTabOrder(qpbAdd, qpbEdit);
	setTabOrder(qpbEdit, qdbbButtonBox->button(QDialogButtonBox::Ok));
	setTabOrder(qdbbButtonBox->button(QDialogButtonBox::Ok), qtwServers);
}

ConnectDialog::~ConnectDialog() {
#ifdef USE_ZEROCONF
	if (bAllowZeroconf && Global::get().zeroconf && Global::get().zeroconf->isOk()) {
		Global::get().zeroconf->stopBrowser();
		Global::get().zeroconf->cleanupResolvers();
	}
#endif
	ServerItem::qmIcons.clear();

	QList< FavoriteServer > ql;
	qmPingCache.clear();

	foreach (ServerItem *si, qlItems) {
		if (si->uiPing)
			qmPingCache.insert(UnresolvedServerAddress(si->qsHostname, si->usPort), si->uiPing);

		if (si->itType != ServerItem::FavoriteType)
			continue;
		ql << si->toFavoriteServer();
	}
	Global::get().db->setFavorites(ql);
	Global::get().db->setPingCache(qmPingCache);

	Global::get().s.qbaConnectDialogHeader   = qtwServers->header()->saveState();
	Global::get().s.qbaConnectDialogGeometry = saveGeometry();
}

void ConnectDialog::accept() {
	ServerItem *si = static_cast< ServerItem * >(qtwServers->currentItem());
	if (!si || (bAllowHostLookup && si->qlAddresses.isEmpty()) || si->qsHostname.isEmpty()) {
		qWarning() << "Invalid server";
		return;
	}

	qsPassword = si->qsPassword;
	qsServer   = si->qsHostname;
	usPort     = si->usPort;

	if (si->qsUsername.isEmpty()) {
		bool ok;
		QString defUserName = QInputDialog::getText(this, tr("Connecting to %1").arg(si->qsName), tr("Enter username"),
													QLineEdit::Normal, Global::get().s.qsUsername, &ok)
								  .trimmed();
		if (!ok)
			return;
		Global::get().s.qsUsername = si->qsUsername = defUserName;
	}

	qsUsername = si->qsUsername;

	Global::get().s.qsLastServer = si->qsName;

	QDialog::accept();
}

void ConnectDialog::OnSortChanged(int logicalIndex, Qt::SortOrder) {
	if (logicalIndex != 2) {
		return;
	}

	foreach (ServerItem *si, qlItems) {
		if (si->uiPing && (si->uiPing != si->uiPingSort)) {
			si->uiPingSort = si->uiPing;
			si->setDatas();
		}
	}
}

void ConnectDialog::on_qaFavoriteAdd_triggered() {
	ServerItem *si = static_cast< ServerItem * >(qtwServers->currentItem());
	if (!si || (si->itType == ServerItem::FavoriteType))
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
		qdbbButtonBox->button(QDialogButtonBox::Ok)->setFocus();
	}
	delete cde;
}

void ConnectDialog::on_qaFavoriteEdit_triggered() {
	ServerItem *si = static_cast< ServerItem * >(qtwServers->currentItem());
	if (!si || (si->itType != ServerItem::FavoriteType))
		return;

	QString host;
#ifdef USE_ZEROCONF
	if (!si->zeroconfHost.isEmpty())
		host = QLatin1Char('@') + si->zeroconfHost;
	else
		host = si->qsHostname;
#else
	host          = si->qsHostname;
#endif
	ConnectDialogEdit *cde = new ConnectDialogEdit(this, si->qsName, host, si->qsUsername, si->usPort, si->qsPassword);

	if (cde->exec() == QDialog::Accepted) {
		si->qsName     = cde->qsName;
		si->qsUsername = cde->qsUsername;
		si->qsPassword = cde->qsPassword;
		if ((cde->qsHostname != host) || (cde->usPort != si->usPort)) {
			stopDns(si);

			si->qlAddresses.clear();
			si->reset();

			si->usPort = cde->usPort;
#ifdef USE_ZEROCONF
			if (cde->qsHostname.startsWith(QLatin1Char('@'))) {
				si->qsHostname   = QString();
				si->zeroconfHost = cde->qsHostname.mid(1);
				si->zeroconfRecord =
					BonjourRecord(si->zeroconfHost, QLatin1String("_mumble._tcp."), QLatin1String("local."));
			} else {
				si->qsHostname     = cde->qsHostname;
				si->zeroconfHost   = QString();
				si->zeroconfRecord = BonjourRecord();
			}
#else
            si->qsHostname = cde->qsHostname;
#endif
			startDns(si);
		}
		si->setDatas();
	}
	delete cde;
}

void ConnectDialog::on_qaFavoriteRemove_triggered() {
	ServerItem *si = static_cast< ServerItem * >(qtwServers->currentItem());
	if (!si || (si->itType != ServerItem::FavoriteType))
		return;

	stopDns(si);
	qlItems.removeAll(si);
	delete si;
}

void ConnectDialog::on_qaFavoriteCopy_triggered() {
	ServerItem *si = static_cast< ServerItem * >(qtwServers->currentItem());
	if (!si)
		return;

	QApplication::clipboard()->setMimeData(si->toMimeData());
}

void ConnectDialog::on_qaFavoritePaste_triggered() {
	ServerItem *si = ServerItem::fromMimeData(QApplication::clipboard()->mimeData());
	if (!si)
		return;

	qlItems << si;
	qtwServers->siFavorite->addServerItem(si);
	qtwServers->setCurrentItem(si);
	startDns(si);
}

void ConnectDialog::on_qaUrl_triggered() {
	auto *si = static_cast< const ServerItem * >(qtwServers->currentItem());
	if (!si || si->qsUrl.isEmpty()) {
		return;
	}

	const QStringList allowedSchemes = { QLatin1String("http"), QLatin1String("https") };

	const auto url = QUrl(si->qsUrl);
	if (allowedSchemes.contains(url.scheme())) {
		QDesktopServices::openUrl(url);
	} else {
		// Inform user that the requested URL has been blocked
		QMessageBox msgBox;
		msgBox.setText(QObject::tr("<b>Blocked URL scheme \"%1\"</b>").arg(url.scheme()));
		msgBox.setInformativeText(QObject::tr("The URL uses a scheme that has been blocked for security reasons."));
		msgBox.setDetailedText(QObject::tr("Blocked URL: \"%1\"").arg(url.toString()));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
	}
}

void ConnectDialog::on_qtwServers_customContextMenuRequested(const QPoint &mpos) {
	ServerItem *si = static_cast< ServerItem * >(qtwServers->itemAt(mpos));
	qmPopup->clear();

	if (si && si->bParent) {
		si = nullptr;
	}

	if (si) {
		if (!Global::get().s.disableConnectDialogEditing) {
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

	qmPopup->popup(qtwServers->viewport()->mapToGlobal(mpos), nullptr);
}

void ConnectDialog::on_qtwServers_itemDoubleClicked(QTreeWidgetItem *item, int) {
	qtwServers->setCurrentItem(item);
	accept();
}

void ConnectDialog::on_qtwServers_currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *) {
	ServerItem *si = static_cast< ServerItem * >(item);

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
	if (qtwServers->siPublic && item == qtwServers->siPublic) {
		if (!Global::get().s.bPingServersDialogViewed) {
			// Ask the user for consent to ping the servers. If the user does
			// not give consent, disable the public server list and return.
			int result = QMessageBox::question(
				this, tr("Consent to the transmission of private data"),
				tr("<p>To measure the latency (ping) of public servers and determine the number of active users, "
				   "your IP address must be transmitted to each public server.</p>"
				   "<p>Do you consent to the transmission of your IP address? If you answer no, the public server "
				   "list will be deactivated. However, you can reactivate it at any time in the network settings.</p>"),
				QMessageBox::Yes | QMessageBox::No);
			Global::get().s.bPingServersDialogViewed = true;
			if (result == QMessageBox::No) {
				Global::get().s.bDisablePublicList = true;
				item->setExpanded(false);
				item->setHidden(true);
				return;
			}
		}
		qgbSearch->setVisible(true);
		initList();
		fillList();
	}

	ServerItem *p = static_cast< ServerItem * >(item);

	foreach (ServerItem *si, p->qlChildren) { startDns(si); }
}

void ConnectDialog::on_qtwServers_itemCollapsed(QTreeWidgetItem *item) {
	if (qtwServers->siPublic && item == qtwServers->siPublic) {
		qgbSearch->setVisible(false);
	}
}

void ConnectDialog::initList() {
	if (bPublicInit || (qlPublicServers.count() > 0))
		return;

	bPublicInit = true;

	QUrl url;
	url.setPath(QLatin1String("/v1/list"));

	QUrlQuery query;
	query.addQueryItem(QLatin1String("version"), Version::getRelease());
	url.setQuery(query);

	WebFetch::fetch(QLatin1String("publist"), url, this, SLOT(fetched(QByteArray, QUrl, QMap< QString, QString >)));
}

#ifdef USE_ZEROCONF
void ConnectDialog::onResolved(const BonjourRecord record, const QString host, const uint16_t port) {
	qlBonjourActive.removeAll(record);
	foreach (ServerItem *si, qlItems) {
		if (si->zeroconfRecord == record) {
			unsigned short usport = static_cast< unsigned short >(port);
			if ((host != si->qsHostname) || (usport != si->usPort)) {
				stopDns(si);
				si->usPort     = static_cast< unsigned short >(port);
				si->qsHostname = host;
				startDns(si);
			}
		}
	}
}

void ConnectDialog::onUpdateLanList(const QList< BonjourRecord > &list) {
	QSet< ServerItem * > items;
	QSet< ServerItem * > old =
		QSet< ServerItem * >(qtwServers->siLAN->qlChildren.begin(), qtwServers->siLAN->qlChildren.end());

	foreach (const BonjourRecord &record, list) {
		bool found = false;
		foreach (ServerItem *si, old) {
			if (si->zeroconfRecord == record) {
				items.insert(si);
				found = true;
				break;
			}
		}
		if (!found) {
			ServerItem *si = new ServerItem(record);
			qlItems << si;
			Global::get().zeroconf->startResolver(record);
			startDns(si);
			qtwServers->siLAN->addServerItem(si);
		}
	}
	QSet< ServerItem * > remove = old.subtract(items);
	foreach (ServerItem *si, remove) {
		stopDns(si);
		qlItems.removeAll(si);
		delete si;
	}
}

void ConnectDialog::onLanResolveError(const BonjourRecord record) {
	qlBonjourActive.removeAll(record);
}
#endif

void ConnectDialog::fillList() {
	QList< QTreeWidgetItem * > ql;
	QList< QTreeWidgetItem * > qlNew;

	foreach (const PublicInfo &pi, qlPublicServers) {
		bool found = false;
		foreach (ServerItem *si, qlItems) {
			if ((pi.qsIp == si->qsHostname) && (pi.usPort == si->usPort)) {
				si->qsCountry       = pi.qsCountry;
				si->qsCountryCode   = pi.qsCountryCode;
				si->qsContinentCode = pi.qsContinentCode;
				si->qsUrl           = pi.quUrl.toString();
				si->bCA             = pi.bCA;
				si->setDatas();

				if (si->itType == ServerItem::PublicType)
					found = true;
			}
		}
		if (!found)
			ql << new ServerItem(pi);
	}

	while (!ql.isEmpty()) {
		ServerItem *si = static_cast< ServerItem * >(ql.takeAt(QRandomGenerator::global()->bounded(0, ql.count())));
		qlNew << si;
		qlItems << si;
	}

	foreach (QTreeWidgetItem *qtwi, qlNew) {
		ServerItem *si = static_cast< ServerItem * >(qtwi);
		qtwServers->siPublic->addServerItem(si);
		filterServer(si);
		startDns(si);
	}
}

void ConnectDialog::timeTick() {
	if (!bLastFound && !Global::get().s.qsLastServer.isEmpty()) {
		QList< QTreeWidgetItem * > items =
			qtwServers->findItems(Global::get().s.qsLastServer, Qt::MatchExactly | Qt::MatchRecursive);
		if (!items.isEmpty()) {
			bLastFound = true;
			qtwServers->setCurrentItem(items.at(0));
			if (Global::get().s.bAutoConnect && bAutoConnect) {
				siAutoConnect = static_cast< ServerItem * >(items.at(0));
				if (!siAutoConnect->qlAddresses.isEmpty()) {
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
		foreach (const UnresolvedServerAddress &unresolved, qlDNSLookup) {
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

	ServerItem *current = static_cast< ServerItem * >(qtwServers->currentItem());
	ServerItem *hover =
		static_cast< ServerItem * >(qtwServers->itemAt(qtwServers->viewport()->mapFromGlobal(QCursor::pos())));

	ServerItem *si = nullptr;

	if (tCurrent.elapsed() >= 1000000ULL)
		si = current;
	if (!si && (tHover.elapsed() >= 1000000ULL))
		si = hover;

	if (si) {
		QString hostname    = si->qsHostname.toLower();
		unsigned short port = si->usPort;
		UnresolvedServerAddress unresolved(hostname, port);

		if (si->qlAddresses.isEmpty()) {
			if (!hostname.isEmpty()) {
				qlDNSLookup.removeAll(unresolved);
				qlDNSLookup.prepend(unresolved);
			}
			si = nullptr;
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
			expanded      = true;
			while (p && expanded) {
				expanded = expanded && p->isExpanded();
				p        = p->siParent;
			}
		} while (si->qlAddresses.isEmpty() || !expanded);
	}

	if (si == current)
		tCurrent.restart();
	if (si == hover)
		tHover.restart();

	for (const ServerAddress &addr : si->qlAddresses) {
		sendPing(addr.host.toAddress(), addr.port, si->m_version);
	}
}

void ConnectDialog::filterPublicServerList() const {
	if (!Global::get().s.bDisablePublicList) {
		foreach (ServerItem *const si, qtwServers->siPublic->qlChildren) { filterServer(si); }
	}
}

void ConnectDialog::filterServer(ServerItem *const si) const {
	if (!si->qsName.contains(qsSearchServername, Qt::CaseInsensitive)) {
		si->setHidden(true);
		return;
	}
	if (qsSearchLocation != QLatin1String("all")) {
		if (qsSearchLocation != si->qsCountry && qsSearchLocation != si->qsContinentCode) {
			si->setHidden(true);
			return;
		}
	}
	if (Global::get().s.ssFilter == Settings::ShowReachable && si->dPing == 0.0) {
		si->setHidden(true);
		return;
	} else if (Global::get().s.ssFilter == Settings::ShowPopulated && si->uiUsers == 0) {
		si->setHidden(true);
		return;
	}
	si->setHidden(false);
}

void ConnectDialog::addCountriesToSearchLocation() const {
	QMap< QString, QString > qmCountries;

	foreach (const PublicInfo &pi, qlPublicServers) {
		if (pi.qsCountry != tr("Unknown") && !qmCountries.contains(pi.qsCountry)) {
			qmCountries.insert(pi.qsCountry, pi.qsCountryCode);
		}
	}

	foreach (auto location, qmCountries.keys()) {
		// Set Icon, Text and Data
		qcbSearchLocation->addItem(
			ServerItem::loadIcon(QString::fromLatin1(":/flags/%1.svg").arg(qmCountries.value(location))), location,
			location);
	}
}

void ConnectDialog::startDns(ServerItem *si) {
	if (!bAllowHostLookup) {
		return;
	}

	QString hostname    = si->qsHostname.toLower();
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
		qdbbButtonBox->button(QDialogButtonBox::Ok)->setEnabled(!si->qlAddresses.isEmpty());

	if (!si->qlAddresses.isEmpty()) {
		foreach (const ServerAddress &addr, si->qlAddresses) { qhPings[addr].insert(si); }
		return;
	}
#ifdef USE_ZEROCONF
	if (bAllowZeroconf && si->qsHostname.isEmpty() && !si->zeroconfRecord.serviceName.isEmpty()) {
		if (!qlBonjourActive.contains(si->zeroconfRecord)) {
			Global::get().zeroconf->startResolver(si->zeroconfRecord);
			qlBonjourActive.append(si->zeroconfRecord);
		}
		return;
	}
#endif
	if (!qhDNSWait.contains(unresolved)) {
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

	foreach (const ServerAddress &addr, si->qlAddresses) {
		if (qhPings.contains(addr)) {
			qhPings[addr].remove(si);
			if (qhPings[addr].isEmpty()) {
				qhPings.remove(addr);
				qhPingRand.remove(addr);
			}
		}
	}

	QString hostname    = si->qsHostname.toLower();
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
	ServerResolver *sr = qobject_cast< ServerResolver * >(QObject::sender());
	sr->deleteLater();

	QString hostname    = sr->hostname().toLower();
	unsigned short port = sr->port();
	UnresolvedServerAddress unresolved(hostname, port);

	qsDNSActive.remove(unresolved);

	// An error occurred, or no records were found.
	if (sr->records().size() == 0) {
		return;
	}

	QSet< ServerAddress > qs;
	foreach (ServerResolverRecord record, sr->records()) {
		foreach (const HostAddress &ha, record.addresses()) { qs.insert(ServerAddress(ha, record.port())); }
	}

	QSet< ServerItem * > waiting = qhDNSWait[unresolved];
	foreach (ServerItem *si, waiting) {
		foreach (const ServerAddress &addr, qs) { qhPings[addr].insert(si); }

		si->qlAddresses = qs.values();
	}

	qlDNSLookup.removeAll(unresolved);
	qhDNSCache.insert(unresolved, qs.values());
	qhDNSWait.remove(unresolved);

	foreach (ServerItem *si, waiting) {
		if (si == qtwServers->currentItem()) {
			on_qtwServers_currentItemChanged(si, si);
			if (si == siAutoConnect)
				accept();
		}
	}

	if (bAllowPing) {
		for (const ServerAddress &addr : qs) {
			sendPing(addr.host.toAddress(), addr.port, Version::UNKNOWN);
		}
	}
}

void ConnectDialog::sendPing(const QHostAddress &host, unsigned short port, Version::full_t protocolVersion) {
	ServerAddress addr(HostAddress(host), port);

	quint64 uiRand;
	if (qhPingRand.contains(addr)) {
		uiRand = qhPingRand.value(addr);
	} else {
		uiRand = QRandomGenerator::global()->generate64() << 32;
		qhPingRand.insert(addr, uiRand);
	}

	Mumble::Protocol::PingData pingData;
	// "Encrypt" the timestamp so that server's can't spoof the returned timestamp (easily) to fake a better ping
	pingData.timestamp                    = tPing.elapsed() ^ uiRand;
	pingData.requestAdditionalInformation = true;

	if (!writePing(host, port, protocolVersion, pingData)) {
		return;
	}
	if (protocolVersion == Version::UNKNOWN) {
		// Also attempt to use new ping format in case we are pinging a server that only knows the new format
		writePing(host, port, Mumble::Protocol::PROTOBUF_INTRODUCTION_VERSION, pingData);
	}


	const QSet< ServerItem * > &qs = qhPings.value(addr);

	foreach (ServerItem *si, qs)
		++si->uiSent;
}

bool ConnectDialog::writePing(const QHostAddress &host, unsigned short port, Version::full_t protocolVersion,
							  const Mumble::Protocol::PingData &pingData) {
	m_udpPingEncoder.setProtocolVersion(protocolVersion);

	gsl::span< const Mumble::Protocol::byte > encodedPacket = m_udpPingEncoder.encodePingPacket(pingData);

	if (bIPv4 && host.protocol() == QAbstractSocket::IPv4Protocol) {
		qusSocket4->writeDatagram(reinterpret_cast< const char * >(encodedPacket.data()),
								  static_cast< qint64 >(encodedPacket.size()), host, port);
	} else if (bIPv6 && host.protocol() == QAbstractSocket::IPv6Protocol) {
		qusSocket6->writeDatagram(reinterpret_cast< const char * >(encodedPacket.data()),
								  static_cast< qint64 >(encodedPacket.size()), host, port);
	} else {
		return false;
	}

	return true;
}

void ConnectDialog::udpReply() {
	QUdpSocket *sock = qobject_cast< QUdpSocket * >(sender());

	while (sock->hasPendingDatagrams()) {
		QHostAddress host;
		unsigned short port;

		gsl::span< Mumble::Protocol::byte > buffer = m_udpDecoder.getBuffer();

		std::size_t len = static_cast< std::size_t >(sock->readDatagram(
			reinterpret_cast< char * >(buffer.data()), static_cast< int >(buffer.size()), &host, &port));

		// Pings are special in that they can be decoded in the new or the old format, if the protocol version is set to
		// the old format (which UNKNOWN does). Thus by setting the version to UNKNOWN, we effectively enable to decode
		// either format. We have to reset it to this value every time, since the call to decode may set the protocol
		// version to a more recent version (if a ping in new format is detected).
		m_udpDecoder.setProtocolVersion(Version::UNKNOWN);

		if (m_udpDecoder.decodePing(buffer.subspan(0, len))
			&& m_udpDecoder.getMessageType() == Mumble::Protocol::UDPMessageType::Ping) {
			if (host.scopeId() == QLatin1String("0"))
				host.setScopeId(QLatin1String(""));

			ServerAddress address(HostAddress(host), port);

			if (qhPings.contains(address)) {
				Mumble::Protocol::PingData pingData = m_udpDecoder.getPingData();

				quint64 elapsed = tPing.elapsed() - (pingData.timestamp ^ qhPingRand.value(address));

				for (ServerItem *si : qhPings.value(address)) {
					si->m_version    = pingData.serverVersion;
					quint32 users    = pingData.userCount;
					quint32 maxusers = pingData.maxUserCount;
					si->uiBandwidth  = pingData.maxBandwidthPerUser;

					if (!si->uiPingSort)
						si->uiPingSort = qmPingCache.value(UnresolvedServerAddress(si->qsHostname, si->usPort));

					si->setDatas(static_cast< double >(elapsed), users, maxusers);
					if (si->itType == ServerItem::PublicType) {
						filterServer(si);
					}
				}
			}
		}
	}
}

void ConnectDialog::fetched(QByteArray xmlData, QUrl, QMap< QString, QString > headers) {
	if (xmlData.isNull()) {
		QMessageBox::warning(this, QLatin1String("Mumble"), tr("Failed to fetch server list"), QMessageBox::Ok);
		return;
	}

	QDomDocument doc;
	doc.setContent(xmlData);

	qlPublicServers.clear();
	qsUserCountry       = headers.value(QLatin1String("Geo-Country"));
	qsUserCountryCode   = headers.value(QLatin1String("Geo-Country-Code")).toLower();
	qsUserContinentCode = headers.value(QLatin1String("Geo-Continent-Code")).toLower();

	QDomElement root = doc.documentElement();
	QDomNode n       = root.firstChild();
	while (!n.isNull()) {
		QDomElement e = n.toElement();
		if (!e.isNull()) {
			if (e.tagName() == QLatin1String("server")) {
				PublicInfo pi;
				pi.qsName          = e.attribute(QLatin1String("name"));
				pi.quUrl           = e.attribute(QLatin1String("url"));
				pi.qsIp            = e.attribute(QLatin1String("ip"));
				pi.usPort          = e.attribute(QLatin1String("port")).toUShort();
				pi.qsCountry       = e.attribute(QLatin1String("country"), tr("Unknown"));
				pi.qsCountryCode   = e.attribute(QLatin1String("country_code")).toLower();
				pi.qsContinentCode = e.attribute(QLatin1String("continent_code")).toLower();
				pi.bCA             = e.attribute(QLatin1String("ca")).toInt() ? true : false;

				qlPublicServers << pi;
			}
		}
		n = n.nextSibling();
	}
	addCountriesToSearchLocation();
	tPublicServers.restart();

	fillList();
}

void ConnectDialog::on_qleSearchServername_textChanged(const QString &searchServername) {
	qsSearchServername = searchServername;
	filterPublicServerList();
}

void ConnectDialog::on_qcbSearchLocation_currentIndexChanged(int searchLocationIndex) {
	qsSearchLocation = qcbSearchLocation->itemData(searchLocationIndex).toString();
	filterPublicServerList();
}

void ConnectDialog::on_qcbFilter_currentIndexChanged(int filterIndex) {
	const QString filter = qcbFilter->itemText(filterIndex);
	if (filter == tr("Show All")) {
		Global::get().s.ssFilter = Settings::ShowAll;
	} else if (filter == tr("Show Reachable")) {
		Global::get().s.ssFilter = Settings::ShowReachable;
	} else if (filter == tr("Show Populated")) {
		Global::get().s.ssFilter = Settings::ShowPopulated;
	}

	filterPublicServerList();
}
