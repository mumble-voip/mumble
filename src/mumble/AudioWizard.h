// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOWIZARD_H_
#define MUMBLE_MUMBLE_AUDIOWIZARD_H_

#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
# include <QtWidgets/QWizard>
# include <QtWidgets/QWizardPage>
#else
# include <QtGui/QWizard>
# include <QtGui/QWizardPage>
#endif

#include "AudioStats.h"
#include "AudioOutput.h"
#include "GlobalShortcut.h"
#include "Settings.h"

class CompletablePage : public QWizardPage {
		Q_OBJECT
	protected:
		bool bComplete;
	public:
		CompletablePage(QWizard *p = NULL);
		void setComplete(bool);
		bool isComplete() const Q_DECL_OVERRIDE;
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

		bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;
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
		void reject() Q_DECL_OVERRIDE;
		void accept() Q_DECL_OVERRIDE;
		bool validateCurrentPage() Q_DECL_OVERRIDE;
		virtual int nextId() const Q_DECL_OVERRIDE;
};

#endif
