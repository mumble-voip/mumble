// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Translations.h"

#include <QApplication>
#include <QtCore/QDir>
#include <QtCore/QLibraryInfo>
#include <QtCore/QStandardPaths>
#include <QtCore/QTranslator>

namespace Mumble {
namespace Translations {

	LifetimeGuard::LifetimeGuard(LifetimeGuard &&old)
		: m_bundledTranslator(old.m_bundledTranslator), m_overwriteTranslator(old.m_overwriteTranslator),
		  m_qtTranslator(old.m_qtTranslator) {
		// Reset values of old
		old.m_bundledTranslator   = nullptr;
		old.m_overwriteTranslator = nullptr;
		old.m_qtTranslator        = nullptr;
	}

	LifetimeGuard::~LifetimeGuard() {
		delete m_bundledTranslator;
		delete m_overwriteTranslator;
		delete m_qtTranslator;
	}

	QStringList getDefaultTranslationDirectories() {
		QStringList translationDirs;

		const QString translationDirName = "MumbleTranslations";

		// AppData directories
		for (const QString &currentConfigPath : QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)) {
			translationDirs.append(QDir::cleanPath(currentConfigPath));
		}
		// Directories in the config locations
		for (const QString &currentConfigPath : QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)) {
			translationDirs.append(QDir::cleanPath(currentConfigPath + QDir::separator() + translationDirName));
		}
		// A directory in the home directory
		for (const QString &currentHomePath : QStandardPaths::standardLocations(QStandardPaths::HomeLocation)) {
			translationDirs.append(QDir::cleanPath(currentHomePath + QDir::separator() + translationDirName));
		}

		return translationDirs;
	}

	QStringList getTranslationDirectories(const QApplication &app, const QStringList &extraDirectories) {
		QStringList directories;

		// The extra directories have highest priority
		directories += extraDirectories;

		// Next up is the application's root path
		directories.append(app.applicationDirPath());

		// And finally the default locations
		directories += getDefaultTranslationDirectories();

		return directories;
	}

	LifetimeGuard installTranslators(const QLocale &locale, QApplication &app, const QStringList &extraDirectories) {
		LifetimeGuard guard;

		// First install a translator that uses the bundled translations
		guard.m_bundledTranslator = new QTranslator();
		if (guard.m_bundledTranslator->load(locale, ":mumble_")) {
			app.installTranslator(guard.m_bundledTranslator);
		} else {
			qWarning("Unable to find bundled translations for locale \"%s\"", qUtf8Printable(locale.name()));

			delete guard.m_bundledTranslator;
			guard.m_bundledTranslator = nullptr;
		}

		// Now try to add another translator that can overwrite the bundled translations based
		// on translations found in one of the translation directories.
		// The first matching translation file that is found, will be used (first come, first served)
		guard.m_overwriteTranslator = new QTranslator();

		const QString prefix = "";

		bool overwriteTranslatorFound = false;
		for (const QString &currentDir : getTranslationDirectories(app, extraDirectories)) {
			if (guard.m_overwriteTranslator->load(locale, "mumble_", prefix, currentDir)) {
				app.installTranslator(guard.m_overwriteTranslator);

				qWarning("Using extra translation file for locale \"%s\" from directory \"%s\"",
						 qUtf8Printable(locale.name()), qUtf8Printable(currentDir));

				overwriteTranslatorFound = true;

				break;
			}
		}

		if (!overwriteTranslatorFound) {
			delete guard.m_overwriteTranslator;
			guard.m_overwriteTranslator = nullptr;
		}

		// With modularization of Qt 5 some - but not all - of the qt_<locale>.ts files have become
		// so-called meta catalogues which no longer contain actual translations but refer to other
		// more specific ts files like qtbase_<locale>.ts . To successfully load a meta catalogue all
		// of its referenced translations must be available. As we do not want to bundle them all
		// we now try to load the old qt_<locale>.ts file first and then fall back to loading
		// qtbase_<locale>.ts if that failed.
		//
		// See http://doc.qt.io/qt-5/linguist-programmers.html#deploying-translations for more information
		guard.m_qtTranslator = new QTranslator();
		// First we try and see if there is a translation packaged with Mumble that shall overwrite any potentially
		// existing Qt translations. If not, we try to load the qt-translations installed on the host-machine and if
		// that fails as well, we try to load translations bundled in Mumble. Note: Resource starting with :/ are
		// bundled resources specified in a .qrc file
		const QString translationsPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
		if (guard.m_qtTranslator->load(locale, ":/mumble_overwrite_qt_")) {
			app.installTranslator(guard.m_qtTranslator);
		} else if (guard.m_qtTranslator->load(locale, ":/mumble_overwrite_qtbase_")) {
			app.installTranslator(guard.m_qtTranslator);
		} else if (guard.m_qtTranslator->load(locale, "qt_", prefix, translationsPath)) {
			app.installTranslator(guard.m_qtTranslator);
		} else if (guard.m_qtTranslator->load(locale, "qtbase_", prefix, translationsPath)) {
			app.installTranslator(guard.m_qtTranslator);
		} else if (guard.m_qtTranslator->load(locale, ":/qt_")) {
			app.installTranslator(guard.m_qtTranslator);
		} else if (guard.m_qtTranslator->load(locale, ":/qtbase_")) {
			app.installTranslator(guard.m_qtTranslator);
		} else {
			// Did not install Qt translator
			delete guard.m_qtTranslator;
			guard.m_qtTranslator = nullptr;
		}

		return guard;
	}
} // namespace Translations
} // namespace Mumble
