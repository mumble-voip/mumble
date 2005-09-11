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

#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>

#include "AudioStats.h"
#include "AudioInput.h"
#include "Global.h"


AudioStats::AudioStats(QWidget *p) : QDialog(p) {
	setAttribute(Qt::WA_DeleteOnClose, true);

	setWindowTitle("Mumble");

    QLabel *lab;
	QGridLayout *l=new QGridLayout;
    setLayout(l);

	lab = new QLabel(tr("Peak microphone level"), this);
	l->addWidget(lab, 0, 0);
	qlMicLevel = new QLabel(this);
	l->addWidget(qlMicLevel, 0, 1);

	lab = new QLabel(tr("Microphone loudness"), this);
	l->addWidget(lab, 1, 0);
	qlMicVolume = new QLabel(this);
	l->addWidget(qlMicVolume, 1, 1);

	lab = new QLabel(tr("Mic Signal-To-Noise"), this);
	l->addWidget(lab, 2, 0);
	qlMicSNR = new QLabel(this);
	l->addWidget(qlMicSNR, 2, 1);

	lab = new QLabel(tr("Speech Probability"), this);
	l->addWidget(lab, 3, 0);
	qlSpeechProb = new QLabel(this);
	l->addWidget(qlSpeechProb, 3, 1);

	lab = new QLabel(tr("Audio bitrate"), this);
	l->addWidget(lab, 4, 0);
	qlBitrate = new QLabel(this);
	l->addWidget(qlBitrate, 4, 1);

	qtTick = new QTimer(this);
	qtTick->setObjectName("Tick");
	qtTick->start(50);

    QMetaObject::connectSlotsByName(this);

    on_Tick_timeout();
}

void AudioStats::on_Tick_timeout() {
	if (! g.ai || ! g.ai->isRunning())
		return;

	QString txt;

	txt.sprintf("%06.2f dB",g.ai->dPeakMic);
	qlMicLevel->setText(txt);

	double level = g.ai->dLoudness / 200.0;

	txt.sprintf("%03.0f%%", level);
	qlMicVolume->setText(txt);

	txt.sprintf("%06.3f",g.ai->dSnr);
	qlMicSNR->setText(txt);

	txt.sprintf("%03.0f%%",g.ai->dSpeechProb * 100.0);
	qlSpeechProb->setText(txt);

	txt.sprintf("%04.1f kbit/s",g.ai->iBitrate / 1000.0);
	qlBitrate->setText(txt);
}
