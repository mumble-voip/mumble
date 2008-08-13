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

#include "AudioWizard.h"
#include "AudioInput.h"
#include "Global.h"
#include "Settings.h"

AudioWizard::AudioWizard(QWidget *p) : QWizard(p) {
	bInit = true;

	setOption(QWizard::NoCancelButton, false);
#ifndef Q_OS_MAC
	resize(700, 500);
#endif

	addPage(qwpIntro = introPage());
	addPage(qwpDevice = devicePage());
	addPage(qwpDeviceTuning = deviceTuningPage());
	addPage(qwpVolume = volumePage());
	addPage(qwpTrigger = triggerPage());
	addPage(qwpPositional = positionalPage());
	addPage(qwpDone = donePage());
	setWindowTitle(tr("Audio Tuning Wizard"));

	sOldSettings = g.s;

	g.s.lmLoopMode = Settings::Local;
	g.s.dPacketLoss = 0.0;
	g.s.dMaxPacketDelay = 0.0;
	g.s.bMute = false;
	g.s.bDeaf = false;

	g.s.atTransmit = Settings::Continous;

	iMaxPeak = 0;
	iTicks = 0;

	g.bEchoTest = true;

	ticker = new QTimer(this);
	ticker->setObjectName(QLatin1String("Ticker"));

	setObjectName(QLatin1String("Wizard"));
	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(showPage(int)));

	QMetaObject::connectSlotsByName(this);
	bInit = false;

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
	if (AudioInputRegistrar::qmNew) {
		foreach(AudioInputRegistrar *air, *AudioInputRegistrar::qmNew) {
			qcbInput->addItem(air->name);
			if (air->name == AudioInputRegistrar::current)
				qcbInput->setCurrentIndex(qcbInput->count() - 1);
			QList<audioDevice> ql= air->getDeviceChoices();
		}
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

	qcbEcho = new QCheckBox(tr("Use echo cancellation"));
	qcbEcho->setToolTip(tr("Cancel echo from headset or speakers."));
	qcbEcho->setWhatsThis(tr("This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets."));
	qcbEcho->setObjectName(QLatin1String("Echo"));
	qcbEcho->setChecked(g.s.bEcho);
	grid->addWidget(qcbEcho, 3, 1);

	qgbInput->setLayout(grid);



	QGroupBox *qgbOutput = new QGroupBox(tr("Output device"));
	grid = new QGridLayout();

	l = new QLabel(tr("This is the device your speakers or headphones are connected to."));
	l->setWordWrap(true);
	grid->addWidget(l, 0, 0, 1, 2);

	qcbOutput = new QComboBox();
	if (AudioOutputRegistrar::qmNew) {
		foreach(AudioOutputRegistrar *aor, *AudioOutputRegistrar::qmNew) {
			qcbOutput->addItem(aor->name);
			if (aor->name == AudioOutputRegistrar::current)
				qcbOutput->setCurrentIndex(qcbOutput->count() - 1);
			QList<audioDevice> ql= aor->getDeviceChoices();
		}
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

	qcbPositional = new QCheckBox(tr("Enable positional audio"));
	qcbPositional->setToolTip(tr("Allows positioning of sound."));
	qcbPositional->setWhatsThis(tr("This allows Mumble to use positional audio to place voices."));
	qcbPositional->setObjectName(QLatin1String("Positional"));
	qcbPositional->setChecked(g.s.bPositionalAudio);
	grid->addWidget(qcbPositional, 3, 1);

	qgbOutput->setLayout(grid);

	QVBoxLayout *v=new QVBoxLayout;
	v->addWidget(qgbInput);
	v->addWidget(qgbOutput);

	qwpage->setLayout(v);

	on_Input_activated(qcbInput->currentIndex());
	on_Output_activated(qcbOutput->currentIndex());

	return qwpage;
}

QWizardPage *AudioWizard::positionalPage() {
	QWizardPage *qwpage = new QWizardPage;
	qwpage->setTitle(tr("Positional Audio"));
	qwpage->setSubTitle(tr("Adjusting attenuation of positional audio."));

	QVBoxLayout *v = new QVBoxLayout();
	QLabel *l;

	l = new QLabel(tr("Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the "
	                  "volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker "
	                  "configuration being correct in your operating system, so a test is done here."));
	l->setWordWrap(true);
	v->addWidget(l);
	l = new QLabel(tr("The graph below shows the position of <font color=\"red\">you</font>, the <font color=\"yellow\">speakers</font> and a <font color=\"green\">moving sound source</font> as if seen from above. You should hear the audio move between the channels."));
	l->setWordWrap(true);
	v->addWidget(l);

	QHBoxLayout *h = new QHBoxLayout();

	QVBoxLayout *subv = new QVBoxLayout();

	qcbHeadphone = new QCheckBox(tr("Use headphones"));
	qcbHeadphone->setToolTip(tr("Use headphones instead of speakers."));
	qcbHeadphone->setWhatsThis(tr("This ignores the OS speaker configuration and configures the positioning for headphones instead."));
	qcbHeadphone->setObjectName(QLatin1String("Headphone"));
	qcbHeadphone->setChecked(g.s.bPositionalHeadphone);
	subv->addWidget(qcbHeadphone);


	fAngle = 0.0f;
	fX = fY = 0.0f;
	qgsScene = NULL;
	qgiSource = NULL;
	asSource = NULL;
	qgvView = new QGraphicsView();
	qgvView->scale(1.0f, -1.0f);
	subv->addWidget(qgvView);
	qgvView->viewport()->installEventFilter(this);

	h->addLayout(subv);

	subv = new QVBoxLayout();

	h->addLayout(subv);

	v->addLayout(h);

	qwpage->setLayout(v);


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

	l = new QLabel(tr("You should hear a single tone that's changing in frequency. Change the slider below to the lowest value which gives <b>no</b> interruptions or jitter "
	                  "in the sound. Please note that local echo is disabled during this test to "
	                  "improve audio path recognition."));
	l->setWordWrap(true);
	grid->addWidget(l, 1, 0, 1, 2);

	qsOutputDelay = new QSlider(Qt::Horizontal);
	qsOutputDelay->setRange(1, 6);

	qsOutputDelay->setSingleStep(1);
	qsOutputDelay->setPageStep(2);
	qsOutputDelay->setValue(g.s.iOutputDelay);
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

	l = new QLabel(tr("If you position the microphone so it can pick up the speakers or headset, Mumble will measure the total audio path "
	                  "delay in your system; this means the delay from a sample is placed in an outbound buffer until it's found in "
	                  "a matching incoming buffer."));
	l->setWordWrap(true);
	grid->addWidget(l, 3, 0, 1, 2);

	qlAudioPath = new QLabel();
	grid->addWidget(qlAudioPath, 4, 0, 1, 2);

	qwpage->setLayout(grid);
	return qwpage;
}

void AudioWizard::on_Input_activated(int) {
	qcbInputDevice->clear();

	if (! AudioInputRegistrar::qmNew)
		return;

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

void AudioWizard::on_Output_activated(int) {
	qcbOutputDevice->clear();

	if (! AudioOutputRegistrar::qmNew)
		return;

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
	}

	AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qcbInput->currentText());
	qcbEcho->setEnabled(air->canEcho(qcbOutput->currentText()));

	g.ao = AudioOutputPtr(aor->create());
	g.ao->start(QThread::HighPriority);
}

void AudioWizard::on_OutputDelay_valueChanged(int v) {
	qlOutputDelay->setText(tr("%1ms").arg(v*20));
	g.s.iOutputDelay = v;
	on_OutputDevice_activated(0);
	if (! bInit) {
		AudioOutputPtr ao = g.ao;
		ao->playSine(0.0f, 0.0f, 0xfffffff, 0.5f);
	}
}

void AudioWizard::on_MaxAmp_valueChanged(int v) {
	g.s.iMinLoudness = qMin(v, 30000);
}

void AudioWizard::showPage(int v) {
	AudioOutputPtr ao = g.ao;
	if (ao)
		ao->wipe();
	asSource = NULL;

	g.s.bMute = false;
	g.bPosTest = false;

	if (v == 2) {
		g.s.bMute = true;
		ao->playSine(0.0f, 0.0f, 0xfffffff, 0.5f);
	} else if (v == 5) {
		fX = fY = 0.0f;
		g.s.bMute = true;
		g.bPosTest = true;
		if (qgsScene) {
			delete qgsScene;
			qgiSource = NULL;
			qgsScene = NULL;
		}
		playChord();
	}
}

int AudioWizard::nextId() const {
	int nextid = QWizard::nextId();
	if (currentPage() == qwpTrigger && ! g.s.bPositionalAudio)
		nextid++;
	return nextid;
}

void AudioWizard::playChord() {
	AudioOutputPtr ao = g.ao;
	if (! ao || asSource)
		return;
	asSource = ao->playSine(100.0f, 0.0001f, 0xfffffff, 0.5f);
}

void AudioWizard::restartAudio() {
	boost::weak_ptr<AudioInput> wai(g.ai);
	boost::weak_ptr<AudioOutput> wao(g.ao);

	asSource = NULL;

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
	g.bEchoTest = false;
	restartAudio();

	QWizard::reject();
}

void AudioWizard::accept() {
	g.s.atTransmit = sOldSettings.atTransmit;
	g.s.bMute = sOldSettings.bMute;
	g.s.bDeaf = sOldSettings.bDeaf;
	g.s.lmLoopMode = Settings::None;
	g.bEchoTest = false;
	g.bPosTest = false;
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
		abVAD->iValue = lroundf((32767.f/96.0f) * (96.0f + ai->dPeakSignal));
	} else {
		abVAD->iValue = static_cast<int>(ai->fSpeechProb * 32767.0);
	}
	abVAD->update();

	QString txt;
	if (g.iAudioPathTime)
		txt=tr("Audio path is %1ms long.").arg(g.iAudioPathTime*20);
	else
		txt=tr("Audio path cannot be determined. Input not recognized.");
	qlAudioPath->setText(txt);

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
		asSource->fPos[0] = xp;
		asSource->fPos[1] = 0;
		asSource->fPos[2] = yp;
	}
}

void AudioWizard::on_VADmin_valueChanged(int v) {
	g.s.fVADmin = static_cast<float>(v) / 32767.0f;
}

void AudioWizard::on_VADmax_valueChanged(int v) {
	g.s.fVADmax = static_cast<float>(v) / 32767.0f;
}

void AudioWizard::on_Holdtime_valueChanged(int v) {
	g.s.iVoiceHold = v;

	float val = static_cast<float>(v) * 20.0f;
	val = val / 1000.0f;
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

void AudioWizard::on_Echo_clicked(bool on) {
	g.s.bEcho = on;
	restartAudio();
}

void AudioWizard::on_Headphone_clicked(bool on) {
	g.s.bPositionalHeadphone = on;
	restartAudio();
}

void AudioWizard::on_Positional_clicked(bool on) {
	g.s.bPositionalAudio = on;
	restartAudio();
}
