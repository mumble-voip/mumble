// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerResolver.h"

#include <QtNetwork/QDnsLookup>
#include <QtNetwork/QHostInfo>

static qint64 normalizeSrvPriority(quint16 priority, quint16 weight) {
	return static_cast< qint64 >((65535U * priority) + weight);
}

class ServerResolverPrivate : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ServerResolverPrivate)
public:
	ServerResolverPrivate(QObject *parent);

	void resolve(QString hostname, quint16 port);
	QList< ServerResolverRecord > records();

	QString m_origHostname;
	quint16 m_origPort;

	QList< QDnsServiceRecord > m_srvQueue;
	QMap< qsizetype, qsizetype > m_hostInfoIdToIndexMap;
	qsizetype m_srvQueueRemain;

	QList< ServerResolverRecord > m_resolved;

signals:
	void resolved();

public slots:
	void srvResolved();
	void hostResolved(QHostInfo hostInfo);
	void hostFallbackResolved(QHostInfo hostInfo);
};

ServerResolverPrivate::ServerResolverPrivate(QObject *parent) : QObject(parent), m_origPort(0), m_srvQueueRemain(0) {
}

void ServerResolverPrivate::resolve(QString hostname, quint16 port) {
	m_origHostname = hostname;
	m_origPort     = port;

	QDnsLookup *resolver = new QDnsLookup(this);
	connect(resolver, SIGNAL(finished()), this, SLOT(srvResolved()));
	resolver->setType(QDnsLookup::SRV);

	resolver->setName(QLatin1String("_mumble._tcp.") + hostname);
	resolver->lookup();
}

QList< ServerResolverRecord > ServerResolverPrivate::records() {
	return m_resolved;
}

void ServerResolverPrivate::srvResolved() {
	QDnsLookup *resolver = qobject_cast< QDnsLookup * >(sender());

	m_srvQueue       = resolver->serviceRecords();
	m_srvQueueRemain = m_srvQueue.count();

	if (resolver->error() == QDnsLookup::NoError && m_srvQueueRemain > 0) {
		for (decltype(m_srvQueue.count()) i = 0; i < m_srvQueue.count(); i++) {
			QDnsServiceRecord record = m_srvQueue.at(i);
			int hostInfoId           = QHostInfo::lookupHost(record.target(), this, SLOT(hostResolved(QHostInfo)));
			m_hostInfoIdToIndexMap[hostInfoId] = i;
		}
	} else {
		QHostInfo::lookupHost(m_origHostname, this, SLOT(hostFallbackResolved(QHostInfo)));
	}

	delete resolver;
}

void ServerResolverPrivate::hostResolved(QHostInfo hostInfo) {
	const int lookupId       = hostInfo.lookupId();
	const qsizetype idx      = m_hostInfoIdToIndexMap[lookupId];
	QDnsServiceRecord record = m_srvQueue.at(idx);

	if (hostInfo.error() == QHostInfo::NoError) {
		QList< QHostAddress > resolvedAddresses = hostInfo.addresses();

		// Convert QHostAddress -> HostAddress.
		QList< HostAddress > addresses;
		foreach (QHostAddress ha, resolvedAddresses) { addresses << HostAddress(ha); }

		qint64 priority = normalizeSrvPriority(record.priority(), record.weight());
		m_resolved << ServerResolverRecord(record.target(), record.port(), priority, addresses);
	}

	m_srvQueueRemain -= 1;
	if (m_srvQueueRemain == 0) {
		emit resolved();
	}
}

void ServerResolverPrivate::hostFallbackResolved(QHostInfo hostInfo) {
	if (hostInfo.error() == QHostInfo::NoError) {
		QList< QHostAddress > resolvedAddresses = hostInfo.addresses();

		// Convert QHostAddress -> HostAddress.
		QList< HostAddress > addresses;
		foreach (QHostAddress ha, resolvedAddresses) { addresses << HostAddress(ha); }

		m_resolved << ServerResolverRecord(m_origHostname, m_origPort, 0, addresses);
	}

	emit resolved();
}

ServerResolver::ServerResolver(QObject *parent) : QObject(parent) {
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

QList< ServerResolverRecord > ServerResolver::records() {
	if (d) {
		return d->records();
	}
	return QList< ServerResolverRecord >();
}

#include "ServerResolver.moc"
