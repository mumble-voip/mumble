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


void MainWindow::msgServerJoin(Connection *, MessageServerJoin *msg) {
	ClientPlayer *p = pmModel->addPlayer(msg->uiSession, msg->qsPlayerName);
	p->iId = msg->iId;
	g.l->log(Log::PlayerJoin, MainWindow::tr("Joined server: %1.").arg(p->qsName));
}

#define MSG_INIT \
	ClientPlayer *pSrc=ClientPlayer::get(msg->uiSession); \
	Q_UNUSED(pSrc);

#define VICTIM_INIT \
	ClientPlayer *pDst=ClientPlayer::get(msg->uiVictim); \
	 if (! pDst) { \
 		qWarning("MainWindow: Message for nonexistant victim %d.", msg->uiVictim); \
		return; \
	}

void MainWindow::msgServerLeave(Connection *, MessageServerLeave *msg) {
	MSG_INIT;

	if (! pSrc)
		return;

	g.l->log(Log::PlayerLeave, MainWindow::tr("Left server: %1.").arg(pSrc->qsName));
	pmModel->removePlayer(pSrc);
}

void MainWindow::msgServerBanList(Connection *, MessageServerBanList *msg) {
	if (banEdit) {
		banEdit->reject();
		delete banEdit;
		banEdit = NULL;
	}
	banEdit = new BanEditor(msg, this);
	banEdit->show();

}

void MainWindow::msgSpeex(Connection *, MessageSpeex *) {
}

void MainWindow::msgPlayerSelfMuteDeaf(Connection *, MessagePlayerSelfMuteDeaf *msg) {
	MSG_INIT;

	if (! pSrc)
		return;

	pSrc->setSelfMuteDeaf(msg->bMute, msg->bDeaf);

	if (msg->uiSession == g.uiSession || ! g.uiSession)
		return;
	if (pSrc->cChannel != ClientPlayer::get(g.uiSession)->cChannel)
		return;

	QString name = pSrc->qsName;
	if (msg->bMute && msg->bDeaf)
		g.l->log(Log::OtherSelfMute, MainWindow::tr("%1 is now muted and deafened.").arg(name));
	else if (msg->bMute)
		g.l->log(Log::OtherSelfMute, MainWindow::tr("%1 is now muted.").arg(name));
	else
		g.l->log(Log::OtherSelfMute, MainWindow::tr("%1 is now unmuted.").arg(name));
}

void MainWindow::msgPlayerMute(Connection *, MessagePlayerMute *msg) {
	MSG_INIT;
	VICTIM_INIT;

	pDst->setMute(msg->bMute);

	if (!g.uiSession || pDst->cChannel != ClientPlayer::get(g.uiSession)->cChannel)
		return;

	QString vic = pDst->qsName;
	QString admin = pSrc ? pSrc->qsName : MainWindow::tr("server");

	if (msg->uiVictim == g.uiSession) {
		g.l->log(Log::YouMuted, msg->bMute ? MainWindow::tr("You were muted by %1.").arg(admin) : MainWindow::tr("You were unmuted by %1.").arg(admin));
		updateTrayIcon();
	} else {
		g.l->log((msg->uiSession == g.uiSession) ? Log::YouMutedOther : Log::OtherMutedOther, msg->bMute ? MainWindow::tr("%1 muted by %2.").arg(vic).arg(admin) : MainWindow::tr("%1 unmuted by %2.").arg(vic).arg(admin));
	}
}

void MainWindow::msgPlayerDeaf(Connection *, MessagePlayerDeaf *msg) {
	MSG_INIT;
	VICTIM_INIT;

	pDst->setDeaf(msg->bDeaf);

	if (!g.uiSession || pDst->cChannel != ClientPlayer::get(g.uiSession)->cChannel)
		return;

	QString vic = pDst->qsName;
	QString admin = pSrc ? pSrc->qsName : MainWindow::tr("server");

	if (msg->uiVictim == g.uiSession) {
		g.l->log(Log::YouMuted, msg->bDeaf ? MainWindow::tr("You were deafened by %1.").arg(admin) : MainWindow::tr("You were undeafened by %1.").arg(admin));
		updateTrayIcon();
	} else {
		g.l->log((msg->uiSession == g.uiSession) ? Log::YouMutedOther : Log::OtherMutedOther, msg->bDeaf ? MainWindow::tr("%1 deafened by %2.").arg(vic).arg(admin) : MainWindow::tr("%1 undeafened by %2.").arg(vic).arg(admin));
	}
}

void MainWindow::msgPlayerKick(Connection *, MessagePlayerKick *msg) {
	MSG_INIT;
	VICTIM_INIT;
	QString admin = pSrc ? pSrc->qsName : QLatin1String("server");

	if (msg->uiVictim == g.uiSession) {
		g.l->log(Log::YouKicked, MainWindow::tr("You were kicked from the server by %1: %2.").arg(admin).arg(msg->qsReason));
		g.l->setIgnore(Log::ServerDisconnected, 1);
	} else {
		g.l->setIgnore(Log::PlayerLeave, 1);
		g.l->log((msg->uiSession == g.uiSession) ? Log::YouKicked : Log::PlayerKicked, MainWindow::tr("%3 was kicked from the server by %1: %2.").arg(admin).arg(msg->qsReason).arg(pDst->qsName));
	}
}

void MainWindow::msgPlayerBan(Connection *, MessagePlayerBan *msg) {
	MSG_INIT;
	VICTIM_INIT;
	QString admin = pSrc ? pSrc->qsName : QLatin1String("server");
	if (msg->uiVictim == g.uiSession) {
		g.l->log(Log::YouKicked, MainWindow::tr("You were kicked and banned from the server by %1: %2.").arg(admin).arg(msg->qsReason));
		g.l->setIgnore(Log::ServerDisconnected, 1);
	} else {
		g.l->setIgnore(Log::PlayerLeave, 1);
		g.l->log((msg->uiSession == g.uiSession) ? Log::YouKicked : Log::PlayerKicked, MainWindow::tr("%3 was kicked and banned from the server by %1: %2.").arg(admin).arg(msg->qsReason).arg(pDst->qsName));
	}
}

void MainWindow::msgPlayerMove(Connection *, MessagePlayerMove *msg) {
	MSG_INIT;
	VICTIM_INIT;

	Channel *c = Channel::get(msg->iChannelId);
	if (!c) {
		qWarning("MessagePlayerMove for unknown channel.");
		c = Channel::get(0);
	}

	bool log = true;
	if ((msg->uiVictim == g.uiSession) && (msg->uiSession == msg->uiVictim))
		log = false;
	if (g.uiSession == 0)
		log = false;

	QString pname = pDst->qsName;
	QString admin = pSrc ? pSrc->qsName : QLatin1String("server");

	Channel *old = pDst->cChannel;

	if (log) {
		if (g.uiSession == msg->uiVictim) {
			g.l->log(Log::ChannelJoin, MainWindow::tr("You were moved to %1 by %2.").arg(c->qsName).arg(admin));
		} else if (pDst->cChannel == ClientPlayer::get(g.uiSession)->cChannel) {
			if (pDst == pSrc || (!pSrc))
				g.l->log(Log::ChannelLeave, MainWindow::tr("%1 left channel.").arg(pname));
			else
				g.l->log(Log::ChannelLeave, MainWindow::tr("%1 moved out by %2 to %3.").arg(pname).arg(admin).arg(c->qsName));
		}
	}

	pmModel->movePlayer(pDst, c);

	if (log && (g.uiSession != msg->uiVictim) && (pDst->cChannel == ClientPlayer::get(g.uiSession)->cChannel)) {
		if (pDst == pSrc || (!pSrc))
			g.l->log(Log::ChannelJoin, MainWindow::tr("%1 entered channel.").arg(pname));
		else
			g.l->log(Log::ChannelJoin, MainWindow::tr("%1 moved in by %2 from %3.").arg(pname).arg(admin).arg(old->qsName));
	}
}

void MainWindow::msgPlayerRename(Connection *, MessagePlayerRename *msg) {
	MSG_INIT;
	if (pSrc)
		pmModel->renamePlayer(pSrc, msg->qsName);
}

void MainWindow::msgChannelAdd(Connection *, MessageChannelAdd *msg) {
	if (msg->qsName.isEmpty())
		return;

	if (msg->iId == 0) {
		pmModel->renameChannel(Channel::get(0), msg->qsName);
		return;
	}

	Channel *p = Channel::get(msg->iParent);
	if (p)
		pmModel->addChannel(msg->iId, p, msg->qsName);
}

void MainWindow::msgChannelRemove(Connection *, MessageChannelRemove *msg) {
	if (msg->iId == 0)
		return;

	Channel *c = Channel::get(msg->iId);
	if (c)
		pmModel->removeChannel(c);
}

void MainWindow::msgChannelMove(Connection *, MessageChannelMove *msg) {
	if (msg->iId == 0)
		return;

	Channel *c = Channel::get(msg->iId);
	Channel *p = Channel::get(msg->iParent);
	if (!c || !p)
		return;

	Channel *pp = p;
	while (pp) {
		if (pp == c)
			return;
		pp = pp->cParent;
	}
	if (c && p)
		pmModel->moveChannel(c, p);
}

void MainWindow::msgChannelRename(Connection *, MessageChannelRename *msg) {
	if (msg->iId == 0)
		return;

	Channel *c = Channel::get(msg->iId);
	if (c && c->cParent)
		pmModel->renameChannel(c, msg->qsName);
}

void MainWindow::msgChannelDescUpdate(Connection *, MessageChannelDescUpdate *msg) {
	Channel *c = Channel::get(msg->iId);
	if (c) c->qsDesc=msg->qsDesc;
}

void MainWindow::msgChannelLink(Connection *, MessageChannelLink *msg) {
	Channel *c = Channel::get(msg->iId);
	if (!c)
		return;

	QList<Channel *> qlChans;
	foreach(int id, msg->qlTargets) {
		Channel *l = Channel::get(id);
		if (l)
			qlChans << l;
	}

	switch (msg->ltType) {
		case MessageChannelLink::Link:
			pmModel->linkChannels(c, qlChans);
			break;
		case MessageChannelLink::Unlink:
			pmModel->unlinkChannels(c, qlChans);
			break;
		case MessageChannelLink::UnlinkAll:
			pmModel->unlinkAll(c);
			break;
		default:
			qFatal("Unknown link message");
	}
}

void MainWindow::msgServerAuthenticate(Connection *, MessageServerAuthenticate *) {
}

void MainWindow::msgServerReject(Connection *, MessageServerReject *msg) {
	rtLast = msg->rtType;
	g.l->log(Log::ServerDisconnected, MainWindow::tr("Server connection rejected: %1.").arg(msg->qsReason));
	g.l->setIgnore(Log::ServerDisconnected, 1);
}

void MainWindow::msgPermissionDenied(Connection *, MessagePermissionDenied *msg) {
	g.l->log(Log::PermissionDenied, MainWindow::tr("Denied: %1.").arg(msg->qsReason));
}

void MainWindow::msgServerSync(Connection *, MessageServerSync *msg) {
	MSG_INIT;
	g.iMaxBandwidth = msg->iMaxBandwidth;
	g.uiSession = msg->uiSession;
	g.l->clearIgnore();
	g.l->log(Log::Information, msg->qsWelcomeText);
	pmModel->ensureSelfVisible();
	pmModel->recheckLinks();

	AudioInputPtr ai = g.ai;
	if (ai) {
		int bw = ai->getMaxBandwidth();
		if (bw > msg->iMaxBandwidth) {
			g.l->log(Log::Information, MainWindow::tr("Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.").arg(msg->iMaxBandwidth / 125));
		}
		ai->setMaxBandwidth(g.iMaxBandwidth);
	}

	findDesiredChannel();
}

void MainWindow::msgTextMessage(Connection *, MessageTextMessage *msg) {
	MSG_INIT;
	const QString &name = pSrc ? pSrc->qsName : tr("Server", "message from");
	g.l->log(Log::TextMessage, MainWindow::tr("From %1: %2").arg(name).arg(msg->qsMessage),
	         MainWindow::tr("Message from %1").arg(name));
}

void MainWindow::msgEditACL(Connection *, MessageEditACL *msg) {
	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = NULL;
	}
	aclEdit = new ACLEditor(msg, this);
	aclEdit->show();
}

void MainWindow::msgQueryUsers(Connection *, MessageQueryUsers *msg) {
	if (aclEdit)
		aclEdit->returnQuery(msg);
}

void MainWindow::msgPing(Connection *, MessagePing *) {
}

void MainWindow::msgPingStats(Connection *, MessagePingStats *) {
}

void MainWindow::msgTexture(Connection *, MessageTexture *) {
}

void MainWindow::msgCryptSetup(Connection *, MessageCryptSetup *msg) {
	ConnectionPtr c= g.sh->cConnection;
	if (! c)
		return;
	c->csCrypt.setKey(reinterpret_cast<const unsigned char *>(msg->qbaKey.constData()), reinterpret_cast<const unsigned char *>(msg->qbaClientNonce.constData()), reinterpret_cast<const unsigned char *>(msg->qbaServerNonce.constData()));
}

void MainWindow::msgCryptSync(Connection *, MessageCryptSync *msg) {
	ConnectionPtr c= g.sh->cConnection;
	if (! c)
		return;
	if (msg->qbaNonce.isEmpty()) {
		msg->qbaNonce = QByteArray(reinterpret_cast<const char *>(c->csCrypt.encrypt_iv), AES_BLOCK_SIZE);
		g.sh->sendMessage(msg);
	} else if (msg->qbaNonce.size() == AES_BLOCK_SIZE) {
		c->csCrypt.uiResync++;
		memcpy(c->csCrypt.decrypt_iv, msg->qbaNonce.constData(), AES_BLOCK_SIZE);
	}
}

void MainWindow::msgContextAction(Connection *, MessageContextAction *) {
}

void MainWindow::msgContextAddAction(Connection *, MessageContextAddAction *msg) {
	QAction *a = new QAction(msg->qsText, g.mw);
	a->setData(msg->qsAction);
	connect(a, SIGNAL(triggered()), this, SLOT(context_triggered()));
	if (msg->ctx & MessageContextAddAction::CtxServer)
		qlServerActions.append(a);
	if (msg->ctx & MessageContextAddAction::CtxPlayer)
		qlPlayerActions.append(a);
	if (msg->ctx & MessageContextAddAction::CtxChannel)
		qlChannelActions.append(a);
}
