/* Copyright (C) 2015, Mikkel Krautz <mikkel@krautz.dk>

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

#include "mumble_pch.hpp"

#include "XboxInput.h"

const QUuid XboxInputLibrary::quXboxInput = QUuid(QString::fromLatin1("ca3937e3-640c-4d9e-9ef3-903f8b4fbcab"));

XboxInputLibrary::XboxInputLibrary()
	: XInputGetState(NULL)
	, XInputGetStateEx(NULL)
	, bValid(false) {

	QStringList alternatives;
	alternatives << QLatin1String("XInput1_4.dll");
	alternatives << QLatin1String("XInput9_1_0.dll");
	alternatives << QLatin1String("xinput1_3.dll");
	alternatives << QLatin1String("xinput1_2.dll");
	alternatives << QLatin1String("xinput1_1.dll");

	foreach(const QString &lib, alternatives) {
		hXInput = LoadLibraryW(reinterpret_cast<const wchar_t *>(lib.utf16()));
		if (hXInput != NULL) {
			qWarning("XboxInputLibrary: using XInput DLL '%s'", qPrintable(lib));
			bValid = true;
			break;
		}
	}

	*(reinterpret_cast<void **>(&XInputGetState)) = reinterpret_cast<void *>(GetProcAddress(hXInput, "XInputGetState"));
	// Undocumented XInputGetStateEx -- ordinal 100. Might not be in the DLL, but if
	// if is, we want to use it. It provides access to the state of the guide button.
	*(reinterpret_cast<void **>(&XInputGetStateEx)) = reinterpret_cast<void *>(GetProcAddress(hXInput, (char *)100));

	if (XInputGetStateEx != NULL) {
		GetState = XInputGetStateEx;
		qWarning("XboxInputLibrary: using XInputGetStateEx as querying function");
	} else if (XInputGetState != NULL) {
		GetState = XInputGetState;
		qWarning("XboxInputLibrary: using XInputGetState as querying function");
	} else {
		bValid = false;
		qWarning("XboxInputLibrary: no valid querying function found");
	}
}

XboxInputLibrary::~XboxInputLibrary() {
	if (hXInput) {
		FreeLibrary(hXInput);
	}
}

bool XboxInputLibrary::isValid() const {
	return bValid;
}
