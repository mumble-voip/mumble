/*
 Copyright (c) 2009-2010 Ilmar 'Ingaras' Kruis (seaeagle1@users.sourceforge.net)

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

#include "../mumble_plugin_win32.h"

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool ok;

	byte l[2];
	byte r,i;
	float o[3];
	BYTE *hPtr;
	float h;

	/*
		Position as represented by /loc command

		lx, ly = 8 * (byte)l;
		ox, oy, oz = (float)o;

		0 < ox < 160

		x = west->east
		y = south->north
		z = altitude

		r = region
		i = instance nr

		nPtr = pointer to character name (unique on a server)
	*/

	ok = peekProc((BYTE *) 0x00F2D45C, o, 12) &&
	     peekProc((BYTE *) 0x00F2D454, l, 2) &&
	     peekProc((BYTE *) 0x00F2D450, &r, 1) &&
	     peekProc((BYTE *) 0x00F2D458, &i, 1) &&
	     peekProc((BYTE *)(pModule + 0x00C0F150), &hPtr, 4);

	if (! ok)
		return false;

	ok = peekProc((BYTE *)(hPtr  + 0x000007E0), &h, 4);

	if (! ok)
		return false;

	// Set identity
	//if(nPtr3 > 0)
	//	identity.assign(nPtr3);

	// Use region as context since each region has its own coordinate system
	if (r == 1)
		context = "Eriador";
	else if (r == 2)
		context = "Rhovannion";
	else
		return true;

	// If we're in an instance, append the instance id
	if (i != 0)
		context += i;

	// Heading should be between 0 and 360
	if (h < 0 || h > 360)
		return true;

	// Limit coordinates to byte-values, otherwise we probably have a read error
	if (l[0] == 255 && l[1] == 255)
		return true;

	avatar_pos[0] = (float)l[0] * 160.0f + o[0];
	avatar_pos[1] = o[2];
	avatar_pos[2] = (float)l[1] * 160.0f + o[1];

	avatar_front[0] = sinf(h * static_cast<float>(M_PI) / 180.0f);
	avatar_front[1] = 0.0f;
	avatar_front[2] = cosf(h * static_cast<float>(M_PI) / 180.0f);

	avatar_top[0] = 0.0;
	avatar_top[1] = 1.0;
	avatar_top[2] = 0.0;

	// TODO: 3rd person camera support
	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	if (! initialize(pids, L"lotroclient.exe"))
		return false;

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, context, identity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Lord of the Rings Online (Codemasters Edition, Vol III Update 1, v3.2.5.8031). Context support based on region and instance. No Identity support.");
}

static std::wstring description(L"Lord of the Rings Online (EU), Vol III Update 1");
static std::wstring shortname(L"Lord of the Rings Online");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin lotroplug = {
	MUMBLE_PLUGIN_MAGIC,
	description,
	shortname,
	NULL,
	NULL,
	trylock1,
	generic_unlock,
	longdesc,
	fetch
};

static MumblePlugin2 lotroplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &lotroplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &lotroplug2;
}
