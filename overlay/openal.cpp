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

#include "lib.h"
#include <al.h>

#define FDEF(name) static __typeof__(&name) o##name = NULL; static HardHook hh##name
#define INJECT(name) ({ o##name = reinterpret_cast<__typeof__(&name)>(GetProcAddress(hAL, #name)); if (o##name) { hh##name.setup(reinterpret_cast<voidFunc>(o##name), reinterpret_cast<voidFunc>(my##name)); hh##name.inject(); } else { ods("OpenAL: No GetProc for %s", #name);} })

FDEF(alListeneriv);
FDEF(alListenerfv);
FDEF(alListener3f);
FDEF(alListener3i);

static bool bHooked = false;
static bool bChaining = false;

static void __cdecl myalListenerfv(ALenum param, ALfloat *values) {
	ods("OpenAL: Chaining alListenerfv %x", param);
	hhalListenerfv.restore();
	oalListenerfv(param, values);
	hhalListenerfv.inject();

	if (bVideoHooked) {
		DWORD t = GetTickCount();
		switch(param) {
			case AL_POSITION:
			{
				uiAudioCount++;
				sm->uiAudioTick = t;
				sm->fPos[0] = values[0];
				sm->fPos[1] = values[1];
				sm->fPos[2] = values[2];
				break;
			}
			case AL_ORIENTATION:
			{
				uiAudioCount++;
				sm->uiAudioTick = t;
				sm->fFront[0] = values[0];
				sm->fFront[1] = values[1];
				sm->fFront[2] = values[2];
				sm->fTop[0] = values[3];
				sm->fTop[1] = values[4];
				sm->fTop[2] = values[5];
				break;
			}
			default:
				break;
		}
	}
}

static void __cdecl myalListeneriv(ALenum param, ALint *values) {
	ods("OpenAL: Chaining alListeneriv %x", param);
	hhalListeneriv.restore();
	oalListeneriv(param, values);
	hhalListeneriv.inject();

	if (bVideoHooked) {
		DWORD t = GetTickCount();
		switch(param) {
			case AL_POSITION:
			{
				uiAudioCount++;
				sm->uiAudioTick = t;
				sm->fPos[0] = values[0];
				sm->fPos[1] = values[1];
				sm->fPos[2] = values[2];
				break;
			}
			case AL_ORIENTATION:
			{
				uiAudioCount++;
				sm->uiAudioTick = t;
				sm->fFront[0] = values[0];
				sm->fFront[1] = values[1];
				sm->fFront[2] = values[2];
				sm->fTop[0] = values[3];
				sm->fTop[1] = values[4];
				sm->fTop[2] = values[5];
				break;
			}
			default:
				break;
		}
	}
}

static void __cdecl myalListener3f(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3) {
	ods("OpenAL: Chaining alListener3f %x", param);
	hhalListener3f.restore();
	oalListener3f(param, v1, v2, v3);
	hhalListener3f.inject();

	if (bVideoHooked) {
		DWORD t = GetTickCount();
		switch(param) {
			case AL_POSITION:
			{
				uiAudioCount++;
				sm->uiAudioTick = t;
				sm->fPos[0] = v1;
				sm->fPos[1] = v2;
				sm->fPos[2] = v3;
				break;
			}
			default:
				break;
		}
	}

}

static void __cdecl myalListener3i(ALenum param, ALint v1, ALint v2, ALint v3) {
	ods("OpenAL: Chaining alListener3i %x", param);
	hhalListener3i.restore();
	oalListener3i(param, v1, v2, v3);
	hhalListener3i.inject();

	if (bVideoHooked) {
		DWORD t = GetTickCount();
		switch(param) {
			case AL_POSITION:
			{
				uiAudioCount++;
				sm->uiAudioTick = t;
				sm->fPos[0] = v1;
				sm->fPos[1] = v2;
				sm->fPos[2] = v3;
				break;
			}
			default:
				break;
		}
	}
}

void checkOpenALHook() {
	if (bChaining) {
		return;
		ods("Causing a chain");
	}

	bChaining = true;

	HMODULE hAL = GetModuleHandle(L"OpenAL32.DLL");

	if (hAL != NULL) {
		if (! bHooked) {
			wchar_t procname[1024];
			GetModuleFileName(NULL, procname, 1024);
			ods("CreateWnd in unhooked OpenAL App %ls", procname);
			bHooked = true;

			INJECT(alListenerfv);
			INJECT(alListeneriv);
			INJECT(alListener3f);
			INJECT(alListener3i);
		}
	}

	bChaining = false;
}
