// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "ViewCert.h"

static QString decode_utf8_qssl_string(const QString &input) {
	QString i = input;
	return QUrl::fromPercentEncoding(i.replace(QLatin1String("\\x"), QLatin1String("%")).toLatin1());
}

#if QT_VERSION >= 0x050000
static QString decode_utf8_qssl_string(const QStringList &list) {
	if (list.count() > 0) {
		return decode_utf8_qssl_string(list.at(0));
	}
	return QString();
}
#endif

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

	foreach(QSslCertificate c, qlCerts)
		qlwChain->addItem(tr("%1 %2").arg(decode_utf8_qssl_string(c.subjectInfo(QSslCertificate::CommonName)), decode_utf8_qssl_string(c.subjectInfo(QSslCertificate::Organization))));
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

void ViewCert::on_Chain_currentRowChanged(int idx) {
	qlwCert->clear();
	if ((idx < 0) || (idx >= qlCerts.size()))
		return;

	QStringList l;
	const QSslCertificate &c=qlCerts.at(idx);

	l << tr("Common Name: %1").arg(decode_utf8_qssl_string(c.subjectInfo(QSslCertificate::CommonName)));
	l << tr("Organization: %1").arg(decode_utf8_qssl_string(c.subjectInfo(QSslCertificate::Organization)));
	l << tr("Subunit: %1").arg(decode_utf8_qssl_string(c.subjectInfo(QSslCertificate::OrganizationalUnitName)));
	l << tr("Country: %1").arg(decode_utf8_qssl_string(c.subjectInfo(QSslCertificate::CountryName)));
	l << tr("Locality: %1").arg(decode_utf8_qssl_string(c.subjectInfo(QSslCertificate::LocalityName)));
	l << tr("State: %1").arg(decode_utf8_qssl_string(c.subjectInfo(QSslCertificate::StateOrProvinceName)));
	l << tr("Valid from: %1").arg(c.effectiveDate().toString());
	l << tr("Valid to: %1").arg(c.expiryDate().toString());
	l << tr("Serial: %1").arg(QString::fromLatin1(c.serialNumber().toHex()));
	l << tr("Public Key: %1 bits %2").arg(c.publicKey().length()).arg((c.publicKey().algorithm() == QSsl::Rsa) ? tr("RSA") : tr("DSA"));
	l << tr("Digest (SHA-1): %1").arg(QString::fromLatin1(c.digest(QCryptographicHash::Sha1).toHex()));

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
	l << tr("Common Name: %1").arg(decode_utf8_qssl_string(c.issuerInfo(QSslCertificate::CommonName)));
	l << tr("Organization: %1").arg(decode_utf8_qssl_string(c.issuerInfo(QSslCertificate::Organization)));
	l << tr("Unit Name: %1").arg(decode_utf8_qssl_string(c.issuerInfo(QSslCertificate::OrganizationalUnitName)));
	l << tr("Country: %1").arg(decode_utf8_qssl_string(c.issuerInfo(QSslCertificate::CountryName)));
	l << tr("Locality: %1").arg(decode_utf8_qssl_string(c.issuerInfo(QSslCertificate::LocalityName)));
	l << tr("State: %1").arg(decode_utf8_qssl_string(c.issuerInfo(QSslCertificate::StateOrProvinceName)));

	qlwCert->addItems(l);
}
