/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Benjamin Jemlich <pcgod@users.sourceforge.net>

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

#include <shobjidl.h>
#include <propkey.h>
#include <propvarutil.h>

#include "TaskList.h"

void TaskList::addToRecentList(QString name, QString host, int port) {
	OSVERSIONINFOEXW ovi;
	memset(&ovi, 0, sizeof(ovi));

	ovi.dwOSVersionInfoSize = sizeof(ovi);
	GetVersionEx(reinterpret_cast<OSVERSIONINFOW *>(&ovi));
	// Tasklist is Windows 7 only
	if ((ovi.dwMajorVersion < 6) || (ovi.dwBuildNumber < 7600))
		return;

	HRESULT hr;
	IShellLink *link;

	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, __uuidof(IShellLink), reinterpret_cast<void **>(&link));
	if (!link || FAILED(hr))
		return;

	int len;
	STACKVAR(wchar_t, appPath, QCoreApplication::applicationFilePath().length() + 1);
	len = QCoreApplication::applicationFilePath().toWCharArray(appPath);
	appPath[len] = 0;

	link->SetPath(appPath);

	QUrl url;
	url.setScheme(QLatin1String("mumble"));
	url.setHost(host);
	url.setPort(port);
	url.addQueryItem(QLatin1String("title"), name);
	url.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));

	QString qs = QLatin1String(url.toEncoded());

	STACKVAR(wchar_t, urlArg, qs.length() + 1);
	len = qs.toWCharArray(urlArg);
	urlArg[len] = 0;

	link->SetArguments(urlArg);

	IPropertyStore *ps;
	hr = link->QueryInterface(__uuidof(IPropertyStore), reinterpret_cast<void **>(&ps));
	if (FAILED(hr))
		return;

	STACKVAR(wchar_t, wcName, host.length() + 1);
	len = name.toWCharArray(wcName);
	wcName[len] = 0;

	PROPVARIANT pt;
	InitPropVariantFromString(wcName, &pt);

	hr = ps->SetValue(PKEY_Title, pt);
	if (FAILED(hr))
		return;

	hr = ps->Commit();
	if (FAILED(hr))
		return;

	SHAddToRecentDocs(SHARD_LINK, link);

	PropVariantClear(&pt);
	ps->Release();
	link->Release();
}
