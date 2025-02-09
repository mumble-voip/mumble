// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_THEMES_H_
#define MUMBLE_MUMBLE_THEMES_H_

#include <Settings.h>
#include <ThemeInfo.h>
#ifndef Q_MOC_RUN
#	include <boost/optional.hpp>
#endif

class Themes {
public:
	/// Returns the style configured in the given settings structure
	static boost::optional< ThemeInfo::StyleInfo > getConfiguredStyle(const Settings &settings);

	/// Updates the given settings object to be configured to the given style
	///
	/// @note Does not apply the theme @see apply
	///
	/// @param settings Settings object to update
	/// @param style Style to set
	/// @param outChanged Will be set to true if the style in settings actually changed. Will not be changed otherwise.
	static void setConfiguredStyle(Settings &settings, boost::optional< ThemeInfo::StyleInfo > style, bool &outChanged);

	/// Applies the theme
	///
	/// @note Can only apply a theme before MainWindow etc. is opened
	static bool apply();

	/// Return a theme name to theme map
	static ThemeMap getThemes();

	/// Returns the per user themes directory
	static QDir getUserThemesDirectory();

private:
	/// Applies the fallback stylesheet
	static void applyFallback();

	/// Tries to apply the configured theme.
	/// @return True on success. False on failure.
	static bool applyConfigured();

	// Sets the theme to a QSS theme
	static void setTheme(QString &themeQss, QStringList &skinPaths);

	/// Returns list of theme search directories ordered ascending by priority (lowest first)
	static QVector< QDir > getSearchDirectories();

	/// Returns default style-sheet used for fall-backs
	static QString getDefaultStylesheet();

	/// userStylesheetPath returns the absolute path to the
	/// user.qss file.
	static QString userStylesheetPath();

	/// readStylesheet fills stylesheetContent with the content
	/// of the file at stylesheetFn, if available.
	/// If a the file is is available, the function returns true.
	/// If no file is available, it returns false.
	static bool readStylesheet(const QString &stylesheetFn, QString &stylesheetContent);
};

#endif // MUMBLE_MUMBLE_THEMES_H_
