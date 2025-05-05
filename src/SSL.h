// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SSL_H_
#define MUMBLE_SSL_H_

#include <QList>
#include <QSslCipher>
#include <QString>

class MumbleSSL {
public:
	static void initialize();
	static void destroy();
	static QString defaultOpenSSLCipherString();
	static QList< QSslCipher > ciphersFromOpenSSLCipherString(QString cipherString);
	static void addSystemCA();
	static QString protocolToString(QSsl::SslProtocol protocol);
};

#endif
