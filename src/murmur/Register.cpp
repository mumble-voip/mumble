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

#include <QtXml>

#include "Server.h"
#include "Version.h"

void Server::initRegister() {
	http = NULL;
	qssReg = NULL;

	connect(&qtTick, SIGNAL(timeout()), this, SLOT(update()));

	if (! qsRegName.isEmpty()) {
		if ((! qsRegName.isEmpty()) && (! qsRegPassword.isEmpty()) && (qurlRegWeb.isValid()) && (qsPassword.isEmpty()))
			qtTick.start((60 + (qrand() % 120))* 1000);
		else
			log("Registration needs nonempty name, password and url, and the server must not be password protected.");
	} else {
		log("Not registering server as public");
	}
}

void Server::abort() {
	if (http) {
		http->setSocket(NULL);
		http->deleteLater();
		http = NULL;
	}
	if (qssReg) {
		qssReg->deleteLater();
		qssReg = NULL;
	}
}

void Server::update() {
	abort();
	qtTick.start(1000 * (60 * 60 + (qrand() % 300)));

	QDomDocument doc;
	QDomElement root=doc.createElement(QLatin1String("server"));
	doc.appendChild(root);

	QDomElement tag;
	QDomText t;

	tag=doc.createElement(QLatin1String("name"));
	root.appendChild(tag);

	t=doc.createTextNode(qsRegName);
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("host"));
	root.appendChild(tag);

	t=doc.createTextNode(qsRegHost);
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("password"));
	root.appendChild(tag);

	t=doc.createTextNode(qsRegPassword);
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("port"));
	root.appendChild(tag);

	t=doc.createTextNode(QString::number(usPort));
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("url"));
	root.appendChild(tag);

	t=doc.createTextNode(qurlRegWeb.toString());
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("digest"));
	root.appendChild(tag);

	t=doc.createTextNode(getDigest());
	tag.appendChild(t);
	
	tag=doc.createElement(QLatin1String("users"));
	root.appendChild(tag);

	t=doc.createTextNode(QString::number(qhUsers.count()));
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("channels"));
	root.appendChild(tag);

	t=doc.createTextNode(QString::number(qhChannels.count()));
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("version"));
	root.appendChild(tag);

	t=doc.createTextNode(QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)));
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("release"));
	root.appendChild(tag);

	t=doc.createTextNode(QLatin1String(MUMBLE_RELEASE));
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("os"));
	root.appendChild(tag);

#if defined(Q_WS_WIN)
	t=doc.createTextNode(QLatin1String("Win"));
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("osver"));
	root.appendChild(tag);

	t=doc.createTextNode(QString::number(QSysInfo::WindowsVersion));
	tag.appendChild(t);
#elif defined(Q_WS_MAC)
	t=doc.createTextNode(QLatin1String("OSX"));
	tag.appendChild(t);

	tag=doc.createElement(QLatin1String("osver"));
	root.appendChild(tag);

	t=doc.createTextNode(QString::number(QSysInfo::MacintoshVersion));
	tag.appendChild(t);
#else
	t=doc.createTextNode(QLatin1String("X11"));
	tag.appendChild(t);
#endif	

	tag=doc.createElement(QLatin1String("qt"));
	root.appendChild(tag);

	t=doc.createTextNode(qVersion());
	tag.appendChild(t);

	qssReg = new QSslSocket(this);
	qssReg->setLocalCertificate(qscCert);
	qssReg->setPrivateKey(qskKey);

	http = new QHttp(QLatin1String("mumble.hive.no"), QHttp::ConnectionModeHttps, 443, this);
	http->setSocket(qssReg);

	connect(http, SIGNAL(done(bool)), this, SLOT(done(bool)));
	connect(http, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(regSslError(const QList<QSslError> &)));

	QHttpRequestHeader h(QLatin1String("POST"), QLatin1String("/register.cgi"));
	h.setValue(QLatin1String("Connection"), QLatin1String("Keep-Alive"));
	h.setValue(QLatin1String("Host"), QLatin1String("mumble.hive.no"));
	h.setContentType(QLatin1String("text/xml"));
	http->request(h, doc.toString().toUtf8());
}

void Server::done(bool err) {
	if (! http || ! qssReg)
		return;
	if (err) {
		log("Registration failed: %s", qPrintable(http->errorString()));
	} else {
		QByteArray qba = http->readAll();
		log("Registration: %s", qPrintable(QString(QLatin1String(qba))));
	}
	abort();
}

void Server::regSslError(const QList<QSslError> &errs) {
	foreach(const QSslError &e, errs)
	log("Registration: SSL Handshake error: %s", qPrintable(e.errorString()));
}
