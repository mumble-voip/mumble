// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioConfigDialog.h"

#include "Accessibility.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "AudioOutputSample.h"
#include "AudioOutputToken.h"
#include "NetworkConfig.h"
#include "Utils.h"
#include "Global.h"

#include <QSignalBlocker>

#include <cstdint>

const QString AudioOutputDialog::name = QLatin1String("AudioOutputWidget");
const QString AudioInputDialog::name  = QLatin1String("AudioInputWidget");


static ConfigWidget *AudioInputDialogNew(Settings &st) {
	return new AudioInputDialog(st);
}

static ConfigWidget *AudioOutputDialogNew(Settings &st) {
	return new AudioOutputDialog(st);
}

static ConfigRegistrar registrarAudioInputDialog(1000, AudioInputDialogNew);
static ConfigRegistrar registrarAudioOutputDialog(1010, AudioOutputDialogNew);

void AudioInputDialog::hideEvent(QHideEvent *) {
	qtTick->stop();
}

void AudioInputDialog::showEvent(QShowEvent *) {
	qtTick->start(20);

	// In case the user has changed the audio output device and is now coming
	// back.
	updateEchoEnableState();
}

AudioInputDialog::AudioInputDialog(Settings &st) : ConfigWidget(st) {
	qtTick = new QTimer(this);
	qtTick->setObjectName(QLatin1String("Tick"));

	setupUi(this);

	qlInputHelp->setVisible(false);

	if (AudioInputRegistrar::qmNew) {
		QList< QString > keys = AudioInputRegistrar::qmNew->keys();
		foreach (QString key, keys) { qcbSystem->addItem(key); }
	}
	qcbSystem->setEnabled(qcbSystem->count() > 1);

	qcbTransmit->addItem(tr("Continuous"), Settings::Continuous);
	qcbTransmit->addItem(tr("Voice Activity"), Settings::VAD);
	qcbTransmit->addItem(tr("Push To Talk"), Settings::PushToTalk);

	abSpeech->qcBelow  = Qt::red;
	abSpeech->qcInside = Qt::yellow;
	abSpeech->qcAbove  = Qt::green;

	qcbDevice->view()->setTextElideMode(Qt::ElideRight);

	on_qcbMuteCue_clicked(Global::get().s.bTxMuteCue);
	on_Tick_timeout();
	on_qcbIdleAction_currentIndexChanged(Global::get().s.iaeIdleAction);

	// Hide the slider by default
	showSpeexNoiseSuppressionSlider(false);

#ifndef USE_RENAMENOISE
	// Hide options related to ReNameNoise
	qrbNoiseSupReNameNoise->setVisible(false);
	qrbNoiseSupBoth->setVisible(false);
#endif
}

QString AudioInputDialog::title() const {
	return tr("Audio Input");
}

const QString &AudioInputDialog::getName() const {
	return AudioInputDialog::name;
}

QIcon AudioInputDialog::icon() const {
	return QIcon(QLatin1String("skin:config_audio_input.png"));
}

void AudioInputDialog::load(const Settings &r) {
	QList< QString > keys;

	if (AudioInputRegistrar::qmNew)
		keys = AudioInputRegistrar::qmNew->keys();
	else
		keys.clear();

	const auto index = keys.indexOf(AudioInputRegistrar::current);
	if (index >= 0)
		loadComboBox(qcbSystem, static_cast< int >(index));

	verifyMicrophonePermission();

	loadCheckBox(qcbExclusive, r.bExclusiveInput);

	qlePushClickPathOn->setText(r.qsTxAudioCueOn);
	qlePushClickPathOff->setText(r.qsTxAudioCueOff);
	qleMuteCuePath->setText(r.qsTxMuteCue);
	s.muteCueShown = r.muteCueShown;


	loadComboBox(qcbTransmit, r.atTransmit);
	loadSlider(qsTransmitHold, r.iVoiceHold);
	loadSlider(qsTransmitMin, static_cast< int >(r.fVADmin * 32767.0f + 0.5f));
	loadSlider(qsTransmitMax, static_cast< int >(r.fVADmax * 32767.0f + 0.5f));
	loadSlider(qsFrames, (r.iFramesPerPacket == 1) ? 1 : (r.iFramesPerPacket / 2 + 1));
	loadSlider(qsDoublePush, static_cast< int >(static_cast< float >(r.uiDoublePush) / 1000.f + 0.5f));
	loadSlider(qsPTTHold, static_cast< int >(r.pttHold));

	if (r.vsVAD == Settings::Amplitude)
		qrbAmplitude->setChecked(true);
	else
		qrbSNR->setChecked(true);

	loadCheckBox(qcbPushWindow, r.bShowPTTButtonWindow);
	loadCheckBox(qcbEnableCuePTT, r.audioCueEnabledPTT);
	loadCheckBox(qcbEnableCueVAD, r.audioCueEnabledVAD);
	updateAudioCueEnabled();
	loadCheckBox(qcbMuteCue, r.bTxMuteCue);
	loadSlider(qsQuality, r.iQuality);
	loadCheckBox(qcbAllowLowDelay, r.bAllowLowDelay);
	if (r.iSpeexNoiseCancelStrength != 0) {
		loadSlider(qsSpeexNoiseSupStrength, -r.iSpeexNoiseCancelStrength);
	} else {
		loadSlider(qsSpeexNoiseSupStrength, 14);
	}

	bool allowReNameNoise = SAMPLE_RATE == 48000;

	if (!allowReNameNoise) {
		const QString tooltip = QObject::tr("RNNoise is not available due to a sample rate mismatch.");
		qrbNoiseSupReNameNoise->setEnabled(false);
		qrbNoiseSupReNameNoise->setToolTip(tooltip);
		qrbNoiseSupBoth->setEnabled(false);
		qrbNoiseSupBoth->setToolTip(tooltip);
	}

	switch (r.noiseCancelMode) {
		case Settings::NoiseCancelOff:
			loadCheckBox(qrbNoiseSupDeactivated, true);
			break;
		case Settings::NoiseCancelSpeex:
			loadCheckBox(qrbNoiseSupSpeex, true);
			break;
		case Settings::NoiseCancelRNN:
#ifdef USE_RENAMENOISE
			if (allowReNameNoise) {
				loadCheckBox(qrbNoiseSupReNameNoise, true);
			} else {
				// We have to switch to speex as a fallback
				loadCheckBox(qrbNoiseSupSpeex, true);
			}
#else
			// We have to switch to speex as a fallback
			loadCheckBox(qrbNoiseSupSpeex, true);
#endif
			break;
		case Settings::NoiseCancelBoth:
#ifdef USE_RENAMENOISE
			if (allowReNameNoise) {
				loadCheckBox(qrbNoiseSupBoth, true);
			} else {
				// We have to switch to speex as a fallback
				loadCheckBox(qrbNoiseSupSpeex, true);
			}
#else
			// We have to switch to speex as a fallback
			loadCheckBox(qrbNoiseSupSpeex, true);
#endif
			break;
	}

	loadSlider(qsAmp, 20000 - r.iMinLoudness);

	// Idle auto actions
	qsbIdle->setValue(static_cast< int >(r.iIdleTime) / 60);
	loadComboBox(qcbIdleAction, r.iaeIdleAction);
	loadCheckBox(qcbUndoIdleAction, r.bUndoIdleActionUponActivity);

	updateEchoEnableState();
}

void AudioInputDialog::verifyMicrophonePermission() {
	if (!AudioInputRegistrar::qmNew) {
		return;
	}

	AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbSystem->currentText());
	if (air->isMicrophoneAccessDeniedByOS()) {
		qcbDevice->setEnabled(false);
		if (air->name == QLatin1String("CoreAudio")) {
			qlInputHelp->setVisible(true);
			qlInputHelp->setText(
				tr("Access to the microphone was denied. Please allow Mumble to use the microphone "
				   "by changing the settings in System Preferences -> Security & Privacy -> Privacy -> "
				   "Microphone."));
		} else if (air->name == QLatin1String("WASAPI")) {
			qlInputHelp->setVisible(true);
			qlInputHelp->setText(tr("Access to the microphone was denied. Please check that your operating system's "
									"microphone settings allow Mumble to use the microphone."));
		}
	} else {
		qcbDevice->setEnabled(true);
		qlInputHelp->setVisible(false);
		qlInputHelp->setText("");
	}
}

void AudioInputDialog::save() const {
	s.iQuality                  = qsQuality->value();
	s.bAllowLowDelay            = qcbAllowLowDelay->isChecked();
	s.iSpeexNoiseCancelStrength = (qsSpeexNoiseSupStrength->value() == 14) ? 0 : -qsSpeexNoiseSupStrength->value();

	if (qrbNoiseSupDeactivated->isChecked()) {
		s.noiseCancelMode = Settings::NoiseCancelOff;
	} else if (qrbNoiseSupBoth->isChecked()) {
		s.noiseCancelMode = Settings::NoiseCancelBoth;
	} else if (qrbNoiseSupReNameNoise->isChecked()) {
		s.noiseCancelMode = Settings::NoiseCancelRNN;
	} else {
		s.noiseCancelMode = Settings::NoiseCancelSpeex;
	}

	s.iMinLoudness     = 18000 - qsAmp->value() + 2000;
	s.iVoiceHold       = qsTransmitHold->value();
	s.fVADmin          = static_cast< float >(qsTransmitMin->value()) / 32767.0f;
	s.fVADmax          = static_cast< float >(qsTransmitMax->value()) / 32767.0f;
	s.vsVAD            = qrbSNR->isChecked() ? Settings::SignalToNoise : Settings::Amplitude;
	s.iFramesPerPacket = qsFrames->value();
	s.iFramesPerPacket = (s.iFramesPerPacket == 1) ? 1 : ((s.iFramesPerPacket - 1) * 2);
	s.uiDoublePush     = static_cast< unsigned int >(qsDoublePush->value() * 1000);
	s.pttHold          = static_cast< quint64 >(qsPTTHold->value());
	s.atTransmit       = static_cast< Settings::AudioTransmit >(qcbTransmit->currentIndex());

	// Idle auto actions
	s.iIdleTime                   = static_cast< unsigned int >(qsbIdle->value() * 60);
	s.iaeIdleAction               = static_cast< Settings::IdleAction >(qcbIdleAction->currentIndex());
	s.bUndoIdleActionUponActivity = qcbUndoIdleAction->isChecked();

	s.bShowPTTButtonWindow = qcbPushWindow->isChecked();
	s.audioCueEnabledPTT   = qcbEnableCuePTT->isChecked();
	s.audioCueEnabledVAD   = qcbEnableCueVAD->isChecked();
	s.qsTxAudioCueOn       = qlePushClickPathOn->text();
	s.qsTxAudioCueOff      = qlePushClickPathOff->text();

	s.bTxMuteCue  = qcbMuteCue->isChecked();
	s.qsTxMuteCue = qleMuteCuePath->text();

	s.qsAudioInput    = qcbSystem->currentText();
	s.echoOption      = static_cast< EchoCancelOptionID >(qcbEcho->currentData().toInt());
	s.bExclusiveInput = qcbExclusive->isChecked();

	if (AudioInputRegistrar::qmNew) {
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbSystem->currentText());
		int idx                  = qcbDevice->currentIndex();
		if (idx > -1) {
			air->setDeviceChoice(qcbDevice->itemData(idx), s);
		}
	}
}

void AudioInputDialog::on_qsFrames_valueChanged(int v) {
	int val = (v == 1) ? 10 : (v - 1) * 20;
	qlFrames->setText(tr("%1 ms").arg(val));
	updateBitrate();

	Mumble::Accessibility::setSliderSemanticValue(qsFrames, QString("%1 %2").arg(val).arg(tr("milliseconds")));
}

void AudioInputDialog::on_qsDoublePush_valueChanged(int v) {
	if (v == 0) {
		qlDoublePush->setText(tr("Off"));
		Mumble::Accessibility::setSliderSemanticValue(qsDoublePush, tr("Off"));
	} else {
		qlDoublePush->setText(tr("%1 ms").arg(v));
		Mumble::Accessibility::setSliderSemanticValue(qsDoublePush, QString("%1 %2").arg(v).arg(tr("milliseconds")));
	}
}

void AudioInputDialog::on_qsPTTHold_valueChanged(int v) {
	if (v == 0) {
		qlPTTHold->setText(tr("Off"));
		Mumble::Accessibility::setSliderSemanticValue(qsPTTHold, tr("Off"));
	} else {
		qlPTTHold->setText(tr("%1 ms").arg(v));
		Mumble::Accessibility::setSliderSemanticValue(qsPTTHold, QString("%1 %2").arg(v).arg(tr("milliseconds")));
	}
}

void AudioInputDialog::on_qsTransmitHold_valueChanged(int v) {
	float val = static_cast< float >(v * 10);
	val       = val / 1000.0f;
	qlTransmitHold->setText(tr("%1 s").arg(val, 0, 'f', 2));
	Mumble::Accessibility::setSliderSemanticValue(qsTransmitHold,
												  QString("%1 %2").arg(val, 0, 'f', 2).arg(tr("seconds")));
}

void AudioInputDialog::on_qsQuality_valueChanged(int v) {
	qlQuality->setText(tr("%1 kb/s").arg(static_cast< float >(v) / 1000.0f, 0, 'f', 1));
	updateBitrate();

	Mumble::Accessibility::setSliderSemanticValue(
		qsQuality, QString("%1 %2").arg(static_cast< float >(v) / 1000.0f, 0, 'f', 1).arg(tr("kilobits per second")));
}

void AudioInputDialog::on_qsSpeexNoiseSupStrength_valueChanged(int v) {
	QPalette pal;

	if (v < 15) {
		qlSpeexNoiseSupStrength->setText(tr("Off"));
		pal.setColor(qlSpeexNoiseSupStrength->foregroundRole(), Qt::red);
		Mumble::Accessibility::setSliderSemanticValue(qsSpeexNoiseSupStrength, tr("Off"));
	} else {
		qlSpeexNoiseSupStrength->setText(tr("-%1 dB").arg(v));
		Mumble::Accessibility::setSliderSemanticValue(qsSpeexNoiseSupStrength,
													  QString("-%1 %2").arg(v).arg(tr("decibels")));
	}
	qlSpeexNoiseSupStrength->setPalette(pal);
}

void AudioInputDialog::on_qsAmp_valueChanged(int v) {
	v       = 18000 - v + 2000;
	float d = 20000.0f / static_cast< float >(v);
	qlAmp->setText(QString::fromLatin1("%1").arg(d, 0, 'f', 2));

	Mumble::Accessibility::setSliderSemanticValue(qsAmp, QString("%1").arg(d, 0, 'f', 2));
}

void AudioInputDialog::on_qsTransmitMin_valueChanged() {
	Mumble::Accessibility::setSliderSemanticValue(qsTransmitMin, Mumble::Accessibility::SliderMode::READ_PERCENT, "%");
}

void AudioInputDialog::on_qsTransmitMax_valueChanged() {
	Mumble::Accessibility::setSliderSemanticValue(qsTransmitMax, Mumble::Accessibility::SliderMode::READ_PERCENT, "%");
}

void AudioInputDialog::updateBitrate() {
	if (!qsQuality || !qsFrames || !qlBitrate)
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

	if (Global::get().s.bTransmitPosition)
		posrate = 12;
	else
		posrate = 0;

	posrate = posrate * 100 * 8;

	overhead = overhead / p;
	posrate  = posrate / p;

	int total = audiorate + overhead + posrate;

	QPalette pal;

	if (Global::get().uiSession && (total > Global::get().iMaxBandwidth)) {
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

	qsQuality->setAccessibleDescription(v);
	qsFrames->setAccessibleDescription(v);
}

void AudioInputDialog::on_qcbEnableCuePTT_clicked() {
	updateAudioCueEnabled();
}

void AudioInputDialog::on_qcbEnableCueVAD_clicked() {
	updateAudioCueEnabled();
}

void AudioInputDialog::updateAudioCueEnabled() {
	bool enabled = qcbEnableCuePTT->isChecked() || qcbEnableCueVAD->isChecked();

	qpbPushClickBrowseOn->setEnabled(enabled);
	qpbPushClickBrowseOff->setEnabled(enabled);
	qpbPushClickPreview->setEnabled(enabled);
	qpbPushClickReset->setEnabled(enabled);
	qlePushClickPathOn->setEnabled(enabled);
	qlePushClickPathOff->setEnabled(enabled);
	qlPushClickOn->setEnabled(enabled);
	qlPushClickOff->setEnabled(enabled);
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
	AudioOutputPtr ao = Global::get().ao;
	if (ao) {
		AudioOutputToken sample = ao->playSample(qlePushClickPathOn->text(), Global::get().s.cueVolume);
		if (sample) {
			sample.connect< AudioOutputSample >(&AudioOutputSample::playbackFinished, *this,
												&AudioInputDialog::continuePlayback);
		} else {
			// If we fail to playback the first play on play at least off
			ao->playSample(qlePushClickPathOff->text(), Global::get().s.cueVolume);
		}
	}
}

void AudioInputDialog::on_qcbMuteCue_clicked(bool b) {
	qleMuteCuePath->setEnabled(b);
	qpbMuteCueBrowse->setEnabled(b);
	qpbMuteCuePreview->setEnabled(b);
	s.muteCueShown = true;
}

void AudioInputDialog::on_qpbMuteCueBrowse_clicked() {
	QString defaultpath(qleMuteCuePath->text());
	QString qsnew = AudioOutputSample::browseForSndfile(defaultpath);
	if (!qsnew.isEmpty())
		qleMuteCuePath->setText(qsnew);
}


void AudioInputDialog::on_qpbMuteCuePreview_clicked() {
	AudioOutputPtr ao = Global::get().ao;
	if (ao) {
		ao->playSample(qleMuteCuePath->text(), Global::get().s.cueVolume);
	}
}

void AudioInputDialog::continuePlayback() {
	AudioOutputPtr ao = Global::get().ao;
	if (ao) {
		ao->playSample(qlePushClickPathOff->text(), Global::get().s.cueVolume);
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

	QList< audioDevice > choices;

	if (AudioInputRegistrar::qmNew) {
		auto air         = AudioInputRegistrar::qmNew->value(qcbSystem->currentText());
		QVariant current = air->getDeviceChoice();
		choices          = air->getDeviceChoices();

		for (int i = 0; i < choices.size(); ++i) {
			auto &choice = choices.at(i);
			qcbDevice->addItem(choice.first, choice.second);
			qcbDevice->setItemData(i, choice.first.toHtmlEscaped(), Qt::ToolTipRole);

			if (choice.second == current) {
				qcbDevice->setCurrentIndex(i);
			}
		}

		updateEchoEnableState();

		qcbExclusive->setEnabled(air->canExclusive());
	}

	qcbDevice->setEnabled(!choices.isEmpty());
	verifyMicrophonePermission();
}

void AudioInputDialog::updateEchoEnableState() {
	AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbSystem->currentText());

	AudioOutputDialog *outputWidget =
		static_cast< AudioOutputDialog * >(ConfigDialog::getConfigWidget(AudioOutputDialog::name));
	QString outputInterface;
	if (outputWidget) {
		outputInterface = outputWidget->getCurrentlySelectedOutputInterfaceName();
	} else {
		// Fallback for when the outputWIdget is not (yet) available -> use the value from the current settings
		outputInterface = s.qsAudioOutput;
	}

	qcbEcho->clear();
	qcbEcho->setToolTip(QObject::tr("If enabled this tries to cancel out echo from the audio stream."));
	qcbEcho->setCurrentIndex(0);

	bool hasUsableEchoOption = false;

	qcbEcho->insertItem(0, tr("Disabled"), "disabled");
	qcbEcho->setItemData(0, tr("Disable echo cancellation."), Qt::ToolTipRole);

	int i = 0;
	for (EchoCancelOptionID echoid : air->echoOptions) {
		if (air->canEcho(echoid, outputInterface)) {
			++i;
			hasUsableEchoOption                = true;
			const EchoCancelOption &echoOption = EchoCancelOption::getOptions()[static_cast< std::size_t >(echoid)];
			qcbEcho->insertItem(i, echoOption.description, static_cast< int >(echoid));
			qcbEcho->setItemData(i, echoOption.explanation, Qt::ToolTipRole);
			if (s.echoOption == echoid) {
				qcbEcho->setCurrentIndex(i);
			}
		}
	}

	if (hasUsableEchoOption) {
		qcbEcho->setEnabled(true);
	} else {
		qcbEcho->setCurrentIndex(0);
		qcbEcho->setEnabled(false);
		qcbEcho->setToolTip(QObject::tr("Echo cancellation is not supported for the interface "
										"combination \"%1\" (in) and \"%2\" (out).")
								.arg(air->name)
								.arg(outputInterface));
	}
}

void AudioInputDialog::showSpeexNoiseSuppressionSlider(bool show) {
	qlSpeexNoiseSup->setVisible(show);
	qsSpeexNoiseSupStrength->setVisible(show);
	qlSpeexNoiseSupStrength->setVisible(show);
}

void AudioInputDialog::on_Tick_timeout() {
	AudioInputPtr ai = Global::get().ai;

	if (!ai.get() || !ai->m_preprocessor)
		return;

	abSpeech->iBelow = qsTransmitMin->value();
	abSpeech->iAbove = qsTransmitMax->value();

	if (qrbAmplitude->isChecked()) {
		abSpeech->iValue = static_cast< int >((32767.f / 96.0f) * (96.0f + ai->dPeakCleanMic) + 0.5f);
	} else {
		abSpeech->iValue = static_cast< int >(ai->fSpeechProb * 32767.0f + 0.5f);
	}
	abSpeech->update();
}


void AudioInputDialog::on_qcbIdleAction_currentIndexChanged(int v) {
	const Settings::IdleAction action = static_cast< Settings::IdleAction >(v);

	const bool enabled = (action != Settings::Nothing);

	qlIdle->setEnabled(enabled);
	qlIdle2->setEnabled(enabled);
	qsbIdle->setEnabled(enabled);
	qcbUndoIdleAction->setEnabled(enabled);
}

void AudioInputDialog::on_qrbNoiseSupSpeex_toggled(bool checked) {
	showSpeexNoiseSuppressionSlider(checked);
}

void AudioInputDialog::on_qrbNoiseSupBoth_toggled(bool checked) {
	showSpeexNoiseSuppressionSlider(checked);
}

void AudioOutputDialog::enablePulseAudioAttenuationOptionsFor(const QString &outputName) {
	if (outputName == QLatin1String("PulseAudio")) {
		qcbOnlyAttenuateSameOutput->show();
		qcbAttenuateLoopbacks->show();
	} else {
		qcbOnlyAttenuateSameOutput->hide();
		qcbAttenuateLoopbacks->hide();
	}
}

AudioOutputDialog::AudioOutputDialog(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	if (AudioOutputRegistrar::qmNew) {
		QList< QString > keys = AudioOutputRegistrar::qmNew->keys();
		foreach (QString key, keys) { qcbSystem->addItem(key); }
	}
	qcbSystem->setEnabled(qcbSystem->count() > 1);

	qcbLoopback->addItem(tr("None"), Settings::None);
	qcbLoopback->addItem(tr("Local"), Settings::Local);
	qcbLoopback->addItem(tr("Server"), Settings::Server);

	qcbDevice->view()->setTextElideMode(Qt::ElideRight);

	// Distance in cm
	qsMinDistance->setRange(0, 200);
	// Distance in m
	qsbMinimumDistance->setRange(0.0, 1000.0);
	// Distance in cm
	qsMaxDistance->setRange(10, 2000);
	// Distance in m
	qsbMaximumDistance->setRange(1.0, 10000.0);
	qsMinimumVolume->setRange(0, 100);
	qsbMinimumVolume->setRange(qsMinimumVolume->minimum(), qsMinimumVolume->maximum());
	qsBloom->setRange(0, 75);
	qsbBloom->setRange(qsBloom->minimum(), qsBloom->maximum());

	QString minDistanceTooltip = tr("Distance at which audio volume from another player starts decreasing");
	QString maxDistanceTooltip = tr("Distance at which a player's audio volume has reached its minimum value");
	QString minVolumeTooltip =
		tr("The minimum volume a player's audio will fade out to with increasing distance. Set to 0% for it to fade "
		   "into complete silence for a realistic maximum hearing distance.");
	QString bloomTooltip = tr("If an audio source is close enough, blooming will cause the audio to be played on all "
							  "speakers more or less regardless of their position (albeit with lower volume)");

	qlMinDistance->setToolTip(minDistanceTooltip);
	qsMinDistance->setToolTip(minDistanceTooltip);
	qsbMinimumDistance->setToolTip(minDistanceTooltip);
	qlMaxDistance->setToolTip(maxDistanceTooltip);
	qsMaxDistance->setToolTip(maxDistanceTooltip);
	qsbMaximumDistance->setToolTip(maxDistanceTooltip);
	qlMinimumVolume->setToolTip(minVolumeTooltip);
	qsMinimumVolume->setToolTip(minVolumeTooltip);
	qsbMinimumVolume->setToolTip(minVolumeTooltip);
	qlBloom->setToolTip(bloomTooltip);
	qsBloom->setToolTip(bloomTooltip);
	qsbBloom->setToolTip(bloomTooltip);
}

QString AudioOutputDialog::title() const {
	return tr("Audio Output");
}

const QString &AudioOutputDialog::getName() const {
	return AudioOutputDialog::name;
}

QIcon AudioOutputDialog::icon() const {
	return QIcon(QLatin1String("skin:config_audio_output.png"));
}

QString AudioOutputDialog::getCurrentlySelectedOutputInterfaceName() const {
	if (qcbSystem) {
		return qcbSystem->currentText();
	}

	return QString();
}

void AudioOutputDialog::load(const Settings &r) {
	QList< QString > keys;

	if (AudioOutputRegistrar::qmNew)
		keys = AudioOutputRegistrar::qmNew->keys();
	else
		keys.clear();

	const auto index = keys.indexOf(AudioOutputRegistrar::current);
	if (index >= 0)
		loadComboBox(qcbSystem, static_cast< int >(index));

	loadCheckBox(qcbExclusive, r.bExclusiveOutput);
	loadSlider(qsDelay, r.iOutputDelay);
	loadSlider(qsVolume, static_cast< int >(r.fVolume * 100.0f + 0.5f));
	loadSlider(qsOtherVolume, static_cast< int >((1.0f - r.fOtherVolume) * 100.0f + 0.5f));
	loadCheckBox(qcbAttenuateOthersOnTalk, r.bAttenuateOthersOnTalk);
	loadCheckBox(qcbAttenuateOthers, r.bAttenuateOthers);
	loadCheckBox(qcbAttenuateUsersOnPrioritySpeak, r.bAttenuateUsersOnPrioritySpeak);
	loadCheckBox(qcbOnlyAttenuateSameOutput, r.bOnlyAttenuateSameOutput);
	loadCheckBox(qcbAttenuateLoopbacks, r.bAttenuateLoopbacks);

	const bool attenuationActive = r.bAttenuateOthers || r.bAttenuateOthersOnTalk;
	qcbOnlyAttenuateSameOutput->setEnabled(attenuationActive);
	qcbAttenuateLoopbacks->setEnabled(attenuationActive && r.bOnlyAttenuateSameOutput);

	enablePulseAudioAttenuationOptionsFor(AudioOutputRegistrar::current);

	loadSlider(qsJitter, r.iJitterBufferSize);
	loadComboBox(qcbLoopback, r.lmLoopMode);
	loadSlider(qsPacketDelay, static_cast< int >(r.dMaxPacketDelay));
	loadSlider(qsPacketLoss, static_cast< int >(r.dPacketLoss * 100.0f + 0.5f));
	qsbMinimumDistance->setValue(r.fAudioMinDistance);
	qsbMaximumDistance->setValue(r.fAudioMaxDistance);
	qsbMinimumVolume->setValue(static_cast< int >(r.fAudioMaxDistVolume * 100));
	qsbBloom->setValue(static_cast< int >(r.fAudioBloom * 100));
	loadCheckBox(qcbHeadphones, r.bPositionalHeadphone);
	loadCheckBox(qcbPositional, r.bPositionalAudio);

	qsOtherVolume->setEnabled(r.bAttenuateOthersOnTalk || r.bAttenuateOthers);
	qlOtherVolume->setEnabled(r.bAttenuateOthersOnTalk || r.bAttenuateOthers);
	qcbAttenuateLoopbacks->setEnabled(r.bOnlyAttenuateSameOutput);
}

void AudioOutputDialog::save() const {
	s.iOutputDelay                   = qsDelay->value();
	s.fVolume                        = static_cast< float >(qsVolume->value()) / 100.0f;
	s.fOtherVolume                   = 1.0f - (static_cast< float >(qsOtherVolume->value()) / 100.0f);
	s.bAttenuateOthersOnTalk         = qcbAttenuateOthersOnTalk->isChecked();
	s.bAttenuateOthers               = qcbAttenuateOthers->isChecked();
	s.bOnlyAttenuateSameOutput       = qcbOnlyAttenuateSameOutput->isChecked();
	s.bAttenuateLoopbacks            = qcbAttenuateLoopbacks->isChecked();
	s.bAttenuateUsersOnPrioritySpeak = qcbAttenuateUsersOnPrioritySpeak->isChecked();
	s.iJitterBufferSize              = qsJitter->value();
	s.qsAudioOutput                  = qcbSystem->currentText();
	s.lmLoopMode                     = static_cast< Settings::LoopMode >(qcbLoopback->currentIndex());
	s.dMaxPacketDelay                = static_cast< float >(qsPacketDelay->value());
	s.dPacketLoss                    = static_cast< float >(qsPacketLoss->value()) / 100.0f;
	s.fAudioMinDistance              = static_cast< float >(qsbMinimumDistance->value());
	s.fAudioMaxDistance              = static_cast< float >(qsbMaximumDistance->value());
	s.fAudioMaxDistVolume            = static_cast< float >(qsbMinimumVolume->value()) / 100.0f;
	s.fAudioBloom                    = static_cast< float >(qsbBloom->value()) / 100.0f;
	s.bPositionalAudio               = qcbPositional->isChecked();
	s.bPositionalHeadphone           = qcbHeadphones->isChecked();
	s.bExclusiveOutput               = qcbExclusive->isChecked();


	if (AudioOutputRegistrar::qmNew) {
		AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew->value(qcbSystem->currentText());
		int idx                   = qcbDevice->currentIndex();
		if (idx > -1) {
			aor->setDeviceChoice(qcbDevice->itemData(idx), s);
		}
	}
}

void AudioOutputDialog::on_qcbSystem_currentIndexChanged(int) {
	qcbDevice->clear();

	QList< audioDevice > choices;

	if (AudioOutputRegistrar::qmNew) {
		auto aor         = AudioOutputRegistrar::qmNew->value(qcbSystem->currentText());
		QVariant current = aor->getDeviceChoice();
		choices          = aor->getDeviceChoices();

		for (int i = 0; i < choices.size(); ++i) {
			auto &choice = choices.at(i);
			qcbDevice->addItem(choice.first, choice.second);
			qcbDevice->setItemData(i, choice.first.toHtmlEscaped(), Qt::ToolTipRole);

			if (choice.second == current) {
				qcbDevice->setCurrentIndex(i);
			}
		}

		bool canmute = aor->canMuteOthers();
		qsOtherVolume->setEnabled(canmute);
		qcbAttenuateOthersOnTalk->setEnabled(canmute);

		enablePulseAudioAttenuationOptionsFor(aor->name);

		qcbAttenuateOthers->setEnabled(canmute);
		qlOtherVolume->setEnabled(canmute);

		bool usesdelay = aor->usesOutputDelay();
		qsDelay->setEnabled(usesdelay);
		qlDelay->setEnabled(usesdelay);

		qcbExclusive->setEnabled(aor->canExclusive());
	}

	qcbDevice->setEnabled(!choices.isEmpty());
}

void AudioOutputDialog::on_qsJitter_valueChanged(int v) {
	qlJitter->setText(tr("%1 ms").arg(v * 10));
	Mumble::Accessibility::setSliderSemanticValue(qsJitter, QString("%1 %2").arg(v * 10).arg(tr("milliseconds")));
}

void AudioOutputDialog::on_qsVolume_valueChanged(int v) {
	QPalette pal;

	if (v > 100) {
		pal.setColor(qlVolume->foregroundRole(), Qt::red);
	}
	qlVolume->setPalette(pal);

	qlVolume->setText(tr("%1 %").arg(v));
	Mumble::Accessibility::setSliderSemanticValue(qsVolume, Mumble::Accessibility::SliderMode::READ_PERCENT, "%");
}

void AudioOutputDialog::on_qsOtherVolume_valueChanged(int v) {
	qlOtherVolume->setText(tr("%1 %").arg(v));
	Mumble::Accessibility::setSliderSemanticValue(qsOtherVolume, Mumble::Accessibility::SliderMode::READ_PERCENT, "%");
}

void AudioOutputDialog::on_qsPacketDelay_valueChanged(int v) {
	qlPacketDelay->setText(tr("%1 ms").arg(v));
	Mumble::Accessibility::setSliderSemanticValue(qsPacketDelay, QString("%1 %2").arg(v).arg(tr("milliseconds")));
}

void AudioOutputDialog::on_qsPacketLoss_valueChanged(int v) {
	qlPacketLoss->setText(tr("%1 %").arg(v));
	Mumble::Accessibility::setSliderSemanticValue(qsPacketLoss, Mumble::Accessibility::SliderMode::READ_PERCENT, "%");
}

void AudioOutputDialog::on_qsDelay_valueChanged(int v) {
	qlDelay->setText(tr("%1 ms").arg(v * 10));
	Mumble::Accessibility::setSliderSemanticValue(qsDelay, QString("%1 %2").arg(v * 10).arg(tr("milliseconds")));
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

void AudioOutputDialog::on_qsMinDistance_valueChanged(int value) {
	QSignalBlocker blocker(qsbMinimumDistance);
	qsbMinimumDistance->setValue(value / 10.0);

	// Ensure that max distance is always a least 1m larger than min distance
	qsbMaximumDistance->setValue(std::max(qsbMaximumDistance->value(), (value / 10.0) + 1));
}

void AudioOutputDialog::on_qsbMinimumDistance_valueChanged(double value) {
	QSignalBlocker blocker(qsMinDistance);
	qsMinDistance->setValue(static_cast< int >(value * 10));
	Mumble::Accessibility::setSliderSemanticValue(qsMinDistance, QString("%1 %2").arg(value * 10).arg(tr("meters")));

	// Ensure that max distance is always a least 1m larger than min distance
	qsMaxDistance->setValue(std::max(qsMaxDistance->value(), static_cast< int >(value * 10) + 10));
}

void AudioOutputDialog::on_qsMaxDistance_valueChanged(int value) {
	QSignalBlocker blocker(qsbMaximumDistance);
	qsbMaximumDistance->setValue(value / 10.0);

	// Ensure that min distance is always a least 1m less than max distance
	qsbMinimumDistance->setValue(std::min(qsbMinimumDistance->value(), (value / 10.0) - 1));
}
void AudioOutputDialog::on_qsbMaximumDistance_valueChanged(double value) {
	QSignalBlocker blocker(qsMaxDistance);
	qsMaxDistance->setValue(static_cast< int >(value * 10));
	Mumble::Accessibility::setSliderSemanticValue(qsMaxDistance, QString("%1 %2").arg(value * 10).arg(tr("meters")));

	// Ensure that min distance is always a least 1m less than max distance
	qsMinDistance->setValue(std::min(qsMinDistance->value(), static_cast< int >(value * 10) - 10));
}

void AudioOutputDialog::on_qsMinimumVolume_valueChanged(int value) {
	QSignalBlocker blocker(qsbMinimumVolume);
	qsbMinimumVolume->setValue(value);
	Mumble::Accessibility::setSliderSemanticValue(qsMinimumVolume, Mumble::Accessibility::SliderMode::READ_PERCENT,
												  "%");
}

void AudioOutputDialog::on_qsbMinimumVolume_valueChanged(int value) {
	QSignalBlocker blocker(qsMinimumVolume);
	qsMinimumVolume->setValue(value);
}

void AudioOutputDialog::on_qsBloom_valueChanged(int value) {
	QSignalBlocker blocker(qsbBloom);
	qsbBloom->setValue(value);
	Mumble::Accessibility::setSliderSemanticValue(qsBloom, Mumble::Accessibility::SliderMode::READ_PERCENT, "%");
}

void AudioOutputDialog::on_qsbBloom_valueChanged(int value) {
	QSignalBlocker blocker(qsBloom);
	qsBloom->setValue(value);
}

void AudioOutputDialog::on_qcbAttenuateOthersOnTalk_clicked(bool checked) {
	bool b = qcbAttenuateOthers->isChecked() || checked;
	qsOtherVolume->setEnabled(b);
	qlOtherVolume->setEnabled(b);

	qcbOnlyAttenuateSameOutput->setEnabled(b);
	qcbAttenuateLoopbacks->setEnabled(b && qcbOnlyAttenuateSameOutput->isChecked());
}

void AudioOutputDialog::on_qcbAttenuateOthers_clicked(bool checked) {
	bool b = qcbAttenuateOthersOnTalk->isChecked() || checked;
	qsOtherVolume->setEnabled(b);
	qlOtherVolume->setEnabled(b);

	qcbOnlyAttenuateSameOutput->setEnabled(b);
	qcbAttenuateLoopbacks->setEnabled(b && qcbOnlyAttenuateSameOutput->isChecked());
}

void AudioOutputDialog::on_qcbOnlyAttenuateSameOutput_clicked(bool checked) {
	qcbAttenuateLoopbacks->setEnabled(checked);
}
