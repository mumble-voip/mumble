// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ScreenCapture.h"

#include "Log.h"

#ifdef USE_SCREEN_SHARING
#	include "CaptureSourceLister.h"
#	include "FrameGrabber.h"
#	include "VideoEncoder.h"
#	include <QtCore/QPointer>
#	include <QtCore/QThread>
#	include <QtGui/QImage>

#	include <mutex>
#	ifdef Q_OS_MAC
#		include "SCKitCapture.h"
#	elif defined(HAS_WAYLAND_PORTAL)
#		include "XdgPortalCapture.h"
#	endif
#endif

#include "Global.h"

#include <algorithm>

#ifdef USE_SCREEN_SHARING
/// Rounded up, so that grabbed frames never come in faster than the encoder's frame rate limit lets them through
static int captureInterval(int frameRate) {
	return static_cast< int >((VideoEncoder::frameInterval(frameRate) + 999) / 1000);
}
#endif

ScreenCapture::ScreenCapture(QObject *parent) : QObject(parent) {
#ifdef USE_SCREEN_SHARING
	m_frameRate = VideoEncoder::DEFAULT_FRAME_RATE;
#endif

	m_captureTimer = new QTimer(this);
#ifdef USE_SCREEN_SHARING
	m_captureTimer->setInterval(captureInterval(m_frameRate));
#endif
	m_captureTimer->setTimerType(Qt::PreciseTimer);
	connect(m_captureTimer, &QTimer::timeout, this, &ScreenCapture::captureFrame);

#ifdef USE_SCREEN_SHARING
	// Colour conversion and encoding take a considerable amount of time per frame, so they are done on a
	// separate thread to keep the GUI responsive.
	m_encoderThread = new QThread(this);
	m_encoderThread->setObjectName(QLatin1String("VideoEncoder"));
	m_encoder = new VideoEncoder();
	m_encoder->moveToThread(m_encoderThread);
	connect(m_encoderThread, &QThread::finished, m_encoder, &QObject::deleteLater);
	connect(m_encoder, &VideoEncoder::frameEncoded, this, &ScreenCapture::frameEncoded, Qt::DirectConnection);
	connect(m_encoder, &VideoEncoder::failed, this, &ScreenCapture::onEncoderFailed);
	m_encoderThread->start();

	// Grabbing a picture can take several milliseconds, which must neither block the GUI nor be held up by it
	m_grabThread = new QThread(this);
	m_grabThread->setObjectName(QLatin1String("ScreenGrabber"));
	m_grabContext = new QObject();
	m_grabTimer   = new QTimer(m_grabContext);
	m_grabTimer->setInterval(captureInterval(m_frameRate));
	m_grabTimer->setTimerType(Qt::PreciseTimer);
	connect(m_grabTimer, &QTimer::timeout, m_grabContext, [this]() { grabFrame(); });
	m_grabContext->moveToThread(m_grabThread);
	connect(m_grabThread, &QThread::finished, m_grabContext, &QObject::deleteLater);
	m_grabThread->start();
#endif
}

ScreenCapture::~ScreenCapture() {
	stopCapture();

#ifdef USE_SCREEN_SHARING
#	if defined(Q_OS_MAC) || defined(HAS_WAYLAND_PORTAL)
	// The native picker may still be open, after which frames could come in for the encoder that is deleted below
	closeNativeSink();
#	endif
	m_grabThread->quit();
	m_grabThread->wait();
	m_encoderThread->quit();
	m_encoderThread->wait();
#endif
}

void ScreenCapture::startCapture() {
#ifndef USE_SCREEN_SHARING
	// This way it's sent to the chatbox. I don't know if this should be a qWarning instead.
	Global::get().l->log(Log::Warning,
						 QObject::tr("Screen sharing requires Mumble to be built with -Dscreen-sharing=ON."));
#else
	if (m_capturing)
		return;

	m_capturing = true;
	m_streamClock.start();
	m_encoder->start(m_streamClock);

	std::shared_ptr< FrameGrabber > grabber = FrameGrabber::create(m_source);
	if (!grabber) {
		// The source can only be grabbed on the GUI thread
		m_captureTimer->start();
		return;
	}

	m_grabbing               = true;
	const quint64 generation = ++m_captureGeneration;
	QMetaObject::invokeMethod(
		m_grabContext,
		[this, grabber = std::move(grabber), generation]() mutable {
			m_grabber           = std::move(grabber);
			m_grabberGeneration = generation;
			m_grabTimer->start();
		},
		Qt::QueuedConnection);
#endif
}

void ScreenCapture::stopCapture() {
	if (!m_capturing)
		return;

	m_captureTimer->stop();
	m_capturing = false;

#ifdef USE_SCREEN_SHARING
	if (m_grabbing) {
		// Waits for the grabber to be gone, so that no more frames are submitted from here on
		m_grabbing = false;
		QMetaObject::invokeMethod(
			m_grabContext,
			[this]() {
				m_grabTimer->stop();
				m_grabber.reset();
			},
			Qt::BlockingQueuedConnection);
	}
#	ifdef Q_OS_MAC
	sckit_stop();
#	elif defined(HAS_WAYLAND_PORTAL)
	xdg_portal_stop();
#	endif
#	if defined(Q_OS_MAC) || defined(HAS_WAYLAND_PORTAL)
	// The stream may deliver a few more frames while it is being stopped
	closeNativeSink();
#	endif
	m_encoder->stop();
#endif
}

bool ScreenCapture::isCapturing() const {
	return m_capturing;
}

void ScreenCapture::requestKeyFrame() {
#ifdef USE_SCREEN_SHARING
	if (m_capturing)
		m_encoder->requestKeyFrame();
#endif
}

#ifdef USE_SCREEN_SHARING

void ScreenCapture::setEncoderSelection(const VideoEncoderSelection &selection) {
	m_encoder->setSelection(selection);
}

void ScreenCapture::setFrameRate(int frameRate) {
	m_frameRate = std::clamp(frameRate, VideoEncoder::MIN_FRAME_RATE, VideoEncoder::MAX_FRAME_RATE);
	m_encoder->setFrameRate(m_frameRate);

	const int interval = captureInterval(m_frameRate);
	m_captureTimer->setInterval(interval);
	QMetaObject::invokeMethod(
		m_grabContext, [this, interval]() { m_grabTimer->setInterval(interval); }, Qt::QueuedConnection);
}

void ScreenCapture::setBitrate(int bitrate) {
	m_encoder->setBitrate(bitrate);
}

void ScreenCapture::setSource(const CaptureSource &source) {
	m_source = source;
}

void ScreenCapture::onEncoderFailed() {
#	if defined(Q_OS_MAC) || defined(HAS_WAYLAND_PORTAL)
	// Native capture streams start the encoder before they are reported as started
	const bool nativeStarting = !m_capturing && m_nativeSink;
#	else
	const bool nativeStarting = false;
#	endif
	if (!m_capturing && !nativeStarting)
		return;

	Global::get().l->log(Log::Warning, QObject::tr("Screen sharing stopped: The video could not be encoded."));
	if (m_capturing) {
		stopCapture();
		emit captureEnded();
		return;
	}

#	if defined(Q_OS_MAC) || defined(HAS_WAYLAND_PORTAL)
	closeNativeSink();
#		ifdef Q_OS_MAC
	sckit_stop();
#		elif defined(HAS_WAYLAND_PORTAL)
	xdg_portal_stop();
#		endif
	emit captureAborted();
#	endif
}

#	if defined(Q_OS_MAC) || defined(HAS_WAYLAND_PORTAL)
struct ScreenCapture::NativeFrameSink {
	std::mutex mutex;
	/// Reset once the capture is stopped, after which frames that still come in are dropped
	VideoEncoder *encoder = nullptr;
	QElapsedTimer streamClock;

	void submit(const QImage &frame) {
		std::lock_guard< std::mutex > lock(mutex);
		if (encoder)
			encoder->submitFrame(frame, streamClock.nsecsElapsed() / 1000);
	}
};

void ScreenCapture::closeNativeSink() {
	if (!m_nativeSink)
		return;

	{
		std::lock_guard< std::mutex > lock(m_nativeSink->mutex);
		m_nativeSink->encoder = nullptr;
	}
	m_nativeSink.reset();
}

void ScreenCapture::startCaptureNative() {
	if (m_capturing)
		return;

	// Frames are handed to the encoder on the thread they come in on, so that a busy GUI can't hold them up. Hence
	// the encoder is started right away, frames may come in before onStarted() runs.
	closeNativeSink();
	m_streamClock.start();
	m_encoder->start(m_streamClock);
	auto sink         = std::make_shared< NativeFrameSink >();
	sink->encoder     = m_encoder;
	sink->streamClock = m_streamClock;
	m_nativeSink      = sink;

	// Keep a safe pointer — the lambdas below must not capture `this` without guard. They also must not act on a
	// capture that has been replaced by a later one.
	QPointer< ScreenCapture > self = this;

	auto onStarted = [self, sink]() {
		if (!self || self->m_nativeSink != sink)
			return;
		self->m_capturing = true;
		emit self->captureStarted();
	};
	auto onCancelled = [self, sink]() {
		if (!self || self->m_nativeSink != sink)
			return;
		if (self->m_capturing) {
			// The user ended the running screen share through the system instead of through Mumble
			self->stopCapture();
			emit self->captureEnded();
			return;
		}
		self->closeNativeSink();
		self->m_encoder->stop();
		emit self->captureAborted();
	};
	auto onError = [self, sink](QString error) {
		if (!self || self->m_nativeSink != sink)
			return;
		Global::get().l->log(Log::Warning, QObject::tr("Screen capture failed: %1").arg(error));
		if (self->m_capturing) {
			// The screen share has been announced already, so it has to be ended like one stopping by itself
			self->stopCapture();
			emit self->captureEnded();
			return;
		}
		self->closeNativeSink();
		self->m_encoder->stop();
		emit self->captureAborted();
	};
	auto onFrame = [sink](QImage frame) { sink->submit(frame); };

#		ifdef Q_OS_MAC
	sckit_startWithNativePicker(m_frameRate, std::move(onStarted), std::move(onCancelled), std::move(onError),
								std::move(onFrame));
#		else
	xdg_portal_startCapture(m_frameRate, std::move(onStarted), std::move(onCancelled), std::move(onError),
							std::move(onFrame));
#		endif
}
#	endif // Q_OS_MAC || HAS_WAYLAND_PORTAL

void ScreenCapture::grabFrame() {
	if (!m_grabber)
		return;

	const qint64 captureTime = m_streamClock.nsecsElapsed() / 1000;

	QImage image;
	switch (m_grabber->grab(image)) {
		case FrameGrabber::Result::Frame:
			m_encoder->submitFrame(image, captureTime);
			break;
		case FrameGrabber::Result::Unchanged:
			break;
		case FrameGrabber::Result::Failed:
			m_grabTimer->stop();
			m_grabber.reset();
			QMetaObject::invokeMethod(
				this, [this, generation = m_grabberGeneration]() { onGrabFailed(generation); }, Qt::QueuedConnection);
			break;
	}
}

void ScreenCapture::onGrabFailed(quint64 generation) {
	if (!m_grabbing || generation != m_captureGeneration)
		return;

	Global::get().l->log(Log::Warning, QObject::tr("Screen capture failed."));
	stopCapture();
	emit captureEnded();
}

#endif // USE_SCREEN_SHARING

void ScreenCapture::captureFrame() {
#ifdef USE_SCREEN_SHARING
	const qint64 captureTime = m_streamClock.nsecsElapsed() / 1000;

	// Delegate platform-specific grab to CaptureSourceLister.
	QImage image = grabCaptureSource(m_source);
	if (image.isNull()) {
		Global::get().l->log(Log::Warning, QObject::tr("Screen capture failed."));
		stopCapture();
		emit captureEnded();
		return;
	}

	m_encoder->submitFrame(image, captureTime);
#endif
}
