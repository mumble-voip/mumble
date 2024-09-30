// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioReceiverBuffer.h"

#include <algorithm>
#include <cassert>

#include <tracy/Tracy.hpp>

AudioReceiver::AudioReceiver(ServerUser &receiver, Mumble::Protocol::audio_context_t context,
							 const VolumeAdjustment &volumeAdjustment)
	: m_receiver(receiver), m_context(context), m_volumeAdjustment(volumeAdjustment) {
}

AudioReceiver::AudioReceiver(ServerUser &receiver, Mumble::Protocol::audio_context_t context,
							 VolumeAdjustment &&volumeAdjustment)
	: m_receiver(receiver), m_context(context), m_volumeAdjustment(std::move(volumeAdjustment)) {
}

ServerUser &AudioReceiver::getReceiver() {
	return m_receiver;
}

const ServerUser &AudioReceiver::getReceiver() const {
	return m_receiver;
}

Mumble::Protocol::audio_context_t AudioReceiver::getContext() const {
	return m_context;
}

void AudioReceiver::setContext(Mumble::Protocol::audio_context_t context) {
	m_context = context;
}

const VolumeAdjustment &AudioReceiver::getVolumeAdjustment() const {
	return m_volumeAdjustment;
}

void AudioReceiver::setVolumeAdjustment(const VolumeAdjustment &adjustment) {
	m_volumeAdjustment = adjustment;
}

void AudioReceiver::setVolumeAdjustment(VolumeAdjustment &&adjustment) {
	m_volumeAdjustment = std::move(adjustment);
}

AudioReceiverBuffer::AudioReceiverBuffer() {
	// These are just educated guesses at reasonable starting capacities for these vectors
	m_regularReceivers.reserve(50);
	m_positionalReceivers.reserve(10);
}

void AudioReceiverBuffer::addReceiver(const ServerUser &sender, ServerUser &receiver,
									  Mumble::Protocol::audio_context_t context, bool positionalDataAvailable,
									  const VolumeAdjustment &volumeAdjustment) {
	if (sender.uiSession == receiver.uiSession || receiver.bDeaf || receiver.bSelfDeaf) {
		return;
	}

	forceAddReceiver(receiver, context, positionalDataAvailable && sender.ssContext == receiver.ssContext,
					 volumeAdjustment);
}

void AudioReceiverBuffer::forceAddReceiver(ServerUser &receiver, Mumble::Protocol::audio_context_t context,
										   bool includePositionalData, const VolumeAdjustment &volumeAdjustment) {
	ZoneScoped;

	std::vector< AudioReceiver > &receiverList = includePositionalData ? m_positionalReceivers : m_regularReceivers;
	std::unordered_map< const ServerUser *, std::size_t > &userEntryIndices =
		includePositionalData ? m_positionalReceiverIndices : m_regularReceiverIndices;

	auto it = userEntryIndices.find(&receiver);
	if (it == userEntryIndices.end()) {
		// No entry for that user yet
		receiverList.emplace_back(receiver, context, volumeAdjustment);
		userEntryIndices[&receiver] = receiverList.size() - 1;
	} else {
		// We already have an entry for the given user -> update that instead of adding a new one
		AudioReceiver &receiverEntry = receiverList[it->second];

		assert(receiverEntry.getReceiver().uiSession == receiver.uiSession);

		receiverEntry.setContext(std::min(receiverEntry.getContext(), context));

		if (receiverEntry.getVolumeAdjustment().factor < volumeAdjustment.factor) {
			receiverEntry.setVolumeAdjustment(volumeAdjustment);
		}
	}
}

void AudioReceiverBuffer::preprocessBuffer() {
	ZoneScoped;

	preprocessBuffer(m_regularReceivers);
	preprocessBuffer(m_positionalReceivers);
}

void AudioReceiverBuffer::clear() {
	m_regularReceivers.clear();
	m_regularReceiverIndices.clear();
	m_positionalReceivers.clear();
	m_positionalReceiverIndices.clear();
}

std::vector< AudioReceiver > &AudioReceiverBuffer::getReceivers(bool receivePositionalData) {
	if (receivePositionalData) {
		return m_positionalReceivers;
	} else {
		return m_regularReceivers;
	}
}

void AudioReceiverBuffer::preprocessBuffer(std::vector< AudioReceiver > &receiverList) {
	ZoneScoped;

#ifndef NDEBUG
	// Sort the list such that entries with same receiver are next to each other
	std::sort(receiverList.begin(), receiverList.end(), [](const AudioReceiver &lhs, const AudioReceiver &rhs) {
		return lhs.getReceiver().uiSession < rhs.getReceiver().uiSession;
	});

	// Assert that our list does not contain any duplicate receivers
	assert(std::unique(receiverList.begin(), receiverList.end(),
					   [](const AudioReceiver &lhs, const AudioReceiver &rhs) {
						   return lhs.getReceiver().uiSession == rhs.getReceiver().uiSession;
					   })
		   == receiverList.end());
#endif

	// Sort the receivers, such that we can efficiently partition them into different regions
	// Note: The list doesn't contains any duplicate receivers
	std::sort(receiverList.begin(), receiverList.end(), [](const AudioReceiver &lhs, const AudioReceiver &rhs) {
		// 1. Sort into block of compatible protocol versions
		if (!Mumble::Protocol::protocolVersionsAreCompatible(lhs.getReceiver().m_version,
															 rhs.getReceiver().m_version)) {
			return lhs.getReceiver().m_version < rhs.getReceiver().m_version;
		}

		// 2. Within each block, sort based on the audio context
		if (lhs.getContext() != rhs.getContext()) {
			return lhs.getContext() < rhs.getContext();
		}

		// 3. Within each context, sort based on volume adjustments (but in descending order!)
		return lhs.getVolumeAdjustment().factor > rhs.getVolumeAdjustment().factor;
	});
}
