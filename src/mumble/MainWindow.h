// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_MAINWINDOW_H_
#define MUMBLE_MUMBLE_MAINWINDOW_H_

#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
# include <QtCore/QPointer>
# include <QtWidgets/QMainWindow>
# include <QtWidgets/QSystemTrayIcon>
#else
# include <QtCore/QWeakPointer>
# include <QtGui/QMainWindow>
# include <QtGui/QSystemTrayIcon>
#endif

#include <QtNetwork/QAbstractSocket>

#include "CustomElements.h"
#include "Message.h"
#include "Mumble.pb.h"
#include "Usage.h"
#include "UserLocalVolumeDialog.h"
#include "MUComboBox.h"

#include "ui_MainWindow.h"

#define MB_QEVENT (QEvent::User + 939)
#define OU_QEVENT (QEvent::User + 940)

class ACLEditor;
class BanEditor;
class UserEdit;
class ServerHandler;
class GlobalShortcut;
class TextToSpeech;
class UserModel;
class Tokens;
class Channel;
class UserInformation;
class VoiceRecorderDialog;
class PTTButtonWidget;

struct ShortcutTarget;

class MessageBoxEvent : public QEvent {
	public:
		QString msg;
		MessageBoxEvent(QString msg);
};

class OpenURLEvent : public QEvent {
	public:
		QUrl url;
		OpenURLEvent(QUrl url);
};

class MainWindow : public QMainWindow, public MessageHandler, public Ui::MainWindow {
		friend class UserModel;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(MainWindow)
	public:
		UserModel *pmModel;
		QSystemTrayIcon *qstiIcon;
		QMenu *qmUser;
		QMenu *qmChannel;
		QMenu *qmDeveloper;
		QMenu *qmTray;
		QIcon qiIcon, qiIconMutePushToMute, qiIconMuteSelf, qiIconMuteServer, qiIconDeafSelf, qiIconDeafServer, qiIconMuteSuppressed;
		QIcon qiTalkingOn, qiTalkingWhisper, qiTalkingShout, qiTalkingOff;
		QMap<unsigned int, UserLocalVolumeDialog *> qmUserVolTracker;

		GlobalShortcut *gsPushTalk, *gsResetAudio, *gsMuteSelf, *gsDeafSelf;
		GlobalShortcut *gsUnlink, *gsPushMute, *gsJoinChannel, *gsToggleOverlay;
		GlobalShortcut *gsMinimal, *gsVolumeUp, *gsVolumeDown, *gsWhisper, *gsLinkChannel;
		GlobalShortcut *gsCycleTransmitMode, *gsTransmitModePushToTalk, *gsTransmitModeContinuous, *gsTransmitModeVAD;
		GlobalShortcut *gsSendTextMessage, *gsSendClipboardTextMessage;
		DockTitleBar *dtbLogDockTitle, *dtbChatDockTitle;

		ACLEditor *aclEdit;
		BanEditor *banEdit;
		UserEdit *userEdit;
		Tokens *tokenEdit;

		VoiceRecorderDialog *voiceRecorderDialog;

		MumbleProto::Reject_RejectType rtLast;
		bool bRetryServer;
		QString qsDesiredChannel;

		bool bSuppressAskOnQuit;
		/// Restart the client after shutdown
		bool restartOnQuit;
		bool bAutoUnmute;

		/// Contains the cursor whose position is immediately before the image to
		/// save when activating the "Save Image As..." context menu item.
		QTextCursor qtcSaveImageCursor;

#if QT_VERSION >= 0x050000
		QPointer<Channel> cContextChannel;
		QPointer<ClientUser> cuContextUser;
#else
		QWeakPointer<Channel> cContextChannel;
		QWeakPointer<ClientUser> cuContextUser;
#endif

		QPoint qpContextPosition;

		void recheckTTS();
		void msgBox(QString msg);
		void setOnTop(bool top);
		void setShowDockTitleBars(bool doShow);
		void updateTrayIcon();
		void updateUserModel();
		void focusNextMainWidget();
		void updateTransmitModeComboBox();
		QPair<QByteArray, QImage> openImageFile();
		
		void updateChatBar();
		void openTextMessageDialog(ClientUser *p);
		void openUserLocalVolumeDialog(ClientUser *p);

#ifdef Q_OS_WIN
#if QT_VERSION >= 0x050000
		bool nativeEvent(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;
#else
		bool winEvent(MSG *, long *) Q_DECL_OVERRIDE;
#endif
		unsigned int uiNewHardware;
#endif
	protected:
		Usage uUsage;
		QTimer *qtReconnect;

		QList<QAction *> qlServerActions;
		QList<QAction *> qlChannelActions;
		QList<QAction *> qlUserActions;

		QHash<ShortcutTarget, int> qmCurrentTargets;
		QHash<QList<ShortcutTarget>, int> qmTargets;
		QMap<int, int> qmTargetUse;
		Channel *mapChannel(int idx) const;
		int iTargetCounter;
		QMap<unsigned int, UserInformation *> qmUserInformations;

		PTTButtonWidget *qwPTTButtonWidget;

		MUComboBox *qcbTransmitMode;
		QAction *qaTransmitMode;
		QAction *qaTransmitModeSeparator;

		void createActions();
		void setupGui();
		void updateWindowTitle();
		/// updateToolbar updates the state of the toolbar depending on the current
		/// window layout setting.
		/// If the window layout setting is 'custom', the toolbar is made movable. If the
		/// window layout is not 'custom', the toolbar is locked in place at the top of
		/// the MainWindow.
		void updateToolbar();
		void customEvent(QEvent *evt) Q_DECL_OVERRIDE;
		void findDesiredChannel();
		void setupView(bool toggle_minimize = true);
		void closeEvent(QCloseEvent *e) Q_DECL_OVERRIDE;
		void hideEvent(QHideEvent *e) Q_DECL_OVERRIDE;
		void showEvent(QShowEvent *e) Q_DECL_OVERRIDE;
		void changeEvent(QEvent* e) Q_DECL_OVERRIDE;
		void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

		QMenu *createPopupMenu() Q_DECL_OVERRIDE;

		bool handleSpecialContextMenu(const QUrl &url, const QPoint &pos_, bool focus = false);
		Channel* getContextMenuChannel();
		ClientUser* getContextMenuUser();

	public slots:
		void on_qmServer_aboutToShow();
		void on_qaServerConnect_triggered(bool autoconnect = false);
		void on_qaServerDisconnect_triggered();
		void on_qaServerBanList_triggered();
		void on_qaServerUserList_triggered();
		void on_qaServerInformation_triggered();
		void on_qaServerTexture_triggered();
		void on_qaServerTextureRemove_triggered();
		void on_qaServerTokens_triggered();
		void on_qmSelf_aboutToShow();
		void on_qaSelfComment_triggered();
		void on_qaSelfRegister_triggered();
		void qcbTransmitMode_activated(int index);
		void qmUser_aboutToShow();
		void on_qaUserCommentReset_triggered();
		void on_qaUserTextureReset_triggered();
		void on_qaUserCommentView_triggered();
		void on_qaUserKick_triggered();
		void on_qaUserBan_triggered();
		void on_qaUserMute_triggered();
		void on_qaUserDeaf_triggered();
		void on_qaSelfPrioritySpeaker_triggered();
		void on_qaUserPrioritySpeaker_triggered();
		void on_qaUserLocalIgnore_triggered();
		void on_qaUserLocalMute_triggered();
		void on_qaUserLocalVolume_triggered();
		void on_qaUserTextMessage_triggered();
		void on_qaUserRegister_triggered();
		void on_qaUserInformation_triggered();
		void on_qaUserFriendAdd_triggered();
		void on_qaUserFriendRemove_triggered();
		void on_qaUserFriendUpdate_triggered();
		void qmChannel_aboutToShow();
		void on_qaChannelJoin_triggered();
		void on_qaChannelAdd_triggered();
		void on_qaChannelRemove_triggered();
		void on_qaChannelACL_triggered();
		void on_qaChannelLink_triggered();
		void on_qaChannelUnlink_triggered();
		void on_qaChannelUnlinkAll_triggered();
		void on_qaChannelSendMessage_triggered();
		void on_qaChannelFilter_triggered();
		void on_qaChannelCopyURL_triggered();
		void on_qaAudioReset_triggered();
		void on_qaAudioMute_triggered();
		void on_qaAudioDeaf_triggered();
		void on_qaRecording_triggered();
		void on_qaAudioTTS_triggered();
		void on_qaAudioUnlink_triggered();
		void on_qaAudioStats_triggered();
		void on_qaConfigDialog_triggered();
		void on_qaConfigHideFrame_triggered();
		void on_qmConfig_aboutToShow();
		void on_qaConfigMinimal_triggered();
		void on_qaConfigCert_triggered();
		void on_qaAudioWizard_triggered();
		void on_qaDeveloperConsole_triggered();
		void on_qaHelpWhatsThis_triggered();
		void on_qaHelpAbout_triggered();
		void on_qaHelpAboutQt_triggered();
		void on_qaHelpVersionCheck_triggered();
		void on_qaQuit_triggered();
		void on_qaHide_triggered();
		void on_qteChat_tabPressed();
		void on_qteChat_backtabPressed();
		void on_qteChat_ctrlSpacePressed();
		void on_qtvUsers_customContextMenuRequested(const QPoint &mpos);
		void on_qteLog_customContextMenuRequested(const QPoint &pos);
		void on_qteLog_anchorClicked(const QUrl &);
		void on_qteLog_highlighted(const QUrl & link);
		void on_PushToTalk_triggered(bool, QVariant);
		void on_PushToMute_triggered(bool, QVariant);
		void on_VolumeUp_triggered(bool, QVariant);
		void on_VolumeDown_triggered(bool, QVariant);
		void on_gsMuteSelf_down(QVariant);
		void on_gsDeafSelf_down(QVariant);
		void on_gsWhisper_triggered(bool, QVariant);
		void addTarget(ShortcutTarget *);
		void removeTarget(ShortcutTarget *);
		void on_gsCycleTransmitMode_triggered(bool, QVariant);
		void on_gsTransmitModePushToTalk_triggered(bool, QVariant);
		void on_gsTransmitModeContinuous_triggered(bool, QVariant);
		void on_gsTransmitModeVAD_triggered(bool, QVariant);
		void on_gsSendTextMessage_triggered(bool, QVariant);
		void on_gsSendClipboardTextMessage_triggered(bool, QVariant);
		void on_Reconnect_timeout();
		void on_Icon_activated(QSystemTrayIcon::ActivationReason);
		void voiceRecorderDialog_finished(int);
		void qtvUserCurrentChanged(const QModelIndex &, const QModelIndex &);
		void serverConnected();
		void serverDisconnected(QAbstractSocket::SocketError, QString reason);
		void resolverError(QAbstractSocket::SocketError, QString reason);
		void viewCertificate(bool);
		void openUrl(const QUrl &url);
		void context_triggered();
		void updateTarget();
		void updateMenuPermissions();
		/// Handles state changes like talking mode changes and mute/unmute
		/// or priority speaker flag changes for the gui user
		void userStateChanged();
		void destroyUserInformation();
		void trayAboutToShow();
		void sendChatbarMessage(QString msg);
		void pttReleased();
		void whisperReleased(QVariant scdata);
		void onResetAudio();
		void showRaiseWindow();
		void on_qaFilterToggle_triggered();
		/// Opens a save dialog for the image referenced by qtcSaveImageCursor.
		void saveImageAs();
		/// Returns the path to the user's image directory, optionally with a
		/// filename included.
		QString getImagePath(QString filename = QString()) const;
		/// Updates the user's image directory to the given path (any included
		/// filename is discarded).
		void updateImagePath(QString filepath) const;

	public:
		MainWindow(QWidget *parent);
		~MainWindow() Q_DECL_OVERRIDE;

		// From msgHandler. Implementation in Messages.cpp
#define MUMBLE_MH_MSG(x) void msg##x(const MumbleProto:: x &);
		MUMBLE_MH_ALL
#undef MUMBLE_MH_MSG
		void removeContextAction(const MumbleProto::ContextActionModify &msg);
};

#endif
