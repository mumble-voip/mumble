/* copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "BanEditor.h"
#include "ServerHandler.h"
#include "Channel.h"
#include "Global.h"

BanEditor::BanEditor(const MessageServerBanList *msbl, QWidget *p) : QDialog(p) {
	setupUi(this);

	QRegExp rx(QLatin1String("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}"));
	QValidator *validator = new QRegExpValidator(rx, this);
	qleIP->setValidator(validator);

	qlBans = msbl->qlBans;
	refreshBanList();

}

void BanEditor::accept() {
	MessageServerBanList msbl;

	msbl.bQuery = false;
	msbl.qlBans = qlBans;

	g.sh->sendMessage(&msbl);
	QDialog::accept();
}

void BanEditor::on_qlwBans_currentRowChanged() {
	QPair<quint32, int> ban;
	int idx = qlwBans->currentRow();
	if (idx < 0)
		return;
	ban = qlBans[idx];

	QHostAddress addr(ban.first);
	qleIP->setText(addr.toString());
	qsbMask->setValue(ban.second);
}

void BanEditor::on_qpbAdd_clicked() {
	QPair<quint32, int> ban;
	QHostAddress addr;

	bool ok;

	ok = addr.setAddress(qleIP->text());
	if (ok) {
		quint32 base = addr.toIPv4Address();
		quint32 nmask = qsbMask->value();
		quint32 bitmask = (1<<(32-nmask))-1;
		ban.first = base & ~bitmask;
		ban.second = nmask;
		qlBans << ban;
		refreshBanList();
		qlwBans->setCurrentRow(qlBans.indexOf(ban));
	}
}

void BanEditor::on_qpbUpdate_clicked() {
	on_qpbRemove_clicked();
	on_qpbAdd_clicked();
}

void BanEditor::on_qpbRemove_clicked() {
	int idx = qlwBans->currentRow();
	if (idx >= 0)
		qlBans.removeAt(idx);
	refreshBanList();
}

void BanEditor::refreshBanList() {
	QPair<quint32, int> ban;

	qlwBans->clear();

	qSort(qlBans);

	foreach(ban,qlBans) {
		QHostAddress addr(ban.first);
		QString qs = QString::fromLatin1("%1/%2").arg(addr.toString()).arg(ban.second);
		qlwBans->addItem(qs);
	}
}
