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

#include "MainWindow.h"
#include "AudioWizard.h"
#include "AudioInput.h"
#include "ConnectDialog.h"
#include "Player.h"
#include "Channel.h"
#include "ACLEditor.h"
#include "BanEditor.h"
#include "Connection.h"
#include "ServerHandler.h"
#include "About.h"
#include "GlobalShortcut.h"
#include "VersionCheck.h"
#include "PlayerModel.h"
#include "AudioStats.h"
#include "Plugins.h"
#include "Log.h"
#include "Global.h"
#include "Database.h"
#include "ViewCert.h"
#include "TextMessage.h"
#include "NetworkConfig.h"

MessageBoxEvent::MessageBoxEvent(QString m) : QEvent(static_cast<QEvent::Type>(MB_QEVENT)) {
	msg = m;
}

LogTitleBar::LogTitleBar() {
	qtTick = new QTimer(this);
	qtTick->setSingleShot(true);
	connect(qtTick, SIGNAL(timeout()), this, SLOT(tick()));
	size = newsize = 0;
}

QSize LogTitleBar::sizeHint() const {
	return minimumSizeHint();
}

QSize LogTitleBar::minimumSizeHint() const {
	return QSize(size,size);
}

bool LogTitleBar::eventFilter(QObject *, QEvent *evt) {
	QDockWidget *qdw = qobject_cast<QDockWidget*>(parentWidget());

	switch (evt->type()) {
		case QEvent::Leave:
		case QEvent::Enter:
		case QEvent::MouseMove:
		case QEvent::MouseButtonRelease: {
				newsize = 0;
				QPoint p = qdw->mapFromGlobal(QCursor::pos());
				if ((p.x() >= 0) && (p.x() < qdw->width())  && (p.y() >= 0) && (p.y() < 15))
					newsize = 15;
				if (newsize > 0 && !qtTick->isActive())
					qtTick->start(500);
				else if ((newsize == size) && qtTick->isActive())
					qtTick->stop();
				else if (newsize == 0)
					tick();
			}
		default:
			break;
	}

	return false;
}

void LogTitleBar::tick() {
	QDockWidget *qdw = qobject_cast<QDockWidget*>(parentWidget());

	if (newsize == size)
		return;

	size = newsize;
	qdw->setTitleBarWidget(this);
}

MainWindow::MainWindow(QWidget *p) : QMainWindow(p) {
	qiIconMute.addFile(QLatin1String("skin:muted_self.png"));
	qiIconMute.addFile(QLatin1String("skin:muted_overlay.png"));
	qiIconDeaf.addFile(QLatin1String("skin:deafened_self.png"));
	qiIconDeaf.addFile(QLatin1String("skin:deafened_overlay.png"));
	qiIcon.addFile(QLatin1String("skin:mumble.16x16.png"));
	qiIcon.addFile(QLatin1String("skin:mumble.32x32.png"));
	qiIcon.addFile(QLatin1String("skin:mumble.64x64.png"));
#ifdef Q_OS_MAC
	qiIcon.addFile(QLatin1String("skin:mumble.osx.png"));
#else
	// Set application icon except on MacOSX, where the window-icon
	// shown in the title-bar usually serves as a draggable version of the
	// current open document (i.e. you can copy the open document anywhere
	// simply by dragging this icon).
	qApp->setWindowIcon(qiIcon);
#endif

	Channel::add(0, tr("Root"));

	aclEdit = NULL;
	banEdit = NULL;
	bNoHide = false;

	qtReconnect = new QTimer(this);
	qtReconnect->setInterval(10000);
	qtReconnect->setSingleShot(true);
	qtReconnect->setObjectName(QLatin1String("Reconnect"));

	createActions();
	setupUi(this);
	setupGui();

	connect(g.sh, SIGNAL(connected()), this, SLOT(serverConnected()));
	connect(g.sh, SIGNAL(disconnected(QString)), this, SLOT(serverDisconnected(QString)));

	// Fix context of all actions.
	QList<QAction *> qla = findChildren<QAction *>();
	foreach(QAction *a, qla)
		a->setShortcutContext(Qt::ApplicationShortcut);

	on_qmServer_aboutToShow();
	on_qmChannel_aboutToShow();
	on_qmPlayer_aboutToShow();
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

	gsMuteSelf=new GlobalShortcut(this, idx++, tr("Toggle Mute Self", "Global Shortcut"), false);
	gsMuteSelf->setObjectName(QLatin1String("MuteSelf"));
	gsMuteSelf->qsToolTip = tr("Toggle self-mute status.", "Global Shortcut");
	gsMuteSelf->qsWhatsThis = tr("This will toggle your muted status. If you toggle this off, you will also disable self-deafen.", "Global Shortcut");

	gsDeafSelf=new GlobalShortcut(this, idx++, tr("Toggle Deafen Self", "Global Shortcut"), false);
	gsDeafSelf->setObjectName(QLatin1String("DeafSelf"));
	gsDeafSelf->qsToolTip = tr("Toggle self-deafen status.", "Global Shortcut");
	gsDeafSelf->qsWhatsThis = tr("This will toggle your deafen status. If you toggle this on, you will also enable self-mute.", "Global Shortcut");

	gsUnlink=new GlobalShortcut(this, idx++, tr("Unlink Plugin", "Global Shortcut"));
	gsUnlink->setObjectName(QLatin1String("UnlinkPlugin"));

	gsCenterPos=new GlobalShortcut(this, idx++, tr("Force Center Position", "Global Shortcut"));
	gsCenterPos->setObjectName(QLatin1String("CenterPos"));

	GlobalShortcut *gs;

	gs = new GlobalShortcut(this, idx++, tr("Chan Parent", "Global Shortcut"));
	gs->data = 0;
	connect(gs, SIGNAL(triggered(bool)), this, SLOT(pushLink(bool)));

	for (int i = 1; i< 10;i++) {
		gs = new GlobalShortcut(this, idx++, tr("Chan Sub#%1", "Global Shortcut").arg(i));
		gs->data = i;
		connect(gs, SIGNAL(triggered(bool)), this, SLOT(pushLink(bool)));
	}

	gs = new GlobalShortcut(this, idx++, tr("Chan All Subs", "Global Shortcut"));
	gs->data = 10;
	connect(gs, SIGNAL(triggered(bool)), this, SLOT(pushLink(bool)));

	gsPushMute=new GlobalShortcut(this, idx++, tr("Push-to-Mute", "Global Shortcut"));
	gsPushMute->setObjectName(QLatin1String("PushToMute"));

	gsMetaChannel=new GlobalShortcut(this, idx++, tr("Join Channel", "Global Shortcut"));
	gsMetaChannel->setObjectName(QLatin1String("MetaChannel"));

	gsToggleOverlay=new GlobalShortcut(this, idx++, tr("Toggle Overlay", "Global Shortcut"), false);
	gsToggleOverlay->setObjectName(QLatin1String("ToggleOverlay"));
	gsToggleOverlay->qsToolTip = tr("Toggle state of in-game overlay.", "Global Shortcut");
	gsToggleOverlay->qsWhatsThis = tr("This will switch the states of the ingame overlay between showing everybody, just the players who are talking, and nobody.", "Global Shortcut");

	gsAltTalk=new GlobalShortcut(this, idx++, tr("Alt Push-to-Talk", "Global Shortcut"));
	gsAltTalk->setObjectName(QLatin1String("AltPushToTalk"));

	gsMinimal=new GlobalShortcut(this, idx++, tr("Toggle Minimal", "Global Shortcut"));
	gsMinimal->setObjectName(QLatin1String("ToggleMinimal"));

	gsVolumeUp=new GlobalShortcut(this, idx++, tr("Volume Up (+10%)", "Global Shortcut"));
	gsVolumeUp->setObjectName(QLatin1String("VolumeUp"));

	gsVolumeDown=new GlobalShortcut(this, idx++, tr("Volume Down (-10%)", "Global Shortcut"));
	gsVolumeDown->setObjectName(QLatin1String("VolumeDown"));

	qstiIcon = new QSystemTrayIcon(qiIcon, this);
	qstiIcon->setToolTip(QLatin1String("Mumble"));
	qstiIcon->setObjectName(QLatin1String("Icon"));

#ifndef Q_OS_MAC
	qstiIcon->show();
#endif
}

void MainWindow::setupGui()  {
	setWindowTitle(tr("Mumble -- %1").arg(QLatin1String(MUMBLE_RELEASE)));
	setCentralWidget(qtvPlayers);

#ifdef Q_OS_MAC
	QMenu *qmWindow = new QMenu(tr("&Window"), this);
	menubar->insertMenu(qmHelp->menuAction(), qmWindow);
	qmWindow->addAction(tr("Minimize"), this, SLOT(showMinimized()), QKeySequence(tr("Ctrl+M")));
#endif

	LogDocument *ld = new LogDocument(qteLog);
	qteLog->setDocument(ld);

	qteLog->document()->setDefaultStyleSheet(qApp->styleSheet());

	pmModel = new PlayerModel(qtvPlayers);

	qtvPlayers->setModel(pmModel);
	qtvPlayers->setItemDelegate(new PlayerDelegate(qtvPlayers));

	qtvPlayers->setRowHidden(0, QModelIndex(), true);

	qaServerConnect->setShortcuts(QKeySequence::Open);
	qaServerDisconnect->setShortcuts(QKeySequence::Close);
	qaAudioMute->setChecked(g.s.bMute);
	qaAudioDeaf->setChecked(g.s.bDeaf);
	qaAudioTTS->setChecked(g.s.bTTS);
	qaHelpWhatsThis->setShortcuts(QKeySequence::WhatsThis);

	qaConfigMinimal->setChecked(g.s.bMinimalView);
	qaConfigHideFrame->setChecked(g.s.bHideFrame);

	connect(gsResetAudio, SIGNAL(down()), qaAudioReset, SLOT(trigger()));
	connect(gsMuteSelf, SIGNAL(down()), qaAudioMute, SLOT(trigger()));
	connect(gsDeafSelf, SIGNAL(down()), qaAudioDeaf, SLOT(trigger()));
	connect(gsUnlink, SIGNAL(down()), qaAudioUnlink, SLOT(trigger()));
	connect(gsMinimal, SIGNAL(down()), qaConfigMinimal, SLOT(trigger()));

	ltbDockTitle = new LogTitleBar();
	qdwLog->setTitleBarWidget(ltbDockTitle);

	foreach(QWidget *w, qdwLog->findChildren<QWidget *>()) {
		w->installEventFilter(ltbDockTitle);
		w->setMouseTracking(true);
	}

	if (g.s.bMinimalView && ! g.s.qbaMinimalViewGeometry.isNull())
		restoreGeometry(g.s.qbaMinimalViewGeometry);
	else if (! g.s.bMinimalView && ! g.s.qbaMainWindowGeometry.isNull())
		restoreGeometry(g.s.qbaMainWindowGeometry);

	restoreState(g.s.qbaMainWindowState);
	qtvPlayers->header()->restoreState(g.s.qbaHeaderState);

	setupView(false);

	qmTray = new QMenu(this);
	qmTray->addAction(qaQuit);
	qmTray->addAction(qaHelpAbout);
	qmTray->addSeparator();
	qmTray->addAction(qaAudioMute);
	qmTray->addAction(qaAudioDeaf);
	qstiIcon->setContextMenu(qmTray);
	updateTrayIcon();
}

MainWindow::~MainWindow() {
	delete qdwLog->titleBarWidget();
	delete pmModel;
	delete qtvPlayers;
	delete Channel::get(0);
}

void MainWindow::msgBox(QString msg) {
	MessageBoxEvent *mbe=new MessageBoxEvent(msg);
	QApplication::postEvent(this, mbe);
}

void MainWindow::closeEvent(QCloseEvent *e) {
#ifndef Q_OS_MAC
	if (g.sh && g.sh->isRunning() && g.s.bAskOnQuit) {
		QMessageBox mb(QMessageBox::Warning, QLatin1String("Mumble"), tr("Mumble is currently connected to a server. Do you want to Close or Minimize it?"), QMessageBox::NoButton, this);
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

	g.s.qbaMainWindowState = saveState();
	if (g.s.bMinimalView) {
		g.s.qbaMinimalViewGeometry = saveGeometry();
	} else {
		g.s.qbaMainWindowGeometry = saveGeometry();
		g.s.qbaHeaderState = qtvPlayers->header()->saveState();
	}
	QMainWindow::closeEvent(e);
	qApp->quit();
}

void MainWindow::hideEvent(QHideEvent *e) {
#ifndef Q_OS_MAC
	if (g.s.bHideTray && !bNoHide && qstiIcon->isSystemTrayAvailable())
		qApp->postEvent(this, new QEvent(static_cast<QEvent::Type>(TI_QEVENT)));
	QMainWindow::hideEvent(e);
#endif
}

void MainWindow::on_qtvPlayers_customContextMenuRequested(const QPoint &mpos) {
	QModelIndex idx = qtvPlayers->indexAt(mpos);
	if (! idx.isValid())
		idx = qtvPlayers->currentIndex();
	else
		qtvPlayers->setCurrentIndex(idx);
	Player *p = pmModel->getPlayer(idx);

	if (p) {
		qmPlayer->popup(qtvPlayers->mapToGlobal(mpos), qaPlayerMute);
	} else {
		qmChannel->popup(qtvPlayers->mapToGlobal(mpos), qaChannelACL);
	}
}

void MainWindow::updateTrayIcon() {
	ClientPlayer *p=ClientPlayer::get(g.uiSession);

	if (g.s.bDeaf || (p && p->bDeaf)) {
		qstiIcon->setIcon(qiIconDeaf);
	} else if (g.s.bMute || (p && p->bMute)) {
		qstiIcon->setIcon(qiIconMute);
	} else {
		qstiIcon->setIcon(qiIcon);
	}
}

void MainWindow::on_qtvPlayers_doubleClicked(const QModelIndex &idx) {
	Player *p = pmModel->getPlayer(idx);
	if (p) {
		on_qaPlayerTextMessage_triggered();
		return;
	}

	Channel *c = pmModel->getChannel(idx);
	if (!c)
		return;
	MessagePlayerMove mpm;
	mpm.uiVictim = g.uiSession;
	mpm.iChannelId = c->iId;
	g.sh->sendMessage(&mpm);
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
	if (url.scheme() != QLatin1String("mumble")) {
		g.l->log(Log::Warning, tr("URL scheme is not 'mumble'"));
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

	rtLast = MessageServerReject::None;
	qaServerDisconnect->setEnabled(true);
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
	if (found && (chan != ClientPlayer::get(g.uiSession)->cChannel)) {
		MessagePlayerMove mpm;
		mpm.uiVictim = g.uiSession;
		mpm.iChannelId = chan->iId;
		g.sh->sendMessage(&mpm);
	}
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
	QRect geom = frameGeometry();

	if (toggle_minimize) {
		if (! showit) {
			g.s.qbaMainWindowGeometry = saveGeometry();
			g.s.qbaHeaderState = qtvPlayers->header()->saveState();
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
	qtvPlayers->header()->setVisible(showit);
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
		rtLast = MessageServerReject::None;
		qaServerDisconnect->setEnabled(true);
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
	qmServer->addAction(qaServerInformation);
	qmServer->addSeparator();
	qmServer->addAction(qaQuit);

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
	MessageServerBanList msbl;
	msbl.bQuery = true;
	g.sh->sendMessage(&msbl);

	if (banEdit) {
		banEdit->reject();
		delete banEdit;
		banEdit = NULL;
	}
}

void MainWindow::on_qaServerInformation_triggered() {
	ConnectionPtr c = g.sh->cConnection;

	if (! c)
		return;

	CryptState &cs = c->csCrypt;

	QSslCipher qsc = g.sh->qscCipher;

	QString qsControl=tr("<h2>Control channel</h2><p>Encrypted with %1 bit %2<br />%3 ms average latency (%4 variance)</p>").arg(qsc.usedBits()).arg(qsc.name()).arg(c->dTCPPingAvg, 0, 'f', 2).arg(c->dTCPPingVar / (c->uiTCPPackets - 1),0,'f',2);
	QString qsVoice, qsCrypt, qsAudio;

	if (NetworkConfig::TcpModeEnabled()) {
		qsVoice = tr("Voice channel is sent over control channel.");
	} else {
		qsVoice = tr("<h2>Voice channel</h2><p>Encrypted with 128 bit OCB-AES128<br />%1 ms average latency (%4 variance)</p>").arg(c->dUDPPingAvg, 0, 'f', 2).arg(c->dUDPPingVar / (c->uiUDPPackets - 1),0,'f',2);
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
	qsAudio=tr("<h2>Audio bandwidth</h2><p>Maximum %1 kbit/s<br />Current %2 kbit/s (Quality %3)</p>").arg(g.iMaxBandwidth / 125.0,0,'f',1).arg(g.iAudioBandwidth / 125.0,0,'f',1).arg(g.iAudioQuality);

	QMessageBox qmb(QMessageBox::Information, tr("Mumble Server Information"), qsControl + qsVoice + qsCrypt + qsAudio, QMessageBox::Ok, this);
	qmb.setDefaultButton(QMessageBox::Ok);
	qmb.setEscapeButton(QMessageBox::Ok);

	QPushButton *qp = qmb.addButton(tr("&View Certificate"), QMessageBox::ActionRole);
	int res = qmb.exec();
	if ((res == 0) && (qmb.clickedButton() == qp)) {
		ViewCert vc(g.sh->qscCert, this);
		vc.exec();
	}
}

void MainWindow::on_qmPlayer_aboutToShow() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	bool self = p && (p->uiSession == g.uiSession);

	qmPlayer->clear();

	qmPlayer->addAction(qaPlayerKick);
	qmPlayer->addAction(qaPlayerBan);
	qmPlayer->addAction(qaPlayerMute);
	qmPlayer->addAction(qaPlayerDeaf);
	qmPlayer->addAction(qaPlayerLocalMute);
	qmPlayer->addAction(qaPlayerTextMessage);

	if (self) {
		qmPlayer->addSeparator();
		qmPlayer->addAction(qaAudioMute);
		qmPlayer->addAction(qaAudioDeaf);
	}

	if (g.s.bMinimalView) {
		qmPlayer->addSeparator();
		qmPlayer->addMenu(qmServer);
		qmPlayer->addMenu(qmAudio);
		qmPlayer->addMenu(qmConfig);
		qmPlayer->addMenu(qmHelp);
	}

	if (! qlPlayerActions.isEmpty()) {
		qmPlayer->addSeparator();
		foreach(QAction *a, qlPlayerActions)
			qmPlayer->addAction(a);
	}

	if (! p) {
		qaPlayerKick->setEnabled(false);
		qaPlayerBan->setEnabled(false);
		qaPlayerMute->setEnabled(false);
		qaPlayerLocalMute->setEnabled(false);
		qaPlayerDeaf->setEnabled(false);
		qaPlayerTextMessage->setEnabled(false);
	} else {
		qaPlayerKick->setEnabled(! self);
		qaPlayerBan->setEnabled(! self);
		qaPlayerMute->setEnabled(! self || p->bMute);
		qaPlayerDeaf->setEnabled(! self || p->bDeaf);
		qaPlayerTextMessage->setEnabled(true);
		qaPlayerLocalMute->setEnabled(! self);

		qaPlayerMute->setChecked(p->bMute);
		qaPlayerDeaf->setChecked(p->bDeaf);
		qaPlayerLocalMute->setChecked(p->bLocalMute);
	}
}

void MainWindow::on_qaPlayerMute_triggered() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	MessagePlayerMute mpmMsg;
	mpmMsg.uiVictim = p->uiSession;
	mpmMsg.bMute = ! p->bMute;
	g.sh->sendMessage(&mpmMsg);
}

void MainWindow::on_qaPlayerLocalMute_triggered() {
	ClientPlayer *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	p->setLocalMute(qaPlayerLocalMute->isChecked());
}

void MainWindow::on_qaPlayerDeaf_triggered() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	MessagePlayerDeaf mpdMsg;
	mpdMsg.uiVictim = p->uiSession;
	mpdMsg.bDeaf = ! p->bDeaf;
	g.sh->sendMessage(&mpdMsg);
}

void MainWindow::on_qaPlayerKick_triggered() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	unsigned int session = p->uiSession;

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Kicking player %1").arg(p->qsName), tr("Enter reason"), QLineEdit::Normal, QString(), &ok);

	p = ClientPlayer::get(session);
	if (!p)
		return;

	if (ok) {
		MessagePlayerKick mpkMsg;
		mpkMsg.uiVictim=p->uiSession;
		mpkMsg.qsReason = reason;
		g.sh->sendMessage(&mpkMsg);
	}
}

void MainWindow::on_qaPlayerBan_triggered() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	unsigned int session = p->uiSession;

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Banning player %1").arg(p->qsName), tr("Enter reason"), QLineEdit::Normal, QString(), &ok);
	p = ClientPlayer::get(session);
	if (!p)
		return;

	if (ok) {
		MessagePlayerBan mpbMsg;
		mpbMsg.uiVictim=p->uiSession;
		mpbMsg.qsReason = reason;
		g.sh->sendMessage(&mpbMsg);
	}
}

void MainWindow::on_qaPlayerTextMessage_triggered() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());

	if (!p)
		return;

	unsigned int session = p->uiSession;

	TextMessage *tm = new TextMessage(this);
	tm->setWindowTitle(tr("Sending message to %1").arg(p->qsName));
	int res = tm->exec();

	p = ClientPlayer::get(session);

	if (p && (res==QDialog::Accepted)) {
		MessageTextMessage mtxt;
		mtxt.iChannel = -1;
		mtxt.bTree = false;
		mtxt.uiVictim = p->uiSession;
		mtxt.qsMessage = tm->message();
		g.l->log(Log::TextMessage, tr("To %1: %2").arg(p->qsName).arg(mtxt.qsMessage), tr("Message to %1").arg(p->qsName));
		g.sh->sendMessage(&mtxt);
	}
	delete tm;
}

void MainWindow::on_qaQuit_triggered() {
	this->close();
}

void MainWindow::on_qmConfig_aboutToShow() {
	// Don't remove the config, as that messes up OSX.
	foreach(QAction *a, qmConfig->actions())
		if (a != qaConfigDialog)
			qmConfig->removeAction(a);
	qmConfig->addAction(qaAudioWizard);
	qmConfig->addSeparator();
	qmConfig->addAction(qaConfigMinimal);
	if (g.s.bMinimalView)
		qmConfig->addAction(qaConfigHideFrame);
}

void MainWindow::on_qmChannel_aboutToShow() {
	QModelIndex idx = qtvPlayers->currentIndex();

	qmChannel->clear();
	qmChannel->addAction(qaChannelAdd);
	qmChannel->addAction(qaChannelRemove);
	qmChannel->addAction(qaChannelACL);
	qmChannel->addAction(qaChannelRename);
	qmChannel->addAction(qaChannelDescUpdate);
	qmChannel->addSeparator();
	qmChannel->addAction(qaChannelLink);
	qmChannel->addAction(qaChannelUnlink);
	qmChannel->addAction(qaChannelUnlinkAll);
	qmChannel->addSeparator();
	qmChannel->addAction(qaChannelSendMessage);
	qmChannel->addAction(qaChannelSendTreeMessage);

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

	bool add, remove, acl, rename, descUpdate, link, unlink, unlinkall, msg;
	add = remove = acl = rename = descUpdate = link = unlink = unlinkall = msg = false;

	if (g.uiSession != 0) {
		add = true;
		acl = true;
		msg = true;
		descUpdate = true;

		Channel *c = pmModel->getChannel(idx);
		Channel *home = ClientPlayer::get(g.uiSession)->cChannel;

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
	qaChannelRename->setEnabled(rename);
	qaChannelDescUpdate->setEnabled(descUpdate);
	qaChannelLink->setEnabled(link);
	qaChannelUnlink->setEnabled(unlink);
	qaChannelUnlinkAll->setEnabled(unlinkall);
	qaChannelSendMessage->setEnabled(msg);
	qaChannelSendTreeMessage->setEnabled(msg);
}

void MainWindow::on_qaChannelAdd_triggered() {
	bool ok;
	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());
	int iParent = c ? c->iId : 0;
	QString name = QInputDialog::getText(this, QLatin1String("Mumble"), tr("Channel Name"), QLineEdit::Normal, QString(), &ok);

	c = Channel::get(iParent);
	if (! c)
		return;

	if (ok) {
		MessageChannelAdd mca;
		mca.qsName = name;
		mca.iParent = iParent;
		g.sh->sendMessage(&mca);
	}
}

void MainWindow::on_qaChannelRemove_triggered() {
	int ret;
	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());
	if (! c)
		return;

	int id = c->iId;

	ret=QMessageBox::question(this, QLatin1String("Mumble"), tr("Are you sure you want to delete %1 and all its sub-channels?").arg(c->qsName), QMessageBox::Yes, QMessageBox::No);

	c = Channel::get(id);
	if (!c)
		return;

	if (ret == QMessageBox::Yes) {
		MessageChannelRemove mcr;
		mcr.iId = c->iId;
		g.sh->sendMessage(&mcr);
	}
}

void MainWindow::on_qaChannelRename_triggered() {
	bool ok;
	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());
	if (! c)
		return;

	int id = c->iId;

	QString name = QInputDialog::getText(this, QLatin1String("Mumble"), tr("Channel Name"), QLineEdit::Normal, c->qsName, &ok);

	c = Channel::get(id);
	if (! c)
		return;

	if (ok) {
		MessageChannelRename mcr;
		mcr.iId = id;
		mcr.qsName = name;
		g.sh->sendMessage(&mcr);
	}
}

void MainWindow::on_qaChannelDescUpdate_triggered() {
	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());
	if (! c)
		return;

	int id = c->iId;

	TextMessage tm;
	tm.setWindowTitle(tr("Change description of channel %1").arg(c->qsName));

	const QString html = QTextDocumentFragment::fromPlainText(c->qsDesc).toHtml();

	tm.qteEdit->setText(html);
	int res = tm.exec();

	c = Channel::get(id);
	if (!c)
		return;

	if (res==QDialog::Accepted) {
		MessageChannelDescUpdate mcdu;
		mcdu.iId = id;
		mcdu.qsDesc = tm.message();
		g.sh->sendMessage(&mcdu);
	}
}

void MainWindow::on_qaChannelACL_triggered() {
	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());
	int id = c ? c->iId : 0;

	MessageEditACL mea;
	mea.iId = id;
	mea.bQuery = true;
	g.sh->sendMessage(&mea);

	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = NULL;
	}
}

void MainWindow::on_qaChannelLink_triggered() {
	Channel *c = ClientPlayer::get(g.uiSession)->cChannel;
	Channel *l = pmModel->getChannel(qtvPlayers->currentIndex());
	if (! l)
		l = Channel::get(0);

	MessageChannelLink mcl;
	mcl.iId = c->iId;
	mcl.qlTargets << l->iId;
	mcl.ltType = MessageChannelLink::Link;
	g.sh->sendMessage(&mcl);
}

void MainWindow::on_qaChannelUnlink_triggered() {
	Channel *c = ClientPlayer::get(g.uiSession)->cChannel;
	Channel *l = pmModel->getChannel(qtvPlayers->currentIndex());
	if (! l)
		l = Channel::get(0);

	MessageChannelLink mcl;
	mcl.iId = c->iId;
	mcl.qlTargets << l->iId;
	mcl.ltType = MessageChannelLink::Unlink;
	g.sh->sendMessage(&mcl);
}

void MainWindow::on_qaChannelUnlinkAll_triggered() {
	Channel *c = ClientPlayer::get(g.uiSession)->cChannel;

	MessageChannelLink mcl;
	mcl.iId = c->iId;
	mcl.ltType = MessageChannelLink::UnlinkAll;
	g.sh->sendMessage(&mcl);
}

void MainWindow::on_qaChannelSendMessage_triggered() {
	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());

	if (!c)
		return;

	int id = c->iId;

	TextMessage tm;
	tm.setWindowTitle(tr("Sending message to channel %1").arg(c->qsName));
	int res = tm.exec();

	c = Channel::get(id);
	if (!c)
		return;

	if (res==QDialog::Accepted) {
		MessageTextMessage mtxt;
		mtxt.iChannel = id;
		mtxt.bTree = false;
		mtxt.uiVictim = 0;
		mtxt.qsMessage = tm.message();
		g.l->log(Log::TextMessage, tr("To %1: %2").arg(c->qsName).arg(mtxt.qsMessage), tr("Message to %1").arg(c->qsName));
		g.sh->sendMessage(&mtxt);
	}
}

void MainWindow::on_qaChannelSendTreeMessage_triggered() {
	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());

	if (!c)
		return;

	int id = c->iId;

	TextMessage tm;
	tm.setWindowTitle(tr("Sending message to channel tree %1").arg(c->qsName));
	int res = tm.exec();

	c = Channel::get(id);
	if (!c)
		return;

	if (res==QDialog::Accepted) {
		MessageTextMessage mtxt;
		mtxt.iChannel = id;
		mtxt.bTree = true;
		mtxt.uiVictim = 0;
		mtxt.qsMessage = tm.message();
		g.l->log(Log::TextMessage, tr("To tree %1: %2").arg(c->qsName).arg(mtxt.qsMessage), tr("Message to tree %1").arg(c->qsName));
		g.sh->sendMessage(&mtxt);
	}
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

	MessagePlayerSelfMuteDeaf mpsmd;
	mpsmd.bMute = g.s.bMute;
	mpsmd.bDeaf = g.s.bDeaf;
	g.sh->sendMessage(&mpsmd);

	updateTrayIcon();
}

void MainWindow::on_qaAudioDeaf_triggered() {
	AudioInputPtr ai = g.ai;
	if (ai)
		ai->tIdle.restart();

	g.s.bDeaf = qaAudioDeaf->isChecked();
	if (g.s.bDeaf && ! g.s.bMute) {
		g.s.bMute = true;
		qaAudioMute->setChecked(true);
		g.l->log(Log::SelfMute, tr("Muted and deafened."));
	} else if (g.s.bDeaf) {
		g.l->log(Log::SelfMute, tr("Deafened."));
	} else {
		g.l->log(Log::SelfMute, tr("Undeafened."));
	}

	MessagePlayerSelfMuteDeaf mpsmd;
	mpsmd.bMute = g.s.bMute;
	mpsmd.bDeaf = g.s.bDeaf;
	g.sh->sendMessage(&mpsmd);

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
		setOnTop(g.s.bAlwaysOnTop);
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

void MainWindow::on_PushToTalk_triggered(bool down) {
	if (down) {
		g.uiDoublePush = g.tDoublePush.restart();
		g.iPushToTalk++;
	} else if (g.iPushToTalk) {
		g.iPushToTalk--;
	}
}

void MainWindow::on_PushToMute_triggered(bool down) {
	g.bPushToMute = down;
}

void MainWindow::on_AltPushToTalk_triggered(bool down) {
	if (down) {
		g.iAltSpeak++;
		g.iPushToTalk++;
	} else if (g.iPushToTalk) {
		g.iAltSpeak--;
		g.iPushToTalk--;
	}
}

void MainWindow::on_CenterPos_triggered(bool down) {
	g.bCenterPosition = down;

	if (down) {
		g.iAltSpeak++;
		g.iPushToTalk++;
	} else if (g.iPushToTalk) {
		g.iAltSpeak--;
		g.iPushToTalk--;
	}
}

void MainWindow::on_VolumeUp_triggered(bool down) {
	if (down) {
		float v = floorf(g.s.fVolume * 10.0f);
		if (v < 20.0f)
			g.s.fVolume = ++v / 10.0f;
	}
}

void MainWindow::on_VolumeDown_triggered(bool down) {
	if (down) {
		float v = ceilf(g.s.fVolume * 10.0f);
		if (v > 0.0f)
			g.s.fVolume = --v / 10.0f;
	}
}

void MainWindow::pushLink(bool down) {
	if (down) {
		g.iAltSpeak++;
		g.iPushToTalk++;
	} else if (g.iPushToTalk) {
		g.iAltSpeak--;
		g.iPushToTalk--;
	}

	if (g.uiSession == 0)
		return;

	GlobalShortcut *gs = qobject_cast<GlobalShortcut *>(sender());
	if (! gs)
		return;
	int idx = gs->data.toInt();
	Channel *home = ClientPlayer::get(g.uiSession)->cChannel;

	Channel *target = NULL;
	switch (idx) {
		case 0:
			target = home->cParent;
			break;
		case 10:
			break;
		default:
			target = pmModel->getSubChannel(home, idx-1);
			break;
	}


	if (gsMetaChannel->active()) {
		if (! target || ! down)
			return;

		MessagePlayerMove mpm;
		mpm.uiVictim = g.uiSession;
		mpm.iChannelId = target->iId;
		g.sh->sendMessage(&mpm);
		g.l->log(Log::Information, tr("Joining %1.").arg(target->qsName));
	} else {
		MessageChannelLink mcl;
		mcl.iId = home->iId;
		if (down)
			mcl.ltType = MessageChannelLink::PushLink;
		else
			mcl.ltType = MessageChannelLink::PushUnlink;
		if (idx == 10) {
			foreach(Channel *l, home->qlChannels)
				mcl.qlTargets << l->iId;
		} else if (target) {
			mcl.qlTargets << target->iId;
		}
		if (mcl.qlTargets.count() == 0)
			return;
		g.sh->sendMessage(&mcl);
	}
}

void MainWindow::viewCertificate(bool) {
	ViewCert vc(g.sh->qscCert, this);
	vc.exec();
}

void MainWindow::serverConnected() {
	g.uiSession = 0;
	g.l->clearIgnore();
	g.l->setIgnore(Log::PlayerJoin);
	g.l->setIgnore(Log::OtherSelfMute);
	QString host, uname, pw;
	unsigned short port;
	g.sh->getConnectionInfo(host, port, uname, pw);
	g.l->log(Log::ServerConnected, tr("Connected to server %1.").arg(host));
	qaServerDisconnect->setEnabled(true);
	qaServerInformation->setEnabled(true);
	qaServerBanList->setEnabled(true);

	pmModel->renameChannel(Channel::get(0), tr("Root"));
	qtvPlayers->setRowHidden(0, QModelIndex(), false);

	if (g.s.bMute || g.s.bDeaf) {
		MessagePlayerSelfMuteDeaf mpsmd;
		mpsmd.bMute = g.s.bMute;
		mpsmd.bDeaf = g.s.bDeaf;
		g.sh->sendMessage(&mpsmd);
	}
}

void MainWindow::serverDisconnected(QString reason) {
	g.uiSession = 0;
	qaServerDisconnect->setEnabled(false);
	qaServerInformation->setEnabled(false);
	qaServerBanList->setEnabled(false);
	updateTrayIcon();

	QString uname, pw, host;
	unsigned short port;
	g.sh->getConnectionInfo(host, port, uname, pw);

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

	QSet<QAction *> qs;
	qs += qlServerActions.toSet();
	qs += qlChannelActions.toSet();
	qs += qlPlayerActions.toSet();

	foreach(QAction *a, qs)
		delete a;

	qlServerActions.clear();
	qlChannelActions.clear();
	qlPlayerActions.clear();

	pmModel->removeAll();
	qtvPlayers->setRowHidden(0, QModelIndex(), true);

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
					g.sh->start(QThread::TimeCriticalPriority);
				}
				break;
			}
		}
	} else {
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
			case MessageServerReject::InvalidUsername:
			case MessageServerReject::UsernameInUse:
				matched = true;
				uname = QInputDialog::getText(this, tr("Invalid username"), (rtLast == MessageServerReject::InvalidUsername) ? tr("You connected with an invalid username, please try another one.") : tr("That username is already in use, please try another username."), QLineEdit::Normal, uname, &ok, wf);
				break;
			case MessageServerReject::WrongUserPW:
			case MessageServerReject::WrongServerPW:
				matched = true;
				pw = QInputDialog::getText(this, tr("Wrong password"), (rtLast == MessageServerReject::WrongUserPW) ? tr("Wrong password for registered users, please try again.") : tr("Wrong server password for unregistered user account, please try again."), QLineEdit::Password, pw, &ok, wf);
				break;
			default:
				break;
		}
		if (ok && matched) {
			qaServerDisconnect->setEnabled(true);
			g.sh->setConnectionInfo(host, port, uname, pw);
			g.sh->start(QThread::TimeCriticalPriority);
		} else if (!matched && g.s.bReconnect && ! reason.isEmpty()) {
			qaServerDisconnect->setEnabled(true);
			qtReconnect->start();
		}
	}
}

void MainWindow::on_Icon_activated(QSystemTrayIcon::ActivationReason reason) {
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

void MainWindow::customEvent(QEvent *evt) {
	if (evt->type() == TI_QEVENT) {
		hide();
		return;
	}
	if (evt->type() == MB_QEVENT) {
		MessageBoxEvent *mbe=static_cast<MessageBoxEvent *>(evt);
		g.l->log(Log::Information, mbe->msg);
		return;
	}
	if (evt->type() != SERVERSEND_EVENT)
		return;

	ServerHandlerMessageEvent *shme=static_cast<ServerHandlerMessageEvent *>(evt);

	Message *mMsg = Message::networkToMessage(shme->qbaMsg);
	if (mMsg) {
		dispatch(NULL, mMsg);
		delete mMsg;
	}
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

void MainWindow::context_triggered() {
	QAction *a = qobject_cast<QAction *>(sender());

	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());
	ClientPlayer *p = pmModel->getPlayer(qtvPlayers->currentIndex());

	MessageContextAction mca;
	mca.uiSession = g.uiSession;
	mca.qsAction = a->data().toString();
	mca.uiVictim = p ? p->uiSession : 0;
	mca.iChannel = c ? c->iId : -1;

	g.sh->sendMessage(&mca);
}
