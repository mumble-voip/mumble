// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "EnvUtils.h"

#include <QByteArray>

QString EnvUtils::getenv(QString name) {
#ifdef Q_OS_WIN
	QByteArray buf;
	size_t requiredSize = 0;

	static_assert(sizeof(wchar_t) == sizeof(ushort), "expected 2-byte wchar_t");

	const wchar_t *wname = reinterpret_cast<const wchar_t *>(name.utf16());

	// Query the required buffer size (in elements).
	_wgetenv_s(&requiredSize, 0, 0, wname);
	if (requiredSize == 0) {
		return QString();
	}

	// Resize buf to fit the value and put it there.
	buf.resize(static_cast<int>(requiredSize * sizeof(wchar_t)));
	_wgetenv_s(&requiredSize, reinterpret_cast<wchar_t *>(buf.data()), requiredSize, wname);

	// Convert the value to QString and return it.
	const wchar_t *wbuf = reinterpret_cast<const wchar_t *>(buf.constData());
	return QString::fromWCharArray(wbuf);
#else
	QByteArray nameU8 = name.toUtf8();
	char *val = ::getenv(nameU8.constData());
	if (val == NULL) {
		return QString();
	}
	return QString::fromUtf8(val);
#endif
}
