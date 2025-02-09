// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOCACHE_H_
#define MUMBLE_MUMBLE_AUDIOCACHE_H_

#include "MumbleProtocol.h"

#include <array>
#include <vector>

#include <gsl/span>

class AudioOutputCache {
public:
	AudioOutputCache(std::size_t initialCapacity = 512);
	AudioOutputCache(AudioOutputCache &&) = default;

	gsl::span< const Mumble::Protocol::byte > getAudioData() const;
	bool isLastFrame() const;

	float getVolumeAdjustment() const;

	bool containsPositionalInformation() const;
	const std::array< float, 3 > &getPositionalInformation() const;

	Mumble::Protocol::audio_context_t getContext() const;

	void setCapacity(std::size_t capacity);

	void loadFrom(const Mumble::Protocol::AudioData &audioData);

	void clear();

	bool isValid() const;

	operator bool() const;

private:
	std::vector< Mumble::Protocol::byte > m_audioData;
	bool m_isLastFrame                               = false;
	Mumble::Protocol::audio_context_t m_audioContext = Mumble::Protocol::AudioContext::INVALID;
	float m_volumeAdjustment                         = 1.0f;
	bool m_containsPosition                          = false;
	std::array< float, 3 > m_position;
};

#endif // MUMBLE_MUMBLE_AUDIOCACHE_H_
