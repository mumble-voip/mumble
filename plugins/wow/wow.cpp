/* Copyright (C) 2010-2014, Jamie Fraser <jamie.f@mumbledog.com>
   Copyright (C) 2009-2010, deKarl <dekarl@users.sourceforge.net>
   Copyright (C) 2005-2012, Thorvald Natvig <thorvald@natvig.com>

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

/* stdint.h */
typedef int uint32_t;
typedef long long uint64_t;

#if _DEBUG
#endif

#include "../mumble_plugin_win32.h"

uint32_t p_playerBase;
uint64_t g_playerGUID;

/*
 * To update visit http://www.ownedcore.com/forums/world-of-warcraft/world-of-warcraft-bots-programs/wow-memory-editing
 * and look for a thread called [WoW][TheVersion] Info Dump Thread.
 *
 * Where possible, I have included in the comments the different names some posters
 * call each value, to ease in upgrading. "[_]" means the value name may or may not
 * have an underscore in it depending on who's posting the offset.
 */
static uint32_t ptr_ClientConnection=0xEC4628; // ClientConnection or CurMgrPointer
static size_t off_ObjectManager=0x462C; // objectManager or CurMgrOffset
static uint32_t ptr_WorldFrame=0xD64E5C; // Camera[_]Pointer
static size_t off_CameraOffset=0x8208; // Camera[_]Offset

uint32_t getInt32(uint32_t ptr) {
	uint32_t result;
	SIZE_T r;
	BOOL ok=ReadProcessMemory(hProcess, (void *)ptr, &result, sizeof(uint32_t), &r);
	if (ok && (r == sizeof(uint32_t))) {
		return result;
	} else {
		return 0xffffffff;
	}
}

uint64_t getInt64(uint32_t ptr) {
	uint64_t result;
	SIZE_T r;
	BOOL ok=ReadProcessMemory(hProcess, (void *)ptr, &result, sizeof(uint64_t), &r);
	if (ok && (r == sizeof(uint64_t))) {
		return result;
	} else {
		return 0xffffffffffffffff;
	}
}

float getFloat(uint32_t ptr) {
	float result;
	SIZE_T r;
	BOOL ok=ReadProcessMemory(hProcess, (void *)ptr, &result, sizeof(float), &r);
	if (ok && (r == sizeof(float))) {
		return result;
	} else {
		return (float)0xffffffff;
	}
}

int getCStringN(uint32_t ptr, char *buffer, size_t buffersize) {
	SIZE_T r;
	BOOL ok = ReadProcessMemory(hProcess, (void *)ptr, buffer, buffersize, &r);

	/* safety net, just in case we didn't get a string back at all */
	buffer[buffersize-1] = '\0';

	if (ok && (r == buffersize)) {
		return strlen(buffer);
	} else {
		return 0;
	}
}

int getString(uint32_t ptr, std::string &buffer) {
	char buf[1024];
	int bufLength;

	bufLength = getCStringN(ptr, buf, sizeof(buf));
	buffer = buf;

	return bufLength;
}

int getWString(uint32_t ptr, std::wstring &buffer) {
	char buf[1024];
	int bufLength;
	wchar_t wbuf[1024];
	int wbufLength;

	bufLength = getCStringN(ptr, buf, sizeof(buf));
	wbufLength = MultiByteToWideChar(CP_UTF8, 0,
	                                 buf, bufLength,
	                                 wbuf, 1024);
	buffer.assign(wbuf, wbufLength);

	return 0;
}

void getDebug16(uint32_t ptr) {
#ifdef _DEBUG
	unsigned char buf[16];
	SIZE_T r;
	BOOL ok=ReadProcessMemory(hProcess, (void *)ptr, &buf, sizeof(buf), &r);
	if (ok && (r == sizeof(buf))) {
		printf("%08x: %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n",
		       ptr,
		       buf[0], buf[1], buf[2], buf[3],
		       buf[4], buf[5], buf[6], buf[7],
		       buf[8], buf[9], buf[10], buf[11],
		       buf[12], buf[13], buf[14], buf[15]
		      );
	}
#endif
}

void stringDebug(std::string &theString) {
#ifdef _DEBUG
	std::cout << "String length=" << theString.length() << " content=\"" << theString << "\" debug=";
	for (size_t i=0; i<theString.length(); i++) {
		if (i>0) {
			std::cout << " ";
		}
		std::cout << (unsigned int)theString[i];
	}
	std::cout << std::endl;
#endif
}

uint32_t getPlayerBase() {
	uint32_t gClientConnection;
	uint32_t sCurMgr;
	uint32_t curObj;
	uint64_t playerGUID;
	uint32_t playerBase;

	uint32_t nextObj;
	uint64_t GUID;

	playerBase=0;

	gClientConnection=getInt32((uint32_t)pModule + ptr_ClientConnection);
	sCurMgr=getInt32(gClientConnection + off_ObjectManager);
	if (sCurMgr != 0) {
		playerGUID=getInt64(sCurMgr+0xE8); // localGUID
		if (playerGUID != 0) {
			g_playerGUID = playerGUID;
			curObj=getInt32(sCurMgr+0xCC); // firstObject
			while (curObj != 0) {
				nextObj=getInt32(curObj + 0x34); // nextObject
				GUID=getInt64(curObj + 0x28);
				if (playerGUID == GUID) {
					playerBase = curObj;
					break;
				} else if (curObj == nextObj) {
					break;
				} else {
					curObj = nextObj;
				}
			}
		}
	}

	return playerBase;
}

static const unsigned long nameStorePtr        = 0xC86358;  // Player name database
static const unsigned long nameMaskOffset      = 0x02c;  // Offset for the mask used with GUID to select a linked list
static const unsigned long nameBaseOffset      = 0x020;  // Offset for the start of the name linked list
static const unsigned long nameStringOffset    = 0x021;  // Offset to the C string in a name structure

void getPlayerName(std::wstring &identity) {
	/*
	** All the OwnedCore guys seem to be just pulling it from a simple pointer
	** instead of traversing through the NameStore, and since no one's updated
	** nameStorePtr yet I figured I'd try just doing it this way.
	*/
	getWString((uint32_t)pModule +0xEC4668, identity);
	return;

	/*
	** Old code below:
	unsigned long mask, base, offset, current, shortGUID, testGUID;

	mask = getInt32((uint32_t)pModule + nameStorePtr + nameMaskOffset);
	base = getInt32((uint32_t)pModule + nameStorePtr + nameBaseOffset);

	shortGUID = g_playerGUID & 0xffffffff;  // Only half the guid is used to check for a hit
	if (mask == 0xffffffff) {
		identity.clear();
		return;
	}
	offset = 12 * (mask & shortGUID);  // select the appropriate linked list
	current=getInt32(base + offset + 8);   // ptr to lower half of GUID of first element
	offset = getInt32(base + offset);  // this plus 4 is the offset for the next element
	if ((current == 0) || (current & 0x1)) {
		identity.clear();
		return;
	}
	testGUID=getInt32(current);

	while (testGUID != shortGUID) {
		current=getInt32(current + offset + 4);
		if ((current == 0) || (current & 0x1)) {
			identity.clear();
			return;
		}
		testGUID=getInt32(current);
	}
	getWString(current + nameStringOffset, identity);
*/
	//printf("%ls\n", identity.data());
}

void getCamera(float camera_pos[3], float camera_front[3], float camera_top[3]) {
	uint32_t ptr1, ptr2;
	float buf[4][3];

	ptr1 = getInt32((uint32_t)pModule + ptr_WorldFrame);
	ptr2 = getInt32(ptr1+off_CameraOffset);

	peekProc((BYTE *) ptr2+0x08, buf, sizeof(buf));

	/* camera postition */
	camera_pos[0] = -buf[0][1];
	camera_pos[1] =  buf[0][2];
	camera_pos[2] =  buf[0][0];

	/* camera front vector */
	camera_front[0] = -buf[1][1];
	camera_front[1] =  buf[1][2];
	camera_front[2] =  buf[1][0];

	/* camera right vector */
#if 0 /* gets calculated in mumble client */
	camera_right[0] = -buf[2][1];
	camera_right[1] =  buf[2][2];
	camera_right[2] =  buf[2][0];
#endif

	/* camera top vector */
	camera_top[0] = -buf[3][1];
	camera_top[1] =  buf[3][2];
	camera_top[2] =  buf[3][0];
}

typedef class WowData {
		std::wstring nameAvatar;
		bool nameAvatarValid;

		uint64_t playerGUID;
		uint32_t pointerPlayerObject;

	public:
		WowData::WowData() {
			refresh();
		}

		void WowData::updateAvatarName() {
			getPlayerName(nameAvatar);
			if (!nameAvatar.empty()) {
				nameAvatarValid = true;
			} else {
				nameAvatarValid = false;
			}
		}

		std::wstring getNameAvatar() {
			if (!nameAvatarValid) {
				updateAvatarName();
			}

			return nameAvatar;
		}

		void refresh() {
			nameAvatarValid = false;
		}
} WowData_t;

WowData_t wow;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	/* clear position */
	for (int i=0; i<3; i++) {
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=camera_pos[i]=camera_front[i]=camera_top[i]=0.0;
	}

	/* are we still looking at the right object? */
	uint64_t peekGUID, tempGUID;
	peekGUID=getInt64(p_playerBase+0x30);
	if (g_playerGUID != peekGUID) {
		/* no? Try to resynch to the new address. Happens when walking through portals quickly (aka no or short loading screen) */
		tempGUID = g_playerGUID;
		p_playerBase=getPlayerBase();
		if (tempGUID != g_playerGUID) {
			/* GUID of actor changed, likely a character and/or realm change */
			wow.refresh();
		}
		peekGUID=getInt64(p_playerBase+0x28);
		if (g_playerGUID != peekGUID) {
			/* no? we are still getting the expected GUID for our avatar, but we don't have it's current position */
			return true;
		}
	}
	context.clear();
	std::wstringstream identityStream;
	identityStream << wow.getNameAvatar();
	identity = identityStream.str();

	BOOL ok = true;

	// Wow stores as
	// North/South (+ North)
	// East/West (+ West)
	// Up/Down (+Up)
	// ... which isn't a right-hand coordinate system.

	float pos[3];
	ok = ok && peekProc((BYTE *) p_playerBase + 0x838, pos, sizeof(float)*3);
	if (! ok) {
		if (g_playerGUID == 0xffffffffffffffff) {
			return false;
		} else if (g_playerGUID == 0) {
			return true;
		} else {
			/* FIXME need a better way to mark PlayerBase invalid */
			g_playerGUID=0;
			return true; /* we got a good reference for an avatar, but no good position */
		}
	}

	/* convert wow -> right hand coordinate system */
	avatar_pos[0] = -pos[1];
	avatar_pos[1] = pos[2];
	avatar_pos[2] = pos[0];

	float heading=0.0;
	ok = ok && peekProc((BYTE *) p_playerBase + 0x848, &heading, sizeof(heading));
	if (! ok)
		return false;

	float pitch=0.0;
	ok = ok && peekProc((BYTE *) p_playerBase + 0x84C, &pitch, sizeof(pitch));
	if (! ok)
		return false;

	/* TODO use yaw (heading) and pitch angles */
	/* FIXME sin/cos (heading) is right from the numbers, but (-heading) is right from the sound position */
	avatar_front[0]=-sin(heading);
	avatar_front[1]=0.0;
	avatar_front[2]=cos(heading);

	// Dummy top vector, can't tilt your head sideways in wow.
	avatar_top[0]= 0.0;
	avatar_top[1]= 1.0;
	avatar_top[2]= 0.0;

	getCamera(camera_pos, camera_front, camera_top);

	if (! ok)
		return false;

	//printf("P %f %f %f -- %f %f %f \n", avatar_pos[0], avatar_pos[1], avatar_pos[2], avatar_front[0], avatar_front[1], avatar_front[2]);
	//printf("C %f %f %f -- %f %f %f \n", camera_pos[0], camera_pos[1], camera_pos[2], camera_front[0], camera_front[1], camera_front[2]);

	// is it a unit length vector?
	if (fabs((avatar_front[0]*avatar_front[0]+avatar_front[1]*avatar_front[1]+avatar_front[2]*avatar_front[2])-1.0)>0.5) {
		//		printf("F %f %f %f\n", front[0], front[1], front[2]);
		return false;
	}

	// are we around 0/0/0
	if ((fabs(avatar_pos[0])<0.1) && (fabs(avatar_pos[1])<0.1) && (fabs(avatar_pos[2])<0.1)) {
		//		printf("P %f %f %f\n", avatar_pos[0], avatar_pos[1], avatar_pos[2]);
		return false;
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	if (! initialize(pids, L"Wow.exe"))
		return false;

	p_playerBase=getPlayerBase();
	if (p_playerBase != 0) {
		float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
		std::string context;
		std::wstring identity;
		if (fetch(apos, afront, atop, cpos, cfront, ctop, context, identity))
			return true;
	}

	generic_unlock();
	return false;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports World of Warcraft 5.4.7 (18291), with identity support.");
}

static std::wstring description(L"World of Warcraft 5.4.7 (18291)");

static std::wstring shortname(L"World of Warcraft");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin wowplug = {
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

static MumblePlugin2 wowplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &wowplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &wowplug2;
}
