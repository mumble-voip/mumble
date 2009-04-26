#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include <math.h>

#include "../mumble_plugin.h"

HANDLE h;

static DWORD getProcess(const wchar_t *exename) {
	PROCESSENTRY32 pe;
	DWORD pid = 0;

	pe.dwSize = sizeof(pe);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		BOOL ok = Process32First(hSnap, &pe);

		while (ok) {
			if (wcscmp(pe.szExeFile, exename)==0) {
				pid = pe.th32ProcessID;
				break;
			}
			ok = Process32Next(hSnap, &pe);
		}
		CloseHandle(hSnap);
	}
	return pid;
}

static bool peekProc(VOID *base, VOID *dest, SIZE_T len) {
	SIZE_T r;
	BOOL ok=ReadProcessMemory(h, base, dest, len, &r);
	return (ok && (r == len));
}

static void about(HWND h) {
	::MessageBox(h, L"Reads audio position information from World of Warcraft", L"Mumble WoW Plugin", MB_OK);
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=0.0;

	BOOL ok = true;

	// Wow stores as
	// North/South (+ North)
	// East/West (+ West)
	// Up/Down (+Up)
	// ... which isn't a right-hand coordinate system.

	// First found, corpse location ;) 0x00ae28d4
	// Player Pos can be taken from both 0xc5ad34 and
	// 0xc5befc. the first is the "instantanous" position,
	// the last seems to be the "official" one. IE, the
	// first will update Z while jumping, on elevators etc,
	// the latter won't.
	ok = ok && peekProc((BYTE *) 0x00C8DDA4, avatar_pos, 12);
	ok = ok && peekProc((BYTE *) 0x00C8DDBC, avatar_front, 12);

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
        camera_front[i] = avatar_front[i];
        camera_top[i] = avatar_top[i];
    }

	if (! ok)
		return false;

	// Dummy top vector, can't tilt your head sideways in wow.
	avatar_top[2]=-1.0;

//	printf("P %f %f %f -- %f %f %f \n", avatar_pos[0], avatar_pos[1], avatar_pos[2], avatar_front[0], avatar_front[1], avatar_front[2]);

	if (fabs((avatar_front[0]*avatar_front[0]+avatar_front[1]*avatar_front[1]+avatar_front[2]*avatar_front[2])-1.0)>0.5) {
//		printf("F %f %f %f\n", front[0], front[1], front[2]);
		return false;
	}

	if ((fabs(avatar_pos[0])<0.1) && (fabs(avatar_pos[1])<0.1) && (fabs(avatar_pos[2])<0.1)) {
//		printf("P %f %f %f\n", avatar_pos[0], avatar_pos[1], avatar_pos[2]);
		return false;
	}

	return true;
}

static int trylock() {

	h = NULL;

	DWORD pid=getProcess(L"WoW.exe");
	if (!pid)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
    std::string context;
    std::wstring identity;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, context, identity))
		return true;

	CloseHandle(h);
	h = NULL;
	return false;
}

static void unlock() {
	if (h) {
		CloseHandle(h);
		h = NULL;
	}
	return;
}

static const std::wstring longdesc() {
    return std::wstring(L"Supports World of Warcraft 1.1.0(Euro). No context or identity support.");
}

static std::wstring description(L"World of Warcraft 1.10(Euro)");
static std::wstring shortname(L"World of Warcraft");

static MumblePlugin wowplug = {
	MUMBLE_PLUGIN_MAGIC,
	description,
	shortname,
	about,
	NULL,
	trylock,
	unlock,
	longdesc,
	fetch
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &wowplug;
}
