// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CUSTOMELEMENTS_H_
#define MUMBLE_MUMBLE_CUSTOMELEMENTS_H_

#include <QtCore/QObject>
#include <QtGui/QMovie>
#include <QtGui/QTextObjectInterface>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>

Q_DECLARE_METATYPE(QMovie *)

class LogTextBrowser : public QTextBrowser {
private:
	Q_OBJECT
	Q_DISABLE_COPY(LogTextBrowser)

protected:
	QWidget *widgetInFullScreen;
	QString queuedNumericInput;
	void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *) Q_DECL_OVERRIDE;
	QMimeData *createMimeDataFromSelection() const Q_DECL_OVERRIDE;

public:
	int lastCustomObjectIndex  = -1;
	int customObjectFocusIndex = -1;
	QList< QObject * > customObjects;
	LogTextBrowser(QWidget *p = nullptr);

	static QRect calcRectScaledToScreen(const QSize &size);
	bool isWidgetInFullScreen();
	void toggleFullScreen(QWidget *widget);
	void highlightSelectedObject(QPainter *painter, const QRect &rect, QObject *propertyHolder);
	QObject *customObjectAt(const QPoint &pos);
	/// Removes the content of the client's log, deletes the objects used by text objects
	/// and clears the counter for custom objects as well as their focus index.
	void clear();
	void repaint(const QRect &rect = QRect());
	void reflow();
	int getScrollX();
	int getScrollY();
	QPoint getScrollPos();
	QRect getLogRect();
	void setScrollX(int scrollPos);
	void setScrollY(int scrollPos);
	void changeScrollX(int change);
	void changeScrollY(int change);
	void scrollObjectIntoView(const QRect &rect);
	bool isScrolledToBottom();
};

class ChatbarTextEdit : public QTextEdit {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ChatbarTextEdit)
	void inFocus(bool);
	void applyPlaceholder();
	QStringList qslHistory;
	QString qsHistoryTemp;
	int iHistoryIndex;
	static const int MAX_HISTORY = 50;
	bool m_justPasted;

protected:
	QString qsDefaultText;
	bool bDefaultVisible;
	void focusInEvent(QFocusEvent *) Q_DECL_OVERRIDE;
	void focusOutEvent(QFocusEvent *) Q_DECL_OVERRIDE;
	void contextMenuEvent(QContextMenuEvent *) Q_DECL_OVERRIDE;
	void dragEnterEvent(QDragEnterEvent *) Q_DECL_OVERRIDE;
	void dragMoveEvent(QDragMoveEvent *) Q_DECL_OVERRIDE;
	void dropEvent(QDropEvent *) Q_DECL_OVERRIDE;
	bool event(QEvent *) Q_DECL_OVERRIDE;
	QSize minimumSizeHint() const Q_DECL_OVERRIDE;
	QSize sizeHint() const Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
	bool canInsertFromMimeData(const QMimeData *source) const Q_DECL_OVERRIDE;
	void insertFromMimeData(const QMimeData *source) Q_DECL_OVERRIDE;
	bool sendImagesFromMimeData(const QMimeData *source);
	bool emitPastedImage(const QImage &image, const QString &filePath = "");

public:
	void setDefaultText(const QString &, bool = false);
	unsigned int completeAtCursor();
signals:
	void tabPressed(void);
	void backtabPressed(void);
	void ctrlSpacePressed(void);
	void entered(QString);
	void ctrlEnterPressed(QString);
	void pastedImage(QString);
public slots:
	void pasteAndSend_triggered();
	void doResize();
	void doScrollbar();
	void addToHistory(const QString &str);
	void historyUp();
	void historyDown();

public:
	ChatbarTextEdit(QWidget *p = nullptr);
};

class VideoUtils {
public:
	enum class VideoControl {
		None,
		View,
		VideoBar,
		PlayPause,
		Cache,
		Loop,
		PreviousFrame,
		NextFrame,
		ResetSpeed,
		DecreaseSpeed,
		IncreaseSpeed,
		FullScreen
	};
	static const int videoBarHeight           = 4;
	static const int underVideoBarHeight      = 20;
	static const int videoControlsHeight      = videoBarHeight + underVideoBarHeight;
	static const int videoBarMinWidth         = 260;
	static const int cacheOffsetX             = -180;
	static const int loopModeOffsetX          = -140;
	static const int frameTraversalOffsetX    = -100;
	static const int speedOffsetX             = -70;
	static const int fullScreenOffsetX        = -15;
	static const int videoControlsRevertDelay = 1200;

	static bool isInBoundsOnAxis(int posOnAxis, int start, int length);
	static bool isInBounds(const QPoint &pos, const QRect &bounds);
	static int calcPosOnAxisFromOffset(int offset, int start, int length);
	static QRect calcVideoControlsRect(QObject *propertyHolder);

	static VideoControl videoControlAt(const QPoint &pos, const QRect &rect);
	static double videoBarProportionAt(const QPoint &pos, const QRect &rect);
	static void drawVideoControls(QPainter *painter, const QRect &rect, QObject *propertyHolder, double opacity = 1);
	static void drawCenteredPlayIcon(QPainter *painter, const QRect &rect);
	static void addVideoControlsTransition(QObject *propertyHolder, QWidget *area, bool isIdleCursorHidden = true);
	static void startOrHoldVideoControlsTransition(QObject *propertyHolder, const QPoint &hoveredPos = QPoint(),
												   QWidget *cursorArea = nullptr);

	static void updateVideoControls(QObject *propertyHolder, QWidget *area = nullptr);
	static void updatePropertyRect(QObject *propertyHolder, const QRect &rect);
	static QSizeF calcIntrinsicSize(const QSize &size, bool areVideoControlsOn);
	static void setPropertyFullScreen(QObject *propertyHolder, bool on);
	static void setAttributesWidthAndHeight(QObject *propertyHolder, QSize &size);
};

class ImageAnimationTextObject : public QObject, public QTextObjectInterface {
	Q_OBJECT
	Q_INTERFACES(QTextObjectInterface)

protected:
	QSizeF intrinsicSize(QTextDocument *doc, int charPos, const QTextFormat &fmt) Q_DECL_OVERRIDE;
	void drawObject(QPainter *painter, const QRectF &rectF, QTextDocument *doc, int charPos,
					const QTextFormat &fmt) Q_DECL_OVERRIDE;

public:
	static bool areVideoControlsOn;
	ImageAnimationTextObject();
	static QObject *createImageAnimation(const QByteArray &animationBa, LogTextBrowser *parentLog,
										 bool &isAnimationCheckOnly);
	static QObject *createImageAnimation(const QByteArray &animationBa, LogTextBrowser *parentLog);
	static void mousePress(QMovie *animation, const QPoint &mouseDocPos, const Qt::MouseButton &button);
	static bool scroll(QMovie *animation, const QPoint &mouseDocPos, bool isScrollingUp);
	static void keyPress(QMovie *animation, const Qt::Key &key, bool isObjectSelectionChanged = false);

	enum class LoopMode { Unchanged, Loop, NoLoop };
	static QString loopModeToString(LoopMode mode);

	static void toggleVideoControls(LogTextBrowser *log);
	static void toggleVideoControlsFullScreen(QObject *propertyHolder);
	static int getTotalTime(QObject *propertyHolder);
	static int getCurrentTime(QObject *propertyHolder, int frameIndex);
	static void setFrame(QMovie *animation, int frameIndex);
	static void setFrameByTime(QMovie *animation, int milliseconds);
	static void setFrameByProportion(QMovie *animation, double proportion);
	static void changeFrame(QMovie *animation, int amount);
	static void changeFrameByTime(QMovie *animation, int milliseconds);
	static void togglePause(QMovie *animation);
	static void toggleCache(QMovie *animation);
	static void toggleFullScreen(QMovie *animation);
	static void escapeFullScreen(QMovie *animation);
	static void stopPlayback(QMovie *animation);
	static void resetPlayback(QMovie *animation);
	static void setSpeed(QMovie *animation, int percentage);
	static void resetSpeed(QMovie *animation);
	static void invertSpeed(QMovie *animation);
	static void changeSpeed(QMovie *animation, int percentageStep);
	static void setLoopMode(QMovie *animation, LoopMode mode);
	static void changeLoopMode(QMovie *animation, int steps);
};

class FullScreenImageAnimation : public QLabel {
	Q_OBJECT

protected:
	void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

public:
	FullScreenImageAnimation(QMovie *animation, LogTextBrowser *parent = nullptr);
};

class DockTitleBar : public QLabel {
private:
	Q_OBJECT
	Q_DISABLE_COPY(DockTitleBar)
protected:
	QTimer *qtTick;
	int size;
	int newsize;

public:
	DockTitleBar();
	QSize sizeHint() const Q_DECL_OVERRIDE;
	QSize minimumSizeHint() const Q_DECL_OVERRIDE;
public slots:
	void tick();

protected:
	bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;
};

#endif // CUSTOMELEMENTS_H_
