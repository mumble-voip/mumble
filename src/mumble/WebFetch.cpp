// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "WebFetch.h"

#include "NetworkConfig.h"
#include "Global.h"

#include <QtCore/QRegularExpression>
#include <QtNetwork/QNetworkReply>

WebFetch::WebFetch(QString service, QUrl url, QObject *obj, const char *slot)
	: QObject(), qoObject(obj), cpSlot(slot), m_service(service) {
	url.setScheme(QLatin1String("https"));

	if (!Global::get().s.qsServicePrefix.isEmpty()) {
		url.setHost(prefixedServiceHost());
	} else {
		url.setHost(serviceHost());
	}

	qnr = Network::get(url);
	connect(qnr, SIGNAL(finished()), this, SLOT(finished()));
	connect(this, SIGNAL(fetched(QByteArray, QUrl, QMap< QString, QString >)), obj, slot);
}

QString WebFetch::prefixedServiceHost() const {
	if (Global::get().s.qsServicePrefix.isEmpty()) {
		return serviceHost();
	}
	return QString::fromLatin1("%1-%2.mumble.info").arg(Global::get().s.qsServicePrefix, m_service);
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
	Q_ASSERT(qobject_cast< QNetworkReply * >(sender()) == qnr);
	qnr->disconnect();
	qnr->deleteLater();

	QUrl url = qnr->request().url();

	if (qnr->error() == QNetworkReply::NoError) {
		QByteArray a = qnr->readAll();

		// empty response is not an error
		if (a.isNull())
			a.append("");

		QMap< QString, QString > headers;

		foreach (const QByteArray &headerName, qnr->rawHeaderList()) {
			QString name  = fromUtf8(headerName);
			QString value = fromUtf8(qnr->rawHeader(headerName));
			if (!name.isEmpty() && !value.isEmpty()) {
				headers.insert(name, value);
				if (name == QLatin1String("Use-Service-Prefix")) {
					const QRegularExpression servicePrefixRegExp(
						QRegularExpression::anchoredPattern(QLatin1String("^[a-zA-Z]+$")));
					if (servicePrefixRegExp.match(value).hasMatch()) {
						Global::get().s.qsServicePrefix = value.toLower();
					}
				}
			}
		}

		emit fetched(a, url, headers);
		deleteLater();
	} else if (url.host() == prefixedServiceHost() && url.host() != serviceHost()) {
		// We have tried to fetch from a local service domain (e.Global::get(). de-update.mumble.info)
		// which has failed, so naturally we want to try the non-local one (update.mumble.info)
		// as well as maybe that one will work.
		// This of course only makes sense, if prefixedServiceHost() and serviceHost() are in fact
		// different hosts.
		url.setHost(serviceHost());

		qnr = Network::get(url);
		connect(qnr, SIGNAL(finished()), this, SLOT(finished()));
	} else {
		emit fetched(QByteArray(), url, QMap< QString, QString >());
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
