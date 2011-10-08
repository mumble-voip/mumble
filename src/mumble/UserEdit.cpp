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

#include "UserEdit.h"
#include "Channel.h"
#include "ServerHandler.h"
#include "Global.h"
#include "User.h"

UserEdit::UserEdit(const MumbleProto::UserList &msg, QWidget *p) : QDialog(p) {
	bool gotLastActive = false;
	bool gotLastChannel = false;

	setupUi(this);

	qtwUserList->setFocus();
	qtwUserList->setContextMenuPolicy(Qt::CustomContextMenu);

	//qtwUserList->header()->setResizeMode(0, QHeaderView::ResizeToContents); // user name
	//qtwUserList->header()->setResizeMode(1, QHeaderView::ResizeToContents); // last seen
	//qtwUserList->header()->setResizeMode(2, QHeaderView::Stretch); // on channel
	qtwUserList->sortByColumn(0, Qt::AscendingOrder); // sort by user name
	qmUsers.clear();

	for (int i=0;i<msg.users_size(); ++i) {
	const MumbleProto::UserList_User &u = msg.users(i);
		UserInfo uie;
		uie.user_id = u.user_id();
		uie.name = u8(u.name());
		if (u.has_last_channel())
			gotLastChannel = true;
		uie.last_channel = u.last_channel();
		if (u.has_last_active()) {
			gotLastActive = true;
			uie.last_active = QDateTime::fromString(u8(u.last_active()));
		}
		qmUsers.insert(uie.user_id, uie);
	}
	
	// If we don't have UserInfo from server (Murmur < 1.2.4), hide widgets.
	if (!gotLastActive) {
		qtwUserList->setColumnHidden(1, true);
		qtwUserList->setColumnHidden(2, true);
	} else if (!gotLastChannel) {
		qtwUserList->setColumnHidden(2, true);
	}
	refreshUserList();
}

void UserEdit::refreshUserList() {
	qtwUserList->clear();
	QMapIterator<int, UserInfo> i(qmUsers);

	while (i.hasNext()) {
		i.next();
		UserEditListItem *ueli = new UserEditListItem(i.key());
		ueli->setText(0, i.value().name);

		QString last_active;
		if (i.value().last_active.isValid()) {
			last_active = i.value().last_active.toString(QLatin1String("yyyy-MM-dd hh:mm:ss"));
		} else
			last_active.clear();

		if (!last_active.isEmpty()) {
			ueli->setText(1, last_active);

			Channel *c = Channel::get(i.value().last_channel);
			QString tree;
			if (c) {
				QStringList channel_tree;
				while (c->cParent != NULL) {
					channel_tree.prepend(c->qsName);
					c = c->cParent;
				}
			tree = QLatin1String("/ ") + channel_tree.join(QLatin1String(" / "));
			} else
				tree = QLatin1String("-");
			ueli->setText(2, tree);
		} else {
			ueli->setText(1, QLatin1String("-"));
			ueli->setText(2, QLatin1String("-"));
		}
		qtwUserList->addTopLevelItem(ueli);
	}
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
		for (i=qmChanged.constBegin(); i!=qmChanged.constEnd(); ++i) {
			MumbleProto::UserList_User *u = mpul.add_users();
			u->set_user_id(i.key());
			if (! i.value().isEmpty())
				u->set_name(u8(i.value()));
		}
		g.sh->sendMessage(mpul);
	}

	QDialog::accept();
}

void UserEdit::on_qtwUserList_customContextMenuRequested(const QPoint &point) {
	QMenu *menu = new QMenu(this);

	QAction *action;

	if (!(qtwUserList->selectedItems().count() > 1))
	{
		action = menu->addAction(tr("Rename"));
		connect(action, SIGNAL(triggered()), this, SLOT(on_qaUserRename_triggered()));
		menu->addSeparator();
	}

	action = menu->addAction(tr("Remove"));
	connect(action, SIGNAL(triggered()), this, SLOT(on_qaUserRemove_triggered()));

	menu->exec(qtwUserList->mapToGlobal(point));
	delete menu;
}

void UserEdit::on_qaUserRemove_triggered() {
	while (qtwUserList->selectedItems().count() > 0) {
		QTreeWidgetItem *qlwi = qtwUserList->selectedItems().takeAt(0);
		int id = qlwi->data(0, Qt::UserRole).toInt();
		qmChanged.insert(id, QString());
		delete qlwi;
	}
}

void UserEdit::on_qaUserRename_triggered() {
	QTreeWidgetItem *item = qtwUserList->currentItem();
	if (item) {
		qtwUserList->editItem(item, 0);
	}
}

UserEditListItem::UserEditListItem(const int userid) : QTreeWidgetItem() {
	setFlags(flags() | Qt::ItemIsEditable);
	setData(0, Qt::UserRole, userid);
}

void UserEdit::on_qlSearch_textChanged(QString) {
	qtwUserList->clearSelection();
	for (int i=0; i < qtwUserList->topLevelItemCount(); ++i)
	{
	const QString name = qtwUserList->topLevelItem(i)->text(0);
	const QString last_channel = qtwUserList->topLevelItem(i)->text(2);
	if (!name.contains(qlSearch->text(), Qt::CaseInsensitive) && !last_channel.contains(qlSearch->text(), Qt::CaseInsensitive))
		qtwUserList->setItemHidden(qtwUserList->topLevelItem(i), true);
	else
		qtwUserList->setItemHidden(qtwUserList->topLevelItem(i), false);
	}
}