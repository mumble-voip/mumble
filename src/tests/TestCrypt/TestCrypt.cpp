// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include "SSL.h"
#include "Timer.h"
#include "Utils.h"
#include "crypto/CryptStateOCB2.h"
#include <string>

class TestCrypt : public QObject {
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();
	void testvectors();
	void authcrypt();
	void xexstarAttack();
	void ivrecovery();
	void reverserecovery();
	void tamper();
};

void TestCrypt::initTestCase() {
	MumbleSSL::initialize();
}

void TestCrypt::cleanupTestCase() {
	MumbleSSL::destroy();
}

void TestCrypt::reverserecovery() {
	CryptStateOCB2 enc, dec;
	enc.genKey();

	// For our testcase, we're going to FORCE iv
	char encrypt_iv[AES_BLOCK_SIZE];
	memset(encrypt_iv, 0x55, AES_BLOCK_SIZE);
	enc.setEncryptIV(std::string(reinterpret_cast< const char * >(encrypt_iv), AES_BLOCK_SIZE));
	dec.setKey(enc.getRawKey(), enc.getDecryptIV(), enc.getEncryptIV());

	unsigned char secret[10] = "abcdefghi";
	unsigned char crypted[512][14];
	unsigned char decr[10];

	memset(crypted, 0, sizeof(crypted));

	// OOO recovery (within 15 packets)
	/*
		for(int i=0;i<128;i++) {
			for(int i=0;i<15;i++)
				enc.encrypt(secret, crypted[i], 10);

			dec.stat_lost = dec.stat_late = 0;
			for(unsigned int i=14;i<=15;i--) {
				QVERIFY(dec.decrypt(crypted[i], decr, 14));
				QCOMPARE(dec.stat_lost, i);
				QCOMPARE(dec.stat_late, 14-i);
			}
			for(int i=0;i<15;i++)
				QVERIFY(!dec.decrypt(crypted[i], decr, 14));
		}
	*/
	// OOO recovery up to 30 packets, but NOT beyond

	int i;

	for (i = 0; i < 128; i++)
		enc.encrypt(secret, crypted[i], 10);

	for (i = 0; i < 30; i++)
		QVERIFY(dec.decrypt(crypted[127 - i], decr, 14));
	for (; i < 128; i++)
		QVERIFY(!dec.decrypt(crypted[127 - i], decr, 14));
	for (i = 0; i < 30; i++)
		QVERIFY(!dec.decrypt(crypted[127 - i], decr, 14));


	// Extensive replay attack test
	for (i = 0; i < 512; i++)
		enc.encrypt(secret, crypted[i], 10);

	for (i = 0; i < 512; i++)
		QVERIFY(dec.decrypt(crypted[i], decr, 14));
	for (i = 0; i < 512; i++)
		QVERIFY(!dec.decrypt(crypted[i], decr, 14));
}

void TestCrypt::ivrecovery() {
	CryptStateOCB2 enc, dec;
	enc.genKey();

	// For our testcase, we're going to FORCE iv
	char encrypt_iv[AES_BLOCK_SIZE];
	memset(encrypt_iv, 0x55, AES_BLOCK_SIZE);
	enc.setEncryptIV(std::string(reinterpret_cast< const char * >(encrypt_iv), AES_BLOCK_SIZE));
	dec.setKey(enc.getRawKey(), enc.getDecryptIV(), enc.getEncryptIV());

	unsigned char secret[10] = "abcdefghi";
	unsigned char crypted[14];
	unsigned char decr[10];

	enc.encrypt(secret, crypted, 10);

	// Can decrypt.
	QVERIFY(dec.decrypt(crypted, decr, 14));
	// .. correctly.
	QVERIFY(memcmp(secret, decr, 10) == 0);

	// But will refuse to reuse same IV.
	QVERIFY(!dec.decrypt(crypted, decr, 14));

	// Recover from lost packet.
	for (int i = 0; i < 16; i++)
		enc.encrypt(secret, crypted, 10);

	QVERIFY(dec.decrypt(crypted, decr, 14));

	// Wraparound.
	for (int i = 0; i < 128; i++) {
		dec.m_statsLocal.lost = 0;
		for (int j = 0; j < 15; j++)
			enc.encrypt(secret, crypted, 10);
		QVERIFY(dec.decrypt(crypted, decr, 14));
		QCOMPARE(dec.m_statsLocal.lost, 14U);
	}

	QVERIFY(enc.getEncryptIV() == dec.getDecryptIV());

	// Wrap too far
	for (int i = 0; i < 257; i++)
		enc.encrypt(secret, crypted, 10);

	QVERIFY(!dec.decrypt(crypted, decr, 14));

	// Sync it
	dec.setDecryptIV(enc.getEncryptIV());
	enc.encrypt(secret, crypted, 10);

	QVERIFY(dec.decrypt(crypted, decr, 14));
}

void TestCrypt::testvectors() {
	// Test vectors are from draft-krovetz-ocb-00.txt
	const unsigned char rawkey[AES_BLOCK_SIZE] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
												   0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

	CryptStateOCB2 cs;
	std::string rawkey_str = std::string(reinterpret_cast< const char * >(rawkey), AES_BLOCK_SIZE);
	cs.setKey(rawkey_str, rawkey_str, rawkey_str);

	unsigned char tag[16];
	QVERIFY(cs.ocb_encrypt(nullptr, nullptr, 0, rawkey, tag));

	const unsigned char blanktag[AES_BLOCK_SIZE] = { 0xBF, 0x31, 0x08, 0x13, 0x07, 0x73, 0xAD, 0x5E,
													 0xC7, 0x0E, 0xC6, 0x9E, 0x78, 0x75, 0xA7, 0xB0 };
	for (int i = 0; i < AES_BLOCK_SIZE; i++)
		QCOMPARE(tag[i], blanktag[i]);

	unsigned char source[40];
	unsigned char crypt[40];
	for (unsigned char i = 0; i < 40; i++)
		source[i] = i;
	QVERIFY(cs.ocb_encrypt(source, crypt, 40, rawkey, tag));
	const unsigned char longtag[AES_BLOCK_SIZE] = { 0x9D, 0xB0, 0xCD, 0xF8, 0x80, 0xF7, 0x3E, 0x3E,
													0x10, 0xD4, 0xEB, 0x32, 0x17, 0x76, 0x66, 0x88 };
	const unsigned char crypted[40]             = { 0xF7, 0x5D, 0x6B, 0xC8, 0xB4, 0xDC, 0x8D, 0x66, 0xB8, 0x36,
                                        0xA2, 0xB0, 0x8B, 0x32, 0xA6, 0x36, 0x9F, 0x1C, 0xD3, 0xC5,
                                        0x22, 0x8D, 0x79, 0xFD, 0x6C, 0x26, 0x7F, 0x5F, 0x6A, 0xA7,
                                        0xB2, 0x31, 0xC7, 0xDF, 0xB9, 0xD5, 0x99, 0x51, 0xAE, 0x9C };

	for (int i = 0; i < AES_BLOCK_SIZE; i++)
		QCOMPARE(tag[i], longtag[i]);

	for (int i = 0; i < 40; i++)
		QCOMPARE(crypt[i], crypted[i]);
}

void TestCrypt::authcrypt() {
	for (unsigned int len = 0; len < 128; len++) {
		const unsigned char rawkey[AES_BLOCK_SIZE] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
													   0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
		const unsigned char nonce[AES_BLOCK_SIZE]  = { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
                                                      0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
		std::string rawkey_str = std::string(reinterpret_cast< const char * >(rawkey), AES_BLOCK_SIZE);
		std::string nonce_str  = std::string(reinterpret_cast< const char * >(nonce), AES_BLOCK_SIZE);
		CryptStateOCB2 cs;
		cs.setKey(rawkey_str, nonce_str, nonce_str);

		std::vector< unsigned char > src;
		src.resize(len);
		for (unsigned char i = 0; i < len; i++)
			src[i] = static_cast< unsigned char >(i + 1);

		unsigned char enctag[AES_BLOCK_SIZE];
		unsigned char dectag[AES_BLOCK_SIZE];
		std::vector< unsigned char > encrypted;
		encrypted.resize(len);
		std::vector< unsigned char > decrypted;
		decrypted.resize(len);

		QVERIFY(cs.ocb_encrypt(src.data(), encrypted.data(), len, nonce, enctag));
		QVERIFY(cs.ocb_decrypt(encrypted.data(), decrypted.data(), len, nonce, dectag));

		for (int i = 0; i < AES_BLOCK_SIZE; i++)
			QCOMPARE(enctag[i], dectag[i]);

		for (unsigned int i = 0; i < len; i++)
			QCOMPARE(src[i], decrypted[i]);
	}
}

// Test prevention of the attack described in section 4.1 of https://eprint.iacr.org/2019/311
void TestCrypt::xexstarAttack() {
	const unsigned char rawkey[AES_BLOCK_SIZE] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
												   0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	const unsigned char nonce[AES_BLOCK_SIZE]  = { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
                                                  0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
	std::string rawkey_str                     = std::string(reinterpret_cast< const char * >(rawkey), AES_BLOCK_SIZE);
	std::string nonce_str                      = std::string(reinterpret_cast< const char * >(nonce), AES_BLOCK_SIZE);
	CryptStateOCB2 cs;
	cs.setKey(rawkey_str, nonce_str, nonce_str);

	std::vector< unsigned char > src;
	src.resize(2 * AES_BLOCK_SIZE);
	// Set first block to `len(secondBlock)`
	src[AES_BLOCK_SIZE - 1] = AES_BLOCK_SIZE * 8;
	// Set second block to arbitrary value
	memset(src.data() + AES_BLOCK_SIZE, 42, AES_BLOCK_SIZE);

	unsigned char enctag[AES_BLOCK_SIZE];
	unsigned char dectag[AES_BLOCK_SIZE];
	std::vector< unsigned char > encrypted;
	encrypted.resize(2 * AES_BLOCK_SIZE);
	std::vector< unsigned char > decrypted;
	decrypted.resize(2 * AES_BLOCK_SIZE);

	const bool failed_encrypt = !cs.ocb_encrypt(src.data(), encrypted.data(), 2 * AES_BLOCK_SIZE, nonce, enctag, false);

	// Perform the attack
	encrypted[AES_BLOCK_SIZE - 1] ^= AES_BLOCK_SIZE * 8;
	for (unsigned int i = 0; i < AES_BLOCK_SIZE; ++i)
		enctag[i] = src[AES_BLOCK_SIZE + i] ^ encrypted[AES_BLOCK_SIZE + i];

	const bool failed_decrypt = !cs.ocb_decrypt(encrypted.data(), decrypted.data(), 1 * AES_BLOCK_SIZE, nonce, dectag);

	// Verify forged tag (should match if attack is properly implemented)
	for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
		QCOMPARE(enctag[i], dectag[i]);
	}

	// Make sure we detected the attack
	QVERIFY(failed_encrypt);
	QVERIFY(failed_decrypt);

	// The assumption that critical packets do not turn up by pure chance turned out to be incorrect
	// since digital silence appears to produce them in mass.
	// So instead we now modify the packet in a way which should not affect the audio but will
	// prevent the attack.
	QVERIFY(cs.ocb_encrypt(src.data(), encrypted.data(), 2 * AES_BLOCK_SIZE, nonce, enctag));
	QVERIFY(cs.ocb_decrypt(encrypted.data(), decrypted.data(), 2 * AES_BLOCK_SIZE, nonce, dectag));

	// Tags should match
	for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
		QCOMPARE(enctag[i], dectag[i]);
	}

	// Actual content should have been changed such that the critical block is no longer all 0.
	QCOMPARE(src[0], static_cast< unsigned char >(0));
	QCOMPARE(decrypted[0], static_cast< unsigned char >(1));
}

void TestCrypt::tamper() {
	const unsigned char rawkey[AES_BLOCK_SIZE] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
												   0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	const unsigned char nonce[AES_BLOCK_SIZE]  = { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
                                                  0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
	std::string rawkey_str                     = std::string(reinterpret_cast< const char * >(rawkey), AES_BLOCK_SIZE);
	std::string nonce_str                      = std::string(reinterpret_cast< const char * >(nonce), AES_BLOCK_SIZE);
	CryptStateOCB2 cs;
	cs.setKey(rawkey_str, nonce_str, nonce_str);

	const unsigned char msg[] = "It was a funky funky town!";
	unsigned int len          = sizeof(msg);

	std::vector< unsigned char > encrypted;
	encrypted.resize(len + 4);
	std::vector< unsigned char > decrypted;
	decrypted.resize(len);
	cs.encrypt(msg, encrypted.data(), len);

	for (unsigned int i = 0; i < len * 8; i++) {
		encrypted[i / 8] ^= static_cast< unsigned char >(1 << (i % 8));
		QVERIFY(!cs.decrypt(encrypted.data(), decrypted.data(), len + 4));
		encrypted[i / 8] ^= static_cast< unsigned char >(1 << (i % 8));
	}
	QVERIFY(cs.decrypt(encrypted.data(), decrypted.data(), len + 4));
}

QTEST_MAIN(TestCrypt)
#include "TestCrypt.moc"
