// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioWizard.h"

#include "AudioInput.h"
#include "AudioOutputSample.h"
#include "Log.h"
#include "MainWindow.h"
#include "Utils.h"

#include <QtGui/QMouseEvent>
#include <QtWidgets/QGraphicsEllipseItem>

#include <cmath>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

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
	bLastActive = false;
	g.bInAudioWizard = true;

	ticker = new QTimer(this);
	ticker->setObjectName(QLatin1String("Ticker"));

	setupUi(this);

	// Done
	qcbUsage->setChecked(g.s.bUsage);

	// Device
	if (AudioInputRegistrar::qmNew) {
		foreach(AudioInputRegistrar *air, *AudioInputRegistrar::qmNew) {
			qcbInput->addItem(air->name);
			if (air->name == AudioInputRegistrar::current) {
				qcbInput->setCurrentIndex(qcbInput->count() - 1);
				qcbEcho->setEnabled(air->canEcho(qcbOutput->currentText()));
			}
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
				qcbAttenuateOthers->setEnabled(aor->canMuteOthers());
			}
			QList<audioDevice> ql= aor->getDeviceChoices();
		}
	}

	if (qcbOutput->count() < 2) {
		qcbOutput->setEnabled(false);
	}

	qcbHighContrast->setChecked(g.s.bHighContrast);
	on_qcbHighContrast_clicked(g.s.bHighContrast);
#ifdef Q_OS_WIN
	// On windows we can autodetect this
	qcbHighContrast->setVisible(false);
#endif

	// Settings
	if (g.s.iQuality == 16000 && g.s.iFramesPerPacket == 6)
		qrbQualityLow->setChecked(true);
	else if (g.s.iQuality == 40000 && g.s.iFramesPerPacket == 2)
		qrbQualityBalanced->setChecked(true);
	else if (g.s.iQuality == 72000 && g.s.iFramesPerPacket == 1)
		qrbQualityUltra->setChecked(true);
	else
		qrbQualityCustom->setChecked(true);

	quint32 iMessage = Settings::LogNone;
	for (int i = Log::firstMsgType;i <= Log::lastMsgType; ++i) {
		iMessage |= (g.s.qmMessages[i] & (Settings::LogSoundfile | Settings::LogTTS));
	}

#ifdef USE_NO_TTS
	qrbNotificationCustom->setChecked(false);
	qrbNotificationCustom->setDisabled(true);
	qrbNotificationTTS->setChecked(false);
	qrbNotificationTTS->setDisabled(true);
	qrbNotificationSounds->setChecked(true);
#else
	if (iMessage == Settings::LogTTS && g.s.bTTS)
		qrbNotificationTTS->setChecked(true);
	else if (iMessage == Settings::LogSoundfile)
		qrbNotificationSounds->setChecked(true);
	else // If we find mixed message types or only tts with main tts disable assume custom
		qrbNotificationCustom->setChecked(true);
	qrbNotificationCustom->setVisible(qrbNotificationCustom->isChecked());
#endif

	qrbQualityCustom->setVisible(qrbQualityCustom->isChecked());
	qlQualityCustom->setVisible(qrbQualityCustom->isChecked());

	qcbPositional->setChecked(g.s.bPositionalAudio);
	qcbAttenuateOthers->setChecked(g.s.bAttenuateOthers);

	on_qcbInput_activated(qcbInput->currentIndex());
	on_qcbOutput_activated(qcbOutput->currentIndex());

	abAmplify->qcBelow = Qt::blue;
	abAmplify->qcInside = Qt::green;
	abAmplify->qcAbove = Qt::red;

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

	qsVAD->setValue(iroundf(g.s.fVADmax * 32767.f + 0.5f));

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

	bTransmitChanged = false;

	iMaxPeak = 0;
	iTicks = 0;

	qpTalkingOn = QPixmap::fromImage(QImage(QLatin1String("skin:talking_on.svg")).scaled(64,64));
	qpTalkingOff = QPixmap::fromImage(QImage(QLatin1String("skin:talking_off.svg")).scaled(64,64));

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

	Audio::stopInput();

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

	qcbAttenuateOthers->setEnabled(aor->canMuteOthers());

	qcbOutputDevice->setEnabled(ql.count() > 1);

	on_qcbOutputDevice_activated(0);
}

void AudioWizard::on_qcbOutputDevice_activated(int) {
	if (bInit)
		return;

	if (! AudioOutputRegistrar::qmNew)
		return;

	Audio::stopOutput();

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
	qlOutputDelay->setText(tr("%1 ms").arg(v*10));
	g.s.iOutputDelay = v;
	restartAudio();
}

void AudioWizard::on_qsMaxAmp_valueChanged(int v) {
	g.s.iMinLoudness = qMin(v, 30000);
}

void AudioWizard::showPage(int pageid) {
	if (pageid == -1)
		return;

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

	if ((cp == qwpTrigger) || (cp == qwpSettings)) {
		if (! bTransmitChanged)
			g.s.atTransmit = sOldSettings.atTransmit;
		else if (qrPTT->isChecked())
			g.s.atTransmit = Settings::PushToTalk;
		else
			g.s.atTransmit = Settings::VAD;
	} else {
		g.s.atTransmit = Settings::Continuous;
	}
}

int AudioWizard::nextId() const {
	AudioOutputPtr ao = g.ao;

	int nextid = QWizard::nextId();
	if (currentPage() == qwpSettings && ! g.s.bPositionalAudio)
		nextid++;
	else if ((currentPage() == qwpDevice) && ! bDelay)
		nextid++;
	return nextid;
}

void AudioWizard::playChord() {
	AudioOutputPtr ao = g.ao;
	if (! ao || aosSource || bInit)
		return;
	aosSource = ao->playSample(QLatin1String(":/wb_male.oga"), true);
}

void AudioWizard::restartAudio() {
	aosSource = NULL;

	Audio::stop();

	g.s.qsAudioInput = qcbInput->currentText();
	g.s.qsAudioOutput = qcbOutput->currentText();

	Audio::start();

	if (qgsScene) {
		delete qgsScene;
		qgiSource = NULL;
		qgsScene = NULL;
	}

	if ((currentPage() == qwpPositional) || (currentPage() == qwpDeviceTuning))
		playChord();
}

void AudioWizard::reject() {
	g.s = sOldSettings;

	g.s.lmLoopMode = Settings::None;
	restartAudio();

	AudioOutputPtr ao = g.ao;
	if (ao)
		ao->wipe();
	aosSource = NULL;
	g.bInAudioWizard = false;

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

	// Switch TTS<->Sounds according to user selection
	if (!qrbNotificationCustom->isChecked()) {
		Settings::MessageLog mlReplace = qrbNotificationTTS->isChecked() ? Settings::LogSoundfile : Settings::LogTTS;

		for (int i = Log::firstMsgType;i <= Log::lastMsgType; ++i) {
			if (g.s.qmMessages[i] & mlReplace)
				g.s.qmMessages[i] ^= Settings::LogSoundfile | Settings::LogTTS;
		}

		if (qrbNotificationTTS->isChecked()) {
			g.s.bTTS = true;
			g.mw->qaAudioTTS->setChecked(true);
		}
	}

	g.s.bUsage = qcbUsage->isChecked();
	g.bPosTest = false;
	restartAudio();
	g.bInAudioWizard = false;
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

	abVAD->iBelow = iroundf(g.s.fVADmin * 32767.0f + 0.5f);
	abVAD->iAbove = iroundf(g.s.fVADmax * 32767.0f + 0.5f);

	if (g.s.vsVAD == Settings::Amplitude) {
		abVAD->iValue = iroundf((32767.f/96.0f) * (96.0f + ai->dPeakCleanMic) + 0.5f);
	} else {
		abVAD->iValue = iroundf(ai->fSpeechProb * 32767.0f + 0.5f);
	}
	abVAD->update();

	bool active = ai->isTransmitting();
	if (active != bLastActive) {
		bLastActive = active;
		qlTalkIcon->setPixmap(active ? qpTalkingOn : qpTalkingOff);
	}

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

void AudioWizard::on_qsVAD_valueChanged(int v) {
	if (! bInit) {
		g.s.fVADmax = static_cast<float>(v) / 32767.0f;
		g.s.fVADmin = g.s.fVADmax * 0.9f;
	}
}

void AudioWizard::on_qrSNR_clicked(bool on) {
	if (on) {
		g.s.vsVAD = Settings::SignalToNoise;
		g.s.atTransmit = Settings::VAD;
		updateTriggerWidgets(false);
		bTransmitChanged = true;
	}
}

void AudioWizard::on_qrAmplitude_clicked(bool on) {
	if (on) {
		g.s.vsVAD = Settings::Amplitude;
		g.s.atTransmit = Settings::VAD;
		updateTriggerWidgets(false);
		bTransmitChanged = true;
	}
}

void AudioWizard::on_qrPTT_clicked(bool on) {
	if (on) {
		g.s.atTransmit = Settings::PushToTalk;
		updateTriggerWidgets(true);
		bTransmitChanged = true;
	}
}

void AudioWizard::on_skwPTT_keySet(bool valid, bool last) {
	if (valid)
		qrPTT->setChecked(true);
	else if (qrPTT->isChecked())
		qrAmplitude->setChecked(true);
	updateTriggerWidgets(valid);
	bTransmitChanged = true;

	if (last) {

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
		GlobalShortcutEngine::engine->bNeedRemap = true;
		GlobalShortcutEngine::engine->needRemap();
	}
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
	g.s.bTransmitPosition = on;
	restartAudio();
}

void AudioWizard::updateTriggerWidgets(bool ptt) {
	qwVAD->setEnabled(!ptt);
	qwpTrigger->setComplete(!ptt || (skwPTT->qlButtons.count() > 0));
}

void AudioWizard::on_qcbAttenuateOthers_clicked(bool checked) {
	g.s.bAttenuateOthers = checked;
}

void AudioWizard::on_qcbHighContrast_clicked(bool on) {
	g.s.bHighContrast = on;

	qliAmpTuningText->setVisible(!g.s.bHighContrast);
	qliAmpTuningTextHC->setVisible(g.s.bHighContrast);

	qliVolumeTuningText->setVisible(!g.s.bHighContrast);
	qliVolumeTuningTextHC->setVisible(g.s.bHighContrast);

	qliVadTuningText->setVisible(!g.s.bHighContrast);
	qliVadTuningTextHC->setVisible(g.s.bHighContrast);
}

void AudioWizard::on_qrbQualityLow_clicked() {
	g.s.iQuality = 16000;
	g.s.iFramesPerPacket = 6;
	restartAudio();
}

void AudioWizard::on_qrbQualityBalanced_clicked() {
	g.s.iQuality = 40000;
	g.s.iFramesPerPacket = 2;
	restartAudio();
}

void AudioWizard::on_qrbQualityUltra_clicked() {
	g.s.iQuality = 72000;
	g.s.iFramesPerPacket = 1;
	restartAudio();
}

void AudioWizard::on_qrbQualityCustom_clicked() {
	g.s.iQuality = sOldSettings.iQuality;
	g.s.iFramesPerPacket = sOldSettings.iFramesPerPacket;
	restartAudio();
}
