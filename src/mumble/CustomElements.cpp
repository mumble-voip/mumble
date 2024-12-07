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
#include <QtGui/QMovie>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtWidgets/QScrollBar>

LogTextBrowser::LogTextBrowser(QWidget *p) : QTextBrowser(p) {
}

void LogTextBrowser::update() {
	document()->documentLayout()->update(QRect(getScrollX(), getScrollY(), width(), height()));
}

int LogTextBrowser::getScrollX() {
	return horizontalScrollBar()->value();
}

int LogTextBrowser::getScrollY() {
	return verticalScrollBar()->value();
}

void LogTextBrowser::setScrollX(int scroll_pos) {
	horizontalScrollBar()->setValue(scroll_pos);
}

void LogTextBrowser::setScrollY(int scroll_pos) {
	verticalScrollBar()->setValue(scroll_pos);
}

void LogTextBrowser::changeScrollX(int change) {
	QScrollBar *scrollBar = horizontalScrollBar();
	scrollBar->setValue(scrollBar->value() + change);
}

void LogTextBrowser::changeScrollY(int change) {
	QScrollBar *scrollBar = verticalScrollBar();
	scrollBar->setValue(scrollBar->value() + change);
}

void LogTextBrowser::scrollItemIntoView(QRect rect) {
	QRect logRect(getScrollX(), getScrollY(), width(), height());
	int rectXWithWidth     = rect.x() + rect.width();
	int logRectXWithWidth  = logRect.x() + logRect.width();
	int rectYWithHeight    = rect.y() + rect.height();
	int logRectYWithHeight = logRect.y() + logRect.height();

	bool isItemToTheRight  = rectXWithWidth > logRectXWithWidth;
	bool isItemToTheLeft   = rect.x() < logRect.x();
	bool isItemBelow       = rectYWithHeight > logRectYWithHeight;
	bool isItemAbove       = rect.y() < logRect.y();
	bool isItemAtAllInView = rect.x() < logRectXWithWidth && rectXWithWidth > logRect.x()
							 && rect.y() < logRectYWithHeight && rectYWithHeight > logRect.y();

	int offset          = 20;
	bool isWithinWidth  = rect.width() <= logRect.width() - offset;
	bool isWithinHeight = rect.height() <= logRect.height() - offset;
	// Scroll to the item if it fits the log window and is at all out of view. Otherwise only
	// scroll to the item if it is entirely out of view. Both ways scroll just far enough, with the set offset as
	// margin, to show the item at the bottom or top of the log window if the previous position was above or below the
	// item respectively. If the item does not fit the log window then scroll as far as the start or end of the item is
	// still in view if the previous position was entirely above or below the item respectively. The same principles
	// apply horizontally, where below is to the right and above is to the left:
	if ((isWithinWidth && (isItemToTheRight || isItemToTheLeft)) || !isItemAtAllInView) {
		setScrollX((isWithinWidth && isItemToTheRight) || (!isWithinWidth && isItemToTheLeft)
					   ? rectXWithWidth - logRect.width() + offset
					   : rect.x() - offset);
	}
	if ((isWithinHeight && (isItemBelow || isItemAbove)) || !isItemAtAllInView) {
		setScrollY((isWithinHeight && isItemBelow) || (!isWithinHeight && isItemAbove)
					   ? rectYWithHeight - logRect.height() + offset
					   : rect.y() - offset);
	}
	if (getScrollX() == logRect.x() && getScrollY() == logRect.y()) {
		update();
	}
}

bool LogTextBrowser::isScrolledToBottom() {
	const QScrollBar *scrollBar = verticalScrollBar();
	return scrollBar->value() == scrollBar->maximum();
}

void LogTextBrowser::mousePressEvent(QMouseEvent *qme) {
	QPoint mouseDocPos                     = qme->pos();
	Qt::MouseButton mouseButton            = qme->button();
	QAbstractTextDocumentLayout *docLayout = document()->documentLayout();
	// Set the vertical axis of the position to include the scrollable area above it:
	mouseDocPos.setY(mouseDocPos.y() + getScrollY());

	AnimationTextObject::mousePress(docLayout, mouseDocPos, mouseButton);
}

void LogTextBrowser::keyPressEvent(QKeyEvent *qke) {
	Qt::Key key                     = static_cast< Qt::Key >(qke->key());
	Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();
	int logWindowHeight             = height();
	int logInternalHeight           = verticalScrollBar()->maximum();

	int lastItemIndex  = lastCustomInteractiveItemIndex;
	int itemFocusIndex = customInteractiveItemFocusIndex;
	int scrollStep     = 15;
	if (modifiers.testFlag(Qt::ControlModifier)) {
		switch (key) {
			case Qt::Key_A:
				return selectAll();
			case Qt::Key_C:
				return copy();
			case Qt::Key_0:
				return queuedNumericInput.push_back('0');
			case Qt::Key_1:
				return queuedNumericInput.push_back('1');
			case Qt::Key_2:
				return queuedNumericInput.push_back('2');
			case Qt::Key_3:
				return queuedNumericInput.push_back('3');
			case Qt::Key_4:
				return queuedNumericInput.push_back('4');
			case Qt::Key_5:
				return queuedNumericInput.push_back('5');
			case Qt::Key_6:
				return queuedNumericInput.push_back('6');
			case Qt::Key_7:
				return queuedNumericInput.push_back('7');
			case Qt::Key_8:
				return queuedNumericInput.push_back('8');
			case Qt::Key_9:
				return queuedNumericInput.push_back('9');
			default:
				break;
		}
	}
	switch (key) {
		case Qt::Key_Down:
			return changeScrollY(scrollStep);
		case Qt::Key_Up:
			return changeScrollY(-scrollStep);
		case Qt::Key_Right:
			return changeScrollX(15);
		case Qt::Key_Left:
			return changeScrollX(-15);
		case Qt::Key_PageDown:
			return changeScrollY(logWindowHeight);
		case Qt::Key_PageUp:
			return changeScrollY(-logWindowHeight);
		case Qt::Key_Home:
			return setScrollY(0);
		case Qt::Key_End:
			return setScrollY(logInternalHeight);
		case Qt::Key_Return:
		case Qt::Key_Enter:
			customInteractiveItemFocusIndex += itemFocusIndex == lastItemIndex ? -lastItemIndex : 1;
			break;
		case Qt::Key_Backspace:
			customInteractiveItemFocusIndex += itemFocusIndex < 1 ? lastItemIndex + (itemFocusIndex == -1 ? 1 : 0) : -1;
			break;
		case Qt::Key_Escape:
			customInteractiveItemFocusIndex = -1;
			return update();
		default:
			break;
	}
	bool isItemSelectionChanged = customInteractiveItemFocusIndex != itemFocusIndex;

	AnimationTextObject::keyPress(this, isItemSelectionChanged, key);
}

void LogTextBrowser::keyReleaseEvent(QKeyEvent *qke) {
	Qt::Key key = static_cast< Qt::Key >(qke->key());
	if (queuedNumericInput.length() > 0 && key == Qt::Key_Control) {
		int lastItemIndex            = lastCustomInteractiveItemIndex;
		int itemFocusIndex           = queuedNumericInput.toInt();
		bool isItemFocusIndexTooHigh = itemFocusIndex > lastItemIndex;

		customInteractiveItemFocusIndex = isItemFocusIndexTooHigh ? -1 : itemFocusIndex;
		queuedNumericInput.clear();
		if (isItemFocusIndexTooHigh) {
			return update();
		}
		AnimationTextObject::keyPress(this, true, key);
	}
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
					if (emitPastedImage(image, path)) {
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

bool ChatbarTextEdit::emitPastedImage(QImage image, QString filePath) {
	if (filePath.endsWith(".gif")) {
		QFile file(filePath);
		if (file.open(QIODevice::ReadOnly)) {
			QByteArray animationBa(file.readAll());
			file.close();
			QString base64ImageData = qvariant_cast< QString >(animationBa.toBase64());
			emit pastedImage("<br /><img src=\"data:image/GIF;base64," + base64ImageData + "\" />");
		} else {
			Global::get().l->log(Log::Information, tr("Unable to read animated image file: %1").arg(filePath));
		}
		return true;
	}

	QString processedImage = Log::imageToImg(image, static_cast< int >(Global::get().uiImageLength));
	if (processedImage.length() > 0) {
		QString imgHtml = QLatin1String("<br />") + processedImage;
		emit pastedImage(imgHtml);
		return true;
	}
	return false;
}


bool AnimationTextObject::areVideoControlsOn = false;

QString AnimationTextObject::loopModeToString(LoopMode mode) {
	switch (mode) {
		case Unchanged:
			return "Unchanged";
		case Loop:
			return "Loop";
		case NoLoop:
			return "No loop";
		default:
			return "Undefined";
	}
}

void AnimationTextObject::updateVideoControls(QObject *propertyHolder) {
	QRect rect              = propertyHolder->property("posAndSize").toRect();
	int videoControlsHeight = videoBarHeight + underVideoBarHeight;
	int videoControlsY      = rect.y() + rect.height() - videoControlsHeight;
	QRect videoControlsRect(rect.x(), videoControlsY, rect.width(), videoControlsHeight);

	emit Global::get().mw->qteLog->document()->documentLayout()->update(videoControlsRect);
}

void AnimationTextObject::setFrame(QMovie *animation, int frameIndex) {
	int lastFrameIndex       = animation->property("lastFrameIndex").toInt();
	bool isFrameIndexTooLow  = frameIndex < 0;
	bool isFrameIndexTooHigh = frameIndex > lastFrameIndex;
	if (isFrameIndexTooLow || isFrameIndexTooHigh) {
		frameIndex = isFrameIndexTooLow ? 0 : lastFrameIndex;
	}
	if (animation->cacheMode() == QMovie::CacheAll) {
		animation->jumpToFrame(frameIndex);
		return;
	}

	bool wasRunning = animation->state() == QMovie::Running;
	if (!wasRunning) {
		animation->setPaused(false);
	}
	bool isStartTried = false;
	// Can only load the target frame by traversing
	// in sequential order when the frames are not cached:
	while (animation->currentFrameNumber() != frameIndex) {
		if (!animation->jumpToNextFrame()) {
			// Continue traversing animations that either are stopped or do stop after one or more iterations:
			if (animation->state() == QMovie::NotRunning && !isStartTried) {
				animation->start();
				isStartTried = true;
				continue;
			}
			break;
		}
	}
	if (!wasRunning) {
		animation->setPaused(true);
	}
}

void AnimationTextObject::setFrameByProportion(QMovie *animation, double proportion) {
	int msPassedAtProportion = (int) round(proportion * getTotalTime(animation));
	setFrameByTime(animation, msPassedAtProportion);
}

void AnimationTextObject::setFrameByTime(QMovie *animation, int milliseconds) {
	int totalMs            = getTotalTime(animation);
	bool isTimeAfterEnd    = milliseconds > totalMs;
	bool isTimeBeforeStart = milliseconds < 0;
	if (isTimeAfterEnd || isTimeBeforeStart) {
		milliseconds = isTimeAfterEnd ? totalMs : 0;
	}
	QList< QVariant > frameDelays = animation->property("frameDelays").toList();
	qsizetype frameDelayAmount    = frameDelays.length();
	int msUntilCurrentFrame       = 0;

	int frameIndex = 0;
	for (int i = 0; i < frameDelayAmount; ++i) {
		int delay = frameDelays[i].toInt();
		msUntilCurrentFrame += delay;
		if (milliseconds <= msUntilCurrentFrame) {
			bool isNextFrame            = i + 1 < frameDelayAmount;
			int currentFrameDifference  = msUntilCurrentFrame - milliseconds;
			int previousFrameDifference = currentFrameDifference - delay;
			int nextFrameDifference =
				isNextFrame ? msUntilCurrentFrame + frameDelays[i + 1].toInt() - milliseconds : -1;

			bool isPreviousFrameCloser = abs(previousFrameDifference) < currentFrameDifference;
			bool isNextFrameCloser     = isNextFrame ? abs(nextFrameDifference) < currentFrameDifference : false;
			// The first delay has passed by the second frame and so on,
			// hence the index is greater by 1 for the frame of the full delay:
			frameIndex = i + 1 + (isPreviousFrameCloser ? -1 : isNextFrameCloser ? 1 : 0);
			break;
		}
	}
	setFrame(animation, frameIndex);
}

void AnimationTextObject::togglePause(QMovie *animation) {
	int lastFrameIndex            = animation->property("lastFrameIndex").toInt();
	QMovie::MovieState state      = animation->state();
	bool wasStoppedOrNeverStarted = state == QMovie::NotRunning;
	if (animation->speed() < 0) {
		int frameIndex           = animation->currentFrameNumber();
		int frameIndexToSwitchTo = wasStoppedOrNeverStarted && frameIndex == 0 ? lastFrameIndex : frameIndex;
		if (wasStoppedOrNeverStarted) {
			animation->start();
			animation->setPaused(true);
			setFrame(animation, frameIndexToSwitchTo);
		}
		bool wasPlayingInReverse = animation->property("isPlayingInReverse").toBool();
		animation->setProperty("isPlayingInReverse", !wasPlayingInReverse);
		if (wasPlayingInReverse) {
			emit animation->stateChanged(state);
		} else {
			emit animation->frameChanged(frameIndexToSwitchTo);
		}
	} else if (wasStoppedOrNeverStarted) {
		animation->start();
		// Ensure the animation starts on the first attempt to do so:
		animation->setPaused(false);
	} else {
		animation->setPaused(state != QMovie::Paused);
	}
}

void AnimationTextObject::toggleCache(QMovie *animation) {
	int lastFrameIndex               = animation->property("lastFrameIndex").toInt();
	bool wasCached                   = animation->cacheMode() == QMovie::CacheAll;
	QMovie::CacheMode cacheModeToSet = wasCached ? QMovie::CacheNone : QMovie::CacheAll;
	QMovie::MovieState state         = animation->state();
	bool wasPaused                   = state == QMovie::Paused;
	bool wasRunning                  = state == QMovie::Running;

	int previousFrame = animation->currentFrameNumber();
	// Turning caching on or off requires reloading the animation, which is done via `setDevice`,
	// otherwise it will not play properly or dispose of the cache when it is not to be used:
	animation->stop();
	QIODevice *device = animation->device();
	// Prepare the animation to be loaded when starting for the first time:
	device->reset();
	animation->setDevice(device);
	animation->setCacheMode(cacheModeToSet);
	animation->start();

	// Restore the state of the animation playback to what it was before reloading it
	// but ensure it can be resumed when caching is off by pausing it if it is not at the start or end:
	setFrame(animation, previousFrame);
	if (wasPaused || (!wasRunning && previousFrame != 0 && previousFrame != lastFrameIndex)) {
		animation->setPaused(true);
	} else if (!wasRunning) {
		animation->stop();
	}
	updateVideoControls(animation);
}

void AnimationTextObject::stopPlayback(QMovie *animation) {
	animation->stop();
	animation->setProperty("isPlayingInReverse", false);
}

void AnimationTextObject::resetPlayback(QMovie *animation) {
	// Show the first frame that the animation would continue from if started again
	// without caching anyway, indicating that the animation was stopped instead of paused:
	setFrame(animation, 0);
	stopPlayback(animation);
}

void AnimationTextObject::setSpeed(QMovie *animation, int percentage) {
	// Pausing the animation should only be done via the play state to avoid confusion:
	if (percentage == 0) {
		return;
	}
	animation->setSpeed(percentage);
	updateVideoControls(animation);
	// `QMovie` does not itself support reverse playback but this can be and is implemented using it:
	bool wasPlayingInReverse = animation->property("isPlayingInReverse").toBool();
	bool wasRunning          = animation->state() == QMovie::Running || wasPlayingInReverse;
	if (percentage < 0) {
		if (wasPlayingInReverse) {
			return;
		}
		animation->setPaused(true);
		animation->setProperty("isPlayingInReverse", wasRunning);
		if (wasRunning) {
			// Trigger the signal `frameChanged` where the handler also supports reverse playback:
			emit animation->frameChanged(animation->currentFrameNumber());
		}
	} else if (wasPlayingInReverse) {
		animation->setProperty("isPlayingInReverse", false);
		animation->setPaused(!wasRunning);
	}
}

void AnimationTextObject::changeLoopMode(QMovie *animation, int steps) {
	LoopMode loopMode     = qvariant_cast< LoopMode >(animation->property("LoopMode"));
	int loopModeChangedTo = loopMode + steps;
	int loopModeResult =
		loopModeChangedTo > NoLoop ? 0 : loopModeChangedTo < 0 ? static_cast< int >(NoLoop) : loopModeChangedTo;
	animation->setProperty("LoopMode", static_cast< LoopMode >(loopModeResult));
	updateVideoControls(animation);
}

void AnimationTextObject::changeFrame(QMovie *animation, int amount) {
	int lastFrameIndex       = animation->property("lastFrameIndex").toInt();
	int frameIndex           = animation->currentFrameNumber() + amount;
	int amountOfTimesGreater = (int) abs(floor(frameIndex / (double) lastFrameIndex));

	int lastFrameIndexScaledToInput = lastFrameIndex * amountOfTimesGreater;
	int frameIndexWrappedBackward   = lastFrameIndexScaledToInput + 1 + frameIndex;
	int frameIndexWrappedForward    = frameIndex - 1 - lastFrameIndexScaledToInput;
	setFrame(animation, frameIndex < 0 ? frameIndexWrappedBackward
									   : frameIndex > lastFrameIndex ? frameIndexWrappedForward : frameIndex);
}

void AnimationTextObject::changeFrameByTime(QMovie *animation, int milliseconds) {
	setFrameByTime(animation, getCurrentTime(animation, animation->currentFrameNumber()) + milliseconds);
}

void AnimationTextObject::changeSpeed(QMovie *animation, int percentageStep) {
	int speed          = animation->speed();
	int nextPercentage = speed + percentageStep;
	setSpeed(animation, speed + percentageStep * (nextPercentage != 0 ? 1 : 2));
}

int AnimationTextObject::getTotalTime(QObject *propertyHolder) {
	return propertyHolder->property("totalMs").toInt();
}

int AnimationTextObject::getCurrentTime(QObject *propertyHolder, int frameIndex) {
	int lastFrameIndex            = propertyHolder->property("lastFrameIndex").toInt();
	QList< QVariant > frameDelays = propertyHolder->property("frameDelays").toList();
	int msUntilCurrentFrame       = 0;
	// Determine the time until the current frame or the time until the end of the last frame
	// if on the last frame, so as to show a clear time for the start and end:
	for (int i = 0; i < (frameIndex == lastFrameIndex ? frameDelays.length() : frameIndex); ++i) {
		msUntilCurrentFrame += frameDelays[i].toInt();
	}
	return msUntilCurrentFrame;
}

bool AnimationTextObject::isInBoundsOnAxis(QPoint pos, bool yInsteadOfX, int start, int length) {
	int posOnAxis = yInsteadOfX ? pos.y() : pos.x();
	return posOnAxis >= start && posOnAxis <= start + length;
}

bool AnimationTextObject::isInBounds(QPoint pos, QRect bounds) {
	return isInBoundsOnAxis(pos, false, bounds.x(), bounds.width())
		   && isInBoundsOnAxis(pos, true, bounds.y(), bounds.height());
}

int AnimationTextObject::getOffset(int offset, int start, int length) {
	return start + offset + (offset < 0 ? length : 0);
}

void AnimationTextObject::drawVideoControls(QPainter *painter, QObject *propertyHolder, QPixmap frame, bool wasPaused,
											bool wasCached, int frameIndex, int speedPercentage) {
	QRect rect              = propertyHolder->property("posAndSize").toRect();
	int videoControlsHeight = videoBarHeight + underVideoBarHeight;
	QSize viewSizeMin(rect.width(), rect.height() - videoControlsHeight);
	auto getOffsetX     = [rect](int offset) { return rect.x() + offset + (offset < 0 ? rect.width() : 0); };
	int cacheX          = getOffsetX(cacheOffsetX);
	int loopModeX       = getOffsetX(loopModeOffsetX);
	int frameTraversalX = getOffsetX(frameTraversalOffsetX);
	int speedX          = getOffsetX(speedOffsetX);

	int videoBarX                = rect.x();
	int videoBarY                = rect.y() + rect.height() - videoControlsHeight;
	int underVideoBarY           = videoBarY + videoBarHeight;
	int underVideoBarWithMarginY = underVideoBarY + 14;

	auto convertUnit = [](int integer, int exponent, int decimalAmount = 0) -> double {
		bool noDecimals              = decimalAmount == 0;
		int exponentForDecimalAmount = exponent < 0 ? exponent + decimalAmount : exponent - decimalAmount;

		double product = integer * pow(10, exponentForDecimalAmount);
		return noDecimals ? product : round(product) / (double) pow(10, decimalAmount);
	};
	auto padDecimals = [](QString numberStr, int decimalAmount) -> QString {
		qsizetype decimalMarkIndex     = numberStr.lastIndexOf('.');
		bool isDecimal                 = decimalMarkIndex != -1 && decimalMarkIndex < numberStr.length() - 1;
		qsizetype currentDecimalAmount = isDecimal ? numberStr.sliced(decimalMarkIndex + 1).length() : 0;
		qsizetype decimalFillerAmount  = decimalAmount - currentDecimalAmount;

		QString decimalFillers = QString('0').repeated(decimalFillerAmount);
		return decimalFillerAmount > 0 ? numberStr.append((!isDecimal ? "." : "") + decimalFillers) : numberStr;
	};
	auto padNumber = [](QString numberStr, int digitAmount) -> QString {
		qsizetype numberLength                = numberStr.length();
		qsizetype decimalMarkIndex            = numberStr.lastIndexOf('.');
		qsizetype decimalsIncludingMarkLength = decimalMarkIndex != -1 ? numberLength - decimalMarkIndex : 0;
		return numberStr.rightJustified(digitAmount + decimalsIncludingMarkLength, '0');
	};
	auto formatTime = [padDecimals, padNumber](double seconds, double totalSeconds = 0) -> QString {
		auto getTimeNumbers = [](double seconds) -> QList< double > {
			auto floorDivision   = [](double dividend, double divisor = 60) { return (int) floor(dividend / divisor); };
			int minutes          = floorDivision(seconds);
			int hours            = floorDivision(minutes);
			int remainingMinutes = std::max(minutes - hours * 60, 0);
			double remainingSeconds = std::max< double >(seconds - minutes * 60, 0);
			return QList< double >({ remainingSeconds, (double) remainingMinutes, (double) hours });
		};
		auto getDigitAmount = [](int number) -> int {
			int digitAmount = 0;
			do {
				number /= 10;
				++digitAmount;
			} while (number != 0);
			return digitAmount;
		};

		QList< double > timeNumbers      = getTimeNumbers(seconds);
		QList< double > totalTimeNumbers = totalSeconds == 0 ? timeNumbers : getTimeNumbers(totalSeconds);
		int timeNumberAmount             = (int) timeNumbers.length();
		int decimalAmount                = 1;

		int lastTimeNumberIndex = 0;
		for (int i = timeNumberAmount - 1; i >= 0; --i) {
			if (totalTimeNumbers[i] > 0) {
				lastTimeNumberIndex = i;
				break;
			}
		}

		QString timeStr;
		for (int i = 0; i < timeNumberAmount; ++i) {
			double number         = timeNumbers[i];
			bool isSeconds        = i == 0;
			bool isLastNumber     = i == lastTimeNumberIndex;
			bool hasAnotherNumber = i < lastTimeNumberIndex;
			if (number == 0 && !hasAnotherNumber && !isLastNumber) {
				break;
			}
			QString numberStr = QString::number(number);
			if (hasAnotherNumber || isLastNumber) {
				int digitAmount = isLastNumber ? getDigitAmount((int) totalTimeNumbers[i]) : 2;
				numberStr       = padNumber(numberStr, digitAmount);
			}
			timeStr.prepend(isSeconds ? padDecimals(numberStr, decimalAmount) : numberStr.append(":"));
		}
		return timeStr;
	};

	QList< QVariant > frameDelays = propertyHolder->property("frameDelays").toList();
	int totalMs                   = getTotalTime(propertyHolder);
	int msUntilCurrentFrame       = getCurrentTime(propertyHolder, frameIndex);
	// Convert to seconds rounded to one decimal:
	double totalS   = convertUnit(totalMs, -3, 1);
	double currentS = convertUnit(msUntilCurrentFrame, -3, 1);

	painter->drawPixmap(QRect(rect.topLeft(), viewSizeMin), frame);
	painter->fillRect(videoBarX, videoBarY, viewSizeMin.width(), videoControlsHeight, QBrush(QColor(50, 50, 50, 180)));

	double videoBarProgress = msUntilCurrentFrame / (double) totalMs;
	QBrush videoBarBrush(QColor(0, 0, 200));
	painter->fillRect(videoBarX, videoBarY, viewSizeMin.width(), 4, QBrush(QColor(90, 90, 90, 180)));
	painter->fillRect(videoBarX, videoBarY, (int) round(viewSizeMin.width() * videoBarProgress), 4, videoBarBrush);

	painter->setPen(QColor(149, 165, 166));
	QString speedStr = padDecimals(QString::number(convertUnit(speedPercentage, -2)), 2);
	QPoint speedPos(speedX, underVideoBarWithMarginY);
	painter->drawText(speedPos, speedStr);
	// Draw the plus "+":
	painter->drawLine(speedPos.x() - 9, speedPos.y() - 11, speedPos.x() - 9, speedPos.y() - 3);
	painter->drawLine(speedPos.x() - 13, speedPos.y() - 7, speedPos.x() - 5, speedPos.y() - 7);
	// Draw the minus "-":
	painter->drawLine(speedPos.x() - 13, speedPos.y() + 2, speedPos.x() - 5, speedPos.y() + 2);
	// Draw the circle "o":
	painter->drawEllipse(speedPos.x() - 26, speedPos.y() - 6, 6, 6);

	QPoint frameTraversalPos(frameTraversalX, underVideoBarWithMarginY);
	painter->drawText(frameTraversalPos, "<  >");

	LoopMode loopMode           = qvariant_cast< LoopMode >(propertyHolder->property("LoopMode"));
	QString loopModeStr         = loopModeToString(loopMode);
	QFont font                  = painter->font();
	qsizetype loopModeStrLength = loopModeStr.length();
	int loopModeStrOffset       = loopModeStrLength > 7 ? 12 : loopModeStrLength > 4 ? 5 : 0;
	double fontSizeSmall        = 0.7;
	font.setPointSize((int) round(font.pointSize() * fontSizeSmall));
	painter->setFont(font);
	painter->drawText(QPointF(loopModeX, underVideoBarY + 8), "mode:");
	painter->drawText(QPointF(loopModeX - (double) loopModeStrOffset, underVideoBarY + 17), loopModeStr);

	QString cachedStr = QString::fromStdString(wasCached ? "on" : "off");
	painter->drawText(QPointF(cacheX, underVideoBarY + 8), "cache:");
	painter->drawText(QPointF(cacheX + 5, underVideoBarY + 17), cachedStr);
	font.setPointSize((int) round(font.pointSize() / fontSizeSmall));
	painter->setFont(font);

	QString totalTimeStr   = formatTime(totalS);
	QString currentTimeStr = formatTime(currentS, totalS);
	painter->drawText(QPoint(videoBarX + 20, underVideoBarWithMarginY),
					  tr("%1 / %2").arg(currentTimeStr).arg(totalTimeStr));

	QPointF iconTopPos(videoBarX + 2, underVideoBarY + 2);
	if (wasPaused) {
		// Add a play-icon, which is a right-pointing triangle, like this "▶":
		QPolygonF polygon(
			{ iconTopPos, QPointF(videoBarX + 15, underVideoBarY + 10), QPointF(videoBarX + 2, underVideoBarY + 18) });
		QPainterPath path;
		path.addPolygon(polygon);
		painter->fillPath(path, QBrush(Qt::white));
	} else {
		// Add a pause-icon, which is two vertical rectangles next to each other, like this "||":
		QSize pauseBarSize(4, 16);
		QBrush brush(Qt::white);
		painter->fillRect(QRect(iconTopPos.toPoint(), pauseBarSize), brush);
		painter->fillRect(QRect(QPoint(videoBarX + 11, underVideoBarY + 2), pauseBarSize), brush);
	}
}

AnimationTextObject::VideoController AnimationTextObject::mousePressVideoControls(QObject *propertyHolder,
																				  QPoint mouseDocPos) {
	QRect rect              = propertyHolder->property("posAndSize").toRect();
	int videoControlsHeight = videoBarHeight + underVideoBarHeight;
	QSize viewSizeMin(rect.width(), rect.height() - videoControlsHeight);
	auto getOffsetX     = [rect](int offset) { return rect.x() + offset + (offset < 0 ? rect.width() : 0); };
	int cacheX          = getOffsetX(cacheOffsetX);
	int loopModeX       = getOffsetX(loopModeOffsetX);
	int frameTraversalX = getOffsetX(frameTraversalOffsetX);
	int speedX          = getOffsetX(speedOffsetX);

	auto isThisInBoundsOnAxis = [mouseDocPos](bool yInsteadOfX, int start, int length) {
		return isInBoundsOnAxis(mouseDocPos, yInsteadOfX, start, length);
	};
	auto isThisInBounds = [mouseDocPos](QRect bounds) { return isInBounds(mouseDocPos, bounds); };
	int videoControlsY  = rect.y() + rect.height() - videoControlsHeight;
	int underVideoBarY  = videoControlsY + videoBarHeight;

	QRect viewRect(rect.topLeft(), viewSizeMin);
	QRect playPauseRect(rect.x(), underVideoBarY, 15, underVideoBarHeight);

	QRect cacheRect(cacheX, underVideoBarY, 25, underVideoBarHeight);
	QRect loopModeRect(loopModeX, underVideoBarY, 24, underVideoBarHeight);

	int frameTraversalWidth = 12;
	QRect framePreviousRect(frameTraversalX, underVideoBarY, frameTraversalWidth, underVideoBarHeight);
	QRect frameNextRect(frameTraversalX + frameTraversalWidth + 2, underVideoBarY, frameTraversalWidth,
						underVideoBarHeight);

	int speedWidth       = 9;
	int speedMinusHeight = 9;
	int speedPlusHeight  = 11;
	QRect speedResetRect(speedX - 28, underVideoBarY, speedWidth, underVideoBarHeight);
	QRect speedMinusRect(speedX - 14, underVideoBarY + speedPlusHeight, speedWidth, speedMinusHeight);
	QRect speedPlusRect(speedX - 14, underVideoBarY, speedWidth, speedPlusHeight);

	if (!isThisInBoundsOnAxis(false, viewRect.x(), viewRect.width()))
		return None;
	if (isThisInBoundsOnAxis(true, viewRect.y(), viewRect.height()))
		return View;
	if (isThisInBoundsOnAxis(true, videoControlsY, videoBarHeight))
		return VideoBar;
	if (isThisInBounds(playPauseRect))
		return PlayPause;
	if (isThisInBounds(cacheRect))
		return CacheSwitch;
	if (isThisInBounds(loopModeRect))
		return LoopSwitch;
	if (isThisInBounds(framePreviousRect))
		return PreviousFrame;
	if (isThisInBounds(frameNextRect))
		return NextFrame;
	if (isThisInBounds(speedResetRect))
		return ResetSpeed;
	if (isThisInBounds(speedMinusRect))
		return DecreaseSpeed;
	if (isThisInBounds(speedPlusRect))
		return IncreaseSpeed;
	return None;
}

void AnimationTextObject::mousePress(QAbstractTextDocumentLayout *docLayout, QPoint mouseDocPos,
									 Qt::MouseButton mouseButton) {
	QTextFormat baseFmt = docLayout->formatAt(mouseDocPos);
	if (!baseFmt.isCharFormat() || baseFmt.objectType() != Log::Animation) {
		return;
	}
	QMovie *animation = qvariant_cast< QMovie * >(baseFmt.toCharFormat().property(1));
	QRect rect        = animation->property("posAndSize").toRect();
	// Ensure the selection was within the text object's vertical space,
	// such as if an inline image is not as tall as the content before it:
	if (!isInBoundsOnAxis(mouseDocPos, true, rect.y(), rect.height())) {
		return;
	}

	auto setFrameByVideoBarSelection = [animation, mouseDocPos, rect]() {
		double videoBarPercentage = (mouseDocPos.x() - rect.x()) / (double) rect.width();
		setFrameByProportion(animation, videoBarPercentage);
	};
	bool isLeftMouseButtonPressed   = mouseButton == Qt::LeftButton;
	bool isMiddleMouseButtonPressed = mouseButton == Qt::MiddleButton;
	if (areVideoControlsOn) {
		VideoController videoController = mousePressVideoControls(animation, mouseDocPos);
		if (isLeftMouseButtonPressed) {
			switch (videoController) {
				case VideoBar:
					return setFrameByVideoBarSelection();
				case View:
				case PlayPause:
					return togglePause(animation);
				case CacheSwitch:
					return toggleCache(animation);
				case LoopSwitch:
					return changeLoopMode(animation, 1);
				case PreviousFrame:
					return changeFrame(animation, -1);
				case NextFrame:
					return changeFrame(animation, 1);
				case ResetSpeed:
					return setSpeed(animation, 100);
				case DecreaseSpeed:
					return changeSpeed(animation, -10);
				case IncreaseSpeed:
					return changeSpeed(animation, 10);
				default:
					return;
			}
		} else if (isMiddleMouseButtonPressed) {
			switch (videoController) {
				case View:
				case PlayPause:
					return resetPlayback(animation);
				case LoopSwitch:
					return changeLoopMode(animation, -1);
				case PreviousFrame:
					return changeFrame(animation, -5);
				case NextFrame:
					return changeFrame(animation, 5);
				case DecreaseSpeed:
					return changeSpeed(animation, -50);
				case IncreaseSpeed:
					return changeSpeed(animation, 50);
				default:
					return;
			}
		}
		return;
	}
	if (isLeftMouseButtonPressed) {
		togglePause(animation);
	} else if (isMiddleMouseButtonPressed) {
		resetPlayback(animation);
	}
	// Right mouse button shows the context menu for the text object,
	// which is handled where the custom context menu for the log is.
}

void AnimationTextObject::keyPress(LogTextBrowser *log, bool isItemSelectionChanged, Qt::Key key) {
	bool isItemAtIndex        = false;
	int itemIndex             = log->customInteractiveItemFocusIndex;
	QMovie *animation         = nullptr;
	QList< QTextFormat > fmts = log->document()->allFormats();
	for (auto fmt : fmts) {
		if (fmt.objectType() != Log::Animation) {
			continue;
		}
		animation     = qvariant_cast< QMovie * >(fmt.property(1));
		isItemAtIndex = itemIndex == animation->property("customInteractiveItemIndex").toInt();
		if (isItemAtIndex) {
			break;
		}
	}
	if (!isItemAtIndex) {
		return;
	}

	bool isKeyBoundToAction         = true;
	Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();
	if (modifiers.testFlag(Qt::NoModifier) || modifiers.testFlag(Qt::KeypadModifier)) {
		switch (key) {
			case Qt::Key_Space:
			case Qt::Key_K:
				togglePause(animation);
				break;
			case Qt::Key_Q:
				resetPlayback(animation);
				break;
			case Qt::Key_V:
				areVideoControlsOn = !areVideoControlsOn;
				break;
			case Qt::Key_C:
				toggleCache(animation);
				break;
			case Qt::Key_O:
				changeLoopMode(animation, -1);
				break;
			case Qt::Key_L:
				changeLoopMode(animation, 1);
				break;
			case Qt::Key_Comma:
				changeFrame(animation, -1);
				break;
			case Qt::Key_Period:
				changeFrame(animation, 1);
				break;
			case Qt::Key_N:
				changeFrame(animation, -5);
				break;
			case Qt::Key_M:
				changeFrame(animation, 5);
				break;
			case Qt::Key_H:
				changeFrameByTime(animation, -1000);
				break;
			case Qt::Key_J:
				changeFrameByTime(animation, 1000);
				break;
			case Qt::Key_F:
				changeFrameByTime(animation, -5000);
				break;
			case Qt::Key_G:
				changeFrameByTime(animation, 5000);
				break;
			case Qt::Key_S:
			case Qt::Key_Minus:
				changeSpeed(animation, -5);
				break;
			case Qt::Key_D:
			case Qt::Key_Plus:
				changeSpeed(animation, 5);
				break;
			case Qt::Key_W:
				changeSpeed(animation, -25);
				break;
			case Qt::Key_E:
				changeSpeed(animation, 25);
				break;
			case Qt::Key_R:
				setSpeed(animation, 100);
				break;
			case Qt::Key_0:
				setFrameByProportion(animation, 0);
				break;
			case Qt::Key_1:
				setFrameByProportion(animation, 0.1);
				break;
			case Qt::Key_2:
				setFrameByProportion(animation, 0.2);
				break;
			case Qt::Key_3:
				setFrameByProportion(animation, 0.3);
				break;
			case Qt::Key_4:
				setFrameByProportion(animation, 0.4);
				break;
			case Qt::Key_5:
				setFrameByProportion(animation, 0.5);
				break;
			case Qt::Key_6:
				setFrameByProportion(animation, 0.6);
				break;
			case Qt::Key_7:
				setFrameByProportion(animation, 0.7);
				break;
			case Qt::Key_8:
				setFrameByProportion(animation, 0.8);
				break;
			case Qt::Key_9:
				setFrameByProportion(animation, 0.9);
				break;
			default:
				isKeyBoundToAction = false;
				break;
		}
	} else {
		isKeyBoundToAction = false;
	}
	if (isKeyBoundToAction || isItemSelectionChanged) {
		log->scrollItemIntoView(animation->property("posAndSize").toRect());
	}
}

void AnimationTextObject::drawCenteredPlayIcon(QPainter *painter, QRect rect) {
	int centerX = (int) round(rect.x() + rect.width() / (double) 2);
	int centerY = (int) round(rect.y() + rect.height() / (double) 2);
	// Add a play-icon, which is a right-pointing triangle, like this "▶":
	QPolygonF polygon(
		{ QPointF(centerX - 8, centerY - 10), QPointF(centerX + 12, centerY), QPointF(centerX - 8, centerY + 10) });
	QPainterPath path;
	QPen thinBlackPen(Qt::black, 0.25);
	path.addPolygon(polygon);
	painter->fillPath(path, QBrush(Qt::white));
	// Add outline contrast to the triangle:
	painter->strokePath(path, thinBlackPen);

	auto drawCenteredCircle = [painter, centerX, centerY](int diameter) {
		int radius = diameter / 2;
		painter->drawEllipse(centerX - radius, centerY - radius, diameter, diameter);
	};
	// Add a ring around the triangle:
	painter->setPen(QPen(Qt::white, 2));
	drawCenteredCircle(40);
	// Add outline contrast to the ring:
	painter->setPen(thinBlackPen);
	drawCenteredCircle(36);
	drawCenteredCircle(44);
}

void AnimationTextObject::updatePropertyRect(QObject *propertyHolder, QRect rect) {
	QVariant propertyPosAndSize = propertyHolder->property("posAndSize");
	QRect propertyRect          = propertyPosAndSize.isValid() ? propertyPosAndSize.toRect() : QRect();
	// Update the property for the position and size of the animation if it has not been set before,
	// if its width changes when video controls are switched on or off,
	// if its vertical position changes, such as if content above it has its height altered by text wrapping,
	// or if its horizontal position changes, such as if the window resizes when the animation is centered
	// and dependent on available blank space:
	if (propertyRect.width() != rect.width() || propertyRect.y() != rect.y() || propertyRect.x() != rect.x()) {
		propertyHolder->setProperty("posAndSize", QVariant(rect));
	}
}

AnimationTextObject::AnimationTextObject() : QObject() {
}

QSizeF AnimationTextObject::intrinsicSize(QTextDocument *, int, const QTextFormat &fmt) {
	QMovie *animation = qvariant_cast< QMovie * >(fmt.property(1));
	QSize size        = animation->currentPixmap().size();
	if (areVideoControlsOn) {
		int videoControlsHeight = videoBarHeight + underVideoBarHeight;
		int viewWidthMin        = size.width() - videoControlsHeight;
		size.setWidth(viewWidthMin);
	}
	return QSizeF(size);
}

void AnimationTextObject::drawObject(QPainter *painter, const QRectF &rectF, QTextDocument *doc, int,
									 const QTextFormat &fmt) {
	LogTextBrowser *log = qobject_cast< LogTextBrowser * >(doc->parent());
	QMovie *animation   = qvariant_cast< QMovie * >(fmt.property(1));
	QRect rect          = rectF.toRect();
	QPixmap frame       = animation->currentPixmap();
	bool wasRunning     = animation->state() == QMovie::Running || animation->property("isPlayingInReverse").toBool();
	updatePropertyRect(animation, rect);

	painter->setRenderHint(QPainter::Antialiasing);
	if (log->customInteractiveItemFocusIndex == animation->property("customInteractiveItemIndex").toInt()) {
		painter->setPen(QPen(QColor(50, 50, 200), 2));
		painter->drawRect(QRect(rect.x() - 1, rect.y() - 1, rect.width() + 2, rect.height() + 2));
	}
	if (areVideoControlsOn) {
		bool wasCached      = animation->cacheMode() == QMovie::CacheAll;
		int frameIndex      = animation->currentFrameNumber();
		int speedPercentage = animation->speed();
		drawVideoControls(painter, animation, frame, !wasRunning, wasCached, frameIndex, speedPercentage);
		return;
	}
	painter->drawPixmap(rect, frame);
	if (!wasRunning) {
		drawCenteredPlayIcon(painter, rect);
	}
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
	foreach (ClientUser *usr, ClientUser::c_qmUsers) { qlsUsernames.append(usr->qsName); }
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

			foreach (QString name, qlsUsernames) {
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

		foreach (ClientUser *usr, ClientUser::c_qmUsers) {
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
