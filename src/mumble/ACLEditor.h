/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _ACLEDITOR_H
#define _ACLEDITOR_H

#include "mumble_pch.hpp"
#include "Message.h"
#include "ACL.h"
#include "Group.h"

#include "ui_ACLEditor.h"

class ACLEditor : public QDialog, public Ui::ACLEditor {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ACLEditor)
	protected:
		typedef QPair<QString, int> idname;
		MumbleProto::ACL msg;
		enum WaitID {
			GroupAdd, GroupRemove, GroupInherit, ACLList
		};
		QHash<int, QString> qhNameCache;
		QHash<QString, int> qhIDCache;
		QHash<QString, int> qhNameWait;

		int iUnknown;

		void refill(WaitID what);

		Group *currentGroup();
		ChanACL *currentACL();

		int iId;
		bool bInheritACL;
		QList<ChanACL *> qlACLs;
		QList<Group *> qlGroups;
		int numInheritACL;

		const QString userName(int id);
		int id(const QString &uname);

		QList<QCheckBox *> qlACLAllow;
		QList<QCheckBox *> qlACLDeny;

		void addToolTipsWhatsThis();
		void showEvent(QShowEvent *);
		void fillWidgetFromSet(QListWidget *, const QSet<int> &);
	public:
		ACLEditor(const MumbleProto::ACL &mea, QWidget *p = NULL);
		~ACLEditor();
		void returnQuery(const MumbleProto::QueryUsers &mqu);
	public slots:
		void accept();
	public slots:
		void refillACL();
		void refillGroupNames();
		void refillGroupAdd();
		void refillGroupRemove();
		void refillGroupInherit();
		void groupEnableCheck();
		void ACLEnableCheck();

		void on_qlwACLs_currentRowChanged();
		void on_qpbACLAdd_clicked();
		void on_qpbACLRemove_clicked();
		void on_qpbACLUp_clicked();
		void on_qpbACLDown_clicked();
		void on_qcbACLInherit_clicked(bool checked);
		void on_qcbACLApplyHere_clicked(bool checked);
		void on_qcbACLApplySubs_clicked(bool checked);
		void on_qcbACLGroup_activated(const QString &text);
		void on_qleACLUser_editingFinished();
		void ACLPermissions_clicked();

		void on_qcbGroupList_activated(const QString &text);
		void on_qpbGroupRemove_clicked();
		void on_qcbGroupInherit_clicked(bool checked);
		void on_qcbGroupInheritable_clicked(bool checked);
		void on_qpbGroupAddAdd_clicked();
		void on_qpbGroupAddRemove_clicked();
		void on_qpbGroupRemoveAdd_clicked();
		void on_qpbGroupRemoveRemove_clicked();
		void on_qpbGroupInheritRemove_clicked();
};

#else
class ACLEditor;
#endif
