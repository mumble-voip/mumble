#include <benchmark/benchmark.h>

#include "AudioReceiverBuffer.h"
#include "MumbleProtocol.h"

#include <algorithm>
#include <random>
#include <vector>

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution< unsigned int > random_context(Mumble::Protocol::AudioContext::begin,
															 Mumble::Protocol::AudioContext::end);
std::uniform_int_distribution< int > random_volume_adjustment(-60, 30 + 1);
std::uniform_int_distribution< unsigned int > random_version(Version::toRaw(1, 2, 0), Version::toRaw(1, 6, 0));

std::vector< Mumble::Protocol::audio_context_t > contexts;
std::vector< VolumeAdjustment > volumeAdjustments;
std::vector< ServerUser > users;

constexpr const std::size_t ReceiverCountRange = 0;
constexpr const std::size_t DuplicateRange     = 1;

constexpr int Multiplier         = 2;
constexpr int ReceiverCountBegin = 1;
constexpr int ReceiverCountEnd   = 512;

struct ReceiverData {
	ServerUser *receiver;
	Mumble::Protocol::audio_context_t context;
	bool containsPositionalData;
	VolumeAdjustment volumeAdjustment;
};

std::vector< ReceiverData > selectedData;

void globalInit() {
	for (int i = 0; i <= ReceiverCountEnd; ++i) {
		contexts.push_back(random_context(rng));
		volumeAdjustments.push_back(VolumeAdjustment::fromDBAdjustment(random_volume_adjustment(rng)));
		users.push_back(ServerUser(i, random_version(rng)));
	}

	// add one additional user acting as the sender
	users.push_back(ServerUser(ReceiverCountEnd + 1, random_version(rng)));
}

class Fixture : public ::benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State &state) {
		selectedData.clear();

		std::size_t totalReceivers     = state.range(ReceiverCountRange);
		std::size_t duplicateReceivers = totalReceivers * (state.range(DuplicateRange) / 100.0f);

		for (std::size_t i = 0; i < totalReceivers - duplicateReceivers; ++i) {
			selectedData.push_back({ &users[i], contexts[i], false, volumeAdjustments[i] });
		}

		std::uniform_int_distribution< unsigned int > random_index(0, selectedData.size() - 1);

		for (std::size_t i = 0; i < duplicateReceivers; ++i) {
			selectedData.push_back(selectedData[random_index(rng)]);
		}

		std::random_shuffle(selectedData.begin(), selectedData.end());
	}
};

std::size_t getUniqueReceivers(std::vector< ReceiverData > &receivers) {
	ServerUser sender = users[users.size() - 1];
	AudioReceiverBuffer buffer;

	for (std::size_t i = 0; i < receivers.size(); ++i) {
		ReceiverData &data = receivers[i];

		buffer.addReceiver(sender, *data.receiver, data.context, data.containsPositionalData, data.volumeAdjustment);
	}

	return buffer.getReceivers(false).size();
}

BENCHMARK_DEFINE_F(Fixture, BM_addReceiver)(::benchmark::State &state) {
	AudioReceiverBuffer buffer;

	ServerUser sender = users[users.size() - 1];

	for (auto _ : state) {
		for (std::size_t i = 0; i < selectedData.size(); ++i) {
			ReceiverData &data = selectedData[i];

			buffer.addReceiver(sender, *data.receiver, data.context, data.containsPositionalData,
							   data.volumeAdjustment);
		}

		buffer.clear();
	}

	state.counters["unique receivers"] = getUniqueReceivers(selectedData);
}

BENCHMARK_REGISTER_F(Fixture, BM_addReceiver)
	->ArgsProduct({ benchmark::CreateRange(ReceiverCountBegin, ReceiverCountEnd, /*multi=*/Multiplier),
					{ 0, 10, 40, 80 } });


unsigned int dummyProcessing(const AudioReceiver &receiver) {
	return receiver.getReceiver().uiSession;
}

BENCHMARK_DEFINE_F(Fixture, BM_full)(::benchmark::State &state) {
	AudioReceiverBuffer buffer;

	ServerUser sender = users[users.size() - 1];

	for (auto _ : state) {
		for (std::size_t i = 0; i < selectedData.size(); ++i) {
			ReceiverData &data = selectedData[i];

			buffer.addReceiver(sender, *data.receiver, data.context, data.containsPositionalData,
							   data.volumeAdjustment);
		}

		buffer.preprocessBuffer();

		std::vector< AudioReceiver > &receivers = buffer.getReceivers(false);
		ReceiverRange< std::vector< AudioReceiver >::iterator > currentRange =
			AudioReceiverBuffer::getReceiverRange(receivers.begin(), receivers.end());

		while (currentRange.begin != currentRange.end) {
			for (auto it = currentRange.begin; it != currentRange.end; ++it) {
				benchmark::DoNotOptimize(dummyProcessing(*it));
			}

			// Find next range
			currentRange = AudioReceiverBuffer::getReceiverRange(currentRange.end, receivers.end());
		}

		buffer.clear();
	}

	state.counters["unique receivers"] = getUniqueReceivers(selectedData);
}

BENCHMARK_REGISTER_F(Fixture, BM_full)
	->ArgsProduct({ benchmark::CreateRange(ReceiverCountBegin, ReceiverCountEnd, /*multi=*/Multiplier),
					{ 0, 10, 40, 80 } });


int main(int argc, char **argv) {
	globalInit();

	::benchmark::Initialize(&argc, argv);
	::benchmark::RunSpecifiedBenchmarks();
}