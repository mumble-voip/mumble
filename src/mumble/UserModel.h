/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MUMBLE_MUMBLE_USERMODEL_H_
#define MUMBLE_MUMBLE_USERMODEL_H_

#include <QtCore/QAbstractItemModel>
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtGui/QIcon>

class User;
class ClientUser;
class Channel;

struct ModelItem {
	friend class UserModel;

private:
	Q_DISABLE_COPY(ModelItem)
public:
	Channel *cChan;
	ClientUser *pUser;

	bool bCommentSeen;

	ModelItem *parent;
	QList<ModelItem *> qlChildren;
	QList<ModelItem *> qlHiddenChildren;
	int iUsers;

	static QHash <Channel *, ModelItem *> c_qhChannels;
	static QHash <ClientUser *, ModelItem *> c_qhUsers;
	static bool bUsersTop;

	ModelItem(Channel *c);
	ModelItem(ClientUser *p);
	ModelItem(ModelItem *);
	~ModelItem();

	ModelItem *child(int idx) const;

	bool validRow(int idx) const;
	ClientUser *userAt(int idx) const;
	Channel *channelAt(int idx) const;
	int rowOf(Channel *c) const;
	int rowOf(ClientUser *p) const;
	int rowOfSelf() const;
	int rows() const;
	int insertIndex(Channel *c) const;
	int insertIndex(ClientUser *p) const;
	QString hash() const;
	void wipe();
};

class UserModel : public QAbstractItemModel {
		friend struct ModelItem;
		friend class UserView;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(UserModel)
	protected:
		QIcon qiTalkingOn, qiTalkingWhisper, qiTalkingShout, qiTalkingOff;
		QIcon qiMutedSelf, qiMutedServer, qiMutedLocal, qiIgnoredLocal, qiMutedSuppressed;
		QIcon qiPrioritySpeaker;
		QIcon qiRecording;
		QIcon qiDeafenedSelf, qiDeafenedServer;
		QIcon qiAuthenticated, qiChannel, qiLinkedChannel, qiActiveChannel;
		QIcon qiFriend;
		QIcon qiComment, qiCommentSeen, qiFilter;
		ModelItem *miRoot;
		QSet<Channel *> qsLinked;
		QMap<QString, ClientUser *> qmHashes;

		bool bClicked;

		void recursiveClone(const ModelItem *old, ModelItem *item, QModelIndexList &from, QModelIndexList &to);
		ModelItem *moveItem(ModelItem *oldparent, ModelItem *newparent, ModelItem *item);

		QString stringIndex(const QModelIndex &index) const;
	public:
		UserModel(QObject *parent = 0);
		~UserModel();

		QModelIndex index(ClientUser *, int column = 0) const;
		QModelIndex index(Channel *, int column = 0) const;
		QModelIndex index(ModelItem *) const;

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

		ClientUser *addUser(unsigned int id, const QString &name);
		ClientUser *getUser(const QModelIndex &idx) const;
		ClientUser *getUser(const QString &hash) const;

		Channel *addChannel(int id, Channel *p, const QString &name);
		Channel *getChannel(const QModelIndex &idx) const;

		Channel *getSubChannel(Channel *p, int idx) const;

		void renameUser(ClientUser *p, const QString &name);
		void renameChannel(Channel *c, const QString &name);
		void repositionChannel(Channel *c, const int position);
		void setUserId(ClientUser *p, int id);
		void setHash(ClientUser *p, const QString &hash);
		void setFriendName(ClientUser *p, const QString &name);
		void setComment(ClientUser *p, const QString &comment);
		void setCommentHash(ClientUser *p, const QByteArray &hash);
		void seenComment(const QModelIndex &idx);

		void moveUser(ClientUser *p, Channel *c);
		void moveChannel(Channel *c, Channel *p);
		void setComment(Channel *c, const QString &comment);
		void setCommentHash(Channel *c, const QByteArray &hash);

		void removeUser(ClientUser *p);
		void removeChannel(Channel *c);

		void linkChannels(Channel *c, QList<Channel *> links);
		void unlinkChannels(Channel *c, QList<Channel *> links);
		void unlinkAll(Channel *c);

		void removeAll();

		void expandAll(Channel *c);
		void collapseEmpty(Channel *c);

		QVariant otherRoles(const QModelIndex &idx, int role) const;

		unsigned int uiSessionComment;
		int iChannelDescription;
	public slots:
		/// Invalidates the model data of the ClientUser triggering this slot.
		void userStateChanged();
		void ensureSelfVisible();
		void recheckLinks();
		void updateOverlay() const;
		void toggleChannelFiltered(Channel *c);
};

#endif
