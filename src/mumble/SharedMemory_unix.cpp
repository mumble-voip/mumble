// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SharedMemory.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <QDebug>

struct SharedMemory2Private {
	int iShmemFD;
};

SharedMemory2::SharedMemory2(QObject *p, unsigned int minsize, const QString &memname) : QObject(p) {
	a_ucData = nullptr;

	d           = new SharedMemory2Private();
	d->iShmemFD = -1;

	int prot = PROT_READ;

	if (memname.isEmpty()) {
		prot |= PROT_WRITE;
		for (int i = 0; i < 100; ++i) {
			qsName      = QString::fromLatin1("/MumbleOverlayMemory%1").arg(++uiIndex);
			d->iShmemFD = shm_open(qsName.toUtf8().constData(), O_RDWR | O_CREAT | O_EXCL, 0600);
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
		qsName      = memname;
		d->iShmemFD = shm_open(qsName.toUtf8().constData(), O_RDONLY, 0600);
	}
	if (d->iShmemFD == -1) {
		qWarning() << "SharedMemory2: Failed to open shared memory segment" << qsName;
		return;
	}
	struct stat buf;
	fstat(d->iShmemFD, &buf);
	unsigned int memsize = static_cast< unsigned int >(buf.st_size);
	if (memsize < minsize) {
		qWarning() << "SharedMemory2: Segment too small" << memsize << minsize;
	} else if (memsize > std::numeric_limits< unsigned int >::max()) {
		qWarning() << "SharedMemory2: Segment too big" << memsize;
	} else {
		a_ucData = reinterpret_cast< unsigned char * >(mmap(nullptr, minsize, prot, MAP_SHARED, d->iShmemFD, 0));
		if (a_ucData != reinterpret_cast< unsigned char * >(-1)) {
			uiSize = memsize;
			return;
		}
		qWarning() << "SharedMemory2: Failed to map shared memory segment" << qsName;
		a_ucData = nullptr;
	}

	close(d->iShmemFD);
	d->iShmemFD = -1;
	shm_unlink(qsName.toUtf8().constData());
}

SharedMemory2::~SharedMemory2() {
	systemRelease();
	if (a_ucData) {
		munmap(a_ucData, uiSize);
		a_ucData = nullptr;
	}
}

void SharedMemory2::systemRelease() {
	if (d->iShmemFD != -1) {
		close(d->iShmemFD);
		d->iShmemFD = -1;
		shm_unlink(qsName.toUtf8().constData());
	}
}
