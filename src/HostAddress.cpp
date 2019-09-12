// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HostAddress.h"

#include "ByteSwap.h"

#ifdef Q_OS_WIN
# include "win.h"
# include <winsock2.h>
# include <ws2tcpip.h>
#else
# include <arpa/inet.h>
# if defined(Q_OS_FREEBSD) || defined(Q_OS_OPENBSD)
#  include <netinet/in.h>
#  include <sys/socket.h>
#  include <sys/types.h>
# endif
#endif

HostAddress::HostAddress() {
	addr[0] = addr[1] = 0ULL;
}

HostAddress::HostAddress(const Q_IPV6ADDR &address) {
	memcpy(qip6.c, address.c, 16);
}

HostAddress::HostAddress(const std::string &address) {
	if (address.length() != 16)
		addr[0] = addr[1] = 0ULL;
	else
		for (int i=0;i<16;++i)
			qip6[i] = address[i];
}

HostAddress::HostAddress(const QByteArray &address) {
	if (address.length() != 16)
		addr[0] = addr[1] = 0ULL;
	else
		for (int i=0;i<16;++i)
			qip6[i] = address[i];
}

HostAddress::HostAddress(const QHostAddress &address) {
	if (address.protocol() == QAbstractSocket::IPv6Protocol) {
		const Q_IPV6ADDR &a = address.toIPv6Address();
		memcpy(qip6.c, a.c, 16);
	} else {
		addr[0] = 0ULL;
		shorts[4] = 0;
		shorts[5] = 0xffff;
		hash[3] = htonl(address.toIPv4Address());
	}
}

HostAddress::HostAddress(const sockaddr_storage &address) {
	if (address.ss_family == AF_INET) {
		const struct sockaddr_in *in = reinterpret_cast<const struct sockaddr_in *>(&address);
		addr[0] = 0ULL;
		shorts[4] = 0;
		shorts[5] = 0xffff;
		hash[3] = in->sin_addr.s_addr;
	} else if (address.ss_family == AF_INET6) {
		const struct sockaddr_in6 *in6 = reinterpret_cast<const struct sockaddr_in6 *>(&address);
		memcpy(qip6.c, in6->sin6_addr.s6_addr, 16);
	} else {
		addr[0] = addr[1] = 0ULL;
	}
}

bool HostAddress::operator < (const HostAddress &other) const {
	return memcmp(qip6.c, other.qip6.c, 16) < 0;
}

bool HostAddress::operator == (const HostAddress &other) const {
	return ((addr[0] == other.addr[0]) && (addr[1] == other.addr[1]));
}

bool HostAddress::match(const HostAddress &netmask, int bits) const {
	quint64 mask[2];

	if (bits == 128) {
		mask[0] = mask[1] = 0xffffffffffffffffULL;
	} else if (bits > 64) {
		mask[0] = 0xffffffffffffffffULL;
		mask[1] = SWAP64(~((1ULL << (128-bits)) - 1));
	} else {
		mask[0] = SWAP64(~((1ULL << (64-bits)) - 1));
		mask[1] = 0ULL;
	}
	return ((addr[0] & mask[0]) == (netmask.addr[0] & mask[0])) && ((addr[1] & mask[1]) == (netmask.addr[1] & mask[1]));
}

std::string HostAddress::toStdString() const {
	return std::string(reinterpret_cast<const char *>(qip6.c), 16);
}

bool HostAddress::isV6() const {
	return (addr[0] != 0ULL) || (shorts[4] != 0) || (shorts[5] != 0xffff);
}

bool HostAddress::isValid() const {
	return (addr[0] != 0ULL) || (addr[1] != 0ULL);
}

QHostAddress HostAddress::toAddress() const {
	if (isV6())
		return QHostAddress(qip6);
	else {
		return QHostAddress(ntohl(hash[3]));
	}
}

QByteArray HostAddress::toByteArray() const {
	return QByteArray(reinterpret_cast<const char *>(qip6.c), 16);
}

void HostAddress::toSockaddr(sockaddr_storage *dst) const {
	memset(dst, 0, sizeof(*dst));
	if (isV6()) {
		struct sockaddr_in6 *in6 = reinterpret_cast<struct sockaddr_in6 *>(dst);
		dst->ss_family = AF_INET6;
		memcpy(in6->sin6_addr.s6_addr, qip6.c, 16);
	} else {
		struct sockaddr_in *in = reinterpret_cast<struct sockaddr_in *>(dst);
		dst->ss_family = AF_INET;
		in->sin_addr.s_addr = hash[3];
	}
}

quint32 qHash(const HostAddress &ha) {
	return (ha.hash[0] ^ ha.hash[1] ^ ha.hash[2] ^ ha.hash[3]);
}

QString HostAddress::toString() const {
	if (isV6()) {
		if (isValid()) {
			QString qs;
			qs.sprintf("[%x:%x:%x:%x:%x:%x:%x:%x]", ntohs(shorts[0]), ntohs(shorts[1]), ntohs(shorts[2]), ntohs(shorts[3]), ntohs(shorts[4]), ntohs(shorts[5]), ntohs(shorts[6]), ntohs(shorts[7]));
			return qs.replace(QRegExp(QLatin1String("(:0)+")),QLatin1String(":"));
		} else {
			return QLatin1String("[::]");
		}
	} else {
		return QHostAddress(ntohl(hash[3])).toString();
	}
}
