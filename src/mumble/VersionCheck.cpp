/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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
	qhAgent->setObjectName(QString::fromAscii("Agent"));
	quUrl.setScheme(QString::fromAscii("http"));
	quUrl.setHost(QString::fromAscii("mumble.sourceforge.net"));
	quUrl.setPath(QString::fromAscii("/ver.php"));
	quUrl.addQueryItem(QString::fromAscii("ver"), QString::fromAscii(QUrl::toPercentEncoding(QString::fromAscii(MUMBLE_RELEASE))));
	quUrl.addQueryItem(QString::fromAscii("date"), QString::fromAscii(QUrl::toPercentEncoding(QString::fromAscii(__DATE__))));
	quUrl.addQueryItem(QString::fromAscii("time"), QString::fromAscii(QUrl::toPercentEncoding(QString::fromAscii(__TIME__))));

    QMetaObject::connectSlotsByName(this);

	QFile f(qApp->applicationFilePath());
	if (! f.open(QIODevice::ReadOnly)) {
		qWarning("VersionCheck: Failed to open binary");
	} else {
		QByteArray a = f.readAll();
		if (a.size() < 1) {
			qWarning("VersionCheck: suspiciously small binary");
		} else {
			quUrl.addQueryItem(QString::fromAscii("crc"), QString::number(qChecksum(a.data(), a.size()),16));
		}
	}

	QHttpRequestHeader req(QString::fromAscii("GET"), quUrl.toString(QUrl::RemoveScheme|QUrl::RemoveAuthority));
	req.setValue(QString::fromAscii("Host"), quUrl.host());

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
			QMessageBox::information(static_cast<QWidget *>(parent()), tr("Mumble"), QString::fromAscii(a), QMessageBox::Ok| QMessageBox::Default| QMessageBox::Escape, QMessageBox::NoButton);
	} else {
			QMessageBox::information(static_cast<QWidget *>(parent()), tr("Mumble"), tr("Mumble failed to retrieve version information from the SourceForge server."), QMessageBox::Ok| QMessageBox::Default| QMessageBox::Escape, QMessageBox::NoButton);
	}
	deleteLater();
}
