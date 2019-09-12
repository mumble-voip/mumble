// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include "SSL.h"
#include "Timer.h"
#include "CryptState.h"
#include "Utils.h"

class TestCrypt : public QObject {
		Q_OBJECT
	private slots:
		void initTestCase();
		void cleanupTestCase();
		void testvectors();
		void authcrypt();
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
	CryptState enc, dec;
	enc.genKey();

	// For our testcase, we're going to FORCE iv
	memset(enc.encrypt_iv, 0x55, AES_BLOCK_SIZE);

	dec.setKey(enc.raw_key, enc.decrypt_iv, enc.encrypt_iv);

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

	for (i=0;i<128;i++)
		enc.encrypt(secret, crypted[i], 10);

	for (i=0;i<30;i++)
		QVERIFY(dec.decrypt(crypted[127-i], decr, 14));
	for (;i<128;i++)
		QVERIFY(!dec.decrypt(crypted[127-i], decr, 14));
	for (i=0;i<30;i++)
		QVERIFY(!dec.decrypt(crypted[127-i], decr, 14));


	// Extensive replay attack test
	for (i=0;i<512;i++)
		enc.encrypt(secret, crypted[i], 10);

	for (i=0;i<512;i++)
		QVERIFY(dec.decrypt(crypted[i], decr, 14));
	for (i=0;i<512;i++)
		QVERIFY(!dec.decrypt(crypted[i], decr, 14));
}

void TestCrypt::ivrecovery() {
	CryptState enc, dec;
	enc.genKey();

	// For our testcase, we're going to FORCE iv
	memset(enc.encrypt_iv, 0x55, AES_BLOCK_SIZE);

	dec.setKey(enc.raw_key, enc.decrypt_iv, enc.encrypt_iv);

	unsigned char secret[10] = "abcdefghi";
	unsigned char crypted[14];
	unsigned char decr[10];

	enc.encrypt(secret, crypted, 10);

	// Can decrypt.
	QVERIFY(dec.decrypt(crypted, decr, 14));
	// .. correctly.
	QVERIFY(memcmp(secret, decr, 10)==0);

	// But will refuse to reuse same IV.
	QVERIFY(! dec.decrypt(crypted, decr, 14));

	// Recover from lost packet.
	for (int i=0;i<16;i++)
		enc.encrypt(secret, crypted, 10);

	QVERIFY(dec.decrypt(crypted, decr, 14));

	// Wraparound.
	for (int i=0;i<128;i++) {
		dec.uiLost = 0;
		for (int j=0;j<15;j++)
			enc.encrypt(secret, crypted, 10);
		QVERIFY(dec.decrypt(crypted, decr, 14));
		QCOMPARE(dec.uiLost, 14U);
	}

	QVERIFY(memcmp(enc.encrypt_iv, dec.decrypt_iv, AES_BLOCK_SIZE)==0);

	// Wrap too far
	for (int i=0;i<257;i++)
		enc.encrypt(secret, crypted, 10);

	QVERIFY(! dec.decrypt(crypted, decr, 14));

	// Sync it
	dec.setDecryptIV(enc.encrypt_iv);
	enc.encrypt(secret, crypted, 10);

	QVERIFY(dec.decrypt(crypted, decr, 14));
}

void TestCrypt::testvectors() {
	// Test vectors are from draft-krovetz-ocb-00.txt
	const unsigned char rawkey[AES_BLOCK_SIZE] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

	CryptState cs;
	cs.setKey(rawkey, rawkey, rawkey);

	unsigned char tag[16];
	cs.ocb_encrypt(NULL, NULL, 0, rawkey, tag);

	const unsigned char blanktag[AES_BLOCK_SIZE] = {0xBF,0x31,0x08,0x13,0x07,0x73,0xAD,0x5E,0xC7,0x0E,0xC6,0x9E,0x78,0x75,0xA7,0xB0};
	for (int i=0;i<AES_BLOCK_SIZE;i++)
		QCOMPARE(tag[i], blanktag[i]);

	unsigned char source[40];
	unsigned char crypt[40];
	for (int i=0;i<40;i++)
		source[i]=i;
	cs.ocb_encrypt(source, crypt, 40, rawkey, tag);
	const unsigned char longtag[AES_BLOCK_SIZE] = {0x9D,0xB0,0xCD,0xF8,0x80,0xF7,0x3E,0x3E,0x10,0xD4,0xEB,0x32,0x17,0x76,0x66,0x88};
	const unsigned char crypted[40] = {0xF7,0x5D,0x6B,0xC8,0xB4,0xDC,0x8D,0x66,0xB8,0x36,0xA2,0xB0,0x8B,0x32,0xA6,0x36,0x9F,0x1C,0xD3,0xC5,0x22,0x8D,0x79,0xFD,
	                                   0x6C,0x26,0x7F,0x5F,0x6A,0xA7,0xB2,0x31,0xC7,0xDF,0xB9,0xD5,0x99,0x51,0xAE,0x9C
	                                  };

	for (int i=0;i<AES_BLOCK_SIZE;i++)
		QCOMPARE(tag[i], longtag[i]);

	for (int i=0;i<40;i++)
		QCOMPARE(crypt[i], crypted[i]);
}

void TestCrypt::authcrypt() {
	for (int len=0;len<128;len++) {
		const unsigned char rawkey[AES_BLOCK_SIZE] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
		const unsigned char nonce[AES_BLOCK_SIZE] = {0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};
		CryptState cs;
		cs.setKey(rawkey, nonce, nonce);

		STACKVAR(unsigned char, src, len);
		for (int i=0;i<len;i++)
			src[i] = (i + 1);

		unsigned char enctag[AES_BLOCK_SIZE];
		unsigned char dectag[AES_BLOCK_SIZE];
		STACKVAR(unsigned char, encrypted, len);
		STACKVAR(unsigned char, decrypted, len);

		cs.ocb_encrypt(src, encrypted, len, nonce, enctag);
		cs.ocb_decrypt(encrypted, decrypted, len, nonce, dectag);

		for (int i=0;i<AES_BLOCK_SIZE;i++)
			QCOMPARE(enctag[i], dectag[i]);

		for (int i=0;i<len;i++)
			QCOMPARE(src[i], decrypted[i]);
	}
}

void TestCrypt::tamper() {
	const unsigned char rawkey[AES_BLOCK_SIZE] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	const unsigned char nonce[AES_BLOCK_SIZE] = {0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};
	CryptState cs;
	cs.setKey(rawkey, nonce, nonce);

	const unsigned char msg[] = "It was a funky funky town!";
	int len = sizeof(msg);

	STACKVAR(unsigned char, encrypted, len+4);
	STACKVAR(unsigned char, decrypted, len);
	cs.encrypt(msg, encrypted, len);

	for (int i=0;i<len*8;i++) {
		encrypted[i/8] ^= 1 << (i % 8);
		QVERIFY(! cs.decrypt(encrypted, decrypted, len+4));
		encrypted[i/8] ^= 1 << (i % 8);
	}
	QVERIFY(cs.decrypt(encrypted, decrypted, len+4));
}

QTEST_MAIN(TestCrypt)
#include "TestCrypt.moc"
