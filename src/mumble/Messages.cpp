/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "mumble_pch.hpp"

#include "About.h"
#include "ACLEditor.h"
#include "AudioInput.h"
#include "AudioStats.h"
#include "AudioWizard.h"
#include "BanEditor.h"
#include "Channel.h"
#include "Connection.h"
#include "ConnectDialog.h"
#include "Database.h"
#include "Global.h"
#include "GlobalShortcut.h"
#include "Log.h"
#include "MainWindow.h"
#include "Overlay.h"
#include "Plugins.h"
#include "ServerHandler.h"
#include "User.h"
#include "UserEdit.h"
#include "UserInformation.h"
#include "UserModel.h"
#include "VersionCheck.h"
#include "ViewCert.h"

#define ACTOR_INIT \
	ClientUser *pSrc=NULL; \
	if (msg.has_actor()) \
		pSrc = ClientUser::get(msg.actor()); \
	Q_UNUSED(pSrc);

#define VICTIM_INIT \
	ClientUser *pDst=ClientUser::get(msg.session()); \
	 if (! pDst) { \
 		qWarning("MainWindow: Message for nonexistant victim %d.", msg.session()); \
		return; \
	}

#define SELF_INIT \
	ClientUser *pSelf = ClientUser::get(g.uiSession);


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

	QString reason(u8(msg.reason()));;

	switch (rtLast) {
		case MumbleProto::Reject_RejectType_InvalidUsername:
			reason = tr("Invalid username");
			break;
		case MumbleProto::Reject_RejectType_UsernameInUse:
			reason = tr("Username in use");
			break;
		case MumbleProto::Reject_RejectType_WrongUserPW:
			reason = tr("Wrong certificate or password");
			break;
		case MumbleProto::Reject_RejectType_WrongServerPW:
			reason = tr("Wrong password");
			break;
		case MumbleProto::Reject_RejectType_AuthenticatorFail:
			reason = tr("Your account information can not be verified currently. Please try again later");
			break;
		default:
			break;
	}

	g.l->log(Log::ServerDisconnected, tr("Server connection rejected: %1.").arg(reason));
	g.l->setIgnore(Log::ServerDisconnected, 1);
}

void MainWindow::msgServerSync(const MumbleProto::ServerSync &msg) {
	g.sh->sendPing(); // Send initial ping to establish UDP connection

	g.uiSession = msg.session();
	g.pPermissions = static_cast<ChanACL::Permissions>(msg.permissions());
	g.l->clearIgnore();
	g.l->log(Log::Information, tr("Welcome message: %1").arg(u8(msg.welcome_text())));
	pmModel->ensureSelfVisible();
	pmModel->recheckLinks();

	qmTargetUse.clear();
	qmTargets.clear();
	for (int i=1;i<6;++i) {
		qmTargetUse.insert(i, i);
	}
	iTargetCounter = 100;

	AudioInput::setMaxBandwidth(msg.max_bandwidth());

	findDesiredChannel();

	QString host, uname, pw;
	unsigned short port;

	g.sh->getConnectionInfo(host, port, uname, pw);

	QList<Shortcut> sc = Database::getShortcuts(g.sh->qbaDigest);
	if (! sc.isEmpty()) {
		for (int i=0;i<sc.count(); ++i) {
			Shortcut &s = sc[i];
			s.iIndex = g.mw->gsWhisper->idx;
		}
		g.s.qlShortcuts << sc;
		GlobalShortcutEngine::engine->bNeedRemap = true;
	}

	ClientUser *p=ClientUser::get(g.uiSession);
	connect(p, SIGNAL(talkingChanged()), this, SLOT(talkingChanged()));

	qstiIcon->setToolTip(tr("Mumble: %1").arg(Channel::get(0)->qsName));

	// Update QActions and menues
	on_qmServer_aboutToShow();
	on_qmSelf_aboutToShow();
	qmChannel_aboutToShow();
	qmUser_aboutToShow();
	on_qmConfig_aboutToShow();

	updateTrayIcon();
}


void MainWindow::msgServerConfig(const MumbleProto::ServerConfig &msg) {
	if (msg.has_welcome_text())
		g.l->log(Log::Information, tr("Welcome message: %1").arg(u8(msg.welcome_text())));
	if (msg.has_max_bandwidth())
		AudioInput::setMaxBandwidth(msg.max_bandwidth());
	if (msg.has_allow_html())
		g.bAllowHTML = msg.allow_html();
	if (msg.has_message_length())
		g.uiMessageLength = msg.message_length();
	if (msg.has_image_message_length())
		g.uiImageLength = msg.image_message_length();
}

void MainWindow::msgPermissionDenied(const MumbleProto::PermissionDenied &msg) {
	switch (msg.type()) {
		case MumbleProto::PermissionDenied_DenyType_Permission: {
				VICTIM_INIT;
				SELF_INIT;
				Channel *c = Channel::get(msg.channel_id());
				if (! c)
					return;
				QString pname = ChanACL::permName(static_cast<ChanACL::Permissions>(msg.permission()));
				if (pDst == pSelf)
					g.l->log(Log::PermissionDenied, tr("You were denied %1 privileges in %2.").arg(Log::msgColor(pname, Log::Privilege)).arg(Log::formatChannel(c)));
				else
					g.l->log(Log::PermissionDenied, tr("%3 was denied %1 privileges in %2.").arg(Log::msgColor(pname, Log::Privilege)).arg(Log::formatChannel(c)).arg(Log::formatClientUser(pDst, Log::Target)));
			}
			break;
		case MumbleProto::PermissionDenied_DenyType_SuperUser: {
				g.l->log(Log::PermissionDenied, tr("Denied: Cannot modify SuperUser."));
			}
			break;
		case MumbleProto::PermissionDenied_DenyType_ChannelName: {
				g.l->log(Log::PermissionDenied, tr("Denied: Invalid channel name."));
			}
			break;
		case MumbleProto::PermissionDenied_DenyType_TextTooLong: {
				g.l->log(Log::PermissionDenied, tr("Denied: Text message too long."));
			}
			break;
		case MumbleProto::PermissionDenied_DenyType_H9K: {
				if (g.bHappyEaster) {
					unsigned short p;
					QString h, u, pw;
					bool bold = g.s.bDeaf;
					bool bold2 = g.s.bTTS;
					g.sh->getConnectionInfo(h, p, u, pw);
					g.s.bDeaf = false;
					g.s.bTTS = true;
					quint32 oflags = g.s.qmMessages.value(Log::PermissionDenied);
					g.s.qmMessages[Log::PermissionDenied] = (oflags | Settings::LogTTS) & (~Settings::LogSoundfile);
					g.l->log(Log::PermissionDenied, QString::fromUtf8(g.ccHappyEaster + 39).arg(u));
					g.s.qmMessages[Log::PermissionDenied] = oflags;
					g.s.bDeaf = bold;
					g.s.bTTS = bold2;
					g.mw->setWindowIcon(QIcon(QString::fromUtf8(g.ccHappyEaster)));
					g.mw->setStyleSheet(QString::fromUtf8(g.ccHappyEaster + 82));
					qWarning() << "Happy Easter";
				}
			}
			break;
		case MumbleProto::PermissionDenied_DenyType_TemporaryChannel: {
				g.l->log(Log::PermissionDenied, tr("Denied: Operation not permitted in temporary channel."));
			}
			break;
		case MumbleProto::PermissionDenied_DenyType_MissingCertificate: {
				VICTIM_INIT;
				SELF_INIT;
				if (pDst == pSelf)
					g.l->log(Log::PermissionDenied, tr("You need a certificate to perform this operation."));
				else
					g.l->log(Log::PermissionDenied, tr("%1 does not have a certificate.").arg(Log::formatClientUser(pDst, Log::Target)));
			}
			break;
		case MumbleProto::PermissionDenied_DenyType_UserName: {
				if (msg.has_name())
					g.l->log(Log::PermissionDenied, tr("Invalid username: %1.").arg(u8(msg.name())));
				else
					g.l->log(Log::PermissionDenied, tr("Invalid username."));
			}
			break;
		case MumbleProto::PermissionDenied_DenyType_ChannelFull: {
				g.l->log(Log::PermissionDenied, tr("Channel is full."));
			}
			break;
		case MumbleProto::PermissionDenied_DenyType_NestingLimit: {
				g.l->log(Log::PermissionDenied, tr("Channel nesting limit reached."));
			}
			break;
		default: {
				if (msg.has_reason())
					g.l->log(Log::PermissionDenied, tr("Denied: %1.").arg(u8(msg.reason())));
				else
					g.l->log(Log::PermissionDenied, tr("Permission denied."));
			}
			break;
	}
}

void MainWindow::msgUDPTunnel(const MumbleProto::UDPTunnel &) {
}

void MainWindow::msgUserState(const MumbleProto::UserState &msg) {
	ACTOR_INIT;
	SELF_INIT;
	ClientUser *pDst = ClientUser::get(msg.session());
	bool bNewUser = false;

	if (! pDst) {
		if (msg.has_name()) {
			pDst = pmModel->addUser(msg.session(), u8(msg.name()));
			bNewUser = true;
		} else {
			return;
		}
	}

	if (msg.has_user_id())
		pmModel->setUserId(pDst, msg.user_id());

	if (msg.has_hash()) {
		pmModel->setHash(pDst, u8(msg.hash()));
		const QString &name = Database::getFriend(pDst->qsHash);
		if (! name.isEmpty())
			pmModel->setFriendName(pDst, name);
		if (Database::isLocalMuted(pDst->qsHash))
			pDst->setLocalMute(true);
		if (Database::isLocalIgnored(pDst->qsHash))
			pDst->setLocalIgnore(true);
	}

	if (bNewUser)
		g.l->log(Log::UserJoin, tr("%1 connected.").arg(Log::formatClientUser(pDst, Log::Source)));

	if (msg.has_self_deaf() || msg.has_self_mute()) {
		if (msg.has_self_mute())
			pDst->setSelfMute(msg.self_mute());
		if (msg.has_self_deaf())
			pDst->setSelfDeaf(msg.self_deaf());

		if (pSelf && pDst != pSelf && (pDst->cChannel == pSelf->cChannel)) {
			QString name = pDst->qsName;
			if (pDst->bSelfMute && pDst->bSelfDeaf)
				g.l->log(Log::OtherSelfMute, tr("%1 is now muted and deafened.").arg(Log::formatClientUser(pDst, Log::Target)));
			else if (pDst->bSelfMute)
				g.l->log(Log::OtherSelfMute, tr("%1 is now muted.").arg(Log::formatClientUser(pDst, Log::Target)));
			else
				g.l->log(Log::OtherSelfMute, tr("%1 is now unmuted.").arg(Log::formatClientUser(pDst, Log::Target)));
		}
	}

	if (msg.has_recording()) {
		pDst->setRecording(msg.recording());

		// Do nothing during initial sync
		if (pSelf) {
			if (pDst == pSelf) {
				if (pDst->bRecording) {
					g.l->log(Log::Recording, tr("Recording started"));
				} else {
					g.l->log(Log::Recording, tr("Recording stopped"));
				}
			} else if (pDst->cChannel->allLinks().contains(pSelf->cChannel)) {
				if (pDst->bRecording) {
					g.l->log(Log::Recording, tr("%1 started recording.").arg(Log::formatClientUser(pDst, Log::Source)));
				} else {
					g.l->log(Log::Recording, tr("%1 stopped recording.").arg(Log::formatClientUser(pDst, Log::Source)));
				}
			}
		}
	}

	if (msg.has_deaf() || msg.has_mute() || msg.has_suppress() || msg.has_priority_speaker()) {
		if (msg.has_mute())
			pDst->setMute(msg.mute());
		if (msg.has_deaf())
			pDst->setDeaf(msg.deaf());
		if (msg.has_suppress())
			pDst->setSuppress(msg.suppress());
		if (msg.has_priority_speaker())
			pDst->setPrioritySpeaker(msg.priority_speaker());

		if (pSelf && ((pDst->cChannel == pSelf->cChannel) || (pSrc == pSelf))) {
			if (pDst == pSelf) {
				if (msg.has_mute() && msg.has_deaf() && pDst->bMute && pDst->bDeaf) {
					g.l->log(Log::YouMuted, tr("You were muted and deafened by %1.").arg(Log::formatClientUser(pSrc, Log::Source)));
				} else if (msg.has_mute() && msg.has_deaf() && !pDst->bMute && !pDst->bDeaf) {
					g.l->log(Log::YouMuted, tr("You were unmuted and undeafened by %1.").arg(Log::formatClientUser(pSrc, Log::Source)));
				} else {
					if (msg.has_mute()) {
						if (pDst->bMute)
							g.l->log(Log::YouMuted, tr("You were muted by %1.").arg(Log::formatClientUser(pSrc, Log::Source)));
						else
							g.l->log(Log::YouMuted, tr("You were unmuted by %1.").arg(Log::formatClientUser(pSrc, Log::Source)));
					}

					if (msg.has_deaf()) {
						if (!pDst->bDeaf)
							g.l->log(Log::YouMuted, tr("You were undeafened by %1.").arg(Log::formatClientUser(pSrc, Log::Source)));
					}
				}

				if (msg.has_suppress()) {
					if (pDst->bSuppress)
						g.l->log(Log::YouMuted, tr("You were suppressed."));
					else {
						if (msg.has_channel_id())
							g.l->log(Log::YouMuted, tr("You were unsuppressed."));
						else
							g.l->log(Log::YouMuted, tr("You were unsuppressed by %1.").arg(Log::formatClientUser(pSrc, Log::Source)));
					}
				}

				updateTrayIcon();
			} else if (pSrc == pSelf) {
				if (msg.has_mute() && msg.has_deaf() && pDst->bMute && pDst->bDeaf) {
					g.l->log(Log::YouMutedOther, tr("You muted and deafened %1.").arg(Log::formatClientUser(pDst, Log::Target)));
				} else if (msg.has_mute() && msg.has_deaf() && !pDst->bMute && !pDst->bDeaf) {
					g.l->log(Log::YouMutedOther, tr("You unmuted and undeafened %1.").arg(Log::formatClientUser(pDst, Log::Target)));
				} else {
					if (msg.has_mute()) {
						if (pDst->bMute)
							g.l->log(Log::YouMutedOther, tr("You muted %1.").arg(Log::formatClientUser(pDst, Log::Target)));
						else
							g.l->log(Log::YouMutedOther, tr("You unmuted %1.").arg(Log::formatClientUser(pDst, Log::Target)));
					}

					if (msg.has_deaf()) {
						if (!pDst->bDeaf)
							g.l->log(Log::YouMutedOther, tr("You undeafened %1.").arg(Log::formatClientUser(pDst, Log::Target)));
					}
				}

				if (msg.has_suppress()) {
					if (! msg.has_channel_id()) {
						if (pDst->bSuppress)
							g.l->log(Log::YouMutedOther, tr("You suppressed %1.").arg(Log::formatClientUser(pDst, Log::Target)));
						else
							g.l->log(Log::YouMutedOther, tr("You unsuppressed %1.").arg(Log::formatClientUser(pDst, Log::Target)));
					}
				}
			} else {
				if (msg.has_mute() && msg.has_deaf() && pDst->bMute && pDst->bDeaf) {
					g.l->log(Log::OtherMutedOther, tr("%1 muted and deafened by %2.").arg(Log::formatClientUser(pDst, Log::Target), Log::formatClientUser(pSrc, Log::Source)));
				} else if (msg.has_mute() && msg.has_deaf() && !pDst->bMute && !pDst->bDeaf) {
					g.l->log(Log::OtherMutedOther, tr("%1 unmuted and undeafened by %2.").arg(Log::formatClientUser(pDst, Log::Target), Log::formatClientUser(pSrc, Log::Source)));
				} else {
					if (msg.has_mute()) {
						if (pDst->bMute)
							g.l->log(Log::OtherMutedOther, tr("%1 muted by %2.").arg(Log::formatClientUser(pDst, Log::Target), Log::formatClientUser(pSrc, Log::Source)));
						else
							g.l->log(Log::OtherMutedOther, tr("%1 unmuted by %2.").arg(Log::formatClientUser(pDst, Log::Target), Log::formatClientUser(pSrc, Log::Source)));
					}

					if (msg.has_deaf()) {
						if (!pDst->bDeaf)
							g.l->log(Log::OtherMutedOther, tr("%1 undeafened by %2.").arg(Log::formatClientUser(pDst, Log::Target), Log::formatClientUser(pSrc, Log::Source)));
					}
				}

				if (msg.has_suppress()) {
					if (! msg.has_channel_id()) {
						if (pDst->bSuppress)
							g.l->log(Log::OtherMutedOther, tr("%1 suppressed by %2.").arg(Log::formatClientUser(pDst, Log::Target), Log::formatClientUser(pSrc, Log::Source)));
						else
							g.l->log(Log::OtherMutedOther, tr("%1 unsuppressed by %2.").arg(Log::formatClientUser(pDst, Log::Target), Log::formatClientUser(pSrc, Log::Source)));
					}
				}
			}
		}
	}

	if (msg.has_channel_id()) {
		Channel *c = Channel::get(msg.channel_id());
		if (!c) {
			qWarning("MessageUserMove for unknown channel.");
			c = Channel::get(0);
		}

		Channel *old = pDst->cChannel;
		if (c != old) {
			bool log = pSelf && !((pDst == pSelf) && (pSrc == pSelf));

			if (log) {
				if (pDst == pSelf) {
					g.l->log(Log::ChannelJoin, tr("You were moved to %1 by %2.").arg(Log::formatChannel(c)).arg(Log::formatClientUser(pSrc, Log::Source)));
				} else if (pDst->cChannel == pSelf->cChannel) {
					if (pDst == pSrc)
						g.l->log(Log::ChannelLeave, tr("%1 moved to %2.").arg(Log::formatClientUser(pDst, Log::Target)).arg(Log::formatChannel(c)));
					else
						g.l->log(Log::ChannelLeave, tr("%1 moved to %2 by %3.").arg(Log::formatClientUser(pDst, Log::Target)).arg(Log::formatChannel(c)).arg(Log::formatClientUser(pSrc, Log::Source)));
				}
			}

			pmModel->moveUser(pDst, c);

			if (pDst == pSelf) {
				g.mw->updateChatBar();
			}

			if (log && (pDst != pSelf) && (pDst->cChannel == pSelf->cChannel)) {
				if (pDst == pSrc)
					g.l->log(Log::ChannelJoin, tr("%1 entered channel.").arg(Log::formatClientUser(pDst, Log::Target)));
				else
					g.l->log(Log::ChannelJoin, tr("%1 moved in from %2 by %3.").arg(Log::formatClientUser(pDst, Log::Target)).arg(Log::formatChannel(old)).arg(Log::formatClientUser(pSrc, Log::Source)));

				if (pDst->bRecording)
					g.l->log(Log::Recording, tr("%1 is recording").arg(Log::formatClientUser(pDst, Log::Target)));
			}
		}
	}
	if (msg.has_name()) {
		pmModel->renameUser(pDst, u8(msg.name()));
	}
	if (msg.has_texture_hash()) {
		pDst->qbaTextureHash = blob(msg.texture_hash());
		pDst->qbaTexture = QByteArray();
		g.o->verifyTexture(pDst);
	}
	if (msg.has_texture()) {
		pDst->qbaTexture = blob(msg.texture());
		if (pDst->qbaTexture.isEmpty()) {
			pDst->qbaTextureHash = QByteArray();
		} else {
			pDst->qbaTextureHash = sha1(pDst->qbaTexture);
			Database::setBlob(pDst->qbaTextureHash, pDst->qbaTexture);
		}
		g.o->verifyTexture(pDst);
	}
	if (msg.has_comment_hash())
		pmModel->setCommentHash(pDst, blob(msg.comment_hash()));
	if (msg.has_comment())
		pmModel->setComment(pDst, u8(msg.comment()));
}

void MainWindow::msgUserRemove(const MumbleProto::UserRemove &msg) {
	VICTIM_INIT;
	ACTOR_INIT;
	SELF_INIT;

	QString reason = u8(msg.reason());

	if (pDst == pSelf) {
		if (msg.ban())
			g.l->log(Log::YouKicked, tr("You were kicked and banned from the server by %1: %2.").arg(Log::formatClientUser(pSrc, Log::Source)).arg(reason));
		else
			g.l->log(Log::YouKicked, tr("You were kicked from the server by %1: %2.").arg(Log::formatClientUser(pSrc, Log::Source)).arg(reason));
	} else if (pSrc) {
		if (msg.ban())
			g.l->log((pSrc == pSelf) ? Log::YouKicked : Log::UserKicked, tr("%3 was kicked and banned from the server by %1: %2.").arg(Log::formatClientUser(pSrc, Log::Source)).arg(reason).arg(Log::formatClientUser(pDst, Log::Target)));
		else
			g.l->log((pSrc == pSelf) ? Log::YouKicked : Log::UserKicked, tr("%3 was kicked from the server by %1: %2.").arg(Log::formatClientUser(pSrc, Log::Source)).arg(reason).arg(Log::formatClientUser(pDst, Log::Target)));
	} else {
		if(pDst->cChannel == pSelf->cChannel)
			g.l->log(Log::ChannelLeave, tr("%1 left channel.").arg(Log::formatClientUser(pDst, Log::Source)));
		g.l->log(Log::UserLeave, tr("%1 disconnected.").arg(Log::formatClientUser(pDst, Log::Source)));
	}
	if (pDst != pSelf)
		pmModel->removeUser(pDst);
}

void MainWindow::msgChannelState(const MumbleProto::ChannelState &msg) {
	if (! msg.has_channel_id())
		return;

	Channel *c = Channel::get(msg.channel_id());
	Channel *p = msg.has_parent() ? Channel::get(msg.parent()) : NULL;

	if (!c) {
		// Addresses channel does not exist so create it
		if (p && msg.has_name()) {
			c = pmModel->addChannel(msg.channel_id(), p, u8(msg.name()));
			c->bTemporary = msg.temporary();
			p = NULL; // No need to move it later

			ServerHandlerPtr sh = g.sh;
			if (sh)
				c->bFiltered = Database::isChannelFiltered(sh->qbaDigest, c->iId);

		} else {
			qWarning("Server attempted state change on nonexistent channel");
			return;
		}
	}

	if (p) {
		// Channel move
		Channel *pp = p;
		while (pp) {
			if (pp == c) {
				qWarning("Server asked to move a channel into itself or one of its children");
				return;
			}

			pp = pp->cParent;
		}
		pmModel->moveChannel(c, p);
	}

	if (msg.has_name())
		pmModel->renameChannel(c, u8(msg.name()));

	if (msg.has_description_hash())
		pmModel->setCommentHash(c, blob(msg.description_hash()));
	if (msg.has_description())
		pmModel->setComment(c, u8(msg.description()));

	if (msg.has_position()) {
		pmModel->repositionChannel(c, msg.position());
	}

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
	if (c && (c->iId != 0)) {
		if (c->bFiltered) {
			ServerHandlerPtr sh = g.sh;
			if (sh)
				Database::setChannelFiltered(sh->qbaDigest, c->iId, false);
			c->bFiltered = false;
		}
		pmModel->removeChannel(c);
	}
}

void MainWindow::msgTextMessage(const MumbleProto::TextMessage &msg) {
	ACTOR_INIT;
	QString target;

	// Silently drop the message if this user is set to "ignore"
	if (pSrc && pSrc->bLocalIgnore)
		return;

	const QString &plainName = pSrc ? pSrc->qsName : tr("Server", "message from");
	const QString &name = pSrc ? Log::formatClientUser(pSrc, Log::Source) : tr("Server", "message from");

	if (msg.tree_id_size() > 0) {
		target += tr("(Tree) ");
	} else if (msg.channel_id_size() > 0) {
		target += tr("(Channel) ");
	}

	g.l->log(Log::TextMessage, tr("%2%1: %3").arg(name).arg(target).arg(u8(msg.message())),
	         tr("Message from %1").arg(plainName));
}

void MainWindow::msgACL(const MumbleProto::ACL &msg) {
	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = NULL;
	}
	if (Channel::get(msg.channel_id())) {
		aclEdit = new ACLEditor(msg.channel_id(), msg, this);
		aclEdit->show();
	}
}

void MainWindow::msgQueryUsers(const MumbleProto::QueryUsers &msg) {
	if (aclEdit)
		aclEdit->returnQuery(msg);
}

void MainWindow::msgPing(const MumbleProto::Ping &) {
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

void MainWindow::msgContextActionModify(const MumbleProto::ContextActionModify &msg) {
	if (msg.has_operation() && msg.operation() == MumbleProto::ContextActionModify_Operation_Remove) {
		removeContextAction(msg);
		return;
	}

	if (msg.has_operation() && msg.operation() != MumbleProto::ContextActionModify_Operation_Add)
		return;

	QAction *a = new QAction(u8(msg.text()), g.mw);
	a->setData(u8(msg.action()));
	connect(a, SIGNAL(triggered()), this, SLOT(context_triggered()));
	unsigned int ctx = msg.context();
	if (ctx & MumbleProto::ContextActionModify_Context_Server)
		qlServerActions.append(a);
	if (ctx & MumbleProto::ContextActionModify_Context_User)
		qlUserActions.append(a);
	if (ctx & MumbleProto::ContextActionModify_Context_Channel)
		qlChannelActions.append(a);
}

void MainWindow::removeContextAction(const MumbleProto::ContextActionModify &msg) {
	QString action = u8(msg.action());

	QSet<QAction *> qs;
	qs += qlServerActions.toSet();
	qs += qlChannelActions.toSet();
	qs += qlUserActions.toSet();

	foreach(QAction *a, qs) {
		if (a->data() == action) {
			qlServerActions.removeOne(a);
			qlChannelActions.removeOne(a);
			qlUserActions.removeOne(a);
			delete a;
		}
	}
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
		if (msg.has_os_displayable_version())
			g.sh->qsOSDisplayableVersion = u8(msg.os_displayable_version());
	}
}

void MainWindow::msgUserList(const MumbleProto::UserList &msg) {
	if (userEdit) {
		userEdit->reject();
		delete userEdit;
		userEdit = NULL;
	}
	userEdit = new UserEdit(msg, this);
	userEdit->show();
}

void MainWindow::msgVoiceTarget(const MumbleProto::VoiceTarget &) {
}

void MainWindow::msgPermissionQuery(const MumbleProto::PermissionQuery &msg) {
	Channel *current = pmModel->getChannel(qtvUsers->currentIndex());

	if (msg.flush()) {
		foreach(Channel *c, Channel::c_qhChannels)
			c->uiPermissions = 0;

		// We always need the permissions of the current focus channel
		if (current && current->iId != static_cast<int>(msg.channel_id())) {
			g.sh->requestChannelPermissions(current->iId);

			current->uiPermissions = ChanACL::All;
		}
	}
	Channel *c = Channel::get(msg.channel_id());
	if (c) {
		c->uiPermissions = msg.permissions();
		if (c->iId == 0)
			g.pPermissions = static_cast<ChanACL::Permissions>(c->uiPermissions);
		if (c == current) {
			updateMenuPermissions();
		}
	}
}

void MainWindow::msgCodecVersion(const MumbleProto::CodecVersion &msg) {
	int alpha = msg.has_alpha() ? msg.alpha() : -1;
	int beta = msg.has_beta() ? msg.beta() : -1;
	bool pref = msg.prefer_alpha();

#ifdef USE_OPUS
	g.bOpus = msg.opus();
#endif

	// Workaround for broken 1.2.2 servers
	if (g.sh && g.sh->uiVersion == 0x010202 && alpha != -1 && alpha == beta) {
		if (pref)
			beta = g.iCodecBeta;
		else
			alpha = g.iCodecAlpha;
	}

	if ((alpha != -1) && (alpha != g.iCodecAlpha)) {
		g.iCodecAlpha = alpha;
		if (pref && ! g.qmCodecs.contains(alpha))
			pref = ! pref;
	}
	if ((beta != -1) && (beta != g.iCodecBeta)) {
		g.iCodecBeta = beta;
		if (! pref && ! g.qmCodecs.contains(beta))
			pref = ! pref;
	}
	g.bPreferAlpha = pref;

	int willuse = pref ? g.iCodecAlpha : g.iCodecBeta;

	static bool warned = false;

	if (! g.qmCodecs.contains(willuse)) {
		if (! warned) {
			g.l->log(Log::CriticalError, tr("Unable to find matching CELT codecs with other clients. You will not be able to talk to all users."));
			warned = true;
		}
	} else {
		warned = false;
	}
}

void MainWindow::msgUserStats(const MumbleProto::UserStats &msg) {
	UserInformation *ui = qmUserInformations.value(msg.session());
	if (ui) {
		ui->update(msg);
	} else {
		ui = new UserInformation(msg, g.ocIntercept ? g.mw : NULL);
		ui->setAttribute(Qt::WA_DeleteOnClose, true);
		connect(ui, SIGNAL(destroyed()), this, SLOT(destroyUserInformation()));

		qmUserInformations.insert(msg.session(), ui);
		ui->show();
	}
}

void MainWindow::msgRequestBlob(const MumbleProto::RequestBlob &) {
}

void MainWindow::msgSuggestConfig(const MumbleProto::SuggestConfig &msg) {
	if (msg.has_version() && (msg.version() > MumbleVersion::getRaw())) {
		g.l->log(Log::Warning, tr("The server requests minimum client version %1").arg(MumbleVersion::toString(msg.version())));
	}
	if (msg.has_positional() && (msg.positional() != g.s.doPositionalAudio())) {
		if (msg.positional())
			g.l->log(Log::Warning, tr("The server requests positional audio be enabled."));
		else
			g.l->log(Log::Warning, tr("The server requests positional audio be disabled."));
	}
	if (msg.has_push_to_talk() && (msg.push_to_talk() != (g.s.atTransmit == Settings::PushToTalk))) {
		if (msg.push_to_talk())
			g.l->log(Log::Warning, tr("The server requests Push-to-Talk be enabled."));
		else
			g.l->log(Log::Warning, tr("The server requests Push-to-Talk be disabled."));
	}
}
