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

const QUuid XboxInput::s_XboxInputGuid = QUuid(QString::fromLatin1("ca3937e3-640c-4d9e-9ef3-903f8b4fbcab"));

XboxInput::XboxInput()
	: GetState(NULL)
	, m_getStateFunc(NULL)
	, m_getStateExFunc(NULL)
	, m_xinputlib(NULL)
	, m_valid(false) {

	// Load the most suitable XInput DLL available.
	//
	// We prefer 1_4 and 1_3 over the others because they provide
	// XInputGetStateEx(), which allows us to query the state of
	// the guide button.
	//
	// See https://msdn.microsoft.com/en-us/library/windows/desktop/hh405051(v=vs.85).aspx
	// for more information.
	QStringList alternatives;
	alternatives << QLatin1String("XInput1_4.dll");
	alternatives << QLatin1String("xinput1_3.dll");
	alternatives << QLatin1String("XInput9_1_0.dll");
	alternatives << QLatin1String("xinput1_2.dll");
	alternatives << QLatin1String("xinput1_1.dll");

	foreach(const QString &lib, alternatives) {
		m_xinputlib = LoadLibraryW(reinterpret_cast<const wchar_t *>(lib.utf16()));
		if (m_xinputlib != NULL) {
			qWarning("XboxInput: using XInput DLL '%s'", qPrintable(lib));
			m_valid = true;
			break;
		}
	}

	if (!m_valid) {
		qWarning("XboxInput: no valid XInput DLL was found on the system.");
		return;
	}

	*(reinterpret_cast<void **>(&m_getStateFunc)) = reinterpret_cast<void *>(GetProcAddress(m_xinputlib, "XInputGetState"));
	// Undocumented XInputGetStateEx -- ordinal 100. It is available in XInput 1.3 and greater.
	// It provides access to the state of the guide button.
	// For reference, see SDL's XInput support: http://www.libsdl.org/tmp/SDL/src/core/windows/SDL_xinput.c
	*(reinterpret_cast<void **>(&m_getStateExFunc)) = reinterpret_cast<void *>(GetProcAddress(m_xinputlib, (char *)100));

	if (m_getStateExFunc != NULL) {
		GetState = m_getStateExFunc;
		qWarning("XboxInput: using XInputGetStateEx() as querying function.");
	} else if (m_getStateFunc != NULL) {
		GetState = m_getStateFunc;
		qWarning("XboxInput: using XInputGetState() as querying function.");
	} else {
		m_valid = false;
		qWarning("XboxInput: no valid querying function found.");
	}
}

XboxInput::~XboxInput() {
	if (m_xinputlib) {
		FreeLibrary(m_xinputlib);
	}
}

bool XboxInput::isValid() const {
	return m_valid;
}
