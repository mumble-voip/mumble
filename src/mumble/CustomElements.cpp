// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CustomElements.h"

#include "ClientUser.h"
#include "MainWindow.h"
#include "Log.h"
#include "Utils.h"

#include <QtCore/QTimer>
#include <QtGui/QAbstractTextDocumentLayout>
#include <QtGui/QClipboard>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QScrollBar>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

LogTextBrowser::LogTextBrowser(QWidget *p) : QTextBrowser(p) {}

void LogTextBrowser::resizeEvent(QResizeEvent *e) {
	scrollLogToBottom();
	QTextBrowser::resizeEvent(e);
}

bool LogTextBrowser::event(QEvent *e) {
	if (e->type() == LogDocumentResourceAddedEvent::Type) {
		scrollLogToBottom();
	}
	return QTextBrowser::event(e);
}

int LogTextBrowser::getLogScroll() {
	return verticalScrollBar()->value();
}

int LogTextBrowser::getLogScrollMaximum() {
	return verticalScrollBar()->maximum();
}

void LogTextBrowser::setLogScroll(int scroll_pos) {
	verticalScrollBar()->setValue(scroll_pos);
}

void LogTextBrowser::scrollLogToBottom() {
	verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}


void ChatbarTextEdit::focusInEvent(QFocusEvent *qfe) {
	inFocus(true);
	QTextEdit::focusInEvent(qfe);
}

void ChatbarTextEdit::focusOutEvent(QFocusEvent *qfe) {
	inFocus(false);
	QTextEdit::focusOutEvent(qfe);
}

void ChatbarTextEdit::inFocus(bool focus) {
	if (focus) {
		if (bDefaultVisible) {
			QFont f = font();
			f.setItalic(false);
			setFont(f);
			setPlainText(QString());
			bDefaultVisible = false;
		}
	} else {
		if (toPlainText().trimmed().isEmpty() || bDefaultVisible) {
			QFont f = font();
			f.setItalic(true);
			setFont(f);
			setHtml(qsDefaultText);
			bDefaultVisible = true;
		} else {
			bDefaultVisible = false;
		}
	}
}

void ChatbarTextEdit::contextMenuEvent(QContextMenuEvent *qcme) {
	QMenu *menu = createStandardContextMenu();

	QAction *action = new QAction(tr("Paste and &Send") + QLatin1Char('\t'), menu);
	action->setEnabled(!QApplication::clipboard()->text().isEmpty());
	connect(action, SIGNAL(triggered()), this, SLOT(pasteAndSend_triggered()));
	if (menu->actions().count() > 6)
		menu->insertAction(menu->actions()[6], action);
	else
		menu->addAction(action);

	menu->exec(qcme->globalPos());
	delete menu;
}

void ChatbarTextEdit::dropEvent(QDropEvent *evt) {
	inFocus(true);
	QTextEdit::dropEvent(evt);
}

ChatbarTextEdit::ChatbarTextEdit(QWidget *p) : QTextEdit(p), iHistoryIndex(-1) {
	setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMinimumHeight(0);
	connect(this, SIGNAL(textChanged()), SLOT(doResize()));

	bDefaultVisible = true;
	setDefaultText(tr("<center>Type chat message here</center>"));
}

QSize ChatbarTextEdit::minimumSizeHint() const {
	return QSize(0, fontMetrics().height());
}

QSize ChatbarTextEdit::sizeHint() const {
	QSize sh = QTextEdit::sizeHint();
	sh.setHeight(static_cast<qint32>(document()->documentLayout()->documentSize().height()));
	const_cast<ChatbarTextEdit *>(this)->setMaximumHeight(sh.height());
	return sh;
}

void ChatbarTextEdit::resizeEvent(QResizeEvent *e) {
	QTextEdit::resizeEvent(e);
	QTimer::singleShot(0, this, SLOT(doScrollbar()));
}

void ChatbarTextEdit::doResize() {
	updateGeometry();
	QTimer::singleShot(0, this, SLOT(doScrollbar()));
}

void ChatbarTextEdit::doScrollbar() {
	setVerticalScrollBarPolicy(sizeHint().height() > height() ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff);
	ensureCursorVisible();
}

void ChatbarTextEdit::setDefaultText(const QString &new_default, bool force) {
	qsDefaultText = new_default;

	if (bDefaultVisible || force) {
		QFont f = font();
		f.setItalic(true);
		setFont(f);
		setHtml(qsDefaultText);
		bDefaultVisible = true;
	}
}

bool ChatbarTextEdit::event(QEvent *evt) {
	if (evt->type() == QEvent::ShortcutOverride) {
		return false;
	}

	if (evt->type() == QEvent::KeyPress) {
		QKeyEvent *kev = static_cast<QKeyEvent*>(evt);
		if ((kev->key() == Qt::Key_Enter || kev->key() == Qt::Key_Return) && !(kev->modifiers() & Qt::ShiftModifier)) {
			const QString msg = toPlainText();
			if (!msg.isEmpty()) {
				addToHistory(msg);
				emit entered(msg);
			}
			return true;
		}
		if (kev->key() == Qt::Key_Tab) {
			emit tabPressed();
			return true;
		} else if (kev->key() == Qt::Key_Backtab) {
			emit backtabPressed();
			return true;
		} else if (kev->key() == Qt::Key_Space && kev->modifiers() == Qt::ControlModifier) {
			emit ctrlSpacePressed();
			return true;
		} else if (kev->key() == Qt::Key_Up && kev->modifiers() == Qt::ControlModifier) {
			historyUp();
			return true;
		} else if (kev->key() == Qt::Key_Down && kev->modifiers() == Qt::ControlModifier) {
			historyDown();
			return true;
		}
	}
	return QTextEdit::event(evt);
}

/**
 * The bar will try to complete the username, if the nickname
 * is already complete it will try to find a longer match. If
 * there is none it will cycle the nicknames alphabetically.
 * Nothing is done on mismatch.
 */
unsigned int ChatbarTextEdit::completeAtCursor() {
	// Get an alphabetically sorted list of usernames
	unsigned int id = 0;

	QList<QString> qlsUsernames;

	if (ClientUser::c_qmUsers.empty()) return id;
	foreach(ClientUser *usr, ClientUser::c_qmUsers) {
		qlsUsernames.append(usr->qsName);
	}
	std::sort(qlsUsernames.begin(), qlsUsernames.end());

	QString target = QString();
	QTextCursor tc = textCursor();

	if (toPlainText().isEmpty() || tc.position() == 0) {
		target = qlsUsernames.first();
		tc.insertText(target);
	} else {
		bool bBaseIsName = false;
		int iend = tc.position();
		int istart = toPlainText().lastIndexOf(QLatin1Char(' '), iend - 1) + 1;
		QString base = toPlainText().mid(istart, iend - istart);
		tc.setPosition(istart);
		tc.setPosition(iend, QTextCursor::KeepAnchor);

		if (qlsUsernames.last() == base) {
			bBaseIsName = true;
			target = qlsUsernames.first();
		} else {
			if (qlsUsernames.contains(base)) {
				// Prevent to complete to what's already there
				while (qlsUsernames.takeFirst() != base) {}
				bBaseIsName = true;
			}

			foreach(QString name, qlsUsernames) {
				if (name.startsWith(base, Qt::CaseInsensitive)) {
					target = name;
					break;
				}
			}
		}

		if (bBaseIsName && target.isEmpty() && !qlsUsernames.empty()) {
			// If autocomplete failed and base was a name get the next one
			target = qlsUsernames.first();
		}

		if (!target.isEmpty()) {
			tc.insertText(target);
		}
	}

	if (!target.isEmpty()) {
		setTextCursor(tc);

		foreach(ClientUser *usr, ClientUser::c_qmUsers) {
			if (usr->qsName == target) {
				id = usr->uiSession;
				break;
			}
		}
	}
	return id;
}

void ChatbarTextEdit::addToHistory(const QString &str) {
	iHistoryIndex = -1;
	qslHistory.push_front(str);
	if (qslHistory.length() > MAX_HISTORY) {
		qslHistory.pop_back();
	}
}

void ChatbarTextEdit::historyUp() {
	if (qslHistory.length() == 0)
		return;

	if (iHistoryIndex == -1) {
		qsHistoryTemp = toPlainText();
	}

	if (iHistoryIndex < qslHistory.length() - 1) {
		setPlainText(qslHistory[++iHistoryIndex]);
		moveCursor(QTextCursor::End);
	}
}

void ChatbarTextEdit::historyDown() {
	if (iHistoryIndex < 0) {
		return;
	} else if (iHistoryIndex == 0) {
		setPlainText(qsHistoryTemp);
		iHistoryIndex--;
	} else {
		setPlainText(qslHistory[--iHistoryIndex]);
	}
	moveCursor(QTextCursor::End);
}

void ChatbarTextEdit::pasteAndSend_triggered() {
	paste();
	addToHistory(toPlainText());
	emit entered(toPlainText());
}

DockTitleBar::DockTitleBar() : QLabel(tr("Drag here")) {
	setAlignment(Qt::AlignCenter);
	setEnabled(false);
	qtTick = new QTimer(this);
	qtTick->setSingleShot(true);
	connect(qtTick, SIGNAL(timeout()), this, SLOT(tick()));
	size = newsize = 0;
}

QSize DockTitleBar::sizeHint() const {
	return minimumSizeHint();
}

QSize DockTitleBar::minimumSizeHint() const {
	return QSize(size,size);
}

bool DockTitleBar::eventFilter(QObject *, QEvent *evt) {
	QDockWidget *qdw = qobject_cast<QDockWidget*>(parentWidget());

	if (! this->isEnabled())
		return false;

	switch (evt->type()) {
		case QEvent::Leave:
		case QEvent::Enter:
		case QEvent::MouseMove:
		case QEvent::MouseButtonRelease: {
				newsize = 0;
				QPoint p = qdw->mapFromGlobal(QCursor::pos());
				if ((p.x() >= iroundf(static_cast<float>(qdw->width()) * 0.1f + 0.5f)) && (p.x() < iroundf(static_cast<float>(qdw->width()) * 0.9f + 0.5f))  && (p.y() >= 0) && (p.y() < 15))
					newsize = 15;
				if (newsize > 0 && !qtTick->isActive())
					qtTick->start(500);
				else if ((newsize == size) && qtTick->isActive())
					qtTick->stop();
				else if (newsize == 0)
					tick();
			}
		default:
			break;
	}

	return false;
}

void DockTitleBar::tick() {
	QDockWidget *qdw = qobject_cast<QDockWidget*>(parentWidget());

	if (newsize == size)
		return;

	size = newsize;
	qdw->setTitleBarWidget(this);
}
