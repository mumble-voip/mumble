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

class User;
class Overlay;

class OverlayConfig : public ConfigWidget, public Ui::OverlayConfig {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayConfig)
	protected:
		QFont qfFont;
		QColor qcUser, qcWhisper, qcTalking, qcChannel, qcChannelTalking;

		static void setColorLabel(QLabel *label, QColor col);
	public:
		OverlayConfig(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void on_qsMaxHeight_valueChanged(int v);
		void on_qpbSetFont_clicked();
		void on_qpbUser_clicked();
		void on_qpbTalking_clicked();
		void on_qpbWhisper_clicked();
		void on_qpbChannel_clicked();
		void on_qpbChannelTalking_clicked();
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

class OverlayUser : public QGraphicsItemGroup {
	private:
		Q_DISABLE_COPY(OverlayUser);
	protected:
		enum TextColor { Passive, Talking, Linked, WhisperPrivate, WhisperChannel };
		QGraphicsPixmapItem *qgpiMuted, *qgpiDeafened;
		QGraphicsPixmapItem *qgpiAvatar;
		QGraphicsPixmapItem *qgpiName[5];
		QGraphicsPixmapItem *qgpiChannel;
		
		unsigned int uiSize;
		ClientUser *cuUser;
		QString qsName;
		QString qsChannelName;
		QByteArray qbaAvatar;
		
	public:
		OverlayUser(ClientUser *cu, unsigned int uiSize);
		void updateUser();

		static QPixmap createPixmap(const QString &str, unsigned int height, unsigned int maxwidth, QColor col, QPainterPath &);
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
		int iItemHeight;
		QRect qrLast;
		Timer t;
		
		quint64 uiPid;

		QGraphicsScene qgs;
		QGraphicsPixmapItem *qgpiCursor;

		QMap<QObject *, OverlayUser *> qmUsers;

		void setupRender();
		
		bool eventFilter(QObject *, QEvent *);
	protected slots:
		void readyRead();
		void userDestroyed(QObject *);
		void changed(const QList<QRectF> &);
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
		void fixup();
		void updateCursor();
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
		
		QList<QColor> qlColors;

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
