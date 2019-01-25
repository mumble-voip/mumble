// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CRASHREPORTER_H_
#define MUMBLE_MUMBLE_CRASHREPORTER_H_

#include <QtCore/QObject>
#include <QtCore/QEventLoop>
#include <QtNetwork/QNetworkReply>

#if QT_VERSION >= 0x050000
# include <QtWidgets/QDialog>
# include <QtWidgets/QProgressDialog>
# include <QtWidgets/QLineEdit>
# include <QtWidgets/QTextEdit>
#else
# include <QtGui/QDialog>
# include <QtGui/QProgressDialog>
# include <QtGui/QLineEdit>
# include <QtGui/QTextEdit>
#endif

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
