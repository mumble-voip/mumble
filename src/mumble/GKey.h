// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2015, Jordan J Klassen <forivall@gmail.com>

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

#ifndef MUMBLE_MUMBLE_GKEY_H
#define MUMBLE_MUMBLE_GKEY_H

#include <QtCore/QLibrary>
#include <QtCore/QString>
#include <QtCore/QUuid>

#define GKEY_LOGITECH_DLL_REG_HKEY HKEY_CLASSES_ROOT
#ifdef _M_X64
#define GKEY_LOGITECH_DLL_REG_PATH L"Wow6432Node\\CLSID\\{7bded654-f278-4977-a20f-6e72a0d07859}\\ServerBinary"
#define GKEY_LOGITECH_DLL_DEFAULT_LOCATION "C:/Program Files/Logitech Gaming Software/SDK/G-key/x64/LogitechGkey.dll"
#else
#define GKEY_LOGITECH_DLL_REG_PATH L"CLSID\\{7bded654-f278-4977-a20f-6e72a0d07859}\\ServerBinary"
#define GKEY_LOGITECH_DLL_DEFAULT_LOCATION "C:/Program Files/Logitech Gaming Software/SDK/G-key/x86/LogitechGkey.dll"
#endif

#define GKEY_MIN_MOUSE_BUTTON 6
#define GKEY_MAX_MOUSE_BUTTON 20
#define GKEY_MIN_KEYBOARD_BUTTON 1
#define GKEY_MAX_KEYBOARD_BUTTON 29
#define GKEY_MIN_KEYBOARD_MODE 1
#define GKEY_MAX_KEYBOARD_MODE 3

#define GKEY_BUTTON_MOUSE 1
#define GKEY_BUTTON_KEYBOARD 2

#define GKEY_MOUSE_GUID "c41e60af-9022-46cf-bc39-37981082d716"
#define GKEY_KEYBOARD_GUID "153e64e6-98c8-4e03-80ef-5ffd33d25b8a"

class GKeyLibrary
{
public:
	GKeyLibrary();
	virtual ~GKeyLibrary();
	bool isValid() const;
	static const QUuid quMouse;
	static const QUuid quKeyboard;

	bool isMouseButtonPressed(int button);
	bool isKeyboardGkeyPressed(int key, int mode);
	QString getMouseButtonString(int button);
	QString getKeyboardGkeyString(int key, int mode);

protected:
	QLibrary qlLogiGkey;
	bool bValid;

	BOOL (*LogiGkeyInit)(void *);
	void (*LogiGkeyShutdown)();
	BOOL (*LogiGkeyIsMouseButtonPressed)(int button);
	BOOL (*LogiGkeyIsKeyboardGkeyPressed)(int key, int mode);
	wchar_t *(*LogiGkeyGetMouseButtonString)(int button);
	wchar_t *(*LogiGkeyGetKeyboardGkeyString)(int key, int mode);
};

#endif // MUMBLE_MUMBLE_GKEY_H
