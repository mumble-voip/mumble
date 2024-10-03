// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SSL.h"
#include "SSLLocks.h"

#include "Version.h"


#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>

#include <openssl/ssl.h>

void MumbleSSL::initialize() {
	// Let Qt initialize its copy of OpenSSL, if it's different than
	// Mumble's. (Initialization is a side-effect of calling
	// QSslSocket::supportsSsl()).
	QSslSocket::supportsSsl();

	// Initialize our copy of OpenSSL.
	SSL_library_init(); // Safe to discard return value, per OpenSSL man pages.
	SSL_load_error_strings();

	SSLLocks::initialize();
}

void MumbleSSL::destroy() {
	SSLLocks::destroy();
}

QString MumbleSSL::defaultOpenSSLCipherString() {
	return QLatin1String("EECDH+AESGCM:EDH+aRSA+AESGCM:DHE-RSA-AES256-SHA:DHE-RSA-AES128-SHA:AES256-SHA:AES128-SHA");
}

QList< QSslCipher > MumbleSSL::ciphersFromOpenSSLCipherString(QString cipherString) {
	QList< QSslCipher > chosenCiphers;

	SSL_CTX *ctx           = nullptr;
	SSL *ssl               = nullptr;
	const SSL_METHOD *meth = nullptr;
	int i                  = 0;

	QByteArray csbuf    = cipherString.toLatin1();
	const char *ciphers = csbuf.constData();

	meth = SSLv23_server_method();
	if (!meth) {
		qWarning("MumbleSSL: unable to get SSL method");
		goto out;
	}

	// We use const_cast to be compatible with OpenSSL 0.9.8.
	ctx = SSL_CTX_new(const_cast< SSL_METHOD * >(meth));
	if (!ctx) {
		qWarning("MumbleSSL: unable to allocate SSL_CTX");
		goto out;
	}

	if (!SSL_CTX_set_cipher_list(ctx, ciphers)) {
		qWarning("MumbleSSL: error parsing OpenSSL cipher string in ciphersFromOpenSSLCipherString");
		goto out;
	}

	ssl = SSL_new(ctx);
	if (!ssl) {
		qWarning("MumbleSSL: unable to create SSL object in ciphersFromOpenSSLCipherString");
		goto out;
	}

	while (1) {
		const char *name = SSL_get_cipher_list(ssl, i);
		if (!name) {
			break;
		}
		QSslCipher c = QSslCipher(QString::fromLatin1(name));
		if (!c.isNull()) {
			chosenCiphers << c;
		}
		++i;
	}

out:
	SSL_CTX_free(ctx);
	SSL_free(ssl);
	return chosenCiphers;
}

void MumbleSSL::addSystemCA() {
	auto config = QSslConfiguration::defaultConfiguration();

	config.addCaCertificates(QSslConfiguration::systemCaCertificates());

	QSslConfiguration::setDefaultConfiguration(config);

#ifdef Q_OS_WIN
	// Work around issue #1271.
	// Skype's click-to-call feature creates an enormous
	// amount of certificates in the Root CA store.
	{
		QSslConfiguration sslCfg        = QSslConfiguration::defaultConfiguration();
		QList< QSslCertificate > caList = sslCfg.caCertificates();

		QList< QSslCertificate > filteredCaList;
		foreach (QSslCertificate cert, caList) {
			QStringList orgs = cert.subjectInfo(QSslCertificate::Organization);
			bool skip        = false;
			foreach (QString ou, orgs) {
				if (ou.contains(QLatin1String("Skype"), Qt::CaseInsensitive)) {
					skip = true;
					break;
				}
			}
			if (skip) {
				continue;
			}

			filteredCaList.append(cert);
		}

		sslCfg.setCaCertificates(filteredCaList);
		QSslConfiguration::setDefaultConfiguration(sslCfg);

		qWarning("SSL: CA certificate filter applied. Filtered size: %i, original size: %i", filteredCaList.size(),
				 caList.size());
	}
#endif
}

QString MumbleSSL::protocolToString(QSsl::SslProtocol protocol) {
	switch (protocol) {
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
		case QSsl::TlsV1_0:
			return QLatin1String("TLS 1.0");
		case QSsl::TlsV1_1:
			return QLatin1String("TLS 1.1");
#endif
		case QSsl::TlsV1_2:
			return QLatin1String("TLS 1.2");
		case QSsl::TlsV1_3:
			return QLatin1String("TLS 1.3");
		case QSsl::AnyProtocol:
			return QLatin1String("AnyProtocol");
		case QSsl::SecureProtocols:
			return QLatin1String("SecureProtocols");
		default:
		case QSsl::UnknownProtocol:
			return QLatin1String("UnknownProtocol");
	}
}
