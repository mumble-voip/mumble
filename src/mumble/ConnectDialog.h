// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CONNECTDIALOG_H_
#define MUMBLE_MUMBLE_CONNECTDIALOG_H_

#ifndef Q_MOC_RUN
#	include <boost/accumulators/accumulators.hpp>
#	include <boost/accumulators/statistics/stats.hpp>
#endif

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QtGlobal>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QTreeWidgetItem>

#include <QtNetwork/QHostInfo>

#ifdef USE_ZEROCONF
#	include "BonjourRecord.h"
#	include <dns_sd.h>
#endif

#include "HostAddress.h"
#include "MumbleProtocol.h"
#include "Net.h"
#include "ServerAddress.h"
#include "Timer.h"
#include "UnresolvedServerAddress.h"
#include "Version.h"

struct FavoriteServer;
class QUdpSocket;

struct PublicInfo {
	QString qsName;
	QUrl quUrl;
	QString qsIp;
	QString qsCountry;
	QString qsCountryCode;
	QString qsContinentCode;
	unsigned short usPort;
	bool bCA;
};

struct PingStats {
private:
	Q_DISABLE_COPY(PingStats)
protected:
	void init();

public:
	Version::full_t m_version;
	quint32 uiPing;
	quint32 uiPingSort;
	quint32 uiUsers;
	quint32 uiMaxUsers;
	quint32 uiBandwidth;
	quint32 uiSent;
	quint32 uiRecv;

	double dPing;

	typedef boost::accumulators::accumulator_set<
		double,
		boost::accumulators::stats< boost::accumulators::tag::count, boost::accumulators::tag::extended_p_square > >
		asQuantileType;
	asQuantileType *asQuantile;

	void reset();

	PingStats();
	~PingStats();
};

class ServerItem;

class ServerViewDelegate : public QStyledItemDelegate {
	Q_OBJECT
	Q_DISABLE_COPY(ServerViewDelegate)
public:
	ServerViewDelegate(QObject *p = nullptr);
	~ServerViewDelegate();

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

class ServerView : public QTreeWidget {
	Q_OBJECT
	Q_DISABLE_COPY(ServerView)
public:
	ServerItem *siFavorite, *siLAN, *siPublic;

	ServerView(QWidget *);
	~ServerView() Q_DECL_OVERRIDE;

	void fixupName(ServerItem *si);

protected:
	QMimeData *mimeData(const QList< QTreeWidgetItem * > &) const Q_DECL_OVERRIDE;
	QStringList mimeTypes() const Q_DECL_OVERRIDE;
	Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;
	bool dropMimeData(QTreeWidgetItem *, int, const QMimeData *, Qt::DropAction) Q_DECL_OVERRIDE;
};

#include "ui_ConnectDialog.h"
#include "ui_ConnectDialogEdit.h"

class ServerItem : public QTreeWidgetItem, public PingStats {
	Q_DISABLE_COPY(ServerItem)
protected:
	void init();

public:
	enum ItemType { FavoriteType, LANType, PublicType };

	static QMap< QString, QIcon > qmIcons;

	bool bParent;
	ServerItem *siParent;
	QList< ServerItem * > qlChildren;

	QString qsName;

	QString qsHostname;
	unsigned short usPort;
	bool bCA;

	QString qsUsername;
	QString qsPassword;

	QString qsCountry;
	QString qsCountryCode;
	QString qsContinentCode;

	QString qsUrl;
#ifdef USE_ZEROCONF
	QString zeroconfHost;
	BonjourRecord zeroconfRecord;
#endif
	/// Contains the resolved addresses for
	/// this ServerItem.
	QList< ServerAddress > qlAddresses;

	ItemType itType;

	ServerItem(const FavoriteServer &fs);
	ServerItem(const PublicInfo &pi);
	ServerItem(const QString &name, const QString &host, unsigned short port, const QString &uname,
			   const QString &password = QString());
#ifdef USE_ZEROCONF
	ServerItem(const BonjourRecord &br);
#endif
	ServerItem(const QString &name, ItemType itype);
	ServerItem(const ServerItem *si);
	~ServerItem();

	/// Converts given mime data into a ServerItem object
	///
	/// This function checks the clipboard for a valid mumble:// style
	/// URL and converts it into a ServerItem ready to add to the connect
	/// dialog. It also parses .lnk files of InternetShortcut/URL type
	/// to enable those to be dropped onto the clipboard.
	///
	/// @note If needed can query the user for a user name using a modal dialog.
	/// @note If a server item is returned it's the callers responsibility to delete it.
	///
	/// @param mime Mime data to analyze
	/// @param default_name If true the hostname is set as item name if none is given
	/// @param p Parent widget to use in case the user has to be queried
	/// @return Server item or nullptr if mime data invalid.
	///
	static ServerItem *fromMimeData(const QMimeData *mime, bool default_name = true, QWidget *p = nullptr,
									bool convertHttpUrls = false);
	/// Create a ServerItem from a mumble:// URL
	static ServerItem *fromUrl(QUrl url, QWidget *p);

	void addServerItem(ServerItem *child);

	FavoriteServer toFavoriteServer() const;
	QMimeData *toMimeData() const;
	static QMimeData *toMimeData(const QString &name, const QString &host, unsigned short port,
								 const QString &channel = QString());

	static QIcon loadIcon(const QString &name);

	void setDatas(double ping = 0.0, quint32 users = 0, quint32 maxusers = 0);
	bool operator<(const QTreeWidgetItem &) const Q_DECL_OVERRIDE;

	QVariant data(int column, int role) const Q_DECL_OVERRIDE;
};

class ConnectDialogEdit : public QDialog, protected Ui::ConnectDialogEdit {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ConnectDialogEdit)

	void init();

protected:
	bool bOk;
	bool bCustomLabel;
	ServerItem *m_si;

public slots:
	void validate();
	void accept();

	void on_qbFill_clicked();
	void on_qbDiscard_clicked();
	void on_qcbShowPassword_toggled(bool);
	void on_qleName_textEdited(const QString &);
	void on_qleServer_textEdited(const QString &);
	void showNotice(const QString &text);
	bool updateFromClipboard();

public:
	QString qsName, qsHostname, qsUsername, qsPassword;
	unsigned short usPort;
	ConnectDialogEdit(QWidget *parent, const QString &name, const QString &host, const QString &user,
					  unsigned short port, const QString &password);
	/// Add a new Server
	/// Prefills from clipboard content or the connected to server if available
	ConnectDialogEdit(QWidget *parent);
	virtual ~ConnectDialogEdit();
};

class ConnectDialog : public QDialog, public Ui::ConnectDialog {
	friend class ServerView;

private:
	Q_OBJECT
	Q_DISABLE_COPY(ConnectDialog)
protected:
	static QList< PublicInfo > qlPublicServers;
	static QString qsUserCountry, qsUserCountryCode, qsUserContinentCode;
	static Timer tPublicServers;

	QMenu *qmPopup;
	QPushButton *qpbEdit;

	bool bPublicInit;
	bool bAutoConnect;

	Timer tPing;
	Timer tCurrent, tHover, tRestart;
	QUdpSocket *qusSocket4;
	QUdpSocket *qusSocket6;
	QTimer *qtPingTick;
	QList< ServerItem * > qlItems;

	ServerItem *siAutoConnect;

	QList< UnresolvedServerAddress > qlDNSLookup;
	QSet< UnresolvedServerAddress > qsDNSActive;
	QHash< UnresolvedServerAddress, QSet< ServerItem * > > qhDNSWait;
	QHash< UnresolvedServerAddress, QList< ServerAddress > > qhDNSCache;

	QHash< ServerAddress, quint64 > qhPingRand;
	QHash< ServerAddress, QSet< ServerItem * > > qhPings;

	QMap< UnresolvedServerAddress, unsigned int > qmPingCache;

	QString qsSearchServername;
	QString qsSearchLocation;

	bool bIPv4;
	bool bIPv6;
	int iPingIndex;
	Mumble::Protocol::UDPPingEncoder< Mumble::Protocol::Role::Client > m_udpPingEncoder;
	Mumble::Protocol::UDPDecoder< Mumble::Protocol::Role::Client > m_udpDecoder;

	bool bLastFound;

	/// bAllowPing determines whether ConnectDialog can use
	/// UDP packets to ping remote hosts to be able to show a
	/// ping latency and user count.
	bool bAllowPing;
	/// bAllowHostLookup determines whether ConnectDialog can
	/// resolve hosts via DNS, Bonjour, and so on.
	bool bAllowHostLookup;
	/// bAllowZeroconf determines whether ConfigDialog can use
	/// zeroconf to find nearby servers on the local network.
	bool bAllowZeroconf;
	/// bAllowFilters determines whether filters are available
	/// in the ConfigDialog. If this option is disabled, the
	/// 'Show All' filter is forced, and no other filter can
	/// be chosen.
	bool bAllowFilters;


	void sendPing(const QHostAddress &, unsigned short port, Version::full_t protocolVersion);
	bool writePing(const QHostAddress &host, unsigned short port, Version::full_t protocolVersion,
				   const Mumble::Protocol::PingData &pingData);

	void initList();
	void fillList();

	void startDns(ServerItem *);
	void stopDns(ServerItem *);

	/// Calls ConnectDialog::filterServer for each server in
	/// the public server list
	void filterPublicServerList() const;
	/// Hides the given ServerItem according to the current
	/// filter settings. It is checked that the name of the
	/// given server matches ConnectDialog#qsSearchServername
	/// and the location is equal to ConnectDialog#qsSearchLocation.
	/// Lastly it is checked that the server is reachable or
	/// populated, should the respective filters be set.
	///
	/// \param si  ServerItem that should be filtered
	void filterServer(ServerItem *const si) const;

	/// Enumerates all countries in ConnectDialog#qlPublicServers
	/// and adds an entry to the location filter Combobox with
	/// with the country name as text, the countrycode as
	/// data and the flag of the country as the icon.
	void addCountriesToSearchLocation() const;
public slots:
	void accept();
	void fetched(QByteArray xmlData, QUrl, QMap< QString, QString >);

	void udpReply();
	void lookedUp();
	void timeTick();

	void on_qaFavoriteAdd_triggered();
	void on_qaFavoriteAddNew_triggered();
	void on_qaFavoriteEdit_triggered();
	void on_qaFavoriteRemove_triggered();
	void on_qaFavoriteCopy_triggered();
	void on_qaFavoritePaste_triggered();
	void on_qaUrl_triggered();
	void on_qtwServers_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
	void on_qtwServers_itemDoubleClicked(QTreeWidgetItem *, int);
	void on_qtwServers_customContextMenuRequested(const QPoint &);
	/// If the expanded item is the public server list
	/// the user is asked (only once) for consent to transmit
	/// the IP to all public servers. If the user does not
	/// consent the public server list is disabled. If
	/// the user does consent the public server list is
	/// filled and the search dialog is shown.
	/// Finally name resolution is triggered for all child
	/// items of the expanded item.
	void on_qtwServers_itemExpanded(QTreeWidgetItem *item);
	/// Hides the search dialog if the collapsed item is
	/// the public server list
	void on_qtwServers_itemCollapsed(QTreeWidgetItem *item);
	void OnSortChanged(int, Qt::SortOrder);

public:
	QString qsServer, qsUsername, qsPassword;
	unsigned short usPort;
	ConnectDialog(QWidget *parent, bool autoconnect);
	~ConnectDialog();

#ifdef USE_ZEROCONF
protected:
	QList< BonjourRecord > qlBonjourActive;
public slots:
	void onUpdateLanList(const QList< BonjourRecord > &);

	void onResolved(const BonjourRecord, const QString, const uint16_t);
	void onLanResolveError(const BonjourRecord);
#endif
private slots:
	void on_qleSearchServername_textChanged(const QString &searchServername);
	void on_qcbSearchLocation_currentIndexChanged(int searchLocationIndex);
	void on_qcbFilter_currentIndexChanged(int filterIndex);
};

#endif
