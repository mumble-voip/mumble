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

#include "mumble_pch.hpp"

#include "UserLocalVolume.h"
#include "Global.h"
#include "ClientUser.h"

UserLocalVolume::UserLocalVolume(QWidget *p, unsigned int sessionId)
    : QWidget(p, Qt::Window)
	, m_clientSession(sessionId) {
    setupUi(this);
	ClientUser *user = ClientUser::get(sessionId);
	if (user) {
		QString title = tr("Adjusting local volume for %1").arg(user->qsName);
		setWindowTitle(title);
        qsUserLocalVolume -> setValue(qRound(log2(user->fLocalVolume) * 6.0));
	}
}

void UserLocalVolume::on_qsUserLocalVolume_valueChanged(int Value) {

    qsbUserLocalVolume -> setValue(Value);
    ClientUser *user = ClientUser::get(m_clientSession);
    if (user) {
        user -> fLocalVolume = static_cast<float>
                (pow(2.0, qsUserLocalVolume -> value() / 6.0));
        // Decibel formula +6db = *2
    }
}


void UserLocalVolume::on_qsbUserLocalVolume_valueChanged(int Value) {
    qsUserLocalVolume->setValue(Value);
    if (Value > 0) {
        if (Value < 11) {
            qsbUserLocalVolume -> setStyleSheet(tr("QSpinBox {color:green;}"));
        } else if (Value < 21) {
            qsbUserLocalVolume -> setStyleSheet(tr("QSpinBox {color:yellow;}"));
        } else {
            qsbUserLocalVolume -> setStyleSheet(tr("QSpinBox {color:red;}"));
        }
    } else if (Value < 0) {
        if (Value > -11) {
            qsbUserLocalVolume -> setStyleSheet(tr("QSpinBox {color:green;}"));
        } else if (Value > -21) {
            qsbUserLocalVolume -> setStyleSheet(tr("QSpinBox {color:yellow;}"));
        } else {
            qsbUserLocalVolume -> setStyleSheet(tr("QSpinBox {color:red;}"));
        }
    } else {
        qsbUserLocalVolume -> setStyleSheet(tr(""));
    }
}

void UserLocalVolume::on_qbbButtons_clicked(QAbstractButton *button) {
    if (button == qbbButtons -> button(QDialogButtonBox::Reset)) {
        qsUserLocalVolume -> setValue(0);
    }

    if (button == qbbButtons -> button(QDialogButtonBox::Ok)) {
        UserLocalVolume::close();
    }
}
