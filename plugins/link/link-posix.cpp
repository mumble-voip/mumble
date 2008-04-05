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

static wchar_t wcPluginName[256];

struct LinkedMem {
	uint32_t uiVersion;
	uint32_t ui32Tick;
	float	fPosition[3];
	float	fFront[3];
	float	fTop[3];
	wchar_t	name[256];
};

static const int32_t GetTickCount() {
	struct timeval tv;
	gettimeofday(&tv,NULL);

	return tv.tv_usec / 1000 + tv.tv_sec * 1000;
}

static struct LinkedMem * const lm_invalid = reinterpret_cast<struct LinkedMem *>(-1);
static struct LinkedMem *lm = lm_invalid;
static int shmfd = -1;

static void unlock() {
	lm->ui32Tick = 0;
	lm->uiVersion = 0;
	wcsncpy(wcPluginName, L"Link", 256);
}

static int trylock() {
	if (lm == lm_invalid)
		return false;

	if (lm->uiVersion == 1) {
		if ((GetTickCount() - lm->ui32Tick) < 5000) {
			if (lm->name[0]) {
				wcsncpy(wcPluginName, lm->name, 256);
			}
			return true;
		}
	}
	return false;
}


static int fetch(float *pos, float *front, float *top) {
	if ((GetTickCount() - lm->ui32Tick) > 5000)
		return false;

	for (int i=0;i<3;i++)
		pos[i]=lm->fPosition[i];
	for (int i=0;i<3;i++)
		front[i]=lm->fFront[i];
	for (int i=0;i<3;i++)
		top[i]=lm->fTop[i];

	return true;
}

__attribute__((constructor))
static void load_plugin() {
  bool bCreated = false;

  shmfd = shm_open("/MumbleLink", O_RDWR, S_IRUSR | S_IWUSR);
  if (shmfd < 0) {
  	bCreated = true;
        shmfd = shm_open("/MumbleLink", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  }

  if(shmfd < 0) {
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
    if(lm != lm_invalid)
      munmap(lm, sizeof(struct LinkedMem));

    if(shmfd > -1)
      close(shmfd);

    shm_unlink("/MumbleLink");
}

static MumblePlugin linkplug = {
	MUMBLE_PLUGIN_MAGIC,
	L"Link v1.0.1",
	wcPluginName,
	NULL,
	NULL,
	trylock,
	unlock,
	fetch
};

extern "C" __attribute__((visibility("default"))) MumblePlugin *getMumblePlugin() {
	return &linkplug;
}
