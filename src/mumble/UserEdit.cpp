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

#include "UserEdit.h"
#include "Channel.h"
#include "Global.h"
#include "ServerHandler.h"
#include "User.h"

UserEdit::UserEdit(const MumbleProto::UserList &msg, QWidget *p) : QDialog(p)
	, iInactiveForDaysFiltervalue(0) {
	setupUi(this);

	qtwUserList->setFocus();
	qtwUserList->setContextMenuPolicy(Qt::CustomContextMenu);

	int n = msg.users_size();
	setWindowTitle(tr("Registered Users: %n Account(s)", "", n));

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	qtwUserList->header()->setSectionResizeMode(0, QHeaderView::Stretch); // user name
	qtwUserList->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // last seen
	qtwUserList->header()->setSectionResizeMode(2, QHeaderView::Stretch); // on channel
#else
	qtwUserList->header()->setResizeMode(0, QHeaderView::Stretch); // user name
	qtwUserList->header()->setResizeMode(1, QHeaderView::ResizeToContents); // last seen
	qtwUserList->header()->setResizeMode(2, QHeaderView::Stretch); // on channel
#endif
	qtwUserList->sortByColumn(0, Qt::AscendingOrder); // sort by user name
	qmUsers.clear();

	for (int i = 0; i < msg.users_size(); ++i) {
		const MumbleProto::UserList_User &u = msg.users(i);
		UserInfo uie;
		protoUserToUserInfo(u, uie);
		qmUsers.insert(uie.user_id, uie);
	}
	refreshUserList();
}

void UserEdit::protoUserToUserInfo(const MumbleProto::UserList_User & u, UserInfo & uie)
{
	uie.user_id = u.user_id();
	uie.name = u8(u.name());
	if (u.has_last_channel()) {
		uie.last_channel = u.last_channel();
	}
	if (u.has_last_seen()) {
		uie.last_active = QDateTime::fromString(u8(u.last_seen()), Qt::ISODate);
		uie.last_active.setTimeSpec(Qt::UTC);
	}
}

void UserEdit::refreshUserList() {
	qtwUserList->clearSelection();
	qtwUserList->clear();

	QMapIterator<int, UserInfo> i(qmUsers);
	while (i.hasNext()) {
		i.next();
		UserEditListItem *ueli = new UserEditListItem(i.key());

		const QString username = i.value().name;
		ueli->setText(0, username);

		QString qsLastActive;
		QDateTime qdtLastActive(i.value().last_active);
		int iSeenDaysAgo = 0;
		if (qdtLastActive.isValid()) {
			iSeenDaysAgo = qdtLastActive.daysTo(QDateTime::currentDateTime().toUTC());
			qsLastActive = tr("%1").arg(QString::number(iSeenDaysAgo));
		}
		ueli->setText(1, qsLastActive);
		ueli->setToolTip(1, qdtLastActive.toLocalTime().toString(Qt::ISODate));

		boost::optional<int> lastchanid = i.value().last_channel;
		Channel *c = lastchanid ? Channel::get(*lastchanid) : NULL;
		QString lastchantreestring = getChanneltreestring(c);
		ueli->setText(2, lastchantreestring);

		if (lastchanid) {
			showExtendedGUI();
		} else {
			hideExtendedGUI();
		}

		bool bHidden = false;
		bHidden = bHidden || ((iInactiveForDaysFiltervalue > 0) && (iSeenDaysAgo < iInactiveForDaysFiltervalue));
		bHidden = bHidden || (!username.contains(qlSearch->text(), Qt::CaseInsensitive)
				&& !lastchantreestring.contains(qlSearch->text(), Qt::CaseInsensitive));
		if (bHidden) {
			delete ueli;
			continue;
		}

		qtwUserList->addTopLevelItem(ueli);
	}
}

QString UserEdit::getChanneltreestring(Channel* c) const
{
	QString tree = QLatin1String("-");
	if (c) {
		QStringList channel_tree;
		while (c->cParent != NULL) {
			channel_tree.prepend(c->qsName);
			c = c->cParent;
		}

		//TODO: This seems unnecessary
		QStringList _channel_tree;
		for (QStringList::iterator it = channel_tree.begin(); it != channel_tree.end(); ++it) {
			_channel_tree.append(*it);
		}

		channel_tree.clear();
		channel_tree.append(_channel_tree);

		tree = QLatin1String("/ ") + channel_tree.join(QLatin1String(" / "));
	}
	return tree;
}

void UserEdit::showExtendedGUI()
{
	qtwUserList->showColumn(1);
	qtwUserList->showColumn(2);
	qlInactive->show();
	qsbInactive->show();
	qcbInactive->show();
}

void UserEdit::hideExtendedGUI()
{
	qtwUserList->hideColumn(1);
	qtwUserList->hideColumn(2);
	qlInactive->hide();
	qsbInactive->hide();
	qcbInactive->hide();
}

void UserEdit::accept() {
	QList<QTreeWidgetItem *> ql = qtwUserList->findItems(QString(), Qt::MatchStartsWith);
	foreach(QTreeWidgetItem * qlwi, ql) {
		const QString &name = qlwi->text(0);
		int id = qlwi->data(0, Qt::UserRole).toInt();
		if (qmUsers.value(id).name != name) {
			qmChanged.insert(id, name);
		}
	}

	if (! qmChanged.isEmpty()) {
		MumbleProto::UserList mpul;
		QMap<int, QString>::const_iterator i;
		for (i = qmChanged.constBegin(); i != qmChanged.constEnd(); ++i) {
			MumbleProto::UserList_User *u = mpul.add_users();
			u->set_user_id(i.key());
			if (! i.value().isEmpty()) {
				u->set_name(u8(i.value()));
			}
		}
		g.sh->sendMessage(mpul);
	}

	QDialog::accept();
}

void UserEdit::on_qpbRename_clicked() {
	int idx = qtwUserList->currentIndex().row();
	if (idx >= 0) {
		QTreeWidgetItem *item = qtwUserList->currentItem();
		if (item) {
			qtwUserList->editItem(item);
		}
	}
}

void UserEdit::on_qpbRemove_clicked() {
	while (qtwUserList->selectedItems().count() > 0) {
		QTreeWidgetItem *qlwi = qtwUserList->selectedItems().takeAt(0);
		int id = qlwi->data(0, Qt::UserRole).toInt();
		qmChanged.insert(id, QString());
		delete qlwi;
	}
}

void UserEdit::on_qtwUserList_customContextMenuRequested(const QPoint &point) {
	QMenu *menu = new QMenu(this);

	QAction *action;

	if (!(qtwUserList->selectedItems().count() > 1))
	{
		action = menu->addAction(tr("Rename"));
		connect(action, SIGNAL(triggered()), this, SLOT(renameTriggered()));
		menu->addSeparator();
	}

	action = menu->addAction(tr("Remove"));
	connect(action, SIGNAL(triggered()), this, SLOT(on_qpbRemove_clicked()));

	menu->exec(qtwUserList->mapToGlobal(point));
	delete menu;
}

void UserEdit::renameTriggered() {
	QTreeWidgetItem *item = qtwUserList->currentItem();
	if (item) {
		qtwUserList->editItem(item, 0);
	}
}

void UserEdit::on_qlSearch_textChanged(QString) {
	refreshUserList();
}

void UserEdit::on_qtwUserList_itemSelectionChanged() {
	qpbRename->setEnabled(qtwUserList->selectedItems().count() == 1);
	qpbRemove->setEnabled(qtwUserList->selectedItems().count() > 0);
}

void UserEdit::on_qsbInactive_valueChanged(int) {
	updateInactiveDaysFilter();
}

void UserEdit::on_qcbInactive_currentIndexChanged(int) {
	updateInactiveDaysFilter();
}

void UserEdit::updateInactiveDaysFilter() {
	const int iTimespanUnit = qcbInactive->currentIndex();
	const int iTimespanCount = qsbInactive->value();
	int iInactiveForDays = 0;
	switch (iTimespanUnit) {
		case 0:
			iInactiveForDays = iTimespanCount;
			break;
		case 1:
			iInactiveForDays = iTimespanCount * 7;
			break;
		case 2:
			iInactiveForDays = iTimespanCount * 30;
			break;
		case 3:
			iInactiveForDays = iTimespanCount * 365;
			break;
		default:
			break;
	}
	iInactiveForDaysFiltervalue = iInactiveForDays;
	refreshUserList();
}


UserEditListItem::UserEditListItem(const int userid) : QTreeWidgetItem() {
	setFlags(flags() | Qt::ItemIsEditable);
	setData(0, Qt::UserRole, userid);
}

bool UserEditListItem::operator<(const QTreeWidgetItem &other) const {
	// Avoid duplicating the User sorting code for a little more complexity
	User first, second;
	first.qsName = text(0);
	second.qsName = other.text(0);
	return User::lessThan(&first, &second);
}
