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

#include "PlayerModel.h"
#include "MainWindow.h"
#include "Message.h"
#include "ServerHandler.h"
#include "Channel.h"
#include "Player.h"
#include "Global.h"
#include "LCD.h"
#include "Log.h"

QHash <Channel *, ModelItem *> ModelItem::c_qhChannels;
QHash <ClientPlayer *, ModelItem *> ModelItem::c_qhPlayers;
bool ModelItem::bPlayersTop = false;

ModelItem::ModelItem(Channel *c) {
	this->cChan = c;
	this->pPlayer = NULL;
	c_qhChannels.insert(c, this);
	parent = c_qhChannels.value(c->cParent);
	iPlayers = 0;
}

ModelItem::ModelItem(ClientPlayer *p) {
	this->cChan = NULL;
	this->pPlayer = p;
	c_qhPlayers.insert(p, this);
	parent = c_qhChannels.value(p->cChannel);
	iPlayers = 0;
}

ModelItem::ModelItem(ModelItem *i) {
	// Create a shallow clone
	this->cChan = i->cChan;
	this->pPlayer = i->pPlayer;
	this->parent = i->parent;

	if (pPlayer)
		c_qhPlayers.insert(pPlayer, this);
	else if (cChan)
		c_qhChannels.insert(cChan, this);

	iPlayers = i->iPlayers;
}

ModelItem::~ModelItem() {
	Q_ASSERT(qlChildren.count() == 0);

	if (cChan && c_qhChannels.value(cChan) == this)
		c_qhChannels.remove(cChan);
	if (pPlayer && c_qhPlayers.value(pPlayer) == this)
		c_qhPlayers.remove(pPlayer);
}

void ModelItem::wipe() {
	foreach(ModelItem *i, qlChildren) {
		i->wipe();
		delete i;
	}
	qlChildren.clear();
	iPlayers = 0;
}

ModelItem *ModelItem::child(int idx) const {
	if (! validRow(idx))
		return NULL;

	return qlChildren.at(idx);
}

bool ModelItem::validRow(int idx) const {
	return ((idx >= 0) && (idx < qlChildren.count()));
}

ClientPlayer *ModelItem::playerAt(int idx) const {
	if (! validRow(idx))
		return NULL;
	return qlChildren.at(idx)->pPlayer;
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

int ModelItem::rowOf(ClientPlayer *p) const {
	for (int i=0;i<qlChildren.count();i++)
		if (qlChildren.at(i)->pPlayer == p)
			return i;
	return -1;
}

int ModelItem::rowOfSelf() const {
	// Root?
	if (! parent)
		return 0;

	if (pPlayer)
		return parent->rowOf(pPlayer);
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

	return qls.indexOf(c->qsName) + (bPlayersTop ? ocount : 0);
}

int ModelItem::insertIndex(ClientPlayer *p) const {
	QList<QString> qls;
	ModelItem *item;

	int ocount = 0;

	foreach(item, qlChildren) {
		if (item->pPlayer) {
			if (item->pPlayer != p)
				qls << item->pPlayer->qsName;
		} else
			ocount++;
	}

	qls << p->qsName;
	qSort(qls);

	return qls.indexOf(p->qsName) + (bPlayersTop ? 0 : ocount);
}

PlayerModel::PlayerModel(QObject *p) : QAbstractItemModel(p) {
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

	ModelItem::bPlayersTop = g.s.bPlayerTop;

	miRoot = new ModelItem(Channel::get(0));
}

PlayerModel::~PlayerModel() {
	removeAll();
	Q_ASSERT(ModelItem::c_qhPlayers.count() == 0);
	Q_ASSERT(ModelItem::c_qhChannels.count() == 1);
	delete miRoot;
}


int PlayerModel::columnCount(const QModelIndex &) const {
	return 2;
}

QModelIndex PlayerModel::index(int row, int column, const QModelIndex &p) const {
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

QModelIndex PlayerModel::index(ClientPlayer *p, int column) const {
	ModelItem *item = ModelItem::c_qhPlayers.value(p);
	Q_ASSERT(p);
	Q_ASSERT(item);
	if (!p || ! item)
		return QModelIndex();
	QModelIndex idx=createIndex(item->rowOfSelf(), column, item);
	return idx;
}

QModelIndex PlayerModel::index(Channel *c) const {
	ModelItem *item = ModelItem::c_qhChannels.value(c);
	Q_ASSERT(c);
	Q_ASSERT(item);
	if (!item || !c)
		return QModelIndex();
	QModelIndex idx=createIndex(item->rowOfSelf(), 0, item);
	return idx;
}

QModelIndex PlayerModel::index(ModelItem *item) const {
	return createIndex(item->rowOfSelf(), 0, item);
}

QModelIndex PlayerModel::parent(const QModelIndex &idx) const {
	if (! idx.isValid())
		return QModelIndex();

	ModelItem *item = static_cast<ModelItem *>(idx.internalPointer());

	ModelItem *pitem = (item) ? item->parent : NULL;

	if (! pitem)
		return QModelIndex();

	return createIndex(pitem->rowOfSelf(), 0, pitem);
}

int PlayerModel::rowCount(const QModelIndex &p) const {
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

QString PlayerModel::stringIndex(const QModelIndex &idx) const {
	ModelItem *item = static_cast<ModelItem *>(idx.internalPointer());
	if (!idx.isValid())
		return QLatin1String("invIdx");
	if (!item)
		return QLatin1String("invPtr");
	if (item->pPlayer)
		return QString::fromLatin1("P:%1 [%2,%3]").arg(item->pPlayer->qsName).arg(idx.row()).arg(idx.column());
	else
		return QString::fromLatin1("C:%1 [%2,%3]").arg(item->cChan->qsName).arg(idx.row()).arg(idx.column());
}

QVariant PlayerModel::data(const QModelIndex &idx, int role) const {
	if (!idx.isValid())
		return QVariant();

	ModelItem *item = static_cast<ModelItem *>(idx.internalPointer());

	Channel *c = item->cChan;
	ClientPlayer *p = item->pPlayer;

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
				if (idx.column() == 0)
					return p->qsName;
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
					Channel *home = ClientPlayer::get(g.uiSession)->cChannel;

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

Qt::ItemFlags PlayerModel::flags(const QModelIndex &idx) const {
	if (!idx.isValid())
		return Qt::ItemIsDropEnabled;

	if (idx.column() != 0)
		return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QVariant PlayerModel::otherRoles(const QModelIndex &idx, int role) const {
	ModelItem *item = static_cast<ModelItem *>(idx.internalPointer());
	ClientPlayer *p = item->pPlayer;
	Channel *c = item->cChan;
	int section = idx.column();
	bool isPlayer = p != NULL;

	switch (role) {
		case Qt::ToolTipRole:
			switch (section) {
				case 0:
					return isPlayer ? p->qsName : c->qsName;
				case 1:
					return isPlayer ? p->getFlagsString() : QVariant();
			}
			break;
		case Qt::WhatsThisRole:
			switch (section) {
				case 0:
					if (isPlayer)
						return tr("This is a player connected to the server. The icon to the left of the player indicates "
						          "whether or not they are talking:<br />"
						          "<img src=\"skin:talking_on.png\" /> Talking<br />"
						          "<img src=\"skin:talking_off.png\" /> Not talking"
						         );
					else
						return tr("This is a channel on the server. Only players in the same channel can hear each other.");
				case 1:
					return tr("This shows the flags the player has on the server, if any:<br />"
					          "<img src=\"skin:authenticated.png\" />Authenticated user<br />"
					          "<img src=\"skin:muted_self.png\" />Muted (by self)<br />"
					          "<img src=\"skin:muted_server.png\" />Muted (by admin)<br />"
					          "<img src=\"skin:deafened_self.png\" />Deafened (by self)<br />"
					          "<img src=\"skin:deafened_server.png\" />Deafened (by admin)<br />"
					          "A player muted by himself is probably just away, talking on the phone or something like that.<br />"
					          "A player muted by an admin is probably also just away, and the noise the player is making was annoying "
					          "enough that an admin muted him.");
			}
			break;
	}
	return QVariant();
}

QVariant PlayerModel::headerData(int section, Qt::Orientation orientation,
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

void PlayerModel::recursiveClone(const ModelItem *old, ModelItem *item, QModelIndexList &from, QModelIndexList &to) {
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

ModelItem *PlayerModel::moveItem(ModelItem *oldparent, ModelItem *newparent, ModelItem *item) {
	// Here's the idea. We insert the item, update persistent indexes, THEN remove it.

	int oldrow = oldparent->qlChildren.indexOf(item);
	int newrow = -1;

	if (item->cChan)
		newrow = newparent->insertIndex(item->cChan);
	else
		newrow = newparent->insertIndex(item->pPlayer);

	if ((oldparent == newparent) && (newrow == oldrow)) {
		emit dataChanged(index(item),index(item));
		return item;
	}

	// Shallow clone
	ModelItem *t = new ModelItem(item);

	// Store the index if it's "active".
	// The selection is stored as "from"-"to" pairs, so if we move up in the same channel,
	// we'd move only "from" and select half the channel.

	QTreeView *v=g.mw->qtvPlayers;
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
		newparent->cChan->addPlayer(item->pPlayer);
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

void PlayerModel::expandAll(Channel *c) {
	QStack<Channel *> chans;

	while (c) {
		chans.push(c);
		c = c->cParent;
	}
	while (! chans.isEmpty()) {
		c = chans.pop();
		g.mw->qtvPlayers->setExpanded(index(c), true);
	}
}

void PlayerModel::collapseEmpty(Channel *c) {
	while (c) {
		ModelItem *mi = ModelItem::c_qhChannels.value(c);
		if (mi->iPlayers == 0)
			g.mw->qtvPlayers->setExpanded(index(c), false);
		else
			break;
		c = c->cParent;
	}
}

void PlayerModel::ensureSelfVisible() {
	if (! g.uiSession)
		return;

	g.mw->qtvPlayers->scrollTo(index(ClientPlayer::get(g.uiSession)));
}

void PlayerModel::recheckLinks() {
	if (! g.uiSession)
		return;

	bool bChanged = false;

	Channel *home = ClientPlayer::get(g.uiSession)->cChannel;

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

ClientPlayer *PlayerModel::addPlayer(unsigned int id, const QString &name) {
	ClientPlayer *p = ClientPlayer::add(id, this);
	p->qsName = name;

	ModelItem *item = new ModelItem(p);

	connect(p, SIGNAL(talkingChanged(bool)), this, SLOT(playerTalkingChanged(bool)));
	connect(p, SIGNAL(muteDeafChanged()), this, SLOT(playerMuteDeafChanged()));

	Channel *c = Channel::get(0);
	ModelItem *citem = ModelItem::c_qhChannels.value(c);

	item->parent = citem;

	int row = citem->insertIndex(p);

	beginInsertRows(index(citem), row, row);
	citem->qlChildren.insert(row, item);
	c->addPlayer(p);
	endInsertRows();

	while (citem) {
		citem->iPlayers++;
		citem = citem->parent;
	}

	if (g.uiSession && (p->cChannel == ClientPlayer::get(g.uiSession)->cChannel))
		updateOverlay();

	return p;
}

void PlayerModel::removePlayer(ClientPlayer *p) {
	Channel *c = p->cChannel;
	ModelItem *item = ModelItem::c_qhPlayers.value(p);
	ModelItem *citem = ModelItem::c_qhChannels.value(c);

	int row = citem->qlChildren.indexOf(item);

	beginRemoveRows(index(citem), row, row);
	c->removePlayer(p);
	citem->qlChildren.removeAt(row);
	endRemoveRows();

	p->cChannel = NULL;

	ClientPlayer::remove(p);
	delete p;
	delete item;

	while (citem) {
		citem->iPlayers--;
		citem = citem->parent;
	}

	if (g.s.ceExpand == Settings::ChannelsWithPlayers)
		collapseEmpty(c);

	if (g.uiSession && (p->cChannel == ClientPlayer::get(g.uiSession)->cChannel))
		updateOverlay();
}

void PlayerModel::movePlayer(ClientPlayer *p, Channel *np) {
	Channel *oc = p->cChannel;
	ModelItem *opi = ModelItem::c_qhChannels.value(oc);
	ModelItem *pi = ModelItem::c_qhChannels.value(np);
	ModelItem *item = ModelItem::c_qhPlayers.value(p);

	item = moveItem(opi, pi, item);

	if (p->uiSession == g.uiSession) {
		ensureSelfVisible();
		recheckLinks();
	}

	while (opi) {
		opi->iPlayers--;
		opi = opi->parent;
	}
	while (pi) {
		pi->iPlayers++;
		pi = pi->parent;
	}

	if (g.s.ceExpand == Settings::ChannelsWithPlayers) {
		expandAll(np);
		collapseEmpty(oc);
	}

	if (g.uiSession) {
		Channel *home = ClientPlayer::get(g.uiSession)->cChannel;
		if (home==np || home == oc)
			updateOverlay();
	}
}

void PlayerModel::renamePlayer(ClientPlayer *p, const QString &name) {
	Channel *c = p->cChannel;
	p->qsName = name;

	ModelItem *pi = ModelItem::c_qhChannels.value(c);
	ModelItem *item = ModelItem::c_qhPlayers.value(p);
	moveItem(pi, pi, item);

	if (g.uiSession && (p->cChannel == ClientPlayer::get(g.uiSession)->cChannel))
		updateOverlay();
}

void PlayerModel::renameChannel(Channel *c, const QString &name) {
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


Channel *PlayerModel::addChannel(int id, Channel *p, const QString &name) {
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
		g.mw->qtvPlayers->setExpanded(index(item), true);

	return c;
}

void PlayerModel::removeChannel(Channel *c) {
	ModelItem *item, *i;

	item=ModelItem::c_qhChannels.value(c);

	foreach(i, item->qlChildren) {
		if (i->pPlayer)
			removePlayer(i->pPlayer);
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

void PlayerModel::moveChannel(Channel *c, Channel *p) {
	Channel *oc = c->cParent;
	ModelItem *opi = ModelItem::c_qhChannels.value(c->cParent);
	ModelItem *pi = ModelItem::c_qhChannels.value(p);
	ModelItem *item = ModelItem::c_qhChannels.value(c);
	item = moveItem(opi, pi, item);

	while (opi) {
		opi->iPlayers -= item->iPlayers;
		opi = opi->parent;
	}
	while (pi) {
		pi->iPlayers += item->iPlayers;
		pi = pi->parent;
	}

	ensureSelfVisible();

	if (g.s.ceExpand == Settings::ChannelsWithPlayers) {
		collapseEmpty(oc);
	}
}

void PlayerModel::linkChannels(Channel *c, QList<Channel *> links) {
	foreach(Channel *l, links)
		c->link(l);
	recheckLinks();
}

void PlayerModel::unlinkChannels(Channel *c, QList<Channel *> links) {
	foreach(Channel *l, links)
		c->unlink(l);
	recheckLinks();
}

void PlayerModel::unlinkAll(Channel *c) {
	c->unlink(NULL);
	recheckLinks();
}

void PlayerModel::removeAll() {
	ModelItem *item = miRoot;
	ModelItem *i;

	foreach(i, item->qlChildren) {
		if (i->pPlayer)
			removePlayer(i->pPlayer);
		else
			removeChannel(i->cChan);
	}

	qsLinked.clear();

	updateOverlay();
}

ClientPlayer *PlayerModel::getPlayer(const QModelIndex &idx) const {
	if (! idx.isValid())
		return NULL;

	ModelItem *item;
	item = static_cast<ModelItem *>(idx.internalPointer());

	return item->pPlayer;
}

Channel *PlayerModel::getChannel(const QModelIndex &idx) const {
	if (! idx.isValid())
		return NULL;

	ModelItem *item;
	item = static_cast<ModelItem *>(idx.internalPointer());

	if (item->pPlayer)
		return item->pPlayer->cChannel;
	else
		return item->cChan;
}

Channel *PlayerModel::getSubChannel(Channel *p, int idx) const {
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

void PlayerModel::playerTalkingChanged(bool) {
	ClientPlayer *p=static_cast<ClientPlayer *>(sender());
	QModelIndex idx = index(p);
	emit dataChanged(idx, idx);
	updateOverlay();
}

void PlayerModel::playerMuteDeafChanged() {
	ClientPlayer *p=static_cast<ClientPlayer *>(sender());
	QModelIndex idx = index(p, 1);
	emit dataChanged(idx, idx);
	if (g.uiSession && (p->cChannel == ClientPlayer::get(g.uiSession)->cChannel))
		updateOverlay();
}

Qt::DropActions PlayerModel::supportedDropActions() const {
	return Qt::CopyAction;
}

QStringList PlayerModel::mimeTypes() const {
	QStringList sl;
	sl << QLatin1String("mumble/dragentry");
	return sl;
}

QMimeData *PlayerModel::mimeData(const QModelIndexList &idxs) const {
	QModelIndex idx;
	QByteArray qba;
	QDataStream ds(&qba, QIODevice::WriteOnly);

	foreach(idx, idxs) {
		ClientPlayer *p = getPlayer(idx);
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

bool PlayerModel::dropMimeData(const QMimeData *md, Qt::DropAction, int, int, const QModelIndex &p) {
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
		MessagePlayerMove mpm;
		mpm.uiVictim = uiSession;
		mpm.iChannelId = c->iId;
		g.sh->sendMessage(&mpm);
	} else {
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
				g.l->log(Log::CriticalError, MainWindow::tr("Unknown Channel Drag mode in PlayerModel::dropMimeData."));
				break;
		}
		MessageChannelMove mcm;
		mcm.iId = iId;
		mcm.iParent = c->iId;
		g.sh->sendMessage(&mcm);
	}

	return true;
}

void PlayerModel::updateOverlay() const {
	g.lcd->updatePlayerView();
}

PlayerDelegate::PlayerDelegate(QObject *p) : QItemDelegate(p) {
}

QSize PlayerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
	if (index.column() == 1) {
		const QAbstractItemModel *m = index.model();
		QVariant data = m->data(index);
		QList<QVariant> ql = data.toList();
		return QSize(18 * ql.count(), 18);
	} else {
		return QItemDelegate::sizeHint(option, index);
	}
}

void PlayerDelegate::paint(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
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
