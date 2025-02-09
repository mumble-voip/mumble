// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SELFSIGNEDCERTIFICATE_H_
#define MUMBLE_SELFSIGNEDCERTIFICATE_H_

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509v3.h>

#include <QtCore/QString>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

enum CertificateType { CertificateTypeServerCertificate, CertificateTypeClientCertificate };

class SelfSignedCertificate {
private:
	static bool generate(CertificateType certificateType, QString clientCertName, QString clientCertEmail,
						 QSslCertificate &qscCert, QSslKey &qskKey);
	static EVP_PKEY *generate_rsa_keypair();

public:
	static bool generateMumbleCertificate(QString name, QString email, QSslCertificate &qscCert, QSslKey &qskKey);
	static bool generateMurmurV2Certificate(QSslCertificate &qscCert, QSslKey &qskKey);
};

#endif
