// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PacketDataStream.h"

quint32 PacketDataStream::size() const {
	return offset;
}

quint32 PacketDataStream::capacity() const {
	return maxsize;
}

bool PacketDataStream::isValid() const {
	return ok;
}

quint32 PacketDataStream::left() const {
	return maxsize - offset;
}

quint32 PacketDataStream::undersize() const {
	return overshoot;
}

void PacketDataStream::append(const quint64 v) {
#ifndef QT_NO_DEBUG
	Q_ASSERT(v <= 0xff);
#endif
	if (offset < maxsize)
		data[offset++] = static_cast< unsigned char >(v);
	else {
		ok = false;
		overshoot++;
	}
};

void PacketDataStream::append(const char *d, quint32 len) {
	if (left() >= len) {
		memcpy(&data[offset], d, len);
		offset += len;
	} else {
		unsigned int l = left();
		memset(&data[offset], 0, l);
		offset += l;
		overshoot += len - l;
		ok = false;
	}
}

void PacketDataStream::skip(quint32 len) {
	if (left() >= len)
		offset += len;
	else
		ok = false;
}

quint64 PacketDataStream::next() {
	if (offset < maxsize)
		return data[offset++];
	else {
		ok = false;
		return 0;
	}
};

quint8 PacketDataStream::next8() {
	if (offset < maxsize)
		return data[offset++];
	else {
		ok = false;
		return 0;
	}
}

void PacketDataStream::rewind() {
	offset = 0;
}

void PacketDataStream::truncate() {
	maxsize = offset;
}

const unsigned char *PacketDataStream::dataPtr() const {
	return reinterpret_cast< const unsigned char * >(&data[offset]);
}

unsigned char *PacketDataStream::dataPtr() {
	return reinterpret_cast< unsigned char * >(&data[offset]);
}

const char *PacketDataStream::charPtr() const {
	return reinterpret_cast< const char * >(&data[offset]);
}

QByteArray PacketDataStream::dataBlock(quint32 len) {
	if (len <= left()) {
		QByteArray a(charPtr(), static_cast< int >(len));
		offset += len;
		return a;
	} else {
		ok = false;
		return QByteArray();
	}
}

void PacketDataStream::setup(unsigned char *d, unsigned int msize) {
	data      = d;
	offset    = 0;
	overshoot = 0;
	maxsize   = msize;
	ok        = true;
}

PacketDataStream::PacketDataStream(const unsigned char *d, unsigned int msize) {
	setup(const_cast< unsigned char * >(d), msize);
};

PacketDataStream::PacketDataStream(const char *d, unsigned int msize) {
	setup(const_cast< unsigned char * >(reinterpret_cast< const unsigned char * >(d)), msize);
};

PacketDataStream::PacketDataStream(char *d, unsigned int msize) {
	setup(reinterpret_cast< unsigned char * >(d), msize);
};

PacketDataStream::PacketDataStream(unsigned char *d, unsigned int msize) {
	setup(d, msize);
};

PacketDataStream::PacketDataStream(const QByteArray &qba) {
	setup(const_cast< unsigned char * >(reinterpret_cast< const unsigned char * >(qba.constData())),
		  static_cast< unsigned int >(qba.size()));
}

PacketDataStream::PacketDataStream(QByteArray &qba) {
	unsigned char *ptr = reinterpret_cast< unsigned char * >(qba.data());
	setup(ptr, static_cast< unsigned int >(qba.capacity()));
}

PacketDataStream &PacketDataStream::operator<<(const quint64 value) {
	quint64 i = value;

	if ((i & 0x8000000000000000LL) && (~i < 0x100000000LL)) {
		// Signed number.
		i = ~i;
		if (i <= 0x3) {
			// Special case for -1 to -4. The most significant bits of the first byte must be (in binary) 111111
			// followed by the 2 bits representing the absolute value of the encoded number. Shortcase for -1 to -4
			append(0xFC | i);
			return *this;
		} else {
			// Add flag byte, whose most significant bits are (in binary) 111110 that indicates
			// that what follows is the varint encoding of the absolute value of i, but that the
			// value itself is supposed to be negative.
			append(0xF8);
		}
	}
	if (i < 0x80) {
		// Encode as 7-bit, positive number -> most significant bit of first byte must be zero
		append(i);
	} else if (i < 0x4000) {
		// Encode as 14-bit, positive number -> most significant bits of first byte must be (in binary) 10
		append((i >> 8) | 0x80);
		append(i & 0xFF);
	} else if (i < 0x200000) {
		// Encode as 21-bit, positive number -> most significant bits of first byte must be (in binary) 110
		append((i >> 16) | 0xC0);
		append((i >> 8) & 0xFF);
		append(i & 0xFF);
	} else if (i < 0x10000000) {
		// Encode as 28-bit, positive number -> most significant bits of first byte must be (in binary) 1110
		append((i >> 24) | 0xE0);
		append((i >> 16) & 0xFF);
		append((i >> 8) & 0xFF);
		append(i & 0xFF);
	} else if (i < 0x100000000LL) {
		// Encode as 32-bit, positive number -> most significant bits of first byte must be (in binary) 111100
		// Remaining bits in first byte remain unused
		append(0xF0);
		append((i >> 24) & 0xFF);
		append((i >> 16) & 0xFF);
		append((i >> 8) & 0xFF);
		append(i & 0xFF);
	} else {
		// Encode as 64-bit, positive number -> most significant bits of first byte must be (in binary) 111101
		// Remaining bits in first byte remain unused
		append(0xF4);
		append((i >> 56) & 0xFF);
		append((i >> 48) & 0xFF);
		append((i >> 40) & 0xFF);
		append((i >> 32) & 0xFF);
		append((i >> 24) & 0xFF);
		append((i >> 16) & 0xFF);
		append((i >> 8) & 0xFF);
		append(i & 0xFF);
	}
	return *this;
}

PacketDataStream &PacketDataStream::operator>>(quint64 &i) {
	quint64 v = next();

	if ((v & 0x80) == 0x00) {
		i = (v & 0x7F);
	} else if ((v & 0xC0) == 0x80) {
		i = (v & 0x3F) << 8 | next();
	} else if ((v & 0xF0) == 0xF0) {
		switch (v & 0xFC) {
			case 0xF0:
				i = next() << 24 | next() << 16 | next() << 8 | next();
				break;
			case 0xF4:
				i = next() << 56 | next() << 48 | next() << 40 | next() << 32 | next() << 24 | next() << 16
					| next() << 8 | next();
				break;
			case 0xF8:
				*this >> i;
				i = ~i;
				break;
			case 0xFC:
				i = v & 0x03;
				i = ~i;
				break;
			default:
				ok = false;
				i  = 0;
				break;
		}
	} else if ((v & 0xF0) == 0xE0) {
		i = (v & 0x0F) << 24 | next() << 16 | next() << 8 | next();
	} else if ((v & 0xE0) == 0xC0) {
		i = (v & 0x1F) << 16 | next() << 8 | next();
	}
	return *this;
}

PacketDataStream &PacketDataStream::operator<<(const QByteArray &a) {
	*this << a.size();
	append(a.constData(), static_cast< unsigned int >(a.size()));
	return *this;
}

PacketDataStream &PacketDataStream::operator>>(QByteArray &a) {
	quint32 len;
	*this >> len;
	if (len > left()) {
		len = left();
		ok  = false;
	}
	a = QByteArray(reinterpret_cast< const char * >(&data[offset]), static_cast< int >(len));
	offset += len;
	return *this;
}

PacketDataStream &PacketDataStream::operator<<(const QString &s) {
	return *this << s.toUtf8();
}

// Using the data directly instead of through qbuff avoids a copy.
PacketDataStream &PacketDataStream::operator>>(QString &s) {
	quint32 len;
	*this >> len;
	if (len > left()) {
		len = left();
		ok  = false;
	}
	s = QString::fromUtf8(reinterpret_cast< const char * >(&data[offset]), static_cast< int >(len));
	offset += len;
	return *this;
}

PacketDataStream &PacketDataStream::operator<<(const bool b) {
	quint32 v = b ? 1 : 0;
	return *this << v;
}

PacketDataStream &PacketDataStream::operator>>(bool &b) {
	quint32 v;
	*this >> v;
	b = v ? true : false;
	return *this;
}

union double64u {
	quint64 ui;
	double d;
};

PacketDataStream &PacketDataStream::operator<<(const double v) {
	double64u u;
	u.d = v;
	return *this << u.ui;
}

PacketDataStream &PacketDataStream::operator>>(double &v) {
	double64u u;
	*this >> u.ui;
	v = u.d;
	return *this;
}

union float32u {
	quint8 ui[4];
	float f;
};

PacketDataStream &PacketDataStream::operator<<(const float v) {
	float32u u;
	u.f = v;
	append(u.ui[0]);
	append(u.ui[1]);
	append(u.ui[2]);
	append(u.ui[3]);
	return *this;
}

PacketDataStream &PacketDataStream::operator>>(float &v) {
	float32u u;
	if (left() < 4) {
		ok = false;
		v  = 0;
	}
	u.ui[0] = next8();
	u.ui[1] = next8();
	u.ui[2] = next8();
	u.ui[3] = next8();
	v       = u.f;
	return *this;
}

#define INTMAPOPERATOR(type)                                                          \
	template<> PacketDataStream &PacketDataStream::operator<<< type >(const type v) { \
		return *this << static_cast< quint64 >(v);                                    \
	}                                                                                 \
	template<> PacketDataStream &PacketDataStream::operator>>< type >(type &v) {      \
		quint64 vv;                                                                   \
		*this >> vv;                                                                  \
		v = static_cast< type >(vv);                                                  \
		return *this;                                                                 \
	}

// INTMAPOPERATOR(qsizetype);
INTMAPOPERATOR(int);
INTMAPOPERATOR(unsigned int);
INTMAPOPERATOR(short);
INTMAPOPERATOR(unsigned short);
INTMAPOPERATOR(char);
INTMAPOPERATOR(unsigned char);

#undef INTMAPOPERATOR
