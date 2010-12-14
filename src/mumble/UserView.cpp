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
  \fn void UserView::activated(const QModelIndex &idx)
  Depending on whether idx points to a channel or user this function
  either moves the player to the channel or opens a message window.
  This Slot connected to the objects activated signal. The activated
  signal could, for example, be triggered by doubleclick.
*/

/*!
  \fn void UserView::keyboardSearch(const QString &search)
  This implementation provides a recursive realtime search over
  the whole channel tree. It also features delayed selection
  with with automatic expanding of folded channels.
*/

UserDelegate::UserDelegate(QObject *p) : QStyledItemDelegate(p) {
}

void UserDelegate::paint(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	const QAbstractItemModel *m = index.model();
	const QModelIndex idxc1 = index.sibling(index.row(), 1);
	QVariant data = m->data(idxc1);
	QList<QVariant> ql = data.toList();

	painter->save();

	QStyleOptionViewItemV4 o = option;
	initStyleOption(&o, index);

	QStyle *style = o.widget->style();
	QIcon::Mode iconMode = QIcon::Normal;

	QPalette::ColorRole colorRole = ((o.state & QStyle::State_Selected) ? QPalette::HighlightedText : QPalette::Text);
#ifdef Q_OS_WIN
	// Qt's Vista Style has the wrong highlight color for treeview items
	// We can't check for QStyleSheetStyle so we have to search the children list search for a QWindowsVistaStyle
	if (qobject_cast<QWindowsVistaStyle *>(style) || style->findChild<QWindowsVistaStyle *>()) {
		colorRole = QPalette::Text;
	}
#endif

	// draw background
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &o, painter, o.widget);

	// resize rect to exclude the flag icons
	o.rect = option.rect.adjusted(0, 0, -18 * ql.count(), 0);

	// draw icon
	QRect decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &o, o.widget);
	o.icon.paint(painter, decorationRect, o.decorationAlignment, iconMode, QIcon::On);

	// draw text
	QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &o, o.widget);
	QString itemText = o.fontMetrics.elidedText(o.text, o.textElideMode, textRect.width());
	painter->setFont(o.font);
	style->drawItemText(painter, textRect, o.displayAlignment, o.palette, true, itemText, colorRole);

	// draw flag icons to original rect
	QRect ps = QRect(option.rect.right() - (ql.size() * 18), option.rect.y(), ql.size() * 18, option.rect.height());
	for (int i = 0; i < ql.size(); ++i) {
		QRect r = ps;
		r.setSize(QSize(16, 16));
		r.translate(i * 18 + 1, 1);
		QRect p = QStyle::alignedRect(option.direction, option.decorationAlignment, r.size(), r);
		qvariant_cast<QIcon>(ql[i]).paint(painter, p, option.decorationAlignment, iconMode, QIcon::On);
	}

	painter->restore();
}

bool UserDelegate::helpEvent(QHelpEvent *evt, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) {
	if (index.isValid()) {
		const QAbstractItemModel *m = index.model();
		const QModelIndex idxc1 = index.sibling(index.row(), 1);
		QVariant data = m->data(idxc1);
		QList<QVariant> ql = data.toList();
		int offset = 0;
		offset = ql.size() * 18;
		offset = option.rect.topRight().x() - offset;

		if (evt->pos().x() >= offset) {
			return QStyledItemDelegate::helpEvent(evt, view, option, idxc1);
		}
	}
	return QStyledItemDelegate::helpEvent(evt, view, option, index);
}

UserView::UserView(QWidget *p) : QTreeView(p) {
	setItemDelegate(new UserDelegate(this));

	qtSearch = new QTimer(this);
	qtSearch->setInterval(QApplication::keyboardInputInterval());
	qtSearch->setSingleShot(true);

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(nodeActivated(const QModelIndex &)));

	connect(qtSearch, SIGNAL(timeout()), this, SLOT(selectSearchResult()));
}

bool UserView::event(QEvent *evt) {
	if (evt->type() == QEvent::WhatsThisClicked) {
		QWhatsThisClickedEvent *qwtce = static_cast<QWhatsThisClickedEvent *>(evt);
		QDesktopServices::openUrl(qwtce->href());
		evt->accept();
		return true;
	}
	return QTreeView::event(evt);
}

void UserView::mouseReleaseEvent(QMouseEvent *evt) {
	QPoint qpos = evt->pos();

	QModelIndex idx = indexAt(qpos);
	if ((evt->button() == Qt::LeftButton) && idx.isValid()) {
		UserModel *um = static_cast<UserModel *>(model());
		ClientUser *cu = um->getUser(idx);
		Channel * c = um->getChannel(idx);
		if ((cu && ! cu->qbaCommentHash.isEmpty()) ||
		        (! cu && c && ! c->qbaDescHash.isEmpty())) {
			QRect r = visualRect(idx);

			int offset = 18;

			if (cu) {
				// Calculate pixel offset of comment flag
				if (cu->bRecording)
					offset += 18;
				if (cu->bPrioritySpeaker)
					offset += 18;
				if (cu->bMute)
					offset += 18;
				if (cu->bSuppress)
					offset += 18;
				if (cu->bSelfMute)
					offset += 18;
				if (cu->bLocalMute)
					offset += 18;
				if (cu->bSelfDeaf)
					offset += 18;
				if (cu->bDeaf)
					offset += 18;
				if (! cu->qsFriendName.isEmpty())
					offset += 18;
				if (cu->iId >= 0)
					offset += 18;
			}

			offset = r.topRight().x() - offset;

			if ((qpos.x() >= offset) && (qpos.x() <= (offset+18))) {
				QString str = um->data(idx, Qt::ToolTipRole).toString();
				if (str.isEmpty()) {
					um->bClicked = true;
				} else {
					QWhatsThis::showText(viewport()->mapToGlobal(r.bottomRight()), str, this);
					um->seenComment(idx);
				}
				return;
			}
		}
	}
	QTreeView::mouseReleaseEvent(evt);
}

void UserView::keyPressEvent(QKeyEvent *ev) {
	if (ev->key() == Qt::Key_Return || ev->key() == Qt::Key_Enter)
		UserView::nodeActivated(currentIndex());
	QTreeView::keyPressEvent(ev);
}

void UserView::nodeActivated(const QModelIndex &idx) {
	UserModel *um = static_cast<UserModel *>(model());
	ClientUser *p = um->getUser(idx);
	if (p) {
		g.mw->openTextMessageDialog(p);
		return;
	}

	Channel *c = um->getChannel(idx);
	if (c) {
		// if a channel is activated join it
		g.sh->joinChannel(c->iId);
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
	bool visible = true;
	while (visible && p.isValid()) {
		visible = visible && isExpanded(p);
		p = p.parent();
	}

	if (visible)
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
