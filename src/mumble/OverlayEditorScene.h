// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OVERLAYEDITORSCENE_H_
#define MUMBLE_MUMBLE_OVERLAYEDITORSCENE_H_

#include <QtCore/QtGlobal>
#include <QtWidgets/QGraphicsScene>

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

	void contextMenuEvent(QGraphicsSceneContextMenuEvent *e) Q_DECL_OVERRIDE;
	void mousePressEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;
	void updateCursorShape(const QPointF &point);

	void drawBackground(QPainter *, const QRectF &) Q_DECL_OVERRIDE;

	QGraphicsPixmapItem *childAt(const QPointF &);
	QRectF selectedRect() const;

	static Qt::WindowFrameSection rectSection(const QRectF &rect, const QPointF &point, qreal dist = 3.0f);

public:
	Settings::TalkState tsColor;
	unsigned int uiZoom;
	OverlaySettings os;

	OverlayEditorScene(const OverlaySettings &, QObject *p = nullptr);
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
