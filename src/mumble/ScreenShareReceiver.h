// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCREENSHARERECEIVER_H_
#define MUMBLE_MUMBLE_SCREENSHARERECEIVER_H_

#include "MumbleProtocol.h"
#include "MumbleUDP.pb.h"

#include <QtCore/QElapsedTimer>
#include <QtCore/QObject>
#include <QtGui/QImage>

#include <cstdint>
#include <deque>
#include <map>
#include <vector>

#ifdef USE_SCREEN_SHARING
extern "C" {
#	include <libavcodec/avcodec.h>
#	include <libswscale/swscale.h>
}
#endif

class QTimer;

/// Reassembles UDP video fragments and decodes video frames.
///
/// The receiver is meant to live on its own thread (see QObject::moveToThread()). handleVideoPacket() and
/// resetSender() may be called from any thread: they only queue the work, which is then carried out on the
/// receiver's thread. This keeps all reassembly and decoder state on a single thread and keeps decoding off
/// both the network and the GUI thread. frameDecoded() is emitted from the receiver's thread, so it has to be
/// connected with a queued connection to deliver frames to the GUI.
///
/// Frames are not shown the moment they arrive. Each sender's capture timestamps are mapped onto the local
/// clock and every frame is emitted at its scheduled display time, behind a small jitter buffer whose size
/// adapts to how much the arrival times vary. Frames that are already late by the time a newer frame is due
/// are skipped, so playback catches up instead of falling behind. All of this only relies on frame numbers
/// and timestamps, so it is independent of the codec in use.
///
/// When a frame is lost, the following frames can't be decoded until the next key frame. Instead of waiting
/// for the sender's next periodic key frame, keyFrameNeeded() is emitted so that one can be requested.
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
	/// Emitted from the receiver's thread when the stream of the given sender can't be decoded until its next
	/// key frame. While that is the case, it is emitted again every now and then.
	void keyFrameNeeded(quint32 senderSession);

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
		quint32 receivedCount = 0;
		std::vector< QByteArray > fragments;
		bool isKeyFrame               = false;
		quint32 width                 = 0;
		quint32 height                = 0;
		MumbleUDP::Video::Codec codec = MumbleUDP::Video::H264;
		quint64 timestamp             = 0;

		bool isComplete() const { return fragmentCount > 0 && receivedCount == fragmentCount; }
	};

	/// Maps a sender's capture timestamps onto the local clock (all values in microseconds).
	///
	/// The transit time of a frame is its local arrival time minus its capture timestamp. Since the two clocks
	/// have unrelated epochs, only differences between transit times are meaningful. The smallest transit time
	/// seen recently is taken as the base (the frame that got through the network fastest), and how much later
	/// than that frames tend to arrive determines how much buffering is needed.
	class PlayoutClock {
	public:
		/// Feeds the arrival time of a completely received frame into the model.
		void update(quint64 timestamp, qint64 arrivalTime);
		/// Local time at which the frame with the given capture timestamp should be displayed. Like all times
		/// derived from timestamps, it may have wrapped around and has to be compared by difference.
		quint64 displayTime(quint64 timestamp) const;

	private:
		/// The smaller of the minimum transit times of both windows
		quint64 baseTransit() const;

		bool m_valid = false;
		/// The base transit time is the minimum over the current and the previous window, so that it can
		/// follow slow changes (e.g. clock drift or a route change) instead of sticking to an old minimum.
		qint64 m_windowStart     = 0;
		quint64 m_minTransit     = 0;
		quint64 m_prevMinTransit = 0;
		double m_peakDelay       = 0;
	};

	struct DecodedFrame {
		quint64 displayTime = 0;
		QImage image;
	};

	struct SenderState {
		/// frame_number -> fragments received so far
		std::map< quint64, PendingFrame > pending;
		/// Frames are decoded strictly in frame_number order; this is the next one due.
		bool hasNextFrame     = false;
		quint64 nextFrame     = 0;
		quint64 lastTimestamp = 0;
		PlayoutClock clock;
		/// Decoded frames waiting for their display time, in display order.
		std::deque< DecodedFrame > decoded;
		/// Local time at which a key frame was last asked for, or -1 if none was yet.
		qint64 lastKeyFrameRequest = -1;
	};

	std::map< quint32, SenderState > m_senders;

	/// Local reference clock for arrival and display times.
	QElapsedTimer m_clock;
	/// Wakes the receiver up for the next display time or loss deadline.
	QTimer *m_timer = nullptr;

	qint64 now() const;
	/// Decodes whatever is ready, gives up on frames that are overdue and emits frames that are due.
	void processSender(quint32 session, SenderState &sender);
	void onTimer();
	void scheduleTimer();
	/// Asks for a key frame from the given sender, unless that was done only recently.
	void requestKeyFrame(quint32 session, SenderState &sender);

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
	void decodeCompleteFrame(quint32 session, SenderState &sender, const PendingFrame &frame);
#endif
};

#endif // MUMBLE_MUMBLE_SCREENSHARERECEIVER_H_
