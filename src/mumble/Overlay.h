/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MUMBLE_MUMBLE_OVERLAY_H_
#define MUMBLE_MUMBLE_OVERLAY_H_

#include <QtCore/QtGlobal>
#include <QtCore/QUrl>
#include <QtNetwork/QLocalSocket>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
# include <QtWidgets/QGraphicsItem>
#else
# include <QtGui/QGraphicsItem>
#endif

#include "ConfigDialog.h"
#include "OverlayText.h"
#include "../../overlay/overlay.h"

#include "ui_OverlayEditor.h"

class ClientUser;
class Overlay;
class QLocalServer;
class OverlayClient;
class OverlayConfig;

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
		void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
		int type() const;

		template <typename T>
		QRectF boundingRect() const;
};

class OverlayMouse : public QGraphicsPixmapItem {
	private:
		Q_DISABLE_COPY(OverlayMouse)
	public:
		bool contains(const QPointF &) const;
		bool collidesWithPath(const QPainterPath &, Qt::ItemSelectionMode = Qt::IntersectsItemShape) const;
		OverlayMouse(QGraphicsItem * = NULL);
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

		QMap<QString, QString> qmOverlayHash;
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

#ifdef Q_OS_WIN
typedef void (__cdecl *HooksProc)();
class OverlayPrivateWin : public OverlayPrivate {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayPrivateWin)
	protected:
		QLibrary *qlOverlay;
	public:
		HooksProc hpInstall, hpRemove;

		void setActive(bool);
		OverlayPrivateWin(QObject *);
		~OverlayPrivateWin();
};
#endif

#endif
