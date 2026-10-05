// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ScreenCapture.h"

#include "Log.h"

#ifdef USE_SCREEN_SHARING
#	include <QtGui/QGuiApplication>
#	include <QtGui/QImage>
#	include <QtGui/QPixmap>
#	include <QtGui/QScreen>
#endif

#include "Global.h"

#include <algorithm>

// These values are still hardcoded. This should probably be a setting.
// For now these values seem alright for testing
static constexpr int CAPTURE_INTERVAL_MS = 66;        // ~15 fps
static constexpr int VIDEO_BITRATE       = 1'500'000; // 1.5 Mbps
static constexpr int VIDEO_FPS           = 15;
static constexpr int VIDEO_GOP_SIZE      = 60 * VIDEO_FPS; // keyframe every ~60 s — receivers request them on loss
/// Minimum time between a key frame and one sent on request. Viewers tend to lose the same packets and each of
/// them asks for a key frame, so this keeps a single loss from causing a burst of key frames.
static constexpr qint64 MIN_KEYFRAME_REQUEST_INTERVAL_US = 500'000;

ScreenCapture::ScreenCapture(QObject *parent) : QObject(parent) {
	m_captureTimer = new QTimer(this);
	m_captureTimer->setInterval(CAPTURE_INTERVAL_MS);
	connect(m_captureTimer, &QTimer::timeout, this, &ScreenCapture::captureFrame);

#ifdef USE_SCREEN_SHARING
	m_keyFrameTimer = new QTimer(this);
	m_keyFrameTimer->setSingleShot(true);
	connect(m_keyFrameTimer, &QTimer::timeout, this, [this]() { m_keyFrameRequested = true; });
#endif
}

ScreenCapture::~ScreenCapture() {
	stopCapture();
}

void ScreenCapture::startCapture() {
#ifndef USE_SCREEN_SHARING
	// This way it's sent to the chatbox. I don't know if this should be a qWarning instead.
	Global::get().l->log(Log::Warning,
						 QObject::tr("Screen sharing requires Mumble to be built with -Dscreen-sharing=ON."));
#else
	if (m_capturing)
		return;

	m_frameNumber = 0;
	m_lastPts     = -1;
	m_capturing   = true;
	m_streamClock.start();
	m_captureTimer->start();
#endif
}

void ScreenCapture::stopCapture() {
	if (!m_capturing)
		return;

	m_captureTimer->stop();
	m_capturing = false;

#ifdef USE_SCREEN_SHARING
	m_keyFrameTimer->stop();
	destroyEncoder();
#endif
}

bool ScreenCapture::isCapturing() const {
	return m_capturing;
}

void ScreenCapture::requestKeyFrame() {
#ifdef USE_SCREEN_SHARING
	if (!m_capturing || m_keyFrameRequested || m_keyFrameTimer->isActive())
		return;

	const qint64 currentTime = m_streamClock.nsecsElapsed() / 1000;
	const qint64 earliest    = m_lastKeyFrameTime + MIN_KEYFRAME_REQUEST_INTERVAL_US;
	if (m_lastKeyFrameTime >= 0 && currentTime < earliest) {
		m_keyFrameTimer->start(static_cast< int >((earliest - currentTime + 999) / 1000));
		return;
	}

	m_keyFrameRequested = true;
#endif
}

void ScreenCapture::captureFrame() {
#ifdef USE_SCREEN_SHARING
	const qint64 captureTime = m_streamClock.nsecsElapsed() / 1000;

	QScreen *screen = QGuiApplication::primaryScreen();
	if (!screen)
		return;

	// Grab the entire primary screen.
	QPixmap pixmap = screen->grabWindow(0);
	if (pixmap.isNull()) {
		// grabWindow(0) fails silently under Wayland.
		// In the future, this should be replaced with dg-desktop-portal
		Global::get().l->log(Log::Warning, QObject::tr("Screen capture failed: grabWindow returned null. "));
		stopCapture();
		emit captureEnded();
		return;
	}

	// Convert to Format_RGBA8888 for mapping to AV_PIX_FMT_RGB24.
	QImage image     = pixmap.toImage().convertToFormat(QImage::Format_RGBA8888);
	const int width  = image.width();
	const int height = image.height();

	// (Re-)initialise the encoder if this is the first frame or the resolution changed.
	if (!m_codecCtx || m_encoderWidth != width || m_encoderHeight != height) {
		destroyEncoder();
		if (!initEncoder(width, height)) {
			// Retrying with the next frame would fail the same way
			stopCapture();
			emit captureEnded();
			return;
		}
	}

	// Colour-space conversion: RGBA24 to YUV420P.
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
			m_lastKeyFrameTime = m_streamClock.nsecsElapsed() / 1000;
			m_keyFrameTimer->stop();
		}

		emit frameEncoded(encoded);
		av_packet_unref(m_packet);
	}
#endif
}

#ifdef USE_SCREEN_SHARING
bool ScreenCapture::initEncoder(int width, int height) {
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
	m_codecCtx->framerate = { VIDEO_FPS, 1 }; // nominal rate; actual frame spacing comes from pts
	m_codecCtx->pix_fmt   = AV_PIX_FMT_YUV420P;
	m_codecCtx->bit_rate  = VIDEO_BITRATE;
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

void ScreenCapture::destroyEncoder() {
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
#endif
