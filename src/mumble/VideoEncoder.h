// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VIDEOENCODER_H_
#define MUMBLE_MUMBLE_VIDEOENCODER_H_

#include "ScreenCapture.h"
#include "VideoEncoderBackend.h"

#include <QtCore/QElapsedTimer>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtGui/QImage>

#include <atomic>
#include <memory>

class QTimer;

/// Turns captured images into an encoded video stream.
///
/// The encoder is meant to live on its own thread (see QObject::moveToThread()), so that colour conversion and
/// encoding don't block the GUI. All public methods may be called from any thread: they only hand the work over,
/// which is then carried out on the encoder's thread. frameEncoded() is emitted from the encoder's thread.
///
/// The actual encoding is done by a VideoEncoderBackend. Encoders are tried in order of preference (see
/// setSelection()), and the first one that can be opened for the captured picture size is used. When the preferred
/// encoder changes while streaming (e.g. because a viewer joined that can't decode the current codec), the stream
/// switches over to it, starting with a key frame.
///
/// Frames are limited to the target frame rate. Capture sources may deliver frames much faster and irregularly
/// (e.g. only when the screen content changes), and encoding may not keep up with them. In both cases only the
/// most recent frame is kept: older ones are dropped instead of piling up.
class VideoEncoder : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(VideoEncoder)

public:
	/// Range of the frame rate in frames per second (see setFrameRate())
	static constexpr int MIN_FRAME_RATE     = 1;
	static constexpr int MAX_FRAME_RATE     = 60;
	static constexpr int DEFAULT_FRAME_RATE = 15;
	/// Target bit rate in bits per second
	static constexpr int BITRATE = 1'500'000;

	/// Time between two frames at the given frame rate, in microseconds.
	static qint64 frameInterval(int frameRate);

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

	/// Sets what the choice of the encoder is based on. The selection is kept across streams.
	void setSelection(const VideoEncoderSelection &selection);

	/// Sets the maximum number of frames per second, clamped to MIN_FRAME_RATE..MAX_FRAME_RATE. With a higher
	/// frame rate, motion is smoother, but every frame gets fewer bits, so details get blurrier. When streaming,
	/// the encoder is opened again with the new frame rate, starting with a key frame. The frame rate is kept
	/// across streams.
	void setFrameRate(int frameRate);

signals:
	/// Emitted for every successfully encoded frame.
	void frameEncoded(const EncodedVideoFrame &frame);
	/// Emitted from the encoder's thread when no encoder could be opened for the stream. The stream has been stopped
	/// then, as further frames would fail the same way.
	void failed();

private:
	void processStart(const QElapsedTimer &streamClock);
	void processStop();
	void processKeyFrameRequest();
	void processSelection(const VideoEncoderSelection &selection);
	void processFrameRate(int frameRate);
	/// Encodes the incoming frame if the frame rate allows it, or schedules it for the next frame slot.
	void processIncomingFrame();

	/// Makes the next frame a key frame, encoding the last frame again if no new one is on its way.
	void forceKeyFrame();
	/// Encodes the last frame again if no new one is on its way, so that viewers notice lost frames even while
	/// the screen doesn't change.
	void sendHeartbeat();
	void encodeImage(const QImage &srcImage, qint64 captureTime);
	/// Opens the most preferred encoder that works for the given picture size and format. Afterwards,
	/// m_encoderWidth, m_encoderHeight and m_encoderFormat are set to the given values, whether that worked or not.
	bool openBackend(int width, int height, QImage::Format format);
	/// Whether encoders take pictures in the given format (see VideoEncoderConfig::inputFormat)
	static bool isEncodable(QImage::Format format);

	qint64 now() const;

	/// The most recent frame that was submitted but not yet encoded. Shared with the submitting thread, so it
	/// is protected by m_incomingMutex. The encoder's thread is only woken up when it goes from empty to set.
	QMutex m_incomingMutex;
	QImage m_incomingFrame;
	qint64 m_incomingCaptureTime = 0;
	/// Counts the calls to start() and stop(). Unlike m_running, it changes as soon as they are called, so that
	/// frames that are still being encoded at that point aren't emitted anymore.
	std::atomic< quint64 > m_stream{ 0 };

	// Everything below is only used on the encoder's thread.

	bool m_running = false;
	/// Value of m_stream for the stream that is being encoded
	quint64 m_currentStream = 0;
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
	/// Timestamp of the picture that was to be encoded as a key frame, until its packet came out, or -1
	qint64 m_forcedKeyFrameTimestamp = -1;
	/// Fires when a held back key frame request may be served.
	QTimer *m_keyFrameTimer = nullptr;
	/// Fires when no frame was encoded for a while, see sendHeartbeat()
	QTimer *m_heartbeatTimer = nullptr;

	/// IDs of the encoders to use, most preferred first
	QStringList m_encoderOrder;
	int m_frameRate = DEFAULT_FRAME_RATE;
	std::unique_ptr< VideoEncoderBackend > m_backend;
	/// Picture size m_backend was opened for (or tried to)
	int m_encoderWidth             = 0;
	int m_encoderHeight            = 0;
	QImage::Format m_encoderFormat = QImage::Format_Invalid;
	/// The encoder that was used last, so that switching to a different one can be reported
	QString m_lastEncoderId;
	/// Encoders that failed to encode a picture during the current stream, which are not opened again
	QStringList m_failedEncoders;
};

#endif // MUMBLE_MUMBLE_VIDEOENCODER_H_
