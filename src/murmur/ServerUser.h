// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_SERVERUSER_H_
#define MUMBLE_MURMUR_SERVERUSER_H_

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#include "ClientType.h"
#include "Connection.h"
#include "HostAddress.h"
#include "Mumble.pb.h"
#include "MumbleProtocol.h"
#include "ServerUserInfo.h"
#include "Timer.h"

#include <QtCore/QElapsedTimer>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QStringList>

#ifdef Q_OS_WIN
#	include <winsock2.h>
#else
#	include <sys/socket.h>
#endif

#include <atomic>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

// Unfortunately, this needs to be "large enough" to hold
// enough frames to account for both short-term and
// long-term "maladjustments".

#define N_BANDWIDTH_SLOTS 360

struct BandwidthRecord {
	int iRecNum;
	int iSum;
	Timer tFirst;
	Timer tIdleControl;
	unsigned short a_iBW[N_BANDWIDTH_SLOTS];
	Timer a_qtWhen[N_BANDWIDTH_SLOTS];
	mutable std::mutex qmMutex;

	BandwidthRecord();
	bool addFrame(int size, int maxpersec);
	int onlineSeconds() const;
	int idleSeconds() const;
	void resetIdleSeconds();
	int bandwidth() const;
};

struct WhisperTarget {
	struct Channel {
		unsigned int id;
		bool includeChildren;
		bool includeLinks;
		QString targetGroup;
	};

	std::vector< unsigned int > sessions;
	std::vector< WhisperTarget::Channel > channels;
};

/// Limits the bandwidth of a user's screen share.
///
/// This is a token bucket that is refilled at the allowed rate. It holds what may be sent within a couple of
/// seconds, as key frames are much larger than the frames in between: a stream that stays within the limit on
/// average still gets through, even though it exceeds it for a moment with every key frame.
///
/// Whole frames are dropped rather than single fragments, as a frame that misses a fragment can't be decoded and only
/// wastes the bandwidth of the fragments that got through.
class VideoBandwidthLimiter {
public:
	/// @param frameNumber The number of the frame the packet belongs to
	/// @param size The size of the packet in bytes
	/// @param maxBitsPerSecond The allowed bandwidth in bits per second, or 0 if there is no limit
	/// @returns Whether the packet may be relayed
	bool allow(std::uint64_t frameNumber, std::size_t size, unsigned int maxBitsPerSecond);

private:
	/// How much may be sent at once, as time at the allowed rate
	static constexpr double BURST_SECONDS = 2.0;

	std::mutex m_mutex;
	/// Time since the bucket was last refilled
	Timer m_lastRefill;
	/// Number of bytes that may be sent right now. Negative while a frame that was let through is paid back.
	double m_budget = 0;
	/// Whether a frame has been seen yet, i.e. whether m_frameNumber and m_frameAllowed are set
	bool m_hasFrame = false;
	/// The frame the last packet belonged to
	std::uint64_t m_frameNumber = 0;
	/// Whether the packets of that frame are let through
	bool m_frameAllowed = false;
};

class ServerUser;

struct WhisperTargetCache {
	QSet< ServerUser * > channelTargets;
	QSet< ServerUser * > directTargets;
	QHash< ServerUser *, VolumeAdjustment > listeningTargets;
};

class Server;

/// A simple implementation for rate-limiting.
/// See https://en.wikipedia.org/wiki/Leaky_bucket
class LeakyBucket {
private:
	/// The amount of tokens that are drained per second.
	/// (The size of the whole in the bucket)
	unsigned int m_tokensPerSec;
	/// The maximum amount of tokens that may be encountered.
	/// (The capacity of the bucket)
	unsigned int m_maxTokens;
	/// The amount of tokens currently stored
	/// (The amount of whatever currently is in the bucket)
	long m_currentTokens;
	/// A timer that is used to measure time intervals. It is essential
	/// that this timer uses a monotonic clock (which is why QElapsedTimer is
	/// used instead of QTime or QDateTime).
	/// TODO: Switch to Timer.
	QElapsedTimer m_timer;

public:
	/// @param tokens The amount of tokens that should be added.
	/// @returns Whether adding this amount of tokens triggers rate
	/// 	limiting (true means the corresponding packet has to be
	/// 	discared and false means the packet may be processed)
	bool ratelimit(int tokens);

	LeakyBucket(unsigned int tokensPerSec, unsigned int maxTokens);
};

class CryptState;

class ServerUser : public Connection, public ServerUserInfo {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ServerUser)
protected:
	Server *s;

	Timer m_lastActivityTimer;

public:
	enum State { Rejected, Connected, Authenticating, Authenticated };
	std::atomic< State > sState;
	std::atomic< bool > was_authenticated = false;
	ClientType m_clientType;
	operator QString() const;

	std::int64_t activityTime() const;
	void resetActivityTime();

	void sendMessage(const ::google::protobuf::Message &msg, Mumble::Protocol::TCPMessageType msgType);
	void sendMessage(const ::google::protobuf::Message &msg, Mumble::Protocol::TCPMessageType msgType,
					 QByteArray &cache);

	float dUDPPingAvg, dUDPPingVar;
	float dTCPPingAvg, dTCPPingVar;
	quint32 uiUDPPackets, uiTCPPackets;

	HostAddress haAddress;

	/// Holds whether the user is using TCP
	/// or UDP for voice packets.
	///
	/// If the flag is 0, the user is using
	/// TCP.
	///
	/// If the flag is 1, the user is using
	/// UDP.
	QAtomicInt aiUdpFlag;

	QList< int > qlCodecs;
	bool bOpus;
	/// The video capabilities the user's client announced, as they are relayed to other clients, or nothing if the
	/// client never announced them.
	std::optional< MumbleProto::UserState_VideoCapabilities > m_videoCapabilities;
	/// Video codecs (MumbleUDP::Video::Codec values) the user's client can decode, or nothing if the client
	/// never announced them.
	std::optional< std::vector< unsigned int > > m_videoDecoders;
	/// Sessions of the users whose screen share this user wants to receive. As this is used by the voice
	/// thread when relaying video, it is only changed while holding the voice thread lock.
	QSet< unsigned int > m_videoSubscriptions;

	QStringList qslAccessTokens;

	QMap< int, WhisperTarget > qmTargets;
	QMap< int, WhisperTargetCache > qmTargetCache;
	QMap< QString, QString > qmWhisperRedirect;

	LeakyBucket leakyBucket;
	LeakyBucket m_pluginMessageBucket;
	/// For video subscriptions and key frame requests, which clients send automatically (key frame requests up to
	/// twice a second per watched stream). Kept apart from leakyBucket so that they don't use up its budget for what
	/// the user does, and so that a dropped subscription, which the client can't notice, stays unlikely.
	LeakyBucket m_videoControlBucket;

	/// The frame of each sharing user's video that is currently being tunneled to this user through TCP, and whether
	/// it is sent or dropped. Only used by the main thread.
	struct TunneledVideoFrame {
		std::uint64_t frameNumber;
		bool send;
	};
	QHash< unsigned int, TunneledVideoFrame > m_tunneledVideoFrames;

	int iLastPermissionCheck;
	QMap< int, unsigned int > qmPermissionSent;
#ifdef Q_OS_UNIX
	int sUdpSocket;
#else
	SOCKET sUdpSocket;
#endif
	BandwidthRecord bwr;
	VideoBandwidthLimiter m_videoBandwidth;
	struct sockaddr_storage saiUdpAddress;
	struct sockaddr_storage saiTcpLocalAddress;

	/// qmCrypt locks access to csCrypt.
	std::mutex qmCrypt;
	std::unique_ptr< CryptState > csCrypt;

	ServerUser(Server *parent, QSslSocket *socket);
	~ServerUser();

	void rejectConnection(bool forceDisconnect = false);
};

#endif
