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

#include <QtXml>

#include "Usage.h"
#include "Version.h"
#include "Global.h"
#include "OSInfo.h"
#include "LCD.h"

Usage::Usage(QObject *p) : QObject(p) {
	// Wait 10 minutes (so we know they're actually using this), then...
	QTimer::singleShot(60 * 10 * 1000, this, SLOT(registerUsage()));
}

void Usage::registerUsage() {
	if (! g.s.bUsage || g.s.bFirstTime)
		return;

	QDomDocument doc;
	QDomElement root=doc.createElement(QLatin1String("usage"));
	doc.appendChild(root);

	QDomElement tag;
	QDomText t;

	OSInfo::fillXml(doc, root);

	tag=doc.createElement(QLatin1String("in"));
	root.appendChild(tag);
	t=doc.createTextNode(g.s.qsAudioInput);
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("out"));
	root.appendChild(tag);
	t=doc.createTextNode(g.s.qsAudioOutput);
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("lcd"));
	root.appendChild(tag);
	t=doc.createTextNode(QString::number(g.lcd->hasDevices() ? 1 : 0));
	tag.appendChild(t);

	QBuffer *qb = new QBuffer();
	qb->setData(doc.toString().toUtf8());
	qb->open(QIODevice::ReadOnly);

	QNetworkRequest req(QUrl(QLatin1String("http://mumble.info/usage.cgi")));
	req.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("text/xml"));

	QNetworkReply *rep = g.nam->post(req, qb);
	qb->setParent(rep);

	connect(rep, SIGNAL(finished()), rep, SLOT(deleteLater()));
}
