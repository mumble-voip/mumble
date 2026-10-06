// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCREENSHAREVIEWER_H_
#define MUMBLE_MUMBLE_SCREENSHAREVIEWER_H_

#include "VideoFrame.h"

#include <QtWidgets/QDialog>

class VideoWidget;

/// Floating window that displays the screen share stream from a single remote user.
class ScreenShareViewer : public QDialog {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ScreenShareViewer)

public:
	explicit ScreenShareViewer(quint32 senderSession, const QString &senderName, QWidget *parent = nullptr);

	/// Show the window with the last stored frame.
	void showAndRefresh();

public slots:
	void updateFrame(VideoFrame frame);

private:
	/// Draws the frames. Only exists when screen sharing is supported.
	VideoWidget *m_videoWidget = nullptr;
	quint32 m_senderSession;
};

#endif // MUMBLE_MUMBLE_SCREENSHAREVIEWER_H_
