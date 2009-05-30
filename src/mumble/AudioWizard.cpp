/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include "AudioWizard.h"
#include "AudioInput.h"
#include "Global.h"
#include "Settings.h"
#include "MainWindow.h"

CompletablePage::CompletablePage(QWizard *p) : QWizardPage(p) {
	bComplete = true;
}

void CompletablePage::setComplete(bool b) {
	bComplete = b;
	emit completeChanged();
}

bool CompletablePage::isComplete() const {
	return bComplete;
}

AudioWizard::AudioWizard(QWidget *p) : QWizard(p) {
	bInit = true;

	ticker = new QTimer(this);
	ticker->setObjectName(QLatin1String("Ticker"));

	setupUi(this);

	// Done
	qcbUsage->setChecked(g.s.bUsage);

	// Device
	if (AudioInputRegistrar::qmNew) {
		foreach(AudioInputRegistrar *air, *AudioInputRegistrar::qmNew) {
			qcbInput->addItem(air->name);
			if (air->name == AudioInputRegistrar::current)
				qcbInput->setCurrentIndex(qcbInput->count() - 1);
			QList<audioDevice> ql= air->getDeviceChoices();
		}
	}
	if (qcbInput->count() < 2) {
		qcbInput->setEnabled(false);
	}

	qcbEcho->setChecked(g.s.bEcho);

	if (AudioOutputRegistrar::qmNew) {
		foreach(AudioOutputRegistrar *aor, *AudioOutputRegistrar::qmNew) {
			qcbOutput->addItem(aor->name);
			if (aor->name == AudioOutputRegistrar::current) {
				qcbOutput->setCurrentIndex(qcbOutput->count() - 1);
				bDelay = aor->usesOutputDelay();
			}
			QList<audioDevice> ql= aor->getDeviceChoices();
		}
	}

	if (qcbOutput->count() < 2) {
		qcbOutput->setEnabled(false);
	}

	qcbPositional->setChecked(g.s.bPositionalAudio);

	on_qcbInput_activated(qcbInput->currentIndex());
	on_qcbOutput_activated(qcbOutput->currentIndex());

	// Trigger
	foreach(const Shortcut &s, g.s.qlShortcuts) {
		if (s.iIndex == g.mw->gsPushTalk->idx) {
			skwPTT->setShortcut(s.qlButtons);
			break;
		}
	}

	if (g.s.atTransmit == Settings::PushToTalk)
		qrPTT->setChecked(true);
	else if (g.s.vsVAD == Settings::Amplitude)
		qrAmplitude->setChecked(true);
	else
		qrSNR->setChecked(true);

	abVAD->qcBelow = Qt::red;
	abVAD->qcInside = Qt::yellow;
	abVAD->qcAbove = Qt::green;

	qsMinVAD->setValue(static_cast<int>(g.s.fVADmin * 32767.0));
	qsMaxVAD->setValue(static_cast<int>(g.s.fVADmax * 32767));

	// Positional
	qcbHeadphone->setChecked(g.s.bPositionalHeadphone);

	fAngle = 0.0f;
	fX = fY = 0.0f;
	qgsScene = NULL;
	qgiSource = NULL;
	aosSource = NULL;
	qgvView->scale(1.0f, -1.0f);
	qgvView->viewport()->installEventFilter(this);

	// Volume
	qsMaxAmp->setValue(g.s.iMinLoudness);

	// Device Tuning
	qsOutputDelay->setValue(g.s.iOutputDelay);

	on_qsOutputDelay_valueChanged(qsOutputDelay->value());

	setOption(QWizard::NoCancelButton, false);
	resize(700, 500);

	updateTriggerWidgets(qrPTT->isChecked());
	sOldSettings = g.s;
	g.s.lmLoopMode = Settings::Local;
	g.s.dPacketLoss = 0.0;
	g.s.dMaxPacketDelay = 0.0;
	g.s.bMute = true;
	g.s.bDeaf = false;

	g.s.atTransmit = Settings::Continous;
	bTransmitChanged = false;

	iMaxPeak = 0;
	iTicks = 0;

	bInit = false;

	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(showPage(int)));

	ticker->setSingleShot(false);
	ticker->start(20);
}

bool AudioWizard::eventFilter(QObject *obj, QEvent *evt) {
	if ((evt->type() == QEvent::MouseButtonPress) ||
	        (evt->type() == QEvent::MouseMove)) {
		QMouseEvent *qme = dynamic_cast<QMouseEvent *>(evt);
		if (qme) {
			if (qme->buttons() & Qt::LeftButton) {
				QPointF qpf = qgvView->mapToScene(qme->pos());
				fX = static_cast<float>(qpf.x());
				fY = static_cast<float>(qpf.y());
			}
		}
	}
	return QWizard::eventFilter(obj, evt);
}

void AudioWizard::on_qcbInput_activated(int) {
	qcbInputDevice->clear();

	if (! AudioInputRegistrar::qmNew)
		return;

	AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbInput->currentText());
	QList<audioDevice> ql = air->getDeviceChoices();

	foreach(audioDevice d, ql) {
		qcbInputDevice->addItem(d.first, d.second);
	}

	qcbInputDevice->setEnabled(ql.count() > 1);

	on_qcbInputDevice_activated(0);
}

void AudioWizard::on_qcbInputDevice_activated(int) {
	if (bInit)
		return;

	if (! AudioInputRegistrar::qmNew)
		return;

	boost::weak_ptr<AudioInput> wai(g.ai);
	g.ai.reset();

	while (! wai.expired()) {
	}

	AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbInput->currentText());
	int idx = qcbInputDevice->currentIndex();
	if (idx > -1) {
		air->setDeviceChoice(qcbInputDevice->itemData(idx), g.s);
	}

	qcbEcho->setEnabled(air->canEcho(qcbOutput->currentText()));

	g.ai = AudioInputPtr(air->create());
	g.ai->start(QThread::HighestPriority);
}

void AudioWizard::on_qcbOutput_activated(int) {
	qcbOutputDevice->clear();

	if (! AudioOutputRegistrar::qmNew)
		return;

	AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew->value(qcbOutput->currentText());
	QList<audioDevice> ql = aor->getDeviceChoices();

	foreach(audioDevice d, ql) {
		qcbOutputDevice->addItem(d.first, d.second);
	}

	qcbOutputDevice->setEnabled(ql.count() > 1);

	on_qcbOutputDevice_activated(0);
}

void AudioWizard::on_qcbOutputDevice_activated(int) {
	if (bInit)
		return;

	if (! AudioOutputRegistrar::qmNew)
		return;

	boost::weak_ptr<AudioOutput> wai(g.ao);
	g.ao.reset();

	while (! wai.expired()) {
	}

	AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew->value(qcbOutput->currentText());
	int idx = qcbOutputDevice->currentIndex();
	if (idx > -1) {
		aor->setDeviceChoice(qcbOutputDevice->itemData(idx), g.s);
		bDelay = aor->usesOutputDelay();
	}

	AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbInput->currentText());
	qcbEcho->setEnabled(air->canEcho(qcbOutput->currentText()));

	g.ao = AudioOutputPtr(aor->create());
	g.ao->start(QThread::HighPriority);
}

void AudioWizard::on_qsOutputDelay_valueChanged(int v) {
	qlOutputDelay->setText(tr("%1ms").arg(v*20));
	g.s.iOutputDelay = v;
	on_qcbOutputDevice_activated(0);
	if (! bInit) {
		AudioOutputPtr ao = g.ao;
		ao->playSine(0.0f, 0.0f, 0xfffffff, 0.5f);
	}
}

void AudioWizard::on_qsMaxAmp_valueChanged(int v) {
	g.s.iMinLoudness = qMin(v, 30000);
}

void AudioWizard::showPage(int) {
	CompletablePage *cp = qobject_cast<CompletablePage *>(currentPage());

	AudioOutputPtr ao = g.ao;
	if (ao)
		ao->wipe();
	aosSource = NULL;

	g.bPosTest = false;

	if (cp == qwpIntro) {
		g.s.bMute = true;
	} else if (cp == qwpDone) {
		g.s.bMute = true;
	} else if (cp == qwpDeviceTuning) {
		g.s.bMute = true;
		playChord();
	} else if (cp == qwpPositional) {
		fX = fY = 0.0f;
		g.s.bMute = true;
		g.bPosTest = true;
		if (qgsScene) {
			delete qgsScene;
			qgiSource = NULL;
			qgsScene = NULL;
		}
		playChord();
	} else {
		g.s.bMute = false;
	}
}

int AudioWizard::nextId() const {
	AudioOutputPtr ao = g.ao;

	int nextid = QWizard::nextId();
	if (currentPage() == qwpTrigger && ! g.s.bPositionalAudio)
		nextid++;
	else if ((currentPage() == qwpDevice) && ! bDelay)
		nextid++;
	return nextid;
}

void AudioWizard::playChord() {
	AudioOutputPtr ao = g.ao;
	if (! ao || aosSource)
		return;
	aosSource = ao->playSample(QLatin1String("skin:wb_male.oga"), true);
}

void AudioWizard::restartAudio() {
	boost::weak_ptr<AudioInput> wai(g.ai);
	boost::weak_ptr<AudioOutput> wao(g.ao);

	aosSource = NULL;

	g.ai.reset();
	g.ao.reset();

	while (! wai.expired() || ! wao.expired()) {
		// Where is QThread::yield() ?
	}

	g.s.qsAudioInput = qcbInput->currentText();
	g.s.qsAudioOutput = qcbOutput->currentText();

	g.ai = AudioInputRegistrar::newFromChoice(g.s.qsAudioInput);
	if (g.ai)
		g.ai->start(QThread::HighestPriority);
	g.ao = AudioOutputRegistrar::newFromChoice(g.s.qsAudioOutput);
	if (g.ao)
		g.ao->start(QThread::HighPriority);

	if (qgsScene) {
		delete qgsScene;
		qgiSource = NULL;
		qgsScene = NULL;
	}

	if (currentPage() == qwpPositional)
		playChord();
}

void AudioWizard::reject() {
	g.s = sOldSettings;

	g.s.lmLoopMode = Settings::None;
	restartAudio();

	QWizard::reject();
}

void AudioWizard::accept() {
	if (! bTransmitChanged)
		g.s.atTransmit = sOldSettings.atTransmit;
	else if (qrPTT->isChecked())
		g.s.atTransmit = Settings::PushToTalk;
	else
		g.s.atTransmit = Settings::VAD;

	g.s.bMute = sOldSettings.bMute;
	g.s.bDeaf = sOldSettings.bDeaf;
	g.s.lmLoopMode = Settings::None;

	const QList<QVariant> &buttons = skwPTT->getShortcut();
	QList<Shortcut> ql;
	bool found = false;
	foreach(Shortcut s, g.s.qlShortcuts) {
		if (s.iIndex == g.mw->gsPushTalk->idx) {
			if (buttons.isEmpty())
				continue;
			else if (! found) {
				s.qlButtons = buttons;
				found = true;
			}
		}
		ql << s;
	}
	if (! found && ! buttons.isEmpty()) {
		Shortcut s;
		s.iIndex = g.mw->gsPushTalk->idx;
		s.bSuppress = false;
		s.qlButtons = buttons;
		ql << s;
	}
	g.s.qlShortcuts = ql;

	g.s.bUsage = qcbUsage->isChecked();
	g.bPosTest = false;
	GlobalShortcutEngine::engine->bNeedRemap = true;
	GlobalShortcutEngine::engine->needRemap();
	restartAudio();
	QWizard::accept();
}

bool AudioWizard::validateCurrentPage() {
	if (currentId() == 1) {
		if ((qcbInput->currentIndex() < 0) || (qcbOutput->currentIndex() < 0))
			return false;
	}
	return true;
}

void AudioWizard::on_Ticker_timeout() {
	AudioInputPtr ai = g.ai;
	AudioOutputPtr ao = g.ao;
	if (! ai || ! ao)
		return;

	int iPeak = static_cast<int>(ai->dMaxMic);

	if (iTicks++ >= 50) {
		iMaxPeak = 0;
		iTicks = 0;
	}
	if (iPeak > iMaxPeak)
		iMaxPeak = iPeak;

	abAmplify->iBelow = qsMaxAmp->value();
	abAmplify->iValue = iPeak;
	abAmplify->iPeak = iMaxPeak;
	abAmplify->update();

	abVAD->iBelow = qsMinVAD->value();
	abVAD->iAbove = qsMaxVAD->value();

	if (g.s.vsVAD == Settings::Amplitude) {
		abVAD->iValue = iroundf((32767.f/96.0f) * (96.0f + ai->dPeakMic));
	} else {
		abVAD->iValue = static_cast<int>(ai->fSpeechProb * 32767.0);
	}
	abVAD->update();

	if (! qgsScene) {
		unsigned int nspeaker = 0;
		const float *spos = ao->getSpeakerPos(nspeaker);
		if ((nspeaker > 0) && spos) {
			qgsScene = new QGraphicsScene(QRectF(-4.0f, -4.0f, 8.0f, 8.0f), this);
			qgsScene->addEllipse(QRectF(-0.12f, -0.12f, 0.24f, 0.24f), QPen(Qt::black), QBrush(Qt::darkRed));
			for (unsigned int i=0;i<nspeaker;++i) {
				if ((spos[3*i] != 0.0f) || (spos[3*i+1] != 0.0f) || (spos[3*i+2] != 0.0f))
					qgsScene->addEllipse(QRectF(spos[3*i] - 0.1f, spos[3*i+2] - 0.1f, 0.2f, 0.2f), QPen(Qt::black), QBrush(Qt::yellow));
			}
			qgiSource = qgsScene->addEllipse(QRectF(-.15f, -.15f, 0.3f, 0.3f), QPen(Qt::black), QBrush(Qt::green));
			qgvView->setScene(qgsScene);
			qgvView->fitInView(-4.0f, -4.0f, 8.0f, 8.0f, Qt::KeepAspectRatio);
		}
	} else if (currentPage() == qwpPositional) {
		float xp, yp;
		if ((fX == 0.0f) && (fY == 0.0f)) {
			fAngle += 0.05f;

			xp = sinf(fAngle) * 2.0f;
			yp = cosf(fAngle) * 2.0f;
		} else {
			xp = fX;
			yp = fY;
		}

		qgiSource->setPos(xp, yp);
		if (aosSource) {
			aosSource->fPos[0] = xp;
			aosSource->fPos[1] = 0;
			aosSource->fPos[2] = yp;
		}
	}
}

void AudioWizard::on_qsMinVAD_valueChanged(int v) {
	g.s.fVADmin = static_cast<float>(v) / 32767.0f;
}

void AudioWizard::on_qsMaxVAD_valueChanged(int v) {
	g.s.fVADmax = static_cast<float>(v) / 32767.0f;
}

void AudioWizard::on_qrSNR_clicked(bool on) {
	if (on) {
		g.s.vsVAD = Settings::SignalToNoise;
		updateTriggerWidgets(false);
		bTransmitChanged = true;
	}
}

void AudioWizard::on_qrAmplitude_clicked(bool on) {
	if (on) {
		g.s.vsVAD = Settings::Amplitude;
		updateTriggerWidgets(false);
		bTransmitChanged = true;
	}
}

void AudioWizard::on_qrPTT_clicked(bool on) {
	if (on) {
		updateTriggerWidgets(true);
		bTransmitChanged = true;
	}
}

void AudioWizard::on_skwPTT_keySet(bool valid) {
	if (valid)
		qrPTT->setChecked(true);
	else if (qrPTT->isChecked())
		qrSNR->setChecked(true);
	updateTriggerWidgets(valid);
	bTransmitChanged = true;
}

void AudioWizard::on_qcbEcho_clicked(bool on) {
	g.s.bEcho = on;
	restartAudio();
}

void AudioWizard::on_qcbHeadphone_clicked(bool on) {
	g.s.bPositionalHeadphone = on;
	restartAudio();
}

void AudioWizard::on_qcbPositional_clicked(bool on) {
	g.s.bPositionalAudio = on;
	restartAudio();
}

void AudioWizard::updateTriggerWidgets(bool ptt) {
	qwVAD->setEnabled(!ptt);
	qwpTrigger->setComplete(!ptt || (skwPTT->qlButtons.count() > 0));
}
