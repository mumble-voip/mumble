// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ScreenShareViewer.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>

ScreenShareViewer::ScreenShareViewer(quint32 senderSession, const QString &senderName, QWidget *parent)
	: QDialog(parent, Qt::Window), m_senderSession(senderSession) {
	setWindowTitle(tr("%1's screen").arg(senderName));
	setAttribute(Qt::WA_DeleteOnClose, false);

	m_imageLabel = new QLabel(this);
	m_imageLabel->setAlignment(Qt::AlignCenter);
	m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_imageLabel->setMinimumSize(320, 240);
	m_imageLabel->setText(tr("Waiting for first frame…"));

	m_scrollArea = new QScrollArea(this);
	m_scrollArea->setWidget(m_imageLabel);
	m_scrollArea->setWidgetResizable(false);
	m_scrollArea->setAlignment(Qt::AlignCenter);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_scrollArea);

	resize(800, 600);
}

void ScreenShareViewer::updateImageDisplay() {
	if (m_currentFrame.isNull())
		return;

	// Get the available size in the scroll area
	QSize areaSize = m_scrollArea->viewport()->size();

	// Scale the image to fit, keeping aspect ratio
	QPixmap scaled = QPixmap::fromImage(m_currentFrame).scaled(areaSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	m_imageLabel->setPixmap(scaled);
	m_imageLabel->resize(scaled.size());
}

void ScreenShareViewer::resizeEvent(QResizeEvent *event) {
	QDialog::resizeEvent(event);
	updateImageDisplay();
}

void ScreenShareViewer::updateFrame(QImage frame) {
	if (frame.isNull())
		return;

	m_currentFrame = frame;

	// Show scaled version to fit the window
	updateImageDisplay();

	// Auto-resize the window on first real frame (up to the available screen).
	if (!isVisible()) {
		QSize desired = frame.size().boundedTo(QSize(1280, 800));
		resize(desired);
		show();
	}
}
