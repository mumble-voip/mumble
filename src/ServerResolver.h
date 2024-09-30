// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SERVERRESOLVER_H_
#define MUMBLE_MUMBLE_SERVERRESOLVER_H_

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "Net.h" // for HostAddress
#include "ServerResolverRecord.h"

class ServerResolverPrivate;

class ServerResolver : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ServerResolver)
public:
	ServerResolver(QObject *parent = nullptr);

	QString hostname();
	quint16 port();

	void resolve(QString hostname, quint16 port);
	QList< ServerResolverRecord > records();

signals:
	/// Resolved is fired once the ServerResolver
	/// has resolved the server address.
	void resolved();

private:
	ServerResolverPrivate *d;
};

#endif
