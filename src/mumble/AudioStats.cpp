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

#include "AudioStats.h"
#include "AudioInput.h"
#include "Global.h"
#include "smallft.h"

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

	float scale = (width() * 1.0f) / (iMax - iMin);
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

AudioEchoWidget::AudioEchoWidget(QWidget *p) : QGLWidget(p) {
	setMinimumSize(100, 60);
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

	ai->qmSpeex.lock();

	spx_int32_t sz;
	speex_echo_ctl(ai->sesEcho, SPEEX_ECHO_GET_IMPULSE_RESPONSE_SIZE, &sz);

	STACKVAR(spx_int32_t, w, sz);
	STACKVAR(float, W, sz);

	speex_echo_ctl(ai->sesEcho, SPEEX_ECHO_GET_IMPULSE_RESPONSE, w);

	ai->qmSpeex.unlock();

	int N = 160;
	int n = 2 * N;
	int M = sz / n;

	drft_lookup d;
	mumble_drft_init(&d, n);

	for(int j=0;j<M;j++) {
		for(int i=0;i<n;i++)
			W[j*n+i] = w[j*n+i] / (1.f * n);
		mumble_drft_forward(&d, & W[j*n]);
	}

	mumble_drft_clear(&d);

	float xscale = 1.0f / N;
	float yscale = 1.0f / M;

	glBegin(GL_QUADS);

	for (int j = 0; j < M; j++) {
		for (int i=1;i < N; i++) {
			float xa = i * xscale;
			float ya = j * yscale;

			float xb = xa + xscale;
			float yb = ya + yscale;

			mapEchoToColor(sqrtf(W[j*n+2*i]*W[j*n+2*i]+W[j*n+2*i-1]*W[j*n+2*i-1]) / 65536.f);
			glVertex2f(xa, ya);
			glVertex2f(xb, ya);
			glVertex2f(xb, yb);
			glVertex2f(xa, yb);
		}
	}

	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 0.0, 1.0);
	xscale = 1.0 / (2*n);
	yscale = 1.0 / (200. * 32767.);
	for(int i=0;i<2*n;i++) {
		glVertex2f(i*xscale, 0.5 + w[i] * yscale);
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
	if (ai.get() == NULL || ! ai->sppPreprocess)
		return;

	QPolygonF poly;

	ai->qmSpeex.lock();

	spx_int32_t ps_size = 0;
	speex_preprocess_ctl(ai->sppPreprocess, SPEEX_PREPROCESS_GET_PSD_SIZE, &ps_size);

	STACKVAR(spx_int32_t, noise, ps_size);
	STACKVAR(spx_int32_t, ps, ps_size);

	speex_preprocess_ctl(ai->sppPreprocess, SPEEX_PREPROCESS_GET_PSD, ps);
	speex_preprocess_ctl(ai->sppPreprocess, SPEEX_PREPROCESS_GET_NOISE_PSD, noise);

	ai->qmSpeex.unlock();

	qreal sx, sy;

	sx = (width() - 1.0f) / (ps_size * 1.0f);
	sy = height() - 1;

	poly << QPointF(0.0f, height() - 1);
	float fftmul = 1.0 / (32768.0);
	for (int i=0; i < ps_size; i++) {
		qreal xp, yp;
		xp = i * sx;
		yp = sqrtf(sqrtf(noise[i])) - 1;
		yp = yp * fftmul;
		yp = qMin(yp * 3000.0, 1.0);
		yp = (1 - yp) * sy;
		poly << QPointF(xp, yp);
	}

	poly << QPointF(width() - 1, height() - 1);
	poly << QPointF(0.0f, height() - 1);

	paint.setPen(Qt::blue);
	paint.setBrush(Qt::blue);
	paint.drawPolygon(poly);

	poly.clear();

	for (int i=0;i < ps_size; i++) {
		qreal xp, yp;
		xp = i * sx;
		yp = sqrtf(sqrtf(ps[i])) - 1;
		yp = yp * fftmul;
		yp = qMin(yp * 3000.0, 1.0);
		yp = (1 - yp) * sy;
		poly << QPointF(xp, yp);
	}

	paint.setPen(Qt::red);
	paint.drawPolyline(poly);
}

AudioStats::AudioStats(QWidget *p) : QDialog(p) {
	setAttribute(Qt::WA_DeleteOnClose, true);

	qtTick = new QTimer(this);
	qtTick->setObjectName(QLatin1String("Tick"));
	qtTick->start(50);

	setupUi(this);
	AudioInputPtr ai = g.ai;

	if (ai && ai->sesEcho) {
		qgbEcho->setVisible(true);
	}  else {
		qgbEcho->setVisible(false);
	}


	bTalking = false;

	abSpeech->iPeak = -1;
	abSpeech->qcBelow = Qt::red;
	abSpeech->qcInside = Qt::yellow;
	abSpeech->qcAbove = Qt::green;

	on_Tick_timeout();
}

void AudioStats::on_Tick_timeout() {
	AudioInputPtr ai = g.ai;

	if (ai.get() == NULL || ! ai->sppPreprocess)
		return;

	bool nTalking = ai->isTransmitting();

	QString txt;

	txt.sprintf("%06.2f dB",ai->dPeakMic);
	qlMicLevel->setText(txt);

	txt.sprintf("%06.2f dB",ai->dPeakSpeaker);
	qlSpeakerLevel->setText(txt);

	txt.sprintf("%06.2f dB",ai->dPeakSignal);
	qlSignalLevel->setText(txt);

	spx_int32_t ps_size = 0;
	speex_preprocess_ctl(ai->sppPreprocess, SPEEX_PREPROCESS_GET_PSD_SIZE, &ps_size);

	STACKVAR(spx_int32_t, noise, ps_size);
	STACKVAR(spx_int32_t, ps, ps_size);

	speex_preprocess_ctl(ai->sppPreprocess, SPEEX_PREPROCESS_GET_PSD, ps);
	speex_preprocess_ctl(ai->sppPreprocess, SPEEX_PREPROCESS_GET_NOISE_PSD, noise);

	float s = 0.0;
	float n = 0.0001;

	int start = (ps_size * 300) / SAMPLE_RATE;
	int stop = (ps_size * 2000) / SAMPLE_RATE;

	for(int i=start;i<stop;i++) {
		s += sqrtf(ps[i]);
		n += sqrtf(noise[i]);
	}

	txt.sprintf("%06.3f",s / n);
	qlMicSNR->setText(txt);

	spx_int32_t v;
	speex_preprocess_ctl(ai->sppPreprocess, SPEEX_PREPROCESS_GET_AGC_GAIN, &v);
	txt.sprintf("%03.0f%%",10000.0f / v);
	qlMicVolume->setText(txt);

	txt.sprintf("%03.0f%%",ai->fSpeechProb * 100.0f);
	qlSpeechProb->setText(txt);

	txt.sprintf("%04.1f kbit/s",ai->iBitrate / 1000.0f);
	qlBitrate->setText(txt);

	if (nTalking != bTalking) {
		bTalking = nTalking;
		QFont f = qlSpeechProb->font();
		f.setBold(bTalking);
		qlSpeechProb->setFont(f);
	}

	if (g.uiDoublePush > 1000000)
		txt = tr(">1000ms");
	else
		txt.sprintf("%04llums",g.uiDoublePush / 1000);
	qlDoublePush->setText(txt);

	abSpeech->iBelow = lroundf(g.s.fVADmin * 32767.0f);
	abSpeech->iAbove = lroundf(g.s.fVADmax * 32767.0f);

	if (g.s.vsVAD == Settings::Amplitude) {
		abSpeech->iValue = lroundf(32767.f * powf(10.0f, (ai->dPeakMic / 20.0f)));
	} else {
		abSpeech->iValue = lroundf(ai->fSpeechProb * 32767.0f);
	}

	abSpeech->update();

	anwNoise->update();
	if (aewEcho)
		aewEcho->updateGL();
}
