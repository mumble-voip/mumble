/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Tray.h"
#include "Server.h"
#include "Meta.h"
#include "Version.h"

Tray::Tray(QObject *p, LogEmitter *logger) : QObject(p) {
	le = logger;

	qsti = new QSystemTrayIcon(qApp->windowIcon(), this);
	qsti->setObjectName(QLatin1String("Tray"));
	qsti->setToolTip(tr("Murmur"));

	qaQuit = new QAction(tr("&Quit Murmur"), this);
	qaQuit->setShortcut(tr("Ctrl+Q", "Quit"));
	qaQuit->setObjectName(QLatin1String("Quit"));

	qaShowLog = new QAction(tr("&Show Log"), this);
	qaShowLog->setShortcut(tr("Ctrl+L", "Quit"));
	qaShowLog->setObjectName(QLatin1String("ShowLog"));

	// Can't construct a QMenu which decends from QObject, and qsti is a QObject.
	// Qt bug?
	qm = new QMenu(tr("Murmur"), NULL);
	qm->addAction(qaShowLog);
	qm->addSeparator();
	qm->addAction(qaQuit);
	qsti->setContextMenu(qm);

	qsti->show();

	connect(le, SIGNAL(newLogEntry(const QString &)), this, SLOT(addLogMessage(const QString &)));

	QMetaObject::connectSlotsByName(this);
}

void Tray::on_Tray_activated(QSystemTrayIcon::ActivationReason r) {
	if (r == QSystemTrayIcon::Trigger) {
		qsti->showMessage(tr("Murmur"), tr("%1 server running.").arg(meta->qhServers.count()), QSystemTrayIcon::Information, 5000);
	}
}

void Tray::on_Quit_triggered() {
	if (QMessageBox::question(NULL, tr("Murmur"), tr("Are you sure you want to quit Murmur?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
		qApp->quit();
	}
}

void Tray::on_ShowLog_triggered() {
	QMainWindow *mw = new QMainWindow();
	mw->setAttribute(Qt::WA_DeleteOnClose);
	QTextBrowser *tb = new QTextBrowser();
	mw->resize(675, 300);
	mw->setCentralWidget(tb);
	mw->setWindowTitle(QString::fromLatin1("Murmur -- %1").arg(MUMBLE_RELEASE));

	connect(le, SIGNAL(newLogEntry(const QString &)), tb, SLOT(append(const QString &)));

	foreach(const QString &m, qlLog)
		tb->append(m);

	mw->show();
}

void Tray::addLogMessage(const QString &msg) {
	if (qlLog.count() >= 1000)
		qlLog.removeFirst();

	qlLog.append(msg);
}
