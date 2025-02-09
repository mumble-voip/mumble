// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DEVELOPERCONSOLE_H_
#define MUMBLE_MUMBLE_DEVELOPERCONSOLE_H_

#include <memory>

#include <QtCore/QObject>

class QMainWindow;
class QTextBrowser;

class DeveloperConsole : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(DeveloperConsole)

public:
	DeveloperConsole(QTextBrowser *textBox, QObject *parent = nullptr);
	~DeveloperConsole();

	void show();

private:
	std::unique_ptr< QMainWindow > m_window;
};

#endif
