// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TextMessage.h"

TextMessage::TextMessage(QWidget *p, QString title, bool bChannel) : QDialog(p) {
	setupUi(this);
	if (!bChannel)
		qcbTreeMessage->setHidden(true);
	setWindowTitle(title);
	bTreeMessage = false;

	QObject::connect(rteMessage, SIGNAL(accept()), this, SLOT(accept()));
}

void TextMessage::on_qcbTreeMessage_stateChanged(int s) {
	bTreeMessage = s == Qt::Checked ? true : false;
}

QString TextMessage::autoFormat(QString qsPlain) {
	QRegExp qr;
	qr.setMinimal(true);
	qr.setPatternSyntax(QRegExp::RegExp2);
	qr.setCaseSensitivity(Qt::CaseInsensitive);

	qr.setPattern(QLatin1String("[\\r\\n]+"));
	qsPlain.replace(qr, QLatin1String("<br />"));

	qr.setPattern(QLatin1String("\\*(\\w+)\\*"));
	qsPlain.replace(qr, QLatin1String("<b>\\1</b>"));

	qr.setPattern(QLatin1String("\"([^\"]+)\""));
	qsPlain.replace(qr, QLatin1String("\"<i>\\1</i>\""));

	qr.setPattern(QLatin1String("[a-z-]+://[^ <]*"));
	qr.setMinimal(false);

	int idx = 0;
	do {
		idx = qr.indexIn(qsPlain, idx);
		if (idx >= 0) {
			QString url = qr.capturedTexts().at(0);
			QUrl u(url);
			if (u.isValid()) {
				int len = qr.matchedLength();
				QString replacement = QString::fromLatin1("<a href=\"%1\">%1</a>").arg(url);
				qsPlain.replace(idx, len, replacement);
				idx += replacement.length();
			} else {
				idx++;
			}
		}
	} while (idx >= 0);
	return qsPlain;
}

QString TextMessage::message() {
	return rteMessage->text();
}
