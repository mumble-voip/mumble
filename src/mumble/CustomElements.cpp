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
	widgetInFullScreen = nullptr;
}

QRect LogTextBrowser::calcRectScaledToScreen(const QSize &size) {
	QSize screenSize = QGuiApplication::primaryScreen()->size();
	QSize sizeScaled = size.scaled(screenSize, Qt::KeepAspectRatio);
	QPoint centerPadding((int) round((screenSize.width() - sizeScaled.width()) / (double) 2),
						 (int) round((screenSize.height() - sizeScaled.height()) / (double) 2));
	return QRect(centerPadding, sizeScaled);
}

void LogTextBrowser::toggleFullScreen(QWidget *widget) {
	bool isPreviousWidgetInFullScreen = isWidgetInFullScreen();
	if (widget == nullptr) {
		if (!isPreviousWidgetInFullScreen) {
			return;
		}
		widget = widgetInFullScreen;
	}
	if (isPreviousWidgetInFullScreen) {
		QWidget *wgtFs                 = widgetInFullScreen;
		bool isGivenWidgetInFullScreen = widget == wgtFs;
		wgtFs->close();
		if (isGivenWidgetInFullScreen) {
			return;
		}
	}

	connect(widget, &QWidget::destroyed, this, [this]() { widgetInFullScreen = nullptr; });
	widget->setAttribute(Qt::WA_DeleteOnClose);
	widget->setWindowFlag(Qt::Window);
	widget->showFullScreen();
	widgetInFullScreen = widget;
}

bool LogTextBrowser::isWidgetInFullScreen() {
	return widgetInFullScreen != nullptr;
}

void LogTextBrowser::highlightSelectedItem(QPainter *painter, const QRect &rect, QObject *propertyHolder) {
	if (customItemFocusIndex != propertyHolder->property("customItemIndex").toInt()) {
		return;
	}
	painter->setPen(QPen(QColor(50, 50, 200), 2));
	painter->drawRect(rect.adjusted(-1, -1, 1, 1));
}

void LogTextBrowser::clear() {
	for (QObject *item : customItems) {
		item->deleteLater();
	}
	lastCustomItemIndex  = -1;
	customItemFocusIndex = -1;
	customItems.clear();
	QTextBrowser::clear();
}

void LogTextBrowser::update(const QRect &rect) {
	document()->documentLayout()->update(!rect.isEmpty() ? rect : getLogRect());
}

int LogTextBrowser::getScrollX() {
	return horizontalScrollBar()->value();
}

int LogTextBrowser::getScrollY() {
	return verticalScrollBar()->value();
}

QPoint LogTextBrowser::getScrollPos() {
	return QPoint(getScrollX(), getScrollY());
}

QRect LogTextBrowser::getLogRect() {
	return QRect(getScrollPos(), size());
}

void LogTextBrowser::setScrollX(int scrollPos) {
	horizontalScrollBar()->setValue(scrollPos);
}

void LogTextBrowser::setScrollY(int scrollPos) {
	verticalScrollBar()->setValue(scrollPos);
}

void LogTextBrowser::changeScrollX(int change) {
	QScrollBar *scrollBar = horizontalScrollBar();
	scrollBar->setValue(scrollBar->value() + change);
}

void LogTextBrowser::changeScrollY(int change) {
	QScrollBar *scrollBar = verticalScrollBar();
	scrollBar->setValue(scrollBar->value() + change);
}

void LogTextBrowser::scrollItemIntoView(const QRect &rect) {
	QRect logRect          = getLogRect();
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
	if (getScrollPos() == logRect.topLeft()) {
		update();
	}
}

bool LogTextBrowser::isScrolledToBottom() {
	const QScrollBar *scrollBar = verticalScrollBar();
	return scrollBar->value() == scrollBar->maximum();
}

void LogTextBrowser::mousePressEvent(QMouseEvent *mouseEvt) {
	QPoint mouseDocPos          = mouseEvt->pos() + getScrollPos();
	Qt::MouseButton mouseButton = mouseEvt->button();

	QTextFormat fmt = document()->documentLayout()->formatAt(mouseDocPos);
	if (!fmt.hasProperty(1)) {
		return;
	}
	QObject *obj = qvariant_cast< QObject * >(fmt.property(1));
	QRect rect   = obj->property("posAndSize").toRect();
	// Ensure the selection was within the text object's vertical space,
	// such as if an inline animation is not as tall as the content before it:
	if (!VideoUtils::isInBoundsOnAxis(mouseDocPos.y(), rect.y(), rect.height())) {
		return;
	}

	switch (qvariant_cast< Log::TextObjectType >(fmt.objectType())) {
		case Log::TextObjectType::Animation: {
			QMovie *animation = qobject_cast< QMovie * >(obj);
			AnimationTextObject::mousePress(animation, mouseDocPos, mouseButton);
			break;
		}
		case Log::TextObjectType::NoCustomObject:
			break;
	}
}

void LogTextBrowser::keyPressEvent(QKeyEvent *keyEvt) {
	Qt::Key key                     = static_cast< Qt::Key >(keyEvt->key());
	Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();
	int logWindowHeight             = height();
	int logInternalHeight           = verticalScrollBar()->maximum();

	int lastItemIndex  = lastCustomItemIndex;
	int itemFocusIndex = customItemFocusIndex;
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
			return changeScrollX(scrollStep);
		case Qt::Key_Left:
			return changeScrollX(-scrollStep);
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
			if (lastItemIndex != -1) {
				customItemFocusIndex += itemFocusIndex == lastItemIndex ? -lastItemIndex : 1;
			}
			break;
		case Qt::Key_Backspace:
			if (lastItemIndex != -1) {
				customItemFocusIndex += itemFocusIndex < 1 ? lastItemIndex + (itemFocusIndex == -1 ? 1 : 0) : -1;
			}
			break;
		case Qt::Key_Escape:
			customItemFocusIndex = -1;
			return update();
		default:
			break;
	}
	if (customItemFocusIndex == -1) {
		return;
	}
	bool isItemSelectionChanged = customItemFocusIndex != itemFocusIndex;

	QObject *item = customItems[customItemFocusIndex];
	switch (qvariant_cast< Log::TextObjectType >(item->property("objectType"))) {
		case Log::TextObjectType::Animation: {
			QMovie *animation = qobject_cast< QMovie * >(item);
			AnimationTextObject::keyPress(animation, key, isItemSelectionChanged);
			break;
		}
		case Log::TextObjectType::NoCustomObject:
			break;
	}
}

void LogTextBrowser::keyReleaseEvent(QKeyEvent *keyEvt) {
	Qt::Key key = static_cast< Qt::Key >(keyEvt->key());
	if (queuedNumericInput.size() == 0 || key != Qt::Key_Control) {
		return;
	}

	bool isItemFocusIndex;
	int itemFocusIndex           = queuedNumericInput.toInt(&isItemFocusIndex);
	bool isItemFocusIndexTooHigh = isItemFocusIndex && itemFocusIndex > lastCustomItemIndex;

	customItemFocusIndex = !isItemFocusIndex ? -1 : isItemFocusIndexTooHigh ? lastCustomItemIndex : itemFocusIndex;
	queuedNumericInput.clear();
	if (isItemFocusIndex && customItemFocusIndex != -1) {
		QObject *item = customItems[customItemFocusIndex];
		return scrollItemIntoView(item->property("posAndSize").toRect());
	}
	update();
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
					}
				}

				return count > 0;
			}
		} else {
			Global::get().l->log(Log::Information, tr("This server does not allow sending images."));
		}
	}
	return false;
}

bool ChatbarTextEdit::emitPastedImage(QImage image, QString filePath) {
	qsizetype fileExtStartIndex = filePath.lastIndexOf('.') + 1;
	QString fileExt             = (fileExtStartIndex != 0 ? filePath.sliced(fileExtStartIndex) : "").toLower();

	Log::TextObjectType txtObjType = Log::findTxtObjType(fileExt);
	if (txtObjType == Log::TextObjectType::Animation) {
		bool isAnimation = true;
		QFile file(filePath);
		if (file.open(QIODevice::ReadOnly)) {
			QByteArray animationBa(file.readAll());
			file.close();
			AnimationTextObject::createAnimation(animationBa, nullptr, isAnimation);
			if (isAnimation) {
				QString base64ImageData = qvariant_cast< QString >(animationBa.toBase64());
				QString img = QLatin1String("<img src=\"data:image/%2;base64,%1\" />").arg(base64ImageData, fileExt);
				emit pastedImage("<br/>" + img);
			}
		} else {
			Global::get().l->log(Log::Information, tr("Unable to read animated image file: %1").arg(filePath));
			return false;
		}
		if (isAnimation) {
			return true;
		}
	}

	int maxImageSize = static_cast< int >(Global::get().uiImageLength);
	QString img      = !fileExt.isEmpty() ? Log::imageToImg(image, maxImageSize, fileExt.toUtf8())
									 : Log::imageToImg(image, maxImageSize);
	if (img.size() > 0) {
		emit pastedImage("<br/>" + img);
		return true;
	}
	Global::get().l->log(Log::Information, tr("Unable to send image %1: too large.").arg(filePath));
	return false;
}


bool VideoUtils::isInBoundsOnAxis(int posOnAxis, int start, int length) {
	return posOnAxis >= start && posOnAxis <= start + length;
}

bool VideoUtils::isInBounds(const QPoint &pos, const QRect &bounds) {
	return isInBoundsOnAxis(pos.x(), bounds.x(), bounds.width())
		   && isInBoundsOnAxis(pos.y(), bounds.y(), bounds.height());
}

int VideoUtils::calcPosOnAxisFromOffset(int offset, int start, int length) {
	return start + offset + (offset < 0 ? length : 0);
}

QRect VideoUtils::calcVideoControlsRect(QObject *propertyHolder) {
	QRect rect         = propertyHolder->property("posAndSize").toRect();
	int videoControlsY = rect.y() + rect.height() - videoControlsHeight;
	return QRect(rect.x(), videoControlsY, rect.width(), videoControlsHeight);
}

VideoUtils::VideoControl VideoUtils::videoControlAt(const QPoint &pos, const QRect &rect) {
	auto calcXFromOffset = [&rect](int offsetX) { return calcPosOnAxisFromOffset(offsetX, rect.x(), rect.width()); };
	int cacheX           = calcXFromOffset(cacheOffsetX);
	int loopModeX        = calcXFromOffset(loopModeOffsetX);
	int frameTraversalX  = calcXFromOffset(frameTraversalOffsetX);
	int speedX           = calcXFromOffset(speedOffsetX);
	int fullScreenX      = calcXFromOffset(fullScreenOffsetX);

	auto isThisInBounds = [&pos](const QRect &bounds) { return isInBounds(pos, bounds); };
	int videoControlsY  = rect.y() + rect.height() - videoControlsHeight;
	int underVideoBarY  = videoControlsY + videoBarHeight;

	QSize viewSize(rect.width(), rect.height() - videoControlsHeight);
	QRect viewRect(rect.topLeft(), viewSize);
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
	QRect speedResetRect(speedX - 2, underVideoBarY, speedWidth, underVideoBarHeight);
	QRect speedMinusRect(speedX + 12, underVideoBarY + speedPlusHeight, speedWidth, speedMinusHeight);
	QRect speedPlusRect(speedX + 12, underVideoBarY, speedWidth, speedPlusHeight);

	int fullScreenWidth = 15;
	QRect fullScreenRect(fullScreenX, underVideoBarY, fullScreenWidth, underVideoBarHeight);

	if (!isInBoundsOnAxis(pos.x(), viewRect.x(), viewRect.width()))
		return VideoControl::None;
	if (isInBoundsOnAxis(pos.y(), viewRect.y(), viewRect.height()))
		return VideoControl::View;
	if (isInBoundsOnAxis(pos.y(), videoControlsY, videoBarHeight))
		return VideoControl::VideoBar;
	if (isThisInBounds(playPauseRect))
		return VideoControl::PlayPause;
	if (isThisInBounds(cacheRect))
		return VideoControl::Cache;
	if (isThisInBounds(loopModeRect))
		return VideoControl::Loop;
	if (isThisInBounds(framePreviousRect))
		return VideoControl::PreviousFrame;
	if (isThisInBounds(frameNextRect))
		return VideoControl::NextFrame;
	if (isThisInBounds(speedResetRect))
		return VideoControl::ResetSpeed;
	if (isThisInBounds(speedMinusRect))
		return VideoControl::DecreaseSpeed;
	if (isThisInBounds(speedPlusRect))
		return VideoControl::IncreaseSpeed;
	if (isThisInBounds(fullScreenRect))
		return VideoControl::FullScreen;
	return VideoControl::None;
}

double VideoUtils::videoBarProportionAt(const QPoint &pos, const QRect &rect) {
	return (pos.x() - rect.x()) / (double) rect.width();
}

void VideoUtils::drawVideoControls(QPainter *painter, const QRect &rect, QObject *propertyHolder, double opacity) {
	auto convertUnit = [](int integer, int exponent, int decimalAmount = 0) -> double {
		bool noDecimals              = decimalAmount == 0;
		int exponentForDecimalAmount = exponent < 0 ? exponent + decimalAmount : exponent - decimalAmount;

		double product = integer * pow(10, exponentForDecimalAmount);
		return noDecimals ? product : round(product) / (double) pow(10, decimalAmount);
	};
	auto padDecimals = [](QString numberStr, int decimalAmount) -> QString {
		qsizetype decimalMarkIndex     = numberStr.lastIndexOf('.');
		bool isDecimal                 = decimalMarkIndex != -1 && decimalMarkIndex < numberStr.size() - 1;
		qsizetype currentDecimalAmount = isDecimal ? numberStr.sliced(decimalMarkIndex + 1).size() : 0;
		qsizetype decimalFillerAmount  = decimalAmount - currentDecimalAmount;

		QString decimalFillers = QString('0').repeated(decimalFillerAmount);
		return decimalFillerAmount > 0 ? numberStr.append((!isDecimal ? "." : "") + decimalFillers) : numberStr;
	};
	auto padNumber = [](QString numberStr, int digitAmount) -> QString {
		qsizetype numberLength                = numberStr.size();
		qsizetype decimalMarkIndex            = numberStr.lastIndexOf('.');
		qsizetype decimalsIncludingMarkLength = decimalMarkIndex != -1 ? numberLength - decimalMarkIndex : 0;
		return numberStr.rightJustified(digitAmount + decimalsIncludingMarkLength, '0');
	};
	auto formatTime = [&padDecimals, &padNumber](double seconds, double totalSeconds = 0) -> QString {
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
		int timeNumberAmount             = (int) timeNumbers.size();
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

	Log::TextObjectType objType = qvariant_cast< Log::TextObjectType >(propertyHolder->property("objectType"));
	bool isFullScreen           = propertyHolder->property("isFullScreen").toBool();
	auto calcXFromOffset = [&rect](int offsetX) { return calcPosOnAxisFromOffset(offsetX, rect.x(), rect.width()); };
	int cacheX           = calcXFromOffset(cacheOffsetX);
	int loopModeX        = calcXFromOffset(loopModeOffsetX);
	int frameTraversalX  = calcXFromOffset(frameTraversalOffsetX);
	int speedX           = calcXFromOffset(speedOffsetX);
	int fullScreenX      = calcXFromOffset(fullScreenOffsetX);

	int videoBarX                      = rect.x();
	int videoBarY                      = rect.y() + rect.height() - videoControlsHeight;
	int underVideoBarY                 = videoBarY + videoBarHeight;
	int underVideoBarWithSmallMarginY  = underVideoBarY + 3;
	int underVideoBarWithMediumMarginY = underVideoBarY + 14;

	bool wasPaused      = false;
	bool wasCached      = false;
	int speedPercentage = -1;
	int totalMs         = -1;
	int currentMs       = -1;
	switch (objType) {
		case Log::TextObjectType::Animation: {
			QMovie *animation = qobject_cast< QMovie * >(propertyHolder);
			int frameIndex    = animation->currentFrameNumber();
			wasPaused = animation->state() != QMovie::Running && !animation->property("isPlayingInReverse").toBool();
			wasCached = animation->cacheMode() == QMovie::CacheAll;
			speedPercentage = animation->speed();
			totalMs         = AnimationTextObject::getTotalTime(propertyHolder);
			currentMs       = AnimationTextObject::getCurrentTime(propertyHolder, frameIndex);
			break;
		}
		default:
			break;
	}
	// Convert to seconds rounded to one decimal:
	double totalS   = convertUnit(totalMs, -3, 1);
	double currentS = convertUnit(currentMs, -3, 1);

	int alpha               = (int) round(255 * opacity);
	int alphaFsVcBg         = isFullScreen ? (int) round(255 * 0.85) : 255;
	double videoBarProgress = currentMs / (double) totalMs;
	int videoBarWidth       = (int) round(rect.width() * videoBarProgress);
	QBrush videoControlsBackgroundBrush(QColor(50, 50, 50, isFullScreen ? std::min(alpha, alphaFsVcBg) : alpha));
	QBrush videoBarBackgroundBrush(wasCached ? QColor(120, 120, 120, alpha) : QColor(90, 90, 90, alpha));
	QBrush videoBarBrush(QColor(0, 0, 200, alpha));
	painter->fillRect(videoBarX, videoBarY, rect.width(), videoControlsHeight, videoControlsBackgroundBrush);
	painter->fillRect(videoBarX, videoBarY, rect.width(), videoBarHeight, videoBarBackgroundBrush);
	painter->fillRect(videoBarX, videoBarY, videoBarWidth, videoBarHeight, videoBarBrush);

	painter->setPen(QColor(149, 165, 166, alpha));
	QPoint fullScreenPos(fullScreenX, underVideoBarWithSmallMarginY);
	int cornerLength                 = 4;
	int cornerGap                    = 6;
	int nextCornerStart              = cornerLength + cornerGap;
	int nextCornerEnd                = nextCornerStart + cornerLength;
	QList< QPolygon > fullScreenIcon = {
		QPolygon({ QPoint(0, cornerLength), QPoint(), QPoint(cornerLength, 0) }),
		QPolygon({ QPoint(nextCornerEnd, cornerLength), QPoint(nextCornerEnd, 0), QPoint(nextCornerStart, 0) }),
		QPolygon({ QPoint(0, nextCornerStart), QPoint(0, nextCornerEnd), QPoint(cornerLength, nextCornerEnd) }),
		QPolygon({ QPoint(nextCornerEnd, nextCornerStart), QPoint(nextCornerEnd, nextCornerEnd),
				   QPoint(nextCornerStart, nextCornerEnd) })
	};
	// Draw the square corners "⛶", with the edges towards the inside when in full screen and vice versa:
	for (int i = 0; i < fullScreenIcon.size(); ++i) {
		QPolygon &corner = fullScreenIcon[i];
		for (QPoint &point : corner) {
			point += fullScreenPos;
		}
		if (isFullScreen) {
			QPoint offset(i == 0 || i == 2 ? cornerLength : -cornerLength, i < 2 ? cornerLength : -cornerLength);
			corner[1] += offset;
		}
		painter->drawPolyline(corner);
	}

	QString speedStr = padDecimals(QString::number(convertUnit(speedPercentage, -2)), 2);
	QPoint speedPos(speedX, underVideoBarWithSmallMarginY);
	painter->drawText(speedPos.x() + 26, speedPos.y() + 11, speedStr);
	// Draw the plus "+":
	painter->drawLine(speedPos.x() + 17, speedPos.y(), speedPos.x() + 17, speedPos.y() + 8);
	painter->drawLine(speedPos.x() + 13, speedPos.y() + 4, speedPos.x() + 21, speedPos.y() + 4);
	// Draw the minus "-":
	painter->drawLine(speedPos.x() + 13, speedPos.y() + 13, speedPos.x() + 21, speedPos.y() + 13);
	// Draw the circle "o":
	painter->drawEllipse(speedPos.x(), speedPos.y() + 5, 6, 6);

	QPoint frameTraversalPos(frameTraversalX, underVideoBarWithMediumMarginY);
	painter->drawText(frameTraversalPos, "<  >");

	QFont font = painter->font();
	switch (objType) {
		case Log::TextObjectType::Animation: {
			auto loopMode       = qvariant_cast< AnimationTextObject::LoopMode >(propertyHolder->property("LoopMode"));
			QString loopModeStr = AnimationTextObject::loopModeToString(loopMode);
			qsizetype loopModeStrLength = loopModeStr.size();
			int loopModeStrOffset       = loopModeStrLength > 7 ? 12 : loopModeStrLength > 4 ? 5 : 0;
			double fontSizeSmall        = 0.7;
			font.setPointSize((int) round(font.pointSize() * fontSizeSmall));
			painter->setFont(font);
			painter->drawText(QPoint(loopModeX, underVideoBarY + 8), "Mode:");
			painter->drawText(QPoint(loopModeX - loopModeStrOffset, underVideoBarY + 17), loopModeStr);

			painter->drawText(QPoint(cacheX, underVideoBarY + 8), "Cache:");
			painter->drawText(QPoint(cacheX + 5, underVideoBarY + 17), QLatin1String(wasCached ? "On" : "Off"));
			font.setPointSize((int) round(font.pointSize() / fontSizeSmall));
			painter->setFont(font);
		}
		default:
			break;
	}

	QString totalTimeStr   = formatTime(totalS);
	QString currentTimeStr = formatTime(currentS, totalS);
	double fontSizeLarge   = 1.2;
	if (isFullScreen) {
		font.setPointSize((int) round(font.pointSize() * fontSizeLarge));
		painter->setFont(font);
	}
	painter->drawText(QPoint(videoBarX + 20, underVideoBarWithMediumMarginY + (isFullScreen ? 1 : 0)),
					  QLatin1String("%1 / %2").arg(currentTimeStr, totalTimeStr));
	if (isFullScreen) {
		font.setPointSize((int) round(font.pointSize() / fontSizeLarge));
		painter->setFont(font);
	}

	QPoint iconTopPos(videoBarX + 2, underVideoBarY + 2);
	QBrush playIconBrush(QColor(255, 255, 255, alpha));
	if (wasPaused) {
		// Add a play-icon, which is a right-pointing triangle, like this "▶":
		QPolygon playIcon(
			{ iconTopPos, QPoint(videoBarX + 15, underVideoBarY + 10), QPoint(videoBarX + 2, underVideoBarY + 18) });
		painter->setPen(Qt::NoPen);
		painter->setBrush(playIconBrush);
		painter->drawConvexPolygon(playIcon);
	} else {
		// Add a pause-icon, which is two vertical rectangles next to each other, like this "||":
		QSize pauseBarSize(4, 16);
		painter->fillRect(QRect(iconTopPos, pauseBarSize), playIconBrush);
		painter->fillRect(QRect(QPoint(iconTopPos.x() + 9, iconTopPos.y()), pauseBarSize), playIconBrush);
	}
}

void VideoUtils::drawCenteredPlayIcon(QPainter *painter, const QRect &rect) {
	int centerX = (int) round(rect.x() + rect.width() / (double) 2);
	int centerY = (int) round(rect.y() + rect.height() / (double) 2);

	double proportion =
		rect.width() > 149 && rect.height() > 149 ? 1 : rect.width() > 49 && rect.height() > 49 ? 0.5 : 0.25;
	int vertexLeftOffsetX  = (int) round(8 * proportion);
	int vertexLeftOffsetY  = (int) round(10 * proportion);
	int vertexRightOffsetX = (int) round(12 * proportion);
	int ringDiameter       = (int) round(40 * proportion);
	int ringOutlineOffset  = (int) round(4 * proportion);
	qreal ringWidth        = (qreal)(2 * proportion);

	QPolygon playIcon({ QPoint(centerX - vertexLeftOffsetX, centerY - vertexLeftOffsetY),
						QPoint(centerX + vertexRightOffsetX, centerY),
						QPoint(centerX - vertexLeftOffsetX, centerY + vertexLeftOffsetY) });
	painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush(Qt::white));
	painter->drawConvexPolygon(playIcon);
	painter->setBrush(Qt::NoBrush);
	// Add outline contrast to the triangle:
	painter->setPen(QPen(Qt::black, 0.3));
	painter->drawConvexPolygon(playIcon);

	auto drawCenteredCircle = [painter, centerX, centerY](int diameter) {
		int radius = diameter / 2;
		painter->drawEllipse(centerX - radius, centerY - radius, diameter, diameter);
	};
	// Add a ring around the triangle:
	painter->setPen(QPen(Qt::white, ringWidth));
	drawCenteredCircle(ringDiameter);
	// Add outline contrast to the ring:
	painter->setPen(QPen(Qt::black, 0.25));
	drawCenteredCircle(ringDiameter - ringOutlineOffset);
	drawCenteredCircle(ringDiameter + ringOutlineOffset);
}

void VideoUtils::addVideoControlsTransition(QObject *propertyHolder, QWidget *area, bool isIdleCursorHidden) {
	static const int transitionTimerInterval = 25;
	static const double proportionPerChange  = 0.2;
	QTimer *transitionTimer                  = new QTimer(area);

	propertyHolder->setProperty("isVideoControlsTransitionOn", false);
	propertyHolder->setProperty("isVideoControlsFadeIn", true);
	propertyHolder->setProperty("videoControlsOpacity", (double) 0);
	QObject::connect(transitionTimer, &QTimer::timeout, area, [propertyHolder, area, isIdleCursorHidden]() {
		if (!propertyHolder->property("isVideoControlsTransitionOn").toBool()) {
			return;
		}
		bool isFadeIn = propertyHolder->property("isVideoControlsFadeIn").toBool();
		if (!isFadeIn) {
			int delay = propertyHolder->property("videoControlsRevertDelay").toInt();
			if (delay == -1) {
				return;
			}
			if (delay > 0) {
				propertyHolder->setProperty("videoControlsRevertDelay", delay - transitionTimerInterval);
				return;
			}
		}

		double opacity = propertyHolder->property("videoControlsOpacity").toDouble();
		opacity += isFadeIn ? proportionPerChange : -proportionPerChange;
		propertyHolder->setProperty("videoControlsOpacity", opacity);
		if (opacity >= 1 || opacity <= 0) {
			propertyHolder->setProperty("isVideoControlsFadeIn", !isFadeIn);
			if (isFadeIn) {
				propertyHolder->setProperty("videoControlsRevertDelay", videoControlsRevertDelay);
			} else {
				propertyHolder->setProperty("isVideoControlsTransitionOn", false);
				if (isIdleCursorHidden) {
					area->setCursor(Qt::BlankCursor);
				}
			}
		}
		updateVideoControls(propertyHolder, area);
	});
	transitionTimer->start(transitionTimerInterval);
	if (isIdleCursorHidden) {
		area->setCursor(Qt::BlankCursor);
	}
}

void VideoUtils::startOrHoldVideoControlsTransition(QObject *propertyHolder, const QPoint &hoveredPos,
													QWidget *cursorArea) {
	double opacity               = propertyHolder->property("videoControlsOpacity").toDouble();
	bool areVideoControlsHovered = isInBounds(hoveredPos, calcVideoControlsRect(propertyHolder));
	if (opacity >= 1) {
		int revertDelay = !areVideoControlsHovered ? videoControlsRevertDelay : -1;
		propertyHolder->setProperty("videoControlsRevertDelay", revertDelay);
	} else {
		propertyHolder->setProperty("isVideoControlsTransitionOn", true);
	}
	if (cursorArea != nullptr) {
		cursorArea->unsetCursor();
	}
}

void VideoUtils::updateVideoControls(QObject *propertyHolder, QWidget *area) {
	QRect videoControlsRect = calcVideoControlsRect(propertyHolder);
	int padding             = 2;
	videoControlsRect.adjust(0, -padding, 0, padding);
	if (area != nullptr) {
		area->update(videoControlsRect);
	} else {
		LogTextBrowser *log = qobject_cast< LogTextBrowser * >(propertyHolder->parent());
		log->update(videoControlsRect);
	}
}

bool VideoUtils::updatePropertyRect(QObject *propertyHolder, const QRect &rect) {
	QVariant propertyRect   = propertyHolder->property("posAndSize");
	bool wasRectInitialized = propertyRect.isValid();
	QRect posAndSize        = wasRectInitialized ? propertyRect.toRect() : QRect();
	if (posAndSize != rect) {
		propertyHolder->setProperty("posAndSize", rect);
	}
	return wasRectInitialized;
}

QSizeF VideoUtils::calcIntrinsicSize(QObject *propertyHolder, const QSize &size, bool areVideoControlsOn) {
	// The max layout size is set by its first value, so start with the largest value:
	int maxWidth = size.width() < VideoUtils::videoBarMinWidth ? VideoUtils::videoBarMinWidth : size.width();
	int minWidth = size.width() - VideoUtils::videoControlsHeight;

	bool isMinWidthTooSmall = minWidth < videoBarMinWidth;
	int videoControlsWidth  = isMinWidthTooSmall ? videoBarMinWidth : minWidth;
	int maxHeight           = size.height() + (isMinWidthTooSmall ? videoBarMinWidth - minWidth : 0);

	bool isPosAndSizeInitialized = propertyHolder->property("posAndSize").isValid();
	return QSizeF(!isPosAndSizeInitialized ? QSize(maxWidth, maxHeight)
										   : areVideoControlsOn ? QSize(videoControlsWidth, maxHeight) : size);
}

void VideoUtils::setAttributesWidthAndHeight(QObject *propertyHolder, QSize &size) {
	QVariant propertyWidth  = propertyHolder->property("overrideWidth");
	QVariant propertyHeight = propertyHolder->property("overrideHeight");
	bool isWidth            = propertyWidth.isValid();
	bool isHeight           = propertyHeight.isValid();
	int minScaledSize       = size.width() > 49 && size.height() > 49 ? 50 : 1;
	if (isWidth) {
		int width = propertyWidth.toInt();
		if (!isHeight) {
			size.setHeight(std::max(size.height() + width - size.width(), minScaledSize));
		}
		size.setWidth(width);
	}
	if (isHeight) {
		int height = propertyHeight.toInt();
		if (!isWidth) {
			size.setWidth(std::max(size.width() + height - size.height(), minScaledSize));
		}
		size.setHeight(height);
	}
}


bool AnimationTextObject::areVideoControlsOn = false;

QObject *AnimationTextObject::createAnimation(const QByteArray &animationBa, LogTextBrowser *parent,
											  bool &isAnimationCheckOnly) {
	QMovie *animation = new QMovie(parent);
	QBuffer *buffer   = new QBuffer(animation);
	buffer->setData(animationBa);
	buffer->open(QIODevice::ReadOnly);
	animation->setDevice(buffer);
	if (!animation->isValid()) {
		isAnimationCheckOnly = false;
		delete animation;
		return nullptr;
	}
	// Load and start the animation but stop or pause it after this when it should not play by default:
	animation->start();

	int frameCount     = animation->frameCount();
	int frameCountTest = 0;
	int totalMs        = 0;
	QList< QVariant > frameDelays;
	// Test how many frames there are by index in case the animation format does not support `frameCount`.
	// Also determine the total play time used for the video controls by gathering the time from each frame.
	// The current time is determined by a list of the time between frames since each delay until the next
	// frame may vary:
	while (animation->jumpToFrame(++frameCountTest)) {
		int delay = animation->nextFrameDelay();
		frameDelays.append(QVariant(delay));
		totalMs += delay;
	}
	if (frameCount == 0) {
		frameCount = frameCountTest;
	}
	bool isAnimation = frameCount > 1;
	if (!isAnimation || isAnimationCheckOnly) {
		isAnimationCheckOnly = isAnimation;
		delete animation;
		return nullptr;
	}
	int lastFrameIndex = frameCount - 1;
	animation->setProperty("lastFrameIndex", QVariant(lastFrameIndex));
	animation->setProperty("totalMs", QVariant(totalMs));
	animation->setProperty("frameDelays", frameDelays);
	animation->jumpToFrame(0);
	animation->stop();

	animation->setProperty("LoopMode", QVariant::fromValue(LoopMode::Unchanged));
	// Track reverse playback instead of using the in-built controls which do not support it:
	animation->setProperty("isPlayingInReverse", false);
	// Block further signals during sequential traversal until reaching the preceding frame:
	animation->setProperty("isTraversingFrames", false);
	animation->setProperty("isFullScreen", false);
	qsizetype frameDelayAmount = frameDelays.size();
	auto refresh               = [animation, parent]() {
        if (animation->property("isFullScreen").toBool()) {
            return;
        }
        QRect rect = animation->property("posAndSize").toRect();
        parent->update(rect);
	};
	auto getLoopMode = [animation]() { return qvariant_cast< LoopMode >(animation->property("LoopMode")); };
	// Refresh the image on change:
	connect(animation, &QMovie::updated, refresh);
	// Refresh the image once more when the animation is paused or stopped:
	connect(animation, &QMovie::stateChanged, [refresh](QMovie::MovieState currentState) {
		if (currentState != QMovie::Running) {
			refresh();
		}
	});
	// Start the animation again when it finishes if the loop mode is `Loop`:
	connect(animation, &QMovie::finished, [animation, getLoopMode]() {
		if (getLoopMode() == LoopMode::Loop) {
			animation->start();
		}
	});
	// Stop the animation at the end of the last frame if the loop mode is `NoLoop` and
	// play the animation in reverse if the property for it is `true`:
	connect(animation, &QMovie::frameChanged,
			[animation, getLoopMode, frameDelays, frameDelayAmount, lastFrameIndex](int frameIndex) {
				auto getFrameDelay = [animation, frameDelays, frameDelayAmount](int targetFrameIndex) -> int {
					double speed                 = abs(animation->speed() / (double) 100);
					bool isIndexInBoundsForDelay = targetFrameIndex >= 0 && targetFrameIndex < frameDelayAmount;
					return (int) round(
						frameDelays[isIndexInBoundsForDelay ? targetFrameIndex : frameDelayAmount - 1].toInt() / speed);
				};
				auto isAtFrameAndRunning = [animation](int targetFrameIndex) -> bool {
					int currentFrameIndex = animation->currentFrameNumber();
					bool wasRunning =
						animation->state() == QMovie::Running || animation->property("isPlayingInReverse").toBool();
					return currentFrameIndex == targetFrameIndex && wasRunning;
				};
				auto isAtFrameAndRunningWithNoLoop = [getLoopMode, isAtFrameAndRunning](int targetFrameIndex) {
					return isAtFrameAndRunning(targetFrameIndex) && getLoopMode() == LoopMode::NoLoop;
				};
				auto stopAtEndOfCurrentFrame = [animation, isAtFrameAndRunningWithNoLoop, getFrameDelay, frameIndex]() {
					int delay = getFrameDelay(frameIndex);
					QTimer::singleShot(delay, Qt::PreciseTimer, animation,
									   [animation, isAtFrameAndRunningWithNoLoop, frameIndex]() {
										   if (!isAtFrameAndRunningWithNoLoop(frameIndex)) {
											   return;
										   }
										   setFrame(animation, frameIndex);
										   stopPlayback(animation);
									   });
				};

				if (animation->property("isPlayingInReverse").toBool()) {
					if (animation->property("isTraversingFrames").toBool()) {
						return;
					}
					if (isAtFrameAndRunningWithNoLoop(0)) {
						stopAtEndOfCurrentFrame();
						return;
					}
					int precedingFrameIndex = frameIndex <= 0 ? lastFrameIndex : frameIndex - 1;
					int precedingFrameDelay = getFrameDelay(precedingFrameIndex);
					QTimer::singleShot(precedingFrameDelay, Qt::PreciseTimer, animation,
									   [animation, isAtFrameAndRunning, frameIndex, precedingFrameIndex]() {
										   if (!isAtFrameAndRunning(frameIndex)) {
											   return;
										   }
										   bool wasCached = animation->cacheMode() == QMovie::CacheAll;
										   if (!wasCached) {
											   animation->setProperty("isTraversingFrames", true);
										   }
										   setFrame(animation, precedingFrameIndex);
										   if (!wasCached) {
											   animation->setProperty("isTraversingFrames", false);
											   emit animation->frameChanged(precedingFrameIndex);
										   }
									   });
				} else if (isAtFrameAndRunningWithNoLoop(lastFrameIndex)) {
					stopAtEndOfCurrentFrame();
				}
			});
	return animation;
}

QObject *AnimationTextObject::createAnimation(const QByteArray &animationBa, LogTextBrowser *parent) {
	bool isAnimationCheckOnly = false;
	return createAnimation(animationBa, parent, isAnimationCheckOnly);
}

QString AnimationTextObject::loopModeToString(LoopMode mode) {
	switch (mode) {
		case LoopMode::Unchanged:
			return "Unchanged";
		case LoopMode::Loop:
			return "Loop";
		case LoopMode::NoLoop:
			return "No loop";
	}
	return "Undefined";
}

void AnimationTextObject::toggleVideoControls() {
	areVideoControlsOn = !areVideoControlsOn;
}

void AnimationTextObject::toggleVideoControlsFullScreen(QObject *propertyHolder) {
	bool areVideoControlsOnFullScreen = propertyHolder->property("areVideoControlsOnFullScreen").toBool();
	propertyHolder->setProperty("areVideoControlsOnFullScreen", !areVideoControlsOnFullScreen);
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
			// Continue traversing the animation if it either is stopped or does stop after one or more iterations:
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

void AnimationTextObject::setFrameByTime(QMovie *animation, int milliseconds) {
	int totalMs            = getTotalTime(animation);
	bool isTimeAfterEnd    = milliseconds > totalMs;
	bool isTimeBeforeStart = milliseconds < 0;
	if (isTimeAfterEnd || isTimeBeforeStart) {
		milliseconds = isTimeAfterEnd ? totalMs : 0;
	}
	QList< QVariant > frameDelays = animation->property("frameDelays").toList();
	qsizetype frameDelayAmount    = frameDelays.size();
	int msUntilCurrentFrame       = 0;

	int frameIndex = 0;
	for (int i = 0; i < frameDelayAmount; ++i) {
		int delay = frameDelays[i].toInt();
		msUntilCurrentFrame += delay;
		if (msUntilCurrentFrame < milliseconds) {
			continue;
		}

		bool isNextFrame            = i + 1 < frameDelayAmount;
		int currentFrameDifference  = msUntilCurrentFrame - milliseconds;
		int previousFrameDifference = currentFrameDifference - delay;
		int nextFrameDifference = isNextFrame ? msUntilCurrentFrame + frameDelays[i + 1].toInt() - milliseconds : -1;

		bool isPreviousFrameCloser = abs(previousFrameDifference) < currentFrameDifference;
		bool isNextFrameCloser     = isNextFrame ? abs(nextFrameDifference) < currentFrameDifference : false;
		// The first delay has passed by the second frame and so on,
		// hence the index is greater by 1 for the frame of the full delay:
		frameIndex = i + 1 + (isPreviousFrameCloser ? -1 : isNextFrameCloser ? 1 : 0);
		break;
	}
	setFrame(animation, frameIndex);
}

void AnimationTextObject::setFrameByProportion(QMovie *animation, double proportion) {
	int msPassedAtProportion = (int) round(proportion * getTotalTime(animation));
	setFrameByTime(animation, msPassedAtProportion);
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
	int lastFrameIndex                  = animation->property("lastFrameIndex").toInt();
	bool wasCached                      = animation->cacheMode() == QMovie::CacheAll;
	QMovie::CacheMode previousCacheMode = wasCached ? QMovie::CacheNone : QMovie::CacheAll;
	QMovie::MovieState state            = animation->state();
	bool wasPaused                      = state == QMovie::Paused;
	bool wasRunning                     = state == QMovie::Running;

	int previousFrame = animation->currentFrameNumber();
	// Turning caching on or off requires reloading the animation, which is done via `setDevice`,
	// otherwise it will not play properly or dispose of the cache when it is not to be used:
	QIODevice *device = animation->device();
	// Prepare the animation to be loaded when starting for the first time:
	device->reset();
	animation->setDevice(device);
	animation->setCacheMode(previousCacheMode);
	animation->start();

	// Restore the state of the animation playback to what it was before reloading it
	// but ensure it can be resumed when caching is off by pausing it if it is not at the start or end:
	setFrame(animation, previousFrame);
	if (wasPaused || (!wasRunning && previousFrame != 0 && previousFrame != lastFrameIndex)) {
		animation->setPaused(true);
	} else if (!wasRunning) {
		animation->stop();
	}
	VideoUtils::updateVideoControls(animation);
}

void AnimationTextObject::toggleFullScreen(QMovie *animation) {
	LogTextBrowser *log = qobject_cast< LogTextBrowser * >(animation->parent());
	QWidget *fsAnim     = log->isWidgetInFullScreen() ? nullptr : new FullScreenAnimation(animation, log);
	log->toggleFullScreen(fsAnim);
}

void AnimationTextObject::escapeFullScreen(QMovie *animation) {
	LogTextBrowser *log = qobject_cast< LogTextBrowser * >(animation->parent());
	if (log->isWidgetInFullScreen()) {
		log->toggleFullScreen(nullptr);
	}
}

void AnimationTextObject::stopPlayback(QMovie *animation) {
	animation->stop();
	animation->setProperty("isPlayingInReverse", false);
}

void AnimationTextObject::resetPlayback(QMovie *animation) {
	// Show the first frame that the animation would continue from if started again
	// without caching anyway, indicating that the animation was reset instead of paused:
	setFrame(animation, 0);
	stopPlayback(animation);
}

void AnimationTextObject::setSpeed(QMovie *animation, int percentage) {
	// Pausing the animation should only be done via the play state to avoid confusion:
	if (percentage == 0) {
		return;
	}
	animation->setSpeed(percentage);
	VideoUtils::updateVideoControls(animation);
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

void AnimationTextObject::resetSpeed(QMovie *animation) {
	QVariant propertySpeedAtPreviousSpeedReset = animation->property("speedAtPreviousSpeedReset");
	bool isSpeedPreviouslyReset                = propertySpeedAtPreviousSpeedReset.isValid();
	int currentSpeed                           = animation->speed();

	bool isOriginalSpeed = currentSpeed == 100;
	int previousSpeed    = isOriginalSpeed && isSpeedPreviouslyReset ? propertySpeedAtPreviousSpeedReset.toInt() : 100;
	if (!isOriginalSpeed) {
		animation->setProperty("speedAtPreviousSpeedReset", currentSpeed);
	}
	setSpeed(animation, isOriginalSpeed ? previousSpeed : 100);
}

void AnimationTextObject::invertSpeed(QMovie *animation) {
	setSpeed(animation, animation->speed() * -1);
}

void AnimationTextObject::changeSpeed(QMovie *animation, int percentageStep) {
	int speed          = animation->speed();
	int nextPercentage = speed + percentageStep;
	setSpeed(animation, speed + percentageStep * (nextPercentage != 0 ? 1 : 2));
}

void AnimationTextObject::setLoopMode(QMovie *animation, LoopMode mode) {
	animation->setProperty("LoopMode", QVariant::fromValue(mode));
	VideoUtils::updateVideoControls(animation);
}

void AnimationTextObject::changeLoopMode(QMovie *animation, int steps) {
	LoopMode loopMode     = qvariant_cast< LoopMode >(animation->property("LoopMode"));
	int lastLoopModeInt   = static_cast< int >(LoopMode::NoLoop);
	int loopModeChangedTo = static_cast< int >(loopMode) + steps;
	int loopModeResult =
		loopModeChangedTo > lastLoopModeInt ? 0 : loopModeChangedTo < 0 ? lastLoopModeInt : loopModeChangedTo;
	setLoopMode(animation, static_cast< LoopMode >(loopModeResult));
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
	for (int i = 0; i < (frameIndex == lastFrameIndex ? frameDelays.size() : frameIndex); ++i) {
		msUntilCurrentFrame += frameDelays[i].toInt();
	}
	return msUntilCurrentFrame;
}

void AnimationTextObject::mousePress(QMovie *animation, const QPoint &mouseDocPos, const Qt::MouseButton &mouseButton) {
	bool isFullScreen                 = animation->property("isFullScreen").toBool();
	bool areVideoControlsOnFullScreen = animation->property("areVideoControlsOnFullScreen").toBool();
	bool isLeftMouseButtonPressed     = mouseButton == Qt::LeftButton;
	bool isMiddleMouseButtonPressed   = mouseButton == Qt::MiddleButton;
	if ((!isFullScreen && !areVideoControlsOn) || (isFullScreen && !areVideoControlsOnFullScreen)) {
		if (isLeftMouseButtonPressed) {
			togglePause(animation);
		} else if (isMiddleMouseButtonPressed) {
			resetPlayback(animation);
		}
		// Right mouse button shows the context menu for the text object,
		// which is handled where the custom context menu for the log is.
		return;
	}

	QRect rect                            = animation->property("posAndSize").toRect();
	VideoUtils::VideoControl videoControl = VideoUtils::videoControlAt(mouseDocPos, rect);
	if (isLeftMouseButtonPressed) {
		switch (videoControl) {
			case VideoUtils::VideoControl::VideoBar:
				return setFrameByProportion(animation, VideoUtils::videoBarProportionAt(mouseDocPos, rect));
			case VideoUtils::VideoControl::View:
			case VideoUtils::VideoControl::PlayPause:
				return togglePause(animation);
			case VideoUtils::VideoControl::Cache:
				return toggleCache(animation);
			case VideoUtils::VideoControl::Loop:
				return changeLoopMode(animation, 1);
			case VideoUtils::VideoControl::PreviousFrame:
				return changeFrame(animation, -1);
			case VideoUtils::VideoControl::NextFrame:
				return changeFrame(animation, 1);
			case VideoUtils::VideoControl::ResetSpeed:
				return resetSpeed(animation);
			case VideoUtils::VideoControl::DecreaseSpeed:
				return changeSpeed(animation, -10);
			case VideoUtils::VideoControl::IncreaseSpeed:
				return changeSpeed(animation, 10);
			case VideoUtils::VideoControl::FullScreen:
				return toggleFullScreen(animation);
			case VideoUtils::VideoControl::None:
				return;
		}
	} else if (isMiddleMouseButtonPressed) {
		switch (videoControl) {
			case VideoUtils::VideoControl::View:
			case VideoUtils::VideoControl::PlayPause:
				return resetPlayback(animation);
			case VideoUtils::VideoControl::Loop:
				return changeLoopMode(animation, -1);
			case VideoUtils::VideoControl::PreviousFrame:
				return changeFrame(animation, -5);
			case VideoUtils::VideoControl::NextFrame:
				return changeFrame(animation, 5);
			case VideoUtils::VideoControl::DecreaseSpeed:
				return changeSpeed(animation, -50);
			case VideoUtils::VideoControl::IncreaseSpeed:
				return changeSpeed(animation, 50);
			default:
				return;
		}
	}
}

void AnimationTextObject::keyPress(QMovie *animation, const Qt::Key &key, bool isItemSelectionChanged) {
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
				if (animation->property("isFullScreen").toBool()) {
					toggleVideoControlsFullScreen(animation);
				} else {
					toggleVideoControls();
				}
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
			case Qt::Key_B:
				changeFrame(animation, -5);
				break;
			case Qt::Key_N:
				changeFrame(animation, 5);
				break;
			case Qt::Key_H:
				changeFrameByTime(animation, -1000);
				break;
			case Qt::Key_J:
				changeFrameByTime(animation, 1000);
				break;
			case Qt::Key_U:
				changeFrameByTime(animation, -5000);
				break;
			case Qt::Key_I:
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
				resetSpeed(animation);
				break;
			case Qt::Key_X:
				invertSpeed(animation);
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
			case Qt::Key_F:
				toggleFullScreen(animation);
				break;
			case Qt::Key_Escape:
				escapeFullScreen(animation);
				break;
			default:
				isKeyBoundToAction = false;
				break;
		}
	} else {
		isKeyBoundToAction = false;
	}
	if (isKeyBoundToAction || isItemSelectionChanged) {
		LogTextBrowser *log = qobject_cast< LogTextBrowser * >(animation->parent());
		log->scrollItemIntoView(animation->property("posAndSize").toRect());
	}
}

AnimationTextObject::AnimationTextObject() : QObject() {
}

QSizeF AnimationTextObject::intrinsicSize(QTextDocument *, int, const QTextFormat &fmt) {
	QMovie *animation = qvariant_cast< QMovie * >(fmt.property(1));
	QSize size        = animation->currentPixmap().size();
	VideoUtils::setAttributesWidthAndHeight(animation, size);
	return VideoUtils::calcIntrinsicSize(animation, size, areVideoControlsOn);
}

void AnimationTextObject::drawObject(QPainter *painter, const QRectF &rectF, QTextDocument *doc, int,
									 const QTextFormat &fmt) {
	QMovie *animation   = qvariant_cast< QMovie * >(fmt.property(1));
	LogTextBrowser *log = qobject_cast< LogTextBrowser * >(doc->parent());
	QRect rect          = rectF.toRect();
	if (!animation->property("isFullScreen").toBool() && !VideoUtils::updatePropertyRect(animation, rect)) {
		return log->update(rect);
	}
	QPixmap frame   = animation->currentPixmap();
	bool wasRunning = animation->state() == QMovie::Running || animation->property("isPlayingInReverse").toBool();

	painter->setRenderHint(QPainter::Antialiasing);
	log->highlightSelectedItem(painter, rect, animation);
	painter->drawPixmap(areVideoControlsOn ? rect.adjusted(0, 0, 0, -VideoUtils::videoControlsHeight) : rect, frame);
	if (areVideoControlsOn) {
		VideoUtils::drawVideoControls(painter, rect, animation);
	} else if (!wasRunning) {
		VideoUtils::drawCenteredPlayIcon(painter, rect);
	}
}


FullScreenAnimation::FullScreenAnimation(QMovie *animation, LogTextBrowser *parent) : QLabel(parent) {
	// Sets the animation viewer to fill all available space:
	setScaledContents(true);
	setMovie(animation);

	if (!animation->property("areVideoControlsOnFullScreen").isValid()) {
		animation->setProperty("areVideoControlsOnFullScreen", true);
	}
	animation->setProperty("isFullScreen", true);
	connect(this, &FullScreenAnimation::destroyed, animation, [animation]() {
		animation->setProperty("isFullScreen", false);
		animation->setProperty("baseSize", {});
	});
	connect(animation, &QMovie::stateChanged, this, [animation, this]() {
		VideoUtils::updateVideoControls(animation, this);
		VideoUtils::startOrHoldVideoControlsTransition(animation);
	});
	VideoUtils::addVideoControlsTransition(animation, this);
}

void FullScreenAnimation::mousePressEvent(QMouseEvent *mouseEvt) {
	QMovie *animation     = movie();
	QPoint widgetClickPos = mouseEvt->pos();
	AnimationTextObject::mousePress(animation, widgetClickPos, mouseEvt->button());
	VideoUtils::updateVideoControls(animation, this);
	VideoUtils::startOrHoldVideoControlsTransition(animation, widgetClickPos, this);
}

void FullScreenAnimation::mouseMoveEvent(QMouseEvent *mouseEvt) {
	VideoUtils::startOrHoldVideoControlsTransition(movie(), mouseEvt->pos(), this);
}

void FullScreenAnimation::keyPressEvent(QKeyEvent *keyEvt) {
	QMovie *animation = movie();
	AnimationTextObject::keyPress(animation, static_cast< Qt::Key >(keyEvt->key()));
	VideoUtils::updateVideoControls(animation, this);
	VideoUtils::startOrHoldVideoControlsTransition(animation);
}

void FullScreenAnimation::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	QMovie *animation = movie();
	QPixmap frame     = animation->currentPixmap();
	QSize frameSize   = frame.size();

	QRect posAndSize          = animation->property("posAndSize").toRect();
	QVariant propertyBaseSize = animation->property("baseSize");
	bool isRectChanged        = !propertyBaseSize.isValid() || propertyBaseSize.toSize() != frameSize;
	QRect rect                = isRectChanged ? LogTextBrowser::calcRectScaledToScreen(frameSize) : posAndSize;
	if (isRectChanged) {
		animation->setProperty("baseSize", frameSize);
		animation->setProperty("posAndSize", rect);
	}

	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawPixmap(rect, frame);
	double opacity = animation->property("videoControlsOpacity").toDouble();
	if (opacity > 0 && animation->property("areVideoControlsOnFullScreen").toBool()) {
		VideoUtils::drawVideoControls(&painter, rect, animation, opacity);
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
