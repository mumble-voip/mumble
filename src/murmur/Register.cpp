// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Meta.h"
#include "OSInfo.h"
#include "Server.h"
#include "Version.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QDomDocument>

#include <QRandomGenerator>

void Server::initRegister() {
	connect(&qtTick, SIGNAL(timeout()), this, SLOT(update()));

	if (!qsRegName.isEmpty()) {
		if (!qsRegName.isEmpty() && !qsRegPassword.isEmpty() && qurlRegWeb.isValid() && qsPassword.isEmpty()
			&& bAllowPing)
			qtTick.start((60 + (static_cast< int >(QRandomGenerator::global()->generate()) % 120)) * 1000);
		else
			log("Registration needs nonempty 'registername', 'registerpassword' and 'registerurl', must have an empty "
				"'password' and allowed pings.");
	} else {
		log("Not registering server as public");
	}
}

void Server::update() {
	if (qsRegName.isEmpty() || qsRegPassword.isEmpty() || !qurlRegWeb.isValid() || !qsPassword.isEmpty() || !bAllowPing)
		return;

	// When QNAM distinguishes connections by client cert, move this to Meta
	if (!qnamNetwork)
		qnamNetwork = new QNetworkAccessManager(this);

	qtTick.start(1000 * (60 * 60 + (static_cast< int >(QRandomGenerator::global()->generate()) % 300)));

	QDomDocument doc;
	QDomElement root = doc.createElement(QLatin1String("server"));
	doc.appendChild(root);

	OSInfo::fillXml(doc, root, qlBind);

	QDomElement tag;
	QDomText t;

	tag = doc.createElement(QLatin1String("name"));
	root.appendChild(tag);
	t = doc.createTextNode(qsRegName);
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("host"));
	root.appendChild(tag);
	t = doc.createTextNode(qsRegHost);
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("password"));
	root.appendChild(tag);
	t = doc.createTextNode(qsRegPassword);
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("port"));
	root.appendChild(tag);
	t = doc.createTextNode(QString::number(usPort));
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("url"));
	root.appendChild(tag);
	t = doc.createTextNode(qurlRegWeb.toString());
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("digest"));
	root.appendChild(tag);
	t = doc.createTextNode(getDigest());
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("users"));
	root.appendChild(tag);
	t = doc.createTextNode(QString::number(qhUsers.count()));
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("channels"));
	root.appendChild(tag);
	t = doc.createTextNode(QString::number(qhChannels.count()));
	tag.appendChild(t);

	if (!qsRegLocation.isEmpty()) {
		tag = doc.createElement(QLatin1String("location"));
		root.appendChild(tag);
		t = doc.createTextNode(qsRegLocation);
		tag.appendChild(t);
	}

	QNetworkRequest qnr(QUrl(QLatin1String("https://publist-registration.mumble.info/v1/register")));
	qnr.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("text/xml"));

	QSslConfiguration ssl = qnr.sslConfiguration();
	ssl.setLocalCertificate(qscCert);
	ssl.setPrivateKey(qskKey);

	/* Work around bug in QSslConfiguration */
	QList< QSslCertificate > calist = ssl.caCertificates();
	calist << QSslConfiguration::defaultConfiguration().caCertificates();
	calist << Meta::mp->qlCA;
	calist << Meta::mp->qlIntermediates;
	calist << qscCert;
	ssl.setCaCertificates(calist);

	ssl.setCiphers(Meta::mp->qlCiphers);

	qnr.setSslConfiguration(ssl);

	QNetworkReply *rep = qnamNetwork->post(qnr, doc.toString().toUtf8());
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));
	connect(rep, SIGNAL(sslErrors(const QList< QSslError > &)), this, SLOT(regSslError(const QList< QSslError > &)));
}

void Server::finished() {
	QNetworkReply *rep = qobject_cast< QNetworkReply * >(sender());

	if (rep->error() != QNetworkReply::NoError) {
		log(QString("Registration failed: %1").arg(rep->errorString()));
	} else {
		QByteArray qba = rep->readAll();
		log(QString("Registration: %1").arg(QLatin1String(qba)));
	}
	rep->deleteLater();
}

void Server::regSslError(const QList< QSslError > &errs) {
	foreach (const QSslError &e, errs)
		log(QString("Registration: SSL Handshake error: %1").arg(e.errorString()));
}
