// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// Ignore old-style casts for the whole file.
// We can't use push/pop. They were implemented in GCC 4.6,
// but we still build with GCC 4.2 for the legacy OS X Universal
// build.
#if defined(__GNUC__)
# pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include "Cert.h"

#include "SelfSignedCertificate.h"
#include "Utils.h"

#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QToolTip>

#include <openssl/evp.h>
#include <openssl/pkcs12.h>
#include <openssl/x509.h>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

#define SSL_STRING(x) QString::fromLatin1(x).toUtf8().data()

CertView::CertView(QWidget *p) : QGroupBox(p) {
	QGridLayout *grid = new QGridLayout(this);
	QLabel *l;

	l = new QLabel(tr("Name"));
	grid->addWidget(l, 0, 0, 1, 1, Qt::AlignLeft);

	qlSubjectName = new QLabel();
	qlSubjectName->setTextFormat(Qt::PlainText);
	qlSubjectName->setWordWrap(true);
	grid->addWidget(qlSubjectName, 0, 1, 1, 1);

	l = new QLabel(tr("Email"));
	grid->addWidget(l, 1, 0, 1, 1, Qt::AlignLeft);

	qlSubjectEmail = new QLabel();
	qlSubjectEmail->setTextFormat(Qt::PlainText);
	qlSubjectEmail->setWordWrap(true);
	grid->addWidget(qlSubjectEmail, 1, 1, 1, 1);

	l = new QLabel(tr("Issuer"));
	grid->addWidget(l, 2, 0, 1, 1, Qt::AlignLeft);

	qlIssuerName = new QLabel();
	qlIssuerName->setTextFormat(Qt::PlainText);
	qlIssuerName->setWordWrap(true);
	grid->addWidget(qlIssuerName, 2, 1, 1, 1);

	l = new QLabel(tr("Expiry Date"));
	grid->addWidget(l, 3, 0, 1, 1, Qt::AlignLeft);

	qlExpiry = new QLabel();
	qlExpiry->setWordWrap(true);
	grid->addWidget(qlExpiry, 3, 1, 1, 1);

	grid->setColumnStretch(1, 1);
}

void CertView::setCert(const QList<QSslCertificate> &cert) {
	qlCert = cert;

	if (qlCert.isEmpty()) {
		qlSubjectName->setText(QString());
		qlSubjectEmail->setText(QString());
		qlIssuerName->setText(QString());
		qlExpiry->setText(QString());
	} else {
		QSslCertificate qscCert = qlCert.at(0);

#if QT_VERSION >= 0x050000
		const QStringList &names = qscCert.subjectInfo(QSslCertificate::CommonName);
		QString name;
		if (names.count() > 0) {
			name = names.at(0);
		}

		QStringList emails = qscCert.subjectAlternativeNames().values(QSsl::EmailEntry);
#else
		const QString &name = qscCert.subjectInfo(QSslCertificate::CommonName);
		QStringList emails(qscCert.alternateSubjectNames().values(QSsl::EmailEntry));
#endif

		QString tmpName = name;
		tmpName = tmpName.replace(QLatin1String("\\x"), QLatin1String("%"));
		tmpName = QUrl::fromPercentEncoding(tmpName.toLatin1());

		qlSubjectName->setText(tmpName);

		if (emails.count() > 0)
			qlSubjectEmail->setText(emails.join(QLatin1String("\n")));
		else
			qlSubjectEmail->setText(tr("(none)"));

		if (qscCert.expiryDate() <= QDateTime::currentDateTime())
			qlExpiry->setText(QString::fromLatin1("<font color=\"red\"><b>%1</b></font>").arg(Qt::escape(qscCert.expiryDate().toString(Qt::SystemLocaleDate))));
		else
			qlExpiry->setText(qscCert.expiryDate().toString(Qt::SystemLocaleDate));

		if (qlCert.count() > 1)
			qscCert = qlCert.last();

#if QT_VERSION >= 0x050000
		const QStringList &issuerNames = qscCert.issuerInfo(QSslCertificate::CommonName);
		QString issuerName;
		if (issuerNames.count() > 0) {
			issuerName = issuerNames.at(0);
		}
#else
		const QString &issuerName = qscCert.issuerInfo(QSslCertificate::CommonName);
#endif
		qlIssuerName->setText((issuerName == name) ? tr("Self-signed") : issuerName);
	}
}

CertWizard::CertWizard(QWidget *p) : QWizard(p) {
	setupUi(this);

	setOption(QWizard::NoCancelButton, false);

	qwpExport->setCommitPage(true);
	qwpExport->setComplete(false);
	qlPasswordNotice->setVisible(false);
}

int CertWizard::nextId() const {
	switch (currentId()) {
		case 0: {	// Welcome
				if (qrbQuick->isChecked())
					return 5;
				else if (qrbCreate->isChecked())
					return 1;
				else if (qrbImport->isChecked())
					return 2;
				else if (qrbExport->isChecked())
					return 3;
				return -1;
			}
		case 2: // Import
			if (validateCert(kpCurrent))
				return 4;
			else
				return 5;
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
			if (validateCert(kpCurrent))
				return 4;
			else
				return 3;
	}
	return -1;
}

void CertWizard::initializePage(int id) {
	if (id == 0) {
		kpCurrent = kpNew = g.s.kpCertificate;

		if (validateCert(kpCurrent)) {
			qrbQuick->setEnabled(false);
			qrbExport->setEnabled(true);
			cvWelcome->setCert(kpCurrent.first);
			cvWelcome->setVisible(true);
		} else {
			qrbQuick->setEnabled(true);
			qrbExport->setEnabled(false);
			cvWelcome->setVisible(false);
			qrbQuick->setChecked(true);
		}
	}
	if (id == 3) {
		cvExport->setCert(kpNew.first);
	}
	if (id == 4) {
		cvNew->setCert(kpNew.first);
		cvCurrent->setCert(kpCurrent.first);
	}
	if (id == 2) {
		on_qleImportFile_textChanged(qleImportFile->text());
	}

	QWizard::initializePage(id);
}

bool CertWizard::validateCurrentPage() {
	if (currentPage() == qwpNew) {
		QRegExp ereg(QLatin1String("(^$)|((.+)@(.+))"), Qt::CaseInsensitive, QRegExp::RegExp2);
		if (!ereg.exactMatch(qleEmail->text())) {
			qlError->setText(tr("Unable to validate email.<br />Enter a valid (or blank) email to continue."));
			qwpNew->setComplete(false);
			return false;
		} else {
			kpNew = generateNewCert(qleName->text(), qleEmail->text());
			
			if (! validateCert(kpNew)) {
				qlError->setText(tr("There was an error generating your certificate.<br />Please try again."));
				return false;
			}
		}
	}
	if (currentPage() == qwpExport) {
		QByteArray qba = exportCert(kpNew);
		if (qba.isEmpty()) {
			QToolTip::showText(qleExportFile->mapToGlobal(QPoint(0,0)), tr("Your certificate and key could not be exported to PKCS#12 format. There might be an error in your certificate."), qleExportFile);
			return false;
		}
		QFile f(qleExportFile->text());
		if (! f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered)) {
			QToolTip::showText(qleExportFile->mapToGlobal(QPoint(0,0)), tr("The file could not be opened for writing. Please use another file."), qleExportFile);
			return false;
		}
		if (! f.setPermissions(QFile::ReadOwner | QFile::WriteOwner)) {
			QToolTip::showText(qleExportFile->mapToGlobal(QPoint(0,0)), tr("The file's permissions could not be set. No certificate and key has been written. Please use another file."), qleExportFile);
			return false;
		}
		qint64 written = f.write(qba);
		f.close();
		if (written != qba.length()) {
			QToolTip::showText(qleExportFile->mapToGlobal(QPoint(0,0)), tr("The file could not be written successfully. Please use another file."), qleExportFile);
			return false;
		}
	}
	if (currentPage() == qwpImport) {
		QFile f(qleImportFile->text());
		if (! f.open(QIODevice::ReadOnly | QIODevice::Unbuffered)) {
			QToolTip::showText(qleImportFile->mapToGlobal(QPoint(0,0)), tr("The file could not be opened for reading. Please use another file."), qleImportFile);
			return false;
		}
		QByteArray qba = f.readAll();
		f.close();
		if (qba.isEmpty()) {
			QToolTip::showText(qleImportFile->mapToGlobal(QPoint(0,0)), tr("The file is empty or could not be read. Please use another file."), qleImportFile);
			return false;
		}
		QPair<QList<QSslCertificate>, QSslKey> imp = importCert(qba, qlePassword->text());
		if (! validateCert(imp)) {
			QToolTip::showText(qleImportFile->mapToGlobal(QPoint(0,0)), tr("The file did not contain a valid certificate and key. Please use another file."), qleImportFile);
			return false;
		}
		kpNew = imp;
	}
	if (currentPage() == qwpFinish) {
		g.s.kpCertificate = kpNew;
	}
	return QWizard::validateCurrentPage();
}

void CertWizard::on_qleEmail_textChanged(const QString &) {
	qwpNew->setComplete(true);
}

void CertWizard::on_qpbExportFile_clicked() {
	QString fname = QFileDialog::getSaveFileName(this, tr("Select file to export certificate to"), qleExportFile->text(), QLatin1String("PKCS12 (*.p12 *.pfx *.pkcs12);;All (*)"));
	if (! fname.isNull()) {
		QFileInfo fi(fname);
		if (fi.suffix().isEmpty())
			fname += QLatin1String(".p12");
		qleExportFile->setText(QDir::toNativeSeparators(fname));
	}
}

void CertWizard::on_qleExportFile_textChanged(const QString &text) {
	if (text.isEmpty()) {
		qwpExport->setComplete(false);
		return;
	}

	QString fname = QDir::fromNativeSeparators(text);

	QFile f(fname);

	QFileInfo fi(f);
	if (fi.exists()) {
		if (fi.isWritable()) {
			qwpExport->setComplete(f.open(QIODevice::WriteOnly | QIODevice::Append));
			return;
		}
	} else {
		if (f.open(QIODevice::WriteOnly)) {
			if (f.remove()) {
				qwpExport->setComplete(true);
				return;
			}
		}
	}
	qwpExport->setComplete(false);
}

void CertWizard::on_qpbImportFile_clicked() {
	QString fname = QFileDialog::getOpenFileName(this, tr("Select file to import certificate from"), qleImportFile->text(), QLatin1String("PKCS12 (*.p12 *.pfx *.pkcs12);;All (*)"));
	if (! fname.isNull()) {
		qleImportFile->setText(QDir::toNativeSeparators(fname));
	}
}

void CertWizard::on_qleImportFile_textChanged(const QString &text) {
	if (text.isEmpty()) {
		qlePassword->clear();
		qlePassword->setEnabled(false);
		qlPassword->setEnabled(false);
		qlPasswordNotice->clear();
		qlPasswordNotice->setVisible(false);
		qwpImport->setComplete(false);
		return;
	}

	QString fname = QDir::fromNativeSeparators(text);

	QFile f(fname);
	if (f.open(QIODevice::ReadOnly)) {
		QByteArray qba = f.readAll();
		QPair<QList<QSslCertificate>, QSslKey> imp = importCert(qba, qlePassword->text());
		if (validateCert(imp)) {
			qlePassword->setEnabled(false);
			qlPassword->setEnabled(false);
			qlPasswordNotice->clear();
			qlPasswordNotice->setVisible(false);
			cvImport->setCert(imp.first);
			qwpImport->setComplete(true);
			return;
		} else {
			qlePassword->setEnabled(true);
			qlPassword->setEnabled(true);
			qlPasswordNotice->setText(tr("Unable to import. Missing password or incompatible file type."));
			qlPasswordNotice->setVisible(true);
		}
	} else {
		qlePassword->clear();
		qlePassword->setEnabled(false);
		qlPassword->setEnabled(false);
		qlPasswordNotice->clear();
		qlPasswordNotice->setVisible(false);
	}
	cvImport->setCert(QList<QSslCertificate>());
	qwpImport->setComplete(false);
}

void CertWizard::on_qlePassword_textChanged(const QString &) {
	on_qleImportFile_textChanged(qleImportFile->text());
}

void CertWizard::on_qlIntroText_linkActivated(const QString &url) {
	QDesktopServices::openUrl(QUrl(url));
}

bool CertWizard::validateCert(const Settings::KeyPair &kp) {
	bool valid = ! kp.second.isNull() && ! kp.first.isEmpty();
	foreach(const QSslCertificate &cert, kp.first)
		valid = valid && ! cert.isNull();
	return valid;
}

Settings::KeyPair CertWizard::generateNewCert(QString qsname, const QString &qsemail) {
	QSslCertificate qscCert;
	QSslKey qskKey;

	// Ignore return value.
	// The method sets qscCert and qskKey to null values if it fails.
	SelfSignedCertificate::generateMumbleCertificate(qsname, qsemail, qscCert, qskKey);

	QList<QSslCertificate> qlCert;
	qlCert << qscCert;

	return Settings::KeyPair(qlCert, qskKey);
}

Settings::KeyPair CertWizard::importCert(QByteArray data, const QString &pw) {
	X509 *x509 = NULL;
	EVP_PKEY *pkey = NULL;
	PKCS12 *pkcs = NULL;
	BIO *mem = NULL;
	STACK_OF(X509) *certs = NULL;
	Settings::KeyPair kp;
	int ret = 0;

	mem = BIO_new_mem_buf(data.data(), data.size());
	Q_UNUSED(BIO_set_close(mem, BIO_NOCLOSE));
	pkcs = d2i_PKCS12_bio(mem, NULL);
	if (pkcs) {
		ret = PKCS12_parse(pkcs, NULL, &pkey, &x509, &certs);
		if (pkcs && !pkey && !x509 && ! pw.isEmpty()) {
			if (certs) {
				if (ret)
					sk_X509_free(certs);
				certs = NULL;
			}
			ret = PKCS12_parse(pkcs, pw.toUtf8().constData(), &pkey, &x509, &certs);
		}
		if (pkey && x509 && X509_check_private_key(x509, pkey)) {
			unsigned char *dptr;
			QByteArray key, crt;

			key.resize(i2d_PrivateKey(pkey, NULL));
			dptr=reinterpret_cast<unsigned char *>(key.data());
			i2d_PrivateKey(pkey, &dptr);

			crt.resize(i2d_X509(x509, NULL));
			dptr=reinterpret_cast<unsigned char *>(crt.data());
			i2d_X509(x509, &dptr);

			QSslCertificate qscCert = QSslCertificate(crt, QSsl::Der);
			QSslKey qskKey = QSslKey(key, QSsl::Rsa, QSsl::Der);

			QList<QSslCertificate> qlCerts;
			qlCerts << qscCert;

			if (certs) {
				for (int i=0;i<sk_X509_num(certs);++i) {
					X509 *c = sk_X509_value(certs, i);

					crt.resize(i2d_X509(c, NULL));
					dptr=reinterpret_cast<unsigned char *>(crt.data());
					i2d_X509(c, &dptr);

					QSslCertificate cert = QSslCertificate(crt, QSsl::Der);
					qlCerts << cert;
				}
			}
			bool valid = ! qskKey.isNull();
			foreach(const QSslCertificate &cert, qlCerts)
				valid = valid && ! cert.isNull();
			if (valid)
				kp = Settings::KeyPair(qlCerts, qskKey);
		}
	}

	if (ret) {
		if (pkey)
			EVP_PKEY_free(pkey);
		if (x509)
			X509_free(x509);
		if (certs)
			sk_X509_free(certs);
	}
	if (pkcs)
		PKCS12_free(pkcs);
	if (mem)
		BIO_free(mem);

	return kp;
}

QByteArray CertWizard::exportCert(const Settings::KeyPair &kp) {
	X509 *x509 = NULL;
	EVP_PKEY *pkey = NULL;
	PKCS12 *pkcs = NULL;
	BIO *mem = NULL;
	STACK_OF(X509) *certs = sk_X509_new_null();
	const unsigned char *p;
	char *data = NULL;

	if (kp.first.isEmpty())
		return QByteArray();

	QByteArray crt = kp.first.at(0).toDer();
	QByteArray key = kp.second.toDer();
	QByteArray qba;

	p = reinterpret_cast<const unsigned char *>(key.constData());
	pkey = d2i_AutoPrivateKey(NULL, &p, key.length());

	if (pkey) {
		p = reinterpret_cast<const unsigned char *>(crt.constData());
		x509 = d2i_X509(NULL, &p, crt.length());

		if (x509 && X509_check_private_key(x509, pkey)) {
			X509_keyid_set1(x509, NULL, 0);
			X509_alias_set1(x509, NULL, 0);


			QList<QSslCertificate> qlCerts = kp.first;
			qlCerts.removeFirst();

			foreach(const QSslCertificate &cert, qlCerts) {
				X509 *c = NULL;
				crt = cert.toDer();
				p = reinterpret_cast<const unsigned char *>(crt.constData());

				c = d2i_X509(NULL, &p, crt.length());
				if (c)
					sk_X509_push(certs, c);
			}

			pkcs = PKCS12_create(SSL_STRING(""), SSL_STRING("Mumble Identity"), pkey, x509, certs, -1, -1, 0, 0, 0);
			if (pkcs) {
				long size;
				mem = BIO_new(BIO_s_mem());
				i2d_PKCS12_bio(mem, pkcs);
				Q_UNUSED(BIO_flush(mem));
				size = BIO_get_mem_data(mem, &data);
				qba = QByteArray(data, static_cast<int>(size));
			}
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
	if (certs)
		sk_X509_free(certs);

	return qba;
}
