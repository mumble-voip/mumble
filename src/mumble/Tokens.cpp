// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "Tokens.h"

#include "Database.h"
#include "Global.h"
#include "ServerHandler.h"

Tokens::Tokens(QWidget *p) : QDialog(p) {
	setupUi(this);

	qbaDigest = g.sh->qbaDigest;
	QStringList tokens = Database::getTokens(qbaDigest);
	tokens.sort();
	foreach(const QString &qs, tokens) {
		QListWidgetItem *qlwi = new QListWidgetItem(qs);
		qlwi->setFlags(qlwi->flags() | Qt::ItemIsEditable);
		qlwTokens->addItem(qlwi);
	}
}

void Tokens::accept() {
	QStringList qsl;

	QList<QListWidgetItem *> items = qlwTokens->findItems(QString(), Qt::MatchStartsWith);
	foreach(QListWidgetItem *qlwi, items) {
		const QString &text = qlwi->text().trimmed();
		if (! text.isEmpty())
			qsl << text;
	}
	Database::setTokens(qbaDigest, qsl);
	g.sh->setTokens(qsl);
	QDialog::accept();
}

void Tokens::on_qpbAdd_clicked() {
	QListWidgetItem *qlwi = new QListWidgetItem(tr("Empty Token"));
	qlwi->setFlags(qlwi->flags() | Qt::ItemIsEditable);

	qlwTokens->addItem(qlwi);
	qlwTokens->editItem(qlwi);
}

void Tokens::on_qpbRemove_clicked() {
	foreach(QListWidgetItem *qlwi, qlwTokens->selectedItems())
		delete qlwi;
}

