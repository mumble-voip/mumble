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

// These values are still hardcoded. This should probably be a setting.
// For now these values seem alright for testing
static constexpr int CAPTURE_INTERVAL_MS = 66;        // ~15 fps
static constexpr int VIDEO_BITRATE       = 1'500'000; // 1.5 Mbps
static constexpr int VIDEO_FPS           = 15;
static constexpr int VIDEO_GOP_SIZE      = 5; // keyframe every ~333 ms — limits UDP error propagation

ScreenCapture::ScreenCapture(QObject *parent) : QObject(parent) {
	m_captureTimer = new QTimer(this);
	m_captureTimer->setInterval(CAPTURE_INTERVAL_MS);
	connect(m_captureTimer, &QTimer::timeout, this, &ScreenCapture::captureFrame);
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
	m_capturing   = true;
	m_captureTimer->start();
#endif
}

void ScreenCapture::stopCapture() {
	if (!m_capturing)
		return;

	m_captureTimer->stop();
	m_capturing = false;

#ifdef USE_SCREEN_SHARING
	destroyEncoder();
#endif
}

bool ScreenCapture::isCapturing() const {
	return m_capturing;
}

void ScreenCapture::captureFrame() {
#ifdef USE_SCREEN_SHARING
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
		return;
	}

	// Convert to Format_RGBA8888 for mapping to AV_PIX_FMT_RGB24.
	QImage image     = pixmap.toImage().convertToFormat(QImage::Format_RGBA8888);
	const int width  = image.width();
	const int height = image.height();

	// (Re-)initialise the encoder if this is the first frame or the resolution changed.
	if (!m_codecCtx || m_encoderWidth != width || m_encoderHeight != height) {
		destroyEncoder();
		if (!initEncoder(width, height))
			return;
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

	m_frame->pts = static_cast< int64_t >(m_frameNumber);

	if (avcodec_send_frame(m_codecCtx, m_frame) < 0)
		return;

	while (avcodec_receive_packet(m_codecCtx, m_packet) == 0) {
		QByteArray encodedData(reinterpret_cast< const char * >(m_packet->data), m_packet->size);
		const bool isKey = (m_packet->flags & AV_PKT_FLAG_KEY) != 0;
		emit frameEncoded(encodedData, m_frameNumber, isKey);
		av_packet_unref(m_packet);
	}

	++m_frameNumber;
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
	m_codecCtx->time_base = { 1, VIDEO_FPS };
	m_codecCtx->pix_fmt   = AV_PIX_FMT_YUV420P;
	m_codecCtx->bit_rate  = VIDEO_BITRATE;
	m_codecCtx->gop_size  = VIDEO_GOP_SIZE;

	// Minimise encoding latency. These could maybe be settings?
	av_opt_set(m_codecCtx->priv_data, "preset", "superfast", 0);
	av_opt_set(m_codecCtx->priv_data, "tune", "zerolatency", 0);

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
