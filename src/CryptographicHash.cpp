// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "murmur_pch.h"

#include "CryptographicHash.h"

class CryptographicHashPrivate {
	public:
		CryptographicHashPrivate(const EVP_MD *type);
		~CryptographicHashPrivate();
		void addData(const QByteArray &buf);
		QByteArray result();

	private:
		EVP_MD_CTX *mdctx;
		bool ok;
		QByteArray final;

		CryptographicHashPrivate();
};

CryptographicHashPrivate::CryptographicHashPrivate()
	: ok(false)
	, mdctx(NULL) {
}

CryptographicHashPrivate::CryptographicHashPrivate(const EVP_MD *type) {
	ok = true;
	mdctx = EVP_MD_CTX_create();

	if (mdctx == NULL) {
		ok = false;
		return;
	}

	int err = EVP_DigestInit_ex(mdctx, type, NULL);
	if (err != 1) {
		ok = false;
		return;
	}
}

CryptographicHashPrivate::~CryptographicHashPrivate() {
	if (mdctx) {
		EVP_MD_CTX_destroy(mdctx);
		mdctx = NULL;
	}
}

void CryptographicHashPrivate::addData(const QByteArray &buf) {
	if (!ok) {
		return;
	}

	int err = EVP_DigestUpdate(mdctx, buf.constData(), buf.size());
	if (err != 1) {
		ok = false;
	}
}

QByteArray CryptographicHashPrivate::result() {
	if (!ok) {
		return QByteArray();
	}

	if (!final.isNull()) {
		return final;
	}

	QByteArray digest(EVP_MD_CTX_size(mdctx), '\0');
	int err = EVP_DigestFinal_ex(mdctx, reinterpret_cast<unsigned char *>(digest.data()), NULL);
	if (err != 1) {
		ok = false;
		return QByteArray();
	}

	final = digest;
	return final;
}

CryptographicHash::CryptographicHash()
	: d(NULL) {
}

CryptographicHash::CryptographicHash(CryptographicHash::Algorithm algo)
	: d(NULL) {

	switch (algo) {
		case CryptographicHash::Sha1:
			d = new CryptographicHashPrivate(EVP_sha1());
			break;
		case CryptographicHash::Sha256:
			d = new CryptographicHashPrivate(EVP_sha256());
			break;
	}
}

void CryptographicHash::addData(const QByteArray &buf) {
	if (d) {
		d->addData(buf);
	}
}

QByteArray CryptographicHash::result() const {
	if (d) {
		return d->result();
	}
	return QByteArray();
}

QByteArray CryptographicHash::hash(const QByteArray &buf, Algorithm algo) {
	CryptographicHash h(algo);
	h.addData(buf);
	return h.result();
}

QString CryptographicHash::humanReadableAlgorithmName(CryptographicHash::Algorithm algo) {
	switch (algo) {
		case CryptographicHash::Sha1:
			return QLatin1String("SHA-1");
		case CryptographicHash::Sha256:
			return QLatin1String("SHA-256");
	}

	return QString();
}

QString CryptographicHash::shortAlgorithmName(CryptographicHash::Algorithm algo) {
	switch (algo) {
		case CryptographicHash::Sha1:
			return QLatin1String("sha1");
		case CryptographicHash::Sha256:
			return QLatin1String("sha256");
	}

	return QString();
}
