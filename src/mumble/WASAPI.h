/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _WASAPI_H
#define _WASAPI_H

#include "AudioInput.h"
#include "AudioOutput.h"
#include "ConfigDialog.h"
#include "ui_WASAPI.h"
#include <mmreg.h>
#include <strsafe.h>
#include <mmdeviceapi.h>
#include <avrt.h>
#include <audioclient.h>
#include <ksmedia.h>
#include <functiondiscoverykeys.h>
#include <propidl.h>
#include <initguid.h>
#include <delayimp.h>


class WASAPIConfig : public ConfigWidget, public Ui::WASAPIConfig {
		Q_OBJECT
	public:
		WASAPIConfig(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void save() const;
		void load(const Settings &r);
		bool expert(bool);
};

class WASAPISystem : public QObject {
	Q_OBJECT
	public:
		static const QHash<QString, QString> getDevices(EDataFlow dataflow);
		static const QHash<QString, QString> getInputDevices();
		static const QHash<QString, QString> getOutputDevices();
		static const QList<audioDevice> mapToDevice(const QHash<QString, QString>, const QString &);
};

class WASAPIInput : public AudioInput {
		Q_OBJECT
	public:
		WASAPIInput();
		~WASAPIInput();
		void run();
};

class WASAPIOutput : public AudioOutput {
		Q_OBJECT
	public:
		WASAPIOutput();
		~WASAPIOutput();
		void run();
};

#endif
