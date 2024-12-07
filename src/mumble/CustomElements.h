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
	QString queuedNumericInput;
	void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *) Q_DECL_OVERRIDE;

public:
	int lastCustomInteractiveItemIndex  = -1;
	int customInteractiveItemFocusIndex = -1;
	LogTextBrowser(QWidget *p = nullptr);

	void update();
	int getScrollX();
	int getScrollY();
	void setScrollX(int scroll_pos);
	void setScrollY(int scroll_pos);
	void changeScrollX(int change);
	void changeScrollY(int change);
	void scrollItemIntoView(QRect rect);
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
	bool emitPastedImage(QImage image, QString filePath = "");

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

class AnimationTextObject : public QObject, public QTextObjectInterface {
	Q_OBJECT
	Q_INTERFACES(QTextObjectInterface)

protected:
	static const int videoBarHeight        = 4;
	static const int underVideoBarHeight   = 20;
	static const int cacheOffsetX          = -170;
	static const int loopModeOffsetX       = -130;
	static const int frameTraversalOffsetX = -90;
	static const int speedOffsetX          = -30;
	QSizeF intrinsicSize(QTextDocument *doc, int posInDoc, const QTextFormat &fmt) Q_DECL_OVERRIDE;
	void drawObject(QPainter *painter, const QRectF &rectF, QTextDocument *doc, int posInDoc,
					const QTextFormat &fmt) Q_DECL_OVERRIDE;

public:
	static bool areVideoControlsOn;
	AnimationTextObject();
	static void mousePress(QAbstractTextDocumentLayout *docLayout, QPoint mouseDocPos, Qt::MouseButton button);
	static void keyPress(LogTextBrowser *log, bool isItemSelectionChanged, Qt::Key key);

	enum VideoController {
		VideoBar,
		View,
		PlayPause,
		CacheSwitch,
		LoopSwitch,
		PreviousFrame,
		NextFrame,
		ResetSpeed,
		DecreaseSpeed,
		IncreaseSpeed,
		None
	};
	enum LoopMode { Unchanged, Loop, NoLoop };
	static QString loopModeToString(LoopMode mode);

	static void updateVideoControls(QObject *propertyHolder);
	static int getTotalTime(QObject *propertyHolder);
	static int getCurrentTime(QObject *propertyHolder, int frameIndex);
	static void setFrame(QMovie *animation, int frameIndex);
	static void setFrameByTime(QMovie *animation, int milliseconds);
	static void setFrameByProportion(QMovie *animation, double proportion);
	static void togglePause(QMovie *animation);
	static void toggleCache(QMovie *animation);
	static void stopPlayback(QMovie *animation);
	static void resetPlayback(QMovie *animation);
	static void setSpeed(QMovie *animation, int percentage);
	static void changeLoopMode(QMovie *animation, int steps);
	static void changeFrame(QMovie *animation, int amount);
	static void changeFrameByTime(QMovie *animation, int milliseconds);
	static void changeSpeed(QMovie *animation, int percentageStep);

	static bool isInBoundsOnAxis(QPoint pos, bool yInsteadOfX, int start, int length);
	static bool isInBounds(QPoint pos, QRect bounds);
	static int getOffset(int offset, int start, int length);
	static void setRectAndVideoControlPositioning(QObject *propertyHolder, QRect rect);
	static VideoController mousePressVideoControls(QObject *propertyHolder, QPoint mouseDocPos);
	static void drawVideoControls(QPainter *painter, QObject *propertyHolder, QPixmap frame, bool wasPaused,
								  bool wasCached, int frameIndex, int speedPercentage);

	static void updatePropertyRect(QObject *propertyHolder, QRect rect);
	static void drawCenteredPlayIcon(QPainter *painter, QRect rect);
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
