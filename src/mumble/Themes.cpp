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
	
	qApp->setStyleSheet(getDefaultStylesheet());
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
	
	qApp->setStyleSheet(QString::fromUtf8(file.readAll()));
	
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

QVector<QDir> Themes::getSearchDirectories() {
	QVector<QDir> themeSearchDirectories;

	// Built-in themes contained in the binary have the lowest priority
	themeSearchDirectories << QDir(QLatin1String(":builtin/themes"));
	// Next come themes found in the applications Themes directory
	themeSearchDirectories << QDir(MumbleApplication::instance()->applicationVersionRootPath() + QLatin1String("/Themes"));
	// Highest priorty have themes located in the user directory
	themeSearchDirectories << QDir(g.qdBasePath.absolutePath() + QLatin1String("/Themes"));
	
	return themeSearchDirectories;
}

QString Themes::getDefaultStylesheet() {
	return QLatin1String(".log-channel{text-decoration:none;}.log-user{text-decoration:none;}p{margin:0;}");
}
