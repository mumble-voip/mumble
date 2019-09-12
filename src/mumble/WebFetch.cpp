// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "WebFetch.h"

#include "NetworkConfig.h"

#include <QtNetwork/QNetworkReply>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

WebFetch::WebFetch(QString service, QUrl url, QObject *obj, const char *slot)
	: QObject()
	, qoObject(obj)
	, cpSlot(slot)
	, m_service(service) {

	url.setScheme(QLatin1String("https"));

	if (!g.s.qsServicePrefix.isEmpty()) {
		url.setHost(prefixedServiceHost());
	} else {
		url.setHost(serviceHost());
	}

	qnr = Network::get(url);
	connect(qnr, SIGNAL(finished()), this, SLOT(finished()));
	connect(this, SIGNAL(fetched(QByteArray,QUrl,QMap<QString,QString>)), obj, slot);
}

QString WebFetch::prefixedServiceHost() const {
	if (g.s.qsServicePrefix.isEmpty()) {
		return serviceHost();
	}
	return QString::fromLatin1("%1-%2.mumble.info").arg(g.s.qsServicePrefix, m_service);
}

QString WebFetch::serviceHost() const {
	return QString::fromLatin1("%1.mumble.info").arg(m_service);
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
				if (name == QLatin1String("Use-Service-Prefix")) {
					QRegExp servicePrefixRegExp(QLatin1String("^[a-zA-Z]+$"));
					if (servicePrefixRegExp.exactMatch(value)) {
						g.s.qsServicePrefix = value.toLower();
					}
				}
			}
		}

		emit fetched(a, url, headers);
		deleteLater();
	} else if (url.host() == prefixedServiceHost()) {
		url.setHost(serviceHost());

		qnr = Network::get(url);
		connect(qnr, SIGNAL(finished()), this, SLOT(finished()));
	} else {
		emit fetched(QByteArray(), url, QMap<QString,QString>());
		deleteLater();
	}
}

/**
 * @brief Fetch URL from mumble servers.
 *
 * If fetching fails, the slot is invoked with a null QByteArray.
 * @param url URL to fetch. Hostname and scheme must be blank.
 * @param obj Object to invoke slot on.
 * @param slot Slot to be triggered, invoked with the signature of \link fetched.
 */
void WebFetch::fetch(const QString &service, const QUrl &url, QObject *obj, const char *slot) {
	Q_ASSERT(!service.isEmpty());
	Q_ASSERT(url.scheme().isEmpty());
	Q_ASSERT(url.host().isEmpty());
	Q_ASSERT(obj);
	Q_ASSERT(slot);

	new WebFetch(service, url, obj, slot);
}
