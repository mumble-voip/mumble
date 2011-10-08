/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _USEREDIT_H
#define _USEREDIT_H

#include "mumble_pch.hpp"
#include "ui_UserEdit.h"
#include "Message.h"
#include "User.h"

class UserEditListItem : public QTreeWidgetItem {
	public:
		UserEditListItem(const int userid);
};

class UserEdit : public QDialog, public Ui::UserEdit {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(UserEdit)
	protected:
		QMap<int, UserInfo> qmUsers;
		QMap<int, QString> qmChanged;
	public:
		UserEdit(const MumbleProto::UserList &mpul, QWidget *p = NULL);
	public slots:
		void accept();
		void on_qlSearch_textChanged(QString );
	public slots:
		void on_qtwUserList_customContextMenuRequested(const QPoint&);
		void on_qaUserRemove_triggered();
		void on_qaUserRename_triggered();
		void refreshUserList();
};

#endif
