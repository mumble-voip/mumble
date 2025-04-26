// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/**
 * Test-/Exampleprogram which links against the Mumble link plugin to send
 * positional data.
 */

// Allow use of wcsncpy without warnings
#define _CRT_SECURE_NO_WARNINGS

#include <QtCore>

#ifdef Q_OS_WIN
#	include "win.h"
#else
#	include <fcntl.h>
#	include <sys/mman.h>
#endif

struct LinkedMem {
#ifdef WIN32
	UINT32 uiVersion;
	DWORD uiTick;
#else
	uint32_t uiVersion;
	uint32_t uiTick;
#endif
	float fAvatarPosition[3];
	float fAvatarFront[3];
	float fAvatarTop[3];
	wchar_t name[256];
	float fCameraPosition[3];
	float fCameraFront[3];
	float fCameraTop[3];
	wchar_t identity[256];
#ifdef WIN32
	UINT32 context_len;
#else
	uint32_t context_len;
#endif
	unsigned char context[256];
	wchar_t description[2048];
};

LinkedMem *lm = nullptr;


void initMumble() {
#ifdef WIN32
	HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
	if (!hMapObject)
		return;

	lm = (LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));
	if (!lm) {
		CloseHandle(hMapObject);
		hMapObject = nullptr;
		return;
	}
#else
	char memname[256];
	snprintf(memname, 256, "/MumbleLink.%d", getuid());

	int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

	if (shmfd < 0) {
		return;
	}

	lm = (LinkedMem *) (mmap(nullptr, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0));

	if (lm == (void *) (-1)) {
		lm = nullptr;
		return;
	}
#endif
}

void updateMumble() {
	if (!lm)
		return;

	if (lm->uiVersion != 2) {
		wcsncpy(lm->name, L"TestLink", 256);
		wcsncpy(lm->description, L"TestLink is a test of the Link plugin.", 2048);
		lm->uiVersion = 2;
	}
	lm->uiTick++;

	// Left handed coordinate system.
	// X positive towards "right".
	// Y positive towards "up".
	// Z positive towards "into screen".
	//
	// 1 unit = 1 meter

	// Unit vector pointing out of the avatars eyes (here Front looks into scene).
	lm->fAvatarFront[0] = 0.0f;
	lm->fAvatarFront[1] = 0.0f;
	lm->fAvatarFront[2] = 1.0f;

	// Unit vector pointing out of the top of the avatars head (here Top looks straight up).
	lm->fAvatarTop[0] = 0.0f;
	lm->fAvatarTop[1] = 1.0f;
	lm->fAvatarTop[2] = 0.0f;

	// Position of the avatar (here standing slightly off the origin)
	lm->fAvatarPosition[0] += 0.001f;
	lm->fAvatarPosition[1] = 0.0f;
	lm->fAvatarPosition[2] = 0.5f;

	// Same as avatar but for the camera.
	lm->fCameraPosition[0] = 0.0f;
	lm->fCameraPosition[1] = 0.0f;
	lm->fCameraPosition[2] = 0.0f;

	lm->fCameraFront[0] = 0.0f;
	lm->fCameraFront[1] = 0.0f;
	lm->fCameraFront[2] = 1.0f;

	lm->fCameraTop[0] = 0.0f;
	lm->fCameraTop[1] = 1.0f;
	lm->fCameraTop[2] = 0.0f;

	// Identifier which uniquely identifies a certain player in a context (e.g. the ingame Name).
	wcsncpy(lm->identity, L"Unique ID", 256);
	// Context should be equal for players which should be able to hear each other positional and
	// differ for those who shouldn't (e.g. it could contain the server+port and team)
	memcpy(lm->context, "ContextBlob\x00\x01\x02\x03\x04", 16);
	lm->context_len = 16;
}

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);

	initMumble();
	if (!lm)
		qFatal("No Link!");

	lm->fAvatarPosition[0];

	while (true) {
		qWarning("Tick!");
		updateMumble();
#ifdef WIN32
		Sleep(100);
#else
		usleep(100 * 1000);
#endif
	}
}

#undef _CRT_SECURE_NO_WARNINGS
