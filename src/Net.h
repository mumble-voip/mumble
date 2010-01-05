/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _NET_H
#define _NET_H

#include "murmur_pch.h"

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

	bool match(const HostAddress &, int) const;

	QString toString() const;

	std::string toStdString() const;
	QHostAddress toAddress() const;
	QByteArray toByteArray() const;
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
};

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
#define SWAP64(x) (x)
#else
#ifdef __x86_64__
#define SWAP64(x) ({register quint64 __out, __in = (x); __asm__("bswap %q0" : "=r"(__out) : "0"(__in)); __out;})
#else
#define SWAP64(x) qbswap<quint64>(x)
#endif
#endif

#endif
