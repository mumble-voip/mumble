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

#include "ViewCert.h"

static QString decode_utf8_qssl_string(const QString &input) {
	QString i = input;
	return QUrl::fromPercentEncoding(i.replace(QLatin1String("\\x"), QLatin1String("%")).toLatin1());
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

	resize(500,300);
}

void ViewCert::on_Chain_currentRowChanged(int idx) {
	qlwCert->clear();
	if ((idx < 0) || (idx >= qlCerts.size()))
		return;

	QStringList l;
	const QSslCertificate &c=qlCerts.at(idx);
	const QMultiMap<QSsl::AlternateNameEntryType, QString> &alts = c.alternateSubjectNames();
	QMultiMap<QSsl::AlternateNameEntryType, QString>::const_iterator i;

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
	l << tr("Digest (MD5): %1").arg(QString::fromLatin1(c.digest().toHex()));
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
