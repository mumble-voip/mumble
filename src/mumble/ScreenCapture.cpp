// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ScreenCapture.h"

#include "Log.h"

#ifdef USE_SCREEN_SHARING
#	include "CaptureSourceLister.h"
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
#endif
}

ScreenCapture::~ScreenCapture() {
	stopCapture();

#ifdef USE_SCREEN_SHARING
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
	m_captureTimer->start();
#endif
}

void ScreenCapture::stopCapture() {
	if (!m_capturing)
		return;

	m_captureTimer->stop();
	m_capturing = false;

#ifdef USE_SCREEN_SHARING
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
