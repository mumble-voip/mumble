// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "EnvUtils.h"

#include <QByteArray>

namespace EnvUtils {

QString getenv(QString name) {
#ifdef Q_OS_WIN
	QByteArray buf;
	size_t requiredSize = 0;

	static_assert(sizeof(wchar_t) == sizeof(ushort), "expected 2-byte wchar_t");

	const wchar_t *wname = reinterpret_cast< const wchar_t * >(name.utf16());

	// Query the required buffer size (in elements).
	if (_wgetenv_s(&requiredSize, 0, 0, wname) != 0) {
		return QString();
	}
	if (requiredSize == 0) {
		return QString();
	}

	// Resize buf to fit the value and put it there.
	buf.resize(static_cast< int >(requiredSize * sizeof(wchar_t)));
	if (_wgetenv_s(&requiredSize, reinterpret_cast< wchar_t * >(buf.data()), requiredSize, wname) != 0) {
		return QString();
	}

	// Convert the value to QString and return it.
	const wchar_t *wbuf = reinterpret_cast< const wchar_t * >(buf.constData());
	return QString::fromWCharArray(wbuf);
#else
	QByteArray name8bit = name.toLocal8Bit();
	char *val           = ::getenv(name8bit.constData());
	if (!val) {
		return QString();
	}
	return QString::fromLocal8Bit(val);
#endif
}

bool setenv(QString name, QString value) {
#ifdef Q_OS_WIN
	return _wputenv_s(reinterpret_cast< const wchar_t * >(name.utf16()),
					  reinterpret_cast< const wchar_t * >(value.utf16()))
		   == 0;
#else
	const int OVERWRITE = 1;
	return ::setenv(name.toLocal8Bit().constData(), value.toLocal8Bit().constData(), OVERWRITE) == 0;
#endif
}

bool waylandIsUsed() {
	// If wayland is used, this environment variable is expected to be set
	return getenv(QStringLiteral("WAYLAND_DISPLAY")) != "";
}

} // namespace EnvUtils
