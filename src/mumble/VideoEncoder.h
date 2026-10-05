// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VIDEOENCODER_H_
#define MUMBLE_MUMBLE_VIDEOENCODER_H_

#include "ScreenCapture.h"

#include <QtCore/QElapsedTimer>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtGui/QImage>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

class QTimer;

/// Turns captured images into an encoded video stream.
///
/// The encoder is meant to live on its own thread (see QObject::moveToThread()), so that colour conversion and
/// encoding don't block the GUI. All public methods may be called from any thread: they only hand the work over,
/// which is then carried out on the encoder's thread. frameEncoded() is emitted from the encoder's thread.
///
/// Frames are limited to the target frame rate. Capture sources may deliver frames much faster and irregularly
/// (e.g. only when the screen content changes), and encoding may not keep up with them. In both cases only the
/// most recent frame is kept: older ones are dropped instead of piling up.
class VideoEncoder : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(VideoEncoder)

public:
	// These values are still hardcoded. This should probably be a setting.
	// For now these values seem alright for testing
	static constexpr int FPS                  = 15;
	static constexpr qint64 FRAME_INTERVAL_US = 1'000'000 / FPS;
	/// Target bit rate in bits per second
	static constexpr int BITRATE = 1'500'000;

	explicit VideoEncoder(QObject *parent = nullptr);
	~VideoEncoder() override;

	/// Starts a new stream. Timestamps passed to submitFrame() are relative to the given clock.
	void start(const QElapsedTimer &streamClock);
	/// Ends the stream. Frames that have not been encoded yet are dropped.
	void stop();

	/// Hands over a captured image to be encoded.
	/// @param captureTime  Capture time in microseconds on the stream clock passed to start().
	void submitFrame(const QImage &image, qint64 captureTime);

	/// Makes the encoder emit a key frame as soon as possible, so that viewers who lost part of the stream can
	/// start decoding again. Requests that come in shortly after a key frame are held back for a moment, which
	/// limits how many key frames are sent no matter how many viewers ask for them.
	void requestKeyFrame();

signals:
	/// Emitted for every successfully encoded frame.
	void frameEncoded(const EncodedVideoFrame &frame);

private:
	void processStart(const QElapsedTimer &streamClock);
	void processStop();
	void processKeyFrameRequest();
	/// Encodes the incoming frame if the frame rate allows it, or schedules it for the next frame slot.
	void processIncomingFrame();

	/// Makes the next frame a key frame, encoding the last frame again if no new one is on its way.
	void forceKeyFrame();
	void encodeImage(const QImage &srcImage, qint64 captureTime);
	bool initEncoder(int width, int height);
	void destroyEncoder();

	qint64 now() const;

	/// The most recent frame that was submitted but not yet encoded. Shared with the submitting thread, so it
	/// is protected by m_incomingMutex. The encoder's thread is only woken up when it goes from empty to set.
	QMutex m_incomingMutex;
	QImage m_incomingFrame;
	qint64 m_incomingCaptureTime = 0;

	// Everything below is only used on the encoder's thread.

	bool m_running = false;
	QElapsedTimer m_streamClock;
	quint64 m_frameNumber = 0;
	/// Timestamp of the last frame handed to the encoder, used to keep pts strictly increasing.
	qint64 m_lastPts = -1;

	/// Fires when the next frame slot opens up while a frame is pending.
	QTimer *m_frameRateTimer = nullptr;
	/// Time at which the last frame was handed to the encoder, or -1 if none was yet.
	qint64 m_lastEncodeTime = -1;
	/// The frame that was last handed to the encoder, kept to be encoded again as a requested key frame.
	QImage m_lastFrame;

	/// Set when the next frame shall be encoded as a key frame.
	bool m_keyFrameRequested = false;
	/// Time at which the last key frame was emitted, or -1 if none was yet.
	qint64 m_lastKeyFrameTime = -1;
	/// Fires when a held back key frame request may be served.
	QTimer *m_keyFrameTimer = nullptr;

	AVCodecContext *m_codecCtx = nullptr;
	AVFrame *m_frame           = nullptr;
	AVPacket *m_packet         = nullptr;
	SwsContext *m_swsCtx       = nullptr;
	int m_encoderWidth         = 0;
	int m_encoderHeight        = 0;
};

#endif // MUMBLE_MUMBLE_VIDEOENCODER_H_
