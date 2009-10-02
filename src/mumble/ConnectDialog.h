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
	unsigned short usPort;
};

struct PingStats {
	quint32 uiVersion;
	quint32 uiPing;
	quint32 uiPingSort;
	quint32 uiUsers;
	quint32 uiMaxUsers;
	quint32 uiBandwidth;
	quint32 uiSent;

	double dPing;

	typedef boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::count, boost::accumulators::tag::extended_p_square> > asQuantileType;
	asQuantileType *asQuantile;

	PingStats();
};

class ServerView : public QTreeWidget {
		Q_OBJECT;
		Q_DISABLE_COPY(ServerView);
	public:
		ServerView(QWidget *);
	protected:
		virtual QMimeData *mimeData(const QList<QTreeWidgetItem *>) const;
		virtual QStringList mimeTypes() const;
		virtual Qt::DropActions supportedDropActions() const;
		virtual bool dropMimeData(QTreeWidgetItem *, int, const QMimeData *, Qt::DropAction);
};

#include "ui_ConnectDialog.h"
#include "ui_ConnectDialogEdit.h"

class ServerItem : public QTreeWidgetItem, public PingStats {
		Q_DISABLE_COPY(ServerItem);
	protected:
		void init();
	public:
		enum ItemType { FavoriteType, LANType, PublicType };

		static QMap<QString, QIcon> qmIcons;

		QString qsName;

		QString qsHostname;
		unsigned short usPort;

		QString qsUsername;
		QString qsPassword;

		QString qsCountry;
		QString qsCountryCode;

		QString qsUrl;

		QString qsBonjourHost;
		BonjourRecord brRecord;

		QList<QHostAddress> qlAddresses;

		ItemType itType;

		ServerItem(const FavoriteServer &fs);
		ServerItem(const PublicInfo &pi);
		ServerItem(const QString &name, const QString &host, unsigned short port, const QString &uname);
		ServerItem(const BonjourRecord &br);
		static ServerItem *fromMimeData(const QMimeData *mime, QWidget *p = NULL);

		FavoriteServer toFavoriteServer() const;
		QMimeData *toMimeData() const;

		static QIcon loadIcon(const QString &name);

		void setDatas(double ping = 0.0, quint32 users = 0, quint32 maxusers = 0);
		bool operator< (const QTreeWidgetItem &) const;

		QVariant data(int column, int role) const;

		void hideCheck();
};

class ConnectDialogEdit : public QDialog, protected Ui::ConnectDialogEdit {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ConnectDialogEdit);
	protected:
		bool bOk;
	public slots:
		void validate();
		void accept();

		void on_qleServer_textChanged(const QString&);
	public:
		QString qsName, qsHostname, qsUsername;
		unsigned short usPort;
		ConnectDialogEdit(QWidget *parent, const QString &name = QString(), const QString &host = QString(), const QString &user = QString(), unsigned short port = 64738);
};

class ConnectDialog : public QDialog, public Ui::ConnectDialog {
		friend class ServerView;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ConnectDialog)
	protected:
		static QList<PublicInfo> qlPublicServers;
		static Timer tPublicServers;

		QMenu *qmPopup, *qmFilters;
		QActionGroup *qagFilters;

		bool bPublicInit;

		Timer tPing;
		Timer tCurrent, tHover, tRestart;
		QUdpSocket *qusSocket4;
		QUdpSocket *qusSocket6;
		QTimer *qtPingTick;
		QList<ServerItem *> qlItems;

		QList<QString> qlDNSLookup;
		QSet<QString> qsDNSActive;
		QHash<QString, QSet<ServerItem *> > qhDNSWait;
		QHash<QString, QList<QHostAddress> > qhDNSCache;

		QList<qpAddress> qlAddresses;
		QHash<qpAddress, quint64> qhPingRand;
		QHash<qpAddress, QSet<ServerItem *> > qhPings;

		bool bIPv4;
		bool bIPv6;
		int iPingIndex;

		bool bLastFound;

		QMap<QString, QIcon> qmIcons;

		void pingList();
		void sendPing(const QHostAddress &, unsigned short port);

		void initList();
		void fillList();

		void restartDns();
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
		void OnSortChanged(int, Qt::SortOrder);
	public:
		QString qsServer, qsUsername, qsPassword;
		unsigned short usPort;
		ConnectDialog(QWidget *parent);
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
