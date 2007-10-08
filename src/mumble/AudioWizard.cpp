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

#include "AudioWizard.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "Global.h"
#include "Settings.h"

AudioBar::AudioBar(QWidget *p) : QWidget(p) {
	qcBelow = Qt::yellow;
	qcAbove = Qt::red;
	qcInside = Qt::green;
	iMin = 0;
	iMax = 32768;
	iBelow = 2000;
	iAbove = 22000;
	iValue = 1000;
	setMinimumSize(100,20);
}

void AudioBar::paintEvent(QPaintEvent *) {
	QPainter p(this);

	if (iBelow > iAbove)
		iBelow = iAbove;

	if (iValue < iMin)
		iValue = iMin;
	else if (iValue > iMax)
		iValue = iMax;

//    p.fillRect(QRect(0,0, 10, 10), Qt::blue);

	double scale = (width() * 1.0) / (iMax - iMin);
	int h = height();

	p.scale(scale, h);

	p.fillRect(QRect(0,0, 10, 10), Qt::blue);

	if (iValue <= iBelow) {
		p.fillRect(0, 0, iValue-1, 1, qcBelow);
		p.fillRect(iValue, 0, iBelow-iValue, 1, qcBelow.darker(300));
		p.fillRect(iBelow, 0, iAbove-iBelow, 1, qcInside.darker(300));
		p.fillRect(iAbove, 0, iMax-iAbove, 1, qcAbove.darker(300));
	} else if (iValue <= iAbove) {
		p.fillRect(0, 0, iBelow, 1, qcBelow);
		p.fillRect(iBelow, 0, iValue-iBelow, 1, qcInside);
		p.fillRect(iValue, 0, iAbove-iValue, 1, qcInside.darker(300));
		p.fillRect(iAbove, 0, iMax-iAbove, 1, qcAbove.darker(300));
	} else {
		p.fillRect(0, 0, iBelow, 1, qcBelow);
		p.fillRect(iBelow, 0, iAbove-iBelow, 1, qcInside);
		p.fillRect(iAbove, 0, iValue-iAbove, 1, qcAbove);
		p.fillRect(iValue, 0, iMax-iValue, 1, qcAbove.darker(300));
	}

	if ((iPeak >= iMin) && (iPeak <= iMax))  {
		if (iPeak <= iBelow)
			p.setPen(qcBelow.lighter(150));
		else if (iPeak <= iAbove)
			p.setPen(qcInside.lighter(150));
		else
			p.setPen(qcAbove.lighter(150));
		p.drawLine(iPeak, 0, iPeak, 1);
	}

}

AudioWizard::AudioWizard() {
	bInit = true;

	addPage(introPage());
	addPage(devicePage());
	addPage(deviceTuningPage());
	addPage(volumePage());
	addPage(triggerPage());
	addPage(donePage());
	setWindowTitle(tr("Audio Tuning Wizard"));

	sOldSettings = g.s;

	g.s.lmLoopMode = Settings::Local;
	g.s.dPacketLoss = 0.0;
	g.s.dMaxPacketDelay = 0.0;

	g.s.atTransmit = Settings::Continous;

	iMaxPeak = 0;
	iTicks = 0;

	ticker = new QTimer(this);
	ticker->setObjectName(QLatin1String("Ticker"));

	setObjectName(QLatin1String("Wizard"));
	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(showPage(int)));

	QMetaObject::connectSlotsByName(this);
	bInit = false;
	resize(700, 500);
	ticker->setSingleShot(false);
	ticker->start(20);
}

QWizardPage *AudioWizard::introPage() {
	QWizardPage *qwpage = new QWizardPage;
	qwpage->setTitle(tr("Introduction"));
	qwpage->setSubTitle(tr("Welcome to the Mumble Audio Wizard"));
	QVBoxLayout *v=new QVBoxLayout;
	QLabel *label=new QLabel(tr("This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct "
	                            "parameters for sound processing in Mumble."));
	label->setWordWrap(true);
	v->addWidget(label);

	label=new QLabel(tr("Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the "
	                    "server."));
	label->setWordWrap(true);
	v->addWidget(label);

	qwpage->setLayout(v);
	return qwpage;
}

QWizardPage *AudioWizard::donePage() {
	QWizardPage *qwpage = new QWizardPage;
	qwpage->setTitle(tr("Finished"));
	qwpage->setSubTitle(tr("Enjoy using Mumble"));
	QVBoxLayout *v=new QVBoxLayout;
	QLabel *label=new QLabel(tr("Congratulations. You should now be ready to enjoy a richer sound experience with Mumble."));
	label->setWordWrap(true);
	v->addWidget(label);

	qwpage->setLayout(v);
	return qwpage;
}

QWizardPage *AudioWizard::devicePage() {
	QWizardPage *qwpage = new QWizardPage;
	QLabel *l;
	QGridLayout *grid;

	qwpage->setTitle(tr("Device selection"));
	qwpage->setSubTitle(tr("Selecting the input and output device to use with Mumble."));


	QGroupBox *qgbInput = new QGroupBox(tr("Input device"));
	grid = new QGridLayout();

	l = new QLabel(tr("This is the device your microphone is connected to."));
	l->setWordWrap(true);
	grid->addWidget(l, 0, 0, 1, 2);

	qcbInput = new QComboBox();
	foreach(AudioInputRegistrar *air, *AudioInputRegistrar::qmNew) {
		qcbInput->addItem(air->name);
		if (air->name == AudioInputRegistrar::current)
			qcbInput->setCurrentIndex(qcbInput->count() - 1);
		QList<audioDevice> ql= air->getDeviceChoices();
		if (ql.count() > 0)
			qhOldInputDevice.insert(air->name, ql.at(0).second);
	}

	l = new QLabel(tr("System"));
	l->setBuddy(qcbInput);

	qcbInput->setToolTip(tr("Input method for audio"));
	qcbInput->setWhatsThis(tr("<b>This is the input method to use for audio.</b><br />Most likely you want to use DirectSound."));
	qcbInput->setObjectName(QLatin1String("Input"));

	if (qcbInput->count() < 2) {
		qcbInput->setEnabled(false);
	}

	grid->addWidget(l, 1, 0);
	grid->addWidget(qcbInput, 1, 1);


	qcbInputDevice = new QComboBox();
	l = new QLabel(tr("Device"));
	l->setBuddy(qcbInputDevice);

	qcbInputDevice->setToolTip(tr("Input device to use"));
	qcbInputDevice->setWhatsThis(tr("<b>Selects which sound card to use for audio input.</b>"));
	qcbInputDevice->setObjectName(QLatin1String("InputDevice"));

	grid->addWidget(l, 2, 0);
	grid->addWidget(qcbInputDevice, 2, 1);

	qgbInput->setLayout(grid);



	QGroupBox *qgbOutput = new QGroupBox(tr("Output device"));
	grid = new QGridLayout();

	l = new QLabel(tr("This is the device your speakers or headphones are connected to."));
	l->setWordWrap(true);
	grid->addWidget(l, 0, 0, 1, 2);

	qcbOutput = new QComboBox();
	foreach(AudioOutputRegistrar *aor, *AudioOutputRegistrar::qmNew) {
		qcbOutput->addItem(aor->name);
		if (aor->name == AudioOutputRegistrar::current)
			qcbOutput->setCurrentIndex(qcbOutput->count() - 1);
		QList<audioDevice> ql= aor->getDeviceChoices();
		if (ql.count() > 0)
			qhOldOutputDevice.insert(aor->name, ql.at(0).second);
	}

	l = new QLabel(tr("System"));
	l->setBuddy(qcbOutput);

	qcbOutput->setToolTip(tr("Output method for audio"));
	qcbOutput->setWhatsThis(tr("<b>This is the Output method to use for audio.</b><br />Most likely you want to use DirectSound."));
	qcbOutput->setObjectName(QLatin1String("Output"));

	if (qcbOutput->count() < 2) {
		qcbOutput->setEnabled(false);
	}

	grid->addWidget(l, 1, 0);
	grid->addWidget(qcbOutput, 1, 1);


	qcbOutputDevice = new QComboBox();
	l = new QLabel(tr("Device"));
	l->setBuddy(qcbOutputDevice);

	qcbOutputDevice->setToolTip(tr("Output device to use"));
	qcbOutputDevice->setWhatsThis(tr("<b>Selects which sound card to use for audio Output.</b>"));
	qcbOutputDevice->setObjectName(QLatin1String("OutputDevice"));

	grid->addWidget(l, 2, 0);
	grid->addWidget(qcbOutputDevice, 2, 1);

	qgbOutput->setLayout(grid);

	QVBoxLayout *v=new QVBoxLayout;
	v->addWidget(qgbInput);
	v->addWidget(qgbOutput);

	qwpage->setLayout(v);

	on_Input_activated(qcbInput->currentIndex());
	on_Output_activated(qcbOutput->currentIndex());

	return qwpage;
}

QWizardPage *AudioWizard::volumePage() {
	QWizardPage *qwpage = new QWizardPage;
	qwpage->setTitle(tr("Volume tuning"));
	qwpage->setSubTitle(tr("Tuning microphone hardware volume to optimal settings."));

	QVBoxLayout *v = new QVBoxLayout();
	QLabel *l;

	l = new QLabel(tr("Open your sound control panel and go to the recording settings. "
	                  "Make sure the microphone is selected as active input with maximum recording volume. If there's an option to enable a \"Microphone boost\" make sure it's checked."));
	l->setWordWrap(true);
	v->addWidget(l);

	l = new QLabel(tr("Speak loudly, as when you are annoyed or excited. "
	                  "Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green "
	                  "but <b>not</b> the red "
	                  "zone while you speak."));
	l->setWordWrap(true);
	v->addWidget(l);

	abAmplify = new AudioBar();
	abAmplify->qcBelow = Qt::blue;
	v->addWidget(abAmplify);

	l = new QLabel(tr("Now talk softly, as you would when talking late at night and you don't want to disturb anyone. "
	                  "Adjust the slider below so that the bar moves into green when you talk, but stays blue while you're silent."));
	l->setWordWrap(true);
	v->addWidget(l);

	qsMaxAmp = new QSlider(Qt::Horizontal);
	qsMaxAmp->setRange(1, 32767);
	qsMaxAmp->setSingleStep(100);
	qsMaxAmp->setPageStep(1000);
	qsMaxAmp->setValue(g.s.iMinLoudness);
	qsMaxAmp->setObjectName(QLatin1String("MaxAmp"));

	v->addWidget(qsMaxAmp);

	qwpage->setLayout(v);

	return qwpage;
}

QWizardPage *AudioWizard::triggerPage() {
	QWizardPage *qwpage = new QWizardPage;
	qwpage->setTitle(tr("Voice Activity Detection"));
	qwpage->setSubTitle(tr("Letting Mumble figure out when you're talking and when you're silent."));

	QVBoxLayout *v = new QVBoxLayout();
	QLabel *l;

	l = new QLabel(tr("This will help Mumble figure out when you are talking. The first step is selecting which data value to use."));
	l->setWordWrap(true);
	v->addWidget(l);

	qrAmplitude = new QRadioButton(tr("Raw amplitude from input"));
	qrAmplitude->setObjectName(QLatin1String("Amplitude"));
	v->addWidget(qrAmplitude);

	qrSNR = new QRadioButton(tr("Signal-To-Noise ratio"));
	qrSNR->setObjectName(QLatin1String("SNR"));
	v->addWidget(qrSNR);

	switch (g.s.vsVAD) {
		case Settings::Amplitude:
			qrAmplitude->setChecked(true);
			break;
		case Settings::SignalToNoise:
			qrSNR->setChecked(true);
			break;
	}

	abVAD = new AudioBar();
	abVAD->qcBelow = Qt::red;
	abVAD->qcInside = Qt::yellow;
	abVAD->qcAbove = Qt::green;

	v->addWidget(abVAD);

	l = new QLabel(tr("Next you need to adjust the following two sliders. The first few utterances you say should end up in the "
	                  "green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you're "
	                  "not talking, everything should be in the red (definitively not speech)."));
	l->setWordWrap(true);
	v->addWidget(l);

	qsMinVAD = new QSlider(Qt::Horizontal);
	qsMinVAD->setRange(1, 32767);
	qsMinVAD->setSingleStep(100);
	qsMinVAD->setPageStep(1000);
	qsMinVAD->setValue(static_cast<int>(g.s.fVADmin * 32767.0));
	qsMinVAD->setObjectName(QLatin1String("VADmin"));

	v->addWidget(qsMinVAD);

	qsMaxVAD = new QSlider(Qt::Horizontal);
	qsMaxVAD->setRange(1, 32767);
	qsMaxVAD->setSingleStep(100);
	qsMaxVAD->setPageStep(1000);
	qsMaxVAD->setValue(static_cast<int>(g.s.fVADmax * 32767));
	qsMaxVAD->setObjectName(QLatin1String("VADmax"));

	v->addWidget(qsMaxVAD);

	l = new QLabel(tr("Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before "
	                  "it breaks off. This allows you to catch your breath while speaking."));
	l->setWordWrap(true);
	v->addWidget(l);

	QHBoxLayout *h = new QHBoxLayout;

	qsHoldtime = new QSlider(Qt::Horizontal);
	qsHoldtime->setRange(20, 250);
	qsHoldtime->setSingleStep(5);
	qsHoldtime->setPageStep(25);
	qsHoldtime->setValue(g.s.iVoiceHold);
	qsHoldtime->setObjectName(QLatin1String("Holdtime"));

	qlHoldtime = new QLabel();
	qlHoldtime->setMinimumWidth(40);
	on_Holdtime_valueChanged(qsHoldtime->value());

	h->addWidget(qsHoldtime);
	h->addWidget(qlHoldtime);

	v->addLayout(h);

	qwpage->setLayout(v);

	return qwpage;
}

QWizardPage *AudioWizard::deviceTuningPage() {
	QWizardPage *qwpage = new QWizardPage;
	qwpage->setTitle(tr("Device tuning"));
	qwpage->setSubTitle(tr("Changing hardware output delays to their minimum value."));

	QGridLayout *grid = new QGridLayout();

	QLabel *l = new QLabel(tr("To keep latency to an absolute minium, it's important to buffer as little audio as possible on the soundcard. "
	                          "However, many soundcards report that they require a much smaller buffer than what they can actually work with, "
	                          "so the only way to set this value is to try and fail."));
	l->setWordWrap(true);
	grid->addWidget(l, 0, 0, 1, 2);

	l = new QLabel(tr("You should hear a multitone chord. Change the slider below to the lowest value which gives <b>no</b> interruptions or jitter "
	                  "in the sound."));
	l->setWordWrap(true);
	grid->addWidget(l, 1, 0, 1, 2);

	qsOutputDelay = new QSlider(Qt::Horizontal);
	qsOutputDelay->setRange(1, 6);

	qsOutputDelay->setSingleStep(1);
	qsOutputDelay->setPageStep(2);
	qsOutputDelay->setValue(g.s.iDXOutputDelay);
	qsOutputDelay->setObjectName(QLatin1String("OutputDelay"));

	qlOutputDelay=new QLabel();
	qlOutputDelay->setMinimumWidth(30);
	on_OutputDelay_valueChanged(qsOutputDelay->value());
	qsOutputDelay->setToolTip(tr("Amount of data to buffer."));
	qsOutputDelay->setWhatsThis(tr("This sets the amount of data to prebuffer in the output buffer. "
	                               "Experiment with different values and set it to the lowest which doesn't "
	                               "cause rapid jitter in the sound."));
	grid->addWidget(qsOutputDelay, 2, 1);
	grid->addWidget(qlOutputDelay, 2, 2);

	qwpage->setLayout(grid);
	return qwpage;
}

void AudioWizard::on_Input_activated(int) {
	qcbInputDevice->clear();

	AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbInput->currentText());
	QList<audioDevice> ql = air->getDeviceChoices();

	foreach(audioDevice d, ql) {
		qcbInputDevice->addItem(d.first, d.second);
	}

	qcbInputDevice->setEnabled(ql.count() > 1);

	on_InputDevice_activated(0);
}

void AudioWizard::on_InputDevice_activated(int) {
	if (bInit)
		return;

	boost::weak_ptr<AudioInput> wai(g.ai);
	g.ai.reset();

	while (! wai.expired()) {
	}

	AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbInput->currentText());
	int idx = qcbInputDevice->currentIndex();
	if (idx > -1) {
		air->setDeviceChoice(qcbInputDevice->itemData(idx));
	}

	g.ai = AudioInputPtr(air->create());
	g.ai->start(QThread::HighestPriority);
}

void AudioWizard::on_Output_activated(int) {
	qcbOutputDevice->clear();

	AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew->value(qcbOutput->currentText());
	QList<audioDevice> ql = aor->getDeviceChoices();

	foreach(audioDevice d, ql) {
		qcbOutputDevice->addItem(d.first, d.second);
	}

	qcbOutputDevice->setEnabled(ql.count() > 1);

	on_OutputDevice_activated(0);
}

void AudioWizard::on_OutputDevice_activated(int) {
	if (bInit)
		return;

	boost::weak_ptr<AudioOutput> wai(g.ao);
	g.ao.reset();

	while (! wai.expired()) {
	}

	AudioOutputRegistrar *aor = AudioOutputRegistrar::qmNew->value(qcbOutput->currentText());
	int idx = qcbOutputDevice->currentIndex();
	if (idx > -1) {
		aor->setDeviceChoice(qcbOutputDevice->itemData(idx));
	}

	g.ao = AudioOutputPtr(aor->create());
	g.ao->start(QThread::HighPriority);
}

void AudioWizard::on_OutputDelay_valueChanged(int v) {
	qlOutputDelay->setText(tr("%1ms").arg(v*20));
	g.s.iDXOutputDelay = v;
	on_OutputDevice_activated(0);
	if (! bInit)
		playChord();
}

void AudioWizard::on_MaxAmp_valueChanged(int v) {
	g.s.iMinLoudness = qMin(v, 30000);
}

void AudioWizard::showPage(int v) {
	AudioOutputPtr ao = g.ao;
	ao->wipe();

	if (v == 2)
		playChord();
}

void AudioWizard::playChord() {
	AudioOutputPtr ao = g.ao;
	ao->playSine(261.63);
	ao->playSine(329.63);
	ao->playSine(392.00);
}

void AudioWizard::restartAudio() {
	boost::weak_ptr<AudioInput> wai(g.ai);
	boost::weak_ptr<AudioOutput> wao(g.ao);

	g.ai.reset();
	g.ao.reset();

	while (! wai.expired() || ! wao.expired()) {
		// Where is QThread::yield() ?
	}

	g.s.qsAudioInput = qcbInput->currentText();
	g.s.qsAudioOutput = qcbOutput->currentText();

	g.ai = AudioInputRegistrar::newFromChoice(g.s.qsAudioInput);
	g.ai->start(QThread::HighestPriority);
	g.ao = AudioOutputRegistrar::newFromChoice(g.s.qsAudioOutput);
	g.ao->start(QThread::HighPriority);
}

void AudioWizard::reject() {
	g.s = sOldSettings;

	foreach(AudioInputRegistrar *air, *AudioInputRegistrar::qmNew) {
		const QString &name = air->name;
		if (qhOldInputDevice.contains(name))
			air->setDeviceChoice(qhOldInputDevice.value(name));
	}

	foreach(AudioOutputRegistrar *aor, *AudioOutputRegistrar::qmNew) {
		const QString &name = aor->name;
		if (qhOldOutputDevice.contains(name))
			aor->setDeviceChoice(qhOldOutputDevice.value(name));
	}

	g.s.lmLoopMode = Settings::None;
	restartAudio();

	QWizard::reject();
}

void AudioWizard::accept() {
	g.s.atTransmit = sOldSettings.atTransmit;
	g.s.lmLoopMode = Settings::None;
	restartAudio();
	QWizard::accept();
}

void AudioWizard::on_Ticker_timeout() {
	AudioInputPtr ai = g.ai;
	if (! ai)
		return;

	int iPeak = static_cast<int>(32767 * pow(10.0, (ai->dPeakMic / 20.0)));

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
		abVAD->iValue = iPeak;
		abVAD->iPeak = iMaxPeak;
	} else {
		abVAD->iValue = static_cast<int>(ai->dSNR * 1000.0);
		abVAD->iPeak = -1;
	}
	abVAD->update();
}

void AudioWizard::on_VADmin_valueChanged(int v) {
	g.s.fVADmin = v / 32767.0;
}

void AudioWizard::on_VADmax_valueChanged(int v) {
	g.s.fVADmax = v / 32767.0;
}

void AudioWizard::on_Holdtime_valueChanged(int v) {
	g.s.iVoiceHold = v;

	double val = v * 20;
	val = val / 1000.0;
	qlHoldtime->setText(tr("%1 s").arg(val, 0, 'f', 2));
}

void AudioWizard::on_SNR_clicked(bool on) {
	if (on)
		g.s.vsVAD = Settings::SignalToNoise;
}

void AudioWizard::on_Amplitude_clicked(bool on) {
	if (on)
		g.s.vsVAD = Settings::Amplitude;
}
