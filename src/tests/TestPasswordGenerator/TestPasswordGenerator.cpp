// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include "PasswordGenerator.h"

// Get the password alphabet from PasswordGenerator.
extern QVector<QChar> mumble_password_generator_alphabet();

class TestPasswordGenerator : public QObject {
		Q_OBJECT
	private slots:
		void random();
};

void TestPasswordGenerator::random() {
	QVector<QChar> alphabet = mumble_password_generator_alphabet();
	for (int i = 0; i < 100; i++) {
		QString out = PasswordGenerator::generatePassword(i);
		QVERIFY(out.size() == i);
		for (int j = 0; j < out.size(); j++) {
			QVERIFY(alphabet.contains(out.at(j)));
		}
	}
}

QTEST_MAIN(TestPasswordGenerator)
#include "TestPasswordGenerator.moc"
