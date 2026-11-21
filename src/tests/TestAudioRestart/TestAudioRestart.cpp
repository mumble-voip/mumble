// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioRestart.h"

#include <QCoreApplication>
#include <QEvent>
#include <QThread>
#include <QtTest>

#include <memory>
#include <thread>

// Like AudioInput/AudioOutput, the QThread object retains its creator's thread affinity.
class AudioInstance : public QThread {
public:
	AudioRestart m_restart;
};

class TestAudioRestart : public QObject {
	Q_OBJECT
private slots:
	void cancelOnReplacement();
	void coalesceNotifications();
	void followUpDuringRestart();
	void restartDestroysSource();
	void permissionRetryWithoutInput();
};

void TestAudioRestart::cancelOnReplacement() {
	int restarts = 0;
	auto input   = std::make_unique< AudioInstance >();
	input->m_restart.schedule(*input, [&restarts]() { ++restarts; });
	input.reset();
	input = std::make_unique< AudioInstance >();

	// An old pending request must neither restart the replacement nor suppress its own request.
	QCoreApplication::sendPostedEvents(nullptr, QEvent::MetaCall);
	QCOMPARE(restarts, 0);
	input->m_restart.schedule(*input, [&restarts]() { ++restarts; });
	QCOMPARE(restarts, 0);
	QCoreApplication::sendPostedEvents(nullptr, QEvent::MetaCall);
	QCOMPARE(restarts, 1);
}

void TestAudioRestart::coalesceNotifications() {
	AudioInstance output;
	int restarts           = 0;
	QThread *restartThread = nullptr;
	std::thread notificationThread([&]() {
		for (int event = 0; event < 20; ++event) {
			output.m_restart.schedule(output, [&]() {
				++restarts;
				restartThread = QThread::currentThread();
			});
		}
	});
	notificationThread.join();
	QCOMPARE(restarts, 0);
	QCoreApplication::sendPostedEvents(nullptr, QEvent::MetaCall);
	QCOMPARE(restarts, 1);
	QCOMPARE(restartThread, QCoreApplication::instance()->thread());
}

void TestAudioRestart::followUpDuringRestart() {
	AudioInstance input;
	int restarts = 0;
	input.m_restart.schedule(input, [&]() {
		++restarts;
		std::thread notificationThread([&]() { input.m_restart.schedule(input, [&restarts]() { ++restarts; }); });
		notificationThread.join();
	});

	QCoreApplication::sendPostedEvents(nullptr, QEvent::MetaCall);
	QCOMPARE(restarts, 1);
	QCoreApplication::sendPostedEvents(nullptr, QEvent::MetaCall);
	QCOMPARE(restarts, 2);
}

void TestAudioRestart::restartDestroysSource() {
	auto output    = std::make_unique< AudioInstance >();
	bool restarted = false;
	output->m_restart.schedule(*output, [&]() {
		output.reset();
		restarted = true;
	});

	QCoreApplication::sendPostedEvents(nullptr, QEvent::MetaCall);
	QVERIFY(!output);
	QVERIFY(restarted);
}

void TestAudioRestart::permissionRetryWithoutInput() {
	AudioRestart permissionRestart;
	std::unique_ptr< AudioInstance > input;
	permissionRestart.schedule(*QCoreApplication::instance(), [&]() { input = std::make_unique< AudioInstance >(); });

	QVERIFY(!input);
	QCoreApplication::sendPostedEvents(nullptr, QEvent::MetaCall);
	QVERIFY(input);
}

QTEST_GUILESS_MAIN(TestAudioRestart)
#include "TestAudioRestart.moc"
