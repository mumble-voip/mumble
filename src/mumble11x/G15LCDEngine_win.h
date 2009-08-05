/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

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

#ifndef __G15LCDENGINE_WIN32__
#define __G15LCDENGINE_WIN32__

#include "mumble_pch.hpp"
#include "LCD.h"
#include "../../g15helper/g15helper.h"

class G15LCDDeviceWin;

class G15LCDEngineWin : public LCDEngine {
		friend class G15LCDDeviceWin;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(G15LCDEngineWin)
	protected:
		bool bUnavailable;
		bool bRunning;
		QProcess *qpHelper;
		QString qsHelperExecutable;
		bool framebufferReady() const;
		void submitFrame(bool alert, uchar *buf, size_t len);
		void setProcessStatus(bool run);
	public:
		G15LCDEngineWin();
		~G15LCDEngineWin();
		QList<LCDDevice *> devices() const;
	public slots:
		void on_Helper_finished(int exitCode, QProcess::ExitStatus status);
};

class G15LCDDeviceWin : public LCDDevice {
	protected:
		G15LCDEngineWin *engine;
		bool bEnabled;
	public:
		G15LCDDeviceWin(G15LCDEngineWin *e);
		~G15LCDDeviceWin();
		bool enabled();
		void setEnabled(bool e);
		void blitImage(QImage *img, bool alert);
		QString name() const;
		Type type() const;
		QSize size() const;
};

#else
class G15LCDEngine;
class G15LCDDevice;
#endif
