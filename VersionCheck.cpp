/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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

#include "VersionCheck.h"
#include "Global.h"

VersionCheck::VersionCheck(QObject *p) : QObject(p) {
	qhAgent = new QHttp(this);
	qhAgent->setObjectName("Agent");
	quUrl.setScheme("http");
	quUrl.setHost("mumble.sourceforge.net");
	quUrl.setPath("/ver.php");
	quUrl.addQueryItem("ver", QUrl::toPercentEncoding(MUMBLE_RELEASE));
	quUrl.addQueryItem("date", QUrl::toPercentEncoding(__DATE__));
	quUrl.addQueryItem("time", QUrl::toPercentEncoding(__TIME__));

    QMetaObject::connectSlotsByName(this);

	QFile f(qApp->applicationFilePath());
	if (! f.open(QIODevice::ReadOnly)) {
		qWarning("VersionCheck: Failed to open binary");
		return;
	}
	QByteArray a = f.readAll();

	if (a.size() < 1) {
		qWarning("VersionCehck: suspiciously small binary");
		return;
	}

	quUrl.addQueryItem("crc", QString::number(qChecksum(a.data(), a.size()),16));
	QHttpRequestHeader req("GET", quUrl.toString(QUrl::RemoveScheme|QUrl::RemoveAuthority));
	req.setValue("Host", quUrl.host());

	qhAgent->setHost(quUrl.host());
	iReqId=qhAgent->request(req);
}

void VersionCheck::on_Agent_requestFinished(int id, bool error) {
	if (id != iReqId)
		return;

	QHttpResponseHeader head = qhAgent->lastResponse();
	if ((head.statusCode() == 200) && !error) {
		QByteArray a=qhAgent->readAll();
		if (a.size() > 0)
			QMessageBox::information(static_cast<QWidget *>(parent()), tr("Mumble"), a, QMessageBox::Ok| QMessageBox::Default| QMessageBox::Escape, QMessageBox::NoButton);
	} else {
			QMessageBox::information(static_cast<QWidget *>(parent()), tr("Mumble"), tr("Failed to retrieve version information from server."), QMessageBox::Ok| QMessageBox::Default| QMessageBox::Escape, QMessageBox::NoButton);
	}
	deleteLater();
}
