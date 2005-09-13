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

	qlMicLevel->setToolTip(tr("Peak power in last frame"));
	qlMicLevel->setWhatsThis(tr("This shows the peak power in the last frame (20 ms), and is the same measurement "
								"as you would usually find displayed as \"input power\". Please disregard this and "
								"look at <b>Loudness</b> instead, which is much more steady and disregards outliers."
								));
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
								  "of a sentence is always detected as speech, the problem is the pauses between words and the "
								  "start of speech. It's hard to distinguish a sigh from a word starting with 'h'."));
	qlBitrate->setToolTip(tr("Bitrate of last frame"));
	qlBitrate->setWhatsThis(tr("This is the bitrate of the last transmitted frame (20 ms), and as such will jump up and down "
							   "as the VBR adjusts the quality. To adjust the peak bitrate, adjust <b>Compression Complexity</b> "
							   "in the Settings dialog"));

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
