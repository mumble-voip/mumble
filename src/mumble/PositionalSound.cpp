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

#include "mumble_pch.hpp"

#include "PositionalSound.h"

#include "Global.h"


PlotWidget::PlotWidget(QWidget *p, const QList<float> &xl,const QList<float> &yl) : QWidget(p), xdata(xl), ydata(yl) {
	setPalette(QPalette(QColor(255,255,255)));
	setAutoFillBackground(true);
	setMinimumSize(20,20);
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void PlotWidget::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	if (xdata.size() < 2 || ydata.size() < 2)
		return;

	float minx = xdata.front();
	float maxx = xdata.back();

	float miny = ydata.front();
	float maxy = ydata.back();

	float f;

	foreach(f,ydata) {
		miny = qMin(miny,f);
		maxy = qMax(maxy,f);
	}

	if (miny == maxy) {
		miny -= 1;
		maxy += 1;
	}

	const int hght = this->height();
	const int wdth = this->width();

	painter.drawLine(15,hght - 20, wdth, hght -20);
	painter.drawLine(20,hght - 15, 20, 0);

	float ticstep = 1;

	while ((maxx - minx) / ticstep > 10.9)
		ticstep *= 2;

	for (f = (lround(minx / ticstep) + 1) * ticstep; f < maxx; f += ticstep) {
		const int ipos = lround(20 + (wdth-20) * (f-minx) / (maxx-minx));

		painter.drawLine(ipos,hght - 20, ipos, hght -17);
		painter.drawText(QRect(ipos-20,hght-17,40,17),Qt::AlignCenter,QString::number(lroundf(f)));
	}

	ticstep = 1;

	while ((maxy - miny) / ticstep > 10.9)
		ticstep *= 2;

	for (f = (lround(miny / ticstep) + 1) * ticstep; f < maxy; f += ticstep) {
		const int ipos = lround((hght-20) *(1 - (f - miny) / (maxy-miny)));

		painter.drawLine(17 ,ipos , 20, ipos);
		painter.drawText(QRect(0,ipos-10,17,20),Qt::AlignCenter,QString::number(lroundf(f)));
	}

	QList<float>::const_iterator xit = xdata.begin();
	QList<float>::const_iterator yit = ydata.begin();

	float xold = *(xit++);
	float yold = *(yit++);

	painter.setPen(QColor(255,0,0));
	while (xit != xdata.end() && yit != ydata.end()) {
		painter.drawLine(
		    lround(20 + (wdth-20) * (xold-minx) / (maxx-minx)),
		    lround((hght-20) *(1 - (yold - miny) / (maxy-miny))),
		    lround(20 + (wdth-20) * (*xit-minx) / (maxx-minx)),
		    lround((hght-20) * (1 - (*yit - miny) / (maxy-miny))));
		xold = *(xit++);
		yold = *(yit++);
	}

}

int PlotWidget::heightForWidth(int w) const {
	return w * 3 / 4;
}

static ConfigWidget *PositionalSoundConfigDialogNew(Settings &st) {
	return new PositionalSoundConfig(st);
}

static ConfigRegistrar registrar(29, PositionalSoundConfigDialogNew);


PositionalSoundConfig::PositionalSoundConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	plot = new PlotWidget(qgbVolume,xdata,ydata);
	plot->setObjectName(QString::fromUtf8("Plot"));

	gridLayout->addWidget(plot, 3, 1, 1, 3);

	qcbModel->addItem(tr("constant"),Settings::CONSTANT);
	qcbModel->addItem(tr("linear"),Settings::LINEAR);

	update();
}

PositionalSoundConfig::~PositionalSoundConfig() {
	delete plot;
}

QString PositionalSoundConfig::title() const {
	return tr("Positional Sound");
}

QIcon PositionalSoundConfig::icon() const {
	return QIcon(QLatin1String("skin:config_dsound.png"));
}

void PositionalSoundConfig::save() const {
	s.ePositionalSoundModel = static_cast<Settings::PositionalSoundModels>(qcbModel->itemData(qcbModel->currentIndex()).toInt());

	s.fPositionalSoundDistance = qdsbDistance->value();
	s.fPositionalSoundPreGain = qdsbPreGain->value();
	s.fPositionalSoundMaxAtt = qdsbMaxAtt->value();

	s.bPositionalSoundEnable = qcbEnable->checkState() == Qt::Checked;
	s.bPositionalSoundSwap = qcbSwap->checkState() == Qt::Checked;
}

void PositionalSoundConfig::load(const Settings &r) {
	for (int i=0;i<qcbModel->count();i++) {
		if (qcbModel->itemData(i).toInt() == r.ePositionalSoundModel) {
			loadComboBox(qcbModel, i);
			break;
		}
	}

	qdsbDistance->setValue(r.fPositionalSoundDistance);
	qdsbPreGain->setValue(r.fPositionalSoundPreGain);
	qdsbMaxAtt->setValue(r.fPositionalSoundMaxAtt);

	qcbEnable->setCheckState(r.bPositionalSoundEnable ? Qt::Checked : Qt::Unchecked);
	qcbSwap->setCheckState(r.bPositionalSoundSwap ? Qt::Checked : Qt::Unchecked);

	update();
}

bool PositionalSoundConfig::expert(bool) {
	return false;
}

void PositionalSoundConfig::update() {

	const Settings::PositionalSoundModels model =
	    static_cast<Settings::PositionalSoundModels>(qcbModel->itemData(qcbModel->currentIndex()).toInt());

	const float distance = qdsbDistance->value();
	const float pregain = qdsbPreGain->value();
	const float maxatt = qdsbMaxAtt->value();

	float yold = pregain;
	float xmax = 1000;

	xdata.clear();
	ydata.clear();

	for (float xv = 0; xv < xmax; xv+= 1) {
		xdata << xv;

		float yv = 0;
		switch (model) {
			case Settings::CONSTANT:
				yv = PositionalSound::ModelConstant(pregain);
				break;
			case Settings::LINEAR:
				yv = PositionalSound::ModelLinear(pregain,maxatt,distance,xv);
				break;
		}

		ydata << yv;

		if (yold - yv < 1e-10 && xmax >= 1000 && xv > 0)
			xmax = 1.1*xv+1;

		yold = yv;
	}

	plot->update();
}

void PositionalSoundConfig::on_qcbEnable_stateChanged(int state) {
	if (state == Qt::Checked) {
		qcbSwap->setEnabled(true);
		qgbVolume->setEnabled(true);
	} else {
		qcbSwap->setEnabled(false);
		qgbVolume->setEnabled(false);
	}
}

void PositionalSoundConfig::on_qcbModel_currentIndexChanged(int i) {
	if (qcbModel->itemData(i).toInt() == Settings::CONSTANT) {
		qdsbDistance->setEnabled(false);
		qdsbMaxAtt->setEnabled(false);
	} else {
		qdsbDistance->setEnabled(true);
		qdsbMaxAtt->setEnabled(true);
	}

	update();
}

void PositionalSoundConfig::on_qdsbDistance_valueChanged(double) {
	update();
}

void PositionalSoundConfig::on_qdsbPreGain_valueChanged(double) {
	update();
}
void PositionalSoundConfig::on_qdsbMaxAtt_valueChanged(double) {
	update();
}

float PositionalSound::todB(float ratio) {
	return 20.0f * log10f(ratio);
}

float PositionalSound::toRatio(float dB) {
	return powf(10.0f, dB / 20.0f);
}

float PositionalSound::ModelConstant(float pregain) {
	return pregain;
}

float PositionalSound::ModelLinear(float pregain, float maxatt, float distance, float d) {
	float att = 10.0f * d/distance;
	return pregain - (att < maxatt ? att : maxatt);
}

float PositionalSound::calcdB(float d) {
	switch (g.s.ePositionalSoundModel) {
		case Settings::CONSTANT:
			return ModelConstant(g.s.fPositionalSoundPreGain);
		case Settings::LINEAR:
			return ModelLinear(g.s.fPositionalSoundPreGain, g.s.fPositionalSoundMaxAtt, g.s.fPositionalSoundDistance, d);
		default:
			break;
	}
	return 0.0f;
}
