// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OVERLAYCONFIG_H_
#define MUMBLE_MUMBLE_OVERLAYCONFIG_H_

#include "ConfigWidget.h"
#include "OverlayText.h"

#include "ui_Overlay.h"

class OverlayUserGroup;
struct OverlayAppInfo;
class OverlayPositionableItem;

class OverlayConfig : public ConfigWidget, public Ui::OverlayConfig {
private:
	Q_OBJECT
	Q_DISABLE_COPY(OverlayConfig)

	void initDisplay();
	void initDisplayFps();
	void initDisplayClock();
	void refreshFpsDemo();
	void refreshFpsLive();
	void refreshTimeLive();
	void addWhitelistPath(const QString &path);

protected:
	QPixmap qpScreen;
	QGraphicsPixmapItem *qgpiScreen;
	QGraphicsScene qgs;
	QGraphicsScene qgsFpsPreview;
	BasepointPixmap bpFpsDemo;
	BasepointPixmap bpTimeDemo;
	QGraphicsPixmapItem *qgpiFpsDemo;
	OverlayPositionableItem *qgpiFpsLive;
	OverlayPositionableItem *qgpiTimeLive;
	OverlayUserGroup *oug;
	QGraphicsTextItem *qgtiInstructions;

	float fViewScale;

	bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;

	bool supportsInstallableOverlay();
	bool isInstalled();
	bool needsUpgrade();
	bool installFiles();
	bool uninstallFiles();
	bool supportsCertificates();
	bool installerIsValid();
	void showCertificates();

	void updateOverlayExclusionModeState();
protected slots:
	void on_qpbInstall_clicked();
	void on_qpbUninstall_clicked();

	void on_qcbOverlayExclusionMode_currentIndexChanged(int);

	void on_qlwLaunchers_itemSelectionChanged();
	void on_qpbLaunchersAdd_clicked();
	void on_qpbLaunchersRemove_clicked();

	void on_qlwWhitelist_itemSelectionChanged();
	void on_qpbWhitelistAdd_clicked();
	void on_qpbWhitelistRemove_clicked();

	void on_qlwPaths_itemSelectionChanged();
	void on_qpbPathsAdd_clicked();
	void on_qpbPathsRemove_clicked();

	void on_qlwBlacklist_itemSelectionChanged();
	void on_qpbBlacklistAdd_clicked();
	void on_qpbBlacklistRemove_clicked();

	void on_qcbEnable_stateChanged(int);
	void on_qcbShowFps_stateChanged(int);
	void on_qcbShowTime_stateChanged(int);
	void on_qpbFpsFont_clicked();
	void on_qpbFpsColor_clicked();
	void on_qpbLoadPreset_clicked();
	void on_qpbSavePreset_clicked();
	void resizeScene(bool force = false);

public:
	/// The unique name of this ConfigWidget
	static const QString name;
	OverlayConfig(Settings &st);
	virtual QString title() const Q_DECL_OVERRIDE;
	virtual const QString &getName() const Q_DECL_OVERRIDE;
	virtual QIcon icon() const Q_DECL_OVERRIDE;
public slots:
	void accept() const Q_DECL_OVERRIDE;
	void save() const Q_DECL_OVERRIDE;
	void load(const Settings &r) Q_DECL_OVERRIDE;
};

#endif
