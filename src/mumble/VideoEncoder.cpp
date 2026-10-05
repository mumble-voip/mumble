// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VideoEncoder.h"

#include "Log.h"
#include "Global.h"

#include <QtCore/QMutexLocker>
#include <QtCore/QTimer>

extern "C" {
#include <libavutil/opt.h>
}

#include <algorithm>

static constexpr int VIDEO_GOP_SIZE = 60 * VideoEncoder::FPS; // keyframe every ~60 s — receivers request them on loss
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
}

VideoEncoder::~VideoEncoder() {
	destroyEncoder();
}

void VideoEncoder::start(const QElapsedTimer &streamClock) {
	QMetaObject::invokeMethod(
		this, [this, streamClock]() { processStart(streamClock); }, Qt::QueuedConnection);
}

void VideoEncoder::stop() {
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
	m_keyFrameRequested = false;
	m_lastFrame         = QImage();
	{
		QMutexLocker lock(&m_incomingMutex);
		m_incomingFrame = QImage();
	}
	destroyEncoder();
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

void VideoEncoder::processIncomingFrame() {
	if (!m_running)
		return;

	const qint64 currentTime = now();
	const qint64 nextSlot    = m_lastEncodeTime + FRAME_INTERVAL_US;
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

	encodeImage(frame, captureTime);
}

void VideoEncoder::encodeImage(const QImage &srcImage, qint64 captureTime) {
	// Convert to Format_RGBA8888 for mapping to AV_PIX_FMT_RGBA.
	QImage image = srcImage.convertToFormat(QImage::Format_RGBA8888);
	// libx264 (YUV420P) requires even dimensions — crop one pixel if needed.
	const int width  = image.width() & ~1;
	const int height = image.height() & ~1;
	if (width <= 0 || height <= 0)
		return;
	if (width != image.width() || height != image.height())
		image = image.copy(0, 0, width, height);

	// (Re-)initialise the encoder when the resolution changes.
	if (!m_codecCtx || m_encoderWidth != width || m_encoderHeight != height) {
		destroyEncoder();
		if (!initEncoder(width, height))
			return;
	}

	// Colour-space conversion: RGBA to YUV420P.
	m_swsCtx = sws_getCachedContext(m_swsCtx, width, height, AV_PIX_FMT_RGBA, width, height, AV_PIX_FMT_YUV420P,
									SWS_BICUBIC, nullptr, nullptr, nullptr);
	if (!m_swsCtx)
		return;

	if (av_frame_make_writable(m_frame) < 0)
		return;

	const uint8_t *srcData[1] = { image.constBits() };
	int srcLinesize[1]        = { static_cast< int >(image.bytesPerLine()) };
	sws_scale(m_swsCtx, srcData, srcLinesize, 0, height, m_frame->data, m_frame->linesize);

	// The encoder runs on a microsecond time base, so the capture time can be used as pts directly. This lets
	// rate control see the real frame spacing, independently of the codec and of how regularly frames arrive.
	// Encoders reject non-increasing pts, which could only happen for two frames within the same microsecond.
	m_lastPts    = std::max(captureTime, m_lastPts + 1);
	m_frame->pts = m_lastPts;
	// The frame is reused, so the picture type has to be reset after a forced key frame.
	m_frame->pict_type = m_keyFrameRequested ? AV_PICTURE_TYPE_I : AV_PICTURE_TYPE_NONE;

	if (avcodec_send_frame(m_codecCtx, m_frame) < 0)
		return;

	m_keyFrameRequested = false;

	// Encoders may delay, reorder or drop frames, so all metadata is taken from the packet that comes out.
	while (avcodec_receive_packet(m_codecCtx, m_packet) == 0) {
		EncodedVideoFrame encoded;
		encoded.data        = QByteArray(reinterpret_cast< const char * >(m_packet->data), m_packet->size);
		encoded.frameNumber = m_frameNumber++;
		encoded.timestamp   = static_cast< quint64 >(m_packet->pts != AV_NOPTS_VALUE ? m_packet->pts : m_lastPts);
		encoded.width       = static_cast< quint32 >(m_encoderWidth);
		encoded.height      = static_cast< quint32 >(m_encoderHeight);
		encoded.isKeyFrame  = (m_packet->flags & AV_PKT_FLAG_KEY) != 0;

		if (encoded.isKeyFrame) {
			// Also serves any request that is currently being held back
			m_lastKeyFrameTime = now();
			m_keyFrameTimer->stop();
		}

		emit frameEncoded(encoded);
		av_packet_unref(m_packet);
	}
}

bool VideoEncoder::initEncoder(int width, int height) {
	// To use hardware-accelerated encoding (e.g. h264_videotoolbox on macOS,
	// h264_nvenc on NVIDIA), replace "libx264" with the appropriate encoder name
	// and add any codec-specific option calls below.
	const char *encoderName = "libx264";
	const AVCodec *codec    = avcodec_find_encoder_by_name(encoderName);
	if (!codec) {
		// I'm logging straight into the chatbox here so I can test things. But this probably should be a qWarning
		Global::get().l->log(Log::Warning,
							 QObject::tr("H.264 encoder (libx264) not available. "
										 "Ensure libx264 is installed and libavcodec was compiled with it."));
		return false;
	}

	m_codecCtx = avcodec_alloc_context3(codec);
	if (!m_codecCtx)
		return false;

	m_codecCtx->width     = width;
	m_codecCtx->height    = height;
	m_codecCtx->time_base = { 1, 1'000'000 }; // pts are capture timestamps in microseconds
	m_codecCtx->framerate = { FPS, 1 };       // nominal rate; actual frame spacing comes from pts
	m_codecCtx->pix_fmt   = AV_PIX_FMT_YUV420P;
	m_codecCtx->bit_rate  = BITRATE;
	m_codecCtx->gop_size  = VIDEO_GOP_SIZE;

	// Minimise encoding latency. These could maybe be settings?
	av_opt_set(m_codecCtx->priv_data, "preset", "superfast", 0);
	av_opt_set(m_codecCtx->priv_data, "tune", "zerolatency", 0);
	// Make requested key frames IDR frames, as decoders can only start over at those.
	av_opt_set(m_codecCtx->priv_data, "forced-idr", "1", 0);

	if (avcodec_open2(m_codecCtx, codec, nullptr) < 0) {
		avcodec_free_context(&m_codecCtx);
		return false;
	}

	m_frame         = av_frame_alloc();
	m_frame->format = AV_PIX_FMT_YUV420P;
	m_frame->width  = width;
	m_frame->height = height;
	if (av_frame_get_buffer(m_frame, 0) < 0) {
		av_frame_free(&m_frame);
		avcodec_free_context(&m_codecCtx);
		return false;
	}

	m_packet = av_packet_alloc();

	m_encoderWidth  = width;
	m_encoderHeight = height;

	// A new encoder starts with a key frame anyway.
	m_keyFrameRequested = false;
	m_lastKeyFrameTime  = -1;
	m_keyFrameTimer->stop();
	return true;
}

void VideoEncoder::destroyEncoder() {
	if (m_swsCtx) {
		sws_freeContext(m_swsCtx);
		m_swsCtx = nullptr;
	}
	if (m_frame) {
		av_frame_free(&m_frame);
	}
	if (m_packet) {
		av_packet_free(&m_packet);
	}
	if (m_codecCtx) {
		avcodec_free_context(&m_codecCtx);
	}
	m_encoderWidth  = 0;
	m_encoderHeight = 0;
}
