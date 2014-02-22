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

#ifndef MUMBLE_MUMBLE_OVERLAYEDITORSCENE_H_
#define MUMBLE_MUMBLE_OVERLAYEDITORSCENE_H_

#include <QtGui/QGraphicsScene>

#include "Settings.h"

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

		OverlayEditorScene(const OverlaySettings &, QObject *p = NULL);
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

#endif
