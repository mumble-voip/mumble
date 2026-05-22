// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ResponsiveImageDialog.h"

#include <QtGui/QWheelEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QVBoxLayout>

#include <QtCore/QTimer>
#include <algorithm>

ResponsiveImageDialog::ResponsiveImageDialog(const QPixmap &pixmap, QWidget *parent)
	: QDialog(parent), m_pixmap(pixmap) {
	setWindowTitle(tr("Image Preview"));
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMinimumSize(200, 150);
	setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	m_scene = new QGraphicsScene(this);
	m_scene->addPixmap(m_pixmap);

	m_view = new QGraphicsView(m_scene, this);
	m_view->setDragMode(QGraphicsView::ScrollHandDrag);
	m_view->setRenderHint(QPainter::SmoothPixmapTransform);
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Use no frame shape for a cleaner look.
	m_view->setFrameShape(QFrame::NoFrame);

	layout->addWidget(m_view);

	// Setup zooming with wheel events.
	m_view->viewport()->installEventFilter(this);
	m_view->installEventFilter(this);

	// Set initial size to image size, but clamp it to reasonable min/max values.
	int initialWidth  = std::clamp(pixmap.width() + 40, 300, 1200);
	int initialHeight = std::clamp(pixmap.height() + 40, 200, 900);
	resize(initialWidth, initialHeight);
}

void ResponsiveImageDialog::showEvent(QShowEvent *event) {
	QDialog::showEvent(event);
	// Ensure fitInView is called after the window layout is fully initialized.
	QTimer::singleShot(0, this, [this]() { m_view->fitInView(m_scene->itemsBoundingRect(), Qt::KeepAspectRatio); });
}

void ResponsiveImageDialog::changeEvent(QEvent *event) {
	if (event->type() == QEvent::WindowStateChange) {
		QTimer::singleShot(0, this, [this]() { m_view->fitInView(m_scene->itemsBoundingRect(), Qt::KeepAspectRatio); });
	}
	QDialog::changeEvent(event);
}

bool ResponsiveImageDialog::eventFilter(QObject *obj, QEvent *event) {
	if (obj == m_view->viewport() && event->type() == QEvent::Wheel) {
		QWheelEvent *wheelEvent = static_cast< QWheelEvent * >(event);
		// Zoom in or out.
		if (wheelEvent->angleDelta().y() > 0) {
			m_view->scale(1.1, 1.1);
		} else if (wheelEvent->angleDelta().y() < 0) {
			m_view->scale(1.0 / 1.1, 1.0 / 1.1);
		}
		return true; // Event handled.
	} else if (obj == m_view && event->type() == QEvent::Resize) {
		m_view->fitInView(m_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
	}
	return QDialog::eventFilter(obj, event);
}