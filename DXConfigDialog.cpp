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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QList>
#include <windows.h>
#include <dsound.h>
#include <string>
#include <iostream>

#include "AudioInput.h"
#include "AudioOutput.h"
#include "DXConfigDialog.h"
#include "Global.h"


// Things to include
// atTransmit (ptt, voice etc)
// vad grace (extra frames to count)
// quality, complexity, max amplification

BOOL CALLBACK DSEnumProc(LPGUID lpGUID, const WCHAR* lpszDesc,
			 const WCHAR* lpszDrvName, void *ctx)
{

	if ( lpGUID )
	{
		QMap<QString, LPGUID> *qmDevices;
		qmDevices = reinterpret_cast<QMap<QString, LPGUID> *>(ctx);

		qmDevices->insert(QString::fromUtf16(reinterpret_cast<const ushort*>(lpszDesc)), lpGUID);
	}
	
	return(true);
}


DXConfigDialog::DXConfigDialog(QWidget *p) : ConfigWidget(p) {

	QComboBox *qcbInputDevice = new QComboBox();
	QComboBox *qcbOutputDevice = new QComboBox();
	
	QHBoxLayout *h;
	QLabel *l;

	QVBoxLayout *v;
	
	QList<QString> keys;
	QString key;
	
	qmDSDevices = new QMap<QString, LPGUID>();

	DirectSoundEnumerate(reinterpret_cast<LPDSENUMCALLBACK>(DSEnumProc),
			     reinterpret_cast<void*>(qmDSDevices));

	foreach(key, qmDSDevices->keys())
	{
		qcbOutputDevice->addItem(key);
		qcbInputDevice->addItem(key);
	}


	v = new QVBoxLayout();
	
	l = new QLabel(tr("Input Device"));
	
	h = new QHBoxLayout();
	h->addWidget(l);
	h->addStretch(1);
	h->addWidget(qcbInputDevice);
	v->addLayout(h);

	l = new QLabel(tr("Output Device"));

	h = new QHBoxLayout();
	h->addWidget(l);
	h->addStretch(1);
	h->addWidget(qcbOutputDevice);
	v->addLayout(h);

	/* Add option for output buffer size */
		

	v->addStretch(1);

	setLayout(v);
//    QMetaObject::connectSlotsByName(this);
}

QString DXConfigDialog::title() const {
	return tr("DX Audio Config");
}

QIcon DXConfigDialog::icon() const {
	return ConfigWidget::icon();
}

void DXConfigDialog::accept() {
	/* foo */
}
