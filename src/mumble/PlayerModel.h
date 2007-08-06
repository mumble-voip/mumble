/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

class Player;
class ClientPlayer;
class Channel;

class PlayerDelegate : public QItemDelegate {
		Q_OBJECT
	public:
		PlayerDelegate(QObject *parent = NULL);
		QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
		void paint(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

struct ModelItem {
	friend class PlayerModel;

	Channel *cChan;
	ClientPlayer *pPlayer;
	QList<Channel *> qlChannels;
	QList<ClientPlayer *> qlPlayers;

	static QHash <Channel *, ModelItem *> c_qhChannels;
	static QHash <ClientPlayer *, ModelItem *> c_qhPlayers;
	static bool bPlayersTop;

	ModelItem(Channel *c);
	ModelItem(ClientPlayer *p);
	~ModelItem();

	ModelItem *parent() const;
	ModelItem *child(int idx) const;

	bool validRow(int idx) const;
	ClientPlayer *playerAt(int idx) const;
	Channel *channelAt(int idx) const;
	int rowOf(Channel *c) const;
	int rowOf(ClientPlayer *p) const;
	int rowOfSelf() const;
	int rows() const;
	int insertIndex(Channel *c) const;
	int insertIndex(ClientPlayer *p) const;
	void insertChannel(Channel *c);
	void insertPlayer(ClientPlayer *p);
};

class ChannelItem;

class PlayerModel : public QAbstractItemModel {
		friend class ModelItem;
		Q_OBJECT
	protected:
		QIcon qiTalkingOn, qiTalkingAlt, qiTalkingOff;
		QIcon qiMutedSelf, qiMutedServer, qiMutedLocal;
		QIcon qiDeafenedSelf, qiDeafenedServer;
		QIcon qiAuthenticated, qiChannel, qiLinkedChannel;
		ModelItem *miRoot;
		QSet<Channel *> qsLinked;

		QModelIndex index(ClientPlayer *, int column = 0) const;
		QModelIndex index(Channel *) const;
		QModelIndex index(ChannelItem *) const;

		void hidePlayer(ClientPlayer *p);
		void showPlayer(ClientPlayer *p, Channel *c);

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
		bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex & parent);

		ClientPlayer *addPlayer(unsigned int id, const QString &name);
		ClientPlayer *getPlayer(const QModelIndex &idx) const;

		Channel *addChannel(int id, Channel *p, const QString &name);
		Channel *getChannel(const QModelIndex &idx) const;

		Channel *getSubChannel(Channel *p, int idx) const;

		void renamePlayer(ClientPlayer *p, const QString &name);
		void renameChannel(Channel *c, const QString &name);

		void movePlayer(ClientPlayer *p, Channel *c);
		void moveChannel(Channel *c, Channel *p);

		void removePlayer(ClientPlayer *p);
		void removeChannel(Channel *c);

		void linkChannels(Channel *c, QList<Channel *> links);
		void unlinkChannels(Channel *c, QList<Channel *> links);
		void unlinkAll(Channel *c);

		void removeAll();

		QVariant otherRoles(int column, int role, bool isPlayer) const;
	public slots:
		void playerTalkingChanged(bool talking);
		void playerMuteDeafChanged();
		void ensureSelfVisible();
		void recheckLinks();
		void updateOverlay() const;
};

#else
class PlayerModel;
#endif
