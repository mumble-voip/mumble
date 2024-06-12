// Copyright 2024 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_TRAYICON_H_
#define MUMBLE_MUMBLE_WIDGETS_TRAYICON_H_

#include <functional>

#include <QAction>
#include <QTimer>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon {
	Q_OBJECT

public:
	TrayIcon();

	void toggleShowHide();

public slots:
	void on_hideAction_triggered();
	void on_showAction_triggered();

	void on_icon_update();
	void on_tray_unhighlight();

private:
	std::reference_wrapper< QIcon > m_statusIcon;
	QIcon *m_currentIcon     = nullptr;
	QMenu *m_contextMenu     = nullptr;
	QAction *m_showAction    = nullptr;
	QAction *m_hideAction    = nullptr;
	QTimer *m_highlightTimer = nullptr;

	void updateContextMenu();
	void applyIcon(QIcon &icon);

private slots:
	void on_icon_clicked(QSystemTrayIcon::ActivationReason reason);
	void on_timer_triggered();
};

#endif // MUMBLE_MUMBLE_WIDGETS_TRAYICON_H_
