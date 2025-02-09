// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "XboxInput.h"

#include <QtCore/QStringList>

const QUuid XboxInput::s_XboxInputGuid = QUuid(QString::fromLatin1("ca3937e3-640c-4d9e-9ef3-903f8b4fbcab"));

XboxInput::XboxInput()
	: GetState(nullptr), m_getStateFunc(nullptr), m_getStateExFunc(nullptr), m_xinputlib(nullptr), m_valid(false) {
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

	foreach (const QString &lib, alternatives) {
		m_xinputlib = LoadLibraryW(reinterpret_cast< const wchar_t * >(lib.utf16()));
		if (m_xinputlib) {
			qWarning("XboxInput: using XInput DLL '%s'", qPrintable(lib));
			m_valid = true;
			break;
		}
	}

	if (!m_valid) {
		qWarning("XboxInput: no valid XInput DLL was found on the system.");
		return;
	}

	m_getStateFunc = reinterpret_cast< XboxInputGetStateFunc >(GetProcAddress(m_xinputlib, "XInputGetState"));
	// Undocumented XInputGetStateEx -- ordinal 100. It is available in XInput 1.3 and greater.
	// It provides access to the state of the guide button.
	// For reference, see SDL's XInput support: http://www.libsdl.org/tmp/SDL/src/core/windows/SDL_xinput.c
	m_getStateExFunc = reinterpret_cast< XboxInputGetStateFunc >(GetProcAddress(m_xinputlib, (char *) 100));

	if (m_getStateExFunc) {
		GetState = m_getStateExFunc;
		qWarning("XboxInput: using XInputGetStateEx() as querying function.");
	} else if (m_getStateFunc) {
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
