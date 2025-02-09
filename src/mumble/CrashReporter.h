// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CRASHREPORTER_H_
#	define MUMBLE_MUMBLE_CRASHREPORTER_H_

#	include <QtCore/QEventLoop>
#	include <QtCore/QObject>
#	include <QtNetwork/QNetworkReply>
#	include <QtWidgets/QDialog>
#	include <QtWidgets/QLineEdit>
#	include <QtWidgets/QProgressDialog>
#	include <QtWidgets/QTextEdit>

class CrashReporter : QDialog {
	Q_OBJECT
	Q_DISABLE_COPY(CrashReporter)

public:
	CrashReporter(QWidget *p = 0);
	~CrashReporter() Q_DECL_OVERRIDE;
	void run();

protected:
	QEventLoop *qelLoop;
	QProgressDialog *qpdProgress;
	QNetworkReply *qnrReply;
	QLineEdit *qleEmail;
	QTextEdit *qteDescription;
public slots:
	void uploadFinished();
	void uploadProgress(qint64, qint64);
};

#else
class CrashReporter;
#endif
