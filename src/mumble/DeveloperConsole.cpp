// Copyright 2016-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DeveloperConsole.h"

#include "LogEmitter.h"
#include "Global.h"

#include <QtWidgets/QTextBrowser>

DeveloperConsole::DeveloperConsole(QObject *parent) : QObject(parent) {
	connect(Global::get().le.data(), SIGNAL(newLogEntry(const QString &)), this, SLOT(addLogMessage(const QString &)));
}

DeveloperConsole::~DeveloperConsole() {
	QMainWindow *mw = m_window.data();
	delete mw;
}

void DeveloperConsole::show() {
	if (m_window.isNull()) {
		QMainWindow *mw = new QMainWindow();
		mw->setAttribute(Qt::WA_DeleteOnClose);
		QTextBrowser *tb = new QTextBrowser();
		mw->resize(675, 300);
		mw->setCentralWidget(tb);
		mw->setWindowTitle(tr("Developer Console"));

		connect(Global::get().le.data(), SIGNAL(newLogEntry(const QString &)), tb, SLOT(append(const QString &)));

		foreach (const QString &m, m_logEntries)
			tb->append(m);
		m_window = QPointer< QMainWindow >(mw);
	}

	m_window.data()->show();
	m_window.data()->activateWindow();
}

void DeveloperConsole::addLogMessage(const QString &msg) {
	if (m_logEntries.count() >= 1000)
		m_logEntries.removeFirst();

	m_logEntries.append(msg);
}
