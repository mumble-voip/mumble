// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCREENSHARERECEIVER_H_
#define MUMBLE_MUMBLE_SCREENSHARERECEIVER_H_

#include "MumbleProtocol.h"
#include "MumbleUDP.pb.h"

#include <QtCore/QObject>
#include <QtGui/QImage>

#include <cstdint>
#include <map>
#include <vector>

#ifdef USE_SCREEN_SHARING
extern "C" {
#	include <libavcodec/avcodec.h>
#	include <libswscale/swscale.h>
}
#endif

/// Reassembles UDP video fragments and decodes video frames.
///
/// Thread-safe to call handleVideoPacket() from a non-GUI thread;
/// frameDecoded() is emitted via a queued connection and delivered on the
/// GUI thread when connected with Qt::QueuedConnection.
class ScreenShareReceiver : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ScreenShareReceiver)

public:
	explicit ScreenShareReceiver(QObject *parent = nullptr);
	~ScreenShareReceiver() override;

	/// Called (potentially from the ServerHandler thread) for every incoming Video UDP message.
	void handleVideoPacket(const Mumble::Protocol::VideoData &videoData);

	/// Tear down decoder state for a sender who stopped sharing.
	void resetSender(quint32 senderSession);

signals:
	void frameDecoded(quint32 senderSession, QImage frame);

private:
#ifdef USE_SCREEN_SHARING
	struct PendingFrame {
		quint32 fragmentCount = 0;
		std::vector< QByteArray > fragments;
		bool isKeyFrame               = false;
		quint32 width                 = 0;
		quint32 height                = 0;
		MumbleUDP::Video::Codec codec = MumbleUDP::Video::H264;
	};

	/// sender_session -> frame_number -> pending fragment data
	std::map< quint32, std::map< quint64, PendingFrame > > m_fragmentBuffer;

	struct DecoderState {
		AVCodecContext *codecCtx      = nullptr;
		AVFrame *frame                = nullptr;
		AVPacket *packet              = nullptr;
		SwsContext *swsCtx            = nullptr;
		int swsWidth                  = 0;
		int swsHeight                 = 0;
		MumbleUDP::Video::Codec codec = MumbleUDP::Video::H264;
		/// Drop P-frames until the decoder has seen at least one IDR keyframe.
		bool gotKeyFrame = false;
	};
	std::map< quint32, DecoderState > m_decoders;

	bool ensureDecoder(quint32 session, MumbleUDP::Video::Codec codec);
	void destroyDecoder(quint32 session);
	void decodeCompleteFrame(quint32 session, const QByteArray &encodedData, quint32 width, quint32 height,
							 bool isKeyFrame, MumbleUDP::Video::Codec codec);
#endif
};

#endif // MUMBLE_MUMBLE_SCREENSHARERECEIVER_H_
