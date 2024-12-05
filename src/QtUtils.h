// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_QTUTILS_H_
#define MUMBLE_QTUTILS_H_

#include <QCryptographicHash>
#include <QString>
#include <QStringList>

#include <filesystem>

#include <memory>

class QObject;

namespace Mumble {
namespace QtUtils {

	/**
	 * A deleter function to be used for QObjects that must not be deleted using
	 * delete directly but rather by calling deleteLater() on them (and thus letting
	 * Qt perform the actual deletion).
	 *
	 * This function is intended to be used in smart-pointers holding QObjects.
	 */
	void deleteQObject(QObject *object);

	QString decode_utf8_qssl_string(const QString &input);

	/**
	 * Applies decode_utf8_qssl_string on the first element in the
	 * given list. If the list is empty an empty String is returned.
	 */
	QString decode_first_utf8_qssl_string(const QStringList &list);

	/**
	 * Creates a platform agnostic path from a QString
	 */
	std::filesystem::path qstring_to_path(const QString &input);

} // namespace QtUtils
} // namespace Mumble

template< typename T > using qt_unique_ptr = std::unique_ptr< T, decltype(&Mumble::QtUtils::deleteQObject) >;

/// Creates a new unique_ptr with custom deleter for any given QObject*
template< typename T, typename... Args > qt_unique_ptr< T > make_qt_unique(Args &&... args) {
	static_assert(std::is_base_of< QObject, T >::value, "");

	return qt_unique_ptr< T >{ new T(std::forward< Args >(args)...), Mumble::QtUtils::deleteQObject };
}

// For backwards compatibility we have to keep these functions in the global namespace
inline QString u8(const ::std::string &str) {
	return QString::fromUtf8(str.data(), static_cast< int >(str.length()));
}

inline QString u8(const ::std::wstring &str) {
	return QString::fromStdWString(str);
}

inline ::std::string u8(const QString &str) {
	const QByteArray &qba = str.toUtf8();
	return ::std::string(qba.constData(), static_cast< std::size_t >(qba.length()));
}

inline QByteArray blob(const ::std::string &str) {
	return QByteArray(str.data(), static_cast< int >(str.length()));
}

inline ::std::string blob(const QByteArray &str) {
	return ::std::string(str.constData(), static_cast< std::size_t >(str.length()));
}

inline QByteArray sha1(const QByteArray &blob) {
	return QCryptographicHash::hash(blob, QCryptographicHash::Sha1);
}

inline QByteArray sha1(const QString &str) {
	return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha1);
}

#endif // MUMBLE_QTUTILS_H_
