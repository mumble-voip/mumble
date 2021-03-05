// Copyright 2016-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QObject>

namespace Mumble {
namespace QtUtils {
	void deleteQObject(QObject *object) { object->deleteLater(); }

}; // namespace QtUtils
}; // namespace Mumble
