/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _CONNECTDIALOG_H
#define _CONNECTDIALOG_H

#include "mumble_pch.hpp"
#include "ui_ConnectDialog.h"
#include "Timer.h"

struct PublicInfo {
	QString name;
	QUrl url;
	QString ip;
	unsigned short port;
};

class TextSortedItem : public QTreeWidgetItem {
	public:
		TextSortedItem(QTreeWidget *, const QStringList &);
		bool operator< (const QTreeWidgetItem &) const;
};

class ConnectDialog : public QDialog, public Ui::ConnectDialog {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ConnectDialog)
	protected:
		static QList<PublicInfo> qlPublicServers;
		static Timer tPublicServers;
		bool bPublicInit;
		QSqlTableModel *qstmServers;
		QModelIndex qmiDirty;
		QSqlRecord toRecord() const;
		bool bDirty;
		QHttp *qhList;

		void initList();
		void fillList();
		void fillEmpty();
	public slots:
		void accept();
		void on_qpbAdd_clicked();
		void on_qpbRemove_clicked();
		void onSelection_Changed(const QModelIndex &n, const QModelIndex &p);
		void onDirty(const QString &qs = QString());

		void on_qtwTab_currentChanged(int);
		void on_Request_done(bool);
		void on_qpbURL_clicked();
		void on_qpbCopy_clicked();
	public:
		QString qsServer, qsUsername, qsPassword;
		unsigned short usPort;
		ConnectDialog(QWidget *parent);
};

#else
class ConnectDialog;
#endif
