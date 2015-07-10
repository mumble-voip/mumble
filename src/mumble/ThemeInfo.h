/* Copyright (C) 2015 Stefan Hacker <dd0t@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef MUMBLE_MUMBLE_THEMEINFO_H_
#define MUMBLE_MUMBLE_THEMEINFO_H_

#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>
#include <QMetaType>
#ifndef Q_MOC_RUN
#include <boost/optional.hpp>
#endif

class QSettings;
class QDir;

class ThemeInfo;
typedef QMap<QString, ThemeInfo> ThemeMap;

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
		QMap<QString, QFileInfo> qssFiles;
	};

	typedef QMap<QString, StyleInfo> StylesMap;

	/// Takes stock of all mumble themes in the given folders.
	/// 
	/// If a theme with the same name is available in multiple directories
	/// only the last occurance will be returned.
	/// 
	/// @param themesDirectories List of directories to search for theme directories.
	/// @return Map of theme name to Theme
	static ThemeMap scanDirectories(const QVector<QDir> &themesDirectories);
	
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
	static boost::optional<ThemeInfo> load(const QDir &themeDirectory);
	
	/// @return Style with given name or default
	StyleInfo getStyle(QString name) const;
	
	/// Ideally unique theme name. A theme with identical name can override.
	QString name;
	/// Style name to style mapping.
	StylesMap styles;
	/// Default style
	QString defaultStyle;
};

Q_DECLARE_METATYPE(ThemeInfo);
Q_DECLARE_METATYPE(ThemeInfo::StyleInfo);

#endif // MUMBLE_MUMBLE_THEMEINFO_H_
