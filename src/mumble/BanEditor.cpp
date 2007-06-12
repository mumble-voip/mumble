/* copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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
	setWindowTitle(tr("Mumble - Edit Bans"));

	qlwBans = new QListWidget();
	qleIP = new QLineEdit(tr("0.0.0.0"));
	qsbMask = new QSpinBox();
	qpbAdd = new QPushButton(tr("&Add"));
	qpbUpdate = new QPushButton(tr("&Update"));
	qpbRemove = new QPushButton(tr("&Remove"));

	qlwBans->setObjectName(QString::fromAscii("Bans"));

	qleIP->setObjectName(QString::fromAscii("IP"));
	QRegExp rx(QString::fromAscii("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}"));
    QValidator *validator = new QRegExpValidator(rx, this);
    qleIP->setValidator(validator);

	qsbMask->setObjectName(QString::fromAscii("Mask"));
	qsbMask->setRange(8,32);
	qsbMask->setValue(24);
	qpbAdd->setObjectName(QString::fromAscii("Add"));
	qpbUpdate->setObjectName(QString::fromAscii("Update"));
	qpbRemove->setObjectName(QString::fromAscii("Remove"));

	QGridLayout *grid = new QGridLayout;

	grid->addWidget(qlwBans,0,0,2,1);
	grid->addWidget(qleIP,0,1,1,2);
	grid->addWidget(qsbMask,0,3);
	grid->addWidget(qpbAdd,1,1);
	grid->addWidget(qpbUpdate,1,2);
	grid->addWidget(qpbRemove,1,3);

    QPushButton *okButton = new QPushButton(tr("&OK"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    okButton->setToolTip(tr("Accept changes"));
    okButton->setWhatsThis(tr("This button will accept current groups/ACLs and send them to "
    						"the server. Note that if you mistakenly remove write permission "
    						"from yourself, the server will add it."));
    QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	cancelButton->setToolTip(tr("Reject changes"));
	cancelButton->setWhatsThis(tr("This button will cancels all changes and closes the dialog without "
								  "updating the ACLs or groups on the server."));

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addStretch(1);
    buttons->addWidget(okButton);
    buttons->addWidget(cancelButton);

    QVBoxLayout *ml = new QVBoxLayout;
    ml->addLayout(grid);
    ml->addSpacing(12);
    ml->addLayout(buttons);
    setLayout(ml);

	qlBans = msbl->qlBans;
	refreshBanList();

    QMetaObject::connectSlotsByName(this);
	addToolTipsWhatsThis();
}

void BanEditor::addToolTipsWhatsThis() {
}

void BanEditor::accept() {
	MessageServerBanList msbl;

	msbl.bQuery = false;
	msbl.qlBans = qlBans;

	g.sh->sendMessage(&msbl);
	QDialog::accept();
}

void BanEditor::on_Bans_currentRowChanged() {
	QPair<quint32, int> ban;
	int idx = qlwBans->currentRow();
	if (idx < 0)
		return;
	ban = qlBans[idx];

	QHostAddress addr(ban.first);
	qleIP->setText(addr.toString());
	qsbMask->setValue(ban.second);
}

void BanEditor::on_Add_clicked() {
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

void BanEditor::on_Update_clicked() {
	on_Remove_clicked();
	on_Add_clicked();
}

void BanEditor::on_Remove_clicked() {
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
		QString qs = QString::fromAscii("%1/%2").arg(addr.toString()).arg(ban.second);
		qlwBans->addItem(qs);
	}
}
