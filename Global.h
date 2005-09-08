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

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "Settings.h"

#ifndef MUMBLE_VERSION
#define MUMBLE_RELEASE "Compiled " __DATE__ " " __TIME__
#else
#define XTEXT(X) #X
#define TEXT(X) XTEXT(X)
#define MUMBLE_RELEASE TEXT(MUMBLE_VERSION)
#endif

// Global helper class to spread variables around across threads.

class MainWindow;
class ServerHandler;
class AudioInput;
class AudioOutput;
class Database;

struct Global {
	MainWindow *mw;
	Settings s;
	ServerHandler *sh;
	AudioInput *ai;
	AudioOutput *ao;
	Database *db;
	Global();
};

// -Wshadow is bugged. If an inline function of a class uses a variable or
// parameter named 'g', that will generate a warning even if the class header
// is inclued long before this definition.

#define g g_global_struct
extern Global g_global_struct;

#else
class Settings;
#endif
