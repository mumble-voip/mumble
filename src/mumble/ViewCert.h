// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VIEWCERT_H_
#define MUMBLE_MUMBLE_VIEWCERT_H_

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QtGlobal>
#include <QtNetwork/QSslCertificate>
#include <QtWidgets/QDialog>

class QListWidget;

class ViewCert : public QDialog {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ViewCert)
protected:
	QList< QSslCertificate > qlCerts;
	QListWidget *qlwChain, *qlwCert;
protected slots:
	void on_Chain_currentRowChanged(int);

public:
	ViewCert(QList< QSslCertificate > c, QWidget *p);
	static QString prettifyDigest(QString);
};

#endif
