// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OverlayPositionableItem.h"

#include "Utils.h"

#include <QtCore/QEvent>
#include <QtGui/QPen>
#include <QtWidgets/QGraphicsScene>

OverlayPositionableItem::OverlayPositionableItem(QRectF *posPtr, const bool isPositionable)
	: m_position(posPtr), m_isPositionEditable(isPositionable), m_qgeiHandle(nullptr) {
}

OverlayPositionableItem::~OverlayPositionableItem() {
	delete m_qgeiHandle;
	m_qgeiHandle = nullptr;
}

void OverlayPositionableItem::createPositioningHandle() {
	m_qgeiHandle = new QGraphicsEllipseItem(QRectF(-4.0f, -4.0f, 8.0f, 8.0f));
	m_qgeiHandle->setPen(QPen(Qt::darkRed, 0.0f));
	m_qgeiHandle->setBrush(Qt::red);
	m_qgeiHandle->setZValue(0.5f);
	m_qgeiHandle->setFlag(QGraphicsItem::ItemIsMovable);
	m_qgeiHandle->setFlag(QGraphicsItem::ItemIsSelectable);
	scene()->addItem(m_qgeiHandle);
	m_qgeiHandle->installSceneEventFilter(this);
}

bool OverlayPositionableItem::sceneEventFilter(QGraphicsItem *watched, QEvent *e) {
	switch (e->type()) {
		case QEvent::GraphicsSceneMouseMove:
		case QEvent::GraphicsSceneMouseRelease:
			QMetaObject::invokeMethod(this, "onMove", Qt::QueuedConnection);
			break;
		default:
			break;
	}
	return QGraphicsItem::sceneEventFilter(watched, e);
}

void OverlayPositionableItem::onMove() {
	if (!m_qgeiHandle) {
		return;
	}

	const QRectF &sr = scene()->sceneRect();
	const QPointF &p = m_qgeiHandle->pos();

	m_position->setX(qBound< qreal >(0.0, p.x() / sr.width(), 1.0f));
	m_position->setY(qBound< qreal >(0.0, p.y() / sr.height(), 1.0f));

	m_qgeiHandle->setPos(m_position->x() * sr.width(), m_position->y() * sr.height());

	updateRender();
}

void OverlayPositionableItem::updateRender() {
	const QRectF &sr = scene()->sceneRect();
	// Translate the 0..1 float position to the real scene coordinates (relative to absolute position)
	QPoint absPos(static_cast< int >(sr.width() * m_position->x() + 0.5f),
				  static_cast< int >(sr.height() * m_position->y() + 0.5f));

	if (m_isPositionEditable) {
		if (!m_qgeiHandle) {
			createPositioningHandle();
		}
		m_qgeiHandle->setPos(absPos.x(), absPos.y());
	}

	QRectF br = boundingRect();
	// Limit the position by the elements width (to make sure it is right-/bottom-bound rather than outside of the scene
	QPoint maxPos(static_cast< int >(sr.width() - br.width() + 0.5f),
				  static_cast< int >(sr.height() - br.height() + 0.5f));
	int basex = qBound< int >(0, absPos.x(), maxPos.x());
	int basey = qBound< int >(0, absPos.y(), maxPos.y());
	setPos(basex, basey);
}

void OverlayPositionableItem::setItemVisible(const bool &visible) {
	setVisible(visible);
	if (m_qgeiHandle) {
		m_qgeiHandle->setVisible(visible);
	}
}
