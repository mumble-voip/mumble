// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_THEMEINFO_H_
#define MUMBLE_MUMBLE_THEMEINFO_H_

#include <QMetaType>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>
#include <QtCore/QString>
#ifndef Q_MOC_RUN
#	include <boost/optional.hpp>
#endif

class QSettings;
class QDir;

class ThemeInfo;
typedef QMap< QString, ThemeInfo > ThemeMap;

/// Description of a Mumble theme with multiple styles
class ThemeInfo {
public:
	/// A specific style of a Mumble theme
	///
	/// Multiple styles can for example be used to differentiate light/dark
	/// variants of a theme.
	///
	/// A single style can refer to multiple run-time platform specific qss
	/// theme files.
	class StyleInfo {
	public:
		/// Name of the theme containing this style
		QString themeName;
		/// Name for the style
		QString name;

		/// @return Returns platform specific qss or defaultQss if none available
		QFileInfo getPlatformQss() const;

		/// Default QSS file for the style
		QFileInfo defaultQss;
		/// Platform specific QSS files available
		QMap< QString, QFileInfo > qssFiles;
	};

	typedef QMap< QString, StyleInfo > StylesMap;

	/// Takes stock of all mumble themes in the given folders.
	///
	/// If a theme with the same name is available in multiple directories
	/// only the last occurrence will be returned.
	///
	/// @param themesDirectories List of directories to search for theme directories.
	/// @return Map of theme name to Theme
	static ThemeMap scanDirectories(const QVector< QDir > &themesDirectories);

	/// Takes stock of all mumble themes in the given folder
	///
	/// Uses loadThemeInfoFromDirectory on each directory in the folder
	/// to find themes. Themes with the same names will override each other.
	///
	/// @param themesDirectory Directory to scan for theme directories
	/// @return Map of theme name to Theme
	static ThemeMap scanDirectory(const QDir &themesDirectory);

	/// Loads the theme description from a given directory
	///
	/// @param themeDirectory
	/// @return Theme if description was correctly loaded. boost::none if not.
	static boost::optional< ThemeInfo > load(const QDir &themeDirectory);

	/// @return Style with given name or default
	StyleInfo getStyle(QString name_) const;

	/// Ideally unique theme name. A theme with identical name can override.
	QString name;
	/// Style name to style mapping.
	StylesMap styles;
	/// Default style
	QString defaultStyle;
};

Q_DECLARE_METATYPE(ThemeInfo)
Q_DECLARE_METATYPE(ThemeInfo::StyleInfo)

#endif // MUMBLE_MUMBLE_THEMEINFO_H_
