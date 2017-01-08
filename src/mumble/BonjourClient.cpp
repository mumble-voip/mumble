// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "BonjourClient.h"

#include "BonjourServiceBrowser.h"
#include "BonjourServiceResolver.h"

BonjourClient::BonjourClient() {
	bsrResolver = NULL;
	bsbBrowser = NULL;
#ifdef Q_OS_WIN
	HMODULE hLib = LoadLibrary(L"DNSSD.DLL");
	if (hLib == NULL) {
		qWarning("Bonjour: Failed to load dnssd.dll");
		return;
	}
	FreeLibrary(hLib);
#endif
	bsbBrowser = new BonjourServiceBrowser(this);
	bsbBrowser->browseForServiceType(QLatin1String("_mumble._tcp"));
	bsrResolver = new BonjourServiceResolver(this);
	return;
}

BonjourClient::~BonjourClient() {
	delete bsbBrowser;
	delete bsrResolver;
}
