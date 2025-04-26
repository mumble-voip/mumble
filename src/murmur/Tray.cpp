// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Tray.h"

#include "About.h"
#include "LogEmitter.h"
#include "Meta.h"
#include "Server.h"
#include "Version.h"

#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTextBrowser>

Tray::Tray(QObject *p, LogEmitter *logger) : QObject(p) {
	le = logger;

	qsti = new QSystemTrayIcon(qApp->windowIcon(), this);
	qsti->setObjectName(QLatin1String("Tray"));
	qsti->setToolTip(tr("Murmur"));

	qaQuit = new QAction(tr("&Quit Murmur"), this);
	qaQuit->setShortcut(tr("Ctrl+Q", "Quit"));
	qaQuit->setObjectName(QLatin1String("Quit"));

	qaAbout = new QAction(tr("&About Murmur"), this);
	qaAbout->setObjectName(QLatin1String("About"));

	qaShowLog = new QAction(tr("&Show Log"), this);
	qaShowLog->setShortcut(tr("Ctrl+L", "Quit"));
	qaShowLog->setObjectName(QLatin1String("ShowLog"));

	// Can't construct a QMenu which decends from QObject, and qsti is a QObject.
	// Qt bug?
	qm = new QMenu(tr("Murmur"), nullptr);
	qm->addAction(qaShowLog);
	qm->addSeparator();
	qm->addAction(qaAbout);
	qm->addAction(qaQuit);
	qsti->setContextMenu(qm);

	qsti->show();

	connect(le, SIGNAL(newLogEntry(const QString &)), this, SLOT(addLogMessage(const QString &)));

	QMetaObject::connectSlotsByName(this);
}

void Tray::on_Tray_activated(QSystemTrayIcon::ActivationReason r) {
	if (r == QSystemTrayIcon::Trigger) {
		qsti->showMessage(tr("Murmur"), tr("%1 server running.").arg(meta->qhServers.count()),
						  QSystemTrayIcon::Information, 5000);
	}
}

void Tray::on_Quit_triggered() {
	if (QMessageBox::question(nullptr, tr("Murmur"), tr("Are you sure you want to quit Murmur?"),
							  QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
		== QMessageBox::Yes) {
		qApp->quit();
	}
}

void Tray::on_About_triggered() {
	AboutDialog ad(nullptr);
	ad.exec();
}

void Tray::on_ShowLog_triggered() {
	QMainWindow *mw = new QMainWindow();
	mw->setAttribute(Qt::WA_DeleteOnClose);
	QTextBrowser *tb = new QTextBrowser();
	mw->resize(675, 300);
	mw->setCentralWidget(tb);
	mw->setWindowTitle(QString::fromLatin1("Murmur -- %1").arg(Version::getRelease()));

	connect(le, SIGNAL(newLogEntry(const QString &)), tb, SLOT(append(const QString &)));

	foreach (const QString &m, qlLog)
		tb->append(m);

	mw->show();
}

void Tray::addLogMessage(const QString &msg) {
	if (qlLog.count() >= 1000)
		qlLog.removeFirst();

	qlLog.append(msg);
}
