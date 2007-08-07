/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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
#include "Overlay.h"
#include "Global.h"
#include "Database.h"
#include "ViewCert.h"

MessageBoxEvent::MessageBoxEvent(QString m) : QEvent(static_cast<QEvent::Type>(MB_QEVENT)) {
	msg = m;
}

MainWindow::MainWindow(QWidget *p) : QMainWindow(p) {
	Channel::add(0, tr("Root"), NULL);

	aclEdit = NULL;
	banEdit = NULL;

	qtReconnect = new QTimer(this);
	qtReconnect->setInterval(10000);
	qtReconnect->setSingleShot(true);
	qtReconnect->setObjectName(QLatin1String("Reconnect"));

	createActions();
	setupGui();

	connect(g.sh, SIGNAL(connected()), this, SLOT(serverConnected()));
	connect(g.sh, SIGNAL(disconnected(QString)), this, SLOT(serverDisconnected(QString)));
}

void MainWindow::createActions() {
	qaQuit = new QAction(tr("&Quit"), this);
	qaQuit->setToolTip(tr("Closes the program"));
	qaQuit->setWhatsThis(tr("Exits the application."));
	qaQuit->setObjectName(QLatin1String("Quit"));
	qaQuit->setShortcut(tr("Ctrl+Q", "Quit"));

	qaServerConnect=new QAction(tr("&Connect"), this);
	qaServerConnect->setToolTip(tr("Open the server connection dialog"));
	qaServerConnect->setWhatsThis(tr("Shows a dialog of registered servers, and also allows quick connect."));
	qaServerConnect->setObjectName(QLatin1String("ServerConnect"));
	qaServerConnect->setShortcuts(QKeySequence::Open);
	qaServerDisconnect=new QAction(tr("&Disconnect"), this);
	qaServerDisconnect->setToolTip(tr("Disconnect from server"));
	qaServerDisconnect->setWhatsThis(tr("Disconnects you from the server."));
	qaServerDisconnect->setObjectName(QLatin1String("ServerDisconnect"));
	qaServerDisconnect->setEnabled(false);
	qaServerDisconnect->setShortcuts(QKeySequence::Close);
	qaServerBanList=new QAction(tr("&Ban lists"), this);
	qaServerBanList->setToolTip(tr("Edit ban lists on server"));
	qaServerBanList->setWhatsThis(tr("This lets you edit the server-side IP ban lists."));
	qaServerBanList->setObjectName(QLatin1String("ServerBanList"));
	qaServerBanList->setEnabled(false);
	qaServerInformation=new QAction(tr("&Information"), this);
	qaServerInformation->setToolTip(tr("Show information about the server connection"));
	qaServerInformation->setWhatsThis(tr("This will show extended information about the connection to the server."));
	qaServerInformation->setObjectName(QLatin1String("ServerInformation"));
	qaServerInformation->setEnabled(false);

	qaPlayerKick=new QAction(tr("&Kick"), this);
	qaPlayerKick->setObjectName(QLatin1String("PlayerKick"));
	qaPlayerKick->setToolTip(tr("Kick player (with reason)"));
	qaPlayerKick->setWhatsThis(tr("Kick selected player off server. You'll be asked to specify a reason."));
	qaPlayerBan=new QAction(tr("&Ban"), this);
	qaPlayerBan->setObjectName(QLatin1String("PlayerBan"));
	qaPlayerBan->setToolTip(tr("Kick and ban player (with reason)"));
	qaPlayerBan->setWhatsThis(tr("Kick and ban selected player from server. You'll be asked to specify a reason."));
	qaPlayerMute=new QAction(tr("&Mute"), this);
	qaPlayerMute->setObjectName(QLatin1String("PlayerMute"));
	qaPlayerMute->setCheckable(true);
	qaPlayerMute->setToolTip(tr("Mute player"));
	qaPlayerMute->setWhatsThis(tr("Mute or unmute player on server. Unmuting a deafened player will also undeafen them."));
	qaPlayerDeaf=new QAction(tr("&Deafen"), this);
	qaPlayerDeaf->setObjectName(QLatin1String("PlayerDeaf"));
	qaPlayerDeaf->setCheckable(true);
	qaPlayerDeaf->setToolTip(tr("Deafen player"));
	qaPlayerDeaf->setWhatsThis(tr("Deafen or undeafen player on server. Deafening a player will also mute them."));
	qaPlayerLocalMute=new QAction(tr("&Local Mute"), this);
	qaPlayerLocalMute->setObjectName(QLatin1String("PlayerLocalMute"));
	qaPlayerLocalMute->setCheckable(true);
	qaPlayerLocalMute->setToolTip(tr("Mute player"));
	qaPlayerLocalMute->setWhatsThis(tr("Mute or unmute player locally."));
	qaPlayerTextMessage = new QAction(tr("Send Messa&ge"), this);
	qaPlayerTextMessage->setObjectName(QLatin1String("PlayerTextMessage"));
	qaPlayerTextMessage->setToolTip(tr("Send a Text Message"));
	qaPlayerTextMessage->setWhatsThis(tr("Sends a text message to another user."));

	qaChannelAdd=new QAction(tr("&Add"), this);
	qaChannelAdd->setObjectName(QLatin1String("ChannelAdd"));
	qaChannelAdd->setToolTip(tr("Add new channel"));
	qaChannelAdd->setWhatsThis(tr("This adds a new sub-channel to the currently selected channel."));
	qaChannelRemove=new QAction(tr("&Remove"), this);
	qaChannelRemove->setObjectName(QLatin1String("ChannelRemove"));
	qaChannelRemove->setToolTip(tr("Remove channel"));
	qaChannelRemove->setWhatsThis(tr("This removes a channel and all sub-channels."));
	qaChannelACL=new QAction(tr("&Edit ACL"), this);
	qaChannelACL->setObjectName(QLatin1String("ChannelACL"));
	qaChannelACL->setToolTip(tr("Edit Groups and ACL for channel"));
	qaChannelACL->setWhatsThis(tr("This opens the Group and ACL dialog for the channel, to control permissions."));
	qaChannelRename=new QAction(tr("&Rename channel"), this);
	qaChannelRename->setObjectName(QLatin1String("ChannelRename"));
	qaChannelRename->setToolTip(tr("Renames the channel"));
	qaChannelRename->setWhatsThis(tr("This renames a channel."));
	qaChannelLink=new QAction(tr("&Link"), this);
	qaChannelLink->setObjectName(QLatin1String("ChannelLink"));
	qaChannelLink->setToolTip(tr("Link your channel to another channel"));
	qaChannelLink->setWhatsThis(tr("This links your current channel to the selected channel. If they have permission to speak in "
	                               "the other channel, players can now hear each other. This is a permanent link, and will last until "
	                               "manually unlinked or the server is restarted. Please see the shortcuts for push-to-link."));
	qaChannelUnlink=new QAction(tr("&Unlink"), this);
	qaChannelUnlink->setObjectName(QLatin1String("ChannelUnlink"));
	qaChannelUnlink->setToolTip(tr("Unlink your channel from another channel"));
	qaChannelUnlink->setWhatsThis(tr("This unlinks your current channel from the selected channel."));
	qaChannelUnlinkAll=new QAction(tr("Unlink &All"), this);
	qaChannelUnlinkAll->setObjectName(QLatin1String("ChannelUnlinkAll"));
	qaChannelUnlinkAll->setToolTip(tr("Unlinks your channel from all linked channels."));
	qaChannelUnlinkAll->setWhatsThis(tr("This unlinks your current channel (not the selected one) from all linked channels."));

	qaAudioReset=new QAction(tr("&Reset"), this);
	qaAudioReset->setObjectName(QLatin1String("AudioReset"));
	qaAudioReset->setToolTip(tr("Reset audio preprocessor"));
	qaAudioReset->setWhatsThis(tr("This will reset the audio preprocessor, including noise cancellation, automatic gain and voice activity detection. "
	                              "If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, "
	                              "use this to avoid having to wait for the preprocessor to readjust."));
	qaAudioMute=new QAction(tr("&Mute Self"), this);
	qaAudioMute->setObjectName(QLatin1String("AudioMute"));
	qaAudioMute->setCheckable(true);
	qaAudioMute->setChecked(g.s.bMute);
	qaAudioMute->setToolTip(tr("Mute yourself"));
	qaAudioMute->setWhatsThis(tr("Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafened will also undeafen."));
	qaAudioDeaf=new QAction(tr("&Deafen Self"), this);
	qaAudioDeaf->setObjectName(QLatin1String("AudioDeaf"));
	qaAudioDeaf->setCheckable(true);
	qaAudioDeaf->setChecked(g.s.bDeaf);
	qaAudioDeaf->setToolTip(tr("Deafen yourself"));
	qaAudioDeaf->setWhatsThis(tr("Deafen or undeafen yourself. When deafened, you will not hear anything. Deafening yourself will also mute."));
	qaAudioTTS=new QAction(tr("&Text-To-Speech"), this);
	qaAudioTTS->setObjectName(QLatin1String("AudioTextToSpeech"));
	qaAudioTTS->setCheckable(true);
	qaAudioTTS->setChecked(g.qs->value(QLatin1String("TextToSpeech"), true).toBool());
	qaAudioTTS->setToolTip(tr("Toggle Text-To-Speech"));
	qaAudioTTS->setWhatsThis(tr("Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Configuration dialog will actually be spoken."));
	qaAudioStats=new QAction(tr("S&tatistics"), this);
	qaAudioStats->setObjectName(QLatin1String("AudioStats"));
	qaAudioStats->setToolTip(tr("Display audio statistics"));
	qaAudioStats->setWhatsThis(tr("Pops up a small dialog with information about your current audio input."));
	qaAudioUnlink=new QAction(tr("&Unlink"), this);
	qaAudioUnlink->setObjectName(QLatin1String("AudioUnlink"));
	qaAudioUnlink->setToolTip(tr("Forcibly unlink plugin"));
	qaAudioUnlink->setWhatsThis(tr("This forces the current plugin to unlink, which is handy if it is reading "
	                               "completely wrong data."));

	qaConfigDialog=new QAction(tr("&Settings"), this);
	qaConfigDialog->setObjectName(QLatin1String("ConfigDialog"));
	qaConfigDialog->setToolTip(tr("Configure Mumble"));
	qaConfigDialog->setWhatsThis(tr("Allows you to change most settings for Mumble."));

	qaAudioWizard=new QAction(tr("&Audio Wizard"), this);
	qaAudioWizard->setObjectName(QLatin1String("AudioWizard"));
	qaAudioWizard->setToolTip(tr("Start the audio configuration wizard"));
	qaAudioWizard->setWhatsThis(tr("This will guide you through the process of configuring your audio hardware."));

	qaHelpWhatsThis = new QAction(tr("&What's This?"), this);
	qaHelpWhatsThis->setObjectName(QLatin1String("HelpWhatsThis"));
	qaHelpWhatsThis->setToolTip(tr("Enter What's This? mode"));
	qaHelpWhatsThis->setWhatsThis(tr("Click this to enter \"What's This?\" mode. Your cursor will turn into a question mark. Click "
	                                 "on any button, menu choice or area to show a description of what it is."));
	qaHelpWhatsThis->setShortcuts(QKeySequence::WhatsThis);
	qaHelpAbout=new QAction(tr("&About"), this);
	qaHelpAbout->setObjectName(QLatin1String("HelpAbout"));
	qaHelpAbout->setToolTip(tr("Information about Mumble"));
	qaHelpAbout->setWhatsThis(tr("Shows a small dialog with information and license for Mumble."));
	qaHelpAboutSpeex=new QAction(tr("About &Speex"), this);
	qaHelpAboutSpeex->setObjectName(QLatin1String("HelpAboutSpeex"));
	qaHelpAboutSpeex->setToolTip(tr("Information about Speex"));
	qaHelpAboutSpeex->setWhatsThis(tr("Shows a small dialog with information about Speex."));
	qaHelpAboutQt=new QAction(tr("&About Qt"), this);
	qaHelpAboutQt->setObjectName(QLatin1String("HelpAboutQt"));
	qaHelpAboutQt->setToolTip(tr("Information about Qt"));
	qaHelpAboutQt->setWhatsThis(tr("Shows a small dialog with information about Qt."));
	qaHelpVersionCheck=new QAction(tr("Check for &Updates"), this);
	qaHelpVersionCheck->setObjectName(QLatin1String("HelpVersionCheck"));
	qaHelpVersionCheck->setToolTip(tr("Check for new version of Mumble"));
	qaHelpVersionCheck->setWhatsThis(tr("Connects to the Mumble webpage to check if a new version is available, and notifies "
	                                    "you with an appropriate download URL if this is the case."));
}

void MainWindow::setupGui()  {
	setWindowTitle(tr("Mumble -- %1").arg(QLatin1String(MUMBLE_RELEASE)));

	pmModel = new PlayerModel(this);

	QTreeView *view = new QTreeView(this);
	qtvPlayers = view;
	qtvPlayers->setObjectName(QLatin1String("Players"));
	qtvPlayers->setContextMenuPolicy(Qt::CustomContextMenu);
	qtvPlayers->setModel(pmModel);
	qtvPlayers->setItemDelegate(new PlayerDelegate(view));
	qtvPlayers->setDragEnabled(true);
	qtvPlayers->setAcceptDrops(true);
	qtvPlayers->setDropIndicatorShown(true);
	qtvPlayers->setIndentation(10);

	qteLog = new QTextEdit(this);
	qteLog->setReadOnly(true);
	qteLog->setToolTip(tr("Log of messages"));
	qteLog->setWhatsThis(tr("This shows all recent activity. Connecting to servers, errors and information messages all show up here.<br />"
	                        "To configure exactly which messages show up here, use the <b>Settings</b> command from the menu."));

	qmServer = new QMenu(tr("&Server"), this);
	qmPlayer = new QMenu(tr("&Player"), this);
	qmChannel = new QMenu(tr("&Channel"), this);
	qmAudio = new QMenu(tr("&Audio"), this);
	qmConfig = new QMenu(tr("C&onfigure"), this);
	qmHelp = new QMenu(tr("&Help"), this);

	qmServer->setObjectName(QLatin1String("ServerMenu"));
	qmPlayer->setObjectName(QLatin1String("PlayerMenu"));
	qmChannel->setObjectName(QLatin1String("ChannelMenu"));
	qmAudio->setObjectName(QLatin1String("AudioMenu"));
	qmConfig->setObjectName(QLatin1String("ConfigMenu"));
	qmHelp->setObjectName(QLatin1String("HelpMenu"));

	qmServer->addAction(qaServerConnect);
	qmServer->addAction(qaServerDisconnect);
	qmServer->addAction(qaServerBanList);
	qmServer->addAction(qaServerInformation);
	qmServer->addSeparator();
	qmServer->addAction(qaQuit);

	qmPlayer->addAction(qaPlayerKick);
	qmPlayer->addAction(qaPlayerBan);
	qmPlayer->addAction(qaPlayerMute);
	qmPlayer->addAction(qaPlayerDeaf);
	qmPlayer->addAction(qaPlayerLocalMute);
	qmPlayer->addAction(qaPlayerTextMessage);

	qmChannel->addAction(qaChannelAdd);
	qmChannel->addAction(qaChannelRemove);
	qmChannel->addAction(qaChannelACL);
	qmChannel->addAction(qaChannelRename);
	qmChannel->addSeparator();
	qmChannel->addAction(qaChannelLink);
	qmChannel->addAction(qaChannelUnlink);
	qmChannel->addAction(qaChannelUnlinkAll);

	qmAudio->addAction(qaAudioMute);
	qmAudio->addAction(qaAudioDeaf);
	qmAudio->addSeparator();
	qmAudio->addAction(qaAudioReset);
	qmAudio->addAction(qaAudioUnlink);
	qmAudio->addSeparator();
	qmAudio->addAction(qaAudioTTS);
	qmAudio->addSeparator();
	qmAudio->addAction(qaAudioStats);

	qmConfig->addAction(qaConfigDialog);
#ifdef Q_OS_WIN
	qmConfig->addAction(qaAudioWizard);
#endif

	qmHelp->addAction(qaHelpWhatsThis);
	qmHelp->addSeparator();
	qmHelp->addAction(qaHelpAbout);
	qmHelp->addAction(qaHelpAboutSpeex);
	qmHelp->addAction(qaHelpAboutQt);
	qmHelp->addSeparator();
	qmHelp->addAction(qaHelpVersionCheck);

	menuBar()->addMenu(qmServer);
	menuBar()->addMenu(qmPlayer);
	menuBar()->addMenu(qmChannel);
	menuBar()->addMenu(qmAudio);
	menuBar()->addMenu(qmConfig);
	menuBar()->addMenu(qmHelp);

	int idx = 1;

	gsPushTalk=new GlobalShortcut(this, idx++, tr("Push-to-Talk", "Global Shortcut"));
	gsPushTalk->setObjectName(QLatin1String("PushToTalk"));

	gsResetAudio=new GlobalShortcut(this, idx++, tr("Reset Audio Processor", "Global Shortcut"));
	gsResetAudio->setObjectName(QLatin1String("ResetAudio"));
	connect(gsResetAudio, SIGNAL(down()), qaAudioReset, SLOT(trigger()));

	gsMuteSelf=new GlobalShortcut(this, idx++, tr("Toggle Mute Self", "Global Shortcut"));
	gsMuteSelf->setObjectName(QLatin1String("MuteSelf"));
	connect(gsMuteSelf, SIGNAL(down()), qaAudioMute, SLOT(trigger()));

	gsDeafSelf=new GlobalShortcut(this, idx++, tr("Toggle Deafen Self", "Global Shortcut"));
	gsDeafSelf->setObjectName(QLatin1String("DeafSelf"));
	connect(gsDeafSelf, SIGNAL(down()), qaAudioDeaf, SLOT(trigger()));

	gsUnlink=new GlobalShortcut(this, idx++, tr("Unlink Plugin", "Global Shortcut"));
	gsUnlink->setObjectName(QLatin1String("UnlinkPlugin"));
	connect(gsUnlink, SIGNAL(down()), qaAudioUnlink, SLOT(trigger()));

	gsCenterPos=new GlobalShortcut(this, idx++, tr("Force Center Position", "Global Shortcut"));
	gsCenterPos->setObjectName(QLatin1String("CenterPos"));

	GlobalShortcut *gs;

	gs = new GlobalShortcut(this, idx++, tr("Chan Parent", "Global Shortcut"));
	gs->setData(0);
	connect(gs, SIGNAL(triggered(bool)), this, SLOT(pushLink(bool)));

	for (int i = 1; i< 10;i++) {
		gs = new GlobalShortcut(this, idx++, tr("Chan Sub#%1", "Global Shortcut").arg(i));
		gs->setData(i);
		connect(gs, SIGNAL(triggered(bool)), this, SLOT(pushLink(bool)));
	}

	gs = new GlobalShortcut(this, idx++, tr("Chan All Subs", "Global Shortcut"));
	gs->setData(10);
	connect(gs, SIGNAL(triggered(bool)), this, SLOT(pushLink(bool)));

	gsPushMute=new GlobalShortcut(this, idx++, tr("Push-to-Mute", "Global Shortcut"));
	gsPushMute->setObjectName(QLatin1String("PushToMute"));

	gsMetaChannel=new GlobalShortcut(this, idx++, tr("Join Channel", "Global Shortcut"));
	gsMetaChannel->setObjectName(QLatin1String("MetaChannel"));

	gsToggleOverlay=new GlobalShortcut(this, idx++, tr("Toggle Overlay", "Global Shortcut"));
	gsToggleOverlay->setObjectName(QLatin1String("ToggleOverlay"));
	connect(gsToggleOverlay, SIGNAL(down()), g.o, SLOT(toggleShow()));

	gsAltTalk=new GlobalShortcut(this, idx++, tr("Alt Push-to-Talk", "Global Shortcut"));
	gsAltTalk->setObjectName(QLatin1String("AltPushToTalk"));

	if (g.qs->value(QLatin1String("Horizontal"), true).toBool()) {
		qsSplit = new QSplitter(Qt::Horizontal, this);
		qsSplit->addWidget(qteLog);
		qsSplit->addWidget(qtvPlayers);
	} else {
		qsSplit = new QSplitter(Qt::Vertical, this);
		qsSplit->addWidget(qtvPlayers);
		qsSplit->addWidget(qteLog);
	}
	setCentralWidget(qsSplit);

	restoreGeometry(g.qs->value(QLatin1String("mwgeom")).toByteArray());
	restoreState(g.qs->value(QLatin1String("mw")).toByteArray());
	qsSplit->restoreState(g.qs->value(QLatin1String("mwSplitter")).toByteArray());

	qstiIcon = new QSystemTrayIcon(qApp->windowIcon(), this);
	qstiIcon->setObjectName(QLatin1String("Icon"));
	qstiIcon->show();

	QMetaObject::connectSlotsByName(this);
}

void MainWindow::msgBox(QString msg) {
	MessageBoxEvent *mbe=new MessageBoxEvent(msg);
	QApplication::postEvent(this, mbe);
}

void MainWindow::closeEvent(QCloseEvent *e) {
	g.uiSession = 0;
	g.qs->setValue(QLatin1String("mwgeom"), saveGeometry());
	g.qs->setValue(QLatin1String("mw"), saveState());
	g.qs->setValue(QLatin1String("mwSplitter"), qsSplit->saveState());
	QMainWindow::closeEvent(e);
	qApp->quit();
}

void MainWindow::hideEvent(QHideEvent *e) {
	if (qstiIcon->isSystemTrayAvailable())
		qApp->postEvent(this, new QEvent(static_cast<QEvent::Type>(TI_QEVENT)));
	QMainWindow::hideEvent(e);
}

void MainWindow::appendLog(QString entry) {
	qteLog->append(entry);
	QTextCursor p=qteLog->textCursor();
	p.movePosition(QTextCursor::End);
	qteLog->setTextCursor(p);
	qteLog->ensureCursorVisible();
}

void MainWindow::on_Players_customContextMenuRequested(const QPoint &mpos) {
	QModelIndex idx = qtvPlayers->indexAt(mpos);
	if (! idx.isValid())
		idx = qtvPlayers->currentIndex();
	Player *p = pmModel->getPlayer(idx);

	if (p) {
		qmPlayer->popup(qtvPlayers->mapToGlobal(mpos), qaPlayerMute);
	} else {
		qmChannel->popup(qtvPlayers->mapToGlobal(mpos), qaChannelACL);
	}
}

void MainWindow::on_Players_doubleClicked(const QModelIndex &idx) {
	Player *p = pmModel->getPlayer(idx);
	if (p) {
		on_PlayerTextMessage_triggered();
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

void MainWindow::openUrl(const QUrl &url) {
	g.l->log(Log::Information, tr("Opening URL %1").arg(url.toString()));
	if (url.scheme() != QLatin1String("mumble")) {
		g.l->log(Log::Warning, tr("URL scheme is not 'mumble'"));
		return;
	}
	QString host = url.host();
	int port = url.port(64738);
	QString user = url.userName();
	QString pw = url.password();
	qsDesiredChannel = url.path();

	if (user.isEmpty()) {
		bool ok;
		user = g.qs->value(QLatin1String("defUserName")).toString();
		user = QInputDialog::getText(this, tr("Connecting to %1").arg(url.toString()), tr("Enter username"), QLineEdit::Normal, user, &ok);
		if (! ok || user.isEmpty())
			return;

		g.qs->setValue(QLatin1String("defUserName"), user);
	}

	if (g.sh && g.sh->isRunning()) {
		on_ServerDisconnect_triggered();
		g.sh->wait();
	}

	rtLast = MessageServerReject::None;
	qaServerDisconnect->setEnabled(true);
	g.sh->setConnectionInfo(host, port, user, pw);
	g.sh->start(QThread::TimeCriticalPriority);
}

void MainWindow::on_ServerConnect_triggered() {
	ConnectDialog *cd = new ConnectDialog(this);
	int res = cd->exec();

	if (g.sh && g.sh->isRunning() && res == QDialog::Accepted) {
		on_ServerDisconnect_triggered();
		g.sh->wait();
	}

	if (res == QDialog::Accepted) {
		qsDesiredChannel = QString();
		rtLast = MessageServerReject::None;
		qaServerDisconnect->setEnabled(true);
		g.sh->setConnectionInfo(cd->qsServer, cd->iPort, cd->qsUsername, cd->qsPassword);
		g.sh->start(QThread::TimeCriticalPriority);
	}
	delete cd;
}

void MainWindow::on_Reconnect_timeout() {
	g.l->log(Log::ServerDisconnected, tr("Reconnecting."));
	g.sh->start(QThread::TimeCriticalPriority);
}

void MainWindow::on_ServerDisconnect_triggered() {
	if (qtReconnect->isActive())
		qtReconnect->stop();
	if (g.sh && g.sh->isRunning())
		g.sh->disconnect();
}

void MainWindow::on_ServerBanList_triggered() {
	MessageServerBanList msbl;
	msbl.bQuery = true;
	g.sh->sendMessage(&msbl);

	if (banEdit) {
		banEdit->reject();
		delete banEdit;
		banEdit = NULL;
	}
}

void MainWindow::on_ServerInformation_triggered() {

	QSslCipher qsc = g.sh->qscCipher;

	QMessageBox qmb(QMessageBox::Information, tr("Mumble Server Information"),
	                tr("Control channel: %1 ms latency, Encrypted with %3 bit %4<br />"
	                   "Voice channel: %2 ms latency, Encrypted with OCB-AES128"
	                  ).arg(g.sh->uiTCPPing / 1000.0, 0, 'f', 2).arg(g.sh->uiUDPPing / 1000.0, 0, 'f', 2).arg(qsc.usedBits()).arg(qsc.name()), QMessageBox::Ok, this);

	qmb.setDefaultButton(QMessageBox::Ok);
	qmb.setEscapeButton(QMessageBox::Ok);

	QPushButton *qp = qmb.addButton(tr("&View Certificate"), QMessageBox::ActionRole);
	int res = qmb.exec();
	if ((res == 0) && (qmb.clickedButton() == qp)) {
		ViewCert vc(g.sh->qscCert, this);
		vc.exec();
	}
}

void MainWindow::on_PlayerMenu_aboutToShow() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (! p) {
		qaPlayerKick->setEnabled(false);
		qaPlayerBan->setEnabled(false);
		qaPlayerMute->setEnabled(false);
		qaPlayerLocalMute->setEnabled(false);
		qaPlayerDeaf->setEnabled(false);
		qaPlayerTextMessage->setEnabled(false);
	} else {
		qaPlayerKick->setEnabled(true);
		qaPlayerBan->setEnabled(true);
		qaPlayerMute->setEnabled(true);
		qaPlayerLocalMute->setEnabled(true);
		qaPlayerDeaf->setEnabled(true);
		qaPlayerMute->setChecked(p->bMute);
		qaPlayerLocalMute->setChecked(p->bLocalMute);
		qaPlayerDeaf->setChecked(p->bDeaf);
		qaPlayerTextMessage->setEnabled(true);
	}
}

void MainWindow::on_PlayerMute_triggered() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	MessagePlayerMute mpmMsg;
	mpmMsg.uiVictim = p->uiSession;
	mpmMsg.bMute = ! p->bMute;
	g.sh->sendMessage(&mpmMsg);
}

void MainWindow::on_PlayerLocalMute_triggered() {
	ClientPlayer *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	p->setLocalMute(qaPlayerLocalMute->isChecked());
}

void MainWindow::on_PlayerDeaf_triggered() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	MessagePlayerDeaf mpdMsg;
	mpdMsg.uiVictim = p->uiSession;
	mpdMsg.bDeaf = ! p->bDeaf;
	g.sh->sendMessage(&mpdMsg);
}

void MainWindow::on_PlayerKick_triggered() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	short session = p->uiSession;

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

void MainWindow::on_PlayerBan_triggered() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	short session = p->uiSession;

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

void MainWindow::on_PlayerTextMessage_triggered() {
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());

	if (!p)
		return;

	short session = p->uiSession;

	bool ok;
	QString message = QInputDialog::getText(this, tr("Sending message to %1").arg(p->qsName), tr("Enter message"), QLineEdit::Normal, QString(), &ok);
	p = ClientPlayer::get(session);
	if (!p)
		return;

	if (ok) {
		MessageTextMessage mtxt;
		mtxt.uiVictim = p->uiSession;
		mtxt.qsMessage = message;
		g.l->log(Log::TextMessage, tr("To %1: %2").arg(p->qsName).arg(mtxt.qsMessage), tr("Message to %1").arg(p->qsName));
		g.sh->sendMessage(&mtxt);
	}
}

void MainWindow::on_Quit_triggered() {
	qApp->closeAllWindows();
}

void MainWindow::on_ChannelMenu_aboutToShow() {
	QModelIndex idx = qtvPlayers->currentIndex();

	bool add, remove, acl, rename, link, unlink, unlinkall;

	add = remove = acl = rename = link = unlink = unlinkall = false;

	if (g.uiSession != 0) {
		add = true;
		acl = true;

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
	qaChannelLink->setEnabled(link);
	qaChannelUnlink->setEnabled(unlink);
	qaChannelUnlinkAll->setEnabled(unlinkall);
}

void MainWindow::on_ChannelAdd_triggered() {
	bool ok;
	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());
	int iParent = c ? c->iId : 0;
	QString name = QInputDialog::getText(this, tr("Mumble"), tr("Channel Name"), QLineEdit::Normal, QString(), &ok);

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

void MainWindow::on_ChannelRemove_triggered() {
	int ret;
	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());
	if (! c)
		return;

	int id = c->iId;

	ret=QMessageBox::question(this, tr("Mumble"), tr("Are you sure you want to delete %1 and all its sub-channels?").arg(c->qsName), QMessageBox::Yes, QMessageBox::No);

	c = Channel::get(id);
	if (!c)
		return;

	if (ret == QMessageBox::Yes) {
		MessageChannelRemove mcr;
		mcr.iId = c->iId;
		g.sh->sendMessage(&mcr);
	}
}

void MainWindow::on_ChannelRename_triggered() {
	bool ok;
	Channel *c = pmModel->getChannel(qtvPlayers->currentIndex());
	if (! c)
		return;

	int id = c->iId;

	QString name = QInputDialog::getText(this, tr("Mumble"), tr("Channel Name"), QLineEdit::Normal, c->qsName, &ok);

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

void MainWindow::on_ChannelACL_triggered() {
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

void MainWindow::on_ChannelLink_triggered() {
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

void MainWindow::on_ChannelUnlink_triggered() {
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

void MainWindow::on_ChannelUnlinkAll_triggered() {
	Channel *c = ClientPlayer::get(g.uiSession)->cChannel;

	MessageChannelLink mcl;
	mcl.iId = c->iId;
	mcl.ltType = MessageChannelLink::UnlinkAll;
	g.sh->sendMessage(&mcl);
}

void MainWindow::on_AudioReset_triggered() {
	AudioInputPtr ai = g.ai;
	if (ai)
		ai->bResetProcessor = true;
}

void MainWindow::on_AudioMute_triggered() {
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
}

void MainWindow::on_AudioDeaf_triggered() {
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
}

void MainWindow::on_AudioTextToSpeech_triggered() {
	g.s.bTTS = qaAudioTTS->isChecked();
}

void MainWindow::on_AudioStats_triggered() {
	AudioStats *as=new AudioStats(this);
	as->show();
}

void MainWindow::on_AudioUnlink_triggered() {
	g.p->bUnlink = true;
}

void MainWindow::on_ConfigDialog_triggered() {
	ConfigDialog dlg;
	dlg.exec();
}

void MainWindow::on_AudioWizard_triggered() {
#ifdef Q_OS_WIN
	AudioWizard aw;
	aw.exec();
#endif
}

void MainWindow::on_HelpWhatsThis_triggered() {
	QWhatsThis::enterWhatsThisMode();
}

void MainWindow::on_HelpAbout_triggered() {
	AboutDialog adAbout(this);
	adAbout.exec();
}

void MainWindow::on_HelpAboutSpeex_triggered() {
	AboutSpeexDialog adAbout(this);
	adAbout.exec();
}

void MainWindow::on_HelpAboutQt_triggered() {
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_HelpVersionCheck_triggered() {
	new VersionCheck(this);
}

void MainWindow::on_PushToTalk_triggered(bool down) {
	if (down)
		g.iPushToTalk++;
	else if (g.iPushToTalk)
		g.iPushToTalk--;
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
	int idx = gs->data().toInt();
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
	g.l->log(Log::ServerConnected, tr("Connected to server."));
	qaServerDisconnect->setEnabled(true);
	qaServerInformation->setEnabled(true);
	qaServerBanList->setEnabled(true);

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

	QString uname, pw, host;
	int port;
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

	pmModel->removeAll();

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

		switch (rtLast) {
			case MessageServerReject::InvalidUsername:
			case MessageServerReject::UsernameInUse:
				matched = true;
				uname = QInputDialog::getText(this, tr("Invalid username"), (rtLast == MessageServerReject::InvalidUsername) ? tr("You connected with an invalid username, please try another one.") : tr("That username is already in use, please try another username."), QLineEdit::Normal, uname, &ok);
				break;
			case MessageServerReject::WrongUserPW:
			case MessageServerReject::WrongServerPW:
				matched = true;
				pw = QInputDialog::getText(this, tr("Wrong password"), (rtLast == MessageServerReject::WrongUserPW) ? tr("Wrong password for registered users, please try again.") : tr("Wrong server password for unregistered user account, please try again."), QLineEdit::Password, pw, &ok);
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

void MainWindow::on_Icon_activated(QSystemTrayIcon::ActivationReason) {
	if (! isVisible()) {
		show();
		showNormal();
	}
	activateWindow();
}


void MainWindow::customEvent(QEvent *evt) {
	if (evt->type() == TI_QEVENT) {
		hide();
		return;
	}
	if (evt->type() == MB_QEVENT) {
		MessageBoxEvent *mbe=static_cast<MessageBoxEvent *>(evt);
		QMessageBox::warning(NULL, tr("Mumble"), mbe->msg, QMessageBox::Ok, QMessageBox::NoButton);
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
