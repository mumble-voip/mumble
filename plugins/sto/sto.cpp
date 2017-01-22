// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32_32bit.h" // Include standard plugin header.
#include <limits> // Include limits header for the "u8" function.

static procptr32_t identptr, contextptr, posptr;

static void inline u8(std::wstring &dst, const std::string &src) {
	if (src.length() > static_cast<size_t>(std::numeric_limits<int>::max())) {
		dst.clear();
		return;
	}

	int len = MultiByteToWideChar(CP_UTF8, 0, src.data(), static_cast<int>(src.length()), NULL, 0);

	wchar_t *wbuff = reinterpret_cast<wchar_t *>(_alloca(sizeof(wchar_t) * len));
	MultiByteToWideChar(CP_UTF8, 0, src.data(), static_cast<int>(src.length()), wbuff, len);

	dst.assign(wbuff, len);
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	char identblock[0x200];
	char contextblock[0x80];
	float posblock[64];

	if (! peekProc(identptr, identblock) ||
	        ! peekProc(contextptr, contextblock) ||
	        ! peekProc(posptr, posblock))
		return false;

	std::string ident = std::string(identblock+0x188, strnlen(identblock + 0x188, 0x78)) + std::string("@") + std::string(identblock, strnlen(identblock, 0x80));
	u8(identity, ident);

#ifdef PLUGIN_DEBUG
	printf("%ls\n", identity.c_str());
#endif

	context.assign(reinterpret_cast<char *>(contextblock), 4);
#ifdef PLUGIN_DEBUG
	DWORD *ctx = reinterpret_cast<DWORD *>(contextblock);
	printf("%08x\n", *ctx);
#endif

#ifdef PLUGIN_DEBUG
	printf("Avatar   %8.3f %8.3f %8.3f\n", posblock[0], posblock[1], posblock[2]);
	printf("AvatarF  %8.3f %8.3f %8.3f\n", posblock[15], posblock[16], posblock[17]);
	printf("Camera   %8.3f %8.3f %8.3f\n", posblock[3], posblock[4], posblock[5]);
	printf("CameraF  %8.3f %8.3f %8.3f\n", posblock[18], posblock[19], posblock[20]);
	printf("SpawnST  %d\n", * reinterpret_cast<DWORD *>(& posblock[52]));
#endif
	if (* reinterpret_cast<DWORD *>(& posblock[52]) == 1) {
		for (int i=0;i<3;++i) {
			avatar_pos[i] = posblock[i];
			avatar_front[i] = posblock[i+15];
			camera_pos[i] = posblock[i+3];
			camera_front[i] = posblock[i+18];

			avatar_top[i] = camera_top[i] = 0.0f;
		}
	} else {
		for (int i=0;i<3;++i) {
			avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
		}
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	identptr = contextptr = posptr = 0;

	if (! initialize(pids, L"GameClient.exe"))
		return false;

	char version[17];
	peekProc(pModule + 0x1616080, version);
	version[16]=0;

	if (memcmp(version, "ST.0.20100217c.3", sizeof(version)) == 0) {
#ifdef PLUGIN_DEBUG
		printf("STO: WANTLINK %s\n", version);
#endif
		contextptr = pModule + 0x16b2cd4;
		identptr = contextptr + 0x444;
		posptr = pModule + 0x18c2340;

		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Star Trek Online");
}

static std::wstring description(L"Star Trek Online ST.0.20100208b.4");
static std::wstring shortname(L"Star Trek Online");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin stoplug = {
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

static MumblePlugin2 stoplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &stoplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &stoplug2;
}
