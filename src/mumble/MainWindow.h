// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_MAINWINDOW_H_
#define MUMBLE_MUMBLE_MAINWINDOW_H_

#include <QtCore/QHash>
#include <QtCore/QPointer>
#include <QtCore/QSet>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QtGlobal>
#include <QtGui/QImage>
#include <QtNetwork/QAbstractSocket>
#include <QtWidgets/QMainWindow>

#include "CustomElements.h"
#include "Log.h"
#include "MUComboBox.h"
#include "ModernShellMenuSerializer.h"
#include "Mumble.pb.h"
#include "MumbleProtocol.h"
#include "QtUtils.h"
#include "Usage.h"
#include "UserLocalNicknameDialog.h"

#include <memory>
#include <optional>
#include <stack>
#include <vector>

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
class ScreenShareManager;
class UserInformation;
class VoiceRecorderDialog;
class PositionalAudioViewer;
class PTTButtonWidget;
struct PersistentChatPreviewSpec;
class PersistentChatGateway;
class PersistentChatController;
class PersistentChatHistoryModel;
class PersistentChatHistoryDelegate;
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
class ModernShellHost;
#endif
class QAction;
class QObject;
class QFrame;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QMenu;
class QTimer;
class QPushButton;
class QToolButton;
class QWidget;

namespace Search {
class SearchDialog;
}

class MenuLabel;
class ListenerVolumeController;
class ListenerVolumeSlider;
class PersistentChatListWidget;
class UserLocalVolumeSlider;

struct ShortcutTarget;

struct ContextMenuTarget {
	ClientUser *user = nullptr;
	Channel *channel = nullptr;
};

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

class MainWindow : public QMainWindow, public Ui::MainWindow {
	friend class UserModel;

private:
	Q_OBJECT
	Q_DISABLE_COPY(MainWindow)
public:
	bool isServerLogViewVisible() const;
	UserModel *pmModel;
	QMenu *qmUser;
	QMenu *qmChannel;
	QMenu *qmListener;
	QMenu *qmDeveloper;
	QIcon qiIcon, qiIconMutePushToMute, qiIconMuteSelf, qiIconMuteServer, qiIconDeafSelf, qiIconDeafServer,
		qiIconMuteSuppressed;
	QIcon qiTalkingOn, qiTalkingWhisper, qiTalkingShout, qiTalkingOff;
	QIcon m_iconInformation;
	std::unordered_map< unsigned int, qt_unique_ptr< UserLocalNicknameDialog > > qmUserNicknameTracker;

	/// "Action" for when there are no actions available
	QAction *qaEmpty;

	GlobalShortcut *gsPushTalk, *gsResetAudio, *gsMuteSelf, *gsDeafSelf;
	GlobalShortcut *gsUnlink, *gsPushMute, *gsJoinChannel;
#ifdef USE_OVERLAY
	GlobalShortcut *gsToggleOverlay;
#endif
	GlobalShortcut *gsMinimal, *gsVolumeUp, *gsVolumeDown, *gsWhisper, *gsLinkChannel, *gsListenChannel;
	GlobalShortcut *gsCycleTransmitMode, *gsToggleMainWindowVisibility, *gsTransmitModePushToTalk,
		*gsTransmitModeContinuous, *gsTransmitModeVAD;
	GlobalShortcut *gsSendTextMessage, *gsSendClipboardTextMessage;
	GlobalShortcut *gsToggleTalkingUI;
	GlobalShortcut *gsToggleSearch;
	GlobalShortcut *gsServerConnect, *gsServerDisconnect, *gsServerInformation, *gsServerTokens;
	GlobalShortcut *gsServerUserList, *gsServerBanList;
	GlobalShortcut *gsSelfPrioritySpeaker;
	GlobalShortcut *gsRecording;
	GlobalShortcut *gsSelfComment, *gsServerTexture, *gsServerTextureRemove;
	GlobalShortcut *gsSelfRegister, *gsAudioStats;
	GlobalShortcut *gsConfigDialog, *gsAudioWizard, *gsConfigCert;
	GlobalShortcut *gsAudioTTS;
	GlobalShortcut *gsHelpAbout, *gsHelpAboutQt, *gsHelpVersionCheck;
	GlobalShortcut *gsTogglePositionalAudio;
	GlobalShortcut *gsMoveBack;
	GlobalShortcut *gsCycleListenerAttenuationMode, *gsListenerAttenuationUp, *gsListenerAttenuationDown;
	GlobalShortcut *gsAdaptivePush;

	DockTitleBar *dtbLogDockTitle, *dtbChatDockTitle;

	ACLEditor *aclEdit;
	BanEditor *banEdit;
	UserEdit *userEdit;
	Tokens *tokenEdit;

	VoiceRecorderDialog *voiceRecorderDialog;

	MumbleProto::Reject_RejectType rtLast;
	bool bRetryServer;
	QString qsDesiredChannel;

	bool forceQuit;
	/// Restart the client after shutdown
	bool restartOnQuit;

	/// Cached copy of the image currently targeted by the log/persistent-chat image actions.
	QImage m_selectedLogImage;
	std::unique_ptr< ScreenShareManager > m_screenShareManager;

	QPointer< Channel > cContextChannel;
	QPointer< ClientUser > cuContextUser;

	QPoint qpContextPosition;

	void recheckTTS();
	void msgBox(QString msg);
	void setOnTop(bool top);
	void setShowDockTitleBars(bool doShow);
	void updateAudioToolTips();
	void updateUserModel();
	void focusNextMainWidget();
	QPair< QByteArray, QImage > openImageFile();
	void setupPersistentChatDock();
	void setupServerNavigator();
	Settings::WindowLayout effectiveWindowLayout() const;
	bool usesModernShell() const;
	void refreshShellLayout();
	void applyShellLayout();
	void activateLegacyShell();
	void activateModernShell();
	void queueModernShellSnapshotSync();
	void syncModernShellSnapshot();
	void beginNativeWindowMoveOrResize();
	void endNativeWindowMoveOrResize();
	void updateServerNavigatorChrome();
	void syncServerNavigatorUserMenu();
	void positionServerNavigatorUserMenu();
	void toggleServerNavigatorUserMenu();
	void closeServerNavigatorUserMenu();
	void refreshCustomChromeStyles();
	void refreshServerNavigatorStyles();
	void refreshServerNavigatorSectionHeights();
	void refreshServerNavigatorMotdHeight();
	void refreshPersistentChatStyles();
	void syncPersistentChatGatewayHandler();

	struct PersistentChatTarget {
		bool valid                   = false;
		bool directMessage           = false;
		bool legacyTextPath          = false;
		bool readOnly                = false;
		bool serverLog               = false;
		ClientUser *user             = nullptr;
		Channel *channel             = nullptr;
		MumbleProto::ChatScope scope = MumbleProto::Channel;
		unsigned int scopeID         = 0;
		QString label;
		QString description;
		QString statusMessage;
	};

	struct PersistentTextChannel {
		unsigned int textChannelID = 0;
		unsigned int aclChannelID  = 0;
		unsigned int position      = 0;
		QString name;
		QString description;
	};

	enum class RoomCreateType : unsigned char { Voice, Text };

	struct PersistentChatPreview {
		QString canonicalUrl;
		QString title;
		QString subtitle;
		QString description;
		QImage thumbnailImage;
		QString openLabel;
		unsigned int previewAssetID = 0;
		bool metadataFinished       = false;
		bool thumbnailFinished      = false;
		bool failed                 = false;
		bool siteSnapshotRequested  = false;
		bool siteSnapshotFinished   = false;
	};

	struct PersistentChatAssetDownload {
		unsigned int assetID = 0;
		quint64 nextOffset   = 0;
		quint64 totalSize    = 0;
		QByteArray bytes;
		QSet< QString > previewKeys;
	};

	struct PersistentChatRenderRequest {
		QString statusMessage;
		bool scrollToBottom         = true;
		bool preserveScrollPosition = false;
	};

	void loadState(bool minimalView);
	void storeState(bool minimalView);

	bool hasPersistentChatCapabilities() const;
	PersistentChatTarget legacyChatTarget() const;
	PersistentChatTarget currentPersistentChatTarget() const;
	void refreshPersistentChatView(bool forceReload = false);
	void requestOlderPersistentChatHistory();
	void setPersistentChatWelcomeText(const QString &message);
	void updatePersistentChatWelcome();
	void clearPersistentChatView(const QString &message, const QString &title = QString(),
								 const QStringList &hints = QStringList());
	void markPersistentChatAvailable(bool refreshUi = true);
	void clearPersistentChatReplyTarget(bool refreshChatBar);
	void setPersistentChatReplyTarget(const std::optional< MumbleProto::ChatMessage > &message);
	std::optional< MumbleProto::ChatEmbedRef >
		persistentChatPrimaryEmbed(const MumbleProto::ChatMessage &message) const;
	std::optional< QString > persistentChatPreviewKey(const MumbleProto::ChatMessage &message) const;
	PersistentChatPreviewSpec persistentChatPreviewSpec(const QString &previewKey) const;
	QString persistentChatScopeLabel(MumbleProto::ChatScope scope, unsigned int scopeID) const;
	void ensurePersistentChatPreview(const QString &previewKey);
	void ensurePersistentChatPreviewAssetDownload(unsigned int assetID, const QString &previewKey);
	void ensurePersistentChatPreviewSiteSnapshot(const QString &previewKey);
	void handlePersistentChatPreviewSiteSnapshotResult(const QString &previewKey, const QImage &image, bool success);
	int persistentChatPreviewContentWidth(int leftPadding) const;
	QString persistentChatPreviewHtml(const QString &previewKey, int availableWidth) const;
	void updatePersistentChatPreviewViewIfVisible(const QString &previewKey);
	void setPersistentChatTargetUsesVoiceTree(bool useVoiceTree);
	bool isServerNavigatorCompactHeight() const;
	void updateServerNavigatorVoiceTreeHeight();
	void updatePersistentChatChannelListHeight();
	void rebuildPersistentChatChannelList();
	void handlePersistentTextChannelSync(const MumbleProto::TextChannelSync &msg);
	void updatePersistentChatScopeSelectorLabels();
	Channel *currentVoiceChannel() const;
	Channel *selectedVoiceTreeChannel() const;
	void focusPersistentChatVoiceChannel(Channel *channel);
	std::size_t cachedPersistentChatUnreadCount(MumbleProto::ChatScope scope, unsigned int scopeID) const;
	void setCachedPersistentChatUnreadCount(MumbleProto::ChatScope scope, unsigned int scopeID,
											unsigned int lastReadMessageID, std::size_t unreadCount);
	std::size_t totalCachedPersistentChatUnreadCount() const;
	bool navigateToPersistentChatScope(MumbleProto::ChatScope scope, unsigned int scopeID);
	bool canCreateVoiceRoom(Channel *channel) const;
	bool canCreateAnyVoiceRoom() const;
	bool voiceRoomCreationForcesTemporary(Channel *channel) const;
	bool canCreateTextRoom() const;
	void createRoom(RoomCreateType preferredType, Channel *preferredVoiceParent = nullptr);
	bool canManagePersistentTextChannels() const;
	std::optional< PersistentTextChannel > selectedPersistentTextChannel() const;
	bool promptForPersistentTextChannel(PersistentTextChannel &textChannel, bool isNew);
	void openServerSettingsDialog();
	void createPersistentTextChannel();
	void editPersistentTextChannel();
	void removePersistentTextChannel();
	void editPersistentTextChannelACL();
	void setDefaultPersistentTextChannel();
	void showPersistentTextChannelContextMenu(const QPoint &position);
	void updatePersistentTextChannelControls();
	void showLogContextMenu(LogTextBrowser *browser, const QPoint &position);
	QImage imageFromLogBrowser(const LogTextBrowser *browser, const QTextCursor &cursor) const;
	void openImageDialog(const QImage &image);
	void openImageDialog(LogTextBrowser *browser, const QTextCursor &cursor);
	QString registerPersistentChatInlineDataImageSource(const QString &source);
	QUrl persistentChatInlineDataImageOpenUrl(const QString &token) const;
	QUrl persistentChatInlineDataImageResourceUrl(const QString &token) const;
	QImage persistentChatInlineDataImageFromSource(const QString &source) const;
	QImage persistentChatInlineDataImageFromUrl(const QUrl &url) const;
	void setPersistentChatContentMode(bool showServerLog, bool preserveScrollPosition = false,
									  bool showComposer = false);
	void renderLegacyActivityView(bool preserveScrollPosition = false);
	void renderServerLogView(bool preserveScrollPosition = false);
	void flushPersistentChatRender();
	void renderPersistentChatViewImmediately(const QString &statusMessage = QString(), bool scrollToBottom = true,
											 bool preserveScrollPosition = false);
	void renderPersistentChatView(const QString &statusMessage = QString(), bool scrollToBottom = true,
								  bool preserveScrollPosition = false);
	bool canMarkPersistentChatRead(bool willScrollToBottom = false) const;
	std::size_t persistentChatUnreadCount() const;
	void handlePersistentChatMessage(const MumbleProto::ChatMessage &msg);
	void handlePersistentChatHistory(const MumbleProto::ChatHistoryResponse &msg);
	void handlePersistentChatReadState(const MumbleProto::ChatReadStateUpdate &msg);
	void handlePersistentChatEmbedState(const MumbleProto::ChatEmbedState &msg);
	void handlePersistentChatReactionState(const MumbleProto::ChatReactionState &msg);
	bool canSendToPersistentChatTarget(const PersistentChatTarget &target, bool requestPermissions) const;
	bool canDeletePersistentChatMessages(const PersistentChatTarget &target, bool requestPermissions) const;
	bool deletePersistentChatMessage(unsigned int messageID);
	void syncPersistentChatInputState(bool baseEnabled);
	bool attachPersistentChatClipboardImage();
	void attachPersistentChatImage(const QImage &image);
	void attachPersistentChatImages(const QList< QUrl > &urls);
	bool attachPersistentChatImageData(const QString &dataUrl);
	void openPersistentChatImagePicker();
	void cachePersistentChatChannelSelection(const QListWidgetItem *item);
	void clearPersistentChatChannelSelection();
	QListWidgetItem *findPersistentChatChannelItem(int scopeValue, unsigned int scopeID) const;
	QList< QListWidgetItem * > persistentChatChannelListSelectedItems() const;
	QListWidgetItem *persistentChatChannelListCurrentItem();
	const QListWidgetItem *persistentChatChannelListCurrentItem() const;
	bool markPersistentChatRead(bool rerender = true, bool willScrollToBottom = false);
	void updatePersistentChatChrome(const PersistentChatTarget &target);
	void updatePersistentChatSendButton();
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	QVariantMap buildModernShellSnapshot();
	bool handleModernShellScopeSelection(const QString &scopeToken);
	bool handleModernShellVoiceJoin(const QString &scopeToken);
	bool handleModernShellScopeAction(const QString &scopeToken, const QString &actionId);
	bool handleModernShellScopeActionValueChanged(const QString &scopeToken, const QString &actionId, int value,
												  bool final);
	bool handleModernShellReplyStart(qulonglong messageID);
	void handleModernShellReplyCancel();
	bool handleModernShellReactionToggle(qulonglong messageID, const QString &emoji, bool active);
	bool handleModernShellMessageDelete(qulonglong messageID);
	bool handleModernShellParticipantMessage(qulonglong session);
	bool handleModernShellParticipantJoin(qulonglong session);
	bool handleModernShellParticipantMove(qulonglong session, const QString &targetScopeToken);
	bool handleModernShellParticipantAction(qulonglong session, const QString &actionId);
	bool handleModernShellParticipantActionValueChanged(qulonglong session, const QString &actionId, int value,
														bool final);
	bool handleModernShellChannelMove(const QString &sourceScopeToken, const QString &targetScopeToken,
									  const QString &placement);
	bool handleModernShellAppAction(const QString &actionId);
	void togglePreferredModernShellLayout();
	enum class ModernShellMenuContext : unsigned char {
		AppServer,
		AppSelf,
		AppConfigure,
		AppHelp,
		Participant,
		Scope,
		Listener
	};
	QVariantList serializeModernShellMenu(QMenu *menu, ModernShellMenuContext context,
										  ModernShellMenuSerializer::ActionRegistry *registry = nullptr) const;
	ModernShellMenuSerializer::ActionDefinition modernShellActionDefinition(ModernShellMenuContext context,
																			QAction *action) const;
	bool triggerModernShellSerializedAction(const ModernShellMenuSerializer::ActionRegistry &registry,
											const QString &actionId, ClientUser *contextUser = nullptr,
											Channel *contextChannel = nullptr);
#endif
	void triggerContextAction(const QString &actionData, ClientUser *user, Channel *channel);
	void updateChatBar(bool forcePersistentChatReload = false);
	void openTextMessageDialog(ClientUser *p);
	void openUserLocalNicknameDialog(const ClientUser &p);

#ifdef Q_OS_WIN
	bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) Q_DECL_OVERRIDE;
	unsigned int uiNewHardware;
#endif
protected:
	Usage uUsage;
	QTimer *qtReconnect;
	std::unique_ptr< NotificationSoundBlocker > m_reconnectSoundBlocker;

	QList< QAction * > qlServerActions;
	QList< QAction * > qlChannelActions;
	QList< QAction * > qlUserActions;
	QAction *qaServerSettings = nullptr;
	QAction *qaCreateTextRoom = nullptr;

	QHash< ShortcutTarget, int > qmCurrentTargets;
	/// A map that contains information about the currently active
	/// shout/whisper targets. The mapping is between a List of
	/// ShortcutTargets that are all triggered together and the
	/// target ID for this specific combination of ShortcutTargets.
	/// The target ID is what the server uses to identify this specific
	/// set of ShortcutTargets.
	QHash< QList< ShortcutTarget >, int > qmTargets;
	/// This is a map between all target IDs the client will ever use
	/// and a helper-number (see iTargetCounter).
	QMap< int, int > qmTargetUse;
	Channel *mapChannel(int idx) const;
	/// This is a pure helper number whose job is to always be increased
	/// if a new VoiceTarget is needed. It will be used as the helper
	/// number in qmTargetUse.
	int iTargetCounter;
	QMap< unsigned int, UserInformation * > qmUserInformations;
	QSet< unsigned int > m_pendingUserInformationSessions;

	std::unique_ptr< PositionalAudioViewer > m_paViewer;

	PTTButtonWidget *qwPTTButtonWidget;

	MUComboBox *qcbTransmitMode;
	QAction *qaTransmitMode;
	QAction *qaTransmitModeSeparator;
	QAction *qaUserRemoteSpeechCleanup        = nullptr;
	QAction *qaChannelScreenShareStart        = nullptr;
	QAction *qaChannelScreenShareStop         = nullptr;
	QAction *qaChannelScreenShareWatch        = nullptr;
	QAction *qaChannelScreenShareStopWatching = nullptr;
	QAction *qaChannelScreenShareOpenWindow   = nullptr;

	Search::SearchDialog *m_searchDialog = nullptr;

	qt_unique_ptr< MenuLabel > m_localVolumeLabel;
	qt_unique_ptr< UserLocalVolumeSlider > m_userLocalVolumeSlider;
	qt_unique_ptr< ListenerVolumeController > m_listenerVolumeController;
	qt_unique_ptr< ListenerVolumeSlider > m_listenerVolumeSlider;
	QWidget *m_serverNavigatorContainer                        = nullptr;
	QFrame *m_serverNavigatorHeaderFrame                       = nullptr;
	QFrame *m_serverNavigatorContentFrame                      = nullptr;
	QFrame *m_serverNavigatorFooterFrame                       = nullptr;
	QLabel *m_serverNavigatorEyebrow                           = nullptr;
	QLabel *m_serverNavigatorTitle                             = nullptr;
	QLabel *m_serverNavigatorSubtitle                          = nullptr;
	QLabel *m_serverNavigatorVoiceSectionEyebrow               = nullptr;
	QLabel *m_serverNavigatorVoiceSectionSubtitle              = nullptr;
	QFrame *m_serverNavigatorTextChannelsFrame                 = nullptr;
	QFrame *m_serverNavigatorTextChannelsDivider               = nullptr;
	QLabel *m_serverNavigatorTextChannelsEyebrow               = nullptr;
	QLabel *m_serverNavigatorTextChannelsTitle                 = nullptr;
	QLabel *m_serverNavigatorTextChannelsSubtitle              = nullptr;
	QToolButton *m_serverNavigatorServerSettingsButton         = nullptr;
	QToolButton *m_serverNavigatorCreateTextChannelButton      = nullptr;
	QFrame *m_serverNavigatorTextChannelsMotdFrame             = nullptr;
	QLabel *m_serverNavigatorTextChannelsMotdTitle             = nullptr;
	QLabel *m_serverNavigatorTextChannelsMotdBody              = nullptr;
	QToolButton *m_serverNavigatorTextChannelsMotdToggleButton = nullptr;
	QLabel *m_serverNavigatorFooter                            = nullptr;
	QPushButton *m_serverNavigatorFooterPresenceButton         = nullptr;
	QLabel *m_serverNavigatorFooterAvatar                      = nullptr;
	QLabel *m_serverNavigatorFooterName                        = nullptr;
	QLabel *m_serverNavigatorFooterPresence                    = nullptr;
	QPointer< QWidget > m_serverNavigatorUserMenuPopup;
	QWidget *m_persistentChatContainer                             = nullptr;
	QWidget *m_logSurface                                          = nullptr;
	QWidget *m_persistentChatComposerInputRow                      = nullptr;
	QFrame *m_persistentChatHeaderFrame                            = nullptr;
	QLabel *m_persistentChatHeaderEyebrow                          = nullptr;
	QLabel *m_persistentChatHeaderTitle                            = nullptr;
	QLabel *m_persistentChatHeaderContext                          = nullptr;
	QLabel *m_persistentChatHeaderSubtitle                         = nullptr;
	QWidget *m_persistentChatSidebarContainer                      = nullptr;
	QLabel *m_persistentChatSidebarEyebrow                         = nullptr;
	QLabel *m_persistentChatChannelListLabel                       = nullptr;
	QLabel *m_persistentChatSidebarSubtitle                        = nullptr;
	QLabel *m_persistentChatSidebarFooter                          = nullptr;
	QWidget *m_persistentChatChannelToolbar                        = nullptr;
	QListWidget *m_persistentChatChannelList                       = nullptr;
	QFrame *m_persistentChatDivider                                = nullptr;
	QWidget *m_persistentChatConversationPanel                     = nullptr;
	QWidget *m_persistentChatLogPanel                              = nullptr;
	PersistentChatListWidget *m_persistentChatHistory              = nullptr;
	LogTextBrowser *m_persistentChatLogView                        = nullptr;
	PersistentChatGateway *m_persistentChatGateway                 = nullptr;
	PersistentChatController *m_persistentChatController           = nullptr;
	PersistentChatHistoryModel *m_persistentChatHistoryModel       = nullptr;
	PersistentChatHistoryDelegate *m_persistentChatHistoryDelegate = nullptr;
	QFrame *m_persistentChatComposerFrame                          = nullptr;
	QFrame *m_persistentChatReplyFrame                             = nullptr;
	QLabel *m_persistentChatReplyLabel                             = nullptr;
	QLabel *m_persistentChatReplySnippet                           = nullptr;
	QToolButton *m_persistentChatReplyCancelButton                 = nullptr;
	QToolButton *m_persistentChatAttachButton                      = nullptr;
	QToolButton *m_persistentChatSendButton                        = nullptr;
	QString m_persistentChatWelcomeText;
	bool m_persistentChatMotdExpanded        = false;
	bool m_hasPersistentChatSupport          = false;
	bool m_persistentChatTargetUsesVoiceTree = false;
	std::optional< int > m_persistentChatSelectedScopeValue;
	unsigned int m_persistentChatSelectedScopeID  = 0;
	unsigned int m_defaultPersistentTextChannelID = 0;
	QHash< unsigned int, PersistentTextChannel > m_persistentTextChannels;
	QHash< unsigned int, unsigned int > m_userIdleSeconds;
	std::vector< MumbleProto::ChatMessage > m_persistentChatMessages;
	QHash< QString, PersistentChatPreview > m_persistentChatPreviews;
	QHash< unsigned int, PersistentChatAssetDownload > m_persistentChatAssetDownloads;
	QHash< QString, QString > m_persistentChatInlineDataImageSources;
	QHash< QString, unsigned int > m_persistentChatLastReadByScope;
	QHash< QString, int > m_persistentChatUnreadByScope;
	std::optional< PersistentChatRenderRequest > m_pendingPersistentChatRender;
	std::optional< MumbleProto::ChatScope > m_visiblePersistentChatScope;
	unsigned int m_visiblePersistentChatScopeID           = 0;
	unsigned int m_visiblePersistentChatLastReadMessageID = 0;
	unsigned int m_visiblePersistentChatOldestMessageID   = 0;
	bool m_visiblePersistentChatHasMore                   = false;
	bool m_persistentChatLoadingOlder                     = false;
	bool m_persistentChatRenderQueued                     = false;
	QTimer *m_persistentChatResizeRenderTimer             = nullptr;
	QTimer *m_persistentChatScrollIdleTimer               = nullptr;
	QTimer *m_userPresenceRefreshTimer                    = nullptr;
	int m_pendingPersistentChatViewportWidth              = -1;
	int m_lastPersistentChatViewportWidth                 = -1;
	int m_persistentChatBottomLockRendersRemaining        = 0;
	bool m_persistentChatPreviewRefreshPending            = false;
	bool m_persistentChatRestoreAnchorPending             = false;
	bool m_persistentChatLogStickToBottom                 = true;
	QString m_persistentChatPendingAnchorRowId;
	int m_persistentChatPendingAnchorOffset = 0;
	std::optional< MumbleProto::ChatMessage > m_pendingPersistentChatReply;
	QTimer *m_modernShellSyncTimer                = nullptr;
	QTimer *m_nativeWindowMoveResizeRecoveryTimer = nullptr;
	qint64 m_modernShellLastSnapshotSyncMs        = 0;
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	ModernShellHost *m_modernShellHost                     = nullptr;
	QObject *m_persistentChatPreviewSnapshotRenderer       = nullptr;
	bool m_modernShellSnapshotPendingAfterNativeMoveResize = false;
#endif
	bool m_shellLayoutInitialized              = false;
	Settings::WindowLayout m_activeShellLayout = Settings::LayoutModern;
	bool m_modernLayoutCompatibleServer        = false;
	bool m_modernShellRuntimeDisabled          = false;
	bool m_nativeWindowMoveResizeActive        = false;

	std::stack< unsigned int > m_previousChannels;
	std::optional< unsigned int > m_movedBackFromChannel;

	static constexpr int stateVersion(bool modernShell);

	void createActions();
	void handleModernShellBootFailure(const QString &reason);
	void setupGui();
	void updateWindowTitle();
	/// updateToolbar updates the state of the toolbar depending on the current
	/// window layout setting.
	/// If the window layout setting is 'custom', the toolbar is made movable. If the
	/// window layout is not 'custom', the toolbar is locked in place at the top of
	/// the MainWindow.
	void updateToolbar();
	void updateFavoriteButton();
	void customEvent(QEvent *evt) Q_DECL_OVERRIDE;
	void findDesiredChannel();
	void setupView(bool toggle_minimize = true);
	bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent *e) Q_DECL_OVERRIDE;
	void hideEvent(QHideEvent *e) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent *e) Q_DECL_OVERRIDE;
	void changeEvent(QEvent *e) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

	QMenu *createPopupMenu() Q_DECL_OVERRIDE;

	bool handleSpecialContextMenu(const QUrl &url, const QPoint &pos_, bool focus = false);
	Channel *getContextMenuChannel();
	ClientUser *getContextMenuUser();
	ContextMenuTarget getContextMenuTargets();
	QString screenShareStreamForChannel(const Channel *channel) const;

	void autocompleteUsername();

public slots:
	void on_qmServer_aboutToShow();
	void on_qaServerConnect_triggered(bool autoconnect = false);
	void on_qaServerDisconnect_triggered();
	void on_qaServerBanList_triggered();
	void on_qaServerUserList_triggered();
	void on_qaServerInformation_triggered();
	void on_qaServerSettings_triggered();
	void on_qaCreateTextRoom_triggered();
	void on_qaServerTexture_triggered();
	void on_qaServerTextureRemove_triggered();
	void on_qaServerTokens_triggered();
	void on_qmSelf_aboutToShow();
	void on_qaSelfComment_triggered();
	void on_qaSelfRegister_triggered();
	void qcbTransmitMode_activated(int index);
	void updateTransmitModeComboBox(Settings::AudioTransmit newMode);
	void qmUser_aboutToShow();
	void qmListener_aboutToShow();
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
	void on_qaUserLocalIgnoreTTS_triggered();
	void on_qaUserLocalMute_triggered();
	void triggerUserRemoteSpeechCleanup();
	void on_qaUserLocalNickname_triggered();
	void on_qaUserTextMessage_triggered();
	void on_qaUserRegister_triggered();
	void on_qaUserInformation_triggered();
	void on_qaUserFriendAdd_triggered();
	void on_qaUserFriendRemove_triggered();
	void on_qaUserFriendUpdate_triggered();
	void qmChannel_aboutToShow();
	void on_qaChannelJoin_triggered();
	void on_qaUserJoin_triggered();
	void on_qaUserMove_triggered();
	void on_qaChannelListen_triggered();
	void on_qaChannelAdd_triggered();
	void on_qaChannelRemove_triggered();
	void on_qaChannelACL_triggered();
	void on_qaChannelLink_triggered();
	void on_qaChannelUnlink_triggered();
	void on_qaChannelUnlinkAll_triggered();
	void on_qaChannelSendMessage_triggered();
	void on_qaChannelHide_triggered();
	void on_qaChannelPin_triggered();
	void on_qaChannelCopyURL_triggered();
	void startChannelScreenShare();
	void stopChannelScreenShare();
	void watchChannelScreenShare();
	void stopWatchingChannelScreenShare();
	void openChannelScreenShareWindow();
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
	void on_qaPositionalAudioViewer_triggered();
	void on_qaHelpWhatsThis_triggered();
	void on_qaHelpAbout_triggered();
	void on_qaHelpAboutQt_triggered();
	void on_qaHelpVersionCheck_triggered();
	void on_qaQuit_triggered();
	void on_qteChat_tabPressed();
	void on_qteChat_backtabPressed();
	void on_qteChat_ctrlSpacePressed();
	void on_persistentChatScopeChanged(int index);
	Q_INVOKABLE void on_qtvUsers_customContextMenuRequested(const QPoint &mpos,
															bool usePositionForGettingContext = true);
	void showUsersContextMenu(const QPoint &mpos, bool usePositionForGettingContext = true);
	void on_qteLog_customContextMenuRequested(const QPoint &pos);
	void on_qteLog_anchorClicked(const QUrl &);
	void on_qteLog_highlighted(const QUrl &link);
	void on_PushToTalk_triggered(bool, QVariant);
	void on_PushToMute_triggered(bool, QVariant);
	void on_VolumeUp_triggered(bool, QVariant);
	void on_VolumeDown_triggered(bool, QVariant);
	void on_gsMuteSelf_down(QVariant);
	void on_gsDeafSelf_down(QVariant);
	void on_gsWhisper_triggered(bool, QVariant);
	void addTarget(ShortcutTarget *);
	void removeTarget(ShortcutTarget *);
	void on_gsListenChannel_triggered(bool, QVariant);
	void on_gsCycleTransmitMode_triggered(bool, QVariant);
	void on_gsToggleMainWindowVisibility_triggered(bool, QVariant);
	void on_gsTransmitModePushToTalk_triggered(bool, QVariant);
	void on_gsTransmitModeContinuous_triggered(bool, QVariant);
	void on_gsTransmitModeVAD_triggered(bool, QVariant);
	void on_gsSendTextMessage_triggered(bool, QVariant);
	void on_gsSendClipboardTextMessage_triggered(bool, QVariant);
	void on_gsToggleTalkingUI_triggered(bool, QVariant);
	void on_gsToggleSearch_triggered(bool, QVariant);
	void on_gsServerConnect_triggered(bool, QVariant);
	void on_gsServerDisconnect_triggered(bool, QVariant);
	void on_gsServerInformation_triggered(bool, QVariant);
	void on_gsServerTokens_triggered(bool, QVariant);
	void on_gsServerUserList_triggered(bool, QVariant);
	void on_gsServerBanList_triggered(bool, QVariant);
	void on_gsSelfPrioritySpeaker_triggered(bool, QVariant);
	void on_gsRecording_triggered(bool, QVariant);
	void on_gsSelfComment_triggered(bool, QVariant);
	void on_gsServerTexture_triggered(bool, QVariant);
	void on_gsServerTextureRemove_triggered(bool, QVariant);
	void on_gsSelfRegister_triggered(bool, QVariant);
	void on_gsAudioStats_triggered(bool, QVariant);
	void on_gsConfigDialog_triggered(bool, QVariant);
	void on_gsAudioWizard_triggered(bool, QVariant);
	void on_gsConfigCert_triggered(bool, QVariant);
	void on_gsAudioTTS_triggered(bool, QVariant);
	void on_gsHelpAbout_triggered(bool, QVariant);
	void on_gsHelpAboutQt_triggered(bool, QVariant);
	void on_gsHelpVersionCheck_triggered(bool, QVariant);
	void on_gsTogglePositionalAudio_triggered(bool, QVariant);
	void on_gsMoveBack_triggered(bool, QVariant);
	void on_gsCycleListenerAttenuationMode_triggered(bool, QVariant);
	void on_gsListenerAttenuationUp_triggered(bool, QVariant);
	void on_gsListenerAttenuationDown_triggered(bool, QVariant);
	void on_gsAdaptivePush_triggered(bool, QVariant);

	void on_Reconnect_timeout();
	void on_qaTalkingUIToggle_triggered();
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
	void on_muteCuePopup_triggered();
	void showMuteCuePopup();
	/// Handles state changes like talking mode changes and mute/unmute
	/// or priority speaker flag changes for the gui user
	void userStateChanged();
	void on_channelStateChanged(Channel *channel, bool forceUpdateTree);
	void destroyUserInformation();
	void sendChatbarMessage(QString msg);
	void sendChatbarText(QString msg, bool plainText = false);
	void pttReleased();
	void whisperReleased(QVariant scdata);
	void onResetAudio();
	void showRaiseWindow();
	void on_qaFilterToggle_triggered();
	/// Opens a save dialog for the image selected from the log or persistent chat history.
	void saveImageAs();
	/// Returns the path to the user's image directory, optionally with a
	/// filename included.
	QString getImagePath(QString filename = QString()) const;

	/// Shows a dialog with the currently selected log or chat image.
	void showImageDialog();
	/// Updates the user's image directory to the given path (any included
	/// filename is discarded).
	void updateImagePath(QString filepath) const;
	void setTransmissionMode(Settings::AudioTransmit mode);
	/// Sets the local user's mute state
	///
	/// @param mute Whether to mute the user
	void setAudioMute(bool mute);
	/// Sets the local user's deaf state
	///
	/// @param deaf Whether to deafen the user
	void setAudioDeaf(bool deaf);
	// Callback the search action being triggered
	void on_qaSearch_triggered();
	void toggleSearchDialogVisibility();
	/// Enables or disables the recording feature
	void enableRecording(bool recordingAllowed);
	/// Invokes OS native window highlighting
	void highlightWindow();
	void handleUserMoved(unsigned int sessionID, const std::optional< unsigned int > &prevChannelID,
						 unsigned int newChannelID);
	void on_qaMoveBack_triggered();
signals:
	/// Signal emitted when the server and the client have finished
	/// synchronizing (after a new connection).
	void serverSynchronized();
	/// Signal emitted whenever a user adds a new ChannelListener
	void userAddedChannelListener(ClientUser *user, Channel *channel);
	/// Signal emitted whenever a user removes a ChannelListener
	void userRemovedChannelListener(ClientUser *user, Channel *channel);
	void transmissionModeChanged(Settings::AudioTransmit newMode);

	/// Signal emitted when the local user changes their talking status either actively or passively
	void talkingStatusChanged();

	/// Signal when channel state has been changed
	void channelStateChanged(Channel *channel, bool forceUpdateTree);

	/// Signal emitted when the connection was terminated and all cleanup code has been run
	void disconnectedFromServer();

	/// Signal emitted when the window manager notifies the Mumble MainWindow that the application was just minimized
	void windowMinimized();
	/// Signal emitted when the user requested to toggle the MainWindow visibility
	void windowVisibilityToggled();
	/// Signal emitted whenever the Mumble MainWindow regains the active state from the window manager
	void windowActivated();

public:
	MainWindow(QWidget *parent);
	~MainWindow() Q_DECL_OVERRIDE;
	std::optional< unsigned int > userIdleSeconds(unsigned int session) const;
	bool isUserIdle(unsigned int session) const;
	void refreshUserPresenceStats();

	// Implementation in Messages.cpp
#define PROCESS_MUMBLE_TCP_MESSAGE(name, value) void msg##name(const MumbleProto::name &);
	MUMBLE_ALL_TCP_MESSAGES
#undef PROCESS_MUMBLE_TCP_MESSAGE
	void removeContextAction(const MumbleProto::ContextActionModify &msg);
	/// Logs a message that an action could not be saved permanently because
	/// the user has no certificate and can't be reliably identified.
	///
	/// @param actionName  The name of the action that has been executed.
	/// @param p  The user on which the action was performed.
	void logChangeNotPermanent(const QString &actionName, ClientUser *const p) const;
	void refreshTextDocumentStylesheets();

	void openServerConnectDialog(bool autoconnect = false);
	void disconnectFromServer();
	void addServerAsFavorite();
	void openServerInformationDialog();
	void openServerTokensDialog();
	void openServerUserListDialog();
	void openServerBanListDialog();
	void toggleSelfPrioritySpeaker();
	void recording();
	void openSelfCommentDialog();
	void changeServerTexture();
	void removeServerTexture();
	void selfRegister();
	void openAudioStatsDialog();
	void openConfigDialog();
	void openConfigDialogPage(const QString &pageName);
	void openAudioWizardDialog();
	void openCertWizardDialog();
	void enableAudioTTS(bool enable);
	void openAboutDialog();
	void openAboutQtDialog();
	void versionCheck();
	void enablePositionalAudio(bool enable);
};

#endif
