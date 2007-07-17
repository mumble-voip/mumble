/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#include "Player.h"
#include "Channel.h"
#include "Overlay.h"
#include "Global.h"



class SharedMemoryPrivate {
	public:
		int fd;
		sem_t *sem;
};

SharedMemory::SharedMemory() {
	d = new SharedMemoryPrivate();
	d->fd = -1;
	d->sem = NULL;
	sm = NULL;
}

SharedMemory::~SharedMemory() {
	if (sm)
		munmap(sm, sizeof(SharedMem));

	if (d->sem != 0) {
		sem_close(d->sem);
		sem_unlink("/MumbleOverlaySem");
	}

	if (d->fd >= 0) {
		close(d->fd);
		shm_unlink("/MumbleOverlayMem");
	}
	delete d;
	d = NULL;
}

void SharedMemory::resolve(QLibrary *) {
	d->fd = shm_open("/MumbleOverlayMem", O_RDWR|O_CREAT, 0600);
	if (d->fd >= 0) {
		ftruncate(d->fd, sizeof(SharedMem));
		sm = static_cast<SharedMem *>(mmap(NULL, sizeof(SharedMem), PROT_READ|PROT_WRITE, MAP_SHARED, d->fd, 0));
		if (sm == reinterpret_cast<SharedMem *>(-1)) {
			sm = NULL;
		}
	}  else {
		qWarning("SharedMemory: Failed to open: %s", strerror(errno));
	}
	d->sem = sem_open("/MumbleOverlaySem", O_CREAT, 0600, 1);
	if (d->sem == SEM_FAILED) {
		qWarning("SharedMemory: Failed to create semaphore: %s",strerror(errno));
		d->sem = NULL;
	}
}

bool SharedMemory::tryLock() {
	if (d->sem) {
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
	}
	return false;
}

void SharedMemory::unlock() {
	if (d->sem)
		sem_post(d->sem);
}

class OverlayPrivate {
};

void Overlay::platformInit() {
	d = new OverlayPrivate();
}

void Overlay::setActive(bool) {
	if (! sm.sm)
		return;
}

void Overlay::on_Timer_timeout() {
	if (sm.sm)
		sm.sm->lastAppAlive = time(NULL);
}
