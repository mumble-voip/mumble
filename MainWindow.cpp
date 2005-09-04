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
#include "MainWindow.h"
#include "AudioInput.h"

MainWindow *g_mwMainWindow;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setupGui();

	connect(g_shServer, SIGNAL(connected()), this, SLOT(serverConnected()));
	connect(g_shServer, SIGNAL(disconnected()), this, SLOT(serverDisconnected()));
}

void MainWindow::setupGui()  {
	QMenu *qmServer, *qmPlayer, *qmAudio, *qmHelp;

	setWindowTitle("Mumble -- Compiled " __DATE__ " " __TIME__);

	m_qlwPlayers = new QListWidget(this);
	setCentralWidget(m_qlwPlayers);

	qmServer = new QMenu("&Server", this);
	qmPlayer = new QMenu("&Player", this);
	qmAudio = new QMenu("&Audio", this);
	qmHelp = new QMenu("&Help", this);

	qmServer->setObjectName("ServerMenu");
	qmPlayer->setObjectName("PlayerMenu");
	qmAudio->setObjectName("AudioMenu");
	qmHelp->setObjectName("HelpMenu");

	m_qaServerConnect=new QAction("&Connect", this);
	m_qaServerDisconnect=new QAction("&Disconnect", this);
	m_qaServerConnect->setObjectName("ServerConnect");
	m_qaServerDisconnect->setObjectName("ServerDisconnect");
	m_qaServerDisconnect->setEnabled(FALSE);

	qmServer->addAction(m_qaServerConnect);
	qmServer->addAction(m_qaServerDisconnect);

	m_qaPlayerKick=new QAction("&Kick", this);
	m_qaPlayerMute=new QAction("&Mute", this);
	m_qaPlayerDeaf=new QAction("&Deafen", this);
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

	m_qaAudioReset=new QAction("&Reset", this);
	m_qaAudioReset->setObjectName("AudioReset");

	qmAudio->addAction(m_qaAudioReset);

	m_qaHelpAbout=new QAction("&About", this);
	m_qaHelpAbout->setObjectName("HelpAbout");
	m_qaHelpAboutQt=new QAction("&About QT", this);
	m_qaHelpAboutQt->setObjectName("HelpAboutQt");

	qmHelp->addAction(m_qaHelpAbout);
	qmHelp->addAction(m_qaHelpAboutQt);

	menuBar()->addMenu(qmServer);
	menuBar()->addMenu(qmPlayer);
	menuBar()->addMenu(qmAudio);
	menuBar()->addMenu(qmHelp);

    QMetaObject::connectSlotsByName(this);
}

void MainWindow::on_ServerConnect_triggered()
{
	m_qaServerConnect->setEnabled(FALSE);

	QString server = QInputDialog::getText(this, "Server Address", "Addr", QLineEdit::Normal, "128.39.114.2");
	QString user = QInputDialog::getText(this, "Username", "Uname");

	g_shServer->setConnectionInfo(server, user);
	g_shServer->start();
}

void MainWindow::on_ServerDisconnect_triggered()
{
	g_shServer->disconnect();
	m_qaServerDisconnect->setEnabled(FALSE);
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

	QString reason = QInputDialog::getText(this, "Kicking", "Reason");
	MessagePlayerKick mpkMsg;
	mpkMsg.m_sPlayerId=p->m_sId;
	mpkMsg.m_qsReason = reason;
	g_shServer->sendMessage(&mpkMsg);
}

void MainWindow::on_AudioReset_triggered()
{
	if (g_aiInput)
		g_aiInput->m_bResetProcessor = true;
}

void MainWindow::on_HelpAbout_triggered()
{
	AboutDialog adAbout(this);
	adAbout.exec();
}

void MainWindow::on_HelpAboutQt_triggered()
{
	QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::playerTalkingChanged(Player *p, bool bTalking)
{
	QListWidgetItem *item=m_qmItems[p];
	item->setBackgroundColor(bTalking ? Qt::lightGray : Qt::white);
}

void MainWindow::serverConnected()
{
	m_qaServerDisconnect->setEnabled(TRUE);
}

void MainWindow::serverDisconnected()
{
	m_qaServerConnect->setEnabled(TRUE);
	QMapIterator<Player *, QListWidgetItem *> iItems(m_qmItems);
	while (iItems.hasNext()) {
		iItems.next();
		delete iItems.value();
	}
	m_qmItems.clear();
	m_qmPlayers.clear();
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
	if (p->m_bMute) {
		if (p->m_bDeaf)
			item->setTextColor(Qt::blue);
		else
			item->setTextColor(Qt::yellow);
	} else if (p->m_bDeaf) {
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
}

#define MSG_INIT \
 Player *p=Player::get(m_sPlayerId); \
 if (! p) \
 	qFatal("MainWindow: Message for nonexistant player %d", m_sPlayerId); \
 QListWidgetItem *item=g_mwMainWindow->m_qmItems[p]

void MessageServerLeave::process(Connection *) {
	Player *p=Player::get(m_sPlayerId);
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

void MessagePlayerMute::process(Connection *) {
	MSG_INIT;
	p->m_bMute = m_bMute;
	MainWindow::setItemColor(item, p);
}

void MessagePlayerDeaf::process(Connection *) {
	MSG_INIT;
	p->m_bDeaf = m_bDeaf;
	MainWindow::setItemColor(item, p);
}

void MessagePlayerKick::process(Connection *) {
	MSG_INIT;
	QMessageBox::warning(g_mwMainWindow, "Kicked from server", m_qsReason, QMessageBox::Ok, QMessageBox::NoButton);
}
