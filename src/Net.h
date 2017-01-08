// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_NET_H_
#define MUMBLE_NET_H_

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtNetwork/Q_IPV6ADDR>

#ifndef DEFAULT_MUMBLE_PORT
#define DEFAULT_MUMBLE_PORT 64738
#endif

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

	bool operator < (const HostAddress &) const;
	bool operator == (const HostAddress &) const;

	bool match(const HostAddress &, int bits) const;

	QString toString() const;

	std::string toStdString() const;
	QHostAddress toAddress() const;
	QByteArray toByteArray() const;
	void toSockaddr(struct sockaddr_storage *dst) const;
};

Q_DECLARE_TYPEINFO(HostAddress, Q_MOVABLE_TYPE);

quint32 qHash(const HostAddress &);

struct Ban {
	HostAddress haAddress;
	int iMask;
	QString qsUsername;
	QString qsHash;
	QString qsReason;
	QDateTime qdtStart;
	unsigned int iDuration;
	bool isExpired() const;
	bool isValid() const;
	bool operator < (const Ban &) const;
	bool operator == (const Ban &) const;
	QString toString() const;
};

quint32 qHash(const Ban &);

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
#define SWAP64(x) (x)
#else
#if defined(__x86_64__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
#define SWAP64(x) __builtin_bswap64(x)
#else
#define SWAP64(x) qbswap<quint64>(x)
#endif
#endif

#endif
