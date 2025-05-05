// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HTMLFilter.h"

#include <QtCore/QXmlStreamReader>

QString HTMLFilter::escapeTags(const QString &in) {
	QString out;
	for (int i = 0; i < in.size(); i++) {
		if (in.at(i) == QLatin1Char('<')) {
			out += QLatin1String("&lt;");
		} else if (in.at(i) == QLatin1Char('>')) {
			out += QLatin1String("&gt;");
		} else {
			out += in.at(i);
		}
	}
	return out;
}

bool HTMLFilter::filter(const QString &in, QString &out) {
	if (!in.contains(QLatin1Char('<'))) {
		out = in.simplified();
	} else {
		QXmlStreamReader qxsr(QString::fromLatin1("<document>%1</document>").arg(in));
		QString qs;
		while (!qxsr.atEnd()) {
			switch (qxsr.readNext()) {
				case QXmlStreamReader::Invalid:
					return false;
				case QXmlStreamReader::Characters:
					qs += qxsr.text();
					break;
				case QXmlStreamReader::EndElement:
					if ((qxsr.name() == QLatin1String("br")) || (qxsr.name() == QLatin1String("p")))
						qs += QLatin1Char('\n');
					break;
				default:
					break;
			}
		}
		out = escapeTags(qs.simplified());
	}
	return true;
}
