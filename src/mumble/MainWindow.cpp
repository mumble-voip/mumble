/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "MainWindow.h"
#include "AudioWizard.h"
#include "Cert.h"
#include "AudioInput.h"
#include "ConnectDialog.h"
#include "User.h"
#include "Channel.h"
#include "ACLEditor.h"
#include "BanEditor.h"
#include "UserEdit.h"
#include "Tokens.h"
#include "Connection.h"
#include "ServerHandler.h"
#include "About.h"
#include "GlobalShortcut.h"
#include "VersionCheck.h"
#include "UserModel.h"
#include "AudioStats.h"
#include "Plugins.h"
#include "Log.h"
#include "Overlay.h"
#include "Global.h"
#include "Database.h"
#include "ViewCert.h"
#include "TextMessage.h"
#include "NetworkConfig.h"
#include "ACL.h"

/*!
  \fn void MainWindow::findDesiredChannel()
  This function tries to join a desired channel on connect. It gets called
  directly after server syncronization is completed.
  \see void MainWindow::msgServerSync(const MumbleProto::ServerSync &msg)
*/

/*!
  \fn void MainWindow::on_qleChat_tabPressed()
  Controlls tab username completion for the chatbar.
  \see ChatbarLineEdit::completeAtCursor()
*/

/*!
  \fn void MainWindow::on_qleChat_ctrlSpacePressed()
  Controlls ctrl space username completion and selection for the chatbar.
  \see ChatbarLineEdit::completeAtCursor()
*/

/*!
  \fn void MainWindow::qtvUserCurrentChanged(const QModelIndex &, const QModelIndex &)
  This function updates the qleChat bar default text according to
  the selected user/channel in the users treeview.
*/

/*!
  \fn void MainWindow::serverConnected()
  This function prepares the UI for receiving server data. It gets called once the
  connection to the server is established but before the server Sync is complete.
*/

/*!
  \fn MainWindow::updateMenuPermissions()
  This function updates the UI according to the permission of the user in the current channel.
  If possible the permissions are fetched from a cache. Otherwise they are requested by the server
  via a PermissionQuery call (whose reply updates the cache and calls this function again).
  \see MainWindow::msgPermissionQuery(const MumbleProto::PermissionQuery &msg)
*/

/*!
  \fn QPair<QByteArray, QImage> MainWindow::openImageFile()
  Presents a file open dialog, opens the selected picture and returns it.
  \return Pair consisting of the raw file contents and the image. Unitialized on error or cancel.
*/

MessageBoxEvent::MessageBoxEvent(QString m) : QEvent(static_cast<QEvent::Type>(MB_QEVENT)) {
	msg = m;
}

MainWindow::MainWindow(QWidget *p) : QMainWindow(p) {
	qiIconMuteSelf.addFile(QLatin1String("skin:muted_self.svg"));
	qiIconMuteServer.addFile(QLatin1String("skin:muted_server.svg"));
	qiIconMuteSuppressed.addFile(QLatin1String("skin:muted_suppressed.svg"));
	qiIconDeafSelf.addFile(QLatin1String("skin:deafened_self.svg"));
	qiIconDeafServer.addFile(QLatin1String("skin:deafened_server.svg"));

	qiIcon.addFile(QLatin1String("skin:mumble.svg"));
#ifdef Q_OS_MAC
	qiIcon.addFile(QLatin1String("skin:mumble.osx.png"));
#else
	// Set application icon except on MacOSX, where the window-icon
	// shown in the title-bar usually serves as a draggable version of the
	// current open document (i.e. you can copy the open document anywhere
	// simply by dragging this icon).
	qApp->setWindowIcon(qiIcon);
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
	bNoHide = false;

	qtReconnect = new QTimer(this);
	qtReconnect->setInterval(10000);
	qtReconnect->setSingleShot(true);
	qtReconnect->setObjectName(QLatin1String("Reconnect"));

	createActions();
	setupUi(this);
	setupGui();

	connect(g.sh, SIGNAL(connected()), this, SLOT(serverConnected()));
	connect(g.sh, SIGNAL(disconnected(QAbstractSocket::SocketError, QString)), this, SLOT(serverDisconnected(QAbstractSocket::SocketError, QString)));

	// Fix context of all actions.
	QList<QAction *> qla = findChildren<QAction *>();
	foreach(QAction *a, qla)
		a->setShortcutContext(Qt::ApplicationShortcut);

	on_qmServer_aboutToShow();
	on_qmChannel_aboutToShow();
	on_qmUser_aboutToShow();
	on_qmConfig_aboutToShow();

	setOnTop(g.s.bAlwaysOnTop);
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

	gsMetaChannel=new GlobalShortcut(this, idx++, tr("Join Channel", "Global Shortcut"));
	gsMetaChannel->setObjectName(QLatin1String("MetaChannel"));

	gsToggleOverlay=new GlobalShortcut(this, idx++, tr("Toggle Overlay", "Global Shortcut"), false);
	gsToggleOverlay->setObjectName(QLatin1String("ToggleOverlay"));
	gsToggleOverlay->qsToolTip = tr("Toggle state of in-game overlay.", "Global Shortcut");
	gsToggleOverlay->qsWhatsThis = tr("This will switch the states of the in-game overlay between showing everybody, just the users who are talking, and nobody.", "Global Shortcut");
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

	gsWhisper = new GlobalShortcut(this, idx++, tr("Whisper"), false, QVariant::fromValue(ShortcutTarget()));
	gsWhisper->setObjectName(QLatin1String("gsWhisper"));

#ifndef Q_OS_MAC
	qstiIcon->show();
#endif
}

void MainWindow::setupGui()  {
	setWindowTitle(tr("Mumble -- %1").arg(QLatin1String(MUMBLE_RELEASE)));
	setCentralWidget(qtvUsers);

#ifdef Q_OS_MAC
	QMenu *qmWindow = new QMenu(tr("&Window"), this);
	menubar->insertMenu(qmHelp->menuAction(), qmWindow);
	qmWindow->addAction(tr("Minimize"), this, SLOT(showMinimized()), QKeySequence(tr("Ctrl+M")));
#endif

	LogDocument *ld = new LogDocument(qteLog);
	qteLog->setDocument(ld);

	qteLog->document()->setDefaultStyleSheet(qApp->styleSheet());

	pmModel = new UserModel(qtvUsers);
	qtvUsers->setModel(pmModel);
	qtvUsers->setRowHidden(0, QModelIndex(), true);

	qaServerConnect->setShortcuts(QKeySequence::Open);
	qaServerDisconnect->setShortcuts(QKeySequence::Close);
	qaAudioMute->setChecked(g.s.bMute);
	qaAudioDeaf->setChecked(g.s.bDeaf);
	qaAudioTTS->setChecked(g.s.bTTS);
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
	qleChat->setDefaultText(tr("Not connected"));
	qleChat->setEnabled(false);

	if (g.s.bMinimalView && ! g.s.qbaMinimalViewGeometry.isNull())
		restoreGeometry(g.s.qbaMinimalViewGeometry);
	else if (! g.s.bMinimalView && ! g.s.qbaMainWindowGeometry.isNull())
		restoreGeometry(g.s.qbaMainWindowGeometry);

	restoreState(g.s.qbaMainWindowState);
	qtvUsers->header()->restoreState(g.s.qbaHeaderState);

	connect(qtvUsers->selectionModel(),
	        SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
	        SLOT(qtvUserCurrentChanged(const QModelIndex &, const QModelIndex &)));

	setupView(false);

	qmTray = new QMenu(this);
	setupIconMenu(false);
	qstiIcon->setContextMenu(qmTray);

	updateTrayIcon();
}

MainWindow::~MainWindow() {
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
	return false;
}
#endif

void MainWindow::closeEvent(QCloseEvent *e) {
#ifndef Q_OS_MAC
	if (g.sh && g.sh->isRunning() && g.s.bAskOnQuit && !bSuppressAskOnQuit) {
		QMessageBox mb(QMessageBox::Warning, tr("Mumble"), tr("Mumble is currently connected to a server. Do you want to Close or Minimize it?"), QMessageBox::NoButton, this);
		QPushButton *qpbClose = mb.addButton(tr("Close"), QMessageBox::YesRole);
		QPushButton *qpbMinimize = mb.addButton(tr("Minimize"), QMessageBox::NoRole);
		mb.setDefaultButton(qpbMinimize);
		mb.setEscapeButton(qpbMinimize);
		mb.exec();
		if (mb.clickedButton() != qpbClose) {
			e->accept();
			return;
		}
	}
#endif
	g.uiSession = 0;
	g.pPermissions = ChanACL::None;

	g.s.qbaMainWindowState = saveState();
	if (g.s.bMinimalView) {
		g.s.qbaMinimalViewGeometry = saveGeometry();
	} else {
		g.s.qbaMainWindowGeometry = saveGeometry();
		g.s.qbaHeaderState = qtvUsers->header()->saveState();
	}
	QMainWindow::closeEvent(e);
	qApp->quit();
}

void MainWindow::hideEvent(QHideEvent *e) {
#ifndef Q_OS_MAC
#ifdef Q_OS_UNIX
	if (! qApp->activeModalWidget() && ! qApp->activePopupWidget())
#endif
		if (g.s.bHideTray && !bNoHide && qstiIcon->isSystemTrayAvailable())
			qApp->postEvent(this, new QEvent(static_cast<QEvent::Type>(TI_QEVENT)));
	QMainWindow::hideEvent(e);
#endif
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
	} else {
		qstiIcon->setIcon(qiIcon);
	}
}

void MainWindow::on_qteLog_customContextMenuRequested(const QPoint &mpos) {
#if QT_VERSION >= 0x040400
	QMenu *menu = qteLog->createStandardContextMenu(mpos);
#else
	QMenu *menu = qteLog->createStandardContextMenu();
#endif
	menu->addSeparator();
	menu->addAction(tr("Clear"), qteLog, SLOT(clear(void)));
	menu->exec(qteLog->mapToGlobal(mpos));
	delete menu;
}

void MainWindow::openUrl(const QUrl &url) {
	g.l->log(Log::Information, tr("Opening URL %1").arg(url.toString()));
	if (url.scheme() == QLatin1String("file")) {
		QFile f(url.toLocalFile());
		if (! f.exists() || ! f.open(QIODevice::ReadOnly)) {
			g.l->log(Log::Warning, tr("File does not exist"));
			return;
		}
		f.close();

		QSettings *qs = new QSettings(f.fileName(), QSettings::IniFormat);
#if QT_VERSION >= 0x040500
		qs->setIniCodec("UTF-8");
#endif
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
	major = 1;
	minor = 1;
	patch = 0;

	QString version = url.queryItemValue(QLatin1String("version"));

	QRegExp rx(QLatin1String("(\\d+)\\.(\\d+)\\.(\\d+)"));
	if (rx.exactMatch(version)) {
		major = rx.cap(1).toInt();
		minor = rx.cap(2).toInt();
		patch = rx.cap(3).toInt();
	}

	if ((major != 1) || (minor != 2) || (patch > 0)) {
		g.l->log(Log::Warning, tr("This version of Mumble can't handle URLs for Mumble version %1.%2.%3").arg(major).arg(minor).arg(patch));
		return;
	}

	QString host = url.host();
	unsigned short port = static_cast<unsigned short>(url.port(64738));
	QString user = url.userName();
	QString pw = url.password();
	qsDesiredChannel = url.path();

	if (g.sh && g.sh->isRunning()) {
		QString oHost, oUser, oPw;
		unsigned short oport;
		g.sh->getConnectionInfo(oHost, oport, oUser, oPw);

		if ((user.isEmpty() || (user == oUser)) &&
		        (host.isEmpty() || ((host == oHost) && (port == oport)))) {
			findDesiredChannel();
			return;
		}
	}

	Database::fuzzyMatch(user, pw, host, port);

	if (user.isEmpty()) {
		bool ok;
		user = QInputDialog::getText(this, tr("Connecting to %1").arg(url.toString()), tr("Enter username"), QLineEdit::Normal, g.s.qsUsername, &ok);
		if (! ok || user.isEmpty())
			return;
		g.s.qsUsername = user;
	}

	if (g.sh && g.sh->isRunning()) {
		on_qaServerDisconnect_triggered();
		g.sh->wait();
	}

	rtLast = MumbleProto::Reject_RejectType_None;
	qaServerDisconnect->setEnabled(true);
	g.l->log(Log::ServerConnected, tr("Connecting to server %1.").arg(host));
	g.sh->setConnectionInfo(host, port, user, pw);
	g.sh->start(QThread::TimeCriticalPriority);
}

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
			MumbleProto::UserState mpus;
			mpus.set_session(g.uiSession);
			mpus.set_channel_id(chan->iId);
			g.sh->sendMessage(mpus);
		}
		qtvUsers->setCurrentIndex(pmModel->index(chan));
	} else {
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
		bNoHide = true;
		setWindowFlags(wf);
		show();
		bNoHide = false;
	}
}

void MainWindow::setupView(bool toggle_minimize) {
	bool showit = ! g.s.bMinimalView;

	bNoHide = true;

	// Update window layout
	int iTmp = g.s.iWindowLayout;
	switch (iTmp) {
		case Settings::LayoutClassic:
			removeDockWidget(qdwLog);
			addDockWidget(Qt::LeftDockWidgetArea, qdwLog);
			qdwLog->show();
			splitDockWidget(qdwLog, qdwChat, Qt::Vertical);
			break;
		case Settings::LayoutStacked:
			removeDockWidget(qdwLog);
			addDockWidget(Qt::BottomDockWidgetArea, qdwLog);
			qdwLog->show();
			splitDockWidget(qdwLog, qdwChat, Qt::Vertical);
			break;
		case Settings::LayoutHybrid:
			removeDockWidget(qdwLog);
			addDockWidget(Qt::LeftDockWidgetArea, qdwLog);
			qdwLog->show();
			removeDockWidget(qdwChat);
			addDockWidget(Qt::BottomDockWidgetArea, qdwChat);
			if (g.s.bShowChatbar) qdwChat->show();
			break;
		default:
			iTmp = Settings::LayoutCustom;
			break;
	}
	g.s.iWindowLayout = iTmp;

	QRect geom = frameGeometry();

	if (toggle_minimize) {
		if (! showit) {
			g.s.qbaMainWindowGeometry = saveGeometry();
			g.s.qbaHeaderState = qtvUsers->header()->saveState();
		} else {
			g.s.qbaMinimalViewGeometry = saveGeometry();
		}
	}

	Qt::WindowFlags f = Qt::Window;
	if (!showit && g.s.bHideFrame)
		f = Qt::Window | Qt::FramelessWindowHint;
#ifndef Q_OS_MAC
	else if (!showit)
		f = Qt::Tool;
#endif

	if (g.s.bAlwaysOnTop)
		f |= Qt::WindowStaysOnTopHint;

	setWindowFlags(f);

	qdwLog->setVisible(showit);
	qdwChat->setVisible(showit && g.s.bShowChatbar);
	qtvUsers->header()->setVisible(showit);
	menuBar()->setVisible(showit);

	if (toggle_minimize) {
		if (! showit && ! g.s.qbaMinimalViewGeometry.isNull())
			restoreGeometry(g.s.qbaMinimalViewGeometry);
		else if (showit && ! g.s.qbaMainWindowGeometry.isNull())
			restoreGeometry(g.s.qbaMainWindowGeometry);
	} else {
		QRect newgeom = frameGeometry();
		resize(geometry().width()-newgeom.width()+geom.width(),
		       geometry().height()-newgeom.height()+geom.height());
		move(geom.x(), geom.y());
	}

	show();
	activateWindow();
	bNoHide = false;

}

void MainWindow::on_qaServerConnect_triggered() {
	ConnectDialog *cd = new ConnectDialog(this);
	int res = cd->exec();

	if (cd->qsServer.isEmpty() || (cd->usPort==0) || cd->qsUsername.isEmpty())
		res = QDialog::Rejected;

	if (g.sh && g.sh->isRunning() && res == QDialog::Accepted) {
		on_qaServerDisconnect_triggered();
		g.sh->wait();
	}

	if (res == QDialog::Accepted) {
		qsDesiredChannel = QString();
		rtLast = MumbleProto::Reject_RejectType_None;
		qaServerDisconnect->setEnabled(true);
		g.l->log(Log::ServerConnected, tr("Connecting to server %1.").arg(cd->qsServer));
		g.sh->setConnectionInfo(cd->qsServer, cd->usPort, cd->qsUsername, cd->qsPassword);
		g.sh->start(QThread::TimeCriticalPriority);
	}
	delete cd;
}

void MainWindow::on_Reconnect_timeout() {
	if (g.sh->isRunning())
		return;
	g.l->log(Log::ServerDisconnected, tr("Reconnecting."));
	g.sh->start(QThread::TimeCriticalPriority);
}

void MainWindow::on_qmServer_aboutToShow() {
	qmServer->clear();
	qmServer->addAction(qaServerConnect);
	qmServer->addAction(qaServerDisconnect);
	qmServer->addAction(qaServerBanList);
	qmServer->addAction(qaServerUserList);
	qmServer->addAction(qaServerInformation);
	qmServer->addAction(qaServerTexture);
	qmServer->addAction(qaServerTextureRemove);
	qmServer->addAction(qaServerTokens);
	qmServer->addSeparator();
	qmServer->addAction(qaQuit);

	qaServerBanList->setEnabled(g.pPermissions & (ChanACL::Ban | ChanACL::Write));
	qaServerUserList->setEnabled(g.pPermissions & (ChanACL::Register | ChanACL::Write));
	qaServerInformation->setEnabled(g.uiSession != 0);
	qaServerTexture->setEnabled(g.uiSession != 0);
	qaServerTokens->setEnabled(g.uiSession != 0);

	ClientUser *user = ClientUser::get(g.uiSession);
	qaServerTextureRemove->setEnabled(user && ! user->qbaTexture.isEmpty());

	if (! qlServerActions.isEmpty()) {
		qmServer->addSeparator();
		foreach(QAction *a, qlServerActions)
			qmServer->addAction(a);
	}
}

void MainWindow::on_qaServerDisconnect_triggered() {
	if (qtReconnect->isActive())
		qtReconnect->stop();
	if (g.sh && g.sh->isRunning())
		g.sh->disconnect();
}

void MainWindow::on_qaServerBanList_triggered() {
	MumbleProto::BanList mpbl;
	mpbl.set_query(true);
	g.sh->sendMessage(mpbl);

	if (banEdit) {
		banEdit->reject();
		delete banEdit;
		banEdit = NULL;
	}
}

void MainWindow::on_qaServerUserList_triggered() {
	MumbleProto::UserList mpul;
	g.sh->sendMessage(mpul);

	if (userEdit) {
		userEdit->reject();
		delete userEdit;
		userEdit = NULL;
	}
}

void MainWindow::on_qaServerInformation_triggered() {
	ConnectionPtr c = g.sh->cConnection;

	if (! c)
		return;

	CryptState &cs = c->csCrypt;

	QSslCipher qsc = g.sh->qscCipher;

	unsigned int version = g.sh->uiVersion;

	QString qsVersion=tr("<h2>Version</h2><p>Protocol %1.%2.%3.</p>").arg(QString::number((version >> 16) & 0xFF),
	                  QString::number((version >> 8) & 0xFF),
	                  QString::number(version & 0xFF));

	if (g.sh->qsRelease.isEmpty() || g.sh->qsOS.isEmpty() || g.sh->qsOSVersion.isEmpty()) {
		qsVersion.append(tr("<p>No build information or OS version available.</p>"));
	} else {
		qsVersion.append(tr("<p>%1 (%2)<br />%3</p>")
		                 .arg(g.sh->qsRelease, g.sh->qsOS, g.sh->qsOSVersion));
	}

	QString qsControl=tr("<h2>Control channel</h2><p>Encrypted with %1 bit %2<br />%3 ms average latency (%4 deviation)</p>").arg(QString::number(qsc.usedBits()),
	                  qsc.name(),
	                  QString::fromLatin1("%1").arg(boost::accumulators::mean(g.sh->accTCP), 0, 'f', 2),
	                  QString::fromLatin1("%1").arg(sqrt(boost::accumulators::variance(g.sh->accTCP)),0,'f',2));
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
	qsAudio=tr("<h2>Audio bandwidth</h2><p>Maximum %1 kbit/s<br />Current %2 kbit/s</p>").arg(g.iMaxBandwidth / 1000.0,0,'f',1).arg(g.iAudioBandwidth / 1000.0,0,'f',1);

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

	QImage &img = choice.second;
	img = img.convertToFormat(QImage::Format_ARGB32);
	if ((img.height() != 60) || (img.width() != 600)) {
		QImage final(600, 60, QImage::Format_ARGB32);
		img = img.scaled(600, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		qWarning() << img.width() << img.height();
		final.fill(0);
		{
			QPainter p(&final);
			p.drawImage(0, 0, img);
		}
		img = final;
	}
	QByteArray qba(reinterpret_cast<const char *>(img.bits()), img.numBytes());
	qba = qCompress(qba);
	MumbleProto::UserState mpus;
	mpus.set_texture(qba.constData(), qba.length());
	g.sh->sendMessage(mpus);
}

void MainWindow::on_qaServerTextureRemove_triggered() {
	MumbleProto::UserState mpus;
	mpus.set_texture(NULL, 0);
	g.sh->sendMessage(mpus);
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

void MainWindow::on_qmUser_aboutToShow() {
	ClientUser *p = pmModel->getUser(qtvUsers->currentIndex());
	bool self = p && (p->uiSession == g.uiSession);

	qmUser->clear();

	if (g.pPermissions & (ChanACL::Kick | ChanACL::Ban | ChanACL::Write))
		qmUser->addAction(qaUserKick);
	if (g.pPermissions & (ChanACL::Ban | ChanACL::Write))
		qmUser->addAction(qaUserBan);
	qmUser->addAction(qaUserMute);
	qmUser->addAction(qaUserDeaf);
	qmUser->addAction(qaUserLocalMute);
	qmUser->addAction(qaUserComment);
	qmUser->addAction(qaUserTextMessage);

	if (p && (p->iId < 0) && (g.pPermissions & ((self ? ChanACL::SelfRegister : ChanACL::Register) | ChanACL::Write))) {
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

	if (g.s.bMinimalView) {
		qmUser->addSeparator();
		qmUser->addMenu(qmServer);
		qmUser->addMenu(qmAudio);
		qmUser->addMenu(qmConfig);
		qmUser->addMenu(qmHelp);
	}

	if (! qlUserActions.isEmpty()) {
		qmUser->addSeparator();
		foreach(QAction *a, qlUserActions)
			qmUser->addAction(a);
	}

	if (! p) {
		qaUserKick->setEnabled(false);
		qaUserBan->setEnabled(false);
		qaUserMute->setEnabled(false);
		qaUserLocalMute->setEnabled(false);
		qaUserDeaf->setEnabled(false);
		qaUserTextMessage->setEnabled(false);
	} else {
		qaUserKick->setEnabled(! self);
		qaUserBan->setEnabled(! self);
		qaUserMute->setEnabled(! self || p->bMute || p->bSuppress);
		qaUserDeaf->setEnabled(! self || p->bDeaf);
		qaUserTextMessage->setEnabled(true);
		qaUserLocalMute->setEnabled(! self);
		qaUserComment->setEnabled(self || (g.pPermissions & (ChanACL::Move | ChanACL::Write)));

		qaUserMute->setChecked(p->bMute || p->bSuppress);
		qaUserDeaf->setChecked(p->bDeaf);
		qaUserLocalMute->setChecked(p->bLocalMute);
	}
	updateMenuPermissions();
}

void MainWindow::on_qaUserMute_triggered() {
	User *p = pmModel->getUser(qtvUsers->currentIndex());
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
	ClientUser *p = pmModel->getUser(qtvUsers->currentIndex());
	if (!p)
		return;

	bool muted = qaUserLocalMute->isChecked();

	p->setLocalMute(muted);
	if (! p->qsHash.isEmpty())
		Database::setLocalMuted(p->qsHash, muted);
}

void MainWindow::on_qaUserDeaf_triggered() {
	User *p = pmModel->getUser(qtvUsers->currentIndex());
	if (!p)
		return;

	MumbleProto::UserState mpus;
	mpus.set_session(p->uiSession);
	mpus.set_deaf(! p->bDeaf);
	g.sh->sendMessage(mpus);
}

void MainWindow::on_qaUserRegister_triggered() {
	User *p = pmModel->getUser(qtvUsers->currentIndex());
	if (!p)
		return;

	unsigned int session = p->uiSession;

	QMessageBox::StandardButton result;

	if (session == g.uiSession)
		result = QMessageBox::question(this, tr("Register yourself as %1").arg(p->qsName), tr("<p>You are about to register yourself on this server. This action cannot be undone, and your username cannot be changed once this is done. You will forever be known as '%1' on this server.</p><p>Are you sure you want to register yourself?</p>").arg(p->qsName), QMessageBox::Yes|QMessageBox::No);
	else
		result = QMessageBox::question(this, tr("Register user %1").arg(p->qsName), tr("<p>You are about to register %1 on the server. This action cannot be undone, the username cannot be changed, and as a registered user, %1 will have access to the server even if you change the server password.</p><p>From this point on, %1 will be authenticated with the certificate currently in use.</p><p>Are you sure you want to register %1?</p>").arg(p->qsName), QMessageBox::Yes|QMessageBox::No);

	if (result == QMessageBox::Yes) {
		p = ClientUser::get(session);
		if (! p)
			return;
		MumbleProto::UserState mpus;
		mpus.set_session(p->uiSession);
		mpus.set_user_id(0);
		g.sh->sendMessage(mpus);
	}
}

void MainWindow::on_qaUserFriendAdd_triggered() {
	ClientUser *p = pmModel->getUser(qtvUsers->currentIndex());
	if (!p)
		return;

	Database::addFriend(p->qsName, p->qsHash);
	pmModel->setFriendName(p, p->qsName);
}

void MainWindow::on_qaUserFriendUpdate_triggered() {
	on_qaUserFriendAdd_triggered();
}

void MainWindow::on_qaUserFriendRemove_triggered() {
	ClientUser *p = pmModel->getUser(qtvUsers->currentIndex());
	if (!p)
		return;

	Database::removeFriend(p->qsHash);
	pmModel->setFriendName(p, QString());
}

void MainWindow::on_qaUserKick_triggered() {
	User *p = pmModel->getUser(qtvUsers->currentIndex());
	if (!p)
		return;

	unsigned int session = p->uiSession;

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Kicking user %1").arg(p->qsName), tr("Enter reason"), QLineEdit::Normal, QString(), &ok);

	p = ClientUser::get(session);
	if (!p)
		return;

	if (ok) {
		MumbleProto::UserRemove mpur;
		mpur.set_session(p->uiSession);
		mpur.set_reason(u8(reason));
		g.sh->sendMessage(mpur);
	}
}

void MainWindow::on_qaUserBan_triggered() {
	User *p = pmModel->getUser(qtvUsers->currentIndex());
	if (!p)
		return;

	unsigned int session = p->uiSession;

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Banning user %1").arg(p->qsName), tr("Enter reason"), QLineEdit::Normal, QString(), &ok);
	p = ClientUser::get(session);
	if (!p)
		return;

	if (ok) {
		MumbleProto::UserRemove mpur;
		mpur.set_session(p->uiSession);
		mpur.set_reason(u8(reason));
		mpur.set_ban(true);
		g.sh->sendMessage(mpur);
	}
}

void MainWindow::on_qaUserTextMessage_triggered() {
	User *p = pmModel->getUser(qtvUsers->currentIndex());

	if (!p)
		return;

	unsigned int session = p->uiSession;

	::TextMessage *texm = new ::TextMessage(this, tr("Sending message to %1").arg(p->qsName));
	int res = texm->exec();

	p = ClientUser::get(session);

	if (p && (res==QDialog::Accepted)) {
		QString msg = texm->message();

		if (! msg.isEmpty()) {
			MumbleProto::TextMessage mptm;
			mptm.add_session(p->uiSession);
			mptm.set_message(u8(msg));
			g.sh->sendMessage(mptm);
			g.l->log(Log::TextMessage, tr("To %1: %2").arg(p->qsName).arg(texm->message()), tr("Message to %1").arg(p->qsName));
		}
	}
	delete texm;
}

void MainWindow::on_qaUserComment_triggered() {
	User *p = pmModel->getUser(qtvUsers->currentIndex());

	if (!p)
		return;

	unsigned int session = p->uiSession;

	::TextMessage *texm = new ::TextMessage(this, tr("Change comment on user %1").arg(p->qsName));

	texm->rteMessage->setText(p->qsComment);
	int res = texm->exec();

	p = ClientUser::get(session);

	if (p && (res==QDialog::Accepted)) {
		MumbleProto::UserState mpus;
		mpus.set_session(session);
		mpus.set_comment(u8(texm->message()));
		g.sh->sendMessage(mpus);
	}
	delete texm;
}

void MainWindow::on_qaQuit_triggered() {
	bSuppressAskOnQuit = true;
	this->close();
}

void MainWindow::on_qleChat_returnPressed() {
	if (qleChat->text().isEmpty() || g.uiSession == 0) return; // Check if text & connection is available

	User *p = pmModel->getUser(qtvUsers->currentIndex());
	Channel *c = pmModel->getChannel(qtvUsers->currentIndex());

	MumbleProto::TextMessage mptm;
	QString qsText;

	qsText = qleChat->text();
	if (! Qt::mightBeRichText(qsText)) {
		qsText = TextMessage::autoFormat(qsText);
	}

	mptm.set_message(u8(qsText));

	if (p == NULL || p->uiSession == g.uiSession) {
		// Channel tree message
		if (c == NULL) // If no channel selected fallback to current one
			c = ClientUser::get(g.uiSession)->cChannel;

		mptm.add_channel_id(c->iId);
		g.l->log(Log::TextMessage, tr("To channel %1: %2").arg(c->qsName).arg(qsText), tr("Message to channel %1").arg(c->qsName));
	} else {
		// User message
		mptm.add_session(p->uiSession);
		g.l->log(Log::TextMessage, tr("To %1: %2").arg(p->qsName).arg(qsText), tr("Message to %1").arg(p->qsName));
	}

	g.sh->sendMessage(mptm);
	qleChat->clear();
}

void MainWindow::on_qleChat_tabPressed() {
	qleChat->completeAtCursor();
}

void MainWindow::on_qleChat_ctrlSpacePressed() {
	unsigned int res = qleChat->completeAtCursor();
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
	qmConfig->addAction(qaConfigMinimal);
	if (g.s.bMinimalView)
		qmConfig->addAction(qaConfigHideFrame);
}

void MainWindow::on_qmChannel_aboutToShow() {
	QModelIndex idx = qtvUsers->currentIndex();

	qmChannel->clear();
	qmChannel->addAction(qaChannelAdd);
	qmChannel->addAction(qaChannelACL);
	qmChannel->addAction(qaChannelRemove);
	qmChannel->addSeparator();
	qmChannel->addAction(qaChannelLink);
	qmChannel->addAction(qaChannelUnlink);
	qmChannel->addAction(qaChannelUnlinkAll);
	qmChannel->addSeparator();
	qmChannel->addAction(qaChannelSendMessage);

	if (g.s.bMinimalView) {
		qmChannel->addSeparator();
		qmChannel->addMenu(qmServer);
		qmChannel->addMenu(qmAudio);
		qmChannel->addMenu(qmConfig);
		qmChannel->addMenu(qmHelp);
	}

	if (! qlChannelActions.isEmpty()) {
		qmChannel->addSeparator();
		foreach(QAction *a, qlChannelActions)
			qmChannel->addAction(a);
	}

	bool add, addtemp, remove, acl, rename, descUpdate, link, unlink, unlinkall, msg;
	add = addtemp = remove = acl = rename = descUpdate = link = unlink = unlinkall = msg = false;

	if (g.uiSession != 0) {
		add = true;
		addtemp = true;
		acl = true;
		msg = true;
		descUpdate = true;

		Channel *c = pmModel->getChannel(idx);
		Channel *home = ClientUser::get(g.uiSession)->cChannel;

		if (c && c->bTemporary)
			addtemp = false;

		if (c && c->iId != 0) {
			rename = true;
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

	qaChannelAdd->setEnabled(add);
	qaChannelRemove->setEnabled(remove);
	qaChannelACL->setEnabled(acl);
	qaChannelLink->setEnabled(link);
	qaChannelUnlink->setEnabled(unlink);
	qaChannelUnlinkAll->setEnabled(unlinkall);
	qaChannelSendMessage->setEnabled(msg);
	updateMenuPermissions();
}

void MainWindow::on_qaChannelAdd_triggered() {
	Channel *c = pmModel->getChannel(qtvUsers->currentIndex());
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
	Channel *c = pmModel->getChannel(qtvUsers->currentIndex());
	if (! c)
		return;

	int id = c->iId;

	ret=QMessageBox::question(this, tr("Mumble"), tr("Are you sure you want to delete %1 and all its sub-channels?").arg(c->qsName), QMessageBox::Yes, QMessageBox::No);

	c = Channel::get(id);
	if (!c)
		return;

	if (ret == QMessageBox::Yes) {
		MumbleProto::ChannelRemove mpcr;
		mpcr.set_channel_id(c->iId);
		g.sh->sendMessage(mpcr);
	}
}

void MainWindow::on_qaChannelACL_triggered() {
	Channel *c = pmModel->getChannel(qtvUsers->currentIndex());
	int id = c ? c->iId : 0;

	MumbleProto::ACL mpacl;
	mpacl.set_channel_id(id);
	mpacl.set_query(true);

	g.sh->sendMessage(mpacl);

	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = NULL;
	}
}

void MainWindow::on_qaChannelLink_triggered() {
	Channel *c = ClientUser::get(g.uiSession)->cChannel;
	Channel *l = pmModel->getChannel(qtvUsers->currentIndex());
	if (! l)
		l = Channel::get(0);

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(c->iId);
	mpcs.add_links_add(l->iId);
	g.sh->sendMessage(mpcs);
}

void MainWindow::on_qaChannelUnlink_triggered() {
	Channel *c = ClientUser::get(g.uiSession)->cChannel;
	Channel *l = pmModel->getChannel(qtvUsers->currentIndex());
	if (! l)
		l = Channel::get(0);

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(c->iId);
	mpcs.add_links_remove(l->iId);
	g.sh->sendMessage(mpcs);
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
	Channel *c = pmModel->getChannel(qtvUsers->currentIndex());

	if (!c)
		return;

	int id = c->iId;

	::TextMessage *texm = new ::TextMessage(this, tr("Sending message to channel %1").arg(c->qsName), true);
	int res = texm->exec();

	c = Channel::get(id);

	if (c && (res==QDialog::Accepted)) {
		MumbleProto::TextMessage mptm;
		if (texm->bTreeMessage)
			mptm.add_tree_id(id);
		else
			mptm.add_channel_id(id);
		mptm.set_message(u8(texm->message()));
		g.sh->sendMessage(mptm);

		if (texm->bTreeMessage)
			g.l->log(Log::TextMessage, tr("To tree %1: %2").arg(c->qsName).arg(texm->message()), tr("Message to tree %1").arg(c->qsName));
		else
			g.l->log(Log::TextMessage, tr("To channel %1: %2").arg(c->qsName).arg(texm->message()), tr("Message to channel %1").arg(c->qsName));
	}
	delete texm;
}

void MainWindow::updateMenuPermissions() {
	Channel *c = g.uiSession ? pmModel->getChannel(qtvUsers->currentIndex()) : NULL;
	ChanACL::Permissions p = static_cast<ChanACL::Permissions>(c ? c->uiPermissions : ChanACL::None);

	if (c && ! p) {
		MumbleProto::PermissionQuery mppq;
		mppq.set_channel_id(c->iId);
		g.sh->sendMessage(mppq);
		if (c->iId == 0)
			p = g.pPermissions;
		else
			p = ChanACL::All;

		c->uiPermissions = p;
	}

	Channel *cparent = c ? c->cParent : NULL;
	ChanACL::Permissions pparent = static_cast<ChanACL::Permissions>(cparent ? cparent->uiPermissions : ChanACL::None);

	if (cparent && ! pparent) {
		MumbleProto::PermissionQuery mppq;
		mppq.set_channel_id(cparent->iId);
		g.sh->sendMessage(mppq);
		if (cparent->iId == 0)
			pparent = g.pPermissions;
		else
			pparent = ChanACL::All;

		cparent->uiPermissions = pparent;
	}

	ClientUser *user = g.uiSession ? ClientUser::get(g.uiSession) : NULL;
	Channel *homec = user ? user->cChannel : NULL;
	ChanACL::Permissions homep = static_cast<ChanACL::Permissions>(homec ? homec->uiPermissions : ChanACL::None);

	if (homec && ! homep) {
		MumbleProto::PermissionQuery mppq;
		mppq.set_channel_id(homec->iId);
		g.sh->sendMessage(mppq);
		if (homec->iId == 0)
			homep = g.pPermissions;
		else
			homep = ChanACL::All;

		homec->uiPermissions = homep;
	}

	qaUserMute->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen));
	qaUserDeaf->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen));
	qaUserTextMessage->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));

	qaChannelAdd->setEnabled(p & (ChanACL::Write | ChanACL::MakeChannel | ChanACL::MakeTempChannel));
	qaChannelRemove->setEnabled(p & ChanACL::Write);
	qaChannelACL->setEnabled((p & ChanACL::Write) || (pparent & ChanACL::Write));

	qaChannelLink->setEnabled((p & (ChanACL::Write | ChanACL::LinkChannel)) && (homep & (ChanACL::Write | ChanACL::LinkChannel)));
	qaChannelUnlink->setEnabled((p & (ChanACL::Write | ChanACL::LinkChannel)) || (homep & (ChanACL::Write | ChanACL::LinkChannel)));
	qaChannelUnlinkAll->setEnabled(p & (ChanACL::Write | ChanACL::LinkChannel));

	qaChannelSendMessage->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));
	qleChat->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));
}

void MainWindow::on_qaAudioReset_triggered() {
	AudioInputPtr ai = g.ai;
	if (ai)
		ai->bResetProcessor = true;
}

void MainWindow::on_qaAudioMute_triggered() {
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

	MumbleProto::UserState mpus;
	mpus.set_self_mute(g.s.bMute);
	mpus.set_self_deaf(g.s.bDeaf);
	g.sh->sendMessage(mpus);

	updateTrayIcon();
}

void MainWindow::on_qaAudioDeaf_triggered() {
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
	} else {
		g.l->log(Log::SelfMute, tr("Undeafened."));
	}

	MumbleProto::UserState mpus;
	mpus.set_self_mute(g.s.bMute);
	mpus.set_self_deaf(g.s.bDeaf);
	g.sh->sendMessage(mpus);

	updateTrayIcon();
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
	ConfigDialog *dlg= new ConfigDialog(this);
	if (dlg->exec() == QDialog::Accepted)
		setupView(false);
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

void MainWindow::on_qaHelpAboutSpeex_triggered() {
	AboutSpeexDialog adAbout(this);
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
	if (down) {
		g.uiDoublePush = g.tDoublePush.restart();
		g.iPushToTalk++;
	} else if (g.iPushToTalk) {
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
			case -1:
				c = Channel::get(0);
				break;
			case -2:
			case -3:
				c = ClientUser::get(g.uiSession)->cChannel;
				if (idx == -2)
					c = c->cParent;
				break;
			default:
				c = pmModel->getSubChannel(c, -4 - idx);
				break;
		}
	} else {
		c = Channel::get(idx);
	}
	return c;
}

void MainWindow::updateTarget() {
	if (qsCurrentTargets.isEmpty()) {
		g.bCenterPosition = false;
		g.iTarget = 0;
	} else {
		bool center = false;
		QList<ShortcutTarget> ql;
		foreach(const ShortcutTarget &st, qsCurrentTargets) {
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
		if (gsMetaChannel->active()) {
			if (! st.bUsers) {
				Channel *c = mapChannel(st.iChannel);
				if (c) {
					MumbleProto::UserState mpus;
					mpus.set_session(g.uiSession);
					mpus.set_channel_id(c->iId);
					g.sh->sendMessage(mpus);
				}
				return;
			}
		}

		qsCurrentTargets.insert(st);
		updateTarget();

		g.iPushToTalk++;
	} else if (g.iPushToTalk) {
		qsCurrentTargets.remove(st);
		updateTarget();

		g.iPushToTalk--;
	}
}

void MainWindow::viewCertificate(bool) {
	ViewCert vc(g.sh->qscCert, this);
	vc.exec();
}

void MainWindow::serverConnected() {
	g.uiSession = 0;
	g.pPermissions = ChanACL::None;
	g.iCodecAlpha = 0x8000000a;
	g.bPreferAlpha = true;
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
	pmModel->setComment(root, QString());
	root->uiPermissions = 0;

	qtvUsers->setRowHidden(0, QModelIndex(), false);

	if (g.s.bMute || g.s.bDeaf) {
		MumbleProto::UserState mpus;
		mpus.set_self_mute(g.s.bMute);
		mpus.set_self_deaf(g.s.bDeaf);
		g.sh->sendMessage(mpus);
	}
}

void MainWindow::serverDisconnected(QAbstractSocket::SocketError err, QString reason) {
	g.uiSession = 0;
	g.pPermissions = ChanACL::None;
	qaServerDisconnect->setEnabled(false);
	qaServerInformation->setEnabled(false);
	qaServerBanList->setEnabled(false);
	qtvUsers->setCurrentIndex(QModelIndex());
	qleChat->setEnabled(false);
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


	if (! g.sh->qlErrors.isEmpty()) {
		foreach(QSslError e, g.sh->qlErrors)
			g.l->log(Log::ServerDisconnected, tr("SSL Verification failed: %1").arg(e.errorString()));
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
				qsl << QString::fromLatin1("<li>%1</li>").arg(e.errorString());

			QMessageBox qmb(QMessageBox::Warning, tr("Mumble"),
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
	}
#ifdef USE_DBUS
	else if (err == QAbstractSocket::SslHandshakeFailedError) {
		if (QMessageBox::warning(this, tr("SSL Version mismatch"), tr("This server is using an older encryption standard. It might be an older 1.1 based Mumble server.<br />Would you like to launch the compatibility client to connect to it?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {

			QString host, user, pw;
			unsigned short port;
			QUrl url;

			g.sh->getConnectionInfo(host, port, user, pw);
			url.setScheme(QLatin1String("mumble"));
			url.setHost(host);
			url.setPort(port);
			url.setUserName(user);
			url.addQueryItem(QLatin1String("version"), QLatin1String("1.1.8"));

			QDBusInterface qdbi(QLatin1String("net.sourceforge.mumble.mumble11x"), QLatin1String("/"), QLatin1String("net.sourceforge.mumble.Mumble"));

			QDBusMessage reply=qdbi.call(QLatin1String("openUrl"), QLatin1String(url.toEncoded()));
			if (reply.type() == QDBusMessage::ReplyMessage) {
				this->close();
				return;
			} else {
				QString executable = QApplication::instance()->applicationFilePath();
				int idx = executable.lastIndexOf(QLatin1String("mumble"));
				if (idx >= 0) {
					QStringList args;
					args << url.toString();

					executable.replace(idx, 6, QLatin1String("mumble11x"));
					if (QProcess::startDetached(executable, args))
						this->close();
					return;
				}
			}
			QMessageBox::critical(this, tr("Failed to launch compatibility client"), tr("The compatibility client could not be found, or failed to start.<br />Note that the compatibility client is an optional component for most installations, and might not be installed."), QMessageBox::Ok, QMessageBox::Ok);
		}
	}
#endif
	else {
		bool ok = false;
		bool matched = false;

		if (! reason.isEmpty()) {
			g.l->log(Log::ServerDisconnected, tr("Server connection failed: %1.").arg(reason));
		}  else {
			g.l->log(Log::ServerDisconnected, tr("Disconnected from server."));
		}

		Qt::WindowFlags wf = 0;
#ifdef Q_OS_MAC
		wf = Qt::Sheet;
#endif

		switch (rtLast) {
			case MumbleProto::Reject_RejectType_InvalidUsername:
			case MumbleProto::Reject_RejectType_UsernameInUse:
				matched = true;
				uname = QInputDialog::getText(this, tr("Invalid username"), (rtLast == MumbleProto::Reject_RejectType_InvalidUsername) ? tr("You connected with an invalid username, please try another one.") : tr("That username is already in use, please try another username."), QLineEdit::Normal, uname, &ok, wf);
				break;
			case MumbleProto::Reject_RejectType_WrongUserPW:
			case MumbleProto::Reject_RejectType_WrongServerPW:
				matched = true;
				pw = QInputDialog::getText(this, tr("Wrong password"), (rtLast == MumbleProto::Reject_RejectType_WrongUserPW) ? tr("Wrong password for registered users, please try again.") : tr("Wrong server password for unregistered user account, please try again."), QLineEdit::Password, pw, &ok, wf);
				break;
			default:
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
			qtReconnect->start();
		}
	}
	AudioInput::setMaxBandwidth(-1);
}

void MainWindow::setupIconMenu(bool top) {
	qmTray->clear();
	if (top) {
		qmTray->addAction(qaQuit);
		qmTray->addSeparator();
		qmTray->addAction(qaAudioDeaf);
		qmTray->addAction(qaAudioMute);
		qmTray->addSeparator();
		qmTray->addAction(qaHelpAbout);
	} else {
		qmTray->addAction(qaHelpAbout);
		qmTray->addSeparator();
		qmTray->addAction(qaAudioMute);
		qmTray->addAction(qaAudioDeaf);
		qmTray->addSeparator();
		qmTray->addAction(qaQuit);
	}
}


void MainWindow::on_Icon_activated(QSystemTrayIcon::ActivationReason reason) {
	// FIXME: Workaround for activated sending both doubleclick and trigger
	static Timer tDoubleClick;
	static bool bDoubleClick = false;

	if (reason == QSystemTrayIcon::DoubleClick) {
		bDoubleClick = true;
		tDoubleClick.restart();
	} else if (bDoubleClick && (reason == QSystemTrayIcon::Trigger) && !(tDoubleClick.elapsed() > 100000UL)) {
		return;
	}

	if (reason == QSystemTrayIcon::Context) {
		bool top = false;

		QPoint p = qstiIcon->geometry().center();
		if (p.isNull()) {
			p = QCursor::pos();
		}

		QDesktopWidget dw;

		QRect qr = dw.screenGeometry(p);

		if (p.y() < (qr.height() / 2))
			top = true;

		setupIconMenu(top);
	}

	if (reason == QSystemTrayIcon::Trigger) {
		if (! isVisible()) {
			show();
			showNormal();
			activateWindow();
		} else {
			hide();
		}
	}
}

void MainWindow::qtvUserCurrentChanged(const QModelIndex &, const QModelIndex &) {
	User *p = pmModel->getUser(qtvUsers->currentIndex());
	Channel *c = pmModel->getChannel(qtvUsers->currentIndex());

	if (g.uiSession == 0) {
		qleChat->setDefaultText(tr("Not connected"));
	} else if (p == NULL || p->uiSession == g.uiSession) {
		// Channel tree target
		if (c == NULL) // If no channel selected fallback to current one
			c = ClientUser::get(g.uiSession)->cChannel;

		qleChat->setDefaultText(tr("Type message to channel '%1' here").arg(c->qsName));
	} else {
		// User target
		qleChat->setDefaultText(tr("Type message to user '%1' here").arg(p->qsName));
	}

	updateMenuPermissions();
}

void MainWindow::customEvent(QEvent *evt) {
	if (evt->type() == TI_QEVENT) {
		hide();
		return;
	} else if (evt->type() == MB_QEVENT) {
		MessageBoxEvent *mbe=static_cast<MessageBoxEvent *>(evt);
		g.l->log(Log::Information, mbe->msg);
		return;
	} else if (evt->type() != SERVERSEND_EVENT) {
		return;
	}

	ServerHandlerMessageEvent *shme=static_cast<ServerHandlerMessageEvent *>(evt);

#define MUMBLE_MH_MSG(x) case MessageHandler:: x : { \
		MumbleProto:: x msg; \
		if (msg.ParseFromArray(shme->qbaMsg.constData(), shme->qbaMsg.size())) { \
			printf("%s:\n", #x); \
			msg.PrintDebugString(); \
			msg##x(msg); \
		} \
		break; \
	}

	switch (shme->uiType) {
			MUMBLE_MH_ALL
	}


#undef MUMBLE_MH_MSG
}

void MainWindow::on_qteLog_anchorClicked(const QUrl &url) {
	QDesktopServices::openUrl(url);
}

void MainWindow::on_qteLog_highlighted(const QUrl &url) {
	if (! url.isValid())
		QToolTip::hideText();
	else
		QToolTip::showText(QCursor::pos(), url.toString(), qteLog, QRect());
}

void MainWindow::on_qdwChat_dockLocationChanged(Qt::DockWidgetArea) {
	g.s.iWindowLayout = Settings::LayoutCustom;
}

void MainWindow::on_qdwLog_dockLocationChanged(Qt::DockWidgetArea) {
	g.s.iWindowLayout = Settings::LayoutCustom;
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

QPair<QByteArray, QImage> MainWindow::openImageFile() {
	QPair<QByteArray, QImage> retval;

	QString fname = QFileDialog::getOpenFileName(this, tr("Choose image file"), g.s.qsImagePath, tr("Images (*.png *.jpg)"));

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

	QImage img;
	if (! img.loadFromData(qba)) {
		QMessageBox::warning(this, tr("Failed to load image"), tr("Image format not recognized."));
		return retval;
	}

	retval.first = qba;
	retval.second = img;

	return retval;
}
