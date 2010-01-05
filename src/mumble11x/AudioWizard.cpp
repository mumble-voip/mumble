/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

CompletablePage *AudioWizard::introPage() {
	CompletablePage *qwpage = new CompletablePage(this);
	qwpage->setTitle(tr("Introduction"));
	qwpage->setSubTitle(tr("Welcome to the Mumble Audio Wizard"));
	QVBoxLayout *v=new QVBoxLayout(qwpage);
	QLabel *label=new QLabel(tr("This is the audio tuning wizard for Mumble. This will help you correctly set the input levels of your sound card, and also set the correct "
	                            "parameters for sound processing in Mumble."), qwpage);
	label->setWordWrap(true);
	v->addWidget(label);

	label=new QLabel(tr("Please be aware that as long as this wizard is active, audio will be looped locally to allow you to listen to it, and no audio will be sent to the "
	                    "server."), qwpage);
	label->setWordWrap(true);
	v->addWidget(label);

	return qwpage;
}

CompletablePage *AudioWizard::donePage() {
	CompletablePage *qwpage = new CompletablePage(this);
	qwpage->setTitle(tr("Finished"));
	qwpage->setSubTitle(tr("Enjoy using Mumble"));
	QVBoxLayout *v=new QVBoxLayout(qwpage);

	QLabel *label=new QLabel(tr("Congratulations. You should now be ready to enjoy a richer sound experience with Mumble."), qwpage);
	label->setWordWrap(true);
	v->addWidget(label);

	label=new QLabel(tr("Mumble is under continuous development, and the development team wants to focus on the features "
	                    "that benefit the most users. To this end, Mumble supports submitting anonymous statistics "
	                    "about your configuration to the developers. These statistcs are essential for future development, and also "
	                    "make sure the features you use aren't deprecated."));
	label->setWordWrap(true);
	v->addWidget(label);

	qcbUsage = new QCheckBox(tr("Submit anonymous statistics to the Mumble project"));
	qcbUsage->setChecked(g.s.bUsage);
	v->addWidget(qcbUsage);

	return qwpage;
}

CompletablePage *AudioWizard::devicePage() {
	CompletablePage *qwpage = new CompletablePage(this);
	QLabel *l;
	QGridLayout *grid;

	qwpage->setTitle(tr("Device selection"));
	qwpage->setSubTitle(tr("Selecting the input and output device to use with Mumble."));

	QGroupBox *qgbInput = new QGroupBox(tr("Input device"), qwpage);
	grid = new QGridLayout(qgbInput);

	l = new QLabel(tr("This is the device your microphone is connected to."), qgbInput);
	l->setWordWrap(true);
	grid->addWidget(l, 0, 0, 1, 2);

	qcbInput = new QComboBox(qgbInput);
	if (AudioInputRegistrar::qmNew) {
		foreach(AudioInputRegistrar *air, *AudioInputRegistrar::qmNew) {
			qcbInput->addItem(air->name);
			if (air->name == AudioInputRegistrar::current)
				qcbInput->setCurrentIndex(qcbInput->count() - 1);
			QList<audioDevice> ql= air->getDeviceChoices();
		}
	}

	l = new QLabel(tr("System"), qgbInput);
	l->setBuddy(qcbInput);

	qcbInput->setToolTip(tr("Input method for audio"));
	qcbInput->setWhatsThis(tr("<b>This is the input method to use for audio.</b><br />Most likely you want to use DirectSound."));
	qcbInput->setObjectName(QLatin1String("Input"));

	if (qcbInput->count() < 2) {
		qcbInput->setEnabled(false);
	}

	grid->addWidget(l, 1, 0);
	grid->addWidget(qcbInput, 1, 1);


	qcbInputDevice = new QComboBox(qgbInput);
	l = new QLabel(tr("Device"), qgbInput);
	l->setBuddy(qcbInputDevice);

	qcbInputDevice->setToolTip(tr("Input device to use"));
	qcbInputDevice->setWhatsThis(tr("<b>Selects which sound card to use for audio input.</b>"));
	qcbInputDevice->setObjectName(QLatin1String("InputDevice"));

	grid->addWidget(l, 2, 0);
	grid->addWidget(qcbInputDevice, 2, 1);

	qcbEcho = new QCheckBox(tr("Use echo cancellation"), qgbInput);
	qcbEcho->setToolTip(tr("Cancel echo from headset or speakers."));
	qcbEcho->setWhatsThis(tr("This enables echo cancellation of outgoing audio, which helps both on speakers and on headsets."));
	qcbEcho->setObjectName(QLatin1String("Echo"));
	qcbEcho->setChecked(g.s.bEcho);
	grid->addWidget(qcbEcho, 3, 1);

	QGroupBox *qgbOutput = new QGroupBox(tr("Output device"), qwpage);
	grid = new QGridLayout(qgbOutput);

	l = new QLabel(tr("This is the device your speakers or headphones are connected to."), qgbOutput);
	l->setWordWrap(true);
	grid->addWidget(l, 0, 0, 1, 2);

	qcbOutput = new QComboBox(qgbOutput);
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

	l = new QLabel(tr("System"), qgbOutput);
	l->setBuddy(qcbOutput);

	qcbOutput->setToolTip(tr("Output method for audio"));
	qcbOutput->setWhatsThis(tr("<b>This is the Output method to use for audio.</b><br />Most likely you want to use DirectSound."));
	qcbOutput->setObjectName(QLatin1String("Output"));

	if (qcbOutput->count() < 2) {
		qcbOutput->setEnabled(false);
	}

	grid->addWidget(l, 1, 0);
	grid->addWidget(qcbOutput, 1, 1);


	qcbOutputDevice = new QComboBox(qgbOutput);
	l = new QLabel(tr("Device"), qgbOutput);
	l->setBuddy(qcbOutputDevice);

	qcbOutputDevice->setToolTip(tr("Output device to use"));
	qcbOutputDevice->setWhatsThis(tr("<b>Selects which sound card to use for audio Output.</b>"));
	qcbOutputDevice->setObjectName(QLatin1String("OutputDevice"));

	grid->addWidget(l, 2, 0);
	grid->addWidget(qcbOutputDevice, 2, 1);

	qcbPositional = new QCheckBox(tr("Enable positional audio"), qgbOutput);
	qcbPositional->setToolTip(tr("Allows positioning of sound."));
	qcbPositional->setWhatsThis(tr("This allows Mumble to use positional audio to place voices."));
	qcbPositional->setObjectName(QLatin1String("Positional"));
	qcbPositional->setChecked(g.s.bPositionalAudio);
	grid->addWidget(qcbPositional, 3, 1);

	QVBoxLayout *v=new QVBoxLayout(qwpage);
	v->addWidget(qgbInput);
	v->addWidget(qgbOutput);

	on_Input_activated(qcbInput->currentIndex());
	on_Output_activated(qcbOutput->currentIndex());

	return qwpage;
}

CompletablePage *AudioWizard::positionalPage() {
	CompletablePage *qwpage = new CompletablePage(this);
	qwpage->setTitle(tr("Positional Audio"));
	qwpage->setSubTitle(tr("Adjusting attenuation of positional audio."));

	QVBoxLayout *v = new QVBoxLayout(qwpage);
	QLabel *l;

	l = new QLabel(tr("Mumble supports positional audio for some games, and will position the voice of other players relative to their position in game. Depending on their position, the "
	                  "volume of the voice will be changed between the speakers to simulate the direction and distance the other player is at. Such positioning depends on your speaker "
	                  "configuration being correct in your operating system, so a test is done here."), qwpage);
	l->setWordWrap(true);
	v->addWidget(l);
	l = new QLabel(tr("The graph below shows the position of <font color=\"red\">you</font>, the <font color=\"yellow\">speakers</font> and a <font color=\"green\">moving sound source</font> as if seen from above. You should hear the audio move between the channels."), qwpage);
	l->setWordWrap(true);
	v->addWidget(l);

	qcbHeadphone = new QCheckBox(tr("Use headphones"), qwpage);
	qcbHeadphone->setToolTip(tr("Use headphones instead of speakers."));
	qcbHeadphone->setWhatsThis(tr("This ignores the OS speaker configuration and configures the positioning for headphones instead."));
	qcbHeadphone->setObjectName(QLatin1String("Headphone"));
	qcbHeadphone->setChecked(g.s.bPositionalHeadphone);
	v->addWidget(qcbHeadphone);


	fAngle = 0.0f;
	fX = fY = 0.0f;
	qgsScene = NULL;
	qgiSource = NULL;
	aosSource = NULL;
	qgvView = new QGraphicsView(qwpage);
	qgvView->scale(1.0f, -1.0f);
	qgvView->viewport()->installEventFilter(this);

	v->addWidget(qgvView);

	return qwpage;
}

CompletablePage *AudioWizard::volumePage() {
	CompletablePage *qwpage = new CompletablePage(this);
	qwpage->setTitle(tr("Volume tuning"));
	qwpage->setSubTitle(tr("Tuning microphone hardware volume to optimal settings."));

	QVBoxLayout *v = new QVBoxLayout(qwpage);
	QLabel *l;

	l = new QLabel(tr("Open your sound control panel and go to the recording settings. "
	                  "Make sure the microphone is selected as active input with maximum recording volume. If there's an option to enable a \"Microphone boost\" make sure it's checked."), qwpage);
	l->setWordWrap(true);
	v->addWidget(l);

	l = new QLabel(tr("Speak loudly, as when you are annoyed or excited. "
	                  "Decrease the volume in the sound control panel until the bar below stays as high as possible in the blue and green "
	                  "but <b>not</b> the red "
	                  "zone while you speak."), qwpage);
	l->setWordWrap(true);
	v->addWidget(l);

	abAmplify = new AudioBar(qwpage);
	abAmplify->qcBelow = Qt::blue;
	v->addWidget(abAmplify);

	l = new QLabel(tr("Now talk softly, as you would when talking late at night and you don't want to disturb anyone. "
	                  "Adjust the slider below so that the bar moves into green when you talk, but stays blue while you're silent."), qwpage);
	l->setWordWrap(true);
	v->addWidget(l);

	qsMaxAmp = new QSlider(Qt::Horizontal, qwpage);
	qsMaxAmp->setRange(1, 32767);
	qsMaxAmp->setSingleStep(100);
	qsMaxAmp->setPageStep(1000);
	qsMaxAmp->setValue(g.s.iMinLoudness);
	qsMaxAmp->setObjectName(QLatin1String("MaxAmp"));

	v->addWidget(qsMaxAmp);

	return qwpage;
}

CompletablePage *AudioWizard::triggerPage() {
	CompletablePage *qwpage = new CompletablePage(this);
	qwpage->setTitle(tr("Voice Activity Detection"));
	qwpage->setSubTitle(tr("Letting Mumble figure out when you're talking and when you're silent."));

	QHBoxLayout *h;
	QVBoxLayout *v;

	v= new QVBoxLayout(qwpage);
	QLabel *l;

	l = new QLabel(tr("This will help Mumble figure out when you are talking. The first step is selecting which data value to use."), qwpage);
	l->setWordWrap(true);
	v->addWidget(l);

	h = new QHBoxLayout;

	qrPTT = new QRadioButton(tr("Push To Talk:"), qwpage);
	qrPTT->setObjectName(QLatin1String("PTT"));
	h->addWidget(qrPTT);

	skwPTT = new ShortcutKeyWidget(qwpage);
	skwPTT->setObjectName(QLatin1String("PTTKey"));
	foreach(const Shortcut &s, g.s.qlShortcuts) {
		if (s.iIndex == g.mw->gsPushTalk->idx) {
			skwPTT->setShortcut(s.qlButtons);
			break;
		}
	}
	h->addWidget(skwPTT);

	v->addLayout(h);

	qrSNR = new QRadioButton(tr("Signal-To-Noise ratio"), qwpage);
	qrSNR->setObjectName(QLatin1String("SNR"));
	v->addWidget(qrSNR);

	qrAmplitude = new QRadioButton(tr("Raw amplitude from input"), qwpage);
	qrAmplitude->setObjectName(QLatin1String("Amplitude"));
	v->addWidget(qrAmplitude);

	if (g.s.atTransmit == Settings::PushToTalk)
		qrPTT->setChecked(true);
	else if (g.s.vsVAD == Settings::Amplitude)
		qrAmplitude->setChecked(true);
	else
		qrSNR->setChecked(true);

	qwVAD = new QWidget(qwpage);
	v->addWidget(qwVAD);

	v = new QVBoxLayout(qwVAD);

	abVAD = new AudioBar(qwpage);
	abVAD->qcBelow = Qt::red;
	abVAD->qcInside = Qt::yellow;
	abVAD->qcAbove = Qt::green;

	v->addWidget(abVAD);

	l = new QLabel(tr("Next you need to adjust the following two sliders. The first few utterances you say should end up in the "
	                  "green area (definitive speech). While talking, you should stay inside the yellow (might be speech) and when you're "
	                  "not talking, everything should be in the red (definitively not speech)."), qwpage);
	l->setWordWrap(true);
	v->addWidget(l);

	qsMinVAD = new QSlider(Qt::Horizontal, qwpage);
	qsMinVAD->setRange(1, 32767);
	qsMinVAD->setSingleStep(100);
	qsMinVAD->setPageStep(1000);
	qsMinVAD->setValue(static_cast<int>(g.s.fVADmin * 32767.0));
	qsMinVAD->setObjectName(QLatin1String("VADmin"));

	v->addWidget(qsMinVAD);

	qsMaxVAD = new QSlider(Qt::Horizontal, qwpage);
	qsMaxVAD->setRange(1, 32767);
	qsMaxVAD->setSingleStep(100);
	qsMaxVAD->setPageStep(1000);
	qsMaxVAD->setValue(static_cast<int>(g.s.fVADmax * 32767));
	qsMaxVAD->setObjectName(QLatin1String("VADmax"));

	v->addWidget(qsMaxVAD);

	l = new QLabel(tr("Last, you need to adjust the grace time. Mumble may detect this much non-speech and still keep transmitting before "
	                  "it breaks off. This allows you to catch your breath while speaking."), qwpage);
	l->setWordWrap(true);
	v->addWidget(l);

	h = new QHBoxLayout;

	qsHoldtime = new QSlider(Qt::Horizontal, qwpage);
	qsHoldtime->setRange(20, 250);
	qsHoldtime->setSingleStep(5);
	qsHoldtime->setPageStep(25);
	qsHoldtime->setValue(g.s.iVoiceHold);
	qsHoldtime->setObjectName(QLatin1String("Holdtime"));

	qlHoldtime = new QLabel(qwpage);
	qlHoldtime->setMinimumWidth(40);
	on_Holdtime_valueChanged(qsHoldtime->value());

	h->addWidget(qsHoldtime);
	h->addWidget(qlHoldtime);

	v->addLayout(h);

	return qwpage;
}

CompletablePage *AudioWizard::deviceTuningPage() {
	CompletablePage *qwpage = new CompletablePage(this);
	qwpage->setTitle(tr("Device tuning"));
	qwpage->setSubTitle(tr("Changing hardware output delays to their minimum value."));

	QGridLayout *grid = new QGridLayout(qwpage);

	QLabel *l = new QLabel(tr("To keep latency to an absolute minium, it's important to buffer as little audio as possible on the soundcard. "
	                          "However, many soundcards report that they require a much smaller buffer than what they can actually work with, "
	                          "so the only way to set this value is to try and fail."), qwpage);
	l->setWordWrap(true);
	grid->addWidget(l, 0, 0, 1, 2);

	l = new QLabel(tr("You should hear a single tone that's changing in frequency. Change the slider below to the lowest value which gives <b>no</b> interruptions or jitter "
	                  "in the sound. Please note that local echo is disabled during this test to "
	                  "improve audio path recognition."), qwpage);
	l->setWordWrap(true);
	grid->addWidget(l, 1, 0, 1, 2);

	qsOutputDelay = new QSlider(Qt::Horizontal, qwpage);
	qsOutputDelay->setRange(1, 6);

	qsOutputDelay->setSingleStep(1);
	qsOutputDelay->setPageStep(2);
	qsOutputDelay->setValue(g.s.iOutputDelay);
	qsOutputDelay->setObjectName(QLatin1String("OutputDelay"));

	qlOutputDelay=new QLabel(qwpage);
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
	                  "a matching incoming buffer."), qwpage);
	l->setWordWrap(true);
	grid->addWidget(l, 3, 0, 1, 2);

	qlAudioPath = new QLabel(qwpage);
	grid->addWidget(qlAudioPath, 4, 0, 1, 2);

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

	AudioInputPtr ai = g.ai;
	g.ai.reset();

	while (! ai.unique()) {}
	ai.reset();

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

	AudioOutputPtr ao = g.ao;
	g.ao.reset();

	while (! ao.unique()) {}
	ao.reset();

	aosSource = NULL;

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

void AudioWizard::on_OutputDelay_valueChanged(int v) {
	qlOutputDelay->setText(tr("%1ms").arg(v*20));
	g.s.iOutputDelay = v;
	on_OutputDevice_activated(0);
	if (! bInit) {
		playChord();
	}
}

void AudioWizard::on_MaxAmp_valueChanged(int v) {
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
	qWarning("RequestPlayChord");
	AudioOutputPtr ao = g.ao;
	qWarning("%p %p", ao.get(), aosSource);
	if (! ao || aosSource)
		return;
	aosSource = ao->playSample(QLatin1String("skin:wb_male.spx"), true);
}

void AudioWizard::restartAudio() {
	AudioInputPtr ai = g.ai;
	AudioOutputPtr ao = g.ao;

	aosSource = NULL;

	g.ai.reset();
	g.ao.reset();

	while (! ai.unique() || ! ao.unique()) {}
	ai.reset();
	ao.reset();

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
	g.bEchoTest = false;
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
		abVAD->iValue = lroundf((32767.f/96.0f) * (96.0f + ai->dPeakMic));
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
		if (aosSource) {
			aosSource->fPos[0] = xp;
			aosSource->fPos[1] = 0;
			aosSource->fPos[2] = yp;
		}
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
	if (on) {
		g.s.vsVAD = Settings::SignalToNoise;
		updateTriggerWidgets(false);
		bTransmitChanged = true;
	}
}

void AudioWizard::on_Amplitude_clicked(bool on) {
	if (on) {
		g.s.vsVAD = Settings::Amplitude;
		updateTriggerWidgets(false);
		bTransmitChanged = true;
	}
}

void AudioWizard::on_PTT_clicked(bool on) {
	if (on) {
		updateTriggerWidgets(true);
		bTransmitChanged = true;
	}
}

void AudioWizard::on_PTTKey_keySet(bool valid) {
	if (valid)
		qrPTT->setChecked(true);
	else if (qrPTT->isChecked())
		qrSNR->setChecked(true);
	updateTriggerWidgets(valid);
	bTransmitChanged = true;
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

void AudioWizard::updateTriggerWidgets(bool ptt) {
	qwVAD->setEnabled(!ptt);
	qwpTrigger->setComplete(!ptt || (skwPTT->qlButtons.count() > 0));
}
