// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "BonjourClient.h"

BonjourClient::BonjourClient() {
#ifdef Q_OS_WIN
	HMODULE hLib = LoadLibrary(L"DNSSD.DLL");
	if (hLib == NULL) {
		qWarning("Bonjour: Failed to load dnssd.dll");
		return;
	}
	FreeLibrary(hLib);
#endif
	bsbBrowser.reset(new BonjourServiceBrowser(this));
	bsbBrowser->browseForServiceType(QLatin1String("_mumble._tcp"));
	bsrResolver.reset(new BonjourServiceResolver(this));
	return;
}
