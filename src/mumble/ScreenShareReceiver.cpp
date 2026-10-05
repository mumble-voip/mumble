// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ScreenShareReceiver.h"

#include "MumbleProtocol.h"

#include <QtCore/QTimer>

#include <algorithm>
#include <cmath>

#ifdef USE_SCREEN_SHARING
// Playout tuning. All times are in microseconds.
/// Lower and upper bound of the jitter buffer. Screen sharing favours low latency, so the upper bound is
/// kept well below what one would use for e.g. video on demand.
static constexpr qint64 MIN_PLAYOUT_DELAY = 20'000;
static constexpr qint64 MAX_PLAYOUT_DELAY = 300'000;
/// Safety margin on top of the observed delay variation.
static constexpr qint64 PLAYOUT_DELAY_MARGIN = 10'000;
/// Per-frame decay of the delay peak, so that the buffer shrinks again over a few seconds once the network
/// calms down.
static constexpr double PEAK_DELAY_DECAY = 0.98;
/// How much the delay peak may grow per frame. Persistently higher jitter is still picked up within a few
/// frames, but a one-off stall (whose frames are late no matter what) does not blow up the buffer and with it
/// the latency for the following seconds.
static constexpr double PEAK_DELAY_MAX_STEP = 10'000;
/// Length of the windows over which the minimum transit time is tracked.
static constexpr qint64 TRANSIT_WINDOW = 5'000'000;
/// A frame that is this much older than the last one means the sender started a new stream.
static constexpr qint64 STREAM_RESTART_THRESHOLD = 1'000'000;
static constexpr quint64 STREAM_RESTART_FRAMES   = 64;
/// Bounds for the amount of buffered data per sender.
static constexpr std::size_t MAX_PENDING_FRAMES = 60;
static constexpr std::size_t MAX_DECODED_FRAMES = 30;
/// How long to wait for a requested key frame before asking again. The key frame itself may get lost as well,
/// or the sender may hold the request back for a moment.
static constexpr qint64 KEYFRAME_REQUEST_INTERVAL = 500'000;

/// Difference a - b of two points in time. Timestamps come from other clients and may be anything, so all times
/// wrap around instead of overflowing, and are only compared by the sign of their difference (serial number
/// arithmetic, as in RTP).
static qint64 timeDiff(quint64 a, quint64 b) {
	return static_cast< qint64 >(a - b);
}

/// Maps the protocol's Codec enum to the corresponding FFmpeg codec ID.
/// To add support for a new codec: add the proto enum value in MumbleUDP.proto,
/// then add a case here returning the appropriate AV_CODEC_ID_*.
static AVCodecID codecIdForProtoCodec(MumbleUDP::Video::Codec c) {
	switch (c) {
		case MumbleUDP::Video::H264:
			return AV_CODEC_ID_H264;
		default:
			return AV_CODEC_ID_NONE;
	}
}
#endif

std::vector< MumbleUDP::Video::Codec > ScreenShareReceiver::supportedCodecs() {
	std::vector< MumbleUDP::Video::Codec > codecs;
#ifdef USE_SCREEN_SHARING
	for (MumbleUDP::Video::Codec codec : { MumbleUDP::Video::H264 }) {
		const AVCodec *decoder = avcodec_find_decoder(codecIdForProtoCodec(codec));
		if (!decoder)
			continue;

		// Make sure that the decoder can actually be opened
		AVCodecContext *ctx = avcodec_alloc_context3(decoder);
		if (ctx && avcodec_open2(ctx, decoder, nullptr) == 0)
			codecs.push_back(codec);
		avcodec_free_context(&ctx);
	}
#endif
	return codecs;
}

ScreenShareReceiver::ScreenShareReceiver(QObject *parent) : QObject(parent) {
#ifdef USE_SCREEN_SHARING
	m_clock.start();

	// Parented, so that it moves to the receiver's thread together with the receiver.
	m_timer = new QTimer(this);
	m_timer->setSingleShot(true);
	m_timer->setTimerType(Qt::PreciseTimer);
	connect(m_timer, &QTimer::timeout, this, &ScreenShareReceiver::onTimer);
#endif
}

ScreenShareReceiver::~ScreenShareReceiver() {
#ifdef USE_SCREEN_SHARING
	// Tear down all decoders.
	for (std::pair< const unsigned int, DecoderState > &kv : m_decoders) {
		DecoderState &ds = kv.second;
		if (ds.swsCtx) {
			sws_freeContext(ds.swsCtx);
		}
		if (ds.frame) {
			av_frame_free(&ds.frame);
		}
		if (ds.packet) {
			av_packet_free(&ds.packet);
		}
		if (ds.codecCtx) {
			avcodec_free_context(&ds.codecCtx);
		}
	}
#endif
}


void ScreenShareReceiver::handleVideoPacket(const Mumble::Protocol::VideoData &videoData) {
#ifndef USE_SCREEN_SHARING
	Q_UNUSED(videoData);
#else
	// videoData.payload points into the caller's network buffer, so take a copy before handing it over.
	VideoPacket packet;
	packet.senderSession = videoData.senderSession;
	packet.codec         = videoData.codec;
	packet.width         = videoData.width;
	packet.height        = videoData.height;
	packet.frameNumber   = videoData.frameNumber;
	packet.fragmentIndex = videoData.fragmentIndex;
	packet.fragmentCount = videoData.fragmentCount;
	packet.payload       = QByteArray(reinterpret_cast< const char * >(videoData.payload.data()),
                                static_cast< qsizetype >(videoData.payload.size()));
	packet.isKeyFrame    = videoData.isKeyFrame;
	packet.timestamp     = videoData.timestamp;

	QMetaObject::invokeMethod(
		this, [this, packet = std::move(packet)]() { processPacket(packet); }, Qt::QueuedConnection);
#endif
}

void ScreenShareReceiver::resetSender(quint32 senderSession) {
#ifdef USE_SCREEN_SHARING
	QMetaObject::invokeMethod(
		this, [this, senderSession]() { processReset(senderSession); }, Qt::QueuedConnection);
#else
	Q_UNUSED(senderSession);
#endif
}

#ifdef USE_SCREEN_SHARING
void ScreenShareReceiver::processPacket(const VideoPacket &videoData) {
	const quint32 session   = videoData.senderSession;
	const quint64 frameNum  = videoData.frameNumber;
	const quint32 fragIdx   = videoData.fragmentIndex;
	const quint32 fragCount = videoData.fragmentCount;

	if (fragCount == 0 || fragIdx >= fragCount)
		return;

	SenderState &sender = m_senders[session];

	if (sender.hasNextFrame && frameNum < sender.nextFrame) {
		const bool restarted = sender.nextFrame - frameNum > STREAM_RESTART_FRAMES
							   || timeDiff(sender.lastTimestamp, videoData.timestamp) > STREAM_RESTART_THRESHOLD;
		if (!restarted) {
			// Duplicate or straggler of a frame that was already decoded or given up on.
			return;
		}

		// The sender started over (frame numbers and timestamps begin at zero again).
		processReset(session);
		processPacket(videoData);
		return;
	}

	// Bound memory use under heavy loss, when frames keep arriving incomplete. The oldest frame is the one
	// least likely to still be completed in time.
	if (sender.pending.size() >= MAX_PENDING_FRAMES && !sender.pending.count(frameNum))
		sender.pending.erase(sender.pending.begin());

	PendingFrame &pf = sender.pending[frameNum];

	// Initialize frame on first fragment
	if (pf.fragmentCount == 0) {
		pf.fragmentCount = fragCount;
		pf.fragments.resize(fragCount);
		pf.width     = videoData.width;
		pf.height    = videoData.height;
		pf.codec     = videoData.codec;
		pf.timestamp = videoData.timestamp;
	} else if (pf.fragmentCount != fragCount) {
		// Inconsistent with the fragments seen so far
		return;
	}

	// OR keyframe flag (UDP fragments may arrive out of order)
	pf.isKeyFrame |= videoData.isKeyFrame;

	// Store fragment (duplicates are ignored)
	if (!pf.fragments[fragIdx].isEmpty())
		return;
	pf.fragments[fragIdx] = videoData.payload;
	++pf.receivedCount;

	if (pf.isComplete()) {
		sender.clock.update(pf.timestamp, now());

		if (!sender.hasNextFrame) {
			// First complete frame of this stream. Start at the oldest frame seen so far rather than at this one,
			// as a large key frame may well complete after the frame following it. Older frames that never
			// complete are given up on like any other lost frame. The decoder discards everything up to the first
			// keyframe anyway.
			sender.hasNextFrame = true;
			sender.nextFrame    = sender.pending.begin()->first;
		}
	}

	processSender(session, sender);
	scheduleTimer();
}

void ScreenShareReceiver::processReset(quint32 senderSession) {
	m_senders.erase(senderSession);
	destroyDecoder(senderSession);
	scheduleTimer();
}

qint64 ScreenShareReceiver::now() const {
	return m_clock.nsecsElapsed() / 1000;
}

void ScreenShareReceiver::processSender(quint32 session, SenderState &sender) {
	const quint64 currentTime = static_cast< quint64 >(now());

	// Decode in frame order for as long as the next frame is complete. Frames have to be decoded even if they
	// end up never being shown, since later frames reference them.
	while (sender.hasNextFrame) {
		auto head = sender.pending.find(sender.nextFrame);
		if (head != sender.pending.end() && head->second.isComplete()) {
			decodeCompleteFrame(session, sender, head->second);
			sender.lastTimestamp = head->second.timestamp;
			sender.pending.erase(head);
			++sender.nextFrame;
			continue;
		}

		// The next frame is missing or incomplete. Wait for it until a later, complete frame is due for
		// display (or too much has piled up); at that point it is considered lost.
		auto firstComplete = std::find_if(
			sender.pending.upper_bound(sender.nextFrame), sender.pending.end(),
			[](const std::pair< const quint64, PendingFrame > &entry) { return entry.second.isComplete(); });
		if (firstComplete == sender.pending.end())
			break;

		const bool overdue = timeDiff(sender.clock.displayTime(firstComplete->second.timestamp), currentTime) <= 0;
		if (!overdue && sender.pending.size() < MAX_PENDING_FRAMES)
			break;

		sender.nextFrame = firstComplete->first;
		sender.pending.erase(sender.pending.begin(), firstComplete);

		// Later frames depend on the lost one, so decoding them would only produce corrupted pictures.
		// Resume at the next keyframe instead.
		auto decoder = m_decoders.find(session);
		if (decoder != m_decoders.end())
			decoder->second.gotKeyFrame = false;
	}

	// Show the newest frame that is due. Older due frames were not shown in time and are skipped, which is
	// how playback catches up after a stall.
	auto due = std::find_if(sender.decoded.rbegin(), sender.decoded.rend(), [currentTime](const DecodedFrame &frame) {
		return timeDiff(frame.displayTime, currentTime) <= 0;
	});
	if (due != sender.decoded.rend()) {
		const QImage image = due->image;
		sender.decoded.erase(sender.decoded.begin(), due.base());
		emit frameDecoded(session, image);
	}
}

void ScreenShareReceiver::requestKeyFrame(quint32 session, SenderState &sender) {
	const qint64 currentTime = now();
	if (sender.lastKeyFrameRequest >= 0 && currentTime - sender.lastKeyFrameRequest < KEYFRAME_REQUEST_INTERVAL)
		return;

	sender.lastKeyFrameRequest = currentTime;
	emit keyFrameNeeded(session);
}

void ScreenShareReceiver::onTimer() {
	for (std::pair< const quint32, SenderState > &entry : m_senders)
		processSender(entry.first, entry.second);

	scheduleTimer();
}

void ScreenShareReceiver::scheduleTimer() {
	bool hasDeadline    = false;
	quint64 deadline    = 0;
	const auto consider = [&](quint64 time) {
		if (!hasDeadline || timeDiff(time, deadline) < 0) {
			deadline    = time;
			hasDeadline = true;
		}
	};

	for (const std::pair< const quint32, SenderState > &entry : m_senders) {
		const SenderState &sender = entry.second;

		// Display times don't always increase from frame to frame, e.g. when the base transit time drops
		for (const DecodedFrame &frame : sender.decoded)
			consider(frame.displayTime);

		// Deadline for giving up on a missing frame (see processSender())
		if (sender.hasNextFrame) {
			auto head = sender.pending.find(sender.nextFrame);
			if (head == sender.pending.end() || !head->second.isComplete()) {
				for (auto it = sender.pending.upper_bound(sender.nextFrame); it != sender.pending.end(); ++it) {
					if (it->second.isComplete()) {
						consider(sender.clock.displayTime(it->second.timestamp));
						break;
					}
				}
			}
		}
	}

	if (!hasDeadline) {
		m_timer->stop();
		return;
	}

	// Deadlines are at most MAX_PLAYOUT_DELAY after the arrival of a frame. Limiting the delay to that keeps it in
	// range for timestamps that make no sense, too. Round up, so that we don't wake up just before the deadline.
	const qint64 delayUs =
		std::clamp(timeDiff(deadline, static_cast< quint64 >(now())), static_cast< qint64 >(0), MAX_PLAYOUT_DELAY);
	m_timer->start(static_cast< int >((delayUs + 999) / 1000));
}

void ScreenShareReceiver::PlayoutClock::update(quint64 timestamp, qint64 arrivalTime) {
	const quint64 transit = static_cast< quint64 >(arrivalTime) - timestamp;

	if (!m_valid) {
		m_valid          = true;
		m_windowStart    = arrivalTime;
		m_minTransit     = transit;
		m_prevMinTransit = transit;
		m_peakDelay      = 0;
	}

	if (arrivalTime - m_windowStart >= TRANSIT_WINDOW) {
		m_prevMinTransit = m_minTransit;
		m_minTransit     = transit;
		m_windowStart    = arrivalTime;
	}
	if (timeDiff(transit, m_minTransit) < 0)
		m_minTransit = transit;

	// How much later than the fastest recent frame this one arrived
	const qint64 delay = timeDiff(transit, baseTransit());
	m_peakDelay        = std::max(std::min(static_cast< double >(delay), m_peakDelay + PEAK_DELAY_MAX_STEP),
                           m_peakDelay * PEAK_DELAY_DECAY);
}

quint64 ScreenShareReceiver::PlayoutClock::baseTransit() const {
	return timeDiff(m_minTransit, m_prevMinTransit) < 0 ? m_minTransit : m_prevMinTransit;
}

quint64 ScreenShareReceiver::PlayoutClock::displayTime(quint64 timestamp) const {
	const qint64 bufferDelay = std::clamp(static_cast< qint64 >(std::lround(m_peakDelay)) + PLAYOUT_DELAY_MARGIN,
										  MIN_PLAYOUT_DELAY, MAX_PLAYOUT_DELAY);

	return timestamp + baseTransit() + static_cast< quint64 >(bufferDelay);
}

bool ScreenShareReceiver::ensureDecoder(quint32 session, MumbleUDP::Video::Codec protoCodec) {
	if (m_decoders.count(session) && m_decoders[session].codecCtx)
		return true;

	const AVCodecID avCodecId = codecIdForProtoCodec(protoCodec);
	if (avCodecId == AV_CODEC_ID_NONE)
		return false;

	const AVCodec *codec = avcodec_find_decoder(avCodecId);
	if (!codec)
		return false;

	DecoderState ds;
	ds.codec    = protoCodec;
	ds.codecCtx = avcodec_alloc_context3(codec);
	if (!ds.codecCtx)
		return false;

	if (avcodec_open2(ds.codecCtx, codec, nullptr) < 0) {
		avcodec_free_context(&ds.codecCtx);
		return false;
	}

	ds.frame            = av_frame_alloc();
	ds.packet           = av_packet_alloc();
	m_decoders[session] = ds;
	return true;
}

void ScreenShareReceiver::destroyDecoder(quint32 session) {
	auto it = m_decoders.find(session);
	if (it == m_decoders.end())
		return;

	DecoderState &ds = it->second;
	if (ds.swsCtx) {
		sws_freeContext(ds.swsCtx);
	}
	if (ds.frame) {
		av_frame_free(&ds.frame);
	}
	if (ds.packet) {
		av_packet_free(&ds.packet);
	}
	if (ds.codecCtx) {
		avcodec_free_context(&ds.codecCtx);
	}
	m_decoders.erase(it);
}

void ScreenShareReceiver::decodeCompleteFrame(quint32 session, SenderState &sender, const PendingFrame &pf) {
	if (!ensureDecoder(session, pf.codec))
		return;

	const bool isKeyFrame = pf.isKeyFrame;

	DecoderState &ds = m_decoders[session];

	// Drop non-keyframes until the decoder has seen at least one IDR.
	// Without SPS/PPS (which come with the keyframe) the decoder can't
	// reference picture parameters and emits "non-existing PPS" errors.
	// This is the case after a frame was lost, or when joining a stream that is already running. Rather than
	// waiting for the sender's next periodic key frame, ask for one right away.
	if (!ds.gotKeyFrame && !isKeyFrame) {
		requestKeyFrame(session, sender);
		return;
	}

	if (isKeyFrame) {
		// Flush any buffered decoder state from a previous stream so the new
		// IDR is treated as a clean start.
		avcodec_flush_buffers(ds.codecCtx);
		ds.gotKeyFrame = true;
	}

	QByteArray encodedData;
	for (const QByteArray &fragment : pf.fragments)
		encodedData.append(fragment);

	av_packet_unref(ds.packet);
	ds.packet->data = reinterpret_cast< uint8_t * >(const_cast< char * >(encodedData.constData()));
	ds.packet->size = static_cast< int >(encodedData.size());
	// The decoder hands the timestamp back on the decoded picture, which also covers decoders that delay or
	// reorder frames.
	ds.packet->pts = static_cast< int64_t >(pf.timestamp);

	if (avcodec_send_packet(ds.codecCtx, ds.packet) < 0) {
		// Packet was rejected (e.g. corrupted reference frame from UDP loss).
		// Flush and wait for the next keyframe so we don't propagate corruption.
		avcodec_flush_buffers(ds.codecCtx);
		ds.gotKeyFrame = false;
		requestKeyFrame(session, sender);
		return;
	}

	while (avcodec_receive_frame(ds.codecCtx, ds.frame) == 0) {
		const int dw = ds.frame->width;
		const int dh = ds.frame->height;

		// (Re-)create the sws context if dimensions changed.
		if (!ds.swsCtx || ds.swsWidth != dw || ds.swsHeight != dh) {
			if (ds.swsCtx)
				sws_freeContext(ds.swsCtx);
			ds.swsCtx = sws_getContext(dw, dh, static_cast< AVPixelFormat >(ds.frame->format), dw, dh, AV_PIX_FMT_RGBA,
									   SWS_BILINEAR, nullptr, nullptr, nullptr);
			ds.swsWidth  = dw;
			ds.swsHeight = dh;
		}
		if (!ds.swsCtx)
			continue;

		QImage img(dw, dh, QImage::Format_RGBA8888);
		uint8_t *dstData[1] = { img.bits() };
		int dstStride[1]    = { static_cast< int >(img.bytesPerLine()) };
		sws_scale(ds.swsCtx, ds.frame->data, ds.frame->linesize, 0, dh, dstData, dstStride);

		const quint64 timestamp = ds.frame->best_effort_timestamp != AV_NOPTS_VALUE
									  ? static_cast< quint64 >(ds.frame->best_effort_timestamp)
									  : pf.timestamp;

		sender.decoded.push_back({ sender.clock.displayTime(timestamp), img });
		if (sender.decoded.size() > MAX_DECODED_FRAMES)
			sender.decoded.pop_front();
	}
}
#endif
