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
#include "ACLEditor.h"
#include "ServerHandler.h"
#include "Global.h"

ACLEditor::ACLEditor(const MessageEditACL *mea, QWidget *p) : QDialog(p) {
	MessageEditACL::ACLStruct as;
	MessageEditACL::GroupStruct gs;

	QTabWidget *qtwTab = new QTabWidget(this);
	QWidget *groupEditor=new QWidget();
	QWidget *aclEditor=new QWidget();

	qtwTab->addTab(groupEditor, tr("&Groups"));
	qtwTab->addTab(aclEditor, tr("&ACL"));

    QPushButton *okButton = new QPushButton(tr("&OK"));
    okButton->setDefault(true);
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

    QVBoxLayout *l = new QVBoxLayout;
    l->addWidget(qtwTab);
    l->addStretch(1);
    l->addSpacing(12);
    l->addLayout(buttons);
    setLayout(l);

	acls = mea->acls;
	groups = mea->groups;

	foreach(as, acls) {
		if (as.iPlayerId != -1)
			addQuery(ACLList, as.iPlayerId);
	}
	foreach(gs, groups) {
		int id;
		foreach(id, gs.qlAdd)
			addQuery(GroupAdd, id);
		foreach(id, gs.qlRemove)
			addQuery(GroupRemove, id);
		foreach(id, gs.qlInheritedMembers)
			addQuery(GroupInherit, id);
	}

	refill(GroupAdd);
	refill(GroupRemove);
	refill(GroupInherit);
	refill(ACLList);

	doneQuery();
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
			qWarning("Got back %d %s", id, name.toLatin1().constData());
			qhIDCache[name] = id;
			qhNameCache[id] = name;
		}
	}
	cleanQuery();
}

void ACLEditor::refill(WaitID wid) {
}
