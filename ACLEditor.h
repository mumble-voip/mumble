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

#ifndef _ACLEDITOR_H
#define _ACLEDITOR_H

#include <QDialog>
#include <QWidget>
#include <QListWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QList>
#include <QHash>
#include "Message.h"

class ACLEditor : public QDialog {
	Q_OBJECT;
	protected:
		enum WaitID {
			GroupAdd, GroupRemove, GroupInherit, ACLList
		};
		QHash<int, QString> qhNameCache;
		QHash<QString, int> qhIDCache;

		QHash<int, QSet<WaitID> > qhIDWait;
		QHash<QString, QSet<WaitID> > qhNameWait;

		void addQuery(WaitID me, int id);
		void addQuery(WaitID me, QString name);
		void doneQuery();
		void cleanQuery();
		void refill(WaitID what);

		bool bInheritACL;
		QList<MessageEditACL::ACLStruct> acls;
		QList<MessageEditACL::GroupStruct> groups;
		int numInheritACL;

		QString userName(int id);

		QCheckBox *qcbACLInherit;
		QPushButton *qpbACLAdd;
		QPushButton *qpbACLRemove;
		QPushButton *qpbACLUp;
		QPushButton *qpbACLDown;
		QListWidget *qlwACLs;
		QCheckBox *qcbACLApplyHere;
		QCheckBox *qcbACLApplySubs;
		QComboBox *qcbACLGroup;
		QLineEdit *qleACLUser;
		QList<QCheckBox *> qlACLAllow;
		QList<QCheckBox *> qlACLDeny;

		QComboBox *qcbGroupList;
		QCheckBox *qcbGroupInherit;
		QCheckBox *qcbGroupInheritable;
		QCheckBox *qcbGroupInherited;
		QListWidget *qlwGroupAdd;
		QListWidget *qlwGroupRemove;
		QListWidget *qlwGroupInherit;
		QLineEdit *qleGroupAdd;
		QLineEdit *qleGroupRemove;
		QPushButton *qpbGroupAddAdd;
		QPushButton *qpbGroupAddRemove;
		QPushButton *qpbGroupRemoveAdd;
		QPushButton *qpbGroupRemoveRemove;
		QPushButton *qpbGroupInheritRemove;
	public:
		ACLEditor(const MessageEditACL *mea, QWidget *p = NULL);
		void returnQuery(const MessageQueryUsers *mqu);
	public slots:
		void accept();
	public slots:
		void refillACL();
		void refillGroupAdd();
		void refillGroupRemove();
		void refillGroupInherit();

};

#else
class ACLEditor;
#endif
