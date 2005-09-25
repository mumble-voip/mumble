/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#include <QApplication>
#include <QIcon>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include "ACLEditor.h"
#include "ACL.h"
#include "ServerHandler.h"
#include "Global.h"

ACLEditor::ACLEditor(const MessageEditACL *mea, QWidget *p) : QDialog(p) {
	QLabel *l;
	MessageEditACL::ACLStruct as;
	MessageEditACL::GroupStruct gs;

	QTabWidget *qtwTab = new QTabWidget(this);
	QWidget *groupEditor=new QWidget();
	QWidget *aclEditor=new QWidget();

	QGroupBox *qgbACLs = new QGroupBox(tr("Active ACLs"));
	QGroupBox *qgbACLapply = new QGroupBox(tr("Context"));
	QGroupBox *qgbACLugroup = new QGroupBox(tr("User/Group"));
	QGroupBox *qgbACLpermissions = new QGroupBox(tr("Permissions"));

	QGroupBox *qgbGroups = new QGroupBox(tr("Group"));
	QGroupBox *qgbGroupMembers = new QGroupBox(tr("Members"));

	qlwACLs = new QListWidget();
	qlwACLs->setObjectName("ACLList");
	qpbACLAdd=new QPushButton(tr("&Add"));
	qpbACLAdd->setObjectName("ACLAdd");
	qpbACLRemove=new QPushButton(tr("&Remove"));
	qpbACLRemove->setObjectName("ACLRemove");
	qpbACLUp=new QPushButton(tr("&Up"));
	qpbACLUp->setObjectName("ACLUp");
	qpbACLDown=new QPushButton(tr("&Down"));
	qpbACLDown->setObjectName("ACLDown");
	qcbACLInherit=new QCheckBox(tr("Inherit ACLs"));
	qcbACLInherit->setObjectName("ACLInherit");

	QHBoxLayout *qhblAclList = new QHBoxLayout;
	qhblAclList->addWidget(qcbACLInherit);
	qhblAclList->addStretch(1);
	qhblAclList->addWidget(qpbACLUp);
	qhblAclList->addWidget(qpbACLDown);
	qhblAclList->addWidget(qpbACLAdd);
	qhblAclList->addWidget(qpbACLRemove);

	QVBoxLayout *qvblAclList = new QVBoxLayout;
	qvblAclList->addWidget(qlwACLs);
	qvblAclList->addLayout(qhblAclList);

	qgbACLs->setLayout(qvblAclList);


	QGridLayout *grid = new QGridLayout();

	qcbACLApplyHere=new QCheckBox(tr("Applies to this channel"));
	qcbACLApplyHere->setObjectName("ACLApplyHere");
	qcbACLApplySubs=new QCheckBox(tr("Applies to subchannels"));
	qcbACLApplySubs->setObjectName("ACLApplySubs");
	grid->addWidget(qcbACLApplyHere,0,0);
	grid->addWidget(qcbACLApplySubs,1,0);

	qgbACLapply->setLayout(grid);

	grid = new QGridLayout();

	qcbACLGroup=new QComboBox();
	qcbACLGroup->setObjectName("ACLGroup");
	l=new QLabel(tr("Group"));
	l->setBuddy(qcbACLGroup);
	grid->addWidget(qcbACLGroup,0,0);
	grid->addWidget(l,0,1);

	qleACLUser=new QLineEdit();
	qleACLUser->setObjectName("ACLUser");
	l=new QLabel(tr("User ID"));
	l->setBuddy(qleACLUser);
	grid->addWidget(qleACLUser,1,0);
	grid->addWidget(l,1,1);

	qgbACLugroup->setLayout(grid);


	grid = new QGridLayout();

	l=new QLabel(tr("Deny"));
	grid->addWidget(l,0,1);
	l=new QLabel(tr("Allow"));
	grid->addWidget(l,0,2);

	int perm=1;
	int idx=1;
	QString name;
	while (! (name = ChanACL::permName(static_cast<ChanACL::Perm>(perm))).isEmpty()) {
		QCheckBox *qcb;
		l = new QLabel(name);
		grid->addWidget(l,idx,0);
		qcb=new QCheckBox();
		grid->addWidget(qcb,idx,1);
		qlACLDeny << qcb;
		qcb=new QCheckBox();
		grid->addWidget(qcb,idx,2);
		qlACLAllow << qcb;

		idx++;
		perm = perm * 2;
	}

	qgbACLpermissions->setLayout(grid);

	grid = new QGridLayout();
	grid->addWidget(qgbACLs, 0, 0, 1, 2);
	grid->addWidget(qgbACLapply, 1, 0);
	grid->addWidget(qgbACLugroup, 2, 0);
	grid->addWidget(qgbACLpermissions,1, 1, 2, 1);
	aclEditor->setLayout(grid);


	grid = new QGridLayout();

	qcbGroupList=new QComboBox();
	qcbGroupList->setObjectName("GroupList");
	qcbGroupList->setEditable(true);
	grid->addWidget(qcbGroupList,0,0);
	qcbGroupInherit=new QCheckBox(tr("Inherit"));
	qcbGroupInherit->setObjectName("GroupInherit");
	grid->addWidget(qcbGroupInherit,0,1);
	qcbGroupInheritable=new QCheckBox(tr("Inheritable"));
	qcbGroupInheritable->setObjectName("GroupInheritable");
	grid->addWidget(qcbGroupInheritable,0,2);
	qcbGroupInherited=new QCheckBox(tr("Inherited"));
	qcbGroupInherited->setObjectName("GroupInherited");
	qcbGroupInherited->setEnabled(false);
	grid->addWidget(qcbGroupInherited,0,3);

	qgbGroups->setLayout(grid);

	grid = new QGridLayout();

	qlwGroupAdd = new QListWidget();
	qlwGroupAdd->setObjectName("ListGroupAdd");
	qlwGroupRemove = new QListWidget();
	qlwGroupRemove->setObjectName("ListGroupRemove");
	qlwGroupInherit = new QListWidget();
	qlwGroupInherit->setObjectName("ListGroupInherit");

	l = new QLabel(tr("Add"));
	grid->addWidget(l, 0, 0, 1, 2);
	l = new QLabel(tr("Remove"));
	grid->addWidget(l, 0, 2, 1, 2);
	l = new QLabel(tr("Inherit"));
	grid->addWidget(l, 0, 4, 1, 2);

	grid->addWidget(qlwGroupAdd, 1, 0, 1, 2);
	grid->addWidget(qlwGroupRemove, 1, 2, 1, 2);
	grid->addWidget(qlwGroupInherit, 1, 4, 2, 2);

	qleGroupAdd=new QLineEdit();
	qleGroupAdd->setObjectName("GroupAddName");
	qleGroupRemove=new QLineEdit();
	qleGroupRemove->setObjectName("GroupRemoveName");

	qpbGroupAddAdd=new QPushButton(tr("Add"));
	qpbGroupAddAdd->setObjectName("GroupAddAdd");
	qpbGroupAddRemove=new QPushButton(tr("Remove"));
	qpbGroupAddRemove->setObjectName("GroupAddRemove");

	qpbGroupRemoveAdd=new QPushButton(tr("Add"));
	qpbGroupRemoveAdd->setObjectName("GroupRemoveAdd");
	qpbGroupRemoveRemove=new QPushButton(tr("Remove"));
	qpbGroupRemoveRemove->setObjectName("GroupRemoveRemove");

	qpbGroupInheritRemove=new QPushButton(tr("Add to Remove"));
	qpbGroupInheritRemove->setObjectName("GroupInheritRemove");

	grid->addWidget(qleGroupAdd, 2, 0);
	grid->addWidget(qpbGroupAddAdd, 2, 1);
	grid->addWidget(qpbGroupAddRemove, 3, 0, 1, 2);

	grid->addWidget(qleGroupRemove, 2, 2);
	grid->addWidget(qpbGroupRemoveAdd, 2, 3);
	grid->addWidget(qpbGroupRemoveRemove, 3, 2, 1, 2);

	grid->addWidget(qpbGroupInheritRemove, 3, 4, 1, 2);

	qgbGroupMembers->setLayout(grid);

	grid = new QGridLayout();
	grid->addWidget(qgbGroups, 0, 0);
	grid->addWidget(qgbGroupMembers, 1, 0);

	groupEditor->setLayout(grid);

	qtwTab->addTab(groupEditor, tr("&Groups"));
	qtwTab->addTab(aclEditor, tr("&ACL"));

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
    ml->addWidget(qtwTab);
    ml->addStretch(1);
    ml->addSpacing(12);
    ml->addLayout(buttons);
    setLayout(ml);


	acls = mea->acls;
	groups = mea->groups;

	numInheritACL = 0;

	bInheritACL = mea->bInheritACL;

	foreach(as, acls) {
		if (as.bInherited)
			numInheritACL++;
		if (as.iPlayerId != -1)
			addQuery(ACLList, as.iPlayerId);
	}
	foreach(gs, groups) {
		int id;
		foreach(id, gs.qsAdd)
			addQuery(GroupAdd, id);
		foreach(id, gs.qsRemove)
			addQuery(GroupRemove, id);
		foreach(id, gs.qsInheritedMembers)
			addQuery(GroupInherit, id);
	}

	foreach(gs, groups) {
		qcbGroupList->addItem(gs.qsName);
	}

	refill(GroupAdd);
	refill(GroupRemove);
	refill(GroupInherit);
	refill(ACLList);

	doneQuery();

    QMetaObject::connectSlotsByName(this);

	resize(minimumSize());
}

void ACLEditor::accept() {
	QDialog::accept();
}

void ACLEditor::addQuery(WaitID me, int id) {
	qhIDWait[id].insert(me);
}

void ACLEditor::addQuery(WaitID me, QString name) {
	qhNameWait[name].insert(me);
}

void ACLEditor::doneQuery() {
	MessageQueryUsers mqu;

	cleanQuery();

	foreach(int id, qhIDWait.keys()) {
		mqu.qlIds << id;
		mqu.qlNames << QString();
	}
	foreach(QString name, qhNameWait.keys()) {
		mqu.qlIds << -1;
		mqu.qlNames << name;
	}
	if (mqu.qlIds.count() > 0)
		g.sh->sendMessage(&mqu);
}

void ACLEditor::cleanQuery() {
	QSet<WaitID> notify;

	foreach(int id, qhIDWait.keys()) {
		if (qhNameCache.contains(id)) {
			notify = notify.unite(qhIDWait.value(id));
			qhIDWait.remove(id);
		}
	}
	foreach(QString name, qhNameWait.keys()) {
		if (qhIDCache.contains(name)) {
			notify = notify.unite(qhNameWait.value(name));
			qhNameWait.remove(name);
		}
	}
	foreach(WaitID wid, notify) {
		refill(wid);
	}
}

void ACLEditor::returnQuery(const MessageQueryUsers *mqu) {
	int i;
	for(i=0;i<mqu->qlIds.count();i++) {
		int id = mqu->qlIds[i];
		QString name = mqu->qlNames[i];
		if ((id != -1) && ! name.isEmpty()) {
			qhIDCache[name] = id;
			qhNameCache[id] = name;
		}
	}
	cleanQuery();
}

void ACLEditor::refill(WaitID wid) {
	switch (wid) {
		case ACLList:
			refillACL();
			break;
		case GroupInherit:
			refillGroupInherit();
			break;
		case GroupRemove:
			refillGroupRemove();
			break;
		case GroupAdd:
			refillGroupAdd();
			break;
	}
}

QString ACLEditor::userName(int id) {
	if (qhNameCache.contains(id))
		return qhNameCache.value(id);
	else
		return QString("#%1").arg(id);

}

void ACLEditor::refillACL() {
	MessageEditACL::ACLStruct as;
	int idx = qlwACLs->currentRow();
	qlwACLs->clear();
	foreach(as, acls) {
		if (! bInheritACL && as.bInherited)
			continue;
		QString text;
		if (as.iPlayerId == -1)
			text=QString("@%1").arg(as.qsGroup);
		else
			text=userName(as.iPlayerId);
		QListWidgetItem *item=new QListWidgetItem(text, qlwACLs);
		if (as.bInherited) {
			QFont f = item->font();
			f.setItalic(true);
			item->setFont(f);
		}
	}
	qlwACLs->setCurrentRow(idx);
}

void ACLEditor::refillGroupAdd() {
	QString group = qcbGroupList->currentText().toLower();
	MessageEditACL::GroupStruct gs;

	bool found = false;

	foreach(gs, groups) {
		if (gs.qsName == group) {
			found = true;
			break;
		}
	}

	if (! found)
		return;

	QStringList qsl;
	foreach(int id, gs.qsAdd) {
		qsl << userName(id);
	}
	qsl.sort();
	qlwGroupAdd->clear();
	foreach(QString name, qsl) {
		qlwGroupAdd->addItem(name);
	}
}

void ACLEditor::refillGroupRemove() {
}

void ACLEditor::refillGroupInherit() {
}
