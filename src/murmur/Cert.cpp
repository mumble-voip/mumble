// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "murmur_pch.h"

#include "Meta.h"
#include "Server.h"

#define SSL_STRING(x) QString::fromLatin1(x).toUtf8().data()

static BN_GENCB *mumble_BN_GENCB_new() {
#if OPENSSL_VERSION >= 0x10100000L
	return BN_GENCB_new();
#else
	return reinterpret_cast<BN_GENCB *>(malloc(sizeof(BN_GENCB)));
#endif
}

static void mumble_BN_GENCB_free(BN_GENCB *cb) {
#if OPENSSL_VERSION >= 0x10100000L
	BN_GENCB_free(cb);
#else
	free(cb);
#endif
}

static int add_ext(X509 * crt, int nid, char *value) {
	X509_EXTENSION *ex;
	X509V3_CTX ctx;
	X509V3_set_ctx_nodb(&ctx);
	X509V3_set_ctx(&ctx, crt, crt, NULL, NULL, 0);
	ex = X509V3_EXT_conf_nid(NULL, &ctx, nid, value);
	if (!ex)
		return 0;

	X509_add_ext(crt, ex, -1);
	X509_EXTENSION_free(ex);
	return 1;
}

static bool selfSignedServerCert_SHA1_RSA_2048(QSslCertificate &qscCert, QSslKey &qskKey) {
	bool ok = true;

	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);

	X509 *x509 = X509_new();
	EVP_PKEY *pkey = EVP_PKEY_new();
	RSA *rsa = RSA_generate_key(2048,RSA_F4,NULL,NULL);
	EVP_PKEY_assign_RSA(pkey, rsa);

	X509_set_version(x509, 2);
	ASN1_INTEGER_set(X509_get_serialNumber(x509),1);
	X509_gmtime_adj(X509_get_notBefore(x509),0);
	X509_gmtime_adj(X509_get_notAfter(x509),60*60*24*365*20);
	X509_set_pubkey(x509, pkey);

	X509_NAME *name=X509_get_subject_name(x509);

	X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, reinterpret_cast<unsigned char *>(const_cast<char *>("Murmur Autogenerated Certificate v2")), -1, -1, 0);
	X509_set_issuer_name(x509, name);
	add_ext(x509, NID_basic_constraints, SSL_STRING("critical,CA:FALSE"));
	add_ext(x509, NID_ext_key_usage, SSL_STRING("serverAuth,clientAuth"));
	add_ext(x509, NID_subject_key_identifier, SSL_STRING("hash"));
	add_ext(x509, NID_netscape_comment, SSL_STRING("Generated from murmur"));

	X509_sign(x509, pkey, EVP_sha1());

	{
		QByteArray crt;
		crt.resize(i2d_X509(x509, NULL));
		unsigned char *dptr=reinterpret_cast<unsigned char *>(crt.data());
		i2d_X509(x509, &dptr);

		qscCert = QSslCertificate(crt, QSsl::Der);
		if (qscCert.isNull()) {
			ok = false;
		}
	}

	{
		QByteArray key;
		key.resize(i2d_PrivateKey(pkey, NULL));
		unsigned char *dptr=reinterpret_cast<unsigned char *>(key.data());
		i2d_PrivateKey(pkey, &dptr);

		qskKey = QSslKey(key, QSsl::Rsa, QSsl::Der);
		if (qskKey.isNull()) {
			ok = false;
		}
	}

	if (!ok) {
		qscCert = QSslCertificate();
		qskKey = QSslKey();
	}

	return ok;
}

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
// dh_progress is a status callback for DH_generate_parameters_ex.
// We use it to run the event loop while generating DH params, in
// order to keep the Murmur GUI on Windows responsive during the
// process.
static int dh_progress(int, int, BN_GENCB *) {
	qApp->processEvents();
	return 1;
}
#endif

bool Server::isKeyForCert(const QSslKey &key, const QSslCertificate &cert) {
	if (key.isNull() || cert.isNull() || (key.type() != QSsl::PrivateKey))
		return false;

	QByteArray qbaKey = key.toDer();
	QByteArray qbaCert = cert.toDer();

	X509 *x509 = NULL;
	EVP_PKEY *pkey = NULL;
	BIO *mem = NULL;

	mem = BIO_new_mem_buf(qbaKey.data(), qbaKey.size());
	Q_UNUSED(BIO_set_close(mem, BIO_NOCLOSE));
	pkey = d2i_PrivateKey_bio(mem, NULL);
	BIO_free(mem);

	mem = BIO_new_mem_buf(qbaCert.data(), qbaCert.size());
	Q_UNUSED(BIO_set_close(mem, BIO_NOCLOSE));
	x509 = d2i_X509_bio(mem, NULL);
	BIO_free(mem);
	mem = NULL;

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

	// Clear all exising SSL settings
	// for this server.
	qscCert.clear();
	qlIntermediates.clear();
	qskKey.clear();
#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
	qsdhpDHParams = QSslDiffieHellmanParameters();
#endif

	crt = getConf("certificate", QString()).toByteArray();
	key = getConf("key", QString()).toByteArray();
	pass = getConf("passphrase", QByteArray()).toByteArray();
	dhparams = getConf("sslDHParams", Meta::mp.qbaDHParams).toByteArray();

	QList<QSslCertificate> ql;

	// Attempt to load the private key.
	if (! key.isEmpty()) {
		qskKey = Server::privateKeyFromPEM(key, pass);
	}

	// If we still can't load the key, try loading any keys from the certificate
	if (qskKey.isNull() && ! crt.isEmpty()) {
		qskKey = Server::privateKeyFromPEM(crt);
	}

	// If have a key, walk the list of certs, find the one for our key,
	// remove any certs for our key from the list, what's left is part of
	// the CA certificate chain.
	if (! qskKey.isNull()) {
		ql << QSslCertificate::fromData(crt);
		ql << QSslCertificate::fromData(key);
		for (int i=0;i<ql.size();++i) {
			const QSslCertificate &c = ql.at(i);
			if (isKeyForCert(qskKey, c)) {
				qscCert = c;
				ql.removeAt(i);
			}
		}
		qlIntermediates = ql;
	}

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
	if (! dhparams.isEmpty()) {
		QSslDiffieHellmanParameters qdhp = QSslDiffieHellmanParameters::fromEncoded(dhparams);
		if (qdhp.isValid()) {
			qsdhpDHParams = qdhp;
		} else {
			log(QString::fromLatin1("Unable to use specified Diffie-Hellman parameters (sslDHParams): %1").arg(qdhp.errorString()));
		}
	}
#else
	if (! dhparams.isEmpty()) {
		log("Diffie-Hellman parameters (sslDHParams) were specified, but will not be used. This version of Murmur does not support Diffie-Hellman parameters.");
	}
#endif

	QString issuer;
#if QT_VERSION >= 0x050000
	QStringList issuerNames = qscCert.issuerInfo(QSslCertificate::CommonName);
	if (! issuerNames.isEmpty()) {
		issuer = issuerNames.first();
	}
#else
	issuer = qscCert.issuerInfo(QSslCertificate::CommonName);
#endif

	// Really old certs/keys are no good, throw them away so we can
	// generate a new one below.
	if (issuer == QString::fromUtf8("Murmur Autogenerated Certificate")) {
		log("Old autogenerated certificate is unusable for registration, invalidating it");
		qscCert = QSslCertificate();
		qskKey = QSslKey();
	}

	// If we have a cert, and it's a self-signed one, but we're binding to
	// all the same addresses as the Meta server is, use it's cert instead.
	// This allows a self-signed certificate generated by Murmur to be
	// replaced by a CA-signed certificate in the .ini file.
	if (!qscCert.isNull() && issuer == QString::fromUtf8("Murmur Autogenerated Certificate v2") && ! Meta::mp.qscCert.isNull() && ! Meta::mp.qskKey.isNull() && (Meta::mp.qlBind == qlBind)) {
		qscCert = Meta::mp.qscCert;
		qskKey = Meta::mp.qskKey;
		qlIntermediates = Meta::mp.qlIntermediates;

		if (!qscCert.isNull() && !qskKey.isNull()) {
			bUsingMetaCert = true;
		}
	}

	// If we still don't have a certificate by now, try to load the one from Meta
	if (qscCert.isNull() || qskKey.isNull()) {
		if (! key.isEmpty() || ! crt.isEmpty()) {
			log("Certificate specified, but failed to load.");
		}

		qskKey = Meta::mp.qskKey;
		qscCert = Meta::mp.qscCert;
		qlIntermediates = Meta::mp.qlIntermediates;

		if (!qscCert.isNull() && !qskKey.isNull()) {
			bUsingMetaCert = true;
		}

		// If loading from Meta doesn't work, build+sign a new one
		if (qscCert.isNull() || qskKey.isNull()) {
			log("Generating new server certificate.");

			if (!selfSignedServerCert_SHA1_RSA_2048(qscCert, qskKey)) {
				log("Certificate or key generation failed");
			}

			setConf("certificate", qscCert.toPem());
			setConf("key", qskKey.toPem());
		}
	}

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
	if (qsdhpDHParams.isEmpty()) {
		log("Generating new server 2048-bit Diffie-Hellman parameters. This could take a while...");

		DH *dh = DH_new();
		if (dh == NULL) {
			qFatal("DH_new failed: unable to generate Diffie-Hellman parameters for virtual server");
		}

		// Generate DH params.
		// We register a status callback in order to update the UI
		// for Murmur on Windows. We don't show the actual status,
		// but we do it to keep Murmur on Windows responsive while
		// generating the parameters.
		BN_GENCB *cb = mumble_BN_GENCB_new();
		BN_GENCB_set(cb, dh_progress, NULL);
		if (DH_generate_parameters_ex(dh, 2048, 2, cb) == 0) {
			qFatal("DH_generate_parameters_ex failed: unable to generate Diffie-Hellman parameters for virtual server");
		}

		BIO *mem = BIO_new(BIO_s_mem());
		if (PEM_write_bio_DHparams(mem, dh) == 0) {
			qFatal("PEM_write_bio_DHparams failed: unable to write generated Diffie-Hellman parameters to memory");
		}

		char *pem = NULL;
		long len = BIO_get_mem_data(mem, &pem);
		if (len <= 0) {
			qFatal("BIO_get_mem_data returned an empty or invalid buffer");
		}

		QByteArray pemdh(pem, len);
		QSslDiffieHellmanParameters qdhp = QSslDiffieHellmanParameters::fromEncoded(pemdh);
		if (!qdhp.isValid()) {
			qFatal("QSslDiffieHellmanParameters: unable to import generated Diffie-HellmanParameters: %s", qdhp.errorString().toStdString().c_str());
		}

		qsdhpDHParams = qdhp;
		setConf("sslDHParams", pemdh);

		BIO_free(mem);
		mumble_BN_GENCB_free(cb);
		DH_free(dh);
	}
#endif

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
	return QString::fromLatin1(qscCert.digest(QCryptographicHash::Sha1).toHex());
}
