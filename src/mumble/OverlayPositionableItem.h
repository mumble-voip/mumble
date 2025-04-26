// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OVERLAYPOSITIONABLEITEM_H
#define MUMBLE_MUMBLE_OVERLAYPOSITIONABLEITEM_H

#include <QtCore/QtGlobal>
#include <QtWidgets/QGraphicsItem>

class OverlayPositionableItem : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT
	Q_DISABLE_COPY(OverlayPositionableItem)

public:
	OverlayPositionableItem(QRectF *posPtr, const bool isPositionable = false);
	virtual ~OverlayPositionableItem();
	void updateRender();
	void setItemVisible(const bool &visible);

private:
	/// Float value between 0 and 1 where 0,0 is top left, and 1,1 is bottom right
	QRectF *m_position;
	const bool m_isPositionEditable;
	QGraphicsEllipseItem *m_qgeiHandle;
	void createPositioningHandle();
	bool sceneEventFilter(QGraphicsItem *, QEvent *) Q_DECL_OVERRIDE;
private slots:
	void onMove();
};

#endif
