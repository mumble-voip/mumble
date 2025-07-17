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

#include <QtCore/QPropertyAnimation>
#include <QtGui/QDesktopServices>
#include <QtGui/QHelpEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QWhatsThis>

RichTooltip::RichTooltip(QWidget &parentWidget) : QTextEdit(&parentWidget) {
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setTextInteractionFlags(Qt::NoTextInteraction);
	setFocusPolicy(Qt::NoFocus);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_ShowWithoutActivating);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

QSize RichTooltip::getDocumentSize() {
	QTextDocument *doc = document();
	return QSizeF(doc->idealWidth() + 5, doc->size().height()).toSize();
}

void RichTooltip::show(const QString &text, const QPoint &pos, bool isAnimated) {
	QTextDocument *doc = document();
	QTextCursor tc     = textCursor();
	doc->setPageSize(QSizeF());
	Log::setHtml(text, tc, std::bind(&QTextEdit::clear, this));
	for (QObject *obj : qvariant_cast< QList< QObject * > >(doc->property("customObjects"))) {
		switch (qvariant_cast< Log::TextObjectType >(obj->property("objectType"))) {
			case Log::TextObjectType::ImageAnimation: {
				QMovie *animation = qobject_cast< QMovie * >(obj);
				animation->start();
				animation->setPaused(false);
				break;
			}
			case Log::TextObjectType::NoCustomObject:
				break;
		}
	}
	move(pos, false);
	QWidget::show();

	setMinimumHeight(0);
	QSize docSize = getDocumentSize();
	doc->setPageSize(docSize);
	QPropertyAnimation *animation = nullptr;
	if (isAnimated) {
		animation = new QPropertyAnimation(this, "size", this);
		animation->setDuration(150);
		animation->setStartValue(QSize(docSize.width(), 0));
		animation->setEndValue(docSize);
	}
	move(pos);
	if (isAnimated) {
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	} else {
		resize(docSize);
	}
}

void RichTooltip::move(const QPoint &pos, bool isVisible) {
	QWidget *parentWidget = qobject_cast< QWidget * >(parent());
	int docWidth          = getDocumentSize().width();
	bool fitsToTheRight   = pos.x() + docWidth <= parentWidget->x() + parentWidget->width();
	QWidget::move(!fitsToTheRight ? pos - QPoint(docWidth, 0) : pos);
	setVisible(isVisible);
}

void RichTooltip::destruct(bool isAnimated) {
	if (!isAnimated) {
		return deleteLater();
	}

	QSize docSize  = getDocumentSize();
	auto animation = new QPropertyAnimation(this, "size", this);
	animation->setDuration(150);
	animation->setStartValue(docSize);
	animation->setEndValue(QSize(docSize.width(), 0));
	connect(animation, &QAbstractAnimation::finished, this, &QObject::deleteLater);
	animation->start();
}

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
	foreach (QObject *obj, hierarchy) {
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
	bool isTooltip = richTooltip != nullptr;
	if (index.isValid()) {
		quintptr itemId           = index.internalId();
		bool isAnotherItemHovered = previousItemId != itemId;
		if (isAnotherItemHovered) {
			previousItemId = itemId;
		}
		QPoint tooltipPos = evt->globalPos() + QPoint(0, 10);
		if (isTooltip && !isAnotherItemHovered) {
			richTooltip->move(tooltipPos);
		} else {
			if (!isTooltip) {
				UserView *userView = Global::get().mw->qtvUsers;
				richTooltip        = new RichTooltip(*userView);
			}
			richTooltip->show(index.data(Qt::ToolTipRole).toString(), tooltipPos, !isTooltip);
		}
		return true;
	}
	if (isTooltip) {
		richTooltip->destruct();
		richTooltip = nullptr;
	}
	return QStyledItemDelegate::helpEvent(evt, view, option, index);
}

UserView::UserView(QWidget *p) : QTreeView(p), m_userDelegate(make_qt_unique< UserDelegate >(this)) {
	// This attribute is a way to make the `HoverLeave` event work with the theme set to "None" too.
	setAttribute(Qt::WA_Hover);
	adjustIcons();
	setItemDelegate(m_userDelegate.get());

	// Because in Qt fonts take some time to initialize properly, we have to delay the call
	// to adjustIcons a bit in order to give the fonts the necessary time (so we can read out
	// the actual font details).
	QTimer::singleShot(0, [this]() { adjustIcons(); });

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(nodeActivated(const QModelIndex &)));
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

/**
 * This implementation contains a special handler to display
 * custom what's this entries for items and to remove the
 * custom tooltip when no longer hovering the user view.
 * All other events are passed on.
 */
bool UserView::event(QEvent *evt) {
	bool isAction = true;
	switch (evt->type()) {
		case QEvent::WhatsThisClicked: {
			QWhatsThisClickedEvent *qwtce = static_cast< QWhatsThisClickedEvent * >(evt);
			QDesktopServices::openUrl(qwtce->href());
			break;
		}
		case QEvent::HoverLeave: {
			RichTooltip *tooltip = m_userDelegate->richTooltip;
			if (tooltip != nullptr) {
				tooltip->destruct();
				m_userDelegate->richTooltip = nullptr;
			} else {
				isAction = false;
			}
			break;
		}
		default:
			isAction = false;
			break;
	}
	if (isAction) {
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
	if (idx.isValid()) {
		RichTooltip *tooltip = m_userDelegate->richTooltip;
		if (tooltip != nullptr) {
			tooltip->destruct(false);
			m_userDelegate->richTooltip = nullptr;
		}

		if (evt->button() == Qt::LeftButton) {
			UserModel *userModel         = qobject_cast< UserModel * >(model());
			const ClientUser *clientUser = userModel->getUser(idx);
			const Channel *channel       = userModel->getChannel(idx);

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
						if (m_userDelegate->richTooltip == nullptr) {
							UserView *userView          = Global::get().mw->qtvUsers;
							m_userDelegate->richTooltip = new RichTooltip(*userView);
						}
						m_userDelegate->richTooltip->show(str, viewport()->mapToGlobal(r.bottomRight()));
						userModel->seenComment(idx);
					}
					return;
				}
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
