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

#include "Server.h"
#include "Meta.h"
#include "Version.h"
#include "OSInfo.h"

void Server::initRegister() {
	connect(&qtTick, SIGNAL(timeout()), this, SLOT(update()));

	if (! qsRegName.isEmpty()) {
		if ((! qsRegName.isEmpty()) && (! qsRegPassword.isEmpty()) && qurlRegWeb.isValid() && qsPassword.isEmpty() && bAllowPing)
			qtTick.start((60 + (qrand() % 120))* 1000);
		else
			log("Registration needs nonempty 'registername', 'registerpassword' and 'registerurl', must have an empty 'password' and allowed pings.");
	} else {
		log("Not registering server as public");
	}
}

void Server::update() {
	if (!((! qsRegName.isEmpty()) && (! qsRegName.isEmpty()) && (! qsRegPassword.isEmpty()) && qurlRegWeb.isValid() && qsPassword.isEmpty() && bAllowPing))
		return;

	// When QNAM distinguishes connections by client cert, move this to Meta
	if (! qnamNetwork)
		qnamNetwork = new QNetworkAccessManager(this);

	qtTick.start(1000 * (60 * 60 + (qrand() % 300)));

	QDomDocument doc;
	QDomElement root=doc.createElement(QLatin1String("server"));
	doc.appendChild(root);

	OSInfo::fillXml(doc, root, meta->qsOS, meta->qsOSVersion, qlBind);

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

	if (!qsRegLocation.isEmpty()) {
		tag=doc.createElement(QLatin1String("location"));
		root.appendChild(tag);
		t=doc.createTextNode(qsRegLocation);
		tag.appendChild(t);
	}

	QNetworkRequest qnr(QUrl(QLatin1String("https://mumble.hive.no/register.cgi")));
	qnr.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("text/xml"));

	QSslConfiguration ssl = qnr.sslConfiguration();
	ssl.setLocalCertificate(qscCert);
	ssl.setPrivateKey(qskKey);

	/* Work around bug in QSslConfiguration */
	QList<QSslCertificate> calist = ssl.caCertificates();
	calist << QSslSocket::defaultCaCertificates();
	calist << qscCert;
	ssl.setCaCertificates(calist);

	qnr.setSslConfiguration(ssl);

	QNetworkReply *rep = qnamNetwork->post(qnr, doc.toString().toUtf8());
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));
	connect(rep, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(regSslError(const QList<QSslError> &)));
}

void Server::finished() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());

	if (rep->error() != QNetworkReply::NoError) {
		log(QString("Registration failed: %1").arg(rep->errorString()));
	} else {
		QByteArray qba = rep->readAll();
		log(QString("Registration: %1").arg(QLatin1String(qba)));
	}
	rep->deleteLater();
}

void Server::regSslError(const QList<QSslError> &errs) {
	foreach(const QSslError &e, errs)
		log(QString("Registration: SSL Handshake error: %1").arg(e.errorString()));
}
