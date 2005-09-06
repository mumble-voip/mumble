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
#include "MainWindow.h"
#include "AudioInput.h"
#include "Settings.h"

MainWindow *g_mwMainWindow;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setupGui();

	m_sMyId = 0;

	connect(g_shServer, SIGNAL(connected()), this, SLOT(serverConnected()));
	connect(g_shServer, SIGNAL(disconnected(QString)), this, SLOT(serverDisconnected(QString)));

	m_tts=new TextToSpeech(this);
	recheckTTS();
	m_tts->say(tr("Welcome to Mumble."));
}

void MainWindow::setupGui()  {
	QMenu *qmServer, *qmPlayer, *qmAudio, *qmHelp;
	QAction *qa;
	QActionGroup *qag;
	QMenu *qm;

	setWindowTitle(tr("Mumble -- %1").arg(QString(MUMBLE_VERSION)));

	m_qlwPlayers = new QListWidget(this);
	setCentralWidget(m_qlwPlayers);

	qmServer = new QMenu(tr("&Server"), this);
	qmPlayer = new QMenu(tr("&Player"), this);
	qmAudio = new QMenu(tr("&Audio"), this);
	qmHelp = new QMenu(tr("&Help"), this);

	qmServer->setObjectName("ServerMenu");
	qmPlayer->setObjectName("PlayerMenu");
	qmAudio->setObjectName("AudioMenu");
	qmHelp->setObjectName("HelpMenu");

	m_qaServerConnect=new QAction(tr("&Connect"), this);
	m_qaServerDisconnect=new QAction(tr("&Disconnect"), this);
	m_qaServerConnect->setObjectName("ServerConnect");
	m_qaServerDisconnect->setObjectName("ServerDisconnect");
	m_qaServerDisconnect->setEnabled(FALSE);

	qmServer->addAction(m_qaServerConnect);
	qmServer->addAction(m_qaServerDisconnect);

	m_qaPlayerKick=new QAction(tr("&Kick"), this);
	m_qaPlayerMute=new QAction(tr("&Mute"), this);
	m_qaPlayerDeaf=new QAction(tr("&Deafen"), this);
	m_qaPlayerKick->setObjectName("PlayerKick");
	m_qaPlayerMute->setObjectName("PlayerMute");
	m_qaPlayerDeaf->setObjectName("PlayerDeaf");
	m_qaPlayerKick->setEnabled(FALSE);
	m_qaPlayerMute->setCheckable(TRUE);
	m_qaPlayerMute->setEnabled(FALSE);
	m_qaPlayerDeaf->setCheckable(TRUE);
	m_qaPlayerDeaf->setEnabled(FALSE);

	qmPlayer->addAction(m_qaPlayerKick);
	qmPlayer->addAction(m_qaPlayerMute);
	qmPlayer->addAction(m_qaPlayerDeaf);

	m_qaAudioReset=new QAction(tr("&Reset"), this);
	m_qaAudioReset->setObjectName("AudioReset");
	m_qaAudioMute=new QAction(tr("&Mute"), this);
	m_qaAudioMute->setObjectName("AudioMute");
	m_qaAudioDeaf=new QAction(tr("&Deaf"), this);
	m_qaAudioDeaf->setObjectName("AudioDeaf");
	m_qaAudioShortcuts=new QAction(tr("&Shortcuts"), this);
	m_qaAudioShortcuts->setObjectName("AudioShortcuts");
	m_qaAudioTTS=new QAction(tr("&Text-To-Speech"), this);
	m_qaAudioTTS->setObjectName("AudioTextToSpeech");
	m_qaAudioMute->setCheckable(true);
	m_qaAudioDeaf->setCheckable(true);
	m_qaAudioTTS->setCheckable(true);
	g_s.bMute = qs.value("AudioMute", false). toBool();
	g_s.bDeaf = qs.value("AudioDeaf", false). toBool();
	m_qaAudioMute->setChecked(g_s.bMute);
	m_qaAudioDeaf->setChecked(g_s.bDeaf);
	m_qaAudioTTS->setChecked(qs.value("TextToSpeech", true).toBool());

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

	qmAudio->addAction(m_qaAudioMute);
	qmAudio->addAction(m_qaAudioDeaf);
	qmAudio->addSeparator();
	qmAudio->addAction(m_qaAudioReset);
	qmAudio->addAction(m_qaAudioShortcuts);
	qmAudio->addAction(m_qaAudioTTS);
	qmAudio->addMenu(qm);

	m_qaHelpAbout=new QAction(tr("&About"), this);
	m_qaHelpAbout->setObjectName("HelpAbout");
	m_qaHelpAboutQt=new QAction(tr("&About QT"), this);
	m_qaHelpAboutQt->setObjectName("HelpAboutQt");

	qmHelp->addAction(m_qaHelpAbout);
	qmHelp->addAction(m_qaHelpAboutQt);

	menuBar()->addMenu(qmServer);
	menuBar()->addMenu(qmPlayer);
	menuBar()->addMenu(qmAudio);
	menuBar()->addMenu(qmHelp);

	m_gsPushTalk=new GlobalShortcut(this, 1, "Push-to-Talk");
	m_gsPushTalk->setObjectName("PushToTalk");

	m_gsResetAudio=new GlobalShortcut(this, 2, "Reset Audio Processor");
	m_gsResetAudio->setObjectName("ResetAudio");
	connect(m_gsResetAudio, SIGNAL(down()), m_qaAudioReset, SLOT(trigger()));

	m_gsMuteSelf=new GlobalShortcut(this, 3, "Toggle Mute Self");
	m_gsMuteSelf->setObjectName("MuteSelf");
	connect(m_gsMuteSelf, SIGNAL(down()), m_qaAudioMute, SLOT(trigger()));

	m_gsDeafSelf=new GlobalShortcut(this, 4, "Toggle Deafen Self");
	m_gsDeafSelf->setObjectName("DeafSelf");
	connect(m_gsDeafSelf, SIGNAL(down()), m_qaAudioDeaf, SLOT(trigger()));

    QMetaObject::connectSlotsByName(this);
}

void MainWindow::recheckTTS()
{
	m_tts->setEnabled(m_qaAudioTTS->isChecked());
}

void MainWindow::on_ServerConnect_triggered()
{
	ConnectDialog *cd = new ConnectDialog(this);
	int res = cd->exec();

	if (res == QDialog::Accepted) {
		m_qaServerConnect->setEnabled(false);
		m_qaServerDisconnect->setEnabled(true);
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
	QListWidgetItem *item = m_qlwPlayers->currentItem();
	if (! item) {
		m_qaPlayerKick->setEnabled(false);
		m_qaPlayerMute->setEnabled(false);
		m_qaPlayerDeaf->setEnabled(false);
	} else {
		Player *p = m_qmPlayers[item];
		m_qaPlayerKick->setEnabled(true);
		m_qaPlayerMute->setEnabled(true);
		m_qaPlayerDeaf->setEnabled(true);
		m_qaPlayerMute->setChecked(p->m_bMute);
		m_qaPlayerDeaf->setChecked(p->m_bDeaf);
	}
}

void MainWindow::on_PlayerMute_triggered()
{
	QListWidgetItem *item = m_qlwPlayers->currentItem();
	if (! item)
		return;
	Player *p = m_qmPlayers[item];
	MessagePlayerMute mpmMsg;
	mpmMsg.m_sPlayerId = p->m_sId;
	mpmMsg.m_bMute = ! p->m_bMute;
	g_shServer->sendMessage(&mpmMsg);
}

void MainWindow::on_PlayerDeaf_triggered()
{
	QListWidgetItem *item = m_qlwPlayers->currentItem();
	if (! item)
		return;
	Player *p = m_qmPlayers[item];
	MessagePlayerDeaf mpdMsg;
	mpdMsg.m_sPlayerId = p->m_sId;
	mpdMsg.m_bDeaf = ! p->m_bDeaf;
	g_shServer->sendMessage(&mpdMsg);
}

void MainWindow::on_PlayerKick_triggered()
{
	QListWidgetItem *item = m_qlwPlayers->currentItem();
	if (! item)
		return;
	Player *p = m_qmPlayers[item];

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Kicking player %1").arg(p->m_qsName), tr("Enter reason"), QLineEdit::Normal, "", &ok);
	if (ok) {
		MessagePlayerKick mpkMsg;
		mpkMsg.m_sPlayerId=p->m_sId;
		mpkMsg.m_qsReason = reason;
		g_shServer->sendMessage(&mpkMsg);
	}
}

void MainWindow::on_AudioReset_triggered()
{
	if (g_aiInput)
		g_aiInput->m_bResetProcessor = true;
}

void MainWindow::on_AudioShortcuts_triggered()
{
	GlobalShortcut::configure();
}

void MainWindow::on_AudioMute_triggered()
{
	g_s.bMute = m_qaAudioMute->isChecked();
	if (! g_s.bMute && g_s.bDeaf) {
		g_s.bDeaf = false;
		m_qaAudioDeaf->setChecked(false);
		m_tts->say(tr("Unmuted and undeafened"));
	} else if (! g_s.bMute) {
		m_tts->say(tr("Unmuted"));
	} else {
		m_tts->say(tr("Muted"));
	}

	MessagePlayerSelfMuteDeaf mpsmd;
	mpsmd.m_bMute = g_s.bMute;
	mpsmd.m_bDeaf = g_s.bDeaf;
	g_shServer->sendMessage(&mpsmd);

	qs.setValue("AudioMute", g_s.bMute);
	qs.setValue("AudioDeaf", g_s.bDeaf);
}

void MainWindow::on_AudioDeaf_triggered()
{
	g_s.bDeaf = m_qaAudioDeaf->isChecked();
	if (g_s.bDeaf && ! g_s.bMute) {
		g_s.bMute = true;
		m_qaAudioMute->setChecked(true);
		m_tts->say(tr("Muted and deafened"));
	} else if (g_s.bDeaf) {
		m_tts->say(tr("Deafened"));
	} else {
		m_tts->say(tr("Undeafened"));
	}

	MessagePlayerSelfMuteDeaf mpsmd;
	mpsmd.m_bMute = g_s.bMute;
	mpsmd.m_bDeaf = g_s.bDeaf;
	g_shServer->sendMessage(&mpsmd);

	qs.setValue("AudioMute", g_s.bMute);
	qs.setValue("AudioDeaf", g_s.bDeaf);
}

void MainWindow::on_AudioTextToSpeech_triggered()
{
	qs.setValue("TextToSpeech", m_qaAudioTTS->isChecked());
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
	QListWidgetItem *item=m_qmItems[p];
	item->setBackgroundColor(bTalking ? Qt::lightGray : Qt::white);
}

void MainWindow::serverConnected()
{
	m_tts->say(tr("Connected to server"));
	m_tts->setEnabled(false);
	m_qaServerDisconnect->setEnabled(true);

	if (g_s.bMute || g_s.bDeaf) {
		MessagePlayerSelfMuteDeaf mpsmd;
		mpsmd.m_bMute = g_s.bMute;
		mpsmd.m_bDeaf = g_s.bDeaf;
		g_shServer->sendMessage(&mpsmd);
	}
}

void MainWindow::serverDisconnected(QString reason)
{
	m_sMyId = 0;
	recheckTTS();
	m_qaServerConnect->setEnabled(true);
	m_qaServerDisconnect->setEnabled(false);
	QMapIterator<Player *, QListWidgetItem *> iItems(m_qmItems);
	while (iItems.hasNext()) {
		iItems.next();
		delete iItems.value();
	}
	m_qmItems.clear();
	m_qmPlayers.clear();

	if (! reason.isEmpty()) {
  	  m_tts->say(tr("Server connection failed. %1").arg(reason));
	  QMessageBox::warning(this, "Server connection failed", reason, QMessageBox::Ok, QMessageBox::NoButton);
    } else {
	  m_tts->say(tr("Disconnected from server."));
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
	if (p->m_bMute || p->m_bSelfMute) {
		if (p->m_bDeaf || p->m_bSelfDeaf)
			item->setTextColor(Qt::blue);
		else
			item->setTextColor(Qt::yellow);
	} else if (p->m_bDeaf || p->m_bSelfDeaf) {
		item->setTextColor(Qt::magenta);
	} else {
		item->setTextColor(Qt::black);
	}
}

void MessageServerJoin::process(Connection *) {
	QListWidgetItem *item = new QListWidgetItem(m_qsPlayerName, g_mwMainWindow->m_qlwPlayers);
	Player *p = Player::add(m_sPlayerId);
	p->m_qsName = m_qsPlayerName;
	p->m_sId = m_sPlayerId;

	item->setData(Qt::UserRole, p);

	g_mwMainWindow->m_qmPlayers[item]=p;
	g_mwMainWindow->m_qmItems[p]=item;

	QObject::connect(p, SIGNAL(talkingChanged(Player *, bool)), g_mwMainWindow, SLOT(playerTalkingChanged(Player *, bool)));

	g_mwMainWindow->m_tts->say(QObject::tr("Joined now: %1").arg(p->m_qsName));
}

#define MSG_INIT \
 Player *p=Player::get(m_sPlayerId); \
 if (! p) \
 	qFatal("MainWindow: Message for nonexistant player %d", m_sPlayerId); \
 QListWidgetItem *item=g_mwMainWindow->m_qmItems[p]; \
 Q_UNUSED(item)

void MessageServerLeave::process(Connection *) {
	Player *p=Player::get(m_sPlayerId);

	g_mwMainWindow->m_tts->say(QObject::tr("Left now: %1").arg(p->m_qsName));
	if (g_mwMainWindow->m_qmItems.contains(p)) {
		QListWidgetItem *item=g_mwMainWindow->m_qmItems.take(p);

		g_mwMainWindow->m_qmPlayers.remove(item);

		delete item;
		Player::remove(p);
		delete p;
	}
}

void MessageSpeex::process(Connection *) {
}

void MessagePlayerSelfMuteDeaf::process(Connection *) {
	MSG_INIT;
	p->m_bSelfMute = m_bMute;
	p->m_bSelfDeaf = m_bDeaf;
	MainWindow::setItemColor(item, p);
}

void MessagePlayerMute::process(Connection *) {
	MSG_INIT;
	p->m_bMute = m_bMute;
	MainWindow::setItemColor(item, p);

	if (m_sPlayerId == g_mwMainWindow->m_sMyId)
		g_mwMainWindow->m_tts->say(m_bMute ? QObject::tr("You are muted") : QObject::tr("You are unmuted"));
}

void MessagePlayerDeaf::process(Connection *) {
	MSG_INIT;
	p->m_bDeaf = m_bDeaf;
	MainWindow::setItemColor(item, p);

	if (m_sPlayerId == g_mwMainWindow->m_sMyId)
		g_mwMainWindow->m_tts->say(m_bDeaf ? QObject::tr("You are deafened") : QObject::tr("You are undeafened"));
}

void MessagePlayerKick::process(Connection *) {
	MSG_INIT;
	g_mwMainWindow->m_tts->say(QObject::tr("You were kicked from the server. %1").arg(m_qsReason));
	QMessageBox::warning(g_mwMainWindow, QObject::tr("Kicked from server"), m_qsReason, QMessageBox::Ok, QMessageBox::NoButton);
}

void MessageServerAuthenticate::process(Connection *) {
}

void MessageServerReject::process(Connection *) {
	g_mwMainWindow->m_tts->say(QObject::tr("Server connection rejected. %1").arg(m_qsReason));
	QMessageBox::warning(g_mwMainWindow, QObject::tr("Server Rejected Connection"), m_qsReason, QMessageBox::Ok, QMessageBox::NoButton);
}

void MessageServerSync::process(Connection *) {
	MSG_INIT;
	g_mwMainWindow->m_sMyId = m_sPlayerId;
	g_mwMainWindow->recheckTTS();
}
