// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* SPECIFICATION
 * The code interfacing with the Logitech G-Keys DLL was implemented using the
 * following spec:
 *
 * The G-keys DLL lives in
 * "C:\Program Files\Logitech Gaming Software\SDK\G-key\x64\LogitechGkey.dll" for x64 and
 * "C:\Program Files\Logitech Gaming Software\SDK\G-key\x86\LogitechGkey.dll" for x86.
 *
 * Its location can also be read from the registry, using the following keys:
 *
 * x86:
 * "HKEY_CLASSES_ROOT\CLSID\{7bded654-f278-4977-a20f-6e72a0d07859}\ServerBinary"
 *
 * x64:
 * "HKEY_CLASSES_ROOT\Wow6432Node\CLSID\{7bded654-f278-4977-a20f-6e72a0d07859}\ServerBinary"
 *
 * The registry keys are needed if a user installed the Logitech Gaming
 * Software in a non-standard location.
 *
 * The DLL has an init function, it's called "LogiGkeyInit". It takes a
 * pointer, but the parameter must always be nullptr. The function returns a BOOL
 * as a status code.
 *
 * The DLL also has a shutdown function, called "LogiGkeyShutdown". It takes
 * no parameters and does not return anything.
 *
 * You can poll for button states with the DLL using the functions
 * "LogiGkeyIsMouseButtonPressed" and "LogiGkeyIsKeyboardGkeyPressed".
 *
 * The function "LogiGkeyIsMouseButtonPressed" takes a single int parameter, a
 * button number. Mouse button numbers run from 6 up to and including 20. The
 * function returns a BOOL that is true if the button is pressed, and false if
 * not.
 *
 * The function "LogiGkeyIsKeyboardGkeyPressed" takes two int parameters, a
 * button number and a mode number. Keyboard button numbers run from 1 up to
 * and including 29. The mode number can 1, 2 or 3. The mode checks the button
 * state in a specific mode. Typically, one queries all buttons for all modes,
 * so one ends up with 29*3 calls to the function. The function returns a BOOL
 * that is true if the button in the given mode is pressed, and false if not.
 *
 * There are also two functions, "LogiGkeyGetMouseButtonString" and
 * "LogiGkeyGetKeyboardGkeyString". They take the same parameters as the
 * polling functions above, but do not check whether the button is pressed or
 * not. Instead, they return the name of the button being queried as a pointer
 * to a NUL-terminated array of wchar_t's. Presumably, the pointer will be
 * nullptr if the name cannot be retrieved or translated.
 */

/* USAGE
 * In order to use the gkeys on a logitech keyboard, any user must have the
 * Logitech Gaming Software version 8.55+ installed on their computer. Then
 * (re)start mumble. When mumble initializes the library, the LGS (Logitech
 * Gaming Software) will create a profile called "mumble", featuring the mumble
 * icon. In LGS, right click this icon, and select either "Set as Default" or
 * "Set as Persistent". (See "What are persistent and default profiles?" in the
 * LGS help by clicking on the "?" icon on the LGS window, or at
 * http://www.logitech.com/assets/51813/3/lgs-guide.pdf). If mumble is not set
 * as the default or persistent profile, then your keys will not be active
 * unless mumble is the active window.
 */

#include "GKey.h"

#ifdef Q_CC_GNU
#	define RESOLVE(var)                                                            \
		{                                                                           \
			var    = reinterpret_cast< __typeof__(var) >(qlLogiGkey.resolve(#var)); \
			bValid = bValid && var;                                                 \
		}
#else
#	define RESOLVE(var)                                                                          \
		{                                                                                         \
			*reinterpret_cast< void ** >(&var) = static_cast< void * >(qlLogiGkey.resolve(#var)); \
			bValid                             = bValid && var;                                   \
		}
#endif

const QUuid GKeyLibrary::quMouse    = QUuid(QString::fromLatin1(GKEY_MOUSE_GUID));
const QUuid GKeyLibrary::quKeyboard = QUuid(QString::fromLatin1(GKEY_KEYBOARD_GUID));

GKeyLibrary::GKeyLibrary() {
	QStringList alternatives;

	HKEY key   = nullptr;
	DWORD type = 0;
	WCHAR wcLocation[510];
	DWORD len = 510;
	if (RegOpenKeyEx(GKEY_LOGITECH_DLL_REG_HKEY, GKEY_LOGITECH_DLL_REG_PATH, 0, KEY_READ, &key) == ERROR_SUCCESS) {
		LONG err = RegQueryValueEx(key, L"", nullptr, &type, reinterpret_cast< LPBYTE >(wcLocation), &len);
		if (err == ERROR_SUCCESS && type == REG_SZ) {
			const auto qsLocation = QString::fromUtf16(reinterpret_cast< char16_t * >(wcLocation), len / 2);
			qWarning("GKeyLibrary: Found ServerBinary with libLocation = \"%s\", len = %lu", qPrintable(qsLocation),
					 static_cast< unsigned long >(len));
			alternatives << qsLocation;
		} else {
			qWarning("GKeyLibrary: Error looking up ServerBinary (Error: 0x%lx, Type: 0x%lx, len: %lu)",
					 static_cast< unsigned long >(err), static_cast< unsigned long >(type),
					 static_cast< unsigned long >(len));
		}
	}

	alternatives << QString::fromLatin1(GKEY_LOGITECH_DLL_DEFAULT_LOCATION);
	foreach (const QString &lib, alternatives) {
		qlLogiGkey.setFileName(lib);

		if (qlLogiGkey.load()) {
			bValid = true;
			break;
		}
	}

	RESOLVE(LogiGkeyInit);
	RESOLVE(LogiGkeyShutdown);
	RESOLVE(LogiGkeyIsMouseButtonPressed);
	RESOLVE(LogiGkeyIsKeyboardGkeyPressed);
	RESOLVE(LogiGkeyGetMouseButtonString);
	RESOLVE(LogiGkeyGetKeyboardGkeyString);

	if (bValid)
		bValid = LogiGkeyInit(nullptr);
}

GKeyLibrary::~GKeyLibrary() {
	if (LogiGkeyShutdown)
		LogiGkeyShutdown();
}

bool GKeyLibrary::isValid() const {
	return bValid;
}

bool GKeyLibrary::isMouseButtonPressed(int button) {
	return LogiGkeyIsMouseButtonPressed(button);
}

bool GKeyLibrary::isKeyboardGkeyPressed(int key, int mode) {
	return LogiGkeyIsKeyboardGkeyPressed(key, mode);
}

QString GKeyLibrary::getMouseButtonString(int button) {
	return QString::fromWCharArray(LogiGkeyGetMouseButtonString(button));
}

QString GKeyLibrary::getKeyboardGkeyString(int key, int mode) {
	return QString::fromWCharArray(LogiGkeyGetKeyboardGkeyString(key, mode));
}

#undef RESOLVE
