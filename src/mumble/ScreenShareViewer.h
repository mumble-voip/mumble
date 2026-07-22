// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCREENSHAREVIEWER_H_
#define MUMBLE_MUMBLE_SCREENSHAREVIEWER_H_

#include <QtGui/QImage>
#include <QtWidgets/QDialog>

class QLabel;
class QScrollArea;

/// Floating window that displays the screen share stream from a single remote user.
class ScreenShareViewer : public QDialog {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ScreenShareViewer)

public:
	explicit ScreenShareViewer(quint32 senderSession, const QString &senderName, QWidget *parent = nullptr);

	/// Returns true once the user has explicitly closed the window.
	/// While dismissed, new frames update the stored image but do not reopen the window.
	bool isDismissed() const;
	/// Show the window and repaint with the last stored frame.
	void showAndRefresh();

public slots:
	void updateFrame(QImage frame);

protected:
	void resizeEvent(QResizeEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

private:
	void updateImageDisplay();

	QLabel *m_imageLabel;
	quint32 m_senderSession;
	QImage m_currentFrame;
	bool m_dismissed = false;
};

#endif // MUMBLE_MUMBLE_SCREENSHAREVIEWER_H_
