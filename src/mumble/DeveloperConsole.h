// Copyright 2016-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DEVELOPERCONSOLE_H_
#define MUMBLE_MUMBLE_DEVELOPERCONSOLE_H_

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QStringList>
#include <QtWidgets/QMainWindow>

class DeveloperConsole : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(DeveloperConsole);

protected:
	QPointer< QMainWindow > m_window;
	QStringList m_logEntries;
public slots:
	void addLogMessage(const QString &);

public:
	DeveloperConsole(QObject *parent = nullptr);
	~DeveloperConsole();
	void show();
};

#endif
