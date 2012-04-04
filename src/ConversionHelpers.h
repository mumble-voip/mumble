/* Copyright (C) 2005-2012, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef CONVERSION_HELPERS_H_
#define CONVERSION_HELPERS_H_

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QCryptographicHash>
#include <string>

inline QString u8(const ::std::string &str) {
	return QString::fromUtf8(str.data(), static_cast<int>(str.length()));
}

inline QString u8(const ::std::wstring &str) {
	return QString::fromStdWString(str);
}

inline ::std::string u8(const QString &str) {
	const QByteArray &qba = str.toUtf8();
	return ::std::string(qba.constData(), qba.length());
}

inline QByteArray blob(const ::std::string &str) {
	return QByteArray(str.data(), static_cast<int>(str.length()));
}

inline ::std::string blob(const QByteArray &str) {
	return ::std::string(str.constData(), str.length());
}

inline QByteArray sha1(const QByteArray &blob) {
	return QCryptographicHash::hash(blob, QCryptographicHash::Sha1);
}

inline QByteArray sha1(const QString &str) {
	return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha1);
}

#endif // CONVERSION_HELPERS_H_
