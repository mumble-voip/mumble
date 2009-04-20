/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#define ACTOR_INIT \
	ClientPlayer *pSrc=NULL; \
	if (msg.has_actor()) \
		pSrc = ClientPlayer::get(msg.actor()); \
	Q_UNUSED(pSrc);

#define VICTIM_INIT \
	ClientPlayer *pDst=ClientPlayer::get(msg.session()); \
	 if (! pDst) { \
 		qWarning("MainWindow: Message for nonexistant victim %d.", msg.session()); \
		return; \
	}

#define SELF_INIT \
	ClientPlayer *pSelf = ClientPlayer::get(g.uiSession);


void MainWindow::msgAuthenticate(const MumbleProto::Authenticate &) {
}

void MainWindow::msgBanList(const MumbleProto::BanList &msg) {
	if (banEdit) {
		banEdit->reject();
		delete banEdit;
		banEdit = NULL;
	}
	banEdit = new BanEditor(msg, this);
	banEdit->show();
}

void MainWindow::msgReject(const MumbleProto::Reject &msg) {
	rtLast = msg.type();
	g.l->log(Log::ServerDisconnected, tr("Server connection rejected: %1.").arg(u8(msg.reason())));
	g.l->setIgnore(Log::ServerDisconnected, 1);
}

void MainWindow::msgServerSync(const MumbleProto::ServerSync &msg) {
	g.iMaxBandwidth = msg.max_bandwidth();
	g.uiSession = msg.session();
	g.l->clearIgnore();
	g.l->log(Log::Information, u8(msg.welcome_text()));
	pmModel->ensureSelfVisible();
	pmModel->recheckLinks();

	AudioInputPtr ai = g.ai;
	if (ai) {
		int bw = ai->getMaxBandwidth();
		if (bw > g.iMaxBandwidth) {
			g.l->log(Log::Information, tr("Server maximum bandwidth is only %1 kbit/s. Quality auto-adjusted.").arg(g.iMaxBandwidth / 125));
		}
		ai->setMaxBandwidth(g.iMaxBandwidth);
	}

	findDesiredChannel();
}

void MainWindow::msgPermissionDenied(const MumbleProto::PermissionDenied &msg) {
	g.l->log(Log::PermissionDenied, tr("Denied: %1.").arg(u8(msg.reason())));
}

void MainWindow::msgUDPTunnel(const MumbleProto::UDPTunnel &) {
	qWarning("Fudge!");
}

void MainWindow::msgUserState(const MumbleProto::UserState &msg) {
	ACTOR_INIT;
	SELF_INIT;
	ClientPlayer *pDst=ClientPlayer::get(msg.session());

	if (! pDst) {
		if (msg.has_name()) {
			pDst = pmModel->addPlayer(msg.session(), u8(msg.name()));
			g.l->log(Log::PlayerJoin, tr("Joined server: %1.").arg(pDst->qsName));
			if (! msg.has_texture())
				g.o->verifyTexture(pDst);
		} else {
			return;
		}
	}

	if (msg.has_self_deaf() || msg.has_self_mute()) {
		if (msg.has_self_mute())
			pDst->setSelfMute(msg.self_mute());
		if (msg.has_self_deaf())
			pDst->setSelfDeaf(msg.self_deaf());

		if (pSelf && pDst != pSelf && (pDst->cChannel == pSelf->cChannel)) {
			QString name = pDst->qsName;
			if (pDst->bSelfMute && pDst->bSelfDeaf)
				g.l->log(Log::OtherSelfMute, tr("%1 is now muted and deafened.").arg(name));
			else if (pDst->bSelfMute)
				g.l->log(Log::OtherSelfMute, tr("%1 is now muted.").arg(name));
			else
				g.l->log(Log::OtherSelfMute, tr("%1 is now unmuted.").arg(name));
		}
	}

	if (msg.has_deaf() || msg.has_mute()) {
		if (msg.has_mute())
			pDst->setMute(msg.mute());
		if (msg.has_deaf())
			pDst->setDeaf(msg.deaf());

		if (pSelf && (pDst->cChannel == pSelf->cChannel)) {
			QString vic = pDst->qsName;
			QString admin = pSrc ? pSrc->qsName : tr("the server");

			if (pDst == pSelf) {
				if (pDst->bMute && pDst->bDeaf)
					g.l->log(Log::YouMuted, tr("You were deafened by %1.").arg(admin));
				else if (pDst->bMute)
					g.l->log(Log::YouMuted, tr("You were muted by %1.").arg(admin));
				else
					g.l->log(Log::YouMuted, tr("You were unmuted by %1.").arg(admin));

				updateTrayIcon();
			} else {
				if (pDst->bMute && pDst->bDeaf)
					g.l->log(Log::YouMuted, tr("%1 deafened by %2.").arg(vic).arg(admin));
				else if (pDst->bMute)
					g.l->log(Log::YouMuted, tr("%1 muted by %2.").arg(vic).arg(admin));
				else
					g.l->log(Log::YouMuted, tr("%1 unmuted by %2.").arg(vic).arg(admin));
			}
		}
	}

	if (msg.has_channel_id()) {
		Channel *c = Channel::get(msg.channel_id());
		if (!c) {
			qWarning("MessagePlayerMove for unknown channel.");
			c = Channel::get(0);
		}

		Channel *old = pDst->cChannel;
		if (c != old) {
			bool log = pSelf && !((pDst == pSelf) && (pSrc == pSelf));

			QString pname = pDst->qsName;
			QString admin = pSrc ? pSrc->qsName : tr("the server");

			if (log) {
				if (pDst == pSelf) {
					g.l->log(Log::ChannelJoin, tr("You were moved to %1 by %2.").arg(c->qsName).arg(admin));
				} else if (pDst->cChannel == ClientPlayer::get(g.uiSession)->cChannel) {
					if (pDst == pSrc)
						g.l->log(Log::ChannelLeave, tr("%1 moved to %2.").arg(pname).arg(c->qsName));
					else
						g.l->log(Log::ChannelLeave, tr("%1 moved to %2 by %3.").arg(pname).arg(c->qsName).arg(admin));
				}
			}

			pmModel->movePlayer(pDst, c);


			if (log && (pDst != pSelf) && (pDst->cChannel == pSelf->cChannel)) {
				if (pDst == pSrc)
					g.l->log(Log::ChannelJoin, tr("%1 entered channel.").arg(pname));
				else
					g.l->log(Log::ChannelJoin, tr("%1 moved in from %2 by %3.").arg(pname).arg(old->qsName).arg(admin));
			}
		}
	}
	if (msg.has_name()) {
		pmModel->renamePlayer(pDst, u8(msg.name()));
	}
	if (msg.has_texture()) {
		const std::string &str = msg.texture();
		pDst->qbaTexture = QByteArray(str.data(), str.size());
		g.o->verifyTexture(pDst);
	}
	if (msg.has_plugin_context()) {
		QMutexLocker qml(&g.p->qmPlugins);
		pDst->ssContext = msg.plugin_context();
	}
}

void MainWindow::msgUserRemove(const MumbleProto::UserRemove &msg) {
	VICTIM_INIT;
	ACTOR_INIT;
	SELF_INIT;

	QString admin = pSrc ? pSrc->qsName : tr("the server");
	QString reason = u8(msg.reason());

	if (pDst == pSelf) {
		if (msg.ban())
			g.l->log(Log::YouKicked, tr("You were kicked and banned from the server by %1: %2.").arg(admin).arg(reason));
		else
			g.l->log(Log::YouKicked, tr("You were kicked from the server by %1: %2.").arg(admin).arg(reason));
	} else if (pSrc) {
		if (msg.ban())
			g.l->log((pSrc == pSelf) ? Log::YouKicked : Log::PlayerKicked, tr("%3 was kicked and banned from the server by %1: %2.").arg(admin).arg(reason).arg(pDst->qsName));
		else
			g.l->log((pSrc == pSelf) ? Log::YouKicked : Log::PlayerKicked, tr("%3 was kicked from the server by %1: %2.").arg(admin).arg(reason).arg(pDst->qsName));
	} else {
		g.l->log(Log::PlayerLeave, tr("Left server: %1.").arg(pDst->qsName));
	}
	pmModel->removePlayer(pDst);
}

void MainWindow::msgChannelState(const MumbleProto::ChannelState &msg) {
	if (! msg.has_channel_id())
		return;

	Channel *c = Channel::get(msg.channel_id());
	Channel *p = Channel::get(msg.parent());

	if (!c) {
		if (msg.has_parent() && p && msg.has_name()) {
			c = pmModel->addChannel(msg.channel_id(), p, u8(msg.name()));
			p = NULL;
		} else {
			return;
		}
	}

	if (p) {
		Channel *pp = p;
		while (pp) {
			if (pp == c)
				return;
			pp = pp->cParent;
		}
		pmModel->moveChannel(c, p);
	}

	if (msg.has_name())
		pmModel->renameChannel(c, u8(msg.name()));

	if (msg.has_description())
		c->qsDesc = u8(msg.description());

	if (msg.links_size()) {
		QList<Channel *> ql;
		pmModel->unlinkAll(c);
		for (int i=0;i<msg.links_size();++i) {
			Channel *l = Channel::get(msg.links(i));
			if (l)
				ql << l;
		}
		if (! ql.isEmpty())
			pmModel->linkChannels(c, ql);
	}
	if (msg.links_remove_size()) {
		QList<Channel *> ql;
		for (int i=0;i<msg.links_remove_size();++i) {
			Channel *l = Channel::get(msg.links_remove(i));
			if (l)
				ql << l;
		}
		if (! ql.isEmpty())
			pmModel->unlinkChannels(c, ql);
	}
	if (msg.links_add_size()) {
		QList<Channel *> ql;
		for (int i=0;i<msg.links_add_size();++i) {
			Channel *l = Channel::get(msg.links_add(i));
			if (l)
				ql << l;
		}
		if (! ql.isEmpty())
			pmModel->linkChannels(c, ql);
	}
}

void MainWindow::msgChannelRemove(const MumbleProto::ChannelRemove &msg) {
	Channel *c = Channel::get(msg.channel_id());
	if (c && (c->iId != 0))
		pmModel->removeChannel(c);
}

void MainWindow::msgTextMessage(const MumbleProto::TextMessage &msg) {
	ACTOR_INIT;
	const QString &name = pSrc ? pSrc->qsName : tr("the server", "message from");
	g.l->log(Log::TextMessage, tr("From %1: %2").arg(name).arg(u8(msg.message())),
	         tr("Message from %1").arg(name));
}

void MainWindow::msgACL(const MumbleProto::ACL &msg) {
	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = NULL;
	}
	if (Channel::get(msg.channel_id())) {
		aclEdit = new ACLEditor(msg, this);
		aclEdit->show();
	}
}

void MainWindow::msgQueryUsers(const MumbleProto::QueryUsers &msg) {
	if (aclEdit)
		aclEdit->returnQuery(msg);
}

void MainWindow::msgPing(const MumbleProto::Ping &msg) {
}

void MainWindow::msgCryptSetup(const MumbleProto::CryptSetup &msg) {
	ConnectionPtr c= g.sh->cConnection;
	if (! c)
		return;
	if (msg.has_key() && msg.has_client_nonce() && msg.has_server_nonce()) {
		const std::string &key = msg.key();
		const std::string &client_nonce = msg.client_nonce();
		const std::string &server_nonce = msg.server_nonce();
		if (key.size() == AES_BLOCK_SIZE && client_nonce.size() == AES_BLOCK_SIZE && server_nonce.size() == AES_BLOCK_SIZE)
			c->csCrypt.setKey(reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(client_nonce.data()), reinterpret_cast<const unsigned char *>(server_nonce.data()));
	} else if (msg.has_server_nonce()) {
		const std::string &server_nonce = msg.server_nonce();
		if (server_nonce.size() == AES_BLOCK_SIZE) {
			c->csCrypt.uiResync++;
			memcpy(c->csCrypt.decrypt_iv, server_nonce.data(), AES_BLOCK_SIZE);
		}
	} else {
		MumbleProto::CryptSetup mpcs;
		mpcs.set_client_nonce(std::string(reinterpret_cast<const char *>(c->csCrypt.encrypt_iv), AES_BLOCK_SIZE));
		g.sh->sendMessage(mpcs);
	}
}

void MainWindow::msgContextAction(const MumbleProto::ContextAction &) {
}

void MainWindow::msgContextActionAdd(const MumbleProto::ContextActionAdd &msg) {
	QAction *a = new QAction(u8(msg.text()), g.mw);
	a->setData(u8(msg.action()));
	connect(a, SIGNAL(triggered()), this, SLOT(context_triggered()));
	unsigned int ctx = msg.context();
	if (ctx & MumbleProto::ContextActionAdd_Context_Server)
		qlServerActions.append(a);
	if (ctx & MumbleProto::ContextActionAdd_Context_User)
		qlPlayerActions.append(a);
	if (ctx & MumbleProto::ContextActionAdd_Context_Channel)
		qlChannelActions.append(a);
}

void MainWindow::msgVersion(const MumbleProto::Version &msg) {
	if (msg.has_version())
		g.sh->uiVersion = msg.version();
	if (msg.has_release())
		g.sh->qsRelease = u8(msg.release());
	if (msg.has_os()) {
		g.sh->qsOS = u8(msg.os());
		if (msg.has_os_version())
			g.sh->qsOSVersion = u8(msg.os_version());
	}
}
