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

#include <QPainter>
#include <QMimeData>
#include "PlayerModel.h"
#include "MainWindow.h"
#include "Message.h"
#include "ServerHandler.h"
#include "Global.h"

ChannelItem::ChannelItem(ChannelItem *p, Channel *cptr) {
	ciParent = p;
	c = cptr;
}

void ChannelItem::dump() {
	Channel *subc;
	qWarning("ChannelItem %p", this);
	qWarning("Parent %p", ciParent);
	qWarning("Channel %p (%s)", c, c->qsName.toLatin1().constData());
	qWarning("Channels: %d", qlChannels.count());
	qWarning("Players: %d", qlPlayers.count());
	foreach (subc, qlChannels)
		qWarning("SubChannel %d", subc->iId);
	qWarning("--");
	foreach (subc, qlChannels)
		g.mw->pmModel->qhChannelItems[subc]->dump();
}

PlayerModel::PlayerModel(QObject *p) : QAbstractItemModel(p) {
	qiTalkingOn=QIcon(":/icons/talking_on.png");
	qiTalkingOff=QIcon(":/icons/talking_off.png");
	qiMutedSelf=QIcon(":/icons/muted_self.png");
	qiMutedServer=QIcon(":/icons/muted_server.png");
	qiDeafenedSelf=QIcon(":/icons/deafened_self.png");
	qiDeafenedServer=QIcon(":/icons/deafened_server.png");
	qiAuthenticated=QIcon(":/icons/authenticated.png");

	Channel *c = Channel::get(0);
	ciRoot = new ChannelItem(NULL, c);
	qhChannelItems[c] = ciRoot;
}

PlayerModel::~PlayerModel() {
	removeAll();
}


int PlayerModel::columnCount(const QModelIndex &) const
{
	return 2;
}

QVariant PlayerModel::data(const QModelIndex &idx, int role) const
{
    if (!idx.isValid())
        return QVariant();

	QVariant v = otherRoles(idx.column(), role);
	if (v.isValid())
		return v;

	int row = idx.row();

	ChannelItem *item = static_cast<ChannelItem *>(idx.internalPointer());

	if (row < item->qlChannels.count()) {
	    if (role != Qt::DisplayRole)
	        return QVariant();
	    return item->qlChannels[row]->qsName;
	} else {
		row -= item->qlChannels.count();
	}

	if (row >= item->qlPlayers.count())
		return QVariant();

	Player *p = item->qlPlayers[row];

	if ((role == Qt::DecorationRole) && (idx.column() == 0))
		return (p->bTalking) ? qiTalkingOn : qiTalkingOff;

	if ((role == Qt::FontRole) && (idx.column() == 0) && (p->sId == g.sId)) {
		QFont f = g.mw->font();
		f.setBold(true);
		return f;
	}

    if (role != Qt::DisplayRole)
        return QVariant();

	switch(idx.column()) {
		case 0:
			return p->qsName;
		case 1:
			QList<QVariant> l;
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
			return l;
	}
	return QVariant();
}

Qt::ItemFlags PlayerModel::flags(const QModelIndex &idx) const
{
    if (!idx.isValid() || idx.column() != 0)
        return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QVariant PlayerModel::otherRoles(int section, int role) const
{
	switch(role) {
		case Qt::ToolTipRole:
			switch(section) {
				case 0:
					return tr("Name of player");
				case 1:
					return tr("Player flags");
			}
			break;
		case Qt::WhatsThisRole:
			switch(section) {
				case 0:
					return tr("This list shows all the players connected to the server. The icon to the left of the player indicates "
						"whether or not they are talking:<br />"
						"<img src=\":/icons/talking_on.png\" /> Talking<br />"
						"<img src=\":/icons/talking_off.png\" /> Not talking"
						);
				case 1:
					return tr("This shows the flags the player has on the server, if any:<br />"
								"<img src=\":/icons/authenticated.png\" />Authenticated user<br />"
								"<img src=\":/icons/muted_self.png\" />Muted (by self)<br />"
								"<img src=\":/icons/muted_server.png\" />Muted (by admin)<br />"
								"<img src=\":/icons/deafened_self.png\" />Deafened (by self)<br />"
								"<img src=\":/icons/deafened_server.png\" />Deafened (by admin)<br />"
								"A player muted by himself is probably just away, talking on the phone or something like that.<br />"
								"A player muted by an admin is probably also just away, and the noise the player is making was annoying "
								"enough that an admin muted him.");
			}
			break;
	}
	return QVariant();
}

QVariant PlayerModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
	if (orientation != Qt::Horizontal)
		return QVariant();

	QVariant v = otherRoles(section, role);

	if (v.isValid())
		return v;

	switch(role) {
		case Qt::DisplayRole:
			switch(section) {
				case 0:
					return QString("Name");
				case 1:
					return QString("Flags");
			}
	}

    return QVariant();
}

QModelIndex PlayerModel::index(int row, int column, const QModelIndex &p) const
{
	ChannelItem *item;
	if ( ! p.isValid()) {
		item = ciRoot;
	} else {
        item = static_cast<ChannelItem *>(p.internalPointer());
        if (p.row() < 0 || p.row() >= item->qlChannels.count()) {
        	return QModelIndex();
		}
        item = qhChannelItems.value(item->qlChannels[p.row()]);
	}
	if (row >= (item->qlPlayers.count() + item->qlChannels.count()))
		return QModelIndex();

    return createIndex(row, column, item);
}

QModelIndex PlayerModel::index(Player *p, int column) const
{
	ChannelItem *item = qhChannelItems.value(p->cChannel);
	return createIndex(item->qlChannels.count() + item->qlPlayers.indexOf(p), column, item);
}

QModelIndex PlayerModel::index(Channel *c) const
{
	ChannelItem *item = qhChannelItems.value(c->cParent);
	if (! item)
		return QModelIndex();

	return createIndex(item->qlChannels.indexOf(c), 0, item);
}

QModelIndex PlayerModel::parentIndex(ChannelItem *ci) const
{
	return index(ci->c);
}

QModelIndex PlayerModel::parent(const QModelIndex &idx) const
{
	if (! idx.isValid())
	    return QModelIndex();

    ChannelItem *item = static_cast<ChannelItem *>(idx.internalPointer());

    if (! item || ! item->ciParent)
    	return QModelIndex();

    if (idx.row() >= (item->qlPlayers.count() + item->qlChannels.count()))
    	return QModelIndex();

    return createIndex(item->ciParent->qlChannels.indexOf(item->c), 0, item->ciParent);
}


int PlayerModel::rowCount(const QModelIndex &p) const
{
    ChannelItem *item;

    if (!p.isValid())
        item = ciRoot;
    else
        item = static_cast<ChannelItem *>(p.internalPointer());

	if (p.row() == -1) {
		// Catch when it's asking for "this" item
		return (item->qlPlayers.count() + item->qlChannels.count());
	}

	if (p.row() >= item->qlChannels.count()) {
		return 0;
	} else {
		item = qhChannelItems.value(item->qlChannels[p.row()]);
		return (item->qlPlayers.count() + item->qlChannels.count());
	}
}

void PlayerModel::hidePlayer(Player *p) {
	Channel *c = p->cChannel;
	ChannelItem *item = qhChannelItems.value(c);

	int row = item->qlPlayers.indexOf(p);
	int rowidx = row + item->qlChannels.count();
	beginRemoveRows(parentIndex(item), rowidx, rowidx);
	c->removePlayer(p);
	item->qlPlayers.removeAt(row);
	endRemoveRows();

	p->cChannel = NULL;
}

void PlayerModel::showPlayer(Player *p, Channel *c) {
	ChannelItem *item = qhChannelItems.value(c);

	QStringList names;
	foreach(Player *chanp, item->qlPlayers) {
		names << chanp->qsName;
	}
	names << p->qsName;
	qSort(names);

	int rows = names.indexOf(p->qsName);
	int rowidx = rows + item->qlChannels.count();

	beginInsertRows(parentIndex(item), rowidx, rowidx);
	c->addPlayer(p);
	item->qlPlayers.insert(rows, p);
	endInsertRows();
}

Player *PlayerModel::addPlayer(short id, QString name) {
	Player *p = Player::add(id, this);
	p->qsName = name;

	connect(p, SIGNAL(talkingChanged(bool)), this, SLOT(playerTalkingChanged(bool)));
	connect(p, SIGNAL(muteDeafChanged()), this, SLOT(playerMuteDeafChanged()));

	showPlayer(p, Channel::get(0));

	return p;
}

void PlayerModel::removePlayer(Player *p) {
	Channel *c = p->cChannel;
	ChannelItem *item = qhChannelItems.value(c);

	int row = item->qlPlayers.indexOf(p);
	int rowidx = row + item->qlChannels.count();
	beginRemoveRows(parentIndex(item), rowidx, rowidx);
	c->removePlayer(p);
	Player::remove(p);
	item->qlPlayers.removeAt(row);
	delete p;
	endRemoveRows();
}

void PlayerModel::movePlayer(Player *p, int id) {
	Channel *np = Channel::get(id);
	hidePlayer(p);
	showPlayer(p, np);
}

void PlayerModel::showChannel(Channel *c, Channel *p) {
	ChannelItem *pitem = p ? qhChannelItems.value(p) : ciRoot;
	ChannelItem *item = qhChannelItems.value(c);

	QStringList names;
	foreach(Channel *subc, pitem->qlChannels) {
		names << subc->qsName;
	}
	names << c->qsName;
	qSort(names);

	int rows = names.indexOf(c->qsName);

	QModelIndex pidx = index(p);
	beginInsertRows(pidx, rows, rows);
	pitem->qlChannels.insert(rows, c);
	item->ciParent = pitem;
	p->addChannel(c);
	endInsertRows();
}

void PlayerModel::hideChannel(Channel *c) {
	ChannelItem *myitem, *pitem;
	Channel *p;

	p = Channel::get(c->iParent);

	myitem=qhChannelItems.value(c);
	pitem=qhChannelItems.value(p);

	int row = pitem->qlChannels.indexOf(c);
	beginRemoveRows(parentIndex(myitem), row, row);
	pitem->qlChannels.removeAt(row);
	p->removeChannel(c);
	myitem->ciParent = NULL;
	endRemoveRows();
}

Channel *PlayerModel::addChannel(int id, Channel *p, QString name) {
	Channel *c = Channel::add(id, name, p);
	ChannelItem *ci = new ChannelItem(NULL, c);
	qhChannelItems[c] = ci;

	showChannel(c, p);
	return c;
}

void PlayerModel::removeChannel(Channel *c) {
	ChannelItem *myitem;
	Player *pl;
	Channel *subc;

	myitem=qhChannelItems.value(c);

	foreach(subc, myitem->qlChannels)
		removeChannel(subc);

	foreach(pl, myitem->qlPlayers)
		removePlayer(pl);

	hideChannel(c);
	Channel::remove(c);

	delete myitem;
	delete c;
	qhChannelItems.remove(c);
}

void PlayerModel::moveChannel(Channel *c, int id) {
	Channel *np = Channel::get(id);
	hideChannel(c);
	showChannel(c, np);
}

void PlayerModel::removeAll() {
	ChannelItem *item = ciRoot;
	Player *p;

	while (item->qlChannels.count() > 0)
		removeChannel(item->qlChannels[0]);

	foreach(p, item->qlPlayers)
		removePlayer(p);
}

Player *PlayerModel::getPlayer(const QModelIndex &idx) const
{
	if (! idx.isValid())
		return NULL;

    ChannelItem *item;
    item = static_cast<ChannelItem *>(idx.internalPointer());

	int row = idx.row();

	if (row < item->qlChannels.count())
		return NULL;

	row-=item->qlChannels.count();
	return item->qlPlayers[row];
}

Channel *PlayerModel::getChannel(const QModelIndex &idx) const
{
	if (! idx.isValid())
		return NULL;

    ChannelItem *item;
    item = static_cast<ChannelItem *>(idx.internalPointer());

	int row = idx.row();

	if (row < item->qlChannels.count())
		return item->qlChannels[row];

	return item->c;
}

void PlayerModel::playerTalkingChanged(bool bTalking)
{
	Player *p=static_cast<Player *>(sender());
	QModelIndex idx = index(p);
	emit dataChanged(idx, idx);
}

void PlayerModel::playerMuteDeafChanged()
{
	Player *p=static_cast<Player *>(sender());
	QModelIndex idx = index(p, 1);
	emit dataChanged(idx, idx);
}

Qt::DropActions PlayerModel::supportedDropActions() const {
	return Qt::CopyAction;
}

QStringList PlayerModel::mimeTypes() const {
	QStringList sl;
	sl << "mumble/dragentry";
	return sl;
}

QMimeData *PlayerModel::mimeData(const QModelIndexList &idxs) const {
	QModelIndex idx;
	QByteArray qba;
	QDataStream ds(&qba, QIODevice::WriteOnly);

	foreach(idx, idxs) {
		Player *p = getPlayer(idx);
		Channel *c = getChannel(idx);
		if (p) {
			ds << false;
			ds << p->sId;
		} else {
			ds << true;
			ds << c->iId;
		}
	}
	QMimeData *md = new QMimeData();
	md->setData("mumble/dragentry", qba);
	return md;
}

bool PlayerModel::dropMimeData (const QMimeData *md, Qt::DropAction action, int row, int column, const QModelIndex &p) {
	if (! md->hasFormat(mimeTypes().at(0)))
		return false;

	QByteArray qba = md->data(mimeTypes().at(0));
	QDataStream ds(qba);

	bool isChannel;
	int iId = -1;
	short sId = -1;
	ds >> isChannel;

	if (isChannel)
		ds >> iId;
	else
		ds >> sId;

	Channel *c;
	if ( ! p.isValid()) {
		c = Channel::get(0);
	} else {
		ChannelItem *item;
        item = static_cast<ChannelItem *>(p.internalPointer());
        if (p.row() >= 0 && p.row() < item->qlChannels.count())
        	c = item->qlChannels[p.row()];
        else
        	c = item->c;
	}

	if (! isChannel) {
		MessagePlayerMove mpm;
		mpm.sVictim = sId;
		mpm.iChannelId = c->iId;
		g.sh->sendMessage(&mpm);
	} else {
		MessageChannelMove mcm;
		mcm.iId = iId;
		mcm.iParent = c->iId;
		g.sh->sendMessage(&mcm);
	}

	return true;
}

PlayerDelegate::PlayerDelegate(QObject *p) : QItemDelegate(p) {
}

QSize PlayerDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index) const {
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
		for(int i=0;i<ql.size();i++) {
			QRect r = option.rect;
			r.setSize(QSize(16,16));
			r.translate(i*18+1,1);
			drawDecoration(painter, option, r, qvariant_cast<QIcon>(ql[i]).pixmap(QSize(16,16)));
		}
		painter->restore();
		return;
	} else {
		QItemDelegate::paint(painter,option,index);
	}
}
