/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Net.h"

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

quint32 qHash(const HostAddress &ha) {
	return (ha.hash[0] ^ ha.hash[1] ^ ha.hash[2] ^ ha.hash[3]);
}

bool Ban::operator <(const Ban &other) const {
	return haAddress < other.haAddress;
}

bool Ban::operator ==(const Ban &other) const {
	return (haAddress == other.haAddress) && (qsUsername == other.qsUsername) && (qsHash == other.qsHash) && (qsReason == other.qsReason) && (qdtStart == other.qdtStart) && (iDuration == other.iDuration);
}

bool Ban::isValid() const {
	return haAddress.isValid() && (iMask >= 8) && (iMask <= 128);
}
