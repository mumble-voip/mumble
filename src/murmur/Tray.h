// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_TRAY_H_
#define MUMBLE_MURMUR_TRAY_H_

#include "Logger.h"

#include <memory>

#include <QtCore/QObject>
#include <QtWidgets/QSystemTrayIcon>

class QAction;
class QMainWindow;

class Tray : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Tray)

protected:
	mumble::log::SinkPtr m_logSink;
	std::unique_ptr< QMainWindow > m_window;
	QSystemTrayIcon *qsti;
	QMenu *qm;
	QAction *qaQuit;
	QAction *qaAbout;
	QAction *qaShowLog;
public slots:
	void on_Tray_activated(QSystemTrayIcon::ActivationReason);
	void on_About_triggered();
	void on_Quit_triggered();
	void on_ShowLog_triggered();

public:
	Tray(QObject *parent = nullptr);
	~Tray();
};

#endif
