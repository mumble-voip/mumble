/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _PLAYERMODEL_H
#define _PLAYERMODEL_H

#include <QAbstractItemModel>
#include <QItemDelegate>
#include <QIcon>
#include "Player.h"

class PlayerDelegate : public QItemDelegate {
	Q_OBJECT
	public:
		PlayerDelegate(QObject *parent = NULL);
		QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index) const;
		void paint(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

struct ModelItem {
	friend class PlayerModel;

	Channel *cChan;
	Player *pPlayer;
	QList<Channel *> qlChannels;
	QList<Player *> qlPlayers;

	static QHash <Channel *, ModelItem *> c_qhChannels;
	static QHash <Player *, ModelItem *> c_qhPlayers;

	ModelItem(Channel *c);
	ModelItem(Player *p);
	~ModelItem();

	ModelItem *parent() const;
	ModelItem *child(int idx) const;

	bool validRow(int idx) const;
	Player *playerAt(int idx) const;
	Channel *channelAt(int idx) const;
	int rowOf(Channel *c) const;
	int rowOf(Player *p) const;
	int rowOfSelf() const;
	int rows() const;
	int insertIndex(Channel *c) const;
	int insertIndex(Player *p) const;
	void insertChannel(Channel *c);
	void insertPlayer(Player *p);

	bool isValid() const;
};

class PlayerModel : public QAbstractItemModel {
	friend class ChannelItem;
	Q_OBJECT
protected:
	QIcon qiTalkingOn, qiTalkingOff;
	QIcon qiMutedSelf, qiMutedServer;
	QIcon qiDeafenedSelf, qiDeafenedServer;
	QIcon qiAuthenticated, qiChannel;
	ModelItem *miRoot;
	QHash <Channel *, ChannelItem *> qhChannelItems;

	QModelIndex index(Player *, int column = 0) const;
	QModelIndex index(Channel *) const;
	QModelIndex index(ChannelItem *) const;

	void hidePlayer(Player *p);
	void showPlayer(Player *p, Channel *c);

	void hideChannel(Channel *c);
	void showChannel(Channel *c, Channel *p);

	QString stringIndex(const QModelIndex &index) const;

	void unbugHide(const QModelIndex &index);
public:
	PlayerModel(QObject *parent = 0);
	~PlayerModel();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::DropActions supportedDropActions() const;
	QStringList mimeTypes() const;
	QMimeData *mimeData(const QModelIndexList &idx) const;
	bool dropMimeData ( const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex & parent);

	Player *addPlayer(short id, QString name);
	Player *getPlayer(const QModelIndex &idx) const;

	Channel *addChannel(int id, Channel *p, QString name);
	Channel *getChannel(const QModelIndex &idx) const;

	void movePlayer(Player *p, int id);
	void moveChannel(Channel *p, int id);

	void removePlayer(Player *p);
	void removeChannel(Channel *c);

	void removeAll();

	QVariant otherRoles(int column, int role, bool isPlayer) const;
public slots:
	void playerTalkingChanged(bool talking);
	void playerMuteDeafChanged();
};

#else
class PlayerModel;
#endif
