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

#include "ACLEditor.h"
#include "ACL.h"
#include "ServerHandler.h"
#include "Channel.h"
#include "Global.h"

ACLEditor::ACLEditor(const MessageEditACL *mea, QWidget *p) : QDialog(p) {
	QLabel *l;
	MessageEditACL::ACLStruct as;
	MessageEditACL::GroupStruct gs;
	MessageEditACL::ACLStruct *asp;
	MessageEditACL::GroupStruct *gsp;

	iId = mea->iId;
	setWindowTitle(tr("Mumble - Edit ACL for %1").arg(Channel::get(iId)->qsName));

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
	qlwACLs->setObjectName(QString::fromAscii("ACLList"));
	qpbACLAdd=new QPushButton(tr("&Add"));
	qpbACLAdd->setObjectName(QString::fromAscii("ACLAdd"));
	qpbACLRemove=new QPushButton(tr("&Remove"));
	qpbACLRemove->setObjectName(QString::fromAscii("ACLRemove"));
	qpbACLUp=new QPushButton(tr("&Up"));
	qpbACLUp->setObjectName(QString::fromAscii("ACLUp"));
	qpbACLDown=new QPushButton(tr("&Down"));
	qpbACLDown->setObjectName(QString::fromAscii("ACLDown"));
	qcbACLInherit=new QCheckBox(tr("Inherit ACLs"));
	qcbACLInherit->setObjectName(QString::fromAscii("ACLInherit"));

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
	qcbACLApplyHere->setObjectName(QString::fromAscii("ACLApplyHere"));
	qcbACLApplySubs=new QCheckBox(tr("Applies to sub-channels"));
	qcbACLApplySubs->setObjectName(QString::fromAscii("ACLApplySubs"));
	grid->addWidget(qcbACLApplyHere,0,0);
	grid->addWidget(qcbACLApplySubs,1,0);

	qgbACLapply->setLayout(grid);

	grid = new QGridLayout();

	qcbACLGroup=new QComboBox();
	qcbACLGroup->setEditable(true);
	qcbACLGroup->setObjectName(QString::fromAscii("ACLGroup"));
	l=new QLabel(tr("Group"));
	l->setBuddy(qcbACLGroup);
	grid->addWidget(qcbACLGroup,0,0);
	grid->addWidget(l,0,1);

	qleACLUser=new QLineEdit();
	qleACLUser->setObjectName(QString::fromAscii("ACLUser"));
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
		connect(qcb, SIGNAL(clicked(bool)), this, SLOT(ACLPermissions_clicked()));
		grid->addWidget(qcb,idx,1);
		qlACLDeny << qcb;
		qcb=new QCheckBox();
		connect(qcb, SIGNAL(clicked(bool)), this, SLOT(ACLPermissions_clicked()));
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
	qcbGroupList->setObjectName(QString::fromAscii("GroupList"));
	qcbGroupList->setEditable(true);
	grid->addWidget(qcbGroupList,0,0);
	qpbGroupRemove=new QPushButton(tr("Remove"));
	qpbGroupRemove->setObjectName(QString::fromAscii("GroupRemove"));
	grid->addWidget(qpbGroupRemove,0,1);
	qcbGroupInherit=new QCheckBox(tr("Inherit"));
	qcbGroupInherit->setObjectName(QString::fromAscii("GroupInherit"));
	grid->addWidget(qcbGroupInherit,0,2);
	qcbGroupInheritable=new QCheckBox(tr("Inheritable"));
	qcbGroupInheritable->setObjectName(QString::fromAscii("GroupInheritable"));
	grid->addWidget(qcbGroupInheritable,0,3);
	qcbGroupInherited=new QCheckBox(tr("Inherited"));
	qcbGroupInherited->setObjectName(QString::fromAscii("GroupInherited"));
	qcbGroupInherited->setEnabled(false);
	grid->addWidget(qcbGroupInherited,0,4);

	qgbGroups->setLayout(grid);

	grid = new QGridLayout();

	qlwGroupAdd = new QListWidget();
	qlwGroupAdd->setObjectName(QString::fromAscii("ListGroupAdd"));
	qlwGroupRemove = new QListWidget();
	qlwGroupRemove->setObjectName(QString::fromAscii("ListGroupRemove"));
	qlwGroupInherit = new QListWidget();
	qlwGroupInherit->setObjectName(QString::fromAscii("ListGroupInherit"));

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
	qleGroupAdd->setObjectName(QString::fromAscii("GroupAddName"));
	qleGroupRemove=new QLineEdit();
	qleGroupRemove->setObjectName(QString::fromAscii("GroupRemoveName"));

	qpbGroupAddAdd=new QPushButton(tr("Add"));
	qpbGroupAddAdd->setObjectName(QString::fromAscii("GroupAddAdd"));
	qpbGroupAddRemove=new QPushButton(tr("Remove"));
	qpbGroupAddRemove->setObjectName(QString::fromAscii("GroupAddRemove"));

	qpbGroupRemoveAdd=new QPushButton(tr("Add"));
	qpbGroupRemoveAdd->setObjectName(QString::fromAscii("GroupRemoveAdd"));
	qpbGroupRemoveRemove=new QPushButton(tr("Remove"));
	qpbGroupRemoveRemove->setObjectName(QString::fromAscii("GroupRemoveRemove"));

	qpbGroupInheritRemove=new QPushButton(tr("Add to Remove"));
	qpbGroupInheritRemove->setObjectName(QString::fromAscii("GroupInheritRemove"));

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
	cancelButton->setWhatsThis(tr("This button will cancel all changes and closes the dialog without "
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

	foreach (as, mea->acls) {
		asp = new MessageEditACL::ACLStruct(as);
		acls << asp;
	}
	foreach (gs, mea->groups) {
		gsp = new MessageEditACL::GroupStruct(gs);
		groups << gsp;
	}

	numInheritACL = 0;

	bInheritACL = mea->bInheritACL;
	qcbACLInherit->setChecked(bInheritACL);

	foreach(asp, acls) {
		if (asp->bInherited)
			numInheritACL++;
		if (asp->iPlayerId != -1)
			addQuery(ACLList, asp->iPlayerId);
	}
	foreach(gsp, groups) {
		int id;
		foreach(id, gsp->qsAdd)
			addQuery(GroupAdd, id);
		foreach(id, gsp->qsRemove)
			addQuery(GroupRemove, id);
		foreach(id, gsp->qsInheritedMembers)
			addQuery(GroupInherit, id);
	}

	refill(GroupAdd);
	refill(GroupRemove);
	refill(GroupInherit);
	refill(ACLList);

	doneQuery();

    QMetaObject::connectSlotsByName(this);

	refillGroupNames();

	resize(minimumSize());

	ACLEnableCheck();
	groupEnableCheck();

	addToolTipsWhatsThis();
}

ACLEditor::~ACLEditor() {
	MessageEditACL::ACLStruct *asp;
	MessageEditACL::GroupStruct *gsp;

	foreach(asp, acls) {
		delete asp;
	}
	foreach(gsp, groups) {
		delete gsp;
	}
}

void ACLEditor::addToolTipsWhatsThis() {
	qlwACLs->setToolTip(tr("List of entries"));
	qlwACLs->setWhatsThis(tr("This shows all the entries active on this channel. Entries inherited from parent channels will be "
						"show in italics."));
	qcbACLInherit->setToolTip(tr("Inherit ACL of parent?"));
	qcbACLInherit->setWhatsThis(tr("This sets whether or not the ACL up the chain of parent channels are applied to this object. "
						"Only those entries that are marked in the parent as \"Apply to sub-channels\" will be inherited."));
	qpbACLAdd->setToolTip(tr("Add new entry"));
	qpbACLAdd->setWhatsThis(tr("This adds a new entry, initially set with no permissions and applying to all."));
	qpbACLRemove->setToolTip(tr("Remove entry"));
	qpbACLRemove->setWhatsThis(tr("This removes the currently selected entry."));
	qpbACLUp->setToolTip(tr("Move entry up"));
	qpbACLUp->setWhatsThis(tr("This moves the entry up in the list. As entries are evaluated in order, this may change "
						"the effective permissions of users. You cannot move an entry above an inherited entry, if you "
						"really need that you'll have to duplicate the inherited entry."));
	qpbACLDown->setToolTip(tr("Move entry down"));
	qpbACLDown->setWhatsThis(tr("This moves the entry down in the list. As entries are evaluated in order, this may change "
						"the effective permissions of users."));
	qcbACLApplyHere->setToolTip(tr("Entry should apply to this channel"));
	qcbACLApplyHere->setWhatsThis(tr("This makes the entry apply to this channel."));
	qcbACLApplySubs->setToolTip(tr("Entry should apply to sub-channels."));
	qcbACLApplySubs->setWhatsThis(tr("This makes the entry apply to sub-channels of this channel."));
	qcbACLGroup->setToolTip(tr("Group this entry applies to"));
	qcbACLGroup->setWhatsThis(tr("This controls which group of users this entry applies to.<br />Note that the group is evaluated "
							"in the context of the channel the entry is used in. For example, the default ACL on the Root "
							"channel gives <i>Write</i> permission to the <i>admin</i> group. This entry, if inherited by a "
							"channel, will give a user write privileges if he belongs to the <i>admin</i> group in that channel, "
							"even if he doesn't belong to the <i>admin</i> group in the channel where the ACL originated.<br />"
							"If a group name starts with a !, its membership is negated, and if it starts with a ~, it is evaluated in the channel the ACL was defined in, rather than "
							"the channel the ACL is active in. Order is important; <i>!~in</i> is valid, but <i>~!in</i> is not.<br />"
							"A few special predefined groups are:<br />"
							"<b>all</b> - Everyone will match.<br />"
							"<b>auth</b> - All authenticated users will match.<br />"
							"<b>sub,a,b,c</b> - User currently in a sub-channel minimum <i>a</i> common parents, and between <i>b</i> and <i>c</i> channels down the chain. "
							"See the website for more extensive documentation on this one.<br />"
							"<b>in</b> - Users currently in the channel will match (convenience for <i>sub,0,0,0</i>).<br />"
							"<b>out</b> - Users outside the channel will match (convenience for <i>!sub,0,0,0</i>).<br />"
							"Note that an entry applies to either a user or a group, not both."));
	qleACLUser->setToolTip(tr("User this entry applies to"));
	qleACLUser->setWhatsThis(tr("This controls which user this entry applies to. Just type in the user name and hit enter "
							"to query the server for a match."));

	int idx;
	int p = 0x1;
	for(idx=0;idx<qlACLAllow.count();idx++) {
		ChanACL::Perm prm=static_cast<ChanACL::Perm>(p);
		QString perm = ChanACL::permName(prm);
		qlACLAllow[idx]->setToolTip(tr("Allow %1").arg(perm));
		qlACLDeny[idx]->setToolTip(tr("Deny %1").arg(perm));
		qlACLAllow[idx]->setWhatsThis(tr("This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.<br />%2").arg(perm).arg(ChanACL::whatsThis(prm)));
		qlACLDeny[idx]->setWhatsThis(tr("This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.<br />%2").arg(perm).arg(ChanACL::whatsThis(prm)));
		p = p * 2;
	}

	qcbGroupList->setToolTip(tr("List of groups"));
	qcbGroupList->setWhatsThis(tr("This is all the groups currently defined for the channel. To create a new group, just type in the "
							"name and press enter."));
	qpbGroupRemove->setToolTip(tr("Remove selected group"));
	qpbGroupRemove->setWhatsThis(tr("This removes the currently selected group. If the group was inherited, it will not be removed "
							"from the list, but all local information about the group will be cleared."));
	qcbGroupInherit->setToolTip(tr("Inherit group members from parent"));
	qcbGroupInherit->setWhatsThis(tr("This inherits all the members in the group from the parent, if the group is marked as "
							"<i>Inheritable</i> in the parent channel."));
	qcbGroupInheritable->setToolTip(tr("Make group inheritable to sub-channels"));
	qcbGroupInheritable->setWhatsThis(tr("This makes this group inheritable to sub-channels. If the group is non-inheritable, "
							"sub-channels are still free to create a new group with the same name."));
	qcbGroupInherited->setToolTip(tr("Group was inherited from parent channel"));
	qcbGroupInherited->setWhatsThis(tr("This indicates that the group was inherited from the parent channel. You cannot edit "
							"this flag, it's just for information."));
	qleGroupAdd->setToolTip(tr("Add member to group"));
	qleGroupAdd->setWhatsThis(tr("Type in the name of a player you wish to add to the group and press enter."));
	qleGroupRemove->setToolTip(tr("Remove member from group"));
	qleGroupRemove->setWhatsThis(tr("Type in the name of a player you wish to remove from the group and press enter."));
}


void ACLEditor::accept() {
	MessageEditACL::ACLStruct as;
	MessageEditACL::GroupStruct gs;
	MessageEditACL::ACLStruct *asp;
	MessageEditACL::GroupStruct *gsp;
	MessageEditACL mea;

	mea.iId = iId;
	mea.bQuery = false;
	mea.bInheritACL = bInheritACL;

	foreach(asp, acls) {
		as = *asp;
		if (as.bInherited)
			continue;
		mea.acls << as;
	}

	foreach(gsp, groups) {
		gs = *gsp;
		if (gs.bInherited && gs.bInherit && gs.bInheritable && (gs.qsAdd.count() == 0) && (gs.qsRemove.count() == 0))
			continue;
		gs.qsInheritedMembers.clear();
		mea.groups << gs;
	}

	g.sh->sendMessage(&mea);
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
		qhIDCache[name] = id;
		qhNameCache[id] = name;
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
		return QString::fromAscii("#%1").arg(id);
}

void ACLEditor::refillACL() {
	MessageEditACL::ACLStruct *as;

	foreach(as, qhACLNameWait.keys()) {
		if (acls.indexOf(as) >= 0) {
			QString name = qhACLNameWait.value(as);
			if (qhIDCache.contains(name)) {
				int id = qhIDCache.value(name);
				if (id != -1) {
					as->iPlayerId = id;
					as->qsGroup = QString();
				}
				qhACLNameWait.remove(as);
			}
		}
	}

	int idx = qlwACLs->currentRow();
	bool previnh = bInheritACL;
	bInheritACL = qcbACLInherit->isChecked();


	qlwACLs->clear();
	foreach(as, acls) {
		if (! bInheritACL && as->bInherited)
			continue;
		QString text;
		if (as->iPlayerId == -1)
			text=QString::fromAscii("@%1").arg(as->qsGroup);
		else
			text=userName(as->iPlayerId);
		QListWidgetItem *item=new QListWidgetItem(text, qlwACLs);
		if (as->bInherited) {
			QFont f = item->font();
			f.setItalic(true);
			item->setFont(f);
		}
	}
	if (bInheritACL && ! previnh)
		idx += numInheritACL;
	if (! bInheritACL && previnh)
		idx -= numInheritACL;

	qlwACLs->setCurrentRow(idx);
}

void ACLEditor::refillGroupNames() {
	MessageEditACL::GroupStruct *gsp;

	QString text = qcbGroupList->currentText().toLower();
	QStringList qsl;

	foreach(gsp, groups) {
		qsl << gsp->qsName;
	}
	qsl.sort();

	qcbGroupList->clear();
	foreach(QString name, qsl)
		qcbGroupList->addItem(name);
	qcbGroupList->setCurrentIndex(qcbGroupList->findText(text, Qt::MatchExactly));
}

MessageEditACL::GroupStruct *ACLEditor::currentGroup() {
	QString group = qcbGroupList->currentText().toLower();
	MessageEditACL::GroupStruct *gs;

	foreach(gs, groups) {
		if (gs->qsName == group) {
			return gs;
		}
	}

	return NULL;
}

MessageEditACL::ACLStruct *ACLEditor::currentACL() {
	int idx = qlwACLs->currentRow();
	if (idx == -1)
		return NULL;

	if (! bInheritACL)
		idx += numInheritACL;
	return acls[idx];
}

void ACLEditor::refillGroupAdd() {
	MessageEditACL::GroupStruct *gs;

	foreach(gs, qhAddNameWait.keys()) {
		if (groups.indexOf(gs) >= 0) {
			QString name = qhAddNameWait.value(gs);
			if (qhIDCache.contains(name)) {
				int id = qhIDCache.value(name);
				if (id != -1) {
					gs->qsAdd.insert(id);
				}
				qhAddNameWait.remove(gs);
			}
		}
	}

	gs = currentGroup();

	if (! gs)
		return;


	QStringList qsl;
	foreach(int id, gs->qsAdd) {
		qsl << userName(id);
	}
	qsl.sort();
	qlwGroupAdd->clear();
	foreach(QString name, qsl) {
		qlwGroupAdd->addItem(name);
	}
}

void ACLEditor::refillGroupRemove() {
	MessageEditACL::GroupStruct *gs;

	foreach(gs, qhRemoveNameWait.keys()) {
		if (groups.indexOf(gs) >= 0) {
			QString name = qhRemoveNameWait.value(gs);
			if (qhIDCache.contains(name)) {
				int id = qhIDCache.value(name);
				if (id != -1) {
					gs->qsRemove.insert(id);
				}
				qhRemoveNameWait.remove(gs);
			}
		}
	}

	gs = currentGroup();
	if (! gs)
		return;

	QStringList qsl;
	foreach(int id, gs->qsRemove) {
		qsl << userName(id);
	}
	qsl.sort();
	qlwGroupRemove->clear();
	foreach(QString name, qsl) {
		qlwGroupRemove->addItem(name);
	}
}

void ACLEditor::refillGroupInherit() {
	MessageEditACL::GroupStruct *gs = currentGroup();

	if (! gs)
		return;

	QStringList qsl;
	foreach(int id, gs->qsInheritedMembers) {
		qsl << userName(id);
	}
	qsl.sort();
	qlwGroupInherit->clear();
	foreach(QString name, qsl) {
		qlwGroupInherit->addItem(name);
	}
}

void ACLEditor::groupEnableCheck() {
	MessageEditACL::GroupStruct *gs = currentGroup();

	bool ena = true;

	if (! gs)
		ena = false;
	else
		ena = gs->bInherit;

	qlwGroupRemove->setEnabled(ena);
	qlwGroupInherit->setEnabled(ena);
	qleGroupRemove->setEnabled(ena);
	qpbGroupRemoveAdd->setEnabled(ena);
	qpbGroupRemoveRemove->setEnabled(ena);
	qpbGroupInheritRemove->setEnabled(ena);

	ena = (gs != NULL);
	qlwGroupAdd->setEnabled(ena);
	qpbGroupAddAdd->setEnabled(ena);
	qpbGroupAddRemove->setEnabled(ena);
	qcbGroupInherit->setEnabled(ena);
	qcbGroupInheritable->setEnabled(ena);

	if (gs) {
		qcbGroupInherit->setChecked(gs->bInherit);
		qcbGroupInheritable->setChecked(gs->bInheritable);
		qcbGroupInherited->setChecked(gs->bInherited);
	}
}

void ACLEditor::ACLEnableCheck() {
	MessageEditACL::ACLStruct *as = currentACL();
	MessageEditACL::GroupStruct *gs;;

	bool ena = true;
	if (! as)
		ena = false;
	else
		ena = ! as->bInherited;

	qpbACLRemove->setEnabled(ena);
	qpbACLUp->setEnabled(ena);
	qpbACLDown->setEnabled(ena);
	qcbACLApplyHere->setEnabled(ena);
	qcbACLApplySubs->setEnabled(ena);
	qcbACLGroup->setEnabled(ena);
	qleACLUser->setEnabled(ena);

	int idx;
	for(idx=0;idx<qlACLAllow.count();idx++) {
		qlACLAllow[idx]->setEnabled(ena);
		qlACLDeny[idx]->setEnabled(ena);
	}

	if (as) {
		qcbACLApplyHere->setChecked(as->bApplyHere);
		qcbACLApplySubs->setChecked(as->bApplySubs);
		int p = 0x1;
		for(idx=0;idx<qlACLAllow.count();idx++) {
			qlACLAllow[idx]->setChecked(static_cast<int>(as->pAllow) & p);
			qlACLDeny[idx]->setChecked(static_cast<int>(as->pDeny) & p);
			p = p * 2;
		}
		qcbACLGroup->clear();
		qcbACLGroup->addItem(QString());
		qcbACLGroup->addItem(QString::fromAscii("all"));
		qcbACLGroup->addItem(QString::fromAscii("auth"));
		qcbACLGroup->addItem(QString::fromAscii("in"));
		qcbACLGroup->addItem(QString::fromAscii("sub"));
		qcbACLGroup->addItem(QString::fromAscii("out"));
		qcbACLGroup->addItem(QString::fromAscii("~in"));
		qcbACLGroup->addItem(QString::fromAscii("~sub"));
		qcbACLGroup->addItem(QString::fromAscii("~out"));
		foreach(gs, groups)
			qcbACLGroup->addItem(gs->qsName);
		if (as->iPlayerId == -1) {
			qleACLUser->setText(QString());
			qcbACLGroup->addItem(as->qsGroup);
			qcbACLGroup->setCurrentIndex(qcbACLGroup->findText(as->qsGroup, Qt::MatchExactly));
		} else {
			qleACLUser->setText(userName(as->iPlayerId));
		}
	}
}

void ACLEditor::on_ACLList_currentRowChanged() {
	ACLEnableCheck();
}

void ACLEditor::on_ACLAdd_clicked() {
	MessageEditACL::ACLStruct *as = new MessageEditACL::ACLStruct;
	as->bApplyHere = true;
	as->bApplySubs = true;
	as->bInherited = false;
	as->qsGroup = QString::fromAscii("all");
	as->iPlayerId = -1;
	as->pAllow = ChanACL::None;
	as->pDeny = ChanACL::None;
	acls << as;
	refillACL();
	qlwACLs->setCurrentRow(qlwACLs->count() - 1);
}

void ACLEditor::on_ACLRemove_clicked() {
	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;
	acls.removeAll(as);
	delete as;
	refillACL();
}

void ACLEditor::on_ACLUp_clicked() {
	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;

	int idx = acls.indexOf(as);
	if (idx <= numInheritACL)
		return;
	acls.swap(idx - 1, idx);
	qlwACLs->setCurrentRow(qlwACLs->currentRow() - 1);
	refillACL();
}

void ACLEditor::on_ACLDown_clicked() {
	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;

	int idx = acls.indexOf(as) + 1;
	if (idx >= acls.count())
		return;
	acls.swap(idx - 1, idx);
	qlwACLs->setCurrentRow(qlwACLs->currentRow() + 1);
	refillACL();
}

void ACLEditor::on_ACLInherit_clicked(bool) {
	refillACL();
}

void ACLEditor::on_ACLApplyHere_clicked(bool checked) {
	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;

	as->bApplyHere = checked;
}

void ACLEditor::on_ACLApplySubs_clicked(bool checked) {
	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;

	as->bApplySubs = checked;
}

void ACLEditor::on_ACLGroup_activated(const QString &text) {
	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;

	as->iPlayerId = -1;

	if (text.isEmpty()) {
		qcbACLGroup->setCurrentIndex(1);
		as->qsGroup=QString::fromAscii("all");
	} else {
		qleACLUser->setText(QString());
		as->qsGroup=text;
	}
	refillACL();
}

void ACLEditor::on_ACLUser_editingFinished() {
	QString text = qleACLUser->text();

	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;

	if (text.isEmpty()) {
		as->iPlayerId = -1;
		if (qcbACLGroup->currentIndex() == 0) {
			qcbACLGroup->setCurrentIndex(1);
			as->qsGroup=QString::fromAscii("all");
		}
		refillACL();
	} else {
		qcbACLGroup->setCurrentIndex(0);
		qhACLNameWait[as] = text;
		addQuery(ACLList, text);
		doneQuery();
	}
}

void ACLEditor::ACLPermissions_clicked() {
	QCheckBox *source = qobject_cast<QCheckBox *>(sender());

	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;

	int a, d, p, idx;
	a = 0;
	d = 0;

	p = 0x1;
	for(idx=0;idx<qlACLAllow.count();idx++) {
		if (qlACLAllow[idx]->isChecked() && qlACLDeny[idx]->isChecked()) {
			if (source == qlACLAllow[idx])
				qlACLDeny[idx]->setChecked(false);
			else
				qlACLAllow[idx]->setChecked(false);
		}
		if (qlACLAllow[idx]->isChecked())
			a |= p;
		if (qlACLDeny[idx]->isChecked())
			d |= p;
		p = p * 2;
	}

	as->pAllow=static_cast<ChanACL::Permissions>(a);
	as->pDeny=static_cast<ChanACL::Permissions>(d);
}

void ACLEditor::on_GroupList_activated(const QString &text) {
	MessageEditACL::GroupStruct *gs = currentGroup();
	if (text.isEmpty())
		return;
	if (! gs) {
		QString name = text.toLower();
		gs = new MessageEditACL::GroupStruct;
		gs->bInherited = false;
		gs->bInherit = true;
		gs->bInheritable = true;
		gs->qsName = name;
		groups << gs;
	}

	refillGroupNames();
	refillGroupAdd();
	refillGroupRemove();
	refillGroupInherit();
	groupEnableCheck();
}

void ACLEditor::on_GroupRemove_clicked() {
	MessageEditACL::GroupStruct *gs = currentGroup();
	if (! gs)
		return;
	if (gs->bInherited) {
		gs->bInheritable = true;
		gs->bInherit = true;
		gs->qsAdd.clear();
		gs->qsRemove.clear();
	} else {
		groups.removeAll(gs);
		delete gs;
	}
	refillGroupNames();
	refillGroupAdd();
	refillGroupRemove();
	refillGroupInherit();
	groupEnableCheck();
}

void ACLEditor::on_GroupInherit_clicked(bool checked) {
	MessageEditACL::GroupStruct *gs = currentGroup();
	if (! gs)
		return;
	gs->bInherit = checked;
	groupEnableCheck();
}

void ACLEditor::on_GroupInheritable_clicked(bool checked) {
	MessageEditACL::GroupStruct *gs = currentGroup();
	if (! gs)
		return;
	gs->bInheritable = checked;
}

void ACLEditor::on_GroupAddName_editingFinished() {
	QString text = qleGroupAdd->text();

	MessageEditACL::GroupStruct *gs = currentGroup();
	if (! gs)
		return;

	if (text.isEmpty())
		return;

	qhAddNameWait[gs] = text;
	addQuery(GroupAdd, text);
	doneQuery();
}

void ACLEditor::on_GroupAddAdd_clicked() {
	on_GroupAddName_editingFinished();
}

void ACLEditor::on_GroupAddRemove_clicked() {
	MessageEditACL::GroupStruct *gs = currentGroup();
	if (! gs)
		return;

	QListWidgetItem *item= qlwGroupAdd->currentItem();
	if (! item)
		return;

	int id = qhIDCache.value(item->text());
	gs->qsAdd.remove(id);
	refillGroupAdd();
}

void ACLEditor::on_GroupRemoveName_editingFinished() {
	QString text = qleGroupRemove->text();

	MessageEditACL::GroupStruct *gs = currentGroup();
	if (! gs)
		return;

	if (text.isEmpty())
		return;

	qhRemoveNameWait[gs] = text;
	addQuery(GroupRemove, text);
	doneQuery();
}

void ACLEditor::on_GroupRemoveAdd_clicked() {
	on_GroupRemoveName_editingFinished();
}

void ACLEditor::on_GroupRemoveRemove_clicked() {
	MessageEditACL::GroupStruct *gs = currentGroup();
	if (! gs)
		return;

	QListWidgetItem *item= qlwGroupRemove->currentItem();
	if (! item)
		return;

	int id = qhIDCache.value(item->text());
	gs->qsRemove.remove(id);
	refillGroupRemove();
}

void ACLEditor::on_GroupInheritRemove_clicked() {
	MessageEditACL::GroupStruct *gs = currentGroup();
	if (! gs)
		return;

	QListWidgetItem *item= qlwGroupInherit->currentItem();
	if (! item)
		return;

	int id = qhIDCache.value(item->text());
	gs->qsRemove.insert(id);
	refillGroupRemove();
}
