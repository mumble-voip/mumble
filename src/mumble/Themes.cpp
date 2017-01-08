// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "Themes.h"
#include "Global.h"
#include "MainWindow.h"
#include "MumbleApplication.h"

boost::optional<ThemeInfo::StyleInfo> Themes::getConfiguredStyle(const Settings &settings) {
	if (settings.themeName.isEmpty() && settings.themeStyleName.isEmpty()) {
			return boost::none;
	}

	const ThemeMap themes = getThemes();
	ThemeMap::const_iterator themeIt = themes.find(settings.themeName);
	if (themeIt == themes.end()) {
		qWarning() << "Could not find configured theme" << settings.themeName;
		return boost::none;
	}
	
	ThemeInfo::StylesMap::const_iterator styleIt = themeIt->styles.find(settings.themeStyleName);
	if (styleIt == themeIt->styles.end()) {
		qWarning() << "Configured theme" << settings.themeName << "does not have configured style" << settings.themeStyleName;
		return boost::none;
	}
	
	return *styleIt;
}

void Themes::setConfiguredStyle(Settings &settings, boost::optional<ThemeInfo::StyleInfo> style, bool &outChanged) {
	if (style) {
		if (settings.themeName != style->themeName || settings.themeStyleName != style->name) {
			settings.themeName = style->themeName;
			settings.themeStyleName = style->name;
			outChanged = true;
		}
	} else {
		if (!settings.themeName.isEmpty() || !settings.themeStyleName.isEmpty()) {
			settings.themeName = settings.themeStyleName = QString();
			outChanged = true;
		}
	}
}

void Themes::applyFallback() {
	qWarning() << "Applying fallback style sheet";
	
	QStringList skinPaths;
	skinPaths << QLatin1String(":/");
	QDir::setSearchPaths(QLatin1String("skin"), skinPaths);
	
	QString userStylesheetFn = userStylesheetPath();
	QString userStylesheetContent;
	if (readStylesheet(userStylesheetFn, userStylesheetContent)) {
		qWarning("Themes: allowing user stylesheet at '%s' to override fallback stylesheet", qPrintable(userStylesheetFn));
	}

	qApp->setStyleSheet(
		getDefaultStylesheet() +
		QLatin1String("\n") +
		userStylesheetContent
	);
}

bool Themes::applyConfigured() {
	boost::optional<ThemeInfo::StyleInfo> style = Themes::getConfiguredStyle(g.s);
	if (!style) {
		return false;
	}
	
	const QFileInfo qssFile(style->getPlatformQss());
	
	qWarning() << "Theme:" << style->themeName;
	qWarning() << "Style:" << style->name;
	qWarning() << "--> qss:" << qssFile.absoluteFilePath();
	
	QFile file(qssFile.absoluteFilePath());
	if (!file.open(QFile::ReadOnly)) {
		qWarning() << "Failed to open theme stylesheet:" << file.errorString();
		return false;
	}
	
	QStringList skinPaths;
	skinPaths << qssFile.path();
	skinPaths << QLatin1String(":/"); // Some skins might want to fall-back on our built-in resources
	QDir::setSearchPaths(QLatin1String("skin"), skinPaths);

	QString themeQss = QString::fromUtf8(file.readAll());

	QString userStylesheetFn = userStylesheetPath();
	QString userStylesheetContent;
	if (readStylesheet(userStylesheetFn, userStylesheetContent)) {
		qWarning("Themes: allowing user stylesheet at '%s' to override the chosen theme", qPrintable(userStylesheetFn));
	}
	
	qApp->setStyleSheet(
		themeQss +
		QLatin1String("\n") +
		userStylesheetContent
	);

	return true;
}

bool Themes::apply() {
	const bool result = applyConfigured();
	if (!result) {
		applyFallback();
	}
	
	if (g.mw != NULL) {
		g.mw->qteLog->document()->setDefaultStyleSheet(qApp->styleSheet());
	}
	return result;
}

ThemeMap Themes::getThemes() {
	return ThemeInfo::scanDirectories(getSearchDirectories());
}

QDir Themes::getUserThemesDirectory() {
	return QDir(g.qdBasePath.absolutePath() + QLatin1String("/Themes"));
}

QVector<QDir> Themes::getSearchDirectories() {
	QVector<QDir> themeSearchDirectories;

	// Built-in themes contained in the binary have the lowest priority
	themeSearchDirectories << QDir(QLatin1String(":builtin/themes"));
	// Next come themes found in the applications Themes directory
	themeSearchDirectories << QDir(MumbleApplication::instance()->applicationVersionRootPath() + QLatin1String("/Themes"));
	// Highest priorty have themes located in the user directory
	themeSearchDirectories << getUserThemesDirectory();
	
	return themeSearchDirectories;
}

QString Themes::userStylesheetPath() {
	return g.qdBasePath.absolutePath() + QLatin1String("/user.qss");
}

bool Themes::readStylesheet(const QString &stylesheetFn, QString &stylesheetContent) {
	QFile file(stylesheetFn);
	if (!file.open(QFile::ReadOnly)) {
		stylesheetContent = QString();
		return false;
	}

	stylesheetContent = QString::fromUtf8(file.readAll());
	return true;
}

QString Themes::getDefaultStylesheet() {
	return QLatin1String(".log-channel{text-decoration:none;}.log-user{text-decoration:none;}p{margin:0;}#qwMacWarning{background-color:#FFFEDC;border-radius:5px;border:1px solid #B5B59E;}");
}
