/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "murmur_pch.h"

#include "SSL.h"

#include "Version.h"

void MumbleSSL::addSystemCA() {
#if QT_VERSION < 0x040700 && !defined(NO_SYSTEM_CA_OVERRIDE)
#if defined(Q_OS_WIN)
	QStringList qsl;
	qsl << QLatin1String("Ca");
	qsl << QLatin1String("Root");
	qsl << QLatin1String("AuthRoot");
	foreach(const QString &store, qsl) {
		HCERTSTORE hCertStore;
		PCCERT_CONTEXT pCertContext = NULL;

		bool found = false;

		hCertStore = CertOpenSystemStore(NULL, store.utf16());
		if (! hCertStore) {
			qWarning("SSL: Failed to open CA store %s", qPrintable(store));
			continue;
		}

		while (pCertContext = CertEnumCertificatesInStore(hCertStore, pCertContext)) {
			QByteArray qba(reinterpret_cast<const char *>(pCertContext->pbCertEncoded), pCertContext->cbCertEncoded);

			QList<QSslCertificate> ql = QSslCertificate::fromData(qba, QSsl::Pem);
			ql += QSslCertificate::fromData(qba, QSsl::Der);
			if (! ql.isEmpty()) {
				found = true;
				QSslSocket::addDefaultCaCertificates(ql);
			}
		}
		if (found)
			qWarning("SSL: Added CA certificates from system store '%s'", qPrintable(store));

		CertCloseStore(hCertStore, 0);
	}

#elif defined(Q_OS_MAC)
	CFArrayRef certs = NULL;
	bool found = false;

	if (SecTrustCopyAnchorCertificates(&certs) == noErr) {
		int ncerts = CFArrayGetCount(certs);
		for (int i = 0; i < ncerts; i++) {
			CFDataRef data = NULL;

			SecCertificateRef cert = reinterpret_cast<SecCertificateRef>(const_cast<void *>(CFArrayGetValueAtIndex(certs, i)));
			if (! cert)
				continue;

			if (SecKeychainItemExport(cert, kSecFormatX509Cert, kSecItemPemArmour, NULL, &data) == noErr) {
				const char *ptr = reinterpret_cast<const char *>(CFDataGetBytePtr(data));
				int len = CFDataGetLength(data);
				QByteArray qba(ptr, len);

				QList<QSslCertificate> ql = QSslCertificate::fromData(qba, QSsl::Pem);
				if (! ql.isEmpty()) {
					found = true;
					QSslSocket::addDefaultCaCertificates(ql);
				}
			}
		}

		CFRelease(certs);

		if (found)
			qWarning("SSL: Added CA certificates from 'System Roots' store.");
	}
#elif defined(Q_OS_UNIX)
	QStringList qsl;

#ifdef SYSTEM_CA_DIR
	QSslSocket::addDefaultCaCertificates(QLatin1String(MUMTEXT(SYSTEM_CA_DIR)));
#else
#ifdef SYSTEM_CA_BUNDLE
	qsl << QLatin1String(MUMTEXT(SYSTEM_CA_BUNDLE));
#else
#ifdef __FreeBSD__
	qsl << QLatin1String("/usr/local/share/certs/ca-root-nss.crt");
#else
	qsl << QLatin1String("/etc/pki/tls/certs/ca-bundle.crt");
	qsl << QLatin1String("/etc/ssl/certs/ca-certificates.crt");
#endif
#endif

	foreach(const QString &filename, qsl) {
		QFile f(filename);
		if (f.exists() && f.open(QIODevice::ReadOnly)) {
			QList<QSslCertificate> ql = QSslCertificate::fromDevice(&f, QSsl::Pem);
			ql += QSslCertificate::fromDevice(&f, QSsl::Der);
			if (! ql.isEmpty()) {
				qWarning("SSL: Added CA certificates from '%s'", qPrintable(filename));
				QSslSocket::addDefaultCaCertificates(ql);
			}
		}
	}
#endif // SYSTEM_CA_DIR
#endif // Q_OS_UNIX

	QSet<QByteArray> digests;
	QList<QSslCertificate> ql;
	foreach(const QSslCertificate &crt, QSslSocket::defaultCaCertificates()) {
		QByteArray digest = crt.digest(QCryptographicHash::Sha1);
		if (! digests.contains(digest) && crt.isValid()) {
			ql << crt;
			digests.insert(digest);
		}
	}
	QSslSocket::setDefaultCaCertificates(ql);
#endif // NO_SYSTEM_CA_OVERRIDE

#if QT_VERSION >= 0x040800
	// Don't perform on-demand loading of root certificates
	QSslSocket::addDefaultCaCertificates(QSslSocket::systemCaCertificates());
#endif

#ifdef Q_OS_WIN
	// Work around issue #1271.
	// Skype's click-to-call feature creates an enormous
	// amount of certificates in the Root CA store.
	{
		QSslConfiguration sslCfg = QSslConfiguration::defaultConfiguration();
		QList<QSslCertificate> caList = sslCfg.caCertificates();

		QList<QSslCertificate> filteredCaList;
		foreach (QSslCertificate cert, caList) {
			QString ou = cert.subjectInfo(QSslCertificate::Organization);
			if (ou.contains(QLatin1String("Skype"), Qt::CaseInsensitive)) {
				continue;
			}
			filteredCaList.append(cert);
		}

		sslCfg.setCaCertificates(filteredCaList);
		QSslConfiguration::setDefaultConfiguration(sslCfg);

		qWarning("SSL: CA certificate filter applied. Filtered size: %i, original size: %i", filteredCaList.size(), caList.size());
	}
#endif
}
