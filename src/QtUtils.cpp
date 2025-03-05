// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "QtUtils.h"

#include <QObject>
#include <QStringList>
#include <QUrl>

#include <filesystem>

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

	CaseInsensitiveQString::CaseInsensitiveQString(const QString &str) : m_str(str) {}

	CaseInsensitiveQString::CaseInsensitiveQString(QString &&str) : m_str(std::move(str)) {}

	CaseInsensitiveQString &CaseInsensitiveQString::operator=(const QString &str) {
		m_str = str;

		return *this;
	}

	CaseInsensitiveQString &CaseInsensitiveQString::operator=(QString &&str) {
		m_str = std::move(str);

		return *this;
	}

	CaseInsensitiveQString::operator const QString &() const { return m_str; }

	CaseInsensitiveQString::operator QString &() { return m_str; }

	bool operator==(const QString &lhs, const CaseInsensitiveQString &rhs) {
		return lhs.compare(rhs.m_str, Qt::CaseInsensitive) == 0;
	}

	bool operator==(const CaseInsensitiveQString &lhs, const CaseInsensitiveQString &rhs) { return lhs.m_str == rhs; }

	bool operator==(const CaseInsensitiveQString &lhs, const QString &rhs) { return rhs == lhs; }

	bool operator!=(const QString &lhs, const CaseInsensitiveQString &rhs) { return !(lhs == rhs); }

	bool operator!=(const CaseInsensitiveQString &lhs, const CaseInsensitiveQString &rhs) { return !(lhs == rhs); }

	bool operator!=(const CaseInsensitiveQString &lhs, const QString &rhs) { return !(lhs == rhs); }

	bool operator<(const QString &lhs, const CaseInsensitiveQString &rhs) {
		return lhs.compare(rhs.m_str, Qt::CaseInsensitive) < 0;
	}

	bool operator<(const CaseInsensitiveQString &lhs, const CaseInsensitiveQString &rhs) { return lhs.m_str < rhs; }

	bool operator<(const CaseInsensitiveQString &lhs, const QString &rhs) { return rhs >= lhs; }

	bool operator<=(const QString &lhs, const CaseInsensitiveQString &rhs) {
		return lhs.compare(rhs.m_str, Qt::CaseInsensitive) <= 0;
	}

	bool operator<=(const CaseInsensitiveQString &lhs, const CaseInsensitiveQString &rhs) { return lhs.m_str <= rhs; }

	bool operator<=(const CaseInsensitiveQString &lhs, const QString &rhs) { return rhs >= lhs; }

	bool operator>(const QString &lhs, const CaseInsensitiveQString &rhs) {
		return lhs.compare(rhs.m_str, Qt::CaseInsensitive) > 0;
	}

	bool operator>(const CaseInsensitiveQString &lhs, const CaseInsensitiveQString &rhs) { return lhs.m_str > rhs; }

	bool operator>(const CaseInsensitiveQString &lhs, const QString &rhs) { return rhs <= lhs; }

	bool operator>=(const QString &lhs, const CaseInsensitiveQString &rhs) {
		return lhs.compare(rhs.m_str, Qt::CaseInsensitive) >= 0;
	}

	bool operator>=(const CaseInsensitiveQString &lhs, const CaseInsensitiveQString &rhs) { return lhs.m_str >= rhs; }

	bool operator>=(const CaseInsensitiveQString &lhs, const QString &rhs) { return rhs <= lhs; }

	std::filesystem::path qstring_to_path(const QString &input) {
		// Path handling uses wide character encoding on Windows.
		// When converting from QStrings, we need to take that
		// into account, otherwise raw file operations will fail when
		// the path contains Unicode characters.
#ifdef Q_OS_WIN
		return std::filesystem::path(input.toStdWString());
#else
		return std::filesystem::path(input.toUtf8().data());
#endif
	}

} // namespace QtUtils
} // namespace Mumble
