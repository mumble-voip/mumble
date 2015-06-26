/* Copyright (C) 2015, Jordan J Klassen <forivall@gmail.com>

   Some rights reserved.

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

#include "mumble_pch.hpp"

#include "Gkey.h"

#ifdef Q_CC_GNU
#define RESOLVE(var) { var = reinterpret_cast<__typeof__(var)>(qlLogiGkey.resolve(#var)); bValid = bValid && (var != NULL); }
#else
#define RESOLVE(var) { * reinterpret_cast<void **>(&var) = static_cast<void *>(qlLogiGkey.resolve(#var)); bValid = bValid && (var != NULL); }
#endif

GkeyLibrary::GkeyLibrary()
{
	qWarning("GlobalShortcutWinGkey: initialize and load");
	// TODO: lookup from registry first and then try the default location
	qlLogiGkey.setFileName(QString::fromLatin1(GKEY_LOGITECH_DLL_DEFAULT_LOCATION));

	if (!qlLogiGkey.load())
		bValid = false;

	RESOLVE(LogiGkeyInit);
	RESOLVE(LogiGkeyShutdown);
	RESOLVE(LogiGkeyIsMouseButtonPressed);
	RESOLVE(LogiGkeyIsKeyboardGkeyPressed);
	RESOLVE(LogiGkeyGetMouseButtonString);
	RESOLVE(LogiGkeyGetKeyboardGkeyString);

	if (bValid)
		bValid = LogiGkeyInit(NULL);
}

GkeyLibrary::~GkeyLibrary() {
	if (LogiGkeyShutdown != NULL)
		LogiGkeyShutdown();
}

bool GkeyLibrary::isValid() const {
	return bValid;
}

bool GkeyLibrary::isMouseButtonPressed(int button) {
	return LogiGkeyIsMouseButtonPressed(button);
}

bool GkeyLibrary::isKeyboardGkeyPressed(int key, int mode) {
	return LogiGkeyIsKeyboardGkeyPressed(key, mode);
}

QString GkeyLibrary::getMouseButtonString(int button) {
	return QString::fromWCharArray(LogiGkeyGetMouseButtonString(button));
}

QString GkeyLibrary::getKeyboardGkeyString(int key, int mode) {
	return QString::fromWCharArray(LogiGkeyGetKeyboardGkeyString(key, mode));
}
