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

#include "UserModel.h"
#include "MainWindow.h"
#include "Message.h"
#include "ServerHandler.h"
#include "Channel.h"
#include "User.h"
#include "Global.h"
#include "Overlay.h"
#include "LCD.h"
#include "Log.h"

QHash <Channel *, ModelItem *> ModelItem::c_qhChannels;
QHash <ClientUser *, ModelItem *> ModelItem::c_qhUsers;
bool ModelItem::bUsersTop = false;

ModelItem::ModelItem(Channel *c) {
	this->cChan = c;
	this->pUser = NULL;
	c_qhChannels.insert(c, this);
	parent = c_qhChannels.value(c->cParent);
	iUsers = 0;
}

ModelItem::ModelItem(ClientUser *p) {
	this->cChan = NULL;
	this->pUser = p;
	c_qhUsers.insert(p, this);
	parent = c_qhChannels.value(p->cChannel);
	iUsers = 0;
}

ModelItem::ModelItem(ModelItem *i) {
	// Create a shallow clone
	this->cChan = i->cChan;
	this->pUser = i->pUser;
	this->parent = i->parent;

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
	QList<QString> qls;
	ModelItem *item;

	int ocount = 0;

	foreach(item, qlChildren) {
		if (item->cChan) {
			if (item->cChan != c)
				qls << item->cChan->qsName;
		} else
			ocount++;
	}
	qls << c->qsName;
	qSort(qls);

	return qls.indexOf(c->qsName) + (bUsersTop ? ocount : 0);
}

int ModelItem::insertIndex(ClientUser *p) const {
	QList<QString> qls;
	ModelItem *item;

	int ocount = 0;

	foreach(item, qlChildren) {
		if (item->pUser) {
			if (item->pUser != p)
				qls << item->pUser->qsName;
		} else
			ocount++;
	}

	qls << p->qsName;
	qSort(qls);

	return qls.indexOf(p->qsName) + (bUsersTop ? 0 : ocount);
}

UserModel::UserModel(QObject *p) : QAbstractItemModel(p) {
	qiTalkingOn=QIcon(QLatin1String("skin:talking_on.png"));
	qiTalkingAlt=QIcon(QLatin1String("skin:talking_alt.png"));
	qiTalkingOff=QIcon(QLatin1String("skin:talking_off.png"));
	qiMutedSelf=QIcon(QLatin1String("skin:muted_self.png"));
	qiMutedServer=QIcon(QLatin1String("skin:muted_server.png"));
	qiMutedLocal=QIcon(QLatin1String("skin:muted_local.png"));
	qiDeafenedSelf=QIcon(QLatin1String("skin:deafened_self.png"));
	qiDeafenedServer=QIcon(QLatin1String("skin:deafened_server.png"));
	qiAuthenticated=QIcon(QLatin1String("skin:authenticated.png"));
	qiChannel=QIcon(QLatin1String("skin:channel.png"));
	qiLinkedChannel=QIcon(QLatin1String("skin:channel_linked.png"));
	qiFriend=QIcon(QLatin1String(":/emblems/emblem-favorite.svg"));

	ModelItem::bUsersTop = g.s.bUserTop;

	miRoot = new ModelItem(Channel::get(0));
}

UserModel::~UserModel() {
	removeAll();
	Q_ASSERT(ModelItem::c_qhUsers.count() == 0);
	Q_ASSERT(ModelItem::c_qhChannels.count() == 1);
	delete miRoot;
}


int UserModel::columnCount(const QModelIndex &) const {
	return 2;
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

QModelIndex UserModel::index(Channel *c) const {
	ModelItem *item = ModelItem::c_qhChannels.value(c);
	Q_ASSERT(c);
	Q_ASSERT(item);
	if (!item || !c)
		return QModelIndex();
	QModelIndex idx=createIndex(item->rowOfSelf(), 0, item);
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
					return (p->bTalking) ? (p->bAltSpeak ? qiTalkingAlt : qiTalkingOn) : qiTalkingOff;
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
				if (! p->qsFriendName.isEmpty())
					l << qiFriend;
				if (p->iId >= 0)
					l << qiAuthenticated;
				if (p->bMute)
					l << qiMutedServer;
				if (p->bDeaf)
					l << qiDeafenedServer;
				if (p->bSelfMute)
					l << qiMutedSelf;
				if (p->bSelfDeaf)
					l << qiDeafenedSelf;
				if (p->bLocalMute)
					l << qiMutedLocal;
				return l;
			default:
				break;
		}
	} else {
		switch (role) {
			case Qt::DecorationRole:
				if (idx.column() == 0) {
					return qsLinked.contains(c) ? qiLinkedChannel : qiChannel;
				}
			case Qt::DisplayRole:
				if (idx.column() == 0)
					return c->qsName;
				else
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
			switch (section) {
				case 0: {
						if (isUser) {
							if (p->qsComment.isEmpty())
								return p->qsName;
							else
								return p->qsComment;
						} else {
							if (c->qsDesc.isEmpty())
								return c->qsName;
							else
								return c->qsDesc;
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
						return tr("This is a user connected to the server. The icon to the left of the user indicates "
						          "whether or not they are talking:<br />"
						          "<img src=\"skin:talking_on.png\" /> Talking<br />"
						          "<img src=\"skin:talking_off.png\" /> Not talking"
						         );
					else
						return tr("This is a channel on the server. Only users in the same channel can hear each other.");
				case 1:
					return tr("This shows the flags the user has on the server, if any:<br />"
					          "<img src=\":/emblems/emblem-favorite.svg\" />On your friend list<br />"
					          "<img src=\"skin:authenticated.png\" />Authenticated user<br />"
					          "<img src=\"skin:muted_self.png\" />Muted (by self)<br />"
					          "<img src=\"skin:muted_server.png\" />Muted (by admin)<br />"
					          "<img src=\"skin:deafened_self.png\" />Deafened (by self)<br />"
					          "<img src=\"skin:deafened_server.png\" />Deafened (by admin)<br />"
					          "A user muted by himself is probably just away, talking on the phone or something like that.<br />"
					          "A user muted by an admin is probably also just away, and the noise the user is making was annoying "
					          "enough that an admin muted him.");
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

	connect(p, SIGNAL(talkingChanged(bool)), this, SLOT(userTalkingChanged(bool)));
	connect(p, SIGNAL(muteDeafChanged()), this, SLOT(userMuteDeafChanged()));

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

	if (g.uiSession && (p->cChannel == ClientUser::get(g.uiSession)->cChannel))
		updateOverlay();

	return p;
}

void UserModel::removeUser(ClientUser *p) {
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
	delete p;
	delete item;

	while (citem) {
		citem->iUsers--;
		citem = citem->parent;
	}

	if (g.s.ceExpand == Settings::ChannelsWithUsers)
		collapseEmpty(c);

	if (g.uiSession && (p->cChannel == ClientUser::get(g.uiSession)->cChannel))
		updateOverlay();
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

	if (g.uiSession) {
		Channel *home = ClientUser::get(g.uiSession)->cChannel;
		if (home==np || home == oc)
			updateOverlay();
	}
}

void UserModel::renameUser(ClientUser *p, const QString &name) {
	Channel *c = p->cChannel;
	p->qsName = name;

	ModelItem *pi = ModelItem::c_qhChannels.value(c);
	ModelItem *item = ModelItem::c_qhUsers.value(p);
	moveItem(pi, pi, item);

	if (g.uiSession && (p->cChannel == ClientUser::get(g.uiSession)->cChannel))
		updateOverlay();
}

void UserModel::setUserId(ClientUser *p, int id) {
	p->iId = id;
	QModelIndex idx = index(p, 1);
	emit dataChanged(idx, idx);
}

void UserModel::setFriendName(ClientUser *p, const QString &name) {
	p->qsFriendName = name;
	QModelIndex idx_a = index(p, 0);
	QModelIndex idx_b = index(p, 1);
	emit dataChanged(idx_a, idx_b);
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

void UserModel::userTalkingChanged(bool) {
	ClientUser *p=static_cast<ClientUser *>(sender());
	QModelIndex idx = index(p);
	emit dataChanged(idx, idx);
	updateOverlay();
}

void UserModel::userMuteDeafChanged() {
	ClientUser *p=static_cast<ClientUser *>(sender());
	QModelIndex idx = index(p, 1);
	emit dataChanged(idx, idx);
	if (g.uiSession && (p->cChannel == ClientUser::get(g.uiSession)->cChannel))
		updateOverlay();
}

Qt::DropActions UserModel::supportedDropActions() const {
	return Qt::CopyAction;
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

bool UserModel::dropMimeData(const QMimeData *md, Qt::DropAction, int, int, const QModelIndex &p) {
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
		MumbleProto::UserState mpus;
		mpus.set_session(uiSession);
		mpus.set_channel_id(c->iId);
		g.sh->sendMessage(mpus);
	} else if (c->iId != iId) {
		int ret;
		switch (g.s.ceChannelDrag) {
			case Settings::Ask:
				ret=QMessageBox::question(g.mw, tr("Mumble"), tr("Are you sure you want to drag this channel?"), QMessageBox::Yes, QMessageBox::No);

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
				break;
		}
		MumbleProto::ChannelState mpcs;
		mpcs.set_channel_id(iId);
		mpcs.set_parent(c->iId);
		g.sh->sendMessage(mpcs);
	}

	return true;
}

void UserModel::updateOverlay() const {
	g.o->updateOverlay();
	g.lcd->updateUserView();
}

UserDelegate::UserDelegate(QObject *p) : QItemDelegate(p) {
}

QSize UserDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
	if (index.column() == 1) {
		const QAbstractItemModel *m = index.model();
		QVariant data = m->data(index);
		QList<QVariant> ql = data.toList();
		return QSize(18 * ql.count(), 18);
	} else {
		return QItemDelegate::sizeHint(option, index);
	}
}

void UserDelegate::paint(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	if (index.column() == 1) {
		const QAbstractItemModel *m = index.model();
		QVariant data = m->data(index);
		QList<QVariant> ql = data.toList();

		painter->save();
		for (int i=0;i<ql.size();i++) {
			QRect r = option.rect;
			r.setSize(QSize(16,16));
			r.translate(i*18+1,1);
			QPixmap pixmap= (qvariant_cast<QIcon>(ql[i]).pixmap(QSize(16,16)));
			QPoint p = QStyle::alignedRect(option.direction, option.decorationAlignment, pixmap.size(), r).topLeft();
			painter->drawPixmap(p, pixmap);
		}
		painter->restore();
		return;
	} else {
		QItemDelegate::paint(painter,option,index);
	}
}
