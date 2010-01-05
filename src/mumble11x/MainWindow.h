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

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include "mumble_pch.hpp"

#define TI_QEVENT (QEvent::User + 938)
#define MB_QEVENT (QEvent::User + 939)

class ACLEditor;
class BanEditor;
class ServerHandler;
class GlobalShortcut;
class TextToSpeech;
class PlayerModel;

#include "Message.h"
#include "Usage.h"
#include "ui_MainWindow.h"

class MessageBoxEvent : public QEvent {
	public:
		QString msg;
		MessageBoxEvent(QString msg);
};

class LogTitleBar : public QWidget {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LogTitleBar)
	protected:
		QTimer *qtTick;
		int size;
		int newsize;
	public:
		LogTitleBar();
		QSize sizeHint() const;
		QSize minimumSizeHint() const;
	public slots:
		void tick();
	protected:
		bool eventFilter(QObject *, QEvent *);
};

class MainWindow : public QMainWindow, public MessageHandler, public Ui::MainWindow {
		friend class PlayerModel;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(MainWindow)
	public:
		PlayerModel *pmModel;
		QSystemTrayIcon *qstiIcon;
		QMenu *qmTray;
		QIcon qiIcon, qiIconMute, qiIconDeaf;

		GlobalShortcut *gsPushTalk, *gsResetAudio, *gsMuteSelf, *gsDeafSelf;
		GlobalShortcut *gsUnlink, *gsCenterPos, *gsPushMute, *gsMetaChannel, *gsToggleOverlay;
		GlobalShortcut *gsAltTalk, *gsMinimal, *gsVolumeUp, *gsVolumeDown;
		LogTitleBar *ltbDockTitle;

		ACLEditor *aclEdit;
		BanEditor *banEdit;

		MessageServerReject::RejectType rtLast;
		QString qsDesiredChannel;

		void recheckTTS();
		void msgBox(QString msg);
		void setOnTop(bool top);
		void updateTrayIcon();

#ifdef Q_OS_WIN
		unsigned int uiNewHardware;
#endif
	protected:
		Usage uUsage;
		QTimer *qtReconnect;

		QList<QAction *> qlServerActions;
		QList<QAction *> qlChannelActions;
		QList<QAction *> qlPlayerActions;

		void createActions();
		void setupGui();
		void customEvent(QEvent *evt);
		void findDesiredChannel();
		void setupView(bool toggle_minimize = true);
		bool bNoHide;
		virtual void closeEvent(QCloseEvent *e);
		virtual void hideEvent(QHideEvent *e);
	public slots:
		void on_qtvPlayers_customContextMenuRequested(const QPoint &pos);
		void on_qtvPlayers_doubleClicked(const QModelIndex &idx);
		void on_qmServer_aboutToShow();
		void on_qaServerConnect_triggered();
		void on_qaServerDisconnect_triggered();
		void on_qaServerBanList_triggered();
		void on_qaServerInformation_triggered();
		void on_qmPlayer_aboutToShow();
		void on_qaPlayerKick_triggered();
		void on_qaPlayerBan_triggered();
		void on_qaPlayerMute_triggered();
		void on_qaPlayerDeaf_triggered();
		void on_qaPlayerLocalMute_triggered();
		void on_qaPlayerTextMessage_triggered();
		void on_qmChannel_aboutToShow();
		void on_qaChannelAdd_triggered();
		void on_qaChannelRemove_triggered();
		void on_qaChannelACL_triggered();
		void on_qaChannelRename_triggered();
		void on_qaChannelDescUpdate_triggered();
		void on_qaChannelLink_triggered();
		void on_qaChannelUnlink_triggered();
		void on_qaChannelUnlinkAll_triggered();
		void on_qaChannelSendMessage_triggered();
		void on_qaChannelSendTreeMessage_triggered();
		void on_qaAudioReset_triggered();
		void on_qaAudioMute_triggered();
		void on_qaAudioDeaf_triggered();
		void on_qaAudioTTS_triggered();
		void on_qaAudioUnlink_triggered();
		void on_qaAudioStats_triggered();
		void on_qaConfigDialog_triggered();
		void on_qaConfigHideFrame_triggered();
		void on_qmConfig_aboutToShow();
		void on_qaConfigMinimal_triggered();
		void on_qaAudioWizard_triggered();
		void on_qaHelpWhatsThis_triggered();
		void on_qaHelpAbout_triggered();
		void on_qaHelpAboutSpeex_triggered();
		void on_qaHelpAboutQt_triggered();
		void on_qaHelpVersionCheck_triggered();
		void on_qaQuit_triggered();
		void on_qteLog_customContextMenuRequested(const QPoint &pos);
		void on_qteLog_anchorClicked(const QUrl &);
		void on_qteLog_highlighted(const QUrl & link);
		void on_PushToTalk_triggered(bool);
		void on_PushToMute_triggered(bool);
		void on_AltPushToTalk_triggered(bool);
		void on_CenterPos_triggered(bool);
		void on_VolumeUp_triggered(bool);
		void on_VolumeDown_triggered(bool);
		void on_Reconnect_timeout();
		void on_Icon_activated(QSystemTrayIcon::ActivationReason);
		void serverConnected();
		void serverDisconnected(QString reason);
		void pushLink(bool down);
		void viewCertificate(bool);
		void openUrl(const QUrl &url);
		void context_triggered();
	public:
		MainWindow(QWidget *parent);
		~MainWindow();

		// From msgHandler. Implementation in Messages.cpp
		virtual void msgSpeex(Connection *, MessageSpeex *);
		virtual void msgServerAuthenticate(Connection *, MessageServerAuthenticate *);
		virtual void msgPing(Connection *, MessagePing *);
		virtual void msgPingStats(Connection *, MessagePingStats *);
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
		virtual void msgChannelDescUpdate(Connection *, MessageChannelDescUpdate *);
		virtual void msgServerBanList(Connection *, MessageServerBanList *);
		virtual void msgTextMessage(Connection *, MessageTextMessage *);
		virtual void msgPermissionDenied(Connection *, MessagePermissionDenied *);
		virtual void msgEditACL(Connection *, MessageEditACL *);
		virtual void msgQueryUsers(Connection *, MessageQueryUsers *);
		virtual void msgTexture(Connection *, MessageTexture *);
		virtual void msgCryptSetup(Connection *, MessageCryptSetup *);
		virtual void msgCryptSync(Connection *, MessageCryptSync *);
		virtual void msgContextAddAction(Connection *, MessageContextAddAction *);
		virtual void msgContextAction(Connection *, MessageContextAction *);
};

#else
class MainWindow;
#endif
