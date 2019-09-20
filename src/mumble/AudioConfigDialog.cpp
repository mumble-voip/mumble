// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008, Andreas Messer <andi@bupfen.de>

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

#include "AudioConfigDialog.h"

#include "AudioInput.h"
#include "AudioOutput.h"
#include "AudioOutputSample.h"
#include "Global.h"
#include "NetworkConfig.h"
#include "Utils.h"

static ConfigWidget *AudioInputDialogNew(Settings &st) {
	return new AudioInputDialog(st);
}

static ConfigWidget *AudioOutputDialogNew(Settings &st) {
	return new AudioOutputDialog(st);
}

static ConfigRegistrar iregistrar(1000, AudioInputDialogNew);
static ConfigRegistrar oregistrar(1010, AudioOutputDialogNew);

void AudioInputDialog::hideEvent(QHideEvent *) {
	qtTick->stop();
}

void AudioInputDialog::showEvent(QShowEvent *) {
	qtTick->start(20);
}

AudioInputDialog::AudioInputDialog(Settings &st) : ConfigWidget(st) {
	qtTick = new QTimer(this);
	qtTick->setObjectName(QLatin1String("Tick"));

	setupUi(this);

	if (AudioInputRegistrar::qmNew) {
		QList<QString> keys = AudioInputRegistrar::qmNew->keys();
		foreach(QString key, keys) {
			qcbSystem->addItem(key);
		}
	}
	qcbSystem->setEnabled(qcbSystem->count() > 1);

	qcbTransmit->addItem(tr("Continuous"), Settings::Continuous);
	qcbTransmit->addItem(tr("Voice Activity"), Settings::VAD);
	qcbTransmit->addItem(tr("Push To Talk"), Settings::PushToTalk);

	abSpeech->qcBelow = Qt::red;
	abSpeech->qcInside = Qt::yellow;
	abSpeech->qcAbove = Qt::green;

	qcbDevice->view()->setTextElideMode(Qt::ElideRight);

	on_qcbPushClick_clicked(g.s.bTxAudioCue);
	on_Tick_timeout();
	on_qcbIdleAction_currentIndexChanged(g.s.iaeIdleAction);
}

QString AudioInputDialog::title() const {
	return tr("Audio Input");
}

QIcon AudioInputDialog::icon() const {
	return QIcon(QLatin1String("skin:config_audio_input.png"));
}

void AudioInputDialog::load(const Settings &r) {
	int i;
	QList<QString> keys;

	if (AudioInputRegistrar::qmNew)
		keys=AudioInputRegistrar::qmNew->keys();
	else
		keys.clear();
	i=keys.indexOf(AudioInputRegistrar::current);
	if (i >= 0)
		loadComboBox(qcbSystem, i);

	loadCheckBox(qcbExclusive, r.bExclusiveInput);

	qlePushClickPathOn->setText(r.qsTxAudioCueOn);
	qlePushClickPathOff->setText(r.qsTxAudioCueOff);

	loadComboBox(qcbTransmit, r.atTransmit);
	loadSlider(qsTransmitHold, r.iVoiceHold);
	loadSlider(qsTransmitMin, iroundf(r.fVADmin * 32767.0f + 0.5f));
	loadSlider(qsTransmitMax, iroundf(r.fVADmax * 32767.0f + 0.5f));
	loadSlider(qsFrames, (r.iFramesPerPacket == 1) ? 1 : (r.iFramesPerPacket/2 + 1));
	loadSlider(qsDoublePush, iroundf(static_cast<float>(r.uiDoublePush) / 1000.f + 0.5f));
	loadSlider(qsPTTHold, static_cast<int>(r.pttHold));

	if (r.vsVAD == Settings::Amplitude)
		qrbAmplitude->setChecked(true);
	else
		qrbSNR->setChecked(true);

	loadCheckBox(qcbPushWindow, r.bShowPTTButtonWindow);
	loadCheckBox(qcbPushClick, r.bTxAudioCue);
	loadSlider(qsQuality, r.iQuality);
	loadCheckBox(qcbAllowLowDelay, r.bAllowLowDelay);
	if (r.iNoiseSuppress != 0)
		loadSlider(qsNoise, - r.iNoiseSuppress);
	else
		loadSlider(qsNoise, 14);
	loadCheckBox(qcbDenoise, r.bDenoise);

#ifdef USE_RNNOISE
	qcbDenoise->setEnabled(SAMPLE_RATE == 48000);
#else
	qcbDenoise->setVisible(false);
#endif

	loadSlider(qsAmp, 20000 - r.iMinLoudness);

	// Idle auto actions
	qsbIdle->setValue(r.iIdleTime / 60);
	loadComboBox(qcbIdleAction, r.iaeIdleAction);
	loadCheckBox(qcbUndoIdleAction, r.bUndoIdleActionUponActivity);

	int echo = 0;
	if (r.bEcho)
		echo = r.bEchoMulti ? 2 : 1;

	loadComboBox(qcbEcho, echo);
}

void AudioInputDialog::save() const {
	s.iQuality = qsQuality->value();
	s.bAllowLowDelay = qcbAllowLowDelay->isChecked();
	s.iNoiseSuppress = (qsNoise->value() == 14) ? 0 : - qsNoise->value();
	s.bDenoise = qcbDenoise->isChecked();
	s.iMinLoudness = 18000 - qsAmp->value() + 2000;
	s.iVoiceHold = qsTransmitHold->value();
	s.fVADmin = static_cast<float>(qsTransmitMin->value()) / 32767.0f;
	s.fVADmax = static_cast<float>(qsTransmitMax->value()) / 32767.0f;
	s.vsVAD = qrbSNR->isChecked() ? Settings::SignalToNoise : Settings::Amplitude;
	s.iFramesPerPacket = qsFrames->value();
	s.iFramesPerPacket = (s.iFramesPerPacket == 1) ? 1 : ((s.iFramesPerPacket-1) * 2);
	s.uiDoublePush = qsDoublePush->value() * 1000;
	s.pttHold = qsPTTHold->value();
	s.atTransmit = static_cast<Settings::AudioTransmit>(qcbTransmit->currentIndex());

	// Idle auto actions
	s.iIdleTime = qsbIdle->value() * 60;
	s.iaeIdleAction = static_cast<Settings::IdleAction>(qcbIdleAction->currentIndex());
	s.bUndoIdleActionUponActivity = qcbUndoIdleAction->isChecked();

	s.bShowPTTButtonWindow = qcbPushWindow->isChecked();
	s.bTxAudioCue = qcbPushClick->isChecked();
	s.qsTxAudioCueOn = qlePushClickPathOn->text();
	s.qsTxAudioCueOff = qlePushClickPathOff->text();

	s.qsAudioInput = qcbSystem->currentText();
	s.bEcho = qcbEcho->currentIndex() > 0;
	s.bEchoMulti = qcbEcho->currentIndex() == 2;
	s.bExclusiveInput = qcbExclusive->isChecked();

	if (AudioInputRegistrar::qmNew) {
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbSystem->currentText());
		int idx = qcbDevice->currentIndex();
		if (idx > -1) {
			air->setDeviceChoice(qcbDevice->itemData(idx), s);
		}
	}
}

void AudioInputDialog::on_qsFrames_valueChanged(int v) {
	qlFrames->setText(tr("%1 ms").arg((v==1) ? 10 : (v-1)*20));
	updateBitrate();
}

void AudioInputDialog::on_qsDoublePush_valueChanged(int v) {
	if (v == 0)
		qlDoublePush->setText(tr("Off"));
	else
		qlDoublePush->setText(tr("%1 ms").arg(v));
}

void AudioInputDialog::on_qsPTTHold_valueChanged(int v) {
	if (v == 0)
		qlPTTHold->setText(tr("Off"));
	else
		qlPTTHold->setText(tr("%1 ms").arg(v));
}

void AudioInputDialog::on_qsTransmitHold_valueChanged(int v) {
	float val = static_cast<float>(v * 10);
	val = val / 1000.0f;
	qlTransmitHold->setText(tr("%1 s").arg(val, 0, 'f', 2));
}

void AudioInputDialog::on_qsQuality_valueChanged(int v) {
	qlQuality->setText(tr("%1 kb/s").arg(static_cast<float>(v) / 1000.0f, 0, 'f', 1));
	updateBitrate();
}

void AudioInputDialog::on_qsNoise_valueChanged(int v) {
	QPalette pal;

	if (v < 15) {
		qlNoise->setText(tr("Off"));
		pal.setColor(qlNoise->foregroundRole(), Qt::red);
	} else {
		qlNoise->setText(tr("-%1 dB").arg(v));
	}
	qlNoise->setPalette(pal);
}

void AudioInputDialog::on_qsAmp_valueChanged(int v) {
	v = 18000 - v + 2000;
	float d = 20000.0f/static_cast<float>(v);
	qlAmp->setText(QString::fromLatin1("%1").arg(d, 0, 'f', 2));
}

void AudioInputDialog::updateBitrate() {
	if (! qsQuality || ! qsFrames || ! qlBitrate)
		return;
	int q = qsQuality->value();
	int p = qsFrames->value();

	int audiorate, overhead, posrate;

	audiorate = q;

	// 50 packets, in bits, IP + UDP + Crypt + type + seq + frameheader
	overhead = 100 * 8 * (20 + 8 + 4 + 1 + 2 + p);

	// TCP is 12 more bytes than UDP
	if (NetworkConfig::TcpModeEnabled())
		overhead += 100 * 8 * 12;

	if (g.s.bTransmitPosition)
		posrate = 12;
	else
		posrate = 0;

	posrate = posrate * 100 * 8;

	overhead = overhead / p;
	posrate = posrate / p;

	int total = audiorate + overhead + posrate;

	QPalette pal;

	if (g.uiSession && (total > g.iMaxBandwidth)) {
		pal.setColor(qlBitrate->foregroundRole(), Qt::red);
	}

	qlBitrate->setPalette(pal);

	QString v = tr("%1 kbit/s (Audio %2, Position %4, Overhead %3)")
	        .arg(total / 1000.0, 0, 'f', 1)
	        .arg(audiorate / 1000.0, 0, 'f', 1)
	        .arg(overhead / 1000.0, 0, 'f', 1)
	        .arg(posrate / 1000.0, 0, 'f', 1);

	qlBitrate->setText(v);

	qsQuality->setMinimum(8000);
}

void AudioInputDialog::on_qcbPushClick_clicked(bool b) {
	qpbPushClickBrowseOn->setEnabled(b);
	qpbPushClickBrowseOff->setEnabled(b);
	qpbPushClickPreview->setEnabled(b);
	qpbPushClickReset->setEnabled(b);
	qlePushClickPathOn->setEnabled(b);
	qlePushClickPathOff->setEnabled(b);
	qlPushClickOn->setEnabled(b);
	qlPushClickOff->setEnabled(b);
}

void AudioInputDialog::on_qpbPushClickBrowseOn_clicked() {
	QString defaultpath(qlePushClickPathOn->text());
	QString qsnew = AudioOutputSample::browseForSndfile(defaultpath);
	if (!qsnew.isEmpty())
		qlePushClickPathOn->setText(qsnew);
}

void AudioInputDialog::on_qpbPushClickBrowseOff_clicked() {
	QString defaultpath(qlePushClickPathOff->text());
	QString qsnew = AudioOutputSample::browseForSndfile(defaultpath);
	if (!qsnew.isEmpty())
		qlePushClickPathOff->setText(qsnew);
}

void AudioInputDialog::on_qpbPushClickPreview_clicked() {
	AudioOutputPtr ao = g.ao;
	if (ao) {
		AudioOutputSample *sample = ao->playSample(qlePushClickPathOn->text());
		if (sample)
			connect(sample, SIGNAL(playbackFinished()), this, SLOT(continuePlayback()));
		else // If we fail to playback the first play on play at least off
			ao->playSample(qlePushClickPathOff->text());

	}
}

void AudioInputDialog::continuePlayback() {
	AudioOutputPtr ao = g.ao;
	if (ao) {
		ao->playSample(qlePushClickPathOff->text());
	}
}

void AudioInputDialog::on_qpbPushClickReset_clicked() {
	qlePushClickPathOn->setText(Settings::cqsDefaultPushClickOn);
	qlePushClickPathOff->setText(Settings::cqsDefaultPushClickOff);
}

void AudioInputDialog::on_qcbTransmit_currentIndexChanged(int v) {
	switch (v) {
		case 0:
			qswTransmit->setCurrentWidget(qwContinuous);
			break;
		case 1:
			qswTransmit->setCurrentWidget(qwVAD);
			break;
		case 2:
			qswTransmit->setCurrentWidget(qwPTT);
			break;
	}
}

void AudioInputDialog::on_qcbSystem_currentIndexChanged(int) {
	qcbDevice->clear();

	QList<audioDevice> ql;

	if (AudioInputRegistrar::qmNew) {
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbSystem->currentText());
		ql = air->getDeviceChoices();

		int idx = 0;

		foreach(audioDevice d, ql) {
			qcbDevice->addItem(d.first, d.second);
			qcbDevice->setItemData(idx, Qt::escape(d.first), Qt::ToolTipRole);
			++idx;
		}

		qcbEcho->setEnabled(air->canEcho(s.qsAudioOutput));
		qcbExclusive->setEnabled(air->canExclusive());
	}

	qcbDevice->setEnabled(ql.count() > 1);
}

void AudioInputDialog::on_Tick_timeout() {
	AudioInputPtr ai = g.ai;

	if (ai.get() == NULL || ! ai->sppPreprocess)
		return;

	abSpeech->iBelow = qsTransmitMin->value();
	abSpeech->iAbove = qsTransmitMax->value();

	if (qrbAmplitude->isChecked()) {
		abSpeech->iValue = iroundf((32767.f/96.0f) * (96.0f + ai->dPeakCleanMic) + 0.5f);
	} else {
		abSpeech->iValue = iroundf(ai->fSpeechProb * 32767.0f + 0.5f);
	}
	abSpeech->update();
}


void AudioInputDialog::on_qcbIdleAction_currentIndexChanged(int v) {
	const Settings::IdleAction action = static_cast<Settings::IdleAction>(v);

	const bool enabled = (action != Settings::Nothing);

	qlIdle->setEnabled(enabled);
	qlIdle2->setEnabled(enabled);
	qsbIdle->setEnabled(enabled);
	qcbUndoIdleAction->setEnabled(enabled);
}

AudioOutputDialog::AudioOutputDialog(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	if (AudioOutputRegistrar::qmNew) {
		QList<QString> keys = AudioOutputRegistrar::qmNew->keys();
		foreach(QString key, keys) {
			qcbSystem->addItem(key);
		}
	}
	qcbSystem->setEnabled(qcbSystem->count() > 1);

	qcbLoopback->addItem(tr("None"), Settings::None);
	qcbLoopback->addItem(tr("Local"), Settings::Local);
	qcbLoopback->addItem(tr("Server"), Settings::Server);

	qcbDevice->view()->setTextElideMode(Qt::ElideRight);
}

QString AudioOutputDialog::title() const {
	return tr("Audio Output");
}

QIcon AudioOutputDialog::icon() const {
	return QIcon(QLatin1String("skin:config_audio_output.png"));
}

void AudioOutputDialog::load(const Settings &r) {
	int i;
	QList<QString> keys;

	if (AudioOutputRegistrar::qmNew)
		keys=AudioOutputRegistrar::qmNew->keys();
	else
		keys.clear();
	i=keys.indexOf(AudioOutputRegistrar::current);
	if (i >= 0)
		loadComboBox(qcbSystem, i);

	loadCheckBox(qcbExclusive, r.bExclusiveOutput);
	loadSlider(qsDelay, r.iOutputDelay);
	loadSlider(qsVolume, iroundf(r.fVolume * 100.0f + 0.5f));
	loadSlider(qsOtherVolume, iroundf((1.0f - r.fOtherVolume) * 100.0f + 0.5f));
	loadCheckBox(qcbAttenuateOthersOnTalk, r.bAttenuateOthersOnTalk);
	loadCheckBox(qcbAttenuateOthers, r.bAttenuateOthers);
	loadCheckBox(qcbAttenuateUsersOnPrioritySpeak, r.bAttenuateUsersOnPrioritySpeak);
	loadCheckBox(qcbOnlyAttenuateSameOutput, r.bOnlyAttenuateSameOutput);
	loadCheckBox(qcbAttenuateLoopbacks, r.bAttenuateLoopbacks);
	if (AudioOutputRegistrar::current == QLatin1String("PulseAudio")) {
		qgbAdvancedAttenuation->setVisible(true);
	} else {
		qgbAdvancedAttenuation->setVisible(false);
	}
	loadSlider(qsJitter, r.iJitterBufferSize);
	loadComboBox(qcbLoopback, r.lmLoopMode);
	loadSlider(qsPacketDelay, static_cast<int>(r.dMaxPacketDelay));
	loadSlider(qsPacketLoss, iroundf(r.dPacketLoss * 100.0f + 0.5f));
	loadSlider(qsMinDistance, iroundf(r.fAudioMinDistance * 10.0f + 0.5f));
	loadSlider(qsMaxDistance, iroundf(r.fAudioMaxDistance * 10.0f + 0.5f));
	loadSlider(qsMaxDistVolume, iroundf(r.fAudioMaxDistVolume * 100.0f + 0.5f));
	loadSlider(qsBloom, iroundf(r.fAudioBloom * 100.0f + 0.5f));
	loadCheckBox(qcbHeadphones, r.bPositionalHeadphone);
	loadCheckBox(qcbPositional, r.bPositionalAudio);

	qsOtherVolume->setEnabled(r.bAttenuateOthersOnTalk || r.bAttenuateOthers);
	qlOtherVolume->setEnabled(r.bAttenuateOthersOnTalk || r.bAttenuateOthers);
	qcbAttenuateLoopbacks->setEnabled(r.bOnlyAttenuateSameOutput);
}

void AudioOutputDialog::save() const {
	s.iOutputDelay = qsDelay->value();
	s.fVolume = static_cast<float>(qsVolume->value()) / 100.0f;
	s.fOtherVolume = 1.0f - (static_cast<float>(qsOtherVolume->value()) / 100.0f);
	s.bAttenuateOthersOnTalk = qcbAttenuateOthersOnTalk->isChecked();
	s.bAttenuateOthers = qcbAttenuateOthers->isChecked();
	s.bOnlyAttenuateSameOutput = qcbOnlyAttenuateSameOutput->isChecked();
	s.bAttenuateLoopbacks = qcbAttenuateLoopbacks->isChecked();
	s.bAttenuateUsersOnPrioritySpeak = qcbAttenuateUsersOnPrioritySpeak->isChecked();
	s.iJitterBufferSize = qsJitter->value();
	s.qsAudioOutput = qcbSystem->currentText();
	s.lmLoopMode = static_cast<Settings::LoopMode>(qcbLoopback->currentIndex());
	s.dMaxPacketDelay = static_cast<float>(qsPacketDelay->value());
	s.dPacketLoss = static_cast<float>(qsPacketLoss->value()) / 100.0f;
	s.fAudioMinDistance = static_cast<float>(qsMinDistance->value()) / 10.0f;
	s.fAudioMaxDistance = static_cast<float>(qsMaxDistance->value()) / 10.0f;
	s.fAudioMaxDistVolume = static_cast<float>(qsMaxDistVolume->value()) / 100.0f;
	s.fAudioBloom = static_cast<float>(qsBloom->value()) / 100.0f;
	s.bPositionalAudio = qcbPositional->isChecked();
	s.bPositionalHeadphone = qcbHeadphones->isChecked();
	s.bExclusiveOutput = qcbExclusive->isChecked();


	if (AudioOutputRegistrar::qmNew) {
		AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew->value(qcbSystem->currentText());
		int idx = qcbDevice->currentIndex();
		if (idx > -1) {
			aor->setDeviceChoice(qcbDevice->itemData(idx), s);
		}
	}
}

void AudioOutputDialog::on_qcbSystem_currentIndexChanged(int) {
	qcbDevice->clear();

	QList<audioDevice> ql;

	if (AudioOutputRegistrar::qmNew) {
		AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew->value(qcbSystem->currentText());
		ql = aor->getDeviceChoices();

		int idx = 0;

		foreach(audioDevice d, ql) {
			qcbDevice->addItem(d.first, d.second);
			qcbDevice->setItemData(idx, Qt::escape(d.first), Qt::ToolTipRole);
			++idx;
		}
		bool canmute = aor->canMuteOthers();
		qsOtherVolume->setEnabled(canmute);
		qcbAttenuateOthersOnTalk->setEnabled(canmute);
		if (aor->name == QLatin1String("PulseAudio")) {
			qgbAdvancedAttenuation->setVisible(true);
		} else {
			qgbAdvancedAttenuation->setVisible(false);
		}
		qcbAttenuateOthers->setEnabled(canmute);
		qlOtherVolume->setEnabled(canmute);

		bool usesdelay = aor->usesOutputDelay();
		qsDelay->setEnabled(usesdelay);
		qlDelay->setEnabled(usesdelay);

		qcbExclusive->setEnabled(aor->canExclusive());
	}

	qcbDevice->setEnabled(ql.count() > 1);
}

void AudioOutputDialog::on_qsJitter_valueChanged(int v) {
	qlJitter->setText(tr("%1 ms").arg(v*10));
}

void AudioOutputDialog::on_qsVolume_valueChanged(int v) {
	QPalette pal;

	if (v > 100) {
		pal.setColor(qlVolume->foregroundRole(), Qt::red);
	}
	qlVolume->setPalette(pal);

	qlVolume->setText(tr("%1 %").arg(v));
}

void AudioOutputDialog::on_qsOtherVolume_valueChanged(int v) {
	qlOtherVolume->setText(tr("%1 %").arg(v));
}

void AudioOutputDialog::on_qsPacketDelay_valueChanged(int v) {
	qlPacketDelay->setText(tr("%1 ms").arg(v));
}

void AudioOutputDialog::on_qsPacketLoss_valueChanged(int v) {
	qlPacketLoss->setText(tr("%1 %").arg(v));
}

void AudioOutputDialog::on_qsDelay_valueChanged(int v) {
	qlDelay->setText(tr("%1 ms").arg(v*10));
}

void AudioOutputDialog::on_qcbLoopback_currentIndexChanged(int v) {
	bool ena = false;
	if (v == 1)
		ena = true;

	qsPacketDelay->setEnabled(ena);
	qlPacketDelay->setEnabled(ena);
	qsPacketLoss->setEnabled(ena);
	qlPacketLoss->setEnabled(ena);
}

void AudioOutputDialog::on_qsMinDistance_valueChanged(int v) {
	qlMinDistance->setText(tr("%1 m").arg(v/10.0, 0, 'f', 1));
	if (qsMaxDistance->value() < v)
		qsMaxDistance->setValue(v);
}

void AudioOutputDialog::on_qsMaxDistance_valueChanged(int v) {
	qlMaxDistance->setText(tr("%1 m").arg(v/10.0, 0, 'f', 1));
	if (qsMinDistance->value() > v)
		qsMinDistance->setValue(v);
}

void AudioOutputDialog::on_qsMaxDistVolume_valueChanged(int v) {
	qlMaxDistVolume->setText(tr("%1 %").arg(v));
}

void AudioOutputDialog::on_qsBloom_valueChanged(int v) {
	qlBloom->setText(tr("%1 %").arg(v+100));
}

void AudioOutputDialog::on_qcbPositional_stateChanged(int v) {
	qgbVolume->setEnabled(v);
}

void AudioOutputDialog::on_qcbAttenuateOthersOnTalk_clicked(bool checked) {
	bool b = qcbAttenuateOthers->isChecked() || checked;
	qsOtherVolume->setEnabled(b);
	qlOtherVolume->setEnabled(b);
	qgbAdvancedAttenuation->setEnabled(b);
}

void AudioOutputDialog::on_qcbAttenuateOthers_clicked(bool checked) {
	bool b = qcbAttenuateOthersOnTalk->isChecked() || checked;
	qsOtherVolume->setEnabled(b);
	qlOtherVolume->setEnabled(b);
	qgbAdvancedAttenuation->setEnabled(b);
}

void AudioOutputDialog::on_qcbOnlyAttenuateSameOutput_clicked(bool checked) {
	qcbAttenuateLoopbacks->setEnabled(checked);
}
