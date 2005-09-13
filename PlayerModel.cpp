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
#include "PlayerModel.h"
#include "MainWindow.h"
#include "Global.h"

PlayerModel::PlayerModel(QObject *p) : QAbstractItemModel(p) {
	qiTalkingOn=QIcon(":/icons/talking_on.png");
	qiTalkingOff=QIcon(":/icons/talking_off.png");
	qiMutedSelf=QIcon(":/icons/muted_self.png");
	qiMutedServer=QIcon(":/icons/muted_server.png");
	qiDeafenedSelf=QIcon(":/icons/deafened_self.png");
	qiDeafenedServer=QIcon(":/icons/deafened_server.png");
}

PlayerModel::~PlayerModel() {
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
	if (row >= qlPlayers.count())
		return QVariant();

	Player *p = qlPlayers[row];

	if ((role == Qt::DecorationRole) && (idx.column() == 0))
		return (p->bTalking) ? qiTalkingOn : qiTalkingOff;

	if ((role == Qt::FontRole) && (idx.column() == 0) && (p->sId == g.mw->sMyId)) {
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
    if (!idx.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
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
    return createIndex(row, column);
}

QModelIndex PlayerModel::parent(const QModelIndex &idx) const
{
    return QModelIndex();
}

int PlayerModel::rowCount(const QModelIndex &p) const
{
    if (!p.isValid())
    	return qlPlayers.count();
    else
    	return 0;
}

Player *PlayerModel::addPlayer(short id, QString name) {
	QStringList names;
	foreach(Player *p, qlPlayers) {
		names << p->qsName;
	}
	names << name;
	qSort(names);

	int rows = names.indexOf(name);

	beginInsertRows(QModelIndex(), rows, rows);
	Player *p = Player::add(id, this);
	p->qsName = name;
	qlPlayers.insert(rows, p);
	endInsertRows();

	connect(p, SIGNAL(talkingChanged(bool)), this, SLOT(playerTalkingChanged(bool)));
	connect(p, SIGNAL(muteDeafChanged()), this, SLOT(playerMuteDeafChanged()));

	return p;
}

void PlayerModel::removePlayer(Player *p) {
	int row = qlPlayers.indexOf(p);
	beginRemoveRows(QModelIndex(), row, row);
	Player::remove(p);
	qlPlayers.removeAt(row);
	delete p;
	endRemoveRows();
}

void PlayerModel::removeAllPlayers() {
	while (qlPlayers.count() > 0)
		removePlayer(qlPlayers[0]);
}

Player *PlayerModel::getPlayer(const QModelIndex &idx) const
{
	if (! idx.isValid())
		return NULL;
	if (idx.row() > rowCount(idx.parent()))
		return NULL;
	return qlPlayers[idx.row()];
}

void PlayerModel::playerTalkingChanged(bool bTalking)
{
	Player *p=static_cast<Player *>(sender());
	int row = qlPlayers.indexOf(p);
	QModelIndex idx = createIndex(row, 0);
	emit dataChanged(idx, idx);
}

void PlayerModel::playerMuteDeafChanged()
{
	Player *p=static_cast<Player *>(sender());
	int row = qlPlayers.indexOf(p);
	QModelIndex idx = createIndex(row, 1);
	emit dataChanged(idx, idx);
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
