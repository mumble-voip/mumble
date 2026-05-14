// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_MODERNSHELLBRIDGE_H_
#define MUMBLE_MUMBLE_MODERNSHELLBRIDGE_H_

#if defined(MUMBLE_HAS_MODERN_LAYOUT)

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>

class ModernShellBridge : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ModernShellBridge)
	Q_PROPERTY(QVariantMap snapshot READ snapshot NOTIFY snapshotChanged)

public:
	explicit ModernShellBridge(QObject *parent = nullptr);

	QVariantMap snapshot() const;
	void setSnapshot(const QVariantMap &snapshot);

	Q_INVOKABLE void ready();
	Q_INVOKABLE void selectScope(const QString &scopeToken);
	Q_INVOKABLE void joinVoiceChannel(const QString &scopeToken);
	Q_INVOKABLE void invokeScopeAction(const QString &scopeToken, const QString &actionId);
	Q_INVOKABLE void scopeActionValueChanged(const QString &scopeToken, const QString &actionId, int value, bool final);
	Q_INVOKABLE void sendMessage(const QString &message);
	Q_INVOKABLE void startReply(qulonglong messageId);
	Q_INVOKABLE void cancelReply();
	Q_INVOKABLE void toggleReaction(qulonglong messageId, const QString &emoji, bool active);
	Q_INVOKABLE void deleteMessage(qulonglong messageId);
	Q_INVOKABLE void messageParticipant(qulonglong session);
	Q_INVOKABLE void joinParticipant(qulonglong session);
	Q_INVOKABLE void moveParticipantToChannel(qulonglong session, const QString &scopeToken);
	Q_INVOKABLE void invokeParticipantAction(qulonglong session, const QString &actionId);
	Q_INVOKABLE void participantActionValueChanged(qulonglong session, const QString &actionId, int value, bool final);
	Q_INVOKABLE void moveChannelToChannel(const QString &sourceScopeToken, const QString &targetScopeToken,
										  const QString &placement);
	Q_INVOKABLE void loadOlderHistory();
	Q_INVOKABLE void markRead();
	Q_INVOKABLE void toggleSelfMute();
	Q_INVOKABLE void toggleSelfDeaf();
	Q_INVOKABLE void openConnectDialog();
	Q_INVOKABLE void disconnectServer();
	Q_INVOKABLE void openSettings();
	Q_INVOKABLE bool clipboardHasImage() const;
	Q_INVOKABLE QString clipboardText() const;
	Q_INVOKABLE void setClipboardText(const QString &text);
	Q_INVOKABLE void attachClipboardImage();
	Q_INVOKABLE void openImagePicker();
	Q_INVOKABLE void attachImageData(const QString &dataUrl);
	Q_INVOKABLE void activateLink(const QString &href);
	Q_INVOKABLE void invokeAppAction(const QString &actionId);
	Q_INVOKABLE void toggleLayout();

signals:
	void bootReady();
	void snapshotChanged();
	void scopeSelectionRequested(const QString &scopeToken);
	void voiceJoinRequested(const QString &scopeToken);
	void scopeActionRequested(const QString &scopeToken, const QString &actionId);
	void scopeActionValueChangedRequested(const QString &scopeToken, const QString &actionId, int value, bool final);
	void messageSendRequested(const QString &message);
	void replyStartRequested(qulonglong messageId);
	void replyCancelRequested();
	void reactionToggleRequested(qulonglong messageId, const QString &emoji, bool active);
	void messageDeleteRequested(qulonglong messageId);
	void participantMessageRequested(qulonglong session);
	void participantJoinRequested(qulonglong session);
	void participantMoveRequested(qulonglong session, const QString &scopeToken);
	void participantActionRequested(qulonglong session, const QString &actionId);
	void participantActionValueChangedRequested(qulonglong session, const QString &actionId, int value, bool final);
	void channelMoveRequested(const QString &sourceScopeToken, const QString &targetScopeToken,
							  const QString &placement);
	void olderHistoryRequested();
	void markReadRequested();
	void selfMuteToggleRequested();
	void selfDeafToggleRequested();
	void connectDialogRequested();
	void disconnectRequested();
	void settingsRequested();
	void clipboardImageAttachmentRequested();
	void imagePickerRequested();
	void imageDataAttachmentRequested(const QString &dataUrl);
	void linkActivationRequested(const QString &href);
	void appActionRequested(const QString &actionId);
	void layoutToggleRequested();

private:
	QVariantMap m_snapshot;
};

#endif // defined(MUMBLE_HAS_MODERN_LAYOUT)

#endif // MUMBLE_MUMBLE_MODERNSHELLBRIDGE_H_
