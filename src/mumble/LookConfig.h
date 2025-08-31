// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_LOOKCONFIG_H_
#define MUMBLE_MUMBLE_LOOKCONFIG_H_

#include <optional>

#include "ConfigDialog.h"
#include "ThemeInfo.h"

#include "ui_LookConfig.h"

#include <optional>

class QFileSystemWatcher;
class QTimer;

class LookConfig : public ConfigWidget, Ui::LookConfig {
private:
	Q_OBJECT
	Q_DISABLE_COPY(LookConfig)
	std::optional< QColor > selectedBackgroundColor;

public:
	/// The unique name of this ConfigWidget
	static const QString name;
	LookConfig(Settings &st);
	virtual QString title() const Q_DECL_OVERRIDE;
	virtual const QString &getName() const Q_DECL_OVERRIDE;
	virtual QIcon icon() const Q_DECL_OVERRIDE;

public slots:
	void accept() const Q_DECL_OVERRIDE;
	void save() const Q_DECL_OVERRIDE;
	void load(const Settings &r) Q_DECL_OVERRIDE;
	void talkinguiBackgroundSet(QColor color);
	void talkinguiBackgroundCleared();
	void setActiveThemes(const std::optional< ThemeInfo::StyleInfo > configuredStyle,
						 const std::optional< ThemeInfo::StyleInfo > configuredDarkStyle);
	void setStyleType(StyleType styleType) const;
	StyleType getStyleType() const;
	void themeDirectoryChanged();
	void on_qcbAbbreviateChannelNames_stateChanged(int state);
	void on_qcbUsersAlwaysVisible_stateChanged(int state);
	void qbBackgroundColor_clicked();
	void on_qcbTalkingUITransparent_stateChanged(int state);
	void on_qsbTransparencyLevel_valueChanged(int value);


private:
	/// Reload themes combobox and select given configuredStyle in it
	void reloadThemes();

	/// Timer to prevent change event floods from triggering theme reloads
	QTimer *m_themeDirectoryDebouncer;
	/// Watcher to keep theme list up to date with user theme directory
	QFileSystemWatcher *m_themeDirectoryWatcher;
};

#endif
