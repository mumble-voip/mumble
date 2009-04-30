/* copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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
#include "Player.h"
#include "Global.h"

ACLGroup::ACLGroup(const QString &name) : Group(NULL, name) {
	bInherited = false;
}

ACLEditor::ACLEditor(const MumbleProto::ACL &mea, QWidget *p) : QDialog(p) {
	QLabel *l;

	msg = mea;

	setupUi(this);

	iId = mea.channel_id();
	setWindowTitle(tr("Mumble - Edit ACL for %1").arg(Channel::get(iId)->qsName));

	QGridLayout *grid = new QGridLayout(qgbACLpermissions);

	l=new QLabel(tr("Deny"), qgbACLpermissions);
	grid->addWidget(l,0,1);
	l=new QLabel(tr("Allow"), qgbACLpermissions);
	grid->addWidget(l,0,2);

	int idx=1;
	for (int i=0;i< ((iId == 0) ? 30 : 16);++i) {
		ChanACL::Perm perm = static_cast<ChanACL::Perm>(1 << i);
		QString name = ChanACL::permName(perm);

		if (! name.isEmpty()) {
			QCheckBox *qcb;
			l = new QLabel(name, qgbACLpermissions);
			grid->addWidget(l,idx,0);
			qcb=new QCheckBox(qgbACLpermissions);
			qcb->setToolTip(tr("Deny %1").arg(name));
			qcb->setWhatsThis(tr("This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.<br />%2").arg(name).arg(ChanACL::whatsThis(perm)));
			connect(qcb, SIGNAL(clicked(bool)), this, SLOT(ACLPermissions_clicked()));
			grid->addWidget(qcb,idx,1);

			qlACLDeny << qcb;


			qcb=new QCheckBox(qgbACLpermissions);
			qcb->setToolTip(tr("Allow %1").arg(name));
			qcb->setWhatsThis(tr("This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.<br />%2").arg(name).arg(ChanACL::whatsThis(perm)));
			connect(qcb, SIGNAL(clicked(bool)), this, SLOT(ACLPermissions_clicked()));
			grid->addWidget(qcb,idx,2);

			qlACLAllow << qcb;

			qlPerms << perm;

			++idx;
		}
	}

	connect(qcbGroupAdd->lineEdit(), SIGNAL(returnPressed()), qpbGroupAddAdd, SLOT(animateClick()));
	connect(qcbGroupRemove->lineEdit(), SIGNAL(returnPressed()), qpbGroupRemoveAdd, SLOT(animateClick()));

	foreach(Player *p, ClientPlayer::c_qmPlayers) {
		if (p->iId >= 0) {
			qhNameCache.insert(p->iId, p->qsName);
			qhIDCache.insert(p->qsName.toLower(), p->iId);
		}
	}

	ChanACL *def = new ChanACL(NULL);

	def->bApplyHere = true;
	def->bApplySubs = true;
	def->bInherited = true;
	def->iPlayerId = -1;
	def->qsGroup = QLatin1String("all");
	def->pAllow = ChanACL::Traverse | ChanACL::Enter | ChanACL::Speak | ChanACL::AltSpeak;
	def->pDeny = 0;

	qlACLs << def;

	for (int i=0;i<mea.acls_size();++i) {
		const MumbleProto::ACL_ChanACL &as = mea.acls(i);

		ChanACL *acl = new ChanACL(NULL);
		acl->bApplyHere = as.apply_here();
		acl->bApplySubs = as.apply_subs();
		acl->bInherited = as.inherited();
		acl->iPlayerId = -1;
		if (as.has_user_id())
			acl->iPlayerId = as.user_id();
		else
			acl->qsGroup = u8(as.group());
		acl->pAllow = static_cast<ChanACL::Permissions>(as.grant());
		acl->pDeny = static_cast<ChanACL::Permissions>(as.deny());

		qlACLs << acl;
	}

	for (int i=0;i<mea.groups_size();++i) {
		const MumbleProto::ACL_ChanGroup &gs = mea.groups(i);

		ACLGroup *gp = new ACLGroup(u8(gs.name()));
		for (int j=0;j<gs.add_size();++j)
			gp->qsAdd.insert(gs.add(j));
		for (int j=0;j<gs.remove_size();++j)
			gp->qsRemove.insert(gs.remove(j));
		for (int j=0;j<gs.inherited_members_size();++j)
			gp->qsTemporary.insert(gs.inherited_members(j));

		qlGroups << gp;
	}

	iUnknown = -2;

	numInheritACL = -1;

	bInheritACL = mea.inherit_acls();
	qcbACLInherit->setChecked(bInheritACL);

	foreach(ChanACL *acl, qlACLs) {
		if (acl->bInherited)
			numInheritACL++;
	}

	refill(GroupAdd);
	refill(GroupRemove);
	refill(GroupInherit);
	refill(ACLList);
	refillGroupNames();

	ACLEnableCheck();
	groupEnableCheck();
}

ACLEditor::~ACLEditor() {
	foreach(ChanACL *acl, qlACLs) {
		delete acl;
	}
	foreach(ACLGroup *gp, qlGroups) {
		delete gp;
	}
}

void ACLEditor::showEvent(QShowEvent *evt) {
	ACLEnableCheck();
	QDialog::showEvent(evt);
}

void ACLEditor::accept() {
	msg.set_inherit_acls(bInheritACL);
	msg.clear_acls();
	msg.clear_groups();

	foreach(ChanACL *acl, qlACLs) {
		if (acl->bInherited || (acl->iPlayerId < -1))
			continue;
		MumbleProto::ACL_ChanACL *mpa = msg.add_acls();
		mpa->set_apply_here(acl->bApplyHere);
		mpa->set_apply_subs(acl->bApplySubs);
		if (acl->iPlayerId != -1)
			mpa->set_user_id(acl->iPlayerId);
		else
			mpa->set_group(u8(acl->qsGroup));
		mpa->set_grant(acl->pAllow);
		mpa->set_deny(acl->pDeny);
	}

	foreach(ACLGroup *gp, qlGroups) {
		if (gp->bInherited && gp->bInherit && gp->bInheritable && (gp->qsAdd.count() == 0) && (gp->qsRemove.count() == 0))
			continue;
		MumbleProto::ACL_ChanGroup *mpg = msg.add_groups();
		mpg->set_name(u8(gp->qsName));
		foreach(int pid, gp->qsAdd)
			if (pid >= 0)
				mpg->add_add(pid);
		foreach(int pid, gp->qsRemove)
			if (pid >= 0)
				mpg->add_remove(pid);
	}

	g.sh->sendMessage(msg);

	QDialog::accept();
}


const QString ACLEditor::userName(int pid) {
	if (qhNameCache.contains(pid))
		return qhNameCache.value(pid);
	else
		return QString::fromLatin1("#%1").arg(pid);
}

int ACLEditor::id(const QString &uname) {
	QString name = uname.toLower();
	if (qhIDCache.contains(name))
		return qhIDCache.value(name);
	else {
		if (! qhNameWait.contains(name)) {
			MumbleProto::QueryUsers mpuq;
			mpuq.add_names(u8(name));
			g.sh->sendMessage(mpuq);

			iUnknown--;
			qhNameWait.insert(name, iUnknown);
			qhNameCache.insert(iUnknown, name);
		}
		return qhNameWait.value(name);
	}
}

void ACLEditor::returnQuery(const MumbleProto::QueryUsers &mqu) {
	if (mqu.names_size() != mqu.ids_size())
		return;

	for (int i=0;i < mqu.names_size(); ++i) {
		int pid = mqu.ids(i);
		QString name = u8(mqu.names(i));
		QString lname = name.toLower();
		qhIDCache.insert(lname, pid);
		qhNameCache.insert(pid, name);

		if (qhNameWait.contains(lname)) {
			int tid = qhNameWait.take(lname);

			foreach(ChanACL *acl, qlACLs)
				if (acl->iPlayerId == tid)
					acl->iPlayerId = pid;
			foreach(ACLGroup *gp, qlGroups) {
				if (gp->qsAdd.remove(tid))
					gp->qsAdd.insert(pid);
				if (gp->qsRemove.remove(tid))
					gp->qsRemove.insert(pid);
			}
			qhNameCache.remove(tid);
		}
	}
	refillGroupInherit();
	refillGroupRemove();
	refillGroupAdd();
	refillComboBoxes();
	refillACL();
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

void ACLEditor::refillComboBoxes() {
	QList<QComboBox *> ql;
	ql << qcbGroupAdd;
	ql << qcbGroupRemove;
	ql << qcbACLUser;

	QStringList names = qhNameCache.values();
	names.sort();

	foreach(QComboBox *qcb, ql) {
		qcb->clear();
		qcb->addItems(names);
		qcb->clearEditText();
	}
}

void ACLEditor::refillACL() {
	int idx = qlwACLs->currentRow();
	bool previnh = bInheritACL;
	bInheritACL = qcbACLInherit->isChecked();

	qlwACLs->clear();

	bool first = true;

	foreach(ChanACL *acl, qlACLs) {
		if (first)
			first = false;
		else if (! bInheritACL && acl->bInherited)
			continue;
		QString text;
		if (acl->iPlayerId == -1)
			text=QString::fromLatin1("@%1").arg(acl->qsGroup);
		else
			text=userName(acl->iPlayerId);
		QListWidgetItem *item=new QListWidgetItem(text, qlwACLs);
		if (acl->bInherited) {
			QFont f = item->font();
			f.setItalic(true);
			item->setFont(f);
		}
	}
	if (bInheritACL && ! previnh && (idx != 0))
		idx += numInheritACL;
	if (! bInheritACL && previnh)
		idx -= numInheritACL;

	qlwACLs->setCurrentRow(idx);
}

void ACLEditor::refillGroupNames() {
	QString text = qcbGroupList->currentText().toLower();
	QStringList qsl;

	foreach(ACLGroup *gp, qlGroups) {
		qsl << gp->qsName;
	}
	qsl.sort();

	qcbGroupList->clear();

	foreach(QString name, qsl) {
		qcbGroupList->addItem(name);
	}

	int wantindex = qcbGroupList->findText(text, Qt::MatchExactly);
	qcbGroupList->setCurrentIndex(wantindex);
}

ACLGroup *ACLEditor::currentGroup() {
	QString group = qcbGroupList->currentText().toLower();

	foreach(ACLGroup *gp, qlGroups) {
		if (gp->qsName == group) {
			return gp;
		}
	}

	return NULL;
}

ChanACL *ACLEditor::currentACL() {
	int idx = qlwACLs->currentRow();
	if (idx < 0)
		return NULL;

	if (! bInheritACL)
		idx += numInheritACL;
	return qlACLs[idx];
}

void ACLEditor::fillWidgetFromSet(QListWidget *qlw, const QSet<int> &qs) {
	qlw->clear();

	QList<idname> ql;
	foreach(int pid, qs) {
		ql << idname(userName(pid), pid);
	}
	qStableSort(ql);
	foreach(idname i, ql) {
		QListWidgetItem *qlwi = new QListWidgetItem(i.first, qlw);
		qlwi->setData(Qt::UserRole, i.second);
		if (i.second < 0) {
			QFont f = qlwi->font();
			f.setItalic(true);
			qlwi->setFont(f);
		}
	}
}

void ACLEditor::refillGroupAdd() {
	ACLGroup *gp = currentGroup();

	if (! gp)
		return;

	fillWidgetFromSet(qlwGroupAdd, gp->qsAdd);
}

void ACLEditor::refillGroupRemove() {
	ACLGroup *gp = currentGroup();
	if (! gp)
		return;

	fillWidgetFromSet(qlwGroupRemove, gp->qsRemove);
}

void ACLEditor::refillGroupInherit() {
	ACLGroup *gp = currentGroup();

	if (! gp)
		return;

	fillWidgetFromSet(qlwGroupInherit, gp->qsTemporary);
}

void ACLEditor::groupEnableCheck() {
	ACLGroup *gp = currentGroup();

	bool ena = true;

	if (! gp)
		ena = false;
	else
		ena = gp->bInherit;

	qlwGroupRemove->setEnabled(ena);
	qlwGroupInherit->setEnabled(ena);
	qcbGroupRemove->setEnabled(ena);
	qpbGroupRemoveAdd->setEnabled(ena);
	qpbGroupRemoveRemove->setEnabled(ena);
	qpbGroupInheritRemove->setEnabled(ena);

	ena = (gp != NULL);
	qlwGroupAdd->setEnabled(ena);
	qcbGroupAdd->setEnabled(ena);
	qpbGroupAddAdd->setEnabled(ena);
	qpbGroupAddRemove->setEnabled(ena);
	qcbGroupInherit->setEnabled(ena);
	qcbGroupInheritable->setEnabled(ena);

	if (gp) {
		qcbGroupInherit->setChecked(gp->bInherit);
		qcbGroupInheritable->setChecked(gp->bInheritable);
		qcbGroupInherited->setChecked(gp->bInherited);
	}
}

void ACLEditor::ACLEnableCheck() {
	ChanACL *as = currentACL();

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
	qcbACLUser->setEnabled(ena);

	int idx;
	for (idx=0;idx<qlACLAllow.count();idx++) {
		qlACLAllow[idx]->setEnabled(ena);
		qlACLDeny[idx]->setEnabled(ena);
	}

	if (as) {
		qcbACLApplyHere->setChecked(as->bApplyHere);
		qcbACLApplySubs->setChecked(as->bApplySubs);
		for (idx=0;idx<qlACLAllow.count();idx++) {
			ChanACL::Perm p = qlPerms[idx];
			qlACLAllow[idx]->setChecked(static_cast<int>(as->pAllow) & p);
			qlACLDeny[idx]->setChecked(static_cast<int>(as->pDeny) & p);
		}
		qcbACLGroup->clear();
		qcbACLGroup->addItem(QString());
		qcbACLGroup->addItem(QLatin1String("all"));
		qcbACLGroup->addItem(QLatin1String("auth"));
		qcbACLGroup->addItem(QLatin1String("in"));
		qcbACLGroup->addItem(QLatin1String("sub"));
		qcbACLGroup->addItem(QLatin1String("out"));
		qcbACLGroup->addItem(QLatin1String("~in"));
		qcbACLGroup->addItem(QLatin1String("~sub"));
		qcbACLGroup->addItem(QLatin1String("~out"));
		foreach(ACLGroup *gs, qlGroups)
			qcbACLGroup->addItem(gs->qsName);
		if (as->iPlayerId == -1) {
			qcbACLUser->clearEditText();
			qcbACLGroup->addItem(as->qsGroup);
			qcbACLGroup->setCurrentIndex(qcbACLGroup->findText(as->qsGroup, Qt::MatchExactly));
		} else {
			qcbACLUser->setEditText(userName(as->iPlayerId));
		}
	}
	foreach(QAbstractButton *b, qdbbButtons->buttons()) {
		QPushButton *qpb = qobject_cast<QPushButton *>(b);
		if (qpb) {
			qpb->setAutoDefault(false);
			qpb->setDefault(false);
		}
	}
}

void ACLEditor::on_qlwACLs_currentRowChanged() {
	ACLEnableCheck();
}

void ACLEditor::on_qpbACLAdd_clicked() {
	ChanACL *as = new ChanACL(NULL);
	as->bApplyHere = true;
	as->bApplySubs = true;
	as->bInherited = false;
	as->qsGroup = QLatin1String("all");
	as->iPlayerId = -1;
	as->pAllow = ChanACL::None;
	as->pDeny = ChanACL::None;
	qlACLs << as;
	refillACL();
	qlwACLs->setCurrentRow(qlwACLs->count() - 1);
}

void ACLEditor::on_qpbACLRemove_clicked() {
	ChanACL *as = currentACL();
	if (! as || as->bInherited)
		return;
	qlACLs.removeAll(as);
	delete as;
	refillACL();
}

void ACLEditor::on_qpbACLUp_clicked() {
	ChanACL *as = currentACL();
	if (! as || as->bInherited)
		return;

	int idx = qlACLs.indexOf(as);
	if (idx <= numInheritACL + 1)
		return;
	qlACLs.swap(idx - 1, idx);
	qlwACLs->setCurrentRow(qlwACLs->currentRow() - 1);
	refillACL();
}

void ACLEditor::on_qpbACLDown_clicked() {
	ChanACL *as = currentACL();
	if (! as || as->bInherited)
		return;

	int idx = qlACLs.indexOf(as) + 1;
	if (idx >= qlACLs.count())
		return;
	qlACLs.swap(idx - 1, idx);
	qlwACLs->setCurrentRow(qlwACLs->currentRow() + 1);
	refillACL();
}

void ACLEditor::on_qcbACLInherit_clicked(bool) {
	refillACL();
}

void ACLEditor::on_qcbACLApplyHere_clicked(bool checked) {
	ChanACL *as = currentACL();
	if (! as || as->bInherited)
		return;

	as->bApplyHere = checked;
}

void ACLEditor::on_qcbACLApplySubs_clicked(bool checked) {
	ChanACL *as = currentACL();
	if (! as || as->bInherited)
		return;

	as->bApplySubs = checked;
}

void ACLEditor::on_qcbACLGroup_activated(const QString &text) {
	ChanACL *as = currentACL();
	if (! as || as->bInherited)
		return;

	as->iPlayerId = -1;

	if (text.isEmpty()) {
		qcbACLGroup->setCurrentIndex(1);
		as->qsGroup=QLatin1String("all");
	} else {
		qcbACLUser->clearEditText();
		as->qsGroup=text;
	}
	refillACL();
}

void ACLEditor::on_qcbACLUser_activated() {
	QString text = qcbACLUser->currentText();

	qWarning("ACLUSER INDEXED!");
	qWarning() << text;

	ChanACL *as = currentACL();
	if (! as || as->bInherited)
		return;

	if (text.isEmpty()) {
		as->iPlayerId = -1;
		if (qcbACLGroup->currentIndex() == 0) {
			qcbACLGroup->setCurrentIndex(1);
			as->qsGroup=QLatin1String("all");
		}
		refillACL();
	} else {
		qcbACLGroup->setCurrentIndex(0);
		as->iPlayerId = id(text);
		refillACL();
	}
}

void ACLEditor::ACLPermissions_clicked() {
	QCheckBox *source = qobject_cast<QCheckBox *>(sender());

	ChanACL *as = currentACL();
	if (! as || as->bInherited)
		return;

	int a, d, idx;
	a = 0;
	d = 0;

	for (idx=0;idx<qlACLAllow.count();idx++) {
		ChanACL::Perm p = qlPerms[idx];
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
	}

	as->pAllow=static_cast<ChanACL::Permissions>(a);
	as->pDeny=static_cast<ChanACL::Permissions>(d);
}

void ACLEditor::on_qcbGroupList_activated(const QString &text) {
	ACLGroup *gs = currentGroup();
	if (text.isEmpty())
		return;
	if (! gs) {
		QString name = text.toLower();
		gs = new ACLGroup(name);
		gs->bInherited = false;
		gs->bInherit = true;
		gs->bInheritable = true;
		gs->qsName = name;
		qlGroups << gs;
	}

	refillGroupNames();
	refillGroupAdd();
	refillGroupRemove();
	refillGroupInherit();
	groupEnableCheck();
}

void ACLEditor::on_qpbGroupRemove_clicked() {
	ACLGroup *gs = currentGroup();
	if (! gs)
		return;
	if (gs->bInherited) {
		gs->bInheritable = true;
		gs->bInherit = true;
		gs->qsAdd.clear();
		gs->qsRemove.clear();
	} else {
		qlGroups.removeAll(gs);
		delete gs;
	}
	refillGroupNames();
	refillGroupAdd();
	refillGroupRemove();
	refillGroupInherit();
	groupEnableCheck();
}

void ACLEditor::on_qcbGroupInherit_clicked(bool checked) {
	ACLGroup *gs = currentGroup();
	if (! gs)
		return;
	gs->bInherit = checked;
	groupEnableCheck();
}

void ACLEditor::on_qcbGroupInheritable_clicked(bool checked) {
	ACLGroup *gs = currentGroup();
	if (! gs)
		return;
	gs->bInheritable = checked;
}

void ACLEditor::on_qpbGroupAddAdd_clicked() {
	ACLGroup *gs = currentGroup();
	QString text = qcbGroupAdd->currentText();

	if (! gs)
		return;

	if (text.isEmpty())
		return;

	gs->qsAdd << id(text);
	refillGroupAdd();
	qcbGroupAdd->clearEditText();
}

void ACLEditor::on_qpbGroupAddRemove_clicked() {
	ACLGroup *gs = currentGroup();
	if (! gs)
		return;

	QListWidgetItem *item= qlwGroupAdd->currentItem();
	if (! item)
		return;

	gs->qsAdd.remove(item->data(Qt::UserRole).toInt());
	refillGroupAdd();
	qcbGroupRemove->clearEditText();
}

void ACLEditor::on_qpbGroupRemoveAdd_clicked() {
	QString text = qcbGroupRemove->currentText();

	ACLGroup *gs = currentGroup();
	if (! gs)
		return;

	if (text.isEmpty())
		return;

	gs->qsRemove << id(text);
	refillGroupRemove();
}

void ACLEditor::on_qpbGroupRemoveRemove_clicked() {
	ACLGroup *gs = currentGroup();
	if (! gs)
		return;

	QListWidgetItem *item= qlwGroupRemove->currentItem();
	if (! item)
		return;

	gs->qsRemove.remove(item->data(Qt::UserRole).toInt());
	refillGroupRemove();
}

void ACLEditor::on_qpbGroupInheritRemove_clicked() {
	ACLGroup *gs = currentGroup();
	if (! gs)
		return;

	QListWidgetItem *item= qlwGroupInherit->currentItem();
	if (! item)
		return;

	gs->qsRemove.insert(item->data(Qt::UserRole).toInt());
	refillGroupRemove();
}
