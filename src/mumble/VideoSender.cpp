// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VideoSender.h"

#include "MumbleProtocol.h"
#include "MumbleUDP.pb.h"
#include "ServerHandler.h"
#include "VideoEncoder.h"
#include "Global.h"

#include <QtCore/QTimer>

#include <algorithm>

/// 900 is a bit of a hardcoded arbitrary value. But it seems like a safe value for most MTU.
static constexpr int MAX_FRAGMENT_BYTES = 900;

/// Pacing rate relative to the encoder's target bit rate. Regular frames are well below the average size, so
/// they still go out right away, while a key frame takes a few hundred milliseconds.
static constexpr double PACING_FACTOR       = 2.5;
static constexpr double PACING_BYTES_PER_US = PACING_FACTOR * VideoEncoder::BITRATE / 8 / 1'000'000;
/// Upper limit for how long fragments may wait in the queue. When more is queued than can be sent within that
/// time at the pacing rate, the rate is raised accordingly.
static constexpr qint64 MAX_QUEUE_DELAY_US = 250'000;
/// How much may be sent at once after a pause, as time at the pacing rate. This also bounds the burst after the
/// sender's thread was held up for a moment.
static constexpr qint64 MAX_BURST_US    = 10'000;
static constexpr int PACING_INTERVAL_MS = 5;

VideoSender::VideoSender(QObject *parent) : QObject(parent) {
	m_clock.start();

	// The timer is a child, so it moves to the sender's thread together with it.
	m_timer = new QTimer(this);
	m_timer->setInterval(PACING_INTERVAL_MS);
	m_timer->setTimerType(Qt::PreciseTimer);
	connect(m_timer, &QTimer::timeout, this, &VideoSender::sendDue);
}

VideoSender::~VideoSender() = default;

void VideoSender::sendFrame(const EncodedVideoFrame &frame) {
	QMetaObject::invokeMethod(
		this, [this, frame]() { processFrame(frame); }, Qt::QueuedConnection);
}

void VideoSender::reset() {
	QMetaObject::invokeMethod(
		this, [this]() { processReset(); }, Qt::QueuedConnection);
}

qint64 VideoSender::now() const {
	return m_clock.nsecsElapsed() / 1000;
}

void VideoSender::processFrame(const EncodedVideoFrame &frame) {
	const quint32 session = Global::get().uiSession;
	if (session == 0 || frame.data.isEmpty())
		return;

	// Fragment the encoded frame into UDP-safe chunks, each sent as a MumbleUDP::Video message.
	const int dataSize      = static_cast< int >(frame.data.size());
	const int fragmentCount = (dataSize + MAX_FRAGMENT_BYTES - 1) / MAX_FRAGMENT_BYTES;

	for (int i = 0; i < fragmentCount; ++i) {
		const int offset    = i * MAX_FRAGMENT_BYTES;
		const int chunkSize = std::min(MAX_FRAGMENT_BYTES, dataSize - offset);

		MumbleUDP::Video videoMsg;
		videoMsg.set_sender_session(session);
		videoMsg.set_codec(frame.codec);
		videoMsg.set_width(frame.width);
		videoMsg.set_height(frame.height);
		videoMsg.set_frame_number(frame.frameNumber);
		videoMsg.set_fragment_index(static_cast< std::uint32_t >(i));
		videoMsg.set_fragment_count(static_cast< std::uint32_t >(fragmentCount));
		videoMsg.set_video_data(frame.data.constData() + offset, static_cast< std::size_t >(chunkSize));
		videoMsg.set_is_keyframe(frame.isKeyFrame && i == 0);
		videoMsg.set_timestamp(frame.timestamp);

		const int msgSize = static_cast< int >(videoMsg.ByteSizeLong());
		std::vector< unsigned char > packet(static_cast< std::size_t >(msgSize + 1));
		packet[0] = static_cast< unsigned char >(Mumble::Protocol::UDPMessageType::Video);
		if (!videoMsg.SerializeToArray(packet.data() + 1, msgSize))
			continue;

		m_queuedBytes += packet.size();
		m_queue.push_back(std::move(packet));
	}

	// When idle, start sending right away instead of waiting for the next tick
	if (!m_timer->isActive())
		sendDue();
}

void VideoSender::processReset() {
	m_queue.clear();
	m_queuedBytes = 0;
	m_timer->stop();
}

void VideoSender::sendDue() {
	ServerHandlerPtr sh = Global::get().sh;
	if (!sh) {
		processReset();
		return;
	}

	const qint64 currentTime = now();
	const double rate        = std::max(PACING_BYTES_PER_US, static_cast< double >(m_queuedBytes) / MAX_QUEUE_DELAY_US);
	m_budget     = std::min(m_budget + rate * static_cast< double >(currentTime - m_lastRefill), rate * MAX_BURST_US);
	m_lastRefill = currentTime;

	while (!m_queue.empty() && m_budget > 0) {
		const std::vector< unsigned char > &packet = m_queue.front();
		sh->sendMessage(packet.data(), static_cast< int >(packet.size()));

		m_budget -= static_cast< double >(packet.size());
		m_queuedBytes -= packet.size();
		m_queue.pop_front();
	}

	if (m_queue.empty())
		m_timer->stop();
	else if (!m_timer->isActive())
		m_timer->start();
}
