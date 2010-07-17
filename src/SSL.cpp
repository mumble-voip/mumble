/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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
#include "Version.h"

/* CAs we recommend to end users, so support these */

static const char *recommended_cas[] = {
	/* StartSSL */
	"-----BEGIN CERTIFICATE-----\n"
	"MIIHyTCCBbGgAwIBAgIBATANBgkqhkiG9w0BAQUFADB9MQswCQYDVQQGEwJJTDEW\n"
	"MBQGA1UEChMNU3RhcnRDb20gTHRkLjErMCkGA1UECxMiU2VjdXJlIERpZ2l0YWwg\n"
	"Q2VydGlmaWNhdGUgU2lnbmluZzEpMCcGA1UEAxMgU3RhcnRDb20gQ2VydGlmaWNh\n"
	"dGlvbiBBdXRob3JpdHkwHhcNMDYwOTE3MTk0NjM2WhcNMzYwOTE3MTk0NjM2WjB9\n"
	"MQswCQYDVQQGEwJJTDEWMBQGA1UEChMNU3RhcnRDb20gTHRkLjErMCkGA1UECxMi\n"
	"U2VjdXJlIERpZ2l0YWwgQ2VydGlmaWNhdGUgU2lnbmluZzEpMCcGA1UEAxMgU3Rh\n"
	"cnRDb20gQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUA\n"
	"A4ICDwAwggIKAoICAQDBiNsJvGxGfHiflXu1M5DycmLWwTYgIiRezul38kMKogZk\n"
	"pMyONvg45iPwbm2xPN1yo4UcodM9tDMr0y+v/uqwQVlntsQGfQqedIXWeUyAN3rf\n"
	"OQVSWff0G0ZDpNKFhdLDcfN1YjS6LIp/Ho/u7TTQEceWzVI9ujPW3U3eCztKS5/C\n"
	"Ji/6tRYccjV3yjxd5srhJosaNnZcAdt0FCX+7bWgiA/deMotHweXMAEtcnn6RtYT\n"
	"Kqi5pquDSR3l8u/d5AGOGAqPY1MWhWKpDhk6zLVmpsJrdAfkK+F2PrRt2PZE4XNi\n"
	"HzvEvqBTViVsUQn3qqvKv3b9bZvzndu/PWa8DFaqr5hIlTpL36dYUNk4dalb6kMM\n"
	"Av+Z6+hsTXBbKWWc3apdzK8BMewM69KN6Oqce+Zu9ydmDBpI125C4z/eIT574Q1w\n"
	"+2OqqGwaVLRcJXrJosmLFqa7LH4XXgVNWG4SHQHuEhANxjJ/GP/89PrNbpHoNkm+\n"
	"Gkhpi8KWTRoSsmkXwQqQ1vp5Iki/untp+HDH+no32NgN0nZPV/+Qt+OR0t3vwmC3\n"
	"Zzrd/qqc8NSLf3Iizsafl7b4r4qgEKjZ+xjGtrVcUjyJthkqcwEKDwOzEmDyei+B\n"
	"26Nu/yYwl/WL3YlXtq09s68rxbd2AvCl1iuahhQqcvbjM4xdCUsT37uMdBNSSwID\n"
	"AQABo4ICUjCCAk4wDAYDVR0TBAUwAwEB/zALBgNVHQ8EBAMCAa4wHQYDVR0OBBYE\n"
	"FE4L7xqkQFulF2mHMMo0aEPQQa7yMGQGA1UdHwRdMFswLKAqoCiGJmh0dHA6Ly9j\n"
	"ZXJ0LnN0YXJ0Y29tLm9yZy9zZnNjYS1jcmwuY3JsMCugKaAnhiVodHRwOi8vY3Js\n"
	"LnN0YXJ0Y29tLm9yZy9zZnNjYS1jcmwuY3JsMIIBXQYDVR0gBIIBVDCCAVAwggFM\n"
	"BgsrBgEEAYG1NwEBATCCATswLwYIKwYBBQUHAgEWI2h0dHA6Ly9jZXJ0LnN0YXJ0\n"
	"Y29tLm9yZy9wb2xpY3kucGRmMDUGCCsGAQUFBwIBFilodHRwOi8vY2VydC5zdGFy\n"
	"dGNvbS5vcmcvaW50ZXJtZWRpYXRlLnBkZjCB0AYIKwYBBQUHAgIwgcMwJxYgU3Rh\n"
	"cnQgQ29tbWVyY2lhbCAoU3RhcnRDb20pIEx0ZC4wAwIBARqBl0xpbWl0ZWQgTGlh\n"
	"YmlsaXR5LCByZWFkIHRoZSBzZWN0aW9uICpMZWdhbCBMaW1pdGF0aW9ucyogb2Yg\n"
	"dGhlIFN0YXJ0Q29tIENlcnRpZmljYXRpb24gQXV0aG9yaXR5IFBvbGljeSBhdmFp\n"
	"bGFibGUgYXQgaHR0cDovL2NlcnQuc3RhcnRjb20ub3JnL3BvbGljeS5wZGYwEQYJ\n"
	"YIZIAYb4QgEBBAQDAgAHMDgGCWCGSAGG+EIBDQQrFilTdGFydENvbSBGcmVlIFNT\n"
	"TCBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTANBgkqhkiG9w0BAQUFAAOCAgEAFmyZ\n"
	"9GYMNPXQhV59CuzaEE44HF7fpiUFS5Eyweg78T3dRAlbB0mKKctmArexmvclmAk8\n"
	"jhvh3TaHK0u7aNM5Zj2gJsfyOZEdUauCe37Vzlrk4gNXcGmXCPleWKYK34wGmkUW\n"
	"FjgKXlf2Ysd6AgXmvB618p70qSmD+LIU424oh0TDkBreOKk8rENNZEXO3SipXPJz\n"
	"ewT4F+irsfMuXGRuczE6Eri8sxHkfY+BUZo7jYn0TZNmezwD7dOaHZrzZVD1oNB1\n"
	"ny+v8OqCQ5j4aZyJecRDjkZy42Q2Eq/3JR44iZB3fsNrarnDy0RLrHiQi+fHLB5L\n"
	"EUTINFInzQpdn4XBidUaePKVEFMy3YCEZnXZtWgo+2EuvoSoOMCZEoalHmdkrQYu\n"
	"L6lwhceWD3yJZfWOQ1QOq92lgDmUYMA0yZZwLKMS9R9Ie70cfmu3nZD0Ijuu+Pwq\n"
	"yvqCUqDvr0tVk+vBtfAii6w0TiYiBKGHLHVKt+V9E9e4DGTANtLJL4YSjCMJwRuC\n"
	"O3NJo2pXh5Tl1njFmUNj403gdy3hZZlyaQQaRwnmDwFWJPsfvw55qVguucQJAX6V\n"
	"um0ABj6y6koQOdjQK/W/7HW/lwLFCRsI3FU34oH7N4RDYiDK51ZLZer+bMEkkySh\n"
	"NOsF/5oirpt9P/FlUQqmMGqz9IgcgA38corog14=\n"
	"-----END CERTIFICATE-----\n"
	,
	/* Comodo */
	"-----BEGIN CERTIFICATE-----\n"
	"MIIEMjCCAxqgAwIBAgIBATANBgkqhkiG9w0BAQUFADB7MQswCQYDVQQGEwJHQjEb\n"
	"MBkGA1UECAwSR3JlYXRlciBNYW5jaGVzdGVyMRAwDgYDVQQHDAdTYWxmb3JkMRow\n"
	"GAYDVQQKDBFDb21vZG8gQ0EgTGltaXRlZDEhMB8GA1UEAwwYQUFBIENlcnRpZmlj\n"
	"YXRlIFNlcnZpY2VzMB4XDTA0MDEwMTAwMDAwMFoXDTI4MTIzMTIzNTk1OVowezEL\n"
	"MAkGA1UEBhMCR0IxGzAZBgNVBAgMEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE\n"
	"BwwHU2FsZm9yZDEaMBgGA1UECgwRQ29tb2RvIENBIExpbWl0ZWQxITAfBgNVBAMM\n"
	"GEFBQSBDZXJ0aWZpY2F0ZSBTZXJ2aWNlczCCASIwDQYJKoZIhvcNAQEBBQADggEP\n"
	"ADCCAQoCggEBAL5AnfRu4ep2hxxNRUSOvkbIgwadwSr+GB+O5AL686tdUIoWMQua\n"
	"BtDFcCLNSS1UY8y2bmhGC1Pqy0wkwLxyTurxFa70VJoSCsN6sjNg4tqJVfMiWPPe\n"
	"3M/vg4aijJRPn2jymJBGhCfHdr/jzDUsi14HZGWCwEiwqJH5YZ92IFCokcdmtet4\n"
	"YgNW8IoaE+oxox6gmf049vYnMlhvB/VruPsUK6+3qszWY19zjNoFmag4qMsXeDZR\n"
	"rOme9Hg6jc8P2ULimAyrL58OAd7vn5lJ8S3frHRNG5i1R8XlKdH5kBjHYpy+g8cm\n"
	"ez6KJcfA3Z3mNWgQIJ2P2N7Sw4ScDV7oL8kCAwEAAaOBwDCBvTAdBgNVHQ4EFgQU\n"
	"oBEKIz6W8Qfs4q8p74Klf9AwpLQwDgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQF\n"
	"MAMBAf8wewYDVR0fBHQwcjA4oDagNIYyaHR0cDovL2NybC5jb21vZG9jYS5jb20v\n"
	"QUFBQ2VydGlmaWNhdGVTZXJ2aWNlcy5jcmwwNqA0oDKGMGh0dHA6Ly9jcmwuY29t\n"
	"b2RvLm5ldC9BQUFDZXJ0aWZpY2F0ZVNlcnZpY2VzLmNybDANBgkqhkiG9w0BAQUF\n"
	"AAOCAQEACFb8AvCb6P+k+tZ7xkSAzk/ExfYAWMymtrwUSWgEdujm7l3sAg9g1o1Q\n"
	"GE8mTgHj5rCl7r+8dFRBv/38ErjHT1r0iWAFf2C3BUrz9vHCv8S5dIa2LX1rzNLz\n"
	"Rt0vxuBqw8M0Ayx9lt1awg6nCpnBBYurDC/zXDrPbDdVCYfeU0BsWO/8tqtlbgT2\n"
	"G9w84FoVxp7Z8VlIMCFlA2zs6SFz7JsDoeA3raAVGI/6ugLOpyypEBMs1OUIJqsi\n"
	"l2D4kF501KKaU73yqWjgom7C12yxow+ev+to51byrvLjKzg6CYG1a4XXvi3tPxq3\n"
	"smPi9WIsgtRqAEFQ8TmDn5XpNpaYbg==\n"
	"-----END CERTIFICATE-----\n"
	,
	/* Comodo UserTrust */
	"-----BEGIN CERTIFICATE-----\n"
	"MIIE3TCCA8WgAwIBAgIQcZL75hlfrE0ShXRxNKIYpzANBgkqhkiG9w0BAQUFADB7\n"
	"MQswCQYDVQQGEwJHQjEbMBkGA1UECAwSR3JlYXRlciBNYW5jaGVzdGVyMRAwDgYD\n"
	"VQQHDAdTYWxmb3JkMRowGAYDVQQKDBFDb21vZG8gQ0EgTGltaXRlZDEhMB8GA1UE\n"
	"AwwYQUFBIENlcnRpZmljYXRlIFNlcnZpY2VzMB4XDTA0MDEwMTAwMDAwMFoXDTI4\n"
	"MTIzMTIzNTk1OVowga4xCzAJBgNVBAYTAlVTMQswCQYDVQQIEwJVVDEXMBUGA1UE\n"
	"BxMOU2FsdCBMYWtlIENpdHkxHjAcBgNVBAoTFVRoZSBVU0VSVFJVU1QgTmV0d29y\n"
	"azEhMB8GA1UECxMYaHR0cDovL3d3dy51c2VydHJ1c3QuY29tMTYwNAYDVQQDEy1V\n"
	"VE4tVVNFUkZpcnN0LUNsaWVudCBBdXRoZW50aWNhdGlvbiBhbmQgRW1haWwwggEi\n"
	"MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCyOYWk8n2rQTtiRjeuzcFgdbw5\n"
	"ZflKGkeiucxIzGqY1U01GbmkQuXOSeKKLx580jEHx060g2SdLinVomTEhb2FUTV5\n"
	"pE5okHsceqSSqBfymBXyk8zJpDKVuwxPML2YoAuL5W4bokb6eLyib6tZXqUvz8ra\n"
	"baov66yhs2qqty5nNYt54R5piOLmRs2gpeq+C852OnoOm+r82idbPXMfIuZIYcZM\n"
	"82mxqC4bttQxICy8goqOpA6l14lD/BZarx1x1xFZ2rqHDa/68+HC8KTFZ4zW1lQ6\n"
	"3gqkugN3s2XI/R7TdGKqGMpokx6hhX71R2XL+E1XKHTSNP8wtu72YjAUjCzrAgMB\n"
	"AAGjggEnMIIBIzAfBgNVHSMEGDAWgBSgEQojPpbxB+zirynvgqV/0DCktDAdBgNV\n"
	"HQ4EFgQUiYJnfcSdJnAAS7RQSHzePa4Ebn0wDgYDVR0PAQH/BAQDAgEGMA8GA1Ud\n"
	"EwEB/wQFMAMBAf8wHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMEMBEGA1Ud\n"
	"IAQKMAgwBgYEVR0gADB7BgNVHR8EdDByMDigNqA0hjJodHRwOi8vY3JsLmNvbW9k\n"
	"b2NhLmNvbS9BQUFDZXJ0aWZpY2F0ZVNlcnZpY2VzLmNybDA2oDSgMoYwaHR0cDov\n"
	"L2NybC5jb21vZG8ubmV0L0FBQUNlcnRpZmljYXRlU2VydmljZXMuY3JsMBEGCWCG\n"
	"SAGG+EIBAQQEAwIBBjANBgkqhkiG9w0BAQUFAAOCAQEAnZXLPLh+pQnEHr8Lwsd2\n"
	"jjk8lMYQqk8MyeCrhF2JVOBlO/NtHHw3LCVUX5Yh/DeEkZ0V3BRPgc9UHWtsRWDH\n"
	"LfmXUUz5Zso8oIKMpsjw4unUSvnsP1bJ3XaMw4IBT2wA8x4aYXQERwOpxkBXkbxl\n"
	"IsUnZ09X22Ra2Y0fuoYv9AaunGnt6fTPKRfY4EqfGiAvl0xRu0YHxIo3TiDjCTFo\n"
	"x57Ei53ofhG8MmgQlhGYRNgqUWBNiOt0Ot9DBjLIOVaMOhFS00GkQwP07e8zJ9s5\n"
	"4BROJsnY9TniibiTXbcpJkHqs5uug/x3dcroyrX+4mVKYz5ExNDDXodzqZgcr38V\n"
	"fw==\n"
	"-----END CERTIFICATE-----\n"


};


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
	QSslSocket::addDefaultCaCertificates(MUMTEXT(SYSTEM_CA_DIR));
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

	for (unsigned int i=0;i<sizeof(recommended_cas)/sizeof(recommended_cas[0]);++i) {
		QSslCertificate cert(recommended_cas[i]);
		if (! QSslSocket::defaultCaCertificates().contains(cert)) {
			qWarning("SSL: Adding recommended CA %s", qPrintable(cert.subjectInfo(QSslCertificate::CommonName)));
			QSslSocket::addDefaultCaCertificates(QList<QSslCertificate>() << cert);
		}
	}
}
