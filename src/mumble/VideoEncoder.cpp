// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VideoEncoder.h"

#include "Log.h"
#include "Global.h"

#include <QtCore/QMutexLocker>
#include <QtCore/QTimer>

#include <algorithm>

/// Viewers request a key frame whenever they need one (on joining, after a loss), so periodic key frames are only
/// a safety net. Each key frame is much larger than other frames and delays the frames behind it, which viewers
/// see as a stutter.
static constexpr int KEY_FRAME_INTERVAL_S = 60;
/// When the screen doesn't change, native capture streams stop delivering frames. The last frame is encoded again
/// after this long (which costs next to nothing, as nothing changed), so that viewers notice the loss of the frame
/// before the pause and request a key frame.
static constexpr int HEARTBEAT_INTERVAL_MS = 1'000;
/// Minimum time between a key frame and one sent on request. Viewers tend to lose the same packets and each of
/// them asks for a key frame, so this keeps a single loss from causing a burst of key frames.
static constexpr qint64 MIN_KEYFRAME_REQUEST_INTERVAL_US = 500'000;

VideoEncoder::VideoEncoder(QObject *parent) : QObject(parent) {
	// Both timers are children, so they move to the encoder's thread together with it.
	m_frameRateTimer = new QTimer(this);
	m_frameRateTimer->setSingleShot(true);
	m_frameRateTimer->setTimerType(Qt::PreciseTimer);
	connect(m_frameRateTimer, &QTimer::timeout, this, &VideoEncoder::processIncomingFrame);

	m_keyFrameTimer = new QTimer(this);
	m_keyFrameTimer->setSingleShot(true);
	connect(m_keyFrameTimer, &QTimer::timeout, this, &VideoEncoder::forceKeyFrame);

	m_heartbeatTimer = new QTimer(this);
	m_heartbeatTimer->setSingleShot(true);
	m_heartbeatTimer->setInterval(HEARTBEAT_INTERVAL_MS);
	connect(m_heartbeatTimer, &QTimer::timeout, this, &VideoEncoder::sendHeartbeat);
}

VideoEncoder::~VideoEncoder() = default;

void VideoEncoder::start(const QElapsedTimer &streamClock) {
	const quint64 stream = ++m_stream;
	QMetaObject::invokeMethod(
		this,
		[this, streamClock, stream]() {
			m_currentStream = stream;
			processStart(streamClock);
		},
		Qt::QueuedConnection);
}

void VideoEncoder::stop() {
	++m_stream;
	QMetaObject::invokeMethod(
		this, [this]() { processStop(); }, Qt::QueuedConnection);
}

void VideoEncoder::submitFrame(const QImage &image, qint64 captureTime) {
	bool wakeUp;
	{
		QMutexLocker lock(&m_incomingMutex);
		// Replaces a frame that is still waiting to be encoded
		wakeUp                = m_incomingFrame.isNull();
		m_incomingFrame       = image;
		m_incomingCaptureTime = captureTime;
	}

	if (wakeUp)
		QMetaObject::invokeMethod(
			this, [this]() { processIncomingFrame(); }, Qt::QueuedConnection);
}

void VideoEncoder::requestKeyFrame() {
	QMetaObject::invokeMethod(
		this, [this]() { processKeyFrameRequest(); }, Qt::QueuedConnection);
}

void VideoEncoder::setSelection(const VideoEncoderSelection &selection) {
	QMetaObject::invokeMethod(
		this, [this, selection]() { processSelection(selection); }, Qt::QueuedConnection);
}

void VideoEncoder::setFrameRate(int frameRate) {
	QMetaObject::invokeMethod(
		this, [this, frameRate]() { processFrameRate(frameRate); }, Qt::QueuedConnection);
}

qint64 VideoEncoder::frameInterval(int frameRate) {
	return 1'000'000 / std::clamp(frameRate, MIN_FRAME_RATE, MAX_FRAME_RATE);
}

void VideoEncoder::setBitrate(int bitrate) {
	QMetaObject::invokeMethod(
		this, [this, bitrate]() { processBitrate(bitrate); }, Qt::QueuedConnection);
}

int VideoEncoder::bitrateFor(unsigned int maxBandwidth) {
	if (maxBandwidth == 0)
		return DEFAULT_BITRATE;

	// Leave room for the packet overhead and for the encoder overshooting its target for a moment, as the server
	// drops whatever exceeds the limit. Encoding at a very low bit rate isn't of any use either.
	static constexpr double HEADROOM = 0.8;
	static constexpr int MIN_BITRATE = 50'000;
	return static_cast< int >(std::clamp(maxBandwidth * HEADROOM, static_cast< double >(MIN_BITRATE),
										 static_cast< double >(DEFAULT_BITRATE)));
}

qint64 VideoEncoder::now() const {
	return m_streamClock.nsecsElapsed() / 1000;
}

void VideoEncoder::processStart(const QElapsedTimer &streamClock) {
	m_running        = true;
	m_streamClock    = streamClock;
	m_frameNumber    = 0;
	m_lastPts        = -1;
	m_lastEncodeTime = -1;
	m_lastFrame      = QImage();
}

void VideoEncoder::processStop() {
	m_running = false;
	m_frameRateTimer->stop();
	m_keyFrameTimer->stop();
	m_heartbeatTimer->stop();
	m_keyFrameRequested = false;
	m_lastFrame         = QImage();
	{
		QMutexLocker lock(&m_incomingMutex);
		m_incomingFrame = QImage();
	}
	m_backend.reset();
	m_encoderWidth  = 0;
	m_encoderHeight = 0;
	m_lastEncoderId.clear();
	m_failedEncoders.clear();
}

void VideoEncoder::processFrameRate(int frameRate) {
	frameRate = std::clamp(frameRate, MIN_FRAME_RATE, MAX_FRAME_RATE);
	if (frameRate == m_frameRate)
		return;
	m_frameRate = frameRate;

	if (!m_backend)
		return;

	// The key frame interval is given in frames, so open the encoder again with the next frame
	m_backend.reset();
	m_encoderWidth  = 0;
	m_encoderHeight = 0;
	forceKeyFrame();
}

void VideoEncoder::processSelection(const VideoEncoderSelection &selection) {
	// Waits for the encoders to be probed, which is why this has to happen on the encoder's thread
	QStringList order = VideoEncoders::order(VideoEncoders::available(), selection);
	if (order == m_encoderOrder)
		return;
	m_encoderOrder = std::move(order);

	if (!m_backend || m_encoderOrder.isEmpty() || m_backend->info().id == m_encoderOrder.front())
		return;

	// Switch over to the now preferred encoder with the next frame. Viewers need a key frame for the new codec
	// right away, even if the screen content doesn't change.
	m_backend.reset();
	m_encoderWidth  = 0;
	m_encoderHeight = 0;
	forceKeyFrame();
}

void VideoEncoder::processBitrate(int bitrate) {
	if (bitrate == m_bitrate)
		return;
	m_bitrate = bitrate;

	if (!m_backend)
		return;

	// Encoders can't generally change their bit rate on the fly, so open the encoder again with the next frame
	m_backend.reset();
	m_encoderWidth  = 0;
	m_encoderHeight = 0;
	forceKeyFrame();
}

void VideoEncoder::processKeyFrameRequest() {
	if (!m_running || m_keyFrameRequested || m_keyFrameTimer->isActive())
		return;

	const qint64 currentTime = now();
	const qint64 earliest    = m_lastKeyFrameTime + MIN_KEYFRAME_REQUEST_INTERVAL_US;
	if (m_lastKeyFrameTime >= 0 && currentTime < earliest) {
		m_keyFrameTimer->start(static_cast< int >((earliest - currentTime + 999) / 1000));
		return;
	}

	forceKeyFrame();
}

void VideoEncoder::forceKeyFrame() {
	if (!m_running)
		return;

	m_keyFrameRequested = true;

	// Native capture streams only deliver a frame when the screen content changes, so the key frame might not
	// go out for a long time. Encode the last frame again in that case.
	if (m_lastFrame.isNull())
		return;

	bool idle;
	{
		QMutexLocker lock(&m_incomingMutex);
		idle = m_incomingFrame.isNull();
	}
	if (idle)
		submitFrame(m_lastFrame, now());
}

void VideoEncoder::sendHeartbeat() {
	if (!m_running || m_lastFrame.isNull())
		return;

	bool idle;
	{
		QMutexLocker lock(&m_incomingMutex);
		idle = m_incomingFrame.isNull();
	}
	if (idle)
		submitFrame(m_lastFrame, now());
}

void VideoEncoder::processIncomingFrame() {
	if (!m_running)
		return;

	const qint64 currentTime = now();
	const qint64 nextSlot    = m_lastEncodeTime + frameInterval(m_frameRate);
	if (m_lastEncodeTime >= 0 && currentTime < nextSlot) {
		// Make sure that the latest frame still goes out even if the source does not deliver another one
		// (which happens as soon as the screen content stops changing).
		if (!m_frameRateTimer->isActive())
			m_frameRateTimer->start(static_cast< int >((nextSlot - currentTime + 999) / 1000));
		return;
	}

	QImage frame;
	qint64 captureTime;
	{
		QMutexLocker lock(&m_incomingMutex);
		frame           = std::move(m_incomingFrame);
		m_incomingFrame = QImage();
		captureTime     = m_incomingCaptureTime;
	}
	if (frame.isNull())
		return;

	m_frameRateTimer->stop();
	m_lastFrame      = frame;
	m_lastEncodeTime = currentTime;
	m_heartbeatTimer->start();

	encodeImage(frame, captureTime);
}

void VideoEncoder::encodeImage(const QImage &srcImage, qint64 captureTime) {
	// Encoders generally require even dimensions. The encoder crops the odd pixel itself, which saves copying the
	// picture.
	const int width  = srcImage.width() & ~1;
	const int height = srcImage.height() & ~1;
	if (width <= 0 || height <= 0)
		return;

	// Capture sources deliver pictures in a 32 bit RGB format, which encoders take as they are. Anything else
	// (which shouldn't happen) has to be converted first.
	const QImage image = isEncodable(srcImage.format()) ? srcImage : srcImage.convertToFormat(QImage::Format_RGB32);

	// (Re-)open the encoder when the resolution or the picture format changes.
	if (m_encoderWidth != width || m_encoderHeight != height || m_encoderFormat != image.format())
		openBackend(width, height, image.format());
	if (!m_backend) {
		processStop();
		emit failed();
		return;
	}

	// The encoder runs on a microsecond time base, so the capture time can be used as pts directly. This lets
	// rate control see the real frame spacing, independently of the codec and of how regularly frames arrive.
	// Encoders reject non-increasing pts, which could only happen for two frames within the same microsecond.
	m_lastPts = std::max(captureTime, m_lastPts + 1);

	std::vector< VideoEncoderBackend::Packet > packets;
	const bool keyFrame = m_keyFrameRequested;
	if (!m_backend->encode(image, m_lastPts, keyFrame, packets)) {
		// An encoder that opened may still fail on every picture (e.g. after the GPU was reset), which would leave
		// the stream without any frames. Continue with the next encoder, which starts with a key frame.
		m_failedEncoders << m_backend->info().id;
		m_backend.reset();
		m_encoderWidth  = 0;
		m_encoderHeight = 0;
		forceKeyFrame();
		return;
	}

	m_keyFrameRequested = false;
	if (keyFrame)
		m_forcedKeyFrameTimestamp = m_lastPts;
	bool keyFrameIgnored = false;

	// Encoders may delay, reorder or drop frames, so all metadata is taken from the packets that come out.
	for (VideoEncoderBackend::Packet &packet : packets) {
		EncodedVideoFrame encoded;
		encoded.data        = std::move(packet.data);
		encoded.codec       = m_backend->info().codec;
		encoded.frameNumber = m_frameNumber++;
		encoded.timestamp   = static_cast< quint64 >(packet.timestamp);
		encoded.width       = static_cast< quint32 >(m_encoderWidth);
		encoded.height      = static_cast< quint32 >(m_encoderHeight);
		encoded.isKeyFrame  = packet.isKeyFrame;

		if (encoded.isKeyFrame) {
			// Also serves any request that is currently being held back
			m_lastKeyFrameTime = now();
			m_keyFrameTimer->stop();
		}

		if (static_cast< qint64 >(encoded.timestamp) == m_forcedKeyFrameTimestamp) {
			keyFrameIgnored           = !encoded.isKeyFrame;
			m_forcedKeyFrameTimestamp = -1;
		}

		if (m_stream == m_currentStream)
			emit frameEncoded(encoded);
	}

	if (keyFrameIgnored) {
		// Not every encoder can be made to emit a key frame (or one that decoders can start at) on request, and
		// viewers can't continue without one. A newly opened encoder always starts with a key frame, so open the
		// encoder again with the next frame.
		m_backend.reset();
		m_encoderWidth  = 0;
		m_encoderHeight = 0;
		forceKeyFrame();
	}
}

bool VideoEncoder::isEncodable(QImage::Format format) {
	switch (format) {
		case QImage::Format_RGB32:
		case QImage::Format_ARGB32:
		case QImage::Format_ARGB32_Premultiplied:
		case QImage::Format_RGBX8888:
		case QImage::Format_RGBA8888:
		case QImage::Format_RGBA8888_Premultiplied:
			return true;
		default:
			return false;
	}
}

bool VideoEncoder::openBackend(int width, int height, QImage::Format format) {
	m_backend.reset();
	m_encoderWidth  = width;
	m_encoderHeight = height;
	m_encoderFormat = format;

	VideoEncoderConfig config;
	config.width            = width;
	config.height           = height;
	config.inputFormat      = format;
	config.bitrate          = m_bitrate;
	config.fps              = m_frameRate;
	config.keyFrameInterval = KEY_FRAME_INTERVAL_S * m_frameRate;

	if (m_encoderOrder.isEmpty())
		m_encoderOrder = VideoEncoders::order(VideoEncoders::available(), VideoEncoderSelection());

	// The preferred encoder may not support every picture size (e.g. hardware encoders have size limits), so
	// fall back to the next one in that case.
	for (const QString &id : m_encoderOrder) {
		if (m_failedEncoders.contains(id))
			continue;
		m_backend = VideoEncoders::create(id, config);
		if (m_backend)
			break;
	}

	if (!m_backend) {
		Global::get().l->log(
			Log::Warning,
			QObject::tr("Screen sharing: No video encoder is available for %1x%2.").arg(width).arg(height));
		return false;
	}

	if (m_backend->info().id != m_lastEncoderId) {
		m_lastEncoderId = m_backend->info().id;
		Global::get().l->log(Log::Information,
							 QObject::tr("Screen sharing: Encoding with %1.").arg(m_backend->info().name));
	}

	// A new encoder starts with a key frame anyway.
	m_keyFrameRequested       = false;
	m_lastKeyFrameTime        = -1;
	m_forcedKeyFrameTimestamp = -1;
	m_keyFrameTimer->stop();
	return true;
}
