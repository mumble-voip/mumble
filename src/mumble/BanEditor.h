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

#ifndef MUMBLE_MUMBLE_BANEDITOR_H_
#define MUMBLE_MUMBLE_BANEDITOR_H_

#include "Net.h"

#include "ui_BanEditor.h"

namespace MumbleProto {
class BanList;
}

class BanEditor : public QDialog, public Ui::BanEditor {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(BanEditor)
	protected:
		QList<Ban> qlBans;

		int maskDefaultValue;

		Ban toBan(bool &);
	public:
		BanEditor(const MumbleProto::BanList &msbl, QWidget *p = NULL);
	public slots:
		void accept();
		void on_qlwBans_currentRowChanged();
		void on_qpbAdd_clicked();
		void on_qpbUpdate_clicked();
		void on_qpbRemove_clicked();
		void refreshBanList();
		void on_qdteEnd_editingFinished();
		void on_qpbClear_clicked();
	private slots:
		void on_qleHash_textChanged(QString );
		void on_qleSearch_textChanged(const QString & match);
		void on_qleReason_textChanged(QString );
		void on_qleIP_textChanged(QString );
		void on_qleUser_textChanged(QString );
};

#endif
