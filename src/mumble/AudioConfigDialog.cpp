/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#include "AudioInput.h"
#include "AudioOutput.h"
#include "AudioConfigDialog.h"
#include "Global.h"

static ConfigWidget *AudioConfigDialogNew() {
	return new AudioConfigDialog();
}

static ConfigRegistrar registrar(10, AudioConfigDialogNew);

AudioConfigDialog::AudioConfigDialog(QWidget *p) : ConfigWidget(p) {
	QList<QString> keys;
	QString key;
	int i;

	setupUi(this);

	keys=AudioInputRegistrar::qmNew->keys();
	foreach(key, keys) {
		qcbInput->addItem(key);
	}
	i=keys.indexOf(AudioInputRegistrar::current);
	if (i >= 0)
		qcbInput->setCurrentIndex(i);

	keys=AudioOutputRegistrar::qmNew->keys();
	foreach(key, keys) {
		qcbOutput->addItem(key);
	}
	i=keys.indexOf(AudioOutputRegistrar::current);
	if (i >= 0)
		qcbOutput->setCurrentIndex(i);

	qcbTransmit->addItem(tr("Continuous"), Settings::Continous);
	qcbTransmit->addItem(tr("Voice Activity"), Settings::VAD);
	qcbTransmit->addItem(tr("Push To Talk"), Settings::PushToTalk);
	qcbTransmit->setCurrentIndex(g.s.atTransmit);

	qsTransmitHold->setValue(g.s.iVoiceHold);
	on_qsTransmitHold_valueChanged(qsTransmitHold->value());

	qsFrames->setValue(g.s.iFramesPerPacket);
	on_qsFrames_valueChanged(qsFrames->value());

	qcbPushClick->setChecked(g.s.bPushClick);
	qcbTCP->setChecked(g.s.bTCPCompat);
	qcbReconnect->setChecked(g.s.bReconnect);

	qcbReconnect = new QCheckBox(tr("Automatic Reconnect"));
	qcbReconnect->setToolTip(tr("Reconnect when disconnected"));

	qsQuality->setValue(g.s.iQuality);
	on_qsQuality_valueChanged(qsQuality->value());

	qsComplexity->setValue(g.s.iComplexity);
	on_qsComplexity_valueChanged(qsComplexity->value());

	qsNoise->setValue(- g.s.iNoiseSuppress);
	on_qsNoise_valueChanged(qsNoise->value());

	qsAmp->setValue(20000 - g.s.iMinLoudness);
	on_qsAmp_valueChanged(qsAmp->value());

	qsJitter->setValue(g.s.iJitterBufferSize);
	on_qsJitter_valueChanged(qsJitter->value());

	qcbLoopback->addItem(tr("None"), Global::None);
	qcbLoopback->addItem(tr("Local"), Global::Local);
	qcbLoopback->addItem(tr("Server"), Global::Server);
	qcbLoopback->setCurrentIndex(g.lmLoopMode);

	qsPacketDelay->setValue(static_cast<int>(g.dMaxPacketDelay));
	on_qsPacketDelay_valueChanged(qsPacketDelay->value());

	qsPacketLoss->setValue(static_cast<int>(g.dPacketLoss * 100.0));
	on_qsPacketLoss_valueChanged(qsPacketLoss->value());

	on_qcbTransmit_currentIndexChanged(qcbTransmit->currentIndex());
	on_qcbLoopback_currentIndexChanged(qcbLoopback->currentIndex());
}

QString AudioConfigDialog::title() const {
	return tr("Basic Audio");
}

QIcon AudioConfigDialog::icon() const {
	return QIcon(QLatin1String("skin:config_basic.png"));
}

void AudioConfigDialog::accept() {
	g.s.iQuality = qsQuality->value();
	g.s.iNoiseSuppress = - qsNoise->value();
	g.s.iComplexity = qsComplexity->value();
	g.s.iMinLoudness = 18000 - qsAmp->value() + 2000;
	g.s.iVoiceHold = qsTransmitHold->value();
	g.s.iFramesPerPacket = qsFrames->value();
	g.s.bPushClick = qcbPushClick->isChecked();
	g.s.bTCPCompat = qcbTCP->isChecked();
	g.s.bReconnect = qcbReconnect->isChecked();
	g.s.iJitterBufferSize = qsJitter->value();
	g.s.atTransmit = static_cast<Settings::AudioTransmit>(qcbTransmit->currentIndex());
	g.s.qsAudioInput = qcbInput->currentText();
	g.s.qsAudioOutput = qcbOutput->currentText();
	g.lmLoopMode = static_cast<Global::LoopMode>(qcbLoopback->currentIndex());
	g.dMaxPacketDelay = qsPacketDelay->value();
	g.dPacketLoss = qsPacketLoss->value() / 100.0;
}

void AudioConfigDialog::on_qsFrames_valueChanged(int v) {
	qlFrames->setText(tr("%1 ms").arg(v*20));
	updateBitrate();
}

void AudioConfigDialog::on_qsTransmitHold_valueChanged(int v) {
	double val = v * 20;
	val = val / 1000.0;
	qlTransmitHold->setText(tr("%1 s").arg(val, 0, 'f', 2));
}

void AudioConfigDialog::on_qsQuality_valueChanged(int v) {
	qlQuality->setText(QString::number(v));
	updateBitrate();
}

void AudioConfigDialog::on_qsNoise_valueChanged(int v) {
	qlNoise->setText(tr("-%1 dB").arg(v));
}

void AudioConfigDialog::on_qsComplexity_valueChanged(int v) {
	qlComplexity->setText(QString::number(v));
}

void AudioConfigDialog::on_qsAmp_valueChanged(int v) {
	v = 18000 - v + 2000;
	double d = 20000.0/v;
	qlAmp->setText(QString::fromLatin1("%1").arg(d, 0, 'f', 2));
}

void AudioConfigDialog::on_qsJitter_valueChanged(int v) {
	qlJitter->setText(tr("%1 ms").arg(v*20));
}

void AudioConfigDialog::on_qsPacketDelay_valueChanged(int v) {
	qlPacketDelay->setText(tr("%1 ms").arg(v));
}

void AudioConfigDialog::on_qsPacketLoss_valueChanged(int v) {
	qlPacketLoss->setText(tr("%1%").arg(v));
}


void AudioConfigDialog::updateBitrate() {
	if (! qsQuality || ! qsFrames || ! qlBitrate)
		return;
	int q = qsQuality->value();
	int p = qsFrames->value();

	int audiorate, overhead, posrate;
	float f = q;
	void *es;

	es = speex_encoder_init(&speex_wb_mode);
	speex_encoder_ctl(es,SPEEX_SET_VBR_QUALITY, &f);
	speex_encoder_ctl(es,SPEEX_GET_BITRATE,&audiorate);
	speex_encoder_destroy(es);

	// 50 packets, in bits, IP + UDP + Crypt + type/id (Message header) + flags + seq
	overhead = 50 * 8 * (20 + 8 + 4 + 3 + 1 + 2);

	// TCP is 12 more bytes than UDP
	if (qcbTCP->isChecked())
		overhead += 50 * 8 * 12;

	if (g.s.bTransmitPosition)
		posrate = 12;
	else
		posrate = 0;

	posrate = posrate * 50 * 8;

	overhead = overhead / p;
	posrate = posrate / p;

	int total = audiorate + overhead + posrate;

	QPalette pal;

	if ((total / 8 > g.iMaxBandwidth) && g.uiSession) {
		pal.setColor(qlBitrate->foregroundRole(), Qt::red);
	}

	qlBitrate->setPalette(pal);

	QString v = tr("%1kbit/s (Audio %2, Position %4, Overhead %3)").arg(total / 1000.0, 0, 'f', 1).arg(audiorate / 1000.0, 0, 'f', 1).arg(overhead / 1000.0, 0, 'f', 1).arg(posrate / 1000.0, 0, 'f', 1);
	qlBitrate->setText(v);
}

void AudioConfigDialog::on_qcbTransmit_currentIndexChanged(int v) {
	bool cue = false;
	bool hold = false;

	switch (v) {
		case 1:
			hold = true;
			break;
		case 2:
			cue = true;
			break;
	}

	qcbPushClick->setEnabled(cue);
	qsTransmitHold->setEnabled(hold);
	qlTransmitHold->setEnabled(hold);
}

void AudioConfigDialog::on_qcbLoopback_currentIndexChanged(int v) {
	bool ena = false;
	if (v == 1)
		ena = true;

	qsPacketDelay->setEnabled(ena);
	qlPacketDelay->setEnabled(ena);
	qsPacketLoss->setEnabled(ena);
	qlPacketLoss->setEnabled(ena);
}
