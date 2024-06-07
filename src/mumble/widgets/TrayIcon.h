// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_TRAYICON_H_
#define MUMBLE_MUMBLE_WIDGETS_TRAYICON_H_

#include <functional>

#include <QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon {
	Q_OBJECT

public:
	TrayIcon();

public slots:
	void on_hideAction_triggered();
	void on_showAction_triggered();
	void on_toggleShowHide();

private:
	std::reference_wrapper< QIcon > m_statusIcon;
	QMenu *m_contextMenu  = nullptr;
	QAction *m_showAction = nullptr;
	QAction *m_hideAction = nullptr;

	void updateContextMenu();

private slots:
	void on_icon_clicked(QSystemTrayIcon::ActivationReason reason);
	void on_windowMinimized();
};

#endif // MUMBLE_MUMBLE_WIDGETS_TRAYICON_H_
