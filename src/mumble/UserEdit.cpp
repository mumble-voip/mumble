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

#include "UserEdit.h"
#include "ServerHandler.h"
#include "Global.h"
#include "User.h"

UserEdit::UserEdit(const MumbleProto::UserList &msg, QWidget *p) : QDialog(p) {
	setupUi(this);

	qlwUserList->setContextMenuPolicy(Qt::CustomContextMenu);

	for (int i=0;i<msg.users_size(); ++i) {
		const MumbleProto::UserList_User &u = msg.users(i);
		int id = u.user_id();
		const QString &name = u8(u.name());

		UserEditListItem *ueli = new UserEditListItem(name, id);

		qlwUserList->addItem(ueli);
		qmUsers.insert(id, name);
	}
}

void UserEdit::accept() {
	QList<QListWidgetItem *> ql = qlwUserList->findItems(QString(), Qt::MatchStartsWith);
	foreach(QListWidgetItem * qlwi, ql) {
		const QString &name = qlwi->text();
		int id = qlwi->data(Qt::UserRole).toInt();
		if (qmUsers.value(id) != name) {
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

void UserEdit::on_qpbRemove_clicked() {
	int idx = qlwUserList->currentRow();
	if (idx >= 0) {
		QListWidgetItem *qlwi = qlwUserList->takeItem(idx);
		int id = qlwi->data(Qt::UserRole).toInt();
		qmChanged.insert(id, QString());
		delete qlwi;
	}
}

void UserEdit::on_qlwUserList_customContextMenuRequested(const QPoint &point) {
	QMenu *menu = new QMenu(this);

	QAction *action = menu->addAction(tr("Rename"));
	connect(action, SIGNAL(triggered()), this, SLOT(renameTriggered()));

	menu->addSeparator();

	action = menu->addAction(tr("Remove"));
	connect(action, SIGNAL(triggered()), this, SLOT(on_qpbRemove_clicked()));

	menu->exec(qlwUserList->mapToGlobal(point));
	delete menu;
}

void UserEdit::renameTriggered() {
	QListWidgetItem *item = qlwUserList->currentItem();
	if (item) {
		qlwUserList->editItem(item);
	}
}

UserEditListItem::UserEditListItem(const QString &username, const int userid) : QListWidgetItem(username) {
	setFlags(flags() | Qt::ItemIsEditable);
	setData(Qt::UserRole, userid);
}

bool UserEditListItem::operator<(const QListWidgetItem &other) const {
	// Avoid duplicating the User sorting code for a little more complexity
	User first, second;
	first.qsName = text();
	second.qsName = other.text();
	return User::lessThan(&first, &second);
}
