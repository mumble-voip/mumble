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
#include "MainWindow.h"
#include "AudioInput.h"
#include "Settings.h"
#include "ConnectDialog.h"
#include "Player.h"
#include "Connection.h"
#include "ServerHandler.h"
#include "About.h"
#include "GlobalShortcut.h"
#include "TextToSpeech.h"
#include "VersionCheck.h"

MainWindow *g_mwMainWindow;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setupGui();

	sMyId = 0;

	connect(g_shServer, SIGNAL(connected()), this, SLOT(serverConnected()));
	connect(g_shServer, SIGNAL(disconnected(QString)), this, SLOT(serverDisconnected(QString)));

	tts=new TextToSpeech(this);
	recheckTTS();
	log(tr("Welcome to Mumble."));

	new VersionCheck(this);
}

void MainWindow::setupGui()  {
	QMenu *qmServer, *qmPlayer, *qmAudio, *qmHelp;
	QAction *qa;
	QActionGroup *qag;
	QMenu *qm;

	setWindowTitle(tr("Mumble -- %1").arg(QString(MUMBLE_RELEASE)));

	qlwPlayers = new QListWidget(this);
	qteLog = new QTextEdit(this);
	qteLog->setReadOnly(true);

	qmServer = new QMenu(tr("&Server"), this);
	qmPlayer = new QMenu(tr("&Player"), this);
	qmAudio = new QMenu(tr("&Audio"), this);
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
	qaAudioShortcuts=new QAction(tr("&Shortcuts"), this);
	qaAudioShortcuts->setObjectName("AudioShortcuts");
	qaAudioTTS=new QAction(tr("&Text-To-Speech"), this);
	qaAudioTTS->setObjectName("AudioTextToSpeech");
	qaAudioMute->setCheckable(true);
	qaAudioDeaf->setCheckable(true);
	qaAudioTTS->setCheckable(true);
	g_s.bMute = qs.value("AudioMute", false). toBool();
	g_s.bDeaf = qs.value("AudioDeaf", false). toBool();
	qaAudioMute->setChecked(g_s.bMute);
	qaAudioDeaf->setChecked(g_s.bDeaf);
	qaAudioTTS->setChecked(qs.value("TextToSpeech", true).toBool());

	g_s.atTransmit = static_cast<Settings::AudioTransmit>(qs.value("AudioTransmit", Settings::VAD).toInt());
	qag=new QActionGroup(this);
	qag->setObjectName("AudioTransmit");
	qag->setExclusive(true);
	qm=new QMenu(tr("&Transmit"), this);

	qa=new QAction(tr("Continous"), qag);
	qa->setCheckable(true);
	qa->setData(Settings::Continous);
	if (g_s.atTransmit == Settings::Continous)
		qa->setChecked(true);
	qm->addAction(qa);

	qa=new QAction(tr("Voice Activity"), qag);
	qa->setCheckable(true);
	qa->setData(Settings::VAD);
	if (g_s.atTransmit == Settings::VAD)
		qa->setChecked(true);
	qm->addAction(qa);

	qa=new QAction(tr("Push To Talk"), qag);
	qa->setCheckable(true);
	qa->setData(Settings::PushToTalk);
	if (g_s.atTransmit == Settings::PushToTalk)
		qa->setChecked(true);
	qm->addAction(qa);

	qmAudio->addAction(qaAudioMute);
	qmAudio->addAction(qaAudioDeaf);
	qmAudio->addSeparator();
	qmAudio->addAction(qaAudioReset);
	qmAudio->addAction(qaAudioShortcuts);
	qmAudio->addAction(qaAudioTTS);
	qmAudio->addMenu(qm);

	qaHelpAbout=new QAction(tr("&About"), this);
	qaHelpAbout->setObjectName("HelpAbout");
	qaHelpAboutQt=new QAction(tr("&About QT"), this);
	qaHelpAboutQt->setObjectName("HelpAboutQt");

	qmHelp->addAction(qaHelpAbout);
	qmHelp->addAction(qaHelpAboutQt);

	menuBar()->addMenu(qmServer);
	menuBar()->addMenu(qmPlayer);
	menuBar()->addMenu(qmAudio);
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

    QMetaObject::connectSlotsByName(this);

	QSplitter *qs = new QSplitter(Qt::Horizontal, this);
	qs->addWidget(qteLog);
	qs->addWidget(qlwPlayers);

	setCentralWidget(qs);
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
	QTextCursor pos=qteLog->textCursor();
	pos.movePosition(QTextCursor::End);
	qteLog->setTextCursor(pos);
	qteLog->ensureCursorVisible();
}

void MainWindow::on_ServerConnect_triggered()
{
	ConnectDialog *cd = new ConnectDialog(this);
	int res = cd->exec();

	if (res == QDialog::Accepted) {
		qaServerConnect->setEnabled(false);
		qaServerDisconnect->setEnabled(true);
		g_shServer->setConnectionInfo(cd->qsServer, cd->iPort, cd->qsUsername, cd->qsPassword);
		g_shServer->start();
	}
	delete cd;
}

void MainWindow::on_ServerDisconnect_triggered()
{
	g_shServer->disconnect();
}

void MainWindow::on_PlayerMenu_aboutToShow()
{
	QListWidgetItem *item = qlwPlayers->currentItem();
	if (! item) {
		qaPlayerKick->setEnabled(false);
		qaPlayerMute->setEnabled(false);
		qaPlayerDeaf->setEnabled(false);
	} else {
		Player *p = qmPlayers[item];
		qaPlayerKick->setEnabled(true);
		qaPlayerMute->setEnabled(true);
		qaPlayerDeaf->setEnabled(true);
		qaPlayerMute->setChecked(p->bMute);
		qaPlayerDeaf->setChecked(p->bDeaf);
	}
}

void MainWindow::on_PlayerMute_triggered()
{
	QListWidgetItem *item = qlwPlayers->currentItem();
	if (! item)
		return;
	Player *p = qmPlayers[item];
	MessagePlayerMute mpmMsg;
	mpmMsg.sVictim = p->sId;
	mpmMsg.bMute = ! p->bMute;
	g_shServer->sendMessage(&mpmMsg);
}

void MainWindow::on_PlayerDeaf_triggered()
{
	QListWidgetItem *item = qlwPlayers->currentItem();
	if (! item)
		return;
	Player *p = qmPlayers[item];
	MessagePlayerDeaf mpdMsg;
	mpdMsg.sVictim = p->sId;
	mpdMsg.bDeaf = ! p->bDeaf;
	g_shServer->sendMessage(&mpdMsg);
}

void MainWindow::on_PlayerKick_triggered()
{
	QListWidgetItem *item = qlwPlayers->currentItem();
	if (! item)
		return;
	Player *p = qmPlayers[item];

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Kicking player %1").arg(p->qsName), tr("Enter reason"), QLineEdit::Normal, "", &ok);
	if (ok) {
		MessagePlayerKick mpkMsg;
		mpkMsg.sVictim=p->sId;
		mpkMsg.qsReason = reason;
		g_shServer->sendMessage(&mpkMsg);
	}
}

void MainWindow::on_AudioReset_triggered()
{
	if (g_aiInput)
		g_aiInput->bResetProcessor = true;
}

void MainWindow::on_AudioShortcuts_triggered()
{
	GlobalShortcut::configure();
}

void MainWindow::on_AudioMute_triggered()
{
	g_s.bMute = qaAudioMute->isChecked();
	if (! g_s.bMute && g_s.bDeaf) {
		g_s.bDeaf = false;
		qaAudioDeaf->setChecked(false);
		log(QString(), tr("Un-muted and undeafened"));
	} else if (! g_s.bMute) {
		log(QString(), tr("Unmuted"));
	} else {
		log(QString(), tr("Muted"));
	}

	MessagePlayerSelfMuteDeaf mpsmd;
	mpsmd.bMute = g_s.bMute;
	mpsmd.bDeaf = g_s.bDeaf;
	g_shServer->sendMessage(&mpsmd);

	qs.setValue("AudioMute", g_s.bMute);
	qs.setValue("AudioDeaf", g_s.bDeaf);
}

void MainWindow::on_AudioDeaf_triggered()
{
	g_s.bDeaf = qaAudioDeaf->isChecked();
	if (g_s.bDeaf && ! g_s.bMute) {
		g_s.bMute = true;
		qaAudioMute->setChecked(true);
		log(QString(), tr("Muted and deafened"));
	} else if (g_s.bDeaf) {
		log(QString(), tr("Deafened"));
	} else {
		log(QString(), tr("Undeafened"));
	}

	MessagePlayerSelfMuteDeaf mpsmd;
	mpsmd.bMute = g_s.bMute;
	mpsmd.bDeaf = g_s.bDeaf;
	g_shServer->sendMessage(&mpsmd);

	qs.setValue("AudioMute", g_s.bMute);
	qs.setValue("AudioDeaf", g_s.bDeaf);
}

void MainWindow::on_AudioTextToSpeech_triggered()
{
	qs.setValue("TextToSpeech", qaAudioTTS->isChecked());
	recheckTTS();
}

void MainWindow::on_AudioTransmit_triggered(QAction *act)
{
	g_s.atTransmit = static_cast<Settings::AudioTransmit>(act->data().toInt());
	qs.setValue("AudioTransmit", g_s.atTransmit);
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
	g_s.bPushToTalk = down;
}

void MainWindow::playerTalkingChanged(Player *p, bool bTalking)
{
	QListWidgetItem *item=qmItems[p];
	item->setBackgroundColor(bTalking ? Qt::lightGray : Qt::white);
}

void MainWindow::serverConnected()
{
	log(tr("Connected to server"));
	tts->setEnabled(false);
	qaServerDisconnect->setEnabled(true);

	if (g_s.bMute || g_s.bDeaf) {
		MessagePlayerSelfMuteDeaf mpsmd;
		mpsmd.bMute = g_s.bMute;
		mpsmd.bDeaf = g_s.bDeaf;
		g_shServer->sendMessage(&mpsmd);
	}
}

void MainWindow::serverDisconnected(QString reason)
{
	sMyId = 0;
	recheckTTS();
	qaServerConnect->setEnabled(true);
	qaServerDisconnect->setEnabled(false);
	QMapIterator<Player *, QListWidgetItem *> iItems(qmItems);
	while (iItems.hasNext()) {
		iItems.next();
		delete iItems.value();
	}
	qmItems.clear();
	qmPlayers.clear();

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

void MainWindow::setItemColor(QListWidgetItem *item, Player *p) {
	if (p->bMute || p->bSelfMute) {
		if (p->bDeaf || p->bSelfDeaf)
			item->setTextColor(Qt::blue);
		else
			item->setTextColor(Qt::yellow);
	} else if (p->bDeaf || p->bSelfDeaf) {
		item->setTextColor(Qt::magenta);
	} else {
		item->setTextColor(Qt::black);
	}
}

void MessageServerJoin::process(Connection *) {
	QListWidgetItem *item = new QListWidgetItem(qsPlayerName, g_mwMainWindow->qlwPlayers);
	Player *p = Player::add(sPlayerId);
	p->qsName = qsPlayerName;
	p->sId = sPlayerId;

	item->setData(Qt::UserRole, p);

	g_mwMainWindow->qmPlayers[item]=p;
	g_mwMainWindow->qmItems[p]=item;

	QObject::connect(p, SIGNAL(talkingChanged(Player *, bool)), g_mwMainWindow, SLOT(playerTalkingChanged(Player *, bool)));

	g_mwMainWindow->log(MainWindow::tr("Joined now: %1").arg(p->qsName));
}

#define MSG_INIT \
	Player *pSrc=Player::get(sPlayerId); \
	if (! pSrc) \
		qFatal("MainWindow: Message for nonexistant player %d", sPlayerId); \
	QListWidgetItem *iSrc=g_mwMainWindow->qmItems[pSrc]; \
	Q_UNUSED(iSrc)

#define VICTIM_INIT \
	Player *pDst=Player::get(sVictim); \
	 if (! pDst) \
 		qFatal("MainWindow: Message for nonexistant victim %d", sVictim); \
	QListWidgetItem *iDst=g_mwMainWindow->qmItems[pDst]; \
	Q_UNUSED(iDst)

void MessageServerLeave::process(Connection *) {
	MSG_INIT;

	g_mwMainWindow->log(MainWindow::tr("Left now: %1").arg(pSrc->qsName));
	if (g_mwMainWindow->qmItems.contains(pSrc)) {
		QListWidgetItem *item=g_mwMainWindow->qmItems.take(pSrc);

		g_mwMainWindow->qmPlayers.remove(item);

		delete item;
		Player::remove(pSrc);
		delete pSrc;
	}
}

void MessageSpeex::process(Connection *) {
}

void MessagePlayerSelfMuteDeaf::process(Connection *) {
	MSG_INIT;
	pSrc->bSelfMute = bMute;
	pSrc->bSelfDeaf = bDeaf;
	MainWindow::setItemColor(iSrc, pSrc);
}

void MessagePlayerMute::process(Connection *) {
	MSG_INIT;
	VICTIM_INIT;
	pDst->bMute = bMute;
	MainWindow::setItemColor(iDst, pDst);

	QString vic = pDst->qsName;
	QString admin = pSrc->qsName;

	if (sVictim == g_mwMainWindow->sMyId)
		g_mwMainWindow->log(bMute ? MainWindow::tr("You were muted by %1").arg(admin) : MainWindow::tr("You were unmuted by %1").arg(admin));
	else
		g_mwMainWindow->log(bMute ? MainWindow::tr("%1 muted by %2").arg(vic).arg(admin) : MainWindow::tr("%1 unmuted by %2").arg(vic).arg(admin), QString());
}

void MessagePlayerDeaf::process(Connection *) {
	MSG_INIT;
	VICTIM_INIT;
	pDst->bDeaf = bDeaf;
	MainWindow::setItemColor(iDst, pDst);

	QString vic = pDst->qsName;
	QString admin = pSrc->qsName;

	if (sVictim == g_mwMainWindow->sMyId)
		g_mwMainWindow->log(bDeaf ? MainWindow::tr("You were deafened by %1").arg(admin) : MainWindow::tr("You were undeafened by %1").arg(admin));
	else
		g_mwMainWindow->log(bDeaf ? MainWindow::tr("%1 defened by %2").arg(vic).arg(admin) : MainWindow::tr("%1 undeafened by %2").arg(vic).arg(admin), QString());
}

void MessagePlayerKick::process(Connection *) {
	MSG_INIT;
	VICTIM_INIT;
	if (sVictim == g_mwMainWindow->sMyId)
		g_mwMainWindow->log(MainWindow::tr("You were kicked from the server by %1: %2").arg(pSrc->qsName).arg(qsReason));
	else
		g_mwMainWindow->log(MainWindow::tr("%3 was kicked from the server by %1: %2").arg(pSrc->qsName).arg(qsReason).arg(pDst->qsName));
}

void MessageServerAuthenticate::process(Connection *) {
}

void MessageServerReject::process(Connection *) {
	g_mwMainWindow->log(MainWindow::tr("Server connection rejected: %1").arg(qsReason));
}

void MessageServerSync::process(Connection *) {
	MSG_INIT;
	g_mwMainWindow->sMyId = sPlayerId;
	g_mwMainWindow->recheckTTS();
}
