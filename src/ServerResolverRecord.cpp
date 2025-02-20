// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerResolverRecord.h"

ServerResolverRecord::ServerResolverRecord()
{
}

ServerResolverRecord::ServerResolverRecord(QString hostname_,
                                           quint16 port_,
                                           qint64 priority_,
										   QList< HostAddress > addresses_)
    : m_hostname(hostname_),
      m_port(port_),
      m_priority(priority_),
      m_addresses(addresses_)
{
}

qint64 ServerResolverRecord::priority() const {
	return m_priority;
}

const QString& ServerResolverRecord::hostname() const {
	return m_hostname;
}

quint16 ServerResolverRecord::port() const {
	return m_port;
}

const QList< HostAddress >& ServerResolverRecord::addresses() const {
	return m_addresses;
}
