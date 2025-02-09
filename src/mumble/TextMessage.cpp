// Copyright The Mumble Developers. All rights reserved.
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

QString TextMessage::message() {
	return rteMessage->text();
}
