// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioReceiverBuffer.h"
#include "MumbleProtocol.h"
#include "Version.h"

#include <QObject>
#include <QtTest>

#include <array>
#include <unordered_set>

#include <QDebug>

QDebug &operator<<(QDebug &stream, const ServerUser &user) {
	return stream.nospace() << "ServerUser{ session: " << user.uiSession
							<< ", version: " << Version::toString(user.m_version) << ", deaf: " << user.bDeaf
							<< ", selfDeaf: " << user.bSelfDeaf
							<< ", ssContext: " << QString::fromStdString(user.ssContext) << " }";
}

QDebug &operator<<(QDebug &stream, const AudioReceiver &receiver) {
	return stream.nospace() << "AudioReceiver{ receiver: " << receiver.getReceiver()
							<< ", context: " << receiver.getContext()
							<< ", volAdj: " << receiver.getVolumeAdjustment().factor << " }";
}

bool operator<(const AudioReceiver &lhs, const AudioReceiver &rhs) {
	// session IDs are supposed to be unique, so sorting by them should yield a unique ordering
	return lhs.getReceiver().uiSession < rhs.getReceiver().uiSession;
}


Version::full_t vOld1 = Version::fromComponents(1, 2, 5);
Version::full_t vOld2 = Version::fromComponents(1, 3, 1);
Version::full_t vOld3 = Version::fromComponents(1, 4, 0);
Version::full_t vNew  = Mumble::Protocol::PROTOBUF_INTRODUCTION_VERSION;

std::array< ServerUser, 6 > users = { ServerUser(0, vOld1), ServerUser(1, vOld2), ServerUser(2, vOld3),
									  ServerUser(3, vNew),  ServerUser(4, vNew),  ServerUser(5, vNew) };

ServerUser deafUser(6, vOld1, true);
ServerUser selfDeafUser(7, vNew, false, true);
ServerUser contextUser1(8, vNew, false, false, "context1");
ServerUser contextUser2(9, vNew, false, false, "context2");
ServerUser contextUser3(10, vNew, false, false, "context1");


struct Range {
	Range(ServerUser *begin, ServerUser *end) : begin(begin), end(end){};

	ServerUser *begin;
	ServerUser *end;
};

class PseudoEncoder {
public:
	PseudoEncoder() = default;

	bool checkRequiresEncoding(Version::full_t protocolVersion, Mumble::Protocol::audio_context_t context,
							   float volumeAdjustment) {
		bool requiresEncoding = m_encodings == 0
								|| !Mumble::Protocol::protocolVersionsAreCompatible(m_protocolVersion, protocolVersion)
								|| m_context != context || m_volumeAdjustment != volumeAdjustment;

		if (requiresEncoding) {
			m_encodings++;

			m_protocolVersion  = protocolVersion;
			m_context          = context;
			m_volumeAdjustment = volumeAdjustment;
		}

		return requiresEncoding;
	}

	std::size_t getAmountOfEncodings() const { return m_encodings; }

	void reset() { m_encodings = 0; }

protected:
	std::size_t m_encodings                     = 0;
	Version::full_t m_protocolVersion           = Version::UNKNOWN;
	Mumble::Protocol::audio_context_t m_context = Mumble::Protocol::AudioContext::INVALID;
	float m_volumeAdjustment                    = 0.0f;
};

class TestAudioReceiverBuffer : public QObject {
	Q_OBJECT
private slots:
	void test_preconditions() {
		// Preconditions for these test to make any sense
		QVERIFY(Mumble::Protocol::protocolVersionsAreCompatible(vOld1, vOld2));
		QVERIFY(Mumble::Protocol::protocolVersionsAreCompatible(vOld2, vOld3));
		QVERIFY(Mumble::Protocol::protocolVersionsAreCompatible(vOld1, vOld3));
		QVERIFY(!Mumble::Protocol::protocolVersionsAreCompatible(vOld1, vNew));
		QVERIFY(!Mumble::Protocol::protocolVersionsAreCompatible(vOld2, vNew));
		QVERIFY(!Mumble::Protocol::protocolVersionsAreCompatible(vOld3, vNew));

		// Make sure we are not accidentally using duplicate IDs for our dummy users
		std::unordered_set< unsigned int > usedIDs;
		for (const ServerUser &current : users) {
			QVERIFY(usedIDs.find(current.uiSession) == usedIDs.end());
			usedIDs.insert(current.uiSession);
		}
		for (const ServerUser &current : { deafUser, selfDeafUser, contextUser1, contextUser2, contextUser3 }) {
			QVERIFY(usedIDs.find(current.uiSession) == usedIDs.end());
			usedIDs.insert(current.uiSession);
		}

		QVERIFY(Mumble::Protocol::AudioContext::NORMAL < Mumble::Protocol::AudioContext::WHISPER);
		QVERIFY(Mumble::Protocol::AudioContext::NORMAL < Mumble::Protocol::AudioContext::SHOUT);
		QVERIFY(Mumble::Protocol::AudioContext::NORMAL < Mumble::Protocol::AudioContext::LISTEN);

		QVERIFY(contextUser1.ssContext == contextUser3.ssContext);
		QVERIFY(contextUser1.ssContext != contextUser2.ssContext);
	}

	void test_addReceiver() {
		AudioReceiverBuffer buffer;

		ServerUser &sender = users[0];

		buffer.addReceiver(sender, sender, Mumble::Protocol::AudioContext::LISTEN, false);
		buffer.addReceiver(sender, users[1], Mumble::Protocol::AudioContext::WHISPER, false);
		buffer.addReceiver(sender, users[2], Mumble::Protocol::AudioContext::SHOUT, false);
		buffer.addReceiver(sender, contextUser1, Mumble::Protocol::AudioContext::SHOUT, false);
		buffer.addReceiver(sender, selfDeafUser, Mumble::Protocol::AudioContext::SHOUT, false);
		buffer.addReceiver(sender, deafUser, Mumble::Protocol::AudioContext::SHOUT, false);

		QCOMPARE(buffer.getReceivers(false).size(), static_cast< std::size_t >(3));
		QVERIFY(buffer.getReceivers(true).empty());
	}

	void test_addReceiverPositional() {
		AudioReceiverBuffer buffer;

		ServerUser &sender = contextUser1;

		buffer.addReceiver(sender, users[0], Mumble::Protocol::AudioContext::NORMAL, true);
		buffer.addReceiver(sender, users[1], Mumble::Protocol::AudioContext::NORMAL, true);
		buffer.addReceiver(sender, contextUser2, Mumble::Protocol::AudioContext::NORMAL, true);
		buffer.addReceiver(sender, contextUser3, Mumble::Protocol::AudioContext::NORMAL, true);

		// There is only one receiver whose context matches that of the sender
		QCOMPARE(buffer.getReceivers(true).size(), static_cast< std::size_t >(1));
		// All other receivers will get the audio without positional data
		QCOMPARE(buffer.getReceivers(false).size(), static_cast< std::size_t >(3));
	}

	void test_forceAddReceiver() {
		AudioReceiverBuffer buffer;

		ServerUser &sender = users[0];

		buffer.forceAddReceiver(sender, Mumble::Protocol::AudioContext::NORMAL, false);

		QCOMPARE(buffer.getReceivers(false).size(), static_cast< std::size_t >(1));
		QVERIFY(buffer.getReceivers(true).empty());
	}

	void test_preprocessBuffer() {
		AudioReceiverBuffer buffer;

		ServerUser &sender = users[0];

		buffer.addReceiver(sender, users[3], Mumble::Protocol::AudioContext::LISTEN, false,
						   VolumeAdjustment::fromFactor(1.2f));
		buffer.addReceiver(sender, users[1], Mumble::Protocol::AudioContext::WHISPER, false);
		buffer.addReceiver(sender, users[1], Mumble::Protocol::AudioContext::NORMAL, false);
		buffer.addReceiver(sender, users[2], Mumble::Protocol::AudioContext::SHOUT, false);
		buffer.addReceiver(sender, users[1], Mumble::Protocol::AudioContext::LISTEN, false);
		buffer.addReceiver(sender, users[1], Mumble::Protocol::AudioContext::SHOUT, false);
		buffer.addReceiver(sender, users[3], Mumble::Protocol::AudioContext::LISTEN, false,
						   VolumeAdjustment::fromFactor(1.4f));
		buffer.addReceiver(sender, contextUser1, Mumble::Protocol::AudioContext::SHOUT, false);

		buffer.preprocessBuffer();

		// The preprocessing should have removed all duplicates of users[1] and users[3]
		QCOMPARE(buffer.getReceivers(false).size(), static_cast< std::size_t >(4));
		QVERIFY(buffer.getReceivers(true).empty());

		const AudioReceiver *duplicateReceiver = nullptr;
		const AudioReceiver *volumeReceiver    = nullptr;
		for (const AudioReceiver &current : buffer.getReceivers(false)) {
			if (current.getReceiver().uiSession == users[1].uiSession) {
				duplicateReceiver = &current;
			} else if (current.getReceiver().uiSession == users[3].uiSession) {
				volumeReceiver = &current;
			}
		}

		QVERIFY(duplicateReceiver != nullptr);
		QVERIFY(volumeReceiver != nullptr);

		// Verify that the "Normal" speech receiver has survived (instead of one of the other contexts)
		QCOMPARE(duplicateReceiver->getContext(), Mumble::Protocol::AudioContext::NORMAL);
		// Verify that the highest volume adjustment has survived
		QCOMPARE(volumeReceiver->getVolumeAdjustment().factor, 1.4f);
	}

	void test_getReceiverRange() {
		AudioReceiverBuffer buffer;

		ServerUser &sender = contextUser2;

		buffer.addReceiver(sender, users[0], Mumble::Protocol::AudioContext::NORMAL, false,
						   VolumeAdjustment::fromFactor(1.22f));
		buffer.addReceiver(sender, users[1], Mumble::Protocol::AudioContext::NORMAL, false,
						   VolumeAdjustment::fromFactor(1.22f + 0.6f * AudioReceiverBuffer::maxFactorDiff));
		buffer.addReceiver(sender, users[2], Mumble::Protocol::AudioContext::NORMAL, false,
						   VolumeAdjustment::fromFactor(1.22f + 1.1f * AudioReceiverBuffer::maxFactorDiff));
		buffer.addReceiver(sender, users[3], Mumble::Protocol::AudioContext::NORMAL, false,
						   VolumeAdjustment::fromDBAdjustment(-60));
		buffer.addReceiver(sender, users[4], Mumble::Protocol::AudioContext::NORMAL, false,
						   VolumeAdjustment::fromDBAdjustment(-60 + AudioReceiverBuffer::maxDecibelDiff - 1));
		buffer.addReceiver(sender, users[5], Mumble::Protocol::AudioContext::NORMAL, false,
						   VolumeAdjustment::fromDBAdjustment(-60 + AudioReceiverBuffer::maxDecibelDiff));

		buffer.preprocessBuffer();

		std::vector< AudioReceiver > receivers = buffer.getReceivers(false);
		auto receiverRange = AudioReceiverBuffer::getReceiverRange(receivers.begin(), receivers.end());

		std::array< int, 4 > expectedGroupSizes = { 2, 1, 2, 1 };

		for (std::size_t i = 0; i < expectedGroupSizes.size(); ++i) {
			QVERIFY(receiverRange.begin != receiverRange.end);
			QCOMPARE(std::distance(receiverRange.begin, receiverRange.end), expectedGroupSizes.at(i));
			receiverRange = AudioReceiverBuffer::getReceiverRange(receiverRange.end, receivers.end());
		}

		QVERIFY(receiverRange.begin == receiverRange.end);
	}


	void test_encoding() {
		AudioReceiverBuffer buffer;

		ServerUser &sender = contextUser2;

		buffer.addReceiver(sender, users[3], Mumble::Protocol::AudioContext::SHOUT, false);
		buffer.addReceiver(sender, users[1], Mumble::Protocol::AudioContext::LISTEN, false);
		buffer.addReceiver(sender, users[4], Mumble::Protocol::AudioContext::SHOUT, false);
		buffer.addReceiver(sender, users[2], Mumble::Protocol::AudioContext::NORMAL, false);
		buffer.addReceiver(sender, users[0], Mumble::Protocol::AudioContext::NORMAL, false);
		buffer.addReceiver(sender, contextUser1, Mumble::Protocol::AudioContext::SHOUT, false,
						   VolumeAdjustment::fromFactor(1.4f));
		buffer.addReceiver(sender, contextUser2, Mumble::Protocol::AudioContext::SHOUT, false,
						   VolumeAdjustment::fromFactor(1.2f));
		buffer.addReceiver(sender, contextUser3, Mumble::Protocol::AudioContext::SHOUT, false,
						   VolumeAdjustment::fromFactor(1.2f));

		buffer.preprocessBuffer();

		std::vector< AudioReceiver > receivers = buffer.getReceivers(false);
		auto receiverRange = AudioReceiverBuffer::getReceiverRange(receivers.begin(), receivers.end());

		std::size_t processedReceiver = 0;
		PseudoEncoder encoder;

		while (receiverRange.begin != receiverRange.end) {
			qWarning("Opening a new range");
			qWarning() << "Start:" << *receiverRange.begin;
			qWarning() << "End:" << *(receiverRange.end - 1);

			QVERIFY2(encoder.checkRequiresEncoding(receiverRange.begin->getReceiver().m_version,
												   receiverRange.begin->getContext(),
												   receiverRange.begin->getVolumeAdjustment().factor),
					 "Starting a new range, but no re-encoding is required");

			for (auto it = receiverRange.begin; it != receiverRange.end; ++it) {
				qWarning() << "Processing" << *it;
				QVERIFY2(!encoder.checkRequiresEncoding(it->getReceiver().m_version, it->getContext(),
														it->getVolumeAdjustment().factor),
						 "Mid-range re-encoding required");
				processedReceiver++;
			}

			receiverRange = AudioReceiverBuffer::getReceiverRange(receiverRange.end, receivers.end());
		}

		QCOMPARE(processedReceiver, receivers.size());

		std::size_t requiredReencodings = encoder.getAmountOfEncodings();

		// Check all permutations of receivers in order to check that the one created by preprocessBuffer is in fact the
		// one that minimizes the amount of re-encodings that are required in order to send the audio to all receivers.
		do {
			encoder.reset();

			for (const AudioReceiver &current : receivers) {
				encoder.checkRequiresEncoding(current.getReceiver().m_version, current.getContext(),
											  current.getVolumeAdjustment().factor);
			}

			QVERIFY2(encoder.getAmountOfEncodings() >= requiredReencodings,
					 "There exists a permutation of the receivers, that requires less re-encoding steps");
		} while (std::next_permutation(receivers.begin(), receivers.end()));

		qDebug() << "Sample receiver list required" << requiredReencodings << "encoding steps";
	}

	void test_emptyRange() {
		AudioReceiverBuffer buffer;

		std::vector< AudioReceiver > receivers = buffer.getReceivers(true);

		QVERIFY(receivers.empty());

		ReceiverRange< std::vector< AudioReceiver >::iterator > range =
			AudioReceiverBuffer::getReceiverRange(receivers.begin(), receivers.end());

		QCOMPARE(range.begin, range.end);
	}
};

QTEST_MAIN(TestAudioReceiverBuffer)
#include "TestAudioReceiverBuffer.moc"
