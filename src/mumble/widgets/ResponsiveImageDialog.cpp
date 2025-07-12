// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ResponsiveImageDialog.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>

#include <algorithm>

ResponsiveImageDialog::ResponsiveImageDialog(const QPixmap &pixmap, QWidget *parent)
	: QDialog(parent), m_pixmap(pixmap) {
	setWindowTitle(tr("Image Preview"));
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMinimumSize(200, 150);
	setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(10, 10, 10, 10);
	layout->setSpacing(0);

	QScrollArea *scrollArea = new QScrollArea(this);
	scrollArea->setWidgetResizable(true);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	m_label = new QLabel(this);
	m_label->setAlignment(Qt::AlignCenter);
	m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_label->setMinimumSize(100, 75);
	m_label->setScaledContents(true); // Allow stretching
	m_label->setPixmap(m_pixmap);

	scrollArea->setWidget(m_label);
	layout->addWidget(scrollArea);

	// Set initial size to image size, but clamp to reasonable min/max
	int initialWidth  = std::clamp(pixmap.width() + 60, 300, 1200);
	int initialHeight = std::clamp(pixmap.height() + 60, 200, 900);
	resize(initialWidth, initialHeight);
}

void ResponsiveImageDialog::resizeEvent(QResizeEvent *event) {
	QDialog::resizeEvent(event);
	// No aspect ratio: let QLabel::setScaledContents handle stretching
	// No need to manually scale pixmap
	m_label->setPixmap(m_pixmap);
}