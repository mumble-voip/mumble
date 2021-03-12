// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_TRANSLATIONS_H_
#define MUMBLE_MUMBLE_TRANSLATIONS_H_

#include <QStringList>

class QApplication;
class QLocale;

namespace Mumble {
namespace Translations {

	QStringList getDefaultTranslationDirectories();

	QStringList getTranslationDirectories(const QApplication &app, const QStringList &extraDirectories);

	void installTranslators(const QLocale &locale, QApplication &app, const QStringList &extraDirectories);

}; // namespace Translations
}; // namespace Mumble

#endif // MUMBLE_MUMBLE_TRANSLATIONS_H_
