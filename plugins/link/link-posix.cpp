/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#define __cdecl
typedef unsigned long HWND;

#include "../mumble_plugin.h"

#include <stdio.h>
#include <stdint.h>
#include <wchar.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

static std::wstring wsPluginName;
static std::wstring wsDescription;
static char memname[256];

struct LinkedMem {
	uint32_t uiVersion;
	uint32_t ui32count;
	float	fAvatarPosition[3];
	float	fAvatarFront[3];
	float	fAvatarTop[3];
	wchar_t	name[256];
	float	fCameraPosition[3];
	float	fCameraFront[3];
	float	fCameraTop[3];
	wchar_t	identity[256];
	uint32_t context_len;
	unsigned char context[256];
	wchar_t description[2048];
};

static int32_t GetTickCount() {
	struct timeval tv;
	gettimeofday(&tv,NULL);

	return tv.tv_usec / 1000 + tv.tv_sec * 1000;
}

static struct LinkedMem * const lm_invalid = reinterpret_cast<struct LinkedMem *>(-1);
static struct LinkedMem *lm = lm_invalid;
static int shmfd = -1;

static uint32_t last_tick = 0;
static uint32_t last_count = 0;

static void unlock() {
	lm->ui32count = last_count = 0;
	lm->uiVersion = 0;
	lm->name[0] = 0;
	wsPluginName.assign(L"Link");
	wsDescription.clear();
}

static int trylock() {
	if (lm == lm_invalid)
		return false;

	if ((lm->uiVersion == 1) || (lm->uiVersion == 2)) {
		if (lm->ui32count != last_count) {
			last_tick = GetTickCount();
			last_count = lm->ui32count;

			wchar_t buff[2048];

			if (lm->name[0]) {
				wcsncpy(buff, lm->name, 256);
				buff[255] = 0;
				wsPluginName.assign(buff);
			}
			if (lm->description[0]) {
				wcsncpy(buff, lm->description, 2048);
				buff[2047] = 0;
				wsDescription.assign(buff);
			}
			return true;
		}
	}
	return false;
}

static const std::wstring longdesc() {
	return wsDescription;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top,
                 float *camera_pos, float *camera_front, float *camera_top,
                 std::string &context, std::wstring &identity) {

	if (lm->ui32count != last_count) {
		last_tick = GetTickCount();
		last_count = lm->ui32count;
	} else if ((GetTickCount() - last_tick) > 5000)
		return false;

	if ((lm->uiVersion != 1) && (lm->uiVersion != 2))
		return false;

	for (int i=0;i<3;++i) {
		avatar_pos[i]=lm->fAvatarPosition[i];
		avatar_front[i]=lm->fAvatarFront[i];
		avatar_top[i]=lm->fAvatarTop[i];
	}

	if (lm->uiVersion == 2) {
		for (int i=0;i<3;++i) {
			camera_pos[i]=lm->fCameraPosition[i];
			camera_front[i]=lm->fCameraFront[i];
			camera_top[i]=lm->fCameraTop[i];
		}

		if (lm->context_len > 255)
			lm->context_len = 255;
		lm->identity[255] = 0;

		context.assign(reinterpret_cast<const char *>(lm->context), lm->context_len);
		identity.assign(lm->identity);
	} else {
		for (int i=0;i<3;++i) {
			camera_pos[i]=lm->fAvatarPosition[i];
			camera_front[i]=lm->fAvatarFront[i];
			camera_top[i]=lm->fAvatarTop[i];
		}
		context.clear();
		identity.clear();
	}

	return true;
}

__attribute__((constructor))
static void load_plugin() {
	bool bCreated = false;

	snprintf(memname, 256, "/MumbleLink.%d", getuid());

	shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);
	if (shmfd < 0) {
		bCreated = true;
		shmfd = shm_open(memname, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	}

	if (shmfd < 0) {
		fprintf(stderr,"Mumble Link plugin: error creating shared memory\n");
		return;
	}

	if (bCreated)
		ftruncate(shmfd, sizeof(struct LinkedMem));

	lm = static_cast<struct LinkedMem*>(
	         mmap(NULL, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0));

	if ((lm != lm_invalid) && bCreated)
		memset(lm, 0, sizeof(struct LinkedMem));
}

__attribute__((destructor))
static void unload_plugin() {
	if (lm != lm_invalid)
		munmap(lm, sizeof(struct LinkedMem));

	if (shmfd > -1)
		close(shmfd);

	shm_unlink(memname);
}

static std::wstring description(L"Link v1.2.0");

static MumblePlugin linkplug = {
	MUMBLE_PLUGIN_MAGIC,
	description,
	wsPluginName,
	NULL,
	NULL,
	trylock,
	unlock,
	longdesc,
	fetch
};

extern "C" __attribute__((visibility("default"))) MumblePlugin *getMumblePlugin() {
	return &linkplug;
}
