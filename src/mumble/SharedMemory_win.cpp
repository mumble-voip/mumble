// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SharedMemory.h"

#include "win.h"

#include <QtCore/QDebug>

struct SharedMemory2Private {
	HANDLE hMemory;
};

SharedMemory2::SharedMemory2(QObject *p, unsigned int minsize, const QString &memname) : QObject(p) {
	a_ucData = nullptr;

	d          = new SharedMemory2Private();
	d->hMemory = nullptr;

	if (memname.isEmpty()) {
		// Create a new segment

		for (int i = 0; i < 100; ++i) {
			qsName     = QString::fromLatin1("Local\\MumbleOverlayMemory%1").arg(++uiIndex);
			d->hMemory = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, minsize,
											qsName.toStdWString().c_str());
			if (d->hMemory && GetLastError() != ERROR_ALREADY_EXISTS) {
				break;
			}

			if (d->hMemory)
				CloseHandle(d->hMemory);
			d->hMemory = nullptr;
		}
	} else {
		// Open existing segment

		qsName     = memname;
		d->hMemory = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, minsize,
										qsName.toStdWString().c_str());
		qWarning("%p %lx", d->hMemory, GetLastError());
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			qWarning() << "SharedMemory2: Memory doesn't exist" << qsName;
			if (d->hMemory) {
				CloseHandle(d->hMemory);
				d->hMemory = nullptr;
			}
		}
	}

	if (!d->hMemory) {
		qWarning() << "SharedMemory2: CreateFileMapping failed for" << qsName;
	} else {
		a_ucData = reinterpret_cast< unsigned char * >(MapViewOfFile(d->hMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));

		if (!a_ucData) {
			qWarning() << "SharedMemory2: Failed to map memory" << qsName;
		} else {
			MEMORY_BASIC_INFORMATION mbi;
			memset(&mbi, 0, sizeof(mbi));
			if ((VirtualQuery(a_ucData, &mbi, sizeof(mbi)) == 0) || (mbi.RegionSize < minsize)) {
				qWarning() << "SharedMemory2: Memory too small" << qsName << mbi.RegionSize;
			} else {
				uiSize = mbi.RegionSize;
				return;
			}
		}
	}

	if (a_ucData) {
		UnmapViewOfFile(a_ucData);
		a_ucData = nullptr;
	}
	if (d->hMemory) {
		CloseHandle(d->hMemory);
		d->hMemory = nullptr;
	}
}

SharedMemory2::~SharedMemory2() {
	if (a_ucData)
		UnmapViewOfFile(a_ucData);
	if (d->hMemory)
		CloseHandle(d->hMemory);

	delete d;
}

void SharedMemory2::systemRelease() {
	// This doesn't really do anything on Win32, since it has delete-on-close semantics anyway
	if (d->hMemory) {
		CloseHandle(d->hMemory);
		d->hMemory = nullptr;
	}
}
