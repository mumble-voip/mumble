// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HostAddress.h"

#include "ByteSwap.h"

#include <QRegularExpression>

#ifdef Q_OS_WIN
#	include "win.h"
#	include <winsock2.h>
#	include <ws2tcpip.h>
#else
#	include <arpa/inet.h>
#	if defined(Q_OS_FREEBSD) || defined(Q_OS_OPENBSD)
#		include <netinet/in.h>
#		include <sys/socket.h>
#		include <sys/types.h>
#	endif
#endif

#include <cassert>
#include <cstdint>
#include <utility>

HostAddress::HostAddress(const Q_IPV6ADDR &address) {
	memcpy(m_byteRepresentation.data(), address.c, m_byteRepresentation.size());
}

HostAddress::HostAddress(const std::string &address) {
	if (address.length() != 16) {
		// This is an invalid address -> reset the currently stored address
		m_byteRepresentation.fill(0);
	} else {
		for (std::size_t i = 0; i < m_byteRepresentation.size(); ++i) {
			m_byteRepresentation[i] = static_cast< unsigned char >(address[i]);
		}
	}
}

HostAddress::HostAddress(const QByteArray &address) {
	if (address.length() != 16) {
		// This is an invalid address -> reset the currently stored address
		m_byteRepresentation.fill(0);
	} else {
		for (unsigned int i = 0; i < m_byteRepresentation.size(); ++i) {
			m_byteRepresentation[i] = static_cast< unsigned char >(address[i]);
		}
	}
}

HostAddress::HostAddress(const QHostAddress &address) {
	if (address.protocol() == QAbstractSocket::IPv6Protocol) {
		const Q_IPV6ADDR &a = address.toIPv6Address();
		memcpy(m_byteRepresentation.data(), a.c, m_byteRepresentation.size());
	} else {
		fromIPv4(address.toIPv4Address());
	}
}

HostAddress::HostAddress(const sockaddr_storage &address) {
	if (address.ss_family == AF_INET) {
		const struct sockaddr_in *in = reinterpret_cast< const struct sockaddr_in * >(&address);
		fromIPv4(in->sin_addr.s_addr, false);
	} else if (address.ss_family == AF_INET6) {
		const struct sockaddr_in6 *in6 = reinterpret_cast< const struct sockaddr_in6 * >(&address);
		memcpy(m_byteRepresentation.data(), in6->sin6_addr.s6_addr, m_byteRepresentation.size());
	} else {
		m_byteRepresentation.fill(0);
	}
}

void HostAddress::fromIPv4(std::uint32_t address, bool convertToNetworkOrder) {
	// Store IPv4 address in IPv6 format:
	// - address is stored in the 4 last bytes in network byte order
	// - the 2 bytes just before that are set to 0xFF respectively
	m_byteRepresentation.fill(0);

	m_byteRepresentation[10] = 0xFF;
	m_byteRepresentation[11] = 0xFF;

	if (convertToNetworkOrder) {
		address = htonl(address);
	}

	memcpy(&m_byteRepresentation[12], &address, sizeof(std::uint32_t));
}

bool HostAddress::operator<(const HostAddress &other) const {
	return m_byteRepresentation < other.m_byteRepresentation;
}

bool HostAddress::operator==(const HostAddress &other) const {
	return m_byteRepresentation == other.m_byteRepresentation;
}

bool HostAddress::match(const HostAddress &netmask, unsigned int bits) const {
	for (std::size_t i = 0; i < m_byteRepresentation.size(); ++i) {
		if (bits >= 8) {
			// Compare full byte
			if (m_byteRepresentation[i] != netmask.m_byteRepresentation[i]) {
				return false;
			}
			bits -= 8;
		} else {
			// Compare only the first bits bits (no this is not a typo)
			using mask_t = std::uint8_t;
			mask_t mask =
				static_cast< mask_t >(std::numeric_limits< mask_t >::max() >> (sizeof(mask_t) * CHAR_BIT - bits));
			mask = static_cast< mask_t >(htons(mask));

			if ((m_byteRepresentation[i] & mask) != (netmask.m_byteRepresentation[i] & mask)) {
				return false;
			}

			break;
		}
	}

	return true;
}

std::string HostAddress::toStdString() const {
	return std::string(reinterpret_cast< const char * >(m_byteRepresentation.data()), m_byteRepresentation.size());
}

bool HostAddress::isV6() const {
	std::uint64_t firstEightBytes      = *(reinterpret_cast< const std::uint64_t * >(m_byteRepresentation.data()));
	std::uint16_t bytesNineAndTen      = *(reinterpret_cast< const std::uint16_t * >(&m_byteRepresentation[8]));
	std::uint16_t bytesElevenAndTwelve = *(reinterpret_cast< const std::uint16_t * >(&m_byteRepresentation[10]));
	return firstEightBytes != 0 || bytesNineAndTen != 0 || bytesElevenAndTwelve != 0xFFFF;
}

bool HostAddress::isValid() const {
	return m_byteRepresentation != decltype(m_byteRepresentation){};
}

QHostAddress HostAddress::toAddress() const {
	QHostAddress address = QHostAddress(m_byteRepresentation.data());
	if (!isV6()) {
		address.setAddress(address.toIPv4Address());
	}
	return address;
}

QByteArray HostAddress::toByteArray() const {
	return QByteArray(reinterpret_cast< const char * >(m_byteRepresentation.data()),
					  static_cast< int >(m_byteRepresentation.size()));
}

void HostAddress::toSockaddr(sockaddr_storage *dst) const {
	memset(dst, 0, sizeof(*dst));
	if (isV6()) {
		struct sockaddr_in6 *in6 = reinterpret_cast< struct sockaddr_in6 * >(dst);
		dst->ss_family           = AF_INET6;
		memcpy(in6->sin6_addr.s6_addr, m_byteRepresentation.data(), m_byteRepresentation.size());
	} else {
		struct sockaddr_in *in = reinterpret_cast< struct sockaddr_in * >(dst);
		dst->ss_family         = AF_INET;
		in->sin_addr.s_addr    = toIPv4();
	}
}

std::uint32_t HostAddress::toIPv4() const {
	// The IPv4 address is stored in the last four bytes (in network byte order)
	return *(reinterpret_cast< const std::uint32_t * >(&m_byteRepresentation[12]));
}

const std::array< std::uint8_t, 16 > &HostAddress::getByteRepresentation() const {
	return m_byteRepresentation;
}

void HostAddress::reset() {
	m_byteRepresentation.fill(0);
}

void HostAddress::setByte(std::size_t idx, std::uint8_t value) {
	assert(idx < m_byteRepresentation.size());
	m_byteRepresentation[idx] = value;
}

std::size_t qHash(const HostAddress &ha) {
	return qHashRange(ha.m_byteRepresentation.begin(), ha.m_byteRepresentation.end());
}

QString HostAddress::toString(bool bracketEnclosed) const {
	if (isV6()) {
		if (isValid()) {
			QString str;
			const char *squareBracketOpen  = "";
			const char *squareBracketClose = "";
			if (bracketEnclosed) {
				squareBracketOpen  = "[";
				squareBracketClose = "]";
			}

			const std::uint16_t *shortArray = reinterpret_cast< const std::uint16_t * >(m_byteRepresentation.data());

			str = QString::asprintf("%s%x:%x:%x:%x:%x:%x:%x:%x%s", squareBracketOpen, ntohs(shortArray[0]),
									ntohs(shortArray[1]), ntohs(shortArray[2]), ntohs(shortArray[3]),
									ntohs(shortArray[4]), ntohs(shortArray[5]), ntohs(shortArray[6]),
									ntohs(shortArray[7]), squareBracketClose);

			return str.replace(QRegularExpression(QLatin1String("(:0)+")), QLatin1String(":"));
		} else {
			return bracketEnclosed ? QLatin1String("[::]") : QLatin1String("::");
		}
	} else {
		return toAddress().toString();
	}
}
