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

#ifndef _AUDIOWIZARD_H
#define _AUDIOWIZARD_H

#include "Settings.h"
#include "AudioStats.h"

class AudioWizard: public QWizard {
		Q_OBJECT
	protected:
		QComboBox *qcbInput, *qcbInputDevice;
		QComboBox *qcbOutput, *qcbOutputDevice;

		QSlider *qsOutputDelay;
		QLabel *qlOutputDelay;

		AudioBar *abAmplify;
		QSlider *qsMaxAmp;

		AudioBar *abVAD;
		QRadioButton *qrAmplitude, *qrSNR;
		QSlider *qsMinVAD, *qsMaxVAD;

		QSlider *qsHoldtime;
		QLabel *qlHoldtime;

		QLabel *qlAudioPath;

		QWizardPage *introPage();
		QWizardPage *devicePage();
		QWizardPage *volumePage();
		QWizardPage *triggerPage();
		QWizardPage *deviceTuningPage();
		QWizardPage *donePage();

		QHash<QString, QVariant> qhOldInputDevice;
		QHash<QString, QVariant> qhOldOutputDevice;
		Settings sOldSettings;

		QTimer *ticker;

		bool bInit;

		int iMaxPeak;
		int iTicks;

		void restartAudio();
		void playChord();
	public slots:
		void on_Input_activated(int);
		void on_InputDevice_activated(int);
		void on_Output_activated(int);
		void on_OutputDevice_activated(int);
		void on_OutputDelay_valueChanged(int);
		void on_MaxAmp_valueChanged(int);
		void on_Ticker_timeout();
		void on_VADmin_valueChanged(int);
		void on_VADmax_valueChanged(int);
		void on_Holdtime_valueChanged(int);
		void on_Amplitude_clicked(bool);
		void on_SNR_clicked(bool);
		void showPage(int);
	public:
		AudioWizard();
		void reject();
		void accept();
};

#endif
