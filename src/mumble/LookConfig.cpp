// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "LookConfig.h"
#include "Themes.h"

#include "AudioInput.h"
#include "AudioOutput.h"
#include "MainWindow.h"
#include "SearchDialog.h"
#include "Global.h"

#include <QSystemTrayIcon>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QStack>
#include <QtCore/QTimer>

const QString LookConfig::name = QLatin1String("LookConfig");

static ConfigWidget *LookConfigNew(Settings &st) {
	return new LookConfig(st);
}

static ConfigRegistrar registrar(1100, LookConfigNew);

LookConfig::LookConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		qgbTray->hide();
	}

#ifdef Q_OS_MAC
	// Qt can not hide the window via the native macOS hide function. This should be re-evaluated with new Qt versions.
	qcbHideTray->hide();
#endif

	qcbLanguage->addItem(tr("System default"));
	QDir d(QLatin1String(":"), QLatin1String("mumble_*.qm"), QDir::Name, QDir::Files);
	foreach (const QString &key, d.entryList()) {
		QString cc        = key.mid(7, key.indexOf(QLatin1Char('.')) - 7);
		QLocale tmpLocale = QLocale(cc);

		// If there is no native language name, use the locale
		QString displayName = cc;
		if (!tmpLocale.nativeLanguageName().isEmpty()) {
			displayName = QString(QLatin1String("%1 (%2)")).arg(tmpLocale.nativeLanguageName()).arg(cc);
		} else if (cc == QLatin1String("eo")) {
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

	qcbUserDrag->insertItem(Settings::Ask, tr("Ask"), Settings::Ask);
	qcbUserDrag->insertItem(Settings::DoNothing, tr("Do Nothing"), Settings::DoNothing);
	qcbUserDrag->insertItem(Settings::Move, tr("Move"), Settings::Move);

	connect(qrbLCustom, SIGNAL(toggled(bool)), qcbLockLayout, SLOT(setEnabled(bool)));

	QDir userThemeDirectory = Themes::getUserThemesDirectory();
	if (userThemeDirectory.exists()) {
		m_themeDirectoryWatcher = new QFileSystemWatcher(this);

		// Use a timer to cut down floods of directory changes. We only want
		// to trigger a refresh after nothing has happened for 200ms in the
		// watched directory.
		m_themeDirectoryDebouncer = new QTimer(this);
		m_themeDirectoryDebouncer->setSingleShot(true);
		m_themeDirectoryDebouncer->setInterval(200);
		m_themeDirectoryDebouncer->connect(m_themeDirectoryWatcher, SIGNAL(directoryChanged(QString)), SLOT(start()));

		connect(m_themeDirectoryDebouncer, SIGNAL(timeout()), SLOT(themeDirectoryChanged()));
		m_themeDirectoryWatcher->addPath(userThemeDirectory.path());

		QUrl userThemeDirectoryUrl = QUrl::fromLocalFile(userThemeDirectory.path());
		//: This link is located next to the theme heading in the ui config and opens the user theme directory
		qlThemesDirectory->setText(tr("<a href=\"%1\">Browse</a>").arg(userThemeDirectoryUrl.toString()));
		qlThemesDirectory->setOpenExternalLinks(true);
	}

#define ADD_SEARCH_USERACTION(name)                                                                      \
	qcbSearchUserAction->addItem(Search::SearchDialog::toString(Search::SearchDialog::UserAction::name), \
								 static_cast< int >(Search::SearchDialog::UserAction::name))
	ADD_SEARCH_USERACTION(NONE);
	ADD_SEARCH_USERACTION(JOIN);
#undef ADD_SEARCH_USERACTION
#define ADD_SEARCH_CHANNELACTION(name)                                                                         \
	qcbSearchChannelAction->addItem(Search::SearchDialog::toString(Search::SearchDialog::ChannelAction::name), \
									static_cast< int >(Search::SearchDialog::ChannelAction::name))
	ADD_SEARCH_CHANNELACTION(NONE);
	ADD_SEARCH_CHANNELACTION(JOIN);
#undef ADD_SEARCH_CHANNELACTION
}

QString LookConfig::title() const {
	return tr("User Interface");
}

const QString &LookConfig::getName() const {
	return LookConfig::name;
}

QIcon LookConfig::icon() const {
	return QIcon(QLatin1String("skin:config_ui.png"));
}

void LookConfig::reloadThemes(const boost::optional< ThemeInfo::StyleInfo > configuredStyle) {
	const ThemeMap themes = Themes::getThemes();

	int selectedThemeEntry = 0;

	qcbTheme->clear();
	qcbTheme->addItem(tr("None"));
	for (ThemeMap::const_iterator theme = themes.begin(); theme != themes.end(); ++theme) {
		for (ThemeInfo::StylesMap::const_iterator styleit = theme->styles.begin(); styleit != theme->styles.end();
			 ++styleit) {
			if (configuredStyle && configuredStyle->themeName == styleit->themeName
				&& configuredStyle->name == styleit->name) {
				selectedThemeEntry = qcbTheme->count();
			}

			qcbTheme->addItem(theme->name + QLatin1String(" - ") + styleit->name, QVariant::fromValue(*styleit));
		}
	}

	qcbTheme->setCurrentIndex(selectedThemeEntry);
}

void LookConfig::load(const Settings &r) {
	loadComboBox(qcbLanguage, 0);
	loadComboBox(qcbChannelDrag, 0);
	loadComboBox(qcbUserDrag, 0);

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
	qcbLockLayout->setEnabled(r.wlWindowLayout == Settings::LayoutCustom);


	for (int i = 0; i < qcbLanguage->count(); i++) {
		if (qcbLanguage->itemData(i).toString() == r.qsLanguage) {
			loadComboBox(qcbLanguage, i);
			break;
		}
	}

	loadComboBox(qcbAlwaysOnTop, r.aotbAlwaysOnTop);

	loadComboBox(qcbExpand, r.ceExpand);
	loadComboBox(qcbChannelDrag, r.ceChannelDrag);
	loadComboBox(qcbUserDrag, r.ceUserDrag);
	loadCheckBox(qcbUsersTop, r.bUserTop);

	loadComboBox(qcbQuitBehavior, static_cast< int >(r.quitBehavior));

	loadCheckBox(qcbEnableDeveloperMenu, r.bEnableDeveloperMenu);
	loadCheckBox(qcbLockLayout, (r.wlWindowLayout == Settings::LayoutCustom) && r.bLockLayout);
	loadCheckBox(qcbHideTray, r.bHideInTray);
	loadCheckBox(qcbStateInTray, r.bStateInTray);
	loadCheckBox(qcbShowUserCount, r.bShowUserCount);
	loadCheckBox(qcbShowVolumeAdjustments, r.bShowVolumeAdjustments);
	loadCheckBox(qcbShowNicknamesOnly, r.bShowNicknamesOnly);
	loadCheckBox(qcbShowContextMenuInMenuBar, r.bShowContextMenuInMenuBar);
	loadCheckBox(qcbShowTransmitModeComboBox, r.bShowTransmitModeComboBox);
	loadCheckBox(qcbHighContrast, r.bHighContrast);
	loadCheckBox(qcbChatBarUseSelection, r.bChatBarUseSelection);
	loadCheckBox(qcbFilterHidesEmptyChannels, r.bFilterHidesEmptyChannels);

	const boost::optional< ThemeInfo::StyleInfo > configuredStyle = Themes::getConfiguredStyle(r);
	reloadThemes(configuredStyle);

	loadCheckBox(qcbUsersAlwaysVisible, r.talkingUI_UsersAlwaysVisible);
	loadCheckBox(qcbLocalUserVisible, r.bTalkingUI_LocalUserStaysVisible);
	loadCheckBox(qcbAbbreviateChannelNames, r.bTalkingUI_AbbreviateChannelNames);
	loadCheckBox(qcbAbbreviateCurrentChannel, r.bTalkingUI_AbbreviateCurrentChannel);
	loadCheckBox(qcbShowLocalListeners, r.bTalkingUI_ShowLocalListeners);
	qsbRelFontSize->setValue(r.iTalkingUI_RelativeFontSize);
	qsbSilentUserLifetime->setValue(r.iTalkingUI_SilentUserLifeTime);
	qsbChannelHierarchyDepth->setValue(r.iTalkingUI_ChannelHierarchyDepth);
	qsbMaxNameLength->setValue(r.iTalkingUI_MaxChannelNameLength);
	qsbPrefixCharCount->setValue(r.iTalkingUI_PrefixCharCount);
	qsbPostfixCharCount->setValue(r.iTalkingUI_PostfixCharCount);
	qleAbbreviationReplacement->setText(r.qsTalkingUI_AbbreviationReplacement);

	qleChannelSeparator->setText(r.qsHierarchyChannelSeparator);

	loadComboBox(qcbSearchUserAction, static_cast< int >(r.searchUserAction));
	loadComboBox(qcbSearchChannelAction, static_cast< int >(r.searchChannelAction));
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

	s.ceExpand      = static_cast< Settings::ChannelExpand >(qcbExpand->currentIndex());
	s.ceChannelDrag = static_cast< Settings::ChannelDrag >(qcbChannelDrag->currentIndex());
	s.ceUserDrag    = static_cast< Settings::ChannelDrag >(qcbUserDrag->currentIndex());

	if (qcbUsersTop->isChecked() != s.bUserTop) {
		s.bUserTop              = qcbUsersTop->isChecked();
		s.requireRestartToApply = true;
	}

	s.aotbAlwaysOnTop           = static_cast< Settings::AlwaysOnTopBehaviour >(qcbAlwaysOnTop->currentIndex());
	s.quitBehavior              = static_cast< QuitBehavior >(qcbQuitBehavior->currentIndex());
	s.bEnableDeveloperMenu      = qcbEnableDeveloperMenu->isChecked();
	s.bLockLayout               = qcbLockLayout->isChecked();
	s.bHideInTray               = qcbHideTray->isChecked();
	s.bStateInTray              = qcbStateInTray->isChecked();
	s.bShowUserCount            = qcbShowUserCount->isChecked();
	s.bShowVolumeAdjustments    = qcbShowVolumeAdjustments->isChecked();
	s.bShowNicknamesOnly        = qcbShowNicknamesOnly->isChecked();
	s.bShowContextMenuInMenuBar = qcbShowContextMenuInMenuBar->isChecked();
	s.bShowTransmitModeComboBox = qcbShowTransmitModeComboBox->isChecked();
	s.bHighContrast             = qcbHighContrast->isChecked();
	s.bChatBarUseSelection      = qcbChatBarUseSelection->isChecked();
	s.bFilterHidesEmptyChannels = qcbFilterHidesEmptyChannels->isChecked();

	QVariant themeData = qcbTheme->itemData(qcbTheme->currentIndex());
	if (themeData.isNull()) {
		Themes::setConfiguredStyle(s, boost::none, s.requireRestartToApply);
	} else {
		Themes::setConfiguredStyle(s, themeData.value< ThemeInfo::StyleInfo >(), s.requireRestartToApply);
	}

	s.talkingUI_UsersAlwaysVisible        = qcbUsersAlwaysVisible->isChecked();
	s.bTalkingUI_LocalUserStaysVisible    = qcbLocalUserVisible->isChecked();
	s.bTalkingUI_AbbreviateChannelNames   = qcbAbbreviateChannelNames->isChecked();
	s.bTalkingUI_AbbreviateCurrentChannel = qcbAbbreviateCurrentChannel->isChecked();
	s.bTalkingUI_ShowLocalListeners       = qcbShowLocalListeners->isChecked();
	s.iTalkingUI_RelativeFontSize         = qsbRelFontSize->value();
	s.iTalkingUI_SilentUserLifeTime       = qsbSilentUserLifetime->value();
	s.iTalkingUI_ChannelHierarchyDepth    = qsbChannelHierarchyDepth->value();
	s.iTalkingUI_MaxChannelNameLength     = qsbMaxNameLength->value();
	s.iTalkingUI_PrefixCharCount          = qsbPrefixCharCount->value();
	s.iTalkingUI_PostfixCharCount         = qsbPostfixCharCount->value();
	s.qsTalkingUI_AbbreviationReplacement = qleAbbreviationReplacement->text();

	s.qsHierarchyChannelSeparator = qleChannelSeparator->text();

	s.searchUserAction = static_cast< Search::SearchDialog::UserAction >(qcbSearchUserAction->currentData().toInt());
	s.searchChannelAction =
		static_cast< Search::SearchDialog::ChannelAction >(qcbSearchChannelAction->currentData().toInt());
}

void LookConfig::accept() const {
	Global::get().mw->setShowDockTitleBars((Global::get().s.wlWindowLayout == Settings::LayoutCustom)
										   && !Global::get().s.bLockLayout);
}

void LookConfig::themeDirectoryChanged() {
	qWarning() << "Theme directory changed";
	QVariant themeData = qcbTheme->itemData(qcbTheme->currentIndex());
	if (themeData.isNull()) {
		reloadThemes(boost::none);
	} else {
		reloadThemes(themeData.value< ThemeInfo::StyleInfo >());
	}
}

void LookConfig::on_qcbAbbreviateChannelNames_stateChanged(int state) {
	bool abbreviateNames = state == Qt::Checked;

	// Only enable the abbreviation related settings if abbreviation is actually enabled
	qcbAbbreviateCurrentChannel->setEnabled(abbreviateNames);
	qsbChannelHierarchyDepth->setEnabled(abbreviateNames);
	qsbMaxNameLength->setEnabled(abbreviateNames);
	qsbPrefixCharCount->setEnabled(abbreviateNames);
	qsbPostfixCharCount->setEnabled(abbreviateNames);
	qleAbbreviationReplacement->setEnabled(abbreviateNames);
}

void LookConfig::on_qcbUsersAlwaysVisible_stateChanged(int state) {
	bool usersAlwaysVisible = state == Qt::Checked;

	// Only enable the local user visibility setting when all users are not always visible
	qcbLocalUserVisible->setEnabled(!usersAlwaysVisible);
	// Only enable the user visibility timeout settings when all users are not always visible
	qsbSilentUserLifetime->setEnabled(!usersAlwaysVisible);
}
