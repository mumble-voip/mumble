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
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "User.h"
#include "Channel.h"
#include "Overlay.h"
#include "Global.h"

#if MAC_OS_X_VERSION_MAX_ALLOWED < 1050
#undef SEM_FAILED
#define SEM_FAILED ((sem_t *)-1)
#endif

class SharedMemoryPrivate {
	public:
		char semname[256];
		char memname[256];
		int fd;
		sem_t *sem;
};

SharedMemory::SharedMemory() {
	d = new SharedMemoryPrivate();
	d->fd = -1;
	d->sem = NULL;
	sm = NULL;
	snprintf(d->semname, 256, "/MumbleOverlaySem.%d", getuid());
	snprintf(d->memname, 256, "/MumbleOverlayMem.%d", getuid());
}

SharedMemory::~SharedMemory() {
	if (sm)
		munmap(sm, sizeof(SharedMem));

	if (d->sem != 0) {
		sem_close(d->sem);
		sem_unlink(d->semname);
	}

	if (d->fd >= 0) {
		close(d->fd);
		shm_unlink(d->memname);
	}
	delete d;
	d = NULL;
}

void SharedMemory::resolve(QLibrary *) {
	d->fd = shm_open(d->memname, O_RDWR|O_CREAT, 0600);
	if (d->fd >= 0) {
		ftruncate(d->fd, sizeof(SharedMem));
		sm = static_cast<SharedMem *>(mmap(NULL, sizeof(SharedMem), PROT_READ|PROT_WRITE, MAP_SHARED, d->fd, 0));
		if (sm == reinterpret_cast<SharedMem *>(-1)) {
			sm = NULL;
		}
	}  else {
		qWarning("SharedMemory: Failed to open: %s", strerror(errno));
	}
	d->sem = sem_open(d->semname, O_CREAT, 0600, 1);
	if (d->sem == SEM_FAILED) {
		qWarning("SharedMemory: Failed to create semaphore: %s",strerror(errno));
		d->sem = NULL;
	}
}

bool SharedMemory::tryLock() {
	if (d->sem) {
#ifdef Q_OS_MACX
#define SLEEP_NSEC 10000000
#define SLICE_NSEC (SLEEP_NSEC/5)
		int ret;
		long remain = SLEEP_NSEC;
		struct timespec r, s = { 0, SLICE_NSEC };
		while (remain > 0) {
			if ((ret = sem_trywait(d->sem)) == 0)
				break;
			nanosleep(&s, &r);
			remain -= (SLICE_NSEC - r.tv_nsec);
		}
		return (ret == 0);
#else
		struct timeval tv;
		gettimeofday(&tv, NULL);
		tv.tv_usec += 50000;
		if (tv.tv_usec >= 1000000) {
			tv.tv_usec -= 1000000;
			tv.tv_sec++;
		}
		struct timespec ts;
		ts.tv_sec = tv.tv_sec;
		ts.tv_nsec = tv.tv_usec * 1000;
		return (sem_timedwait(d->sem, &ts) == 0);
#endif
	}
	return false;
}

void SharedMemory::unlock() {
	if (d->sem)
		sem_post(d->sem);
}

void Overlay::platformInit() {
	d = NULL;
}

void Overlay::setActive(bool) {
	if (! sm.sm)
		return;
}

void Overlay::on_Timer_timeout() {
	if (sm.sm)
		sm.sm->lastAppAlive = static_cast<int>(time(NULL));
}
