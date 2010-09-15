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

#ifndef _CONNECTDIALOG_H
#define _CONNECTDIALOG_H

#include "mumble_pch.hpp"
#include "Timer.h"
#include "Database.h"

#ifdef USE_BONJOUR
#include "BonjourClient.h"
#else
#include "bonjourrecord.h"
#endif

typedef QPair<QHostAddress, unsigned short> qpAddress;

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
	quint32 uiVersion;
	quint32 uiPing;
	quint32 uiPingSort;
	quint32 uiUsers;
	quint32 uiMaxUsers;
	quint32 uiBandwidth;
	quint32 uiSent;
	quint32 uiRecv;

	double dPing;

	typedef boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::count, boost::accumulators::tag::extended_p_square> > asQuantileType;
	asQuantileType *asQuantile;

	void reset();

	PingStats();
	~PingStats();
};

class ServerItem;

class ServerView : public QTreeWidget {
		Q_OBJECT
		Q_DISABLE_COPY(ServerView)
	public:
		ServerItem *siFavorite, *siLAN, *siPublic;
		QMap<QString, QString> qmContinentNames;
		QMap<QString, ServerItem *> qmCountry;
		QMap<QString, ServerItem *> qmContinent;

		ServerView(QWidget *);
		~ServerView();

		void fixupName(ServerItem *si);

		ServerItem *getParent(const QString &continent, const QString &countrycode, const QString &countryname, const QString &usercontinentcode, const QString &usercountrycode);
	protected:
		virtual QMimeData *mimeData(const QList<QTreeWidgetItem *>) const;
		virtual QStringList mimeTypes() const;
		virtual Qt::DropActions supportedDropActions() const;
		virtual bool dropMimeData(QTreeWidgetItem *, int, const QMimeData *, Qt::DropAction);
};

#include "ui_ConnectDialog.h"
#include "ui_ConnectDialogEdit.h"

class ServerItem : public QTreeWidgetItem, public PingStats {
		Q_DISABLE_COPY(ServerItem)
	protected:
		void init();
	public:
		enum ItemType { FavoriteType, LANType, PublicType };

		static QMap<QString, QIcon> qmIcons;

		bool bParent;
		ServerItem *siParent;
		QList<ServerItem *> qlChildren;

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

		QString qsBonjourHost;
		BonjourRecord brRecord;

		QList<QHostAddress> qlAddresses;

		ItemType itType;

		ServerItem(const FavoriteServer &fs);
		ServerItem(const PublicInfo &pi);
		ServerItem(const QString &name, const QString &host, unsigned short port, const QString &uname, const QString &password = QString());
		ServerItem(const BonjourRecord &br);
		ServerItem(const QString &name, ItemType itype, const QString &continent = QString(), const QString &country = QString());
		ServerItem(const ServerItem *si);
		~ServerItem();
		static ServerItem *fromMimeData(const QMimeData *mime, QWidget *p = NULL);

		void addServerItem(ServerItem *child);

		FavoriteServer toFavoriteServer() const;
		QMimeData *toMimeData() const;

		static QIcon loadIcon(const QString &name);

		void setDatas(double ping = 0.0, quint32 users = 0, quint32 maxusers = 0);
		bool operator< (const QTreeWidgetItem &) const;

		QVariant data(int column, int role) const;

		void hideCheck();

#if QT_VERSION < 0x040500
		void emitDataChanged();
	private:
		bool m_emitDataChanged;
#endif
};

class ConnectDialogEdit : public QDialog, protected Ui::ConnectDialogEdit {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ConnectDialogEdit)
	protected:
		bool bOk;
	public slots:
		void validate();
		void accept();

		void on_qcbShowPassword_toggled(bool);
	public:
		QString qsName, qsHostname, qsUsername, qsPassword;
		unsigned short usPort;
		ConnectDialogEdit(QWidget *parent, const QString &name = QString(), const QString &host = QString(), const QString &user = QString(), unsigned short port = 64738, const QString &password = QString(), bool add = false);
};

class ConnectDialog : public QDialog, public Ui::ConnectDialog {
		friend class ServerView;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ConnectDialog)
	protected:
		static QList<PublicInfo> qlPublicServers;
		static QString qsUserCountry, qsUserCountryCode, qsUserContinentCode;
		static Timer tPublicServers;

		QMenu *qmPopup, *qmFilters;
		QActionGroup *qagFilters;

		bool bPublicInit;
		bool bAutoConnect;

		Timer tPing;
		Timer tCurrent, tHover, tRestart;
		QUdpSocket *qusSocket4;
		QUdpSocket *qusSocket6;
		QTimer *qtPingTick;
		QList<ServerItem *> qlItems;

		ServerItem *siAutoConnect;

		QList<QString> qlDNSLookup;
		QSet<QString> qsDNSActive;
		QHash<QString, QSet<ServerItem *> > qhDNSWait;
		QHash<QString, QList<QHostAddress> > qhDNSCache;

		QHash<qpAddress, quint64> qhPingRand;
		QHash<qpAddress, QSet<ServerItem *> > qhPings;

		QMap<QPair<QString, unsigned short>, unsigned int> qmPingCache;

		bool bIPv4;
		bool bIPv6;
		int iPingIndex;

		bool bLastFound;

		QMap<QString, QIcon> qmIcons;

		void sendPing(const QHostAddress &, unsigned short port);

		void initList();
		void fillList();

		void startDns(ServerItem *);
		void stopDns(ServerItem *);
	public slots:
		void accept();
		void finished();

		void udpReply();
		void lookedUp(QHostInfo);
		void timeTick();

		void on_qaFavoriteAdd_triggered();
		void on_qaFavoriteAddNew_triggered();
		void on_qaFavoriteEdit_triggered();
		void on_qaFavoriteRemove_triggered();
		void on_qaFavoriteCopy_triggered();
		void on_qaFavoritePaste_triggered();
		void on_qaUrl_triggered();
		void onFiltersTriggered(QAction *);
		void on_qtwServers_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
		void on_qtwServers_itemDoubleClicked(QTreeWidgetItem *, int);
		void on_qtwServers_customContextMenuRequested(const QPoint &);
		void on_qtwServers_itemExpanded(QTreeWidgetItem *);
		void OnSortChanged(int, Qt::SortOrder);
	public:
		QString qsServer, qsUsername, qsPassword;
		unsigned short usPort;
		ConnectDialog(QWidget *parent, bool autoconnect);
		~ConnectDialog();

#ifdef USE_BONJOUR
	protected:
		QList<BonjourRecord> qlBonjourActive;
	public slots:
		void onUpdateLanList(const QList<BonjourRecord> &);
		void onLanBrowseError(DNSServiceErrorType);

		void onResolved(BonjourRecord, QString, int);
		void onLanResolveError(BonjourRecord, DNSServiceErrorType);
#endif
};

#else
class ConnectDialog;
#endif
