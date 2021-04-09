// Copyright 2017-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_HOSTADDRESS_H_
#define MUMBLE_HOSTADDRESS_H_

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QtGlobal>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/Q_IPV6ADDR>

struct HostAddress {
	union {
		Q_IPV6ADDR qip6;
		quint16 shorts[8];
		quint32 hash[4];
		quint64 addr[2];
	};

	HostAddress();
	HostAddress(const Q_IPV6ADDR &);
	HostAddress(const std::string &);
	HostAddress(const QHostAddress &);
	HostAddress(const QByteArray &);
	HostAddress(const struct sockaddr_storage &);

	bool isV6() const;
	bool isValid() const;

	bool operator<(const HostAddress &) const;
	bool operator==(const HostAddress &) const;

	bool match(const HostAddress &, int bits) const;

	QString toString() const;

	std::string toStdString() const;
	QHostAddress toAddress() const;
	QByteArray toByteArray() const;
	void toSockaddr(struct sockaddr_storage *dst) const;
};

Q_DECLARE_TYPEINFO(HostAddress, Q_MOVABLE_TYPE);

quint32 qHash(const HostAddress &);

#endif
