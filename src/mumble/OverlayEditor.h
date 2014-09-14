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

#ifndef MUMBLE_MUMBLE_OVERLAYEDITOR_H_
#define MUMBLE_MUMBLE_OVERLAYEDITOR_H_

#include "OverlayEditorScene.h"
#include "ui_OverlayEditor.h"

struct OverlaySettings;

class OverlayEditor : public QDialog, public Ui::OverlayEditor {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayEditor)
	protected:
		QGraphicsItem *qgiPromote;
		OverlayEditorScene oes;
		OverlaySettings *os;

		void enterEvent(QEvent *);
		void leaveEvent(QEvent *);
	public:
		OverlayEditor(QWidget *p = NULL, QGraphicsItem *qgi = NULL, OverlaySettings *osptr = NULL);
		~OverlayEditor();
	signals:
		void applySettings();
	public slots:
		void reset();
		void apply();
		void accept();

		void on_qrbPassive_clicked();
		void on_qrbTalking_clicked();
		void on_qrbWhisper_clicked();
		void on_qrbShout_clicked();

		void on_qcbAvatar_clicked();
		void on_qcbUser_clicked();
		void on_qcbChannel_clicked();
		void on_qcbMutedDeafened_clicked();
		void on_qcbBox_clicked();

		void on_qsZoom_valueChanged(int);
};

#endif
