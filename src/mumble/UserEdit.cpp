/* copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   copyright (C) 2011, Luki <luki@luki.net.pl>
   copyright (C) 2011, Zuko <zuczeq@gmail.com>

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
	setupUi(this);

	qtwUserList->setFocus();
	qtwUserList->setContextMenuPolicy(Qt::CustomContextMenu);

	int n = msg.users_size();
	if (n > 0)
		setWindowTitle(tr("%n Registered User(s)", "", n));
	else
		setWindowTitle(tr("No Registered Users"));

	qtwUserList->header()->setResizeMode(0, QHeaderView::Stretch); // user name
	qtwUserList->header()->setResizeMode(1, QHeaderView::ResizeToContents); // last seen
	qtwUserList->header()->setResizeMode(2, QHeaderView::Stretch); // on channel
	qtwUserList->sortByColumn(0, Qt::AscendingOrder); // sort by user name
	qmUsers.clear();

	for (int i=0;i<msg.users_size(); ++i) {
	const MumbleProto::UserList_User &u = msg.users(i);
		UserInfo uie;
		uie.user_id = u.user_id();
		uie.name = u8(u.name());
		uie.last_channel = u.last_channel();
		if (u.has_last_seen())
			uie.last_active = u8(u.last_seen());
		else
			uie.last_active = QString();
		qmUsers.insert(uie.user_id, uie);
	}
	refreshUserList();
}

void UserEdit::refreshUserList(int depth, int inactive) {
	qtwUserList->clear();
	QMapIterator<int, UserInfo> i(qmUsers);

	while (i.hasNext()) {
		i.next();
		UserEditListItem *ueli = new UserEditListItem(i.key());
		ueli->setText(0, i.value().name);

		QString last_active;
		QDateTime qdtLastActive;
		int last_seen;
		if (!i.value().last_active.isEmpty()) {
			qdtLastActive.fromString(i.value().last_active, QLatin1String("yyyy-MM-dd hh:mm:ss"));
			if (!qdtLastActive.isValid())
				qdtLastActive = QDateTime::fromString(i.value().last_active, QLatin1String("yyyy-MM-ddThh:mm:ss"));
			last_seen = qdtLastActive.daysTo(QDateTime::currentDateTime());
			if (last_seen == 0)
				last_active = tr("Today");
			else
				last_active = tr("%1 days ago").arg(QString::number(last_seen));
		} else
			last_active.clear();

		if ((inactive > 0) && (last_seen < inactive))
			continue;

		if (!last_active.isEmpty()) {
			ueli->setText(1, last_active);
			ueli->setToolTip(1, qdtLastActive.toString(QLatin1String("yyyy-MM-dd hh:mm:ss")));

			Channel *c = Channel::get(i.value().last_channel);
			QString tree;
			if (c) {
				QStringList channel_tree;
				while (c->cParent != NULL) {
					channel_tree.prepend(c->qsName);
					c = c->cParent;
				}
				if (depth > 0) {
					QStringList _channel_tree;
					for (QStringList::iterator it = channel_tree.begin(); it != channel_tree.end(); ++it) {
						if (depth <= 0)
							break;
						else {
							depth--;
							_channel_tree.append(*it);
						}
					}
					channel_tree.clear();
					channel_tree.append(_channel_tree);
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

void UserEdit::on_qtwUserList_itemSelectionChanged() {
	qpbRename->setEnabled(qtwUserList->selectedItems().count() == 1);
	qpbRemove->setEnabled(qtwUserList->selectedItems().count() > 0);
}

void UserEdit::on_qsbChanneldepth_valueChanged(int ) {
	qtwUserList->clearSelection();
	int cbvalue = qcbInactive->currentIndex();
	int sbvalue;
	switch (cbvalue) {
	case 0:
		sbvalue = qsbInactive->value();
		break;
	case 1:
		sbvalue = qsbInactive->value() * 7;
		break;
	case 2:
		sbvalue = qsbInactive->value() * 30;
		break;
	case 3:
		sbvalue = qsbInactive->value() * 365;
		break;
	default:
		sbvalue = 0;
	}
	refreshUserList(qsbChanneldepth->value(), sbvalue);
}

void UserEdit::on_qsbInactive_valueChanged(int val) {
	on_qsbChanneldepth_valueChanged(val);
}

void UserEdit::on_qcbInactive_currentIndexChanged(int index) {
	on_qsbChanneldepth_valueChanged(index);
}
