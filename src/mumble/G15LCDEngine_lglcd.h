/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2011, Mikkel Krautz <mikkel@krautz.dk>

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

#ifndef MUMBLE_MUMBLE_G15LCDENGINE_LGLCD_H_
#define MUMBLE_MUMBLE_G15LCDENGINE_LGLCD_H_

#include "mumble_pch.hpp"
#include "LCD.h"
#include "../../g15helper/g15helper.h"

class G15LCDDeviceLGLCD;

class G15LCDEngineLGLCD : public LCDEngine {
		friend class G15LCDDeviceLGLCD;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(G15LCDEngineLGLCD)
	protected:
		lgLcdConnectContextEx llcceConnect;
		lgLcdOpenByTypeContext llcContext;
	public:
		G15LCDEngineLGLCD();
		~G15LCDEngineLGLCD();
		QList<LCDDevice *> devices() const;
};

class G15LCDDeviceLGLCD : public LCDDevice {
	protected:
		G15LCDEngineLGLCD *engine;
		bool bEnabled;
	public:
		G15LCDDeviceLGLCD(G15LCDEngineLGLCD *e);
		~G15LCDDeviceLGLCD();
		bool enabled();
		void setEnabled(bool e);
		void blitImage(QImage *img, bool alert);
		QString name() const;
		QSize size() const;
};

#else
class G15LCDEngineLGLCD;
class G15LCDDeviceLGLCD;
#endif
