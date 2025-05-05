// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OverlayConfig.h"

#include "Channel.h"
#include "Database.h"
#include "MainWindow.h"
#include "NetworkConfig.h"
#include "Overlay.h"
#include "OverlayPositionableItem.h"
#include "OverlayText.h"
#include "OverlayUserGroup.h"
#include "PathListWidget.h"
#include "Screen.h"
#include "ServerHandler.h"
#include "User.h"
#include "Global.h"
#include "GlobalShortcut.h"

#ifdef Q_OS_WIN
#	include "../../overlay/overlay_blacklist.h"
#	include "../../overlay/overlay_launchers.h"
#	include "../../overlay/overlay_whitelist.h"
#endif

#include <QSettings>
#include <QtGui/QScreen>
#include <QtGui/QWindow>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QFontDialog>

const QString OverlayConfig::name = QLatin1String("OverlayConfig");

static const int OVERLAYCONFIG_PATH_ROLE    = Qt::UserRole;
static const int OVERLAYCONFIG_BUILTIN_ROLE = Qt::UserRole + 1;

// Hide overlay config for Mac OS X universal builds
#if !defined(USE_MAC_UNIVERSAL)
static ConfigWidget *OverlayConfigDialogNew(Settings &st) {
	return new OverlayConfig(st);
}

static ConfigRegistrar registrarOverlayConfig(6000, OverlayConfigDialogNew);
#endif

void OverlayConfig::initDisplayFps() {
	// set up FPS preview
	qgsFpsPreview.clear();
	qgsFpsPreview.setBackgroundBrush(qgvFpsPreview->backgroundBrush());

	qgpiFpsDemo = new QGraphicsPixmapItem();
	refreshFpsDemo();

	qgsFpsPreview.addItem(qgpiFpsDemo);
	qgpiFpsDemo->show();

	qgvFpsPreview->setScene(&qgsFpsPreview);
	qgvFpsPreview->centerOn(qgpiFpsDemo);

	qgpiFpsLive = new OverlayPositionableItem(&s.os.qrfFps, true);
	qgpiFpsLive->setZValue(-2.0f);
	refreshFpsLive();
}

void OverlayConfig::initDisplayClock() {
	qgpiTimeLive = new OverlayPositionableItem(&s.os.qrfTime, true);
	qgpiTimeLive->setZValue(-2.0f);
	refreshTimeLive();
}

void OverlayConfig::initDisplay() {
	// set up overlay preview
	qgpiScreen = new QGraphicsPixmapItem();
	qgpiScreen->setPixmap(qpScreen);
	qgpiScreen->setOpacity(0.5f);
	qgpiScreen->setZValue(-10.0f);

	initDisplayFps();
	initDisplayClock();

	qgtiInstructions = new QGraphicsTextItem();
	qgtiInstructions->setHtml(QString::fromLatin1("<ul><li>%1</li><li>%2</li><li>%3</li></ul>")
								  .arg(tr("To move the users, drag the little red dot."),
									   tr("To resize the users, mouse wheel over a user."),
									   tr("For more options, right click a user.")));
	qgtiInstructions->setOpacity(1.0f);
	qgtiInstructions->setZValue(-5.0f);
	qgtiInstructions->setDefaultTextColor(Qt::white);

	qgs.clear();
	qgs.setSceneRect(QRectF(0, 0, qgpiScreen->pixmap().width(), qgpiScreen->pixmap().height()));
	qgs.setBackgroundBrush(qgvView->backgroundBrush());

	qgs.addItem(qgpiScreen);
	qgpiScreen->show();

	qgs.addItem(qgpiFpsLive);
	qgpiFpsLive->show();

	qgs.addItem(qgpiTimeLive);
	qgpiTimeLive->show();

	oug                = new OverlayUserGroup(&s.os);
	oug->bShowExamples = true;
	qgs.addItem(oug);
	oug->show();

	qgs.addItem(qgtiInstructions);
	qgtiInstructions->show();

	qgvView->setScene(&qgs);

	qgvView->installEventFilter(this);
}

void OverlayConfig::refreshFpsDemo() {
	bpFpsDemo = OverlayTextLine(QString::fromLatin1("%1").arg(42), s.os.qfFps).createPixmap(s.os.qcFps);
	qgpiFpsDemo->setPixmap(bpFpsDemo);
	qgvFpsPreview->centerOn(qgpiFpsDemo);
}

void OverlayConfig::refreshFpsLive() {
	if (s.os.bFps) {
		qgpiFpsLive->setPixmap(bpFpsDemo.scaled(bpFpsDemo.size() * fViewScale));
		qgpiFpsLive->setOffset((-bpFpsDemo.qpBasePoint + QPoint(0, bpFpsDemo.iAscent)) * fViewScale);
	} else {
		qgpiFpsLive->setPixmap(QPixmap());
	}
	qgpiFpsLive->setItemVisible(s.os.bFps);
}

void OverlayConfig::refreshTimeLive() {
	if (s.os.bTime) {
		bpTimeDemo = OverlayTextLine(QString::fromLatin1("%1").arg(QTime::currentTime().toString()), s.os.qfFps)
						 .createPixmap(s.os.qcFps);
		qgpiTimeLive->setPixmap(bpTimeDemo.scaled(bpTimeDemo.size() * fViewScale));
		qgpiTimeLive->setOffset((-bpTimeDemo.qpBasePoint + QPoint(0, bpTimeDemo.iAscent)) * fViewScale);
	} else {
		qgpiTimeLive->setPixmap(QPixmap());
	}
	qgpiTimeLive->setItemVisible(s.os.bTime);
}

OverlayConfig::OverlayConfig(Settings &st)
	: ConfigWidget(st), qgpiScreen(nullptr), qgs(), qgsFpsPreview(), qgpiFpsDemo(nullptr), oug(nullptr),
	  qgtiInstructions(nullptr), fViewScale(1.0f) {
	setupUi(this);

	qlwPaths->setPathType(PathListWidget::FOLDER);

	qcbOverlayExclusionMode->insertItem(static_cast< int >(OverlaySettings::LauncherFilterExclusionMode),
										tr("Launcher Filter"));
	qcbOverlayExclusionMode->insertItem(static_cast< int >(OverlaySettings::WhitelistExclusionMode), tr("Whitelist"));
	qcbOverlayExclusionMode->insertItem(static_cast< int >(OverlaySettings::BlacklistExclusionMode), tr("Blacklist"));
	qcbOverlayExclusionMode->setCurrentIndex(static_cast< int >(OverlaySettings::LauncherFilterExclusionMode));

	if (!isInstalled()) {
		qswOverlayPage->setCurrentWidget(qwOverlayInstall);
	} else if (needsUpgrade()) {
		qswOverlayPage->setCurrentWidget(qwOverlayUpgrade);
	} else {
		qswOverlayPage->setCurrentWidget(qwOverlayConfig);
		qpbUninstall->setVisible(supportsInstallableOverlay());
	}

	// Grab a desktop screenshot as background
	QScreen *screen = qApp->primaryScreen();

	if (const QWindow *window = windowHandle()) {
		screen = window->screen();
	}

	// From Qt's documentation (Qt Widgets screenshot example):
	// "Although this is unlikely to happen, applications should check for null
	// pointers since there might be situations in which no screen is connected."
	if (screen) {
		qpScreen = screen->grabWindow(0);
	}

	if (qpScreen.size().isEmpty()) {
		qWarning() << __FUNCTION__ << "failed to grab screenshot, falling back.";

		if (screen) {
			const QRect desktopSize = screen->geometry();
			qpScreen                = QPixmap(desktopSize.width(), desktopSize.height());
		} else {
			qpScreen = QPixmap(1280, 720);
		}

		qpScreen.fill(Qt::darkGreen);
	}

	initDisplay();

	// Attach the upgrade button to the install click handler. Currently, the
	// actions they perform are the same. The distinction is only there to inform
	// users as to what's actually going on.
	connect(qpbUpgrade, SIGNAL(clicked()), this, SLOT(on_qpbInstall_clicked()));
}

void OverlayConfig::updateOverlayExclusionModeState() {
	OverlaySettings::OverlayExclusionMode exclusionMode =
		static_cast< OverlaySettings::OverlayExclusionMode >(qcbOverlayExclusionMode->currentIndex());

	switch (exclusionMode) {
		case OverlaySettings::LauncherFilterExclusionMode:
			qwLaunchers->setHidden(false);
			qwWhitelist->setHidden(false);
			qwPaths->setHidden(false);
			qwBlacklist->setHidden(false);

			// Show all whitelist items (including built-in items)
			// in launcher filter mode.
			for (int i = 0; i < qlwWhitelist->count(); i++) {
				QListWidgetItem *item = qlwWhitelist->item(i);
				item->setHidden(false);
			}

			break;
		case OverlaySettings::WhitelistExclusionMode:
			qwLaunchers->setHidden(true);
			qwWhitelist->setHidden(false);
			qwPaths->setHidden(true);
			qwBlacklist->setHidden(true);

			// Hide the built-in items when in WhitelistExclusionMode.
			// They are only considered in launcher filter mode.
			for (int i = 0; i < qlwWhitelist->count(); i++) {
				QListWidgetItem *item = qlwWhitelist->item(i);
				bool isBuiltin        = item->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool();
				if (isBuiltin) {
					item->setHidden(true);
				} else {
					item->setHidden(false);
				}
			}

			break;
		case OverlaySettings::BlacklistExclusionMode:
			qwLaunchers->setHidden(true);
			qwWhitelist->setHidden(true);
			qwPaths->setHidden(true);
			qwBlacklist->setHidden(false);
			break;
	}
}

void OverlayConfig::load(const Settings &r) {
	s.os = r.os;

	loadCheckBox(qcbEnable, s.os.bEnable);
	qcbShowFps->setChecked(s.os.bFps);
	qcbShowTime->setChecked(s.os.bTime);
	qgpFps->setEnabled(s.os.bEnable);

	qcbOverlayExclusionMode->setCurrentIndex(static_cast< int >(s.os.oemOverlayExcludeMode));

	// Launchers
	{
		qlwLaunchers->clear();

		QStringList builtinLaunchers;
#ifdef Q_OS_WIN
		int i = 0;
		while (overlayLaunchers[i]) {
			QString str = QLatin1String(overlayLaunchers[i]);
			builtinLaunchers << str;
			++i;
		}
#endif
		foreach (QString str, builtinLaunchers) {
			OverlayAppInfo oai               = OverlayAppInfo::applicationInfoForId(str);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwLaunchers);
			qlwiApplication->setData(OVERLAYCONFIG_PATH_ROLE, QVariant(str));
			qlwiApplication->setData(OVERLAYCONFIG_BUILTIN_ROLE, QVariant(true));

			// Use italic for 'default' items.
			QFont font = qlwiApplication->font();
			font.setItalic(true);
			qlwiApplication->setFont(font);

			qlwiApplication->setFlags(qlwiApplication->flags() | Qt::ItemIsUserCheckable);

			if (s.os.qslLaunchersExclude.contains(str)) {
				qlwiApplication->setCheckState(Qt::Unchecked);
			} else {
				qlwiApplication->setCheckState(Qt::Checked);
			}
		}

		foreach (QString str, s.os.qslLaunchers) {
			if (builtinLaunchers.contains(str)) {
				continue;
			}
			OverlayAppInfo oai               = OverlayAppInfo::applicationInfoForId(str);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwLaunchers);
			qlwiApplication->setData(OVERLAYCONFIG_PATH_ROLE, QVariant(str));
			qlwiApplication->setData(OVERLAYCONFIG_BUILTIN_ROLE, QVariant(false));
		}
	}

	// Whitelist
	{
		qlwWhitelist->clear();

		QStringList builtinWhitelist;
#ifdef Q_OS_WIN
		int i = 0;
		while (overlayWhitelist[i]) {
			QString str = QLatin1String(overlayWhitelist[i]);
			builtinWhitelist << str;
			++i;
		}
#endif
		foreach (QString str, builtinWhitelist) {
			OverlayAppInfo oai               = OverlayAppInfo::applicationInfoForId(str);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwWhitelist);
			qlwiApplication->setData(OVERLAYCONFIG_PATH_ROLE, QVariant(str));
			qlwiApplication->setData(OVERLAYCONFIG_BUILTIN_ROLE, QVariant(true));

			// Use italic for 'default' items.
			QFont font = qlwiApplication->font();
			font.setItalic(true);
			qlwiApplication->setFont(font);

			qlwiApplication->setFlags(qlwiApplication->flags() | Qt::ItemIsUserCheckable);

			if (s.os.qslWhitelistExclude.contains(str)) {
				qlwiApplication->setCheckState(Qt::Unchecked);
			} else {
				qlwiApplication->setCheckState(Qt::Checked);
			}
		}

		foreach (QString str, s.os.qslWhitelist) {
			if (builtinWhitelist.contains(str)) {
				continue;
			}
			OverlayAppInfo oai               = OverlayAppInfo::applicationInfoForId(str);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwWhitelist);
			qlwiApplication->setData(OVERLAYCONFIG_PATH_ROLE, QVariant(str));
			qlwiApplication->setData(OVERLAYCONFIG_BUILTIN_ROLE, QVariant(false));
		}
	}

	// Paths
	{
		qlwPaths->clear();

		foreach (QString str, s.os.qslPaths) {
			QListWidgetItem *qlwiApplication = new QListWidgetItem(QIcon(), str, qlwPaths);
			qlwiApplication->setData(OVERLAYCONFIG_PATH_ROLE, QVariant(str));
			qlwiApplication->setData(OVERLAYCONFIG_BUILTIN_ROLE, QVariant(false));
		}
	}

	// Blacklist
	{
		qlwBlacklist->clear();

		QStringList builtinBlacklist;
#ifdef Q_OS_WIN
		int i = 0;
		while (overlayBlacklist[i]) {
			QString str = QLatin1String(overlayBlacklist[i]);
			builtinBlacklist << str;
			++i;
		}
#endif
		foreach (QString str, builtinBlacklist) {
			OverlayAppInfo oai               = OverlayAppInfo::applicationInfoForId(str);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwBlacklist);
			qlwiApplication->setData(OVERLAYCONFIG_PATH_ROLE, QVariant(str));
			qlwiApplication->setData(OVERLAYCONFIG_BUILTIN_ROLE, QVariant(true));

			// Use italic for 'default' items.
			QFont font = qlwiApplication->font();
			font.setItalic(true);
			qlwiApplication->setFont(font);

			qlwiApplication->setFlags(qlwiApplication->flags() | Qt::ItemIsUserCheckable);

			if (s.os.qslBlacklistExclude.contains(str)) {
				qlwiApplication->setCheckState(Qt::Unchecked);
			} else {
				qlwiApplication->setCheckState(Qt::Checked);
			}
		}

		foreach (QString str, s.os.qslBlacklist) {
			if (builtinBlacklist.contains(str)) {
				continue;
			}
			OverlayAppInfo oai               = OverlayAppInfo::applicationInfoForId(str);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwBlacklist);
			qlwiApplication->setData(OVERLAYCONFIG_PATH_ROLE, QVariant(str));
			qlwiApplication->setData(OVERLAYCONFIG_BUILTIN_ROLE, QVariant(false));
		}
	}

	initDisplay();
	resizeScene(true);
	update();
}

QString OverlayConfig::title() const {
	return tr("Overlay");
}

const QString &OverlayConfig::getName() const {
	return OverlayConfig::name;
}

QIcon OverlayConfig::icon() const {
	return QIcon(QLatin1String("skin:config_osd.png"));
}

void OverlayConfig::save() const {
	s.os.bEnable = qcbEnable->isChecked();
	s.os.bFps    = qcbShowFps->isChecked();
	s.os.bTime   = qcbShowTime->isChecked();

	s.os.oemOverlayExcludeMode =
		static_cast< OverlaySettings::OverlayExclusionMode >(qcbOverlayExclusionMode->currentIndex());

	// Launchers
	{
		s.os.qslLaunchers.clear();
		s.os.qslLaunchersExclude.clear();

		for (int i = 0; i < qlwLaunchers->count(); i++) {
			bool isBuiltin = (qlwLaunchers->item(i)->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool());

			bool isChecked = false;
			if (isBuiltin) {
				isChecked = qlwLaunchers->item(i)->checkState() == Qt::Checked;
			}

			QString str = qlwLaunchers->item(i)->data(OVERLAYCONFIG_PATH_ROLE).toString();

			if (isBuiltin) {
				if (!isChecked) {
					s.os.qslLaunchersExclude << str;
				}
			} else {
				s.os.qslLaunchers << str;
			}
		}
	}

	// Whitelist
	{
		s.os.qslWhitelist.clear();
		s.os.qslWhitelistExclude.clear();

		for (int i = 0; i < qlwWhitelist->count(); i++) {
			bool isBuiltin = (qlwWhitelist->item(i)->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool());

			bool isChecked = false;
			if (isBuiltin) {
				isChecked = qlwWhitelist->item(i)->checkState() == Qt::Checked;
			}

			QString str = qlwWhitelist->item(i)->data(OVERLAYCONFIG_PATH_ROLE).toString();

			if (isBuiltin) {
				if (!isChecked) {
					s.os.qslWhitelistExclude << str;
				}
			} else {
				s.os.qslWhitelist << str;
			}
		}
	}

	// Paths
	{
		s.os.qslPaths.clear();
		s.os.qslPathsExclude.clear();

		for (int i = 0; i < qlwPaths->count(); i++) {
			bool isBuiltin = (qlwPaths->item(i)->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool());

			bool isChecked = false;
			if (isBuiltin) {
				isChecked = qlwPaths->item(i)->checkState() == Qt::Checked;
			}

			QString str = qlwPaths->item(i)->data(OVERLAYCONFIG_PATH_ROLE).toString();

			if (isBuiltin) {
				if (!isChecked) {
					s.os.qslPathsExclude << str;
				}
			} else {
				s.os.qslPaths << str;
			}
		}
	}

	// Blacklist
	{
		s.os.qslBlacklist.clear();
		s.os.qslBlacklistExclude.clear();

		for (int i = 0; i < qlwBlacklist->count(); i++) {
			bool isBuiltin = (qlwBlacklist->item(i)->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool());

			bool isChecked = false;
			if (isBuiltin) {
				isChecked = qlwBlacklist->item(i)->checkState() == Qt::Checked;
			}

			QString str = qlwBlacklist->item(i)->data(OVERLAYCONFIG_PATH_ROLE).toString();

			if (isBuiltin) {
				if (!isChecked) {
					s.os.qslBlacklistExclude << str;
				}
			} else {
				s.os.qslBlacklist << str;
			}
		}
	}
}

void OverlayConfig::accept() const {
	Global::get().o->forceSettings();
	Global::get().o->setActive(s.os.bEnable);
}

bool OverlayConfig::eventFilter(QObject *obj, QEvent *evt) {
	if (evt->type() == QEvent::Resize)
		QMetaObject::invokeMethod(this, "resizeScene", Qt::QueuedConnection);
	return ConfigWidget::eventFilter(obj, evt);
}

void OverlayConfig::resizeScene(bool force) {
	QSize sz = qgvView->viewport()->size();

	int ph = qgpiScreen->pixmap().height();
	int pw = qgpiScreen->pixmap().width();
	if (!force && ((ph == sz.height() && pw <= sz.width()) || (ph <= sz.height() && pw == sz.width()))) {
		return;
	}

	qgpiScreen->setPixmap(qpScreen.scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	qgs.setSceneRect(QRectF(0, 0, qgpiScreen->pixmap().width(), qgpiScreen->pixmap().height()));

	fViewScale = static_cast< float >(qgpiScreen->pixmap().height()) / static_cast< float >(qpScreen.height());
	refreshFpsLive();
	refreshTimeLive();

	QFont f = qgtiInstructions->font();
	f.setPointSizeF(qgs.sceneRect().height() / 20.0f);
	qgtiInstructions->setFont(f);

	qgtiInstructions->setPos(qgs.sceneRect().width() / 20.0f, qgs.sceneRect().height() / 10.0f);
	qgtiInstructions->setTextWidth(qgs.sceneRect().width() * 18.0f / 20.0f);

	qgvView->fitInView(qgs.sceneRect(), Qt::KeepAspectRatio);
	oug->updateLayout();
	oug->updateUsers();

	qgpiFpsLive->updateRender();
	qgpiTimeLive->updateRender();
}

void OverlayConfig::on_qlwLaunchers_itemSelectionChanged() {
	QList< QListWidgetItem * > items = qlwLaunchers->selectedItems();
	if (items.isEmpty()) {
		qpbLaunchersRemove->setEnabled(false);
		return;
	}

	QListWidgetItem *item = items.at(0);
	bool isBuiltin        = item->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool();

	if (isBuiltin) {
		qpbLaunchersRemove->setEnabled(false);
	} else {
		qpbLaunchersRemove->setEnabled(true);
	}
}

void OverlayConfig::on_qcbOverlayExclusionMode_currentIndexChanged(int) {
	updateOverlayExclusionModeState();
}

void OverlayConfig::on_qpbLaunchersAdd_clicked() {
#if defined(Q_OS_WIN)
	QString file = QFileDialog::getOpenFileName(this, tr("Choose executable"), QString(), QLatin1String("*.exe"));
#elif defined(Q_OS_MAC)
	QString file = QFileDialog::getOpenFileName(this, tr("Choose application"), QString(), QLatin1String("*.app"));
#else
	QString file = QString();
#endif

	if (!file.isEmpty()) {
		QString qsAppIdentifier = OverlayAppInfo::applicationIdentifierForPath(file);
		QStringList qslIdentifiers;
		for (int i = 0; i < qlwLaunchers->count(); i++)
			qslIdentifiers << qlwLaunchers->item(i)->data(Qt::UserRole).toString();
		if (!qslIdentifiers.contains(qsAppIdentifier)) {
			OverlayAppInfo oai               = OverlayAppInfo::applicationInfoForId(qsAppIdentifier);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwLaunchers);
			qlwiApplication->setData(Qt::UserRole, QVariant(qsAppIdentifier));
			qlwLaunchers->setCurrentItem(qlwiApplication);
		}
	}
}

void OverlayConfig::on_qpbLaunchersRemove_clicked() {
	int row = qlwLaunchers->currentRow();
	if (row != -1)
		delete qlwLaunchers->takeItem(row);
}

void OverlayConfig::on_qlwWhitelist_itemSelectionChanged() {
	QList< QListWidgetItem * > items = qlwWhitelist->selectedItems();
	if (items.isEmpty()) {
		qpbWhitelistRemove->setEnabled(false);
		return;
	}

	QListWidgetItem *item = items.at(0);
	bool isBuiltin        = item->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool();

	if (isBuiltin) {
		qpbWhitelistRemove->setEnabled(false);
	} else {
		qpbWhitelistRemove->setEnabled(true);
	}
}

void OverlayConfig::addWhitelistPath(const QString &path) {
	QString qsAppIdentifier = OverlayAppInfo::applicationIdentifierForPath(path);
	QListWidget *sel        = qlwWhitelist;
	QStringList qslIdentifiers;
	for (int i = 0; i < sel->count(); i++)
		qslIdentifiers << sel->item(i)->data(Qt::UserRole).toString();
	if (!qslIdentifiers.contains(qsAppIdentifier)) {
		OverlayAppInfo oai               = OverlayAppInfo::applicationInfoForId(qsAppIdentifier);
		QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, sel);
		qlwiApplication->setData(Qt::UserRole, QVariant(qsAppIdentifier));
		sel->setCurrentItem(qlwiApplication);
	}
}

void OverlayConfig::on_qpbWhitelistAdd_clicked() {
#if defined(Q_OS_WIN)
	QString file = QFileDialog::getOpenFileName(this, tr("Choose executable"), QString(), QLatin1String("*.exe"));
#elif defined(Q_OS_MAC)
	QString file = QFileDialog::getOpenFileName(this, tr("Choose application"), QString(), QLatin1String("*.app"));
#else
	QString file = QString();
#endif

	if (!file.isEmpty()) {
		addWhitelistPath(file);
	}
}

void OverlayConfig::on_qpbWhitelistRemove_clicked() {
	QListWidget *sel = qlwWhitelist;
	int row          = sel->currentRow();
	if (row != -1)
		delete sel->takeItem(row);
}

void OverlayConfig::on_qlwPaths_itemSelectionChanged() {
	QList< QListWidgetItem * > items = qlwPaths->selectedItems();
	if (items.isEmpty()) {
		qpbPathsRemove->setEnabled(false);
		return;
	}

	QListWidgetItem *item = items.at(0);
	bool isBuiltin        = item->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool();

	if (isBuiltin) {
		qpbPathsRemove->setEnabled(false);
	} else {
		qpbPathsRemove->setEnabled(true);
	}
}

void OverlayConfig::on_qpbPathsAdd_clicked() {
	QString existingPath = QFileDialog::getExistingDirectory(this, tr("Choose path"));
	QString dir          = QDir::toNativeSeparators(existingPath);

	QListWidget *sel = qlwPaths;
	QStringList qslIdentifiers;
	for (int i = 0; i < sel->count(); i++)
		qslIdentifiers << sel->item(i)->data(Qt::UserRole).toString();
	if (!dir.isEmpty() && !qslIdentifiers.contains(dir)) {
		QListWidgetItem *qlwiPath = new QListWidgetItem(QIcon(), dir, sel);
		qlwiPath->setData(Qt::UserRole, QVariant(dir));
		sel->setCurrentItem(qlwiPath);
	}
}

void OverlayConfig::on_qpbPathsRemove_clicked() {
	int row = qlwPaths->currentRow();
	if (row != -1)
		delete qlwPaths->takeItem(row);
}

void OverlayConfig::on_qlwBlacklist_itemSelectionChanged() {
	QList< QListWidgetItem * > items = qlwBlacklist->selectedItems();
	if (items.isEmpty()) {
		qpbBlacklistRemove->setEnabled(false);
		return;
	}

	QListWidgetItem *item = items.at(0);
	bool isBuiltin        = item->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool();

	if (isBuiltin) {
		qpbBlacklistRemove->setEnabled(false);
	} else {
		qpbBlacklistRemove->setEnabled(true);
	}
}

void OverlayConfig::on_qpbBlacklistAdd_clicked() {
#if defined(Q_OS_WIN)
	QString file = QFileDialog::getOpenFileName(this, tr("Choose executable"), QString(), QLatin1String("*.exe"));
#elif defined(Q_OS_MAC)
	QString file = QFileDialog::getOpenFileName(this, tr("Choose application"), QString(), QLatin1String("*.app"));
#else
	QString file = QString();
#endif

	if (!file.isEmpty()) {
		QString qsAppIdentifier = OverlayAppInfo::applicationIdentifierForPath(file);
		QListWidget *sel        = qlwBlacklist;
		QStringList qslIdentifiers;
		for (int i = 0; i < sel->count(); i++)
			qslIdentifiers << sel->item(i)->data(Qt::UserRole).toString();
		if (!qslIdentifiers.contains(qsAppIdentifier)) {
			OverlayAppInfo oai               = OverlayAppInfo::applicationInfoForId(qsAppIdentifier);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, sel);
			qlwiApplication->setData(Qt::UserRole, QVariant(qsAppIdentifier));
			sel->setCurrentItem(qlwiApplication);
		}
	}
}

void OverlayConfig::on_qpbBlacklistRemove_clicked() {
	QListWidget *sel = qlwBlacklist;
	int row          = sel->currentRow();
	if (row != -1)
		delete sel->takeItem(row);
}

void OverlayConfig::on_qcbEnable_stateChanged(int state) {
	qgpFps->setEnabled(state == Qt::Checked);
}

void OverlayConfig::on_qpbInstall_clicked() {
	qpbInstall->setEnabled(false);

	if (installFiles()) {
		qswOverlayPage->setCurrentWidget(qwOverlayConfig);
	}

	qpbInstall->setEnabled(true);
}

void OverlayConfig::on_qpbUninstall_clicked() {
	qpbUninstall->setEnabled(false);

	if (uninstallFiles()) {
		qswOverlayPage->setCurrentWidget(qwOverlayInstall);
	}

	qpbUninstall->setEnabled(true);
}

void OverlayConfig::on_qcbShowFps_stateChanged(int state) {
	Q_UNUSED(state);
	s.os.bFps = qcbShowFps->isChecked();
	refreshFpsLive();
}

void OverlayConfig::on_qcbShowTime_stateChanged(int state) {
	Q_UNUSED(state);
	s.os.bTime = qcbShowTime->isChecked();
	refreshTimeLive();
}

void OverlayConfig::on_qpbFpsFont_clicked() {
	bool ok;
	QFont new_font = QFontDialog::getFont(&ok, s.os.qfFps);

	if (ok) {
		s.os.qfFps = new_font;

		refreshFpsDemo();
		refreshFpsLive();
		refreshTimeLive();
	}
}

void OverlayConfig::on_qpbFpsColor_clicked() {
	QColor color = QColorDialog::getColor(s.os.qcFps);

	if (color.isValid()) {
		s.os.qcFps = color;

		refreshFpsDemo();
		refreshFpsLive();
		refreshTimeLive();
	}
}

void OverlayConfig::on_qpbLoadPreset_clicked() {
	QString fn = QFileDialog::getOpenFileName(this, tr("Load Overlay Presets"), QDir::homePath(),
											  tr("Mumble overlay presets (*.mumblelay)"));

	if (fn.isEmpty()) {
		return;
	}

	OverlaySettings load_preset;
	try {
		// First try regular format
		load_preset.load(fn);
	} catch (const std::exception &) {
		// If that fails, fall back to the legacy format
		QSettings qs(fn, QSettings::IniFormat);

		qs.beginGroup(QLatin1String("overlay"));
		load_preset.legacyLoad(&qs);
		qs.endGroup();
	}

#ifdef Q_OS_WIN
	load_preset.qslLaunchers        = s.os.qslLaunchers;
	load_preset.qslLaunchersExclude = s.os.qslLaunchersExclude;

	load_preset.qslWhitelist        = s.os.qslWhitelist;
	load_preset.qslWhitelistExclude = s.os.qslWhitelistExclude;

	load_preset.qslPaths        = s.os.qslPaths;
	load_preset.qslPathsExclude = s.os.qslPathsExclude;

	load_preset.qslBlacklist        = s.os.qslBlacklist;
	load_preset.qslBlacklistExclude = s.os.qslBlacklistExclude;
#endif
	load_preset.bEnable = s.os.bEnable;
	s.os                = load_preset;

	load(s);
}

void OverlayConfig::on_qpbSavePreset_clicked() {
	QString fn = QFileDialog::getSaveFileName(this, tr("Save Overlay Presets"), QDir::homePath(),
											  tr("Mumble overlay presets (*.mumblelay)"));

	if (fn.isEmpty()) {
		return;
	}

	s.os.savePresets(fn);
}
