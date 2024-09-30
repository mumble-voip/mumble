// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Tokens.h"

#include "Database.h"
#include "ServerHandler.h"
#include "Global.h"

Tokens::Tokens(QWidget *p) : QDialog(p) {
	setupUi(this);

	qbaDigest          = Global::get().sh->qbaDigest;
	QStringList tokens = Global::get().db->getTokens(qbaDigest);
	tokens.sort();
	foreach (const QString &qs, tokens) {
		QListWidgetItem *qlwi = new QListWidgetItem(qs);
		qlwi->setFlags(qlwi->flags() | Qt::ItemIsEditable);
		qlwTokens->addItem(qlwi);
	}
}

void Tokens::accept() {
	QStringList qsl;

	QList< QListWidgetItem * > items = qlwTokens->findItems(QString(), Qt::MatchStartsWith);
	foreach (QListWidgetItem *qlwi, items) {
		const QString &text = qlwi->text().trimmed();
		if (!text.isEmpty())
			qsl << text;
	}
	Global::get().db->setTokens(qbaDigest, qsl);
	Global::get().sh->setTokens(qsl);
	QDialog::accept();
}

void Tokens::on_qpbAdd_clicked() {
	QListWidgetItem *qlwi = new QListWidgetItem(tr("Empty Token"));
	qlwi->setFlags(qlwi->flags() | Qt::ItemIsEditable);

	qlwTokens->addItem(qlwi);
	qlwTokens->editItem(qlwi);
}

void Tokens::on_qpbRemove_clicked() {
	foreach (QListWidgetItem *qlwi, qlwTokens->selectedItems())
		delete qlwi;
}
