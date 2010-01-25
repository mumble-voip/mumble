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

#include <QDebug>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "SharedMemory.h"

struct SharedMemory2Private {
	int iShmemFD;
};

SharedMemory2::SharedMemory2(QObject *p, unsigned int minsize, const QString &memname) : QObject(p) {
	a_ucData = NULL;

	d = new SharedMemory2Private();
	d->iShmemFD = -1;

	int prot = PROT_READ;

	if (memname.isEmpty()) {
		prot |= PROT_WRITE;
		for (int i=0;i<100;++i) {
			qsName = QString::fromLatin1("/MumbleOverlayMemory%1").arg(++uiIndex);
			d->iShmemFD=shm_open(qsName.toUtf8().constData(), O_RDWR|O_CREAT|O_EXCL, 0600);
			if (d->iShmemFD != -1) {
				if (ftruncate(d->iShmemFD, minsize) == 0) {
					break;
				} else {
					close(d->iShmemFD);
					d->iShmemFD = -1;
					shm_unlink(qsName.toUtf8().constData());
				}
			}
		}
	} else {
		qsName = memname;
		d->iShmemFD=shm_open(qsName.toUtf8().constData(), O_RDONLY, 0600);
	}
	if (d->iShmemFD == -1) {
		qWarning() << "SharedMemory2: Failed to open shared memory segment" << qsName;
		return;
	}
	struct stat buf;
	fstat(d->iShmemFD, &buf);
	off_t memsize = buf.st_size;
	if (memsize < minsize) {
		qWarning() << "SharedMemory2: Segment too small" << memsize << minsize;
	} else {
		a_ucData = reinterpret_cast<unsigned char *>(mmap(NULL, minsize, prot, MAP_SHARED, d->iShmemFD, 0));
		if (a_ucData != reinterpret_cast<unsigned char *>(-1)) {
			uiSize = memsize;
			return;
		}
		qWarning() << "SharedMemory2: Failed to map shared memory segment" << qsName;
		a_ucData = NULL;
	}

	close(d->iShmemFD);
	d->iShmemFD = -1;
	shm_unlink(qsName.toUtf8().constData());

}

SharedMemory2::~SharedMemory2() {
	systemRelease();
	if (a_ucData) {
		munmap(a_ucData, uiSize);
		a_ucData = NULL;
	}
}

void SharedMemory2::systemRelease() {
	if (d->iShmemFD != -1) {
		close(d->iShmemFD);
		d->iShmemFD = -1;
		shm_unlink(qsName.toUtf8().constData());
	}
}
