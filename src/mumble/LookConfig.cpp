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

#include <QColorDialog>
#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QStack>
#include <QtCore/QTimer>

#include <optional>

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
	for (const QString &key : d.entryList()) {
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
	connect(qbClearBackgroundColor, &QPushButton::clicked, this, &LookConfig::talkinguiBackgroundCleared);
	connect(qbBackgroundColor, &QPushButton::clicked, this, &LookConfig::qbBackgroundColor_clicked);

	connect(qlLAutoTheme, &ClickableLabel::clicked, this, [this]() { qrbAutoStyle->setChecked(true); });
	connect(qlLDarkTheme, &ClickableLabel::clicked, this, [this]() { qrbDarkStyle->setChecked(true); });
	connect(qlLLightTheme, &ClickableLabel::clicked, this, [this]() { qrbLightStyle->setChecked(true); });

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
		// This button is located below the theme selector in the ui config and opens the user theme directory
		connect(qpbThemesDirectory, &QPushButton::clicked, this,
				[userThemeDirectoryUrl]() { QDesktopServices::openUrl(userThemeDirectoryUrl); });
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

void LookConfig::qbBackgroundColor_clicked() {
	QColor color = QColorDialog::getColor(Qt::white, this, tr("Choose a Color"));
	if (color.isValid()) {
		talkinguiBackgroundSet(color);
	}
}

void LookConfig::talkinguiBackgroundSet(QColor color) {
	selectedBackgroundColor = color;
	QString style           = QString("background-color: %1;").arg(color.name());
	qccolorPreview->setStyleSheet(style);
	swBackgroundColor->setCurrentIndex(0);
	qlBackgroundColor->setBuddy(qbClearBackgroundColor);
}

void LookConfig::talkinguiBackgroundCleared() {
	selectedBackgroundColor = std::nullopt;
	swBackgroundColor->setCurrentIndex(1);
	qlBackgroundColor->setBuddy(qbBackgroundColor);
}

const QString &LookConfig::getName() const {
	return LookConfig::name;
}

QIcon LookConfig::icon() const {
	return QIcon(QLatin1String("skin:config_ui.png"));
}


static void selectThemeStyle(const std::optional< ThemeInfo::StyleInfo > &configuredStyle, QComboBox *comboBox) {
	if (configuredStyle) {
		for (int i = 0; i < comboBox->count(); ++i) {
			QVariant themeData = comboBox->itemData(i);
			if (!themeData.canConvert< ThemeInfo::StyleInfo >()) {
				continue;
			}
			ThemeInfo::StyleInfo style = themeData.value< ThemeInfo::StyleInfo >();
			if (style == configuredStyle.value()) {
				comboBox->setCurrentIndex(i);
				return;
			}
		}
	} else {
		comboBox->setCurrentIndex(0); // "None"
	}
}


void LookConfig::setActiveThemes(const std::optional< ThemeInfo::StyleInfo > configuredStyle,
								 const std::optional< ThemeInfo::StyleInfo > configuredDarkStyle) {
	selectThemeStyle(configuredStyle, qcbLightTheme);
	selectThemeStyle(configuredDarkStyle, qcbDarkTheme);
}

void LookConfig::reloadThemes() {
	const ThemeMap themes = Themes::getThemes();

	qcbLightTheme->clear();
	qcbDarkTheme->clear();
	qcbLightTheme->addItem(tr("None"));
	qcbDarkTheme->addItem(tr("None"));
	for (ThemeMap::const_iterator theme = themes.begin(); theme != themes.end(); ++theme) {
		for (ThemeInfo::StylesMap::const_iterator styleit = theme->styles.begin(); styleit != theme->styles.end();
			 ++styleit) {
			qcbLightTheme->addItem(theme->name + QLatin1String(" - ") + styleit->name, QVariant::fromValue(*styleit));
			qcbDarkTheme->addItem(theme->name + QLatin1String(" - ") + styleit->name, QVariant::fromValue(*styleit));
		}
	}
}

void LookConfig::load(const Settings &r) {
	loadComboBox(qcbLanguage, 0);
	loadComboBox(qcbChannelDrag, 0);
	loadComboBox(qcbUserDrag, 0);

	setStyleType(r.styleType);
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

	reloadThemes();
	const std::optional< ThemeInfo::StyleInfo > configuredStyle     = Themes::getThemeStyle(r, false);
	const std::optional< ThemeInfo::StyleInfo > configuredDarkStyle = Themes::getThemeStyle(r, true);
	setActiveThemes(configuredStyle, configuredDarkStyle);

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
	if (r.talkingUI_BackgroundColor.has_value()) {
		talkinguiBackgroundSet(*r.talkingUI_BackgroundColor);
	} else {
		talkinguiBackgroundCleared();
	}

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

	StyleType styleType = getStyleType();
	if (s.styleType != styleType) {
		s.requireThemeApplication = true;
	}
	s.styleType = styleType;

	QVariant themeData = qcbLightTheme->itemData(qcbLightTheme->currentIndex());
	if (themeData.isNull()) {
		s.requireThemeApplication |= Themes::setConfiguredStyle(s, std::nullopt);
	} else {
		s.requireThemeApplication |= Themes::setConfiguredStyle(s, themeData.value< ThemeInfo::StyleInfo >());
	}

	QVariant darkThemeData = qcbDarkTheme->itemData(qcbDarkTheme->currentIndex());
	if (darkThemeData.isNull()) {
		s.requireThemeApplication |= Themes::setConfiguredDarkStyle(s, std::nullopt);
	} else {
		s.requireThemeApplication |= Themes::setConfiguredDarkStyle(s, darkThemeData.value< ThemeInfo::StyleInfo >());
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
	s.talkingUI_BackgroundColor           = selectedBackgroundColor;

	s.qsHierarchyChannelSeparator = qleChannelSeparator->text();

	s.searchUserAction = static_cast< Search::SearchDialog::UserAction >(qcbSearchUserAction->currentData().toInt());
	s.searchChannelAction =
		static_cast< Search::SearchDialog::ChannelAction >(qcbSearchChannelAction->currentData().toInt());
}

void LookConfig::accept() const {
	Global::get().mw->setShowDockTitleBars((Global::get().s.wlWindowLayout == Settings::LayoutCustom)
										   && !Global::get().s.bLockLayout);
}

StyleType LookConfig::getStyleType() const {
	if (qrbAutoStyle->isChecked()) {
		return StyleType::Auto;
	}
	if (qrbDarkStyle->isChecked()) {
		return StyleType::Dark;
	}
	if (qrbLightStyle->isChecked()) {
		return StyleType::Light;
	}
	qWarning() << "Something went wrong fetching the StyleType, resorting to Auto";
	assert(false);
	return StyleType::Auto;
}

void LookConfig::setStyleType(StyleType styleType) const {
	switch (styleType) {
		case StyleType::Light:
			qrbLightStyle->setChecked(true);
			break;
		case StyleType::Dark:
			qrbDarkStyle->setChecked(true);
			break;
		case StyleType::Auto:
			qrbAutoStyle->setChecked(true);
			break;
	}
}

void LookConfig::themeDirectoryChanged() {
	qWarning() << "Theme directory changed";
	reloadThemes();

	auto toOptionalStyle = [](const QVariant &variant) -> std::optional< ThemeInfo::StyleInfo > {
		return variant.isNull() ? std::nullopt : std::optional(variant.value< ThemeInfo::StyleInfo >());
	};

	const std::optional< ThemeInfo::StyleInfo > configuredStyle =
		toOptionalStyle(qcbLightTheme->itemData(qcbLightTheme->currentIndex()));
	const std::optional< ThemeInfo::StyleInfo > configuredDarkStyle =
		toOptionalStyle(qcbDarkTheme->itemData(qcbDarkTheme->currentIndex()));

	setActiveThemes(configuredStyle, configuredDarkStyle);
}

void LookConfig::on_qcbAbbreviateChannelNames_stateChanged(int state) {
	bool abbreviateNames = (state == Qt::Checked) || (state == Qt::PartiallyChecked);

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
