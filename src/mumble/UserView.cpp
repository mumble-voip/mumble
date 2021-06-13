// Copyright 2009-2021 The Mumble Developers. All rights reserved.
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
#include "Global.h"

#include <QtGui/QDesktopServices>
#include <QtGui/QHelpEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QWhatsThis>

const int UserDelegate::FLAG_ICON_DIMENSION = 16;
const int UserDelegate::FLAG_ICON_PADDING   = 1;
const int UserDelegate::FLAG_DIMENSION      = 18;

UserDelegate::UserDelegate(QObject *p) : QStyledItemDelegate(p) {
}

void UserDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	const QAbstractItemModel *m = index.model();
	const QModelIndex idxc1     = index.sibling(index.row(), 1);
	QVariant data               = m->data(idxc1);
	QList< QVariant > ql        = data.toList();

	// Allow a UserView's BackgroundRole to override the current theme's default color.
	QVariant bg = index.data(Qt::BackgroundRole);
	if (bg.isValid()) {
		painter->fillRect(option.rect, bg.value< QBrush >());
	}

	painter->save();

	QStyleOptionViewItem o = option;
	initStyleOption(&o, index);

	QStyle *style        = o.widget->style();
	QIcon::Mode iconMode = QIcon::Normal;

	QPalette::ColorRole colorRole = ((o.state & QStyle::State_Selected) ? QPalette::HighlightedText : QPalette::Text);
#if defined(Q_OS_WIN)
	// Qt's Vista Style has the wrong highlight color for treeview items
	// We can't check for QStyleSheetStyle so we have to search the children list search for a QWindowsVistaStyle
	QList< QObject * > hierarchy = style->findChildren< QObject * >();
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
	QRect textRect   = style->subElementRect(QStyle::SE_ItemViewItemText, &o, o.widget);
	QString itemText = o.fontMetrics.elidedText(o.text, o.textElideMode, textRect.width());
	painter->setFont(o.font);
	style->drawItemText(painter, textRect, o.displayAlignment, o.palette, true, itemText, colorRole);

	// draw flag icons to original rect
	QRect ps = QRect(option.rect.right() - (ql.size() * FLAG_DIMENSION), option.rect.y(), ql.size() * FLAG_DIMENSION,
					 option.rect.height());

	for (int i = 0; i < ql.size(); ++i) {
		QRect r = ps;
		r.setSize(QSize(FLAG_ICON_DIMENSION, FLAG_ICON_DIMENSION));
		r.translate(i * FLAG_DIMENSION + FLAG_ICON_PADDING, FLAG_ICON_PADDING);
		QRect p = QStyle::alignedRect(option.direction, option.decorationAlignment, r.size(), r);
		qvariant_cast< QIcon >(ql[i]).paint(painter, p, option.decorationAlignment, iconMode, QIcon::On);
	}

	painter->restore();
}

bool UserDelegate::helpEvent(QHelpEvent *evt, QAbstractItemView *view, const QStyleOptionViewItem &option,
							 const QModelIndex &index) {
	if (index.isValid()) {
		const QAbstractItemModel *m      = index.model();
		const QModelIndex firstColumnIdx = index.sibling(index.row(), 1);
		QVariant data                    = m->data(firstColumnIdx);
		QList< QVariant > flagList       = data.toList();
		const int offset                 = flagList.size() * -FLAG_DIMENSION;
		const int firstFlagPos           = option.rect.topRight().x() + offset;

		if (evt->pos().x() >= firstFlagPos) {
			return QStyledItemDelegate::helpEvent(evt, view, option, firstColumnIdx);
		}
	}
	return QStyledItemDelegate::helpEvent(evt, view, option, index);
}

UserView::UserView(QWidget *p) : QTreeView(p) {
	setItemDelegate(new UserDelegate(this));

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(nodeActivated(const QModelIndex &)));
}

/**
 * This implementation contains a special handler to display
 * custom what's this entries for items. All other events are
 * passed on.
 */
bool UserView::event(QEvent *evt) {
	if (evt->type() == QEvent::WhatsThisClicked) {
		QWhatsThisClickedEvent *qwtce = static_cast< QWhatsThisClickedEvent * >(evt);
		QDesktopServices::openUrl(qwtce->href());
		evt->accept();
		return true;
	}
	return QTreeView::event(evt);
}

/**
 * This function is used to create custom behaviour when clicking
 * on user/channel flags (e.Global::get(). showing the comment)
 */
void UserView::mouseReleaseEvent(QMouseEvent *evt) {
	QPoint clickPosition = evt->pos();

	QModelIndex idx = indexAt(clickPosition);
	if ((evt->button() == Qt::LeftButton) && idx.isValid()) {
		UserModel *userModel   = qobject_cast< UserModel * >(model());
		ClientUser *clientUser = userModel->getUser(idx);
		Channel *channel       = userModel->getChannel(idx);

		// This is the x offset of the _beginning_ of the comment icon starting from the
		// right.
		// Thus if the comment icon is the last icon that is displayed, this is equal to
		// the negative width of a flag's width (which it is initialized to here). For
		// every flag that is displayed to the right of the comment flag, we have to subtract
		// UserDelegate::FLAG_DIMENSION once.
		int commentFlagPxOffset = -UserDelegate::FLAG_DIMENSION;
		bool hasComment         = false;

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
			if (!clientUser->qsFriendName.isEmpty())
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			if (clientUser->iId >= 0)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;

		} else if (channel && !channel->qbaDescHash.isEmpty()) {
			hasComment = true;

			if (channel->bFiltered)
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;

			if (channel->hasEnterRestrictions) {
				commentFlagPxOffset -= UserDelegate::FLAG_DIMENSION;
			}
		}

		if (hasComment) {
			QRect r                    = visualRect(idx);
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
	UserModel *um = static_cast< UserModel * >(model());
	ClientUser *p = um->getUser(idx);
	if (p) {
		Global::get().mw->openTextMessageDialog(p);
		return;
	}

	Channel *c = um->getChannel(idx);
	if (c) {
		// if a channel is activated join it
		Global::get().sh->joinChannel(Global::get().uiSession, c->iId);
	}
}

void UserView::keyboardSearch(const QString &) {
	// Disable keyboard search for the UserView in order to prevent jumping wildly through the
	// UI just because the user has accidentally typed something on their keyboard.
	return;
}

bool channelHasUsers(const Channel *c) {
	if (c->qlUsers.isEmpty() == false)
		return true;

	int i;

	for (i = 0; i < c->qlChannels.count(); i++) {
		if (channelHasUsers(c->qlChannels[i]))
			return true;
	}
	return false;
}

static bool channelFiltered(const Channel *c) {
	while (c) {
		if (c->bFiltered)
			return true;
		c = c->cParent;
	}
	return false;
}

void UserView::updateChannel(const QModelIndex &idx) {
	UserModel *um = static_cast< UserModel * >(model());

	if (!idx.isValid()) {
		return;
	}

	Channel *c = um->getChannel(idx);

	for (int i = 0; idx.model()->index(i, 0, idx).isValid(); ++i) {
		updateChannel(idx.model()->index(i, 0, idx));
	}

	if (c && idx.parent().isValid()) {
		if (Global::get().s.bFilterActive == false) {
			setRowHidden(idx.row(), idx.parent(), false);
		} else {
			bool isChannelUserIsIn = false;

			// Check whether user resides in this channel or a subchannel
			if (Global::get().uiSession != 0) {
				const ClientUser *user = ClientUser::get(Global::get().uiSession);
				if (user) {
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

			if (channelFiltered(c) && !isChannelUserIsIn) {
				setRowHidden(idx.row(), idx.parent(), true);
			} else {
				if (Global::get().s.bFilterHidesEmptyChannels && !channelHasUsers(c)) {
					setRowHidden(idx.row(), idx.parent(), true);
				} else {
					setRowHidden(idx.row(), idx.parent(), false);
				}
			}
		}
	}
}

void UserView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector< int > &) {
	UserModel *um = static_cast< UserModel * >(model());
	int nRowCount = um->rowCount();
	int i;
	for (i = 0; i < nRowCount; i++)
		updateChannel(um->index(i, 0));

	QTreeView::dataChanged(topLeft, bottomRight);
}
