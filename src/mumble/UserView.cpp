// Copyright The Mumble Developers. All rights reserved.
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
#include <stack>

UserDelegate::UserDelegate(QObject *p) : QStyledItemDelegate(p) {
}

void UserDelegate::adjustIcons(int iconTotalDimension, int iconIconPadding, int iconIconDimension) {
	m_iconTotalDimension = iconTotalDimension;
	m_iconIconPadding    = iconIconPadding;
	m_iconIconDimension  = iconIconDimension;
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
	for (QObject *obj : hierarchy) {
		if (QString::fromUtf8(obj->metaObject()->className()) == QString::fromUtf8("QWindowsVistaStyle")) {
			colorRole = QPalette::Text;
			break;
		}
	}
#endif

	// draw background
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &o, painter, o.widget);

	// resize rect to exclude the icons
	o.rect = option.rect.adjusted(0, 0, static_cast< int >(-m_iconTotalDimension * ql.count()), 0);

	// draw icon
	QRect decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &o, o.widget);
	o.icon.paint(painter, decorationRect, o.decorationAlignment, iconMode, QIcon::On);

	// draw text
	QRect textRect   = style->subElementRect(QStyle::SE_ItemViewItemText, &o, o.widget);
	QString itemText = o.fontMetrics.elidedText(o.text, o.textElideMode, textRect.width());
	painter->setFont(o.font);
	style->drawItemText(painter, textRect, static_cast< int >(o.displayAlignment), o.palette, true, itemText,
						colorRole);

	// draw icons to original rect
	const QRect ps     = QRect(option.rect.right() - (static_cast< int >(ql.size() * m_iconTotalDimension)),
                           option.rect.y(), static_cast< int >(ql.size() * m_iconTotalDimension), option.rect.height());
	const int iconPosY = (option.rect.height() / 2) - (m_iconIconDimension / 2);

	for (int i = 0; i < ql.size(); ++i) {
		QRect r = ps;
		r.setSize(QSize(m_iconIconDimension, m_iconIconDimension));
		r.translate(i * m_iconTotalDimension + m_iconIconPadding, iconPosY);
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
		QList< QVariant > iconList       = data.toList();
		const auto offset                = static_cast< int >(iconList.size() * -m_iconTotalDimension);
		const int firstIconPos           = option.rect.topRight().x() + offset;

		if (evt->pos().x() >= firstIconPos) {
			return QStyledItemDelegate::helpEvent(evt, view, option, firstColumnIdx);
		}
	}
	return QStyledItemDelegate::helpEvent(evt, view, option, index);
}

UserView::UserView(QWidget *p) : QTreeView(p), m_userDelegate(make_qt_unique< UserDelegate >(this)) {
	adjustIcons();
	setItemDelegate(m_userDelegate.get());

	// Because in Qt fonts take some time to initialize properly, we have to delay the call
	// to adjustIcons a bit in order to give the fonts the necessary time (so we can read out
	// the actual font details).
	QTimer::singleShot(0, [this]() { adjustIcons(); });

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(nodeActivated(const QModelIndex &)));

	setSelectionMode(ExtendedSelection);
	setSelectionBehavior(SelectItems);
}

void UserView::adjustIcons() {
	// Calculate the icon size for status icons based on font size
	// This should automaticially adjust size when the user has
	// display scaling enabled
	m_iconTotalDimension  = QFontMetrics(font()).height();
	int iconIconPadding   = 1;
	int iconIconDimension = m_iconTotalDimension - (2 * iconIconPadding);
	m_userDelegate->adjustIcons(m_iconTotalDimension, iconIconPadding, iconIconDimension);
	viewport()->update();
}

void UserView::selectAllUsersInChannel(const QModelIndex &index,
									   QItemSelectionModel::SelectionFlags selectionFlags) const {
	UserModel *userModel = qobject_cast< UserModel * >(model());
	QItemSelection selection;

	std::stack< QModelIndex > stack({ index });
	while (!stack.empty()) {
		QModelIndex currentIndex = stack.top();
		stack.pop();
		int rowCount    = userModel->rowCount(currentIndex);
		int columnCount = userModel->columnCount(currentIndex);
		for (int row = 0; row < rowCount; ++row) {
			for (int column = 0; column < columnCount; ++column) {
				QModelIndex childIndex = userModel->index(row, column, currentIndex);
				ClientUser *user       = userModel->getUser(childIndex);
				if (!user) {
					stack.push(childIndex);
				} else if (!isAnyParentCollapsed(childIndex)) {
					selection.append(QItemSelectionRange(childIndex));
				}
			}
		}
	}
	if (supressEvent) {
		return;
	}
	selectionModel()->select(selection, selectionFlags);
}
std::string getString(UserModel *userModel, QModelIndex index) {
	if (!index.isValid())
		return "[INVALID]";
	ClientUser *user = userModel->getUser(index);
	if (user)
		return user->qsName.toStdString();
	Channel *channel = userModel->getChannel(index);
	if (channel)
		return channel->qsName.toStdString();

	return "";
}
bool UserView::isAnyParentCollapsed(const QModelIndex &index) const {
	if (!index.isValid())
		return false;

	QModelIndex parent = index.parent();
	while (parent.isValid()) {
		if (!isExpanded(parent))
			return true;

		parent = parent.parent();
	}

	return false;
}
void UserView::selectAllFromTo(const QModelIndex &from, const QModelIndex &to, bool onlyChannels,
							   QItemSelectionModel::SelectionFlags selectionFlags) const {
	UserModel *userModel   = qobject_cast< UserModel * >(model());
	QModelIndex startIndex = from;
	QModelIndex endIndex   = to;

	if (visualRect(startIndex).y() > visualRect(endIndex).y()) {
		startIndex = to;
		endIndex   = from;
	}
	QModelIndex currentIndex = startIndex;

	QItemSelection selection;
	bool onlySingle     = currentIndex == endIndex;
	QModelIndex parent  = currentIndex.parent();
	QModelIndex sibling = currentIndex.sibling(currentIndex.row() + 1, currentIndex.column());
	while (currentIndex.isValid()
		   && (!onlySingle || (onlySingle && currentIndex != parent && currentIndex != sibling))) {
		ClientUser *user = userModel->getUser(currentIndex);
		if ((!onlyChannels && user) || (onlyChannels && !user && userModel->getChannel(currentIndex))) {
			selection.append(QItemSelectionRange(currentIndex));
		}
		if (!onlySingle && currentIndex == endIndex)
			break;
		currentIndex = indexBelow(currentIndex);
	}
	if (supressEvent) {
		return;
	}
	selectionModel()->select(selection, selectionFlags);
}
QItemSelectionModel::SelectionFlags UserView::selectionCommand(const QModelIndex &index, const QEvent *event) const {
	QItemSelectionModel::SelectionFlags selectionFlags =
		QTreeView::selectionCommand(index, event) & ~(QItemSelectionModel::Columns | QItemSelectionModel::Rows);

	if (index.isValid() && selectionFlags != QItemSelectionModel::NoUpdate
		&& !selectionFlags.testFlag(QItemSelectionModel::ClearAndSelect)) {
		QItemSelectionModel::SelectionFlags newSelectionFlags =
			selectionFlags
			& (QItemSelectionModel::Select | QItemSelectionModel::Deselect | QItemSelectionModel::Toggle);
		QModelIndexList selectedIndices = selectedIndexes();
		bool isUserSelect               = true;
		bool isChannelSelect            = true;

		UserModel *userModel = qobject_cast< UserModel * >(model());
		if (!selectedIndices.isEmpty()) {
			Channel *selectedChannel = userModel->getChannel(selectedIndices[0]);
			if (userModel->getUser(selectedIndices[0])) {
				isChannelSelect = false;
			} else if (selectedChannel) {
				isUserSelect = false;
				if (selectedChannel->cParent == nullptr) {
					return QItemSelectionModel::NoUpdate;
				}
			}
		}
		ClientUser *clientUser = userModel->getUser(index);
		Channel *channel       = userModel->getChannel(index);
		if (!clientUser && channel) {
			if (!isChannelSelect) {
				if (selectionFlags.testFlag(QItemSelectionModel::Current) && !selectedIndices.isEmpty()) {
					selectAllFromTo(index, selectedIndices.back(), false, newSelectionFlags);
				} else {
					selectAllUsersInChannel(index, newSelectionFlags);
				}
				selectionFlags = QItemSelectionModel::NoUpdate;
			} else if (selectionFlags.testFlag(QItemSelectionModel::SelectCurrent)) {
				if (!selectedIndices.isEmpty()) {
					QModelIndex from = index;
					if (channel->cParent == nullptr)
						from = userModel->index(0, index.column(), index);
					selectAllFromTo(from, selectedIndices.back(), true, newSelectionFlags);
					selectionFlags = QItemSelectionModel::NoUpdate;
				}
			} else if (channel->cParent == nullptr) {
				selectionFlags = QItemSelectionModel::NoUpdate;
			}
		} else if (clientUser) {
			if (!isUserSelect) {
				selectionFlags = QItemSelectionModel::NoUpdate;
			} else if (selectionFlags.testFlag(QItemSelectionModel::Current) && !selectedIndices.isEmpty()) {
				selectAllFromTo(index, selectedIndices.back(), false, newSelectionFlags);
				selectionFlags = QItemSelectionModel::NoUpdate;
			}
		}
	}
	return selectionFlags;
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
 * on user/channel icons (e.Global::get(). showing the comment)
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
		// the negative width of a icon's width (which it is initialized to here). For
		// every icon that is displayed to the right of the comment icon, we have to subtract
		// m_iconTotalDimension once.
		int commentIconPxOffset = -m_iconTotalDimension;
		bool hasComment         = false;

		if (clientUser && !clientUser->qbaCommentHash.isEmpty()) {
			hasComment = true;

			if (clientUser->bLocalIgnore)
				commentIconPxOffset -= m_iconTotalDimension;
			if (clientUser->bRecording)
				commentIconPxOffset -= m_iconTotalDimension;
			if (clientUser->bPrioritySpeaker)
				commentIconPxOffset -= m_iconTotalDimension;
			if (clientUser->bMute)
				commentIconPxOffset -= m_iconTotalDimension;
			if (clientUser->bSuppress)
				commentIconPxOffset -= m_iconTotalDimension;
			if (clientUser->bSelfMute)
				commentIconPxOffset -= m_iconTotalDimension;
			if (clientUser->bLocalMute)
				commentIconPxOffset -= m_iconTotalDimension;
			if (clientUser->bSelfDeaf)
				commentIconPxOffset -= m_iconTotalDimension;
			if (clientUser->bDeaf)
				commentIconPxOffset -= m_iconTotalDimension;
			if (!clientUser->qsFriendName.isEmpty())
				commentIconPxOffset -= m_iconTotalDimension;
			if (clientUser->iId >= 0)
				commentIconPxOffset -= m_iconTotalDimension;

		} else if (channel && !channel->qbaDescHash.isEmpty()) {
			hasComment = true;

			switch (channel->m_filterMode) {
				case ChannelFilterMode::PIN:
				case ChannelFilterMode::HIDE:
					commentIconPxOffset -= m_iconTotalDimension;
					break;
				case ChannelFilterMode::NORMAL:
					// NOOP
					break;
			}

			if (channel->hasEnterRestrictions) {
				commentIconPxOffset -= m_iconTotalDimension;
			}
		}

		if (hasComment) {
			QRect r                    = visualRect(idx);
			const int commentIconPxPos = r.topRight().x() + commentIconPxOffset;

			if ((clickPosition.x() >= commentIconPxPos)
				&& (clickPosition.x() <= (commentIconPxPos + m_iconTotalDimension))) {
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
	// Needed because it causes Toggle select to be executed twice
	supressEvent = true;
	QTreeView::mouseReleaseEvent(evt);
	supressEvent = false;
}

void UserView::keyPressEvent(QKeyEvent *ev) {
	if (ev->key() == Qt::Key_Return || ev->key() == Qt::Key_Enter)
		UserView::nodeActivated(currentIndex());
	// Prevent default select all, and select only users instead
	if (ev->matches(QKeySequence::SelectAll)) {
		ev->accept();

		QModelIndex rootIndex = model()->index(0, 0);
		clearSelection();
		selectAllFromTo(rootIndex, rootIndex, false, QItemSelectionModel::Select);
		return;
	}
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
		setRowHidden(idx.row(), idx.parent(), c->isFiltered());
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
