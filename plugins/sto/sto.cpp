// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32_32bit.h" // Include standard plugin header.
#include <limits> // Include limits header for the "u8" function.

// Memory offsets
const procptr32_t position_offset	= 0x18C2340;
const procptr32_t state_offset		= position_offset + 0xD0;
const procptr32_t context_offset	= 0x16B2CD4;
const procptr32_t identity_offset	= context_offset + 0x444;
const procptr32_t version_offset	= 0x1616080;

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
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	// Boolean value to check if game addresses retrieval is successful
	bool ok;
	// Char values for extra features
	char version[17], identity_block[0x200], context_block[0x80];
	// State
	unsigned char state;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(pModule + state_offset, &state, 1)							&& // Magical state value: 1 when spawned.
			peekProc(pModule + position_offset, avatar_pos, 12)					&& // Avatar Position values (X, Y and Z).
			peekProc(pModule + position_offset + 0xC, camera_pos, 12)			&& // Camera Position values (X, Y and Z).
			peekProc(pModule + position_offset + 0x3C, avatar_front, 12)		&& // Front vector values (X, Y and Z).
			peekProc(pModule + position_offset + 0x48, camera_front, 12)		&& // Top vector values (X, Y and Z).
			peekProc(pModule + context_offset, context_block)					&& // Context block.
			peekProc(pModule + identity_offset, identity_block)					&& // Identity block.
			peekProc(pModule + version_offset, version);						   // Game version

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from memory addresses.
	if (! ok)
		return false;

	// Check if the version is supported by the plugin.
	version[16] = 0;
	if (memcmp(version, "ST.0.20100217c.3", sizeof(version)) != 0)
		return false;

	// Assign context
	context.assign(reinterpret_cast<char *>(context_block), 4);

	// Assign identity
	std::string identity_construct = std::string(identity_block+0x188, strnlen(identity_block + 0x188, 0x78)) + std::string("@") + std::string(identity_block, strnlen(identity_block, 0x80));
	u8(identity, identity_construct);

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"GameClient.exe"))
		return false;

	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::wstring sidentity;
	std::string scontext;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, scontext, sidentity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Star Trek Online version ST.0.20100208b.4 with context and identity support.");
}

static std::wstring description(L"Star Trek Online (ST.0.20100208b.4)");
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
