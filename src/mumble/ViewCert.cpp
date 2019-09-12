// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ViewCert.h"

#include <QtCore/QUrl>
#include <QtNetwork/QSslKey>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>

static QString decode_utf8_qssl_string(const QString &input) {
	QString i = input;
	return QUrl::fromPercentEncoding(i.replace(QLatin1String("\\x"), QLatin1String("%")).toLatin1());
}

#if QT_VERSION >= 0x050000
static QStringList processQSslCertificateInfo(QStringList in) {
	QStringList list;
	foreach (QString str, in) {
		list << decode_utf8_qssl_string(str);
	}
	return list;
}
#else
static QStringList processQSslCertificateInfo(QString in) {
	QStringList out;
	out << decode_utf8_qssl_string(in);
	return out;
}
#endif

static void addQSslCertificateInfo(QStringList &l, const QString &label, const QStringList &items) {
	foreach (const QString &item, items) {
		l << QString(QLatin1String("%1: %2")).arg(label, item);
	}
}

static QString certificateFriendlyName(const QSslCertificate &cert) {
	QStringList cnList = processQSslCertificateInfo(cert.subjectInfo(QSslCertificate::CommonName));
	QStringList orgList = processQSslCertificateInfo(cert.subjectInfo(QSslCertificate::Organization));

	QString cn;
	if (cnList.count() > 0) {
		cn = cnList.at(0);
	}

	QString org;
	if (orgList.count() > 0) {
		org = orgList.at(0);
	}

	return QString(QLatin1String("%1 %2")).arg(cn, org);
}

ViewCert::ViewCert(QList<QSslCertificate> cl, QWidget *p) : QDialog(p) {
	qlCerts = cl;

	setWindowTitle(tr("Certificate Chain Details"));

	QHBoxLayout *h;
	QVBoxLayout *v;
	QGroupBox *qcbChain, *qcbDetails;

	qcbChain=new QGroupBox(tr("Certificate chain"), this);
	h = new QHBoxLayout(qcbChain);
	qlwChain = new QListWidget(qcbChain);
	qlwChain->setObjectName(QLatin1String("Chain"));

	// load certs into a set as a hacky fix to #2141
#if QT_VERSION >= 0x050400
	QSet<QSslCertificate> qlCertSet;
#else
	QList<QSslCertificate> qlCertSet;
#endif
	foreach(QSslCertificate c, qlCerts) {
		if(!qlCertSet.contains(c)) {
			qlwChain->addItem(certificateFriendlyName(c));
			qlCertSet << c;
		}
	}
	h->addWidget(qlwChain);

	qcbDetails=new QGroupBox(tr("Certificate details"), this);
	h = new QHBoxLayout(qcbDetails);
	qlwCert = new QListWidget(qcbDetails);
	h->addWidget(qlwCert);

	QDialogButtonBox *qdbb = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, this);

	v = new QVBoxLayout(this);
	v->addWidget(qcbChain);
	v->addWidget(qcbDetails);
	v->addWidget(qdbb);

	QMetaObject::connectSlotsByName(this);
	connect(qdbb, SIGNAL(accepted()), this, SLOT(accept()));

	resize(510,300);
}

QString ViewCert::prettifyDigest(QString digest) {
	QString pretty_digest = digest.toUpper();
	int step = 2;
	QChar separator = QChar::fromLatin1(':');
	for (int i = step; i < pretty_digest.size(); i += step + 1) {
		pretty_digest.insert(i, separator);
	}
	return pretty_digest;
}

void ViewCert::on_Chain_currentRowChanged(int idx) {
	qlwCert->clear();
	if ((idx < 0) || (idx >= qlCerts.size()))
		return;

	QStringList l;
	const QSslCertificate &c=qlCerts.at(idx);

	addQSslCertificateInfo(l, tr("Common Name"), processQSslCertificateInfo(c.subjectInfo(QSslCertificate::CommonName)));
	addQSslCertificateInfo(l, tr("Organization"), processQSslCertificateInfo(c.subjectInfo(QSslCertificate::Organization)));
	addQSslCertificateInfo(l, tr("Subunit"), processQSslCertificateInfo(c.subjectInfo(QSslCertificate::OrganizationalUnitName)));
	addQSslCertificateInfo(l, tr("Country"), processQSslCertificateInfo(c.subjectInfo(QSslCertificate::CountryName)));
	addQSslCertificateInfo(l, tr("Locality"), processQSslCertificateInfo(c.subjectInfo(QSslCertificate::LocalityName)));
	addQSslCertificateInfo(l, tr("State"), processQSslCertificateInfo(c.subjectInfo(QSslCertificate::StateOrProvinceName)));
	l << tr("Valid from: %1").arg(c.effectiveDate().toString());
	l << tr("Valid to: %1").arg(c.expiryDate().toString());
	l << tr("Serial: %1").arg(QString::fromLatin1(c.serialNumber().toHex()));
	l << tr("Public Key: %1 bits %2").arg(c.publicKey().length()).arg((c.publicKey().algorithm() == QSsl::Rsa) ? tr("RSA") : tr("DSA"));
	l << tr("Digest (SHA-1): %1").arg(prettifyDigest(QString::fromLatin1(c.digest(QCryptographicHash::Sha1).toHex())));
#if QT_VERSION >= 0x050000
	l << tr("Digest (SHA-256): %1").arg(prettifyDigest(QString::fromLatin1(c.digest(QCryptographicHash::Sha256).toHex())));
#endif

#if QT_VERSION >= 0x050000
	const QMultiMap<QSsl::AlternativeNameEntryType, QString> &alts = c.subjectAlternativeNames();
	QMultiMap<QSsl::AlternativeNameEntryType, QString>::const_iterator i;
#else
	const QMultiMap<QSsl::AlternateNameEntryType, QString> &alts = c.alternateSubjectNames();
	QMultiMap<QSsl::AlternateNameEntryType, QString>::const_iterator i;
#endif

	for (i=alts.constBegin(); i != alts.constEnd(); ++i) {
		switch (i.key()) {
			case QSsl::EmailEntry: {
					l << tr("Email: %1").arg(i.value());
				}
				break;
			case QSsl::DnsEntry: {
					l << tr("DNS: %1").arg(i.value());
				}
				break;
			default:
				break;
		}
	}

	l << QString();
	l << tr("Issued by:");
	addQSslCertificateInfo(l, tr("Common Name"), processQSslCertificateInfo(c.issuerInfo(QSslCertificate::CommonName)));
	addQSslCertificateInfo(l, tr("Organization"), processQSslCertificateInfo(c.issuerInfo(QSslCertificate::Organization)));
	addQSslCertificateInfo(l, tr("Unit Name"), processQSslCertificateInfo(c.issuerInfo(QSslCertificate::OrganizationalUnitName)));
	addQSslCertificateInfo(l, tr("Country"), processQSslCertificateInfo(c.issuerInfo(QSslCertificate::CountryName)));
	addQSslCertificateInfo(l, tr("Locality"), processQSslCertificateInfo(c.issuerInfo(QSslCertificate::LocalityName)));
	addQSslCertificateInfo(l, tr("State"), processQSslCertificateInfo(c.issuerInfo(QSslCertificate::StateOrProvinceName)));

	qlwCert->addItems(l);
}
