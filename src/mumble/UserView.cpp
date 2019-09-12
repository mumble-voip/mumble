// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserView.h"

#include "Channel.h"
#include "ClientUser.h"
#include "Log.h"
#include "MainWindow.h"
#include "ServerHandler.h"
#include "UserModel.h"

#include <QtGui/QDesktopServices>
#include <QtGui/QHelpEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QWhatsThis>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

const int UserDelegate::FLAG_ICON_DIMENSION = 16;
const int UserDelegate::FLAG_ICON_PADDING = 1;
const int UserDelegate::FLAG_DIMENSION = 18;

UserDelegate::UserDelegate(QObject *p) : QStyledItemDelegate(p) {
}

void UserDelegate::paint(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	const QAbstractItemModel *m = index.model();
	const QModelIndex idxc1 = index.sibling(index.row(), 1);
	QVariant data = m->data(idxc1);
	QList<QVariant> ql = data.toList();

	// Allow a UserView's BackgroundRole to override the current theme's default color.
	QVariant bg = index.data(Qt::BackgroundRole);
	if (bg.isValid()) {
		painter->fillRect(option.rect, bg.value<QBrush>());
	}

	painter->save();

#if QT_VERSION >= 0x050000
	QStyleOptionViewItem o = option;
#else
	QStyleOptionViewItemV4 o = option;
#endif

	initStyleOption(&o, index);

	QStyle *style = o.widget->style();
	QIcon::Mode iconMode = QIcon::Normal;

	QPalette::ColorRole colorRole = ((o.state & QStyle::State_Selected) ? QPalette::HighlightedText : QPalette::Text);
#if defined(Q_OS_WIN)
	// Qt's Vista Style has the wrong highlight color for treeview items
	// We can't check for QStyleSheetStyle so we have to search the children list search for a QWindowsVistaStyle
	QList<QObject *> hierarchy = style->findChildren<QObject *>();
	hierarchy.insert(0, style);
	foreach (QObject *obj, hierarchy) {
		if (QString::fromUtf8(obj->metaObject()->className()) == QString::fromUtf8("QWindowsVistaStyle")) {
			colorRole = QPalette::Text;
			break;
		}
	}
#endif

	// draw background
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &o, painter, o.widget);

	// resize rect to exclude the flag icons
	o.rect = option.rect.adjusted(0, 0, -FLAG_DIMENSION * ql.count(), 0);

	// draw icon
	QRect decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &o, o.widget);
	o.icon.paint(painter, decorationRect, o.decorationAlignment, iconMode, QIcon::On);

	// draw text
	QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &o, o.widget);
	QString itemText = o.fontMetrics.elidedText(o.text, o.textElideMode, textRect.width());
	painter->setFont(o.font);
	style->drawItemText(painter, textRect, o.displayAlignment, o.palette, true, itemText, colorRole);

	// draw flag icons to original rect
	QRect ps = QRect(option.rect.right() - (ql.size() * FLAG_DIMENSION),
					 option.rect.y(), ql.size() * FLAG_DIMENSION,
					 option.rect.height());

	for (int i = 0; i < ql.size(); ++i) {
		QRect r = ps;
		r.setSize(QSize(FLAG_ICON_DIMENSION, FLAG_ICON_DIMENSION));
		r.translate(i * FLAG_DIMENSION + FLAG_ICON_PADDING, FLAG_ICON_PADDING);
		QRect p = QStyle::alignedRect(option.direction, option.decorationAlignment, r.size(), r);
		qvariant_cast<QIcon>(ql[i]).paint(painter, p, option.decorationAlignment, iconMode, QIcon::On);
	}

	painter->restore();
}

bool UserDelegate::helpEvent(QHelpEvent *evt, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) {
	if (index.isValid()) {
		const QAbstractItemModel *m = index.model();
		const QModelIndex firstColumnIdx = index.sibling(index.row(), 1);
		QVariant data = m->data(firstColumnIdx);
		QList<QVariant> flagList = data.toList();
		const int offset = flagList.size() * -FLAG_DIMENSION;
		const int firstFlagPos = option.rect.topRight().x() + offset;

		if (evt->pos().x() >= firstFlagPos) {
			return QStyledItemDelegate::helpEvent(evt, view, option, firstColumnIdx);
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

/**
 * This implementation contains a special handler to display
 * custom what's this entries for items. All other events are
 * passed on.
 */
bool UserView::event(QEvent *evt) {
	if (evt->type() == QEvent::WhatsThisClicked) {
		QWhatsThisClickedEvent *qwtce = static_cast<QWhatsThisClickedEvent *>(evt);
		QDesktopServices::openUrl(qwtce->href());
		evt->accept();
		return true;
	}
	return QTreeView::event(evt);
}

/**
 * This function is used to create custom behaviour when clicking
 * on user/channel flags (e.g. showing the comment)
 */
void UserView::mouseReleaseEvent(QMouseEvent *evt) {
	QPoint clickPosition = evt->pos();

	QModelIndex idx = indexAt(clickPosition);
	if ((evt->button() == Qt::LeftButton) && idx.isValid()) {
		UserModel *userModel = qobject_cast<UserModel *>(model());
		ClientUser *clientUser = userModel->getUser(idx);
		Channel *channel = userModel->getChannel(idx);

		int commentFlagPxOffset = -UserDelegate::FLAG_DIMENSION;
		bool hasComment = false;

		if (clientUser && !clientUser->qbaCommentHash.isEmpty()) {
			hasComment = true;

			if (clientUser->bLocalIgnore)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (clientUser->bRecording)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (clientUser->bPrioritySpeaker)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (clientUser->bMute)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (clientUser->bSuppress)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (clientUser->bSelfMute)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (clientUser->bLocalMute)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (clientUser->bSelfDeaf)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (clientUser->bDeaf)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (! clientUser->qsFriendName.isEmpty())
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (clientUser->iId >= 0)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;

		} else if (channel && !channel->qbaDescHash.isEmpty()) {
			hasComment = true;

			if (channel->bFiltered)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;

		}

		if (hasComment) {
			QRect r = visualRect(idx);
			const int commentFlagPxPos = r.topRight().x() + commentFlagPxOffset;

			if ((clickPosition.x() >= commentFlagPxPos)
					&& (clickPosition.x() <= (commentFlagPxPos + UserDelegate::FLAG_DIMENSION))) {
				// Clicked comment icon
				QString str = userModel->data(idx, Qt::ToolTipRole).toString();
				if (str.isEmpty()) {
					userModel->bClicked = true;
				} else {
					QWhatsThis::showText(viewport()->mapToGlobal(r.bottomRight()), str, this);
					userModel->seenComment(idx);
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
		g.sh->joinChannel(g.uiSession, c->iId);
	}
}

/**
 * This implementation provides a recursive realtime search over
 * the whole channel tree. It also features delayed selection
 * with with automatic expanding of folded channels.
 */
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

bool channelHasUsers(const Channel *c)
{
	if(c->qlUsers.isEmpty() == false)
		return true;

	int i;	

	for(i=0;i<c->qlChannels.count();i++)
	{
		if(channelHasUsers(c->qlChannels[i]))
			return true;
	}
	return false;
}

static bool channelFiltered(const Channel *c)
{
	while(c) {
		if(c->bFiltered)
			return true;
		c=c->cParent;
	}
	return false;
}

void UserView::updateChannel(const QModelIndex &idx) {
	UserModel *um = static_cast<UserModel *>(model());

	if (!idx.isValid()) {
		return;
	}

	Channel *c = um->getChannel(idx);

	for (int i = 0; idx.model()->index(i, 0, idx).isValid(); ++i) {
		updateChannel(idx.model()->index(i, 0, idx));
	}

	if (c && idx.parent().isValid()) {
		if (g.s.bFilterActive == false) {
			setRowHidden(idx.row(), idx.parent(), false);
		} else {
			bool isChannelUserIsIn = false;
			
			// Check whether user resides in this channel or a subchannel
			if (g.uiSession != 0) {
				const ClientUser* user = ClientUser::get(g.uiSession);
				if (user != NULL) {
					Channel *chan = user->cChannel;
					while (chan) {
						if (chan == c) {
							isChannelUserIsIn = true;
							break;
						}
						chan = chan->cParent;
					}
				}
			}
			
			if(channelFiltered(c) && !isChannelUserIsIn) {
				setRowHidden(idx.row(),idx.parent(),true);
			} else {
				if(g.s.bFilterHidesEmptyChannels && !channelHasUsers(c)) {
					setRowHidden(idx.row(),idx.parent(),true);
				} else {
					setRowHidden(idx.row(),idx.parent(),false);
				}
			}
		}
	}
}

#if QT_VERSION >= 0x050000
void UserView::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> &)
#else
void UserView::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight)
#endif
{
	UserModel *um = static_cast<UserModel *>(model());
	int nRowCount = um->rowCount();
	int i;
	for(i=0;i<nRowCount;i++)
		updateChannel(um->index(i,0));

	QTreeView::dataChanged(topLeft,bottomRight);
}

