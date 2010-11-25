/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "TextMessage.h"

TextMessage::TextMessage(QWidget *p, QString title, bool bChannel) : QDialog(p) {
	setupUi(this);
	if (!bChannel)
		qcbTreeMessage->setHidden(true);
	setWindowTitle(title);
	rteMessage->installEventFilter(this);
	bTreeMessage = false;
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

	qr.setPattern(QLatin1String("[a-z-]+://[^ <$]*"));
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

bool TextMessage::eventFilter(QObject *obj, QEvent *evt) {
	if (obj != rteMessage)
		return false;
	if (evt->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
		if (((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return)) &&
		        (keyEvent->modifiers() == Qt::NoModifier)) {
			accept();
			return true;
		}
	}
	return false;
}

