/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#include <QMenuBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QSplitter>
#include <QTextCursor>
#include <QTime>
#include <QHeaderView>
#include <QListView>
#include <QTreeView>
#include <QWhatsThis>
#include "MainWindow.h"
#include "AudioInput.h"
#include "ConnectDialog.h"
#include "Player.h"
#include "Connection.h"
#include "ServerHandler.h"
#include "About.h"
#include "GlobalShortcut.h"
#include "VersionCheck.h"
#include "PlayerModel.h"
#include "AudioStats.h"
#include "Log.h"
#include "Global.h"

MainWindow::MainWindow(QWidget *p) : QMainWindow(p) {
	createActions();
	setupGui();

	sMyId = 0;

	connect(g.sh, SIGNAL(connected()), this, SLOT(serverConnected()));
	connect(g.sh, SIGNAL(disconnected(QString)), this, SLOT(serverDisconnected(QString)));
}

void MainWindow::createActions() {
	qaServerConnect=new QAction(tr("&Connect"), this);
	qaServerConnect->setToolTip(tr("Open the server connection dialog"));
	qaServerConnect->setWhatsThis(tr("Shows a dialog of registered servers, and also allows quick connect."));
	qaServerConnect->setObjectName("ServerConnect");
	qaServerDisconnect=new QAction(tr("&Disconnect"), this);
	qaServerDisconnect->setToolTip(tr("Disconnect from server"));
	qaServerDisconnect->setWhatsThis(tr("Disconnects you from the server"));
	qaServerDisconnect->setObjectName("ServerDisconnect");
	qaServerDisconnect->setEnabled(false);

	qaPlayerKick=new QAction(tr("&Kick"), this);
	qaPlayerKick->setObjectName("PlayerKick");
	qaPlayerKick->setToolTip(tr("Kick player (with reason)"));
	qaPlayerKick->setWhatsThis(tr("Kick selected player off server. You'll be asked to specify a reason."));
	qaPlayerMute=new QAction(tr("&Mute"), this);
	qaPlayerMute->setObjectName("PlayerMute");
	qaPlayerMute->setCheckable(true);
	qaPlayerMute->setToolTip(tr("Mute player"));
	qaPlayerMute->setWhatsThis(tr("Mute or unmute player on server. Unmuting a deafened player will also undeafen them."));
	qaPlayerDeaf=new QAction(tr("&Deafen"), this);
	qaPlayerDeaf->setObjectName("PlayerDeaf");
	qaPlayerDeaf->setCheckable(true);
	qaPlayerDeaf->setToolTip(tr("Deafen player"));
	qaPlayerDeaf->setWhatsThis(tr("Deafen or undeafen player on server. Deafening a player will also mute them."));


	qaAudioReset=new QAction(tr("&Reset"), this);
	qaAudioReset->setObjectName("AudioReset");
	qaAudioReset->setToolTip(tr("Reset audio preprocessor"));
	qaAudioReset->setWhatsThis(tr("Resets the audio preprocessor, including noice cancellation, automatic gain and voice activity detection. "
									"If something suddenly worsens the audio environment (like dropping the microphone) and it was temporary, "
									"use this to avoid having to wait for the preprocessor to readjust."));
	qaAudioMute=new QAction(tr("&Mute"), this);
	qaAudioMute->setObjectName("AudioMute");
	qaAudioMute->setCheckable(true);
	qaAudioMute->setChecked(g.s.bMute);
	qaAudioMute->setToolTip(tr("Mute yourself"));
	qaAudioMute->setWhatsThis(tr("Mute or unmute yourself. When muted, you will not send any data to the server. Unmuting while deafned will also undeafen."));
	qaAudioDeaf=new QAction(tr("&Deaf"), this);
	qaAudioDeaf->setObjectName("AudioDeaf");
	qaAudioDeaf->setCheckable(true);
	qaAudioDeaf->setChecked(g.s.bDeaf);
	qaAudioDeaf->setToolTip(tr("Deafen yourself"));
	qaAudioDeaf->setWhatsThis(tr("Deafen or undeafen yourself. When deafened, you will not hear anything. Deafining yourself will also mute."));
	qaAudioTTS=new QAction(tr("&Text-To-Speech"), this);
	qaAudioTTS->setObjectName("AudioTextToSpeech");
	qaAudioTTS->setCheckable(true);
	qaAudioTTS->setChecked(qs.value("TextToSpeech", true).toBool());
	qaAudioTTS->setToolTip(tr("Toggle Text-To-Speech"));
	qaAudioTTS->setWhatsThis(tr("Enable or disable the text-to-speech engine. Only messages enabled for TTS in the Config dialog will actually be spoken."));
	qaAudioStats=new QAction(tr("S&tatistics"), this);
	qaAudioStats->setObjectName("AudioStats");
	qaAudioStats->setToolTip(tr("Display audio statistics"));
	qaAudioStats->setWhatsThis(tr("Pops up a small dialog with information about your current audio input."));

	qaConfigDialog=new QAction(tr("&Settings"), this);
	qaConfigDialog->setObjectName("ConfigDialog");
	qaConfigDialog->setToolTip(tr("Configure Mumble"));
	qaConfigDialog->setWhatsThis(tr("Allows you to change most settings for Mumble."));
	qaConfigShortcuts=new QAction(tr("&Shortcuts"), this);
	qaConfigShortcuts->setObjectName("ConfigShortcuts");
	qaConfigShortcuts->setToolTip(tr("Configure hotkeys"));
	qaConfigShortcuts->setWhatsThis(tr("Configure global hotkeys. These will work from anywhere on the system, including inside games."));

	qaHelpWhatsThis = new QAction(tr("&Whats This?"), this);
	qaHelpWhatsThis->setObjectName("HelpWhatsThis");
	qaHelpWhatsThis->setToolTip(tr("Enter What's This? mode"));
	qaHelpWhatsThis->setWhatsThis(tr("Click this to enter \"What's This?\" mode. Your cursor will turn into a quesiton mark. Click "
									"on any button, menu choice or area to show a description of what it is."));
	qaHelpAbout=new QAction(tr("&About"), this);
	qaHelpAbout->setObjectName("HelpAbout");
	qaHelpAbout->setToolTip(tr("Information about Mumble"));
	qaHelpAbout->setWhatsThis(tr("Shows a small dialog with information and license for Mumble"));
	qaHelpAboutQt=new QAction(tr("&About QT"), this);
	qaHelpAboutQt->setObjectName("HelpAboutQt");
	qaHelpAboutQt->setToolTip(tr("Information about Qt"));
	qaHelpAboutQt->setWhatsThis(tr("Shows a small dialog with information about Qt"));
	qaHelpVersionCheck=new QAction(tr("Check &Updates"), this);
	qaHelpVersionCheck->setObjectName("HelpVersionCheck");
	qaHelpVersionCheck->setToolTip(tr("Check for new version of Mumble"));
	qaHelpVersionCheck->setWhatsThis(tr("Connects to the Mumble webpage and sees if a new version is available for download. "
										"No download will be done; you'll be notified of where to get the new version."));
}

void MainWindow::setupGui()  {
	QMenu *qmServer, *qmPlayer, *qmAudio, *qmConfig, *qmHelp;

	setWindowTitle(tr("Mumble -- %1").arg(QString(MUMBLE_RELEASE)));

	QTreeView *view = new QTreeView(this);
	qtvPlayers = view;

	pmModel = new PlayerModel(this);
	view->setModel(pmModel);
	view->setItemDelegate(new PlayerDelegate(view));

	qteLog = new QTextEdit(this);
	qteLog->setReadOnly(true);
	qteLog->setToolTip(tr("Log of messages"));
	qteLog->setWhatsThis(tr("This shows all recent activity. Connecting to servers, errors, information messasges all show up here.<br />"
							"To configure exactly which messages show up here, use the <b>Settings</b> command from the menu."));

	qmServer = new QMenu(tr("&Server"), this);
	qmPlayer = new QMenu(tr("&Player"), this);
	qmAudio = new QMenu(tr("&Audio"), this);
	qmConfig = new QMenu(tr("&Config"), this);
	qmHelp = new QMenu(tr("&Help"), this);

	qmServer->setObjectName("ServerMenu");
	qmPlayer->setObjectName("PlayerMenu");
	qmAudio->setObjectName("AudioMenu");
	qmHelp->setObjectName("HelpMenu");

	qmServer->addAction(qaServerConnect);
	qmServer->addAction(qaServerDisconnect);

	qmPlayer->addAction(qaPlayerKick);
	qmPlayer->addAction(qaPlayerMute);
	qmPlayer->addAction(qaPlayerDeaf);

	qmAudio->addAction(qaAudioMute);
	qmAudio->addAction(qaAudioDeaf);
	qmAudio->addSeparator();
	qmAudio->addAction(qaAudioReset);
	qmAudio->addSeparator();
	qmAudio->addAction(qaAudioTTS);
	qmAudio->addSeparator();
	qmAudio->addAction(qaAudioStats);

	qmConfig->addAction(qaConfigDialog);
	qmConfig->addAction(qaConfigShortcuts);

	qmHelp->addAction(qaHelpWhatsThis);
	qmHelp->addSeparator();
	qmHelp->addAction(qaHelpAbout);
	qmHelp->addAction(qaHelpAboutQt);
	qmHelp->addSeparator();
	qmHelp->addAction(qaHelpVersionCheck);

	menuBar()->addMenu(qmServer);
	menuBar()->addMenu(qmPlayer);
	menuBar()->addMenu(qmAudio);
	menuBar()->addMenu(qmConfig);
	menuBar()->addMenu(qmHelp);

	gsPushTalk=new GlobalShortcut(this, 1, "Push-to-Talk");
	gsPushTalk->setObjectName("PushToTalk");

	gsResetAudio=new GlobalShortcut(this, 2, "Reset Audio Processor");
	gsResetAudio->setObjectName("ResetAudio");
	connect(gsResetAudio, SIGNAL(down()), qaAudioReset, SLOT(trigger()));

	gsMuteSelf=new GlobalShortcut(this, 3, "Toggle Mute Self");
	gsMuteSelf->setObjectName("MuteSelf");
	connect(gsMuteSelf, SIGNAL(down()), qaAudioMute, SLOT(trigger()));

	gsDeafSelf=new GlobalShortcut(this, 4, "Toggle Deafen Self");
	gsDeafSelf->setObjectName("DeafSelf");
	connect(gsDeafSelf, SIGNAL(down()), qaAudioDeaf, SLOT(trigger()));

	qsSplit = new QSplitter(Qt::Horizontal, this);
	qsSplit->addWidget(qteLog);
	qsSplit->addWidget(qtvPlayers);

	setCentralWidget(qsSplit);

	restoreState(qs.value("mw").toByteArray());
	qsSplit->restoreState(qs.value("mwSplitter").toByteArray());

	QPoint ps = qs.value("mwPos").toPoint();
	if (! ps.isNull())
		move(ps);
	QSize sz = qs.value("mwSize").toSize();
	if (sz.isValid())
		resize(sz);

    QMetaObject::connectSlotsByName(this);
}

void MainWindow::closeEvent(QCloseEvent *e) {
	qs.setValue("mwPos", pos());
	qs.setValue("mwSize", size());
	qs.setValue("mw", saveState());
	qs.setValue("mwSplitter", qsSplit->saveState());
	QMainWindow::closeEvent(e);
}

void MainWindow::appendLog(QString entry)
{
	qteLog->append(entry);
	QTextCursor p=qteLog->textCursor();
	p.movePosition(QTextCursor::End);
	qteLog->setTextCursor(p);
	qteLog->ensureCursorVisible();
}

void MainWindow::on_ServerConnect_triggered()
{
	ConnectDialog *cd = new ConnectDialog(this);
	int res = cd->exec();

	if (res == QDialog::Accepted) {
		qaServerConnect->setEnabled(false);
		qaServerDisconnect->setEnabled(true);
		g.sh->setConnectionInfo(cd->qsServer, cd->iPort, cd->qsUsername, cd->qsPassword);
		g.sh->start(QThread::TimeCriticalPriority);
	}
	delete cd;
}

void MainWindow::on_ServerDisconnect_triggered()
{
	g.sh->disconnect();
}

void MainWindow::on_PlayerMenu_aboutToShow()
{
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (! p) {
		qaPlayerKick->setEnabled(false);
		qaPlayerMute->setEnabled(false);
		qaPlayerDeaf->setEnabled(false);
	} else {
		qaPlayerKick->setEnabled(true);
		qaPlayerMute->setEnabled(true);
		qaPlayerDeaf->setEnabled(true);
		qaPlayerMute->setChecked(p->bMute);
		qaPlayerDeaf->setChecked(p->bDeaf);
	}
}

void MainWindow::on_PlayerMute_triggered()
{
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	MessagePlayerMute mpmMsg;
	mpmMsg.sVictim = p->sId;
	mpmMsg.bMute = ! p->bMute;
	g.sh->sendMessage(&mpmMsg);
}

void MainWindow::on_PlayerDeaf_triggered()
{
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	MessagePlayerDeaf mpdMsg;
	mpdMsg.sVictim = p->sId;
	mpdMsg.bDeaf = ! p->bDeaf;
	g.sh->sendMessage(&mpdMsg);
}

void MainWindow::on_PlayerKick_triggered()
{
	Player *p = pmModel->getPlayer(qtvPlayers->currentIndex());
	if (!p)
		return;

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Kicking player %1").arg(p->qsName), tr("Enter reason"), QLineEdit::Normal, "", &ok);
	if (ok) {
		MessagePlayerKick mpkMsg;
		mpkMsg.sVictim=p->sId;
		mpkMsg.qsReason = reason;
		g.sh->sendMessage(&mpkMsg);
	}
}

void MainWindow::on_AudioReset_triggered()
{
	QReadLocker(&g.qrwlAudio);
	if (g.ai)
		g.ai->bResetProcessor = true;
}

void MainWindow::on_AudioMute_triggered()
{
	g.s.bMute = qaAudioMute->isChecked();
	if (! g.s.bMute && g.s.bDeaf) {
		g.s.bDeaf = false;
		qaAudioDeaf->setChecked(false);
		g.l->log(Log::SelfMute, tr("Un-muted and undeafened"));
	} else if (! g.s.bMute) {
		g.l->log(Log::SelfMute, tr("Unmuted"));
	} else {
		g.l->log(Log::SelfMute, tr("Muted"));
	}

	MessagePlayerSelfMuteDeaf mpsmd;
	mpsmd.bMute = g.s.bMute;
	mpsmd.bDeaf = g.s.bDeaf;
	g.sh->sendMessage(&mpsmd);
}

void MainWindow::on_AudioDeaf_triggered()
{
	g.s.bDeaf = qaAudioDeaf->isChecked();
	if (g.s.bDeaf && ! g.s.bMute) {
		g.s.bMute = true;
		qaAudioMute->setChecked(true);
		g.l->log(Log::SelfMute, tr("Muted and deafened"));
	} else if (g.s.bDeaf) {
		g.l->log(Log::SelfMute, tr("Deafened"));
	} else {
		g.l->log(Log::SelfMute, tr("Undeafened"));
	}

	MessagePlayerSelfMuteDeaf mpsmd;
	mpsmd.bMute = g.s.bMute;
	mpsmd.bDeaf = g.s.bDeaf;
	g.sh->sendMessage(&mpsmd);
}

void MainWindow::on_AudioTextToSpeech_triggered()
{
	g.s.bTTS = qaAudioTTS->isChecked();
}

void MainWindow::on_AudioStats_triggered()
{
	AudioStats *as=new AudioStats(this);
	as->show();
}

void MainWindow::on_ConfigDialog_triggered()
{
	ConfigDialog dlg;
	dlg.exec();
}

void MainWindow::on_ConfigShortcuts_triggered()
{
	GlobalShortcut::configure();
}

void MainWindow::on_HelpWhatsThis_triggered()
{
	QWhatsThis::enterWhatsThisMode();
}

void MainWindow::on_HelpAbout_triggered()
{
	AboutDialog adAbout(this);
	adAbout.exec();
}

void MainWindow::on_HelpAboutQt_triggered()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_HelpVersionCheck_triggered()
{
	new VersionCheck(this);
}

void MainWindow::on_PushToTalk_triggered(bool down)
{
	g.bPushToTalk = down;
}

void MainWindow::serverConnected()
{
	sMyId = 0;
	g.l->clearIgnore();
	g.l->setIgnore(Log::PlayerJoin);
	g.l->setIgnore(Log::OtherSelfMute);
	g.l->log(Log::ServerConnected, tr("Connected to server"));
	qaServerDisconnect->setEnabled(true);

	if (g.s.bMute || g.s.bDeaf) {
		MessagePlayerSelfMuteDeaf mpsmd;
		mpsmd.bMute = g.s.bMute;
		mpsmd.bDeaf = g.s.bDeaf;
		g.sh->sendMessage(&mpsmd);
	}
}

void MainWindow::serverDisconnected(QString reason)
{
	sMyId = 0;
	qaServerConnect->setEnabled(true);
	qaServerDisconnect->setEnabled(false);

	pmModel->removeAllPlayers();

	if (! reason.isEmpty()) {
  	  g.l->log(Log::ServerDisconnected, tr("Server connection failed: %1").arg(reason));
    } else {
	  g.l->log(Log::ServerDisconnected, tr("Disconnected from server."));
	}
}

void MainWindow::customEvent(QEvent *evt) {
	if (evt->type() != SERVERSEND_EVENT)
		return;

	ServerHandlerMessageEvent *shme=static_cast<ServerHandlerMessageEvent *>(evt);

	Message *mMsg = Message::networkToMessage(shme->qbaMsg);
	if (mMsg) {
		mMsg->process(NULL);
		delete mMsg;
	}
}

void MessageServerJoin::process(Connection *) {
	Player *p = g.mw->pmModel->addPlayer(sPlayerId, qsPlayerName);
	g.l->log(Log::PlayerJoin, MainWindow::tr("Joined now: %1").arg(p->qsName));
}

#define MSG_INIT \
	Player *pSrc=Player::get(sPlayerId); \
	if (! pSrc) \
		qFatal("MainWindow: Message for nonexistant player %d", sPlayerId);

#define VICTIM_INIT \
	Player *pDst=Player::get(sVictim); \
	 if (! pDst) \
 		qFatal("MainWindow: Message for nonexistant victim %d", sVictim);

void MessageServerLeave::process(Connection *) {
	MSG_INIT;

	g.l->log(Log::PlayerLeave, MainWindow::tr("Left now: %1").arg(pSrc->qsName));
	g.mw->pmModel->removePlayer(pSrc);
}

void MessageSpeex::process(Connection *) {
}

void MessagePlayerSelfMuteDeaf::process(Connection *) {
	MSG_INIT;

	QString name = pSrc->qsName;
	pSrc->setSelfMuteDeaf(bMute, bDeaf);

	if (sPlayerId != g.mw->sMyId) {
		if (bMute && bDeaf)
			g.l->log(Log::OtherSelfMute, MainWindow::tr("%1 is now muted and deafened.").arg(name));
		else if (bMute)
			g.l->log(Log::OtherSelfMute, MainWindow::tr("%1 is now muted").arg(name));
		else
			g.l->log(Log::OtherSelfMute, MainWindow::tr("%1 is now unmuted").arg(name));
	}
}

void MessagePlayerMute::process(Connection *) {
	MSG_INIT;
	VICTIM_INIT;

	pDst->setMute(bMute);

	QString vic = pDst->qsName;
	QString admin = pSrc->qsName;

	if (sVictim == g.mw->sMyId)
		g.l->log(Log::YouMuted, bMute ? MainWindow::tr("You were muted by %1").arg(admin) : MainWindow::tr("You were unmuted by %1").arg(admin));
	else
		g.l->log((sPlayerId == g.mw->sMyId) ? Log::YouMutedOther : Log::OtherMutedOther, bMute ? MainWindow::tr("%1 muted by %2").arg(vic).arg(admin) : MainWindow::tr("%1 unmuted by %2").arg(vic).arg(admin), QString());
}

void MessagePlayerDeaf::process(Connection *) {
	MSG_INIT;
	VICTIM_INIT;

	pDst->setDeaf(bDeaf);

	QString vic = pDst->qsName;
	QString admin = pSrc->qsName;

	if (sVictim == g.mw->sMyId)
		g.l->log(Log::YouMuted, bDeaf ? MainWindow::tr("You were deafened by %1").arg(admin) : MainWindow::tr("You were undeafened by %1").arg(admin));
	else
		g.l->log((sPlayerId == g.mw->sMyId) ? Log::YouMutedOther : Log::OtherMutedOther, bDeaf ? MainWindow::tr("%1 defened by %2").arg(vic).arg(admin) : MainWindow::tr("%1 undeafened by %2").arg(vic).arg(admin), QString());
}

void MessagePlayerKick::process(Connection *) {
	MSG_INIT;
	VICTIM_INIT;
	if (sVictim == g.mw->sMyId) {
		g.l->log(Log::YouKicked, MainWindow::tr("You were kicked from the server by %1: %2").arg(pSrc->qsName).arg(qsReason));
		g.l->setIgnore(Log::ServerDisconnected, 1);
	} else {
		g.l->setIgnore(Log::PlayerLeave, 1);
		g.l->log((sPlayerId == g.mw->sMyId) ? Log::YouKicked : Log::PlayerKicked, MainWindow::tr("%3 was kicked from the server by %1: %2").arg(pSrc->qsName).arg(qsReason).arg(pDst->qsName));
	}
}

void MessageServerAuthenticate::process(Connection *) {
}

void MessageServerReject::process(Connection *) {
	g.l->log(Log::ServerDisconnected, MainWindow::tr("Server connection rejected: %1").arg(qsReason));
	g.l->setIgnore(Log::ServerDisconnected, 1);
}

void MessageServerSync::process(Connection *) {
	MSG_INIT;
	g.mw->sMyId = sPlayerId;
	g.l->clearIgnore();
}
