// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ScreenShareViewer.h"

#include <QtGui/QCloseEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

ScreenShareViewer::ScreenShareViewer(quint32 senderSession, const QString &senderName, QWidget *parent)
	: QDialog(parent, Qt::Window), m_senderSession(senderSession) {
	setWindowTitle(tr("%1's screen").arg(senderName));
	setAttribute(Qt::WA_DeleteOnClose, false);

	m_imageLabel = new QLabel(this);
	m_imageLabel->setAlignment(Qt::AlignCenter);
	m_imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_imageLabel->setMinimumSize(320, 240);
	m_imageLabel->setText(tr("Waiting for first frame…"));

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_imageLabel);

	resize(800, 600);
}

bool ScreenShareViewer::isDismissed() const {
	return m_dismissed;
}

void ScreenShareViewer::showAndRefresh() {
	m_dismissed = false;
	show();
	raise();
	activateWindow();
	updateImageDisplay();
}

void ScreenShareViewer::closeEvent(QCloseEvent *event) {
	// Remember that the user explicitly closed the window so we don't reopen it
	// automatically when new frames arrive.
	m_dismissed = true;
	QDialog::closeEvent(event);
}

void ScreenShareViewer::updateImageDisplay() {
	if (m_currentFrame.isNull())
		return;

	QSize areaSize = size();
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

	// Always update the image data so the viewer shows the latest frame
	// when the user re-opens it via the context menu.
	if (isVisible())
		updateImageDisplay();
}
