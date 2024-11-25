// Copyright 2021-2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QObject>
#include <QStringList>
#include <QUrl>

#include <boost/filesystem.hpp>

namespace Mumble {
namespace QtUtils {
	void deleteQObject(QObject *object) { object->deleteLater(); }

	QString decode_utf8_qssl_string(const QString &input) {
		QString i = input;
		return QUrl::fromPercentEncoding(i.replace(QLatin1String("\\x"), QLatin1String("%")).toLatin1());
	}

	QString decode_first_utf8_qssl_string(const QStringList &list) {
		if (list.count() > 0) {
			return decode_utf8_qssl_string(list.at(0));
		}
		return QString();
	}

	boost::filesystem::path qstring_to_path(const QString &input) {
		// Path handling uses wide character encoding on Windows.
		// When converting from QStrings, we need to take that
		// into account, otherwise raw file operations will fail when
		// the path contains Unicode characters.
#ifdef Q_OS_WIN
		return boost::filesystem::path(input.toStdWString());
#else
		return boost::filesystem::path(input.toUtf8());
#endif
	}

} // namespace QtUtils
} // namespace Mumble
