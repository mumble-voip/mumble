// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MumbleProtocol.h"
#include "PacketDataStream.h"
#include "VolumeAdjustment.h"

#include <QtEndian>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>

namespace Mumble {
namespace Protocol {

	bool protocolVersionsAreCompatible(Version::full_t lhs, Version::full_t rhs) {
		// At this point the protocol version only makes a difference between pre-protobuf and post-protobuf
		return (lhs < PROTOBUF_INTRODUCTION_VERSION) == (rhs < PROTOBUF_INTRODUCTION_VERSION);
	}


	std::size_t getProtobufSize(const ::google::protobuf::Message &message) {
#if GOOGLE_PROTOBUF_VERSION >= 3002000
		// ByteSizeLong() was introduced in Protobuf v3.2 as a replacement for ByteSize()
		return message.ByteSizeLong();
#else
		return message.ByteSize();
#endif
	}

	std::size_t encodeProtobuf(const ::google::protobuf::Message &message, std::vector< byte > &buffer,
							   std::size_t offset, std::size_t maxAllowedSize, bool useCachedSize) {
		// Serialize to buffer
		std::size_t serializedSize;
		if (!useCachedSize) {
			serializedSize = getProtobufSize(message);
		} else {
			serializedSize = static_cast< std::size_t >(message.GetCachedSize());
		}

		assert(serializedSize + offset <= maxAllowedSize);
		if (serializedSize + offset > maxAllowedSize) {
			// In non-Debug builds above assertion will not fire, thus we have to explicitly catch
			// this issue here
			qWarning("Protobuf package size (%zu) would exceed UDP packet size limit (%zu)", serializedSize,
					 maxAllowedSize);

			return 0;
		}

		buffer.resize(serializedSize + offset);

		message.SerializePartialToArray(buffer.data() + offset, static_cast< int >(serializedSize));

		return serializedSize;
	}

	template< Role role >
	ProtocolHandler< role >::ProtocolHandler(Version::full_t protocolVersion) : m_protocolVersion(protocolVersion) {}

	template< Role role > Version::full_t ProtocolHandler< role >::getProtocolVersion() const {
		return m_protocolVersion;
	}

	template< Role role > void ProtocolHandler< role >::setProtocolVersion(Version::full_t protocolVersion) {
		m_protocolVersion = protocolVersion;
	}


	template< Role role >
	UDPAudioEncoder< role >::UDPAudioEncoder(Version::full_t protocolVersion)
		: ProtocolHandler< role >(protocolVersion) {
		m_byteBuffer.resize(MAX_UDP_PACKET_SIZE);

		preparePreEncodedSnippets();
	}

	template< Role role > gsl::span< const byte > UDPAudioEncoder< role >::encodeAudioPacket(const AudioData &data) {
		prepareAudioPacket(data);
		addPositionalData(data);
		return updateAudioPacket(data);
	}

	template< Role role > void UDPAudioEncoder< role >::prepareAudioPacket(const AudioData &data) {
		if (this->getProtocolVersion() < PROTOBUF_INTRODUCTION_VERSION) {
			return prepareAudioPacket_legacy(data);
		} else {
			return prepareAudioPacket_protobuf(data);
		}
	}

	template< Role role > gsl::span< const byte > UDPAudioEncoder< role >::updateAudioPacket(const AudioData &data) {
		if (this->getProtocolVersion() < PROTOBUF_INTRODUCTION_VERSION) {
			return updateAudioPacket_legacy(data);
		} else {
			return updateAudioPacket_protobuf(data);
		}
	}

	template< Role role > void UDPAudioEncoder< role >::addPositionalData(const AudioData &data) {
		if (this->getProtocolVersion() < PROTOBUF_INTRODUCTION_VERSION) {
			addPositionalData_legacy(data);
		} else {
			addPositionalData_protobuf(data);
		}
	}

	template< Role role > void UDPAudioEncoder< role >::dropPositionalData() {
		// Pretend the positional data wasn't there
		m_positionalAudioSize = m_staticPartSize;
	}

	template< Role role > void UDPAudioEncoder< role >::prepareAudioPacket_legacy(const AudioData &data) {
		m_byteBuffer.resize(MAX_UDP_PACKET_SIZE);

		byte type = 0;
		switch (data.usedCodec) {
			case AudioCodec::CELT_Alpha:
				type = 0;
				break;
				// flag = 1 is reserved for ping packets
			case AudioCodec::Speex:
				type = 2;
				break;
			case AudioCodec::CELT_Beta:
				type = 3;
				break;
			case AudioCodec::Opus:
				type = 4;
				break;
		}
		// The audio format (aka: package type) has to be written to the 3 most significant bits of the header byte
		assert(type < (1 << 3));
		type = static_cast< decltype(type) >(type << 5);

		m_byteBuffer[0] = type;

		PacketDataStream stream(m_byteBuffer.data() + 1, static_cast< unsigned int >(m_byteBuffer.size() - 1));

		if (this->getRole() == Role::Server) {
			stream << data.senderSession;
		}

		// The next field contains the sequence number of the first contained audio frame
		stream << static_cast< int >(data.frameNumber);

		switch (data.usedCodec) {
			case AudioCodec::Opus: {
				// If the sent frame is the last one, we set the 14th bit of the size field to indicate this
				assert(data.payload.size() < (1 << 13));
				stream << static_cast< int >(data.isLastFrame ? data.payload.size() | (1 << 13) : data.payload.size());

				// After the size has been encoded, we write the actual Opus frame to the message
				stream.append(reinterpret_cast< const char * >(data.payload.data()),
							  static_cast< unsigned int >(data.payload.size()));
				break;
			}
			case AudioCodec::CELT_Alpha:
			case AudioCodec::CELT_Beta:
			case AudioCodec::Speex: {
				// Simply append the provided payload
				stream.append(reinterpret_cast< const char * >(data.payload.data()),
							  static_cast< unsigned int >(data.payload.size()));
				break;
			}
		}

		// +1 since the stream doesn't know about the flags header byte
		m_staticPartSize = stream.size() + 1;

		if (!stream.isValid()) {
			qWarning("MumbleProtocol: Encoding legacy packet (fixed part) overflowed buffer size");
			m_staticPartSize = 0;
		}

		m_positionalAudioSize = m_staticPartSize;
	}

	template< Role role >
	gsl::span< const byte > UDPAudioEncoder< role >::updateAudioPacket_legacy(const AudioData &data) {
		m_byteBuffer.resize(MAX_UDP_PACKET_SIZE);

		// The 5 least significant bits are where the target is supposed to be encoded
		if (data.targetOrContext >= (1 << 5)) {
			// Invalid target - this can easily happen when activating PTT before being connected to a server
			return {};
		}
		// Re-assemble the header byte by overtaking the 3 most significant bits encoding the audio/packet type
		// and combine that with the target.
		m_byteBuffer[0] = static_cast< byte >(data.targetOrContext) | (m_byteBuffer[0] & 0xe0);

		std::size_t packetSize = data.containsPositionalData ? m_positionalAudioSize : m_staticPartSize;

		return gsl::span< byte >(m_byteBuffer.data(), packetSize);
	}


	template< Role role > void UDPAudioEncoder< role >::addPositionalData_legacy(const AudioData &data) {
		if (data.containsPositionalData) {
			assert(m_byteBuffer.size() >= m_staticPartSize);
			PacketDataStream stream(m_byteBuffer.data() + m_staticPartSize,
									static_cast< unsigned int >(m_byteBuffer.size() - m_staticPartSize));

			// Positional data simply gets attached to the stream after the audio payload
			assert(data.position.size() == 3);
			stream << data.position[0];
			stream << data.position[1];
			stream << data.position[2];

			m_positionalAudioSize = stream.size() + m_staticPartSize;

			if (!stream.isValid()) {
				qWarning("MumbleProtocol: Adding positional data to legacy packet overflowed buffer size");
				m_positionalAudioSize = m_staticPartSize;
			}
		}
	}

	template< Role role > void UDPAudioEncoder< role >::prepareAudioPacket_protobuf(const AudioData &data) {
		// At the moment only Opus is supported in the newer Protobuf UDP protocol
		// if the encoding is different, we automatically fall back to the legacy package format.
		if (data.usedCodec != AudioCodec::Opus) {
			prepareAudioPacket_legacy(data);
		}

		// Note that we are partitioning the audio packet into two segments: a "fixed" part and a "variable" part.
		// The former contains all fields of the audio data that do not depend on where the audio is sent, whereas
		// everything that may be different, depending to whom the message is sent, is part of the latter.
		// This allows to use partial encoding (making use of the fact to Protobuf messages may be concatenated
		// once in wire-format), which avoids having to re-encode the entire message.
		// This is mainly important on the server-side.

		m_audioMessage.Clear();

		if (this->getRole() == Role::Server) {
			m_audioMessage.set_sender_session(data.senderSession);
		}

		m_audioMessage.set_frame_number(data.frameNumber);
		m_audioMessage.set_opus_data(data.payload.data(), data.payload.size());
		m_audioMessage.set_is_terminator(data.isLastFrame);

		// +1 to account for the header byte set below
		m_staticPartSize      = encodeProtobuf(m_audioMessage, m_byteBuffer, 1, MAX_UDP_PACKET_SIZE, false) + 1;
		m_positionalAudioSize = m_staticPartSize;
		m_byteBuffer[0]       = static_cast< byte >(UDPMessageType::Audio);
	}

	std::size_t writeSnippet(gsl::span< const byte > source, std::vector< byte > &destination, std::size_t offset,
							 std::size_t maxPacketSize) {
		if (maxPacketSize <= offset + source.size()) {
			qWarning("MumbleProtocol: Buffer overflow while writing snippet. Max buffer size is %zu and required size "
					 "is %zu",
					 maxPacketSize, offset + source.size());
			return 0;
		}
		destination.resize(offset + source.size());

		std::memcpy(destination.data() + offset, source.data(), source.size());

		return source.size();
	}

	template< Role role >
	gsl::span< const byte > UDPAudioEncoder< role >::updateAudioPacket_protobuf(const AudioData &data) {
		std::size_t offset = data.containsPositionalData ? m_positionalAudioSize : m_staticPartSize;

		// We assume that something was encoded before
		if (offset == 0) {
			qWarning("MumbleProtocol: Can't update a packet that hasn't been prepared yet");
			return {};
		}

		switch (this->getRole()) {
			case Role::Client: {
				m_audioMessage.Clear();
				m_audioMessage.set_target(data.targetOrContext);

				offset += encodeProtobuf(m_audioMessage, m_byteBuffer, offset, MAX_UDP_PACKET_SIZE, false);

				return { m_byteBuffer.data(), offset };
			}
			case Role::Server: {
				if (data.volumeAdjustment.factor != 1.0f) {
					gsl::span< const byte > buffer = getPreEncodedVolumeAdjustment(data.volumeAdjustment);
					if (!buffer.empty()) {
						// Use pre-encoded snippet
						offset += writeSnippet(buffer, m_byteBuffer, offset, MAX_UDP_PACKET_SIZE);
					} else {
						// No pre-encoded snippet found -> use explicit encoding
						m_audioMessage.Clear();
						m_audioMessage.set_volume_adjustment(data.volumeAdjustment.factor);

						offset += encodeProtobuf(m_audioMessage, m_byteBuffer, offset, MAX_UDP_PACKET_SIZE, false);
					}
				}

				gsl::span< const byte > buffer = getPreEncodedContext(static_cast< byte >(data.targetOrContext));
				if (!buffer.empty()) {
					// Use pre-encoded snippet
					offset += writeSnippet(buffer, m_byteBuffer, offset, MAX_UDP_PACKET_SIZE);
				} else {
					// No pre-encoded snippet found -> use explicit encoding
					m_audioMessage.Clear();
					m_audioMessage.set_context(data.targetOrContext);

					offset += encodeProtobuf(m_audioMessage, m_byteBuffer, offset, MAX_UDP_PACKET_SIZE, false);
				}

				return { m_byteBuffer.data(), offset };
			}
		}

		qWarning("MumbleProtocol: Reached theoretically unreachable code");
		return {};
	}


	template< Role role > void UDPAudioEncoder< role >::addPositionalData_protobuf(const AudioData &data) {
		if (data.containsPositionalData) {
			m_audioMessage.Clear();

			for (unsigned int i = 0; i < 3; ++i) {
				m_audioMessage.add_positional_data(data.position[i]);
			}

			m_positionalAudioSize =
				m_staticPartSize
				+ encodeProtobuf(m_audioMessage, m_byteBuffer, m_staticPartSize, MAX_UDP_PACKET_SIZE, false);
		}
	}

	template< Role role > void UDPAudioEncoder< role >::preparePreEncodedSnippets() {
		m_audioMessage.Clear();

		static_assert(AudioContext::BEGIN == 0, "AudioContext::BEGIN is not zero (breaks assumption)");
		static_assert(AudioContext::END > 0, "AudioContext::END is not positive (breaks assumption)");
		m_preEncodedContext.resize(AudioContext::END);

		// Pre-encode the expected voice audio contexts.
		for (audio_context_t current = AudioContext::BEGIN; current < AudioContext::END; ++current) {
			m_audioMessage.set_context(current);

			// The max size of the properly encoded package is the size of the used field type (uint32) plus 1 byte
			// overhead for the varint-encoding plus 1 byte of overhead for encoding the message type and field number.
			bool successful =
				encodeProtobuf(m_audioMessage, m_preEncodedContext[current], 0, sizeof(std::uint32_t) + 1 + 1, false);
			(void) successful;
			assert(successful);
		}

		m_audioMessage.Clear();

		// Pre-encode the expected volume adjustments (the client UI allows to specify integer values between
		// -60dB and +30dB).
		m_preEncodedVolumeAdjustment.resize(preEncodedDBAdjustmentEnd - preEncodedDBAdjustmentBegin);

		for (int dbAdjustment = preEncodedDBAdjustmentBegin; dbAdjustment < preEncodedDBAdjustmentEnd; ++dbAdjustment) {
			float adjustmentFactor = VolumeAdjustment::toFactor(dbAdjustment);

			m_audioMessage.set_volume_adjustment(adjustmentFactor);

			// Store the pre-encoded packet
			// The max-size is the size of the used field (float) plus 1 byte overhead for encoding the field type and
			// number
			bool successful = encodeProtobuf(
				m_audioMessage,
				m_preEncodedVolumeAdjustment[static_cast< std::size_t >(dbAdjustment - preEncodedDBAdjustmentBegin)], 0,
				sizeof(float) + 1, false);
			(void) successful;
			assert(successful);
		}
	}

	template< Role role >
	gsl::span< const byte > UDPAudioEncoder< role >::getPreEncodedContext(audio_context_t context) const {
		if (context >= m_preEncodedContext.size()) {
			return {};
		}

		const std::vector< byte > &data = m_preEncodedContext[context];

		return gsl::span< const byte >(data.data(), data.size());
	}

	template< Role role >
	gsl::span< const byte >
		UDPAudioEncoder< role >::getPreEncodedVolumeAdjustment(const VolumeAdjustment &adjustment) const {
		int index = static_cast< int >(adjustment.dbAdjustment - preEncodedDBAdjustmentBegin);

		if (adjustment.dbAdjustment == VolumeAdjustment::INVALID_DB_ADJUSTMENT || index < 0
			|| static_cast< std::size_t >(index) >= m_preEncodedVolumeAdjustment.size()) {
			// No pre-encoded snippet for the given adjustment
			return {};
		}

		const std::vector< byte > &data = m_preEncodedVolumeAdjustment[static_cast< std::size_t >(index)];

		return gsl::span< const byte >(data.data(), data.size());
	}


	template< Role role >
	UDPPingEncoder< role >::UDPPingEncoder(Version::full_t protocolVersion) : ProtocolHandler< role >(protocolVersion) {
		// Use the assumption that a general ping package will be < 32bytes long (the legacy ping packet is at most
		// 12bytes long)
		m_byteBuffer.reserve(32);
	}

	template< Role role > gsl::span< const byte > UDPPingEncoder< role >::encodePingPacket(const PingData &data) {
		if (this->getProtocolVersion() < PROTOBUF_INTRODUCTION_VERSION) {
			return encodePingPacket_legacy(data);
		} else {
			return encodePingPacket_protobuf(data);
		}
	}

	template< Role role >
	gsl::span< const byte > UDPPingEncoder< role >::encodePingPacket_legacy(const PingData &data) {
		m_byteBuffer.clear();

		std::size_t actualSize = 0;

		if (data.requestAdditionalInformation || data.containsAdditionalInformation) {
			const bool writeAdditionalInformation =
				data.containsAdditionalInformation && this->getRole() == Role::Server;

			// 8 bytes for a uint64 timestamp and 4 emtpy bytes (the server will write its (protocol) version (uint32)
			// in that place before bouncing the ping back; Any further additional info beyond that will be appended to
			// the packet by the server).
			std::size_t packetSize;
			if (writeAdditionalInformation) {
				// uint32: server version
				// uint64: timestamp
				// uint32: user count
				// uint32: max. user count
				// uint32: max. bandwidth per user
				packetSize = 4 * sizeof(std::uint32_t) + sizeof(std::uint64_t);
			} else {
				// uint32: zero (empty spot for the server to fill in its version)
				// uint64: timestamp
				packetSize = sizeof(std::uint32_t) + sizeof(std::uint64_t);
			}

			m_byteBuffer.resize(packetSize);

			if (writeAdditionalInformation) {
				std::uint32_t *dataArray = reinterpret_cast< std::uint32_t * >(m_byteBuffer.data());
				// Legacy pings expect the version to be encoded using the old (legacy) format
				dataArray[0] = qToBigEndian(Version::toLegacyVersion(data.serverVersion));
				// dataArray[1] and dataArray[2] together hold the timestamp (written below)
				dataArray[3] = qToBigEndian(data.userCount);
				dataArray[4] = qToBigEndian(data.maxUserCount);
				dataArray[5] = qToBigEndian(data.maxBandwidthPerUser);
			}

			// Leave four empty bytes up front and then write the uint64 timestamp to the remaining 8 bytes
			*reinterpret_cast< std::uint64_t * >(m_byteBuffer.data() + sizeof(std::uint32_t)) = data.timestamp;

			actualSize = packetSize;
		} else {
			// 8 bytes for a uint64 timestamp + 1byte for the varint encoding of that stamp + 1byte header
			constexpr std::size_t MAX_SIZE = 8 + 1 + 1;
			m_byteBuffer.resize(MAX_SIZE);

			// Write header byte (type bits are zero, so they don't have to be set explicitly)
			m_byteBuffer[0] = static_cast< byte >(LegacyUDPMessageType::Ping) << 5;

			PacketDataStream stream(m_byteBuffer.data() + 1, MAX_SIZE - 1);

			stream << static_cast< quint64 >(data.timestamp);

			// +1 as the stream doesn't know about the header byte
			actualSize = stream.size() + 1;
		}

		return gsl::span< byte >(m_byteBuffer.data(), actualSize);
	}

	template< Role role >
	gsl::span< const byte > UDPPingEncoder< role >::encodePingPacket_protobuf(const PingData &data) {
		m_pingMessage.Clear();

		m_pingMessage.set_timestamp(data.timestamp);

		if (data.requestAdditionalInformation) {
			m_pingMessage.set_request_extended_information(true);
		} else if (data.containsAdditionalInformation) {
			m_pingMessage.set_server_version_v2(data.serverVersion);
			m_pingMessage.set_user_count(data.userCount);
			m_pingMessage.set_max_user_count(data.maxUserCount);
			m_pingMessage.set_max_bandwidth_per_user(data.maxBandwidthPerUser);
		}

		// +1 in order to account for the header byte written below
		std::size_t serializedSize = encodeProtobuf(m_pingMessage, m_byteBuffer, 1, MAX_UDP_PACKET_SIZE, false) + 1;
		m_byteBuffer[0]            = static_cast< byte >(UDPMessageType::Ping);

		return gsl::span< byte >(m_byteBuffer.data(), serializedSize);
	}


	template< Role role >
	UDPDecoder< role >::UDPDecoder(Version::full_t protocolVersion) : ProtocolHandler< role >(protocolVersion) {
		m_byteBuffer.resize(MAX_UDP_PACKET_SIZE);
	}

	template< Role role > gsl::span< byte > UDPDecoder< role >::getBuffer() {
		return gsl::span< byte >(m_byteBuffer.data(), m_byteBuffer.size());
	}

	template< Role role > bool UDPDecoder< role >::decode(const gsl::span< const byte > data, bool restrictToPing) {
		if (data.size() <= 1) {
			// Empty packages or packages consisting only of the header byte are invalid
			return false;
		}

		byte header = data[0];

		if (this->getProtocolVersion() < PROTOBUF_INTRODUCTION_VERSION) {
			// Note: For ping messages we might still have to check the new format, since it could happen that we
			// are receiving pings of a server/client whose version we don't know yet.
			static_assert(static_cast< byte >(UDPMessageType::Ping)
							  != (static_cast< unsigned int >(TCPMessageType::Ping) << 5),
						  "Unexpected coincidence of ping header byte values");

			if (header == static_cast< byte >(UDPMessageType::Ping)) {
				// If the ping message is in the new format, we assume a protocol version of at least
				// PROTOBUF_INTRODUCTION_VERSION
				this->setProtocolVersion(std::max(this->getProtocolVersion(), PROTOBUF_INTRODUCTION_VERSION));

				return decodePing_protobuf(data.subspan(1, data.size() - 1));
			}

			// This might be a legacy ping that requests additional information (they don't come with a header)
			// When set from the client this will have a length of 12 bytes and when sent from the server it will
			// have a size of 24 bytes.
			if ((data.size() == 12 || data.size() == 24) && decodePing_legacy(data)) {
				return true;
			}

			if (restrictToPing) {
				// This is not a ping, so we error out early
				return false;
			}

			// In the legacy format, the message type is encoded as the 3 most significant bits in the header byte
			LegacyUDPMessageType legacyMessageType = static_cast< LegacyUDPMessageType >((header >> 5) & 0x7);

			switch (legacyMessageType) {
				case LegacyUDPMessageType::Ping:
					return decodePing_legacy(data.subspan(1, data.size() - 1));
				case LegacyUDPMessageType::VoiceCELTAlpha:
					return decodeAudio_legacy(data, AudioCodec::CELT_Alpha);
				case LegacyUDPMessageType::VoiceCELTBeta:
					return decodeAudio_legacy(data, AudioCodec::CELT_Beta);
				case LegacyUDPMessageType::VoiceSpeex:
					return decodeAudio_legacy(data, AudioCodec::Speex);
				case LegacyUDPMessageType::VoiceOpus:
					return decodeAudio_legacy(data, AudioCodec::Opus);
			}

			// Invalid message
			return false;
		} else {
			switch (static_cast< UDPMessageType >(header)) {
				case UDPMessageType::Audio:
					if (restrictToPing) {
						// Not a ping
						return false;
					}

					return decodeAudio_protobuf(data.subspan(1, data.size() - 1));
				case UDPMessageType::Ping:
					return decodePing_protobuf(data.subspan(1, data.size() - 1));
			}

			// Unknown package type
			return false;
		}
	}

	template< Role role > bool UDPDecoder< role >::decodePing(const gsl::span< const byte > data) {
		return decode(data, true);
	}

	template< Role role > UDPMessageType UDPDecoder< role >::getMessageType() const { return m_messageType; }

	template< Role role > AudioData UDPDecoder< role >::getAudioData() const {
		assert(m_messageType == UDPMessageType::Audio);

		return m_audioData;
	}

	template< Role role > PingData UDPDecoder< role >::getPingData() const {
		assert(m_messageType == UDPMessageType::Ping);

		return m_pingData;
	}

	template< Role role > bool UDPDecoder< role >::decodePing_legacy(const gsl::span< const byte > data) {
		m_messageType = UDPMessageType::Ping;
		m_pingData    = {};

		if (data.empty()) {
			return false;
		}

		PacketDataStream stream(data.data(), static_cast< unsigned int >(data.size()));

		if (data.size() <= sizeof(std::uint64_t) + 1) {
			// Regular connectivity ping (contains a single varint which may be up to a full 64bit number plus
			// one header byte
			quint64 timestamp;
			stream >> timestamp;

			m_pingData.timestamp = timestamp;

			return true;
		}

		switch (this->getRole()) {
			case Role::Client: {
				// Client-specific code
				static_assert(6 * sizeof(std::uint32_t) == 24, "Unexpected size of uint32_t");
				if (data.size() == 6 * sizeof(std::uint32_t)) {
					// Extended ping containing meta-information
					const std::uint32_t *dataArray = reinterpret_cast< const std::uint32_t * >(data.data());

					// Legacy pings only support the version to be encoded using the old (legacy) format
					m_pingData.serverVersion = Version::fromLegacyVersion(qFromBigEndian(dataArray[0]));

					// Virtual array entries 1 and 2 are actually a single uint64. Note that the timestamp is
					// whatever the client sent to the server and thus it does not require an endian-transformation
					m_pingData.timestamp           = *reinterpret_cast< const std::uint64_t * >(&dataArray[1]);
					m_pingData.userCount           = qFromBigEndian(dataArray[3]);
					m_pingData.maxUserCount        = qFromBigEndian(dataArray[4]);
					m_pingData.maxBandwidthPerUser = qFromBigEndian(dataArray[5]);

					return true;
				} else {
					// Invalid size for legacy ping packet
					return false;
				}

				break;
			}
			case Role::Server: {
				// Server-specific code
				if (data.size() == 4 + sizeof(std::uint64_t) && data[0] == 0 && data[1] == 0 && data[2] == 0
					&& data[3] == 0) {
					// Extended information ping request message. When received by the server, the message contains 4
					// leading, blank bytes followed by a 64bit client-specific timestamp. Thus, the only meaningful
					// decoding to do right now, is reading out the timestamp. Note that the byte-order of this field
					// (and its contents in general) is unspecified and thus the server code should never try to make
					// sense of it.
					m_pingData.timestamp = *reinterpret_cast< const std::uint64_t * >(data.data() + 4);
					m_pingData.requestAdditionalInformation = true;

					return true;
				} else {
					// Invalid size for legacy ping packet
					return false;
				}
				break;
			}
		}

		// This code should never be reached
		return false;
	}

	template< Role role > bool UDPDecoder< role >::decodePing_protobuf(const gsl::span< const byte > data) {
		m_messageType = UDPMessageType::Ping;
		m_pingData    = {};

		if (data.empty()) {
			return false;
		}

		if (!m_pingMessage.ParseFromArray(data.data(), static_cast< int >(data.size()))) {
			// Invalid format
			return false;
		}

		// m_pingMessage now contains the parsed data
		m_pingData.timestamp     = m_pingMessage.timestamp();
		m_pingData.serverVersion = m_pingMessage.server_version_v2();

		// 0 is not a valid version specifier, so if this field is zero, it means Protobuf has used a default
		// value and thus the field was not set. Thus we assume that none of the extra fields are set.
		m_pingData.containsAdditionalInformation = m_pingData.serverVersion != 0;
		if (m_pingData.containsAdditionalInformation) {
			m_pingData.userCount           = m_pingMessage.user_count();
			m_pingData.maxUserCount        = m_pingMessage.max_user_count();
			m_pingData.maxBandwidthPerUser = m_pingMessage.max_bandwidth_per_user();
		}

		m_pingData.requestAdditionalInformation = m_pingMessage.request_extended_information();

		return true;
	}

	template< Role role >
	bool UDPDecoder< role >::decodeAudio_legacy(const gsl::span< const byte > data, AudioCodec codec) {
		m_messageType = UDPMessageType::Audio;
		m_audioData   = {};

		if (data.size() < 1 + 1 + 1) {
			// Audio packets must at least contain one header byte, at least one byte varint-encoding the sequence
			// number and at least one byte of audio payload.
			return false;
		}

		// The target or context is encoded as the five least significant bits of the header byte
		m_audioData.targetOrContext = data[0] & 0x1f;
		m_audioData.usedCodec       = codec;

		PacketDataStream stream(data.data() + 1, static_cast< unsigned int >(data.size() - 1));

		if (this->getRole() == Role::Client) {
			// When the client receives audio packets from the server, there will be an extra field containing the
			// session ID of the client whose audio this is. This field is not present when a client sends audio to the
			// server (as the server knows where the connection is coming from).
			stream >> m_audioData.senderSession;
		}

		quint64 helper;
		stream >> helper;
		m_audioData.frameNumber = helper;

		byte *payloadBegin        = nullptr;
		std::uint64_t payloadSize = 0;
		switch (codec) {
			case AudioCodec::CELT_Alpha:
			case AudioCodec::CELT_Beta:
			case AudioCodec::Speex:
				payloadBegin = stream.dataPtr();
				// For these old codecs, multiple frames may be sent as one payload. Each frame is started by a TOC byte
				// which encodes the length of the following frame and whether there will be a frame after it. The
				// length is encoded as the 7 least significant bits (0x7f) whereas the continuation flag is encoded in
				// the most significant bit (0x80).
				byte header;
				do {
					header = static_cast< byte >(stream.next());

					unsigned int currentFrameSize = header & 0x7f;

					if (currentFrameSize == 0) {
						// An empty frame means that this is the end of the audio transmission
						m_audioData.isLastFrame = true;
					}

					payloadSize += currentFrameSize;

					stream.skip(currentFrameSize);
				} while ((header & 0x80) && stream.isValid());

				break;
			case AudioCodec::Opus:
				// An Opus payload starts with a varint-encoded size field. The max. size is 0x1FFF (13 least
				// significant bits of the flag). If the 14 th bit (0x2000) is set, this means that this is the last
				// packet in the audio transmission.
				stream >> helper;
				payloadSize             = helper & 0x1FFF;
				m_audioData.isLastFrame = helper & 0x2000;

				// We don't include the size/header-field in the actual payload
				payloadBegin = stream.dataPtr();

				stream.skip(static_cast< unsigned int >(payloadSize));
				break;
		}

		if (!stream.isValid()) {
			return false;
		}


		m_audioData.payload = gsl::span< byte >(payloadBegin, static_cast< std::size_t >(payloadSize));

		if (stream.left() == 3 * sizeof(float)) {
			// If there are further bytes after the audio payload, this means that there is positional data attached to
			// the packet.
			m_audioData.containsPositionalData = true;
			for (unsigned int i = 0; i < 3; ++i) {
				stream >> m_audioData.position[i];
			}
		} else if (stream.left() > 0) {
			// The remaining data does not fit the size of positional data -> seems like a invalid package format
			return false;
		}

		// Legacy audio packets don't contain volume adjustments

		return true;
	}

	template< Role role > bool UDPDecoder< role >::decodeAudio_protobuf(const gsl::span< const byte > data) {
		m_messageType = UDPMessageType::Audio;
		m_audioData   = {};

		if (!m_audioMessage.ParseFromArray(data.data(), static_cast< int >(data.size()))) {
			// Invalid format
			return false;
		}

		m_audioData.targetOrContext =
			this->getRole() == Role::Client ? m_audioMessage.context() : m_audioMessage.target();
		// Atm the only codec supported by the new package format is Opus
		m_audioData.usedCodec     = AudioCodec::Opus;
		m_audioData.senderSession = m_audioMessage.sender_session();
		m_audioData.frameNumber   = m_audioMessage.frame_number();
		if (m_audioMessage.opus_data().empty()) {
			// Audio packets without audio data are invalid
			return false;
		}

		std::string &audioPayload = *m_audioMessage.mutable_opus_data();
		m_audioData.payload = gsl::span< byte >(reinterpret_cast< byte * >(&audioPayload[0]), audioPayload.size());

		m_audioData.isLastFrame = m_audioMessage.is_terminator();

		if (m_audioMessage.positional_data_size() != 0) {
			if (m_audioMessage.positional_data_size() != 3) {
				// We always expect a 3D position, if positional data is present
				return false;
			}
			for (unsigned int i = 0; i < 3; ++i) {
				m_audioData.position[i] = m_audioMessage.positional_data(static_cast< int >(i));
			}

			m_audioData.containsPositionalData = true;
		}

		m_audioData.volumeAdjustment = VolumeAdjustment::fromFactor(m_audioMessage.volume_adjustment());
		if (m_audioData.volumeAdjustment.factor == 0.0f) {
			// No volume adjustment was set, reset to default
			m_audioData.volumeAdjustment = VolumeAdjustment::fromFactor(1.0f);
		}

		return true;
	}


	bool operator==(const AudioData &lhs, const AudioData &rhs) {
		if (lhs.isLastFrame == rhs.isLastFrame && lhs.containsPositionalData == rhs.containsPositionalData
			&& lhs.targetOrContext == rhs.targetOrContext && lhs.usedCodec == rhs.usedCodec
			&& lhs.senderSession == rhs.senderSession && lhs.frameNumber == rhs.frameNumber
			&& lhs.payload.size() == rhs.payload.size() && (!lhs.containsPositionalData || lhs.position == rhs.position)
			&& lhs.volumeAdjustment == rhs.volumeAdjustment) {
			// Compare payload
			return std::memcmp(lhs.payload.data(), rhs.payload.data(), lhs.payload.size()) == 0;
		} else {
			return false;
		}
	}

	bool operator!=(const AudioData &lhs, const AudioData &rhs) { return !(lhs == rhs); }

	bool operator==(const PingData &lhs, const PingData &rhs) {
		return lhs.timestamp == rhs.timestamp && lhs.requestAdditionalInformation == rhs.requestAdditionalInformation
			   && lhs.containsAdditionalInformation == rhs.containsAdditionalInformation
			   && lhs.serverVersion == rhs.serverVersion && lhs.userCount == rhs.userCount
			   && lhs.maxUserCount == rhs.maxUserCount && lhs.maxBandwidthPerUser == rhs.maxBandwidthPerUser;
	}

	bool operator!=(const PingData &lhs, const PingData &rhs) { return !(lhs == rhs); }

	// Explicit template instantiation of our classes. We require once instantiation for every available Role.
#define ALL_CLASSES                \
	PROCESS_CLASS(ProtocolHandler) \
	PROCESS_CLASS(UDPAudioEncoder) \
	PROCESS_CLASS(UDPPingEncoder)  \
	PROCESS_CLASS(UDPDecoder)

#define PROCESS_CLASS(className)              \
	template class className< Role::Client >; \
	template class className< Role::Server >;

	ALL_CLASSES

#undef ALL_CLASSES
#undef PROCESS_CLASS

} // namespace Protocol
} // namespace Mumble
