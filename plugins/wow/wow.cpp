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
		bool ok = Process32First(hSnap, &pe);

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
	bool ok=ReadProcessMemory(h, base, dest, len, &r);
	return (ok && (r == len));
}

static void about(HWND h) {
	::MessageBox(h, L"Reads audio position information from World of Warcraft", L"Mumble WoW Plugin", MB_OK);
}

static int fetch(float *pos, float *front, float *top) {
	for(int i=0;i<3;i++)
		pos[i]=front[i]=top[i]=0.0;

	bool ok = true;

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
	ok = ok && peekProc((BYTE *) 0x00C705BC, pos, 12);
	ok = ok && peekProc((BYTE *) 0x00C705D4, front, 12);

	if (! ok)
		return false;

	// Dummy top vector, can't tilt your head sideways in wow.
	top[2]=-1.0;

//	printf("P %f %f %f -- %f %f %f \n", pos[0], pos[1], pos[2], front[0], front[1], front[2]);

	if (fabs((front[0]*front[0]+front[1]*front[1]+front[2]*front[2])-1.0)>0.5) {
//		printf("F %f %f %f\n", front[0], front[1], front[2]);
		return false;
	}

	if ((fabs(pos[0])<0.1) && (fabs(pos[1])<0.1) && (fabs(pos[2])<0.1)) {
//		printf("P %f %f %f\n", pos[0], pos[1], pos[2]);
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

	float pos[3], front[3], top[3];
	if (fetch(pos, front, top))
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


static MumblePlugin wowplug = {
	MUMBLE_PLUGIN_MAGIC,
	L"World of Warcraft 1.9.1(Euro)",
	L"World of Warcraft",
	about,
	NULL,
	trylock,
	unlock,
	fetch
};

extern "C" __declspec(dllexport) __cdecl MumblePlugin *getMumblePlugin() {
	return &wowplug;
}
