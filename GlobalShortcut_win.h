/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#include "GlobalShortcut.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class GlobalShortcutWin : public QObject {
	Q_OBJECT
	public:
		QTimer *timer;
		int ref;
		static const GUID c_guidApp;

		LPDIRECTINPUT8 pDI;
		DIACTIONFORMAT        diafGame;
		QList<LPDIRECTINPUTDEVICE8> qlDevices;
		QHash<int, GlobalShortcut *> qmShortcuts;
		static BOOL CALLBACK EnumSuitableDevicesCB(LPCDIDEVICEINSTANCE, LPDIRECTINPUTDEVICE8, DWORD, DWORD, LPVOID);
		static BOOL CALLBACK EnumDevicesCB(LPCDIDEVICEINSTANCE, LPVOID);
		static BOOL CALLBACK EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
		BOOL bNeedRemap;
		DIACTION *diaActions;
		QString qsUsername;
	public slots:
		void timeTicked();
	public:
		GlobalShortcutWin();
		~GlobalShortcutWin();
		void unacquire();
		void remap();
		void add(GlobalShortcut *);
		void remove(GlobalShortcut *);
		void configure();
};
