// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include "SSL.h"
#include "Timer.h"
#include "Utils.h"
#include "crypto/CryptStateAES256GCM.h"
#include <string>

class TestAES256GCMCrypt : public QObject {
	Q_OBJECT
private slots:
	void authcrypt();
	void ivrepeat();
};

void TestAES256GCMCrypt::authcrypt() {
	for (int len = 1; len < 32; len++) {
		unsigned char rawkey[256 / 8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
										  0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
										  0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

		unsigned char iv[96 / 8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b };
		std::string rawkey_str   = std::string(reinterpret_cast< const char * >(rawkey), 256 / 8);
		std::string nonce_str    = std::string(reinterpret_cast< const char * >(iv), 96 / 8);
		CryptStateAES256GCM cs;
		cs.setKey(rawkey_str, nonce_str, nonce_str);

		STACKVAR(unsigned char, src, len);
		for (int i = 0; i < len; i++)
			src[i] = (i + 1);

		STACKVAR(unsigned char, encrypted, len + cs.headLength);
		STACKVAR(unsigned char, decrypted, len);

		unsigned int enc_len   = 0;
		unsigned int plain_len = 0;

		QVERIFY(cs.encrypt(src, encrypted, len, enc_len));
		QVERIFY(enc_len == len + cs.headLength);
		QVERIFY(cs.decrypt(encrypted, decrypted, enc_len, plain_len));
		QVERIFY(plain_len == (unsigned int) len);
	}
}

void TestAES256GCMCrypt::ivrepeat() {
	unsigned char rawkey[256 / 8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
									  0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
									  0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

	unsigned char iv[96 / 8] = { 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x08, 0x09, 0x0a, 0x0b };
	std::string rawkey_str   = std::string(reinterpret_cast< const char * >(rawkey), 256 / 8);
	std::string nonce_str    = std::string(reinterpret_cast< const char * >(iv), 96 / 8);
	CryptStateAES256GCM cs;
	cs.setKey(rawkey_str, nonce_str, nonce_str);

	unsigned char *src   = (unsigned char *) "The quick brown fox jumps over the lazy dog";
	int len              = strlen((char *) src);
	unsigned int enc_len = 0;

	STACKVAR(unsigned char, encrypted, len + cs.headLength);

	QVERIFY(cs.encrypt(src, encrypted, len, enc_len));
	QVERIFY(!cs.encrypt(src, encrypted, len, enc_len));
}

QTEST_MAIN(TestAES256GCMCrypt)
#include "TestAES256GCMCrypt.moc"
