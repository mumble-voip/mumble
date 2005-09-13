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

#include "DXConfigDialog.h"
#include "Global.h"



BOOL CALLBACK DSEnumProc(LPGUID lpGUID, const WCHAR* lpszDesc,
			 const WCHAR* lpszDrvName, void *ctx)
{
	if ( lpGUID )
	{
		QList<dsDevice> *l =reinterpret_cast<QList<dsDevice> *>(ctx);
		*l << dsDevice(QString::fromUtf16(reinterpret_cast<const ushort*>(lpszDesc)), *lpGUID);
	}

	return(true);
}

DXConfigDialog::DXConfigDialog(QWidget *p) : ConfigWidget(p) {
	QGroupBox *qgbDevices, *qgbOutput;
	QGridLayout *grid;
	QVBoxLayout *v;
	dsDevice dev;
	QLabel *l;
	QByteArray a;

	qlInput << dsDevice(tr("Default DirectSound Voice Input"), DSDEVID_DefaultVoiceCapture);
	qlOutput << dsDevice(tr("Default DirectSound Voice Output"),DSDEVID_DefaultVoicePlayback);

	DirectSoundCaptureEnumerate(DSEnumProc, reinterpret_cast<void *>(&qlInput));
	DirectSoundEnumerate(DSEnumProc, reinterpret_cast<void *>(&qlOutput));

	qcbInputDevice = new QComboBox();
	qcbOutputDevice = new QComboBox();

	foreach(dev, qlInput) {
		a = QByteArray(reinterpret_cast<const char *>(&dev.second), sizeof(GUID));
		qcbInputDevice->addItem(dev.first, a);
		if (a == g.s.qbaDXInput)
			qcbInputDevice->setCurrentIndex(qcbInputDevice->count() - 1);
	}

	foreach(dev, qlOutput) {
		a = QByteArray(reinterpret_cast<const char *>(&dev.second), sizeof(GUID));
		qcbOutputDevice->addItem(dev.first, a);
		if (a == g.s.qbaDXOutput)
			qcbOutputDevice->setCurrentIndex(qcbOutputDevice->count() - 1);
	}

	qgbDevices=new QGroupBox(tr("Device selection"));
	grid=new QGridLayout();

	qcbInputDevice->setToolTip(tr("Device to use for microphone"));
	qcbInputDevice->setWhatsThis(tr("This sets the input device to use, which is where you have connected the "
								"microphone."));
	l = new QLabel(tr("Input"));
	l->setBuddy(qcbInputDevice);
	grid->addWidget(l, 0, 0);
	grid->addWidget(qcbInputDevice, 0, 1);

	qcbOutputDevice->setToolTip(tr("Device to use for speakers/headphones"));
	qcbOutputDevice->setWhatsThis(tr("This sets the output device to use, which is where you have connected your "
									"speakers or your headset"));
	l = new QLabel(tr("Output"));
	l->setBuddy(qcbOutputDevice);
	grid->addWidget(l, 1, 0);
	grid->addWidget(qcbOutputDevice, 1, 1);

	qgbDevices->setLayout(grid);

	qgbOutput=new QGroupBox(tr("Output Options"));
	grid=new QGridLayout();

	qsOutputDelay = new QSlider(Qt::Horizontal);
	qsOutputDelay->setRange(1, 6);
	qsOutputDelay->setSingleStep(1);
	qsOutputDelay->setPageStep(2);
	qsOutputDelay->setValue(g.s.iDXOutputDelay);
	qsOutputDelay->setObjectName("OutputDelay");

	l = new QLabel(tr("OutputDelay"));
	l->setBuddy(qsOutputDelay);

	qlOutputDelay=new QLabel();
	qlOutputDelay->setMinimumWidth(30);
	on_OutputDelay_valueChanged(qsOutputDelay->value());

	qsOutputDelay->setToolTip(tr("Ammount of data to buffer for DirectSound"));
	qsOutputDelay->setWhatsThis(tr("This sets the ammount of data to prebuffer in the directsound buffer. "
								"Experiment with different values and set it to the lowest which doesn't "
								"cause rapid jitter in the sound."));
	grid->addWidget(l, 0, 0);
	grid->addWidget(qsOutputDelay, 0, 1);
	grid->addWidget(qlOutputDelay, 0, 2);

	qgbOutput->setLayout(grid);

	v = new QVBoxLayout();
	v->addWidget(qgbDevices);
	v->addWidget(qgbOutput);
	v->addStretch(1);

	setLayout(v);
    QMetaObject::connectSlotsByName(this);
}

QString DXConfigDialog::title() const {
	return tr("DirectSound");
}

QIcon DXConfigDialog::icon() const {
	return QIcon(":/icons/config_dsound.png");
}

void DXConfigDialog::accept() {
	g.s.iDXOutputDelay = qsOutputDelay->value();
	g.s.qbaDXInput = qcbInputDevice->itemData(qcbInputDevice->currentIndex()).toByteArray();
	g.s.qbaDXOutput = qcbOutputDevice->itemData(qcbOutputDevice->currentIndex()).toByteArray();
}

void DXConfigDialog::on_OutputDelay_valueChanged(int v) {
	qlOutputDelay->setText(tr("%1 ms").arg(v*20));
}
