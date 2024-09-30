// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_HOSTADDRESS_H_
#define MUMBLE_HOSTADDRESS_H_

#include <QByteArray>
#include <QHostAddress>
#include <QString>
#include <Q_IPV6ADDR>

#include <array>
#include <cstdint>

struct HostAddress {
	HostAddress() = default;
	HostAddress(const Q_IPV6ADDR &);
	HostAddress(const std::string &);
	HostAddress(const QHostAddress &);
	HostAddress(const QByteArray &);
	HostAddress(const struct sockaddr_storage &);

	void fromIPv4(std::uint32_t address, bool convertToNetworkOrder = true);

	bool isV6() const;
	bool isValid() const;

	bool operator<(const HostAddress &) const;
	bool operator==(const HostAddress &) const;

	bool match(const HostAddress &, unsigned int bits) const;

	QString toString(bool bracketEnclosed = true) const;

	std::string toStdString() const;
	QHostAddress toAddress() const;
	QByteArray toByteArray() const;
	void toSockaddr(struct sockaddr_storage *dst) const;
	std::uint32_t toIPv4() const;

	const std::array< std::uint8_t, 16 > &getByteRepresentation() const;

	void reset();

	void setByte(std::size_t idx, std::uint8_t value);

	friend std::size_t qHash(const HostAddress &);

private:
	// Binary representation of an IPv6 address
	std::array< std::uint8_t, 16 > m_byteRepresentation;
};

Q_DECLARE_TYPEINFO(HostAddress, Q_MOVABLE_TYPE);

#endif
