/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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

#include "DXConfigDialog.h"
#include "Global.h"

static ConfigWidget *DXConfigDialogNew() {
	return new DXConfigDialog();
}

static ConfigRegistrar registrar(20, DXConfigDialogNew);


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
	QGroupBox *qgbDevices, *qgbOutput, *qgb3D;
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
									"speakers or your headset."));
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
	l = new QLabel(tr("Output Delay"));
	l->setBuddy(qsOutputDelay);
	qlOutputDelay=new QLabel();
	qlOutputDelay->setMinimumWidth(30);
	on_OutputDelay_valueChanged(qsOutputDelay->value());
	qsOutputDelay->setToolTip(tr("Amount of data to buffer for DirectSound"));
	qsOutputDelay->setWhatsThis(tr("This sets the amount of data to prebuffer in the DirectSound buffer. "
								"Experiment with different values and set it to the lowest which doesn't "
								"cause rapid jitter in the sound."));
	grid->addWidget(l, 0, 0);
	grid->addWidget(qsOutputDelay, 0, 1);
	grid->addWidget(qlOutputDelay, 0, 2);

	qgbOutput->setLayout(grid);


	qgb3D=new QGroupBox(tr("Positional Audio"));
	grid=new QGridLayout();

	qcbMethod=new QComboBox();
	qcbMethod->addItem(tr("None"), Settings::None);
	qcbMethod->addItem(tr("Panning"), Settings::Panning);
	qcbMethod->addItem(tr("Light HRTF"), Settings::Light);
	qcbMethod->addItem(tr("Full HRTF"), Settings::Full);
	qcbMethod->setCurrentIndex(static_cast<int>(g.s.a3dModel));

	qcbMethod->setToolTip(tr("3D Sound Algorithm"));
	qcbMethod->setWhatsThis(tr("This sets what 3D Sound algorithm to use.<br />"
							"<b>None</b> - Disable 3D Sound (least CPU).<br />"
							"<b>Panning</b> - Just use stereo panning (some CPU).<br />"
							"<b>Light/Full HRTF</b> - Head-Related Transfer Functions enabled. "
							"This may use a small amount of CPU.<br />"
							"Note that if you have a soundcard with <i>hardware</i> 3D processing, HRTF "
							"processing will be done on the soundcard and will use practically no processing "
							"power."));
	l = new QLabel(tr("Method"));
	l->setBuddy(qcbMethod);
	grid->addWidget(l, 0, 0);
	grid->addWidget(qcbMethod, 0, 1, 1, 2);

	qsMinDistance = new QSlider(Qt::Horizontal);
	qsMaxDistance = new QSlider(Qt::Horizontal);
	qsRollOff = new QSlider(Qt::Horizontal);
	qlIntensity = new QLabel();

	qsMinDistance->setRange(10, 200);
	qsMinDistance->setSingleStep(1);
	qsMinDistance->setPageStep(10);
	qsMinDistance->setValue(lround(g.s.fDXMinDistance * 10));
	qsMinDistance->setObjectName("MinDistance");
	l = new QLabel(tr("MinDistance"));
	l->setBuddy(qsMinDistance);
	qlMinDistance=new QLabel();
	qlMinDistance->setMinimumWidth(40);
	on_MinDistance_valueChanged(qsMinDistance->value());
	qsMinDistance->setToolTip(tr("Minimum distance to player before sound decreases"));
	qsMinDistance->setWhatsThis(tr("This sets the minimum distance for sound calculations. The volume of other players' "
								"speech will not decrease until they are at least this far away from you."));
	grid->addWidget(l, 1, 0);
	grid->addWidget(qsMinDistance, 1, 1);
	grid->addWidget(qlMinDistance, 1, 2);

	qsMaxDistance->setRange(10, 1000);
	qsMaxDistance->setSingleStep(1);
	qsMaxDistance->setPageStep(10);
	qsMaxDistance->setValue(lround(g.s.fDXMaxDistance * 10));
	qsMaxDistance->setObjectName("MaxDistance");
	l = new QLabel(tr("MaxDistance"));
	l->setBuddy(qsMaxDistance);
	qlMaxDistance=new QLabel();
	qlMaxDistance->setMinimumWidth(40);
	on_MaxDistance_valueChanged(qsMaxDistance->value());
	qsMaxDistance->setToolTip(tr("Maximum distance, beyond which sound won't decrease"));
	qsMaxDistance->setWhatsThis(tr("This sets the maximum distance for sound calculations. When farther away than this, "
								"other players' sound volume will not decrease any more."));
	grid->addWidget(l, 2, 0);
	grid->addWidget(qsMaxDistance, 2, 1);
	grid->addWidget(qlMaxDistance, 2, 2);

	qsRollOff->setRange(0, 200);
	qsRollOff->setSingleStep(1);
	qsRollOff->setPageStep(10);
	qsRollOff->setValue(lround(g.s.fDXRollOff * 100));
	qsRollOff->setObjectName("RollOff");
	l = new QLabel(tr("RollOff"));
	l->setBuddy(qsRollOff);
	qlRollOff=new QLabel();
	qlRollOff->setMinimumWidth(40);
	on_RollOff_valueChanged(qsRollOff->value());
	qsRollOff->setToolTip(tr("Factor for sound volume decrease"));
	qsRollOff->setWhatsThis(tr("How fast should sound volume drop when passing beyond the minimum distance. The normal (1.0) is that "
								"sound volume halves each time the distance doubles. Increasing this value means sound volume "
								"drops faster, while decreasing it means it drops slower."));
	grid->addWidget(l, 3, 0);
	grid->addWidget(qsRollOff, 3, 1);
	grid->addWidget(qlRollOff, 3, 2);

	grid->addWidget(qlIntensity, 4, 0, 1, 3);

	qgb3D->setLayout(grid);

	v = new QVBoxLayout();
	v->addWidget(qgbDevices);
	v->addWidget(qgbOutput);
	v->addWidget(qgb3D);
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

	g.s.a3dModel = static_cast<Settings::Audio3D>(qcbMethod->currentIndex());
	g.s.fDXMinDistance = qsMinDistance->value() / 10.0;
	g.s.fDXMaxDistance = qsMaxDistance->value() / 10.0;
	g.s.fDXRollOff = qsRollOff->value() / 100.0;
}

void DXConfigDialog::on_OutputDelay_valueChanged(int v) {
	qlOutputDelay->setText(tr("%1ms").arg(v*20));
}

void DXConfigDialog::on_MinDistance_valueChanged(int v) {
	qlMinDistance->setText(tr("%1m").arg(v/10.0, 0, 'f', 1));
	qsMaxDistance->setMinimum(v);
	updateIntensity();
}

void DXConfigDialog::on_MaxDistance_valueChanged(int v) {
	qlMaxDistance->setText(tr("%1m").arg(v/10.0, 0, 'f', 1));
	if (v > 200)
		v = 200;
	qsMinDistance->setMaximum(v);
	updateIntensity();
}

void DXConfigDialog::on_RollOff_valueChanged(int v) {
	qlRollOff->setText(tr("%1").arg(v/100.0, 0, 'f', 2));
	updateIntensity();
}

void DXConfigDialog::updateIntensity() {
	float min = qsMinDistance->value() / 10.0;
	float max = qsMaxDistance->value() / 10.0;
	float roll = qsRollOff->value() / 100.0;

	float intensity = min / (min + (max-min)*roll);
	qlIntensity->setText(tr("Players more than %1 meters away have %2% intensity").arg(max,0,'f',1).arg(intensity * 100.0, 0, 'f', 1));
}
