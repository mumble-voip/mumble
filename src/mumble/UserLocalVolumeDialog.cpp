// Copyright 2005-2019 The Mumble Developers. All rights reserved.
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


#include "UserLocalVolumeDialog.h"
#include "ClientUser.h"
#include "Database.h"

#include <QtGui/QCloseEvent>
#include <QtWidgets/QPushButton>

#include <cmath>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

UserLocalVolumeDialog::UserLocalVolumeDialog(unsigned int sessionId,
                                             QMap<unsigned int, UserLocalVolumeDialog *> *qmUserVolTracker)
	: QDialog(NULL)
	, m_clientSession(sessionId)
	, m_qmUserVolTracker(qmUserVolTracker) {
	setupUi(this);

	ClientUser *user = ClientUser::get(sessionId);
	if (user) {
		QString title = tr("Adjusting local volume for %1").arg(user->qsName);
		setWindowTitle(title);
		qsUserLocalVolume->setValue(qRound(log2(user->fLocalVolume) * 6.0));
		m_originalVolumeAdjustmentDecibel = qsUserLocalVolume->value();
	}
}

void UserLocalVolumeDialog::closeEvent(QCloseEvent *event) {
	m_qmUserVolTracker->remove(m_clientSession);
	event->accept();
}

void UserLocalVolumeDialog::present(unsigned int sessionId,
                                    QMap<unsigned int, UserLocalVolumeDialog *> *qmUserVolTracker) {
	if (qmUserVolTracker->contains(sessionId)) {
		qmUserVolTracker->value(sessionId)->raise();
	} else {
		UserLocalVolumeDialog *uservol = new UserLocalVolumeDialog(sessionId, qmUserVolTracker);
		uservol->show();
		qmUserVolTracker->insert(sessionId, uservol);
	}
}

void UserLocalVolumeDialog::on_qsUserLocalVolume_valueChanged(int value) {
	qsbUserLocalVolume->setValue(value);
	ClientUser *user = ClientUser::get(m_clientSession);
	if (user) {
		// Decibel formula: +6db = *2
		user->fLocalVolume = static_cast<float>(pow(2.0, qsUserLocalVolume->value() / 6.0));
	}
}

void UserLocalVolumeDialog::on_qsbUserLocalVolume_valueChanged(int value) {
	qsUserLocalVolume->setValue(value);
}

void UserLocalVolumeDialog::on_qbbUserLocalVolume_clicked(QAbstractButton *button) {
	if (button == qbbUserLocalVolume->button(QDialogButtonBox::Reset)) {
		qsUserLocalVolume->setValue(0);
	}
	if (button == qbbUserLocalVolume->button(QDialogButtonBox::Ok)) {
		ClientUser *user = ClientUser::get(m_clientSession);
		if (user && !user->qsHash.isEmpty()) {
			g.db->setUserLocalVolume(user->qsHash, user->fLocalVolume);
		}
		UserLocalVolumeDialog::close();
	}
	if (button == qbbUserLocalVolume->button(QDialogButtonBox::Cancel)) {
		qsUserLocalVolume->setValue(m_originalVolumeAdjustmentDecibel);
		UserLocalVolumeDialog::close();
	}
}

void UserLocalVolumeDialog::reject() {
	m_qmUserVolTracker->remove(m_clientSession);
	UserLocalVolumeDialog::close();
}
