/* Copyright (C) 2010, Stefan Hacker <dd0t@users.sourceforge.net>
   Copyright (C) 2010, Benjamin Jemlich <pcgod@users.sourceforge.net>

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

#ifndef _VOICERECORDERDIALOG_H
#define _VOICERECORDERDIALOG_H

#pragma once

#include "mumble_pch.hpp"

#include "ui_VoiceRecorderDialog.h"

class VoiceRecorderDialog : public QDialog, private Ui::VoiceRecorderDialog {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(VoiceRecorderDialog)

		QTimer *qtTimer;
	public:
		explicit VoiceRecorderDialog(QWidget *p);
		~VoiceRecorderDialog();

		void closeEvent(QCloseEvent *evt);
	public slots:
		void on_qpbStart_clicked();
		void on_qpbStop_clicked();
		void on_qtTimer_timeout();
		void on_qpbTargetDirectoryBrowse_clicked();

		void onRecorderStopped();
		void onRecorderStarted();
		void onRecorderError(int err, QString strerr);

		void reset(bool resettimer=true);
};

#endif // _VOICERECORDERDIALOG_H
