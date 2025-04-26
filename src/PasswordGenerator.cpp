// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PasswordGenerator.h"
#include "crypto/CryptographicRandom.h"

#include <QtCore/QVector>

// This is a password alphabet that tries to be human-friendly.
// Most cases of ambiguitiy have been removed.
//
// Removed characters:
// "0" (zero)
// "1" (one)
// "5" (five)
// "O" (uppercase O)
// "o" (lowercase O)
// "l" (lowercase L)
// "I" (uppercase i)
static char password_alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm',
									'n', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

									'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M',
									'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',

									'2', '3', '4', '6', '7', '8', '9' };

// Getter function for TestPasswordGenerator to get the
// alphabet we're using.
QVector< QChar > mumble_password_generator_alphabet() {
	QVector< QChar > v;
	size_t numAlphabetEntries = sizeof(password_alphabet);
	for (size_t i = 0; i < numAlphabetEntries; i++) {
		v << QChar(QLatin1Char(password_alphabet[i]));
	}
	return v;
}

QString PasswordGenerator::generatePassword(int length) {
	QByteArray buf(length, '\0');

	uint32_t numAlphabetEntries = sizeof(password_alphabet);

	for (int i = 0; i < length; i++) {
		uint32_t index = CryptographicRandom::uniform(numAlphabetEntries - 1);
		buf[i]         = password_alphabet[index];
	}

	return QString::fromLatin1(buf.constData(), buf.size());
}
