// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ModernShellBridge.h"

#if defined(MUMBLE_HAS_MODERN_LAYOUT)

#include "Global.h"

#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#include <QtGui/QImage>
#include <QtGui/QImageReader>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>

namespace {
	void appendModernShellConnectTrace(const QString &message) {
		if (qEnvironmentVariableIntValue("MUMBLE_CONNECT_TRACE") == 0) {
			return;
		}

		QFile traceFile(Global::get().qdBasePath.filePath(QLatin1String("shared-modern-connect-trace.log")));
		if (!traceFile.open(QIODevice::Append | QIODevice::Text)) {
			return;
		}

		const QByteArray line = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs).toUtf8()
								+ " UI " + message.toUtf8() + '\n';
		traceFile.write(line);
		traceFile.flush();
	}

	QList< QUrl > extractLocalImageUrls(const QMimeData *mimeData) {
		QList< QUrl > imageUrls;
		if (!mimeData) {
			return imageUrls;
		}

		const QList< QUrl > urls = mimeData->urls();
		for (const QUrl &url : urls) {
			if (!url.isLocalFile()) {
				continue;
			}

			const QString localPath = url.toLocalFile();
			if (QImageReader::imageFormat(localPath).isEmpty()) {
				continue;
			}

			imageUrls.push_back(url);
		}

		return imageUrls;
	}

	QImage extractMimeImage(const QMimeData *mimeData) {
		if (!mimeData || !mimeData->hasImage()) {
			return QImage();
		}

		const QVariant imageData = mimeData->imageData();
		QImage image             = qvariant_cast< QImage >(imageData);
		if (!image.isNull()) {
			return image;
		}

		const QPixmap pixmap = qvariant_cast< QPixmap >(imageData);
		return pixmap.isNull() ? QImage() : pixmap.toImage();
	}
} // namespace

ModernShellBridge::ModernShellBridge(QObject *parent) : QObject(parent) {
}

QVariantMap ModernShellBridge::snapshot() const {
	return m_snapshot;
}

void ModernShellBridge::setSnapshot(const QVariantMap &snapshot) {
	appendModernShellConnectTrace(
		QStringLiteral("ModernShellBridge::setSnapshot enter keys=%1").arg(snapshot.keys().size()));
	if (m_snapshot == snapshot) {
		appendModernShellConnectTrace(QStringLiteral("ModernShellBridge::setSnapshot unchanged-skip"));
		return;
	}
	m_snapshot = snapshot;
	appendModernShellConnectTrace(QStringLiteral("ModernShellBridge::setSnapshot before-emit"));
	emit snapshotChanged();
	appendModernShellConnectTrace(QStringLiteral("ModernShellBridge::setSnapshot after-emit"));
}

void ModernShellBridge::publishModernShellPatch(const QVariantMap &patch) {
	if (patch.isEmpty()) {
		return;
	}

	emit modernPatchChanged(patch);
}

void ModernShellBridge::publishParticipantTalkState(const QVariantMap &state) {
	if (state.isEmpty()) {
		return;
	}

	emit participantTalkStateChanged(state);
}

void ModernShellBridge::ready() {
	appendModernShellConnectTrace(QStringLiteral("ModernShellBridge::ready"));
	emit bootReady();
}

void ModernShellBridge::selectScope(const QString &scopeToken) {
	emit scopeSelectionRequested(scopeToken.trimmed());
}

void ModernShellBridge::joinVoiceChannel(const QString &scopeToken) {
	emit voiceJoinRequested(scopeToken.trimmed());
}

void ModernShellBridge::invokeScopeAction(const QString &scopeToken, const QString &actionId) {
	emit scopeActionRequested(scopeToken.trimmed(), actionId.trimmed());
}

void ModernShellBridge::scopeActionValueChanged(const QString &scopeToken, const QString &actionId, const int value,
												const bool final) {
	emit scopeActionValueChangedRequested(scopeToken.trimmed(), actionId.trimmed(), value, final);
}

void ModernShellBridge::sendMessage(const QString &message) {
	emit messageSendRequested(message);
}

void ModernShellBridge::startReply(const qulonglong messageId) {
	emit replyStartRequested(messageId);
}

void ModernShellBridge::cancelReply() {
	emit replyCancelRequested();
}

void ModernShellBridge::toggleReaction(const qulonglong messageId, const QString &emoji, const bool active) {
	const QString trimmedEmoji = emoji.trimmed();
	if (messageId == 0 || trimmedEmoji.isEmpty()) {
		return;
	}

	emit reactionToggleRequested(messageId, trimmedEmoji, active);
}

void ModernShellBridge::deleteMessage(const qulonglong messageId) {
	if (messageId == 0) {
		return;
	}

	emit messageDeleteRequested(messageId);
}

void ModernShellBridge::messageParticipant(const qulonglong session) {
	emit participantMessageRequested(session);
}

void ModernShellBridge::joinParticipant(const qulonglong session) {
	emit participantJoinRequested(session);
}

void ModernShellBridge::moveParticipantToChannel(const qulonglong session, const QString &scopeToken) {
	emit participantMoveRequested(session, scopeToken.trimmed());
}

void ModernShellBridge::invokeParticipantAction(const qulonglong session, const QString &actionId) {
	emit participantActionRequested(session, actionId.trimmed());
}

void ModernShellBridge::participantActionValueChanged(const qulonglong session, const QString &actionId,
													  const int value, const bool final) {
	emit participantActionValueChangedRequested(session, actionId.trimmed(), value, final);
}

void ModernShellBridge::moveChannelToChannel(const QString &sourceScopeToken, const QString &targetScopeToken,
											 const QString &placement) {
	emit channelMoveRequested(sourceScopeToken.trimmed(), targetScopeToken.trimmed(), placement.trimmed());
}

void ModernShellBridge::loadOlderHistory() {
	emit olderHistoryRequested();
}

void ModernShellBridge::markRead() {
	emit markReadRequested();
}

void ModernShellBridge::toggleSelfMute() {
	emit selfMuteToggleRequested();
}

void ModernShellBridge::toggleSelfDeaf() {
	emit selfDeafToggleRequested();
}

void ModernShellBridge::openConnectDialog() {
	emit connectDialogRequested();
}

void ModernShellBridge::disconnectServer() {
	emit disconnectRequested();
}

void ModernShellBridge::openSettings() {
	emit settingsRequested();
}

bool ModernShellBridge::clipboardHasImage() const {
	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData   = clipboard ? clipboard->mimeData() : nullptr;
	return !extractMimeImage(mimeData).isNull() || !extractLocalImageUrls(mimeData).isEmpty();
}

QString ModernShellBridge::clipboardText() const {
	const QClipboard *clipboard = QApplication::clipboard();
	return clipboard ? clipboard->text() : QString();
}

void ModernShellBridge::setClipboardText(const QString &text) {
	QClipboard *clipboard = QApplication::clipboard();
	if (clipboard) {
		clipboard->setText(text);
	}
}

void ModernShellBridge::attachClipboardImage() {
	emit clipboardImageAttachmentRequested();
}

void ModernShellBridge::openImagePicker() {
	emit imagePickerRequested();
}

void ModernShellBridge::attachImageData(const QString &dataUrl) {
	emit imageDataAttachmentRequested(dataUrl.trimmed());
}

void ModernShellBridge::activateLink(const QString &href) {
	const QString trimmedHref = href.trimmed();
	if (trimmedHref.isEmpty()) {
		return;
	}

	emit linkActivationRequested(trimmedHref);
}

void ModernShellBridge::invokeAppAction(const QString &actionId) {
	emit appActionRequested(actionId.trimmed());
}

void ModernShellBridge::toggleLayout() {
	emit layoutToggleRequested();
}

#endif // defined(MUMBLE_HAS_MODERN_LAYOUT)
