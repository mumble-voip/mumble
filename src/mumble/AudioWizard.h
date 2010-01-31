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

#include "ui_AudioWizard.h"

class AudioWizard: public QWizard, public Ui::AudioWizard {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioWizard)
	protected:
		bool bTransmitChanged;

		QGraphicsScene *qgsScene;
		QGraphicsItem *qgiSource;
		AudioOutputSample *aosSource;
		float fAngle;
		float fX, fY;

		Settings sOldSettings;

		QTimer *ticker;

		bool bInit;
		bool bDelay;
		bool bLastActive;

		QPixmap qpTalkingOn, qpTalkingOff;

		int iMaxPeak;
		int iTicks;

		void restartAudio();
		void playChord();

		bool eventFilter(QObject *, QEvent *);
	public slots:
		void on_qcbInput_activated(int);
		void on_qcbInputDevice_activated(int);
		void on_qcbOutput_activated(int);
		void on_qcbOutputDevice_activated(int);
		void on_qsOutputDelay_valueChanged(int);
		void on_qsMaxAmp_valueChanged(int);
		void on_Ticker_timeout();
		void on_qsVAD_valueChanged(int);
		void on_qrAmplitude_clicked(bool);
		void on_qrSNR_clicked(bool);
		void on_qrPTT_clicked(bool);
		void on_qcbEcho_clicked(bool);
		void on_qcbHeadphone_clicked(bool);
		void on_qcbPositional_clicked(bool);
		void on_qcbAttenuateOthers_clicked(bool);
		void on_qcbHighContrast_clicked(bool);
		void on_skwPTT_keySet(bool, bool);
		void on_qrbQualityUltra_clicked();
		void on_qrbQualityBalanced_clicked();
		void on_qrbQualityLow_clicked();
		void on_qrbQualityCustom_clicked();
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
