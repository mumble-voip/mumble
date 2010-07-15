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

#include "mumble_pch.hpp"
#include "ConfigDialog.h"
#include "ClientUser.h"
#include "SharedMemory.h"
#include "Global.h"
#include "ui_Overlay.h"
#include "ui_OverlayEditor.h"
#include "../../overlay/overlay.h"

class User;
class Overlay;

struct OverlayAppInfo {
	QString qsDisplayName;
	QIcon qiIcon;

	OverlayAppInfo(QString name, QIcon icon = QIcon());
};

class OverlayGroup : public QGraphicsItem {
	private:
		Q_DISABLE_COPY(OverlayGroup);
	public:
		enum { Type = UserType + 5 };
	public:
		OverlayGroup();

		QRectF boundingRect() const;
		template<class T>
		QRectF boundingRect() const;
		void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

		int type() const;
};

class OverlayUser : public OverlayGroup {
	private:
		Q_DISABLE_COPY(OverlayUser)
	public:
		enum { Type = UserType + 1 };
	protected:
		QGraphicsPixmapItem *qgpiMuted, *qgpiDeafened;
		QGraphicsPixmapItem *qgpiAvatar;
		QGraphicsPixmapItem *qgpiName[4];
		QGraphicsPixmapItem *qgpiChannel;
		QGraphicsPathItem *qgpiBox;

		OverlaySettings *os;

		unsigned int uiSize;
		ClientUser *cuUser;
		Settings::TalkState tsColor;

		QString qsName;
		QString qsChannelName;
		QByteArray qbaAvatar;

		void setup();

	public:
		OverlayUser(ClientUser *cu, unsigned int uiSize, OverlaySettings *osptr);
		OverlayUser(Settings::TalkState ts, unsigned int uiSize, OverlaySettings *osptr);
		void updateUser();
		void updateLayout();

		int type() const;

		static QPixmap createPixmap(const QString &str, unsigned int maxwidth, unsigned int height, QColor col, const QFont &font, QPainterPath &);
		static QRectF scaledRect(const QRectF &qr, qreal scale);
		static QPointF alignedPosition(const QRectF &box, const QRectF &item, Qt::Alignment a);
};

class OverlayUserGroup : public QObject, public OverlayGroup {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayUserGroup);
	public:
		enum { Type = UserType + 3 };
	protected:
		OverlaySettings *os;

		QMap<QObject *, OverlayUser *> qmUsers;
		QList<OverlayUser *> qlExampleUsers;

		QGraphicsEllipseItem *qgeiHandle;

		void contextMenuEvent(QGraphicsSceneContextMenuEvent *);
		void wheelEvent(QGraphicsSceneWheelEvent *);
		bool sceneEventFilter(QGraphicsItem *, QEvent *);
	protected slots:
		void userDestroyed(QObject *);
		void moveUsers();
	public:
		bool bShowExamples;

		OverlayUserGroup(OverlaySettings *osptr = &g.s.os);
		~OverlayUserGroup();

		int type() const;
	public slots:
		void reset();
		void updateUsers();
		void updateLayout();
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

		OverlayEditorScene(QObject *p = NULL, const OverlaySettings &srcos = g.s.os);
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
		OverlaySettings *os;

		void enterEvent(QEvent *);
		void leaveEvent(QEvent *);
	public:
		OverlayEditor(QWidget *p = NULL, QGraphicsItem *qgi = NULL, OverlaySettings *osptr = NULL);
		~OverlayEditor();
	signals:
		void applySettings();
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
	protected:
		QPixmap qpScreen;
		QGraphicsPixmapItem *qgpiScreen;
		QGraphicsScene qgs;
		OverlayUserGroup *oug;
		QGraphicsTextItem *qgtiInstructions;

		bool eventFilter(QObject *, QEvent *);

		QString applicationIdentifierForPath(const QString &path);
		OverlayAppInfo applicationInfoForId(const QString &identifier);
	protected slots:
		void on_qpbInstall_clicked();
		void on_qpbUninstall_clicked();
		void on_qpbAdd_clicked();
		void on_qpbRemove_clicked();
		void on_qrbBlacklist_toggled(bool);
		void on_qcbEnable_stateChanged(int);
		void resizeScene();
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

class OverlayMouse : public QGraphicsPixmapItem {
	private:
		Q_DISABLE_COPY(OverlayMouse)
	public:
		bool contains(const QPointF &) const;
		bool collidesWithPath(const QPainterPath &, Qt::ItemSelectionMode = Qt::IntersectsItemShape) const;
		OverlayMouse(QGraphicsItem * = NULL);
};

class OverlayClient : public QObject {
		friend class Overlay;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayClient)
	protected:
		OverlayMsg omMsg;
		QLocalSocket *qlsSocket;
		SharedMemory2 *smMem;
		QRect qrLast;
		Timer t;

		unsigned int fFps;
		int iOffsetX, iOffsetY;
		QGraphicsPixmapItem *qgpiCursor;
		QGraphicsPixmapItem *qgpiLogo;
		QGraphicsPixmapItem *qgpiFPS;

		quint64 uiPid;
		QGraphicsScene qgs;
		OverlayUserGroup ougUsers;

		bool bWasVisible;
		bool bDelete;

		void setupRender();
		void setupScene(bool show);

		bool eventFilter(QObject *, QEvent *);

		QList<QRectF> qlDirty;
	protected slots:
		void readyRead();
		void changed(const QList<QRectF> &);
		void render();
	public:
		QGraphicsView qgv;
		unsigned int uiWidth, uiHeight;
		int iMouseX, iMouseY;

		OverlayClient(QLocalSocket *, QObject *);
		~OverlayClient();
		void reset();
	public slots:
		void showGui();
		void hideGui();
		void scheduleDelete();
		void updateMouse();
		void updateFPS();
		bool update();
		void openEditor();
};

class OverlayPrivate : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayPrivate)
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

		QSet<unsigned int> qsQueried;
		QSet<unsigned int> qsQuery;

		void platformInit();

		static bool supportsInstallableOverlay();
		static bool isInstalled();
		static bool needsUpgrade();
		static bool installFiles();
		static bool uninstallFiles();

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
		void requestTexture(ClientUser *);

	public slots:
		void updateOverlay();
		void setActive(bool act);
		void toggleShow();
		void forceSettings();
};

#endif
