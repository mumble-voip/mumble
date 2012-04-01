#ifndef MESSAGE_H
#define MESSAGE_H

/**
  Protobuf packet type enumeration for message handler generation.

  Warning: Only append to the end.
 */
#define MUMBLE_MH_ALL \
	MUMBLE_MH_MSG(Version) \
	MUMBLE_MH_MSG(UDPTunnel) \
	MUMBLE_MH_MSG(Authenticate) \
	MUMBLE_MH_MSG(Ping) \
	MUMBLE_MH_MSG(Reject) \
	MUMBLE_MH_MSG(ServerSync) \
	MUMBLE_MH_MSG(ChannelRemove) \
	MUMBLE_MH_MSG(ChannelState) \
	MUMBLE_MH_MSG(UserRemove) \
	MUMBLE_MH_MSG(UserState) \
	MUMBLE_MH_MSG(BanList) \
	MUMBLE_MH_MSG(TextMessage) \
	MUMBLE_MH_MSG(PermissionDenied) \
	MUMBLE_MH_MSG(ACL) \
	MUMBLE_MH_MSG(QueryUsers) \
	MUMBLE_MH_MSG(CryptSetup) \
	MUMBLE_MH_MSG(ContextActionModify) \
	MUMBLE_MH_MSG(ContextAction) \
	MUMBLE_MH_MSG(UserList) \
	MUMBLE_MH_MSG(VoiceTarget) \
	MUMBLE_MH_MSG(PermissionQuery) \
	MUMBLE_MH_MSG(CodecVersion) \
	MUMBLE_MH_MSG(UserStats) \
	MUMBLE_MH_MSG(RequestBlob) \
	MUMBLE_MH_MSG(ServerConfig) \
	MUMBLE_MH_MSG(SuggestConfig)

struct MessageTypes {
	enum UDPMessageType {
		UDPVoiceCELTAlpha,
		UDPPing,
		UDPVoiceSpeex,
		UDPVoiceCELTBeta,
		UDPVoiceOpus
	};

#define MUMBLE_MH_MSG(x) x,
	enum MessageType {
		MUMBLE_MH_ALL
	};
#undef MUMBLE_MH_MSG
};

#endif
