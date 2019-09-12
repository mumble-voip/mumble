// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioStats.h"

#include "AudioInput.h"
#include "Global.h"
#include "Utils.h"
#include "smallft.h"

#include <QtGui/QPainter>

#include <cmath>

AudioBar::AudioBar(QWidget *p) : QWidget(p) {
	qcBelow = Qt::yellow;
	qcAbove = Qt::red;
	qcInside = Qt::green;

	iMin = 0;
	iMax = 32768;
	iBelow = 2000;
	iAbove = 22000;
	iValue = 1000;
	iPeak = -1;
	setMinimumSize(100,20);

	qlReplacableColors	<< Qt::yellow		<< Qt::red		<< Qt::green	<< Qt::blue;
	qlReplacementBrushes	<< Qt::BDiagPattern	<< Qt::DiagCrossPattern << Qt::NoBrush	<< Qt::FDiagPattern;
}

void AudioBar::paintEvent(QPaintEvent *) {
	QPainter p(this);

	if (isEnabled()) {
		qcBelow.setAlphaF(1.0f);
		qcAbove.setAlphaF(1.0f);
		qcInside.setAlphaF(1.0f);
	} else {
		qcBelow.setAlphaF(0.5f);
		qcAbove.setAlphaF(0.5f);
		qcInside.setAlphaF(0.5f);
	}

	if (iBelow > iAbove)
		iBelow = iAbove;

	if (iValue < iMin)
		iValue = iMin;
	else if (iValue > iMax)
		iValue = iMax;

	float scale = static_cast<float>(width()) / static_cast<float>(iMax - iMin);
	int h = height();

	int val = iroundf(static_cast<float>(iValue) * scale + 0.5f);
	int below = iroundf(static_cast<float>(iBelow) * scale + 0.5f);
	int above = iroundf(static_cast<float>(iAbove) * scale  + 0.5f);
	int max = iroundf(static_cast<float>(iMax) * scale + 0.5f);
	int min = iroundf(static_cast<float>(iMin) * scale + 0.5f);
	int peak = iroundf(static_cast<float>(iPeak) * scale + 0.5f);

	if (g.s.bHighContrast) {
		// Draw monochrome representation
		QColor fg = QPalette().windowText().color();

		p.fillRect(0, 0, below, h, QBrush(fg, qlReplacementBrushes.value(qlReplacableColors.indexOf(qcBelow), Qt::CrossPattern)));
		p.fillRect(below, 0, above - below, h, QBrush(fg, qlReplacementBrushes.value(qlReplacableColors.indexOf(qcInside), Qt::NoBrush)));
		p.fillRect(above, 0, max - above, h, QBrush(fg, qlReplacementBrushes.value(qlReplacableColors.indexOf(qcAbove), Qt::CrossPattern)));
		p.fillRect(0, 0, val, h, QBrush(fg, Qt::SolidPattern));

		p.drawRect(0, 0, max - 1, h - 1);
		p.drawLine(below, 0, below, h);
		p.drawLine(above, 0, above, h);
	} else {
		if (val <= below) {
			p.fillRect(0, 0, val, h, qcBelow);
			p.fillRect(val, 0, below-val, h, qcBelow.darker(300));
			p.fillRect(below, 0, above-below, h, qcInside.darker(300));
			p.fillRect(above, 0, max-above, h, qcAbove.darker(300));
		} else if (val <= above) {
			p.fillRect(0, 0, below, h, qcBelow);
			p.fillRect(below, 0, val-below, h, qcInside);
			p.fillRect(val, 0, above-val, h, qcInside.darker(300));
			p.fillRect(above, 0, max-above, h, qcAbove.darker(300));
		} else {
			p.fillRect(0, 0, below, h, qcBelow);
			p.fillRect(below, 0, above-below, h, qcInside);
			p.fillRect(above, 0, val-above, h, qcAbove);
			p.fillRect(val, 0, max-val, h, qcAbove.darker(300));
		}

		if ((peak >= min) && (peak <= max))  {
			if (peak <= below)
				p.setPen(qcBelow.lighter(150));
			else if (peak <= above)
				p.setPen(qcInside.lighter(150));
			else
				p.setPen(qcAbove.lighter(150));
			p.drawLine(peak, 0, peak, h);
		}
	}

}

AudioEchoWidget::AudioEchoWidget(QWidget *p) : QWidget(p) {
	setMinimumSize(100, 60);
}

static inline const QColor mapEchoToColor(float echo) {
	bool neg = (echo < 0.0f);
	echo = fabsf(echo);

	float a, b, c;

	if (echo > 1.0f) {
		echo = 1.0f;
		c = 0.5f;
	} else {
		c = 0.0f;
	}

	if (echo < 0.5f) {
		a = echo * 2.0f;
		b = 0.0f;
	} else {
		a = 1.0f;
		b = (echo - 0.5f) * 2.0f;
	}

	if (neg)
		return QColor::fromRgbF(a, b, c);
	else
		return QColor::fromRgbF(c, b, a);
}

#define WGT(x,y) st->W[(y)*N + 2*(x)+1]

void AudioEchoWidget::paintEvent(QPaintEvent *) {
	QPainter paint(this);

	paint.scale(width(), height());
	paint.fillRect(rect(), Qt::black);

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

	for (int j=0;j<M;j++) {
		for (int i=0;i<n;i++)
			W[j*n+i] = static_cast<float>(w[j*n+i]) / static_cast<float>(n);
		mumble_drft_forward(&d, & W[j*n]);
	}

	mumble_drft_clear(&d);

	float xscale = 1.0f / static_cast<float>(N);
	float yscale = 1.0f / static_cast<float>(M);

	for (int j = 0; j < M; j++) {
		for (int i=1;i < N; i++) {
			float xa = static_cast<float>(i) * xscale;
			float ya = static_cast<float>(j) * yscale;

			float xb = xa + xscale;
			float yb = ya + yscale;

			const QColor &c = mapEchoToColor(sqrtf(W[j*n+2*i]*W[j*n+2*i]+W[j*n+2*i-1]*W[j*n+2*i-1]) / 65536.f);
			paint.fillRect(QRectF(QPointF(xa, ya), QPointF(xb, yb)), c);
		}
	}

	QPolygonF poly;
	xscale = 1.0f / (2.0f * static_cast<float>(n));
	yscale = 1.0f / (200.0f * 32767.0f);
	for (int i = 0; i < 2 * n; i++) {
		poly << QPointF(static_cast<float>(i) * xscale, 0.5f + static_cast<float>(w[i]) * yscale);
	}
	
	paint.setPen(QPen(QBrush(QColor::fromRgbF(1.0f, 0.0f, 1.0f)), 0));
	paint.drawPolyline(poly);
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

	sx = (static_cast<float>(width()) - 1.0f) / static_cast<float>(ps_size);
	sy = static_cast<float>(height()) - 1.0f;

	poly << QPointF(0.0f, height() - 1);
	float fftmul = 1.0 / (32768.0);
	for (int i=0; i < ps_size; i++) {
		qreal xp, yp;
		xp = i * sx;
		yp = sqrtf(sqrtf(static_cast<float>(noise[i]))) - 1.0f;
		yp = yp * fftmul;
		yp = qMin<qreal>(yp * 3000.0f, 1.0f);
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
		yp = sqrtf(sqrtf(static_cast<float>(ps[i]))) - 1.0f;
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

AudioStats::~AudioStats() {
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

	float s = 0.0f;
	float n = 0.0001f;

	int start = (ps_size * 300) / SAMPLE_RATE;
	int stop = (ps_size * 2000) / SAMPLE_RATE;

	for (int i=start;i<stop;i++) {
		s += sqrtf(static_cast<float>(ps[i]));
		n += sqrtf(static_cast<float>(noise[i]));
	}

	txt.sprintf("%06.3f",s / n);
	qlMicSNR->setText(txt);

	spx_int32_t v;
	speex_preprocess_ctl(ai->sppPreprocess, SPEEX_PREPROCESS_GET_AGC_GAIN, &v);
	float fv = powf(10.0f, (static_cast<float>(v) / 20.0f));
	txt.sprintf("%03.0f%%",100.0f / fv);
	qlMicVolume->setText(txt);

	txt.sprintf("%03.0f%%",ai->fSpeechProb * 100.0f);
	qlSpeechProb->setText(txt);

	txt.sprintf("%04.1f kbit/s",static_cast<float>(ai->iBitrate) / 1000.0f);
	qlBitrate->setText(txt);

	if (nTalking != bTalking) {
		bTalking = nTalking;
		QFont f = qlSpeechProb->font();
		f.setBold(bTalking);
		qlSpeechProb->setFont(f);
	}

	if (g.uiDoublePush > 1000000)
		txt = tr(">1000 ms");
	else
		txt.sprintf("%04llu ms",g.uiDoublePush / 1000);
	qlDoublePush->setText(txt);

	abSpeech->iBelow = iroundf(g.s.fVADmin * 32767.0f + 0.5f);
	abSpeech->iAbove = iroundf(g.s.fVADmax * 32767.0f + 0.5f);

	if (g.s.vsVAD == Settings::Amplitude) {
		abSpeech->iValue = iroundf((32767.f/96.0f) * (96.0f + ai->dPeakCleanMic) + 0.5f);
	} else {
		abSpeech->iValue = iroundf(ai->fSpeechProb * 32767.0f + 0.5f);
	}

	abSpeech->update();

	anwNoise->update();
	if (aewEcho)
		aewEcho->update();
}
