// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CertificatePinEvaluator.h"

#include "CryptographicHash.h"

CertificatePinEvaluatorResult::CertificatePinEvaluatorResult() {
}

CertificatePinEvaluatorResult::CertificatePinEvaluatorResult(bool ok, CryptographicHash::Algorithm preferred, CryptographicHash::Algorithm used, QString digest, QString expectedDigest) {
	m_ok = ok;

	m_preferredAlgorithm = preferred;
	m_usedAlgorithm = used;

	m_digest = digest;
	m_expectedDigest = expectedDigest;
}

CertificatePinEvaluatorResult::~CertificatePinEvaluatorResult() {
}

bool CertificatePinEvaluatorResult::isOK() {
	return m_ok;
}

CryptographicHash::Algorithm CertificatePinEvaluatorResult::preferredAlgorithm() {
	return m_preferredAlgorithm;
}

CryptographicHash::Algorithm CertificatePinEvaluatorResult::usedAlgorithm() {
	return m_usedAlgorithm;
}

QString CertificatePinEvaluatorResult::digest() {
	return m_digest;
}

QString CertificatePinEvaluatorResult::expectedDigest() {
	return m_expectedDigest;
}

CertificatePinEvaluator::CertificatePinEvaluator()
	: m_preferredAlgorithm(CryptographicHash::Sha256)
	, m_allowLegacySHA1Pins(false) {
}

CertificatePinEvaluator::~CertificatePinEvaluator() {
}

void CertificatePinEvaluator::setAllowLegacySHA1Pins(bool allow) {
	m_allowLegacySHA1Pins = allow;
}

bool CertificatePinEvaluator::allowLegacySHA1Pins() {
	return m_allowLegacySHA1Pins;
}

void CertificatePinEvaluator::setPreferredAlgorithm(CryptographicHash::Algorithm algo) {
	m_preferredAlgorithm = algo;
}

CryptographicHash::Algorithm CertificatePinEvaluator::preferredAlgorithm() {
	return m_preferredAlgorithm;
}

CertificatePinEvaluatorResult CertificatePinEvaluator::evaluate(const QSslCertificate &cert, QString storedHash) {
	QString stored_digest = storedHash;
	QString expected_digest;
	QString actual_digest;
	CryptographicHash::Algorithm algo;
	CryptographicHash::Algorithm preferred_algo = preferredAlgorithm();
	bool has_stored_digest = false;
	bool pin_ok = false;

	if (m_allowLegacySHA1Pins && stored_digest.size() == 40) { // Non-prefixed SHA1
		has_stored_digest = true;

		algo = CryptographicHash::Sha1;

		expected_digest = stored_digest;
	} else if (stored_digest.startsWith(CryptographicHash::shortAlgorithmName(CryptographicHash::Sha256))) {
		has_stored_digest = true;

		algo = CryptographicHash::Sha256;

		stored_digest.remove(QString::fromLatin1("%1:").arg(CryptographicHash::shortAlgorithmName(CryptographicHash::Sha256)));
		expected_digest = stored_digest;
	} else {
		stored_digest = QString();
		has_stored_digest = false;

		// We didn't find a stored hash.
		// Please use the preferred hashing algorithm when displaying stuff to the user.
		algo = preferred_algo;
	}

	if (has_stored_digest) {
		actual_digest = QString::fromLatin1(CryptographicHash::hash(cert.toDer(), algo).toHex());
		if (actual_digest == expected_digest) {
			pin_ok = true;
		}
	}

	return CertificatePinEvaluatorResult(pin_ok, preferred_algo, algo, actual_digest, expected_digest);
}
