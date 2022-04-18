// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_AUDIORECEIVERBUFFER_H_
#define MUMBLE_MURMUR_AUDIORECEIVERBUFFER_H_

#include "MumbleProtocol.h"
#include "ServerUser.h"
#include "VolumeAdjustment.h"

#include <functional>
#include <unordered_map>
#include <vector>

#include <Tracy.hpp>

class AudioReceiver {
public:
	AudioReceiver(ServerUser &receiver, Mumble::Protocol::audio_context_t context,
				  const VolumeAdjustment &volumeAdjustment);
	AudioReceiver(ServerUser &receiver, Mumble::Protocol::audio_context_t context, VolumeAdjustment &&volumeAdjustment);

	ServerUser &getReceiver();
	const ServerUser &getReceiver() const;

	Mumble::Protocol::audio_context_t getContext() const;
	void setContext(Mumble::Protocol::audio_context_t context);

	const VolumeAdjustment &getVolumeAdjustment() const;
	void setVolumeAdjustment(const VolumeAdjustment &adjustment);
	void setVolumeAdjustment(VolumeAdjustment &&adjustment);

protected:
	std::reference_wrapper< ServerUser > m_receiver;
	Mumble::Protocol::audio_context_t m_context = Mumble::Protocol::AudioContext::INVALID;
	VolumeAdjustment m_volumeAdjustment         = VolumeAdjustment::fromFactor(1.0f);
};


template< typename Iterator > struct ReceiverRange {
	Iterator begin;
	Iterator end;
};


class AudioReceiverBuffer {
public:
	AudioReceiverBuffer();

	void addReceiver(const ServerUser &sender, ServerUser &receiver, Mumble::Protocol::audio_context_t context,
					 bool includePositionalData,
					 const VolumeAdjustment &volumeAdjustment = VolumeAdjustment::fromFactor(1.0f));
	void forceAddReceiver(ServerUser &receiver, Mumble::Protocol::audio_context_t context, bool includePositionalData,
						  const VolumeAdjustment &volumeAdjustment = VolumeAdjustment::fromFactor(1.0f));

	void preprocessBuffer();

	void clear();

	std::vector< AudioReceiver > &getReceivers(bool receivePositionalData);


	template< typename Iterator > static ReceiverRange< Iterator > getReceiverRange(Iterator begin, Iterator end) {
		ZoneScoped;

		ReceiverRange< Iterator > range;
		range.begin = begin;

		if (begin == end) {
			// In this case, it is invalid to dereference begin (as is done further down) and thus we have to
			// return early instead.
			range.end = end;

			return range;
		}

		// Find a range, such that all receivers in [begin, end) are compatible in the sense that they will all receive
		// the exact same audio packet (thus: no re-encoding required between sending the packet to them).
		range.end = std::lower_bound(begin, end, *begin, [](const AudioReceiver &lhs, const AudioReceiver &rhs) {
			return lhs.getContext() == rhs.getContext()
				   && Mumble::Protocol::protocolVersionsAreCompatible(lhs.getReceiver().uiVersion,
																	  rhs.getReceiver().uiVersion)
				   // Allow a little variance between volume adjustments
				   && std::abs(lhs.getVolumeAdjustment().factor - rhs.getVolumeAdjustment().factor) < 0.05f;
		});

		return range;
	}

protected:
	std::vector< AudioReceiver > m_regularReceivers;
	std::unordered_map< const ServerUser *, std::size_t > m_regularReceiverIndices;
	std::vector< AudioReceiver > m_positionalReceivers;
	std::unordered_map< const ServerUser *, std::size_t > m_positionalReceiverIndices;

	void preprocessBuffer(std::vector< AudioReceiver > &receiverList);
};

#endif // MUMBLE_MURMUR_AUDIORECEIVERBUFFER_H_
