/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include <QtNetwork>
#include "SSL.h"

void MumbleSSL::addSystemCA() {
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

		while(pCertContext = CertEnumCertificatesInStore(hCertStore, pCertContext)) {
			QByteArray qba(reinterpret_cast<const char *>(pCertContext->pbCertEncoded), pCertContext->cbCertEncoded);

			QList<QSslCertificate> ql = QSslCertificate::fromData(qba, QSsl::Pem);
			ql.append(QSslCertificate::fromData(qba, QSsl::Der));
			if (! ql.isEmpty()) {
				found = true;
				QSslSocket::addDefaultCaCertificates(ql);
			}
		}
		if (found)
				qWarning("SSL: Added CA certificates from system store '%s'", qPrintable(store));

		CertCloseStore(hCertStore, 0);
	}

#elif defined(Q_OS_UNIX)
	QStringList qsl;

	qsl << QLatin1String("/etc/pki/tls/certs/ca-bundle.crt");
	qsl << QLatin1String("/etc/ssl/certs/ca-certificates.crt");

	foreach(const QString &filename, qsl) {
		QFile f(filename);
		if (f.exists() && f.open(QIODevice::ReadOnly)) {
			QList<QSslCertificate> ql = QSslCertificate::fromDevice(&f, QSsl::Pem);
			ql.append(QSslCertificate::fromDevice(&f, QSsl::Der));
			if (! ql.isEmpty()) {
				qWarning("SSL: Added CA certificates from '%s'", qPrintable(filename));
				QSslSocket::addDefaultCaCertificates(ql);
			}
		}
	}
#endif
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
}
