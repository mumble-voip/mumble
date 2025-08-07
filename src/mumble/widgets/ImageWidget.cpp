// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ImageWidget.h"

#include <QResizeEvent>
#include <iostream>

ImageWidget::ImageWidget(QWidget *p) : QLabel(p) {
}


void ImageWidget::resizeEvent(QResizeEvent *event) {
	const QPixmap scaled = m_pix.scaled(
		sizePolicy().horizontalPolicy() == QSizePolicy::Preferred ? maximumWidth() : event->size().width(),
		sizePolicy().horizontalPolicy() == QSizePolicy::Preferred ? maximumHeight() : event->size().width(),
		Qt::KeepAspectRatio, Qt::SmoothTransformation);
	setPixmap(scaled);

	m_sizeHint = scaled.size();

	QLabel::resizeEvent(event);
}

void ImageWidget::setImage(const QPixmap &pix) {
	m_pix = pix;
	setPixmap(pix.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QSize ImageWidget::sizeHint() const {
	const QSize childSizeHint = QLabel::sizeHint();

	return { sizePolicy().verticalPolicy() == QSizePolicy::Preferred
				 ? std::min(childSizeHint.width(), m_sizeHint.width())
				 : childSizeHint.width(),
			 sizePolicy().horizontalPolicy() == QSizePolicy::Preferred
				 ? std::min(childSizeHint.height(), m_sizeHint.height())
				 : childSizeHint.height() };
}
