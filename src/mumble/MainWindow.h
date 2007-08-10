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

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#define TI_QEVENT (QEvent::User + 938)
#define MB_QEVENT (QEvent::User + 939)

class ACLEditor;
class BanEditor;
class ServerHandler;
class GlobalShortcut;
class TextToSpeech;
class PlayerModel;

#include "Message.h"

class MessageBoxEvent : public QEvent {
	public:
		QString msg;
		MessageBoxEvent(QString msg);
};

class MainWindow : public QMainWindow, public MessageHandler {
		friend class PlayerModel;
		Q_OBJECT
	public:
		PlayerModel *pmModel;
		QMenu *qmServer, *qmPlayer, *qmChannel, *qmAudio, *qmConfig, *qmHelp;
		QAction *qaQuit;
		QAction *qaServerConnect, *qaServerDisconnect, *qaServerBanList, *qaServerInformation;
		QAction *qaPlayerKick, *qaPlayerBan, *qaPlayerMute, *qaPlayerDeaf, *qaPlayerLocalMute;
		QAction *qaPlayerTextMessage;
		QAction *qaAudioReset, *qaAudioMute, *qaAudioDeaf, *qaAudioTTS, *qaAudioStats, *qaAudioUnlink;
		QAction *qaConfigDialog, *qaAudioWizard;
		QAction *qaHelpWhatsThis, *qaHelpAbout, *qaHelpAboutSpeex, *qaHelpAboutQt, *qaHelpVersionCheck;
		QAction *qaChannelAdd, *qaChannelRemove, *qaChannelACL, *qaChannelRename, *qaChannelLink, *qaChannelUnlink, *qaChannelUnlinkAll;
		QSplitter *qsSplit;
		QSystemTrayIcon *qstiIcon;
		QTextBrowser *qteLog;
		QTreeView *qtvPlayers;

		GlobalShortcut *gsPushTalk, *gsResetAudio, *gsMuteSelf, *gsDeafSelf;
		GlobalShortcut *gsUnlink, *gsCenterPos, *gsPushMute, *gsMetaChannel, *gsToggleOverlay;
		GlobalShortcut *gsAltTalk;

		ACLEditor *aclEdit;
		BanEditor *banEdit;

		MessageServerReject::RejectType rtLast;
		QString qsDesiredChannel;

		void recheckTTS();
		void appendLog(QString entry);

		void msgBox(QString msg);
	protected:
		QTimer *qtReconnect;
		void createActions();
		void setupGui();
		void customEvent(QEvent *evt);
		virtual void closeEvent(QCloseEvent *e);
		virtual void hideEvent(QHideEvent *e);
	public slots:
		void on_Players_customContextMenuRequested(const QPoint &pos);
		void on_Players_doubleClicked(const QModelIndex &idx);
		void on_ServerConnect_triggered();
		void on_ServerDisconnect_triggered();
		void on_ServerBanList_triggered();
		void on_ServerInformation_triggered();
		void on_PlayerMenu_aboutToShow();
		void on_PlayerKick_triggered();
		void on_PlayerBan_triggered();
		void on_PlayerMute_triggered();
		void on_PlayerDeaf_triggered();
		void on_PlayerLocalMute_triggered();
		void on_PlayerTextMessage_triggered();
		void on_ChannelMenu_aboutToShow();
		void on_ChannelAdd_triggered();
		void on_ChannelRemove_triggered();
		void on_ChannelACL_triggered();
		void on_ChannelRename_triggered();
		void on_ChannelLink_triggered();
		void on_ChannelUnlink_triggered();
		void on_ChannelUnlinkAll_triggered();
		void on_AudioReset_triggered();
		void on_AudioMute_triggered();
		void on_AudioDeaf_triggered();
		void on_AudioTextToSpeech_triggered();
		void on_AudioUnlink_triggered();
		void on_AudioStats_triggered();
		void on_ConfigDialog_triggered();
		void on_AudioWizard_triggered();
		void on_HelpWhatsThis_triggered();
		void on_HelpAbout_triggered();
		void on_HelpAboutSpeex_triggered();
		void on_HelpAboutQt_triggered();
		void on_HelpVersionCheck_triggered();
		void on_Quit_triggered();
		void on_PushToTalk_triggered(bool);
		void on_PushToMute_triggered(bool);
		void on_AltPushToTalk_triggered(bool);
		void on_CenterPos_triggered(bool);
		void on_Reconnect_timeout();
		void on_Icon_activated(QSystemTrayIcon::ActivationReason);
		void on_Log_anchorClicked(const QUrl &);
		void serverConnected();
		void serverDisconnected(QString reason);
		void pushLink(bool down);
		void viewCertificate(bool);
		void openUrl(const QUrl &url);
	public:
		MainWindow(QWidget *parent);

		// From msgHandler. Implementation in Messages.cpp
		virtual void msgSpeex(Connection *, MessageSpeex *);
		virtual void msgServerAuthenticate(Connection *, MessageServerAuthenticate *);
		virtual void msgPing(Connection *, MessagePing *);
		virtual void msgServerReject(Connection *, MessageServerReject *);
		virtual void msgServerSync(Connection *, MessageServerSync *);
		virtual void msgServerJoin(Connection *, MessageServerJoin *);
		virtual void msgServerLeave(Connection *, MessageServerLeave *);
		virtual void msgPlayerMute(Connection *, MessagePlayerMute *);
		virtual void msgPlayerDeaf(Connection *, MessagePlayerDeaf *);
		virtual void msgPlayerSelfMuteDeaf(Connection *, MessagePlayerSelfMuteDeaf *);
		virtual void msgPlayerKick(Connection *, MessagePlayerKick *);
		virtual void msgPlayerBan(Connection *, MessagePlayerBan *);
		virtual void msgPlayerMove(Connection *, MessagePlayerMove *);
		virtual void msgPlayerRename(Connection *, MessagePlayerRename *);
		virtual void msgChannelAdd(Connection *, MessageChannelAdd *);
		virtual void msgChannelRemove(Connection *, MessageChannelRemove *);
		virtual void msgChannelMove(Connection *, MessageChannelMove *);
		virtual void msgChannelLink(Connection *, MessageChannelLink *);
		virtual void msgChannelRename(Connection *, MessageChannelRename *);
		virtual void msgServerBanList(Connection *, MessageServerBanList *);
		virtual void msgTextMessage(Connection *, MessageTextMessage *);
		virtual void msgPermissionDenied(Connection *, MessagePermissionDenied *);
		virtual void msgEditACL(Connection *, MessageEditACL *);
		virtual void msgQueryUsers(Connection *, MessageQueryUsers *);
		virtual void msgTexture(Connection *, MessageTexture *);
		virtual void msgCryptSetup(Connection *, MessageCryptSetup *);
		virtual void msgCryptSync(Connection *, MessageCryptSync *);

};

#else
class MainWindow;
#endif
