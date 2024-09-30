// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Usage.h"

#include "ClientUser.h"
#include "LCD.h"
#include "NetworkConfig.h"
#include "OSInfo.h"
#include "Version.h"
#include "Global.h"

#include <QtCore/QTimer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QDomElement>

Usage::Usage(QObject *p) : QObject(p) {
	qbReport.open(QBuffer::ReadWrite);
	qdsReport.setDevice(&qbReport);
	qdsReport.setVersion(QDataStream::Qt_4_4);
	qdsReport << static_cast< unsigned int >(2);

	// Wait 10 minutes (so we know they're actually using this), then...
	QTimer::singleShot(60 * 10 * 1000, this, SLOT(registerUsage()));
}

void Usage::registerUsage() {
	if (!Global::get().s.bUsage
		|| Version::get() < Version::fromComponents(
			   1, 2, 3)) // Only register usage if allowed by the user and first wizard run has finished
		return;

	QDomDocument doc;
	QDomElement root = doc.createElement(QLatin1String("usage"));
	doc.appendChild(root);

	QDomElement tag;
	QDomText t;

	OSInfo::fillXml(doc, root);

	tag = doc.createElement(QLatin1String("in"));
	root.appendChild(tag);
	t = doc.createTextNode(Global::get().s.qsAudioInput);
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("out"));
	root.appendChild(tag);
	t = doc.createTextNode(Global::get().s.qsAudioOutput);
	tag.appendChild(t);

	tag = doc.createElement(QLatin1String("lcd"));
	root.appendChild(tag);
	t = doc.createTextNode(QString::number(Global::get().lcd->hasDevices() ? 1 : 0));
	tag.appendChild(t);

	QBuffer *qb = new QBuffer();
	qb->setData(doc.toString().toUtf8());
	qb->open(QIODevice::ReadOnly);

	QNetworkRequest req(QUrl(QLatin1String("https://usage-report.mumble.info/v1/report")));
	Network::prepareRequest(req);
	req.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("text/xml"));

	QNetworkReply *rep = Global::get().nam->post(req, qb);
	qb->setParent(rep);

	connect(rep, SIGNAL(finished()), rep, SLOT(deleteLater()));
}
