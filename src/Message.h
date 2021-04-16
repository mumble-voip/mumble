// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MESSAGE_H_
#define MUMBLE_MESSAGE_H_

#include <QtCore/QCryptographicHash>
#include <QtCore/QString>
#include <string>

/**
  Protobuf packet type enumeration for message handler generation.

  Warning: Only append to the end.
 */
#define MUMBLE_MH_ALL                  \
	MUMBLE_MH_MSG(Version)             \
	MUMBLE_MH_MSG(UDPTunnel)           \
	MUMBLE_MH_MSG(Authenticate)        \
	MUMBLE_MH_MSG(Ping)                \
	MUMBLE_MH_MSG(Reject)              \
	MUMBLE_MH_MSG(ServerSync)          \
	MUMBLE_MH_MSG(ChannelRemove)       \
	MUMBLE_MH_MSG(ChannelState)        \
	MUMBLE_MH_MSG(UserRemove)          \
	MUMBLE_MH_MSG(UserState)           \
	MUMBLE_MH_MSG(BanList)             \
	MUMBLE_MH_MSG(TextMessage)         \
	MUMBLE_MH_MSG(PermissionDenied)    \
	MUMBLE_MH_MSG(ACL)                 \
	MUMBLE_MH_MSG(QueryUsers)          \
	MUMBLE_MH_MSG(CryptSetup)          \
	MUMBLE_MH_MSG(ContextActionModify) \
	MUMBLE_MH_MSG(ContextAction)       \
	MUMBLE_MH_MSG(UserList)            \
	MUMBLE_MH_MSG(VoiceTarget)         \
	MUMBLE_MH_MSG(PermissionQuery)     \
	MUMBLE_MH_MSG(CodecVersion)        \
	MUMBLE_MH_MSG(UserStats)           \
	MUMBLE_MH_MSG(RequestBlob)         \
	MUMBLE_MH_MSG(ServerConfig)        \
	MUMBLE_MH_MSG(SuggestConfig)       \
	MUMBLE_MH_MSG(PluginDataTransmission)

class MessageHandler {
public:
	enum UDPMessageType { UDPVoiceCELTAlpha, UDPPing, UDPVoiceSpeex, UDPVoiceCELTBeta, UDPVoiceOpus };

#define MUMBLE_MH_MSG(x) x,
	enum MessageType { MUMBLE_MH_ALL };
#undef MUMBLE_MH_MSG
};

/// UDPMessageTypeIsValidVoicePacket checks whether the given
/// UDPMessageType is a valid voice packet.
inline bool UDPMessageTypeIsValidVoicePacket(MessageHandler::UDPMessageType umt) {
	switch (umt) {
		case MessageHandler::UDPVoiceCELTAlpha:
		case MessageHandler::UDPVoiceSpeex:
		case MessageHandler::UDPVoiceCELTBeta:
		case MessageHandler::UDPVoiceOpus:
			return true;
		case MessageHandler::UDPPing:
			return false;
	}
	return false;
}

inline QString u8(const ::std::string &str) {
	return QString::fromUtf8(str.data(), static_cast< int >(str.length()));
}

inline QString u8(const ::std::wstring &str) {
	return QString::fromStdWString(str);
}

inline ::std::string u8(const QString &str) {
	const QByteArray &qba = str.toUtf8();
	return ::std::string(qba.constData(), qba.length());
}

inline QByteArray blob(const ::std::string &str) {
	return QByteArray(str.data(), static_cast< int >(str.length()));
}

inline ::std::string blob(const QByteArray &str) {
	return ::std::string(str.constData(), str.length());
}

inline QByteArray sha1(const QByteArray &blob) {
	return QCryptographicHash::hash(blob, QCryptographicHash::Sha1);
}

inline QByteArray sha1(const QString &str) {
	return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha1);
}

#endif
