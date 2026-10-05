// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VIDEOSENDER_H_
#define MUMBLE_MUMBLE_VIDEOSENDER_H_

#include <QtCore/QElapsedTimer>
#include <QtCore/QObject>

#include <cstddef>
#include <deque>
#include <vector>

class QTimer;
struct EncodedVideoFrame;

/// Splits encoded video frames into UDP-sized fragments and sends them to the server at a steady pace.
///
/// A key frame can easily be a hundred times larger than the frames in between. Sending all of its fragments at
/// once creates a burst that overflows queues along the way (in the local network stack, in routers or on the
/// server), and losing a single fragment makes the whole key frame useless. Hence fragments are queued and sent
/// at a rate somewhat above the encoder's target bit rate, which spreads large frames out over a short time
/// while letting the regular frames through without delay. Should the queue grow anyway, e.g. because the
/// encoder overshot its target, the rate goes up so that nothing waits for longer than a fixed amount of time.
///
/// The sender is meant to live on its own thread (see QObject::moveToThread()), so that sending stays steady
/// even while the GUI or the encoder are busy. All public methods may be called from any thread: they only queue
/// the work, which is then carried out on the sender's thread.
class VideoSender : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(VideoSender)

public:
	explicit VideoSender(QObject *parent = nullptr);
	~VideoSender() override;

	/// Queues the given frame to be sent.
	void sendFrame(const EncodedVideoFrame &frame);
	/// Drops everything that has not been sent yet, e.g. because sharing stopped.
	void reset();

private:
	void processFrame(const EncodedVideoFrame &frame);
	void processReset();
	/// Sends as many queued fragments as the current budget allows.
	void sendDue();

	qint64 now() const;

	/// Serialized UDP messages waiting to be sent, in order
	std::deque< std::vector< unsigned char > > m_queue;
	std::size_t m_queuedBytes = 0;

	/// Number of bytes that may be sent right now. Refilled according to the pacing rate, it may become
	/// slightly negative as whole fragments are sent.
	double m_budget = 0;
	/// Time at which m_budget was last refilled, in microseconds on m_clock
	qint64 m_lastRefill = 0;
	QElapsedTimer m_clock;
	/// Runs while fragments are waiting
	QTimer *m_timer = nullptr;
};

#endif // MUMBLE_MUMBLE_VIDEOSENDER_H_
