// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCREENCAPTURE_H_
#define MUMBLE_MUMBLE_SCREENCAPTURE_H_

#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <cstdint>

#ifdef USE_SCREEN_SHARING
extern "C" {
#	include <libavcodec/avcodec.h>
#	include <libavutil/opt.h>
#	include <libswscale/swscale.h>
}
#endif

/// Captures the primary display at ~15 fps and emits encoded video frames via frameEncoded().
///
/// Requires the build option -Dscreen-sharing=ON (links libavcodec/libswscale).
/// Without that option the class is still present but startCapture() logs a warning and
/// returns immediately without capturing anything.
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

signals:
	/// Emitted for every successfully encoded frame.
	/// @param encodedData  Codec-specific encoded byte stream (currently H.264 Annex-B).
	/// @param frameNumber  Monotonically increasing counter starting at 0.
	/// @param isKeyFrame  True when the frame is an IDR / key frame.
	void frameEncoded(QByteArray encodedData, quint64 frameNumber, bool isKeyFrame);

private slots:
	void captureFrame();

private:
#ifdef USE_SCREEN_SHARING
	bool initEncoder(int width, int height);
	void destroyEncoder();

	AVCodecContext *m_codecCtx = nullptr;
	AVFrame *m_frame           = nullptr;
	AVPacket *m_packet         = nullptr;
	SwsContext *m_swsCtx       = nullptr;
	int m_encoderWidth         = 0;
	int m_encoderHeight        = 0;
#endif

	QTimer *m_captureTimer = nullptr;
	quint64 m_frameNumber  = 0;
	bool m_capturing       = false;
};

#endif // MUMBLE_MUMBLE_SCREENCAPTURE_H_
