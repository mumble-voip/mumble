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
#include "UserView.h"
#include "MainWindow.h"
#include "ClientUser.h"
#include "Channel.h"
#include "ServerHandler.h"
#include "Log.h"
#include "Global.h"

/*!
  \fn bool UserView::event(QEvent *evt)
  This implementation contains a special handler to display
  custom what's this entries for items. All other events are
  passed on.
*/

/*!
  \fn void UserView::mouseReleaseEvent(QMouseEvent *evt)
  This function is used to create custom behaviour when clicking
  on user/channel flags (e.g. showing the comment)
*/

/*!
  \fn void UserView::keyboardSearch(const QString &search)
  This implementation provides a recursive realtime search over
  the whole channel tree. It also features delayed selection
  with with automatic expanding of folded channels.
*/

UserDelegate::UserDelegate(QObject *p) : QStyledItemDelegate(p) {
}

QSize UserDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
	if (index.column() == 1) {
		const QAbstractItemModel *m = index.model();
		QVariant data = m->data(index);
		QList<QVariant> ql = data.toList();
		return QSize(18 * ql.count(), 18);
	} else {
		return QStyledItemDelegate::sizeHint(option, index);
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
		QStyledItemDelegate::paint(painter,option,index);
	}
}

UserView::UserView(QWidget *p) : QTreeView(p) {
	setItemDelegate(new UserDelegate(this));

	qtSearch = new QTimer(this);
	qtSearch->setInterval(QApplication::keyboardInputInterval());
	qtSearch->setSingleShot(true);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(contextMenu(const QPoint &)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClick(const QModelIndex &)));
	connect(qtSearch, SIGNAL(timeout()), this, SLOT(selectSearchResult()));
}

bool UserView::event(QEvent *evt) {
	if (evt->type() == QEvent::WhatsThisClicked) {
		QWhatsThisClickedEvent *qwtce = static_cast<QWhatsThisClickedEvent *>(evt);
		QDesktopServices::openUrl(qwtce->href());
	}
	return QTreeView::event(evt);
}

void UserView::mouseReleaseEvent(QMouseEvent *evt) {
	QPoint qpos = evt->pos();

	QModelIndex idx = indexAt(qpos);
	if ((evt->button() == Qt::LeftButton) && idx.isValid() && (idx.column() == 1)) {
		UserModel *um = static_cast<UserModel *>(model());
		ClientUser *cu = um->getUser(idx);
		Channel * c = um->getChannel(idx);
		if ((cu && ! cu->qsComment.isEmpty()) ||
		        (! cu && c && ! c->qsDesc.isEmpty())) {
			QRect r = visualRect(idx);
			qpos = qpos - r.topLeft();

			int offset = 0;

			if (cu) {
				// Calculate pixel offset of comment flag
				if (! cu->qsFriendName.isEmpty())
					offset += 18;
				if (cu->iId >= 0)
					offset += 18;
				if (cu->bMute)
					offset += 18;
				if (cu->bSuppress)
					offset += 18;
				if (cu->bDeaf)
					offset += 18;
				if (cu->bSelfMute)
					offset += 18;
				if (cu->bSelfDeaf)
					offset += 18;
				if (cu->bLocalMute)
					offset += 18;
			}

			if ((qpos.x() >= offset) && (qpos.x() <= (offset+18))) {
				// Clicked on comment flag
				QModelIndex midx = idx.sibling(idx.row(), 0);
				r = r.united(visualRect(midx));
				r.setWidth(r.width() / 2);
				QWhatsThis::showText(viewport()->mapToGlobal(r.bottomRight()), Log::validHtml(cu ? cu->qsComment : c->qsDesc), this);
				um->seenComment(idx);
				return;
			}
		}
	}
	QTreeView::mouseReleaseEvent(evt);
}

void UserView::contextMenu(const QPoint &mpos) {
	UserModel *um = static_cast<UserModel *>(model());
	QModelIndex idx = indexAt(mpos);
	if (! idx.isValid())
		idx = currentIndex();
	else
		setCurrentIndex(idx);
	User *p = um->getUser(idx);

	if (p) {
		g.mw->qmUser->popup(mapToGlobal(mpos), g.mw->qaUserMute);
	} else {
		g.mw->qmChannel->popup(mapToGlobal(mpos), g.mw->qaChannelACL);
	}
}

void UserView::doubleClick(const QModelIndex &idx) {
	UserModel *um = static_cast<UserModel *>(model());
	User *p = um->getUser(idx);
	if (p) {
		g.mw->on_qaUserTextMessage_triggered();
		return;
	}

	Channel *c = um->getChannel(idx);
	if (c) {
		// if a channel is double clicked join it
		MumbleProto::UserState mpus;
		mpus.set_session(g.uiSession);
		mpus.set_channel_id(c->iId);
		g.sh->sendMessage(mpus);
	}
}

void UserView::keyboardSearch(const QString &search) {

	if (qtSearch->isActive()) {
		qpmiSearch = QPersistentModelIndex();
		qtSearch->stop();
	}

	bool forceSkip = false;

	if (tSearch.restart() > (QApplication::keyboardInputInterval() * 1000ULL)) {
		qsSearch = QString();
		forceSkip = true;
	}

	bool isBackspace = (search.length() == 1) && (search.at(0).row() == 0) && (search.at(0).cell() == 8);
	if (isBackspace) {
		if (! qsSearch.isEmpty())
			qsSearch = qsSearch.left(qsSearch.length()-1);
	} else {
		qsSearch += search;
	}

	// Try default search (which doesn't recurse non-expanded items) and see if it returns something "valid"
	QTreeView::keyboardSearch(search);
	QModelIndex start = currentIndex();
	if (start.isValid() && model()->data(start, Qt::DisplayRole).toString().startsWith(qsSearch, Qt::CaseInsensitive))
		return;

	if (forceSkip && start.isValid())
		start = indexBelow(start);

	if (! start.isValid())
		start = model()->index(0, 0, QModelIndex());

	QModelIndexList qmil = model()->match(start, Qt::DisplayRole, qsSearch, 1, Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive));
	if (qmil.count() == 0)
		qmil = model()->match(start, Qt::DisplayRole, qsSearch, 1, Qt::MatchFlags(Qt::MatchContains | Qt::MatchWrap | Qt::MatchRecursive));

	if (qmil.isEmpty())
		return;

	QModelIndex qmi = qmil.at(0);

	QModelIndex p = qmi.parent();
	bool isVisible = true;
	while (isVisible && p.isValid()) {
		isVisible = isVisible && isExpanded(p);
		p = p.parent();
	}

	if (isVisible)
		selectionModel()->setCurrentIndex(qmi, QItemSelectionModel::ClearAndSelect);
	else {
		qpmiSearch = qmi;
		qtSearch->start();
	}
}

void UserView::selectSearchResult() {
	if (qpmiSearch.isValid()) {
		selectionModel()->setCurrentIndex(qpmiSearch, QItemSelectionModel::ClearAndSelect);
	}
	qpmiSearch = QPersistentModelIndex();
}
