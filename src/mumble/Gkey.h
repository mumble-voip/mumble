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

#ifndef MUMBLE_MUMBLE_GKEY_H
#define MUMBLE_MUMBLE_GKEY_H

// #include <stdint.h>
#include <QtCore/QLibrary>
#include <QtCore/QString>

#ifdef _M_X64
#define GKEY_LOGITECH_DLL_REGKEY "HKEY_CLASSES_ROOT\\Wow6432Node\\CLSID\\{7bded654-f278-4977-a20f-6e72a0d07859}\\ServerBinary"
// TODO: lookup from windows registry instead
#define GKEY_LOGITECH_DLL_DEFAULT_LOCATION "C:/Program Files/Logitech Gaming Software/SDK/G-key/x64/LogitechGkey.dll"
#else
#define GKEY_LOGITECH_DLL_REGKEY "HKEY_CLASSES_ROOT\\CLSID\\{7bded654-f278-4977-a20f-6e72a0d07859}\\ServerBinary"
#define GKEY_LOGITECH_DLL_DEFAULT_LOCATION "C:/Program Files/Logitech Gaming Software/SDK/G-key/x86/LogitechGkey.dll"
#endif

#define GKEY_MIN_MOUSE_BUTTON 6
#define GKEY_MAX_MOUSE_BUTTON 20
#define GKEY_MIN_KEYBOARD_BUTTON 1
#define GKEY_MAX_KEYBOARD_BUTTON 29
#define GKEY_MIN_KEYBOARD_MODE 1
#define GKEY_MAX_KEYBOARD_MODE 3

typedef bool (*fnLogiGkeyInit)(void *);
typedef void (*fnLogiGkeyShutdown)();
typedef bool (*fnLogiGkeyIsMouseButtonPressed)(int button);
typedef bool (*fnLogiGkeyIsKeyboardGkeyPressed)(int key, int mode);
typedef wchar_t* (*fnLogiGkeyGetMouseButtonString)(int button);
typedef wchar_t* (*fnLogiGkeyGetKeyboardGkeyString)(int key, int mode);

class GkeyLibrary
{
public:
	GkeyLibrary();
	virtual ~GkeyLibrary();
	bool isValid() const;

	bool isMouseButtonPressed(int button);
	bool isKeyboardGkeyPressed(int key, int mode);
	QString getMouseButtonString(int button);
	QString getKeyboardGkeyString(int key, int mode);

protected:
	QLibrary qlLogiGkey;
	bool bValid;

	bool (*LogiGkeyInit)(void *);
	void (*LogiGkeyShutdown)();
	bool (*LogiGkeyIsMouseButtonPressed)(int button);
	bool (*LogiGkeyIsKeyboardGkeyPressed)(int key, int mode);
	wchar_t *(*LogiGkeyGetMouseButtonString)(int button);
	wchar_t *(*LogiGkeyGetKeyboardGkeyString)(int key, int mode);
};

#endif // MUMBLE_MUMBLE_GKEY_H
