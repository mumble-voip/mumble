// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#include "Meta.h"
#include "SelfSignedCertificate.h"
#include "Server.h"

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>

#ifdef Q_OS_WIN
#	include <winsock2.h>
#endif

QList< QSslCertificate > Server::buildSslChain(const QSslCertificate &leaf, const QList< QSslCertificate > &pool) {
	QList< QSslCertificate > chain;
	if (leaf.isNull()) {
		return chain;
	}
	chain << leaf;
	if (pool.isEmpty()) {
		return chain;
	}

	// Convert the leaf to DER format and create an OpenSSL X509 object from it.
	QByteArray qbaLeaf = leaf.toDer();
	int maxDerSize     = qbaLeaf.size();
	BIO *mem           = BIO_new_mem_buf(qbaLeaf.data(), maxDerSize);
	Q_UNUSED(BIO_set_close(mem, BIO_NOCLOSE));
	X509 *leaf_x509 = d2i_X509_bio(mem, nullptr);
	BIO_free(mem);

	// Prepare an SSL context; the method should not matter, so just go with TLS_method().
	SSL_CTX *ctx = SSL_CTX_new(TLS_method());

	// Add the leaf
	SSL_CTX_use_certificate(ctx, leaf_x509);

	// Construct an OpenSSL X509 object for the pool and add each to the context.
	for (const QSslCertificate &cert : pool) {
		QByteArray qbaCert = cert.toDer();
		int s              = qbaCert.size();
		maxDerSize         = maxDerSize < s ? s : maxDerSize;
		BIO *mem           = BIO_new_mem_buf(qbaCert.data(), s);
		Q_UNUSED(BIO_set_close(mem, BIO_NOCLOSE));
		X509 *x509 = d2i_X509_bio(mem, nullptr);
		BIO_free(mem);
		SSL_CTX_add0_chain_cert(ctx, x509);
	}

	// Do the actual chain building
	int flags = SSL_BUILD_CHAIN_FLAG_CHECK | SSL_BUILD_CHAIN_FLAG_IGNORE_ERROR; // Think of the correct flags
	int ret   = SSL_CTX_build_cert_chain(ctx, flags);

	// Check if the operation is successful.
	// Since we use the SSL_BUILD_CHAIN_FLAG_IGNORE_ERROR flag, a return value of 2 is acceptable.
	if (ret == 1 || ret == 2) {
		// Retrieve the chain
		STACK_OF(X509) *stack = nullptr;
		SSL_CTX_get0_chain_certs(ctx, &stack);

		// Copy the chain back to Qt.
		// Instead of allocating a new buffer every time i2d_X509() is called, we allocate a shared buffer of
		// "maxDerSize" size.
		unsigned char *buffer = (unsigned char *) malloc(maxDerSize);
		while (sk_X509_num(stack) > 0) {
			X509 *next     = sk_X509_shift(stack);
			int actualSize = i2d_X509(next, &buffer);
			X509_free(next);
			if (actualSize == -1) {
				// Failed to encode certificate in DER format.
				chain.clear();
				break;
			}
			// i2d_X509() altered our buffer pointer, we need to set it back manually.
			buffer -= actualSize;
			QByteArray array            = QByteArray::fromRawData((char *) buffer, actualSize);
			QList< QSslCertificate > ql = QSslCertificate::fromData(array, QSsl::EncodingFormat::Der);
			// Data from OpenSSL must correspond to a single certificate!
			if (ql.size() == 1) {
				chain << ql;
			} else {
				chain.clear();
				break;
			}
		}

		// Clean up
		free(buffer);
	} else {
		chain.clear();
	}
	// Pool certificates were added with the "add0" function (as opposed to "add1"),
	// meaning that they are freed when ctx is.
	// Same for the stack, which was obtained with "get0".
	SSL_CTX_free(ctx);
	X509_free(leaf_x509);

	// Drain OpenSSL's per-thread error queue, see below!
	ERR_clear_error();

	return chain;
}

QByteArray Server::chainToPem(const QList< QSslCertificate > &chain) {
	QByteArrayList bytes;
	for (const QSslCertificate &cert : chain) {
		bytes << cert.toPem();
	}
	return bytes.join();
}

bool Server::isKeyForCert(const QSslKey &key, const QSslCertificate &cert) {
	if (key.isNull() || cert.isNull() || (key.type() != QSsl::PrivateKey))
		return false;

	QByteArray qbaKey  = key.toDer();
	QByteArray qbaCert = cert.toDer();

	X509 *x509     = nullptr;
	EVP_PKEY *pkey = nullptr;
	BIO *mem       = nullptr;

	mem = BIO_new_mem_buf(qbaKey.data(), qbaKey.size());
	Q_UNUSED(BIO_set_close(mem, BIO_NOCLOSE));
	pkey = d2i_PrivateKey_bio(mem, nullptr);
	BIO_free(mem);

	mem = BIO_new_mem_buf(qbaCert.data(), qbaCert.size());
	Q_UNUSED(BIO_set_close(mem, BIO_NOCLOSE));
	x509 = d2i_X509_bio(mem, nullptr);
	BIO_free(mem);
	mem = nullptr;

	if (x509 && pkey && X509_check_private_key(x509, pkey)) {
		EVP_PKEY_free(pkey);
		X509_free(x509);
		return true;
	}

	if (pkey)
		EVP_PKEY_free(pkey);
	if (x509)
		X509_free(x509);
	return false;
}

QSslKey Server::privateKeyFromPEM(const QByteArray &buf, const QByteArray &pass) {
	QSslKey key;
	key = QSslKey(buf, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, pass);
	if (key.isNull())
		key = QSslKey(buf, QSsl::Dsa, QSsl::Pem, QSsl::PrivateKey, pass);
#if QT_VERSION >= 0x050500
	if (key.isNull())
		key = QSslKey(buf, QSsl::Ec, QSsl::Pem, QSsl::PrivateKey, pass);
#endif
	return key;
}

void Server::initializeCert() {
	QByteArray crt, key, pass, dhparams;

	// Clear all existing SSL settings
	// for this server.
	qlCertificateChain.clear();
	qskKey.clear();
#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
	qsdhpDHParams = QSslDiffieHellmanParameters();
#endif

	crt      = getConf("certificate", QString()).toByteArray();
	key      = getConf("key", QString()).toByteArray();
	pass     = getConf("passphrase", QByteArray()).toByteArray();
	dhparams = getConf("sslDHParams", Meta::mp.qbaDHParams).toByteArray();



	// Attempt to load the private key.
	if (!key.isEmpty()) {
		qskKey = Server::privateKeyFromPEM(key, pass);
	}

	// If we still can't load the key, try loading any keys from the certificate
	if (qskKey.isNull() && !crt.isEmpty()) {
		qskKey = Server::privateKeyFromPEM(crt);
	}

	// If have a key, walk the list of certs, find the one for our key,
	// remove any certs for our key from the list, what's left is part of
	// the CA certificate chain.
	if (!qskKey.isNull()) {
		QList< QSslCertificate > ql;
		ql << QSslCertificate::fromData(crt);
		ql << QSslCertificate::fromData(key);
		QSslCertificate tmpCrt;
		for (int i = 0; i < ql.size(); ++i) {
			const QSslCertificate &c = ql.at(i);
			if (isKeyForCert(qskKey, c)) {
				tmpCrt = c;
				ql.removeAt(i);
			}
		}
		if (!tmpCrt.isNull()) {
			qlCertificateChain = buildSslChain(tmpCrt, ql);
		}
	}

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
	if (!dhparams.isEmpty()) {
		QSslDiffieHellmanParameters qdhp = QSslDiffieHellmanParameters::fromEncoded(dhparams);
		if (qdhp.isValid()) {
			qsdhpDHParams = qdhp;
		} else {
			log(QString::fromLatin1("Unable to use specified Diffie-Hellman parameters (sslDHParams): %1")
					.arg(qdhp.errorString()));
		}
	}
#else
	if (!dhparams.isEmpty()) {
		log("Diffie-Hellman parameters (sslDHParams) were specified, but will not be used. This version of Murmur does "
			"not support Diffie-Hellman parameters.");
	}
#endif

	QString issuer;

	if (!qlCertificateChain.isEmpty()) {
		QStringList issuerNames = qlCertificateChain[0].issuerInfo(QSslCertificate::CommonName);
		if (!issuerNames.isEmpty()) {
			issuer = issuerNames.first();
		}
	}

	// Really old certs/keys are no good, throw them away so we can
	// generate a new one below.
	if (issuer == QString::fromUtf8("Murmur Autogenerated Certificate")) {
		log("Old autogenerated certificate is unusable for registration, invalidating it");
		qlCertificateChain.clear();
		qlCertificateChain << QSslCertificate();
		qskKey = QSslKey();
	}

	// If we have a cert, and it's a self-signed one, but we're binding to
	// all the same addresses as the Meta server is, use it's cert instead.
	// This allows a self-signed certificate generated by Murmur to be
	// replaced by a CA-signed certificate in the .ini file.
	if (!qlCertificateChain.isEmpty() && !qlCertificateChain[0].isNull()
		&& issuer.startsWith(QString::fromUtf8("Murmur Autogenerated Certificate"))
		&& !Meta::mp.qlCertificateChain.isEmpty() && !Meta::mp.qskKey.isNull() && (Meta::mp.qlBind == qlBind)) {
		qlCertificateChain.clear();
		qlCertificateChain = Meta::mp.qlCertificateChain;
		qskKey             = Meta::mp.qskKey;
		if (!qlCertificateChain.isEmpty() && !qlCertificateChain[0].isNull() && !qskKey.isNull()) {
			bUsingMetaCert = true;
		}
	}

	// If we still don't have a certificate by now, try to load the one from Meta
	if (qlCertificateChain.isEmpty() || qlCertificateChain[0].isNull() || qskKey.isNull()) {
		if (!key.isEmpty() || !crt.isEmpty()) {
			log("Certificate specified, but failed to load.");
		}

		qlCertificateChain.clear();
		qlCertificateChain = Meta::mp.qlCertificateChain;
		qskKey             = Meta::mp.qskKey;

		if (!qlCertificateChain.isEmpty() && !qlCertificateChain[0].isNull() && !qskKey.isNull()) {
			bUsingMetaCert = true;
		}

		// If loading from Meta doesn't work, build+sign a new one
		if (qlCertificateChain.isEmpty() || qlCertificateChain[0].isNull() || qskKey.isNull()) {
			log("Generating new server certificate.");
			if (qlCertificateChain.isEmpty()) {
				qlCertificateChain << QSslCertificate();
			}
			if (!SelfSignedCertificate::generateMurmurV2Certificate(qlCertificateChain[0], qskKey)) {
				log("Certificate or key generation failed");
			}

			setConf("certificate", chainToPem(qlCertificateChain));
			setConf("key", qskKey.toPem());
		}
	}

	// Drain OpenSSL's per-thread error queue
	// to ensure that errors from the operations
	// we've done in here do not leak out into
	// Qt's SSL module.
	//
	// If an error leaks, it can break all connections
	// to the server because each invocation of Qt's SSL
	// read callback checks OpenSSL's per-thread error
	// queue (albeit indirectly, via SSL_get_error()).
	// Qt expects any errors returned from SSL_get_error()
	// to be related to the QSslSocket it is currently
	// processing -- which is the obvious thing to expect:
	// SSL_get_error() takes a pointer to an SSL object
	// and the return code of the failed operation.
	// However, it is also documented as:
	//
	//  "In addition to ssl and ret, SSL_get_error()
	//   inspects the current thread's OpenSSL error
	//   queue."
	//
	// So, if any OpenSSL operation on the main thread
	// forgets to clear the error queue, those errors
	// *will* leak into other things that *do* error
	// checking. In our case, into Qt's SSL read callback,
	// resulting in all clients being disconnected.
	ERR_clear_error();
}

const QString Server::getDigest() const {
	return qlCertificateChain.isEmpty()
			   ? QString()
			   : QString::fromLatin1(qlCertificateChain[0].digest(QCryptographicHash::Sha1).toHex());
}
