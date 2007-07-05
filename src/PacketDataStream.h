/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _PACKETDATASTREAM_H
#define _PACKETDATASTREAM_H

/*
 * GCC doesn't yet do inter-object-file inlining, so unfortunately, this all has to be defined here.
 */

class PacketDataStream {
	protected:
		unsigned char *data;
		quint32 maxsize;
		quint32 offset;
		bool ok;

	public:
		quint32 size() const {
			return offset;
		}

		bool isValid() const {
			return ok;
		}

		quint32 left() const {
			return maxsize - offset;
		}

		void append(const quint32 v) {
			if (offset < maxsize)
				data[offset++] = v;
			else
				ok = false;
		};

		void append(const char *d, quint32 len) {
			if (left() >= len) {
				memcpy(& data[offset], d, len);
				offset += len;
			} else {
				ok = false;
			}
		}

		void skip(quint32 len) {
			if (left() >= len)
				offset += len;
			else
				ok = false;
		}

		quint32 next() {
			if (offset < maxsize)
				return data[offset++];
			else {
				ok = false;
				return 0;
			}
		};

		void rewind() {
			offset = 0;
		}

		void truncate() {
			maxsize = offset;
		}

		const unsigned char *dataPtr() const {
			return reinterpret_cast<const unsigned char *>(& data[offset]);
		}

		const char *charPtr() const {
			return reinterpret_cast<const char *>(& data[offset]);
		}

		QByteArray dataBlock(quint32 len) {
			if (len <= left()) {
				QByteArray a(charPtr(), len);
				offset +=len;
				return a;
			} else {
				ok = false;
				return QByteArray();
			}
		}

	protected:
		void setup(unsigned char *d, int msize) {
			data = d;
			offset = 0;
			maxsize = msize;
			ok = true;
		}
	public:
		PacketDataStream(const char *d, int msize) {
			setup(const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(d)), msize);
		};

		PacketDataStream(char *d, int msize) {
			setup(reinterpret_cast<unsigned char *>(d), msize);
		};

		PacketDataStream(unsigned char *d, int msize) {
			setup(d, msize);
		};

		PacketDataStream &operator <<(const quint32 i) {
			if (i < 0x80) {
			    	// Need top bit clear
				append(i);
			} else if (i < 0x4000) {
			    	// Need top two bits clear
				append((i >> 8) | 0x80);
				append(i & 0xFF);
			} else if (i < 0x200000) {
			    	// Need top three bits clear
				append((i >> 16) | 0xC0);
				append((i >> 8) & 0xFF);
				append(i & 0xFF);
			} else if (i < 0x10000000) {
			    	// Need top four bits clear
				append((i >> 24) | 0xE0);
				append((i >> 16) & 0xFF);
				append((i >> 8) & 0xFF);
				append(i & 0xFF);
			} else {
				append(0xF0);
				append((i >> 24) & 0xFF);
				append((i >> 16) & 0xFF);
				append((i >> 8) & 0xFF);
				append(i & 0xFF);
			}
			return *this;
		}

		PacketDataStream &operator >>(quint32 &i) {
			quint32 v = next();
			if ((v & 0xF0) == 0xF0) {
				i=next() << 24 | next() << 16 | next() << 8 | next();
			} else if ((v & 0xF0) == 0xE0) {
				i=(v & 0x0F) << 24 | next() << 16 | next() << 8 | next();
			} else if ((v & 0xE0) == 0xC0) {
				i=(v & 0x1F) << 16 | next() << 8 | next();
			} else if ((v & 0xC0) == 0x80) {
			        i=(v & 0x3F) << 8 | next();
			} else {
				i=(v & 0x7F);
			}
			return *this;
		}

		PacketDataStream &operator <<(const QByteArray &a) {
			*this << a.size();
			append(a.constData(), a.size());
			return *this;
		}

		PacketDataStream &operator >>(QByteArray &a) {
			quint32 len;
			*this >> len;
			if (len > left()) {
				len = left();
				ok = false;
			}
			a = QByteArray(reinterpret_cast<const char *>(& data[offset]), len);
			offset+=len;
			return *this;
		}

		PacketDataStream &operator <<(const QString &s) {
			return *this << s.toUtf8();
		}

		// Using the data directly instead of through qbuff avoids a copy.
		PacketDataStream &operator >>(QString &s) {
			quint32 len;
			*this >> len;
			if (len > left()) {
				len = left();
				ok = false;
			}
			s = QString::fromUtf8(reinterpret_cast<const char *>(& data[offset]), len);
			offset+=len;
			return *this;
		}

		PacketDataStream &operator <<(const bool b) {
			quint32 v = b ? 1 : 0;
			return *this << v;
		}

		PacketDataStream &operator >>(bool &b) {
			quint32 v;
			*this >> v;
			b = v ? true : false;
			return *this;
		}

#define INTMAPOPERATOR(type) \
		PacketDataStream &operator <<(const type v) { \
			return *this << static_cast<quint32>(v); \
		} \
		PacketDataStream &operator >>(type &v) { \
			quint32 vv; \
			*this >> vv; \
			v = static_cast<type>(vv); \
			return *this; \
		}


		INTMAPOPERATOR(int);
//		Oh, how horribly this bugs
//		INTMAPOPERATOR(unsigned int);

		INTMAPOPERATOR(short);
		INTMAPOPERATOR(unsigned short);
		INTMAPOPERATOR(char);
		INTMAPOPERATOR(unsigned char);

		template <typename T>
		PacketDataStream &operator <<(const QList<T> &l) {
			*this << l.size();
			for(int i=0;i < l.size();i++)
				*this << l.at(i);
			return *this;
		}

		template <typename T>
		PacketDataStream &operator >>(QList<T> &l) {
			l.clear();
			quint32 len;
			*this >> len;
			if (len > left()) {
				len = left();
				ok = false;
			}
			for(quint32 i=0;i<len;i++) {
				if (left() == 0) {
					ok = false;
					break;
				}

				T t;
				*this >> t;
				l.append(t);
			}
			return *this;
		}


		template <typename T>
		PacketDataStream &operator <<(const QSet<T> &s) {
			*this << s.size();
			for(typename QSet<T>::const_iterator i=s.constBegin();i!=s.constEnd();++i)
				*this << *i;
			return *this;
		}

		template <typename T>
		PacketDataStream &operator >>(QSet<T> &s) {
			s.clear();
			quint32 len;
			*this >> len;
			if (len > left()) {
				len = left();
				ok = false;
			}
			for(quint32 i=0;i<len;i++) {
				if (left() == 0) {
					ok = false;
					break;
				}

				T t;
				*this >> t;
				s.insert(t);
			}
			return *this;
		}

		template <typename T,typename U>
		PacketDataStream &operator <<(const QPair<T,U> &p) {
			return *this << p.first << p.second;
		}

		template <typename T,typename U>
		PacketDataStream &operator >>(QPair<T,U> &p) {
			return *this >> p.first >> p.second;
		}

};

#else
class PacketDataStream;
#endif
