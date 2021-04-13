// Copyright 2015-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2015, Fredrik Nordin <freedick@ludd.ltu.se>

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

#ifndef MUMBLE_MUMBLE_USERVOLUME_H_
#define MUMBLE_MUMBLE_USERVOLUME_H_

#include <QMap>

#include "ClientUser.h"
#include "ui_UserLocalVolumeDialog.h"

class UserLocalVolumeDialog : public QDialog, private Ui::UserLocalVolumeDialog {
	Q_OBJECT
	Q_DISABLE_COPY(UserLocalVolumeDialog);

	/// The session ID for the user that the dialog is changing the volume for.
	unsigned int m_clientSession;

	/// The user's original adjustment (in dB) when entering the dialog.
	int m_originalVolumeAdjustmentDecibel;
	QMap< unsigned int, UserLocalVolumeDialog * > *m_qmUserVolTracker;

public slots:
	void closeEvent(QCloseEvent *event);
	void on_qsUserLocalVolume_valueChanged(int value);
	void on_qsbUserLocalVolume_valueChanged(int value);
	void on_qbbUserLocalVolume_clicked(QAbstractButton *b);
	void reject();

public:
	static void present(unsigned int sessionId, QMap< unsigned int, UserLocalVolumeDialog * > *qmUserVolTracker);
	UserLocalVolumeDialog(unsigned int sessionId, QMap< unsigned int, UserLocalVolumeDialog * > *qmUserVolTracker);
};

#endif
