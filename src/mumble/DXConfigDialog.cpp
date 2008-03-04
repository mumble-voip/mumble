/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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

static ConfigWidget *DXConfigDialogNew(Settings &st) {
	return new DXConfigDialog(st);
}

static ConfigRegistrar registrar(20, DXConfigDialogNew);


static BOOL CALLBACK DSEnumProc(LPGUID lpGUID, const WCHAR* lpszDesc,
                                const WCHAR*, void *ctx) {
	if (lpGUID) {
		QList<dsDevice> *l =reinterpret_cast<QList<dsDevice> *>(ctx);
		*l << dsDevice(QString::fromUtf16(reinterpret_cast<const ushort*>(lpszDesc)), *lpGUID);
	}

	return(true);
}

DXConfigDialog::DXConfigDialog(Settings &st) : ConfigWidget(st) {
	dsDevice dev;
	QByteArray a;

	setupUi(this);

	qlInput << dsDevice(tr("Default DirectSound Voice Input"), DSDEVID_DefaultVoiceCapture);
	qlOutput << dsDevice(tr("Default DirectSound Voice Output"),DSDEVID_DefaultVoicePlayback);

	DirectSoundCaptureEnumerate(DSEnumProc, reinterpret_cast<void *>(&qlInput));
	DirectSoundEnumerate(DSEnumProc, reinterpret_cast<void *>(&qlOutput));

	foreach(dev, qlInput) {
		a = QByteArray(reinterpret_cast<const char *>(&dev.second), sizeof(GUID));
		qcbInputDevice->addItem(dev.first, a);
	}

	foreach(dev, qlOutput) {
		a = QByteArray(reinterpret_cast<const char *>(&dev.second), sizeof(GUID));
		qcbOutputDevice->addItem(dev.first, a);
	}


	qcbMethod->addItem(tr("None"), Settings::None);
	qcbMethod->addItem(tr("Panning"), Settings::Panning);
	qcbMethod->addItem(tr("Light HRTF"), Settings::Light);
	qcbMethod->addItem(tr("Full HRTF"), Settings::Full);
}

QString DXConfigDialog::title() const {
	return tr("DirectSound");
}

QIcon DXConfigDialog::icon() const {
	return QIcon(QLatin1String("skin:config_dsound.png"));
}

void DXConfigDialog::load(const Settings &r) {
	for (int i=0;i<qcbInputDevice->count();i++) {
		if (qcbInputDevice->itemData(i).toByteArray() == r.qbaDXInput) {
			qcbInputDevice->setCurrentIndex(i);
			break;
		}
	}

	for (int i=0;i<qcbOutputDevice->count();i++) {
		if (qcbOutputDevice->itemData(i).toByteArray() == r.qbaDXOutput) {
			qcbOutputDevice->setCurrentIndex(i);
			break;
		}
	}

	loadSlider(qsOutputDelay, r.iDXOutputDelay);
	loadComboBox(qcbMethod, static_cast<int>(r.a3dModel));
	loadSlider(qsMinDistance, lroundf(r.fDXMinDistance * 10));
	loadSlider(qsMaxDistance, lroundf(r.fDXMaxDistance * 10));
	loadSlider(qsRollOff, lroundf(r.fDXRollOff * 100));
}

void DXConfigDialog::save() const {
	s.iDXOutputDelay = qsOutputDelay->value();
	s.qbaDXInput = qcbInputDevice->itemData(qcbInputDevice->currentIndex()).toByteArray();
	s.qbaDXOutput = qcbOutputDevice->itemData(qcbOutputDevice->currentIndex()).toByteArray();

	s.a3dModel = static_cast<Settings::Audio3D>(qcbMethod->currentIndex());
	s.fDXMinDistance = qsMinDistance->value() / 10.0;
	s.fDXMaxDistance = qsMaxDistance->value() / 10.0;
	s.fDXRollOff = qsRollOff->value() / 100.0;
}

bool DXConfigDialog::expert(bool b) {
	qgbPositional->setVisible(b);
	qgbOutput->setVisible(b);
	return true;
}

void DXConfigDialog::on_qsOutputDelay_valueChanged(int v) {
	qlOutputDelay->setText(tr("%1ms").arg(v*20));
}

void DXConfigDialog::on_qsMinDistance_valueChanged(int v) {
	qlMinDistance->setText(tr("%1m").arg(v/10.0, 0, 'f', 1));
	qsMaxDistance->setMinimum(v);
	updateIntensity();
}

void DXConfigDialog::on_qsMaxDistance_valueChanged(int v) {
	qlMaxDistance->setText(tr("%1m").arg(v/10.0, 0, 'f', 1));
	if (v > 200)
		v = 200;
	qsMinDistance->setMaximum(v);
	updateIntensity();
}

void DXConfigDialog::on_qsRollOff_valueChanged(int v) {
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

void DXConfigDialog::on_qcbMethod_currentIndexChanged(int v) {
	bool ena = (v > 0);
	qsMinDistance->setEnabled(ena);
	qsMaxDistance->setEnabled(ena);
	qsRollOff->setEnabled(ena);
	qlMinDistance->setEnabled(ena);
	qlMaxDistance->setEnabled(ena);
	qlRollOff->setEnabled(ena);
}
