// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ScreenShareViewer.h"

#ifdef USE_SCREEN_SHARING
#	include "VideoWidget.h"
#endif

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

ScreenShareViewer::ScreenShareViewer(quint32 senderSession, const QString &senderName, QWidget *parent)
	: QDialog(parent, Qt::Window), m_senderSession(senderSession) {
	setWindowTitle(tr("%1's screen").arg(senderName));
	setAttribute(Qt::WA_DeleteOnClose, false);

#ifdef USE_SCREEN_SHARING
	m_videoWidget = new VideoWidget(this);
	m_videoWidget->setMinimumSize(320, 240);
	m_videoWidget->setPlaceholderText(tr("Waiting for first frame…"));
	QWidget *display = m_videoWidget;
#else
	QLabel *display = new QLabel(tr("Screen sharing is not supported by this build of Mumble."), this);
	display->setAlignment(Qt::AlignCenter);
	display->setMinimumSize(320, 240);
#endif

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(display);

	resize(800, 600);
}

void ScreenShareViewer::showAndRefresh() {
	show();
	raise();
	activateWindow();
}

void ScreenShareViewer::updateFrame(QImage frame) {
	if (frame.isNull())
		return;

#ifdef USE_SCREEN_SHARING
	// Always update the image data so the viewer shows the latest frame
	// when the user re-opens it via the context menu.
	m_videoWidget->setFrame(frame);
#endif
}
