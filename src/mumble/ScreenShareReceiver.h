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
/// The receiver is meant to live on its own thread (see QObject::moveToThread()). handleVideoPacket() and
/// resetSender() may be called from any thread: they only queue the work, which is then carried out on the
/// receiver's thread. This keeps all reassembly and decoder state on a single thread and keeps decoding off
/// both the network and the GUI thread. frameDecoded() is emitted from the receiver's thread, so it has to be
/// connected with a queued connection to deliver frames to the GUI.
class ScreenShareReceiver : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ScreenShareReceiver)

public:
	explicit ScreenShareReceiver(QObject *parent = nullptr);
	~ScreenShareReceiver() override;

	/// Called (potentially from the ServerHandler thread) for every incoming Video UDP message.
	void handleVideoPacket(const Mumble::Protocol::VideoData &videoData);

	/// Tear down decoder state for a sender who stopped sharing. May be called from any thread.
	void resetSender(quint32 senderSession);

	/// The video codecs that can be decoded. This opens a decoder for every codec, so it should not be called
	/// more often than necessary.
	static std::vector< MumbleUDP::Video::Codec > supportedCodecs();

signals:
	void frameDecoded(quint32 senderSession, QImage frame);

private:
#ifdef USE_SCREEN_SHARING
	/// Owning copy of a Mumble::Protocol::VideoData, whose payload only points into the network buffer.
	struct VideoPacket {
		quint32 senderSession         = 0;
		MumbleUDP::Video::Codec codec = MumbleUDP::Video::H264;
		quint32 width                 = 0;
		quint32 height                = 0;
		quint64 frameNumber           = 0;
		quint32 fragmentIndex         = 0;
		quint32 fragmentCount         = 0;
		QByteArray payload;
		bool isKeyFrame   = false;
		quint64 timestamp = 0;
	};

	void processPacket(const VideoPacket &packet);
	void processReset(quint32 senderSession);

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
