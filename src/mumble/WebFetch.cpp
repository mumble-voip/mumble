/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#include "mumble_pch.hpp"

#include "WebFetch.h"

#include "Global.h"
#include "NetworkConfig.h"

WebFetch::WebFetch(QUrl url, QObject *obj, const char *slot) : QObject(), qoObject(obj), cpSlot(slot) {
	url.setScheme(QLatin1String("http"));

	// Fix in case the regional host is broken
	url.setHost(g.s.qsRegionalHost);
	if (url.host() != g.s.qsRegionalHost)
		url.setHost(QLatin1String("mumble.info"));

	qnr = Network::get(url);
	connect(qnr, SIGNAL(finished()), this, SLOT(finished()));
	connect(this, SIGNAL(fetched(QByteArray,QUrl,QMap<QString,QString>)), obj, slot);
}

static QString fromUtf8(const QByteArray &qba) {
	if (qba.isEmpty())
		return QString();
	return QString::fromUtf8(qba.constData(), qba.length());
}

void WebFetch::finished() {
	// Note that if this functions succeeds, it should deleteLater() itself, as this is a temporary object.
	Q_ASSERT(qobject_cast<QNetworkReply *>(sender()) == qnr);
	qnr->disconnect();
	qnr->deleteLater();

	QUrl url = qnr->request().url();

	if (qnr->error() == QNetworkReply::NoError) {
		QByteArray a = qnr->readAll();

		// empty response is not an error
		if (a.isNull())
			a.append("");

		QMap<QString, QString> headers;

		foreach(const QByteArray &headerName, qnr->rawHeaderList()) {
			QString name = fromUtf8(headerName);
			QString value = fromUtf8(qnr->rawHeader(headerName));
			if (! name.isEmpty() && ! value.isEmpty()) {
				headers.insert(name, value);
				if (name == QLatin1String("Geo-Country-Code"))
					g.s.qsRegionalHost = value.toLower() + QLatin1String(".mumble.info");
			}
		}

		emit fetched(a, url, headers);
		deleteLater();
	} else if (url.host() == g.s.qsRegionalHost) {
		url.setHost(QLatin1String("mumble.info"));

		qnr = Network::get(url);
		connect(qnr, SIGNAL(finished()), this, SLOT(finished()));
	} else if (url.host() == QLatin1String("mumble.info")) {
		url.setHost(QLatin1String("panic.mumble.info"));

		qnr = Network::get(url);
		connect(qnr, SIGNAL(finished()), this, SLOT(finished()));
	} else {
		emit fetched(QByteArray(), url, QMap<QString,QString>());
		deleteLater();
	}
}

/*!
 * \brief Fetch URL from mumble servers.
 *
 * If fetching fails, the slot is invoked with a null QByteArray.
 * \param url URL to fetch. Hostname and scheme must be blank.
 * \param obj Object to invoke slot on.
 * \param slot Slot to be triggered, invoked with the signature of \link fetched.
 */
void WebFetch::fetch(const QUrl &url, QObject *obj, const char *slot) {
	Q_ASSERT(url.scheme().isEmpty());
	Q_ASSERT(url.host().isEmpty());
	Q_ASSERT(obj);
	Q_ASSERT(slot);

	new WebFetch(url, obj, slot);
}
