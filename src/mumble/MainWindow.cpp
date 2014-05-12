/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "mumble_pch.hpp"

#include "MainWindow.h"

#include "About.h"
#include "ACL.h"
#include "ACLEditor.h"
#include "AudioInput.h"
#include "AudioStats.h"
#include "AudioWizard.h"
#include "BanEditor.h"
#include "CELTCodec.h"
#include "Cert.h"
#include "Channel.h"
#include "Connection.h"
#include "ConnectDialog.h"
#include "Database.h"
#include "Global.h"
#include "GlobalShortcut.h"
#include "Log.h"
#include "Net.h"
#include "NetworkConfig.h"
#include "OverlayClient.h"
#include "Plugins.h"
#include "PTTButtonWidget.h"
#include "RichTextEditor.h"
#include "ServerHandler.h"
#include "TextMessage.h"
#include "Tokens.h"
#include "User.h"
#include "UserEdit.h"
#include "UserInformation.h"
#include "UserModel.h"
#include "VersionCheck.h"
#include "ViewCert.h"
#include "VoiceRecorderDialog.h"
#include "../SignalCurry.h"
#include "Settings.h"

#ifdef Q_OS_WIN
#include "TaskList.h"
#endif

#ifdef USE_COCOA
#include "ConfigDialog_macx.h"
#endif

MessageBoxEvent::MessageBoxEvent(QString m) : QEvent(static_cast<QEvent::Type>(MB_QEVENT)) {
	msg = m;
}

OpenURLEvent::OpenURLEvent(QUrl u) : QEvent(static_cast<QEvent::Type>(OU_QEVENT)) {
	url = u;
}

const QString MainWindow::defaultStyleSheet = QLatin1String(".log-channel{text-decoration:none;}.log-user{text-decoration:none;}p{margin:0;}");

MainWindow::MainWindow(QWidget *p) : QMainWindow(p) {
	qiIconMuteSelf.addFile(QLatin1String("skin:muted_self.svg"));
	qiIconMuteServer.addFile(QLatin1String("skin:muted_server.svg"));
	qiIconMuteSuppressed.addFile(QLatin1String("skin:muted_suppressed.svg"));
	qiIconDeafSelf.addFile(QLatin1String("skin:deafened_self.svg"));
	qiIconDeafServer.addFile(QLatin1String("skin:deafened_server.svg"));
	qiTalkingOff.addFile(QLatin1String("skin:talking_off.svg"));
	qiTalkingOn.addFile(QLatin1String("skin:talking_on.svg"));
	qiTalkingShout.addFile(QLatin1String("skin:talking_alt.svg"));
	qiTalkingWhisper.addFile(QLatin1String("skin:talking_whisper.svg"));

#ifdef Q_OS_MAC
	if (QFile::exists(QLatin1String("skin:mumble.icns")))
		qiIcon.addFile(QLatin1String("skin:mumble.icns"));
	else
		qiIcon.addFile(QLatin1String("skin:mumble.svg"));
#else
	{
		QSvgRenderer svg(QLatin1String("skin:mumble.svg"));
		QPixmap original(512,512);
		original.fill(Qt::transparent);

		QPainter painter(&original);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::TextAntialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.setRenderHint(QPainter::HighQualityAntialiasing);
		svg.render(&painter);

		for (int sz=8;sz<=256;sz+=8)
			qiIcon.addPixmap(original.scaled(sz,sz, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	}

	// Set application icon except on MacOSX, where the window-icon
	// shown in the title-bar usually serves as a draggable version of the
	// current open document (i.e. you can copy the open document anywhere
	// simply by dragging this icon).
	qApp->setWindowIcon(qiIcon);
	
	// Set the icon on the MainWindow directly. This fixes the icon not
	// being set on the MainWindow in certain environments (Ex: GTK+).
	setWindowIcon(qiIcon);
#endif

#ifdef Q_OS_WIN
	uiNewHardware = 1;
#endif
	bSuppressAskOnQuit = false;
	bAutoUnmute = false;

	Channel::add(0, tr("Root"));

	aclEdit = NULL;
	banEdit = NULL;
	userEdit = NULL;
	tokenEdit = NULL;

	voiceRecorderDialog = NULL;

	qwPTTButtonWidget = NULL;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	cuContextUser = QWeakPointer<ClientUser>();
	cContextChannel = QWeakPointer<Channel>();
#endif

	qtReconnect = new QTimer(this);
	qtReconnect->setInterval(10000);
	qtReconnect->setSingleShot(true);
	qtReconnect->setObjectName(QLatin1String("Reconnect"));

	qmUser = new QMenu(tr("&User"), this);
	qmChannel = new QMenu(tr("&Channel"), this);

	createActions();
	setupUi(this);
	setupGui();

	connect(qmUser, SIGNAL(aboutToShow()), this, SLOT(qmUser_aboutToShow()));
	connect(qmChannel, SIGNAL(aboutToShow()), this, SLOT(qmChannel_aboutToShow()));
	connect(qteChat, SIGNAL(entered(QString)), this, SLOT(sendChatbarMessage(QString)));


	// Explicitely add actions to mainwindow so their shortcuts are available
	// if only the main window is visible (e.g. minimal mode)
	addActions(findChildren<QAction*>());

	on_qmServer_aboutToShow();
	on_qmSelf_aboutToShow();
	qmChannel_aboutToShow();
	qmUser_aboutToShow();
	on_qmConfig_aboutToShow();

	setOnTop(g.s.aotbAlwaysOnTop == Settings::OnTopAlways ||
	         (g.s.bMinimalView && g.s.aotbAlwaysOnTop == Settings::OnTopInMinimal) ||
	         (!g.s.bMinimalView && g.s.aotbAlwaysOnTop == Settings::OnTopInNormal));

#ifdef NO_UPDATE_CHECK
	delete qaHelpVersionCheck;
#endif
}

void MainWindow::createActions() {
	int idx = 1;
	gsPushTalk=new GlobalShortcut(this, idx++, tr("Push-to-Talk", "Global Shortcut"), false);
	gsPushTalk->setObjectName(QLatin1String("PushToTalk"));
	gsPushTalk->qsToolTip = tr("Push and hold this button to send voice.", "Global Shortcut");
	gsPushTalk->qsWhatsThis = tr("This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.", "Global Shortcut");


	gsResetAudio=new GlobalShortcut(this, idx++, tr("Reset Audio Processor", "Global Shortcut"));
	gsResetAudio->setObjectName(QLatin1String("ResetAudio"));

	gsMuteSelf=new GlobalShortcut(this, idx++, tr("Mute Self", "Global Shortcut"), false, 0);
	gsMuteSelf->setObjectName(QLatin1String("gsMuteSelf"));
	gsMuteSelf->qsToolTip = tr("Set self-mute status.", "Global Shortcut");
	gsMuteSelf->qsWhatsThis = tr("This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.", "Global Shortcut");

	gsDeafSelf=new GlobalShortcut(this, idx++, tr("Deafen Self", "Global Shortcut"), false, 0);
	gsDeafSelf->setObjectName(QLatin1String("gsDeafSelf"));
	gsDeafSelf->qsToolTip = tr("Set self-deafen status.", "Global Shortcut");
	gsDeafSelf->qsWhatsThis = tr("This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.", "Global Shortcut");

	gsUnlink=new GlobalShortcut(this, idx++, tr("Unlink Plugin", "Global Shortcut"));
	gsUnlink->setObjectName(QLatin1String("UnlinkPlugin"));

	gsPushMute=new GlobalShortcut(this, idx++, tr("Push-to-Mute", "Global Shortcut"));
	gsPushMute->setObjectName(QLatin1String("PushToMute"));

	gsJoinChannel=new GlobalShortcut(this, idx++, tr("Join Channel", "Global Shortcut"));
	gsJoinChannel->setObjectName(QLatin1String("MetaChannel"));
	gsJoinChannel->qsToolTip = tr("Use in conjunction with Whisper to.", "Global Shortcut");

	gsToggleOverlay=new GlobalShortcut(this, idx++, tr("Toggle Overlay", "Global Shortcut"), false);
	gsToggleOverlay->setObjectName(QLatin1String("ToggleOverlay"));
	gsToggleOverlay->qsToolTip = tr("Toggle state of in-game overlay.", "Global Shortcut");
	gsToggleOverlay->qsWhatsThis = tr("This will switch the states of the in-game overlay.", "Global Shortcut");
	connect(gsToggleOverlay, SIGNAL(down(QVariant)), g.o, SLOT(toggleShow()));

	gsMinimal=new GlobalShortcut(this, idx++, tr("Toggle Minimal", "Global Shortcut"));
	gsMinimal->setObjectName(QLatin1String("ToggleMinimal"));

	gsVolumeUp=new GlobalShortcut(this, idx++, tr("Volume Up (+10%)", "Global Shortcut"));
	gsVolumeUp->setObjectName(QLatin1String("VolumeUp"));

	gsVolumeDown=new GlobalShortcut(this, idx++, tr("Volume Down (-10%)", "Global Shortcut"));
	gsVolumeDown->setObjectName(QLatin1String("VolumeDown"));

	qstiIcon = new QSystemTrayIcon(qiIcon, this);
	qstiIcon->setToolTip(tr("Mumble -- %1").arg(QLatin1String(MUMBLE_RELEASE)));
	qstiIcon->setObjectName(QLatin1String("Icon"));

	gsWhisper = new GlobalShortcut(this, idx++, tr("Whisper/Shout"), false, QVariant::fromValue(ShortcutTarget()));
	gsWhisper->setObjectName(QLatin1String("gsWhisper"));

	gsLinkChannel=new GlobalShortcut(this, idx++, tr("Link Channel", "Global Shortcut"));
	gsLinkChannel->setObjectName(QLatin1String("MetaLink"));
	gsLinkChannel->qsToolTip = tr("Use in conjunction with Whisper to.", "Global Shortcut");

	gsCycleTransmitMode=new GlobalShortcut(this, idx++, tr("Cycle Transmit Mode", "Global Shortcut"));
	gsCycleTransmitMode->setObjectName(QLatin1String("gsCycleTransmitMode"));

#ifndef Q_OS_MAC
	qstiIcon->show();
#endif
}

void MainWindow::setupGui()  {
	setWindowTitle(tr("Mumble -- %1").arg(QLatin1String(MUMBLE_RELEASE)));
	setCentralWidget(qtvUsers);
	setAcceptDrops(true);

#ifdef Q_OS_MAC
	QMenu *qmWindow = new QMenu(tr("&Window"), this);
	menubar->insertMenu(qmHelp->menuAction(), qmWindow);
	qmWindow->addAction(tr("Minimize"), this, SLOT(showMinimized()), QKeySequence(tr("Ctrl+M")));

	qtvUsers->setAttribute(Qt::WA_MacShowFocusRect, false);
	qteChat->setAttribute(Qt::WA_MacShowFocusRect, false);
	qteChat->setFrameShape(QFrame::NoFrame);
	qteLog->setFrameStyle(QFrame::NoFrame);
#endif

	LogDocument *ld = new LogDocument(qteLog);
	qteLog->setDocument(ld);

	qteLog->document()->setMaximumBlockCount(g.s.iMaxLogBlocks);
	qteLog->document()->setDefaultStyleSheet(qApp->styleSheet());

	pmModel = new UserModel(qtvUsers);
	qtvUsers->setModel(pmModel);
	qtvUsers->setRowHidden(0, QModelIndex(), true);
	qtvUsers->ensurePolished();

	qaAudioMute->setChecked(g.s.bMute);
	qaAudioDeaf->setChecked(g.s.bDeaf);
	qaAudioTTS->setChecked(g.s.bTTS);
	qaFilterToggle->setChecked(g.s.bFilterActive);

	qaHelpWhatsThis->setShortcuts(QKeySequence::WhatsThis);

	qaConfigMinimal->setChecked(g.s.bMinimalView);
	qaConfigHideFrame->setChecked(g.s.bHideFrame);

	connect(gsResetAudio, SIGNAL(down(QVariant)), qaAudioReset, SLOT(trigger()));
	connect(gsUnlink, SIGNAL(down(QVariant)), qaAudioUnlink, SLOT(trigger()));
	connect(gsMinimal, SIGNAL(down(QVariant)), qaConfigMinimal, SLOT(trigger()));

	dtbLogDockTitle = new DockTitleBar();
	qdwLog->setTitleBarWidget(dtbLogDockTitle);

	foreach(QWidget *w, qdwLog->findChildren<QWidget *>()) {
		w->installEventFilter(dtbLogDockTitle);
		w->setMouseTracking(true);
	}

	dtbChatDockTitle = new DockTitleBar();
	qdwChat->setTitleBarWidget(dtbChatDockTitle);
	qdwChat->installEventFilter(dtbChatDockTitle);
	qteChat->setDefaultText(tr("<center>Not connected</center>"), true);
	qteChat->setEnabled(false);

	setShowDockTitleBars(g.s.wlWindowLayout == Settings::LayoutCustom);

#ifdef USE_COCOA
	// Workaround for QTBUG-3116 -- using a unified toolbar on Mac OS X
	// and using restoreGeometry before the window has updated its frameStrut
	// causes the MainWindow to jump around on screen on launch.  Workaround
	// is to call show() to update the frameStrut and set the windowOpacity to
	// 0 to hide any graphical glitches that occur when we add stuff to the
	// window.
	setWindowOpacity(0.0f);
	show();
#endif

	connect(qtvUsers->selectionModel(),
	        SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
	        SLOT(qtvUserCurrentChanged(const QModelIndex &, const QModelIndex &)));

#ifndef Q_OS_MAC
	setupView(false);
#endif

	if (g.s.bMinimalView && ! g.s.qbaMinimalViewGeometry.isNull())
		restoreGeometry(g.s.qbaMinimalViewGeometry);
	else if (! g.s.bMinimalView && ! g.s.qbaMainWindowGeometry.isNull())
		restoreGeometry(g.s.qbaMainWindowGeometry);

	Settings::WindowLayout wlTmp = g.s.wlWindowLayout;
	if (g.s.bMinimalView && ! g.s.qbaMinimalViewState.isNull())
		restoreState(g.s.qbaMinimalViewState);
	else if (! g.s.bMinimalView && ! g.s.qbaMainWindowState.isNull())
		restoreState(g.s.qbaMainWindowState);
	g.s.wlWindowLayout = wlTmp;

	setupView(false);

	qmTray = new QMenu(this);
	connect(qmTray, SIGNAL(aboutToShow()), this, SLOT(trayAboutToShow()));
	trayAboutToShow();
	qstiIcon->setContextMenu(qmTray);

	updateTrayIcon();

#ifdef USE_COCOA
	setWindowOpacity(1.0f);
#if QT_VERSION < 0x040700
	// Process pending events.  This is done to force the unified
	// toolbar to show up as soon as possible (and not wait until
	// we are back into the Cocoa mainloop)
	qApp->processEvents();
#endif
#endif
}

// Sets whether or not to show the title bars on the MainWindow's
// dock widgets.
void MainWindow::setShowDockTitleBars(bool doShow) {
	dtbLogDockTitle->setEnabled(doShow);
	dtbChatDockTitle->setEnabled(doShow);
}

MainWindow::~MainWindow() {
	delete qwPTTButtonWidget;
	delete qdwLog->titleBarWidget();
	delete pmModel;
	delete qtvUsers;
	delete Channel::get(0);
}

void MainWindow::msgBox(QString msg) {
	MessageBoxEvent *mbe=new MessageBoxEvent(msg);
	QApplication::postEvent(this, mbe);
}

#ifdef Q_OS_WIN
bool MainWindow::winEvent(MSG *msg, long *) {
	if (msg->message == WM_DEVICECHANGE && msg->wParam == DBT_DEVNODES_CHANGED)
		uiNewHardware++;
	else if (msg->message == WM_ACTIVATE && msg->wParam == WA_INACTIVE)
		tInactive.restart();

	return false;
}
#endif

void MainWindow::closeEvent(QCloseEvent *e) {
#ifndef Q_OS_MAC
	ServerHandlerPtr sh = g.sh;
	if (sh && sh->isRunning() && g.s.bAskOnQuit && !bSuppressAskOnQuit) {
		QMessageBox mb(QMessageBox::Warning, QLatin1String("Mumble"), tr("Mumble is currently connected to a server. Do you want to Close or Minimize it?"), QMessageBox::NoButton, this);
		mb.addButton(tr("Close"), QMessageBox::YesRole);
		QPushButton *qpbMinimize = mb.addButton(tr("Minimize"), QMessageBox::NoRole);
		QPushButton *qpbCancel = mb.addButton(tr("Cancel"), QMessageBox::RejectRole);
		mb.setDefaultButton(qpbCancel);
		mb.setEscapeButton(qpbCancel);
		mb.exec();
		if (mb.clickedButton() == qpbMinimize) {
			showMinimized();
			e->ignore();
			return;
		} else if (mb.clickedButton() == qpbCancel) {
			e->ignore();
			return;
		}
	}
	sh.reset();
#endif
	g.uiSession = 0;
	g.pPermissions = ChanACL::None;

	if (g.s.bMinimalView) {
		g.s.qbaMinimalViewGeometry = saveGeometry();
		g.s.qbaMinimalViewState = saveState();
	} else {
		g.s.qbaMainWindowGeometry = saveGeometry();
		g.s.qbaMainWindowState = saveState();
		g.s.qbaHeaderState = qtvUsers->header()->saveState();
	}

	if (qwPTTButtonWidget) {
		qwPTTButtonWidget->close();
		qwPTTButtonWidget->deleteLater();
		qwPTTButtonWidget = NULL;
	}
	g.bQuit = true;

	QMainWindow::closeEvent(e);
	qApp->quit();
}

void MainWindow::hideEvent(QHideEvent *e) {
	if (g.ocIntercept) {
		QMetaObject::invokeMethod(g.ocIntercept, "hideGui", Qt::QueuedConnection);
		e->ignore();
		return;
	}
#ifndef Q_OS_MAC
#ifdef Q_OS_UNIX
	if (! qApp->activeModalWidget() && ! qApp->activePopupWidget())
#endif
		if (g.s.bHideInTray && qstiIcon->isSystemTrayAvailable() && e->spontaneous())
			QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
	QMainWindow::hideEvent(e);
#endif
}

void MainWindow::showEvent(QShowEvent *e) {
#ifndef Q_OS_MAC
#ifdef Q_OS_UNIX
	if (! qApp->activeModalWidget() && ! qApp->activePopupWidget())
#endif
		if (g.s.bHideInTray && qstiIcon->isSystemTrayAvailable() && e->spontaneous())
			QMetaObject::invokeMethod(this, "show", Qt::QueuedConnection);
#endif
	QMainWindow::showEvent(e);
}

void MainWindow::updateTrayIcon() {
	ClientUser *p=ClientUser::get(g.uiSession);

	if (g.s.bDeaf) {
		qstiIcon->setIcon(qiIconDeafSelf);
	} else if (p && p->bDeaf) {
		qstiIcon->setIcon(qiIconDeafServer);
	} else if (g.s.bMute) {
		qstiIcon->setIcon(qiIconMuteSelf);
	} else if (p && p->bMute) {
		qstiIcon->setIcon(qiIconMuteServer);
	} else if (p && p->bSuppress) {
		qstiIcon->setIcon(qiIconMuteSuppressed);
	} else if (p && g.s.bStateInTray) {
		switch (p->tsState) {
			case Settings::Talking:
				qstiIcon->setIcon(qiTalkingOn);
				break;
			case Settings::Whispering:
				qstiIcon->setIcon(qiTalkingWhisper);
				break;
			case Settings::Shouting:
				qstiIcon->setIcon(qiTalkingShout);
				break;
			case Settings::Passive:
			default:
				qstiIcon->setIcon(qiTalkingOff);
				break;
		}
	} else {
		qstiIcon->setIcon(qiIcon);
	}
}

Channel *MainWindow::getContextMenuChannel() {
	if (cContextChannel)
		return cContextChannel.data();

	return NULL;
}

ClientUser *MainWindow::getContextMenuUser() {
	if (cuContextUser)
		return cuContextUser.data();

	return NULL;
}

bool MainWindow::handleSpecialContextMenu(const QUrl &url, const QPoint &pos_, bool focus) {
	if (url.scheme() == QString::fromLatin1("clientid")) {
		bool ok = false;
		QString x(url.host());
		if (x.length() == 40) {
			ClientUser *cu = pmModel->getUser(x);
			if (cu) {
				cuContextUser = cu;
				ok = true;
			}
		} else {
			QByteArray qbaServerDigest = QByteArray::fromBase64(url.path().remove(0, 1).toLatin1());
			cuContextUser = ClientUser::get(url.host().toInt(&ok, 10));
			ServerHandlerPtr sh = g.sh;
			ok = ok && sh && (qbaServerDigest == sh->qbaDigest);
		}
		if (ok && cuContextUser) {
			if (focus) {
				qtvUsers->setCurrentIndex(pmModel->index(cuContextUser.data()));
				qteChat->setFocus();
			} else {
				qpContextPosition = QPoint();
				qmUser->exec(pos_, NULL);
			}
		}
		cuContextUser.clear();
	} else if (url.scheme() == QString::fromLatin1("channelid")) {
		bool ok;
		QByteArray qbaServerDigest = QByteArray::fromBase64(url.path().remove(0, 1).toLatin1());
		cContextChannel = Channel::get(url.host().toInt(&ok, 10));
		ServerHandlerPtr sh = g.sh;
		ok = ok && sh && (qbaServerDigest == sh->qbaDigest);
		if (ok) {
			if (focus) {
				qtvUsers->setCurrentIndex(pmModel->index(cContextChannel.data()));
				qteChat->setFocus();
			} else {
				qpContextPosition = QPoint();
				qmChannel->exec(pos_, NULL);
			}
		}
		cContextChannel.clear();
	} else {
		return false;
	}
	return true;
}

void MainWindow::on_qtvUsers_customContextMenuRequested(const QPoint &mpos) {
	QModelIndex idx = qtvUsers->indexAt(mpos);
	if (! idx.isValid())
		idx = qtvUsers->currentIndex();
	else
		qtvUsers->setCurrentIndex(idx);
	ClientUser *p = pmModel->getUser(idx);

	qpContextPosition = mpos;
	if (p) {
		cuContextUser.clear();
		qmUser->exec(qtvUsers->mapToGlobal(mpos), qaUserMute);
		cuContextUser.clear();
	} else {
		cContextChannel.clear();
		qmChannel->exec(qtvUsers->mapToGlobal(mpos), NULL);
		cContextChannel.clear();
	}
	qpContextPosition = QPoint();
}

void MainWindow::on_qteLog_customContextMenuRequested(const QPoint &mpos) {
	QString link = qteLog->anchorAt(mpos);
	if (! link.isEmpty()) {
		QUrl l(link);

		if (handleSpecialContextMenu(l, qteLog->mapToGlobal(mpos)))
			return;
	}

	QPoint contentPosition = QPoint(QApplication::isRightToLeft() ? (qteLog->horizontalScrollBar()->maximum() - qteLog->horizontalScrollBar()->value()) : qteLog->horizontalScrollBar()->value(), qteLog->verticalScrollBar()->value());
	QMenu *menu = qteLog->createStandardContextMenu(mpos + contentPosition);
	menu->addSeparator();
	menu->addAction(tr("Clear"), qteLog, SLOT(clear(void)));
	menu->exec(qteLog->mapToGlobal(mpos));
	delete menu;
}

static void recreateServerHandler() {
	ServerHandlerPtr sh = g.sh;
	if (sh && sh->isRunning()) {
		g.mw->on_qaServerDisconnect_triggered();
		sh->disconnect();
		sh->wait();
		QCoreApplication::instance()->processEvents();
	}

	g.sh.reset();
	while (sh && ! sh.unique())
		QThread::yieldCurrentThread();
	sh.reset();

	sh = ServerHandlerPtr(new ServerHandler());
	sh->moveToThread(sh.get());
	g.sh = sh;
	g.mw->connect(sh.get(), SIGNAL(connected()), g.mw, SLOT(serverConnected()));
	g.mw->connect(sh.get(), SIGNAL(disconnected(QAbstractSocket::SocketError, QString)), g.mw, SLOT(serverDisconnected(QAbstractSocket::SocketError, QString)));
}

void MainWindow::openUrl(const QUrl &url) {
	g.l->log(Log::Information, tr("Opening URL %1").arg(Qt::escape(url.toString())));
	if (url.scheme() == QLatin1String("file")) {
		QFile f(url.toLocalFile());
		if (! f.exists() || ! f.open(QIODevice::ReadOnly)) {
			g.l->log(Log::Warning, tr("File does not exist"));
			return;
		}
		f.close();

		QSettings *qs = new QSettings(f.fileName(), QSettings::IniFormat);
		qs->setIniCodec("UTF-8");
		if (qs->status() != QSettings::NoError) {
			g.l->log(Log::Warning, tr("File is not a configuration file."));
		} else {
			qSwap(qs, g.qs);
			g.s.load();
			qSwap(qs, g.qs);

			g.l->log(Log::Warning, tr("Settings merged from file."));
		}
		delete qs;
		return;
	}
	if (url.scheme() != QLatin1String("mumble")) {
		g.l->log(Log::Warning, tr("URL scheme is not 'mumble'"));
		return;
	}

	int major, minor, patch;
	int thismajor, thisminor, thispatch;
	MumbleVersion::get(&thismajor, &thisminor, &thispatch);

	// With no version parameter given assume the link refers to our version
	major = 1;
	minor = 2;
	patch = 0;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	QUrlQuery query(url);
	QString version = query.queryItemValue(QLatin1String("version"));
#else
	QString version = url.queryItemValue(QLatin1String("version"));
#endif
	MumbleVersion::get(&major, &minor, &patch, version);

	if ((major < 1) || // No pre 1.2.0
	        (major == 1 && minor <= 1) ||
	        (major > thismajor) || // No future version
	        (major == thismajor && minor > thisminor) ||
	        (major == thismajor && minor == thisminor && patch > thispatch)) {
		g.l->log(Log::Warning, tr("This version of Mumble can't handle URLs for Mumble version %1.%2.%3").arg(major).arg(minor).arg(patch));
		return;
	}

	QString host = url.host();
	unsigned short port = static_cast<unsigned short>(url.port(DEFAULT_MUMBLE_PORT));
	QString user = url.userName();
	QString pw = url.password();
	qsDesiredChannel = url.path();
	QString name;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	if (query.hasQueryItem(QLatin1String("title")))
		name = query.queryItemValue(QLatin1String("title"));
#else
	if (url.hasQueryItem(QLatin1String("title")))
		name = url.queryItemValue(QLatin1String("title"));
#endif

	if (g.sh && g.sh->isRunning()) {
		QString oHost, oUser, oPw;
		unsigned short oport;
		g.sh->getConnectionInfo(oHost, oport, oUser, oPw);
		ClientUser *p = ClientUser::get(g.uiSession);

		if ((user.isEmpty() || (p && p->iId >= 0) || (user == oUser)) &&
		        (host.isEmpty() || ((host == oHost) && (port == oport)))) {
			findDesiredChannel();
			return;
		}
	}

	Database::fuzzyMatch(name, user, pw, host, port);

	if (user.isEmpty()) {
		bool ok;
		user = QInputDialog::getText(this, tr("Connecting to %1").arg(url.toString()), tr("Enter username"), QLineEdit::Normal, g.s.qsUsername, &ok);
		if (! ok || user.isEmpty())
			return;
		g.s.qsUsername = user;
	}

	if (name.isEmpty())
		name = QString::fromLatin1("%1@%2").arg(user).arg(host);

	recreateServerHandler();

	g.s.qsLastServer = name;
	rtLast = MumbleProto::Reject_RejectType_None;
	bRetryServer = true;
	qaServerDisconnect->setEnabled(true);
	g.l->log(Log::Information, tr("Connecting to server %1.").arg(Log::msgColor(Qt::escape(host), Log::Server)));
	g.sh->setConnectionInfo(host, port, user, pw);
	g.sh->start(QThread::TimeCriticalPriority);
}

/**
 * This function tries to join a desired channel on connect. It gets called
 * directly after server syncronization is completed.
 * @see void MainWindow::msgServerSync(const MumbleProto::ServerSync &msg)
 */
void MainWindow::findDesiredChannel() {
	bool found = false;
	QStringList qlChans = qsDesiredChannel.split(QLatin1String("/"));
	Channel *chan = Channel::get(0);
	QString str = QString();
	while (chan && qlChans.count() > 0) {
		QString elem = qlChans.takeFirst().toLower();
		if (elem.isEmpty())
			continue;
		if (str.isNull())
			str = elem;
		else
			str = str + QLatin1String("/") + elem;
		foreach(Channel *c, chan->qlChannels) {
			if (c->qsName.toLower() == str) {
				str = QString();
				found = true;
				chan = c;
				break;
			}
		}
	}
	if (found) {
		if (chan != ClientUser::get(g.uiSession)->cChannel) {
			g.sh->joinChannel(g.uiSession, chan->iId);
		}
		qtvUsers->setCurrentIndex(pmModel->index(chan));
	} else if (g.uiSession) {
		qtvUsers->setCurrentIndex(pmModel->index(ClientUser::get(g.uiSession)->cChannel));
	}
	updateMenuPermissions();
}

void MainWindow::setOnTop(bool top) {
	Qt::WindowFlags wf = windowFlags();
	if (wf.testFlag(Qt::WindowStaysOnTopHint) != top) {
		if (top)
			wf |= Qt::WindowStaysOnTopHint;
		else
			wf &= ~Qt::WindowStaysOnTopHint;
		setWindowFlags(wf);
		show();
	}
}

void MainWindow::setupView(bool toggle_minimize) {
	bool showit = ! g.s.bMinimalView;

	// Update window layout
	Settings::WindowLayout wlTmp = g.s.wlWindowLayout;
	switch (wlTmp) {
		case Settings::LayoutClassic:
			removeDockWidget(qdwLog);
			addDockWidget(Qt::LeftDockWidgetArea, qdwLog);
			qdwLog->show();
			splitDockWidget(qdwLog, qdwChat, Qt::Vertical);
			qdwChat->show();
			break;
		case Settings::LayoutStacked:
			removeDockWidget(qdwLog);
			addDockWidget(Qt::BottomDockWidgetArea, qdwLog);
			qdwLog->show();
			splitDockWidget(qdwLog, qdwChat, Qt::Vertical);
			qdwChat->show();
			break;
		case Settings::LayoutHybrid:
			removeDockWidget(qdwLog);
			removeDockWidget(qdwChat);
			addDockWidget(Qt::LeftDockWidgetArea, qdwLog);
			qdwLog->show();
			addDockWidget(Qt::BottomDockWidgetArea, qdwChat);
			qdwChat->show();
			break;
		default:
			wlTmp = Settings::LayoutCustom;
			break;
	}
	qteChat->updateGeometry();
	g.s.wlWindowLayout = wlTmp;

	QRect geom = frameGeometry();

	if (toggle_minimize) {
		if (! showit) {
			g.s.qbaMainWindowGeometry = saveGeometry();
			g.s.qbaMainWindowState = saveState();
			g.s.qbaHeaderState = qtvUsers->header()->saveState();
		} else {
			g.s.qbaMinimalViewGeometry = saveGeometry();
			g.s.qbaMinimalViewState = saveState();
		}
	}

	Qt::WindowFlags f = Qt::Window;
	if (!showit && g.s.bHideFrame)
		f = Qt::Window | Qt::FramelessWindowHint;
#ifndef Q_OS_MAC
	else if (!showit)
		f = Qt::Tool;
#else
	f |= Qt::MacWindowToolBarButtonHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint;
#endif

	if (g.s.aotbAlwaysOnTop == Settings::OnTopAlways ||
	        (g.s.bMinimalView && g.s.aotbAlwaysOnTop == Settings::OnTopInMinimal) ||
	        (!g.s.bMinimalView && g.s.aotbAlwaysOnTop == Settings::OnTopInNormal))
		f |= Qt::WindowStaysOnTopHint;

	if (! graphicsProxyWidget())
		setWindowFlags(f);

	if (g.s.bShowContextMenuInMenuBar) {
		bool found = false;
		foreach(QAction *a, menuBar()->actions()) {
			if (a == qmUser->menuAction()) {
				found = true;
				break;
			}
		}

		if (!found) {
			menuBar()->insertMenu(qmConfig->menuAction(), qmUser);
			menuBar()->insertMenu(qmConfig->menuAction(), qmChannel);
		}
	} else {
		menuBar()->removeAction(qmUser->menuAction());
		menuBar()->removeAction(qmChannel->menuAction());
	}

	if (! showit) {
		qdwLog->setVisible(showit);
		qdwChat->setVisible(showit);
		qtIconToolbar->setVisible(showit);
	}
	menuBar()->setVisible(showit);

	if (toggle_minimize) {
		if (! showit) {
			if (! g.s.qbaMinimalViewGeometry.isNull())
				restoreGeometry(g.s.qbaMinimalViewGeometry);
			if (! g.s.qbaMinimalViewState.isNull())
				restoreState(g.s.qbaMinimalViewState);
		} else {
			if (! g.s.qbaMainWindowGeometry.isNull())
				restoreGeometry(g.s.qbaMainWindowGeometry);
			if (! g.s.qbaMainWindowState.isNull())
				restoreState(g.s.qbaMainWindowState);
		}
	} else {
		QRect newgeom = frameGeometry();
		resize(geometry().width()-newgeom.width()+geom.width(),
		       geometry().height()-newgeom.height()+geom.height());
		move(geom.x(), geom.y());
	}

	show();
	activateWindow();

	// If activated show the PTT window
	if (g.s.bShowPTTButtonWindow && g.s.atTransmit == Settings::PushToTalk) {
		if (qwPTTButtonWidget) {
			qwPTTButtonWidget->show();
		} else {
			qwPTTButtonWidget = new PTTButtonWidget();
			qwPTTButtonWidget->show();
			connect(qwPTTButtonWidget, SIGNAL(triggered(bool,QVariant)), SLOT(on_PushToTalk_triggered(bool,QVariant)));
		}
	} else {
		if (qwPTTButtonWidget) {
			qwPTTButtonWidget->deleteLater();
			qwPTTButtonWidget = NULL;
		}
	}
}

void MainWindow::on_qaServerConnect_triggered(bool autoconnect) {
	ConnectDialog *cd = new ConnectDialog(this, autoconnect);
	int res = cd->exec();

	if (cd->qsServer.isEmpty() || (cd->usPort==0) || cd->qsUsername.isEmpty())
		res = QDialog::Rejected;

	if (res == QDialog::Accepted) {
		recreateServerHandler();
		qsDesiredChannel = QString();
		rtLast = MumbleProto::Reject_RejectType_None;
		bRetryServer = true;
		qaServerDisconnect->setEnabled(true);
		g.l->log(Log::Information, tr("Connecting to server %1.").arg(Log::msgColor(Qt::escape(cd->qsServer), Log::Server)));
		g.sh->setConnectionInfo(cd->qsServer, cd->usPort, cd->qsUsername, cd->qsPassword);
		g.sh->start(QThread::TimeCriticalPriority);
	}
	delete cd;
}

void MainWindow::on_Reconnect_timeout() {
	if (g.sh->isRunning())
		return;
	g.l->log(Log::Information, tr("Reconnecting."));
	g.sh->start(QThread::TimeCriticalPriority);
}

void MainWindow::on_qmSelf_aboutToShow() {
	ClientUser *user = ClientUser::get(g.uiSession);

	qaServerTexture->setEnabled(user != NULL);
	qaSelfComment->setEnabled(user != NULL);

	qaServerTextureRemove->setEnabled(user && ! user->qbaTextureHash.isEmpty());

	qaSelfRegister->setEnabled(user && (user->iId < 0) && ! user->qsHash.isEmpty() && (g.pPermissions & (ChanACL::SelfRegister | ChanACL::Write)));
	if (g.sh && g.sh->uiVersion >= 0x010203) {
		qaSelfPrioritySpeaker->setEnabled(user && g.pPermissions & (ChanACL::Write | ChanACL::MuteDeafen));
		qaSelfPrioritySpeaker->setChecked(user && user->bPrioritySpeaker);
	} else {
		qaSelfPrioritySpeaker->setEnabled(false);
		qaSelfPrioritySpeaker->setChecked(false);
	}
}

void MainWindow::on_qaSelfComment_triggered() {
	ClientUser *p = ClientUser::get(g.uiSession);
	if (!p)
		return;

	if (! p->qbaCommentHash.isEmpty() && p->qsComment.isEmpty()) {
		p->qsComment = QString::fromUtf8(Database::blob(p->qbaCommentHash));
		if (p->qsComment.isEmpty()) {
			pmModel->uiSessionComment = ~(p->uiSession);
			MumbleProto::RequestBlob mprb;
			mprb.add_session_comment(p->uiSession);
			g.sh->sendMessage(mprb);
			return;
		}
	}

	unsigned int session = p->uiSession;

	::TextMessage *texm = new ::TextMessage(this, tr("Change your comment"));

	texm->rteMessage->setText(p->qsComment);
	int res = texm->exec();

	p = ClientUser::get(session);

	if (p && (res == QDialog::Accepted)) {
		const QString &msg = texm->message();
		MumbleProto::UserState mpus;
		mpus.set_session(session);
		mpus.set_comment(u8(msg));
		g.sh->sendMessage(mpus);

		if (! msg.isEmpty())
			Database::setBlob(sha1(msg), msg.toUtf8());
	}
	delete texm;
}

void MainWindow::on_qaSelfRegister_triggered() {
	ClientUser *p = ClientUser::get(g.uiSession);
	if (!p)
		return;

	QMessageBox::StandardButton result;
	result = QMessageBox::question(this, tr("Register yourself as %1").arg(p->qsName), tr("<p>You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as '%1' on this server.</p><p>Are you sure you want to register yourself?</p>").arg(Qt::escape(p->qsName)), QMessageBox::Yes|QMessageBox::No);

	if (result == QMessageBox::Yes)
		g.sh->registerUser(p->uiSession);
}

void MainWindow::on_qmServer_aboutToShow() {
	qmServer->clear();
	qmServer->addAction(qaServerConnect);
	qmServer->addAction(qaServerDisconnect);
	qmServer->addAction(qaServerBanList);
	qmServer->addAction(qaServerUserList);
	qmServer->addAction(qaServerInformation);
	qmServer->addAction(qaServerTokens);
	qmServer->addSeparator();
	qmServer->addAction(qaQuit);

	qaServerBanList->setEnabled(g.pPermissions & (ChanACL::Ban | ChanACL::Write));
	qaServerUserList->setEnabled(g.pPermissions & (ChanACL::Register | ChanACL::Write));
	qaServerInformation->setEnabled(g.uiSession != 0);
	qaServerTokens->setEnabled(g.uiSession != 0);

	if (! qlServerActions.isEmpty()) {
		qmServer->addSeparator();
		foreach(QAction *a, qlServerActions)
			qmServer->addAction(a);
	}
}

void MainWindow::on_qaServerDisconnect_triggered() {
	if (qtReconnect->isActive()) {
		qtReconnect->stop();
		qaServerDisconnect->setEnabled(false);
	}
	if (g.sh && g.sh->isRunning())
		g.sh->disconnect();
}

void MainWindow::on_qaServerBanList_triggered() {
	g.sh->requestBanList();

	if (banEdit) {
		banEdit->reject();
		delete banEdit;
		banEdit = NULL;
	}
}

void MainWindow::on_qaServerUserList_triggered() {
	g.sh->requestUserList();

	if (userEdit) {
		userEdit->reject();
		delete userEdit;
		userEdit = NULL;
	}
}

static const QString currentCodec() {
	if (g.bOpus)
		return QLatin1String("Opus");

	int v = g.bPreferAlpha ? g.iCodecAlpha : g.iCodecBeta;
	CELTCodec* cc = g.qmCodecs.value(v);
	if (cc)
		return QString::fromLatin1("CELT %1").arg(cc->version());
	else
		return QString::fromLatin1("CELT %1").arg(QString::number(v, 16));
}

void MainWindow::on_qaServerInformation_triggered() {
	ConnectionPtr c = g.sh->cConnection;

	if (! c)
		return;

	CryptState &cs = c->csCrypt;
	QSslCipher qsc = g.sh->qscCipher;

	QString qsVersion=tr("<h2>Version</h2><p>Protocol %1.</p>").arg(MumbleVersion::toString(g.sh->uiVersion));

	if (g.sh->qsRelease.isEmpty() || g.sh->qsOS.isEmpty() || g.sh->qsOSVersion.isEmpty()) {
		qsVersion.append(tr("<p>No build information or OS version available.</p>"));
	} else {
		qsVersion.append(tr("<p>%1 (%2)<br />%3</p>")
		                 .arg(Qt::escape(g.sh->qsRelease), Qt::escape(g.sh->qsOS), Qt::escape(g.sh->qsOSVersion)));
	}

	QString host, uname, pw;
	unsigned short port;

	g.sh->getConnectionInfo(host,port,uname,pw);

	QString qsControl=tr("<h2>Control channel</h2><p>Encrypted with %1 bit %2<br />%3 ms average latency (%4 deviation)</p><p>Remote host %5 (port %6)</p>").arg(QString::number(qsc.usedBits()),
	                  Qt::escape(qsc.name()),
	                  QString::fromLatin1("%1").arg(boost::accumulators::mean(g.sh->accTCP), 0, 'f', 2),
	                  QString::fromLatin1("%1").arg(sqrt(boost::accumulators::variance(g.sh->accTCP)),0,'f',2),
	                  Qt::escape(host),
	                  QString::number(port));
	QString qsVoice, qsCrypt, qsAudio;

	if (NetworkConfig::TcpModeEnabled()) {
		qsVoice = tr("Voice channel is sent over control channel.");
	} else {
		qsVoice = tr("<h2>Voice channel</h2><p>Encrypted with 128 bit OCB-AES128<br />%1 ms average latency (%4 deviation)</p>").arg(boost::accumulators::mean(g.sh->accUDP), 0, 'f', 2).arg(sqrt(boost::accumulators::variance(g.sh->accUDP)),0,'f',2);
		qsCrypt = QString::fromLatin1("<h2>%1</h2><table><tr><th></th><th>%2</th><th>%3</th></tr>"
		                              "<tr><th>%4</th><td>%8</td><td>%12</td></tr>"
		                              "<tr><th>%5</th><td>%9</td><td>%13</td></tr>"
		                              "<tr><th>%6</th><td>%10</td><td>%14</td></tr>"
		                              "<tr><th>%7</th><td>%11</td><td>%15</td></tr>"
		                              "</table>")
		          .arg(tr("UDP Statistics")).arg(tr("To Server")).arg(tr("From Server")).arg(tr("Good")).arg(tr("Late")).arg(tr("Lost")).arg(tr("Resync"))
		          .arg(cs.uiRemoteGood).arg(cs.uiRemoteLate).arg(cs.uiRemoteLost).arg(cs.uiRemoteResync)
		          .arg(cs.uiGood).arg(cs.uiLate).arg(cs.uiLost).arg(cs.uiResync);
	}
	qsAudio=tr("<h2>Audio bandwidth</h2><p>Maximum %1 kbit/s<br />Current %2 kbit/s<br />Codec: %3</p>").arg(g.iMaxBandwidth / 1000.0,0,'f',1).arg(g.iAudioBandwidth / 1000.0,0,'f',1).arg(currentCodec());

	QMessageBox qmb(QMessageBox::Information, tr("Mumble Server Information"), qsVersion + qsControl + qsVoice + qsCrypt + qsAudio, QMessageBox::Ok, this);
	qmb.setDefaultButton(QMessageBox::Ok);
	qmb.setEscapeButton(QMessageBox::Ok);

	QPushButton *qp = qmb.addButton(tr("&View Certificate"), QMessageBox::ActionRole);
	int res = qmb.exec();
	if ((res == 0) && (qmb.clickedButton() == qp)) {
		ViewCert vc(g.sh->qscCert, this);
		vc.exec();
	}
}

void MainWindow::on_qaServerTexture_triggered() {
	QPair<QByteArray, QImage> choice = openImageFile();
	if (choice.first.isEmpty())
		return;

	const QImage &img = choice.second;

	if ((img.height() <= 1024) && (img.width() <= 1024))
		g.sh->setUserTexture(g.uiSession, choice.first);
}

void MainWindow::on_qaServerTextureRemove_triggered() {
	g.sh->setUserTexture(g.uiSession, QByteArray());
}

void MainWindow::on_qaServerTokens_triggered() {
	if (tokenEdit) {
		tokenEdit->reject();
		delete tokenEdit;
		tokenEdit = NULL;
	}

	tokenEdit = new Tokens(this);
	tokenEdit->show();
}

void MainWindow::voiceRecorderDialog_finished(int) {
	voiceRecorderDialog->deleteLater();
	voiceRecorderDialog = NULL;
}

void MainWindow::qmUser_aboutToShow() {
	ClientUser *p = NULL;
	if (g.uiSession != 0) {
		QModelIndex idx;
		if (! qpContextPosition.isNull())
			idx = qtvUsers->indexAt(qpContextPosition);

		if (! idx.isValid())
			idx = qtvUsers->currentIndex();

		p = pmModel->getUser(idx);

		if (cuContextUser)
			p = cuContextUser.data();
	}

	cuContextUser = p;
	qpContextPosition = QPoint();

	bool self = p && (p->uiSession == g.uiSession);

	qmUser->clear();

	if (g.pPermissions & (ChanACL::Kick | ChanACL::Ban | ChanACL::Write))
		qmUser->addAction(qaUserKick);
	if (g.pPermissions & (ChanACL::Ban | ChanACL::Write))
		qmUser->addAction(qaUserBan);
	qmUser->addAction(qaUserMute);
	qmUser->addAction(qaUserDeaf);
	if (g.sh && g.sh->uiVersion >= 0x010203)
		qmUser->addAction(qaUserPrioritySpeaker);
	qmUser->addAction(qaUserLocalMute);
	qmUser->addAction(qaUserLocalIgnore);

	if (self)
		qmUser->addAction(qaSelfComment);
	else {
		qmUser->addAction(qaUserCommentView);
		qmUser->addAction(qaUserCommentReset);
		qmUser->addAction(qaUserTextureReset);
	}

	qmUser->addAction(qaUserTextMessage);
	if (g.sh && g.sh->uiVersion >= 0x010202)
		qmUser->addAction(qaUserInformation);

	if (p && (p->iId < 0) && ! p->qsHash.isEmpty() && (g.pPermissions & ((self ? ChanACL::SelfRegister : ChanACL::Register) | ChanACL::Write))) {
		qmUser->addSeparator();
		qmUser->addAction(qaUserRegister);
	}

	if (p && ! p->qsHash.isEmpty() && (!p->qsFriendName.isEmpty() || (p->uiSession != g.uiSession))) {
		qmUser->addSeparator();
		if (p->qsFriendName.isEmpty())
			qmUser->addAction(qaUserFriendAdd);
		else {
			if (p->qsFriendName != p->qsName)
				qmUser->addAction(qaUserFriendUpdate);
			qmUser->addAction(qaUserFriendRemove);
		}
	}

	if (self) {
		qmUser->addSeparator();
		qmUser->addAction(qaAudioMute);
		qmUser->addAction(qaAudioDeaf);
	}

#ifndef Q_OS_MAC
	if (g.s.bMinimalView) {
		qmUser->addSeparator();
		qmUser->addMenu(qmServer);
		qmUser->addMenu(qmSelf);
		qmUser->addMenu(qmConfig);
		qmUser->addMenu(qmHelp);
	}
#endif

	if (! qlUserActions.isEmpty()) {
		qmUser->addSeparator();
		foreach(QAction *a, qlUserActions)
			qmUser->addAction(a);
	}

	if (! p) {
		qaUserKick->setEnabled(false);
		qaUserBan->setEnabled(false);
		qaUserTextMessage->setEnabled(false);
		qaUserLocalMute->setEnabled(false);
		qaUserLocalIgnore->setEnabled(false);
		qaUserCommentReset->setEnabled(false);
		qaUserTextureReset->setEnabled(false);
		qaUserCommentView->setEnabled(false);
	} else {
		qaUserKick->setEnabled(! self);
		qaUserBan->setEnabled(! self);
		qaUserTextMessage->setEnabled(true);
		qaUserLocalMute->setEnabled(! self);
		qaUserLocalIgnore->setEnabled(! self);
		qaUserCommentReset->setEnabled(! p->qbaCommentHash.isEmpty() && (g.pPermissions & (ChanACL::Move | ChanACL::Write)));
		qaUserTextureReset->setEnabled(! p->qbaTextureHash.isEmpty() && (g.pPermissions & (ChanACL::Move | ChanACL::Write)));
		qaUserCommentView->setEnabled(! p->qbaCommentHash.isEmpty());

		qaUserMute->setChecked(p->bMute || p->bSuppress);
		qaUserDeaf->setChecked(p->bDeaf);
		qaUserPrioritySpeaker->setChecked(p->bPrioritySpeaker);
		qaUserLocalMute->setChecked(p->bLocalMute);
		qaUserLocalIgnore->setChecked(p->bLocalIgnore);
	}
	updateMenuPermissions();
}

void MainWindow::on_qaUserMute_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	MumbleProto::UserState mpus;
	mpus.set_session(p->uiSession);
	if (p->bMute || p->bSuppress) {
		if (p->bMute)
			mpus.set_mute(false);
		if (p->bSuppress)
			mpus.set_suppress(false);
	} else {
		mpus.set_mute(true);
	}
	g.sh->sendMessage(mpus);
}

void MainWindow::on_qaUserLocalMute_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	bool muted = qaUserLocalMute->isChecked();

	p->setLocalMute(muted);
	if (! p->qsHash.isEmpty())
		Database::setLocalMuted(p->qsHash, muted);
}

void MainWindow::on_qaUserLocalIgnore_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	bool ignored = qaUserLocalIgnore->isChecked();

	p->setLocalIgnore(ignored);
	if (! p->qsHash.isEmpty())
		Database::setLocalIgnored(p->qsHash, ignored);
}

void MainWindow::on_qaUserDeaf_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	MumbleProto::UserState mpus;
	mpus.set_session(p->uiSession);
	mpus.set_deaf(! p->bDeaf);
	g.sh->sendMessage(mpus);
}

void MainWindow::on_qaSelfPrioritySpeaker_triggered() {
	ClientUser *p = ClientUser::get(g.uiSession);
	if (!p)
		return;

	MumbleProto::UserState mpus;
	mpus.set_session(p->uiSession);
	mpus.set_priority_speaker(! p->bPrioritySpeaker);
	g.sh->sendMessage(mpus);
}

void MainWindow::on_qaUserPrioritySpeaker_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	MumbleProto::UserState mpus;
	mpus.set_session(p->uiSession);
	mpus.set_priority_speaker(! p->bPrioritySpeaker);
	g.sh->sendMessage(mpus);
}

void MainWindow::on_qaUserRegister_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	unsigned int session = p->uiSession;

	QMessageBox::StandardButton result;

	if (session == g.uiSession)
		result = QMessageBox::question(this, tr("Register yourself as %1").arg(p->qsName), tr("<p>You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as '%1' on this server.</p><p>Are you sure you want to register yourself?</p>").arg(Qt::escape(p->qsName)), QMessageBox::Yes|QMessageBox::No);
	else
		result = QMessageBox::question(this, tr("Register user %1").arg(p->qsName), tr("<p>You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.</p><p>From this point on, %1 will be authenticated with the certificate currently in use.</p><p>Are you sure you want to register %1?</p>").arg(Qt::escape(p->qsName)), QMessageBox::Yes|QMessageBox::No);

	if (result == QMessageBox::Yes) {
		p = ClientUser::get(session);
		if (! p)
			return;
		g.sh->registerUser(p->uiSession);
	}
}

void MainWindow::on_qaUserFriendAdd_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	Database::addFriend(p->qsName, p->qsHash);
	pmModel->setFriendName(p, p->qsName);
}

void MainWindow::on_qaUserFriendUpdate_triggered() {
	on_qaUserFriendAdd_triggered();
}

void MainWindow::on_qaUserFriendRemove_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	Database::removeFriend(p->qsHash);
	pmModel->setFriendName(p, QString());
}

void MainWindow::on_qaUserKick_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	unsigned int session = p->uiSession;

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Kicking user %1").arg(p->qsName), tr("Enter reason"), QLineEdit::Normal, QString(), &ok);

	p = ClientUser::get(session);
	if (!p)
		return;

	if (ok) {
		g.sh->kickBanUser(p->uiSession, reason, false);
	}
}

void MainWindow::on_qaUserBan_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	unsigned int session = p->uiSession;

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Banning user %1").arg(p->qsName), tr("Enter reason"), QLineEdit::Normal, QString(), &ok);
	p = ClientUser::get(session);
	if (!p)
		return;

	if (ok) {
		g.sh->kickBanUser(p->uiSession, reason, true);
	}
}

void MainWindow::on_qaUserTextMessage_triggered() {
	ClientUser *p = getContextMenuUser();

	if (!p)
		return;

	openTextMessageDialog(p);
}

void MainWindow::openTextMessageDialog(ClientUser *p) {
	unsigned int session = p->uiSession;

	::TextMessage *texm = new ::TextMessage(this, tr("Sending message to %1").arg(p->qsName));
	int res = texm->exec();

	// Try to get find the user using the session id.
	// This will return NULL if the user disconnected while typing the message.
	p = ClientUser::get(session);

	if (p && (res == QDialog::Accepted)) {
		QString msg = texm->message();

		if (! msg.isEmpty()) {
			g.sh->sendUserTextMessage(p->uiSession, msg);
			g.l->log(Log::TextMessage, tr("To %1: %2").arg(Log::formatClientUser(p, Log::Target), texm->message()), tr("Message to %1").arg(p->qsName), true);
		}
	}
	delete texm;
}

void MainWindow::on_qaUserCommentView_triggered() {
	ClientUser *p = getContextMenuUser();
	// This has to be done here because UserModel could've set it.
	cuContextUser.clear();

	if (!p)
		return;

	if (! p->qbaCommentHash.isEmpty() && p->qsComment.isEmpty()) {
		p->qsComment = QString::fromUtf8(Database::blob(p->qbaCommentHash));
		if (p->qsComment.isEmpty()) {
			pmModel->uiSessionComment = ~(p->uiSession);
			MumbleProto::RequestBlob mprb;
			mprb.add_session_comment(p->uiSession);
			g.sh->sendMessage(mprb);
			return;
		}
	}

	pmModel->seenComment(pmModel->index(p));

	::TextMessage *texm = new ::TextMessage(this, tr("View comment on user %1").arg(p->qsName));

	texm->rteMessage->setText(p->qsComment, true);
	texm->setAttribute(Qt::WA_DeleteOnClose, true);
	texm->show();
}

void MainWindow::on_qaUserCommentReset_triggered() {
	ClientUser *p = getContextMenuUser();

	if (!p)
		return;

	unsigned int session = p->uiSession;

	int ret = QMessageBox::question(this, QLatin1String("Mumble"),
	                                tr("Are you sure you want to reset the comment of user %1?").arg(Qt::escape(p->qsName)),
	                                QMessageBox::Yes, QMessageBox::No);
	if (ret == QMessageBox::Yes) {
		g.sh->setUserComment(session, QString());
	}
}

void MainWindow::on_qaUserTextureReset_triggered() {
	ClientUser *p = getContextMenuUser();

	if (!p)
		return;

	unsigned int session = p->uiSession;

	int ret = QMessageBox::question(this, QLatin1String("Mumble"),
	                                tr("Are you sure you want to reset the avatar of user %1?").arg(Qt::escape(p->qsName)),
	                                QMessageBox::Yes, QMessageBox::No);
	if (ret == QMessageBox::Yes) {
		g.sh->setUserTexture(session, QByteArray());
	}
}

void MainWindow::on_qaUserInformation_triggered() {
	ClientUser *p = getContextMenuUser();

	if (!p)
		return;

	g.sh->requestUserStats(p->uiSession, false);
}

void MainWindow::on_qaQuit_triggered() {
	bSuppressAskOnQuit = true;
	this->close();
}

void MainWindow::sendChatbarMessage(QString qsText) {
	if (g.uiSession == 0) return; // Check if text & connection is available

	ClientUser *p = pmModel->getUser(qtvUsers->currentIndex());
	Channel *c = pmModel->getChannel(qtvUsers->currentIndex());

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	qsText = qsText.toHtmlEscaped();
#else
	qsText = Qt::escape(qsText);
#endif
	qsText = TextMessage::autoFormat(qsText);

	if (!g.s.bChatBarUseSelection || p == NULL || p->uiSession == g.uiSession) {
		// Channel message
		if (!g.s.bChatBarUseSelection || c == NULL) // If no channel selected fallback to current one
			c = ClientUser::get(g.uiSession)->cChannel;

		g.sh->sendChannelTextMessage(c->iId, qsText, false);
		g.l->log(Log::TextMessage, tr("To %1: %2").arg(Log::formatChannel(c), qsText), tr("Message to channel %1").arg(c->qsName), true);
	} else {
		// User message
		g.sh->sendUserTextMessage(p->uiSession, qsText);
		g.l->log(Log::TextMessage, tr("To %1: %2").arg(Log::formatClientUser(p, Log::Target), qsText), tr("Message to %1").arg(p->qsName), true);
	}

	qteChat->clear();
}

/**
 * Controlls tab username completion for the chatbar.
 * @see ChatbarLineEdit::completeAtCursor()
 */
void MainWindow::on_qteChat_tabPressed() {
	qteChat->completeAtCursor();
}

/**
 * Controlls ctrl space username completion and selection for the chatbar.
 * @see ChatbarLineEdit::completeAtCursor()
 */
void MainWindow::on_qteChat_ctrlSpacePressed() {
	unsigned int res = qteChat->completeAtCursor();
	if (res == 0) return;
	qtvUsers->setCurrentIndex(pmModel->index(ClientUser::get(res)));
}

void MainWindow::on_qmConfig_aboutToShow() {
	// Don't remove the config, as that messes up OSX.
	foreach(QAction *a, qmConfig->actions())
		if (a != qaConfigDialog)
			qmConfig->removeAction(a);
	qmConfig->addAction(qaAudioWizard);
	qmConfig->addAction(qaConfigCert);
	qmConfig->addSeparator();
	qmConfig->addAction(qaAudioTTS);
	qmConfig->addSeparator();
	qmConfig->addAction(qaConfigMinimal);
	if (g.s.bMinimalView)
		qmConfig->addAction(qaConfigHideFrame);
}

void MainWindow::qmChannel_aboutToShow() {
	qmChannel->clear();

	Channel *c = NULL;
	if (g.uiSession != 0) {
		QModelIndex idx;
		if (! qpContextPosition.isNull())
			idx = qtvUsers->indexAt(qpContextPosition);

		if (! idx.isValid())
			idx = qtvUsers->currentIndex();

		c = pmModel->getChannel(idx);

		if (cContextChannel)
			c = cContextChannel.data();
	}

	cContextChannel = c;
	qpContextPosition = QPoint();

	if (c && c->iId != ClientUser::get(g.uiSession)->cChannel->iId) {
		qmChannel->addAction(qaChannelJoin);
		qmChannel->addSeparator();
	}

	qmChannel->addAction(qaChannelAdd);
	qmChannel->addAction(qaChannelACL);
	qmChannel->addAction(qaChannelRemove);
	qmChannel->addSeparator();
	qmChannel->addAction(qaChannelLink);
	qmChannel->addAction(qaChannelUnlink);
	qmChannel->addAction(qaChannelUnlinkAll);
	qmChannel->addSeparator();
	qmChannel->addAction(qaChannelCopyURL);
	qmChannel->addAction(qaChannelSendMessage);

	// hiding the root is nonsense
	if(c && c->cParent) {
		qmChannel->addSeparator();
		qmChannel->addAction(qaChannelFilter);
	}

#ifndef Q_OS_MAC
	if (g.s.bMinimalView) {
		qmChannel->addSeparator();
		qmChannel->addMenu(qmServer);
		qmChannel->addMenu(qmSelf);
		qmChannel->addMenu(qmConfig);
		qmChannel->addMenu(qmHelp);
	}
#endif

	if (! qlChannelActions.isEmpty()) {
		qmChannel->addSeparator();
		foreach(QAction *a, qlChannelActions)
			qmChannel->addAction(a);
	}

	bool add, remove, acl, link, unlink, unlinkall, msg, hide;
	add = remove = acl = link = unlink = unlinkall = msg = hide = false;

	if (g.uiSession != 0) {
		add = true;
		acl = true;
		msg = true;

		Channel *home = ClientUser::get(g.uiSession)->cChannel;

		if (c && c->iId != 0) {
			remove = true;
		}
		if (! c)
			c = Channel::get(0);
		unlinkall = (home->qhLinks.count() > 0);
		if (home != c) {
			if (c->allLinks().contains(home))
				unlink = true;
			else
				link = true;
		}
	}

	if(c)
		qaChannelFilter->setChecked(c->bFiltered);

	qaChannelAdd->setEnabled(add);
	qaChannelRemove->setEnabled(remove);
	qaChannelACL->setEnabled(acl);
	qaChannelLink->setEnabled(link);
	qaChannelUnlink->setEnabled(unlink);
	qaChannelUnlinkAll->setEnabled(unlinkall);
	qaChannelSendMessage->setEnabled(msg);
	updateMenuPermissions();
}

void MainWindow::on_qaChannelJoin_triggered() {
	Channel *c = getContextMenuChannel();

	if (c) {
		g.sh->joinChannel(g.uiSession, c->iId);
	}
}

void MainWindow::on_qaChannelFilter_triggered() {
	Channel *c = getContextMenuChannel();
	
	if (c) {
		UserModel *um = static_cast<UserModel *>(qtvUsers->model());
		um->toggleChannelFiltered(c);
	}
}

void MainWindow::on_qaChannelAdd_triggered() {
	Channel *c = getContextMenuChannel();
	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = NULL;
	}

	aclEdit = new ACLEditor(c ? c->iId : 0, this);
	if (c && (c->uiPermissions & ChanACL::Cached) && !(c->uiPermissions & (ChanACL::Write | ChanACL::MakeChannel))) {
		aclEdit->qcbChannelTemporary->setEnabled(false);
		aclEdit->qcbChannelTemporary->setChecked(true);
	}

	aclEdit->show();
}

void MainWindow::on_qaChannelRemove_triggered() {
	int ret;
	Channel *c = getContextMenuChannel();
	if (! c)
		return;

	int id = c->iId;

	ret=QMessageBox::question(this, QLatin1String("Mumble"), tr("Are you sure you want to delete %1 and all its sub-channels?").arg(Qt::escape(c->qsName)), QMessageBox::Yes, QMessageBox::No);

	c = Channel::get(id);
	if (!c)
		return;

	if (ret == QMessageBox::Yes) {
		g.sh->removeChannel(c->iId);
	}
}

void MainWindow::on_qaChannelACL_triggered() {
	Channel *c = getContextMenuChannel();
	if (! c)
		c = Channel::get(0);
	int id = c->iId;

	if (! c->qbaDescHash.isEmpty() && c->qsDesc.isEmpty()) {
		c->qsDesc = QString::fromUtf8(Database::blob(c->qbaDescHash));
		if (c->qsDesc.isEmpty()) {
			MumbleProto::RequestBlob mprb;
			mprb.add_channel_description(id);
			g.sh->sendMessage(mprb);
		}
	}

	g.sh->requestACL(id);

	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = NULL;
	}
}

void MainWindow::on_qaChannelLink_triggered() {
	Channel *c = ClientUser::get(g.uiSession)->cChannel;
	Channel *l = getContextMenuChannel();
	if (! l)
		l = Channel::get(0);

	g.sh->addChannelLink(c->iId, l->iId);
}

void MainWindow::on_qaChannelUnlink_triggered() {
	Channel *c = ClientUser::get(g.uiSession)->cChannel;
	Channel *l = getContextMenuChannel();
	if (! l)
		l = Channel::get(0);

	g.sh->removeChannelLink(c->iId, l->iId);
}

void MainWindow::on_qaChannelUnlinkAll_triggered() {
	Channel *c = ClientUser::get(g.uiSession)->cChannel;

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(c->iId);
	foreach(Channel *l, c->qsPermLinks)
		mpcs.add_links_remove(l->iId);
	g.sh->sendMessage(mpcs);
}

void MainWindow::on_qaChannelSendMessage_triggered() {
	Channel *c = getContextMenuChannel();

	if (!c)
		return;

	int id = c->iId;

	::TextMessage *texm = new ::TextMessage(this, tr("Sending message to channel %1").arg(c->qsName), true);
	int res = texm->exec();

	c = Channel::get(id);

	if (c && (res==QDialog::Accepted)) {
		g.sh->sendChannelTextMessage(id, texm->message(), texm->bTreeMessage);

		if (texm->bTreeMessage)
			g.l->log(Log::TextMessage, tr("To %1 (Tree): %2").arg(Log::formatChannel(c), texm->message()), tr("Message to tree %1").arg(c->qsName), true);
		else
			g.l->log(Log::TextMessage, tr("To %1: %2").arg(Log::formatChannel(c), texm->message()), tr("Message to channel %1").arg(c->qsName), true);
	}
	delete texm;
}

void MainWindow::on_qaChannelCopyURL_triggered() {
	Channel *c = getContextMenuChannel();
	QString host, uname, pw, channel;
	unsigned short port;

	if (!c)
		return;

	g.sh->getConnectionInfo(host, port, uname, pw);
	// walk back up the channel list to build the URL.
	while (c->cParent != NULL) {
		channel.prepend(c->qsName);
		channel.prepend(QLatin1String("/"));
		c = c->cParent;
	}

	QApplication::clipboard()->setMimeData(ServerItem::toMimeData(c->qsName, host, port, channel), QClipboard::Clipboard);
}

/**
 * This function updates the UI according to the permission of the user in the current channel.
 * If possible the permissions are fetched from a cache. Otherwise they are requested by the server
 * via a PermissionQuery call (whose reply updates the cache and calls this function again).
 * @see MainWindow::msgPermissionQuery(const MumbleProto::PermissionQuery &msg)
 */
void MainWindow::updateMenuPermissions() {
	ClientUser *cu = NULL;
	Channel *c = NULL;

	if (g.uiSession) {
		cu = getContextMenuUser();
		if (!cu)
			cu = pmModel->getUser(qtvUsers->currentIndex());

		c = cu ? cu->cChannel : getContextMenuChannel();
		if (!c)
			c = pmModel->getChannel(qtvUsers->currentIndex());
	}

	ChanACL::Permissions p = c ? static_cast<ChanACL::Permissions>(c->uiPermissions) : ChanACL::None;

	if (c && ! p) {
		g.sh->requestChannelPermissions(c->iId);
		if (c->iId == 0)
			p = g.pPermissions;
		else
			p = ChanACL::All;

		c->uiPermissions = p;
	}

	Channel *cparent = c ? c->cParent : NULL;
	ChanACL::Permissions pparent = cparent ? static_cast<ChanACL::Permissions>(cparent->uiPermissions) : ChanACL::None;

	if (cparent && ! pparent) {
		g.sh->requestChannelPermissions(cparent->iId);
		if (cparent->iId == 0)
			pparent = g.pPermissions;
		else
			pparent = ChanACL::All;

		cparent->uiPermissions = pparent;
	}

	ClientUser *user = g.uiSession ? ClientUser::get(g.uiSession) : NULL;
	Channel *homec = user ? user->cChannel : NULL;
	ChanACL::Permissions homep = homec ? static_cast<ChanACL::Permissions>(homec->uiPermissions) : ChanACL::None;

	if (homec && ! homep) {
		g.sh->requestChannelPermissions(homec->iId);
		if (homec->iId == 0)
			homep = g.pPermissions;
		else
			homep = ChanACL::All;

		homec->uiPermissions = homep;
	}

	if (cu) {
		qaUserMute->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen) && ((cu != user) || cu->bMute || cu->bSuppress));
		qaUserDeaf->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen) && ((cu != user) || cu->bDeaf));
		qaUserPrioritySpeaker->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen));
		qaUserTextMessage->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));
		qaUserInformation->setEnabled((g.pPermissions & (ChanACL::Write | ChanACL::Register)) || (p & (ChanACL::Write | ChanACL::Enter)) || (cu == user));
	} else {
		qaUserMute->setEnabled(false);
		qaUserDeaf->setEnabled(false);
		qaUserPrioritySpeaker->setEnabled(false);
		qaUserTextMessage->setEnabled(false);
		qaUserInformation->setEnabled(false);
	}

	qaChannelJoin->setEnabled(p & (ChanACL::Write | ChanACL::Enter));

	qaChannelAdd->setEnabled(p & (ChanACL::Write | ChanACL::MakeChannel | ChanACL::MakeTempChannel));
	qaChannelRemove->setEnabled(p & ChanACL::Write);
	qaChannelACL->setEnabled((p & ChanACL::Write) || (pparent & ChanACL::Write));

	qaChannelLink->setEnabled((p & (ChanACL::Write | ChanACL::LinkChannel)) && (homep & (ChanACL::Write | ChanACL::LinkChannel)));
	qaChannelUnlink->setEnabled((p & (ChanACL::Write | ChanACL::LinkChannel)) || (homep & (ChanACL::Write | ChanACL::LinkChannel)));
	qaChannelUnlinkAll->setEnabled(p & (ChanACL::Write | ChanACL::LinkChannel));

	qaChannelCopyURL->setEnabled(c);
	qaChannelSendMessage->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));
	qaChannelFilter->setEnabled(true);
	qteChat->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));
}

void MainWindow::userStateChanged() {
	if (g.s.bStateInTray) {
		updateTrayIcon();
	}
	
	ClientUser *user = ClientUser::get(g.uiSession);
	if (user == NULL) {
		g.bAttenuateOthers = false;
		g.prioritySpeakerActiveOverride = false;
		
		return;
	}

	switch (user->tsState) {
		case Settings::Talking:
		case Settings::Whispering:
		case Settings::Shouting:
			g.bAttenuateOthers = g.s.bAttenuateOthersOnTalk;

			g.prioritySpeakerActiveOverride =
			        g.s.bAttenuateUsersOnPrioritySpeak
			        && user->bPrioritySpeaker;
			
			break;
		case Settings::Passive:
		default:
			g.bAttenuateOthers = false;
			g.prioritySpeakerActiveOverride = false;
			break;
	}
}

void MainWindow::on_qaAudioReset_triggered() {
	AudioInputPtr ai = g.ai;
	if (ai)
		ai->bResetProcessor = true;
}

void MainWindow::on_qaFilterToggle_triggered() {	
	g.s.bFilterActive = qaFilterToggle->isChecked();

	UserModel *um = static_cast<UserModel *>(qtvUsers->model());
	um->toggleChannelFiltered(NULL); // force a UI refresh
}

void MainWindow::on_qaAudioMute_triggered() {
	if (g.bInAudioWizard) {
		qaAudioMute->setChecked(!qaAudioMute->isChecked());
		return;
	}

	AudioInputPtr ai = g.ai;
	if (ai)
		ai->tIdle.restart();

	g.s.bMute = qaAudioMute->isChecked();

	if (! g.s.bMute && g.s.bDeaf) {
		g.s.bDeaf = false;
		qaAudioDeaf->setChecked(false);
		g.l->log(Log::SelfMute, tr("Unmuted and undeafened."));
	} else if (! g.s.bMute) {
		g.l->log(Log::SelfMute, tr("Unmuted."));
	} else {
		g.l->log(Log::SelfMute, tr("Muted."));
	}

	if (g.sh) {
		g.sh->setSelfMuteDeafState(g.s.bMute, g.s.bDeaf);
	}

	updateTrayIcon();
}

void MainWindow::on_qaAudioDeaf_triggered() {
	if (g.bInAudioWizard) {
		qaAudioDeaf->setChecked(!qaAudioDeaf->isChecked());
		return;
	}

	if (! qaAudioDeaf->isChecked() && bAutoUnmute) {
		qaAudioDeaf->setChecked(true);
		qaAudioMute->setChecked(false);
		on_qaAudioMute_triggered();
		return;
	}
	AudioInputPtr ai = g.ai;
	if (ai)
		ai->tIdle.restart();

	g.s.bDeaf = qaAudioDeaf->isChecked();
	if (g.s.bDeaf && ! g.s.bMute) {
		bAutoUnmute = true;
		g.s.bMute = true;
		qaAudioMute->setChecked(true);
		g.l->log(Log::SelfMute, tr("Muted and deafened."));
	} else if (g.s.bDeaf) {
		g.l->log(Log::SelfMute, tr("Deafened."));
		bAutoUnmute = false;
	} else {
		g.l->log(Log::SelfMute, tr("Undeafened."));
	}

	if (g.sh) {
		g.sh->setSelfMuteDeafState(g.s.bMute, g.s.bDeaf);
	}

	updateTrayIcon();
}

void MainWindow::on_qaRecording_triggered() {
	if (voiceRecorderDialog) {
		voiceRecorderDialog->show();
		voiceRecorderDialog->raise();
		voiceRecorderDialog->activateWindow();
	} else {
		voiceRecorderDialog = new VoiceRecorderDialog(this);
		connect(voiceRecorderDialog, SIGNAL(finished(int)), this, SLOT(voiceRecorderDialog_finished(int)));
		voiceRecorderDialog->show();
	}
}

void MainWindow::on_qaAudioTTS_triggered() {
	g.s.bTTS = qaAudioTTS->isChecked();
}

void MainWindow::on_qaAudioStats_triggered() {
	AudioStats *as=new AudioStats(this);
	as->show();
}

void MainWindow::on_qaAudioUnlink_triggered() {
	g.p->bUnlink = true;
}

void MainWindow::on_qaConfigDialog_triggered() {
	QDialog *dlg = NULL;
#ifdef USE_COCOA
	// To fit in with Mumble skins, we'll only use the Mac OS X
	// config dialog when we're using the Aqua skin with no external
	// stylesheet set.  Also, the Mac dialog doesn't work when embedded
	// inside the interactive overlay, so there we always force a regular
	// ConfigDialog.
	if (! g.ocIntercept && (g.qsCurrentStyle == QLatin1String("Macintosh (aqua)") || g.qsCurrentStyle.isEmpty())  && g.s.qsSkin.isEmpty())
		dlg = new ConfigDialogMac(this);
#endif
	if (! dlg)
		dlg = new ConfigDialog(this);

	if (dlg->exec() == QDialog::Accepted) {
		setupView(false);
		updateTrayIcon();

		UserModel *um = static_cast<UserModel *>(qtvUsers->model());
		um->toggleChannelFiltered(NULL); // force a UI refresh
	}

	delete dlg;
}

void MainWindow::on_qaConfigMinimal_triggered() {
	g.s.bMinimalView = qaConfigMinimal->isChecked();
	setupView();
}

void MainWindow::on_qaConfigHideFrame_triggered() {
	g.s.bHideFrame = qaConfigHideFrame->isChecked();
	setupView(false);
}

void MainWindow::on_qaConfigCert_triggered() {
	CertWizard *cw = new CertWizard(this);
	cw->exec();
	delete cw;
}

void MainWindow::on_qaAudioWizard_triggered() {
	AudioWizard *aw = new AudioWizard(this);
	aw->exec();
	delete aw;
}

void MainWindow::on_qaHelpWhatsThis_triggered() {
	QWhatsThis::enterWhatsThisMode();
}

void MainWindow::on_qaHelpAbout_triggered() {
	AboutDialog adAbout(this);
	adAbout.exec();
}

void MainWindow::on_qaHelpAboutQt_triggered() {
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_qaHelpVersionCheck_triggered() {
	new VersionCheck(false, this);
}

void MainWindow::on_gsMuteSelf_down(QVariant v) {
	int val = v.toInt();
	if (
	    ((val > 0) && ! g.s.bMute) ||
	    ((val < 0) && g.s.bMute) ||
	    (val == 0)
	) {
		qaAudioMute->setChecked(! qaAudioMute->isChecked());
		on_qaAudioMute_triggered();
	}
}

void MainWindow::on_gsDeafSelf_down(QVariant v) {
	int val = v.toInt();
	if (
	    ((val > 0) && ! g.s.bDeaf) ||
	    ((val < 0) && g.s.bDeaf) ||
	    (val == 0)
	) {
		qaAudioDeaf->setChecked(! qaAudioDeaf->isChecked());
		on_qaAudioDeaf_triggered();
	}
}

void MainWindow::on_PushToTalk_triggered(bool down, QVariant) {
	g.iPrevTarget = 0;
	if (down) {
		g.uiDoublePush = g.tDoublePush.restart();
		g.iPushToTalk++;
	} else if (g.iPushToTalk > 0) {
		QTimer::singleShot(g.s.uiPTTHold, this, SLOT(pttReleased()));
	}
}

void MainWindow::pttReleased() {
	if (g.iPushToTalk > 0) {
		g.iPushToTalk--;
	}
}

void MainWindow::on_PushToMute_triggered(bool down, QVariant) {
	g.bPushToMute = down;
}

void MainWindow::on_VolumeUp_triggered(bool down, QVariant) {
	if (down) {
		float v = floorf(g.s.fVolume * 10.0f);
		if (v < 20.0f)
			g.s.fVolume = ++v / 10.0f;
	}
}

void MainWindow::on_VolumeDown_triggered(bool down, QVariant) {
	if (down) {
		float v = ceilf(g.s.fVolume * 10.0f);
		if (v > 0.0f)
			g.s.fVolume = --v / 10.0f;
	}
}

Channel *MainWindow::mapChannel(int idx) const {
	if (! g.uiSession)
		return NULL;

	Channel *c = NULL;

	if (idx < 0) {
		switch (idx) {
			case SHORTCUT_TARGET_ROOT:
				c = Channel::get(0);
				break;
			case SHORTCUT_TARGET_PARENT:
			case SHORTCUT_TARGET_CURRENT:
				c = ClientUser::get(g.uiSession)->cChannel;
				if (idx == SHORTCUT_TARGET_PARENT)
					c = c->cParent;
				break;
			default:
				if(idx <= SHORTCUT_TARGET_PARENT_SUBCHANNEL)
					c = pmModel->getSubChannel(ClientUser::get(g.uiSession)->cChannel->cParent, SHORTCUT_TARGET_PARENT_SUBCHANNEL - idx);
				else
					c = pmModel->getSubChannel(ClientUser::get(g.uiSession)->cChannel, SHORTCUT_TARGET_SUBCHANNEL - idx);
				break;
		}
	} else {
		c = Channel::get(idx);
	}
	return c;
}

void MainWindow::updateTarget() {
	g.iPrevTarget = g.iTarget;

	if (qmCurrentTargets.isEmpty()) {
		g.bCenterPosition = false;
		g.iTarget = 0;
	} else {
		bool center = false;
		QList<ShortcutTarget> ql;
		foreach(const ShortcutTarget &st, qmCurrentTargets.keys()) {
			ShortcutTarget nt;
			center = center || st.bForceCenter;
			nt.bUsers = st.bUsers;
			if (st.bUsers) {
				foreach(const QString &hash, st.qlUsers) {
					ClientUser *p = pmModel->getUser(hash);
					if (p)
						nt.qlSessions.append(p->uiSession);
				}
				if (! nt.qlSessions.isEmpty())
					ql << nt;
			} else {
				Channel *c = mapChannel(st.iChannel);
				if (c) {
					nt.bLinks = st.bLinks;
					nt.bChildren = st.bChildren;
					nt.iChannel = c->iId;
					nt.qsGroup = st.qsGroup;
					ql << nt;
				}
			}
		}
		if (ql.isEmpty()) {
			g.iTarget = -1;
		} else {
			++iTargetCounter;

			int idx = qmTargets.value(ql);
			if (idx == 0) {
				QMap<int, int> qm;
				QMap<int, int>::const_iterator i;
				for (i=qmTargetUse.constBegin(); i != qmTargetUse.constEnd(); ++i) {
					qm.insert(i.value(), i.key());
				}

				i = qm.constBegin();
				idx = i.value();



				MumbleProto::VoiceTarget mpvt;
				mpvt.set_id(idx);

				foreach(const ShortcutTarget &st, ql) {
					MumbleProto::VoiceTarget_Target *t = mpvt.add_targets();
					if (st.bUsers) {
						foreach(unsigned int uisession, st.qlSessions)
							t->add_session(uisession);
					} else {
						t->set_channel_id(st.iChannel);
						if (st.bChildren)
							t->set_children(true);
						if (st.bLinks)
							t->set_links(true);
						if (! st.qsGroup.isEmpty())
							t->set_group(u8(st.qsGroup));
					}
				}
				g.sh->sendMessage(mpvt);

				qmTargets.insert(ql, idx);

				++i;
				++i;
				int oldidx = i.value();
				if (oldidx) {
					QHash<QList<ShortcutTarget>, int>::iterator mi;
					for (mi = qmTargets.begin(); mi != qmTargets.end(); ++mi) {
						if (mi.value() == oldidx) {
							qmTargets.erase(mi);

							mpvt.Clear();
							mpvt.set_id(oldidx);
							g.sh->sendMessage(mpvt);

							break;
						}
					}
				}
			}
			qmTargetUse.insert(idx, iTargetCounter);
			g.bCenterPosition = center;
			g.iTarget = idx;
		}
	}
}

void MainWindow::on_gsWhisper_triggered(bool down, QVariant scdata) {
	ShortcutTarget st = scdata.value<ShortcutTarget>();

	if (down) {
		if (gsJoinChannel->active()) {
			if (! st.bUsers) {
				Channel *c = mapChannel(st.iChannel);
				if (c) {
					g.sh->joinChannel(g.uiSession, c->iId);
				}
				return;
			}
		}

		if (gsLinkChannel->active()) {
			if (! st.bUsers) {
				Channel *c = ClientUser::get(g.uiSession)->cChannel;
				Channel *l = mapChannel(st.iChannel);
				if (l) {
					if (c->qsPermLinks.contains(l)) {
						g.sh->removeChannelLink(c->iId, l->iId);
					} else {
						g.sh->addChannelLink(c->iId, l->iId);
					}
				}
				return;
			}
		}

		addTarget(&st);
		updateTarget();

		g.iPushToTalk++;
	} else if (g.iPushToTalk > 0) {
		SignalCurry *fwd = new SignalCurry(scdata, true, this);
		connect(fwd, SIGNAL(called(QVariant)), SLOT(whisperReleased(QVariant)));
		QTimer::singleShot(g.s.uiPTTHold, fwd, SLOT(call()));
	}
}

/* Add and remove ShortcutTargets from the qmCurrentTargets Map, which counts
 * the number of push-to-talk events for a given ShortcutTarget.  If this number
 * reaches 0, the ShortcutTarget is removed from qmCurrentTargets.
 */
void MainWindow::addTarget(ShortcutTarget *st)
{
	if (qmCurrentTargets.contains(*st))
		qmCurrentTargets[*st] += 1;
	else
		qmCurrentTargets[*st] = 1;
}

void MainWindow::removeTarget(ShortcutTarget *st)
{
	if (!qmCurrentTargets.contains(*st))
		return;

	if (qmCurrentTargets[*st] == 1)
		qmCurrentTargets.remove(*st);
	else
		qmCurrentTargets[*st] -= 1;
}

void MainWindow::on_gsCycleTransmitMode_triggered(bool down, QVariant scdata) 
{
	if (down) 
	{
		QString qsNewMode;

		switch (g.s.atTransmit)
		{
			case Settings::Continous:
				g.s.atTransmit = Settings::VAD;
				qsNewMode = QString::fromLatin1("Voice Activity");
				break;
			case Settings::VAD:
				g.s.atTransmit = Settings::PushToTalk;
				qsNewMode = QString::fromLatin1("Push To Talk");
				break;
			case Settings::PushToTalk:
				g.s.atTransmit = Settings::Continous;
				qsNewMode = QString::fromLatin1("Continuous");
				break;
		}

		g.l->log(Log::Information, tr("Cycled Transmit Mode to %1").arg(qsNewMode));
	}
}

void MainWindow::whisperReleased(QVariant scdata) {
	if (g.iPushToTalk <= 0)
		return;

	ShortcutTarget st = scdata.value<ShortcutTarget>();

	g.iPushToTalk--;

	removeTarget(&st);
	updateTarget();
}

void MainWindow::onResetAudio()
{
	qWarning("MainWindow: Start audio reset");
	Audio::stop();
	Audio::start();
	qWarning("MainWindow: Audio reset complete");
}

void MainWindow::viewCertificate(bool) {
	ViewCert vc(g.sh->qscCert, this);
	vc.exec();
}

/**
 * This function prepares the UI for receiving server data. It gets called once the
 * connection to the server is established but before the server Sync is complete.
 */
void MainWindow::serverConnected() {
	g.uiSession = 0;
	g.pPermissions = ChanACL::None;
	g.iCodecAlpha = 0x8000000b;
	g.bPreferAlpha = true;
#ifdef USE_OPUS
	g.bOpus = true;
#else
	g.bOpus = false;
#endif
	g.iCodecBeta = 0;

	g.l->clearIgnore();
	g.l->setIgnore(Log::UserJoin);
	g.l->setIgnore(Log::OtherSelfMute);
	QString host, uname, pw;
	unsigned short port;
	g.sh->getConnectionInfo(host, port, uname, pw);
	g.l->log(Log::ServerConnected, tr("Connected."));
	qaServerDisconnect->setEnabled(true);
	qaServerInformation->setEnabled(true);
	qaServerBanList->setEnabled(true);

	Channel *root = Channel::get(0);
	pmModel->renameChannel(root, tr("Root"));
	pmModel->setCommentHash(root, QByteArray());
	root->uiPermissions = 0;

	qtvUsers->setRowHidden(0, QModelIndex(), false);

	g.bAllowHTML = true;
	g.uiMessageLength = 5000;
	g.uiImageLength = 131072;

	if (g.s.bMute || g.s.bDeaf) {
		g.sh->setSelfMuteDeafState(g.s.bMute, g.s.bDeaf);
	}

	// Update QActions and menues
	on_qmServer_aboutToShow();
	on_qmSelf_aboutToShow();
	qmChannel_aboutToShow();
	qmUser_aboutToShow();
	on_qmConfig_aboutToShow();

#ifdef Q_OS_WIN
	TaskList::addToRecentList(g.s.qsLastServer, uname, host, port);
#endif
}

void MainWindow::serverDisconnected(QAbstractSocket::SocketError err, QString reason) {
	g.uiSession = 0;
	g.pPermissions = ChanACL::None;
	g.bAttenuateOthers = false;
	qaServerDisconnect->setEnabled(false);
	qaServerInformation->setEnabled(false);
	qaServerBanList->setEnabled(false);
	qtvUsers->setCurrentIndex(QModelIndex());
	qteChat->setEnabled(false);
	updateTrayIcon();

	QString uname, pw, host;
	unsigned short port;
	g.sh->getConnectionInfo(host, port, uname, pw);
	if (Database::setShortcuts(g.sh->qbaDigest, g.s.qlShortcuts))
		GlobalShortcutEngine::engine->bNeedRemap = true;

	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = NULL;
	}

	if (banEdit) {
		banEdit->reject();
		delete banEdit;
		banEdit = NULL;
	}

	if (userEdit) {
		userEdit->reject();
		delete userEdit;
		userEdit = NULL;
	}

	if (tokenEdit) {
		tokenEdit->reject();
		delete tokenEdit;
		tokenEdit = NULL;
	}

	QSet<QAction *> qs;
	qs += qlServerActions.toSet();
	qs += qlChannelActions.toSet();
	qs += qlUserActions.toSet();

	foreach(QAction *a, qs)
		delete a;

	qlServerActions.clear();
	qlChannelActions.clear();
	qlUserActions.clear();

	pmModel->removeAll();
	qtvUsers->setRowHidden(0, QModelIndex(), true);

	// Update QActions and menues
	on_qmServer_aboutToShow();
	on_qmSelf_aboutToShow();
	qmChannel_aboutToShow();
	qmUser_aboutToShow();
	on_qmConfig_aboutToShow();

	if (! g.sh->qlErrors.isEmpty()) {
		foreach(QSslError e, g.sh->qlErrors)
			g.l->log(Log::Warning, tr("SSL Verification failed: %1").arg(Qt::escape(e.errorString())));
		if (! g.sh->qscCert.isEmpty()) {
			QSslCertificate c = g.sh->qscCert.at(0);
			QString basereason;
			if (! Database::getDigest(host, port).isNull()) {
				basereason = tr("<b>WARNING:</b> The server presented a certificate that was different from the stored one.");
			} else {
				basereason = tr("Sever presented a certificate which failed verification.");
			}
			QStringList qsl;
			foreach(QSslError e, g.sh->qlErrors)
				qsl << QString::fromLatin1("<li>%1</li>").arg(Qt::escape(e.errorString()));

			QMessageBox qmb(QMessageBox::Warning, QLatin1String("Mumble"),
			                tr("<p>%1.<br />The specific errors with this certificate are: </p><ol>%2</ol>"
			                   "<p>Do you wish to accept this certificate anyway?<br />(It will also be stored so you won't be asked this again.)</p>"
			                  ).arg(basereason).arg(qsl.join(QString())), QMessageBox::Yes | QMessageBox::No, this);

			qmb.setDefaultButton(QMessageBox::No);
			qmb.setEscapeButton(QMessageBox::No);

			QPushButton *qp = qmb.addButton(tr("&View Certificate"), QMessageBox::ActionRole);
			forever {
				int res = qmb.exec();

				if ((res == 0) && (qmb.clickedButton() == qp)) {
					ViewCert vc(g.sh->qscCert, this);
					vc.exec();
					continue;
				} else if (res == QMessageBox::Yes) {
					Database::setDigest(host, port, QString::fromLatin1(c.digest(QCryptographicHash::Sha1).toHex()));
					qaServerDisconnect->setEnabled(true);
					on_Reconnect_timeout();
				}
				break;
			}
		}
	} else if (err == QAbstractSocket::SslHandshakeFailedError) {
		QMessageBox::warning(this, tr("SSL Version mismatch"), tr("This server is using an older encryption standard, and is no longer supported by modern versions of Mumble."), QMessageBox::Ok);
	} else {
		bool ok = false;


		if (! reason.isEmpty()) {
			g.l->log(Log::ServerDisconnected, tr("Server connection failed: %1.").arg(Qt::escape(reason)));
		}  else {
			g.l->log(Log::ServerDisconnected, tr("Disconnected from server."));
		}

		Qt::WindowFlags wf = 0;
#ifdef Q_OS_MAC
		wf = Qt::Sheet;
#endif

		bool matched = true;
		switch (rtLast) {
			case MumbleProto::Reject_RejectType_InvalidUsername:
				uname = QInputDialog::getText(this, tr("Invalid username"),
				                              tr("You connected with an invalid username, please try another one."),
				                              QLineEdit::Normal, uname, &ok, wf);
				break;
			case MumbleProto::Reject_RejectType_UsernameInUse:
				uname = QInputDialog::getText(this, tr("Username in use"),
				                              tr("That username is already in use, please try another username."),
				                              QLineEdit::Normal, uname, &ok, wf);
				break;
			case MumbleProto::Reject_RejectType_WrongUserPW:
				pw = QInputDialog::getText(this,
				                           tr("Wrong certificate or password"),
				                           tr("Wrong certificate or password for registered user. If you are\n"
				                              "certain this user is protected by a password please retry.\n"
				                              "Otherwise abort and check your certificate and username."),
				                           QLineEdit::Password, pw, &ok, wf);
				break;
			case MumbleProto::Reject_RejectType_WrongServerPW:
				pw = QInputDialog::getText(this,
				                           tr("Wrong password"),
				                           tr("Wrong server password for unregistered user account, please try again."),
				                           QLineEdit::Password, pw, &ok, wf);
				break;
			default:
				matched = false;
				break;
		}
		if (ok && matched) {
			if (! g.s.bSuppressIdentity)
				Database::setPassword(host, port, uname, pw);
			qaServerDisconnect->setEnabled(true);
			g.sh->setConnectionInfo(host, port, uname, pw);
			on_Reconnect_timeout();
		} else if (!matched && g.s.bReconnect && ! reason.isEmpty()) {
			qaServerDisconnect->setEnabled(true);
			if (bRetryServer) {
				qtReconnect->start();
			}
		}
	}
	qstiIcon->setToolTip(tr("Mumble -- %1").arg(QLatin1String(MUMBLE_RELEASE)));
	AudioInput::setMaxBandwidth(-1);
}

void MainWindow::trayAboutToShow() {
	bool top = false;

	QPoint p = qstiIcon->geometry().center();
	if (p.isNull()) {
		p = QCursor::pos();
	}

	QDesktopWidget dw;

	QRect qr = dw.screenGeometry(p);

	if (p.y() < (qr.height() / 2))
		top = true;

	qmTray->clear();
	if (top) {
		qmTray->addAction(qaQuit);
		qmTray->addSeparator();
		qmTray->addAction(qaAudioDeaf);
		qmTray->addAction(qaAudioMute);
	} else {
		qmTray->addAction(qaAudioMute);
		qmTray->addAction(qaAudioDeaf);
		qmTray->addSeparator();
		qmTray->addAction(qaQuit);
	}
}

void MainWindow::on_Icon_messageClicked() {
	if (isMinimized())
		setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
	show();
	raise();
	activateWindow();
}

void MainWindow::on_Icon_activated(QSystemTrayIcon::ActivationReason reason) {
	// FIXME: Workaround for activated sending both doubleclick and trigger
	static Timer tDoubleClick;
	static bool bDoubleClick = false;

	if (reason == QSystemTrayIcon::DoubleClick) {
		bDoubleClick = true;
		tDoubleClick.restart();
	} else if (bDoubleClick && (reason == QSystemTrayIcon::Trigger)) {
		if (tDoubleClick.elapsed() > 100000UL)
			bDoubleClick = false;
		else
			return;
	}

	if (reason == QSystemTrayIcon::Trigger) {
#ifdef Q_OS_WIN
		if (!isVisible() || isMinimized() || tInactive.elapsed() > 300000UL) {
#else
		if (!isVisible() || isMinimized() || !isActiveWindow()) {
#endif
			if (isMaximized())
				showMaximized();
			else
				showNormal();
			activateWindow();
			raise();
		} else {
			if (g.s.bHideInTray)
				hide();
			else
				showMinimized();
		}
	}
}

/**
 * This function updates the qteChat bar default text according to
 * the selected user/channel in the users treeview.
 */
void MainWindow::qtvUserCurrentChanged(const QModelIndex &, const QModelIndex &) {
	updateChatBar();
}

void MainWindow::updateChatBar() {
	User *p = pmModel->getUser(qtvUsers->currentIndex());
	Channel *c = pmModel->getChannel(qtvUsers->currentIndex());

	if (g.uiSession == 0) {
		qteChat->setDefaultText(tr("<center>Not connected</center>"), true);
	} else if (!g.s.bChatBarUseSelection || p == NULL || p->uiSession == g.uiSession) {
		// Channel tree target
		if (!g.s.bChatBarUseSelection || c == NULL) // If no channel selected fallback to current one
			c = ClientUser::get(g.uiSession)->cChannel;

		qteChat->setDefaultText(tr("<center>Type message to channel '%1' here</center>").arg(Qt::escape(c->qsName)));
	} else {
		// User target
		qteChat->setDefaultText(tr("<center>Type message to user '%1' here</center>").arg(Qt::escape(p->qsName)));
	}

	updateMenuPermissions();
}

void MainWindow::customEvent(QEvent *evt) {
	if (evt->type() == MB_QEVENT) {
		MessageBoxEvent *mbe=static_cast<MessageBoxEvent *>(evt);
		g.l->log(Log::Information, mbe->msg);
		return;
	} else if (evt->type() == OU_QEVENT) {
		OpenURLEvent *oue=static_cast<OpenURLEvent *>(evt);
		openUrl(oue->url);
		return;
	} else if (evt->type() != SERVERSEND_EVENT) {
		return;
	}

	ServerHandlerMessageEvent *shme=static_cast<ServerHandlerMessageEvent *>(evt);

#ifdef QT_NO_DEBUG
#define MUMBLE_MH_MSG(x) case MessageHandler:: x : { \
		MumbleProto:: x msg; \
		if (msg.ParseFromArray(shme->qbaMsg.constData(), shme->qbaMsg.size())) \
			msg##x(msg); \
		break; \
	}
#else
#define MUMBLE_MH_MSG(x) case MessageHandler:: x : { \
		MumbleProto:: x msg; \
		if (msg.ParseFromArray(shme->qbaMsg.constData(), shme->qbaMsg.size())) { \
			printf("%s:\n", #x); \
			msg.PrintDebugString(); \
			msg##x(msg); \
		} \
		break; \
	}
#endif
	switch (shme->uiType) {
			MUMBLE_MH_ALL
	}


#undef MUMBLE_MH_MSG
}


void MainWindow::on_qteLog_anchorClicked(const QUrl &url) {
	if (!handleSpecialContextMenu(url, QCursor::pos(), true)) {
#ifdef Q_OS_MAC
		// Clicking a link can cause the user's default browser to pop up while
		// we're intercepting all events. This can be very confusing (because
		// the user can't click on anything before they dismiss the overlay
		// by hitting their toggle hotkey), so let's disallow clicking links
		// when embedded into the overlay for now.
		if (g.ocIntercept)
			return;
#endif
		if (url.scheme() != QLatin1String("file")
		        && url.scheme() != QLatin1String("qrc")
		        && !url.isRelative())
			QDesktopServices::openUrl(url);
	}
}

void MainWindow::on_qteLog_highlighted(const QUrl &url) {
	if (url.scheme() == QString::fromLatin1("clientid") || url.scheme() == QString::fromLatin1("channelid"))
		return;

	if (! url.isValid())
		QToolTip::hideText();
	else
		QToolTip::showText(QCursor::pos(), url.toString(), qteLog, QRect());
}

void MainWindow::on_qdwChat_dockLocationChanged(Qt::DockWidgetArea) {
	g.s.wlWindowLayout = Settings::LayoutCustom;
}

void MainWindow::on_qdwLog_dockLocationChanged(Qt::DockWidgetArea) {
	g.s.wlWindowLayout = Settings::LayoutCustom;
}

void MainWindow::on_qdwChat_visibilityChanged(bool) {
	g.s.wlWindowLayout = Settings::LayoutCustom;
}

void MainWindow::on_qdwLog_visibilityChanged(bool) {
	g.s.wlWindowLayout = Settings::LayoutCustom;
}

void MainWindow::context_triggered() {
	QAction *a = qobject_cast<QAction *>(sender());

	Channel *c = pmModel->getChannel(qtvUsers->currentIndex());
	ClientUser *p = pmModel->getUser(qtvUsers->currentIndex());

	MumbleProto::ContextAction mpca;
	mpca.set_action(u8(a->data().toString()));
	if (p && p->uiSession)
		mpca.set_session(p->uiSession);
	if (c)
		mpca.set_channel_id(c->iId);
	g.sh->sendMessage(mpca);
}

/**
 * Presents a file open dialog, opens the selected picture and returns it.
 * @return Pair consisting of the raw file contents and the image. Unitialized on error or cancel.
 */
QPair<QByteArray, QImage> MainWindow::openImageFile() {
	QPair<QByteArray, QImage> retval;

	if (g.s.qsImagePath.isEmpty() || ! QDir::root().exists(g.s.qsImagePath)) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
		g.s.qsImagePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
#else
		g.s.qsImagePath = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
#endif
	}

	QString fname = QFileDialog::getOpenFileName(this, tr("Choose image file"), g.s.qsImagePath, tr("Images (*.png *.jpg *.jpeg)"));

	if (fname.isNull())
		return retval;

	QFile f(fname);
	if (! f.open(QIODevice::ReadOnly)) {
		QMessageBox::warning(this, tr("Failed to load image"), tr("Could not open file for reading."));
		return retval;
	}

	QFileInfo fi(f);
	g.s.qsImagePath = fi.absolutePath();

	QByteArray qba = f.readAll();
	f.close();

	QBuffer qb(&qba);
	qb.open(QIODevice::ReadOnly);

	QImageReader qir;
	qir.setAutoDetectImageFormat(false);

	QByteArray fmt;
	if (!RichTextImage::isValidImage(qba, fmt)) {
		QMessageBox::warning(this, tr("Failed to load image"), tr("Image format not recognized."));
		return retval;
	}

	qir.setFormat(fmt);
	qir.setDevice(&qb);

	QImage img = qir.read();
	if (img.isNull()) {
		QMessageBox::warning(this, tr("Failed to load image"), tr("Image format not recognized."));
		return retval;
	}

	retval.first = qba;
	retval.second = img;

	return retval;
}

void MainWindow::destroyUserInformation() {
	UserInformation *ui = static_cast<UserInformation *>(sender());
	QMap<unsigned int, UserInformation *>::iterator i;
	for (i=qmUserInformations.begin(); i != qmUserInformations.end(); ++i) {
		if (i.value() == ui) {
			qmUserInformations.erase(i);
			return;
		}
	}
}

