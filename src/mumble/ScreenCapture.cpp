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
#	ifdef Q_OS_MAC
#		include "SCKitCapture.h"
#	elif defined(HAS_WAYLAND_PORTAL)
#		include "XdgPortalCapture.h"
#	endif
#endif

#include "Global.h"

#ifdef USE_SCREEN_SHARING
// Rounded up, so that grabbed frames never come in faster than the encoder's frame rate limit lets them through
static constexpr int CAPTURE_INTERVAL_MS = static_cast< int >((VideoEncoder::FRAME_INTERVAL_US + 999) / 1000);
#else
static constexpr int CAPTURE_INTERVAL_MS = 67;
#endif

ScreenCapture::ScreenCapture(QObject *parent) : QObject(parent) {
	m_captureTimer = new QTimer(this);
	m_captureTimer->setInterval(CAPTURE_INTERVAL_MS);
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
	m_grabTimer->setInterval(CAPTURE_INTERVAL_MS);
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

void ScreenCapture::setSource(const CaptureSource &source) {
	m_source = source;
}

void ScreenCapture::onEncoderFailed() {
	if (!m_capturing)
		return;

	Global::get().l->log(Log::Warning, QObject::tr("Screen sharing stopped: The video could not be encoded."));
	stopCapture();
	emit captureEnded();
}

#	if defined(Q_OS_MAC) || defined(HAS_WAYLAND_PORTAL)
void ScreenCapture::startCaptureNative() {
	if (m_capturing)
		return;

	// Keep a safe pointer — the lambdas below must not capture `this` without guard.
	QPointer< ScreenCapture > self = this;

	auto onStarted = [self]() {
		if (!self)
			return;
		self->m_capturing = true;
		self->m_streamClock.start();
		self->m_encoder->start(self->m_streamClock);
		emit self->captureStarted();
	};
	auto onCancelled = [self]() {
		if (!self)
			return;
		if (self->m_capturing) {
			// The user ended the running screen share through the system instead of through Mumble
			self->stopCapture();
			emit self->captureEnded();
			return;
		}
		emit self->captureAborted();
	};
	auto onError = [self](QString error) {
		if (!self)
			return;
		Global::get().l->log(Log::Warning, QObject::tr("Screen capture failed: %1").arg(error));
		if (self->m_capturing) {
			// The screen share has been announced already, so it has to be ended like one stopping by itself
			self->stopCapture();
			emit self->captureEnded();
			return;
		}
		self->m_encoder->stop();
		emit self->captureAborted();
	};
	auto onFrame = [self](QImage frame) {
		if (!self || !self->m_capturing)
			return;
		self->m_encoder->submitFrame(frame, self->m_streamClock.nsecsElapsed() / 1000);
	};

#		ifdef Q_OS_MAC
	sckit_startWithNativePicker(std::move(onStarted), std::move(onCancelled), std::move(onError), std::move(onFrame));
#		else
	xdg_portal_startCapture(std::move(onStarted), std::move(onCancelled), std::move(onError), std::move(onFrame));
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
