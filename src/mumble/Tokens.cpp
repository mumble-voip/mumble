/* copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

