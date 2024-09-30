// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <benchmark/benchmark.h>

#include "MumbleProtocol.h"
#include "PacketDataStream.h"

#include <limits>
#include <random>
#include <vector>

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution< unsigned int > random_integer(1, 1024);
std::uniform_int_distribution< unsigned int > random_byte(0, std::numeric_limits< Mumble::Protocol::byte >::max());

constexpr int PAYLOAD_SIZE_RANGE = 0;

constexpr int FROM_PAYLOAD_SIZE       = 0;
constexpr int TO_PAYLOAD_SIZE         = 900;
constexpr int PAYLOAD_SIZE_MULTIPLIER = 2;

std::vector< Mumble::Protocol::byte > audioPayload;
Mumble::Protocol::AudioData audioData;

Mumble::Protocol::UDPAudioEncoder< Mumble::Protocol::Role::Server > encoder;

class Fixture : public ::benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State &state) {
		audioPayload.resize(state.range(PAYLOAD_SIZE_RANGE));

		for (std::size_t i = 0; i < audioPayload.size(); ++i) {
			audioPayload[i] = random_byte(rng);
		}

		audioData.payload                = { audioPayload.data(), audioPayload.size() };
		audioData.frameNumber            = 42;
		audioData.isLastFrame            = false;
		audioData.senderSession          = 137;
		audioData.targetOrContext        = Mumble::Protocol::AudioContext::NORMAL;
		audioData.usedCodec              = Mumble::Protocol::AudioCodec::Opus;
		audioData.position               = { 1.25f, 1260.539f, -3.0765f };
		audioData.containsPositionalData = true;

		encoder.setProtocolVersion(Version::fromComponents(1, 3, 0));
		encoder.encodeAudioPacket(audioData);
		encoder.setProtocolVersion(Mumble::Protocol::PROTOBUF_INTRODUCTION_VERSION);
		encoder.encodeAudioPacket(audioData);
	}
};

BENCHMARK_DEFINE_F(Fixture, BM_encodeLegacyDirect)(::benchmark::State &state) {
	std::vector< Mumble::Protocol::byte > buffer;
	buffer.resize(Mumble::Protocol::MAX_UDP_PACKET_SIZE);

	for (auto _ : state) {
		PacketDataStream stream(buffer.data() + 1, buffer.size() - 1);

		buffer[0] = (static_cast< Mumble::Protocol::byte >(audioData.usedCodec) << 5)
					| static_cast< Mumble::Protocol::byte >(audioData.targetOrContext);

		stream << audioData.senderSession;
		stream << static_cast< quint64 >(audioData.frameNumber);
		stream.append(reinterpret_cast< const char * >(audioData.payload.data()), audioData.payload.size());
		stream.append(reinterpret_cast< const char * >(&audioData.position[0]),
					  sizeof(float) * audioData.position.size());
	}
}

BENCHMARK_REGISTER_F(Fixture, BM_encodeLegacyDirect)
	->RangeMultiplier(PAYLOAD_SIZE_MULTIPLIER)
	->Range(FROM_PAYLOAD_SIZE, TO_PAYLOAD_SIZE);

BENCHMARK_DEFINE_F(Fixture, BM_encodeLegacy)(::benchmark::State &state) {
	encoder.setProtocolVersion(Version::fromComponents(1, 3, 0));

	for (auto _ : state) {
		encoder.encodeAudioPacket(audioData);
	}
}

BENCHMARK_REGISTER_F(Fixture, BM_encodeLegacy)
	->RangeMultiplier(PAYLOAD_SIZE_MULTIPLIER)
	->Range(FROM_PAYLOAD_SIZE, TO_PAYLOAD_SIZE);

BENCHMARK_DEFINE_F(Fixture, BM_encodeLegacy_UpdateOnly)(::benchmark::State &state) {
	encoder.setProtocolVersion(Version::fromComponents(1, 3, 0));

	encoder.prepareAudioPacket(audioData);

	for (auto _ : state) {
		encoder.updateAudioPacket(audioData);
	}
}

BENCHMARK_REGISTER_F(Fixture, BM_encodeLegacy_UpdateOnly)
	->RangeMultiplier(PAYLOAD_SIZE_MULTIPLIER)
	->Range(FROM_PAYLOAD_SIZE, TO_PAYLOAD_SIZE);

BENCHMARK_DEFINE_F(Fixture, BM_encodeNew)(::benchmark::State &state) {
	encoder.setProtocolVersion(Mumble::Protocol::PROTOBUF_INTRODUCTION_VERSION);

	for (auto _ : state) {
		encoder.encodeAudioPacket(audioData);
	}
}

BENCHMARK_REGISTER_F(Fixture, BM_encodeNew)
	->RangeMultiplier(PAYLOAD_SIZE_MULTIPLIER)
	->Range(FROM_PAYLOAD_SIZE, TO_PAYLOAD_SIZE);

BENCHMARK_DEFINE_F(Fixture, BM_encodeNew_UpdateOnly)(::benchmark::State &state) {
	encoder.setProtocolVersion(Mumble::Protocol::PROTOBUF_INTRODUCTION_VERSION);

	encoder.prepareAudioPacket(audioData);

	for (auto _ : state) {
		encoder.updateAudioPacket(audioData);
	}
}

BENCHMARK_REGISTER_F(Fixture, BM_encodeNew_UpdateOnly)
	->RangeMultiplier(PAYLOAD_SIZE_MULTIPLIER)
	->Range(FROM_PAYLOAD_SIZE, TO_PAYLOAD_SIZE);


BENCHMARK_MAIN();
