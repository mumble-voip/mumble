/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "LookConfig.h"
#include "Themes.h"

#include "AudioInput.h"
#include "AudioOutput.h"
#include "Global.h"
#include "MainWindow.h"

static ConfigWidget *LookConfigNew(Settings &st) {
	return new LookConfig(st);
}

static ConfigRegistrar registrar(1100, LookConfigNew);

LookConfig::LookConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

#ifndef Q_OS_MAC
	if (! QSystemTrayIcon::isSystemTrayAvailable())
#endif
		qgbTray->hide();

	qcbLanguage->addItem(tr("System default"));
	QDir d(QLatin1String(":"),QLatin1String("mumble_*.qm"),QDir::Name,QDir::Files);
	foreach(const QString &key, d.entryList()) {
		QString cc = key.mid(7,key.indexOf(QLatin1Char('.'))-7);
		QLocale tmpLocale = QLocale(cc);

		//If there is no native language name, use the locale
		QString displayName = cc;
		if(!tmpLocale.nativeLanguageName().isEmpty()) {
			displayName = QString(QLatin1String("%1 (%2)"))
			        .arg(tmpLocale.nativeLanguageName())
			        .arg(cc);
		} else if (cc == QLatin1String("eo")){
			// Can't initialize QLocale for a countryless language (QTBUG-8452, QTBUG-14592).
			// We only have one so special case it.
			displayName = QLatin1String("Esperanto (eo)");
		}
		
		qcbLanguage->addItem(displayName, QVariant(cc));
	}

	qcbExpand->addItem(tr("None"), Settings::NoChannels);
	qcbExpand->addItem(tr("Only with users"), Settings::ChannelsWithUsers);
	qcbExpand->addItem(tr("All"), Settings::AllChannels);

	qcbChannelDrag->insertItem(Settings::Ask, tr("Ask"), Settings::Ask);
	qcbChannelDrag->insertItem(Settings::DoNothing, tr("Do Nothing"), Settings::DoNothing);
	qcbChannelDrag->insertItem(Settings::Move, tr("Move"), Settings::Move);
}

QString LookConfig::title() const {
	return tr("User Interface");
}

QIcon LookConfig::icon() const {
	return QIcon(QLatin1String("skin:config_ui.png"));
}

void LookConfig::load(const Settings &r) {
	loadComboBox(qcbLanguage, 0);
	loadComboBox(qcbChannelDrag, 0);

	// Load Layout checkbox state
	switch (r.wlWindowLayout) {
		case Settings::LayoutClassic:
			qrbLClassic->setChecked(true);
			break;
		case Settings::LayoutStacked:
			qrbLStacked->setChecked(true);
			break;
		case Settings::LayoutHybrid:
			qrbLHybrid->setChecked(true);
			break;
		case Settings::LayoutCustom:
		default:
			s.wlWindowLayout = Settings::LayoutCustom;
			qrbLCustom->setChecked(true);
			break;
	}


	for (int i=0;i<qcbLanguage->count();i++) {
		if (qcbLanguage->itemData(i).toString() == r.qsLanguage) {
			loadComboBox(qcbLanguage, i);
			break;
		}
	}
	
	loadComboBox(qcbAlwaysOnTop, r.aotbAlwaysOnTop);

	loadComboBox(qcbExpand, r.ceExpand);
	loadComboBox(qcbChannelDrag, r.ceChannelDrag);
	loadCheckBox(qcbUsersTop, r.bUserTop);
	loadCheckBox(qcbAskOnQuit, r.bAskOnQuit);
	loadCheckBox(qcbHideTray, r.bHideInTray);
	loadCheckBox(qcbStateInTray, r.bStateInTray);
	loadCheckBox(qcbShowUserCount, r.bShowUserCount);
	loadCheckBox(qcbShowContextMenuInMenuBar, r.bShowContextMenuInMenuBar);
	loadCheckBox(qcbShowTransmitModeComboBox, r.bShowTransmitModeComboBox);
	loadCheckBox(qcbHighContrast, r.bHighContrast);
	loadCheckBox(qcbChatBarUseSelection, r.bChatBarUseSelection);
	loadCheckBox(qcbFilterHidesEmptyChannels, r.bFilterHidesEmptyChannels);
	
	
	qcbTheme->clear();
	
	const boost::optional<ThemeInfo::StyleInfo> configuredStyle = Themes::getConfiguredStyle(r);
	const ThemeMap themes = Themes::getThemes();
	
	int selectedThemeEntry = 0;
	qcbTheme->addItem(tr("None"));
	for (ThemeMap::const_iterator theme = themes.begin();
	     theme != themes.end();
	     ++theme) {
		
		for (ThemeInfo::StylesMap::const_iterator style = theme->styles.begin();
		     style != theme->styles.end();
		     ++style) {
			
			if (configuredStyle
			     && configuredStyle->themeName == style->themeName
			     && configuredStyle->name == style->name) {
				selectedThemeEntry = qcbTheme->count();
			}
			
			qcbTheme->addItem(theme->name + QLatin1String(" - ") + style->name, QVariant::fromValue(*style));
		}
	}
	
	qcbTheme->setCurrentIndex(selectedThemeEntry);
}

void LookConfig::save() const {
	const QString oldLanguage = s.qsLanguage;
	if (qcbLanguage->currentIndex() == 0)
		s.qsLanguage = QString();
	else
		s.qsLanguage = qcbLanguage->itemData(qcbLanguage->currentIndex()).toString();
	
	if (s.qsLanguage != oldLanguage) {
		s.requireRestartToApply = true;
	}

	// Save Layout radioboxes state
	if (qrbLClassic->isChecked()) {
		s.wlWindowLayout = Settings::LayoutClassic;
	} else if (qrbLStacked->isChecked()) {
		s.wlWindowLayout = Settings::LayoutStacked;
	} else if (qrbLHybrid->isChecked()) {
		s.wlWindowLayout = Settings::LayoutHybrid;
	} else {
		s.wlWindowLayout = Settings::LayoutCustom;
	}

	s.ceExpand=static_cast<Settings::ChannelExpand>(qcbExpand->currentIndex());
	s.ceChannelDrag=static_cast<Settings::ChannelDrag>(qcbChannelDrag->currentIndex());
	
	if (qcbUsersTop->isChecked() != s.bUserTop) {
		s.bUserTop = qcbUsersTop->isChecked();
		s.requireRestartToApply = true;
	}
	
	s.aotbAlwaysOnTop = static_cast<Settings::AlwaysOnTopBehaviour>(qcbAlwaysOnTop->currentIndex());
	s.bAskOnQuit = qcbAskOnQuit->isChecked();
	s.bHideInTray = qcbHideTray->isChecked();
	s.bStateInTray = qcbStateInTray->isChecked();
	s.bShowUserCount = qcbShowUserCount->isChecked();
	s.bShowContextMenuInMenuBar = qcbShowContextMenuInMenuBar->isChecked();
	s.bShowTransmitModeComboBox = qcbShowTransmitModeComboBox->isChecked();
	s.bHighContrast = qcbHighContrast->isChecked();
	s.bChatBarUseSelection = qcbChatBarUseSelection->isChecked();
	s.bFilterHidesEmptyChannels = qcbFilterHidesEmptyChannels->isChecked();
	
	QVariant themeData = qcbTheme->itemData(qcbTheme->currentIndex());
	if (themeData.isNull()) {
		Themes::setConfiguredStyle(s, boost::none, s.requireRestartToApply);
	} else {
		Themes::setConfiguredStyle(s, themeData.value<ThemeInfo::StyleInfo>(), s.requireRestartToApply);
	}
}

void LookConfig::accept() const {
	g.mw->setShowDockTitleBars(g.s.wlWindowLayout == Settings::LayoutCustom);
}

bool LookConfig::expert(bool b) {
	qcbExpand->setVisible(b);
	qliExpand->setVisible(b);
	qcbUsersTop->setVisible(b);
	qcbStateInTray->setVisible(b);
	qcbShowContextMenuInMenuBar->setVisible(b);
	return true;
}
