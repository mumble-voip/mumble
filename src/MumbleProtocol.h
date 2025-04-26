// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLEPROTOCOL_H_
#define MUMBLE_MUMBLEPROTOCOL_H_

#include "MumbleUDP.pb.h"
#include "Version.h"
#include "VolumeAdjustment.h"

#include <cstdint>
#include <vector>

#include <gsl/span>

/**
 * "X-macro" for all Mumble Protobuf TCP messages types.
 *
 * Warning: Only append to the end. Never insert in between or remove an existing entry.
 */
#define MUMBLE_ALL_TCP_MESSAGES                         \
	PROCESS_MUMBLE_TCP_MESSAGE(Version, 0)              \
	PROCESS_MUMBLE_TCP_MESSAGE(UDPTunnel, 1)            \
	PROCESS_MUMBLE_TCP_MESSAGE(Authenticate, 2)         \
	PROCESS_MUMBLE_TCP_MESSAGE(Ping, 3)                 \
	PROCESS_MUMBLE_TCP_MESSAGE(Reject, 4)               \
	PROCESS_MUMBLE_TCP_MESSAGE(ServerSync, 5)           \
	PROCESS_MUMBLE_TCP_MESSAGE(ChannelRemove, 6)        \
	PROCESS_MUMBLE_TCP_MESSAGE(ChannelState, 7)         \
	PROCESS_MUMBLE_TCP_MESSAGE(UserRemove, 8)           \
	PROCESS_MUMBLE_TCP_MESSAGE(UserState, 9)            \
	PROCESS_MUMBLE_TCP_MESSAGE(BanList, 10)             \
	PROCESS_MUMBLE_TCP_MESSAGE(TextMessage, 11)         \
	PROCESS_MUMBLE_TCP_MESSAGE(PermissionDenied, 12)    \
	PROCESS_MUMBLE_TCP_MESSAGE(ACL, 13)                 \
	PROCESS_MUMBLE_TCP_MESSAGE(QueryUsers, 14)          \
	PROCESS_MUMBLE_TCP_MESSAGE(CryptSetup, 15)          \
	PROCESS_MUMBLE_TCP_MESSAGE(ContextActionModify, 16) \
	PROCESS_MUMBLE_TCP_MESSAGE(ContextAction, 17)       \
	PROCESS_MUMBLE_TCP_MESSAGE(UserList, 18)            \
	PROCESS_MUMBLE_TCP_MESSAGE(VoiceTarget, 19)         \
	PROCESS_MUMBLE_TCP_MESSAGE(PermissionQuery, 20)     \
	PROCESS_MUMBLE_TCP_MESSAGE(CodecVersion, 21)        \
	PROCESS_MUMBLE_TCP_MESSAGE(UserStats, 22)           \
	PROCESS_MUMBLE_TCP_MESSAGE(RequestBlob, 23)         \
	PROCESS_MUMBLE_TCP_MESSAGE(ServerConfig, 24)        \
	PROCESS_MUMBLE_TCP_MESSAGE(SuggestConfig, 25)       \
	PROCESS_MUMBLE_TCP_MESSAGE(PluginDataTransmission, 26)

/**
 * "X-macro" for all Mumble Protobuf UDP messages types.
 *
 * Warning: Only append to the end. Never insert in between or remove an existing entry.
 */
#define MUMBLE_ALL_UDP_MESSAGES          \
	PROCESS_MUMBLE_UDP_MESSAGE(Audio, 0) \
	PROCESS_MUMBLE_UDP_MESSAGE(Ping, 1)

namespace Mumble {
namespace Protocol {

	using byte = std::uint8_t;

	// The maximum allowed size in bytes of UDP packets (according to the Mumble protocol)
	constexpr std::size_t MAX_UDP_PACKET_SIZE = 1024;

#define PROCESS_MUMBLE_TCP_MESSAGE(name, value) name = value,
	/**
	 * Enum holding all possible TCP message types
	 */
	enum class TCPMessageType : byte { MUMBLE_ALL_TCP_MESSAGES };
#undef PROCESS_MUMBLE_TCP_MESSAGE
#define PROCESS_MUMBLE_UDP_MESSAGE(name, value) name = value,
	/**
	 * Enum holding all possible UDP message types
	 */
	enum class UDPMessageType : byte { MUMBLE_ALL_UDP_MESSAGES };
#undef PROCESS_MUMBLE_UDP_MESSAGE

	enum class LegacyUDPMessageType : byte { VoiceCELTAlpha, Ping, VoiceSpeex, VoiceCELTBeta, VoiceOpus };

	enum class AudioCodec {
		Opus,
		CELT_Alpha, // 0.7.0
		CELT_Beta,  // 0.11.0
		Speex,
	};

	namespace ReservedTargetIDs {
		constexpr unsigned int REGULAR_SPEECH  = 0;
		constexpr unsigned int SERVER_LOOPBACK = 31;
	} // namespace ReservedTargetIDs

	using audio_context_t = byte;
	namespace AudioContext {
		constexpr audio_context_t INVALID = 0xFF; // This is the equivalent of -1 as a signed 8bit number
		constexpr audio_context_t NORMAL  = 0;
		constexpr audio_context_t SHOUT   = 1;
		constexpr audio_context_t WHISPER = 2;
		constexpr audio_context_t LISTEN  = 3;

		constexpr audio_context_t BEGIN = NORMAL;
		constexpr audio_context_t END   = LISTEN + 1;
	} // namespace AudioContext

	enum class Role { Server, Client };

	constexpr Version::full_t PROTOBUF_INTRODUCTION_VERSION = Version::fromComponents(1, 5, 0);


	bool protocolVersionsAreCompatible(Version::full_t lhs, Version::full_t rhs);


	template< Role role > class ProtocolHandler {
	public:
		ProtocolHandler(Version::full_t protocolVersion = Version::UNKNOWN);

		Version::full_t getProtocolVersion() const;
		void setProtocolVersion(Version::full_t protocolVersion);

		constexpr Role getRole() const { return role; };

	protected:
		Version::full_t m_protocolVersion;
	};

	struct AudioData {
		std::uint32_t targetOrContext = ReservedTargetIDs::REGULAR_SPEECH;
		AudioCodec usedCodec          = AudioCodec::Opus;
		std::uint32_t senderSession   = 0;
		std::uint64_t frameNumber     = 0;
		gsl::span< const byte > payload;
		bool isLastFrame                  = false;
		bool containsPositionalData       = false;
		std::array< float, 3 > position   = { 0, 0, 0 };
		VolumeAdjustment volumeAdjustment = VolumeAdjustment::fromFactor(1.0f);

		friend bool operator==(const AudioData &lhs, const AudioData &rhs);
		friend bool operator!=(const AudioData &lhs, const AudioData &rhs);
	};

	struct PingData {
		std::uint64_t timestamp            = 0;
		bool requestAdditionalInformation  = false;
		bool containsAdditionalInformation = false;
		Version::full_t serverVersion      = Version::UNKNOWN;
		std::uint32_t userCount            = 0;
		std::uint32_t maxUserCount         = 0;
		std::uint32_t maxBandwidthPerUser  = 0;

		friend bool operator==(const PingData &lhs, const PingData &rhs);
		friend bool operator!=(const PingData &lhs, const PingData &rhs);
	};

	template< Role role > class UDPAudioEncoder : public ProtocolHandler< role > {
	public:
		UDPAudioEncoder(Version::full_t protocolVersion = Version::UNKNOWN);

		/**
		 * Encodes an audio packet based on the provided data.
		 * Note: Incremental encoding is also supported via the prepare and update functions.
		 *
		 * @param data The AudioData to encode
		 * @return A span to the encoded data (ready to be sent out)
		 */
		gsl::span< const byte > encodeAudioPacket(const AudioData &data);
		/**
		 * Prepares an audio packet by encoding the "static" part of the audio data. The static part contains
		 * things like the actual audio payload, its type and the sender's session.
		 * In order to also encode positional data, call addPositionalData after calling this function and
		 * to encode the rest of the audio data, call updateAudioPacket after that.
		 *
		 * Note: Calls to this function remove any previously encoded variable parts or positional audio
		 * from the audio packet.
		 *
		 * @param data The AudioData to encode (partially!)
		 */
		void prepareAudioPacket(const AudioData &data);
		/**
		 * This function assumes that an audio packet has already been prepared. In that case it will encode
		 * the "variable" part of the audio packet which contains e.g. audio context (or audio target) and
		 * volume adjustments (if supported by the used protocol).
		 *
		 * @param data The AudioData to encode (partially!)
		 * @return A span to the encoded audio packet (including the static part and potentially positional data)
		 */
		gsl::span< const byte > updateAudioPacket(const AudioData &data);
		/**
		 * This function assumes that an audio packet has already been prepared. In that case it will encode
		 * the given positional data (if any) into the audio packet.
		 *
		 * Note: A call to this function invalidates any variable part that might have been added to the audio
		 * packet before. Thus, another call to updateAudioPacket is required after calling this function.
		 *
		 * @param data The AudioData to take the positional data from
		 */
		void addPositionalData(const AudioData &data);
		/**
		 * This function assumes that an audio packet has already been prepared. In that case it will remove
		 * positional data from the audio packet that was previously added using addPositionalData.
		 *
		 * Note: A call to this function invalidates any variable part that might have been added to the audio
		 * packet before. Thus, another call to updateAudioPacket is required after calling this function.
		 *
		 */
		void dropPositionalData();

	protected:
		static constexpr const int preEncodedDBAdjustmentBegin = -60;
		static constexpr const int preEncodedDBAdjustmentEnd   = 30 + 1;

		std::vector< byte > m_byteBuffer;
		std::size_t m_staticPartSize      = 0;
		std::size_t m_positionalAudioSize = 0;
		MumbleUDP::Audio m_audioMessage;
		std::vector< std::vector< byte > > m_preEncodedContext;
		std::vector< std::vector< byte > > m_preEncodedVolumeAdjustment;

		void prepareAudioPacket_legacy(const AudioData &data);
		gsl::span< const byte > updateAudioPacket_legacy(const AudioData &data);
		void addPositionalData_legacy(const AudioData &data);

		void prepareAudioPacket_protobuf(const AudioData &data);
		gsl::span< const byte > updateAudioPacket_protobuf(const AudioData &data);
		void addPositionalData_protobuf(const AudioData &data);

		void preparePreEncodedSnippets();

		gsl::span< const byte > getPreEncodedContext(audio_context_t context) const;
		gsl::span< const byte > getPreEncodedVolumeAdjustment(const VolumeAdjustment &adjustment) const;
	};

	template< Role role > class UDPPingEncoder : public ProtocolHandler< role > {
	public:
		UDPPingEncoder(Version::full_t protocolVersion = Version::UNKNOWN);

		gsl::span< const byte > encodePingPacket(const PingData &data);

	protected:
		std::vector< byte > m_byteBuffer;
		MumbleUDP::Ping m_pingMessage;

		gsl::span< const byte > encodePingPacket_legacy(const PingData &data);
		gsl::span< const byte > encodePingPacket_protobuf(const PingData &data);
	};

	template< Role role > class UDPDecoder : public ProtocolHandler< role > {
	public:
		UDPDecoder(Version::full_t protocolVersion = Version::UNKNOWN);

		gsl::span< byte > getBuffer();
		bool decode(const gsl::span< const byte > data, bool restrictToPing = false);
		bool decodePing(const gsl::span< const byte > data);

		UDPMessageType getMessageType() const;

		AudioData getAudioData() const;
		PingData getPingData() const;

	protected:
		std::vector< byte > m_byteBuffer;
		UDPMessageType m_messageType;
		AudioData m_audioData = {};
		PingData m_pingData   = {};
		MumbleUDP::Ping m_pingMessage;
		MumbleUDP::Audio m_audioMessage;

		bool decodePing_legacy(const gsl::span< const byte > data);
		bool decodePing_protobuf(const gsl::span< const byte > data);
		bool decodeAudio_legacy(const gsl::span< const byte > data, AudioCodec codec);
		bool decodeAudio_protobuf(const gsl::span< const byte > data);
	};

} // namespace Protocol
} // namespace Mumble

/**
 * This is merely a dummy-function (never used) that is required as a scope for dummy-switch statements on our message
 * type enums. These will cause a compiler error, if there are any entries that have the same numeric value (which we
 * never want to happen). See https://stackoverflow.com/a/50385277
 */
inline void ThisFunctionIsNeverCalledAndShouldSimplyBeOptimizedOut() {
#define ARRAY_NAME Mumble::Protocol::TCPMessageType
#define PROCESS_MUMBLE_TCP_MESSAGE(name, value) \
	case ARRAY_NAME::name:                      \
		break;
	switch (static_cast< ARRAY_NAME >(0)) {
		MUMBLE_ALL_TCP_MESSAGES
		default:
			break;
	}
#undef ARRAY_NAME
#undef PROCESS_MUMBLE_TCP_MESSAGE

#define ARRAY_NAME Mumble::Protocol::UDPMessageType
#define PROCESS_MUMBLE_UDP_MESSAGE(name, value) \
	case ARRAY_NAME::name:                      \
		break;
	switch (static_cast< ARRAY_NAME >(0)) {
		MUMBLE_ALL_UDP_MESSAGES
		default:
			break;
	}
#undef ARRAY_NAME
#undef PROCESS_MUMBLE_UDP_MESSAGE
}

#endif // MUMBLE_MUMBLEPROTOCOL_H_
