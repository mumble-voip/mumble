// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "BanEditor.h"

#include "Channel.h"
#include "Ban.h"
#include "ServerHandler.h"

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

BanEditor::BanEditor(const MumbleProto::BanList &msg, QWidget *p) : QDialog(p)
	, maskDefaultValue(32) {
	setupUi(this);
	qlwBans->setFocus();

	qlBans.clear();
	for (int i=0;i < msg.bans_size(); ++i) {
		const MumbleProto::BanList_BanEntry &be = msg.bans(i);
		Ban b;
		b.haAddress = be.address();
		b.iMask = be.mask();
		b.qsUsername = u8(be.name());
		b.qsHash = u8(be.hash());
		b.qsReason = u8(be.reason());
		b.qdtStart = QDateTime::fromString(u8(be.start()), Qt::ISODate);
		b.qdtStart.setTimeSpec(Qt::UTC);
		if (! b.qdtStart.isValid())
			b.qdtStart = QDateTime::currentDateTime();
		b.iDuration = be.duration();
		if (b.isValid())
			qlBans << b;
	}

	refreshBanList();
}

void BanEditor::accept() {
	MumbleProto::BanList msg;

	foreach(const Ban &b, qlBans) {
		MumbleProto::BanList_BanEntry *be = msg.add_bans();
		be->set_address(b.haAddress.toStdString());
		be->set_mask(b.iMask);
		be->set_name(u8(b.qsUsername));
		be->set_hash(u8(b.qsHash));
		be->set_reason(u8(b.qsReason));
		be->set_start(u8(b.qdtStart.toString(Qt::ISODate)));
		be->set_duration(b.iDuration);
	}

	g.sh->sendMessage(msg);
	QDialog::accept();
}

void BanEditor::on_qlwBans_currentRowChanged() {
	int idx = qlwBans->currentRow();
	if (idx < 0)
		return;

	qpbAdd->setDisabled(true);
	qpbUpdate->setEnabled(false);
	qpbRemove->setEnabled(true);

	const Ban &ban = qlBans.at(idx);

	int maskbits = ban.iMask;

	const QHostAddress &addr = ban.haAddress.toAddress();
	qleIP->setText(addr.toString());
	if (! ban.haAddress.isV6())
		maskbits -= 96;
	qsbMask->setValue(maskbits);
	qleUser->setText(ban.qsUsername);
	qleHash->setText(ban.qsHash);
	qleReason->setText(ban.qsReason);
	qdteStart->setDateTime(ban.qdtStart.toLocalTime());
	qdteEnd->setDateTime(ban.qdtStart.toLocalTime().addSecs(ban.iDuration));

}

Ban BanEditor::toBan(bool &ok) {
	Ban b;

	QHostAddress addr;

	ok = addr.setAddress(qleIP->text());

	if (ok) {
		b.haAddress = addr;
		b.iMask = qsbMask->value();
		if (! b.haAddress.isV6())
			b.iMask += 96;
		b.qsUsername = qleUser->text();
		b.qsHash = qleHash->text();
		b.qsReason = qleReason->text();
		b.qdtStart = qdteStart->dateTime().toUTC();
		const QDateTime &qdte = qdteEnd->dateTime();
		
		if (qdte <= b.qdtStart)
			b.iDuration = 0;
		else
			b.iDuration = static_cast<unsigned int>(b.qdtStart.secsTo(qdte));

		ok = b.isValid();
	}
	return b;
}

void BanEditor::on_qpbAdd_clicked() {
	bool ok;

	qdteStart->setDateTime(QDateTime::currentDateTime());

	Ban b = toBan(ok);

	if (ok) {
		qlBans << b;
		refreshBanList();
		qlwBans->setCurrentRow(qlBans.indexOf(b));
	}

	qlwBans->setCurrentRow(-1);
	qleSearch->clear();
}

void BanEditor::on_qpbUpdate_clicked() {
	int idx = qlwBans->currentRow();
	if (idx >= 0) {
		bool ok;
		Ban b = toBan(ok);
		if (ok) {
			qlBans.replace(idx, b);
			refreshBanList();
			qlwBans->setCurrentRow(qlBans.indexOf(b));
		}
	}
}

void BanEditor::on_qpbRemove_clicked() {
	int idx = qlwBans->currentRow();
	if (idx >= 0)
		qlBans.removeAt(idx);
	refreshBanList();

	qlwBans->setCurrentRow(-1);
	qleUser->clear();
	qleIP->clear();
	qleReason->clear();
	qsbMask->setValue(maskDefaultValue);
	qleHash->clear();

	qdteStart->setDateTime(QDateTime::currentDateTime());
	qdteEnd->setDateTime(QDateTime::currentDateTime());

	qpbRemove->setDisabled(true);
	qpbAdd->setDisabled(true);
}

void BanEditor::refreshBanList() {
	qlwBans->clear();

	std::sort(qlBans.begin(), qlBans.end());

	foreach(const Ban &ban, qlBans) {
		const QHostAddress &addr=ban.haAddress.toAddress();
		if (ban.qsUsername.isEmpty())
			qlwBans->addItem(addr.toString());
		else
			qlwBans->addItem(ban.qsUsername);
	}

	int n = qlBans.count();
	setWindowTitle(tr("Ban List - %n Ban(s)", "", n));
}

void BanEditor::on_qleSearch_textChanged(const QString & match)
{
	qlwBans->clearSelection();

	qpbAdd->setDisabled(true);
	qpbUpdate->setDisabled(true);
	qpbRemove->setDisabled(true);

	qleUser->clear();
	qleIP->clear();
	qleReason->clear();
	qsbMask->setValue(maskDefaultValue);
	qleHash->clear();

	qdteStart->setDateTime(QDateTime::currentDateTime());
	qdteEnd->setDateTime(QDateTime::currentDateTime());

	foreach(QListWidgetItem *item, qlwBans->findItems(QString(), Qt::MatchContains)) {
		if (!item->text().contains(match, Qt::CaseInsensitive))
			item->setHidden(true);
		else
			item->setHidden(false);
	}
}

void BanEditor::on_qleIP_textChanged(QString )
{
	qpbAdd->setEnabled(qleIP->isModified());
	if (qlwBans->currentRow() >= 0)
		qpbUpdate->setEnabled(qleIP->isModified());
}

void BanEditor::on_qleReason_textChanged(QString )
{
	if (qlwBans->currentRow() >= 0)
		qpbUpdate->setEnabled(qleReason->isModified());
}

void BanEditor::on_qdteEnd_editingFinished()
{
	qpbUpdate->setEnabled(!qleIP->text().isEmpty());
	qpbRemove->setDisabled(true);
}

void BanEditor::on_qleUser_textChanged(QString )
{
	if (qlwBans->currentRow() >= 0)
		qpbUpdate->setEnabled(qleUser->isModified());
}

void BanEditor::on_qleHash_textChanged(QString )
{
	if (qlwBans->currentRow() >= 0)
		qpbUpdate->setEnabled(qleHash->isModified());
}

void BanEditor::on_qpbClear_clicked()
{
	qlwBans->setCurrentRow(-1);
	qleUser->clear();
	qleIP->clear();
	qleReason->clear();
	qsbMask->setValue(maskDefaultValue);
	qleHash->clear();

	qdteStart->setDateTime(QDateTime::currentDateTime());
	qdteEnd->setDateTime(QDateTime::currentDateTime());

	qpbAdd->setDisabled(true);
	qpbUpdate->setDisabled(true);
	qpbRemove->setDisabled(true);
}
