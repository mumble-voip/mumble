// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DeveloperConsole.h"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextBrowser>

DeveloperConsole::DeveloperConsole(QTextBrowser *textBox, QObject *parent)
	: QObject(parent), m_window(std::make_unique< QMainWindow >()) {
	m_window->resize(675, 300);
	m_window->setCentralWidget(textBox);
	m_window->setWindowTitle(tr("Developer Console"));
}

DeveloperConsole::~DeveloperConsole() {
}

void DeveloperConsole::show() {
	m_window->show();
	m_window->activateWindow();
}
