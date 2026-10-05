// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCREENCAPTURE_H_
#define MUMBLE_MUMBLE_SCREENCAPTURE_H_

#include <QtCore/QByteArray>
#include <QtCore/QElapsedTimer>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <cstdint>

#ifdef USE_SCREEN_SHARING
#	include "CaptureSource.h"
#endif

class QThread;
class VideoEncoder;

/// An encoded video frame together with the metadata needed to transmit it.
struct EncodedVideoFrame {
	/// Codec-specific encoded byte stream (currently H.264 Annex-B).
	QByteArray data;
	/// Counter of emitted frames in decode order, starting at 0.
	quint64 frameNumber = 0;
	/// Capture time in microseconds relative to the start of the stream.
	quint64 timestamp = 0;
	/// Dimensions of the encoded picture in pixels.
	quint32 width  = 0;
	quint32 height = 0;
	/// True when the frame is an IDR / key frame.
	bool isKeyFrame = false;
};
Q_DECLARE_METATYPE(EncodedVideoFrame)

/// Captures a selected screen or window at ~15 fps and emits encoded video frames via frameEncoded().
///
/// Capturing happens on the GUI thread, but the captured images are encoded on a separate thread owned by this
/// object (see VideoEncoder). Hence frameEncoded() is emitted from that thread.
///
/// On macOS, startCaptureNative() shows the OS-native SCContentSharingPicker and streams
/// frames via SCStream; captureStarted() / captureAborted() signals report the async outcome.
/// On Linux under Wayland, startCaptureNative() uses the xdg-desktop-portal ScreenCast interface
/// and delivers frames via a PipeWire stream.
/// On other platforms (or X11), use setSource() + startCapture() with ScreenPickerDialog.
///
/// Requires the build option -Dscreen-sharing=ON (links libavcodec/libswscale).
class ScreenCapture : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ScreenCapture)

public:
	explicit ScreenCapture(QObject *parent = nullptr);
	~ScreenCapture() override;

	void startCapture();
	void stopCapture();
	bool isCapturing() const;

	/// Makes the encoder emit a key frame as soon as possible, so that viewers who lost part of the stream can
	/// start decoding again. Requests that come in shortly after a key frame are held back for a moment, which
	/// limits how many key frames are sent no matter how many viewers ask for them.
	void requestKeyFrame();

#ifdef USE_SCREEN_SHARING
	/// Sets the capture source for the non-native picker path. Call before startCapture().
	void setSource(const CaptureSource &source);

#	if defined(Q_OS_MAC) || defined(HAS_WAYLAND_PORTAL)
	/// Shows the platform-native picker and starts capturing asynchronously.
	/// On macOS: uses SCContentSharingPicker / SCStream.
	/// On Linux (Wayland): uses xdg-desktop-portal ScreenCast + PipeWire.
	/// captureStarted() is emitted when frames begin; captureAborted() if cancelled/failed.
	void startCaptureNative();
#	endif
#endif

signals:
	/// Emitted from the encoder's thread for every successfully encoded frame.
	void frameEncoded(const EncodedVideoFrame &frame);
	/// Emitted when capturing stopped by itself instead of through stopCapture(), e.g. because grabbing the screen
	/// failed or the user ended it through the system (e.g. the desktop's screen sharing indicator). Capturing has
	/// already stopped by then.
	void captureEnded();

#if defined(USE_SCREEN_SHARING) && (defined(Q_OS_MAC) || defined(HAS_WAYLAND_PORTAL))
	/// Emitted on the main thread when the native stream starts delivering frames.
	void captureStarted();
	/// Emitted on the main thread when the native picker is cancelled or the stream fails to start.
	void captureAborted();
#endif

private slots:
	void captureFrame();

private:
#ifdef USE_SCREEN_SHARING
	CaptureSource m_source; ///< Defaults to EntireScreen, screenIndex=0 (primary display).

	QThread *m_encoderThread = nullptr;
	/// Lives on m_encoderThread and is deleted there once the thread has finished.
	VideoEncoder *m_encoder = nullptr;
#endif

	QTimer *m_captureTimer = nullptr;
	/// Reference clock for frame timestamps, started together with the stream.
	QElapsedTimer m_streamClock;
	bool m_capturing = false;
};

#endif // MUMBLE_MUMBLE_SCREENCAPTURE_H_
