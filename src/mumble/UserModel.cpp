// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserModel.h"

#include "Accessibility.h"
#include "Channel.h"
#include "ClientUser.h"
#include "Database.h"
#include "LCD.h"
#include "Log.h"
#include "MainWindow.h"
#ifdef USE_OVERLAY
#	include "Overlay.h"
#endif
#include "ChannelListenerManager.h"
#include "ServerHandler.h"
#include "Usage.h"
#include "User.h"
#include "VolumeAdjustment.h"
#include "Global.h"

#include <QtCore/QMimeData>
#include <QtCore/QStack>
#include <QtGui/QImageReader>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolTip>
#include <QtWidgets/QWhatsThis>

QHash< const Channel *, ModelItem * > ModelItem::c_qhChannels;
QHash< const ClientUser *, ModelItem * > ModelItem::c_qhUsers;
QHash< const ClientUser *, QList< ModelItem * > > ModelItem::s_userProxies;
bool ModelItem::bUsersTop = false;

ModelItem::ModelItem(Channel *c) {
	this->cChan      = c;
	this->pUser      = nullptr;
	this->isListener = false;
	bCommentSeen     = true;
	c_qhChannels.insert(c, this);
	parent = c_qhChannels.value(c->cParent);
	iUsers = 0;
}

ModelItem::ModelItem(ClientUser *p, bool isListener) {
	this->cChan      = nullptr;
	this->pUser      = p;
	this->isListener = isListener;
	bCommentSeen     = true;
	if (isListener) {
		// The way operator[] works for a QHash is that it'll insert a default-constructed
		// object first, before returning a reference to it, in case there is no entry for
		// the provided key yet. Thus we never have to worry about explicitly adding an empty
		// list for a new user before accessing it.
		s_userProxies[p] << this;
	} else {
		c_qhUsers.insert(p, this);
	}
	parent = c_qhChannels.value(p->cChannel);
	iUsers = 0;
}

ModelItem::ModelItem(ModelItem *i) {
	// Create a shallow clone
	this->cChan        = i->cChan;
	this->pUser        = i->pUser;
	this->parent       = i->parent;
	this->bCommentSeen = i->bCommentSeen;
	this->isListener   = i->isListener;

	if (pUser) {
		if (isListener) {
			s_userProxies[pUser] << this;
		} else {
			c_qhUsers.insert(pUser, this);
		}
	} else if (cChan)
		c_qhChannels.insert(cChan, this);

	iUsers = i->iUsers;
}

ModelItem::~ModelItem() {
	Q_ASSERT(qlChildren.count() == 0);

	if (cChan && c_qhChannels.value(cChan) == this)
		c_qhChannels.remove(cChan);
	if (pUser) {
		if (isListener) {
			s_userProxies[pUser].removeAll(this);
		} else {
			if (c_qhUsers.value(pUser) == this)
				c_qhUsers.remove(pUser);
		}
	}
}

void ModelItem::wipe() {
	foreach (ModelItem *i, qlChildren) {
		i->wipe();
		delete i;
	}
	qlChildren.clear();
	iUsers = 0;
}

ModelItem *ModelItem::child(int idx) const {
	if (!validRow(idx))
		return nullptr;

	return qlChildren.at(idx);
}

bool ModelItem::validRow(int idx) const {
	return ((idx >= 0) && (idx < qlChildren.count()));
}

ClientUser *ModelItem::userAt(int idx) const {
	if (!validRow(idx))
		return nullptr;
	return qlChildren.at(idx)->pUser;
}

Channel *ModelItem::channelAt(int idx) const {
	if (!validRow(idx))
		return nullptr;
	return qlChildren.at(idx)->cChan;
}

int ModelItem::rowOf(Channel *c) const {
	for (int i = 0; i < qlChildren.count(); i++)
		if (qlChildren.at(i)->cChan == c)
			return i;
	return -1;
}

int ModelItem::rowOf(ClientUser *p, const bool lookForListener) const {
	for (int i = 0; i < qlChildren.count(); i++)
		if (qlChildren.at(i)->isListener == lookForListener && qlChildren.at(i)->pUser == p)
			return i;
	return -1;
}

int ModelItem::rowOfSelf() const {
	// Root?
	if (!parent)
		return 0;

	if (pUser)
		return parent->rowOf(pUser, isListener);
	else
		return parent->rowOf(cChan);
}

int ModelItem::rows() const {
	return static_cast< int >(qlChildren.count());
}

int ModelItem::insertIndex(Channel *c) const {
	QList< Channel * > qlpc;
	ModelItem *item;

	int ocount = 0;

	foreach (item, qlChildren) {
		if (item->cChan) {
			if (item->cChan != c) {
				qlpc << item->cChan;
			}
		} else
			ocount++;
	}
	qlpc << c;
	std::sort(qlpc.begin(), qlpc.end(), Channel::lessThan);
	return static_cast< int >(qlpc.indexOf(c) + (bUsersTop ? ocount : 0));
}

int ModelItem::insertIndex(ClientUser *p, bool userIsListener) const {
	QList< ClientUser * > qlclientuser;
	ModelItem *item;

	int ocount        = 0;
	int listenerCount = 0;

	foreach (item, qlChildren) {
		if (item->pUser) {
			if (item->pUser != p) {
				// Make sure listeners and non-listeners are all grouped together and not mixed
				if ((userIsListener && item->isListener) || (!userIsListener && !item->isListener)) {
					qlclientuser << item->pUser;
				}
			}

			if (item->isListener) {
				listenerCount++;
			}
		} else {
			ocount++;
		}
	}

	qlclientuser << p;
	std::sort(qlclientuser.begin(), qlclientuser.end(), ClientUser::lessThan);

	// Make sure that the a user is always added to other users either all above or all below
	// sub-channels) and also make sure that listeners are grouped together and directly above
	// normal users.
	return static_cast< int >(qlclientuser.indexOf(p) + (bUsersTop ? 0 : ocount)
							  + (userIsListener ? 0 : listenerCount));
}

QString ModelItem::hash() const {
	if (pUser) {
		if (!pUser->qsHash.isEmpty())
			return pUser->qsHash + (isListener ? QLatin1String("l") : QString());
		else
			return QLatin1String(sha1(pUser->qsName + (isListener ? QLatin1String("l") : QString())).toHex());
	} else {
		QCryptographicHash chash(QCryptographicHash::Sha1);

		chash.addData(cChan->qsName.toUtf8());
		chash.addData(QString::number(cChan->iId).toUtf8());
		if (Global::get().sh && Global::get().sh->isRunning()) {
			QString host, user, pw;
			unsigned short port;
			Global::get().sh->getConnectionInfo(host, port, user, pw);
			chash.addData(host.toUtf8());
			chash.addData(QString::number(port).toUtf8());
		}
		return QLatin1String(chash.result().toHex());
	}
}

UserModel::UserModel(QObject *p) : QAbstractItemModel(p) {
	qiTalkingOff      = QIcon(QLatin1String("skin:talking_off.svg"));
	qiTalkingOn       = QIcon(QLatin1String("skin:talking_on.svg"));
	qiTalkingMuted    = QIcon(QLatin1String("skin:talking_muted.svg"));
	qiTalkingShout    = QIcon(QLatin1String("skin:talking_alt.svg"));
	qiTalkingWhisper  = QIcon(QLatin1String("skin:talking_whisper.svg"));
	qiPrioritySpeaker = QIcon(QLatin1String("skin:priority_speaker.svg"));
	qiRecording       = QIcon(QLatin1String("skin:actions/media-record.svg"));
	qiMutedPushToMute.addFile(QLatin1String("skin:muted_pushtomute.svg"));
	qiMutedSelf       = QIcon(QLatin1String("skin:muted_self.svg"));
	qiMutedServer     = QIcon(QLatin1String("skin:muted_server.svg"));
	qiMutedLocal      = QIcon(QLatin1String("skin:muted_local.svg"));
	qiIgnoredLocal    = QIcon(QLatin1String("skin:status/text-missing.svg"));
	qiMutedSuppressed = QIcon(QLatin1String("skin:muted_suppressed.svg"));
	qiDeafenedSelf    = QIcon(QLatin1String("skin:deafened_self.svg"));
	qiDeafenedServer  = QIcon(QLatin1String("skin:deafened_server.svg"));
	qiAuthenticated   = QIcon(QLatin1String("skin:authenticated.svg"));
	qiChannel         = QIcon(QLatin1String("skin:channel.svg"));
	qiActiveChannel   = QIcon(QLatin1String("skin:channel_active.svg"));
	qiLinkedChannel   = QIcon(QLatin1String("skin:channel_linked.svg"));
	qiFriend          = QIcon(QLatin1String("skin:emblems/emblem-favorite.svg"));
	qiComment         = QIcon(QLatin1String("skin:comment.svg"));
	qiCommentSeen     = QIcon(QLatin1String("skin:comment_seen.svg"));
	qiFilter          = QIcon(QLatin1String("skin:filter.svg"));
	qiPin             = QIcon(QLatin1String("skin:pin.svg"));
	qiLock_locked     = QIcon(QLatin1String("skin:lock_locked.svg"));
	qiLock_unlocked   = QIcon(QLatin1String("skin:lock_unlocked.svg"));
	qiEar             = QIcon(QLatin1String("skin:ear.svg"));

	ModelItem::bUsersTop = Global::get().s.bUserTop;

	uiSessionComment    = 0;
	iChannelDescription = -1;
	bClicked            = false;

	miRoot = new ModelItem(Channel::get(Channel::ROOT_ID));
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

	if (!p.isValid()) {
		return createIndex(row, column, miRoot);
	} else {
		item = static_cast< ModelItem * >(p.internalPointer());
	}

	if (!item)
		return idx;

	if (!item->validRow(row))
		return idx;

	idx = createIndex(row, column, item->child(row));

	return idx;
}

QModelIndex UserModel::index(ClientUser *p, int column) const {
	ModelItem *item = ModelItem::c_qhUsers.value(p);
	Q_ASSERT(p);
	Q_ASSERT(item);
	if (!p || !item)
		return QModelIndex();
	QModelIndex idx = createIndex(item->rowOfSelf(), column, item);
	return idx;
}

QModelIndex UserModel::index(Channel *c, int column) const {
	ModelItem *item = ModelItem::c_qhChannels.value(c);
	Q_ASSERT(c);
	Q_ASSERT(item);
	if (!item || !c)
		return QModelIndex();
	QModelIndex idx = createIndex(item->rowOfSelf(), column, item);
	return idx;
}

QModelIndex UserModel::index(ModelItem *item) const {
	return createIndex(item->rowOfSelf(), 0, item);
}

QModelIndex UserModel::channelListenerIndex(const ClientUser *user, const Channel *channel, int column) const {
	QList< ModelItem * > items = ModelItem::s_userProxies.value(user);

	ModelItem *item = nullptr;
	for (ModelItem *currentItem : items) {
		ModelItem *parent = currentItem->parent;
		if (currentItem->isListener && parent && parent->cChan == channel) {
			item = currentItem;
			break;
		}
	}

	Q_ASSERT(user);
	Q_ASSERT(channel);
	Q_ASSERT(item);
	if (!item || !channel || !user) {
		return QModelIndex();
	}

	QModelIndex idx = createIndex(item->rowOfSelf(), column, item);

	return idx;
}

QModelIndex UserModel::parent(const QModelIndex &idx) const {
	if (!idx.isValid())
		return QModelIndex();

	ModelItem *item = static_cast< ModelItem * >(idx.internalPointer());

	ModelItem *parent_item = item ? item->parent : nullptr;

	if (!parent_item)
		return QModelIndex();

	return createIndex(parent_item->rowOfSelf(), 0, parent_item);
}

int UserModel::rowCount(const QModelIndex &p) const {
	ModelItem *item;

	int val = 0;

	if (!p.isValid())
		return 1;
	else
		item = static_cast< ModelItem * >(p.internalPointer());

	if (!item || (p.column() != 0))
		return 0;

	val = item->rows();

	return val;
}

QString UserModel::stringIndex(const QModelIndex &idx) const {
	ModelItem *item = static_cast< ModelItem * >(idx.internalPointer());
	if (!idx.isValid())
		return QLatin1String("invIdx");
	if (!item)
		return QLatin1String("invPtr");
	if (item->pUser)
		return QString::fromLatin1("P:%1 [%2,%3]").arg(item->pUser->qsName).arg(idx.row()).arg(idx.column());
	else
		return QString::fromLatin1("C:%1 [%2,%3]").arg(item->cChan->qsName).arg(idx.row()).arg(idx.column());
}

QModelIndex UserModel::getSelectedIndex() const {
	QTreeView *v = Global::get().mw->qtvUsers;
	if (v) {
		QItemSelectionModel *sel = v->selectionModel();

		return sel->currentIndex();
	}

	return QModelIndex();
}

QVariant UserModel::data(const QModelIndex &idx, int role) const {
	if (!idx.isValid())
		return QVariant();

	ModelItem *item = static_cast< ModelItem * >(idx.internalPointer());

	Channel *c        = item->cChan;
	ClientUser *p     = item->pUser;
	ClientUser *pSelf = ClientUser::get(Global::get().uiSession);

	if (!c && !p) {
		return QVariant();
	}

	QVariant v = otherRoles(idx, role);
	if (v.isValid())
		return v;

	QList< QVariant > l;

	if (p) {
		switch (role) {
			case Qt::DecorationRole:
				if (idx.column() == 0) {
					if (item->isListener) {
						return qiEar;
					} else {
						// Select the talking-state symbol to display
						if (p == pSelf && p->bSelfMute) {
							// This is a workaround for a bug that can lead to the user having muted him/herself but
							// the talking icon is stuck at qiTalkingOn for some reason.
							// Until someone figures out how to fix the root of the problem, we'll have this workaround
							// to cure the symptoms of the bug.
							return qiTalkingOff;
						}

						switch (p->tsState) {
							case Settings::Talking:
								return qiTalkingOn;
							case Settings::MutedTalking:
								return qiTalkingMuted;
							case Settings::Whispering:
								return qiTalkingWhisper;
							case Settings::Shouting:
								return qiTalkingShout;
							case Settings::Passive:
							default:
								return qiTalkingOff;
						}
					}
				}
				break;
			case Qt::FontRole:
				if ((idx.column() == 0) && (p->uiSession == Global::get().uiSession)) {
					QFont f = Global::get().mw->font();
					f.setBold(!f.bold());
					f.setItalic(item->isListener);
					return f;
				}
				if (item->isListener) {
					QFont f = Global::get().mw->font();
					f.setItalic(true);
					return f;
				}
				break;
			case Qt::DisplayRole:
				if (idx.column() == 0) {
					// Get the channel the user/listener is in
					const Channel *parentChannel = item->parent ? item->parent->cChan : nullptr;

					return createDisplayString(*p, item->isListener, parentChannel);
				}

				// Most of the following icons are for non-listeners (as listeners are merely proxies) only
				// but in order to not change the order of the icons, the condition is added to each case
				// individually instead of checking it up front.
				if (!p->qbaCommentHash.isEmpty() && !item->isListener)
					l << (item->bCommentSeen ? qiCommentSeen : qiComment);
				if (p->bPrioritySpeaker && !item->isListener)
					l << qiPrioritySpeaker;
				if (p->bRecording)
					l << qiRecording;
				// ClientUser doesn't contain a push-to-mute
				// state because it isn't sent to the server.
				// We can show the icon only for the local user.
				if (p == pSelf && Global::get().bPushToMute && !item->isListener)
					l << qiMutedPushToMute;
				if (p->bMute || item->isListener)
					l << qiMutedServer;
				if (p->bSuppress && !item->isListener)
					l << qiMutedSuppressed;
				if (p->bSelfMute && !item->isListener)
					l << qiMutedSelf;
				if (p->bLocalMute && !item->isListener)
					l << qiMutedLocal;
				if (p->bLocalIgnore && !item->isListener)
					l << qiIgnoredLocal;
				if (p->bDeaf)
					l << qiDeafenedServer;
				if (p->bSelfDeaf)
					l << qiDeafenedSelf;
				if (p->iId >= 0 && !item->isListener)
					l << qiAuthenticated;
				if (!p->qsFriendName.isEmpty() && !item->isListener)
					l << qiFriend;
				return l;
			case Qt::AccessibleTextRole:
				if (item->isListener) {
					return tr("Channel Listener");
				}
				return Mumble::Accessibility::userToText(p);
			case Qt::AccessibleDescriptionRole:
				if (item->isListener) {
					return tr("This channel listener belongs to %1").arg(Mumble::Accessibility::userToText(p));
				}
				return Mumble::Accessibility::userToDescription(p);
			default:
				break;
		}
	} else {
		switch (role) {
			case Qt::DecorationRole:
				if (idx.column() == 0) {
					if (Global::get().uiSession && qsLinked.contains(c)) {
						if (ClientUser::get(Global::get().uiSession)->cChannel == c)
							return qiActiveChannel;
						else
							return qiLinkedChannel;
					}
					return qiChannel;
				}
				break;
			case Qt::DisplayRole:
				if (idx.column() == 0) {
					if (!Global::get().s.bShowUserCount || item->iUsers == 0)
						return c->qsName;

					return QString::fromLatin1("%1 (%2)").arg(c->qsName).arg(item->iUsers);
				}
				if (!c->qbaDescHash.isEmpty())
					l << (item->bCommentSeen ? qiCommentSeen : qiComment);

				switch (c->m_filterMode) {
					case ChannelFilterMode::HIDE:
						l << (qiFilter);
						break;
					case ChannelFilterMode::PIN:
						l << (qiPin);
						break;
					case ChannelFilterMode::NORMAL:
						// NOOP
						break;
				}

				// Show a lock icon for enter restricted channels
				if (c->hasEnterRestrictions.load()) {
					if (c->localUserCanEnter.load()) {
						l << qiLock_unlocked;
					} else {
						l << qiLock_locked;
					}
				}
				return l;
			case Qt::FontRole:
				if (Global::get().uiSession) {
					Channel *home = ClientUser::get(Global::get().uiSession)->cChannel;

					if ((c == home) || qsLinked.contains(c)) {
						QFont f = Global::get().mw->font();
						if (qsLinked.count() > 1)
							f.setItalic(!f.italic());
						if (c == home)
							f.setBold(!f.bold());
						return f;
					}
				}
				break;
			case Qt::BackgroundRole:
				if ((c->iId == 0) && Global::get().sh && Global::get().sh->isStrong()) {
					QColor qc(Qt::green);
					qc.setAlpha(32);
					return qc;
				}
				break;
			case Qt::AccessibleTextRole:
				return Mumble::Accessibility::channelToText(c);
			case Qt::AccessibleDescriptionRole:
				return Mumble::Accessibility::channelToDescription(c);
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
	ModelItem *item = static_cast< ModelItem * >(idx.internalPointer());
	ClientUser *p   = item->pUser;
	Channel *c      = item->cChan;
	int section     = idx.column();
	bool isUser     = p;

	switch (role) {
		case Qt::ToolTipRole:
			const_cast< UserModel * >(this)->uiSessionComment    = 0;
			const_cast< UserModel * >(this)->iChannelDescription = -1;
			const_cast< UserModel * >(this)->bClicked            = false;
			switch (section) {
				case 0: {
					if (isUser) {
						QString qsImage;
						if (!p->qbaTextureHash.isEmpty()) {
							if (p->qbaTexture.isEmpty()) {
								p->qbaTexture = Global::get().db->blob(p->qbaTextureHash);
								if (p->qbaTexture.isEmpty()) {
									MumbleProto::RequestBlob mprb;
									mprb.add_session_texture(p->uiSession);
									Global::get().sh->sendMessage(mprb);
								} else {
#ifdef USE_OVERLAY
									Global::get().o->verifyTexture(p);
#endif
								}
							}
							if (!p->qbaTexture.isEmpty()) {
								QBuffer qb(&p->qbaTexture);
								qb.open(QIODevice::ReadOnly);
								QImageReader qir(&qb, p->qbaTextureFormat);
								QSize sz = qir.size();
								if (sz.width() > 0) {
									qsImage = QString::fromLatin1("<img src=\"data:;base64,");
									qsImage.append(QString::fromLatin1(p->qbaTexture.toBase64().toPercentEncoding()));
									if (sz.width() > 128) {
										int targ = sz.width() / ((sz.width() + 127) / 128);
										qsImage.append(QString::fromLatin1("\" width=\"%1\" />").arg(targ));
									} else {
										qsImage.append(QString::fromLatin1("\" />"));
									}
								}
							}
						}

						if (p->qbaCommentHash.isEmpty()) {
							if (!qsImage.isEmpty())
								return qsImage;
							else
								return p->qsName;
						} else {
							if (p->qsComment.isEmpty()) {
								p->qsComment = QString::fromUtf8(Global::get().db->blob(p->qbaCommentHash));
								if (p->qsComment.isEmpty()) {
									const_cast< UserModel * >(this)->uiSessionComment = p->uiSession;

									MumbleProto::RequestBlob mprb;
									mprb.add_session_comment(p->uiSession);
									Global::get().sh->sendMessage(mprb);
									return QVariant();
								}
							}
							const_cast< UserModel * >(this)->seenComment(idx);
							QString base = Log::validHtml(p->qsComment);
							if (!qsImage.isEmpty())
								return QString::fromLatin1(
										   "<table><tr><td valign=\"top\">%1</td><td>%2</td></tr></table>")
									.arg(qsImage, base);
							return base;
						}
					} else {
						if (c->qbaDescHash.isEmpty()) {
							return c->qsName;
						} else {
							if (c->qsDesc.isEmpty()) {
								c->qsDesc = QString::fromUtf8(Global::get().db->blob(c->qbaDescHash));
								if (c->qsDesc.isEmpty()) {
									const_cast< UserModel * >(this)->iChannelDescription = static_cast< int >(c->iId);

									MumbleProto::RequestBlob mprb;
									mprb.add_channel_description(c->iId);
									Global::get().sh->sendMessage(mprb);
									return QVariant();
								}
							}

							const_cast< UserModel * >(this)->seenComment(idx);
							return Log::validHtml(c->qsDesc);
						}
					}
				} break;
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
												   "<tr><td><img src=\"skin:talking_on.svg\" height=64 /></td><td "
												   "valign=\"middle\">%2</td></tr>"
												   "<tr><td><img src=\"skin:talking_alt.svg\" height=64 /></td><td "
												   "valign=\"middle\">%3</td></tr>"
												   "<tr><td><img src=\"skin:talking_whisper.svg\" height=64 /></td><td "
												   "valign=\"middle\">%4</td></tr>"
												   "<tr><td><img src=\"skin:talking_off.svg\" height=64 /></td><td "
												   "valign=\"middle\">%5</td></tr>"
												   "<tr><td><img src=\"skin:talking_muted.svg\" height=64 /></td><td "
												   "valign=\"middle\">%6</td></tr>"
												   "<tr><td><img src=\"skin:ear.svg\" height=64 /></td><td "
												   "valign=\"middle\">%7</td></tr>"
												   "</table>")
							.arg(tr("This is a user connected to the server. The icon to the left of the user "
									"indicates whether or not they are talking:"),
								 tr("Talking to your channel."), tr("Shouting directly to your channel."),
								 tr("Whispering directly to you."), tr("Not talking."),
								 tr("Talking while being muted on your end"),
								 tr("This is a channel listener. The corresponding user hears everything you say in "
									"this channel."));
					else
						return QString::fromLatin1("%1"
												   "<table>"
												   "<tr><td><img src=\"skin:channel_active.svg\" height=64 /></td><td "
												   "valign=\"middle\">%2</td></tr>"
												   "<tr><td><img src=\"skin:channel_linked.svg\" height=64 /></td><td "
												   "valign=\"middle\">%3</td></tr>"
												   "<tr><td><img src=\"skin:channel.svg\" height=64 /></td><td "
												   "valign=\"middle\">%4</td></tr>"
												   "</table>")
							.arg(tr("This is a channel on the server. The icon indicates the state of the channel:"),
								 tr("Your current channel."),
								 tr("A channel that is linked with your channel. Linked channels can talk to each "
									"other."),
								 tr("A channel on the server that you are not linked to."));
				case 1:
					if (isUser)
						return QString::fromLatin1("%1"
												   "<table>"
												   "<tr><td><img src=\"skin:emblems/emblem-favorite.svg\" height=64 "
												   "/></td><td valign=\"middle\">%2</td></tr>"
												   "<tr><td><img src=\"skin:authenticated.svg\" height=64 /></td><td "
												   "valign=\"middle\">%3</td></tr>"
												   "<tr><td><img src=\"skin:muted_self.svg\" height=64 /></td><td "
												   "valign=\"middle\">%4</td></tr>"
												   "<tr><td><img src=\"skin:muted_server.svg\" height=64 /></td><td "
												   "valign=\"middle\">%5</td></tr>"
												   "<tr><td><img src=\"skin:muted_suppressed.svg\" height=64 "
												   "/></td><td valign=\"middle\">%6</td></tr>"
												   "<tr><td><img src=\"skin:muted_local.svg\" height=64 /></td><td "
												   "valign=\"middle\">%7</td></tr>"
												   "<tr><td><img src=\"skin:muted_pushtomute.svg\" height=64 "
												   "/></td><td valign=\"middle\">%8</td></tr>"
												   "<tr><td><img src=\"skin:deafened_self.svg\" height=64 /></td><td "
												   "valign=\"middle\">%9</td></tr>"
												   "<tr><td><img src=\"skin:deafened_server.svg\" height=64 /></td><td "
												   "valign=\"middle\">%10</td></tr>"
												   "<tr><td><img src=\"skin:comment.svg\" height=64 /></td><td "
												   "valign=\"middle\">%11</td></tr>"
												   "<tr><td><img src=\"skin:comment_seen.svg\" height=64 /></td><td "
												   "valign=\"middle\">%12</td></tr>"
												   "<tr><td><img src=\"skin:status/text-missing.svg\" height=64 "
												   "/></td><td valign=\"middle\">%13</td></tr>"
												   "</table>")
							.arg(tr("This shows the flags the user has on the server, if any:"),
								 tr("On your friend list"), tr("Authenticated user"),
								 tr("Muted (manually muted by self)"), tr("Muted (manually muted by admin)"),
								 tr("Muted (not allowed to speak in current channel)"),
								 tr("Muted (muted by you, only on your machine)"), tr("Muted (push-to-mute)"))
							.arg(tr("Deafened (by self)"), tr("Deafened (by admin)"),
								 tr("User has a new comment set (click to show)"),
								 tr("User has a comment set, which you've already seen. (click to show)"),
								 tr("Ignoring Text Messages"));
					else
						return QString::fromLatin1("%1"
												   "<table>"
												   "<tr><td><img src=\"skin:comment.svg\" height=64 /></td><td "
												   "valign=\"middle\">%10</td></tr>"
												   "<tr><td><img src=\"skin:comment_seen.svg\" height=64 /></td><td "
												   "valign=\"middle\">%11</td></tr>"
												   "<tr><td><img src=\"skin:filter.svg\" height=64 /></td><td "
												   "valign=\"middle\">%12</td></tr>"
												   "<tr><td><img src=\"skin:pin.svg\" height=64 /></td><td "
												   "valign=\"middle\">%13</td></tr>"
												   "<tr><td><img src=\"skin:lock_locked.svg\" height=64 /></td><td "
												   "valign=\"middle\">%14</td></tr>"
												   "<tr><td><img src=\"skin:lock_unlocked.svg\" height=64 /></td><td "
												   "valign=\"middle\">%15</td></tr>"
												   "</table>")
							.arg(tr("This shows the flags the channel has, if any:"),
								 tr("Channel has a new comment set (click to show)"),
								 tr("Channel has a comment set, which you've already seen. (click to show)"),
								 tr("Channel will be hidden when filtering is enabled"),
								 tr("Channel will be pinned when filtering is enabled"),
								 tr("Channel has access restrictions so that you can't enter it"),
								 tr("Channel has access restrictions but you can enter nonetheless"));
			}
			break;
	}
	return QVariant();
}

QVariant UserModel::headerData(int section, Qt::Orientation orientation, int role) const {
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

	beginInsertRows(index(item), 0, static_cast< int >(old->qlChildren.count()));

	for (int i = 0; i < old->qlChildren.count(); ++i) {
		ModelItem *o  = old->qlChildren.at(i);
		ModelItem *mi = new ModelItem(o);
		mi->parent    = item;

		item->qlChildren << mi;

		from << createIndex(i, 0, o);
		from << createIndex(i, 1, o);
		to << createIndex(i, 0, mi);
		to << createIndex(i, 1, mi);
	}

	endInsertRows();

	for (int i = 0; i < old->qlChildren.count(); ++i)
		recursiveClone(old->qlChildren.at(i), item->qlChildren.at(i), from, to);
}

ModelItem *UserModel::moveItem(ModelItem *oldparent, ModelItem *newparent, ModelItem *oldItem) {
	// Here's the idea. We insert the item, update persistent indexes, THEN remove it.

	// Get the current position of the item under its parent (aka its "row")
	auto oldrow = static_cast< int >(oldparent->qlChildren.indexOf(oldItem));

	// Get the row of the item at its new position. This depends on whether we're moving a
	// channel or a user.
	int newrow = -1;
	if (oldItem->cChan) {
		newrow = newparent->insertIndex(oldItem->cChan);
	} else {
		newrow = newparent->insertIndex(oldItem->pUser);
	}

	if ((oldparent == newparent) && (newrow == oldrow)) {
		// This is a no-op. We still claim that the data has changed in order
		// to trigger potential event handlers.
		emit dataChanged(index(oldItem), index(oldItem));
		return oldItem;
	}

	// Shallow clone. newItem is the new ModelItem that will be added to newparent
	ModelItem *newItem = new ModelItem(oldItem);

	// Store the index if it's "active".
	// The selection is stored as "from"-"to" pairs, so if we move up in the same channel,
	// we'd move only "from" and select half the channel.

	// Check whether the moved item is currently selected and if so, store it as a persistent
	// model index in active. Also clear the selection as we're going to mess with the active
	// item.
	QTreeView *v             = Global::get().mw->qtvUsers;
	QItemSelectionModel *sel = v->selectionModel();
	QPersistentModelIndex active;
	QModelIndex oindex = createIndex(oldrow, 0, oldItem);
	if (sel->isSelected(oindex) || (oindex == v->currentIndex())) {
		active = index(oldItem);
		v->clearSelection();
		v->setCurrentIndex(QModelIndex());
	}

	// Check whether the oldItem is currently expanded in order to restore the same
	// state once we have moved it.
	bool expanded = v->isExpanded(index(oldItem));

	if (newparent == oldparent) {
		// If the moving happens within the same parent, we have to watch out that we use the correct
		// row indices for our operation here.
		// As we're inserting the new item before remving the old one, newrow has to be the index
		// applicable before the removal (aka "as is" atm) whereas oldrow has to be applicable
		// after we've inserted the new item.
		if (oldrow >= newrow) {
			// The new item will be inserted above the old one. Thus we have to account for that extra
			// item in the used row index.
			oldrow++;
		} else {
			newrow++;
		}
	}

	// Insert the new item to its (new) parent
	beginInsertRows(index(newparent), newrow, newrow);
	newItem->parent = newparent;
	newparent->qlChildren.insert(newrow, newItem);

	if (oldItem->cChan) {
		// When moving a channel, we'll also have to move any sub-channels
		oldparent->cChan->removeChannel(oldItem->cChan);
		newparent->cChan->addChannel(oldItem->cChan);
	} else {
		newparent->cChan->addClientUser(oldItem->pUser);
	}

	endInsertRows();


	QModelIndexList from, to;
	from << createIndex(oldrow, 0, oldItem);
	from << createIndex(oldrow, 1, oldItem);
	to << createIndex(newrow, 0, newItem);
	to << createIndex(newrow, 1, newItem);

	// Clone all children of oldItem and attach them to newItem
	recursiveClone(oldItem, newItem, from, to);

	// Update all persistent model indices that are affected by our action here. This includes (but is in general
	// not limited to) the "active" index we potentially created above.
	changePersistentIndexList(from, to);

	// Now that we have added the new index, it is time to actually remove the old one
	beginRemoveRows(index(oldparent), oldrow, oldrow);
	oldparent->qlChildren.removeAt(oldrow);
	endRemoveRows();

	// oldItem is now longer needed as it is not present in the model anymore and all potential
	// references to it should be updated to point to the new (moved) item instead.
	// Thus we can delete it and all its children (which have been cloned and reference-updated
	// as well.
	oldItem->wipe();
	delete oldItem;

	if (active.isValid()) {
		// If the moved item has been previously selected, we restore that selection to now be the
		// new item using the "active" model index which has been updated to now point to the new
		// item.
		sel->select(active, QItemSelectionModel::SelectCurrent);
		v->setCurrentIndex(active);
	}

	if (expanded) {
		// If the old item (or rather the parent it has been living in) has been expanded,
		// restore that state for the new item.
		v->expand(index(newItem));
	}

	return newItem;
}

void UserModel::expandAll(Channel *c) {
	QStack< Channel * > chans;

	while (c) {
		chans.push(c);
		c = c->cParent;
	}
	while (!chans.isEmpty()) {
		c = chans.pop();
		Global::get().mw->qtvUsers->setExpanded(index(c), true);
	}
}

void UserModel::collapseEmpty(Channel *c) {
	while (c) {
		ModelItem *mi = ModelItem::c_qhChannels.value(c);
		if (mi->iUsers == 0)
			Global::get().mw->qtvUsers->setExpanded(index(c), false);
		else
			break;
		c = c->cParent;
	}
}

void UserModel::ensureSelfVisible() {
	if (!Global::get().uiSession)
		return;

	Global::get().mw->qtvUsers->scrollTo(index(ClientUser::get(Global::get().uiSession)));
}

void UserModel::recheckLinks() {
	if (!Global::get().uiSession)
		return;

	ClientUser *clientUser = ClientUser::get(Global::get().uiSession);
	if (!clientUser)
		return;

	bool bChanged = false;

	Channel *home = clientUser->cChannel;

	QSet< Channel * > all = home->allLinks();

	if (all == qsLinked)
		return;

	QSet< Channel * > changed = (all - qsLinked);
	changed += (qsLinked - all);

	if ((all.count() == 1) || (qsLinked.count() == 1))
		changed += home;

	qsLinked = all;

	foreach (Channel *c, changed) {
		QModelIndex idx = index(c);
		emit dataChanged(idx, idx);
		bChanged = true;
	}
	if (bChanged)
		updateOverlay();
}

ClientUser *UserModel::addUser(unsigned int id, const QString &name) {
	ClientUser *p = ClientUser::add(id, this);
	p->qsName     = name;

	ModelItem *item = new ModelItem(p);

	connect(p, SIGNAL(talkingStateChanged()), this, SLOT(userStateChanged()));
	connect(p, SIGNAL(muteDeafStateChanged()), this, SLOT(userStateChanged()));
	connect(p, SIGNAL(prioritySpeakerStateChanged()), this, SLOT(userStateChanged()));
	connect(p, SIGNAL(recordingStateChanged()), this, SLOT(userStateChanged()));
	connect(p, &ClientUser::localVolumeAdjustmentsChanged, this, &UserModel::userStateChanged);
	connect(p, &ClientUser::localNicknameChanged, this, &UserModel::userStateChanged);

	Channel *c       = Channel::get(Channel::ROOT_ID);
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

	emit userAdded(p->uiSession);

	return p;
}

void UserModel::removeUser(ClientUser *p) {
	// First remove all listener proxies this user has at the moment
	removeChannelListener(p);

	if (Global::get().uiSession && p->uiSession == Global::get().uiSession)
		Global::get().uiSession = 0;
	Channel *c       = p->cChannel;
	ModelItem *item  = ModelItem::c_qhUsers.value(p);
	ModelItem *citem = ModelItem::c_qhChannels.value(c);

	const auto row = static_cast< int >(citem->qlChildren.indexOf(item));

	beginRemoveRows(index(citem), row, row);
	c->removeUser(p);
	citem->qlChildren.removeAt(row);
	endRemoveRows();

	p->cChannel = nullptr;

	ClientUser::remove(p);
	qmHashes.remove(p->qsHash);

	while (citem) {
		citem->iUsers--;
		citem = citem->parent;
	}

	if (Global::get().s.ceExpand == Settings::ChannelsWithUsers)
		collapseEmpty(c);

	updateOverlay();

	emit userRemoved(p->uiSession);

	delete p;
	delete item;
}

void UserModel::moveUser(ClientUser *p, Channel *np) {
	Channel *oc     = p->cChannel;
	ModelItem *opi  = ModelItem::c_qhChannels.value(oc);
	ModelItem *pi   = ModelItem::c_qhChannels.value(np);
	ModelItem *item = ModelItem::c_qhUsers.value(p);

	item = moveItem(opi, pi, item);

	if (p->uiSession == Global::get().uiSession) {
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

	if (Global::get().s.ceExpand == Settings::ChannelsWithUsers) {
		expandAll(np);
		collapseEmpty(oc);
	}

	updateOverlay();
}

void UserModel::renameUser(ClientUser *p, const QString &name) {
	Channel *c = p->cChannel;
	p->qsName  = name;

	ModelItem *pi   = ModelItem::c_qhChannels.value(c);
	ModelItem *item = ModelItem::c_qhUsers.value(p);
	moveItem(pi, pi, item);

	updateOverlay();
}

void UserModel::setUserId(ClientUser *p, int id) {
	p->iId          = id;
	QModelIndex idx = index(p, 0);
	emit dataChanged(idx, idx);
}

void UserModel::setHash(ClientUser *p, const QString &hash) {
	if (!p->qsHash.isEmpty())
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
		int oldstate    = (cu->qsComment.isEmpty() && cu->qbaCommentHash.isEmpty()) ? 0 : (item->bCommentSeen ? 2 : 1);
		int newstate    = 0;

		cu->qsComment = comment;

		if (!comment.isEmpty()) {
			Global::get().db->setBlob(cu->qbaCommentHash, cu->qsComment.toUtf8());
			if (cu->uiSession == uiSessionComment) {
				uiSessionComment   = 0;
				item->bCommentSeen = false;
				if (bClicked) {
					QRect r = Global::get().mw->qtvUsers->visualRect(index(cu));
					QWhatsThis::showText(Global::get().mw->qtvUsers->viewport()->mapToGlobal(r.bottomRight()),
										 data(index(cu, 0), Qt::ToolTipRole).toString(), Global::get().mw->qtvUsers);
				} else {
					QToolTip::showText(QCursor::pos(), data(index(cu, 0), Qt::ToolTipRole).toString(),
									   Global::get().mw->qtvUsers);
				}
			} else if (cu->uiSession == ~uiSessionComment) {
				uiSessionComment = 0;
				if (cu->uiSession == Global::get().uiSession) {
					QTimer::singleShot(0, Global::get().mw, SLOT(on_qaSelfComment_triggered()));
				} else {
					Global::get().mw->cuContextUser = cu;
					QTimer::singleShot(0, Global::get().mw, SLOT(on_qaUserCommentView_triggered()));
				}
			} else {
				item->bCommentSeen = Global::get().db->seenComment(item->hash(), cu->qbaCommentHash);
				newstate           = item->bCommentSeen ? 2 : 1;
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
		int oldstate    = (cu->qsComment.isEmpty() && cu->qbaCommentHash.isEmpty()) ? 0 : (item->bCommentSeen ? 2 : 1);
		int newstate;

		cu->qsComment      = QString();
		cu->qbaCommentHash = hash;

		item->bCommentSeen = Global::get().db->seenComment(item->hash(), cu->qbaCommentHash);
		newstate           = item->bCommentSeen ? 2 : 1;

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
		int oldstate    = c->qsDesc.isEmpty() ? 0 : (item->bCommentSeen ? 2 : 1);
		int newstate    = 0;

		c->qsDesc = comment;

		if (!comment.isEmpty()) {
			Global::get().db->setBlob(c->qbaDescHash, c->qsDesc.toUtf8());

			if (c->iId == static_cast< unsigned int >(iChannelDescription)) {
				iChannelDescription = -1;
				item->bCommentSeen  = false;
				if (bClicked) {
					QRect r = Global::get().mw->qtvUsers->visualRect(index(c));
					QWhatsThis::showText(Global::get().mw->qtvUsers->viewport()->mapToGlobal(r.bottomRight()),
										 data(index(c, 0), Qt::ToolTipRole).toString(), Global::get().mw->qtvUsers);
				} else {
					QToolTip::showText(QCursor::pos(), data(index(c, 0), Qt::ToolTipRole).toString(),
									   Global::get().mw->qtvUsers);
				}
			} else {
				item->bCommentSeen = Global::get().db->seenComment(item->hash(), c->qbaDescHash);
				newstate           = item->bCommentSeen ? 2 : 1;
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
		int oldstate    = (c->qsDesc.isEmpty() && c->qbaDescHash.isEmpty()) ? 0 : (item->bCommentSeen ? 2 : 1);
		int newstate;

		c->qsDesc      = QString();
		c->qbaDescHash = hash;

		item->bCommentSeen = Global::get().db->seenComment(item->hash(), hash);
		newstate           = item->bCommentSeen ? 2 : 1;

		if (oldstate != newstate) {
			QModelIndex idx = index(c, 0);
			emit dataChanged(idx, idx);
		}
	}
}

void UserModel::seenComment(const QModelIndex &idx) {
	ModelItem *item;
	item = static_cast< ModelItem * >(idx.internalPointer());

	if (item->bCommentSeen)
		return;

	item->bCommentSeen = true;

	emit dataChanged(idx, idx);

	if (item->pUser)
		Global::get().db->setSeenComment(item->hash(), item->pUser->qbaCommentHash);
	else
		Global::get().db->setSeenComment(item->hash(), item->cChan->qbaDescHash);
}

void UserModel::renameChannel(Channel *c, const QString &name) {
	c->qsName = name;

	if (c->iId == 0) {
		QModelIndex idx = index(c);
		emit dataChanged(idx, idx);
	} else {
		Channel *pc     = c->cParent;
		ModelItem *pi   = ModelItem::c_qhChannels.value(pc);
		ModelItem *item = ModelItem::c_qhChannels.value(c);

		moveItem(pi, pi, item);
	}

	emit channelRenamed(c->iId);
}

void UserModel::repositionChannel(Channel *c, const int position) {
	c->iPosition = position;

	if (c->iId == 0) {
		QModelIndex idx = index(c);
		emit dataChanged(idx, idx);
	} else {
		Channel *pc     = c->cParent;
		ModelItem *pi   = ModelItem::c_qhChannels.value(pc);
		ModelItem *item = ModelItem::c_qhChannels.value(c);

		moveItem(pi, pi, item);
	}
}

Channel *UserModel::addChannel(unsigned int id, Channel *p, const QString &name) {
	Channel *c = Channel::add(id, name);

	if (!c)
		return nullptr;

	ModelItem *item  = new ModelItem(c);
	ModelItem *citem = ModelItem::c_qhChannels.value(p);

	item->parent = citem;

	int row = citem->insertIndex(c);

	beginInsertRows(index(citem), row, row);
	p->addChannel(c);
	citem->qlChildren.insert(row, item);
	endInsertRows();

	if (Global::get().s.ceExpand == Settings::AllChannels)
		Global::get().mw->qtvUsers->setExpanded(index(item), true);


	emit channelAdded(c->iId);

	return c;
}

void UserModel::addChannelListener(ClientUser *p, Channel *c) {
	ModelItem *item  = new ModelItem(p, true);
	ModelItem *citem = ModelItem::c_qhChannels.value(c);

	item->parent = citem;

	int row = citem->insertIndex(p, true);

	beginInsertRows(index(citem), row, row);
	citem->qlChildren.insert(row, item);
	endInsertRows();

	while (citem) {
		citem->iUsers++;
		citem = citem->parent;
	}

	updateOverlay();
}

void UserModel::removeChannelListener(const ClientUser *p, const Channel *c) {
	// The way operator[] works for a QHash is that it'll insert a default-constructed
	// object first, before returning a reference to it, in case there is no entry for
	// the provided key yet. Thus we never have to worry about explicitly adding an empty
	// list for a new user before accessing it.
	const QList< ModelItem * > &items = ModelItem::s_userProxies[p];

	if (items.isEmpty()) {
		return;
	}

	if (c) {
		ModelItem *citem = ModelItem::c_qhChannels.value(c);

		ModelItem *item = nullptr;
		for (int i = 0; i < items.size(); i++) {
			if (citem->qlChildren.contains(items[i])) {
				item = items[i];
				break;
			}
		}

		if (item) {
			removeChannelListener(item, citem);
		} else {
			qCritical("UserModel::removeChannelListener: Can't find item for provided channel");
		}
	} else {
		// remove all items
		foreach (ModelItem *currentItem, items) { removeChannelListener(currentItem); }
	}
}

bool UserModel::isChannelListener(const QModelIndex &idx) const {
	if (!idx.isValid()) {
		return false;
	}

	ModelItem *item;
	item = static_cast< ModelItem * >(idx.internalPointer());

	return item->isListener;
}

void UserModel::setSelectedChannelListener(unsigned int userSession, unsigned int channelID) {
	QModelIndex idx = channelListenerIndex(ClientUser::get(userSession), Channel::get(channelID));

	if (!idx.isValid()) {
		return;
	}

	QTreeView *v = Global::get().mw->qtvUsers;
	if (v) {
		v->setCurrentIndex(idx);
	}
}

void UserModel::removeChannelListener(ModelItem *item, ModelItem *citem) {
	if (!citem) {
		citem = item->parent;
	}

	if (!item || !citem) {
		qCritical("UserModel::removeChannelListener: Invalid state encountered");
		return;
	}
	if (!citem->qlChildren.contains(item)) {
		qCritical("UserModel::removeChannelListener: Item does not match parent");
		return;
	}

	ClientUser *p = item->pUser;
	Channel *c    = citem->cChan;

	if (!p) {
		qCritical("UserModel::removeChannelListener: Can't find associated ClientUser");
		return;
	}
	if (!c) {
		qCritical("UserModel::removeChannelListener: Can't find associated Channel");
		return;
	}

	const auto row = static_cast< int >(citem->qlChildren.indexOf(item));

	beginRemoveRows(index(citem), row, row);
	citem->qlChildren.removeAt(row);
	endRemoveRows();

	while (citem) {
		citem->iUsers--;
		citem = citem->parent;
	}

	if (Global::get().s.ceExpand == Settings::ChannelsWithUsers)
		collapseEmpty(c);

	updateOverlay();

	delete item;
}

bool UserModel::removeChannel(Channel *c, const bool onlyIfUnoccupied) {
	const ModelItem *item = ModelItem::c_qhChannels.value(c);

	if (onlyIfUnoccupied && item->iUsers != 0)
		return false; // Checks full hierarchy

	foreach (const ModelItem *i, item->qlChildren) {
		if (i->pUser) {
			if (i->isListener) {
				removeChannelListener(i->pUser, c);
			} else {
				removeUser(i->pUser);
			}
		} else {
			removeChannel(i->cChan);
		}
	}

	Channel *p = c->cParent;

	if (!p)
		return true;

	ModelItem *citem = ModelItem::c_qhChannels.value(p);

	int row = citem->rowOf(c);

	beginRemoveRows(index(citem), row, row);
	p->removeChannel(c);
	citem->qlChildren.removeAt(row);
	qsLinked.remove(c);
	endRemoveRows();

	Channel::remove(c);

	emit channelRemoved(c->iId);

	delete item;
	delete c;
	return true;
}

void UserModel::moveChannel(Channel *c, Channel *p) {
	Channel *oc     = c->cParent;
	ModelItem *opi  = ModelItem::c_qhChannels.value(c->cParent);
	ModelItem *pi   = ModelItem::c_qhChannels.value(p);
	ModelItem *item = ModelItem::c_qhChannels.value(c);
	item            = moveItem(opi, pi, item);

	while (opi) {
		opi->iUsers -= item->iUsers;
		opi = opi->parent;
	}
	while (pi) {
		pi->iUsers += item->iUsers;
		pi = pi->parent;
	}

	ensureSelfVisible();

	if (Global::get().s.ceExpand == Settings::ChannelsWithUsers) {
		collapseEmpty(oc);
	}
}

void UserModel::linkChannels(Channel *c, QList< Channel * > links) {
	foreach (Channel *l, links)
		c->link(l);
	recheckLinks();
}

void UserModel::unlinkChannels(Channel *c, QList< Channel * > links) {
	foreach (Channel *l, links)
		c->unlink(l);
	recheckLinks();
}

void UserModel::unlinkAll(Channel *c) {
	c->unlink(nullptr);
	recheckLinks();
}

void UserModel::removeAll() {
	ModelItem *item = miRoot;
	ModelItem *i;

	uiSessionComment    = 0;
	iChannelDescription = -1;
	bClicked            = false;

	// in order to avoid complications, we remove all ChannelListeners first
	foreach (i, item->qlChildren) {
		if (i->pUser && i->isListener) {
			removeChannelListener(i, item);
		}
	}

	foreach (i, item->qlChildren) {
		if (i->pUser)
			removeUser(i->pUser);
		else
			removeChannel(i->cChan);
	}

	qsLinked.clear();

	updateOverlay();
}

ClientUser *UserModel::getUser(const QModelIndex &idx) const {
	if (!idx.isValid())
		return nullptr;

	ModelItem *item;
	item = static_cast< ModelItem * >(idx.internalPointer());

	return item->pUser;
}

ClientUser *UserModel::getUser(const QString &hash) const {
	return qmHashes.value(hash);
}

ClientUser *UserModel::getSelectedUser() const {
	QModelIndex selected = getSelectedIndex();

	if (selected.isValid()) {
		ModelItem *item = static_cast< ModelItem * >(selected.internalPointer());

		return item->pUser;
	}

	return nullptr;
}

void UserModel::setSelectedUser(unsigned int session) {
	QModelIndex idx = index(ClientUser::get(session));

	if (!idx.isValid()) {
		return;
	}

	QTreeView *v = Global::get().mw->qtvUsers;
	if (v) {
		v->setCurrentIndex(idx);
	}
}

Channel *UserModel::getChannel(const QModelIndex &idx) const {
	if (!idx.isValid())
		return nullptr;

	ModelItem *item;
	item = static_cast< ModelItem * >(idx.internalPointer());

	if (item->pUser)
		if (item->parent && item->parent->cChan) {
			return item->parent->cChan;
		} else {
			// Failsafe in case the item does not have a parent
			qWarning("UserModel::getChannel encountered weird program flow - that's a bug (please report)!");
			return item->pUser->cChannel;
		}
	else
		return item->cChan;
}

Channel *UserModel::getSelectedChannel() const {
	QModelIndex selected = getSelectedIndex();

	if (selected.isValid()) {
		ModelItem *item = static_cast< ModelItem * >(selected.internalPointer());

		return item->cChan;
	}

	return nullptr;
}

void UserModel::setSelectedChannel(unsigned int id) {
	QModelIndex idx = index(Channel::get(id));

	if (!idx.isValid()) {
		return;
	}

	QTreeView *v = Global::get().mw->qtvUsers;
	if (v) {
		v->setCurrentIndex(idx);
	}
}

Channel *UserModel::getSubChannel(Channel *p, int idx) const {
	ModelItem *item = ModelItem::c_qhChannels.value(p);
	if (!item)
		return nullptr;

	foreach (ModelItem *i, item->qlChildren) {
		if (i->cChan) {
			if (idx == 0)
				return i->cChan;
			idx--;
		}
	}
	return nullptr;
}

void UserModel::userStateChanged() {
	ClientUser *user = qobject_cast< ClientUser * >(sender());

	if (!user)
		return;

	const QModelIndex idx = index(user);
	emit dataChanged(idx, idx);

	updateOverlay();
}

void UserModel::on_channelListenerLocalVolumeAdjustmentChanged(unsigned int channelID, float oldValue, float newValue) {
	Q_UNUSED(oldValue);
	Q_UNUSED(newValue);

	const QModelIndex idx = channelListenerIndex(ClientUser::get(Global::get().uiSession), Channel::get(channelID));
	emit dataChanged(idx, idx);
}

void UserModel::forceVisualUpdate(Channel *c) {
	QModelIndex idx;
	if (c) {
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

	foreach (idx, idxs) {
		ClientUser *p = getUser(idx);
		Channel *c    = getChannel(idx);
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

	if (!md->hasFormat(mimeTypes().at(0)))
		return false;

	QByteArray qba = md->data(mimeTypes().at(0));
	QDataStream ds(qba);

	bool isChannel;
	int iId                = -1;
	unsigned int uiSession = 0;
	ds >> isChannel;

	if (isChannel)
		ds >> iId;
	else
		ds >> uiSession;

	Channel *c;
	if (!p.isValid()) {
		c = Channel::get(Channel::ROOT_ID);
	} else {
		c = getChannel(p);
	}

	if (!c)
		return false;

	expandAll(c);

	if (!isChannel) {
		// User dropped somewhere
		int ret;
		switch (Global::get().s.ceUserDrag) {
			case Settings::Ask:
				ret = QMessageBox::question(Global::get().mw, QLatin1String("Mumble"),
											tr("Are you sure you want to drag this user?"), QMessageBox::Yes,
											QMessageBox::No);

				if (ret == QMessageBox::No)
					return false;
				break;
			case Settings::DoNothing:
				Global::get().l->log(
					Log::Information,
					MainWindow::tr("You have User Dragging set to \"Do Nothing\" so the user wasn't moved."));
				return false;
				break;
			case Settings::Move:
				break;
		}
		MumbleProto::UserState mpus;
		mpus.set_session(uiSession);
		mpus.set_channel_id(c->iId);
		Global::get().sh->sendMessage(mpus);
	} else if (c->iId != static_cast< unsigned int >(iId)) {
		// Channel dropped somewhere (not on itself)
		int ret;
		switch (Global::get().s.ceChannelDrag) {
			case Settings::Ask:
				ret = QMessageBox::question(Global::get().mw, QLatin1String("Mumble"),
											tr("Are you sure you want to drag this channel?"), QMessageBox::Yes,
											QMessageBox::No);

				if (ret == QMessageBox::No)
					return false;
				break;
			case Settings::DoNothing:
				Global::get().l->log(
					Log::Information,
					MainWindow::tr("You have Channel Dragging set to \"Do Nothing\" so the channel wasn't moved."));
				return false;
				break;
			case Settings::Move:
				break;
			default:
				Global::get().l->log(Log::CriticalError,
									 MainWindow::tr("Unknown Channel Drag mode in UserModel::dropMimeData."));
				return false;
				break;
		}

		long long inewpos = 0;
		Channel *dropped  = Channel::c_qhChannels.value(static_cast< unsigned int >(iId));

		if (!dropped)
			return false;

		if (p.isValid()) {
			ModelItem *pi = static_cast< ModelItem * >(p.internalPointer());
			if (pi->pUser)
				pi = pi->parent;

			int ifirst = 0;
			int ilast  = pi->rows() - 1;

			if (ilast > 0) {
				while (pi->userAt(ifirst) && ifirst < ilast)
					ifirst++;
				while (pi->userAt(ilast) && ilast > 0)
					ilast--;
			}

			if (row == -1 && column == -1) {
				// Dropped on item
				if (getUser(p)) {
					// Dropped on player
					if (ilast > 0) {
						if (pi->bUsersTop) {
							if (pi->channelAt(ifirst) == dropped || NAMECMPCHANNEL(pi->channelAt(ifirst), dropped)) {
								if (dropped->iPosition == pi->channelAt(ifirst)->iPosition)
									return true;
								inewpos = pi->channelAt(ifirst)->iPosition;
							} else {
								inewpos = static_cast< long long >(pi->channelAt(ifirst)->iPosition) - 20;
							}
						} else {
							if (dropped == pi->channelAt(ilast) || NAMECMPCHANNEL(dropped, pi->channelAt(ilast))) {
								if (pi->channelAt(ilast)->iPosition == dropped->iPosition)
									return true;
								inewpos = pi->channelAt(ilast)->iPosition;
							} else {
								inewpos = static_cast< long long >(pi->channelAt(ilast)->iPosition) + 20;
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
						if (dropped->iPosition == pi->channelAt(ifirst)->iPosition)
							return true;
						inewpos = pi->channelAt(ifirst)->iPosition;
					} else {
						inewpos = static_cast< long long >(pi->channelAt(ifirst)->iPosition) - 20;
					}
				} else if (row > ilast) {
					if (dropped == pi->channelAt(ilast) || NAMECMPCHANNEL(dropped, pi->channelAt(ilast))) {
						if (pi->channelAt(ilast)->iPosition == dropped->iPosition)
							return true;
						inewpos = pi->channelAt(ilast)->iPosition;
					} else {
						inewpos = static_cast< long long >(pi->channelAt(ilast)->iPosition) + 20;
					}
				} else {
					// Dropped between channels
					Channel *lower = pi->channelAt(row);
					Channel *upper = pi->channelAt(row - 1);

					if (lower->iPosition == upper->iPosition && NAMECMPCHANNEL(lower, dropped)
						&& NAMECMPCHANNEL(dropped, upper)) {
						inewpos = upper->iPosition;
					} else if (lower->iPosition > upper->iPosition && NAMECMPCHANNEL(lower, dropped)) {
						inewpos = lower->iPosition;
					} else if (lower->iPosition > upper->iPosition && NAMECMPCHANNEL(dropped, upper)) {
						inewpos = upper->iPosition;
					} else if (lower == dropped || upper == dropped) {
						return true;
					} else if (abs(lower->iPosition) - abs(upper->iPosition) > 1) {
						inewpos = upper->iPosition + (abs(lower->iPosition) - abs(upper->iPosition)) / 2;
					} else {
						// Not enough space, other channels have to be moved
						if (static_cast< long long >(pi->channelAt(ilast)->iPosition) + 40 > INT_MAX) {
							QMessageBox::critical(Global::get().mw, QLatin1String("Mumble"),
												  tr("Cannot perform this movement automatically, please reset the "
													 "numeric sorting indicators or adjust it manually."));
							return false;
						}
						for (int i = row; i <= ilast; i++) {
							Channel *tmp = pi->channelAt(i);
							if (tmp != dropped) {
								MumbleProto::ChannelState mpcs;
								mpcs.set_channel_id(tmp->iId);
								mpcs.set_position(tmp->iPosition + 40);
								Global::get().sh->sendMessage(mpcs);
							}
						}
						inewpos = upper->iPosition + 20;
					}
				}
			}
		}

		if (inewpos > INT_MAX || inewpos < INT_MIN) {
			QMessageBox::critical(Global::get().mw, QLatin1String("Mumble"),
								  tr("Cannot perform this movement automatically, please reset the numeric sorting "
									 "indicators or adjust it manually."));
			return false;
		}

		MumbleProto::ChannelState mpcs;
		mpcs.set_channel_id(static_cast< unsigned int >(iId));
		if (dropped->parent() != c)
			mpcs.set_parent(c->iId);
		mpcs.set_position(static_cast< int >(inewpos));
		Global::get().sh->sendMessage(mpcs);
	}

	return true;
#undef NAMECMPCHANNEL
}

void UserModel::updateOverlay() const {
#ifdef USE_OVERLAY
	Global::get().o->updateOverlay();
#endif
	Global::get().lcd->updateUserView();
}


QString UserModel::createDisplayString(const ClientUser &user, bool isChannelListener, const Channel *parentChannel) {
	// Get the configured volume adjustment. Depending on whether
	// this display string is for a ChannelListener or a regular user, we have to fetch
	// the volume adjustment differently.
	float volumeAdjustment = 1.0f;
	if (isChannelListener) {
		if (parentChannel && user.uiSession == Global::get().uiSession) {
			// Only the listener of the local user can have a volume adjustment
			volumeAdjustment =
				Global::get()
					.channelListenerManager->getListenerVolumeAdjustment(user.uiSession, parentChannel->iId)
					.factor;
		}
	} else {
		volumeAdjustment = user.getLocalVolumeAdjustments();
	}

	// Transform the adjustment into dB
	int localVolumeDecibel = VolumeAdjustment::toIntegerDBAdjustment(volumeAdjustment);

	// Create a friend-tag
	QString friendTag;
	if (!user.qsFriendName.isEmpty() && user.qsName.compare(user.qsFriendName, Qt::CaseInsensitive) != 0) {
		friendTag = QString::fromLatin1("(%2)").arg(user.qsFriendName);
	}

	// Create a nickname-tag
	QString nickname = user.getLocalNickname();

	// Create a tag that indicates the volume adjustments
	QString volumeTag;
	if (std::abs(localVolumeDecibel) > 0 && Global::get().s.bShowVolumeAdjustments) {
		volumeTag = QString::asprintf("|%+d|", localVolumeDecibel);
	}

	QString displayString;
	if (!Global::get().s.bShowNicknamesOnly || nickname.isEmpty()) {
		displayString += user.qsName;
	} else {
		displayString += nickname;
	}

	if (!friendTag.isEmpty()) {
		displayString += " " + friendTag;
	}

	if (!Global::get().s.bShowNicknamesOnly && !nickname.isEmpty()
		&& user.qsName.compare(nickname, Qt::CaseInsensitive) != 0) {
		displayString += " " + QString::fromLatin1("[%1]").arg(nickname);
	}

	if (!volumeTag.isEmpty()) {
		displayString += "   " + volumeTag;
	}

	return displayString;
}
