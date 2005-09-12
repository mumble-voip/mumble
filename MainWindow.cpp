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
#include "MainWindow.h"
#include "AudioInput.h"
#include "ConnectDialog.h"
#include "Player.h"
#include "Connection.h"
#include "ServerHandler.h"
#include "About.h"
#include "GlobalShortcut.h"
#include "TextToSpeech.h"
#include "VersionCheck.h"
#include "PlayerModel.h"
#include "AudioStats.h"
#include "Global.h"

MainWindow::MainWindow(QWidget *p) : QMainWindow(p) {
	setupGui();

	sMyId = 0;

	connect(g.sh, SIGNAL(connected()), this, SLOT(serverConnected()));
	connect(g.sh, SIGNAL(disconnected(QString)), this, SLOT(serverDisconnected(QString)));

	tts=new TextToSpeech(this);
	recheckTTS();
	log(tr("Welcome to Mumble."));

	new VersionCheck(this);
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

	qmServer = new QMenu(tr("&Server"), this);
	qmPlayer = new QMenu(tr("&Player"), this);
	qmAudio = new QMenu(tr("&Audio"), this);
	qmConfig = new QMenu(tr("&Config"), this);
	qmHelp = new QMenu(tr("&Help"), this);

	qmServer->setObjectName("ServerMenu");
	qmPlayer->setObjectName("PlayerMenu");
	qmAudio->setObjectName("AudioMenu");
	qmHelp->setObjectName("HelpMenu");

	qaServerConnect=new QAction(tr("&Connect"), this);
	qaServerDisconnect=new QAction(tr("&Disconnect"), this);
	qaServerConnect->setObjectName("ServerConnect");
	qaServerDisconnect->setObjectName("ServerDisconnect");
	qaServerDisconnect->setEnabled(FALSE);

	qmServer->addAction(qaServerConnect);
	qmServer->addAction(qaServerDisconnect);

	qaPlayerKick=new QAction(tr("&Kick"), this);
	qaPlayerMute=new QAction(tr("&Mute"), this);
	qaPlayerDeaf=new QAction(tr("&Deafen"), this);
	qaPlayerKick->setObjectName("PlayerKick");
	qaPlayerMute->setObjectName("PlayerMute");
	qaPlayerDeaf->setObjectName("PlayerDeaf");
	qaPlayerKick->setEnabled(FALSE);
	qaPlayerMute->setCheckable(TRUE);
	qaPlayerMute->setEnabled(FALSE);
	qaPlayerDeaf->setCheckable(TRUE);
	qaPlayerDeaf->setEnabled(FALSE);

	qmPlayer->addAction(qaPlayerKick);
	qmPlayer->addAction(qaPlayerMute);
	qmPlayer->addAction(qaPlayerDeaf);

	qaAudioReset=new QAction(tr("&Reset"), this);
	qaAudioReset->setObjectName("AudioReset");
	qaAudioMute=new QAction(tr("&Mute"), this);
	qaAudioMute->setObjectName("AudioMute");
	qaAudioDeaf=new QAction(tr("&Deaf"), this);
	qaAudioDeaf->setObjectName("AudioDeaf");
	qaAudioTTS=new QAction(tr("&Text-To-Speech"), this);
	qaAudioTTS->setObjectName("AudioTextToSpeech");
	qaAudioStats=new QAction(tr("S&tatistics"), this);
	qaAudioStats->setObjectName("AudioStats");
	qaAudioMute->setCheckable(true);
	qaAudioDeaf->setCheckable(true);
	qaAudioTTS->setCheckable(true);
	qaAudioMute->setChecked(g.s.bMute);
	qaAudioDeaf->setChecked(g.s.bDeaf);
	qaAudioTTS->setChecked(qs.value("TextToSpeech", true).toBool());

	qmAudio->addAction(qaAudioMute);
	qmAudio->addAction(qaAudioDeaf);
	qmAudio->addSeparator();
	qmAudio->addAction(qaAudioReset);
	qmAudio->addSeparator();
	qmAudio->addAction(qaAudioTTS);
	qmAudio->addSeparator();
	qmAudio->addAction(qaAudioStats);

	qaConfigDialog=new QAction(tr("&Settings"), this);
	qaConfigDialog->setObjectName("ConfigDialog");
	qaConfigShortcuts=new QAction(tr("&Shortcuts"), this);
	qaConfigShortcuts->setObjectName("ConfigShortcuts");

	qmConfig->addAction(qaConfigDialog);
	qmConfig->addAction(qaConfigShortcuts);

	qaHelpAbout=new QAction(tr("&About"), this);
	qaHelpAbout->setObjectName("HelpAbout");
	qaHelpAboutQt=new QAction(tr("&About QT"), this);
	qaHelpAboutQt->setObjectName("HelpAboutQt");

	qmHelp->addAction(qaHelpAbout);
	qmHelp->addAction(qaHelpAboutQt);

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
	g.s.save();
	MainWindow::closeEvent(e);
}

void MainWindow::recheckTTS()
{
	tts->setEnabled(qaAudioTTS->isChecked());
}

void MainWindow::log(QString entry, QString phonetic, bool maytts)
{
	QTime now = QTime::currentTime();
	if (maytts)
		tts->say(phonetic.isNull() ? entry : phonetic);
	if (entry.isNull())
		return;
	qteLog->append(tr("[%1] %2").arg(now.toString(Qt::LocalDate)).arg(entry));
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
	if (g.ai)
		g.ai->bResetProcessor = true;
}

void MainWindow::on_AudioMute_triggered()
{
	g.s.bMute = qaAudioMute->isChecked();
	if (! g.s.bMute && g.s.bDeaf) {
		g.s.bDeaf = false;
		qaAudioDeaf->setChecked(false);
		log(QString(), tr("Un-muted and undeafened"));
	} else if (! g.s.bMute) {
		log(QString(), tr("Unmuted"));
	} else {
		log(QString(), tr("Muted"));
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
		log(QString(), tr("Muted and deafened"));
	} else if (g.s.bDeaf) {
		log(QString(), tr("Deafened"));
	} else {
		log(QString(), tr("Undeafened"));
	}

	MessagePlayerSelfMuteDeaf mpsmd;
	mpsmd.bMute = g.s.bMute;
	mpsmd.bDeaf = g.s.bDeaf;
	g.sh->sendMessage(&mpsmd);
}

void MainWindow::on_AudioTextToSpeech_triggered()
{
	qs.setValue("TextToSpeech", qaAudioTTS->isChecked());
	recheckTTS();
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

void MainWindow::on_HelpAbout_triggered()
{
	AboutDialog adAbout(this);
	adAbout.exec();
}

void MainWindow::on_HelpAboutQt_triggered()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_PushToTalk_triggered(bool down)
{
	g.bPushToTalk = down;
}

void MainWindow::serverConnected()
{
	log(tr("Connected to server"));
	tts->setEnabled(false);
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
	recheckTTS();
	qaServerConnect->setEnabled(true);
	qaServerDisconnect->setEnabled(false);

	pmModel->removeAllPlayers();

	if (! reason.isEmpty()) {
  	  log(tr("Server connection failed: %1").arg(reason));
    } else {
	  log(tr("Disconnected from server."));
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
	g.mw->log(MainWindow::tr("Joined now: %1").arg(p->qsName));
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

	g.mw->log(MainWindow::tr("Left now: %1").arg(pSrc->qsName));
	g.mw->pmModel->removePlayer(pSrc);
}

void MessageSpeex::process(Connection *) {
}

void MessagePlayerSelfMuteDeaf::process(Connection *) {
	MSG_INIT;
	pSrc->setSelfMuteDeaf(bMute, bDeaf);
}

void MessagePlayerMute::process(Connection *) {
	MSG_INIT;
	VICTIM_INIT;

	pDst->setMute(bMute);

	QString vic = pDst->qsName;
	QString admin = pSrc->qsName;

	if (sVictim == g.mw->sMyId)
		g.mw->log(bMute ? MainWindow::tr("You were muted by %1").arg(admin) : MainWindow::tr("You were unmuted by %1").arg(admin));
	else
		g.mw->log(bMute ? MainWindow::tr("%1 muted by %2").arg(vic).arg(admin) : MainWindow::tr("%1 unmuted by %2").arg(vic).arg(admin), QString());
}

void MessagePlayerDeaf::process(Connection *) {
	MSG_INIT;
	VICTIM_INIT;

	pDst->setDeaf(bDeaf);

	QString vic = pDst->qsName;
	QString admin = pSrc->qsName;

	if (sVictim == g.mw->sMyId)
		g.mw->log(bDeaf ? MainWindow::tr("You were deafened by %1").arg(admin) : MainWindow::tr("You were undeafened by %1").arg(admin));
	else
		g.mw->log(bDeaf ? MainWindow::tr("%1 defened by %2").arg(vic).arg(admin) : MainWindow::tr("%1 undeafened by %2").arg(vic).arg(admin), QString());
}

void MessagePlayerKick::process(Connection *) {
	MSG_INIT;
	VICTIM_INIT;
	if (sVictim == g.mw->sMyId)
		g.mw->log(MainWindow::tr("You were kicked from the server by %1: %2").arg(pSrc->qsName).arg(qsReason));
	else
		g.mw->log(MainWindow::tr("%3 was kicked from the server by %1: %2").arg(pSrc->qsName).arg(qsReason).arg(pDst->qsName));
}

void MessageServerAuthenticate::process(Connection *) {
}

void MessageServerReject::process(Connection *) {
	g.mw->log(MainWindow::tr("Server connection rejected: %1").arg(qsReason));
}

void MessageServerSync::process(Connection *) {
	MSG_INIT;
	g.mw->sMyId = sPlayerId;
	g.mw->recheckTTS();
}
