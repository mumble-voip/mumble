// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_TRANSLATIONS_H_
#define MUMBLE_MUMBLE_TRANSLATIONS_H_

#include <QStringList>

class QApplication;
class QLocale;
class QTranslator;

namespace Mumble {
namespace Translations {

	/**
	 * A lifetime guard for translators. Basically a smart-pointer for an ensemble of translators
	 */
	struct LifetimeGuard {
		QTranslator *m_bundledTranslator   = nullptr;
		QTranslator *m_overwriteTranslator = nullptr;
		QTranslator *m_qtTranslator        = nullptr;

		LifetimeGuard() = default;
		LifetimeGuard(LifetimeGuard &&old);
		LifetimeGuard(const LifetimeGuard &) = delete;
		~LifetimeGuard();
	};

	QStringList getDefaultTranslationDirectories();

	QStringList getTranslationDirectories(const QApplication &app, const QStringList &extraDirectories);

	/**
	 * Installs translators for the given locale to the given application. If extraDirectories are provided,
	 * they will be used for the overwrite translator.
	 *
	 * @returns The lifetime guard of the created translators. This object must not be destroyed before
	 *   the given application has terminated.
	 */
	LifetimeGuard installTranslators(const QLocale &locale, QApplication &app, const QStringList &extraDirectories);

} // namespace Translations
} // namespace Mumble

#endif // MUMBLE_MUMBLE_TRANSLATIONS_H_
