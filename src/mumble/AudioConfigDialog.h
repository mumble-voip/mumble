// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOCONFIGDIALOG_H_
#define MUMBLE_MUMBLE_AUDIOCONFIGDIALOG_H_

#include "ConfigDialog.h"

#include "ui_AudioInput.h"
#include "ui_AudioOutput.h"

class AudioInputDialog : public ConfigWidget, public Ui::AudioInput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioInputDialog)
	protected:
		QTimer *qtTick;
		void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
		void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

	public:
		AudioInputDialog(Settings &st);
		QString title() const Q_DECL_OVERRIDE;
		QIcon icon() const Q_DECL_OVERRIDE;

	public slots:
		void save() const Q_DECL_OVERRIDE;
		void load(const Settings &r) Q_DECL_OVERRIDE;
		void updateBitrate();
		void continuePlayback();

		void on_qcbPushClick_clicked(bool);
		void on_qpbPushClickBrowseOn_clicked();
		void on_qpbPushClickBrowseOff_clicked();
		void on_qpbPushClickPreview_clicked();
		void on_qpbPushClickReset_clicked();

		void on_qsTransmitHold_valueChanged(int v);
		void on_qsFrames_valueChanged(int v);
		void on_qsQuality_valueChanged(int v);
		void on_qsAmp_valueChanged(int v);
		void on_qsDoublePush_valueChanged(int v);
		void on_qsPTTHold_valueChanged(int v);
		void on_qsNoise_valueChanged(int v);
		void on_qcbTransmit_currentIndexChanged(int v);
		void on_qcbSystem_currentIndexChanged(int);
		void on_Tick_timeout();
		void on_qcbIdleAction_currentIndexChanged(int v);
};

class AudioOutputDialog : public ConfigWidget, public Ui::AudioOutput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutputDialog)
	public:
		AudioOutputDialog(Settings &st);
		QString title() const Q_DECL_OVERRIDE;
		QIcon icon() const Q_DECL_OVERRIDE;
	public slots:
		void save() const Q_DECL_OVERRIDE;
		void load(const Settings &r) Q_DECL_OVERRIDE;
		void on_qsDelay_valueChanged(int v);
		void on_qsJitter_valueChanged(int v);
		void on_qsVolume_valueChanged(int v);
		void on_qsOtherVolume_valueChanged(int v);
		void on_qsPacketDelay_valueChanged(int v);
		void on_qsPacketLoss_valueChanged(int v);
		void on_qcbLoopback_currentIndexChanged(int v);
		void on_qsMinDistance_valueChanged(int v);
		void on_qsMaxDistance_valueChanged(int v);
		void on_qsBloom_valueChanged(int v);
		void on_qsMaxDistVolume_valueChanged(int v);
		void on_qcbSystem_currentIndexChanged(int);
		void on_qcbPositional_stateChanged(int);
		void on_qcbAttenuateOthersOnTalk_clicked(bool checked);
		void on_qcbAttenuateOthers_clicked(bool checked);
		void on_qcbOnlyAttenuateSameOutput_clicked(bool checked);
};

#endif
