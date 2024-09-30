// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SERVERRESOLVERRECORD_H_
#define MUMBLE_MUMBLE_SERVERRESOLVERRECORD_H_

#include <QtCore/QList>
#include <QtCore/QString>

#include "Net.h" // for HostAddress

class ServerResolverRecord {
public:
	ServerResolverRecord();
	ServerResolverRecord(QString hostname_, quint16 port_, qint64 priority_, QList< HostAddress > addresses_);

	QString hostname();
	quint16 port();
	qint64 priority();
	QList< HostAddress > addresses();

protected:
	QString m_hostname;
	quint16 m_port;
	qint64 m_priority;
	QList< HostAddress > m_addresses;
};

#endif
