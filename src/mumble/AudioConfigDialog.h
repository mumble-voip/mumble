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

#ifndef _AUDIOCONFIGDIALOG_H
#define _AUDIOCONFIGDIALOG_H

#include "ConfigDialog.h"

#include "ui_AudioInput.h"
#include "ui_AudioOutput.h"

class AudioInputDialog : public ConfigWidget, public Ui::AudioInput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioInputDialog)
	protected:
		QTimer *qtTick;
		void hideEvent(QHideEvent *event);
		void showEvent(QShowEvent *event);

	public:
		AudioInputDialog(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;

	public slots:
		void save() const;
		void load(const Settings &r);
		bool expert(bool);
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
		void on_qsNoise_valueChanged(int v);
		void on_qsIdle_valueChanged(int v);
		void on_qcbTransmit_currentIndexChanged(int v);
		void on_qcbSystem_currentIndexChanged(int);
		void on_Tick_timeout();
};

class AudioOutputDialog : public ConfigWidget, public Ui::AudioOutput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutputDialog)
	public:
		AudioOutputDialog(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void save() const;
		void load(const Settings &r);
		bool expert(bool);
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
};

#endif
