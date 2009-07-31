/* Copyright (C) 2009, Mikkel Krautz <mikkel@krautz.dk>

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

#include "CrashReporter.h"
#include "Global.h"

CrashReporter::CrashReporter(QWidget *p) : QMessageBox(p) {
	setWindowTitle(tr("Mumble Crash Report"));
	setIcon(QMessageBox::Question);
	setText(tr("<b>Mumble has recovered from a crash.</b>"));
	setInformativeText(tr("Do you wish to send the crash report to the Mumble developers?<br /><br />" \
	                      "The report will contain a partial copy of Mumble's memory at the time it crashed, as well as information on all DLLs loaded into Mumble."));
	setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	qelLoop = new QEventLoop(this);
	qpdProgress = NULL;
	qnrReply = NULL;
}

CrashReporter::~CrashReporter() {
	if (qpdProgress)
		delete qpdProgress;
	if (qnrReply)
		delete qnrReply;
}

void CrashReporter::reportCrash(const QByteArray &qbaCrashReport) {
	QUrl url(QLatin1String("https://mumble.hive.no/crashreport.php"));

	url.addQueryItem(QLatin1String("ver"), QLatin1String(QUrl::toPercentEncoding(QLatin1String(MUMBLE_RELEASE))));
#if defined(Q_OS_WIN)
	url.addQueryItem(QLatin1String("os"), QLatin1String("Win32"));
#elif defined(Q_OS_MAC)
	url.addQueryItem(QLatin1String("os"), QLatin1String("MacOSX"));
#else
	url.addQueryItem(QLatin1String("os"), QLatin1String("Unix"));
#endif

	QNetworkRequest req(url);
	qnrReply = g.nam->post(req, qbaCrashReport);
	connect(qnrReply, SIGNAL(finished()), this, SLOT(uploadFinished()));
	connect(qnrReply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadProgress(qint64, qint64)));
}

void CrashReporter::uploadFinished() {
	qpdProgress->reset();
	if (qnrReply->error() == QNetworkReply::NoError) {
		if (qnrReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
			QMessageBox::information(NULL, tr("Crash upload successfull"), tr("Thank you for helping make Mumble better!"));
		else
			QMessageBox::critical(NULL, tr("Crash upload failed"), tr("We're really sorry, but it appears the crash upload has failed with error %1 %2. Please inform a developer.").arg(qnrReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()).arg(qnrReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()));
	} else {
		QMessageBox::critical(NULL, tr("Crash upload failed"), tr("This really isn't funny, but apparantly there's a bug in the crash reporting code, and we've failed to upload the report. You may inform a developer about error %1").arg(qnrReply->error()));
	}
	qelLoop->exit(0);
}

void CrashReporter::uploadProgress(qint64 sent, qint64 total) {
	qpdProgress->setMaximum(total);
	qpdProgress->setValue(sent);
}

void CrashReporter::run() {


	QFile qfMinidump(g.qdBasePath.filePath(QLatin1String("mumble.dmp")));
	if (! qfMinidump.exists())
		return;

	qfMinidump.open(QIODevice::ReadOnly);

#ifdef Q_OS_WIN
	if (qfMinidump.peek(4) != "MDMP")
		return;
#endif

	if (exec() == QMessageBox::Ok) {
		qpdProgress = new QProgressDialog(tr("Uploading crash report"), tr("Abort upload"), 0, 100, this);
		qpdProgress->setValue(0);
		connect(qpdProgress, SIGNAL(canceled()), qelLoop, SLOT(quit()));
		reportCrash(qfMinidump.readAll());

		qelLoop->exec(QEventLoop::DialogExec);
	}

	if (! qfMinidump.remove())
		qWarning("CrashReporeter: Unable to remove minidump.");
}
