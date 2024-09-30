// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WEBFETCH_H_
#define MUMBLE_MUMBLE_WEBFETCH_H_

#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QUrl>

class QNetworkReply;

/// WebFetch is a utility class to download data from Mumble services.
class WebFetch : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(WebFetch)
protected:
	QObject *qoObject;
	const char *cpSlot;
	QNetworkReply *qnr;
	QString m_service;

	QString prefixedServiceHost() const;
	QString serviceHost() const;

	WebFetch(QString service, QUrl url, QObject *obj, const char *slot);
signals:
	void fetched(QByteArray data, QUrl url, QMap< QString, QString > headers);
protected slots:
	void finished();

public:
	/// The fetch function downloads the resource specified by the url parameter from the
	/// Mumble service given in the service parameter. Once the download completes, the
	/// function invokes the slot specified via the slot parameter.
	///
	/// Only the path part of the url parameter
	/// is used to construct the final, fully-qualified URL from which the resource is
	/// downloaded.
	///
	/// By default, the service parameter and the url parameter are combined to create
	/// a service as follows:
	///
	///     fullyQualifiedURL = https://${service}.mumble.info/${url.path}
	///
	/// When a resource is downloaded from a Mumble service, the service may optionally
	/// specify a service prefix to use for future requests to all Mumble services. This
	/// is communicated via the Use-Service-Prefix HTTP header in HTTP responses from
	/// Mumble services. When this function encounters such a response header, it stores
	/// the service prefix in the "net/serviceprefix" Mumble setting. If this setting
	/// is non-empty, the fully-qualified URL is instead constructed as such:
	///
	///     fullyQualifiedURL = https://${serivcePrefixSetting}-${service}.mumble.info/${url.path}
	///
	/// The service prefix must only contain ASCII characters 'A' through 'Z' (upper case)
	/// or 'a' through 'z (lower case).
	///
	/// @param  service  The Mumble service name to use for this request.
	///
	///                  The service name specified is used to create base URL
	///                  used by the final, fully-qualified URL as follows:
	///
	///                      baseURL = https://${service}.mumble.info
	///
	///                  If the Mumble setting "net/serviceprefix" is non-empty,
	///                  it will be used as a prefix to the base URL. In this case,
	///                  the base URL will be constructed as follows:
	///
	///                      baseURL = https://${servicePrefixSetting}-${service}.mumble.info
	///
	/// @param  url      The path to the endpoint that the request is targeted at.
	///                  Only the path of the URL will be used. The specified path is
	///                  used in combination with the base URL constructed by the service
	///                  parameter to construct the fully-qualified URL for the HTTP request
	///                  that will be sent by this function.
	///                  The path is used in combination with the base URL from the service
	///                  parameter as follows:
	///
	///                      fullyQualifiedURL = ${baseURL}/${url.path}
	///
	/// @param  slot     A Qt slot of the form fetched(QByteArray data, QUrl url,
	///                                                QMap<QString,QString> httpHeaders)
	///                  If the download initiated by the function was successful, the data
	///                  parameter will be a non-null QByteArray.
	///                  If the download failed, the data parameter will be a null QByteArray.
	static void fetch(const QString &service, const QUrl &url, QObject *obj, const char *slot);
};

#endif
