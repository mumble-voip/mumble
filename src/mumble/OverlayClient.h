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

#ifndef MUMBLE_MUMBLE_OVERLAYCLIENT_H_
#define MUMBLE_MUMBLE_OVERLAYCLIENT_H_

#include <QtCore/QUrl>
#include <QtNetwork/QLocalSocket>

#include "Timer.h"
#include "../../overlay/overlay.h"
#include "SharedMemory.h"
#include "OverlayUserGroup.h"

class ClientUser;
class Overlay;
class QLibrary;
class QLocalServer;

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
		QGraphicsPixmapItem *qgpiTime;

		/// The process ID of the process this OverlayClient is connected to.
		quint64 uiPid;
		/// The path to the executable of the process that this OverlayClient is connected to.
		QString qsExecutablePath;

		QGraphicsScene qgs;
		OverlayUserGroup ougUsers;

#ifdef Q_OS_MAC
		QMap<Qt::CursorShape, QPixmap> qmCursors;
#endif

		bool bWasVisible;
		bool bDelete;

		void setupRender();
		void setupScene(bool show);

		bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;

		void readyReadMsgInit(unsigned int length);

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
		~OverlayClient() Q_DECL_OVERRIDE;
		void reset();
	public slots:
		void showGui();
		void hideGui();
		void scheduleDelete();
		void updateMouse();
		void updateFPS();
		void updateTime();
		bool update();
		void openEditor();
};

#endif
