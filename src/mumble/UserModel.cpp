/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "mumble_pch.hpp"

#include "UserModel.h"

#include "ClientUser.h"
#include "Channel.h"
#include "Database.h"
#include "Global.h"
#include "LCD.h"
#include "Log.h"
#include "MainWindow.h"
#include "Message.h"
#include "Overlay.h"
#include "ServerHandler.h"
#include "Usage.h"
#include "User.h"

QHash <Channel *, ModelItem *> ModelItem::c_qhChannels;
QHash <ClientUser *, ModelItem *> ModelItem::c_qhUsers;
bool ModelItem::bUsersTop = false;

ModelItem::ModelItem(Channel *c) {
	this->cChan = c;
	this->pUser = NULL;
	bCommentSeen = true;
	c_qhChannels.insert(c, this);
	parent = c_qhChannels.value(c->cParent);
	iUsers = 0;
}

ModelItem::ModelItem(ClientUser *p) {
	this->cChan = NULL;
	this->pUser = p;
	bCommentSeen = true;
	c_qhUsers.insert(p, this);
	parent = c_qhChannels.value(p->cChannel);
	iUsers = 0;
}

ModelItem::ModelItem(ModelItem *i) {
	// Create a shallow clone
	this->cChan = i->cChan;
	this->pUser = i->pUser;
	this->parent = i->parent;
	this->bCommentSeen = i->bCommentSeen;

	if (pUser)
		c_qhUsers.insert(pUser, this);
	else if (cChan)
		c_qhChannels.insert(cChan, this);

	iUsers = i->iUsers;
}

ModelItem::~ModelItem() {
	Q_ASSERT(qlChildren.count() == 0);

	if (cChan && c_qhChannels.value(cChan) == this)
		c_qhChannels.remove(cChan);
	if (pUser && c_qhUsers.value(pUser) == this)
		c_qhUsers.remove(pUser);
}

void ModelItem::wipe() {
	foreach(ModelItem *i, qlChildren) {
		i->wipe();
		delete i;
	}
	qlChildren.clear();
	iUsers = 0;
}

ModelItem *ModelItem::child(int idx) const {
	if (! validRow(idx))
		return NULL;

	return qlChildren.at(idx);
}

bool ModelItem::validRow(int idx) const {
	return ((idx >= 0) && (idx < qlChildren.count()));
}

ClientUser *ModelItem::userAt(int idx) const {
	if (! validRow(idx))
		return NULL;
	return qlChildren.at(idx)->pUser;
}

Channel *ModelItem::channelAt(int idx) const {
	if (! validRow(idx))
		return NULL;
	return qlChildren.at(idx)->cChan;
}

int ModelItem::rowOf(Channel *c) const {
	for (int i=0;i<qlChildren.count();i++)
		if (qlChildren.at(i)->cChan == c)
			return i;
	return -1;
}

int ModelItem::rowOf(ClientUser *p) const {
	for (int i=0;i<qlChildren.count();i++)
		if (qlChildren.at(i)->pUser == p)
			return i;
	return -1;
}

int ModelItem::rowOfSelf() const {
	// Root?
	if (! parent)
		return 0;

	if (pUser)
		return parent->rowOf(pUser);
	else
		return parent->rowOf(cChan);
}

int ModelItem::rows() const {
	return qlChildren.count();
}

int ModelItem::insertIndex(Channel *c) const {
	QList<Channel*> qlpc;
	ModelItem *item;

	int ocount = 0;

	foreach(item, qlChildren) {
		if (item->cChan) {
			if (item->cChan != c) {
				qlpc << item->cChan;
			}
		} else
			ocount++;
	}
	qlpc << c;
	qSort(qlpc.begin(), qlpc.end(), Channel::lessThan);
	return qlpc.indexOf(c) + (bUsersTop ? ocount : 0);
}

int ModelItem::insertIndex(ClientUser *p) const {
	QList<ClientUser*> qlclientuser;
	ModelItem *item;

	int ocount = 0;

	foreach(item, qlChildren) {
		if (item->pUser) {
			if (item->pUser != p)
				qlclientuser << item->pUser;
		} else
			ocount++;
	}

	qlclientuser << p;
	qSort(qlclientuser.begin(), qlclientuser.end(), ClientUser::lessThan);

	return qlclientuser.indexOf(p) + (bUsersTop ? 0 : ocount);
}

QString ModelItem::hash() const {
	if (pUser) {
		if (! pUser->qsHash.isEmpty())
			return pUser->qsHash;
		else
			return QLatin1String(sha1(pUser->qsName).toHex());
	} else {
		QCryptographicHash chash(QCryptographicHash::Sha1);

		chash.addData(cChan->qsName.toUtf8());
		chash.addData(QString::number(cChan->iId).toUtf8());
		if (g.sh && g.sh->isRunning()) {
			QString host, user, pw;
			unsigned short port;
			g.sh->getConnectionInfo(host, port, user, pw);
			chash.addData(host.toUtf8());
			chash.addData(QString::number(port).toUtf8());
		}
		return QLatin1String(chash.result().toHex());
	}
}

UserModel::UserModel(QObject *p) : QAbstractItemModel(p) {
	qiTalkingOff=QIcon(QLatin1String("skin:talking_off.svg"));
	qiTalkingOn=QIcon(QLatin1String("skin:talking_on.svg"));
	qiTalkingShout=QIcon(QLatin1String("skin:talking_alt.svg"));
	qiTalkingWhisper=QIcon(QLatin1String("skin:talking_whisper.svg"));
	qiPrioritySpeaker=QIcon(QLatin1String("skin:actions/audio-input-microphone.svg"));
	qiRecording=QIcon(QLatin1String("skin:actions/media-record.svg"));
	qiMutedSelf=QIcon(QLatin1String("skin:muted_self.svg"));
	qiMutedServer=QIcon(QLatin1String("skin:muted_server.svg"));
	qiMutedLocal=QIcon(QLatin1String("skin:muted_local.svg"));
	qiIgnoredLocal=QIcon(QLatin1String("skin:status/text-missing.svg"));
	qiMutedSuppressed=QIcon(QLatin1String("skin:muted_suppressed.svg"));
	qiDeafenedSelf=QIcon(QLatin1String("skin:deafened_self.svg"));
	qiDeafenedServer=QIcon(QLatin1String("skin:deafened_server.svg"));
	qiAuthenticated=QIcon(QLatin1String("skin:authenticated.svg"));
	qiChannel=QIcon(QLatin1String("skin:channel.svg"));
	qiActiveChannel=QIcon(QLatin1String("skin:channel_active.svg"));
	qiLinkedChannel=QIcon(QLatin1String("skin:channel_linked.svg"));
	qiFriend=QIcon(QLatin1String("skin:emblems/emblem-favorite.svg"));
	qiComment=QIcon(QLatin1String("skin:comment.svg"));
	qiCommentSeen=QIcon(QLatin1String("skin:comment_seen.svg"));
	qiFilter=QIcon(QLatin1String("skin:filter.svg"));

	ModelItem::bUsersTop = g.s.bUserTop;

	uiSessionComment = 0;
	iChannelDescription = -1;
	bClicked = false;

	miRoot = new ModelItem(Channel::get(0));
}

UserModel::~UserModel() {
	removeAll();
	Q_ASSERT(ModelItem::c_qhUsers.count() == 0);
	Q_ASSERT(ModelItem::c_qhChannels.count() == 1);
	delete miRoot;
}


int UserModel::columnCount(const QModelIndex &) const {
	return 1;
}

QModelIndex UserModel::index(int row, int column, const QModelIndex &p) const {
	ModelItem *item;
	QModelIndex idx = QModelIndex();

	if ((row < 0) || (column < 0) || (column > 1)) {
		return QModelIndex();
	}

	if (! p.isValid()) {
		return createIndex(row, column, miRoot);
	} else {
		item = static_cast<ModelItem *>(p.internalPointer());
	}

	if (! item)
		return idx;

	if (! item->validRow(row))
		return idx;

	idx = createIndex(row, column, item->child(row));

	return idx;
}

QModelIndex UserModel::index(ClientUser *p, int column) const {
	ModelItem *item = ModelItem::c_qhUsers.value(p);
	Q_ASSERT(p);
	Q_ASSERT(item);
	if (!p || ! item)
		return QModelIndex();
	QModelIndex idx=createIndex(item->rowOfSelf(), column, item);
	return idx;
}

QModelIndex UserModel::index(Channel *c, int column) const {
	ModelItem *item = ModelItem::c_qhChannels.value(c);
	Q_ASSERT(c);
	Q_ASSERT(item);
	if (!item || !c)
		return QModelIndex();
	QModelIndex idx=createIndex(item->rowOfSelf(), column, item);
	return idx;
}

QModelIndex UserModel::index(ModelItem *item) const {
	return createIndex(item->rowOfSelf(), 0, item);
}

QModelIndex UserModel::parent(const QModelIndex &idx) const {
	if (! idx.isValid())
		return QModelIndex();

	ModelItem *item = static_cast<ModelItem *>(idx.internalPointer());

	ModelItem *pitem = (item) ? item->parent : NULL;

	if (! pitem)
		return QModelIndex();

	return createIndex(pitem->rowOfSelf(), 0, pitem);
}

int UserModel::rowCount(const QModelIndex &p) const {
	ModelItem *item;

	int val = 0;

	if (!p.isValid())
		return 1;
	else
		item = static_cast<ModelItem *>(p.internalPointer());

	if (! item || (p.column() != 0))
		return 0;

	val = item->rows();

	return val;
}

QString UserModel::stringIndex(const QModelIndex &idx) const {
	ModelItem *item = static_cast<ModelItem *>(idx.internalPointer());
	if (!idx.isValid())
		return QLatin1String("invIdx");
	if (!item)
		return QLatin1String("invPtr");
	if (item->pUser)
		return QString::fromLatin1("P:%1 [%2,%3]").arg(item->pUser->qsName).arg(idx.row()).arg(idx.column());
	else
		return QString::fromLatin1("C:%1 [%2,%3]").arg(item->cChan->qsName).arg(idx.row()).arg(idx.column());
}

QVariant UserModel::data(const QModelIndex &idx, int role) const {
	if (!idx.isValid())
		return QVariant();

	ModelItem *item = static_cast<ModelItem *>(idx.internalPointer());

	Channel *c = item->cChan;
	ClientUser *p = item->pUser;

	if (!c && !p) {
		return QVariant();
	}

	QVariant v = otherRoles(idx, role);
	if (v.isValid())
		return v;

	QList<QVariant> l;

	if (p) {
		switch (role) {
			case Qt::DecorationRole:
				if (idx.column() == 0)
					switch (p->tsState) {
						case Settings::Talking:
							return qiTalkingOn;
						case Settings::Whispering:
							return qiTalkingWhisper;
						case Settings::Shouting:
							return qiTalkingShout;
						case Settings::Passive:
						default:
							return qiTalkingOff;
					}
				break;
			case Qt::FontRole:
				if ((idx.column() == 0) && (p->uiSession == g.uiSession)) {
					QFont f = g.mw->font();
					f.setBold(! f.bold());
					return f;
				}
				break;
			case Qt::DisplayRole:
				if (idx.column() == 0) {
					if (! p->qsFriendName.isEmpty() && (p->qsFriendName.toLower() != p->qsName.toLower()))
						return QString::fromLatin1("%1 (%2)").arg(p->qsName).arg(p->qsFriendName);
					else
						return p->qsName;
				}
				if (! p->qbaCommentHash.isEmpty())
					l << (item->bCommentSeen ? qiCommentSeen : qiComment);
				if (p->bPrioritySpeaker)
					l << qiPrioritySpeaker;
				if (p->bRecording)
					l << qiRecording;
				if (p->bMute)
					l << qiMutedServer;
				if (p->bSuppress)
					l << qiMutedSuppressed;
				if (p->bSelfMute)
					l << qiMutedSelf;
				if (p->bLocalMute)
					l << qiMutedLocal;
				if (p->bLocalIgnore)
					l << qiIgnoredLocal;
				if (p->bDeaf)
					l << qiDeafenedServer;
				if (p->bSelfDeaf)
					l << qiDeafenedSelf;
				if (p->iId >= 0)
					l << qiAuthenticated;
				if (! p->qsFriendName.isEmpty())
					l << qiFriend;
				return l;
			default:
				break;
		}
	} else {
		switch (role) {
			case Qt::DecorationRole:
				if (idx.column() == 0) {
					if (g.uiSession && qsLinked.contains(c)) {
						if (ClientUser::get(g.uiSession)->cChannel == c)
							return qiActiveChannel;
						else
							return qiLinkedChannel;
					}
					return qiChannel;
				}
				break;
			case Qt::DisplayRole:
				if (idx.column() == 0) {
					if (! g.s.bShowUserCount || item->iUsers == 0)
						return c->qsName;

					return QString::fromLatin1("%1 (%2)").arg(c->qsName).arg(item->iUsers);
				}
				if (! c->qbaDescHash.isEmpty())
					l << (item->bCommentSeen ? qiCommentSeen : qiComment);

				if (c->bFiltered)
					l << (qiFilter);

				return l;
			case Qt::FontRole:
				if (g.uiSession) {
					Channel *home = ClientUser::get(g.uiSession)->cChannel;

					if ((c == home) || qsLinked.contains(c)) {
						QFont f = g.mw->font();
						if (qsLinked.count() > 1)
							f.setItalic(! f.italic());
						if (c == home)
							f.setBold(! f.bold());
						return f;
					}
				}
				break;
			case Qt::BackgroundRole:
				if ((c->iId == 0) && g.sh && g.sh->isStrong()) {
					QColor qc(Qt::green);
					qc.setAlpha(32);
					return qc;
				}
				break;
			default:
				break;
		}
	}
	return QVariant();
}

Qt::ItemFlags UserModel::flags(const QModelIndex &idx) const {
	if (!idx.isValid())
		return Qt::ItemIsDropEnabled;

	if (idx.column() != 0)
		return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QVariant UserModel::otherRoles(const QModelIndex &idx, int role) const {
	ModelItem *item = static_cast<ModelItem *>(idx.internalPointer());
	ClientUser *p = item->pUser;
	Channel *c = item->cChan;
	int section = idx.column();
	bool isUser = p != NULL;

	switch (role) {
		case Qt::ToolTipRole:
			const_cast<UserModel *>(this)->uiSessionComment = 0;
			const_cast<UserModel *>(this)->iChannelDescription = -1;
			const_cast<UserModel *>(this)->bClicked = false;
			switch (section) {
				case 0: {
						if (isUser) {
							QString qsImage;
							if (! p->qbaTextureHash.isEmpty()) {
								if (p->qbaTexture.isEmpty()) {
									p->qbaTexture = Database::blob(p->qbaTextureHash);
									if (p->qbaTexture.isEmpty()) {
										MumbleProto::RequestBlob mprb;
										mprb.add_session_texture(p->uiSession);
										g.sh->sendMessage(mprb);
									} else {
										g.o->verifyTexture(p);
									}
								}
								if (! p->qbaTexture.isEmpty()) {
									QBuffer qb(&p->qbaTexture);
									qb.open(QIODevice::ReadOnly);
									QImageReader qir(&qb, p->qbaTextureFormat);
									QSize sz = qir.size();
									if (sz.width() > 0) {
										qsImage = QString::fromLatin1("<img src=\"data:;base64,");
										qsImage.append(QString::fromLatin1(p->qbaTexture.toBase64().toPercentEncoding()));
										if (sz.width() > 128) {
											int targ = sz.width() / ((sz.width()+127)/ 128);
											qsImage.append(QString::fromLatin1("\" width=\"%1\" />").arg(targ));
										} else {
											qsImage.append(QString::fromLatin1("\" />"));
										}
									}
								}
							}

							if (p->qbaCommentHash.isEmpty()) {
								if (! qsImage.isEmpty())
									return qsImage;
								else
									return p->qsName;
							} else {
								if (p->qsComment.isEmpty()) {
									p->qsComment = QString::fromUtf8(Database::blob(p->qbaCommentHash));
									if (p->qsComment.isEmpty()) {
										const_cast<UserModel *>(this)->uiSessionComment = p->uiSession;

										MumbleProto::RequestBlob mprb;
										mprb.add_session_comment(p->uiSession);
										g.sh->sendMessage(mprb);
										return QVariant();
									}
								}
								const_cast<UserModel *>(this)->seenComment(idx);
								QString base = Log::validHtml(p->qsComment);
								if (! qsImage.isEmpty())
									return QString::fromLatin1("<table><tr><td valign=\"top\">%1</td><td>%2</td></tr></table>").arg(qsImage,base);
								return base;
							}
						} else {
							if (c->qbaDescHash.isEmpty()) {
								return c->qsName;
							} else {
								if (c->qsDesc.isEmpty()) {
									c->qsDesc = QString::fromUtf8(Database::blob(c->qbaDescHash));
									if (c->qsDesc.isEmpty()) {
										const_cast<UserModel *>(this)->iChannelDescription = c->iId;

										MumbleProto::RequestBlob mprb;
										mprb.add_channel_description(c->iId);
										g.sh->sendMessage(mprb);
										return QVariant();
									}
								}

								const_cast<UserModel *>(this)->seenComment(idx);
								return Log::validHtml(c->qsDesc);
							}
						}
					}
					break;
				case 1:
					return isUser ? p->getFlagsString() : QVariant();
			}
			break;
		case Qt::WhatsThisRole:
			switch (section) {
				case 0:
					if (isUser)
						return QString::fromLatin1("%1"
						                           "<table>"
						                           "<tr><td><img src=\"skin:talking_on.svg\" width=64 /></td><td valign=\"middle\">%2</td></tr>"
						                           "<tr><td><img src=\"skin:talking_alt.svg\" width=64 /></td><td valign=\"middle\">%3</td></tr>"
						                           "<tr><td><img src=\"skin:talking_whisper.svg\" width=64 /></td><td valign=\"middle\">%4</td></tr>"
						                           "<tr><td><img src=\"skin:talking_off.svg\" width=64 /></td><td valign=\"middle\">%5</td></tr>"
						                           "</table>").arg(tr("This is a user connected to the server. The icon to the left of the user indicates whether or not they are talking:"),
						                                           tr("Talking to your channel."),
						                                           tr("Shouting directly to your channel."),
						                                           tr("Whispering directly to you."),
						                                           tr("Not talking.")
						                                          );
					else
						return QString::fromLatin1("%1"
						                           "<table>"
						                           "<tr><td><img src=\"skin:channel_active.svg\" width=64 /></td><td valign=\"middle\">%2</td></tr>"
						                           "<tr><td><img src=\"skin:channel_linked.svg\" width=64 /></td><td valign=\"middle\">%3</td></tr>"
						                           "<tr><td><img src=\"skin:channel.svg\" width=64 /></td><td valign=\"middle\">%4</td></tr>"
						                           "</table>").arg(tr("This is a channel on the server. The icon indicates the state of the channel:"),
						                                           tr("Your current channel."),
						                                           tr("A channel that is linked with your channel. Linked channels can talk to each other."),
						                                           tr("A channel on the server that you are not linked to.")
						                                          );
				case 1:
					if (isUser)
						return QString::fromLatin1("%1"
						                           "<table>"
						                           "<tr><td><img src=\"skin:emblems/emblem-favorite.svg\" width=64 /></td><td valign=\"middle\">%2</td></tr>"
						                           "<tr><td><img src=\"skin:authenticated.svg\" width=64 /></td><td valign=\"middle\">%3</td></tr>"
						                           "<tr><td><img src=\"skin:muted_self.svg\" width=64 /></td><td valign=\"middle\">%4</td></tr>"
						                           "<tr><td><img src=\"skin:muted_server.svg\" width=64 /></td><td valign=\"middle\">%5</td></tr>"
						                           "<tr><td><img src=\"skin:muted_suppressed.svg\" width=64 /></td><td valign=\"middle\">%6</td></tr>"
						                           "<tr><td><img src=\"skin:muted_local.svg\" width=64 /></td><td valign=\"middle\">%7</td></tr>"
						                           "<tr><td><img src=\"skin:deafened_self.svg\" width=64 /></td><td valign=\"middle\">%8</td></tr>"
						                           "<tr><td><img src=\"skin:deafened_server.svg\" width=64 /></td><td valign=\"middle\">%9</td></tr>"
						                           "<tr><td><img src=\"skin:comment.svg\" width=64 /></td><td valign=\"middle\">%10</td></tr>"
						                           "<tr><td><img src=\"skin:comment_seen.svg\" width=64 /></td><td valign=\"middle\">%11</td></tr>"
						                           "<tr><td><img src=\"skin:status/text-missing.svg\" width=64 /></td><td valign=\"middle\">%12</td></tr>"
						                           "</table>").arg(tr("This shows the flags the user has on the server, if any:"),
						                                           tr("On your friend list"),
						                                           tr("Authenticated user"),
						                                           tr("Muted (manually muted by self)"),
						                                           tr("Muted (manually muted by admin)"),
						                                           tr("Muted (not allowed to speak in current channel)"),
						                                           tr("Muted (muted by you, only on your machine)")
						                                          ).arg(
						                                           tr("Deafened (by self)"),
						                                           tr("Deafened (by admin)"),
						                                           tr("User has a new comment set (click to show)"),
						                                           tr("User has a comment set, which you've already seen. (click to show)"),
						                                           tr("Ignoring Text Messages")
						);
					else
						return QString::fromLatin1("%1"
						                           "<table>"
						                           "<tr><td><img src=\"skin:comment.svg\" width=64 /></td><td valign=\"middle\">%10</td></tr>"
						                           "<tr><td><img src=\"skin:comment_seen.svg\" width=64 /></td><td valign=\"middle\">%11</td></tr>"
						                           "<tr><td><img src=\"skin:filter.svg\" width=64 /></td><td valign=\"middle\">%12</td></tr>"
						                           "</table>").arg(tr("This shows the flags the channel has, if any:"),
						                                           tr("Channel has a new comment set (click to show)"),
						                                           tr("Channel has a comment set, which you've already seen. (click to show)"),
						                                           tr("Channel will be hidden when filtering is enabled")
						                                          );

			}
			break;
	}
	return QVariant();
}

QVariant UserModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
	if (orientation != Qt::Horizontal)
		return QVariant();

	switch (role) {
		case Qt::DisplayRole:
			switch (section) {
				case 0:
					return tr("Name");
				case 1:
					return tr("Flags");
			}
	}

	return QVariant();
}

void UserModel::recursiveClone(const ModelItem *old, ModelItem *item, QModelIndexList &from, QModelIndexList &to) {
	if (old->qlChildren.isEmpty())
		return;

	beginInsertRows(index(item), 0, old->qlChildren.count());

	for (int i=0;i<old->qlChildren.count();++i) {
		ModelItem *o = old->qlChildren.at(i);
		ModelItem *mi = new ModelItem(o);
		mi->parent = item;

		item->qlChildren << mi;

		from << createIndex(i, 0, o);
		from << createIndex(i, 1, o);
		to << createIndex(i, 0, mi);
		to << createIndex(i, 1, mi);
	}

	endInsertRows();

	for (int i=0;i<old->qlChildren.count();++i)
		recursiveClone(old->qlChildren.at(i), item->qlChildren.at(i), from, to);
}

ModelItem *UserModel::moveItem(ModelItem *oldparent, ModelItem *newparent, ModelItem *item) {
	// Here's the idea. We insert the item, update persistent indexes, THEN remove it.

	int oldrow = oldparent->qlChildren.indexOf(item);
	int newrow = -1;

	if (item->cChan)
		newrow = newparent->insertIndex(item->cChan);
	else
		newrow = newparent->insertIndex(item->pUser);

	if ((oldparent == newparent) && (newrow == oldrow)) {
		emit dataChanged(index(item),index(item));
		return item;
	}

	// Shallow clone
	ModelItem *t = new ModelItem(item);

	// Store the index if it's "active".
	// The selection is stored as "from"-"to" pairs, so if we move up in the same channel,
	// we'd move only "from" and select half the channel.

	QTreeView *v=g.mw->qtvUsers;
	QItemSelectionModel *sel=v->selectionModel();
	QPersistentModelIndex active;
	QModelIndex oindex = createIndex(oldrow, 0, item);
	if (sel->isSelected(oindex) || (oindex == v->currentIndex())) {
		active = index(item);
		v->clearSelection();
		v->setCurrentIndex(QModelIndex());
	}

	bool expanded = v->isExpanded(index(item));

	if (newparent == oldparent) {
		// Mangle rows. newrow needs to be pre-remove. oldrow needs to be postremove.
		if (oldrow >= newrow) {
			oldrow++;
		} else {
			newrow++;
		}
	}

	beginInsertRows(index(newparent), newrow, newrow);
	t->parent = newparent;
	newparent->qlChildren.insert(newrow, t);

	if (item->cChan) {
		oldparent->cChan->removeChannel(item->cChan);
		newparent->cChan->addChannel(item->cChan);
	} else {
		newparent->cChan->addClientUser(item->pUser);
	}

	endInsertRows();


	QModelIndexList from, to;
	from << createIndex(oldrow, 0, item);
	from << createIndex(oldrow, 1, item);
	to << createIndex(newrow, 0, t);
	to << createIndex(newrow, 1, t);

	recursiveClone(item, t, from, to);

	changePersistentIndexList(from, to);

	beginRemoveRows(index(oldparent), oldrow, oldrow);
	oldparent->qlChildren.removeAt(oldrow);
	endRemoveRows();

	item->wipe();
	delete item;

	if (active.isValid()) {
		sel->select(active, QItemSelectionModel::SelectCurrent);
		v->setCurrentIndex(active);
	}
	if (expanded)
		v->expand(index(t));
	return t;
}

void UserModel::expandAll(Channel *c) {
	QStack<Channel *> chans;

	while (c) {
		chans.push(c);
		c = c->cParent;
	}
	while (! chans.isEmpty()) {
		c = chans.pop();
		g.mw->qtvUsers->setExpanded(index(c), true);
	}
}

void UserModel::collapseEmpty(Channel *c) {
	while (c) {
		ModelItem *mi = ModelItem::c_qhChannels.value(c);
		if (mi->iUsers == 0)
			g.mw->qtvUsers->setExpanded(index(c), false);
		else
			break;
		c = c->cParent;
	}
}

void UserModel::ensureSelfVisible() {
	if (! g.uiSession)
		return;

	g.mw->qtvUsers->scrollTo(index(ClientUser::get(g.uiSession)));
}

void UserModel::recheckLinks() {
	if (! g.uiSession)
		return;

	bool bChanged = false;

	Channel *home = ClientUser::get(g.uiSession)->cChannel;

	QSet<Channel *> all = home->allLinks();

	if (all == qsLinked)
		return;

	QSet<Channel *> changed = (all - qsLinked);
	changed += (qsLinked - all);

	if ((all.count() == 1) || (qsLinked.count() == 1))
		changed += home;

	qsLinked = all;

	foreach(Channel *c, changed) {
		QModelIndex idx = index(c);
		emit dataChanged(idx, idx);
		bChanged = true;
	}
	if (bChanged)
		updateOverlay();
}

ClientUser *UserModel::addUser(unsigned int id, const QString &name) {
	ClientUser *p = ClientUser::add(id, this);
	p->qsName = name;

	ModelItem *item = new ModelItem(p);

	connect(p, SIGNAL(talkingStateChanged()), this, SLOT(userStateChanged()));
	connect(p, SIGNAL(muteDeafStateChanged()), this, SLOT(userStateChanged()));
	connect(p, SIGNAL(prioritySpeakerStateChanged()), this, SLOT(userStateChanged()));
	connect(p, SIGNAL(recordingStateChanged()), this, SLOT(userStateChanged()));

	Channel *c = Channel::get(0);
	ModelItem *citem = ModelItem::c_qhChannels.value(c);

	item->parent = citem;

	int row = citem->insertIndex(p);

	beginInsertRows(index(citem), row, row);
	citem->qlChildren.insert(row, item);
	c->addClientUser(p);
	endInsertRows();

	while (citem) {
		citem->iUsers++;
		citem = citem->parent;
	}

	updateOverlay();

	return p;
}

void UserModel::removeUser(ClientUser *p) {
	if (g.uiSession && p->uiSession == g.uiSession)
		g.uiSession = 0;
	Channel *c = p->cChannel;
	ModelItem *item = ModelItem::c_qhUsers.value(p);
	ModelItem *citem = ModelItem::c_qhChannels.value(c);

	int row = citem->qlChildren.indexOf(item);

	beginRemoveRows(index(citem), row, row);
	c->removeUser(p);
	citem->qlChildren.removeAt(row);
	endRemoveRows();

	p->cChannel = NULL;

	ClientUser::remove(p);
	qmHashes.remove(p->qsHash);

	while (citem) {
		citem->iUsers--;
		citem = citem->parent;
	}

	if (g.s.ceExpand == Settings::ChannelsWithUsers)
		collapseEmpty(c);

	updateOverlay();

#ifdef REPORT_JITTER
	g.mw->uUsage.addJitter(p);
#endif

	delete p;
	delete item;
}

void UserModel::moveUser(ClientUser *p, Channel *np) {
	Channel *oc = p->cChannel;
	ModelItem *opi = ModelItem::c_qhChannels.value(oc);
	ModelItem *pi = ModelItem::c_qhChannels.value(np);
	ModelItem *item = ModelItem::c_qhUsers.value(p);

	item = moveItem(opi, pi, item);

	if (p->uiSession == g.uiSession) {
		ensureSelfVisible();
		recheckLinks();
	}

	while (opi) {
		opi->iUsers--;
		opi = opi->parent;
	}
	while (pi) {
		pi->iUsers++;
		pi = pi->parent;
	}

	if (g.s.ceExpand == Settings::ChannelsWithUsers) {
		expandAll(np);
		collapseEmpty(oc);
	}

	updateOverlay();
}

void UserModel::renameUser(ClientUser *p, const QString &name) {
	Channel *c = p->cChannel;
	p->qsName = name;

	ModelItem *pi = ModelItem::c_qhChannels.value(c);
	ModelItem *item = ModelItem::c_qhUsers.value(p);
	moveItem(pi, pi, item);

	updateOverlay();
}

void UserModel::setUserId(ClientUser *p, int id) {
	p->iId = id;
	QModelIndex idx = index(p, 0);
	emit dataChanged(idx, idx);
}

void UserModel::setHash(ClientUser *p, const QString &hash) {
	if (! p->qsHash.isEmpty())
		qmHashes.remove(p->qsHash);

	p->qsHash = hash;
	qmHashes.insert(p->qsHash, p);
}

void UserModel::setFriendName(ClientUser *p, const QString &name) {
	p->qsFriendName = name;
	QModelIndex idx = index(p, 0);
	emit dataChanged(idx, idx);
}

void UserModel::setComment(ClientUser *cu, const QString &comment) {
	cu->qbaCommentHash = comment.isEmpty() ? QByteArray() : sha1(comment);

	if (comment != cu->qsComment) {
		ModelItem *item = ModelItem::c_qhUsers.value(cu);
		int oldstate = (cu->qsComment.isEmpty() && cu->qbaCommentHash.isEmpty()) ? 0 : (item->bCommentSeen ? 2 : 1);
		int newstate = 0;

		cu->qsComment = comment;

		if (! comment.isEmpty()) {
			Database::setBlob(cu->qbaCommentHash, cu->qsComment.toUtf8());
			if (cu->uiSession == uiSessionComment) {
				uiSessionComment = 0;
				item->bCommentSeen = false;
				if (bClicked) {
					QRect r = g.mw->qtvUsers->visualRect(index(cu));
					QWhatsThis::showText(g.mw->qtvUsers->viewport()->mapToGlobal(r.bottomRight()), data(index(cu, 0), Qt::ToolTipRole).toString(), g.mw->qtvUsers);
				} else {
					QToolTip::showText(QCursor::pos(), data(index(cu, 0), Qt::ToolTipRole).toString(), g.mw->qtvUsers);
				}
			} else if (cu->uiSession == ~uiSessionComment) {
				uiSessionComment = 0;
				if (cu->uiSession == g.uiSession) {
					QTimer::singleShot(0, g.mw, SLOT(on_qaSelfComment_triggered()));
				} else {
					g.mw->cuContextUser = cu;
					QTimer::singleShot(0, g.mw, SLOT(on_qaUserCommentView_triggered()));
				}
			} else {
				item->bCommentSeen = Database::seenComment(item->hash(), cu->qbaCommentHash);
				newstate = item->bCommentSeen ? 2 : 1;
			}
		} else {
			item->bCommentSeen = true;
		}

		if (oldstate != newstate) {
			QModelIndex idx = index(cu, 0);
			emit dataChanged(idx, idx);
		}
	}
}

void UserModel::setCommentHash(ClientUser *cu, const QByteArray &hash) {
	if (hash != cu->qbaCommentHash) {
		ModelItem *item = ModelItem::c_qhUsers.value(cu);
		int oldstate = (cu->qsComment.isEmpty() && cu->qbaCommentHash.isEmpty()) ? 0 : (item->bCommentSeen ? 2 : 1);
		int newstate;

		cu->qsComment = QString();
		cu->qbaCommentHash = hash;

		item->bCommentSeen = Database::seenComment(item->hash(), cu->qbaCommentHash);
		newstate = item->bCommentSeen ? 2 : 1;

		if (oldstate != newstate) {
			QModelIndex idx = index(cu, 0);
			emit dataChanged(idx, idx);
		}
	}
}

void UserModel::setComment(Channel *c, const QString &comment) {
	c->qbaDescHash = comment.isEmpty() ? QByteArray() : sha1(comment);

	if (comment != c->qsDesc) {
		ModelItem *item = ModelItem::c_qhChannels.value(c);
		int oldstate = c->qsDesc.isEmpty() ? 0 : (item->bCommentSeen ? 2 : 1);
		int newstate = 0;

		c->qsDesc = comment;

		if (! comment.isEmpty()) {
			Database::setBlob(c->qbaDescHash, c->qsDesc.toUtf8());

			if (c->iId == iChannelDescription) {
				iChannelDescription = -1;
				item->bCommentSeen = false;
				if (bClicked) {
					QRect r = g.mw->qtvUsers->visualRect(index(c));
					QWhatsThis::showText(g.mw->qtvUsers->viewport()->mapToGlobal(r.bottomRight()), data(index(c, 0), Qt::ToolTipRole).toString(), g.mw->qtvUsers);
				} else {
					QToolTip::showText(QCursor::pos(), data(index(c, 0), Qt::ToolTipRole).toString(), g.mw->qtvUsers);
				}
			} else {
				item->bCommentSeen = Database::seenComment(item->hash(), c->qbaDescHash);
				newstate = item->bCommentSeen ? 2 : 1;
			}
		} else {
			item->bCommentSeen = true;
		}

		if (oldstate != newstate) {
			QModelIndex idx = index(c, 0);
			emit dataChanged(idx, idx);
		}
	}
}

void UserModel::setCommentHash(Channel *c, const QByteArray &hash) {
	if (hash != c->qbaDescHash) {
		ModelItem *item = ModelItem::c_qhChannels.value(c);
		int oldstate = (c->qsDesc.isEmpty() && c->qbaDescHash.isEmpty()) ? 0 : (item->bCommentSeen ? 2 : 1);
		int newstate;

		c->qsDesc = QString();
		c->qbaDescHash = hash;

		item->bCommentSeen = Database::seenComment(item->hash(), hash);
		newstate = item->bCommentSeen ? 2 : 1;

		if (oldstate != newstate) {
			QModelIndex idx = index(c, 0);
			emit dataChanged(idx, idx);
		}
	}
}

void UserModel::seenComment(const QModelIndex &idx) {
	ModelItem *item;
	item = static_cast<ModelItem *>(idx.internalPointer());

	if (item->bCommentSeen)
		return;

	item->bCommentSeen = true;

	emit dataChanged(idx, idx);

	if (item->pUser)
		Database::setSeenComment(item->hash(), item->pUser->qbaCommentHash);
	else
		Database::setSeenComment(item->hash(), item->cChan->qbaDescHash);
}

void UserModel::renameChannel(Channel *c, const QString &name) {
	c->qsName = name;

	if (c->iId == 0) {
		QModelIndex idx = index(c);
		emit dataChanged(idx, idx);
	} else {
		Channel *pc = c->cParent;
		ModelItem *pi = ModelItem::c_qhChannels.value(pc);
		ModelItem *item = ModelItem::c_qhChannels.value(c);

		moveItem(pi, pi, item);
	}
}

void UserModel::repositionChannel(Channel *c, const int position) {
	c->iPosition = position;

	if (c->iId == 0) {
		QModelIndex idx = index(c);
		emit dataChanged(idx, idx);
	} else {
		Channel *pc = c->cParent;
		ModelItem *pi = ModelItem::c_qhChannels.value(pc);
		ModelItem *item = ModelItem::c_qhChannels.value(c);

		moveItem(pi, pi, item);
	}
}

Channel *UserModel::addChannel(int id, Channel *p, const QString &name) {
	Channel *c = Channel::add(id, name);

	if (! c)
		return NULL;

	ModelItem *item = new ModelItem(c);
	ModelItem *citem = ModelItem::c_qhChannels.value(p);

	item->parent = citem;

	int row = citem->insertIndex(c);

	beginInsertRows(index(citem), row, row);
	p->addChannel(c);
	citem->qlChildren.insert(row, item);
	endInsertRows();

	if (g.s.ceExpand == Settings::AllChannels)
		g.mw->qtvUsers->setExpanded(index(item), true);

	return c;
}

void UserModel::removeChannel(Channel *c) {
	ModelItem *item, *i;

	item=ModelItem::c_qhChannels.value(c);

	foreach(i, item->qlChildren) {
		if (i->pUser)
			removeUser(i->pUser);
		else
			removeChannel(i->cChan);
	}

	Channel *p = c->cParent;

	if (! p)
		return;

	ModelItem *citem = ModelItem::c_qhChannels.value(p);

	int row = citem->rowOf(c);

	beginRemoveRows(index(citem), row, row);
	p->removeChannel(c);
	citem->qlChildren.removeAt(row);
	qsLinked.remove(c);
	endRemoveRows();

	Channel::remove(c);

	delete item;
	delete c;
}

void UserModel::moveChannel(Channel *c, Channel *p) {
	Channel *oc = c->cParent;
	ModelItem *opi = ModelItem::c_qhChannels.value(c->cParent);
	ModelItem *pi = ModelItem::c_qhChannels.value(p);
	ModelItem *item = ModelItem::c_qhChannels.value(c);
	item = moveItem(opi, pi, item);

	while (opi) {
		opi->iUsers -= item->iUsers;
		opi = opi->parent;
	}
	while (pi) {
		pi->iUsers += item->iUsers;
		pi = pi->parent;
	}

	ensureSelfVisible();

	if (g.s.ceExpand == Settings::ChannelsWithUsers) {
		collapseEmpty(oc);
	}
}

void UserModel::linkChannels(Channel *c, QList<Channel *> links) {
	foreach(Channel *l, links)
		c->link(l);
	recheckLinks();
}

void UserModel::unlinkChannels(Channel *c, QList<Channel *> links) {
	foreach(Channel *l, links)
		c->unlink(l);
	recheckLinks();
}

void UserModel::unlinkAll(Channel *c) {
	c->unlink(NULL);
	recheckLinks();
}

void UserModel::removeAll() {
	ModelItem *item = miRoot;
	ModelItem *i;

	uiSessionComment = 0;
	iChannelDescription = -1;
	bClicked = false;

	foreach(i, item->qlChildren) {
		if (i->pUser)
			removeUser(i->pUser);
		else
			removeChannel(i->cChan);
	}

	qsLinked.clear();

	updateOverlay();
}

ClientUser *UserModel::getUser(const QModelIndex &idx) const {
	if (! idx.isValid())
		return NULL;

	ModelItem *item;
	item = static_cast<ModelItem *>(idx.internalPointer());

	return item->pUser;
}

ClientUser *UserModel::getUser(const QString &hash) const {
	return qmHashes.value(hash);
}

Channel *UserModel::getChannel(const QModelIndex &idx) const {
	if (! idx.isValid())
		return NULL;

	ModelItem *item;
	item = static_cast<ModelItem *>(idx.internalPointer());

	if (item->pUser)
		return item->pUser->cChannel;
	else
		return item->cChan;
}

Channel *UserModel::getSubChannel(Channel *p, int idx) const {
	ModelItem *item=ModelItem::c_qhChannels.value(p);
	if (! item)
		return NULL;

	foreach(ModelItem *i, item->qlChildren) {
		if (i->cChan) {
			if (idx == 0)
				return i->cChan;
			idx--;
		}
	}
	return NULL;
}

void UserModel::userStateChanged() {
	ClientUser *user = qobject_cast<ClientUser *>(sender());
	if (user == NULL)
		return;
	
	const QModelIndex idx = index(user);
	emit dataChanged(idx, idx);
	
	updateOverlay();
}

void UserModel::toggleChannelFiltered(Channel *c) {
	QModelIndex idx;
	if(c) {
		c->bFiltered = !c->bFiltered;

		ServerHandlerPtr sh = g.sh;
		Database::setChannelFiltered(sh->qbaDigest, c->iId, c->bFiltered);
		idx = index(c);
	}

	emit dataChanged(idx, idx);

	updateOverlay();
}

Qt::DropActions UserModel::supportedDropActions() const {
	return Qt::MoveAction;
}

QStringList UserModel::mimeTypes() const {
	QStringList sl;
	sl << QLatin1String("mumble/dragentry");
	return sl;
}

QMimeData *UserModel::mimeData(const QModelIndexList &idxs) const {
	QModelIndex idx;
	QByteArray qba;
	QDataStream ds(&qba, QIODevice::WriteOnly);

	foreach(idx, idxs) {
		ClientUser *p = getUser(idx);
		Channel *c = getChannel(idx);
		if (p) {
			ds << false;
			ds << p->uiSession;
		} else if (c) {
			ds << true;
			ds << c->iId;
		}
	}
	QMimeData *md = new QMimeData();
	md->setData(QLatin1String("mumble/dragentry"), qba);
	return md;
}

bool UserModel::dropMimeData(const QMimeData *md, Qt::DropAction, int row, int column, const QModelIndex &p) {
#define NAMECMPCHANNEL(first, second) (QString::localeAwareCompare(first->qsName, second->qsName) > 0)

	if (! md->hasFormat(mimeTypes().at(0)))
		return false;

	QByteArray qba = md->data(mimeTypes().at(0));
	QDataStream ds(qba);

	bool isChannel;
	int iId = -1;
	unsigned int uiSession = 0;
	ds >> isChannel;

	if (isChannel)
		ds >> iId;
	else
		ds >> uiSession;

	Channel *c;
	if (! p.isValid()) {
		c = Channel::get(0);
	} else {
		c = getChannel(p);
	}

	if (! c)
		return false;

	expandAll(c);

	if (! isChannel) {
		// User dropped somewhere
		MumbleProto::UserState mpus;
		mpus.set_session(uiSession);
		mpus.set_channel_id(c->iId);
		g.sh->sendMessage(mpus);
	} else if (c->iId != iId) {
		// Channel dropped somewhere (not on itself)
		int ret;
		switch (g.s.ceChannelDrag) {
			case Settings::Ask:
				ret=QMessageBox::question(g.mw, QLatin1String("Mumble"), tr("Are you sure you want to drag this channel?"), QMessageBox::Yes, QMessageBox::No);

				if (ret == QMessageBox::No)
					return false;
				break;
			case Settings::DoNothing:
				g.l->log(Log::Information, MainWindow::tr("You have Channel Dragging set to \"Do Nothing\" so the channel wasn't moved."));
				return false;
				break;
			case Settings::Move:
				break;
			default:
				g.l->log(Log::CriticalError, MainWindow::tr("Unknown Channel Drag mode in UserModel::dropMimeData."));
				return false;
				break;
		}

		long long inewpos = 0;
		Channel *dropped = Channel::c_qhChannels.value(iId);

		if (! dropped)
			return false;

		if (p.isValid()) {
			ModelItem *pi = static_cast<ModelItem *>(p.internalPointer());
			if (pi->pUser)
				pi = pi->parent;

			int ifirst = 0;
			int ilast = pi->rows() - 1;

			if (ilast > 0) {
				while (pi->userAt(ifirst) && ifirst < ilast) ifirst++;
				while (pi->userAt(ilast) && ilast > 0) ilast--;
			}

			if (row == -1 && column == -1) {
				// Dropped on item
				if (getUser(p)) {
					// Dropped on player
					if (ilast > 0) {
						if (pi->bUsersTop) {
							if (pi->channelAt(ifirst) == dropped || NAMECMPCHANNEL(pi->channelAt(ifirst), dropped)) {
								if (dropped->iPosition ==  pi->channelAt(ifirst)->iPosition) return true;
								inewpos = pi->channelAt(ifirst)->iPosition;
							} else {
								inewpos = static_cast<long long>(pi->channelAt(ifirst)->iPosition) - 20;
							}
						} else {
							if (dropped == pi->channelAt(ilast) || NAMECMPCHANNEL(dropped, pi->channelAt(ilast))) {
								if (pi->channelAt(ilast)->iPosition == dropped->iPosition) return true;
								inewpos = pi->channelAt(ilast)->iPosition;
							} else {
								inewpos = static_cast<long long>(pi->channelAt(ilast)->iPosition) + 20;
							}
						}
					}
				}
			} else {
				// Dropped between items
				if (ilast == 0) {
					// No channels in there yet
				} else if (row <= ifirst) {
					if (pi->channelAt(ifirst) == dropped || NAMECMPCHANNEL(pi->channelAt(ifirst), dropped)) {
						if (dropped->iPosition ==  pi->channelAt(ifirst)->iPosition) return true;
						inewpos = pi->channelAt(ifirst)->iPosition;
					} else {
						inewpos = static_cast<long long>(pi->channelAt(ifirst)->iPosition) - 20;
					}
				} else if (row > ilast) {
					if (dropped == pi->channelAt(ilast) || NAMECMPCHANNEL(dropped, pi->channelAt(ilast))) {
						if (pi->channelAt(ilast)->iPosition == dropped->iPosition) return true;
						inewpos = pi->channelAt(ilast)->iPosition;
					} else {
						inewpos = static_cast<long long>(pi->channelAt(ilast)->iPosition) + 20;
					}
				} else {
					// Dropped between channels
					Channel *lower = pi->channelAt(row);
					Channel *upper = pi->channelAt(row - 1);

					if (lower->iPosition == upper->iPosition && NAMECMPCHANNEL(lower, dropped) && NAMECMPCHANNEL(dropped, upper)) {
						inewpos = upper->iPosition;
					} else if (lower->iPosition > upper->iPosition && NAMECMPCHANNEL(lower, dropped)) {
						inewpos = lower->iPosition;
					} else if (lower->iPosition > upper->iPosition && NAMECMPCHANNEL(dropped, upper)) {
						inewpos = upper->iPosition;
					} else if (lower == dropped || upper == dropped) {
						return true;
					} else if (abs(lower->iPosition) - abs(upper->iPosition) > 1) {
						inewpos = upper->iPosition + (abs(lower->iPosition) - abs(upper->iPosition))/2;
					} else {
						// Not enough space, other channels have to be moved
						if (static_cast<long long>(pi->channelAt(ilast)->iPosition) + 40 > INT_MAX) {
							QMessageBox::critical(g.mw, QLatin1String("Mumble"), tr("Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually."));
							return false;
						}
						for (int i = row; i <= ilast; i++) {
							Channel *tmp = pi->channelAt(i);
							if (tmp != dropped) {
								MumbleProto::ChannelState mpcs;
								mpcs.set_channel_id(tmp->iId);
								mpcs.set_position(tmp->iPosition + 40);
								g.sh->sendMessage(mpcs);

							}
						}
						inewpos = upper->iPosition + 20;
					}
				}
			}
		}

		if (inewpos > INT_MAX || inewpos < INT_MIN) {
			QMessageBox::critical(g.mw, QLatin1String("Mumble"), tr("Cannot perform this movement automatically, please reset the numeric sorting indicators or adjust it manually."));
			return false;
		}

		MumbleProto::ChannelState mpcs;
		mpcs.set_channel_id(iId);
		if (dropped->parent() != c)
			mpcs.set_parent(c->iId);
		mpcs.set_position(static_cast<int>(inewpos));
		g.sh->sendMessage(mpcs);
	}

	return true;
}

void UserModel::updateOverlay() const {
	g.o->updateOverlay();
	g.lcd->updateUserView();
}
