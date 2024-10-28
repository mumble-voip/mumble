// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SocketRPC.h"

#include "Channel.h"
#include "ClientUser.h"
#include "MainWindow.h"
#include "ServerHandler.h"
#include "Global.h"

#include <QtCore/QProcessEnvironment>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QLocalServer>
#include <QtXml/QDomDocument>

SocketRPCClient::SocketRPCClient(QLocalSocket *s, QObject *p) : QObject(p), qlsSocket(s), qbBuffer(nullptr) {
	qlsSocket->setParent(this);

	connect(qlsSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(qlsSocket, SIGNAL(error(QLocalSocket::LocalSocketError)), this,
			SLOT(error(QLocalSocket::LocalSocketError)));
	connect(qlsSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

	qxsrReader.setDevice(qlsSocket);
	qxswWriter.setAutoFormatting(true);

	qbBuffer = new QBuffer(&qbaOutput, this);
	qbBuffer->open(QIODevice::WriteOnly);
	qxswWriter.setDevice(qbBuffer);
}

void SocketRPCClient::disconnected() {
	deleteLater();
}

void SocketRPCClient::error(QLocalSocket::LocalSocketError) {
}

void SocketRPCClient::readyRead() {
	forever {
		switch (qxsrReader.readNext()) {
			case QXmlStreamReader::Invalid: {
				if (qxsrReader.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
					qWarning() << "Malformed" << qxsrReader.error();
					qlsSocket->abort();
				}
				return;
			} break;
			case QXmlStreamReader::EndDocument: {
				qxswWriter.writeCurrentToken(qxsrReader);

				processXml();

				qxsrReader.clear();
				qxsrReader.setDevice(qlsSocket);

				qxswWriter.setDevice(nullptr);
				delete qbBuffer;
				qbaOutput = QByteArray();
				qbBuffer  = new QBuffer(&qbaOutput, this);
				qbBuffer->open(QIODevice::WriteOnly);
				qxswWriter.setDevice(qbBuffer);
			} break;
			default:
				qxswWriter.writeCurrentToken(qxsrReader);
				break;
		}
	}
}

void SocketRPCClient::processXml() {
	QDomDocument qdd;
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	qdd.setContent(qbaOutput, QDomDocument::ParseOption::Default);
#else
	qdd.setContent(qbaOutput, false);
#endif

	QDomElement request = qdd.firstChildElement();

	if (!request.isNull()) {
		bool ack = false;
		QMap< QString, QVariant > qmRequest;
		QMap< QString, QVariant > qmReply;
		QMap< QString, QVariant >::const_iterator iter;

		QDomNamedNodeMap attributes = request.attributes();
		for (int i = 0; i < attributes.count(); ++i) {
			QDomAttr attr = attributes.item(i).toAttr();
			qmRequest.insert(attr.name(), attr.value());
		}
		QDomNodeList childNodes = request.childNodes();
		for (int i = 0; i < childNodes.count(); ++i) {
			QDomElement child = childNodes.item(i).toElement();
			if (!child.isNull())
				qmRequest.insert(child.nodeName(), child.text());
		}

		iter = qmRequest.find(QLatin1String("reqid"));
		if (iter != qmRequest.constEnd())
			qmReply.insert(iter.key(), iter.value());

		if (request.nodeName() == QLatin1String("focus")) {
			Global::get().mw->show();
			Global::get().mw->raise();
			Global::get().mw->activateWindow();

			ack = true;
		} else if (request.nodeName() == QLatin1String("self")) {
			iter = qmRequest.find(QLatin1String("mute"));
			if (iter != qmRequest.constEnd()) {
				bool set = iter.value().toBool();
				if (set != Global::get().s.bMute) {
					Global::get().mw->qaAudioMute->setChecked(!set);
					Global::get().mw->qaAudioMute->trigger();
				}
			}
			iter = qmRequest.find(QLatin1String("unmute"));
			if (iter != qmRequest.constEnd()) {
				bool set = iter.value().toBool();
				if (set == Global::get().s.bMute) {
					Global::get().mw->qaAudioMute->setChecked(set);
					Global::get().mw->qaAudioMute->trigger();
				}
			}
			iter = qmRequest.find(QLatin1String("togglemute"));
			if (iter != qmRequest.constEnd()) {
				bool set = iter.value().toBool();
				if (set == Global::get().s.bMute) {
					Global::get().mw->qaAudioMute->setChecked(set);
					Global::get().mw->qaAudioMute->trigger();
				} else {
					Global::get().mw->qaAudioMute->setChecked(!set);
					Global::get().mw->qaAudioMute->trigger();
				}
			}
			iter = qmRequest.find(QLatin1String("deaf"));
			if (iter != qmRequest.constEnd()) {
				bool set = iter.value().toBool();
				if (set != Global::get().s.bDeaf) {
					Global::get().mw->qaAudioDeaf->setChecked(!set);
					Global::get().mw->qaAudioDeaf->trigger();
				}
			}
			iter = qmRequest.find(QLatin1String("undeaf"));
			if (iter != qmRequest.constEnd()) {
				bool set = iter.value().toBool();
				if (set == Global::get().s.bDeaf) {
					Global::get().mw->qaAudioDeaf->setChecked(set);
					Global::get().mw->qaAudioDeaf->trigger();
				}
			}
			iter = qmRequest.find(QLatin1String("toggledeaf"));
			if (iter != qmRequest.constEnd()) {
				bool set = iter.value().toBool();
				if (set == Global::get().s.bDeaf) {
					Global::get().mw->qaAudioDeaf->setChecked(set);
					Global::get().mw->qaAudioDeaf->trigger();
				} else {
					Global::get().mw->qaAudioDeaf->setChecked(!set);
					Global::get().mw->qaAudioDeaf->trigger();
				}
			}
			iter = qmRequest.find(QLatin1String("starttalking"));
			if (iter != qmRequest.constEnd()) {
				Global::get().mw->on_PushToTalk_triggered(true, QVariant());
			}
			iter = qmRequest.find(QLatin1String("stoptalking"));
			if (iter != qmRequest.constEnd()) {
				Global::get().mw->on_PushToTalk_triggered(false, QVariant());
			}
			ack = true;
		} else if (request.nodeName() == QLatin1String("url")) {
			if (Global::get().sh && Global::get().sh->isRunning() && Global::get().uiSession) {
				QString host, user, pw;
				unsigned short port;
				QUrl u;

				Global::get().sh->getConnectionInfo(host, port, user, pw);
				u.setScheme(QLatin1String("mumble"));
				u.setHost(host);
				u.setPort(port);
				u.setUserName(user);

				QUrlQuery query;
				query.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));
				u.setQuery(query);

				QStringList path;
				Channel *c = ClientUser::get(Global::get().uiSession)->cChannel;
				while (c->cParent) {
					path.prepend(c->qsName);
					c = c->cParent;
				}
				u.setPath(path.join(QLatin1String("/")));
				qmReply.insert(QLatin1String("href"), u);
			}

			iter = qmRequest.find(QLatin1String("href"));
			if (iter != qmRequest.constEnd()) {
				QUrl u = iter.value().toUrl();
				if (u.isValid() && u.scheme() == QLatin1String("mumble")) {
					OpenURLEvent *oue = new OpenURLEvent(u);
					qApp->postEvent(Global::get().mw, oue);
					ack = true;
				}
			} else {
				ack = true;
			}
		}

		QDomDocument replydoc;
		QDomElement reply = replydoc.createElement(QLatin1String("reply"));

		qmReply.insert(QLatin1String("succeeded"), ack);

		for (iter = qmReply.constBegin(); iter != qmReply.constEnd(); ++iter) {
			QDomElement elem = replydoc.createElement(iter.key());
			QDomText text    = replydoc.createTextNode(iter.value().toString());
			elem.appendChild(text);
			reply.appendChild(elem);
		}

		replydoc.appendChild(reply);

		qlsSocket->write(replydoc.toByteArray());
	}
}

SocketRPC::SocketRPC(const QString &basename, QObject *p) : QObject(p) {
	qlsServer = new QLocalServer(this);

	QString pipepath;

#ifdef Q_OS_WIN
	pipepath = basename;
#else
	{
		QString xdgRuntimePath = QProcessEnvironment::systemEnvironment().value(QLatin1String("XDG_RUNTIME_DIR"));
		QDir xdgRuntimeDir     = QDir(xdgRuntimePath);

		if (!xdgRuntimePath.isNull() && xdgRuntimeDir.exists()) {
			pipepath = xdgRuntimeDir.absoluteFilePath(basename + QLatin1String("Socket"));
		} else {
			pipepath = QDir::home().absoluteFilePath(QLatin1String(".") + basename + QLatin1String("Socket"));
		}
	}

	{
		QFile f(pipepath);
		if (f.exists()) {
			qWarning() << "SocketRPC: Removing old socket on" << pipepath;
			f.remove();
		}
	}
#endif

	if (!qlsServer->listen(pipepath)) {
		qWarning() << "SocketRPC: Listen failed";
		delete qlsServer;
		qlsServer = nullptr;
	} else {
		connect(qlsServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
	}
}

void SocketRPC::newConnection() {
	while (true) {
		QLocalSocket *qls = qlsServer->nextPendingConnection();
		if (!qls)
			break;
		new SocketRPCClient(qls, this);
	}
}

bool SocketRPC::send(const QString &basename, const QString &request, const QMap< QString, QVariant > &param) {
	QString pipepath;

#ifdef Q_OS_WIN
	pipepath = basename;
#else
	{
		QString xdgRuntimePath = QProcessEnvironment::systemEnvironment().value(QLatin1String("XDG_RUNTIME_DIR"));
		QDir xdgRuntimeDir     = QDir(xdgRuntimePath);

		if (!xdgRuntimePath.isNull() && xdgRuntimeDir.exists()) {
			pipepath = xdgRuntimeDir.absoluteFilePath(basename + QLatin1String("Socket"));
		} else {
			pipepath = QDir::home().absoluteFilePath(QLatin1String(".") + basename + QLatin1String("Socket"));
		}
	}
#endif

	QLocalSocket qls;
	qls.connectToServer(pipepath);
	if (!qls.waitForConnected(1000)) {
		return false;
	}

	QDomDocument requestdoc;
	QDomElement req = requestdoc.createElement(request);
	for (QMap< QString, QVariant >::const_iterator iter = param.constBegin(); iter != param.constEnd(); ++iter) {
		QDomElement elem = requestdoc.createElement(iter.key());
		QDomText text    = requestdoc.createTextNode(iter.value().toString());
		elem.appendChild(text);
		req.appendChild(elem);
	}
	requestdoc.appendChild(req);

	qls.write(requestdoc.toByteArray());
	qls.flush();

	if (!qls.waitForReadyRead(2000)) {
		return false;
	}

	QByteArray qba = qls.readAll();

	QDomDocument replydoc;
	replydoc.setContent(qba);

	QDomElement succ = replydoc.firstChildElement(QLatin1String("reply"));
	succ             = succ.firstChildElement(QLatin1String("succeeded"));
	if (succ.isNull())
		return false;

	return QVariant(succ.text()).toBool();
}
