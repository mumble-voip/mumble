// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ACL.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "ClientType.h"
#include "Connection.h"
#include "Group.h"
#include "Meta.h"
#include "MumbleConstants.h"
#include "ProtoUtils.h"
#include "QtUtils.h"
#include "ScreenShare.h"
#include "Server.h"
#include "ServerUser.h"
#include "User.h"
#include "Version.h"
#include "crypto/CryptState.h"

#include "murmur/database/ChronoUtils.h"
#include "murmur/database/DBChatMessage.h"
#include "murmur/database/DBChatReadState.h"
#include "murmur/database/UserProperty.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <unordered_map>

#include <QtCore/QBuffer>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QRandomGenerator>
#include <QtCore/QRegularExpressionMatchIterator>
#include <QtCore/QStack>
#include <QtCore/QTimeZone>
#include <QtCore/QUuid>
#include <QtCore/QtEndian>
#include <QtGui/QImage>
#include <QtGui/QImageReader>
#include <QtGui/QImageWriter>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <tracy/Tracy.hpp>

namespace msdb = ::mumble::server::db;

#define RATELIMIT(user)                   \
	if (user->leakyBucket.ratelimit(1)) { \
		return;                           \
	}

#define MSG_SETUP(st)            \
	if (uSource->sState != st) { \
		return;                  \
	}                            \
	uSource->bwr.resetIdleSeconds()

#define MSG_SETUP_NO_UNIDLE(st) \
	if (uSource->sState != st)  \
	return

#define VICTIM_SETUP                                   \
	ServerUser *pDstServerUser = uSource;              \
	if (msg.has_session())                             \
		pDstServerUser = qhUsers.value(msg.session()); \
	if (!pDstServerUser)                               \
		return;                                        \
	Q_UNUSED(pDstServerUser)

#define PERM_DENIED(who, where, what)                                                                                \
	{                                                                                                                \
		MumbleProto::PermissionDenied mppd;                                                                          \
		mppd.set_permission(static_cast< unsigned int >(what));                                                      \
		mppd.set_channel_id(where->iId);                                                                             \
		mppd.set_session(who->uiSession);                                                                            \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_Permission);                                            \
		sendMessage(uSource, mppd);                                                                                  \
		log(uSource,                                                                                                 \
			QString("%1 not allowed to %2 in %3").arg(who->qsName).arg(ChanACL::permName(what)).arg(where->qsName)); \
	}

#define PERM_DENIED_TYPE(type)                                        \
	{                                                                 \
		MumbleProto::PermissionDenied mppd;                           \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_##type); \
		sendMessage(uSource, mppd);                                   \
	}

#define PERM_DENIED_FALLBACK(type, version, text)                     \
	{                                                                 \
		MumbleProto::PermissionDenied mppd;                           \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_##type); \
		if (uSource->m_version < version)                             \
			mppd.set_reason(u8(text));                                \
		sendMessage(uSource, mppd);                                   \
	}

#define PERM_DENIED_HASH(user)                                                    \
	{                                                                             \
		MumbleProto::PermissionDenied mppd;                                       \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_MissingCertificate); \
		if (user)                                                                 \
			mppd.set_session(user->uiSession);                                    \
		sendMessage(uSource, mppd);                                               \
	}

/// A helper class for managing temporary access tokens.
/// It will add the tokens in the comstructor and remove them again in the destructor effectively
/// turning the tokens into a scope-based property.
class TemporaryAccessTokenHelper {
protected:
	ServerUser *affectedUser;
	QStringList qslTemporaryTokens;
	Server *server;

public:
	TemporaryAccessTokenHelper(ServerUser *affectedUser, const QStringList &tokens, Server *server)
		: affectedUser(affectedUser), qslTemporaryTokens(tokens), server(server) {
		// Add the temporary tokens
		QMutableStringListIterator it(this->qslTemporaryTokens);

		{
			QMutexLocker qml(&server->qmCache);

			while (it.hasNext()) {
				QString &token = it.next();

				// If tokens are treated case-insensitively, transform all temp. tokens to lowercase first
				if (Group::accessTokenCaseSensitivity == Qt::CaseInsensitive) {
					token = token.toLower();
				}

				if (!this->affectedUser->qslAccessTokens.contains(token, Group::accessTokenCaseSensitivity)) {
					// Add token
					this->affectedUser->qslAccessTokens << token;
				} else {
					// It appears, as if the user already has this token set -> it's not a temporary one or a duplicate
					it.remove();
				}
			}
		}

		if (!this->qslTemporaryTokens.isEmpty()) {
			// Clear the cache in order for tokens to take effect
			server->clearACLCache(this->affectedUser);
		}
	}

	~TemporaryAccessTokenHelper() {
		if (!this->qslTemporaryTokens.isEmpty()) {
			{
				QMutexLocker qml(&server->qmCache);

				// remove the temporary tokens
				for (const QString &token : this->qslTemporaryTokens) {
					this->affectedUser->qslAccessTokens.removeOne(token);
				}
			}

			// Clear cache to actually get rid of the temporary tokens
			server->clearACLCache(this->affectedUser);
		}
	}
};

namespace {
std::optional< unsigned int > persistedUserID(const ServerUser *user) {
	if (!user || user->iId < 0) {
		return std::nullopt;
	}

	return static_cast< unsigned int >(user->iId);
}

std::string chatScopeKey(MumbleProto::ChatScope scope, unsigned int scopeID) {
	switch (scope) {
		case MumbleProto::Channel:
			return "channel:" + std::to_string(scopeID);
		case MumbleProto::ServerGlobal:
			return "global";
		case MumbleProto::Aggregate:
			return {};
		case MumbleProto::TextChannel:
			return "text:" + std::to_string(scopeID);
	}

	return {};
}

std::optional< unsigned int > scopeIDFromPrefixedScopeKey(const std::string &scopeKey, const QString &prefix) {
	const QString key = QString::fromStdString(scopeKey);
	if (!key.startsWith(prefix)) {
		return std::nullopt;
	}

	bool ok                    = false;
	const unsigned int scopeID = key.mid(prefix.size()).toUInt(&ok);
	if (!ok) {
		return std::nullopt;
	}

	return scopeID;
}

std::optional< unsigned int > channelIDFromScopeKey(const std::string &scopeKey) {
	return scopeIDFromPrefixedScopeKey(scopeKey, QStringLiteral("channel:"));
}

std::optional< unsigned int > textChannelIDFromScopeKey(const std::string &scopeKey) {
	return scopeIDFromPrefixedScopeKey(scopeKey, QStringLiteral("text:"));
}

bool resolveStoredChatThread(const ::msdb::DBChatThread &thread, const QHash< unsigned int, Channel * > &channels,
							 const QHash< unsigned int, ::msdb::DBTextChannel > &textChannels,
							 MumbleProto::ChatScope &scope, unsigned int &scopeID, Channel *&permissionChannel) {
	switch (thread.scope) {
		case ::msdb::ChatThreadScope::Channel: {
			std::optional< unsigned int > channelID = channelIDFromScopeKey(thread.scopeKey);
			if (!channelID) {
				return false;
			}

			scope             = MumbleProto::Channel;
			scopeID           = channelID.value();
			permissionChannel = channels.value(scopeID);
			return permissionChannel != nullptr;
		}
		case ::msdb::ChatThreadScope::ServerGlobal:
			scope             = MumbleProto::ServerGlobal;
			scopeID           = 0;
			permissionChannel = channels.value(Mumble::ROOT_CHANNEL_ID);
			return permissionChannel != nullptr;
		case ::msdb::ChatThreadScope::Private:
			return false;
		case ::msdb::ChatThreadScope::TextChannel: {
			std::optional< unsigned int > textChannelID = textChannelIDFromScopeKey(thread.scopeKey);
			if (!textChannelID) {
				return false;
			}

			const auto it = textChannels.constFind(textChannelID.value());
			if (it == textChannels.cend()) {
				return false;
			}

			scope             = MumbleProto::TextChannel;
			scopeID           = textChannelID.value();
			permissionChannel = channels.value(it->aclChannelID);
			return permissionChannel != nullptr;
		}
	}

	return false;
}

QSet< ServerUser * > recipientsWithChatHistoryAccess(const QHash< unsigned int, ServerUser * > &connectedUsers,
													 Channel *channel, ChanACL::ACLCache &cache) {
	QSet< ServerUser * > recipients;

	for (ServerUser *currentUser : connectedUsers) {
		if (currentUser && ChanACL::hasPermission(currentUser, channel, ChanACL::ViewTextMessageHistory, &cache)) {
			recipients.insert(currentUser);
		}
	}

	return recipients;
}

std::optional< unsigned int > configuredDefaultTextChannelID(DBWrapper &dbWrapper, unsigned int serverID) {
	unsigned int configuredTextChannelID = 0;
	dbWrapper.getConfigurationTo(serverID, "defaulttextchannel", configuredTextChannelID);
	if (configuredTextChannelID == 0) {
		return std::nullopt;
	}

	return configuredTextChannelID;
}

void storeDefaultTextChannelID(DBWrapper &dbWrapper, unsigned int serverID,
							   std::optional< unsigned int > textChannelID) {
	if (textChannelID && *textChannelID > 0) {
		dbWrapper.setConfiguration(serverID, "defaulttextchannel", std::to_string(*textChannelID));
	} else {
		dbWrapper.clearConfiguration(serverID, "defaulttextchannel");
	}
}

bool sortTextChannelsForPresentation(const ::msdb::DBTextChannel &lhs, const ::msdb::DBTextChannel &rhs) {
	if (lhs.position != rhs.position) {
		return lhs.position < rhs.position;
	}

	if (lhs.name != rhs.name) {
		return lhs.name < rhs.name;
	}

	return lhs.textChannelID < rhs.textChannelID;
}

std::optional< unsigned int > firstTextChannelID(const std::vector<::msdb::DBTextChannel > &textChannels) {
	if (textChannels.empty()) {
		return std::nullopt;
	}

	const auto firstTextChannel =
		std::min_element(textChannels.cbegin(), textChannels.cend(), sortTextChannelsForPresentation);
	return firstTextChannel != textChannels.cend() ? std::optional< unsigned int >(firstTextChannel->textChannelID)
												   : std::nullopt;
}

bool containsTextChannelID(const std::vector<::msdb::DBTextChannel > &textChannels, unsigned int textChannelID) {
	return std::find_if(textChannels.cbegin(), textChannels.cend(),
						[textChannelID](const ::msdb::DBTextChannel &textChannel) {
							return textChannel.textChannelID == textChannelID;
						})
		   != textChannels.cend();
}

bool clientSupportsPersistentChat(const ServerUser *user) {
	return user && user->bSupportsPersistentChat;
}

QList< int > screenShareCodecListFromVersion(const MumbleProto::Version &msg) {
	QList< int > codecs;
	codecs.reserve(msg.supported_screen_share_codecs_size());
	for (int i = 0; i < msg.supported_screen_share_codecs_size(); ++i) {
		codecs.append(static_cast< int >(msg.supported_screen_share_codecs(i)));
	}

	return Mumble::ScreenShare::sanitizeCodecList(codecs);
}

void inferPersistentChatSupport(ServerUser *user) {
	if (user) {
		user->bSupportsPersistentChat = true;
	}
}

QList< int > screenShareCodecListFromCreate(const MumbleProto::ScreenShareCreate &msg) {
	QList< int > codecs;
	codecs.reserve(msg.requested_codecs_size());
	for (int i = 0; i < msg.requested_codecs_size(); ++i) {
		codecs.append(static_cast< int >(msg.requested_codecs(i)));
	}

	return Mumble::ScreenShare::sanitizeCodecList(codecs);
}

constexpr quint64 MESSAGE_SCREEN_SHARE_RELAY_TOKEN_LIFETIME_MSEC = 5ULL * 60ULL * 1000ULL;

QString randomMessageRelayCredential() {
	return QUuid::createUuid().toString(QUuid::WithoutBraces) + QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QSet< ServerUser * > legacyChannelRecipients(const QHash< unsigned int, ServerUser * > &connectedUsers,
											 const ChannelListenerManager &channelListenerManager,
											 const Channel *channel) {
	QSet< ServerUser * > recipients;
	if (!channel) {
		return recipients;
	}

	for (User *currentUser : channel->qlUsers) {
		if (currentUser) {
			recipients.insert(static_cast< ServerUser * >(currentUser));
		}
	}

	for (unsigned int session : channelListenerManager.getListenersForChannel(channel->iId)) {
		ServerUser *currentUser = connectedUsers.value(session);
		if (currentUser) {
			recipients.insert(currentUser);
		}
	}

	return recipients;
}

struct AggregateChatEntry {
	::msdb::DBChatMessage message;
	MumbleProto::ChatScope scope = MumbleProto::Channel;
	unsigned int scopeID         = 0;
};

bool newerAggregateEntry(const AggregateChatEntry &lhs, const AggregateChatEntry &rhs) {
	const std::size_t lhsCreatedAt = ::msdb::toEpochSeconds(lhs.message.createdAt);
	const std::size_t rhsCreatedAt = ::msdb::toEpochSeconds(rhs.message.createdAt);
	if (lhsCreatedAt != rhsCreatedAt) {
		return lhsCreatedAt > rhsCreatedAt;
	}
	if (lhs.message.messageID != rhs.message.messageID) {
		return lhs.message.messageID > rhs.message.messageID;
	}

	return lhs.message.threadID > rhs.message.threadID;
}

msdb::ChatMessageBodyFormat dbBodyFormatFromProto(MumbleProto::ChatBodyFormat format) {
	switch (format) {
		case MumbleProto::ChatBodyFormatMarkdownLite:
			return msdb::ChatMessageBodyFormat::MarkdownLite;
		case MumbleProto::ChatBodyFormatPlainText:
		default:
			return msdb::ChatMessageBodyFormat::PlainText;
	}
}

MumbleProto::ChatBodyFormat protoBodyFormatFromDB(msdb::ChatMessageBodyFormat format) {
	switch (format) {
		case msdb::ChatMessageBodyFormat::MarkdownLite:
			return MumbleProto::ChatBodyFormatMarkdownLite;
		case msdb::ChatMessageBodyFormat::PlainText:
		default:
			return MumbleProto::ChatBodyFormatPlainText;
	}
}

MumbleProto::ChatAssetKind protoAssetKindFromDB(msdb::ChatAssetKind kind) {
	switch (kind) {
		case msdb::ChatAssetKind::Image:
			return MumbleProto::ChatAssetKindImage;
		case msdb::ChatAssetKind::Video:
			return MumbleProto::ChatAssetKindVideo;
		case msdb::ChatAssetKind::Document:
			return MumbleProto::ChatAssetKindDocument;
		case msdb::ChatAssetKind::Binary:
			return MumbleProto::ChatAssetKindBinary;
		case msdb::ChatAssetKind::Unknown:
		default:
			return MumbleProto::ChatAssetKindUnknown;
	}
}

msdb::ChatAssetKind dbAssetKindFromProto(MumbleProto::ChatAssetKind kind) {
	switch (kind) {
		case MumbleProto::ChatAssetKindImage:
			return msdb::ChatAssetKind::Image;
		case MumbleProto::ChatAssetKindVideo:
			return msdb::ChatAssetKind::Video;
		case MumbleProto::ChatAssetKindDocument:
			return msdb::ChatAssetKind::Document;
		case MumbleProto::ChatAssetKindBinary:
			return msdb::ChatAssetKind::Binary;
		case MumbleProto::ChatAssetKindUnknown:
		default:
			return msdb::ChatAssetKind::Unknown;
	}
}

MumbleProto::ChatAssetTransferState protoTransferStateFromInt(MumbleProto::ChatAssetTransferState state) {
	return state;
}

MumbleProto::ChatEmbedStatus protoEmbedStatusFromDB(msdb::ChatEmbedStatus status) {
	switch (status) {
		case msdb::ChatEmbedStatus::Ready:
			return MumbleProto::ChatEmbedStatusReady;
		case msdb::ChatEmbedStatus::Blocked:
			return MumbleProto::ChatEmbedStatusBlocked;
		case msdb::ChatEmbedStatus::Failed:
			return MumbleProto::ChatEmbedStatusFailed;
		case msdb::ChatEmbedStatus::Pending:
		default:
			return MumbleProto::ChatEmbedStatusPending;
	}
}

QString structuredChatLegacyHtml(const QString &bodyText, msdb::ChatMessageBodyFormat bodyFormat) {
	Q_UNUSED(bodyFormat);
	return bodyText.toHtmlEscaped().replace(QLatin1Char('\n'), QLatin1String("<br/>"));
}

MumbleProto::ChatAssetRef protoAssetRefFromDB(const msdb::DBChatMessageAttachment &attachment) {
	MumbleProto::ChatAssetRef protoAttachment;
	protoAttachment.set_asset_id(attachment.assetID);
	if (!attachment.filename.empty()) {
		protoAttachment.set_filename(attachment.filename);
	}
	if (!attachment.mime.empty()) {
		protoAttachment.set_mime(attachment.mime);
	}
	protoAttachment.set_byte_size(attachment.byteSize);
	protoAttachment.set_kind(protoAssetKindFromDB(attachment.kind));
	if (attachment.width > 0) {
		protoAttachment.set_width(attachment.width);
	}
	if (attachment.height > 0) {
		protoAttachment.set_height(attachment.height);
	}
	if (attachment.durationMs > 0) {
		protoAttachment.set_duration_ms(attachment.durationMs);
	}
	protoAttachment.set_inline_safe(attachment.inlineSafe);
	if (attachment.previewAssetID) {
		protoAttachment.set_preview_asset_id(attachment.previewAssetID.value());
	}
	return protoAttachment;
}

MumbleProto::ChatEmbedRef protoEmbedRefFromDB(const msdb::DBChatMessageEmbed &embed) {
	MumbleProto::ChatEmbedRef protoEmbed;
	protoEmbed.set_canonical_url(embed.canonicalUrl);
	protoEmbed.set_status(protoEmbedStatusFromDB(embed.status));
	if (!embed.title.empty()) {
		protoEmbed.set_title(embed.title);
	}
	if (!embed.description.empty()) {
		protoEmbed.set_description(embed.description);
	}
	if (!embed.siteName.empty()) {
		protoEmbed.set_site_name(embed.siteName);
	}
	if (embed.previewAssetID) {
		protoEmbed.set_preview_asset_id(embed.previewAssetID.value());
	}
	if (!embed.errorCode.empty()) {
		protoEmbed.set_error_code(embed.errorCode);
	}
	return protoEmbed;
}

MumbleProto::ChatAssetRef protoAssetRefFromAsset(const msdb::DBChatAsset &asset, const QString &filename,
												 bool inlineSafe) {
	MumbleProto::ChatAssetRef ref;
	ref.set_asset_id(asset.assetID);
	if (!filename.isEmpty()) {
		ref.set_filename(u8(filename));
	}
	ref.set_mime(asset.mime);
	ref.set_byte_size(asset.byteSize);
	ref.set_kind(protoAssetKindFromDB(asset.kind));
	if (asset.width > 0) {
		ref.set_width(asset.width);
	}
	if (asset.height > 0) {
		ref.set_height(asset.height);
	}
	if (asset.durationMs > 0) {
		ref.set_duration_ms(asset.durationMs);
	}
	ref.set_inline_safe(inlineSafe);
	if (asset.previewAssetID) {
		ref.set_preview_asset_id(asset.previewAssetID.value());
	}
	return ref;
}

struct ChatReactionAggregateState {
	unsigned int count = 0;
	bool selfReacted   = false;
};

struct ChatReplyPreview {
	std::optional< std::string > actorName;
	std::optional< std::string > snippet;
};

std::vector< std::pair< std::string, ChatReactionAggregateState > >
	aggregateChatReactions(const std::vector< msdb::DBChatMessageReaction > &reactions,
						   std::optional< unsigned int > viewerUserID) {
	std::map< std::string, ChatReactionAggregateState > grouped;
	for (const msdb::DBChatMessageReaction &reaction : reactions) {
		ChatReactionAggregateState &aggregate = grouped[reaction.emoji];
		aggregate.count++;
		if (viewerUserID && reaction.actorUserID == viewerUserID.value()) {
			aggregate.selfReacted = true;
		}
	}

	std::vector< std::pair< std::string, ChatReactionAggregateState > > ordered(grouped.begin(), grouped.end());
	std::sort(ordered.begin(), ordered.end(), [](const auto &lhs, const auto &rhs) {
		if (lhs.second.count != rhs.second.count) {
			return lhs.second.count > rhs.second.count;
		}
		return lhs.first < rhs.first;
	});
	return ordered;
}

void appendProtoReactionAggregates(google::protobuf::RepeatedPtrField< MumbleProto::ChatReactionAggregate > *target,
								   const std::vector< msdb::DBChatMessageReaction > &reactions,
								   std::optional< unsigned int > viewerUserID) {
	if (!target) {
		return;
	}

	for (const auto &aggregate : aggregateChatReactions(reactions, viewerUserID)) {
		MumbleProto::ChatReactionAggregate *protoReaction = target->Add();
		protoReaction->set_emoji(aggregate.first);
		protoReaction->set_count(aggregate.second.count);
		protoReaction->set_self_reacted(aggregate.second.selfReacted);
	}
}

QString chatReplySnippetText(const QString &text) {
	QString normalized = text;
	normalized.replace(QLatin1String("\r\n"), QLatin1String("\n"));
	normalized.replace(QLatin1Char('\r'), QLatin1Char('\n'));
	normalized.replace(QLatin1Char('\n'), QLatin1Char(' '));
	normalized = normalized.simplified();
	if (normalized.size() > 140) {
		normalized = normalized.left(137).trimmed() + QLatin1String("...");
	}
	return normalized;
}

template< typename AuthorResolver >
std::optional< ChatReplyPreview > resolveReplyPreview(DBWrapper &dbWrapper, unsigned int serverID,
													  const ::msdb::DBChatMessage &message,
													  const AuthorResolver &resolveAuthorName) {
	if (!message.replyToMessageID) {
		return std::nullopt;
	}

	const std::optional<::msdb::DBChatMessage > replyTarget =
		dbWrapper.getChatMessage(serverID, message.replyToMessageID.value());
	if (!replyTarget || replyTarget->deletedAt > std::chrono::system_clock::time_point()) {
		return std::nullopt;
	}

	ChatReplyPreview preview;
	preview.actorName = resolveAuthorName(*replyTarget);

	const QString snippetText = chatReplySnippetText(u8(replyTarget->bodyText));
	if (!snippetText.isEmpty()) {
		preview.snippet = u8(snippetText);
	}

	return preview;
}

QString normalizedMime(const QString &mime) {
	return mime.section(QLatin1Char(';'), 0, 0).trimmed().toLower();
}

msdb::ChatAssetKind inferredAssetKind(const QString &mime) {
	if (mime.startsWith(QLatin1String("image/"))) {
		return msdb::ChatAssetKind::Image;
	}
	if (mime.startsWith(QLatin1String("video/"))) {
		return msdb::ChatAssetKind::Video;
	}
	if (mime == QLatin1String("application/pdf") || mime.startsWith(QLatin1String("text/"))) {
		return msdb::ChatAssetKind::Document;
	}
	return msdb::ChatAssetKind::Binary;
}

bool isAllowedChatAssetMime(msdb::ChatAssetKind kind, const QString &mime) {
	static const QSet< QString > imageMimes = {
		QStringLiteral("image/png"), QStringLiteral("image/jpeg"), QStringLiteral("image/webp"),
		QStringLiteral("image/gif"), QStringLiteral("image/bmp"),
	};
	static const QSet< QString > videoMimes = {
		QStringLiteral("video/mp4"),
		QStringLiteral("video/webm"),
		QStringLiteral("video/quicktime"),
	};
	static const QSet< QString > documentMimes = {
		QStringLiteral("application/pdf"),
		QStringLiteral("text/plain"),
		QStringLiteral("text/markdown"),
	};
	static const QSet< QString > binaryMimes = {
		QStringLiteral("application/octet-stream"),
		QStringLiteral("application/zip"),
	};

	switch (kind) {
		case msdb::ChatAssetKind::Image:
			return imageMimes.contains(mime);
		case msdb::ChatAssetKind::Video:
			return videoMimes.contains(mime);
		case msdb::ChatAssetKind::Document:
			return documentMimes.contains(mime);
		case msdb::ChatAssetKind::Binary:
			return binaryMimes.contains(mime);
		case msdb::ChatAssetKind::Unknown:
		default:
			return false;
	}
}

bool isInlineSafeAsset(msdb::ChatAssetKind kind, const QString &mime, bool requestInline) {
	if (!requestInline) {
		return false;
	}

	return kind == msdb::ChatAssetKind::Image && mime != QLatin1String("image/svg+xml")
		   && mime != QLatin1String("image/svg");
}

bool isValidSha256Hex(const QString &sha256) {
	if (sha256.size() != 64) {
		return false;
	}

	for (const QChar ch : sha256) {
		if (!ch.isDigit() && (ch.toLower() < QLatin1Char('a') || ch.toLower() > QLatin1Char('f'))) {
			return false;
		}
	}

	return true;
}

constexpr int CHAT_PREVIEW_TIMEOUT_MSEC        = 8000;
constexpr qint64 CHAT_PREVIEW_MAX_PAGE_BYTES   = 512 * 1024;
constexpr qint64 CHAT_PREVIEW_MAX_IMAGE_BYTES  = 4 * 1024 * 1024;
constexpr int CHAT_PREVIEW_MAX_REDIRECTS       = 3;
constexpr int CHAT_PREVIEW_MAX_CONCURRENT_HOST = 2;
constexpr int CHAT_PREVIEW_THUMBNAIL_WIDTH     = 640;
constexpr int CHAT_PREVIEW_THUMBNAIL_HEIGHT    = 480;
static const QByteArray s_chatPreviewBrowserUserAgent =
	QByteArrayLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
					  "(KHTML, like Gecko) Chrome/135.0.0.0 Safari/537.36");
static const QByteArray s_chatPreviewAcceptHeader =
	QByteArrayLiteral("text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/*,*/*;q=0.8");
static const QByteArray s_chatPreviewAcceptLanguageHeader = QByteArrayLiteral("en-US,en;q=0.9");

void prepareChatPreviewRequest(QNetworkRequest &request) {
	request.setRawHeader(QByteArrayLiteral("User-Agent"), s_chatPreviewBrowserUserAgent);
	request.setRawHeader(QByteArrayLiteral("Accept"), s_chatPreviewAcceptHeader);
	request.setRawHeader(QByteArrayLiteral("Accept-Language"), s_chatPreviewAcceptLanguageHeader);
}

bool previewContentTypeLooksHtml(const QString &contentType) {
	return contentType.isEmpty() || contentType.contains(QLatin1String("html"))
		   || contentType.contains(QLatin1String("xml"));
}

QString previewImageMetaTag(const QHash< QString, QString > &metaTags) {
	static const QStringList preferredKeys = {
		QStringLiteral("og:image"),      QStringLiteral("og:image:url"),      QStringLiteral("og:image:secure_url"),
		QStringLiteral("twitter:image"), QStringLiteral("twitter:image:src"),
	};

	for (const QString &key : preferredKeys) {
		const QString value = metaTags.value(key).trimmed();
		if (!value.isEmpty()) {
			return value;
		}
	}

	return QString();
}

bool isSanitizableImageMime(const QString &mime) {
	return mime == QLatin1String("image/png") || mime == QLatin1String("image/jpeg")
		   || mime == QLatin1String("image/webp") || mime == QLatin1String("image/gif")
		   || mime == QLatin1String("image/bmp");
}

bool isDirectImageUrl(const QUrl &url) {
	const QString path = url.path().toLower();
	return path.endsWith(QLatin1String(".png")) || path.endsWith(QLatin1String(".jpg"))
		   || path.endsWith(QLatin1String(".jpeg")) || path.endsWith(QLatin1String(".webp"))
		   || path.endsWith(QLatin1String(".gif")) || path.endsWith(QLatin1String(".bmp"));
}

QImage decodeChatImage(const QByteArray &bytes) {
	QBuffer buffer;
	buffer.setData(bytes);
	if (!buffer.open(QIODevice::ReadOnly)) {
		return QImage();
	}

	QImageReader reader(&buffer);
	reader.setAutoTransform(true);
	return reader.read();
}

QByteArray encodeChatImage(const QImage &image, const char *format, int quality = -1) {
	QByteArray encoded;
	QBuffer buffer(&encoded);
	if (!buffer.open(QIODevice::WriteOnly)) {
		return QByteArray();
	}

	QImageWriter writer(&buffer, format);
	writer.setQuality(quality);
	if (!writer.write(image)) {
		return QByteArray();
	}

	return encoded;
}

struct SanitizedChatImage {
	QByteArray bytes;
	QString mime;
	unsigned int width  = 0;
	unsigned int height = 0;
};

std::optional< SanitizedChatImage > sanitizeChatImageBytes(const QByteArray &bytes, bool thumbnailOnly = false) {
	const QImage image = decodeChatImage(bytes);
	if (image.isNull()) {
		return std::nullopt;
	}

	QImage normalized = thumbnailOnly ? image.scaled(CHAT_PREVIEW_THUMBNAIL_WIDTH, CHAT_PREVIEW_THUMBNAIL_HEIGHT,
													 Qt::KeepAspectRatio, Qt::SmoothTransformation)
									  : image;
	SanitizedChatImage result;
	result.width  = static_cast< unsigned int >(normalized.width());
	result.height = static_cast< unsigned int >(normalized.height());

	if (normalized.hasAlphaChannel()) {
		result.bytes = encodeChatImage(normalized, "PNG");
		result.mime  = QStringLiteral("image/png");
	} else {
		result.bytes = encodeChatImage(normalized, "JPEG", 90);
		result.mime  = QStringLiteral("image/jpeg");
	}

	if (result.bytes.isEmpty()) {
		return std::nullopt;
	}

	return result;
}

bool isBlockedPreviewAddress(const QHostAddress &address) {
	if (address.isNull() || address.isLoopback() || address.isBroadcast() || address.isMulticast()) {
		return true;
	}

	if (address.protocol() == QAbstractSocket::IPv4Protocol) {
		const quint32 ip          = address.toIPv4Address();
		const quint32 firstOctet  = (ip >> 24) & 0xffU;
		const quint32 secondOctet = (ip >> 16) & 0xffU;
		if (firstOctet == 0 || firstOctet == 10 || firstOctet == 127) {
			return true;
		}
		if (firstOctet == 169 && secondOctet == 254) {
			return true;
		}
		if (firstOctet == 172 && secondOctet >= 16 && secondOctet <= 31) {
			return true;
		}
		if (firstOctet == 192 && secondOctet == 168) {
			return true;
		}
		if (firstOctet == 100 && secondOctet >= 64 && secondOctet <= 127) {
			return true;
		}
		return false;
	}

	const Q_IPV6ADDR ipv6 = address.toIPv6Address();
	if ((ipv6[0] & 0xfeU) == 0xfcU) {
		return true;
	}
	if (ipv6[0] == 0xfeU && (ipv6[1] & 0xc0U) == 0x80U) {
		return true;
	}
	if (ipv6[0] == 0xfeU && (ipv6[1] & 0xc0U) == 0xc0U) {
		return true;
	}
	return address == QHostAddress::AnyIPv6 || address == QHostAddress::Any;
}

bool isSafePreviewUrl(const QUrl &url) {
	if (!url.isValid() || url.scheme() != QLatin1String("https") || url.host().trimmed().isEmpty()) {
		return false;
	}

	const QHostAddress directAddress(url.host());
	if (!directAddress.isNull()) {
		return !isBlockedPreviewAddress(directAddress);
	}

	const QHostInfo resolved = QHostInfo::fromName(url.host());
	if (resolved.error() != QHostInfo::NoError || resolved.addresses().isEmpty()) {
		return false;
	}

	for (const QHostAddress &address : resolved.addresses()) {
		if (isBlockedPreviewAddress(address)) {
			return false;
		}
	}

	return true;
}

QList< QUrl > extractPreviewableUrls(const QString &text) {
	static const QRegularExpression urlPattern(QStringLiteral(R"((https://[^\s<>()\[\]{}"']+))"),
											   QRegularExpression::CaseInsensitiveOption);
	QList< QUrl > urls;
	QRegularExpressionMatchIterator it = urlPattern.globalMatch(text);
	while (it.hasNext()) {
		const QRegularExpressionMatch match = it.next();
		const QUrl url(match.captured(1));
		if (!url.isValid()) {
			continue;
		}
		urls.append(url);
	}
	return urls;
}

QString decodeHtmlEntityPreviewText(QString text) {
	text.replace(QStringLiteral("&amp;"), QStringLiteral("&"));
	text.replace(QStringLiteral("&quot;"), QStringLiteral("\""));
	text.replace(QStringLiteral("&#39;"), QStringLiteral("'"));
	text.replace(QStringLiteral("&lt;"), QStringLiteral("<"));
	text.replace(QStringLiteral("&gt;"), QStringLiteral(">"));
	return text.trimmed();
}

QString extractHtmlTitle(const QString &html) {
	static const QRegularExpression titlePattern(QStringLiteral(R"(<title[^>]*>(.*?)</title>)"),
												 QRegularExpression::CaseInsensitiveOption
													 | QRegularExpression::DotMatchesEverythingOption);
	const QRegularExpressionMatch match = titlePattern.match(html);
	return match.hasMatch() ? decodeHtmlEntityPreviewText(match.captured(1)) : QString();
}

QHash< QString, QString > extractMetaTags(const QString &html) {
	QHash< QString, QString > tags;
	static const QRegularExpression metaPattern(QStringLiteral(R"(<meta\b([^>]+)>)"),
												QRegularExpression::CaseInsensitiveOption
													| QRegularExpression::DotMatchesEverythingOption);
	static const QRegularExpression attrPattern(QStringLiteral(R"(([A-Za-z_:.-]+)\s*=\s*(['"])(.*?)\2)"),
												QRegularExpression::CaseInsensitiveOption
													| QRegularExpression::DotMatchesEverythingOption);

	QRegularExpressionMatchIterator metaIt = metaPattern.globalMatch(html);
	while (metaIt.hasNext()) {
		const QString attrs = metaIt.next().captured(1);
		QHash< QString, QString > parsedAttrs;
		QRegularExpressionMatchIterator attrIt = attrPattern.globalMatch(attrs);
		while (attrIt.hasNext()) {
			const QRegularExpressionMatch attr = attrIt.next();
			parsedAttrs.insert(attr.captured(1).toLower(), decodeHtmlEntityPreviewText(attr.captured(3)));
		}

		const QString key =
			parsedAttrs.value(QStringLiteral("property"), parsedAttrs.value(QStringLiteral("name"))).toLower();
		const QString content = parsedAttrs.value(QStringLiteral("content"));
		if (!key.isEmpty() && !content.isEmpty()) {
			tags.insert(key, content);
		}
	}

	return tags;
}

quint64 randomUploadID(const QHash< quint64, Server::PendingChatAssetUpload > &pendingUploads) {
	quint64 uploadID = 0;
	do {
		uploadID = QRandomGenerator::global()->generate64();
	} while (uploadID == 0 || pendingUploads.contains(uploadID));

	return uploadID;
}

void sendChatAssetState(Server *server, ServerUser *recipient, quint64 uploadID,
						MumbleProto::ChatAssetTransferState state, const QString &reason = QString(),
						quint64 acceptedByteSize                                = 0,
						const std::optional< MumbleProto::ChatAssetRef > &asset = std::nullopt) {
	MumbleProto::ChatAssetState response;
	response.set_upload_id(uploadID);
	response.set_state(protoTransferStateFromInt(state));
	if (!reason.isEmpty()) {
		response.set_reason(u8(reason));
	}
	if (acceptedByteSize > 0) {
		response.set_accepted_byte_size(acceptedByteSize);
	}
	if (asset) {
		*response.mutable_asset() = asset.value();
	}
	server->sendMessage(recipient, response);
}

MumbleProto::ChatMessage protoChatMessageFromDB(const ::msdb::DBChatMessage &message, MumbleProto::ChatScope scope,
												unsigned int scopeID,
												const std::optional< std::string > &resolvedAuthorName = std::nullopt,
												std::optional< unsigned int > viewerUserID             = std::nullopt,
												const std::optional< ChatReplyPreview > &replyPreview  = std::nullopt) {
	const bool deleted = message.deletedAt > std::chrono::system_clock::time_point();
	MumbleProto::ChatMessage protoMessage;
	protoMessage.set_scope(scope);
	protoMessage.set_scope_id(scopeID);
	protoMessage.set_thread_id(message.threadID);
	protoMessage.set_message_id(message.messageID);
	if (message.authorSession) {
		protoMessage.set_actor(message.authorSession.value());
	}
	if (message.authorUserID) {
		protoMessage.set_actor_user_id(message.authorUserID.value());
	}
	if (resolvedAuthorName && !resolvedAuthorName->empty()) {
		protoMessage.set_actor_name(*resolvedAuthorName);
	}
	if (!deleted && message.replyToMessageID) {
		protoMessage.set_reply_to_message_id(message.replyToMessageID.value());
	}
	if (!deleted && replyPreview) {
		if (replyPreview->actorName && !replyPreview->actorName->empty()) {
			protoMessage.set_reply_actor_name(replyPreview->actorName.value());
		}
		if (replyPreview->snippet && !replyPreview->snippet->empty()) {
			protoMessage.set_reply_snippet(replyPreview->snippet.value());
		}
	}
	if (deleted) {
		protoMessage.set_message(std::string());
	} else {
		protoMessage.set_body_text(message.bodyText);
		protoMessage.set_body_format(protoBodyFormatFromDB(message.bodyFormat));
		protoMessage.set_message(u8(structuredChatLegacyHtml(u8(message.bodyText), message.bodyFormat)));
		for (const msdb::DBChatMessageAttachment &attachment : message.attachments) {
			*protoMessage.add_attachments() = protoAssetRefFromDB(attachment);
		}
		for (const msdb::DBChatMessageEmbed &embed : message.embeds) {
			*protoMessage.add_embeds() = protoEmbedRefFromDB(embed);
		}
		appendProtoReactionAggregates(protoMessage.mutable_reactions(), message.reactions, viewerUserID);
	}
	protoMessage.set_created_at(::msdb::toEpochSeconds(message.createdAt));
	if (!deleted) {
		protoMessage.set_edited_at(::msdb::toEpochSeconds(message.editedAt));
	}
	if (deleted) {
		protoMessage.set_deleted_at(::msdb::toEpochSeconds(message.deletedAt));
	}

	return protoMessage;
}

MumbleProto::ChatReactionState protoReactionStateForMessage(const ::msdb::DBChatMessage &message,
															MumbleProto::ChatScope scope, unsigned int scopeID,
															std::optional< unsigned int > viewerUserID) {
	MumbleProto::ChatReactionState state;
	state.set_scope(scope);
	state.set_scope_id(scopeID);
	state.set_thread_id(message.threadID);
	state.set_message_id(message.messageID);
	appendProtoReactionAggregates(state.mutable_reactions(), message.reactions, viewerUserID);
	return state;
}

MumbleProto::ChatReadStateUpdate protoReadStateFromDB(const ::msdb::DBChatReadState &readState,
													  MumbleProto::ChatScope scope, unsigned int scopeID) {
	MumbleProto::ChatReadStateUpdate protoUpdate;
	protoUpdate.set_scope(scope);
	protoUpdate.set_scope_id(scopeID);
	protoUpdate.set_thread_id(readState.threadID);
	protoUpdate.set_user_id(readState.userID);
	protoUpdate.set_last_read_message_id(readState.lastReadMessageID);
	protoUpdate.set_updated_at(::msdb::toEpochSeconds(readState.updatedAt));

	return protoUpdate;
}

MumbleProto::TextMessage legacyTextMessageFromPersistent(const MumbleProto::ChatMessage &message) {
	MumbleProto::TextMessage legacyMessage;
	if (message.has_actor()) {
		legacyMessage.set_actor(message.actor());
	}
	const QString bodyText = message.has_body_text() ? u8(message.body_text()) : u8(message.message());
	const MumbleProto::ChatBodyFormat bodyFormat =
		message.has_body_format() ? message.body_format() : MumbleProto::ChatBodyFormatPlainText;
	legacyMessage.set_message(u8(structuredChatLegacyHtml(bodyText, dbBodyFormatFromProto(bodyFormat))));

	switch (message.has_scope() ? message.scope() : MumbleProto::Channel) {
		case MumbleProto::Channel:
			legacyMessage.add_channel_id(message.has_scope_id() ? message.scope_id() : Mumble::ROOT_CHANNEL_ID);
			break;
		case MumbleProto::ServerGlobal:
			legacyMessage.add_tree_id(Mumble::ROOT_CHANNEL_ID);
			break;
		case MumbleProto::Aggregate:
		case MumbleProto::TextChannel:
			break;
	}

	return legacyMessage;
}

void sendPersistentChatTextDenied(Server *server, ServerUser *user, const QString &reason) {
	if (!server || !user) {
		return;
	}

	MumbleProto::PermissionDenied denied;
	denied.set_session(user->uiSession);
	denied.set_type(MumbleProto::PermissionDenied_DenyType_Text);
	denied.set_reason(u8(reason));
	server->sendMessage(user, denied);
}
} // namespace

void Server::sendPersistentChatUnsupported(ServerUser *uSource) {
	MumbleProto::PermissionDenied denied;
	denied.set_session(uSource->uiSession);
	denied.set_type(MumbleProto::PermissionDenied_DenyType_Text);
	denied.set_reason(u8(QStringLiteral(
		"This client version does not support persistent chat. Upgrade the client to use persistent chat features.")));
	sendMessage(uSource, denied);
}

void Server::sendTextChannelSync(ServerUser *uSource) {
	if (!clientSupportsPersistentChat(uSource)) {
		return;
	}

	QMutexLocker qml(&qmCache);

	MumbleProto::TextChannelSync sync;
	std::vector<::msdb::DBTextChannel > textChannels = m_dbWrapper.getTextChannels(iServerNum);
	std::sort(textChannels.begin(), textChannels.end(), sortTextChannelsForPresentation);
	const std::optional< unsigned int > configuredDefaultTextChannel =
		configuredDefaultTextChannelID(m_dbWrapper, iServerNum);
	std::optional< unsigned int > fallbackDefaultTextChannelID;

	for (const ::msdb::DBTextChannel &currentTextChannel : textChannels) {
		Channel *permissionChannel = qhChannels.value(currentTextChannel.aclChannelID);
		if (!permissionChannel) {
			continue;
		}

		if (!ChanACL::hasPermission(uSource, permissionChannel, ChanACL::ViewTextMessageHistory, &acCache)) {
			continue;
		}

		MumbleProto::TextChannelInfo *protoChannel = sync.add_channels();
		protoChannel->set_text_channel_id(currentTextChannel.textChannelID);
		protoChannel->set_name(currentTextChannel.name);
		protoChannel->set_description(currentTextChannel.description);
		protoChannel->set_acl_channel_id(currentTextChannel.aclChannelID);
		protoChannel->set_position(currentTextChannel.position);

		if (!fallbackDefaultTextChannelID) {
			fallbackDefaultTextChannelID = currentTextChannel.textChannelID;
		}
		if (configuredDefaultTextChannel && *configuredDefaultTextChannel == currentTextChannel.textChannelID) {
			sync.set_default_text_channel_id(currentTextChannel.textChannelID);
		}
	}

	if (!sync.has_default_text_channel_id() && fallbackDefaultTextChannelID) {
		sync.set_default_text_channel_id(*fallbackDefaultTextChannelID);
	}

	sendMessage(uSource, sync);
}

void Server::persistAndBroadcastChatMessage(ServerUser *uSource, const QString &bodyText,
											::msdb::ChatMessageBodyFormat bodyFormat, MumbleProto::ChatScope scope,
											unsigned int scopeID, Channel *permissionChannel,
											::msdb::ChatThreadScope dbScope,
											const std::vector<::msdb::DBChatMessageAttachment > &attachments,
											std::optional< unsigned int > replyToMessageID,
											const QSet< ServerUser * > &legacyFallbackRecipients) {
	const std::optional< unsigned int > authorUserID = persistedUserID(uSource);
	const std::optional< std::string > authorName =
		uSource->qsName.isEmpty() ? std::nullopt : std::optional< std::string >(u8(uSource->qsName));
	const std::string scopeKey = chatScopeKey(scope, scopeID);
	if (scopeKey.empty()) {
		return;
	}

	::msdb::DBChatThread thread = m_dbWrapper.ensureChatThread(iServerNum, dbScope, scopeKey, authorUserID);
	::msdb::DBChatMessage storedMessage =
		m_dbWrapper.addChatMessage(iServerNum, thread.threadID, u8(bodyText), bodyFormat, attachments, replyToMessageID,
								   authorUserID, uSource->uiSession, authorName);

	const auto now = std::chrono::system_clock::now();
	std::vector<::msdb::DBChatMessageEmbed > initialEmbeds;
	if (bChatPreviewFetchEnabled) {
		QSet< QString > seenUrls;
		const QList< QUrl > previewUrls = extractPreviewableUrls(bodyText);
		for (const QUrl &previewUrl : previewUrls) {
			const QString canonicalUrl = previewUrl.adjusted(QUrl::RemoveFragment).toString();
			if (canonicalUrl.isEmpty() || seenUrls.contains(canonicalUrl)) {
				continue;
			}
			seenUrls.insert(canonicalUrl);

			::msdb::DBChatMessageEmbed embed(iServerNum, storedMessage.messageID);
			embed.urlHash      = u8(QString::fromLatin1(
                QCryptographicHash::hash(canonicalUrl.toUtf8(), QCryptographicHash::Sha256).toHex()));
			embed.canonicalUrl = u8(canonicalUrl);
			embed.status =
				isSafePreviewUrl(previewUrl) ? ::msdb::ChatEmbedStatus::Pending : ::msdb::ChatEmbedStatus::Blocked;
			embed.errorCode = embed.status == ::msdb::ChatEmbedStatus::Blocked ? "blocked_target" : "";
			embed.fetchedAt = now;
			embed.expiresAt = now + std::chrono::hours(24 * 7);
			initialEmbeds.push_back(std::move(embed));

			if (initialEmbeds.size() >= 3) {
				break;
			}
		}
	}

	if (!initialEmbeds.empty()) {
		m_dbWrapper.setChatMessageEmbeds(iServerNum, storedMessage.messageID, initialEmbeds);
		storedMessage.embeds = initialEmbeds;
	}

	const auto resolvedAuthorName = [this](const ::msdb::DBChatMessage &message) -> std::optional< std::string > {
		if (message.authorName && !message.authorName->empty()) {
			return message.authorName;
		}

		if (message.authorSession) {
			ServerUser *currentUser = qhUsers.value(message.authorSession.value());
			if (currentUser && !currentUser->qsName.isEmpty()) {
				return u8(currentUser->qsName);
			}
		}

		if (message.authorUserID && m_dbWrapper.registeredUserExists(iServerNum, message.authorUserID.value())) {
			return m_dbWrapper.getUserName(iServerNum, message.authorUserID.value());
		}

		return std::nullopt;
	};
	const std::optional< ChatReplyPreview > replyPreview =
		resolveReplyPreview(m_dbWrapper, iServerNum, storedMessage, resolvedAuthorName);
	MumbleProto::ChatMessage protoMessage =
		protoChatMessageFromDB(storedMessage, scope, scopeID, authorName, std::nullopt, replyPreview);

	QSet< ServerUser * > persistentRecipients;
	if (scope == MumbleProto::Channel) {
		persistentRecipients = legacyFallbackRecipients;
		persistentRecipients.insert(uSource);
	} else {
		persistentRecipients = recipientsWithChatHistoryAccess(qhUsers, permissionChannel, acCache);
	}

	for (ServerUser *currentUser : persistentRecipients) {
		if (clientSupportsPersistentChat(currentUser)) {
			sendMessage(currentUser, protoMessage);
		}
	}

	if (!legacyFallbackRecipients.isEmpty()) {
		MumbleProto::TextMessage legacyMessage = legacyTextMessageFromPersistent(protoMessage);
		for (ServerUser *currentUser : legacyFallbackRecipients) {
			if (!currentUser || clientSupportsPersistentChat(currentUser) || currentUser == uSource) {
				continue;
			}

			sendMessage(currentUser, legacyMessage);
		}
	}

	if (authorUserID) {
		::msdb::DBChatReadState readState(iServerNum, thread.threadID, authorUserID.value());
		readState.lastReadMessageID = storedMessage.messageID;
		readState.updatedAt         = std::chrono::system_clock::now();
		m_dbWrapper.setChatReadState(readState);

		if (clientSupportsPersistentChat(uSource)) {
			std::optional<::msdb::DBChatReadState > persistedReadState =
				m_dbWrapper.getChatReadState(iServerNum, thread.threadID, authorUserID.value());
			if (persistedReadState) {
				sendMessage(uSource, protoReadStateFromDB(*persistedReadState, scope, scopeID));
			}
		}
	}

	if (bChatPreviewFetchEnabled && permissionChannel) {
		for (const ::msdb::DBChatMessageEmbed &embed : initialEmbeds) {
			if (embed.status == ::msdb::ChatEmbedStatus::Pending) {
				scheduleChatEmbedFetch(thread.threadID, storedMessage.messageID, scope, scopeID,
									   static_cast< unsigned int >(permissionChannel->iId), embed);
			}
		}
	}
}

void Server::applyChatEmbedFetchResult(unsigned int threadID, unsigned int messageID, MumbleProto::ChatScope scope,
									   unsigned int scopeID, unsigned int permissionChannelID,
									   const ::msdb::DBChatMessageEmbed &embed) {
	QMutexLocker qml(&qmCache);

	std::optional<::msdb::DBChatMessage > message = m_dbWrapper.getChatMessage(iServerNum, messageID);
	if (!message || message->deletedAt > std::chrono::system_clock::time_point()) {
		return;
	}

	std::vector<::msdb::DBChatMessageEmbed > embeds = m_dbWrapper.getChatMessageEmbeds(iServerNum, messageID);
	bool replaced                                   = false;
	for (auto &currentEmbed : embeds) {
		if (currentEmbed.urlHash == embed.urlHash) {
			currentEmbed = embed;
			replaced     = true;
			break;
		}
	}
	if (!replaced) {
		embeds.push_back(embed);
	}

	m_dbWrapper.setChatMessageEmbeds(iServerNum, messageID, embeds);

	Channel *permissionChannel = qhChannels.value(permissionChannelID);
	if (!permissionChannel) {
		return;
	}

	MumbleProto::ChatEmbedState state;
	state.set_scope(scope);
	state.set_scope_id(scopeID);
	state.set_thread_id(threadID);
	state.set_message_id(messageID);
	for (const auto &currentEmbed : embeds) {
		*state.add_embeds() = protoEmbedRefFromDB(currentEmbed);
	}

	for (ServerUser *currentUser : recipientsWithChatHistoryAccess(qhUsers, permissionChannel, acCache)) {
		if (clientSupportsPersistentChat(currentUser)) {
			sendMessage(currentUser, state);
		}
	}
}

void Server::scheduleChatEmbedFetch(unsigned int threadID, unsigned int messageID, MumbleProto::ChatScope scope,
									unsigned int scopeID, unsigned int permissionChannelID,
									const ::msdb::DBChatMessageEmbed &initialEmbed) {
	if (!qnamNetwork) {
		return;
	}

	const auto persistPreviewImage = [this](const SanitizedChatImage &thumbnail) -> std::optional< unsigned int > {
		const QString sha256 =
			QString::fromLatin1(QCryptographicHash::hash(thumbnail.bytes, QCryptographicHash::Sha256).toHex());
		const QString storageKey = chatAssetStorageKey(0, sha256);
		const QString objectPath = chatAssetAbsolutePath(storageKey);
		QDir rootDir;
		if (!rootDir.mkpath(QFileInfo(objectPath).absolutePath())) {
			return std::nullopt;
		}
		if (!QFile::exists(objectPath)) {
			QFile objectFile(objectPath);
			if (!objectFile.open(QIODevice::WriteOnly) || objectFile.write(thumbnail.bytes) != thumbnail.bytes.size()) {
				return std::nullopt;
			}
			objectFile.close();
		}

		::msdb::DBChatAsset asset;
		asset.serverID       = iServerNum;
		asset.sha256         = u8(sha256);
		asset.storageKey     = u8(storageKey);
		asset.mime           = u8(thumbnail.mime);
		asset.byteSize       = static_cast< std::uint64_t >(thumbnail.bytes.size());
		asset.kind           = ::msdb::ChatAssetKind::Image;
		asset.width          = thumbnail.width;
		asset.height         = thumbnail.height;
		asset.retentionClass = ::msdb::ChatAssetRetentionClass::PreviewCache;

		return m_dbWrapper.addChatAsset(asset).assetID;
	};

	const auto finish = [this, threadID, messageID, scope, scopeID,
						 permissionChannelID](::msdb::DBChatMessageEmbed embed) {
		embed.fetchedAt = std::chrono::system_clock::now();
		QMetaObject::invokeMethod(
			this,
			[this, threadID, messageID, scope, scopeID, permissionChannelID, embed]() {
				applyChatEmbedFetchResult(threadID, messageID, scope, scopeID, permissionChannelID, embed);
			},
			Qt::QueuedConnection);
	};

	const auto updateHostCount = [this](const QString &hostKey, int delta) {
		if (hostKey.isEmpty()) {
			return;
		}

		const unsigned int current = qhChatPreviewFetchesByHost.value(hostKey, 0);
		if (delta > 0) {
			qhChatPreviewFetchesByHost.insert(hostKey, current + static_cast< unsigned int >(delta));
		} else {
			const unsigned int decrease = static_cast< unsigned int >(-delta);
			if (current <= decrease) {
				qhChatPreviewFetchesByHost.remove(hostKey);
			} else {
				qhChatPreviewFetchesByHost.insert(hostKey, current - decrease);
			}
		}
	};

	auto embedState      = std::make_shared<::msdb::DBChatMessageEmbed >(initialEmbed);
	auto pageTitle       = std::make_shared< QString >();
	auto pageDescription = std::make_shared< QString >();
	auto pageSiteName    = std::make_shared< QString >();

	auto fetchImage = std::make_shared< std::function< void(QUrl, unsigned int) > >();
	*fetchImage = [this, fetchImage, embedState, pageTitle, pageDescription, pageSiteName, persistPreviewImage, finish,
				   updateHostCount](QUrl imageUrl, unsigned int redirectCount) mutable {
		if (redirectCount > CHAT_PREVIEW_MAX_REDIRECTS || !isSafePreviewUrl(imageUrl)) {
			embedState->status    = ::msdb::ChatEmbedStatus::Blocked;
			embedState->errorCode = "blocked_target";
			finish(*embedState);
			return;
		}

		const QString hostKey = imageUrl.host().trimmed().toLower();
		if (qhChatPreviewFetchesByHost.value(hostKey, 0) >= CHAT_PREVIEW_MAX_CONCURRENT_HOST) {
			embedState->status    = ::msdb::ChatEmbedStatus::Failed;
			embedState->errorCode = "host_busy";
			finish(*embedState);
			return;
		}

		updateHostCount(hostKey, +1);
		QNetworkRequest request(imageUrl);
		prepareChatPreviewRequest(request);
		request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::ManualRedirectPolicy);
		request.setTransferTimeout(CHAT_PREVIEW_TIMEOUT_MSEC);
		QNetworkReply *reply = qnamNetwork->get(request);
		reply->setReadBufferSize(CHAT_PREVIEW_MAX_IMAGE_BYTES);
		connect(
			reply, &QNetworkReply::finished, this,
			[this, reply, hostKey, fetchImage, embedState, pageTitle, pageDescription, pageSiteName,
			 persistPreviewImage, finish, updateHostCount, redirectCount]() mutable {
				updateHostCount(hostKey, -1);
				const QVariant redirectTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
				const QString contentType =
					reply->header(QNetworkRequest::ContentTypeHeader).toString().section(';', 0, 0).trimmed().toLower();
				const QByteArray bytes = reply->readAll();
				const bool success     = reply->error() == QNetworkReply::NoError;
				const QUrl sourceUrl   = reply->request().url();
				reply->deleteLater();

				if (redirectTarget.isValid()) {
					(*fetchImage)(sourceUrl.resolved(redirectTarget.toUrl()), redirectCount + 1);
					return;
				}

				embedState->title       = u8(*pageTitle);
				embedState->description = u8(*pageDescription);
				embedState->siteName    = u8(*pageSiteName);

				if (!success || bytes.size() > CHAT_PREVIEW_MAX_IMAGE_BYTES
					|| (!contentType.startsWith(QLatin1String("image/")) && !contentType.isEmpty())) {
					embedState->status    = ::msdb::ChatEmbedStatus::Ready;
					embedState->errorCode = success ? "" : "image_fetch_failed";
					finish(*embedState);
					return;
				}

				if (const auto thumbnail = sanitizeChatImageBytes(bytes, true); thumbnail) {
					embedState->previewAssetID = persistPreviewImage(*thumbnail);
				}

				embedState->status    = ::msdb::ChatEmbedStatus::Ready;
				embedState->errorCode = "";
				finish(*embedState);
			});
	};

	auto fetchPage = std::make_shared< std::function< void(QUrl, unsigned int) > >();
	*fetchPage     = [this, fetchPage, fetchImage, embedState, pageTitle, pageDescription, pageSiteName, finish,
                  updateHostCount](QUrl pageUrl, unsigned int redirectCount) mutable {
        if (redirectCount > CHAT_PREVIEW_MAX_REDIRECTS || !isSafePreviewUrl(pageUrl)) {
            embedState->status    = ::msdb::ChatEmbedStatus::Blocked;
            embedState->errorCode = "blocked_target";
            finish(*embedState);
            return;
        }

        const QString hostKey = pageUrl.host().trimmed().toLower();
        if (qhChatPreviewFetchesByHost.value(hostKey, 0) >= CHAT_PREVIEW_MAX_CONCURRENT_HOST) {
            embedState->status    = ::msdb::ChatEmbedStatus::Failed;
            embedState->errorCode = "host_busy";
            finish(*embedState);
            return;
        }

        updateHostCount(hostKey, +1);
        QNetworkRequest request(pageUrl);
        prepareChatPreviewRequest(request);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::ManualRedirectPolicy);
        request.setTransferTimeout(CHAT_PREVIEW_TIMEOUT_MSEC);
        QNetworkReply *reply = qnamNetwork->get(request);
        reply->setReadBufferSize(CHAT_PREVIEW_MAX_PAGE_BYTES);
        connect(
            reply, &QNetworkReply::finished, this,
            [this, reply, fetchPage, fetchImage, embedState, pageTitle, pageDescription, pageSiteName, finish,
             updateHostCount, hostKey, redirectCount]() mutable {
                updateHostCount(hostKey, -1);
                const QVariant redirectTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
                const QString contentType =
                    reply->header(QNetworkRequest::ContentTypeHeader).toString().section(';', 0, 0).trimmed().toLower();
                const QByteArray bytes = reply->readAll();
                const bool success     = reply->error() == QNetworkReply::NoError;
                const QUrl sourceUrl   = reply->request().url();
                reply->deleteLater();

                if (redirectTarget.isValid()) {
                    (*fetchPage)(sourceUrl.resolved(redirectTarget.toUrl()), redirectCount + 1);
                    return;
                }

                if (!success || bytes.size() > CHAT_PREVIEW_MAX_PAGE_BYTES) {
                    if (!success) {
                        embedState->status    = ::msdb::ChatEmbedStatus::Failed;
                        embedState->errorCode = "fetch_failed";
                        finish(*embedState);
                        return;
                    }
                }

                if (contentType.startsWith(QLatin1String("image/")) || isDirectImageUrl(sourceUrl)) {
                    if (bytes.size() > CHAT_PREVIEW_MAX_PAGE_BYTES) {
                        embedState->status    = ::msdb::ChatEmbedStatus::Failed;
                        embedState->errorCode = "fetch_failed";
                        finish(*embedState);
                        return;
                    }
                    *pageTitle       = QFileInfo(sourceUrl.path()).fileName();
                    *pageDescription = QObject::tr("Direct image preview");
                    *pageSiteName    = sourceUrl.host();
                    (*fetchImage)(sourceUrl, 0);
                    return;
                }

                if (!previewContentTypeLooksHtml(contentType)) {
                    embedState->status    = ::msdb::ChatEmbedStatus::Failed;
                    embedState->errorCode = "unsupported_content_type";
                    finish(*embedState);
                    return;
                }

                // Large SPA responses often keep the useful preview tags near the start of <head>.
                // Parse the prefix we have instead of discarding the preview solely due to size.
                const QByteArray htmlBytes =
                    bytes.size() > CHAT_PREVIEW_MAX_PAGE_BYTES ? bytes.left(CHAT_PREVIEW_MAX_PAGE_BYTES) : bytes;
                const QString html                       = QString::fromUtf8(htmlBytes);
                const QHash< QString, QString > metaTags = extractMetaTags(html);
                *pageTitle                               = metaTags.value(QLatin1String("og:title"),
                                            metaTags.value(QLatin1String("twitter:title"), extractHtmlTitle(html)));
                *pageDescription                         = metaTags.value(
                    QLatin1String("og:description"),
                    metaTags.value(QLatin1String("twitter:description"), metaTags.value(QLatin1String("description"))));
                *pageSiteName = metaTags.value(QLatin1String("og:site_name"), sourceUrl.host());

                embedState->title       = u8(pageTitle->isEmpty() ? sourceUrl.host() : *pageTitle);
                embedState->description = u8(*pageDescription);
                embedState->siteName    = u8(pageSiteName->isEmpty() ? sourceUrl.host() : *pageSiteName);

                const QString imageUrlString = previewImageMetaTag(metaTags);
                if (imageUrlString.isEmpty()) {
                    embedState->status    = ::msdb::ChatEmbedStatus::Ready;
                    embedState->errorCode = "";
                    finish(*embedState);
                    return;
                }

                (*fetchImage)(sourceUrl.resolved(QUrl(imageUrlString)), 0);
            });
	};

	(*fetchPage)(QUrl(QString::fromStdString(initialEmbed.canonicalUrl)), 0);
}

/// Checks whether the given channel has restrictions affecting the ENTER privilege
///
/// @param c A pointer to the Channel that should be checked
/// @return Whether the provided channel has an ACL denying ENTER
bool isChannelEnterRestricted(Channel *c) {
	// A channel is enter restricted if there's an ACL denying enter privileges
	for (ChanACL *acl : c->qlACL) {
		if (acl->pDeny & ChanACL::Enter) {
			return true;
		}
	}

	return false;
}

void Server::msgAuthenticate(ServerUser *uSource, MumbleProto::Authenticate &msg) {
	ZoneScoped;

	if (uSource->sState == ServerUser::Authenticated && (msg.tokens_size() > 0 || !uSource->qslAccessTokens.empty())) {
		// Process a change in access tokens for already authenticated users
		QStringList qsl;

		for (int i = 0; i < msg.tokens_size(); ++i) {
			qsl << u8(msg.tokens(i));
		}

		{
			QMutexLocker qml(&qmCache);
			uSource->qslAccessTokens = qsl;
		}

		clearACLCache(uSource);

		// Send back updated enter states of all channels
		MumbleProto::ChannelState mpcs;

		for (Channel *chan : qhChannels) {
			mpcs.set_channel_id(static_cast< unsigned int >(chan->iId));
			mpcs.set_can_enter(hasPermission(uSource, chan, ChanACL::Enter));
			// As no ACLs have changed, we don't need to update the is_access_restricted message field

			sendMessage(uSource, mpcs);
		}

		sendTextChannelSync(uSource);
	}
	MSG_SETUP(ServerUser::Connected);

	// As the first thing, assign a session ID to this client. Given that the client initiated
	// the authentication procedure we can be sure that this is not just a random TCP connection.
	// Thus it is about time we assign the ID to this client in order to be able to reference it
	// in the following.
	{
		QWriteLocker wl(&qrwlVoiceThread);
		uSource->uiSession = qqIds.dequeue();
		qhUsers.insert(uSource->uiSession, uSource);
		qhHostUsers[uSource->haAddress].insert(uSource);
	}

	Channel *root = qhChannels.value(0);
	Channel *c;

	uSource->qsName = u8(msg.username()).trimmed();

	bool ok     = false;
	bool nameok = validateUserName(uSource->qsName);
	QString pw  = u8(msg.password());

	// Fetch ID and stored username.
	// This function needs to support the fact that sessions may go away.
	int id = authenticate(uSource->qsName, pw, static_cast< int >(uSource->uiSession), uSource->qslEmail,
						  uSource->qsHash, uSource->bVerified, uSource->peerCertificateChain());

	uSource->iId = id >= 0 ? id : -1;

	QString reason;
	MumbleProto::Reject_RejectType rtType = MumbleProto::Reject_RejectType_None;

	if (id == -2 && !nameok) {
		reason = "Invalid username";
		rtType = MumbleProto::Reject_RejectType_InvalidUsername;
	} else if (id == -1) {
		reason = "Wrong certificate or password for existing user";
		rtType = MumbleProto::Reject_RejectType_WrongUserPW;
	} else if (id == -2 && !qsPassword.isEmpty() && qsPassword != pw) {
		reason = "Invalid server password";
		rtType = MumbleProto::Reject_RejectType_WrongServerPW;
	} else if (id == -3) {
		reason = "Your account information can not be verified currently. Please try again later";
		rtType = MumbleProto::Reject_RejectType_AuthenticatorFail;
	} else {
		ok = true;
	}

	ServerUser *uOld = nullptr;
	for (ServerUser *u : qhUsers) {
		if (u == uSource)
			continue;
		if (((u->iId >= 0) && (u->iId == uSource->iId)) || (u->qsName.toLower() == uSource->qsName.toLower())) {
			uOld = u;
			break;
		}
	}

	// Allow reuse of name from same IP
	if (ok && uOld && (uSource->iId == -1)) {
		if ((uOld->peerAddress() != uSource->peerAddress())
			&& (uSource->qsHash.isEmpty() || (uSource->qsHash != uOld->qsHash))) {
			reason = "Username already in use";
			rtType = MumbleProto::Reject_RejectType_UsernameInUse;
			ok     = false;
		}
	}

	if ((id != 0) && (static_cast< unsigned int >(qhUsers.count()) > iMaxUsers)) {
		reason = QString::fromLatin1("Server is full (max %1 users)").arg(iMaxUsers);
		rtType = MumbleProto::Reject_RejectType_ServerFull;
		ok     = false;
	}

	if ((id != 0) && (uSource->qsHash.isEmpty() && bCertRequired)) {
		reason = QString::fromLatin1("A certificate is required to connect to this server");
		rtType = MumbleProto::Reject_RejectType_NoCertificate;
		ok     = false;
	}

	if (ok) {
		if (msg.tokens_size() > 0) {
			// Set the access tokens for the newly connected user
			QStringList qsl;

			for (int i = 0; i < msg.tokens_size(); ++i) {
				qsl << u8(msg.tokens(i));
			}

			{
				QMutexLocker qml(&qmCache);
				uSource->qslAccessTokens = qsl;
			}
		}

		// Clear cache as the "auth" ACL depends on the user id
		// and any cached ACL won't have taken that into account yet
		// Also, if we set access tokens above, we have to reset
		// the ACL cache anyway.
		clearACLCache(uSource);
	}

	Channel *lc = nullptr;
	if (uSource->iId >= 0 && Meta::mp->bRememberChan) {
		unsigned int lastChannelID = m_dbWrapper.getLastChannelID(iServerNum, static_cast< unsigned int >(uSource->iId),
																  static_cast< unsigned int >(iRememberChanDuration),
																  tUptime.elapsed< std::chrono::seconds >());
		lc                         = qhChannels.value(lastChannelID);
	}

	if (!lc || !hasPermission(uSource, lc, ChanACL::Enter) || isChannelFull(lc, uSource)) {
		lc = qhChannels.value(iDefaultChan);
		if (!lc || !hasPermission(uSource, lc, ChanACL::Enter) || isChannelFull(lc, uSource)) {
			lc = root;
			if (isChannelFull(lc, uSource)) {
				reason = QString::fromLatin1("Server channels are full");
				rtType = MumbleProto::Reject_RejectType_ServerFull;
				ok     = false;
			}
		}
	}

	if (!ok) {
		log(uSource, QString("Rejected connection from %1: %2")
						 .arg(addressToString(uSource->peerAddress(), uSource->peerPort()), reason));
		MumbleProto::Reject mpr;
		mpr.set_reason(u8(reason));
		mpr.set_type(rtType);
		sendMessage(uSource, mpr);
		uSource->disconnectSocket();
		return;
	}

	startThread();

	// Kick ghost
	if (uOld) {
		log(uSource, "Disconnecting ghost");
		MumbleProto::UserRemove mpur;
		mpur.set_session(uOld->uiSession);
		mpur.set_reason("You connected to the server from another device");
		sendMessage(uOld, mpur);
		uOld->forceFlush();
		uOld->disconnectSocket(true);
	}

	// Setup UDP encryption
	{
		QMutexLocker l(&uSource->qmCrypt);

		uSource->csCrypt->genKey();

		MumbleProto::CryptSetup mpcrypt;
		mpcrypt.set_key(uSource->csCrypt->getRawKey());
		mpcrypt.set_server_nonce(uSource->csCrypt->getEncryptIV());
		mpcrypt.set_client_nonce(uSource->csCrypt->getDecryptIV());
		sendMessage(uSource, mpcrypt);
	}

	bool fake_celt_support = false;
	if (msg.celt_versions_size() > 0) {
		for (int i = 0; i < msg.celt_versions_size(); ++i)
			uSource->qlCodecs.append(msg.celt_versions(i));
	} else {
		uSource->qlCodecs.append(static_cast< qint32 >(0x8000000b));
		fake_celt_support = true;
	}
	uSource->bOpus = msg.opus();
	recheckCodecVersions(uSource);

	MumbleProto::CodecVersion mpcv;
	mpcv.set_alpha(iCodecAlpha);
	mpcv.set_beta(iCodecBeta);
	mpcv.set_prefer_alpha(bPreferAlpha);
	mpcv.set_opus(bOpus);
	sendMessage(uSource, mpcv);

	if (!bOpus && uSource->bOpus && fake_celt_support) {
		sendTextMessage(
			nullptr, uSource, false,
			QLatin1String("<strong>WARNING:</strong> Your client doesn't support the CELT codec, you won't be able to "
						  "talk to or hear most clients. Please make sure your client was built with CELT support."));
	}

	// Transmit channel tree
	QQueue< Channel * > q;
	QSet< Channel * > chans;
	q << root;
	MumbleProto::ChannelState mpcs;

	while (!q.isEmpty()) {
		c = q.dequeue();
		chans.insert(c);

		mpcs.Clear();

		mpcs.set_channel_id(c->iId);
		if (c->cParent)
			mpcs.set_parent(c->cParent->iId);
		if (c->iId == 0)
			mpcs.set_name(u8(qsRegName.isEmpty() ? QLatin1String("Root") : qsRegName));
		else
			mpcs.set_name(u8(c->qsName));

		mpcs.set_position(c->iPosition);

		if ((uSource->m_version >= Version::fromComponents(1, 2, 2)) && !c->qbaDescHash.isEmpty())
			mpcs.set_description_hash(blob(c->qbaDescHash));
		else if (!c->qsDesc.isEmpty())
			mpcs.set_description(u8(c->qsDesc));

		mpcs.set_max_users(c->uiMaxUsers);

		// Include info about enter restrictions of this channel
		mpcs.set_is_enter_restricted(isChannelEnterRestricted(c));
		mpcs.set_can_enter(hasPermission(uSource, c, ChanACL::Enter));

		sendMessage(uSource, mpcs);

		for (Channel *chan : c->qlChannels) {
			q.enqueue(chan);
		}
	}

	// Transmit links
	for (Channel *chan : chans) {
		if (chan->qhLinks.count() > 0) {
			mpcs.Clear();
			mpcs.set_channel_id(chan->iId);

			for (Channel *l : chan->qhLinks.keys()) {
				mpcs.add_links(l->iId);
			}
			sendMessage(uSource, mpcs);
		}
	}

	if (uSource->iId >= 0) {
		m_dbWrapper.loadChannelListenersOf(iServerNum, *uSource, m_channelListenerManager);
	}

	// Transmit user profile
	MumbleProto::UserState mpus;

	userEnterChannel(uSource, lc, mpus);

	{
		QWriteLocker wl(&qrwlVoiceThread);
		uSource->sState = ServerUser::Authenticated;
	}

	mpus.set_session(uSource->uiSession);
	mpus.set_name(u8(uSource->qsName));
	if (uSource->iId >= 0) {
		mpus.set_user_id(static_cast< unsigned int >(uSource->iId));

		loadTexture(*uSource);

		if (!uSource->qbaTextureHash.isEmpty()) {
			mpus.set_texture_hash(blob(uSource->qbaTextureHash));
		} else if (!uSource->qbaTexture.isEmpty()) {
			mpus.set_texture(blob(uSource->qbaTexture));
		}

		loadComment(*uSource);

		if (!uSource->qbaCommentHash.isEmpty()) {
			mpus.set_comment_hash(blob(uSource->qbaCommentHash));
		} else if (!uSource->qsComment.isEmpty()) {
			mpus.set_comment(u8(uSource->qsComment));
		}
	}
	if (!uSource->qsHash.isEmpty())
		mpus.set_hash(u8(uSource->qsHash));

	mpus.set_channel_id(uSource->cChannel->iId);

	sendAll(mpus, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);

	if ((uSource->qbaTexture.length() >= 4)
		&& (qFromBigEndian< unsigned int >(reinterpret_cast< const unsigned char * >(uSource->qbaTexture.constData()))
			== 600 * 60 * 4))
		mpus.set_texture(blob(uSource->qbaTexture));
	if (!uSource->qsComment.isEmpty())
		mpus.set_comment(u8(uSource->qsComment));
	sendAll(mpus, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);

	// Transmit other users profiles
	for (ServerUser *u : qhUsers) {
		if (u->sState != ServerUser::Authenticated)
			continue;

		if (u == uSource)
			continue;

		mpus.Clear();
		mpus.set_session(u->uiSession);
		mpus.set_name(u8(u->qsName));
		if (u->iId >= 0)
			mpus.set_user_id(static_cast< unsigned int >(u->iId));
		if (uSource->m_version >= Version::fromComponents(1, 2, 2)) {
			if (!u->qbaTextureHash.isEmpty())
				mpus.set_texture_hash(blob(u->qbaTextureHash));
			else if (!u->qbaTexture.isEmpty())
				mpus.set_texture(blob(u->qbaTexture));
		} else if ((uSource->qbaTexture.length() >= 4)
				   && (qFromBigEndian< unsigned int >(
						   reinterpret_cast< const unsigned char * >(uSource->qbaTexture.constData()))
					   == 600 * 60 * 4)) {
			mpus.set_texture(blob(u->qbaTexture));
		}
		if (u->cChannel->iId != 0)
			mpus.set_channel_id(u->cChannel->iId);
		if (u->bDeaf)
			mpus.set_deaf(true);
		else if (u->bMute)
			mpus.set_mute(true);
		if (u->bSuppress)
			mpus.set_suppress(true);
		if (u->bPrioritySpeaker)
			mpus.set_priority_speaker(true);
		if (u->bRecording)
			mpus.set_recording(true);
		if (u->bSelfDeaf)
			mpus.set_self_deaf(true);
		else if (u->bSelfMute)
			mpus.set_self_mute(true);
		if ((uSource->m_version >= Version::fromComponents(1, 2, 2)) && !u->qbaCommentHash.isEmpty())
			mpus.set_comment_hash(blob(u->qbaCommentHash));
		else if (!u->qsComment.isEmpty())
			mpus.set_comment(u8(u->qsComment));
		if (!u->qsHash.isEmpty())
			mpus.set_hash(u8(u->qsHash));


		for (unsigned int channelID : m_channelListenerManager.getListenedChannelsForUser(u->uiSession)) {
			mpus.add_listening_channel_add(channelID);

			if (broadcastListenerVolumeAdjustments) {
				VolumeAdjustment volume = m_channelListenerManager.getListenerVolumeAdjustment(u->uiSession, channelID);
				MumbleProto::UserState::VolumeAdjustment *adjustment = mpus.add_listening_volume_adjustment();
				adjustment->set_listening_channel(channelID);
				adjustment->set_volume_adjustment(volume.factor);
			}
		}

		sendMessage(uSource, mpus);
	}

	// Send synchronisation packet
	MumbleProto::ServerSync mpss;
	mpss.set_session(uSource->uiSession);
	if (!qsWelcomeText.isEmpty())
		mpss.set_welcome_text(u8(qsWelcomeText));
	mpss.set_max_bandwidth(static_cast< unsigned int >(iMaxBandwidth));

	if (uSource->iId == 0) {
		mpss.set_permissions(ChanACL::All);
	} else {
		QMutexLocker qml(&qmCache);
		ChanACL::hasPermission(uSource, root, ChanACL::Enter, &acCache);
		mpss.set_permissions(acCache.value(uSource)->value(root));
	}

	sendMessage(uSource, mpss);

	// Transmit user's listeners - this has to be done AFTER the server-sync message has been sent to uSource as the
	// client may require its own session ID for processing the listeners properly.
	mpus.Clear();
	mpus.set_session(uSource->uiSession);
	for (unsigned int channelID : m_channelListenerManager.getListenedChannelsForUser(uSource->uiSession)) {
		mpus.add_listening_channel_add(channelID);
	}

	// If we are not intending to broadcast the volume adjustments to everyone, we have to send the message to all but
	// uSource without the volume adjustments. Then append the adjustments, but only send them to uSource. If we are in
	// fact broadcasting, just append the adjustments and send to everyone.
	if (!broadcastListenerVolumeAdjustments && mpus.listening_channel_add_size() > 0) {
		sendExcept(uSource, mpus, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);
	}

	std::unordered_map< unsigned int, VolumeAdjustment > volumeAdjustments =
		m_channelListenerManager.getAllListenerVolumeAdjustments(uSource->uiSession);
	for (auto it = volumeAdjustments.begin(); it != volumeAdjustments.end(); ++it) {
		MumbleProto::UserState::VolumeAdjustment *adjustment = mpus.add_listening_volume_adjustment();
		adjustment->set_listening_channel(it->first);
		adjustment->set_volume_adjustment(it->second.factor);
	}

	if (mpus.listening_channel_add_size() > 0 || mpus.listening_volume_adjustment_size() > 0) {
		if (!broadcastListenerVolumeAdjustments) {
			if (uSource->m_version >= Version::fromComponents(1, 2, 2)) {
				sendMessage(uSource, mpus);
			}
		} else {
			sendAll(mpus, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);
		}
	}

	MumbleProto::ServerConfig mpsc;
	mpsc.set_allow_html(bAllowHTML);
	mpsc.set_message_length(static_cast< unsigned int >(iMaxTextMessageLength));
	mpsc.set_image_message_length(static_cast< unsigned int >(iMaxImageMessageLength));
	mpsc.set_max_users(static_cast< unsigned int >(iMaxUsers));
	mpsc.set_recording_allowed(allowRecording);
	mpsc.set_persistent_global_chat_enabled(bPersistentGlobalChatEnabled);
	mpsc.set_screen_share_enabled(bScreenShareEnabled);
	mpsc.set_screen_share_recording_enabled(bScreenShareRecordingEnabled);
	mpsc.set_screen_share_helper_required(bScreenShareHelperRequired);
	for (const int codec : qlPreferredScreenShareCodecs) {
		mpsc.add_preferred_screen_share_codecs(static_cast< MumbleProto::ScreenShareCodec >(codec));
	}
	mpsc.set_screen_share_max_width(uiScreenShareMaxWidth);
	mpsc.set_screen_share_max_height(uiScreenShareMaxHeight);
	mpsc.set_screen_share_max_fps(uiScreenShareMaxFps);
	if (!qsScreenShareRelayUrl.isEmpty()) {
		mpsc.set_screen_share_relay_url(u8(qsScreenShareRelayUrl));
	}
	sendMessage(uSource, mpsc);
	syncScreenShareStateForUser(uSource);

	sendTextChannelSync(uSource);

	MumbleProto::SuggestConfig mpsug;
	if (m_suggestVersion != Version::UNKNOWN) {
		MumbleProto::setSuggestedVersion(mpsug, m_suggestVersion);
	}
	if (m_suggestPositional)
		mpsug.set_positional(m_suggestPositional.value());
	if (m_suggestPushToTalk)
		mpsug.set_push_to_talk(m_suggestPushToTalk.value());
#if GOOGLE_PROTOBUF_VERSION >= 3004000
	if (mpsug.ByteSizeLong() > 0) {
#else
	// ByteSize() has been deprecated as of protobuf v3.4
	if (mpsug.ByteSize() > 0) {
#endif
		sendMessage(uSource, mpsug);
	}

	if (uSource->m_version < Version::fromComponents(1, 4, 0) && Meta::mp->iMaxListenersPerChannel != 0
		&& Meta::mp->iMaxListenerProxiesPerUser != 0) {
		// The server has the ChannelListener feature enabled but the client that connects doesn't have version 1.4.0 or
		// newer meaning that this client doesn't know what ChannelListeners are. Thus we'll send that user a
		// text-message informing about this.
		MumbleProto::TextMessage mptm;

		if (Meta::mp->bAllowHTML) {
			mptm.set_message("<b>[WARNING]</b>: This server has the <b>ChannelListener</b> feature enabled but your "
							 "client version does not support it. "
							 "This means that users <b>might be listening to what you are saying in your channel "
							 "without you noticing!</b> "
							 "You can solve this issue by upgrading to Mumble 1.4.0 or newer.");
		} else {
			mptm.set_message(
				"[WARNING]: This server has the ChannelListener feature enabled but your client version does not "
				"support it. "
				"This means that users might be listening to what you are saying in your channel without you noticing! "
				"You can solve this issue by upgrading to Mumble 1.4.0 or newer.");
		}

		sendMessage(uSource, mptm);
	}

	switch (msg.client_type()) {
		case static_cast< int >(ClientType::BOT):
			uSource->m_clientType = ClientType::BOT;
			m_botCount++;
			break;
		case static_cast< int >(ClientType::REGULAR):
			// No-op (also applies to unknown values of msg.client_type())
			// (The default client type is regular anyway, so we don't need to change anything here)
			break;
	}

	log(uSource, "Authenticated");

	emit userConnected(uSource);
}

void Server::msgBanList(ServerUser *uSource, MumbleProto::BanList &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	if (!hasPermission(uSource, qhChannels.value(0), ChanACL::Ban)) {
		PERM_DENIED(uSource, qhChannels.value(0), ChanACL::Ban);
		return;
	}

	std::set< Ban > previousBans;

	if (msg.query()) {
		msg.clear_query();
		msg.clear_bans();
		for (const Ban &b : m_bans) {
			MumbleProto::BanList_BanEntry *be = msg.add_bans();
			be->set_address(b.haAddress.toStdString());
			be->set_mask(static_cast< unsigned int >(b.iMask));
			be->set_name(u8(b.qsUsername));
			be->set_hash(u8(b.qsHash));
			be->set_reason(u8(b.qsReason));
			be->set_start(u8(b.qdtStart.toString(Qt::ISODate)));
			be->set_duration(b.iDuration);
		}
		sendMessage(uSource, msg);
	} else {
		previousBans = std::set< Ban >(m_bans.begin(), m_bans.end());
		std::set< QString > uniqueBans;

		m_bans.clear();
		for (int i = 0; i < msg.bans_size(); ++i) {
			const MumbleProto::BanList_BanEntry &be = msg.bans(i);

			Ban b;
			b.haAddress  = be.address();
			b.iMask      = static_cast< int >(be.mask());
			b.qsUsername = u8(be.name());
			b.qsHash     = u8(be.hash());
			b.qsReason   = u8(be.reason());
			if (be.has_start()) {
				b.qdtStart = QDateTime::fromString(u8(be.start()), Qt::ISODate);
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
				b.qdtStart.setTimeZone(QTimeZone::UTC);
#else
				b.qdtStart.setTimeSpec(Qt::UTC);
#endif
			} else {
				b.qdtStart = QDateTime::currentDateTime().toUTC();
			}

			b.iDuration = be.duration();

			QString repr = b.toKey();

			// server-side de-duplication
			if (uniqueBans.contains(repr)) {
				continue;
			}

			if (b.isValid()) {
				m_bans.push_back(std::move(b));
				uniqueBans.insert(repr);
			}
		}
		// m_bans needs to be sorted in order for it to be used in the set_difference functions below
		std::sort(m_bans.begin(), m_bans.end());

		std::vector< Ban > removed, added;
		std::set_difference(previousBans.begin(), previousBans.end(), m_bans.begin(), m_bans.end(),
							std::back_inserter(removed));
		std::set_difference(m_bans.begin(), m_bans.end(), previousBans.begin(), previousBans.end(),
							std::back_inserter(added));

		for (const Ban &b : removed) {
			log(uSource, QString("Removed ban: %1").arg(b.toString()));
		}

		for (const Ban &b : added) {
			log(uSource, QString("New ban: %1").arg(b.toString()));
		}

		m_dbWrapper.saveBans(iServerNum, m_bans);
		log(uSource, "Updated banlist");
	}
}

void Server::msgReject(ServerUser *, MumbleProto::Reject &) {
}

void Server::msgServerSync(ServerUser *, MumbleProto::ServerSync &) {
}

void Server::msgPermissionDenied(ServerUser *, MumbleProto::PermissionDenied &) {
}

void Server::msgUDPTunnel(ServerUser *, MumbleProto::UDPTunnel &) {
	// This code should be unreachable
	assert(false);
	qWarning("Messages: Reached theoretically unreachable function msgUDPTunnel");
}

void Server::msgUserState(ServerUser *uSource, MumbleProto::UserState &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	VICTIM_SETUP;

	Channel *root = qhChannels.value(0);

	/*
		First check all permissions involved
	*/
	if ((pDstServerUser->iId == 0) && (uSource->iId != 0)) {
		// Don't allow any action on the SuperUser except initiated directly by the SuperUser himself/herself
		PERM_DENIED_TYPE(SuperUser);
		return;
	}

	msg.set_session(pDstServerUser->uiSession);
	msg.set_actor(uSource->uiSession);

	if (msg.has_name()) {
		PERM_DENIED_TYPE(UserName);
		return;
	}

	if (uSource == pDstServerUser) {
		RATELIMIT(uSource);
	}

	// Handle potential temporary access tokens
	QStringList temporaryAccessTokens;
	for (int i = 0; i < msg.temporary_access_tokens_size(); i++) {
		temporaryAccessTokens << u8(msg.temporary_access_tokens(i));
	}
	TemporaryAccessTokenHelper tempTokenHelper(uSource, temporaryAccessTokens, this);

	if (msg.has_channel_id()) {
		Channel *c = qhChannels.value(msg.channel_id());
		if (!c || (c == pDstServerUser->cChannel))
			return;

		if ((uSource != pDstServerUser) && (!hasPermission(uSource, pDstServerUser->cChannel, ChanACL::Move))) {
			PERM_DENIED(uSource, pDstServerUser->cChannel, ChanACL::Move);
			return;
		}

		if (!hasPermission(uSource, c, ChanACL::Move) && !hasPermission(pDstServerUser, c, ChanACL::Enter)) {
			PERM_DENIED(pDstServerUser, c, ChanACL::Enter);
			return;
		}
		if (isChannelFull(c, uSource)) {
			PERM_DENIED_FALLBACK(ChannelFull, Version::fromComponents(1, 2, 1), QLatin1String("Channel is full"));
			return;
		}
	}

	QList< Channel * > listeningChannelsAdd;
	int passedChannelListener = 0;
	// Check permission for each channel
	for (int i = 0; i < msg.listening_channel_add_size(); i++) {
		Channel *c = qhChannels.value(msg.listening_channel_add(i));

		if (!c) {
			continue;
		}

		if (!hasPermission(pDstServerUser, c, ChanACL::Listen)) {
			PERM_DENIED(pDstServerUser, c, ChanACL::Listen);
			continue;
		}

		if (Meta::mp->iMaxListenersPerChannel >= 0
			&& Meta::mp->iMaxListenersPerChannel - m_channelListenerManager.getListenerCountForChannel(c->iId) - 1
				   < 0) {
			// A limit for the amount of listener proxies per channel is set and it has been reached already
			PERM_DENIED_FALLBACK(ChannelListenerLimit, Version::fromComponents(1, 4, 0),
								 QLatin1String("No more listeners allowed in this channel"));
			continue;
		}

		if (Meta::mp->iMaxListenerProxiesPerUser >= 0
			&& Meta::mp->iMaxListenerProxiesPerUser
					   - m_channelListenerManager.getListenedChannelCountForUser(uSource->uiSession)
					   - passedChannelListener - 1
				   < 0) {
			// A limit for the amount of listener proxies per user is set and it has been reached already
			PERM_DENIED_FALLBACK(UserListenerLimit, Version::fromComponents(1, 4, 0),
								 QLatin1String("No more listeners allowed in this channel"));
			continue;
		}

		passedChannelListener++;

		listeningChannelsAdd << c;
	}

	if (msg.has_mute() || msg.has_deaf() || msg.has_suppress() || msg.has_priority_speaker()) {
		if (pDstServerUser->iId == 0) {
			PERM_DENIED_TYPE(SuperUser);
			return;
		}
		if (!hasPermission(uSource, pDstServerUser->cChannel, ChanACL::MuteDeafen) || msg.suppress()) {
			PERM_DENIED(uSource, pDstServerUser->cChannel, ChanACL::MuteDeafen);
			return;
		}
	}

	if (msg.has_mute() || msg.has_deaf() || msg.has_suppress()) {
		if (pDstServerUser->cChannel->bTemporary) {
			// If the destination user is inside a temporary channel,
			// the source user needs to have the MuteDeafen ACL in the first
			// non-temporary parent channel.

			Channel *c = pDstServerUser->cChannel;
			while (c && c->bTemporary) {
				c = c->cParent;
			}

			if (!c || !hasPermission(uSource, c, ChanACL::MuteDeafen)) {
				PERM_DENIED_TYPE(TemporaryChannel);
				return;
			}
		}
	}

	QString comment;

	if (msg.has_comment()) {
		bool changed = false;
		comment      = u8(msg.comment());
		if (uSource != pDstServerUser) {
			if (!hasPermission(uSource, root, ChanACL::ResetUserContent)) {
				PERM_DENIED(uSource, root, ChanACL::ResetUserContent);
				return;
			}
			if (comment.length() > 0) {
				PERM_DENIED_TYPE(TextTooLong);
				return;
			}
		}


		if (!isTextAllowed(comment, changed)) {
			PERM_DENIED_TYPE(TextTooLong);
			return;
		}
		if (changed)
			msg.set_comment(u8(comment));
	}

	if (msg.has_texture()) {
		if (iMaxImageMessageLength > 0
			&& (msg.texture().length() > static_cast< unsigned int >(iMaxImageMessageLength))) {
			PERM_DENIED_TYPE(TextTooLong);
			return;
		}
		if (uSource != pDstServerUser) {
			if (!hasPermission(uSource, root, ChanACL::ResetUserContent)) {
				PERM_DENIED(uSource, root, ChanACL::ResetUserContent);
				return;
			}
			if (msg.texture().length() > 0) {
				PERM_DENIED_TYPE(TextTooLong);
				return;
			}
		}
	}


	if (msg.has_user_id()) {
		ChanACL::Perm p = (uSource == pDstServerUser) ? ChanACL::SelfRegister : ChanACL::Register;
		if ((pDstServerUser->iId >= 0) || !hasPermission(uSource, root, p)) {
			PERM_DENIED(uSource, root, p);
			return;
		}
		if (pDstServerUser->qsHash.isEmpty()) {
			PERM_DENIED_HASH(pDstServerUser);
			return;
		}
	}

	// Prevent self-targeting state changes from being applied to others
	if ((pDstServerUser != uSource)
		&& (msg.has_self_deaf() || msg.has_self_mute() || msg.has_plugin_context() || msg.has_plugin_identity()
			|| msg.has_recording() || msg.listening_channel_add_size() > 0
			|| msg.listening_channel_remove_size() > 0)) {
		return;
	}

	/*
		-------------------- Permission checks done. Now act --------------------
	*/
	bool bBroadcast = false;

	if (msg.has_texture()) {
		QByteArray qba = blob(msg.texture());
		if (pDstServerUser->iId >= 0) {
			// For registered users store the texture we just received in the database
			if (!setTexture(*pDstServerUser, qba)) {
				return;
			}
		} else {
			// For unregistered users or SuperUser only get the hash
			hashAssign(pDstServerUser->qbaTexture, pDstServerUser->qbaTextureHash, qba);
		}

		// The texture will be sent out later in this function
		bBroadcast = true;
	}

	// Writing to bSelfMute, bSelfDeaf and ssContext
	// requires holding a write lock on qrwlVoiceThread.
	{
		QWriteLocker wl(&qrwlVoiceThread);

		if (msg.has_self_deaf()) {
			pDstServerUser->bSelfDeaf = msg.self_deaf();
			if (pDstServerUser->bSelfDeaf)
				msg.set_self_mute(true);
			bBroadcast = true;
		}

		if (msg.has_self_mute()) {
			pDstServerUser->bSelfMute = msg.self_mute();
			if (!pDstServerUser->bSelfMute) {
				msg.set_self_deaf(false);
				pDstServerUser->bSelfDeaf = false;
			}
			bBroadcast = true;
		}

		if (msg.has_plugin_context()) {
			pDstServerUser->ssContext = msg.plugin_context();

			// Make sure to clear this from the packet so we don't broadcast it
			msg.clear_plugin_context();
		}
	}

	if (msg.has_plugin_identity()) {
		pDstServerUser->qsIdentity = u8(msg.plugin_identity());
		// Make sure to clear this from the packet so we don't broadcast it
		msg.clear_plugin_identity();
	}

	if (!comment.isNull()) {
		setComment(*uSource, comment);

		bBroadcast = true;
	}



	if (msg.has_mute() || msg.has_deaf() || msg.has_suppress() || msg.has_priority_speaker()) {
		// Writing to bDeaf, bMute and bSuppress requires
		// holding a write lock on qrwlVoiceThread.
		QWriteLocker wl(&qrwlVoiceThread);

		if (msg.has_deaf()) {
			pDstServerUser->bDeaf = msg.deaf();
			if (pDstServerUser->bDeaf)
				msg.set_mute(true);
		}
		if (msg.has_mute()) {
			pDstServerUser->bMute = msg.mute();
			if (!pDstServerUser->bMute) {
				msg.set_deaf(false);
				pDstServerUser->bDeaf = false;
			}
		}
		if (msg.has_suppress())
			pDstServerUser->bSuppress = msg.suppress();

		if (msg.has_priority_speaker())
			pDstServerUser->bPrioritySpeaker = msg.priority_speaker();

		log(uSource, QString("Changed speak-state of %1 (%2 %3 %4 %5)")
						 .arg(QString(*pDstServerUser), QString::number(pDstServerUser->bMute),
							  QString::number(pDstServerUser->bDeaf), QString::number(pDstServerUser->bSuppress),
							  QString::number(pDstServerUser->bPrioritySpeaker)));

		bBroadcast = true;
	}

	if (msg.has_recording() && (pDstServerUser->bRecording != msg.recording())) {
		assert(uSource == pDstServerUser);

		pDstServerUser->bRecording = msg.recording();

		MumbleProto::TextMessage mptm;
		mptm.add_tree_id(0);
		if (pDstServerUser->bRecording) {
			if (!allowRecording) {
				// User tried to start recording even though this server forbids it
				// -> Kick user
				MumbleProto::UserRemove mpur;
				mpur.set_session(uSource->uiSession);
				mpur.set_reason("Recording is not allowed on this server");
				sendMessage(uSource, mpur);
				uSource->forceFlush();
				uSource->disconnectSocket(true);

				// We just kicked this user, so there is no point in further processing his/her message
				return;
			} else {
				mptm.set_message(u8(QString(QLatin1String("User '%1' started recording")).arg(pDstServerUser->qsName)));
			}
		} else {
			mptm.set_message(u8(QString(QLatin1String("User '%1' stopped recording")).arg(pDstServerUser->qsName)));
		}

		sendAll(mptm, Version::fromComponents(1, 2, 3), Version::CompareMode::LessThan);

		bBroadcast = true;
	}

	if (msg.has_channel_id()) {
		Channel *c = qhChannels.value(msg.channel_id());

		userEnterChannel(pDstServerUser, c, msg);
		log(uSource, QString("Moved %1 to %2").arg(QString(*pDstServerUser), QString(*c)));
		bBroadcast = true;
	}

	// Handle channel listening
	// Note that it is important to handle the listening channels after channel-joins
	std::set< unsigned int > additionalVolumeAdjustedChannels;
	for (Channel *c : listeningChannelsAdd) {
		addChannelListener(*pDstServerUser, *c);

		log(QString::fromLatin1("\"%1\" is now listening to channel \"%2\"")
				.arg(QString(*pDstServerUser))
				.arg(QString(*c)));

		float volumeFactor =
			m_channelListenerManager.getListenerVolumeAdjustment(pDstServerUser->uiSession, c->iId).factor;

		if (volumeFactor != 1.0f) {
			additionalVolumeAdjustedChannels.insert(c->iId);
		}
	}
	for (int i = 0; i < msg.listening_volume_adjustment_size(); i++) {
		const MumbleProto::UserState::VolumeAdjustment &adjustment = msg.listening_volume_adjustment(i);

		const Channel *channel = qhChannels.value(adjustment.listening_channel());

		if (channel) {
			setChannelListenerVolume(*pDstServerUser, *channel, adjustment.volume_adjustment());

			// If the message contains a new volume adjustment for this channel anyway, we don't have to
			// add this adjustment again
			additionalVolumeAdjustedChannels.erase(channel->iId);
		} else {
			log(uSource, QString::fromLatin1("Invalid channel ID \"%1\" in volume adjustment")
							 .arg(adjustment.listening_channel()));
		}
	}
	for (int i = 0; i < msg.listening_channel_remove_size(); i++) {
		Channel *c = qhChannels.value(msg.listening_channel_remove(i));

		if (c) {
			disableChannelListener(*pDstServerUser, *c);

			log(QString::fromLatin1("\"%1\" is no longer listening to \"%2\"")
					.arg(QString(*pDstServerUser))
					.arg(QString(*c)));

			// If the channel is no longer listened to anyway, we don't need to broadcast its volume adjustment
			additionalVolumeAdjustedChannels.erase(c->iId);
		}
	}
	// For the channels that are listened to and for which no explicit volume adjustment is part of the message yet,
	// but which had a volume adjustment != 1 (restored from the DB), we ensure that this adjustment is broadcast
	// as if it was part of the message all along.
	for (unsigned int channelID : additionalVolumeAdjustedChannels) {
		const Channel *channel = qhChannels.value(channelID);

		if (channel) {
			const float factor =
				m_channelListenerManager.getListenerVolumeAdjustment(pDstServerUser->uiSession, channelID).factor;

			MumbleProto::UserState::VolumeAdjustment *adjustment = msg.add_listening_volume_adjustment();
			adjustment->set_listening_channel(channel->iId);
			adjustment->set_volume_adjustment(factor);
		}
	}

	bool listenerVolumeChanged = msg.listening_volume_adjustment_size() > 0;
	bool listenerChanged       = !listeningChannelsAdd.isEmpty() || msg.listening_channel_remove_size() > 0;

	bool broadcastingBecauseOfVolumeChange = !bBroadcast && listenerVolumeChanged;
	bBroadcast                             = bBroadcast || listenerChanged || listenerVolumeChanged;

	if (listenerChanged || listenerVolumeChanged) {
		// As whisper targets also contain information about ChannelListeners and
		// their associated volume adjustment, we have to clear the target cache
		clearWhisperTargetCache();
	}


	bool bDstAclChanged = false;
	if (msg.has_user_id()) {
		// Handle user (Self-)Registration
		if (registerUser(*pDstServerUser)) {
			assert(pDstServerUser->iId >= 0);
			msg.set_user_id(static_cast< unsigned int >(pDstServerUser->iId));
			bDstAclChanged = true;
		} else {
			// Registration failed
			msg.clear_user_id();
		}

		bBroadcast = true;
	}

	if (bBroadcast) {
		// Texture handling for clients < 1.2.2.
		// Send the texture data in the message.
		if (msg.has_texture() && (pDstServerUser->qbaTexture.length() >= 4)
			&& (qFromBigEndian< unsigned int >(
					reinterpret_cast< const unsigned char * >(pDstServerUser->qbaTexture.constData()))
				!= 600 * 60 * 4)) {
			// This is a new style texture, don't send it because the client doesn't handle it correctly / crashes.
			msg.clear_texture();
			sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);
			msg.set_texture(blob(pDstServerUser->qbaTexture));
		} else {
			// This is an old style texture, empty texture or there was no texture in this packet,
			// send the message unchanged.
			sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);
		}

		// Texture / comment handling for clients >= 1.2.2.
		// Send only a hash of the texture / comment text. The client will request the actual data if necessary.
		if (msg.has_texture() && !pDstServerUser->qbaTextureHash.isEmpty()) {
			msg.clear_texture();
			msg.set_texture_hash(blob(pDstServerUser->qbaTextureHash));
		}
		if (msg.has_comment() && !pDstServerUser->qbaCommentHash.isEmpty()) {
			msg.clear_comment();
			msg.set_comment_hash(blob(pDstServerUser->qbaCommentHash));
		}

		if (uSource->m_version >= Version::fromComponents(1, 2, 2)) {
			sendMessage(uSource, msg);
		}
		if (!broadcastListenerVolumeAdjustments) {
			// Don't broadcast the volume adjustments to everyone
			msg.clear_listening_volume_adjustment();
		}

		if (broadcastListenerVolumeAdjustments || !broadcastingBecauseOfVolumeChange) {
			sendExcept(uSource, msg, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);
		}

		if (bDstAclChanged) {
			clearACLCache(pDstServerUser);
		} else if (listenerChanged || listenerVolumeChanged) {
			// We only have to do this if the ACLs didn't change as
			// clearACLCache calls clearWhisperTargetChache anyways
			clearWhisperTargetCache();
		}
	}

	emit userStateChanged(pDstServerUser);
}

void Server::msgUserRemove(ServerUser *uSource, MumbleProto::UserRemove &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	VICTIM_SETUP;

	msg.set_actor(uSource->uiSession);

	bool ban = msg.has_ban() && msg.ban();

	Channel *c                   = qhChannels.value(0);
	QFlags< ChanACL::Perm > perm = ban ? ChanACL::Ban : (ChanACL::Ban | ChanACL::Kick);

	if ((pDstServerUser->iId == 0) || !hasPermission(uSource, c, perm)) {
		PERM_DENIED(uSource, c, perm);
		return;
	}

	if (ban) {
		// Before Mumble 1.6, a ban meant certificate and IP ban. This is the fallback.
		// Starting with Mumble 1.6, an admin must specify which method to use.
		bool banCertificate = !msg.has_ban_certificate() || msg.ban_certificate();
		bool banIP          = !msg.has_ban_ip() || msg.ban_ip();

		// User might not even have a certificate
		banCertificate &= !pDstServerUser->qsHash.isEmpty();

		if (!banIP && !banCertificate) {
			// No ban method specified
			return;
		}

		Ban b;
		if (banIP) {
			b.haAddress = pDstServerUser->haAddress;
			b.iMask     = 128;
		}
		if (banCertificate) {
			b.qsHash = pDstServerUser->qsHash;
		}
		b.qsReason   = u8(msg.reason());
		b.qsUsername = pDstServerUser->qsName;
		b.qdtStart   = QDateTime::currentDateTime().toUTC();
		b.iDuration  = 0;

		m_bans.push_back(std::move(b));
		m_dbWrapper.saveBans(iServerNum, m_bans);
	}

	sendAll(msg);
	if (ban)
		log(uSource, QString("Kickbanned %1 (%2)").arg(QString(*pDstServerUser), u8(msg.reason())));
	else
		log(uSource, QString("Kicked %1 (%2)").arg(QString(*pDstServerUser), u8(msg.reason())));
	pDstServerUser->disconnectSocket();
}

void Server::msgChannelState(ServerUser *uSource, MumbleProto::ChannelState &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	Channel *c = nullptr;
	Channel *p = nullptr;

	// If this message relates to an existing channel check if the id is really valid
	if (msg.has_channel_id()) {
		c = qhChannels.value(msg.channel_id());
		if (!c)
			return;
	} else {
		RATELIMIT(uSource);
	}

	// Check if the parent exists
	if (msg.has_parent()) {
		p = qhChannels.value(msg.parent());
		if (!p)
			return;
	}

	msg.clear_links();

	QString qsName;
	QString qsDesc;
	if (msg.has_description()) {
		qsDesc       = u8(msg.description());
		bool changed = false;
		if (!isTextAllowed(qsDesc, changed)) {
			PERM_DENIED_TYPE(TextTooLong);
			return;
		}
		if (changed)
			msg.set_description(u8(qsDesc));
	}

	if (msg.has_name()) {
		// If we are sent a channel name this means we want to create this channel so
		// check if the name is valid and not already in use.
		qsName = u8(msg.name());

		if (!validateChannelName(qsName)) {
			PERM_DENIED_TYPE(ChannelName);
			return;
		}

		if (qsName.length() == 0) {
			PERM_DENIED_TYPE(ChannelName);
			return;
		}

		if (p || (c && c->iId != 0)) {
			Channel *cp = p ? p : c->cParent;
			for (Channel *sibling : cp->qlChannels) {
				if (sibling->qsName == qsName) {
					PERM_DENIED_TYPE(ChannelName);
					return;
				}
			}
		}
	}

	if (p) {
		// Having a parent channel given means we either want to create
		// a channel in or move a channel into this parent.

		if (!canNest(p, c)) {
			PERM_DENIED_FALLBACK(NestingLimit, Version::fromComponents(1, 2, 4),
								 QLatin1String("Channel nesting limit reached"));
			return;
		}
	}

	if (!c) {
		// If we don't have a channel handle up to now we want to create a new channel
		// so check if the user has enough rights and we got everything we need.
		if (!p || qsName.isNull())
			return;

		if (iChannelCountLimit != 0 && qhChannels.count() >= iChannelCountLimit) {
			PERM_DENIED_FALLBACK(ChannelCountLimit, Version::fromComponents(1, 3, 0),
								 QLatin1String("Channel count limit reached"));
			return;
		}

		ChanACL::Perm perm = msg.temporary() ? ChanACL::MakeTempChannel : ChanACL::MakeChannel;
		if (!hasPermission(uSource, p, perm)) {
			PERM_DENIED(uSource, p, perm);
			return;
		}

		if ((uSource->iId < 0) && uSource->qsHash.isEmpty()) {
			PERM_DENIED_HASH(uSource);
			return;
		}

		if (p->bTemporary) {
			PERM_DENIED_TYPE(TemporaryChannel);
			return;
		}

		c = createNewChannel(p, qsName, msg.temporary(), msg.position(), msg.max_users());
		hashAssign(c->qsDesc, c->qbaDescHash, qsDesc);

		if (uSource->iId >= 0) {
			Group *g = new Group(c, "admin");
			g->qsAdd << uSource->iId;
		}

		if (!hasPermission(uSource, c, ChanACL::Write)) {
			ChanACL *a    = new ChanACL(c);
			a->bApplyHere = true;
			a->bApplySubs = false;
			if (uSource->iId >= 0)
				a->iUserId = uSource->iId;
			else
				a->qsGroup = QLatin1Char('$') + uSource->qsHash;
			a->pDeny  = ChanACL::None;
			a->pAllow = ChanACL::Write | ChanACL::Traverse;

			clearACLCache();
		}

		if (!c->bTemporary) {
			m_dbWrapper.updateChannelData(iServerNum, *c);
		}

		msg.set_channel_id(c->iId);
		log(uSource, QString("Added channel %1 under %2").arg(QString(*c), QString(*p)));
		emit channelCreated(c);

		sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);
		if (!c->qbaDescHash.isEmpty()) {
			msg.clear_description();
			msg.set_description_hash(blob(c->qbaDescHash));
		}
		sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);

		if (c->bTemporary) {
			// If a temporary channel has been created move the creator right in there
			MumbleProto::UserState mpus;
			mpus.set_session(uSource->uiSession);
			mpus.set_channel_id(c->iId);
			userEnterChannel(uSource, c, mpus);
			sendAll(mpus);
			emit userStateChanged(uSource);
		}
	} else {
		// The message is related to an existing channel c so check if the user is allowed to modify it
		// and perform the modifications
		if (!qsName.isNull()) {
			if (!hasPermission(uSource, c, ChanACL::Write) || (c->iId == 0)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}
		if (!qsDesc.isNull()) {
			if (!hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}
		if (msg.has_position()) {
			if (!hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}
		if (p) {
			// If we received a parent channel check if it differs from the old one and is not
			// Temporary. If that is the case check if the user has enough rights and if the
			// channel name is not used in the target location. Abort otherwise.
			if (p == c->cParent)
				return;

			Channel *ip = p;
			while (ip) {
				if (ip == c)
					return;
				ip = ip->cParent;
			}

			if (p->bTemporary) {
				PERM_DENIED_TYPE(TemporaryChannel);
				return;
			}

			if (!hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}

			QFlags< ChanACL::Perm > parentMakePermission =
				c->bTemporary ? ChanACL::MakeTempChannel : ChanACL::MakeChannel;
			if (!hasPermission(uSource, p, parentMakePermission)) {
				PERM_DENIED(uSource, p, parentMakePermission);
				return;
			}

			QString name = qsName.isNull() ? c->qsName : qsName;

			for (Channel *sibling : p->qlChannels) {
				if (sibling->qsName == name) {
					PERM_DENIED_TYPE(ChannelName);
					return;
				}
			}
		}
		QList< Channel * > qlAdd;
		QList< Channel * > qlRemove;

		if (msg.links_add_size() || msg.links_remove_size()) {
			if (!hasPermission(uSource, c, ChanACL::LinkChannel)) {
				PERM_DENIED(uSource, c, ChanACL::LinkChannel);
				return;
			}
			if (msg.links_remove_size()) {
				for (int i = 0; i < msg.links_remove_size(); ++i) {
					unsigned int link = msg.links_remove(i);
					Channel *l        = qhChannels.value(link);
					if (!l) {
						return;
					}
					qlRemove << l;
				}
			}
			if (msg.links_add_size()) {
				for (int i = 0; i < msg.links_add_size(); ++i) {
					unsigned int link = msg.links_add(i);
					Channel *l        = qhChannels.value(link);
					if (!l) {
						return;
					}
					if (!hasPermission(uSource, l, ChanACL::LinkChannel)) {
						PERM_DENIED(uSource, l, ChanACL::LinkChannel);
						return;
					}
					qlAdd << l;
				}
			}
		}

		if (msg.has_max_users()) {
			if (!hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}

		// All permission checks done -- the update is good.

		if (p) {
			log(uSource, QString("Moved channel %1 from %2 to %3").arg(QString(*c), QString(*c->cParent), QString(*p)));

			{
				QWriteLocker wl(&qrwlVoiceThread);
				c->cParent->removeChannel(c);
				p->addChannel(c);
			}
		}
		if (!qsName.isNull()) {
			log(uSource, QString("Renamed channel %1 to %2").arg(QString(*c), QString(qsName)));
			c->qsName = qsName;
		}
		if (!qsDesc.isNull())
			hashAssign(c->qsDesc, c->qbaDescHash, qsDesc);

		if (msg.has_position())
			c->iPosition = msg.position();

		for (Channel *l : qlAdd) {
			linkChannels(*c, *l);
		}

		for (Channel *l : qlRemove) {
			unlinkChannels(*c, *l);
		}

		if (msg.has_max_users())
			c->uiMaxUsers = msg.max_users();

		if (!c->bTemporary) {
			m_dbWrapper.updateChannelData(iServerNum, *c);
		}
		emit channelStateChanged(c);

		sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);
		if (msg.has_description() && !c->qbaDescHash.isEmpty()) {
			msg.clear_description();
			msg.set_description_hash(blob(c->qbaDescHash));
		}
		sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);
	}
}

void Server::msgChannelRemove(ServerUser *uSource, MumbleProto::ChannelRemove &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	if (!hasPermission(uSource, c, ChanACL::Write) || (c->iId == 0)) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	log(uSource, QString("Removed channel %1").arg(*c));

	removeChannel(c);
}

void Server::msgTextMessage(ServerUser *uSource, MumbleProto::TextMessage &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	// For signal userTextMessage (RPC consumers)
	TextMessage tm;

	// List of users to route the message to
	QSet< ServerUser * > users;
	// List of channels used if dest is a tree of channels
	QQueue< Channel * > q;

	RATELIMIT(uSource);

	int res = 0;
	emit textMessageFilterSig(res, uSource, msg);
	switch (res) {
		// Accept
		case 0:
			// No-op.
			break;
		// Reject
		case 1:
			PERM_DENIED(uSource, uSource->cChannel, ChanACL::TextMessage);
			return;
		// Drop
		case 2:
			return;
	}

	QString text = u8(msg.message());
	bool changed = false;

	if (!isTextAllowed(text, changed)) {
		PERM_DENIED_TYPE(TextTooLong);
		return;
	}
	if (text.isEmpty()) {
		return;
	}
	if (changed) {
		msg.set_message(u8(text));
	}

	tm.qsText = text;

	{ // Happy easter
		char m[29] = { 0117, 0160, 0145, 0156, 040,  0164, 0150, 0145, 040, 0160, 0157, 0144, 040, 0142, 0141,
					   0171, 040,  0144, 0157, 0157, 0162, 0163, 054,  040, 0110, 0101, 0114, 056, 0 };
		if (msg.channel_id_size() == 1 && msg.channel_id(0) == 0 && msg.message() == m) {
			PERM_DENIED_TYPE(H9K);
			return;
		}
	}

	msg.set_actor(uSource->uiSession);

	// Send the message to all users that are in (= have joined) OR are
	// "listening" to channels to which the message has been directed to
	for (int i = 0; i < msg.channel_id_size(); ++i) {
		unsigned int id = msg.channel_id(i);

		Channel *c = qhChannels.value(id);
		if (!c) {
			return;
		}

		if (!ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, &acCache)) {
			PERM_DENIED(uSource, c, ChanACL::TextMessage);
			return;
		}

		// Users directly in that channel
		for (User *p : c->qlUsers) {
			users.insert(static_cast< ServerUser * >(p));
		}

		// Users only listening in that channel
		for (unsigned int session : m_channelListenerManager.getListenersForChannel(c->iId)) {
			ServerUser *currentUser = qhUsers.value(session);
			if (currentUser) {
				users.insert(currentUser);
			}
		}

		tm.qlChannels.append(id);
	}

	// If the message is sent to trees of channels, find all affected channels
	// and append them to q
	for (int i = 0; i < msg.tree_id_size(); ++i) {
		unsigned int id = msg.tree_id(i);

		Channel *c = qhChannels.value(id);
		if (!c) {
			return;
		}

		if (!ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, &acCache)) {
			PERM_DENIED(uSource, c, ChanACL::TextMessage);
			return;
		}

		q.enqueue(c);

		tm.qlTrees.append(id);
	}

	// Go through all channels in q and append all users in those channels
	// to the list of recipients
	// Sub-channels are enqued so they are also checked by a later loop-iteration
	while (!q.isEmpty()) {
		Channel *c = q.dequeue();
		if (ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, &acCache)) {
			for (Channel *sub : c->qlChannels) {
				q.enqueue(sub);
			}
			// Users directly in that channel
			for (User *p : c->qlUsers) {
				users.insert(static_cast< ServerUser * >(p));
			}
			// Users only listening in that channel
			for (unsigned int session : m_channelListenerManager.getListenersForChannel(c->iId)) {
				ServerUser *currentUser = qhUsers.value(session);
				if (currentUser) {
					users.insert(currentUser);
				}
			}
		}
	}

	// Go through all users the message is sent to directly
	for (int i = 0; i < msg.session_size(); ++i) {
		unsigned int session = msg.session(i);
		ServerUser *u        = qhUsers.value(session);
		if (u) {
			if (!ChanACL::hasPermission(uSource, u->cChannel, ChanACL::TextMessage, &acCache)) {
				PERM_DENIED(uSource, u->cChannel, ChanACL::TextMessage);
				return;
			}
			users.insert(u);
		}

		tm.qlSessions.append(session);
	}

	// Remove the message sender from the list of users to send the message to
	users.remove(uSource);

	if (msg.channel_id_size() == 1 && msg.tree_id_size() == 0 && msg.session_size() == 0) {
		Channel *channel = qhChannels.value(msg.channel_id(0));
		if (channel) {
			persistAndBroadcastChatMessage(uSource, text, ::msdb::ChatMessageBodyFormat::PlainText,
										   MumbleProto::Channel, channel->iId, channel,
										   ::msdb::ChatThreadScope::Channel, {}, std::nullopt, users);
			emit userTextMessage(uSource, tm);
			return;
		}
	}

	// Actually send the original message to the affected users
	for (ServerUser *u : users) {
		sendMessage(u, msg);
	}

	// Emit the signal for RPC consumers
	emit userTextMessage(uSource, tm);
}

void Server::msgChatSend(ServerUser *uSource, MumbleProto::ChatSend &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	RATELIMIT(uSource);

	if (!clientSupportsPersistentChat(uSource)) {
		inferPersistentChatSupport(uSource);
	}

	QString bodyText = msg.has_body_text() ? u8(msg.body_text()) : u8(msg.message());
	bool bodyChanged = false;
	const msdb::ChatMessageBodyFormat bodyFormat =
		dbBodyFormatFromProto(msg.has_body_format() ? msg.body_format() : MumbleProto::ChatBodyFormatPlainText);
	if (!isTextAllowed(bodyText, bodyChanged)) {
		PERM_DENIED_TYPE(TextTooLong);
		return;
	}
	if (bodyText.isEmpty()) {
		return;
	}
	const std::optional< unsigned int > replyToMessageID =
		msg.has_reply_to_message_id() && msg.reply_to_message_id() > 0
			? std::optional< unsigned int >(msg.reply_to_message_id())
			: std::nullopt;

	MumbleProto::ChatScope scope = msg.has_scope() ? msg.scope() : MumbleProto::Channel;
	unsigned int scopeID =
		msg.has_scope_id() ? msg.scope_id() : (uSource->cChannel ? uSource->cChannel->iId : Mumble::ROOT_CHANNEL_ID);
	Channel *permissionChannel      = nullptr;
	::msdb::ChatThreadScope dbScope = ::msdb::ChatThreadScope::Channel;

	switch (scope) {
		case MumbleProto::Channel:
			permissionChannel = qhChannels.value(scopeID);
			dbScope           = ::msdb::ChatThreadScope::Channel;
			break;
		case MumbleProto::ServerGlobal:
			scopeID           = 0;
			permissionChannel = qhChannels.value(Mumble::ROOT_CHANNEL_ID);
			dbScope           = ::msdb::ChatThreadScope::ServerGlobal;
			break;
		case MumbleProto::Aggregate:
			return;
		case MumbleProto::TextChannel: {
			std::optional<::msdb::DBTextChannel > textChannel = m_dbWrapper.getTextChannel(iServerNum, scopeID);
			if (!textChannel) {
				return;
			}

			permissionChannel = qhChannels.value(textChannel->aclChannelID);
			dbScope           = ::msdb::ChatThreadScope::TextChannel;
			break;
		}
		default:
			return;
	}

	if (scope == MumbleProto::ServerGlobal && !bPersistentGlobalChatEnabled) {
		MumbleProto::PermissionDenied denied;
		denied.set_permission(static_cast< unsigned int >(ChanACL::TextMessage));
		denied.set_channel_id(Mumble::ROOT_CHANNEL_ID);
		denied.set_session(uSource->uiSession);
		denied.set_type(MumbleProto::PermissionDenied_DenyType_Permission);
		denied.set_reason(u8(QStringLiteral("Global chat is disabled by this server.")));
		sendMessage(uSource, denied);
		return;
	}

	if (!permissionChannel) {
		return;
	}

	if (!ChanACL::hasPermission(uSource, permissionChannel, ChanACL::TextMessage, &acCache)) {
		PERM_DENIED(uSource, permissionChannel, ChanACL::TextMessage);
		return;
	}

	const std::string scopeKey = chatScopeKey(scope, scopeID);
	if (scopeKey.empty()) {
		return;
	}

	const std::optional<::msdb::DBChatThread > existingThread =
		m_dbWrapper.getChatThreadByScope(iServerNum, dbScope, scopeKey);
	if (replyToMessageID) {
		if (!existingThread) {
			sendPersistentChatTextDenied(this, uSource, tr("The selected reply target is unavailable."));
			return;
		}

		const std::optional<::msdb::DBChatMessage > replyTarget =
			m_dbWrapper.getChatMessage(iServerNum, replyToMessageID.value());
		if (!replyTarget) {
			sendPersistentChatTextDenied(this, uSource, tr("The selected reply target no longer exists."));
			return;
		}
		if (replyTarget->threadID != existingThread->threadID) {
			sendPersistentChatTextDenied(this, uSource, tr("Replies have to stay in the same conversation."));
			return;
		}
	}

	QSet< ServerUser * > legacyFallbackRecipients;
	if (scope == MumbleProto::Channel) {
		legacyFallbackRecipients = legacyChannelRecipients(qhUsers, m_channelListenerManager, permissionChannel);
		legacyFallbackRecipients.remove(uSource);
	}

	std::vector< msdb::DBChatMessageAttachment > attachments;
	if (msg.attachment_asset_ids_size() > 0
		&& static_cast< unsigned int >(msg.attachment_asset_ids_size()) > uiChatAttachmentLimit) {
		MumbleProto::PermissionDenied denied;
		denied.set_session(uSource->uiSession);
		denied.set_type(MumbleProto::PermissionDenied_DenyType_Text);
		denied.set_reason(u8(QStringLiteral("Too many attachments for one message.")));
		sendMessage(uSource, denied);
		return;
	}

	attachments.reserve(static_cast< std::size_t >(msg.attachment_asset_ids_size()));
	for (int i = 0; i < msg.attachment_asset_ids_size(); ++i) {
		const unsigned int assetID = msg.attachment_asset_ids(i);
		if (assetID == 0 || !m_dbWrapper.chatAssetExists(iServerNum, assetID)) {
			continue;
		}

		const msdb::DBChatAsset asset = m_dbWrapper.getChatAsset(iServerNum, assetID);
		const bool ownedBySender      = (asset.ownerSession && asset.ownerSession.value() == uSource->uiSession)
								   || (asset.ownerUserID && uSource->iId >= 0
									   && asset.ownerUserID.value() == static_cast< unsigned int >(uSource->iId));
		if (!ownedBySender && !canAccessChatAsset(uSource, assetID)) {
			continue;
		}

		msdb::DBChatMessageAttachment attachment(iServerNum, 0, assetID);
		attachment.displayOrder   = static_cast< unsigned int >(attachments.size());
		attachment.filename       = std::string();
		attachment.mime           = asset.mime;
		attachment.byteSize       = asset.byteSize;
		attachment.kind           = asset.kind;
		attachment.width          = asset.width;
		attachment.height         = asset.height;
		attachment.durationMs     = asset.durationMs;
		attachment.inlineSafe     = isInlineSafeAsset(asset.kind, QString::fromStdString(asset.mime), true);
		attachment.previewAssetID = asset.previewAssetID;
		attachments.push_back(std::move(attachment));
	}

	persistAndBroadcastChatMessage(uSource, bodyText, bodyFormat, scope, scopeID, permissionChannel, dbScope,
								   attachments, replyToMessageID, legacyFallbackRecipients);
}

void Server::msgChatMessage(ServerUser *, MumbleProto::ChatMessage &) {
}

void Server::msgChatMessageDelete(ServerUser *uSource, MumbleProto::ChatMessageDelete &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	RATELIMIT(uSource);

	if (!clientSupportsPersistentChat(uSource)) {
		inferPersistentChatSupport(uSource);
	}

	if (!msg.has_message_id() || msg.message_id() == 0) {
		return;
	}

	std::optional<::msdb::DBChatMessage > message = m_dbWrapper.getChatMessage(iServerNum, msg.message_id());
	if (!message) {
		sendPersistentChatTextDenied(this, uSource, tr("That message is no longer available."));
		return;
	}

	if (msg.has_thread_id() && msg.thread_id() != message->threadID) {
		sendPersistentChatTextDenied(this, uSource, tr("That delete target belongs to a different conversation."));
		return;
	}

	if (message->deletedAt > std::chrono::system_clock::time_point()) {
		return;
	}

	const ::msdb::DBChatThread thread = m_dbWrapper.getChatThread(iServerNum, message->threadID);
	QHash< unsigned int, ::msdb::DBTextChannel > textChannelsByID;
	for (const ::msdb::DBTextChannel &currentTextChannel : m_dbWrapper.getTextChannels(iServerNum)) {
		textChannelsByID.insert(currentTextChannel.textChannelID, currentTextChannel);
	}

	MumbleProto::ChatScope scope = MumbleProto::Channel;
	unsigned int scopeID         = 0;
	Channel *permissionChannel   = nullptr;
	if (!resolveStoredChatThread(thread, qhChannels, textChannelsByID, scope, scopeID, permissionChannel)
		|| !permissionChannel) {
		sendPersistentChatTextDenied(this, uSource, tr("That message is no longer available."));
		return;
	}

	if ((msg.has_scope() && msg.scope() != scope) || (msg.has_scope_id() && msg.scope_id() != scopeID)) {
		sendPersistentChatTextDenied(this, uSource, tr("That delete target belongs to a different conversation."));
		return;
	}

	if (scope == MumbleProto::ServerGlobal && !bPersistentGlobalChatEnabled) {
		sendPersistentChatTextDenied(this, uSource, tr("Global chat is disabled by this server."));
		return;
	}

	if (!ChanACL::hasPermission(uSource, permissionChannel, ChanACL::DeleteTextMessage, &acCache)) {
		PERM_DENIED(uSource, permissionChannel, ChanACL::DeleteTextMessage);
		return;
	}

	std::optional<::msdb::DBChatMessage > deletedMessage =
		m_dbWrapper.deleteChatMessage(iServerNum, message->messageID);
	if (!deletedMessage) {
		return;
	}

	const auto resolvedAuthorName = [this](const ::msdb::DBChatMessage &storedMessage) -> std::optional< std::string > {
		if (storedMessage.authorName && !storedMessage.authorName->empty()) {
			return storedMessage.authorName;
		}

		if (storedMessage.authorSession) {
			ServerUser *currentUser = qhUsers.value(storedMessage.authorSession.value());
			if (currentUser && !currentUser->qsName.isEmpty()) {
				return u8(currentUser->qsName);
			}
		}

		if (storedMessage.authorUserID
			&& m_dbWrapper.registeredUserExists(iServerNum, storedMessage.authorUserID.value())) {
			return m_dbWrapper.getUserName(iServerNum, storedMessage.authorUserID.value());
		}

		return std::nullopt;
	};

	const MumbleProto::ChatMessage protoMessage =
		protoChatMessageFromDB(*deletedMessage, scope, scopeID, resolvedAuthorName(*deletedMessage), std::nullopt);

	QSet< ServerUser * > persistentRecipients;
	if (scope == MumbleProto::Channel) {
		persistentRecipients = legacyChannelRecipients(qhUsers, m_channelListenerManager, permissionChannel);
		persistentRecipients.insert(uSource);
	} else {
		persistentRecipients = recipientsWithChatHistoryAccess(qhUsers, permissionChannel, acCache);
	}

	for (ServerUser *currentUser : persistentRecipients) {
		if (clientSupportsPersistentChat(currentUser)) {
			sendMessage(currentUser, protoMessage);
		}
	}
}

void Server::msgChatHistoryRequest(ServerUser *uSource, MumbleProto::ChatHistoryRequest &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	RATELIMIT(uSource);

	if (!clientSupportsPersistentChat(uSource)) {
		inferPersistentChatSupport(uSource);
	}

	MumbleProto::ChatScope scope = msg.has_scope() ? msg.scope() : MumbleProto::Channel;
	unsigned int scopeID =
		msg.has_scope_id() ? msg.scope_id() : (uSource->cChannel ? uSource->cChannel->iId : Mumble::ROOT_CHANNEL_ID);
	Channel *permissionChannel      = nullptr;
	::msdb::ChatThreadScope dbScope = ::msdb::ChatThreadScope::Channel;

	switch (scope) {
		case MumbleProto::Channel:
			permissionChannel = qhChannels.value(scopeID);
			dbScope           = ::msdb::ChatThreadScope::Channel;
			break;
		case MumbleProto::ServerGlobal:
			scopeID           = 0;
			permissionChannel = qhChannels.value(Mumble::ROOT_CHANNEL_ID);
			dbScope           = ::msdb::ChatThreadScope::ServerGlobal;
			break;
		case MumbleProto::Aggregate:
			break;
		case MumbleProto::TextChannel: {
			std::optional<::msdb::DBTextChannel > textChannel = m_dbWrapper.getTextChannel(iServerNum, scopeID);
			if (!textChannel) {
				return;
			}

			permissionChannel = qhChannels.value(textChannel->aclChannelID);
			dbScope           = ::msdb::ChatThreadScope::TextChannel;
			break;
		}
		default:
			return;
	}

	const unsigned int limit                            = msg.has_limit() ? std::clamp(msg.limit(), 1U, 100U) : 50U;
	const unsigned int startOffset                      = msg.has_start_offset() ? msg.start_offset() : 0;
	const std::optional< unsigned int > beforeMessageID = msg.has_before_message_id() && msg.before_message_id() > 0
															  ? std::optional< unsigned int >(msg.before_message_id())
															  : std::nullopt;

	MumbleProto::ChatHistoryResponse response;
	response.set_scope(scope);
	response.set_scope_id(scopeID);
	response.set_start_offset(startOffset);

	const auto resolvedAuthorName = [this](const ::msdb::DBChatMessage &message) -> std::optional< std::string > {
		if (message.authorName && !message.authorName->empty()) {
			return message.authorName;
		}

		if (message.authorSession) {
			ServerUser *currentUser = qhUsers.value(message.authorSession.value());
			if (currentUser && !currentUser->qsName.isEmpty()) {
				return u8(currentUser->qsName);
			}
		}

		if (message.authorUserID && m_dbWrapper.registeredUserExists(iServerNum, message.authorUserID.value())) {
			return m_dbWrapper.getUserName(iServerNum, message.authorUserID.value());
		}

		return std::nullopt;
	};
	response.set_has_more(false);
	response.set_has_older(false);

	if (scope == MumbleProto::ServerGlobal && !bPersistentGlobalChatEnabled) {
		MumbleProto::PermissionDenied denied;
		denied.set_permission(static_cast< unsigned int >(ChanACL::ViewTextMessageHistory));
		denied.set_channel_id(Mumble::ROOT_CHANNEL_ID);
		denied.set_session(uSource->uiSession);
		denied.set_type(MumbleProto::PermissionDenied_DenyType_Permission);
		denied.set_reason(u8(QStringLiteral("Global chat is disabled by this server.")));
		sendMessage(uSource, denied);
		return;
	}

	if (scope == MumbleProto::Aggregate) {
		QHash< unsigned int, ::msdb::DBTextChannel > textChannelsByID;
		for (const ::msdb::DBTextChannel &currentTextChannel : m_dbWrapper.getTextChannels(iServerNum)) {
			textChannelsByID.insert(currentTextChannel.textChannelID, currentTextChannel);
		}

		std::vector< AggregateChatEntry > entries;
		for (const ::msdb::DBChatThread &currentThread : m_dbWrapper.getChatThreads(iServerNum)) {
			MumbleProto::ChatScope messageScope = MumbleProto::Channel;
			unsigned int messageScopeID         = 0;
			Channel *messagePermissionChannel   = nullptr;

			if (!resolveStoredChatThread(currentThread, qhChannels, textChannelsByID, messageScope, messageScopeID,
										 messagePermissionChannel)) {
				continue;
			}

			if (messageScope == MumbleProto::ServerGlobal && !bPersistentGlobalChatEnabled) {
				continue;
			}

			if (!ChanACL::hasPermission(uSource, messagePermissionChannel, ChanACL::ViewTextMessageHistory, &acCache)) {
				continue;
			}

			for (const ::msdb::DBChatMessage &currentMessage :
				 m_dbWrapper.getChatMessages(iServerNum, currentThread.threadID)) {
				AggregateChatEntry entry;
				entry.message = currentMessage;
				entry.scope   = messageScope;
				entry.scopeID = messageScopeID;
				entries.push_back(std::move(entry));
			}
		}

		std::sort(entries.begin(), entries.end(), newerAggregateEntry);

		const std::size_t offset = startOffset;
		const std::size_t pageEnd =
			std::min< std::size_t >(entries.size(), static_cast< std::size_t >(startOffset) + limit);
		response.set_has_more(entries.size() > static_cast< std::size_t >(startOffset) + limit);

		if (offset < pageEnd) {
			std::vector< AggregateChatEntry > page(entries.begin() + static_cast< std::ptrdiff_t >(offset),
												   entries.begin() + static_cast< std::ptrdiff_t >(pageEnd));
			std::reverse(page.begin(), page.end());

			for (const AggregateChatEntry &entry : page) {
				const std::optional< ChatReplyPreview > replyPreview =
					resolveReplyPreview(m_dbWrapper, iServerNum, entry.message, resolvedAuthorName);
				*response.add_messages() =
					protoChatMessageFromDB(entry.message, entry.scope, entry.scopeID, resolvedAuthorName(entry.message),
										   persistedUserID(uSource), replyPreview);
			}
		}

		sendMessage(uSource, response);
		return;
	}

	if (!permissionChannel) {
		return;
	}

	if (!ChanACL::hasPermission(uSource, permissionChannel, ChanACL::ViewTextMessageHistory, &acCache)) {
		PERM_DENIED(uSource, permissionChannel, ChanACL::ViewTextMessageHistory);
		return;
	}

	const std::string scopeKey                  = chatScopeKey(scope, scopeID);
	std::optional<::msdb::DBChatThread > thread = m_dbWrapper.getChatThreadByScope(iServerNum, dbScope, scopeKey);
	if (!thread) {
		sendMessage(uSource, response);
		return;
	}

	response.set_thread_id(thread->threadID);

	std::vector<::msdb::DBChatMessage > messages =
		beforeMessageID
			? m_dbWrapper.getChatMessagesBefore(iServerNum, thread->threadID, beforeMessageID.value(), limit + 1)
			: m_dbWrapper.getChatMessages(iServerNum, thread->threadID, startOffset, static_cast< int >(limit + 1));
	if (messages.size() > limit) {
		messages.erase(messages.begin());
		response.set_has_more(true);
		response.set_has_older(true);
	}

	if (!messages.empty()) {
		response.set_oldest_message_id(messages.front().messageID);
	}

	for (const ::msdb::DBChatMessage &currentMessage : messages) {
		const std::optional< ChatReplyPreview > replyPreview =
			resolveReplyPreview(m_dbWrapper, iServerNum, currentMessage, resolvedAuthorName);
		*response.add_messages() = protoChatMessageFromDB(
			currentMessage, scope, scopeID, resolvedAuthorName(currentMessage), persistedUserID(uSource), replyPreview);
	}

	const std::optional< unsigned int > userID = persistedUserID(uSource);
	if (userID) {
		std::optional<::msdb::DBChatReadState > readState =
			m_dbWrapper.getChatReadState(iServerNum, thread->threadID, userID.value());
		if (readState) {
			response.set_last_read_message_id(readState->lastReadMessageID);
		}
	}

	sendMessage(uSource, response);
}

void Server::msgChatHistoryResponse(ServerUser *, MumbleProto::ChatHistoryResponse &) {
}

void Server::msgChatReadStateUpdate(ServerUser *uSource, MumbleProto::ChatReadStateUpdate &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	RATELIMIT(uSource);

	if (!clientSupportsPersistentChat(uSource)) {
		inferPersistentChatSupport(uSource);
	}

	const std::optional< unsigned int > userID = persistedUserID(uSource);
	if (!userID) {
		return;
	}

	MumbleProto::ChatScope scope = msg.has_scope() ? msg.scope() : MumbleProto::Channel;
	unsigned int scopeID =
		msg.has_scope_id() ? msg.scope_id() : (uSource->cChannel ? uSource->cChannel->iId : Mumble::ROOT_CHANNEL_ID);
	Channel *permissionChannel      = nullptr;
	::msdb::ChatThreadScope dbScope = ::msdb::ChatThreadScope::Channel;

	switch (scope) {
		case MumbleProto::Channel:
			permissionChannel = qhChannels.value(scopeID);
			dbScope           = ::msdb::ChatThreadScope::Channel;
			break;
		case MumbleProto::ServerGlobal:
			scopeID           = 0;
			permissionChannel = qhChannels.value(Mumble::ROOT_CHANNEL_ID);
			dbScope           = ::msdb::ChatThreadScope::ServerGlobal;
			break;
		case MumbleProto::Aggregate:
			return;
		case MumbleProto::TextChannel: {
			std::optional<::msdb::DBTextChannel > textChannel = m_dbWrapper.getTextChannel(iServerNum, scopeID);
			if (!textChannel) {
				return;
			}

			permissionChannel = qhChannels.value(textChannel->aclChannelID);
			dbScope           = ::msdb::ChatThreadScope::TextChannel;
			break;
		}
		default:
			return;
	}

	if (scope == MumbleProto::ServerGlobal && !bPersistentGlobalChatEnabled) {
		MumbleProto::PermissionDenied denied;
		denied.set_permission(static_cast< unsigned int >(ChanACL::ViewTextMessageHistory));
		denied.set_channel_id(Mumble::ROOT_CHANNEL_ID);
		denied.set_session(uSource->uiSession);
		denied.set_type(MumbleProto::PermissionDenied_DenyType_Permission);
		denied.set_reason(u8(QStringLiteral("Global chat is disabled by this server.")));
		sendMessage(uSource, denied);
		return;
	}

	if (!permissionChannel) {
		return;
	}

	if (!ChanACL::hasPermission(uSource, permissionChannel, ChanACL::ViewTextMessageHistory, &acCache)) {
		PERM_DENIED(uSource, permissionChannel, ChanACL::ViewTextMessageHistory);
		return;
	}

	const std::string scopeKey                  = chatScopeKey(scope, scopeID);
	std::optional<::msdb::DBChatThread > thread = m_dbWrapper.getChatThreadByScope(iServerNum, dbScope, scopeKey);
	if (!thread) {
		return;
	}

	::msdb::DBChatReadState readState(iServerNum, thread->threadID, userID.value());
	readState.lastReadMessageID = msg.has_last_read_message_id() ? msg.last_read_message_id() : 0;
	readState.updatedAt         = std::chrono::system_clock::now();

	m_dbWrapper.setChatReadState(readState);

	std::optional<::msdb::DBChatReadState > persistedReadState =
		m_dbWrapper.getChatReadState(iServerNum, thread->threadID, userID.value());
	if (persistedReadState) {
		sendMessage(uSource, protoReadStateFromDB(*persistedReadState, scope, scopeID));
	}
}

void Server::msgChatAssetUploadInit(ServerUser *uSource, MumbleProto::ChatAssetUploadInit &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	const QString mime = normalizedMime(msg.has_mime() ? u8(msg.mime()) : QString());
	if (mime.isEmpty() || !msg.has_byte_size() || msg.byte_size() == 0) {
		sendChatAssetState(this, uSource, 0, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Missing asset metadata."));
		return;
	}
	if (uiChatAssetMaxBytes > 0 && msg.byte_size() > uiChatAssetMaxBytes) {
		sendChatAssetState(this, uSource, 0, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Asset exceeds the configured size limit."));
		return;
	}
	if (uiChatAssetTotalQuotaBytes > 0
		&& chatAssetStoredBytes() + static_cast< quint64 >(msg.byte_size()) > uiChatAssetTotalQuotaBytes) {
		sendChatAssetState(this, uSource, 0, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Server chat asset quota exceeded."));
		return;
	}

	msdb::ChatAssetKind kind = dbAssetKindFromProto(msg.has_kind() ? msg.kind() : MumbleProto::ChatAssetKindUnknown);
	if (kind == msdb::ChatAssetKind::Unknown) {
		kind = inferredAssetKind(mime);
	}
	if (!isAllowedChatAssetMime(kind, mime)) {
		sendChatAssetState(this, uSource, 0, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Asset MIME type is not allowed by server policy."));
		return;
	}

	const QString declaredHash = msg.has_sha256() ? u8(msg.sha256()).trimmed().toLower() : QString();
	if (!declaredHash.isEmpty() && !isValidSha256Hex(declaredHash)) {
		sendChatAssetState(this, uSource, 0, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Invalid SHA-256 checksum."));
		return;
	}

	QString storageError;
	if (!ensureChatAssetStorageReady(&storageError)) {
		sendChatAssetState(this, uSource, 0, MumbleProto::ChatAssetTransferStateRejected, storageError);
		return;
	}

	PendingChatAssetUpload upload;
	upload.uploadID         = randomUploadID(qhPendingChatAssetUploads);
	upload.ownerSession     = uSource->uiSession;
	upload.ownerUserID      = persistedUserID(uSource);
	upload.filename         = msg.has_filename() ? u8(msg.filename()) : QString();
	upload.mime             = mime;
	upload.sha256           = declaredHash;
	upload.kind             = kind;
	upload.requestInline    = msg.has_request_inline() && msg.request_inline();
	upload.expectedByteSize = msg.byte_size();
	upload.tempFilePath =
		QDir(chatAssetIncomingRootPath())
			.absoluteFilePath(QStringLiteral("%1-%2.part").arg(upload.ownerSession).arg(upload.uploadID));

	QFile tempFile(upload.tempFilePath);
	if (tempFile.exists() && !tempFile.remove()) {
		sendChatAssetState(this, uSource, 0, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Failed to reset temporary upload state."));
		return;
	}
	if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		sendChatAssetState(this, uSource, 0, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Failed to open temporary upload file."));
		return;
	}
	tempFile.close();

	qhPendingChatAssetUploads.insert(upload.uploadID, upload);
	sendChatAssetState(this, uSource, upload.uploadID, MumbleProto::ChatAssetTransferStateAccepted, QString(),
					   upload.expectedByteSize);
}

void Server::msgChatAssetUploadChunk(ServerUser *uSource, MumbleProto::ChatAssetUploadChunk &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	if (!msg.has_upload_id() || !qhPendingChatAssetUploads.contains(msg.upload_id())) {
		return;
	}

	PendingChatAssetUpload &upload = qhPendingChatAssetUploads[msg.upload_id()];
	if (upload.ownerSession != uSource->uiSession) {
		return;
	}
	if (!msg.has_offset() || msg.offset() != upload.receivedByteSize) {
		sendChatAssetState(this, uSource, upload.uploadID, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Upload chunks must be sent sequentially."));
		QFile::remove(upload.tempFilePath);
		qhPendingChatAssetUploads.remove(upload.uploadID);
		return;
	}

	const QByteArray data = blob(msg.data());
	if (data.isEmpty()) {
		return;
	}
	if (data.size() > 256 * 1024) {
		sendChatAssetState(this, uSource, upload.uploadID, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Upload chunk exceeds the maximum allowed size."));
		QFile::remove(upload.tempFilePath);
		qhPendingChatAssetUploads.remove(upload.uploadID);
		return;
	}
	if (upload.receivedByteSize + static_cast< quint64 >(data.size()) > upload.expectedByteSize) {
		sendChatAssetState(this, uSource, upload.uploadID, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Upload exceeds declared asset size."));
		QFile::remove(upload.tempFilePath);
		qhPendingChatAssetUploads.remove(upload.uploadID);
		return;
	}

	QFile tempFile(upload.tempFilePath);
	if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
		sendChatAssetState(this, uSource, upload.uploadID, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Failed to append upload chunk to temporary file."));
		QFile::remove(upload.tempFilePath);
		qhPendingChatAssetUploads.remove(upload.uploadID);
		return;
	}
	if (static_cast< quint64 >(tempFile.size()) != upload.receivedByteSize) {
		tempFile.close();
		sendChatAssetState(this, uSource, upload.uploadID, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Temporary upload file is out of sync."));
		QFile::remove(upload.tempFilePath);
		qhPendingChatAssetUploads.remove(upload.uploadID);
		return;
	}
	if (tempFile.write(data) != data.size()) {
		tempFile.close();
		sendChatAssetState(this, uSource, upload.uploadID, MumbleProto::ChatAssetTransferStateRejected,
						   QStringLiteral("Failed to write upload chunk."));
		QFile::remove(upload.tempFilePath);
		qhPendingChatAssetUploads.remove(upload.uploadID);
		return;
	}
	tempFile.close();

	upload.receivedByteSize += static_cast< quint64 >(data.size());
	upload.finalChunkReceived = upload.finalChunkReceived || (msg.has_final_chunk() && msg.final_chunk());
}

void Server::msgChatAssetUploadCommit(ServerUser *uSource, MumbleProto::ChatAssetUploadCommit &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	if (!msg.has_upload_id() || !qhPendingChatAssetUploads.contains(msg.upload_id())) {
		return;
	}

	PendingChatAssetUpload upload = qhPendingChatAssetUploads.take(msg.upload_id());
	if (upload.ownerSession != uSource->uiSession) {
		qhPendingChatAssetUploads.insert(upload.uploadID, upload);
		return;
	}

	auto reject = [&](const QString &reason) {
		sendChatAssetState(this, uSource, upload.uploadID, MumbleProto::ChatAssetTransferStateRejected, reason);
		QFile::remove(upload.tempFilePath);
	};

	if (upload.receivedByteSize != upload.expectedByteSize) {
		reject(QStringLiteral("Upload size does not match declared asset size."));
		return;
	}
	if (!upload.finalChunkReceived) {
		reject(QStringLiteral("Upload is missing a final chunk marker."));
		return;
	}

	const QString commitFilename = msg.has_filename() ? u8(msg.filename()) : QString();
	if (!commitFilename.isEmpty()) {
		upload.filename = commitFilename;
	}

	const QString commitHash = msg.has_sha256() ? u8(msg.sha256()).trimmed().toLower() : QString();
	if (!commitHash.isEmpty()) {
		if (!isValidSha256Hex(commitHash)) {
			reject(QStringLiteral("Invalid SHA-256 checksum."));
			return;
		}
		upload.sha256 = commitHash;
	}

	QFile tempFile(upload.tempFilePath);
	if (!tempFile.open(QIODevice::ReadOnly)) {
		reject(QStringLiteral("Failed to read back temporary upload file."));
		return;
	}
	const QByteArray fileBytes = tempFile.readAll();
	tempFile.close();
	if (static_cast< quint64 >(fileBytes.size()) != upload.expectedByteSize) {
		reject(QStringLiteral("Temporary upload file size is invalid."));
		return;
	}

	const QString computedHash =
		QString::fromLatin1(QCryptographicHash::hash(fileBytes, QCryptographicHash::Sha256).toHex());
	if (!upload.sha256.isEmpty() && computedHash != upload.sha256) {
		reject(QStringLiteral("SHA-256 checksum mismatch."));
		return;
	}
	upload.sha256 = computedHash;

	QByteArray storedBytes    = fileBytes;
	QString storedMime        = upload.mime;
	unsigned int storedWidth  = 0;
	unsigned int storedHeight = 0;
	std::optional< SanitizedChatImage > previewThumbnail;
	if (upload.kind == msdb::ChatAssetKind::Image && isSanitizableImageMime(upload.mime)) {
		const auto sanitizedImage = sanitizeChatImageBytes(fileBytes, false);
		if (!sanitizedImage) {
			reject(QStringLiteral("Uploaded image could not be decoded safely."));
			return;
		}

		storedBytes      = sanitizedImage->bytes;
		storedMime       = sanitizedImage->mime;
		storedWidth      = sanitizedImage->width;
		storedHeight     = sanitizedImage->height;
		previewThumbnail = sanitizeChatImageBytes(storedBytes, true);
	}

	const QString storedHash =
		QString::fromLatin1(QCryptographicHash::hash(storedBytes, QCryptographicHash::Sha256).toHex());
	const QString storageKey = chatAssetStorageKey(0, storedHash);
	const QString objectPath = chatAssetAbsolutePath(storageKey);
	const QString objectDir  = QFileInfo(objectPath).absolutePath();
	QDir rootDir;
	if (!rootDir.mkpath(objectDir)) {
		reject(QStringLiteral("Failed to create final asset storage directory."));
		return;
	}

	if (QFile::exists(objectPath)) {
		if (!QFile::remove(upload.tempFilePath)) {
			reject(QStringLiteral("Failed to discard duplicate temporary upload file."));
			return;
		}
	} else {
		if (upload.kind == msdb::ChatAssetKind::Image && isSanitizableImageMime(upload.mime)) {
			QFile objectFile(objectPath);
			if (!objectFile.open(QIODevice::WriteOnly) || objectFile.write(storedBytes) != storedBytes.size()) {
				reject(QStringLiteral("Failed to write normalized image into permanent storage."));
				return;
			}
			objectFile.close();
			QFile::remove(upload.tempFilePath);
		} else if (!QFile::rename(upload.tempFilePath, objectPath)) {
			reject(QStringLiteral("Failed to move upload into permanent storage."));
			return;
		}
	}

	msdb::DBChatAsset storedAsset;
	storedAsset.serverID       = iServerNum;
	storedAsset.ownerUserID    = upload.ownerUserID;
	storedAsset.ownerSession   = upload.ownerSession;
	storedAsset.sha256         = u8(storedHash);
	storedAsset.storageKey     = u8(storageKey);
	storedAsset.mime           = u8(storedMime);
	storedAsset.byteSize       = static_cast< std::uint64_t >(storedBytes.size());
	storedAsset.kind           = upload.kind;
	storedAsset.width          = storedWidth;
	storedAsset.height         = storedHeight;
	storedAsset.retentionClass = msdb::ChatAssetRetentionClass::DefaultStorage;

	storedAsset = m_dbWrapper.addChatAsset(storedAsset);
	if (previewThumbnail) {
		const QString previewHash =
			QString::fromLatin1(QCryptographicHash::hash(previewThumbnail->bytes, QCryptographicHash::Sha256).toHex());
		const QString previewStorageKey = chatAssetStorageKey(0, previewHash);
		const QString previewObjectPath = chatAssetAbsolutePath(previewStorageKey);
		if (rootDir.mkpath(QFileInfo(previewObjectPath).absolutePath()) && !QFile::exists(previewObjectPath)) {
			QFile previewFile(previewObjectPath);
			if (previewFile.open(QIODevice::WriteOnly)
				&& previewFile.write(previewThumbnail->bytes) == previewThumbnail->bytes.size()) {
				previewFile.close();
			}
		}

		msdb::DBChatAsset previewAsset;
		previewAsset.serverID       = iServerNum;
		previewAsset.sha256         = u8(previewHash);
		previewAsset.storageKey     = u8(previewStorageKey);
		previewAsset.mime           = u8(previewThumbnail->mime);
		previewAsset.byteSize       = static_cast< std::uint64_t >(previewThumbnail->bytes.size());
		previewAsset.kind           = msdb::ChatAssetKind::Image;
		previewAsset.width          = previewThumbnail->width;
		previewAsset.height         = previewThumbnail->height;
		previewAsset.retentionClass = msdb::ChatAssetRetentionClass::PreviewCache;
		previewAsset                = m_dbWrapper.addChatAsset(previewAsset);
		storedAsset.previewAssetID  = previewAsset.assetID;
		m_dbWrapper.updateChatAssetPreviewAssetID(iServerNum, storedAsset.assetID, previewAsset.assetID);
	}

	sendChatAssetState(this, uSource, upload.uploadID, MumbleProto::ChatAssetTransferStateComplete, QString(),
					   storedAsset.byteSize,
					   protoAssetRefFromAsset(storedAsset, upload.filename,
											  isInlineSafeAsset(upload.kind, storedMime, upload.requestInline)));
}

void Server::msgChatAssetState(ServerUser *, MumbleProto::ChatAssetState &) {
}

void Server::msgChatAssetRequest(ServerUser *uSource, MumbleProto::ChatAssetRequest &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	if (!msg.has_asset_id() || msg.asset_id() == 0 || !m_dbWrapper.chatAssetExists(iServerNum, msg.asset_id())
		|| !canAccessChatAsset(uSource, msg.asset_id())) {
		MumbleProto::PermissionDenied denied;
		denied.set_session(uSource->uiSession);
		denied.set_type(MumbleProto::PermissionDenied_DenyType_Text);
		denied.set_reason(u8(QStringLiteral("Access to this chat asset is not permitted.")));
		sendMessage(uSource, denied);
		return;
	}

	const msdb::DBChatAsset asset = m_dbWrapper.getChatAsset(iServerNum, msg.asset_id());
	const QString assetPath       = chatAssetAbsolutePath(u8(asset.storageKey));
	QFile assetFile(assetPath);
	if (!assetFile.open(QIODevice::ReadOnly)) {
		MumbleProto::PermissionDenied denied;
		denied.set_session(uSource->uiSession);
		denied.set_type(MumbleProto::PermissionDenied_DenyType_Text);
		denied.set_reason(u8(QStringLiteral("Chat asset is unavailable on the server.")));
		sendMessage(uSource, denied);
		return;
	}

	const quint64 offset         = msg.has_offset() ? msg.offset() : 0;
	const quint32 requestedBytes = msg.has_max_bytes() ? msg.max_bytes() : 65536U;
	const quint32 maxBytes       = std::clamp(requestedBytes, 1024U, 262144U);
	if (offset > static_cast< quint64 >(assetFile.size())) {
		return;
	}

	if (!assetFile.seek(static_cast< qint64 >(offset))) {
		return;
	}

	MumbleProto::ChatAssetChunk chunk;
	chunk.set_asset_id(asset.assetID);
	chunk.set_offset(offset);
	chunk.set_data(blob(assetFile.read(static_cast< qint64 >(maxBytes))));
	chunk.set_eof(offset + static_cast< quint64 >(chunk.data().size()) >= asset.byteSize);
	chunk.set_total_size(asset.byteSize);
	chunk.set_mime(asset.mime);
	if (asset.width > 0) {
		chunk.set_width(asset.width);
	}
	if (asset.height > 0) {
		chunk.set_height(asset.height);
	}
	chunk.set_kind(protoAssetKindFromDB(asset.kind));
	sendMessage(uSource, chunk);
	m_dbWrapper.touchChatAsset(iServerNum, asset.assetID);
}

void Server::msgChatAssetChunk(ServerUser *, MumbleProto::ChatAssetChunk &) {
}

void Server::msgChatEmbedState(ServerUser *, MumbleProto::ChatEmbedState &) {
}

void Server::msgChatReactionToggle(ServerUser *uSource, MumbleProto::ChatReactionToggle &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	RATELIMIT(uSource);

	if (!clientSupportsPersistentChat(uSource)) {
		inferPersistentChatSupport(uSource);
	}

	const std::optional< unsigned int > actorUserID = persistedUserID(uSource);
	if (!actorUserID) {
		sendPersistentChatTextDenied(
			this, uSource, tr("Persisted reactions currently require a registered user identity on this server."));
		return;
	}

	if (!msg.has_message_id() || msg.message_id() == 0 || !msg.has_emoji()) {
		return;
	}

	const QString emoji = u8(msg.emoji()).trimmed();
	if (emoji.isEmpty() || emoji.size() > 32) {
		return;
	}

	MumbleProto::ChatScope scope = msg.has_scope() ? msg.scope() : MumbleProto::Channel;
	unsigned int scopeID =
		msg.has_scope_id() ? msg.scope_id() : (uSource->cChannel ? uSource->cChannel->iId : Mumble::ROOT_CHANNEL_ID);
	Channel *permissionChannel      = nullptr;
	::msdb::ChatThreadScope dbScope = ::msdb::ChatThreadScope::Channel;

	switch (scope) {
		case MumbleProto::Channel:
			permissionChannel = qhChannels.value(scopeID);
			dbScope           = ::msdb::ChatThreadScope::Channel;
			break;
		case MumbleProto::ServerGlobal:
			scopeID           = 0;
			permissionChannel = qhChannels.value(Mumble::ROOT_CHANNEL_ID);
			dbScope           = ::msdb::ChatThreadScope::ServerGlobal;
			break;
		case MumbleProto::Aggregate:
			return;
		case MumbleProto::TextChannel: {
			std::optional<::msdb::DBTextChannel > textChannel = m_dbWrapper.getTextChannel(iServerNum, scopeID);
			if (!textChannel) {
				return;
			}

			permissionChannel = qhChannels.value(textChannel->aclChannelID);
			dbScope           = ::msdb::ChatThreadScope::TextChannel;
			break;
		}
		default:
			return;
	}

	if (scope == MumbleProto::ServerGlobal && !bPersistentGlobalChatEnabled) {
		sendPersistentChatTextDenied(this, uSource, tr("Global chat is disabled by this server."));
		return;
	}

	if (!permissionChannel) {
		return;
	}

	if (!ChanACL::hasPermission(uSource, permissionChannel, ChanACL::TextMessage, &acCache)) {
		PERM_DENIED(uSource, permissionChannel, ChanACL::TextMessage);
		return;
	}

	const std::string scopeKey = chatScopeKey(scope, scopeID);
	if (scopeKey.empty()) {
		return;
	}

	const std::optional<::msdb::DBChatThread > thread = m_dbWrapper.getChatThreadByScope(iServerNum, dbScope, scopeKey);
	if (!thread) {
		sendPersistentChatTextDenied(this, uSource, tr("That message is no longer available."));
		return;
	}
	if (msg.has_thread_id() && msg.thread_id() != thread->threadID) {
		sendPersistentChatTextDenied(this, uSource, tr("That reaction target belongs to a different conversation."));
		return;
	}

	std::optional<::msdb::DBChatMessage > message = m_dbWrapper.getChatMessage(iServerNum, msg.message_id());
	if (!message || message->threadID != thread->threadID) {
		sendPersistentChatTextDenied(this, uSource, tr("That message is no longer available."));
		return;
	}
	if (message->deletedAt > std::chrono::system_clock::time_point()) {
		sendPersistentChatTextDenied(this, uSource, tr("That message is no longer available."));
		return;
	}

	const bool active = msg.has_active() ? msg.active() : true;
	m_dbWrapper.setChatMessageReactionActive(iServerNum, message->messageID, actorUserID.value(), u8(emoji), active);

	message = m_dbWrapper.getChatMessage(iServerNum, msg.message_id());
	if (!message) {
		return;
	}

	QSet< ServerUser * > persistentRecipients;
	if (scope == MumbleProto::Channel) {
		persistentRecipients = legacyChannelRecipients(qhUsers, m_channelListenerManager, permissionChannel);
		persistentRecipients.insert(uSource);
	} else {
		persistentRecipients = recipientsWithChatHistoryAccess(qhUsers, permissionChannel, acCache);
	}

	for (ServerUser *currentUser : persistentRecipients) {
		if (!clientSupportsPersistentChat(currentUser)) {
			continue;
		}

		sendMessage(currentUser, protoReactionStateForMessage(*message, scope, scopeID, persistedUserID(currentUser)));
	}
}

void Server::msgChatReactionState(ServerUser *, MumbleProto::ChatReactionState &) {
}

void Server::msgTextChannelSync(ServerUser *uSource, MumbleProto::TextChannelSync &msg) {
	MSG_SETUP(ServerUser::Authenticated);

	Channel *rootChannel = qhChannels.value(Mumble::ROOT_CHANNEL_ID);
	if (!rootChannel) {
		return;
	}

	const MumbleProto::TextChannelSync_Action action =
		msg.has_action() ? msg.action() : MumbleProto::TextChannelSync_Action_Sync;
	if (action == MumbleProto::TextChannelSync_Action_Sync) {
		sendTextChannelSync(uSource);
		return;
	}

	if (!hasPermission(uSource, rootChannel, ChanACL::Write)) {
		PERM_DENIED(uSource, rootChannel, ChanACL::Write);
		return;
	}

	auto broadcastTextChannelSync = [this]() {
		for (ServerUser *currentUser : qhUsers) {
			if (currentUser && currentUser->sState == ServerUser::Authenticated) {
				sendTextChannelSync(currentUser);
			}
		}
	};
	auto refreshStoredDefaultTextChannel = [this]() {
		std::vector<::msdb::DBTextChannel > textChannels = m_dbWrapper.getTextChannels(iServerNum);
		std::sort(textChannels.begin(), textChannels.end(), sortTextChannelsForPresentation);
		const std::optional< unsigned int > configuredDefaultTextChannel =
			configuredDefaultTextChannelID(m_dbWrapper, iServerNum);
		if (configuredDefaultTextChannel && containsTextChannelID(textChannels, *configuredDefaultTextChannel)) {
			return;
		}

		storeDefaultTextChannelID(m_dbWrapper, iServerNum, firstTextChannelID(textChannels));
	};

	if (action == MumbleProto::TextChannelSync_Action_Delete) {
		if (!msg.has_target_text_channel_id()) {
			return;
		}

		const unsigned int textChannelID = msg.target_text_channel_id();
		if (!m_dbWrapper.getTextChannel(iServerNum, textChannelID)) {
			return;
		}

		m_dbWrapper.removeTextChannel(iServerNum, textChannelID);
		refreshStoredDefaultTextChannel();
		broadcastTextChannelSync();
		return;
	}

	if (action == MumbleProto::TextChannelSync_Action_SetDefault) {
		if (!msg.has_target_text_channel_id()) {
			return;
		}

		const unsigned int textChannelID = msg.target_text_channel_id();
		if (!m_dbWrapper.getTextChannel(iServerNum, textChannelID)) {
			return;
		}

		storeDefaultTextChannelID(m_dbWrapper, iServerNum, textChannelID);
		broadcastTextChannelSync();
		return;
	}

	if (msg.channels_size() <= 0) {
		return;
	}

	const MumbleProto::TextChannelInfo &channelInfo = msg.channels(0);
	if (!channelInfo.has_name()) {
		return;
	}

	const QString name = u8(channelInfo.name()).trimmed();
	if (name.isEmpty() || !validateChannelName(name)) {
		return;
	}

	const unsigned int aclChannelID =
		channelInfo.has_acl_channel_id() ? channelInfo.acl_channel_id() : Mumble::ROOT_CHANNEL_ID;
	Channel *permissionChannel = qhChannels.value(aclChannelID);
	if (!permissionChannel) {
		return;
	}

	const QString description   = channelInfo.has_description() ? u8(channelInfo.description()) : QString();
	const unsigned int position = channelInfo.has_position() ? channelInfo.position() : 0;

	if (action == MumbleProto::TextChannelSync_Action_Create) {
		const ::msdb::DBTextChannel createdTextChannel =
			m_dbWrapper.addTextChannel(iServerNum, u8(name), u8(description), aclChannelID, position);
		const std::optional< unsigned int > configuredDefaultTextChannel =
			configuredDefaultTextChannelID(m_dbWrapper, iServerNum);
		const std::vector<::msdb::DBTextChannel > textChannels = m_dbWrapper.getTextChannels(iServerNum);
		if (!configuredDefaultTextChannel || !containsTextChannelID(textChannels, *configuredDefaultTextChannel)) {
			storeDefaultTextChannelID(m_dbWrapper, iServerNum, createdTextChannel.textChannelID);
		}
		broadcastTextChannelSync();
		return;
	}

	if (action != MumbleProto::TextChannelSync_Action_Update || !msg.has_target_text_channel_id()) {
		return;
	}

	std::optional<::msdb::DBTextChannel > existing =
		m_dbWrapper.getTextChannel(iServerNum, msg.target_text_channel_id());
	if (!existing) {
		return;
	}

	existing->name         = u8(name);
	existing->description  = u8(description);
	existing->aclChannelID = aclChannelID;
	existing->position     = position;
	m_dbWrapper.updateTextChannel(*existing);
	broadcastTextChannelSync();
}

/// Helper function to log the groups of the given channel.
///
/// @param server A pointer to the server object the provided channel lives on
/// @param c A pointer to the channel the groups should be logged for
/// @param prefix An optional QString that is being printed before the groups
void logGroups(Server *server, const Channel *c, QString prefix = QString()) {
	if (!prefix.isEmpty()) {
		server->log(prefix);
	}

	if (c->qhGroups.isEmpty()) {
		server->log(QString::fromLatin1("Channel %1 (%2) has no groups set").arg(c->qsName).arg(c->iId));
		return;
	} else {
		server->log(QString::fromLatin1("%1Listing groups specified for channel \"%2\" (%3)...")
						.arg(prefix.isEmpty() ? QLatin1String("") : QLatin1String("\t"))
						.arg(c->qsName)
						.arg(c->iId));
	}

	for (Group *currentGroup : c->qhGroups) {
		QString memberList;
		for (int m : currentGroup->members()) {
			memberList += QString::fromLatin1("\"%1\"").arg(server->getRegisteredUserName(m));
			memberList += ", ";
		}

		if (currentGroup->members().size() > 0) {
			memberList.remove(memberList.length() - 2, 2);
			server->log(QString::fromLatin1("%1Group: \"%2\" contains following users: %3")
							.arg(prefix.isEmpty() ? QLatin1String("\t") : QLatin1String("\t\t"))
							.arg(currentGroup->qsName)
							.arg(memberList));
		} else {
			server->log(QString::fromLatin1("%1Group \"%2\" doesn't contain any users")
							.arg(prefix.isEmpty() ? QLatin1String("\t") : QLatin1String("\t\t"))
							.arg(currentGroup->qsName));
		}
	}
}

/// Helper function to log the ACLs of the given channel.
///
/// @param server A pointer to the server object the provided channel lives on
/// @param c A pointer to the channel the ACLs should be logged for
/// @param prefix An optional QString that is being printed before the ACLs
void logACLs(Server *server, const Channel *c, QString prefix = QString()) {
	if (!prefix.isEmpty()) {
		server->log(prefix);
	}

	for (const ChanACL *a : c->qlACL) {
		server->log(QString::fromLatin1("%1%2")
						.arg(prefix.isEmpty() ? QLatin1String("") : QLatin1String("\t"))
						.arg(static_cast< QString >(*a)));
	}
}


void Server::msgACL(ServerUser *uSource, MumbleProto::ACL &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	// For changing channel properties (the 'Write') ACL we allow two things:
	// 1) As per regular ACL propagating mechanism, we check if the user has been
	// granted Write in the channel they try to edit
	// 2) We allow all users who have been granted 'Write' on the root channel
	// to be able to edit _all_ channels, independent of actual propagated ACLs
	// This is done to prevent users who have permission to create (temporary)
	// channels being able to "lock-out" admins by denying them 'Write' in their
	// channel effectively becoming ungovernable.
	if (!hasPermission(uSource, c, ChanACL::Write) && !hasPermission(uSource, qhChannels.value(0), ChanACL::Write)) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	RATELIMIT(uSource);

	if (msg.has_query() && msg.query()) {
		QStack< Channel * > chans;
		Channel *p;

		QSet< unsigned int > qsId;

		msg.clear_groups();
		msg.clear_acls();
		msg.clear_query();
		msg.set_inherit_acls(c->bInheritACL);

		p = c;
		while (p) {
			chans.push(p);
			if ((p == c) || p->bInheritACL)
				p = p->cParent;
			else
				p = nullptr;
		}

		while (!chans.isEmpty()) {
			p = chans.pop();
			for (ChanACL *acl : p->qlACL) {
				if ((p == c) || (acl->bApplySubs)) {
					MumbleProto::ACL_ChanACL *mpacl = msg.add_acls();

					mpacl->set_inherited(p != c);
					mpacl->set_apply_here(acl->bApplyHere);
					mpacl->set_apply_subs(acl->bApplySubs);
					if (acl->iUserId >= 0) {
						mpacl->set_user_id(static_cast< unsigned int >(acl->iUserId));
						qsId.insert(static_cast< unsigned int >(acl->iUserId));
					} else
						mpacl->set_group(u8(acl->qsGroup));
					mpacl->set_grant(acl->pAllow);
					mpacl->set_deny(acl->pDeny);
				}
			}
		}

		p                        = c->cParent;
		QSet< QString > allnames = Group::groupNames(c);
		for (const QString &name : allnames) {
			Group *g  = c->qhGroups.value(name);
			Group *pg = p ? Group::getGroup(p, name) : nullptr;

			MumbleProto::ACL_ChanGroup *group = msg.add_groups();
			group->set_name(u8(name));
			group->set_inherit(g ? g->bInherit : true);
			group->set_inheritable(g ? g->bInheritable : true);
			group->set_inherited(pg && pg->bInheritable);
			if (g) {
				for (int id : g->qsAdd) {
					qsId.insert(static_cast< unsigned int >(id));
					group->add_add(static_cast< unsigned int >(id));
				}
				for (int id : g->qsRemove) {
					qsId.insert(static_cast< unsigned int >(id));
					group->add_remove(static_cast< unsigned int >(id));
				}
			}
			if (pg) {
				for (int id : pg->members()) {
					qsId.insert(static_cast< unsigned int >(id));
					group->add_inherited_members(static_cast< unsigned int >(id));
				}
			}
		}

		sendMessage(uSource, msg);

		MumbleProto::QueryUsers mpqu;
		for (unsigned int id : qsId) {
			QString uname = getRegisteredUserName(static_cast< int >(id));
			if (!uname.isEmpty()) {
				mpqu.add_ids(id);
				mpqu.add_names(u8(uname));
			}
		}
		if (mpqu.ids_size())
			sendMessage(uSource, mpqu);
	} else {
		{
			QWriteLocker wl(&qrwlVoiceThread);

			QHash< QString, QSet< int > > hOldTemp;

			if (Meta::mp->bLogGroupChanges || Meta::mp->bLogACLChanges) {
				log(uSource, QString::fromLatin1("Updating ACL in channel %1").arg(*c));
			}

			if (Meta::mp->bLogGroupChanges) {
				logGroups(this, c, QLatin1String("These are the groups before applying the change:"));
			}

			for (Group *g : c->qhGroups) {
				hOldTemp.insert(g->qsName, g->qsTemporary);
				delete g;
			}

			if (Meta::mp->bLogACLChanges) {
				logACLs(this, c, QLatin1String("These are the ACLs before applying the changed:"));
			}

			// Clear old ACLs
			for (ChanACL *a : c->qlACL) {
				delete a;
			}

			c->qhGroups.clear();
			c->qlACL.clear();

			c->bInheritACL = msg.inherit_acls();

			// Add new groups
			for (int i = 0; i < msg.groups_size(); ++i) {
				const MumbleProto::ACL_ChanGroup &group = msg.groups(i);
				Group *g                                = new Group(c, u8(group.name()));
				g->bInherit                             = group.inherit();
				g->bInheritable                         = group.inheritable();
				for (int j = 0; j < group.add_size(); ++j)
					if (!getRegisteredUserName(static_cast< int >(group.add(j))).isEmpty())
						g->qsAdd << static_cast< int >(group.add(j));
				for (int j = 0; j < group.remove_size(); ++j)
					if (!getRegisteredUserName(static_cast< int >(group.remove(j))).isEmpty())
						g->qsRemove << static_cast< int >(group.remove(j));

				g->qsTemporary = hOldTemp.value(g->qsName);
			}

			if (Meta::mp->bLogGroupChanges) {
				logGroups(this, c, QLatin1String("And these are the new groups:"));
			}

			// Add new ACLs
			for (int i = 0; i < msg.acls_size(); ++i) {
				const MumbleProto::ACL_ChanACL &mpacl = msg.acls(i);
				if (mpacl.has_user_id() && getRegisteredUserName(static_cast< int >(mpacl.user_id())).isEmpty())
					continue;

				ChanACL *a    = new ChanACL(c);
				a->bApplyHere = mpacl.apply_here();
				a->bApplySubs = mpacl.apply_subs();
				if (mpacl.has_user_id())
					a->iUserId = static_cast< int >(mpacl.user_id());
				else
					a->qsGroup = u8(mpacl.group());
				a->pDeny  = static_cast< ChanACL::Permissions >(mpacl.deny()) & ChanACL::All;
				a->pAllow = static_cast< ChanACL::Permissions >(mpacl.grant()) & ChanACL::All;
			}

			if (Meta::mp->bLogACLChanges) {
				logACLs(this, c, QLatin1String("And these are the new ACLs:"));
			}
		}

		clearACLCache();

		if (!hasPermission(uSource, c, ChanACL::Write) && ((uSource->iId >= 0) || !uSource->qsHash.isEmpty())) {
			{
				QWriteLocker wl(&qrwlVoiceThread);

				ChanACL *a    = new ChanACL(c);
				a->bApplyHere = true;
				a->bApplySubs = false;
				if (uSource->iId >= 0)
					a->iUserId = uSource->iId;
				else
					a->qsGroup = QLatin1Char('$') + uSource->qsHash;
				a->iUserId = uSource->iId;
				a->pDeny   = ChanACL::None;
				a->pAllow  = ChanACL::Write | ChanACL::Traverse;
			}

			clearACLCache();
		}


		if (!c->bTemporary) {
			m_dbWrapper.updateChannelData(iServerNum, *c);
		}
		log(uSource, QString("Updated ACL in channel %1").arg(*c));

		// Send refreshed enter states of this channel to all clients
		MumbleProto::ChannelState mpcs;
		mpcs.set_channel_id(c->iId);

		for (ServerUser *user : qhUsers) {
			mpcs.set_is_enter_restricted(isChannelEnterRestricted(c));
			mpcs.set_can_enter(hasPermission(user, c, ChanACL::Enter));

			sendMessage(user, mpcs);
		}
	}
}

void Server::msgQueryUsers(ServerUser *uSource, MumbleProto::QueryUsers &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	// User needs Write permission on at least one channel in the tree
	bool hasWritePermission = false;
	for (Channel *chan : qhChannels) {
		if (hasPermission(uSource, chan, ChanACL::Write)) {
			hasWritePermission = true;
			break;
		}
	}

	if (!hasWritePermission) {
		return;
	}

	MumbleProto::QueryUsers reply;

	for (int i = 0; i < msg.ids_size(); ++i) {
		unsigned int id     = msg.ids(i);
		const QString &name = getRegisteredUserName(static_cast< int >(id));
		if (!name.isEmpty()) {
			reply.add_ids(id);
			reply.add_names(u8(name));
		}
	}

	for (int i = 0; i < msg.names_size(); ++i) {
		QString name = u8(msg.names(i));
		int id       = getRegisteredUserID(name);
		if (id >= 0) {
			name = getRegisteredUserName(id);
			reply.add_ids(static_cast< unsigned int >(id));
			reply.add_names(u8(name));
		}
	}

	sendMessage(uSource, reply);
}

void Server::msgPing(ServerUser *uSource, MumbleProto::Ping &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	QMutexLocker l(&uSource->qmCrypt);

	uSource->csCrypt->m_statsRemote.good   = msg.good();
	uSource->csCrypt->m_statsRemote.late   = msg.late();
	uSource->csCrypt->m_statsRemote.lost   = msg.lost();
	uSource->csCrypt->m_statsRemote.resync = msg.resync();

	uSource->dUDPPingAvg  = msg.udp_ping_avg();
	uSource->dUDPPingVar  = msg.udp_ping_var();
	uSource->uiUDPPackets = msg.udp_packets();
	uSource->dTCPPingAvg  = msg.tcp_ping_avg();
	uSource->dTCPPingVar  = msg.tcp_ping_var();
	uSource->uiTCPPackets = msg.tcp_packets();

	quint64 ts = msg.timestamp();

	msg.Clear();
	msg.set_timestamp(ts);
	msg.set_good(uSource->csCrypt->m_statsLocal.good);
	msg.set_late(uSource->csCrypt->m_statsLocal.late);
	msg.set_lost(uSource->csCrypt->m_statsLocal.lost);
	msg.set_resync(uSource->csCrypt->m_statsLocal.resync);

	sendMessage(uSource, msg);
}

void Server::msgCryptSetup(ServerUser *uSource, MumbleProto::CryptSetup &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	QMutexLocker l(&uSource->qmCrypt);

	if (!msg.has_client_nonce()) {
		log(uSource, "Requested crypt-nonce resync");
		msg.set_server_nonce(uSource->csCrypt->getEncryptIV());
		sendMessage(uSource, msg);
	} else {
		const std::string &str = msg.client_nonce();
		uSource->csCrypt->m_statsLocal.resync++;
		if (!uSource->csCrypt->setDecryptIV(str)) {
			qWarning("Messages: Cipher resync failed: Invalid nonce from the client!");
		}
	}
}

void Server::msgContextActionModify(ServerUser *, MumbleProto::ContextActionModify &) {
}

void Server::msgContextAction(ServerUser *uSource, MumbleProto::ContextAction &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	unsigned int session = msg.has_session() ? msg.session() : 0;
	int id               = msg.has_channel_id() ? static_cast< int >(msg.channel_id()) : -1;

	if (session && !qhUsers.contains(session))
		return;
	if ((id >= 0) && !qhChannels.contains(static_cast< unsigned int >(id)))
		return;
	emit contextAction(uSource, u8(msg.action()), session, id);
}

/// @param str The std::string to convert
/// @param maxSize The maximum allowed size for this string
/// @returns The given std::string converted to a QString, if its size is less
///  	than or equal to the given maxSize. If it is bigger, "[[Invalid]]"
///  	is returned.
QString convertWithSizeRestriction(const std::string &str, size_t maxSize) {
	if (str.size() > maxSize) {
		return QLatin1String("[[Invalid]]");
	}

	return QString::fromStdString(str);
}

void Server::msgVersion(ServerUser *uSource, MumbleProto::Version &msg) {
	ZoneScoped;

	RATELIMIT(uSource);

	uSource->m_version               = MumbleProto::getVersion(msg);
	uSource->bSupportsPersistentChat = msg.has_supports_persistent_chat() && msg.supports_persistent_chat();
	uSource->bSupportsScreenShareSignaling =
		msg.has_supports_screen_share_signaling() && msg.supports_screen_share_signaling();
	uSource->bSupportsScreenShareCapture =
		msg.has_supports_screen_share_capture() && msg.supports_screen_share_capture();
	uSource->bSupportsScreenShareView     = msg.has_supports_screen_share_view() && msg.supports_screen_share_view();
	uSource->qlSupportedScreenShareCodecs = screenShareCodecListFromVersion(msg);
	uSource->uiMaxScreenShareWidth        = Mumble::ScreenShare::sanitizeLimit(
        msg.has_max_screen_share_width() ? msg.max_screen_share_width() : 0, 0, Mumble::ScreenShare::HARD_MAX_WIDTH);
	uSource->uiMaxScreenShareHeight = Mumble::ScreenShare::sanitizeLimit(
		msg.has_max_screen_share_height() ? msg.max_screen_share_height() : 0, 0, Mumble::ScreenShare::HARD_MAX_HEIGHT);
	uSource->uiMaxScreenShareFps = Mumble::ScreenShare::sanitizeLimit(
		msg.has_max_screen_share_fps() ? msg.max_screen_share_fps() : 0, 0, Mumble::ScreenShare::HARD_MAX_FPS);
	if (msg.has_release()) {
		uSource->qsRelease = convertWithSizeRestriction(msg.release(), 100);
	}
	if (msg.has_os()) {
		uSource->qsOS = convertWithSizeRestriction(msg.os(), 40);

		if (msg.has_os_version()) {
			uSource->qsOSVersion = convertWithSizeRestriction(msg.os_version(), 60);
		}
	}

	log(uSource, QString("Client version %1 (%2 %3: %4)")
					 .arg(Version::toString(uSource->m_version))
					 .arg(uSource->qsOS)
					 .arg(uSource->qsOSVersion)
					 .arg(uSource->qsRelease));
	if (uSource->bSupportsScreenShareSignaling) {
		screenShareDiagnosticLog(
			QStringLiteral("Client %1 advertised screen-share support capture=%2 view=%3 codecs=%4 max=%5x%6@%7")
				.arg(uSource->uiSession)
				.arg(uSource->bSupportsScreenShareCapture)
				.arg(uSource->bSupportsScreenShareView)
				.arg(Mumble::ScreenShare::codecPreferenceString(uSource->qlSupportedScreenShareCodecs))
				.arg(uSource->uiMaxScreenShareWidth)
				.arg(uSource->uiMaxScreenShareHeight)
				.arg(uSource->uiMaxScreenShareFps));
	}
}

void Server::msgUserList(ServerUser *uSource, MumbleProto::UserList &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	// The register permission is required on the root channel to be allowed to
	// view the registered users.
	if (!hasPermission(uSource, qhChannels.value(0), ChanACL::Register)) {
		PERM_DENIED(uSource, qhChannels.value(0), ChanACL::Register);
		return;
	}

	if (msg.users_size() == 0) {
		// Query mode.
		std::vector< UserInfo > users = getAllRegisteredUserProperties();
		for (const UserInfo &info : users) {
			// Skip the SuperUser
			if (info.user_id > 0 && static_cast< unsigned int >(info.user_id) != Mumble::SUPERUSER_ID) {
				::MumbleProto::UserList_User *user = msg.add_users();
				user->set_user_id(static_cast< unsigned int >(info.user_id));
				user->set_name(u8(info.name));
				if (info.last_channel) {
					user->set_last_channel(static_cast< unsigned int >(info.last_channel.value()));
				}
				user->set_last_seen(u8(info.last_active.toString(Qt::ISODate)));
			}
		}
		sendMessage(uSource, msg);
	} else {
		// Update mode
		for (int i = 0; i < msg.users_size(); ++i) {
			const MumbleProto::UserList_User &user = msg.users(i);

			unsigned int id = user.user_id();
			if (id == 0)
				continue;

			if (!user.has_name()) {
				log(uSource, QString::fromLatin1("Unregistered user %1").arg(id));
				unregisterUser(static_cast< int >(id));
			} else {
				const QString &name = u8(user.name()).trimmed();
				if (validateUserName(name)) {
					log(uSource, QString::fromLatin1("Renamed user %1 to '%2'").arg(QString::number(id), name));

					QMap< int, QString > info;
					info.insert(static_cast< int >(::mumble::server::db::UserProperty::Name), name);
					setUserProperties(static_cast< int >(id), info);

					MumbleProto::UserState mpus;
					for (ServerUser *serverUser : qhUsers) {
						if (serverUser->iId == static_cast< int >(id)) {
							serverUser->qsName = name;
							mpus.set_session(serverUser->uiSession);
							break;
						}
					}
					if (mpus.has_session()) {
						mpus.set_actor(uSource->uiSession);
						mpus.set_name(u8(name));
						sendAll(mpus);
					}
				} else {
					MumbleProto::PermissionDenied mppd;
					mppd.set_type(MumbleProto::PermissionDenied_DenyType_UserName);
					if (uSource->m_version < Version::fromComponents(1, 2, 1))
						mppd.set_reason(u8(QString::fromLatin1("%1 is not a valid username").arg(name)));
					else
						mppd.set_name(u8(name));
					sendMessage(uSource, mppd);
				}
			}
		}
	}
}

void Server::msgVoiceTarget(ServerUser *uSource, MumbleProto::VoiceTarget &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	int target = static_cast< int >(msg.id());
	if ((target < 1) || (target >= 0x1f))
		return;

	QWriteLocker lock(&qrwlVoiceThread);

	uSource->qmTargetCache.remove(target);

	int count = msg.targets_size();
	if (count == 0) {
		uSource->qmTargets.remove(target);
	} else {
		WhisperTarget wt;
		for (int i = 0; i < count; ++i) {
			const MumbleProto::VoiceTarget_Target &t = msg.targets(i);
			for (int j = 0; j < t.session_size(); ++j) {
				unsigned int s = t.session(j);
				if (qhUsers.contains(s)) {
					wt.sessions.push_back(s);
				}
			}
			if (t.has_channel_id()) {
				unsigned int id = t.channel_id();
				if (qhChannels.contains(id)) {
					WhisperTarget::Channel wtc;
					wtc.id              = id;
					wtc.includeChildren = t.children();
					wtc.includeLinks    = t.links();
					if (t.has_group()) {
						wtc.targetGroup = u8(t.group());
					}

					wt.channels.push_back(wtc);
				}
			}
		}
		if (wt.sessions.empty() && wt.channels.empty()) {
			uSource->qmTargets.remove(target);
		} else {
			uSource->qmTargets.insert(target, std::move(wt));
		}
	}
}

void Server::msgPermissionQuery(ServerUser *uSource, MumbleProto::PermissionQuery &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	sendClientPermission(uSource, c, true);
}

void Server::msgCodecVersion(ServerUser *, MumbleProto::CodecVersion &) {
}

void Server::msgUserStats(ServerUser *uSource, MumbleProto::UserStats &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);
	VICTIM_SETUP;
	const BandwidthRecord &bwr            = pDstServerUser->bwr;
	const QList< QSslCertificate > &certs = pDstServerUser->peerCertificateChain();

	bool extend = (uSource == pDstServerUser) || hasPermission(uSource, qhChannels.value(0), ChanACL::Ban);

	if (!extend && !hasPermission(uSource, pDstServerUser->cChannel, ChanACL::Enter)) {
		PERM_DENIED(uSource, pDstServerUser->cChannel, ChanACL::Enter);
		return;
	}

	bool details = extend;
	bool local   = extend || (pDstServerUser->cChannel == uSource->cChannel);

	if (msg.stats_only())
		details = false;

	msg.Clear();
	msg.set_session(pDstServerUser->uiSession);

	if (details) {
		for (const QSslCertificate &cert : certs) {
			const QByteArray &der = cert.toDer();
			msg.add_certificates(blob(der));
		}
		msg.set_strong_certificate(pDstServerUser->bVerified);
	}

	if (local) {
		MumbleProto::UserStats_Stats *mpusss;

		QMutexLocker l(&pDstServerUser->qmCrypt);

		mpusss = msg.mutable_from_client();
		mpusss->set_good(pDstServerUser->csCrypt->m_statsLocal.good);
		mpusss->set_late(pDstServerUser->csCrypt->m_statsLocal.late);
		mpusss->set_lost(pDstServerUser->csCrypt->m_statsLocal.lost);
		mpusss->set_resync(pDstServerUser->csCrypt->m_statsLocal.resync);

		mpusss = msg.mutable_from_server();
		mpusss->set_good(pDstServerUser->csCrypt->m_statsRemote.good);
		mpusss->set_late(pDstServerUser->csCrypt->m_statsRemote.late);
		mpusss->set_lost(pDstServerUser->csCrypt->m_statsRemote.lost);
		mpusss->set_resync(pDstServerUser->csCrypt->m_statsRemote.resync);

		bool outsideInitialWindow =
			static_cast< unsigned int >(bwr.onlineSeconds()) > pDstServerUser->csCrypt->m_rollingWindow.count();

		MumbleProto::UserStats_RollingStats *mpussrs = msg.mutable_rolling_stats();

		mpusss = mpussrs->mutable_from_client();
		if (outsideInitialWindow) {
			mpusss->set_good(pDstServerUser->csCrypt->m_statsLocalRolling.good);
			mpusss->set_late(pDstServerUser->csCrypt->m_statsLocalRolling.late);
			mpusss->set_lost(pDstServerUser->csCrypt->m_statsLocalRolling.lost);
			mpusss->set_resync(pDstServerUser->csCrypt->m_statsLocalRolling.resync);
		} else {
			mpusss->CopyFrom(*msg.mutable_from_client());
		}

		mpusss = mpussrs->mutable_from_server();
		if (outsideInitialWindow) {
			mpusss->set_good(pDstServerUser->csCrypt->m_statsRemoteRolling.good);
			mpusss->set_late(pDstServerUser->csCrypt->m_statsRemoteRolling.late);
			mpusss->set_lost(pDstServerUser->csCrypt->m_statsRemoteRolling.lost);
			mpusss->set_resync(pDstServerUser->csCrypt->m_statsRemoteRolling.resync);
		} else {
			mpusss->CopyFrom(*msg.mutable_from_server());
		}

		mpussrs->set_time_window(pDstServerUser->csCrypt->m_rollingWindow.count());
	}

	msg.set_udp_packets(pDstServerUser->uiUDPPackets);
	msg.set_tcp_packets(pDstServerUser->uiTCPPackets);
	msg.set_udp_ping_avg(pDstServerUser->dUDPPingAvg);
	msg.set_udp_ping_var(pDstServerUser->dUDPPingVar);
	msg.set_tcp_ping_avg(pDstServerUser->dTCPPingAvg);
	msg.set_tcp_ping_var(pDstServerUser->dTCPPingVar);

	if (details) {
		MumbleProto::Version *mpv;

		mpv = msg.mutable_version();
		if (pDstServerUser->m_version != Version::UNKNOWN) {
			MumbleProto::setVersion(*mpv, pDstServerUser->m_version);
		}
		if (!pDstServerUser->qsRelease.isEmpty()) {
			mpv->set_release(u8(pDstServerUser->qsRelease));
		}
		if (!pDstServerUser->qsOS.isEmpty()) {
			mpv->set_os(u8(pDstServerUser->qsOS));
			if (!pDstServerUser->qsOSVersion.isEmpty())
				mpv->set_os_version(u8(pDstServerUser->qsOSVersion));
		}

		for (int v : pDstServerUser->qlCodecs) {
			msg.add_celt_versions(v);
		}
		msg.set_opus(pDstServerUser->bOpus);

		msg.set_address(pDstServerUser->haAddress.toStdString());
	}

	if (local)
		msg.set_bandwidth(static_cast< unsigned int >(bwr.bandwidth()));
	msg.set_onlinesecs(static_cast< unsigned int >(bwr.onlineSeconds()));
	if (local)
		msg.set_idlesecs(static_cast< unsigned int >(bwr.idleSeconds()));

	sendMessage(uSource, msg);
}

void Server::msgRequestBlob(ServerUser *uSource, MumbleProto::RequestBlob &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	int ntextures     = msg.session_texture_size();
	int ncomments     = msg.session_comment_size();
	int ndescriptions = msg.channel_description_size();

	if (ndescriptions) {
		MumbleProto::ChannelState mpcs;
		for (int i = 0; i < ndescriptions; ++i) {
			unsigned int id = msg.channel_description(i);
			Channel *c      = qhChannels.value(id);
			if (c && !c->qsDesc.isEmpty()) {
				mpcs.set_channel_id(id);
				mpcs.set_description(u8(c->qsDesc));
				sendMessage(uSource, mpcs);
			}
		}
	}
	if (ntextures || ncomments) {
		MumbleProto::UserState mpus;
		for (int i = 0; i < ntextures; ++i) {
			unsigned int session = msg.session_texture(i);
			ServerUser *su       = qhUsers.value(session);
			if (su && !su->qbaTexture.isEmpty()) {
				mpus.set_session(session);
				mpus.set_texture(blob(su->qbaTexture));
				sendMessage(uSource, mpus);
			}
		}
		if (ntextures)
			mpus.clear_texture();
		for (int i = 0; i < ncomments; ++i) {
			unsigned int session = msg.session_comment(i);
			ServerUser *su       = qhUsers.value(session);
			if (su && !su->qsComment.isEmpty()) {
				mpus.set_session(session);
				mpus.set_comment(u8(su->qsComment));
				sendMessage(uSource, mpus);
			}
		}
	}
}

void Server::msgServerConfig(ServerUser *uSource, MumbleProto::ServerConfig &msg) {
	MSG_SETUP(ServerUser::Authenticated);

	Channel *rootChannel = qhChannels.value(Mumble::ROOT_CHANNEL_ID);
	if (!rootChannel) {
		return;
	}

	if (!hasPermission(uSource, rootChannel, ChanACL::Write)) {
		PERM_DENIED(uSource, rootChannel, ChanACL::Write);
		return;
	}

	auto applyConfig = [this](const char *key, const QString &value) {
		if (value.trimmed().isEmpty()) {
			m_dbWrapper.clearConfiguration(iServerNum, key);
		} else {
			m_dbWrapper.setConfiguration(iServerNum, key, u8(value));
		}
		setLiveConf(QLatin1String(key), value);
	};
	auto applyBoolConfig = [&applyConfig](const char *key, bool value) {
		applyConfig(key, value ? QLatin1String("true") : QLatin1String("false"));
	};
	auto applyPositiveIntConfig = [&applyConfig](const char *key, unsigned int value) {
		const unsigned int cappedValue =
			std::min(value, static_cast< unsigned int >(std::numeric_limits< int >::max()));
		applyConfig(key, cappedValue > 0 ? QString::number(cappedValue) : QString());
	};

	if (msg.has_welcome_text()) {
		applyConfig("welcometext", u8(msg.welcome_text()));
	}
	if (msg.has_max_bandwidth()) {
		applyPositiveIntConfig("bandwidth", msg.max_bandwidth());
	}
	if (msg.has_allow_html()) {
		applyBoolConfig("allowhtml", msg.allow_html());
	}
	if (msg.has_message_length()) {
		applyPositiveIntConfig("textmessagelength", msg.message_length());
	}
	if (msg.has_image_message_length()) {
		applyPositiveIntConfig("imagemessagelength", msg.image_message_length());
	}
	if (msg.has_max_users()) {
		applyPositiveIntConfig("users", msg.max_users());
	}
	if (msg.has_recording_allowed()) {
		applyBoolConfig("allowrecording", msg.recording_allowed());
	}
	if (msg.has_persistent_global_chat_enabled()) {
		applyBoolConfig("persistentglobalchat", msg.persistent_global_chat_enabled());
	}
	if (msg.has_screen_share_enabled()) {
		applyBoolConfig("screen_share_enabled", msg.screen_share_enabled());
	}
	if (msg.has_screen_share_recording_enabled()) {
		applyBoolConfig("screen_share_recording_enabled", msg.screen_share_recording_enabled());
	}
	if (msg.has_screen_share_helper_required()) {
		applyBoolConfig("screen_share_helper_required", msg.screen_share_helper_required());
	}
	if (msg.preferred_screen_share_codecs_size() > 0) {
		QStringList codecTokens;
		for (int i = 0; i < msg.preferred_screen_share_codecs_size(); ++i) {
			const MumbleProto::ScreenShareCodec codec = msg.preferred_screen_share_codecs(i);
			if (Mumble::ScreenShare::isValidCodec(codec)) {
				codecTokens << Mumble::ScreenShare::codecToConfigToken(codec);
			}
		}
		if (!codecTokens.isEmpty()) {
			applyConfig("screen_share_codec_preferences", codecTokens.join(QLatin1Char(' ')));
		}
	}
	if (msg.has_screen_share_max_width()) {
		applyPositiveIntConfig("screen_share_max_width", msg.screen_share_max_width());
	}
	if (msg.has_screen_share_max_height()) {
		applyPositiveIntConfig("screen_share_max_height", msg.screen_share_max_height());
	}
	if (msg.has_screen_share_max_fps()) {
		applyPositiveIntConfig("screen_share_max_fps", msg.screen_share_max_fps());
	}
	if (msg.has_screen_share_relay_url()) {
		applyConfig("screen_share_relay_url", u8(msg.screen_share_relay_url()).trimmed());
	}
}

void Server::msgSuggestConfig(ServerUser *, MumbleProto::SuggestConfig &) {
}

void Server::msgPluginDataTransmission(ServerUser *sender, MumbleProto::PluginDataTransmission &msg) {
	ZoneScoped;

	// A client's plugin has sent us a message that we shall delegate to its receivers

	if (sender->m_pluginMessageBucket.ratelimit(1)) {
		qWarning("Dropping plugin message sent from \"%s\" (%d)", qUtf8Printable(sender->qsName), sender->uiSession);
		return;
	}

	if (!msg.has_data() || !msg.has_dataid()) {
		// Messages without data and/or without a data ID can't be used by the clients. Thus we don't even have to send
		// them
		return;
	}

	if (msg.data().size() > Mumble::Plugins::PluginMessage::MAX_DATA_LENGTH) {
		qWarning("Dropping plugin message sent from \"%s\" (%d) - data too large", qUtf8Printable(sender->qsName),
				 sender->uiSession);
		return;
	}
	if (msg.dataid().size() > Mumble::Plugins::PluginMessage::MAX_DATA_ID_LENGTH) {
		qWarning("Dropping plugin message sent from \"%s\" (%d) - data ID too long", qUtf8Printable(sender->qsName),
				 sender->uiSession);
		return;
	}

	// Always set the sender's session and don't rely on it being set correctly (would
	// allow spoofing the sender's session)
	msg.set_sendersession(sender->uiSession);

	// Copy needed data from message in order to be able to remove info about receivers from the message as this doesn't
	// matter for the client
	size_t receiverAmount = static_cast< std::size_t >(msg.receiversessions_size());
	const ::google::protobuf::RepeatedField<::google::protobuf::uint32 > receiverSessions = msg.receiversessions();

	msg.clear_receiversessions();

	QSet< uint32_t > uniqueReceivers;
	uniqueReceivers.reserve(receiverSessions.size());

	for (int i = 0; static_cast< size_t >(i) < receiverAmount; i++) {
		uint32_t userSession = receiverSessions.Get(i);

		if (!uniqueReceivers.contains(userSession)) {
			uniqueReceivers.insert(userSession);
		} else {
			// Duplicate entry -> ignore
			continue;
		}

		ServerUser *receiver = qhUsers.value(receiverSessions.Get(i));

		if (receiver) {
			// We can simply redirect the message we have received to the clients
			sendMessage(receiver, msg);
		}
	}
}

void Server::msgScreenShareCreate(ServerUser *uSource, MumbleProto::ScreenShareCreate &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	auto deny = [&](const QString &reason) {
		screenShareDiagnosticLog(QStringLiteral("Denied create from session %1 in channel %2: %3")
									 .arg(uSource->uiSession)
									 .arg(uSource->cChannel ? uSource->cChannel->iId : 0)
									 .arg(reason));
		MumbleProto::PermissionDenied denied;
		denied.set_session(uSource->uiSession);
		denied.set_type(MumbleProto::PermissionDenied_DenyType_Text);
		denied.set_reason(u8(reason));
		sendMessage(uSource, denied);
	};

	if (!bScreenShareEnabled) {
		deny(QStringLiteral("Screen sharing is disabled on this server."));
		return;
	}
	if (!supportsScreenShareCapture(uSource)) {
		deny(QStringLiteral("This client does not advertise screen-share capture support."));
		return;
	}

	const MumbleProto::ScreenShareScope scope = msg.has_scope() ? msg.scope() : MumbleProto::ScreenShareScopeChannel;
	if (scope != MumbleProto::ScreenShareScopeChannel) {
		deny(QStringLiteral("Only channel-scoped screen sharing is supported in this build."));
		return;
	}

	Channel *scopeChannel = msg.has_scope_id() ? screenShareScopeChannel(scope, msg.scope_id()) : uSource->cChannel;
	if (!scopeChannel || scopeChannel != uSource->cChannel) {
		deny(QStringLiteral("Screen shares can only be published in the publisher's current channel."));
		return;
	}
	if (!hasPermission(uSource, scopeChannel, ChanACL::Speak)) {
		PERM_DENIED(uSource, scopeChannel, ChanACL::Speak);
		return;
	}
	if (qhScreenShareStreamByOwnerSession.contains(uSource->uiSession)) {
		deny(QStringLiteral("Only one active screen share per user is allowed."));
		return;
	}
	if (qhScreenShareStreamByChannel.contains(scopeChannel->iId)) {
		deny(QStringLiteral("This channel already has an active screen share."));
		return;
	}
	if (!Mumble::ScreenShare::isValidRelayUrl(qsScreenShareRelayUrl)) {
		deny(QStringLiteral("Screen sharing is unavailable because no relay endpoint is configured."));
		return;
	}

	QList< int > requestedCodecs = screenShareCodecListFromCreate(msg);
	if (requestedCodecs.isEmpty()) {
		requestedCodecs = uSource->qlSupportedScreenShareCodecs;
	}
	requestedCodecs = Mumble::ScreenShare::sanitizeCodecList(requestedCodecs);
	const MumbleProto::ScreenShareRelayTransport relayTransport =
		Mumble::ScreenShare::relayTransportFromUrl(qsScreenShareRelayUrl);
	const QList< int > preferredCodecs = Mumble::ScreenShare::isWebRtcRelayTransport(relayTransport)
											 ? Mumble::ScreenShare::webRtcRelayCodecPreferenceList()
											 : qlPreferredScreenShareCodecs;
	const MumbleProto::ScreenShareCodec codec =
		Mumble::ScreenShare::selectPreferredCodec(preferredCodecs, requestedCodecs);
	if (codec == MumbleProto::ScreenShareCodecUnknown) {
		deny(QStringLiteral("No compatible screen-share codec could be negotiated."));
		return;
	}

	QList< int > codecFallbackOrder;
	for (const int preferredCodec : preferredCodecs) {
		if (requestedCodecs.contains(preferredCodec)) {
			codecFallbackOrder.append(preferredCodec);
		}
	}
	for (const int requestedCodec : requestedCodecs) {
		if (!codecFallbackOrder.contains(requestedCodec)) {
			codecFallbackOrder.append(requestedCodec);
		}
	}

	ScreenShareStream stream;
	stream.qsStreamID          = QUuid::createUuid().toString(QUuid::WithoutBraces);
	stream.uiOwnerSession      = uSource->uiSession;
	stream.scope               = scope;
	stream.uiScopeID           = scopeChannel->iId;
	stream.qsRelayRoomID       = QStringLiteral("screen-share-%1-%2").arg(iServerNum).arg(stream.qsStreamID);
	stream.qsRelayUrl          = qsScreenShareRelayUrl;
	stream.qsRelaySessionID    = QUuid::createUuid().toString(QUuid::WithoutBraces);
	stream.qsRelayPublishToken = randomMessageRelayCredential();
	stream.qsRelayViewToken    = randomMessageRelayCredential();
	stream.uiRelayTokenExpiresAt =
		static_cast< quint64 >(QDateTime::currentMSecsSinceEpoch()) + MESSAGE_SCREEN_SHARE_RELAY_TOKEN_LIFETIME_MSEC;
	stream.relayTransport       = relayTransport;
	stream.uiCreatedAt          = static_cast< quint64 >(QDateTime::currentMSecsSinceEpoch());
	stream.state                = MumbleProto::ScreenShareLifecycleStateActive;
	stream.codec                = codec;
	stream.qlCodecFallbackOrder = codecFallbackOrder;
	stream.uiWidth              = Mumble::ScreenShare::negotiateLimit(
        msg.has_requested_width() ? msg.requested_width() : 0, uSource->uiMaxScreenShareWidth, uiScreenShareMaxWidth,
        Mumble::ScreenShare::DEFAULT_MAX_WIDTH, Mumble::ScreenShare::HARD_MAX_WIDTH);
	stream.uiHeight = Mumble::ScreenShare::negotiateLimit(
		msg.has_requested_height() ? msg.requested_height() : 0, uSource->uiMaxScreenShareHeight,
		uiScreenShareMaxHeight, Mumble::ScreenShare::DEFAULT_MAX_HEIGHT, Mumble::ScreenShare::HARD_MAX_HEIGHT);
	stream.uiFps = Mumble::ScreenShare::negotiateLimit(
		msg.has_requested_fps() ? msg.requested_fps() : 0, uSource->uiMaxScreenShareFps, uiScreenShareMaxFps,
		Mumble::ScreenShare::DEFAULT_MAX_FPS, Mumble::ScreenShare::HARD_MAX_FPS);
	stream.uiBitrateKbps =
		Mumble::ScreenShare::sanitizeBitrateKbps(msg.has_requested_bitrate_kbps() ? msg.requested_bitrate_kbps() : 0,
												 stream.codec, stream.uiWidth, stream.uiHeight, stream.uiFps);

	qhScreenShareStreams.insert(stream.qsStreamID, stream);
	qhScreenShareStreamByOwnerSession.insert(stream.uiOwnerSession, stream.qsStreamID);
	qhScreenShareStreamByChannel.insert(stream.uiScopeID, stream.qsStreamID);

	ScreenShareStream &storedStream = qhScreenShareStreams[stream.qsStreamID];
	sendScreenShareStateToAudience(storedStream);
	screenShareDiagnosticLog(QStringLiteral("Created stream %1 owner=%2 channel=%3 codec=%4 requested_codecs=%5 "
											"preferred_codecs=%6 size=%7x%8@%9 bitrate=%10 relay=%11")
								 .arg(storedStream.qsStreamID)
								 .arg(storedStream.uiOwnerSession)
								 .arg(storedStream.uiScopeID)
								 .arg(Mumble::ScreenShare::codecToConfigToken(storedStream.codec))
								 .arg(Mumble::ScreenShare::codecPreferenceString(requestedCodecs))
								 .arg(Mumble::ScreenShare::codecPreferenceString(preferredCodecs))
								 .arg(storedStream.uiWidth)
								 .arg(storedStream.uiHeight)
								 .arg(storedStream.uiFps)
								 .arg(storedStream.uiBitrateKbps)
								 .arg(Mumble::ScreenShare::relayTransportToConfigToken(storedStream.relayTransport)));
	log(uSource, QString::fromLatin1("Started screen share %1 (%2 %3x%4@%5 %6 kbps)")
					 .arg(storedStream.qsStreamID)
					 .arg(Mumble::ScreenShare::codecToConfigToken(storedStream.codec))
					 .arg(storedStream.uiWidth)
					 .arg(storedStream.uiHeight)
					 .arg(storedStream.uiFps)
					 .arg(storedStream.uiBitrateKbps));
}

void Server::msgScreenShareState(ServerUser *, MumbleProto::ScreenShareState &) {
}

void Server::msgScreenShareOffer(ServerUser *uSource, MumbleProto::ScreenShareOffer &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	if (!msg.has_stream_id()) {
		return;
	}

	const QString streamID = u8(msg.stream_id());
	if (!qhScreenShareStreams.contains(streamID)) {
		return;
	}

	const ScreenShareStream &stream = qhScreenShareStreams.value(streamID);
	Channel *channel                = screenShareScopeChannel(stream.scope, stream.uiScopeID);
	if (!channel || uSource->cChannel != channel) {
		return;
	}

	const bool isOwner = uSource->uiSession == stream.uiOwnerSession;
	ServerUser *target = nullptr;
	if (isOwner) {
		if (!msg.has_viewer_session()) {
			return;
		}

		target = qhUsers.value(msg.viewer_session());
		if (!target || target->cChannel != channel || !supportsScreenShareView(target)
			|| !target->qlSupportedScreenShareCodecs.contains(static_cast< int >(stream.codec))) {
			return;
		}
	} else {
		if (!supportsScreenShareView(uSource)
			|| !uSource->qlSupportedScreenShareCodecs.contains(static_cast< int >(stream.codec))) {
			return;
		}

		target = qhUsers.value(stream.uiOwnerSession);
		if (!target || target->cChannel != channel || !supportsScreenShareCapture(target)) {
			return;
		}

		msg.set_viewer_session(uSource->uiSession);
	}

	msg.set_stream_id(u8(stream.qsStreamID));
	msg.set_owner_session(stream.uiOwnerSession);
	if (!stream.qsRelayRoomID.isEmpty()) {
		msg.set_relay_room_id(u8(stream.qsRelayRoomID));
	}
	screenShareDiagnosticLog(QStringLiteral("Forwarding offer stream=%1 from=%2 to=%3 viewer=%4 sdp_bytes=%5")
								 .arg(stream.qsStreamID)
								 .arg(uSource->uiSession)
								 .arg(target ? target->uiSession : 0)
								 .arg(msg.has_viewer_session() ? msg.viewer_session() : 0)
								 .arg(msg.has_sdp() ? msg.sdp().size() : 0));
	sendMessage(target, msg);
}

void Server::msgScreenShareAnswer(ServerUser *uSource, MumbleProto::ScreenShareAnswer &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	if (!msg.has_stream_id()) {
		return;
	}

	const QString streamID = u8(msg.stream_id());
	if (!qhScreenShareStreams.contains(streamID)) {
		return;
	}

	const ScreenShareStream &stream = qhScreenShareStreams.value(streamID);
	Channel *channel                = screenShareScopeChannel(stream.scope, stream.uiScopeID);
	if (!channel || uSource->cChannel != channel) {
		return;
	}

	const bool isOwner = uSource->uiSession == stream.uiOwnerSession;
	ServerUser *target = nullptr;
	if (isOwner) {
		if (!msg.has_viewer_session()) {
			return;
		}

		target = qhUsers.value(msg.viewer_session());
		if (!target || target->cChannel != channel || !supportsScreenShareView(target)) {
			return;
		}
	} else {
		if (!supportsScreenShareView(uSource)) {
			return;
		}

		target = qhUsers.value(stream.uiOwnerSession);
		if (!target || target->cChannel != channel || !supportsScreenShareCapture(target)) {
			return;
		}

		msg.set_viewer_session(uSource->uiSession);
	}

	msg.set_stream_id(u8(stream.qsStreamID));
	msg.set_owner_session(stream.uiOwnerSession);
	if (!stream.qsRelayRoomID.isEmpty()) {
		msg.set_relay_room_id(u8(stream.qsRelayRoomID));
	}
	screenShareDiagnosticLog(QStringLiteral("Forwarding answer stream=%1 from=%2 to=%3 viewer=%4 sdp_bytes=%5")
								 .arg(stream.qsStreamID)
								 .arg(uSource->uiSession)
								 .arg(target ? target->uiSession : 0)
								 .arg(msg.has_viewer_session() ? msg.viewer_session() : 0)
								 .arg(msg.has_sdp() ? msg.sdp().size() : 0));
	sendMessage(target, msg);
}

void Server::msgScreenShareIceCandidate(ServerUser *uSource, MumbleProto::ScreenShareIceCandidate &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	if (!msg.has_stream_id()) {
		return;
	}

	const QString streamID = u8(msg.stream_id());
	if (!qhScreenShareStreams.contains(streamID)) {
		return;
	}

	const ScreenShareStream &stream = qhScreenShareStreams.value(streamID);
	Channel *channel                = screenShareScopeChannel(stream.scope, stream.uiScopeID);
	if (!channel || uSource->cChannel != channel) {
		return;
	}

	const bool isOwner = uSource->uiSession == stream.uiOwnerSession;
	ServerUser *target = nullptr;
	if (isOwner) {
		if (!msg.has_viewer_session()) {
			return;
		}

		target = qhUsers.value(msg.viewer_session());
		if (!target || target->cChannel != channel || !supportsScreenShareView(target)) {
			return;
		}
	} else {
		if (!supportsScreenShareView(uSource)) {
			return;
		}

		target = qhUsers.value(stream.uiOwnerSession);
		if (!target || target->cChannel != channel || !supportsScreenShareCapture(target)) {
			return;
		}

		msg.set_viewer_session(uSource->uiSession);
	}

	msg.set_stream_id(u8(stream.qsStreamID));
	msg.set_owner_session(stream.uiOwnerSession);
	screenShareDiagnosticLog(QStringLiteral("Forwarding ICE stream=%1 from=%2 to=%3 viewer=%4 candidate_bytes=%5")
								 .arg(stream.qsStreamID)
								 .arg(uSource->uiSession)
								 .arg(target ? target->uiSession : 0)
								 .arg(msg.has_viewer_session() ? msg.viewer_session() : 0)
								 .arg(msg.has_candidate() ? msg.candidate().size() : 0));
	sendMessage(target, msg);
}

void Server::msgScreenShareStop(ServerUser *uSource, MumbleProto::ScreenShareStop &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	if (!msg.has_stream_id()) {
		return;
	}

	const QString streamID = u8(msg.stream_id());
	if (!qhScreenShareStreams.contains(streamID)) {
		return;
	}

	const ScreenShareStream &stream = qhScreenShareStreams.value(streamID);
	if (stream.uiOwnerSession != uSource->uiSession) {
		return;
	}

	screenShareDiagnosticLog(
		QStringLiteral("Stopping stream %1 by owner %2 reason=%3")
			.arg(streamID)
			.arg(uSource->uiSession)
			.arg(msg.has_reason() ? u8(msg.reason()) : QStringLiteral("Screen share stopped by publisher")));
	stopScreenShare(streamID, uSource->uiSession, MumbleProto::ScreenShareLifecycleStateStopped,
					msg.has_reason() ? u8(msg.reason()) : QStringLiteral("Screen share stopped by publisher"));
}

#undef RATELIMIT
#undef MSG_SETUP
#undef MSG_SETUP_NO_UNIDLE
#undef VICTIM_SETUP
#undef PERM_DENIED
#undef PERM_DENIED_TYPE
#undef PERM_DENIED_FALLBACK
#undef PERM_DENIED_HASH
