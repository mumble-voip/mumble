// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CERTPINEVALUATOR_H_
#define MUMBLE_MUMBLE_CERTPINEVALUATOR_H_

#include "mumble_pch.hpp"

#include "CryptographicHash.h"

class CertPinEvaluatorResult {
	public:
		friend class CertPinEvaluator;

		~CertPinEvaluatorResult();

		/// Whether the stored pin matched the passed-in certificate.
		bool isOK();

		/// The preferred algorithm.
		CryptographicHash::Algorithm preferredAlgorithm();

		/// The used algorithm.
		CryptographicHash::Algorithm usedAlgorithm();

		/// The computed digest.
		QString digest();

		/// The expected digest.
		QString expectedDigest();

	private:
		CertPinEvaluatorResult();
		CertPinEvaluatorResult(bool ok, CryptographicHash::Algorithm preferred, CryptographicHash::Algorithm used, QString digest, QString expectedDigest);

		bool m_ok;
		CryptographicHash::Algorithm m_preferredAlgorithm;
		CryptographicHash::Algorithm m_usedAlgorithm;
		QString m_digest;
		QString m_expectedDigest;
};

class CertPinEvaluator {
	public:
		CertPinEvaluator();
		~CertPinEvaluator();

		void setAllowLegacySHA1Pins(bool allow);
		bool allowLegacySHA1Pins();

		void setPreferredAlgorithm(CryptographicHash::Algorithm algo);
		CryptographicHash::Algorithm preferredAlgorithm();

		CertPinEvaluatorResult evaluate(const QSslCertificate &cert, QString storedHash);

	private:
		bool m_allowLegacySHA1Pins;
		CryptographicHash::Algorithm m_preferredAlgorithm;
};

#endif
