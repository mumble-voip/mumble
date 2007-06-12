/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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

#include "AudioStats.h"
#include "AudioInput.h"
#include "Global.h"

AudioEchoWidget::AudioEchoWidget(QWidget *p) : QGLWidget(p) {
	setMinimumSize(100, 60);
	mode = MODULUS;
}

void AudioEchoWidget::initializeGL() {
    glDisable(GL_LIGHTING);

	glClearColor(0,0,0,0);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_CULL_FACE);
}

void AudioEchoWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static inline void mapEchoToColor(float echo) {
	bool neg = (echo < 0);
	echo = fabs(echo);

	float a, b, c;

	if (echo > 1.0) {
		echo = 1.0;
		c = 0.5;
	} else {
		c = 0.0;
	}

	if (echo < 0.5) {
		a = echo * 2;
		b = 0;
	} else {
		a = 1;
		b = (echo - 0.5) * 2;
	}

	if (neg)
		glColor3f(a, b, c);
	else
		glColor3f(c, b, a);
}

#define WGT(x,y) st->W[(y)*N + 2*(x)+1]

void AudioEchoWidget::paintGL() {
	AudioInputPtr ai = g.ai;
	if (! ai || ! ai->sesEcho)
		return;

	CloneSpeexEchoState *st = reinterpret_cast<CloneSpeexEchoState *>(ai->sesEcho);

    int N = st->window_size;
    int n = N / 2;
    int M = st->M;

    double xscale = 1.0 / n;
    double yscale = 1.0 / M;


    glBegin(GL_QUADS);

    for(int j = 0; j < M; j++) {
    	for(int i=0;i < n; i++) {
			double xa = i * xscale;
			double ya = j * yscale;

			double xb = xa + xscale;
			double yb = ya + yscale;

			float real = 1.0;
			float imag = 0.0;
			if (i == 0)
				real = st->W[j*N];
			else if (i == n-1)
				real = st->W[j*N + 2*i];
			else {
				real = st->W[j*N + 2*i - 1];
				imag = st->W[j*N + 2*i];
			}

			float v = 0.0;

			switch (mode) {
				case REAL:
					v = real;
					break;
				case IMAGINARY:
					v = imag;
					break;
				case MODULUS:
					v = sqrt(real*real+imag*imag);
					break;
				case PHASE:
					v = atan2(imag,real)/M_PI;
					break;
			}

			mapEchoToColor(v);
			glVertex2f(xa, ya);
			glVertex2f(xb, ya);
			glVertex2f(xb, yb);
			glVertex2f(xa, yb);
		}
	}

	glEnd();
}

AudioNoiseWidget::AudioNoiseWidget(QWidget *p) : QWidget(p) {
	setMinimumSize(100,60);
}

void AudioNoiseWidget::paintEvent(QPaintEvent *) {
	QPainter paint(this);
	QPalette pal;

	paint.fillRect(rect(), pal.color(QPalette::Background));

	AudioInputPtr ai = g.ai;
	if (ai.get() == NULL || ! ai->isRunning() || ! ai->sppPreprocess)
		return;

	QPolygonF poly;

	CloneSpeexPreprocessState *st=reinterpret_cast<CloneSpeexPreprocessState *>(ai->sppPreprocess);

	qreal sx, sy;

	sx = (width() - 1.0f) / (st->ps_size * 1.0f);
	sy = height() - 1;

	poly << QPointF(0.0f, height() - 1);
#ifdef SPEEX_ANCIENT_PP
	float fftmul = 1.0 / (st->ps_size * 32768.0);
#else
	float fftmul = 1.0 / (32768.0);
#endif
	for(int i=0; i < st->ps_size; i++) {
		qreal xp, yp;
		xp = i * sx;
		yp = sqrt(st->noise[i]) - 1;
		yp = yp * fftmul;
		yp = fmin(yp * 30.0, 1.0);
		yp = (1 - yp) * sy;
		poly << QPointF(xp, yp);
	}

	poly << QPointF(width() - 1, height() - 1);
	poly << QPointF(0.0f, height() - 1);

	paint.setPen(Qt::blue);
	paint.setBrush(Qt::blue);
	paint.drawPolygon(poly);

	poly.clear();

	for(int i=0;i < st->ps_size; i++) {
		qreal xp, yp;
		xp = i * sx;
		yp = sqrt(st->ps[i]) - 1;
		yp = yp * fftmul;
		yp = fmin(yp * 30.0, 1.0);
		yp = (1 - yp) * sy;
		poly << QPointF(xp, yp);
	}

	paint.setPen(Qt::red);
	paint.drawPolyline(poly);
}

AudioStats::AudioStats(QWidget *p) : QDialog(p) {
	setAttribute(Qt::WA_DeleteOnClose, true);

	setWindowTitle(tr("Mumble"));

    QLabel *lab;
	QGridLayout *l=new QGridLayout;
    setLayout(l);

	lab = new QLabel(tr("Peak microphone level"), this);
	l->addWidget(lab, 0, 0);
	qlMicLevel = new QLabel(this);
	l->addWidget(qlMicLevel, 0, 1);

	lab = new QLabel(tr("Peak speaker level"), this);
	l->addWidget(lab, 1, 0);
	qlSpeakerLevel = new QLabel(this);
	l->addWidget(qlSpeakerLevel, 1, 1);

	lab = new QLabel(tr("Peak clean level"), this);
	l->addWidget(lab, 2, 0);
	qlSignalLevel = new QLabel(this);
	l->addWidget(qlSignalLevel, 2, 1);

	lab = new QLabel(tr("Microphone loudness"), this);
	l->addWidget(lab, 3, 0);
	qlMicVolume = new QLabel(this);
	l->addWidget(qlMicVolume, 3, 1);

	lab = new QLabel(tr("Mic Signal-To-Noise"), this);
	l->addWidget(lab, 4, 0);
	qlMicSNR = new QLabel(this);
	l->addWidget(qlMicSNR, 4, 1);

	lab = new QLabel(tr("Speech Probability"), this);
	l->addWidget(lab, 5, 0);
	qlSpeechProb = new QLabel(this);
	l->addWidget(qlSpeechProb, 5, 1);

	lab = new QLabel(tr("Audio bitrate"), this);
	l->addWidget(lab, 6, 0);
	qlBitrate = new QLabel(this);
	l->addWidget(qlBitrate, 6, 1);

	anwNoise = new AudioNoiseWidget(this);
	l->addWidget(anwNoise,7,0,1,2);
	l->setRowStretch(7, 1);

	AudioInputPtr ai = g.ai;
	if (ai && ai->sesEcho) {

		QHBoxLayout *hbox = new QHBoxLayout;

		QRadioButton *b;

		b = new QRadioButton(tr("Real"));
		qmEchoMode[b] = AudioEchoWidget::REAL;
		connect(b, SIGNAL(clicked(bool)), this, SLOT(onEchoMode(bool)));
		hbox->addWidget(b);

		b = new QRadioButton(tr("Imaginary"));
		qmEchoMode[b] = AudioEchoWidget::IMAGINARY;
		connect(b, SIGNAL(clicked(bool)), this, SLOT(onEchoMode(bool)));
		hbox->addWidget(b);

		b = new QRadioButton(tr("Modulus"));
		b->setChecked(true);
		qmEchoMode[b] = AudioEchoWidget::MODULUS;
		connect(b, SIGNAL(clicked(bool)), this, SLOT(onEchoMode(bool)));
		hbox->addWidget(b);

		b = new QRadioButton(tr("Phase"));
		qmEchoMode[b] = AudioEchoWidget::PHASE;
		connect(b, SIGNAL(clicked(bool)), this, SLOT(onEchoMode(bool)));
		hbox->addWidget(b);

		l->addLayout(hbox, 8, 0, 1, 2);

		aewEcho = new AudioEchoWidget(this);
		l->addWidget(aewEcho,9,0,1,2);
		l->setRowStretch(9, 1);
	} else {
		aewEcho = NULL;
	}

	qtTick = new QTimer(this);
	qtTick->setObjectName(QString::fromAscii("Tick"));
	qtTick->start(50);

	qlMicLevel->setToolTip(tr("Peak power in last frame"));
	qlMicLevel->setWhatsThis(tr("This shows the peak power in the last frame (20 ms), and is the same measurement "
								"as you would usually find displayed as \"input power\". Please disregard this and "
								"look at <b>Loudness</b> instead, which is much more steady and disregards outliers."
								));
	qlSpeakerLevel->setToolTip(tr("Peak power in last frame"));
	qlSpeakerLevel->setWhatsThis(tr("This shows the peak power in the last frame (20 ms) of the speakers. Unless you "
								"are using a multi-channel sampling method (such as ASIO) with speaker channels "
								"configured, this will be 0. If you have such a setup configured, and this still "
								"shows 0 while you're playing audio from other programs, your setup is not working."));
	qlSpeakerLevel->setToolTip(tr("Peak power in last frame"));
	qlSpeakerLevel->setWhatsThis(tr("This shows the peak power in the last frame (20 ms) after all processing. Ideally, "
								"this should be -96 dB when you're not talking. In reality, a sound studio should see "
								"-60 dB, and you should hopefully see somewhere around -20 dB. When you are talking, this "
								"should rise to somewhere between -5 and -10 dB.<br />"
								"If you are using echo cancellation, and this rises to more than -15 dB when you're not "
								"talking, your setup is not working, and you'll annoy other players with echoes."));
	qlMicVolume->setToolTip(tr("How close the current input level is to ideal"));
	qlMicVolume->setWhatsThis(tr("This shows how close your current input volume is to the ideal. To adjust your "
								 "microphone level, open whatever program you use to adjust the recording volume, "
								 "and look at the value here while talking.<br />"
								 "<b>Talk loud, as you would when you're upset over getting fragged by a noob.</b><br />"
								 "Adjust the volume until this value is close to 100%, but make sure it doesn't go above. "
								 "If it does go above, you are likely to get clipping in parts of your speech, which will "
								 "degrade sound quality"));
	qlMicSNR->setToolTip(tr("Signal-To-Noise ratio from the microphone"));
	qlMicSNR->setWhatsThis(tr("This is the Signal-To-Noise Ratio (SNR) of the microphone in the last frame (20 ms). "
								"It shows how much clearer "
								"the voice is compared to the noise.<br />If this value is below 1.0, there's more noise "
								"than voice in the signal, and so quality is reduced.<br />There is no upper limit to this "
								"value, but don't expect to see much above 40-50 without a sound studio."));
	qlSpeechProb->setToolTip(tr("Probability of speech"));
	qlSpeechProb->setWhatsThis(tr("This is the probability that the last frame (20 ms) was speech and not environment noise.<br />"
								  "Voice activity transmission depends on this being right. The trick with this is that the middle "
								  "of a sentence is always detected as speech; the problem is the pauses between words and the "
								  "start of speech. It's hard to distinguish a sigh from a word starting with 'h'.<br />"
								  "If this is in bold font, it means Mumble is currently transmitting (if you're connected)."));
	qlBitrate->setToolTip(tr("Bitrate of last frame"));
	qlBitrate->setWhatsThis(tr("This is the audio bitrate of the last compressed frame (20 ms), and as such will jump up and down "
							   "as the VBR adjusts the quality. To adjust the peak bitrate, adjust <b>Compression Complexity</b> "
							   "in the Settings dialog."));

	anwNoise->setToolTip(tr("Power spectrum of input signal and noise estimate"));
	anwNoise->setWhatsThis(tr("This shows the power spectrum of the current input signal (red line) and the current noise estimate "
							  "(filled blue).<br />"
							  "All amplitudes are multiplied by 30 to show the interesting parts (how much more signal than noise "
							  "is present in each waveband).<br />"
							  "This is probably only of interest if you're trying to fine-tune noise conditions on your microphone. "
							  "Under good conditions, there should be just a tiny flutter of blue at the bottom. If the blue is more than "
							  "halfway up on the graph, you have a seriously noisy environment."));

	if (aewEcho) {
		aewEcho->setToolTip(tr("Weights of the echo canceller"));
		aewEcho->setWhatsThis(tr("This shows the weights of the echo canceller, with time increasing downwards and frequency increasing to the right.<br />"
								  "Ideally, this should be black, indicating no echo exists at all. More commonly, you'll have one or more horizontal stripes "
								  "of bluish color representing time delayed echo. You should be able to see the weights updated in real time.<br />"
								  "Please note that as long as you have nothing to echo off, you won't see much useful data here. Play some music and "
								  "things should stabilize. <br />"
								  "You can choose to view the real or imaginary parts of the frequency-domain weights, or alternately the computed modulus and "
								  "phase. The most useful of these will likely be modulus, which is the amplitude of the echo, and shows you how much of the "
								  "outgoing signal is being removed at that time step. The other viewing modes are mostly useful to people who want to tune the "
								  "echo cancellation algorithms.<br />"
								  "Please note: If the entire image fluctuates massively while in modulus mode, "
								  "the echo canceller fails to find any correlation whatsoever between the two input "
								  "sources (speakers and microphone). Either you have a very long delay on the echo, or one of the input sources is configured wrong."
								  ));
	}

    QMetaObject::connectSlotsByName(this);

	bTalking = false;

    on_Tick_timeout();
}

void AudioStats::on_Tick_timeout() {
	AudioInputPtr ai = g.ai;

	if (ai.get() == NULL || ! ai->isRunning() || ! ai->sppPreprocess)
		return;

	CloneSpeexPreprocessState *st=reinterpret_cast<CloneSpeexPreprocessState *>(ai->sppPreprocess);
	bool nTalking = ai->isTransmitting();

	QString txt;

	txt.sprintf("%06.2f dB",ai->dPeakMic);
	qlMicLevel->setText(txt);

	txt.sprintf("%06.2f dB",ai->dPeakSpeaker);
	qlSpeakerLevel->setText(txt);

	txt.sprintf("%06.2f dB",ai->dPeakSignal);
	qlSignalLevel->setText(txt);

	txt.sprintf("%06.3f",ai->dSNR);
	qlMicSNR->setText(txt);

	txt.sprintf("%03.0f%%",100.0 / st->agc_gain);
	qlMicVolume->setText(txt);

	txt.sprintf("%03.0f%%",nTalking ? 100.0 : 0.0);
	qlSpeechProb->setText(txt);

	txt.sprintf("%04.1f kbit/s",ai->iBitrate / 1000.0);
	qlBitrate->setText(txt);

	if (nTalking != bTalking) {
		bTalking = nTalking;
		QFont f = qlSpeechProb->font();
		f.setBold(bTalking);
		qlSpeechProb->setFont(f);
	}

	anwNoise->update();
	if (aewEcho)
		aewEcho->updateGL();
}

void AudioStats::onEchoMode(bool) {
	if (aewEcho)
		aewEcho->mode = qmEchoMode[sender()];
}
