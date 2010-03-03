/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _OVERLAY_H
#define _OVERLAY_H

#include "../../overlay/overlay.h"
#include "ConfigDialog.h"
#include "ClientUser.h"
#include "SharedMemory.h"
#include "ui_Overlay.h"
#include "ui_OverlayEditor.h"

class User;
class Overlay;

class OverlayGroup : public QGraphicsItem {
	private:
		Q_DISABLE_COPY(OverlayGroup);
	public:
		enum { Type = UserType + 5 };
	public:
		OverlayGroup();

		QRectF boundingRect() const;
		void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
		
		int type() const;
};

class OverlayUser : public OverlayGroup {
	private:
		Q_DISABLE_COPY(OverlayUser);
	public:
		enum { Type = UserType + 1 };
	protected:
		QGraphicsPixmapItem *qgpiMuted, *qgpiDeafened;
		QGraphicsPixmapItem *qgpiAvatar;
		QGraphicsPixmapItem *qgpiName[4];
		QGraphicsPixmapItem *qgpiChannel;
		QGraphicsPathItem *qgpiBox;

		unsigned int uiSize;
		ClientUser *cuUser;
		Settings::TalkState tsColor;

		QString qsName;
		QString qsChannelName;
		QByteArray qbaAvatar;

		void setup();

	public:
		OverlayUser(ClientUser *cu, unsigned int uiSize);
		OverlayUser(Settings::TalkState ts, unsigned int uiSize);
		void updateUser();
		void updateLayout();

		int type() const;

		static QPixmap createPixmap(const QString &str, unsigned int height, unsigned int maxwidth, QColor col, const QFont &font, QPainterPath &);
		static QRectF scaledRect(const QRectF &qr, qreal scale);
		static QPointF alignedPosition(const QRectF &box, const QRectF &item, Qt::Alignment a);
};

class OverlayUserGroup : public OverlayGroup {
	private:
		Q_DISABLE_COPY(OverlayUserGroup);
	public:
		enum { Type = UserType + 3 };
	protected:
		void contextMenuEvent(QGraphicsSceneContextMenuEvent *);
		void wheelEvent(QGraphicsSceneWheelEvent *);
	public:
		OverlayUserGroup();
		int type() const;
};

class OverlayEditorScene : public QGraphicsScene {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayEditorScene)
	protected:
		QGraphicsItem *qgiGroup;

		QGraphicsPixmapItem *qgpiMuted;
		QGraphicsPixmapItem *qgpiAvatar;
		QGraphicsPixmapItem *qgpiName;
		QGraphicsPixmapItem *qgpiChannel;
		QGraphicsPathItem *qgpiBox;
		QGraphicsRectItem *qgriSelected;
		QGraphicsPixmapItem *qgpiSelected;
		int iDragCorner;
		
		Qt::WindowFrameSection wfsHover;

		unsigned int uiSize;

		void setup();

		void contextMenuEvent(QGraphicsSceneContextMenuEvent *);
		void mousePressEvent(QGraphicsSceneMouseEvent *);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
		void updateCursorShape(const QPointF &point);
		
		void drawBackground(QPainter *, const QRectF &);

		QGraphicsPixmapItem *childAt(const QPointF &);
		QRectF selectedRect() const;

		static Qt::WindowFrameSection rectSection(const QRectF &rect, const QPointF &point, qreal dist = 3.0f);
	public:
		Settings::TalkState tsColor;
		unsigned int uiZoom;
		OverlaySettings os;
	
		OverlayEditorScene(QObject *p = NULL);

	public slots:
		void resync();
		void updateSelected();
		
		void updateMuted();
		void updateUserName();
		void updateChannel();
		void updateAvatar();

		void moveMuted();
		void moveUserName();
		void moveChannel();
		void moveAvatar();
		void moveBox();
};

class OverlayEditor : public QDialog, public Ui::OverlayEditor {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayEditor)
	protected:
		QGraphicsItem *qgiPromote;
		OverlayEditorScene oes;
		
		void enterEvent(QEvent *);
		void leaveEvent(QEvent *);
	public:
		OverlayEditor(QWidget *p = NULL, QGraphicsItem *qgi = NULL);
		~OverlayEditor();
	public slots:
		void reset();
		void apply();
		void accept();

		void on_qrbPassive_clicked();
		void on_qrbTalking_clicked();
		void on_qrbWhisper_clicked();
		void on_qrbShout_clicked();
		
		void on_qcbAvatar_clicked();
		void on_qcbUser_clicked();
		void on_qcbChannel_clicked();
		void on_qcbMutedDeafened_clicked();
		void on_qcbBox_clicked();
		
		void on_qsZoom_valueChanged(int);
};

class OverlayConfig : public ConfigWidget, public Ui::OverlayConfig {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayConfig)
	public:
		OverlayConfig(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void accept() const;
		void save() const;
		void load(const Settings &r);
		bool expert(bool);
};

struct OverlayTextLine {
	enum Decoration { None, Muted, Deafened };
	QString qsText;
	unsigned int uiSession;
	QRgb uiColor;
	Decoration dDecor;
	int iPriority;
	OverlayTextLine() : uiSession(0), uiColor(0), dDecor(None), iPriority(0) { };
	OverlayTextLine(const QString &t, quint32 c, int priority = 0, unsigned int session = 0, Decoration d = None) : qsText(t), uiSession(session), uiColor(c), dDecor(d), iPriority(priority) { };
	bool operator <(const OverlayTextLine &o) const;
};

class OverlayMouse : public QGraphicsPixmapItem {
	private:
		Q_DISABLE_COPY(OverlayMouse);
	public:
		bool contains(const QPointF &) const;
		bool collidesWithPath(const QPainterPath &, Qt::ItemSelectionMode = Qt::IntersectsItemShape) const;
		OverlayMouse(QGraphicsItem * = NULL);
};

class OverlayClient : public QObject {
		friend class Overlay;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayClient);
	protected:
		OverlayMsg omMsg;
		QLocalSocket *qlsSocket;
		SharedMemory2 *smMem;
		QRect qrLast;
		Timer t;

		int iOffsetX, iOffsetY;
		QGraphicsPixmapItem *qgpiCursor;
		QGraphicsPixmapItem *qgpiLogo;

		quint64 uiPid;
		QGraphicsScene qgs;
		OverlayUserGroup ougUsers;
		QMap<QObject *, OverlayUser *> qmUsers;
		QList<OverlayUser *> qlExampleUsers;

		bool bWasVisible;
		bool bDelete;

		void setupRender();
		void setupScene();

		bool eventFilter(QObject *, QEvent *);

		QList<QRectF> qlDirty;
	protected slots:
		void readyRead();
		void userDestroyed(QObject *);
		void changed(const QList<QRectF> &);
		void render();
	public:
		QGraphicsView qgv;
		unsigned int uiWidth, uiHeight;
		int iMouseX, iMouseY;

		OverlayClient(QLocalSocket *, QObject *);
		~OverlayClient();
		bool setTexts(const QList<OverlayTextLine> &lines);
		void reset();
	public slots:
		void showGui();
		void hideGui();
		void scheduleDelete();
		void updateMouse();
		void openEditor();
};

class OverlayPrivate : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayPrivate);
	public:
		OverlayPrivate(QObject *p) : QObject(p) {};
};

class Overlay : public QObject {
		friend class OverlayConfig;
		friend class OverlayClient;
		friend class OverlayUser;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(Overlay)
	protected:
		OverlayPrivate *d;

		QList<OverlayTextLine> qlCurrentTexts;
		QSet<unsigned int> qsQueried;

		void platformInit();
		void setTexts(const QList<OverlayTextLine> &lines);

		QLocalServer *qlsServer;
		QList<OverlayClient *> qlClients;
	protected slots:
		void disconnected();
		void error(QLocalSocket::LocalSocketError);
		void newConnection();
	public:
		Overlay();
		~Overlay();
		bool isActive() const;
		void verifyTexture(ClientUser *cp, bool allowupdate = true);
	public slots:
		void updateOverlay();
		void setActive(bool act);
		void toggleShow();
		void forceSettings();
};

#endif
