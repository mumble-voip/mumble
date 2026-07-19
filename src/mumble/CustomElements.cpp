// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CustomElements.h"

#include "ClientUser.h"
#include "Log.h"
#include "MainWindow.h"
#include "QtWidgetUtils.h"
#include "Utils.h"
#include "Global.h"

#include <QMimeData>
#include <QtCore/QTimer>
#include <QtGui/QAbstractTextDocumentLayout>
#include <QtGui/QClipboard>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QTextBlock>
#include <QtGui/QTextImageFormat>
#include <QtWidgets/QScrollBar>

#include <vector>

/// Custom QTextFormat properties used to remember the natural (unscaled)
/// display size of an image across refits of the chat log.
static constexpr int ImageNaturalWidthProperty  = QTextFormat::UserProperty + 1;
static constexpr int ImageNaturalHeightProperty = QTextFormat::UserProperty + 2;

/// The fraction of the chat log viewport height that a single image may
/// occupy. Keeps a tall image from taking over the entire log; the full-size
/// image is still available via "Open Image".
static constexpr qreal MaxImageHeightFraction = 0.6;

/// Returns the size at which the image would be displayed if it were not
/// scaled to fit the viewport: the size from the message's explicit
/// width/height attributes if present, otherwise the image's own size.
/// Returns an empty size if it cannot be determined.
static QSizeF naturalImageSize(const QTextImageFormat &format, const QImage &image) {
	if (format.hasProperty(ImageNaturalWidthProperty) && format.hasProperty(ImageNaturalHeightProperty)) {
		return QSizeF(format.doubleProperty(ImageNaturalWidthProperty),
					  format.doubleProperty(ImageNaturalHeightProperty));
	}

	const bool hasWidth  = format.hasProperty(QTextFormat::ImageWidth);
	const bool hasHeight = format.hasProperty(QTextFormat::ImageHeight);
	if (hasWidth && hasHeight) {
		return QSizeF(format.width(), format.height());
	}

	if (image.isNull() || image.width() < 1 || image.height() < 1) {
		return QSizeF();
	}

	const QSizeF imageSize(image.width(), image.height());
	if (hasWidth) {
		return QSizeF(format.width(), format.width() * imageSize.height() / imageSize.width());
	}
	if (hasHeight) {
		return QSizeF(format.height() * imageSize.width() / imageSize.height(), format.height());
	}
	return imageSize;
}

LogTextBrowser::LogTextBrowser(QWidget *p) : QTextBrowser(p), m_imageFitTimer(new QTimer(this)) {
	// Refitting images on every single resize step would relayout the whole
	// document continuously while the user drags a splitter, so debounce.
	m_imageFitTimer->setSingleShot(true);
	m_imageFitTimer->setInterval(100);
	connect(m_imageFitTimer, &QTimer::timeout, this, [this]() {
		const bool wasAtBottom = isScrolledToBottom();
		fitImagesToViewport();
		if (wasAtBottom) {
			setLogScroll(verticalScrollBar()->maximum());
		}
	});
}

void LogTextBrowser::resizeEvent(QResizeEvent *e) {
	QTextBrowser::resizeEvent(e);
	m_imageFitTimer->start();
}

bool LogTextBrowser::event(QEvent *e) {
	// Moving the window to a screen with a different scale factor changes the
	// pixel size the displayed image resources should be rendered at, without
	// necessarily changing the logical viewport size.
	if (e->type() == QEvent::DevicePixelRatioChange) {
		m_imageFitTimer->start();
	}
	return QTextBrowser::event(e);
}

void LogTextBrowser::fitImagesToViewport(int fromPosition) {
	LogDocument *doc = qobject_cast< LogDocument * >(document());
	if (!doc) {
		return;
	}

	// Leave some room for the document margin and possible frame decorations
	// around messages so that a fitted image never triggers the horizontal
	// scrollbar.
	const int slack          = static_cast< int >(2 * doc->documentMargin()) + 8;
	const int availableWidth = viewport()->width() - slack;
	if (availableWidth < 1) {
		return;
	}
	const int availableHeight = qMax(1, static_cast< int >(viewport()->height() * MaxImageHeightFraction));

	const qreal pixelRatio = viewport()->devicePixelRatio();
	// After a change of the device pixel ratio, the display resources of all
	// images have to be regenerated, even if their displayed size is
	// unchanged. Only a refit of the whole document gets to reset the flag:
	// a partial (insert-time) refit only covers the images of one message.
	const bool pixelRatioChanged = !qFuzzyCompare(pixelRatio, m_lastImageFitPixelRatio);
	if (fromPosition == 0) {
		m_lastImageFitPixelRatio = pixelRatio;
	}

	struct PendingFit {
		int position;
		int length;
		QTextImageFormat format;
	};
	std::vector< PendingFit > pending;

	// The pixel size wanted for each image resource. Identical images share a
	// single resource entry (the resource name is the data URL), so if
	// occurrences need different display sizes, the largest one wins and the
	// smaller occurrences are scaled down from it at paint time.
	QHash< QString, QSize > displaySizes;

	for (QTextBlock block = doc->findBlock(fromPosition); block.isValid(); block = block.next()) {
		for (QTextBlock::iterator it = block.begin(); !it.atEnd(); ++it) {
			const QTextFragment fragment = it.fragment();
			if (!fragment.isValid() || !fragment.charFormat().isImageFormat()) {
				continue;
			}

			QTextImageFormat format = fragment.charFormat().toImageFormat();
			const QImage original   = doc->originalImage(format.name());
			const QSizeF natural    = naturalImageSize(format, original);
			if (natural.isEmpty()) {
				continue;
			}

			QSizeF target = natural;
			if (natural.width() > availableWidth || natural.height() > availableHeight) {
				target = natural.scaled(QSizeF(availableWidth, availableHeight), Qt::KeepAspectRatio);
			}

			const qreal currentWidth = format.hasProperty(QTextFormat::ImageWidth) ? format.width() : natural.width();
			const qreal currentHeight =
				format.hasProperty(QTextFormat::ImageHeight) ? format.height() : natural.height();
			const bool sizeUpToDate =
				qAbs(currentWidth - target.width()) < 0.5 && qAbs(currentHeight - target.height()) < 0.5;
			if (sizeUpToDate && !pixelRatioChanged) {
				continue;
			}

			if (!original.isNull()) {
				const QSize pixelSize = (target * pixelRatio).toSize();
				QSize &wanted         = displaySizes[format.name()];
				if (!wanted.isValid() || pixelSize.width() * pixelSize.height() > wanted.width() * wanted.height()) {
					wanted = pixelSize;
				}
			}

			if (sizeUpToDate) {
				// Only the display resource needs to be regenerated.
				continue;
			}

			format.setProperty(ImageNaturalWidthProperty, natural.width());
			format.setProperty(ImageNaturalHeightProperty, natural.height());
			format.setWidth(qMax< qreal >(1, target.width()));
			format.setHeight(qMax< qreal >(1, target.height()));
			pending.push_back({ fragment.position(), fragment.length(), format });
		}
	}

	// Qt paints images that are displayed at a size other than their pixel
	// size without any smoothing filter, which makes downscaled images look
	// noticeably pixelated. Replace the displayed resources with smoothly
	// prescaled variants instead (the originals stay available through
	// LogDocument::originalImage()).
	for (auto it = displaySizes.cbegin(); it != displaySizes.cend(); ++it) {
		const QImage original = doc->originalImage(it.key());
		if (original.isNull() || it.value().isEmpty()) {
			continue;
		}
		QImage display = original;
		if (it.value() != original.size()) {
			display = original.scaled(it.value(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		}
		display.setDevicePixelRatio(pixelRatio);
		doc->addResource(QTextDocument::ImageResource, QUrl(it.key()), display);
	}

	if (!pending.empty()) {
		QTextCursor cursor(doc);
		cursor.beginEditBlock();
		for (const PendingFit &fit : pending) {
			cursor.setPosition(fit.position);
			cursor.setPosition(fit.position + fit.length, QTextCursor::KeepAnchor);
			cursor.setCharFormat(fit.format);
		}
		cursor.endEditBlock();
	} else if (!displaySizes.isEmpty()) {
		// Resources were regenerated without any format change, so nothing
		// triggered a repaint yet.
		viewport()->update();
	}
}

int LogTextBrowser::getLogScroll() {
	return verticalScrollBar()->value();
}

void LogTextBrowser::setLogScroll(int scroll_pos) {
	verticalScrollBar()->setValue(scroll_pos);
}

bool LogTextBrowser::isScrolledToBottom() {
	const QScrollBar *scrollBar = verticalScrollBar();
	return scrollBar->value() == scrollBar->maximum();
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
			setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
			bDefaultVisible = false;
		}
	} else {
		if (toPlainText().trimmed().isEmpty() || bDefaultVisible) {
			applyPlaceholder();
		} else {
			setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
			bDefaultVisible = false;
		}
	}
}

void ChatbarTextEdit::contextMenuEvent(QContextMenuEvent *qcme) {
	QMenu *menu = createStandardContextMenu();

	QAction *action = new QAction(tr("Paste and &Send"), menu);
	action->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_V));
	action->setEnabled(!QApplication::clipboard()->text().isEmpty() || !QApplication::clipboard()->image().isNull());
	connect(action, SIGNAL(triggered()), this, SLOT(pasteAndSend_triggered()));
	if (menu->actions().count() > 6) {
		menu->insertAction(menu->actions()[6], action);
	} else {
		menu->addAction(action);
	}

	menu->exec(qcme->globalPos());
	delete menu;
}

void ChatbarTextEdit::dragEnterEvent(QDragEnterEvent *evt) {
	inFocus(true);
	evt->acceptProposedAction();
}

void ChatbarTextEdit::dragMoveEvent(QDragMoveEvent *evt) {
	inFocus(true);
	evt->acceptProposedAction();
}

void ChatbarTextEdit::dropEvent(QDropEvent *evt) {
	inFocus(true);
	if (sendImagesFromMimeData(evt->mimeData())) {
		evt->acceptProposedAction();
	} else {
		QTextEdit::dropEvent(evt);
	}
}

ChatbarTextEdit::ChatbarTextEdit(QWidget *p) : QTextEdit(p), iHistoryIndex(-1) {
	setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMinimumHeight(0);
	connect(this, &ChatbarTextEdit::textChanged, this, &ChatbarTextEdit::doResize);

	bDefaultVisible = true;
	setDefaultText(tr("<center>Type chat message here</center>"));
	setAcceptDrops(true);

	m_justPasted = false;
}

QSize ChatbarTextEdit::minimumSizeHint() const {
	return QSize(0, fontMetrics().height());
}

QSize ChatbarTextEdit::sizeHint() const {
	QSize sh                    = QTextEdit::sizeHint();
	const int minHeight         = minimumSizeHint().height();
	const int documentHeight    = static_cast< int >(document()->documentLayout()->documentSize().height());
	const int chatBarLineHeight = QFontMetrics(ChatbarTextEdit::font()).height();

	sh.setHeight(std::max(minHeight, std::min(chatBarLineHeight * 10, documentHeight)));
	const_cast< ChatbarTextEdit * >(this)->setMaximumHeight(sh.height());
	return sh;
}

void ChatbarTextEdit::resizeEvent(QResizeEvent *e) {
	QTextEdit::resizeEvent(e);
	QTimer::singleShot(0, this, SLOT(doScrollbar()));

	if (bDefaultVisible) {
		QTimer::singleShot(0, [this]() { applyPlaceholder(); });
	}
}

void ChatbarTextEdit::doResize() {
	updateGeometry();
	QTimer::singleShot(0, this, SLOT(doScrollbar()));
}

void ChatbarTextEdit::doScrollbar() {
	const int documentHeight = static_cast< int >(document()->documentLayout()->documentSize().height());
	setVerticalScrollBarPolicy(documentHeight > height() ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff);
	ensureCursorVisible();
}

void ChatbarTextEdit::setDefaultText(const QString &new_default, bool force) {
	qsDefaultText = new_default;

	if (bDefaultVisible || force) {
		applyPlaceholder();
	}
}

void ChatbarTextEdit::applyPlaceholder() {
	QFont f = font();
	f.setItalic(true);
	setFont(f);
	setWordWrapMode(QTextOption::NoWrap);
	setHtml(qsDefaultText);

	Mumble::QtUtils::elideText(*document(), static_cast< uint32_t >(size().width()));

	bDefaultVisible = true;
}

bool ChatbarTextEdit::canInsertFromMimeData(const QMimeData *source) const {
	return (QTextEdit::canInsertFromMimeData(source) || source->hasImage() || source->hasUrls());
}

void ChatbarTextEdit::insertFromMimeData(const QMimeData *source) {
	if (!sendImagesFromMimeData(source)) {
		QTextEdit::insertFromMimeData(source);
	}
}

bool ChatbarTextEdit::sendImagesFromMimeData(const QMimeData *source) {
	if ((source->hasImage() || source->hasUrls())) {
		if (Global::get().bAllowHTML) {
			if (source->hasImage()) {
				// Process the image pasted onto the chatbar.
				QImage image = qvariant_cast< QImage >(source->imageData());
				if (emitPastedImage(image)) {
					return true;
				} else {
					Global::get().l->log(Log::Information, tr("Unable to send image: too large."));
					return false;
				}

			} else if (source->hasUrls()) {
				// Process the files dropped onto the chatbar. URLs here should be understood as the URIs of files.
				QList< QUrl > urlList = source->urls();

				int count = 0;
				for (int i = 0; i < urlList.size(); ++i) {
					QString path = urlList[i].toLocalFile();
					QImage image(path);

					if (image.isNull())
						continue;
					if (emitPastedImage(image)) {
						++count;
					} else {
						Global::get().l->log(Log::Information, tr("Unable to send image %1: too large.").arg(path));
					}
				}

				return (count > 0);
			}
		} else {
			Global::get().l->log(Log::Information, tr("This server does not allow sending images."));
		}
	}
	return false;
}

bool ChatbarTextEdit::emitPastedImage(QImage image) {
	QString processedImage = Log::imageToImg(image, static_cast< int >(Global::get().uiImageLength));
	if (processedImage.length() > 0) {
		QString imgHtml = QLatin1String("<br />") + processedImage;
		emit pastedImage(imgHtml);
		return true;
	}
	return false;
}

bool ChatbarTextEdit::event(QEvent *evt) {
	if (evt->type() == QEvent::ShortcutOverride) {
		return false;
	}

	if (evt->type() == QEvent::KeyPress) {
		QKeyEvent *kev = static_cast< QKeyEvent * >(evt);
		if ((kev->key() == Qt::Key_Enter || kev->key() == Qt::Key_Return) && !(kev->modifiers() & Qt::ShiftModifier)) {
			const QString msg = toPlainText();
			if (!msg.isEmpty()) {
				addToHistory(msg);
				if ((kev->modifiers() & Qt::ControlModifier) && !m_justPasted) {
					emit ctrlEnterPressed(msg);
				} else {
					emit entered(msg);
				}
				m_justPasted = false;
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
		} else if (kev->key() == Qt::Key_V && (kev->modifiers() & Qt::ControlModifier)) {
			if (kev->modifiers() & Qt::ShiftModifier) {
				pasteAndSend_triggered();
				return true;
			} else {
				// Remember that we just pasted into the chat field
				// and allow CTRL+Enter only when we are sure it was
				// released for at least one GUI cycle.
				// See #6568
				m_justPasted = true;
			}
		}
	}

	if (evt->type() == QEvent::KeyRelease) {
		QKeyEvent *kev = static_cast< QKeyEvent * >(evt);
		if (kev->key() == Qt::Key_Control) {
			m_justPasted = false;
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

	QList< QString > qlsUsernames;

	if (ClientUser::c_qmUsers.empty())
		return id;
	for (ClientUser *usr : ClientUser::c_qmUsers) {
		qlsUsernames.append(usr->qsName);
	}
	std::sort(qlsUsernames.begin(), qlsUsernames.end());

	QString target = QString();
	QTextCursor tc = textCursor();

	if (toPlainText().isEmpty() || tc.position() == 0) {
		target = qlsUsernames.first();
		tc.insertText(target);
	} else {
		bool bBaseIsName   = false;
		const int iend     = tc.position();
		const auto istart  = toPlainText().lastIndexOf(QLatin1Char(' '), iend - 1) + 1;
		const QString base = toPlainText().mid(istart, iend - istart);
		tc.setPosition(static_cast< int >(istart));
		tc.setPosition(iend, QTextCursor::KeepAnchor);

		if (qlsUsernames.last() == base) {
			bBaseIsName = true;
			target      = qlsUsernames.first();
		} else {
			if (qlsUsernames.contains(base)) {
				// Prevent to complete to what's already there
				while (qlsUsernames.takeFirst() != base) {
				}
				bBaseIsName = true;
			}

			for (const QString &name : qlsUsernames) {
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

		for (ClientUser *usr : ClientUser::c_qmUsers) {
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
	if (bDefaultVisible) {
		// Clear placeholder
		setPlainText(QString());
	}

	paste();

	if (!toPlainText().isEmpty()) {
		addToHistory(toPlainText());
		emit entered(toPlainText());
	}
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
	return QSize(size, size);
}

bool DockTitleBar::eventFilter(QObject *, QEvent *evt) {
	QDockWidget *qdw = qobject_cast< QDockWidget * >(parentWidget());

	if (!this->isEnabled())
		return false;

	switch (evt->type()) {
		case QEvent::Leave:
		case QEvent::Enter:
		case QEvent::MouseMove:
		case QEvent::MouseButtonRelease: {
			newsize  = 0;
			QPoint p = qdw->mapFromGlobal(QCursor::pos());
			if ((p.x() >= static_cast< int >(static_cast< float >(qdw->width()) * 0.1f + 0.5f))
				&& (p.x() < static_cast< int >(static_cast< float >(qdw->width()) * 0.9f + 0.5f)) && (p.y() >= 0)
				&& (p.y() < 15))
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
	QDockWidget *qdw = qobject_cast< QDockWidget * >(parentWidget());

	if (newsize == size)
		return;

	size = newsize;
	qdw->setTitleBarWidget(this);
}
