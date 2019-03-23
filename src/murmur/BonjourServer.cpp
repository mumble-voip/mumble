// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "BonjourServer.h"

#include "BonjourServiceRegister.h"

BonjourServer::BonjourServer() {
	bsrRegister = NULL;
#ifdef Q_OS_WIN
	static bool bDelayLoadFailed = false;
	if (bDelayLoadFailed) return;

	HMODULE hLib = LoadLibrary(L"DNSSD.DLL");
	if (hLib == NULL) {
		bDelayLoadFailed = true;
		qWarning("Bonjour: Failed to load dnssd.dll");
		return;
	}
	FreeLibrary(hLib);
#endif

	bsrRegister = new BonjourServiceRegister(this);
}

BonjourServer::~BonjourServer() {
	delete bsrRegister;
}
