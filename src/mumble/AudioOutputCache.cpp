// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioOutputCache.h"

#include <cassert>
#include <cstring>

AudioOutputCache::AudioOutputCache(std::size_t initialCapacity) {
	m_audioData.reserve(initialCapacity);
}

gsl::span< const Mumble::Protocol::byte > AudioOutputCache::getAudioData() const {
	return m_audioData;
}

bool AudioOutputCache::isLastFrame() const {
	return m_isLastFrame;
}

float AudioOutputCache::getVolumeAdjustment() const {
	return m_volumeAdjustment;
}

bool AudioOutputCache::containsPositionalInformation() const {
	return m_containsPosition;
}

const std::array< float, 3 > &AudioOutputCache::getPositionalInformation() const {
	assert(m_containsPosition);

	return m_position;
}

Mumble::Protocol::audio_context_t AudioOutputCache::getContext() const {
	return m_audioContext;
}

void AudioOutputCache::setCapacity(std::size_t capacity) {
	m_audioData.reserve(capacity);
}

void AudioOutputCache::loadFrom(const Mumble::Protocol::AudioData &audioData) {
	// First copy audio data
	m_audioData.clear();
	m_audioData.resize(audioData.payload.size());
	std::memcpy(m_audioData.data(), audioData.payload.data(), audioData.payload.size());

	// Then copy remaining fields (that we care about)
	m_isLastFrame      = audioData.isLastFrame;
	m_volumeAdjustment = audioData.volumeAdjustment.factor;
	m_audioContext     = static_cast< Mumble::Protocol::audio_context_t >(audioData.targetOrContext);

	// And finally copy positional data, if available
	if (audioData.containsPositionalData) {
		m_containsPosition = true;

		assert(m_position.size() == 3);
		assert(audioData.position.size() == 3);

		for (unsigned int i = 0; i < 3; ++i) {
			m_position[i] = audioData.position[i];
		}
	} else {
		m_containsPosition = false;
	}
}

void AudioOutputCache::clear() {
	m_audioData.clear();
}

bool AudioOutputCache::isValid() const {
	return !m_audioData.empty();
}

AudioOutputCache::operator bool() const {
	return isValid();
}
