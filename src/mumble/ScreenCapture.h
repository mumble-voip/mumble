// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCREENCAPTURE_H_
#define MUMBLE_MUMBLE_SCREENCAPTURE_H_

#include <QtCore/QByteArray>
#include <QtCore/QElapsedTimer>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <cstdint>

#ifdef USE_SCREEN_SHARING
#	include "CaptureSource.h"
extern "C" {
#	include <libavcodec/avcodec.h>
#	include <libavutil/opt.h>
#	include <libswscale/swscale.h>
}
#endif

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

/// Captures a selected screen or window at ~15 fps and emits encoded video frames via frameEncoded().
///
/// On macOS, startCaptureNative() shows the OS-native SCContentSharingPicker and streams
/// frames via SCStream; captureStarted() / captureAborted() signals report the async outcome.
/// On other platforms, use setSource() + startCapture() with ScreenPickerDialog.
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

#	ifdef Q_OS_MAC
	/// Shows the native macOS SCContentSharingPicker and starts capturing
	/// the selected source via SCStream. Asynchronous: returns immediately.
	/// captureStarted() is emitted when the stream is running; captureAborted() if cancelled/failed.
	void startCaptureNative();
#	endif
#endif

signals:
	/// Emitted for every successfully encoded frame.
	void frameEncoded(const EncodedVideoFrame &frame);
	/// Emitted when capturing stopped by itself instead of through stopCapture(), e.g. because grabbing the screen
	/// failed. Capturing has already stopped by then.
	void captureEnded();

#if defined(USE_SCREEN_SHARING) && defined(Q_OS_MAC)
	/// Emitted on the main thread when the native SCStream starts delivering frames.
	void captureStarted();
	/// Emitted on the main thread when the native picker is cancelled or the stream fails to start.
	void captureAborted();
#endif

private slots:
	void captureFrame();

private:
#ifdef USE_SCREEN_SHARING
	bool initEncoder(int width, int height);
	void destroyEncoder();
	/// Shared encode path used by both capture modes.
	/// @param captureTime  Capture time in microseconds on m_streamClock.
	void encodeImage(const QImage &srcImage, qint64 captureTime);

	CaptureSource m_source; ///< Defaults to EntireScreen, screenIndex=0 (primary display).

	AVCodecContext *m_codecCtx = nullptr;
	AVFrame *m_frame           = nullptr;
	AVPacket *m_packet         = nullptr;
	SwsContext *m_swsCtx       = nullptr;
	int m_encoderWidth         = 0;
	int m_encoderHeight        = 0;

	/// Set when the next frame shall be encoded as a key frame.
	bool m_keyFrameRequested = false;
	/// Time on m_streamClock at which the last key frame was emitted, or -1 if none was yet.
	qint64 m_lastKeyFrameTime = -1;
	/// Fires when a held back key frame request may be served.
	QTimer *m_keyFrameTimer = nullptr;
#endif

	QTimer *m_captureTimer = nullptr;
	/// Reference clock for frame timestamps, started together with the stream.
	QElapsedTimer m_streamClock;
	quint64 m_frameNumber = 0;
	/// Timestamp of the last frame handed to the encoder, used to keep pts strictly increasing.
	qint64 m_lastPts = -1;
	bool m_capturing = false;
};

#endif // MUMBLE_MUMBLE_SCREENCAPTURE_H_
