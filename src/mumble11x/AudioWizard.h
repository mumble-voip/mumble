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

#ifndef _AUDIOWIZARD_H
#define _AUDIOWIZARD_H

#include "Settings.h"
#include "AudioStats.h"
#include "AudioOutput.h"
#include "GlobalShortcut.h"

class CompletablePage : public QWizardPage {
		Q_OBJECT
	protected:
		bool bComplete;
	public:
		CompletablePage(QWizard *p = NULL);
		void setComplete(bool);
		bool isComplete() const;
};

class AudioWizard: public QWizard {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioWizard)
	protected:
		QComboBox *qcbInput, *qcbInputDevice;
		QComboBox *qcbOutput, *qcbOutputDevice;
		QCheckBox *qcbEcho;
		QCheckBox *qcbPositional;

		QSlider *qsOutputDelay;
		QLabel *qlOutputDelay;

		AudioBar *abAmplify;
		QSlider *qsMaxAmp;

		QWidget *qwVAD;
		AudioBar *abVAD;
		QRadioButton *qrAmplitude, *qrSNR, *qrPTT;
		QSlider *qsMinVAD, *qsMaxVAD;
		ShortcutKeyWidget *skwPTT;
		bool bTransmitChanged;

		QSlider *qsHoldtime;
		QLabel *qlHoldtime;

		QLabel *qlAudioPath;

		QGraphicsView *qgvView;
		QGraphicsScene *qgsScene;
		QGraphicsItem *qgiSource;
		QCheckBox *qcbHeadphone;
		AudioOutputSample *aosSource;
		float fAngle;
		float fX, fY;

		QCheckBox *qcbUsage;

		CompletablePage *qwpIntro, *qwpDevice, *qwpVolume, *qwpTrigger, *qwpDeviceTuning, *qwpPositional, *qwpDone;

		CompletablePage *introPage();
		CompletablePage *devicePage();
		CompletablePage *volumePage();
		CompletablePage *triggerPage();
		CompletablePage *deviceTuningPage();
		CompletablePage *positionalPage();
		CompletablePage *donePage();

		Settings sOldSettings;

		QTimer *ticker;

		bool bInit;

		bool bDelay;

		int iMaxPeak;
		int iTicks;

		void restartAudio();
		void playChord();

		bool eventFilter(QObject *, QEvent *);
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
		void on_PTT_clicked(bool);
		void on_Echo_clicked(bool);
		void on_Headphone_clicked(bool);
		void on_Positional_clicked(bool);
		void on_PTTKey_keySet(bool);
		void showPage(int);
		void updateTriggerWidgets(bool);
	public:
		AudioWizard(QWidget *parent);
		void reject();
		void accept();
		bool validateCurrentPage();
		virtual int nextId() const;
};

#endif
