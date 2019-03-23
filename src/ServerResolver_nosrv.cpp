// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerResolver.h"

#include <QtNetwork/QHostInfo>

class ServerResolverPrivate : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ServerResolverPrivate)
	public:
		ServerResolverPrivate(QObject *parent);

		void resolve(QString hostname, quint16 port);
		QList<ServerResolverRecord> records();

		QString m_origHostname;
		quint16 m_origPort;

		QList<ServerResolverRecord> m_resolved;

	signals:
		void resolved();

	public slots:
		void hostResolved(QHostInfo hostInfo);
};

ServerResolverPrivate::ServerResolverPrivate(QObject *parent)
	: QObject(parent)
	, m_origPort(0) {
}

void ServerResolverPrivate::resolve(QString hostname, quint16 port) {
	m_origHostname = hostname;
	m_origPort = port;

	QHostInfo::lookupHost(hostname, this, SLOT(hostResolved(QHostInfo)));
}

QList<ServerResolverRecord> ServerResolverPrivate::records() {
	return m_resolved;
}

void ServerResolverPrivate::hostResolved(QHostInfo hostInfo) {
	if (hostInfo.error() == QHostInfo::NoError) {
		QList<QHostAddress> resolvedAddresses = hostInfo.addresses();
		
		// Convert QHostAddress -> HostAddress.
		QList<HostAddress> addresses;
		foreach (QHostAddress ha, resolvedAddresses) {
			addresses << HostAddress(ha);
		}

		m_resolved << ServerResolverRecord(m_origHostname, m_origPort, 0, addresses);
	}

	emit resolved();
}

ServerResolver::ServerResolver(QObject *parent)
	: QObject(parent) {

	d = new ServerResolverPrivate(this);
}

QString ServerResolver::hostname() {
	if (d) {
		return d->m_origHostname;
	}

	return QString();
}

quint16 ServerResolver::port() {
	if (d) {
		return d->m_origPort;
	}

	return 0;
}

void ServerResolver::resolve(QString hostname, quint16 port) {
	if (d) {
		connect(d, SIGNAL(resolved()), this, SIGNAL(resolved()));
		d->resolve(hostname, port);
	}
}

QList<ServerResolverRecord> ServerResolver::records() {
	if (d) {
		return d->records();
	}
	return QList<ServerResolverRecord>();
}

#include "ServerResolver_nosrv.moc"
