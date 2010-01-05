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

#ifndef _ASIOINPUT_H
#define _ASIOINPUT_H

#include <windows.h>
#include <asiodrvr.h>
#include "AudioInput.h"
#include "ConfigDialog.h"
#include "ui_ASIOInput.h"

typedef QPair<QString, QString> ASIODev;

class ASIOConfig : public ConfigWidget, public Ui::ASIOConfig {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ASIOConfig)
	protected:
		QList<ASIODev> qlDevs;
		bool bOk;
	public:
		ASIOConfig(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void save() const;
		void load(const Settings &r);
		bool expert(bool);
		void clearQuery();
		void on_qcbDevice_activated(int index);
		void on_qpbQuery_clicked();
		void on_qpbConfig_clicked();
		void on_qpbAddMic_clicked();
		void on_qpbRemMic_clicked();
		void on_qpbAddSpeaker_clicked();
		void on_qpbRemSpeaker_clicked();
};

#define IEEE754_64FLOAT 1
#include "asio.h"

class ASIOInput : public AudioInput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ASIOInput)
	protected:
		IASIO *iasio;

		int iNumMic, iNumSpeaker;
		long lBufSize;
		ASIOBufferInfo *abiInfo;
		ASIOChannelInfo *aciInfo;

		// ASIO Callbacks
		static ASIOInput *aiSelf;

		static void sampleRateChanged(ASIOSampleRate sRate);
		static long asioMessages(long selector, long value, void* message, double* opt);
		static void bufferSwitch(long index, ASIOBool processNow);
		static ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);

		void addBuffer(long sampType, int interleave, void *src, float * RESTRICT dst);
		void bufferReady(long index);
		bool initializeDriver();

		QWaitCondition qwDone;
	public:
		ASIOInput();
		~ASIOInput();
		void run();
};

#endif
