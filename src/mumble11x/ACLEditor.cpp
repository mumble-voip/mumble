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

	setupUi(this);

	iId = mea->iId;
	setWindowTitle(tr("Mumble - Edit ACL for %1").arg(Channel::get(iId)->qsName));

	QGridLayout *grid = new QGridLayout(qgbACLpermissions);

	l=new QLabel(tr("Deny"), qgbACLpermissions);
	grid->addWidget(l,0,1);
	l=new QLabel(tr("Allow"), qgbACLpermissions);
	grid->addWidget(l,0,2);

	int perm=1;
	int idx=1;
	QString name;
	while (!(name = ChanACL::permName(static_cast<ChanACL::Perm>(perm))).isEmpty()) {
		QCheckBox *qcb;
		l = new QLabel(name, qgbACLpermissions);
		grid->addWidget(l,idx,0);
		qcb=new QCheckBox(qgbACLpermissions);
		connect(qcb, SIGNAL(clicked(bool)), this, SLOT(ACLPermissions_clicked()));
		grid->addWidget(qcb,idx,1);
		qlACLDeny << qcb;
		qcb=new QCheckBox(qgbACLpermissions);
		connect(qcb, SIGNAL(clicked(bool)), this, SLOT(ACLPermissions_clicked()));
		grid->addWidget(qcb,idx,2);
		qlACLAllow << qcb;

		idx++;
		perm = perm * 2;
	}

	MessageEditACL::ACLStruct *def = new MessageEditACL::ACLStruct();
	def->bApplyHere = true;
	def->bApplySubs = true;
	def->bInherited = true;
	def->iPlayerId = -1;
	def->qsGroup = QLatin1String("all");
	def->pAllow = ChanACL::Traverse | ChanACL::Enter | ChanACL::Speak | ChanACL::AltSpeak;
	def->pDeny = 0;

	acls << def;

	foreach(as, mea->acls) {
		asp = new MessageEditACL::ACLStruct(as);
		acls << asp;
	}
	foreach(gs, mea->groups) {
		gsp = new MessageEditACL::GroupStruct(gs);
		groups << gsp;
	}

	numInheritACL = -1;

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

	refillGroupNames();

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

void ACLEditor::showEvent(QShowEvent *evt) {
	ACLEnableCheck();
	QDialog::showEvent(evt);
}

void ACLEditor::addToolTipsWhatsThis() {
	int idx;
	int p = 0x1;
	for (idx=0;idx<qlACLAllow.count();idx++) {
		ChanACL::Perm prm=static_cast<ChanACL::Perm>(p);
		QString perm = ChanACL::permName(prm);
		qlACLAllow[idx]->setToolTip(tr("Allow %1").arg(perm));
		qlACLDeny[idx]->setToolTip(tr("Deny %1").arg(perm));
		qlACLAllow[idx]->setWhatsThis(tr("This grants the %1 privilege. If a privilege is both allowed and denied, it is denied.<br />%2").arg(perm).arg(ChanACL::whatsThis(prm)));
		qlACLDeny[idx]->setWhatsThis(tr("This revokes the %1 privilege. If a privilege is both allowed and denied, it is denied.<br />%2").arg(perm).arg(ChanACL::whatsThis(prm)));
		p = p * 2;
	}
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
	for (i=0;i<mqu->qlIds.count();i++) {
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
		return QString::fromLatin1("#%1").arg(id);
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

	bool first = true;

	foreach(as, acls) {
		if (first)
			first = false;
		else if (! bInheritACL && as->bInherited)
			continue;
		QString text;
		if (as->iPlayerId == -1)
			text=QString::fromLatin1("@%1").arg(as->qsGroup);
		else
			text=userName(as->iPlayerId);
		QListWidgetItem *item=new QListWidgetItem(text, qlwACLs);
		if (as->bInherited) {
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
	MessageEditACL::GroupStruct *gsp;

	QString text = qcbGroupList->currentText().toLower();
	QStringList qsl;

	foreach(gsp, groups) {
		qsl << gsp->qsName;
	}
	qsl.sort();

	qcbGroupList->clear();

	foreach(QString name, qsl) {
		qcbGroupList->addItem(name);
	}

	int wantindex = qcbGroupList->findText(text, Qt::MatchExactly);
	qcbGroupList->setCurrentIndex(wantindex);
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
	if (idx < 0)
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
	for (idx=0;idx<qlACLAllow.count();idx++) {
		qlACLAllow[idx]->setEnabled(ena);
		qlACLDeny[idx]->setEnabled(ena);
	}

	if (as) {
		qcbACLApplyHere->setChecked(as->bApplyHere);
		qcbACLApplySubs->setChecked(as->bApplySubs);
		int p = 0x1;
		for (idx=0;idx<qlACLAllow.count();idx++) {
			qlACLAllow[idx]->setChecked(static_cast<int>(as->pAllow) & p);
			qlACLDeny[idx]->setChecked(static_cast<int>(as->pDeny) & p);
			p = p * 2;
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
	MessageEditACL::ACLStruct *as = new MessageEditACL::ACLStruct;
	as->bApplyHere = true;
	as->bApplySubs = true;
	as->bInherited = false;
	as->qsGroup = QLatin1String("all");
	as->iPlayerId = -1;
	as->pAllow = ChanACL::None;
	as->pDeny = ChanACL::None;
	acls << as;
	refillACL();
	qlwACLs->setCurrentRow(qlwACLs->count() - 1);
}

void ACLEditor::on_qpbACLRemove_clicked() {
	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;
	acls.removeAll(as);
	delete as;
	refillACL();
}

void ACLEditor::on_qpbACLUp_clicked() {
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

void ACLEditor::on_qpbACLDown_clicked() {
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

void ACLEditor::on_qcbACLInherit_clicked(bool) {
	refillACL();
}

void ACLEditor::on_qcbACLApplyHere_clicked(bool checked) {
	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;

	as->bApplyHere = checked;
}

void ACLEditor::on_qcbACLApplySubs_clicked(bool checked) {
	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;

	as->bApplySubs = checked;
}

void ACLEditor::on_qcbACLGroup_activated(const QString &text) {
	MessageEditACL::ACLStruct *as = currentACL();
	if (! as || as->bInherited)
		return;

	as->iPlayerId = -1;

	if (text.isEmpty()) {
		qcbACLGroup->setCurrentIndex(1);
		as->qsGroup=QLatin1String("all");
	} else {
		qleACLUser->setText(QString());
		as->qsGroup=text;
	}
	refillACL();
}

void ACLEditor::on_qleACLUser_editingFinished() {
	QString text = qleACLUser->text();

	MessageEditACL::ACLStruct *as = currentACL();
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
	for (idx=0;idx<qlACLAllow.count();idx++) {
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

void ACLEditor::on_qcbGroupList_activated(const QString &text) {
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

void ACLEditor::on_qpbGroupRemove_clicked() {
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

void ACLEditor::on_qcbGroupInherit_clicked(bool checked) {
	MessageEditACL::GroupStruct *gs = currentGroup();
	if (! gs)
		return;
	gs->bInherit = checked;
	groupEnableCheck();
}

void ACLEditor::on_qcbGroupInheritable_clicked(bool checked) {
	MessageEditACL::GroupStruct *gs = currentGroup();
	if (! gs)
		return;
	gs->bInheritable = checked;
}

void ACLEditor::on_qpbGroupAddAdd_clicked() {
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

void ACLEditor::on_qpbGroupAddRemove_clicked() {
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

void ACLEditor::on_qpbGroupRemoveAdd_clicked() {
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

void ACLEditor::on_qpbGroupRemoveRemove_clicked() {
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

void ACLEditor::on_qpbGroupInheritRemove_clicked() {
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
