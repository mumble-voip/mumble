// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include <cmath>

/// Test the std::abs works with floats.
/// In fixing mumble-voip/mumble#3281, I
/// stumbled upon http://eigen.tuxfamily.org/bz/show_bug.cgi?id=619.
/// It seems that, on some platforms, std::abs
/// might not call through to the correct libc
/// function.
/// Test that it works for us.
class TestStdAbs : public QObject {
	Q_OBJECT
private slots:
	void floatWorks();
};

void TestStdAbs::floatWorks() {
	const float in = -1.5;
	float out      = std::abs(in);
	QVERIFY(out > 1.2 && out < 1.8);
}

QTEST_MAIN(TestStdAbs)
#include "TestStdAbs.moc"
