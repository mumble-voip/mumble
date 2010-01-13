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

extern bool bIsWin7;

void TaskList::addToRecentList(QString name, QString user, QString host, int port) {
	if (! bIsWin7)
		return;

	HRESULT hr;
	IShellLink *link = NULL;
	IPropertyStore *ps = NULL;
	PROPVARIANT pt;

	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, __uuidof(IShellLink), reinterpret_cast<void **>(&link));
	if (!link || FAILED(hr))
		return;

	QUrl url;
	url.setScheme(QLatin1String("mumble"));
	url.setUserName(user);
	url.setHost(host);
	url.setPort(port);
	url.addQueryItem(QLatin1String("title"), name);
	url.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));

	QSettings settings(QLatin1String("HKEY_CLASSES_ROOT"), QSettings::NativeFormat);

	QString app = settings.value(QLatin1String("mumble/DefaultIcon/.")).toString();
	if (app.isEmpty() || ! QFileInfo(app).exists())
		app = QCoreApplication::applicationFilePath();

	link->SetPath(app.utf16());
	link->SetArguments(QString::fromLatin1(url.toEncoded()).utf16());

	hr = link->QueryInterface(__uuidof(IPropertyStore), reinterpret_cast<void **>(&ps));
	if (FAILED(hr)) {
		qFatal("TaskList: Failed to get property store");
		goto cleanup;
	}

	InitPropVariantFromString(name.utf16(), &pt);
	hr = ps->SetValue(PKEY_Title, pt);
	PropVariantClear(&pt);

	if (FAILED(hr)) {
		qFatal("TaskList: Failed to set title");
		goto cleanup;
	}

	hr = ps->Commit();
	if (FAILED(hr)) {
		qFatal("TaskList: Failed commit");
		goto cleanup;
	}

	SHAddToRecentDocs(SHARD_LINK, link);

cleanup:
	if (ps)
		ps->Release();
	if (link)
		link->Release();
}
