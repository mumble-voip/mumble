// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_TRAY_H_
#define MUMBLE_MURMUR_TRAY_H_

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtWidgets/QSystemTrayIcon>

class LogEmitter;
class QAction;

class Tray : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Tray)

protected:
	QSystemTrayIcon *qsti;
	QMenu *qm;
	QAction *qaQuit;
	QAction *qaAbout;
	QAction *qaShowLog;
	QStringList qlLog;
	LogEmitter *le;
public slots:
	void on_Tray_activated(QSystemTrayIcon::ActivationReason);
	void on_About_triggered();
	void on_Quit_triggered();
	void on_ShowLog_triggered();
	void addLogMessage(const QString &);

public:
	Tray(QObject *parent, LogEmitter *le);
};

#endif
