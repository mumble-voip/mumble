// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_QTUTILS_H_
#define MUMBLE_QTUTILS_H_

#include <QString>

class QObject;
class QStringList;

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

}; // namespace QtUtils
}; // namespace Mumble

#endif // MUMBLE_QTUTILS_H_
