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

#ifndef _WIN32_WINNT
#define  _WIN32_WINNT 0x0501
#include <windows.h>
#endif

#include <QDebug>
#include "SharedMemory.h"

struct SharedMemory2Private {
	HANDLE hMemory;
};

SharedMemory2::SharedMemory2(QObject *p, unsigned int minsize, const QString &memname) : QObject(p) {
	a_ucData = NULL;

	d = new SharedMemory2Private();
	d->hMemory = NULL;

	if (memname.isEmpty()) {
		// Create a new segment

		for (int i=0;i<100;++i) {
			qsName = QString::fromLatin1("Local\\MumbleOverlayMemory%1").arg(++uiIndex);
			d->hMemory = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, minsize, qsName.utf16());
			if (d->hMemory && GetLastError() != ERROR_ALREADY_EXISTS)
				break;

			if (d->hMemory)
				CloseHandle(d->hMemory);
			d->hMemory = NULL;
		}
	} else {
		// Open existing segment

		qsName = memname;
		d->hMemory = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, minsize, qsName.utf16());
		qWarning("%p %lx", d->hMemory, GetLastError());
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			qWarning() << "SharedMemory2: Memory doesn't exist" << qsName;
			if (d->hMemory) {
				CloseHandle(d->hMemory);
				d->hMemory = NULL;
			}
		}
	}

	if (d->hMemory == NULL) {
		qWarning() << "SharedMemory2: CreateFileMapping failed for" << qsName;
	} else {
		a_ucData = reinterpret_cast<unsigned char *>(MapViewOfFile(d->hMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));

		if (a_ucData == NULL) {
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
		a_ucData = NULL;
	}
	if (d->hMemory) {
		CloseHandle(d->hMemory);
		d->hMemory = NULL;
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
		d->hMemory = NULL;
	}
}
