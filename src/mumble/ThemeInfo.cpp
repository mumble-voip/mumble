// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ThemeInfo.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QRegularExpression>
#include <QtCore/QSettings>

QFileInfo ThemeInfo::StyleInfo::getPlatformQss() const {
#if defined(Q_OS_WIN)
	return qssFiles.value(QLatin1String("WIN"), defaultQss);
#elif defined(Q_OS_LINUX)
	return qssFiles.value(QLatin1String("LINUX"), defaultQss);
#elif defined(Q_OS_MAC)
	return qssFiles.value(QLatin1String("MAC"), defaultQss);
#else
	return defaultQss;
#endif
}

boost::optional< ThemeInfo::StyleInfo > readStyleFromConfig(QSettings &themeConfig, const QString &styleId,
															const ThemeInfo &theme, const QDir &themeDir) {
	const QRegularExpression qssPlatformRegex(QString::fromLatin1("^%1/qss_(.*)").arg(styleId));

	ThemeInfo::StyleInfo style;

	style.name      = themeConfig.value(QString::fromLatin1("%1/name").arg(styleId)).toString();
	style.themeName = theme.name;
	style.defaultQss =
		QFileInfo(themeDir.filePath(themeConfig.value(QString::fromLatin1("%1/qss").arg(styleId)).toString()));

	if (style.name.isNull()) {
		qWarning() << "Style " << styleId << " of theme" << theme.name << " has no name, skipping theme";
		return boost::none;
	}

	if (!style.defaultQss.exists() || !style.defaultQss.isFile()) {
		qWarning() << "Style " << style.name << " of theme " << theme.name << " references invalid qss "
				   << style.defaultQss.filePath() << ", skipping theme";
		return boost::none;
	}

	foreach (const QString &platformQssConfig, themeConfig.allKeys().filter(qssPlatformRegex)) {
		const QString platform = qssPlatformRegex.match(platformQssConfig).captured(1);

		const auto platformQss = QFileInfo(themeDir.filePath(themeConfig.value(platformQssConfig).toString()));
		if (!platformQss.exists() || !platformQss.isFile()) {
			qWarning() << "Style" << style.name << " of theme " << theme.name << " references invalid qss "
					   << platformQss.filePath() << " for platform " << platform << ", skipping theme";
			return boost::none;
		}

		style.qssFiles.insert(platform, platformQss);
	}

	return style;
}

boost::optional< ThemeInfo > loadLegacyThemeInfo(const QDir &themeDirectory) {
	ThemeInfo theme;
	theme.name = themeDirectory.dirName();

	QStringList filters;
	filters << QLatin1String("*.qss");

	foreach (const QFileInfo &qssFile, themeDirectory.entryInfoList(filters, QDir::Files)) {
		ThemeInfo::StyleInfo style;
		style.name       = qssFile.baseName();
		style.themeName  = theme.name;
		style.defaultQss = qssFile;

		theme.styles.insert(style.name, style);
	}

	if (theme.styles.isEmpty()) {
		qWarning() << themeDirectory.absolutePath() << " does not seem to contain a old-style theme, skipping";
		return boost::none;
	}

	theme.defaultStyle = theme.styles.begin()->name;

	return theme;
}

boost::optional< ThemeInfo > ThemeInfo::load(const QDir &themeDirectory) {
	QFile themeFile(themeDirectory.absoluteFilePath(QLatin1String("theme.ini")));
	if (!themeFile.exists()) {
		qWarning() << "Directory " << themeDirectory.absolutePath() << " has no theme.ini, trying fallback";

		return loadLegacyThemeInfo(themeDirectory);
	}

	QSettings themeConfig(themeFile.fileName(), QSettings::IniFormat);
	if (themeConfig.status() != QSettings::NoError) {
		qWarning() << "Failed to load theme config from " << themeFile.fileName() << ", skipping";
		return boost::none;
	}

	ThemeInfo theme;
	QStringList styleIds;

	theme.name = themeConfig.value(QLatin1String("theme/name")).toString();
	styleIds   = themeConfig.value(QLatin1String("theme/styles")).toStringList();

	if (theme.name.isNull()) {
		qWarning() << "Theme in " << themeFile.fileName() << " does not have a name, skipping";
		return boost::none;
	}

	if (styleIds.isEmpty()) {
		qWarning() << "Theme " << theme.name << " doesn't have any styles, skipping";
		return boost::none;
	}

	foreach (const QString &styleId, styleIds) {
		boost::optional< ThemeInfo::StyleInfo > style =
			readStyleFromConfig(themeConfig, styleId, theme, themeDirectory);
		if (!style) {
			return boost::none;
		}
		theme.styles.insert(style->name, *style);
	}

	theme.defaultStyle = theme.styles.begin()->name;

	return theme;
}

ThemeInfo::StyleInfo ThemeInfo::getStyle(QString name_) const {
	Q_ASSERT(styles.contains(defaultStyle));

	return styles.value(name_, styles.value(defaultStyle));
}


ThemeMap ThemeInfo::scanDirectory(const QDir &themesDirectory) {
	ThemeMap themes;

	foreach (const QFileInfo &subdirInfo, themesDirectory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
		QDir subdir(subdirInfo.absoluteFilePath());

		boost::optional< ThemeInfo > theme = ThemeInfo::load(subdir);
		if (!theme) {
			continue;
		}

		themes.insert(theme->name, *theme);
	};

	return themes;
}

ThemeMap ThemeInfo::scanDirectories(const QVector< QDir > &themesDirectories) {
	ThemeMap themes;

	foreach (const QDir &themesDirectory, themesDirectories) {
		foreach (const ThemeInfo &theme, scanDirectory(themesDirectory)) { themes.insert(theme.name, theme); }
	}

	return themes;
}
