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

#include "Cert.h"

CertView::CertView(QWidget *p) : QGroupBox(p) {
	QGridLayout *grid = new QGridLayout(this);
	QLabel *l;

	l = new QLabel(tr("Name"));
	grid->addWidget(l, 0, 0, 1, 1);

	qlSubjectName = new QLabel();
	qlSubjectName->setWordWrap(true);
	grid->addWidget(qlSubjectName, 0, 1, 1, 1);

	l = new QLabel(tr("Email"));
	grid->addWidget(l, 1, 0, 1, 1);

	qlSubjectEmail = new QLabel();
	qlSubjectEmail->setWordWrap(true);
	grid->addWidget(qlSubjectEmail, 1, 1, 1, 1);

	l = new QLabel(tr("Issuer"));
	grid->addWidget(l, 2, 0, 1, 1);

	qlIssuerName = new QLabel();
	qlIssuerName->setWordWrap(true);
	grid->addWidget(qlIssuerName, 2, 1, 1, 1);
}

void CertView::setCert(const QList<QSslCertificate> &cert) {
	qlCert = cert;

	if (qlCert.isEmpty()) {
		qlSubjectName->setText(QString());
		qlSubjectEmail->setText(QString());
		qlIssuerName->setText(QString());
	} else {
		QSslCertificate qscCert = qlCert.at(0);

		QStringList emails(qscCert.alternateSubjectNames().values(QSsl::EmailEntry));

		const QString &name = qscCert.subjectInfo(QSslCertificate::CommonName);

		qlSubjectName->setText(name);
		qlSubjectEmail->setText(emails.join(QLatin1String("<br />")));

		if (qlCert.count() > 1)
			qscCert = qlCert.last();

		const QString &issuer = qscCert.issuerInfo(QSslCertificate::CommonName);
		qlIssuerName->setText((issuer == name) ? tr("Self-signed") : issuer);
	}
}

CertWizard::CertWizard(QWidget *p) : QWizard(p) {
	setupUi(this);

	bPendingDns = false;
	qwpExport->setCommitPage(true);
	qwpExport->setComplete(false);
}

int CertWizard::nextId() const {
	switch (currentId()) {
		case 0:	// Welcome
			{
				if (qrbCreate->isChecked())
					return 1;
				else if (qrbImport->isChecked())
					return 2;
				else if (qrbExport->isChecked())
					return 3;
			}
		case 2: // Import
			return 4;
		case 4: // Replace
			if (qrbCreate->isChecked())
				return 3;
			if (qrbImport->isChecked())
				return 5;
			return -1;
		case 3: // Export
			if (qrbCreate->isChecked())
				return 5;
			else
				return -1;
		case 1: // New
			return 4;
	}
	return -1;
}

void CertWizard::initializePage(int id) {
	if (id == 3) {
		cvExport->setCert(qscNew);
	}

	if (id == 4) {
		cvNew->setCert(qscNew);
		cvCurrent->setCert(qscCurrent);
	}

	QWizard::initializePage(id);
}

bool CertWizard::validateCurrentPage() {
	if (currentPage() == qwpNew) {
			generateNewCert();
			if (qscNew.isEmpty() || qscNew.at(0).isNull() || qskNew.isNull()) {
				QToolTip::showText(QCursor::pos(), tr("There was an error generating your certificate. Please try again."), this);
				return false;
			}
	}
	if (currentPage() == qwpExport) {
		QByteArray qba = exportCert();
		if (qba.isEmpty()) {
				QToolTip::showText(qleExportFile->mapToGlobal(QPoint(0,0)), tr("Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate."), qleExportFile);
				return false;
		}
		QFile f(qleExportFile->text());
		if (! f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered)) {
				QToolTip::showText(qleExportFile->mapToGlobal(QPoint(0,0)), tr("The file could not be opened for writing. Please use another file."), qleExportFile);
				return false;
		}
		qint64 written = f.write(qba);
		f.close();
		if (written != qba.length()) {
				QToolTip::showText(qleExportFile->mapToGlobal(QPoint(0,0)), tr("The file could not be written successfully. Please use another file."), qleExportFile);
				return false;
		}
	}
	return QWizard::validateCurrentPage();
}

void CertWizard::on_qleEmail_textChanged(const QString &email) {
	if (email.isEmpty())
		qwpNew->setComplete(true);
	else {
		QRegExp ereg(QLatin1String("(.+)@(.+)"), Qt::CaseInsensitive, QRegExp::RegExp2);

		if (ereg.exactMatch(email)) {
			const QString &domain = ereg.cap(2);
			if (! qmDomains.contains(domain)) {
				qwpNew->setComplete(false);
				if (! bPendingDns) {
					bPendingDns = true;
					qlEmailError->setText(tr("Resolving domain %1.").arg(domain));
					QHostInfo::lookupHost(domain, this, SLOT(lookedUp(QHostInfo)));
				}
			} else {
				QHostInfo info = qmDomains.value(domain);
				if (info.error() == QHostInfo::NoError) {
					qlEmailError->setText(QString());
					qwpNew->setComplete(true);
				} else {
					qlEmailError->setText(tr("Resolving domain %1 failed.").arg(domain));
					qwpNew->setComplete(false);
				}
			}
		} else {
			qlEmailError->setText(tr("Email must be blank or a valid email address."));
			qwpNew->setComplete(false);
		}
	}
}

void CertWizard::on_qpbExportFile_clicked() {
	QFileDialog *qfd = new QFileDialog(this, tr("Select file to export certificate to"));
	QStringList qsl;
	qsl << tr("PKCS12 (*.p12 *.pfx)");
	qsl << tr("All (*)");
	qfd->setNameFilters(qsl);
	qfd->setFilter(QDir::Files | QDir::Writable);
	qfd->selectFile(QDir::fromNativeSeparators(qleExportFile->text()));
	if (qfd->exec()) {
		QStringList qsl = qfd->selectedFiles();
		if (! qsl.isEmpty()) {
			QString name = qsl.at(0);
			QFileInfo fi(name);
			if (fi.suffix().isEmpty())
				name += QLatin1String(".p12");
			qleExportFile->setText(QDir::toNativeSeparators(name));
		}
	}
	delete qfd;
}

void CertWizard::on_qleExportFile_textChanged(const QString &text) {
	if (text.isEmpty()) {
		qwpExport->setComplete(false);
		return;
	}

	QFile f(QDir::fromNativeSeparators(text));
	QFileInfo fi(f);
	if (! fi.isWritable()) {
		qwpExport->setComplete(false);
		return;
	}

	if (fi.exists()) {
		qwpExport->setComplete(f.open(QIODevice::WriteOnly | QIODevice::Append));
	} else {
		if (f.open(QIODevice::WriteOnly)) {
			if (f.remove()) {
				qwpExport->setComplete(true);
				return;
			}
		}
		qwpExport->setComplete(false);
	}
}

void CertWizard::lookedUp(QHostInfo info) {
	bPendingDns = false;
	qmDomains.insert(info.hostName(), info);
	on_qleEmail_textChanged(qleEmail->text());
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

void CertWizard::generateNewCert() {
	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);

	X509 *x509 = X509_new();
	EVP_PKEY *pkey = EVP_PKEY_new();
	RSA *rsa = RSA_generate_key(1024,RSA_F4,NULL,NULL);
	EVP_PKEY_assign_RSA(pkey, rsa);

	X509_set_version(x509, 2);
	ASN1_INTEGER_set(X509_get_serialNumber(x509),1);
	X509_gmtime_adj(X509_get_notBefore(x509),0);
	X509_gmtime_adj(X509_get_notAfter(x509),60*60*24*365*20);
	X509_set_pubkey(x509, pkey);

	X509_NAME *name=X509_get_subject_name(x509);

	QString qsname = qleName->text();
	QString qsemail = qleEmail->text();
	if (qsname.isEmpty())
		qsname = tr("Mumble User");

	X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, reinterpret_cast<unsigned char *>(qsname.toUtf8().data()), -1, -1, 0);
	X509_set_issuer_name(x509, name);
	add_ext(x509, NID_basic_constraints, "critical,CA:FALSE");
	add_ext(x509, NID_ext_key_usage, "clientAuth");
	add_ext(x509, NID_subject_key_identifier, "hash");
	add_ext(x509, NID_netscape_comment, "Generated by Mumble");
	add_ext(x509, NID_subject_alt_name, QString::fromLatin1("email:%1").arg(qsemail).toUtf8().data());

	X509_sign(x509, pkey, EVP_md5());

	QByteArray crt, key;

	crt.resize(i2d_X509(x509, NULL));
	unsigned char *dptr=reinterpret_cast<unsigned char *>(crt.data());
	i2d_X509(x509, &dptr);

	QSslCertificate qscCert = QSslCertificate(crt, QSsl::Der);

	key.resize(i2d_PrivateKey(pkey, NULL));
	dptr=reinterpret_cast<unsigned char *>(key.data());
	i2d_PrivateKey(pkey, &dptr);

	qskNew = QSslKey(key, QSsl::Rsa, QSsl::Der);

	qscNew.clear();
	qscNew << qscCert;
}

QByteArray CertWizard::exportCert() {
	X509 *x509 = NULL;
	EVP_PKEY *pkey = NULL;
	PKCS12 *pkcs = NULL;
	BIO *mem = NULL;
	const unsigned char *p;
	long size;
	char *data = NULL;

	QByteArray key = qskNew.toDer();
	QByteArray crt = qscNew.at(0).toDer();
	QByteArray qba;

	p = reinterpret_cast<const unsigned char *>(key.constData());
	pkey = d2i_AutoPrivateKey(NULL, &p, key.length());

	if (pkey) {
		p = reinterpret_cast<const unsigned char *>(crt.constData());
		x509 = d2i_X509(NULL, &p, crt.length());

		if (x509 && X509_check_private_key(x509, pkey)) {
			X509_keyid_set1(x509, NULL, 0);
			X509_alias_set1(x509, NULL, 0);

			STACK_OF(X509) *certs = sk_new_null();

			QList<QSsslCertificate> qlCerts = qscNew;
			qlCerts.removeFirst();

			foreach(const QSslCertificate &cert, qlCerts) {
				X509 *c = NULL;
				crt = cert.toDer();
				p = reinterpret_cast<const unsigned char *>(crt.constData());

				c = d2i_X509(NULL, &p, crt.length());
				if (c)
					sk_X509_push(certs, c);
			}

			pkcs = PKCS12_create("", "Mumble Identity", pkey, x509, certs, -1, -1, 0, 0, 0);

			if (pkcs) {
				mem = BIO_new(BIO_s_mem());
				i2d_PKCS12_bio(mem, pkcs);
				BIO_flush(mem);

				size = BIO_get_mem_data(mem, &data);

				qba = QByteArray(data, size);
			}
			sk_free(certs);
		}
	}

	if (pkey)
		EVP_PKEY_free(pkey);
	if (x509)
		X509_free(x509);
	if (pkcs)
		PKCS12_free(pkcs);
	if (mem)
		BIO_free(mem);

	return qba;
}
