// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SOCKETRPC_H_
#define MUMBLE_MUMBLE_SOCKETRPC_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QLocalSocket>

class QBuffer;
class QLocalServer;

class SocketRPCClient : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(SocketRPCClient)
	protected:
		QLocalSocket *qlsSocket;
		QXmlStreamReader qxsrReader;
		QXmlStreamWriter qxswWriter;
		QBuffer *qbBuffer;
		QByteArray qbaOutput;

		void processXml();
	public:
		SocketRPCClient(QLocalSocket *s, QObject *p = NULL);
	public slots:
		void disconnected();
		void error(QLocalSocket::LocalSocketError);
		void readyRead();
};

class SocketRPC : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(SocketRPC)
	protected:
		QLocalServer *qlsServer;
	public:
		typedef QMap<QString, QVariant> ParameterMap;
		SocketRPC(const QString &basename, QObject *p = NULL);
		static bool send(const QString &basename, const QString &request, const ParameterMap &param = ParameterMap());
	public slots:
		void newConnection();
};

#endif
