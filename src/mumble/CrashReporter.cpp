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
}

void CrashReporter::reportCrash(const QByteArray &qbaCrashReport) {
	QUrl url(QLatin1String("http://mumble.hive.no/crashreport.php"));

	url.addQueryItem(QLatin1String("ver"), QLatin1String(QUrl::toPercentEncoding(QLatin1String(MUMBLE_RELEASE))));
#if defined(Q_OS_WIN)
	url.addQueryItem(QLatin1String("os"), QLatin1String("Win32"));
#elif defined(Q_OS_MAC)
	url.addQueryItem(QLatin1String("os"), QLatin1String("MacOSX"));
#else
	url.addQueryItem(QLatin1String("os"), QLatin1String("Unix"));
#endif

	QNetworkRequest req(url);
	QNetworkReply *rep = g.nam->post(req, qbaCrashReport);
	connect(rep, SIGNAL(finished()), this, SLOT(uploadFinished()));
}

void CrashReporter::uploadFinished() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());
	if (rep->error() == QNetworkReply::NoError)
		qWarning("CrashReporter: File uploaded successfully.");
	else
		qWarning("CrashReporter: File upload failed with error: %i", rep->error());
}

void CrashReporter::run() {
#ifdef Q_OS_WIN
	QString qsAppData;
	size_t reqSize, bSize;
	_wgetenv_s(&reqSize, NULL, 0, L"APPDATA");
	if (reqSize > 0) {
		STACKVAR(wchar_t, buff, reqSize+1);
		_wgetenv_s(&reqSize, buff, reqSize, L"APPDATA");
		qsAppData=QDir::fromNativeSeparators(QString::fromWCharArray(buff));
	}

	QFile qfMinidump(QString::fromLatin1("%1/Mumble/mumble.dmp").arg(qsAppData));
	if (! qfMinidump.exists())
		return;

	if (exec() == QMessageBox::Ok) {
		qfMinidump.open(QIODevice::ReadOnly);
		if (qfMinidump.peek(4) == "MDMP")
			reportCrash(qfMinidump.readAll());
	}

	if (! qfMinidump.remove())
		qWarning("CrashReporeter: Unable to remove minidump.");
#endif
}
