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
#include "AudioInput.h"
#include "AudioOutput.h"
#include "AudioConfigDialog.h"
#include "Global.h"

// Things to include
// atTransmit (ptt, voice etc)
// vad grace (extra frames to count)
// quality, complexity, max amplification

AudioConfigDialog::AudioConfigDialog(QWidget *p) : ConfigWidget(p) {
	QGroupBox *qgbInterface, *qgbTransmit, *qgbCompress;
	QLabel *l;
	QHBoxLayout *h;
	QVBoxLayout *v;
	QGridLayout *grid;
	QList<QString> keys;
	QString key;
	int i;
	QString tip;

	qgbInterface=new QGroupBox(tr("Interfaces"));
	qgbTransmit=new QGroupBox(tr("Transmission"));
	qgbCompress=new QGroupBox(tr("Compression"));

	grid = new QGridLayout();

	qcbInput = new QComboBox();
	keys=AudioInputRegistrar::qmNew->keys();
	foreach(key, keys) {
		qcbInput->addItem(key);
	}
	i=keys.indexOf(AudioInputRegistrar::current);
	if (i >= 0)
		qcbInput->setCurrentIndex(i);

	l = new QLabel(tr("Input"));
	l->setBuddy(qcbInput);

	tip=tr("<b>This is the input method to use for audio.</b><br />Most likely you want to use DirectSound.");
	qcbInput->setToolTip(tip);
	l->setToolTip(tip);

	grid->addWidget(l, 0, 0);
	grid->addWidget(qcbInput, 0, 1);

	qcbOutput = new QComboBox();
	keys=AudioOutputRegistrar::qmNew->keys();
	foreach(key, keys) {
		qcbOutput->addItem(key);
	}
	i=keys.indexOf(AudioOutputRegistrar::current);
	if (i >= 0)
		qcbOutput->setCurrentIndex(i);

	l = new QLabel(tr("Output"));
	l->setBuddy(qcbOutput);

	tip=tr("<b>This is the output method to use for audio.</b><br />Most likely you want to use DirectSound.");
	qcbOutput->setToolTip(tip);
	l->setToolTip(tip);

	grid->addWidget(l, 1, 0);
	grid->addWidget(qcbOutput, 1, 1);

	qgbInterface->setLayout(grid);


	grid = new QGridLayout();

	qcbTransmit = new QComboBox();
	qcbTransmit->addItem(tr("Continous"), Settings::Continous);
	qcbTransmit->addItem(tr("Voice Activity"), Settings::VAD);
	qcbTransmit->addItem(tr("Push To Talk"), Settings::PushToTalk);
	qcbTransmit->setCurrentIndex(g.s.atTransmit);
	l = new QLabel(tr("Transmit"));
	l->setBuddy(qcbTransmit);

	tip=tr("<b>When should speech be transmitted?</b><br />"
			"<i>Continous</i> - All the time<br />"
			"<i>Voice Activity</i> - When you are speaking clearly.<br />"
			"<i>Push To Talk</i> - When you hold down the hotkey set in Config|Shortcuts.");
	qcbTransmit->setToolTip(tip);
	l->setToolTip(tip);

	grid->addWidget(l, 0, 0);
	grid->addWidget(qcbTransmit, 0, 1, 1, 2);

	qsTransmitHold = new QSlider(Qt::Horizontal);
	qsTransmitHold->setRange(20, 50);
	qsTransmitHold->setSingleStep(1);
	qsTransmitHold->setPageStep(5);
	qsTransmitHold->setValue(g.s.iVoiceHold);
	qsTransmitHold->setObjectName("TransmitHold");

	l = new QLabel(tr("Voice Hold"));
	l->setBuddy(qsTransmitHold);

	qlTransmitHold=new QLabel();
	qlTransmitHold->setMinimumWidth(40);
	on_TransmitHold_valueChanged(qsTransmitHold->value());

	tip=tr("<b>How long after a percieved stop in speech should we continue to transmit?</b><br />"
			"Set this higher if your voice breaks up when you speak (seen by a rapidly strobing voice icon next to your name)<br />"
			"Only makes sense when used with Voice Activity transmission");
	qlTransmitHold->setToolTip(tip);
	qsTransmitHold->setToolTip(tip);
	l->setToolTip(tip);

	grid->addWidget(l, 1, 0);
	grid->addWidget(qsTransmitHold, 1, 1);
	grid->addWidget(qlTransmitHold, 1, 2);

	qgbTransmit->setLayout(grid);


	grid = new QGridLayout();

	qsQuality = new QSlider(Qt::Horizontal);
	qsQuality->setRange(4, 10);
	qsQuality->setSingleStep(1);
	qsQuality->setPageStep(2);
	qsQuality->setValue(g.s.iQuality);
	qsQuality->setObjectName("Quality");

	l = new QLabel(tr("Quality"));
	l->setBuddy(qsQuality);

	qlQuality=new QLabel();
	qlQuality->setMinimumWidth(30);
	on_Quality_valueChanged(qsQuality->value());

	tip=tr("<b>Quality of compression.</b><br />"
			"This determines how much bandwidth Mumble is allowed to use.");
	qsQuality->setToolTip(tip);
	qlQuality->setToolTip(tip);
	l->setToolTip(tip);

	grid->addWidget(l, 0, 0);
	grid->addWidget(qsQuality, 0, 1);
	grid->addWidget(qlQuality, 0, 2);

	qsComplexity = new QSlider(Qt::Horizontal);
	qsComplexity->setRange(4, 10);
	qsComplexity->setSingleStep(1);
	qsComplexity->setPageStep(2);
	qsComplexity->setValue(g.s.iComplexity);
	qsComplexity->setObjectName("Complexity");

	l = new QLabel(tr("Complexity"));
	l->setBuddy(qsComplexity);

	qlComplexity=new QLabel();
	on_Complexity_valueChanged(qsComplexity->value());

	tip=tr("<b>Complexity of compression.</b><br />"
			"This determines how much CPU Mumble is allowed to use to increase sent"
			"voice quality. Settings above 5 give only marginal gain.");
	qsComplexity->setToolTip(tip);
	qlComplexity->setToolTip(tip);
	l->setToolTip(tip);

	grid->addWidget(l, 1, 0);
	grid->addWidget(qsComplexity, 1, 1);
	grid->addWidget(qlComplexity, 1, 2);

	qsAmp = new QSlider(Qt::Horizontal);
	qsAmp->setRange(0, 18000);
	qsAmp->setSingleStep(500);
	qsAmp->setPageStep(2000);
	qsAmp->setValue(20000 - g.s.iMinLoudness);
	qsAmp->setObjectName("Amp");

	l = new QLabel(tr("Amp"));
	l->setBuddy(qsAmp);

	qlAmp=new QLabel();
	on_Amp_valueChanged(qsAmp->value());

	tip=tr("<b>Maximum amplification of input.</b><br />"
			"Mumble normalizes the input volume before compressing, and this sets how much it's allowed to amplify.<br />"
			"The actual level is continually updated based on your current speech pattern, but it will never go<br />"
			"above the level specified here.<br />"
			"If the Mic Volume level of the audio statistics hover around 100%, you probably want to set this to 2.0<br />"
			"or so, but if, like most people, you are unable to reach 100%, set this to something much higher.<br />"
			"Ideally, set it so <i>Mic Volume * Amplification Factor >= 100</i>, even when you're speaking really soft.<br /><br />"
			"Note that there is no harm in setting this to maximum, but Mumble will start picking up other conversations<br />"
			"if you leave it to autotune to that level."
			);
	qsAmp->setToolTip(tip);
	qlAmp->setToolTip(tip);
	l->setToolTip(tip);

	grid->addWidget(l, 2, 0);
	grid->addWidget(qsAmp, 2, 1);
	grid->addWidget(qlAmp, 2, 2);

	qgbCompress->setLayout(grid);

    v = new QVBoxLayout;
    v->addWidget(qgbInterface);
    v->addWidget(qgbTransmit);
    v->addWidget(qgbCompress);
    v->addStretch(1);
    setLayout(v);

    QMetaObject::connectSlotsByName(this);
}

QString AudioConfigDialog::title() const {
	return tr("Basic Audio");
}

QIcon AudioConfigDialog::icon() const {
	return ConfigWidget::icon();
}

void AudioConfigDialog::accept() {
	g.s.iQuality = qsQuality->value();
	g.s.iComplexity = qsComplexity->value();
	g.s.iMinLoudness = 18000 - qsAmp->value() + 2000;
	g.s.iVoiceHold = qsTransmitHold->value();
	g.s.atTransmit = static_cast<Settings::AudioTransmit>(qcbTransmit->currentIndex());
	g.s.qsAudioInput = qcbInput->currentText();
	g.s.qsAudioOutput = qcbOutput->currentText();
}

void AudioConfigDialog::on_TransmitHold_valueChanged(int v) {
	qlTransmitHold->setText(tr("%1 ms").arg(v*20));
}

void AudioConfigDialog::on_Quality_valueChanged(int v) {
	qlQuality->setText(QString::number(v));
}

void AudioConfigDialog::on_Complexity_valueChanged(int v) {
	qlComplexity->setText(QString::number(v));
}

void AudioConfigDialog::on_Amp_valueChanged(int v) {
	v = 18000 - v + 2000;
	double d = 20000.0/v;
	qlAmp->setText(QString("%1").arg(d, 0, 'f', 2));
}
