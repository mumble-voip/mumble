// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "OverlayConfig.h"

#include "Overlay.h"
#include "OverlayUserGroup.h"
#include "OverlayPositionableItem.h"
#include "OverlayText.h"
#include "User.h"
#include "Channel.h"
#include "Global.h"
#include "Message.h"
#include "Database.h"
#include "NetworkConfig.h"
#include "ServerHandler.h"
#include "MainWindow.h"
#include "GlobalShortcut.h"

#ifdef Q_OS_WIN
#include "../../overlay/overlay_launchers.h"
#include "../../overlay/overlay_whitelist.h"
#include "../../overlay/overlay_blacklist.h"
#endif

static const int OVERLAYCONFIG_PATH_ROLE = Qt::UserRole;
static const int OVERLAYCONFIG_BUILTIN_ROLE = Qt::UserRole + 1;

// Hide overlay config for Mac OS X universal builds
#if !defined(USE_MAC_UNIVERSAL)
static ConfigWidget *OverlayConfigDialogNew(Settings &st) {
	return new OverlayConfig(st);
}

static ConfigRegistrar registrar(6000, OverlayConfigDialogNew);
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
	qgtiInstructions->setHtml(QString::fromLatin1("<ul><li>%1</li><li>%2</li><li>%3</li></ul>").arg(
	                              tr("To move the users, drag the little red dot."),
	                              tr("To resize the users, mouse wheel over a user."),
	                              tr("For more options, right click a user.")
	                          ));
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

	oug = new OverlayUserGroup(&s.os);
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
		bpTimeDemo = OverlayTextLine(QString::fromLatin1("%1").arg(QTime::currentTime().toString()), s.os.qfFps).createPixmap(s.os.qcFps);
		qgpiTimeLive->setPixmap(bpTimeDemo.scaled(bpTimeDemo.size() * fViewScale));
		qgpiTimeLive->setOffset((-bpTimeDemo.qpBasePoint + QPoint(0, bpTimeDemo.iAscent)) * fViewScale);
	} else {
		qgpiTimeLive->setPixmap(QPixmap());
	}
	qgpiTimeLive->setItemVisible(s.os.bTime);
}

OverlayConfig::OverlayConfig(Settings &st) :
		ConfigWidget(st),
		qgpiScreen(NULL),
		qgs(),
		qgsFpsPreview(),
		qgpiFpsDemo(NULL),
		oug(NULL),
		qgtiInstructions(NULL),
		fViewScale(1.0f) {
	setupUi(this);

	if (! isInstalled()) {
		qswOverlayPage->setCurrentWidget(qwOverlayInstall);
	} else if (needsUpgrade()) {
		qswOverlayPage->setCurrentWidget(qwOverlayUpgrade);
	} else {
		qswOverlayPage->setCurrentWidget(qwOverlayConfig);
		qpbUninstall->setVisible(supportsInstallableOverlay());
	}

	// grab a desktop screenshot as background
	QRect dsg = QApplication::desktop()->screenGeometry();

#if QT_VERSION > 0x050000
	qpScreen = QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId());
#else
	qpScreen = QPixmap::grabWindow(QApplication::desktop()->winId(), dsg.x(), dsg.y(), dsg.width(), dsg.height());
#endif
	if (qpScreen.size().isEmpty()) {
		qWarning() << __FUNCTION__ << "failed to grab desktop image, trying desktop widget...";

		qpScreen = QPixmap::grabWidget(QApplication::desktop(), dsg);

		if (qpScreen.size().isEmpty()) {
			qWarning() << __FUNCTION__ << "failed to grab desktop widget image, falling back.";

			QRect desktop_size = QApplication::desktop()->screenGeometry();
			qpScreen = QPixmap(desktop_size.width(), desktop_size.height());
			qpScreen.fill(Qt::darkGreen);
		}
	}

	initDisplay();

	// Attach the upgrade button to the install click handler. Currently, the
	// actions they perform are the same. The distinction is only there to inform
	// users as to what's actually going on.
	connect(qpbUpgrade, SIGNAL(clicked()), this, SLOT(on_qpbInstall_clicked()));
}

OverlayAppInfo OverlayConfig::applicationInfoForId(const QString &identifier) {
	QString qsAppName(identifier);
	QIcon qiAppIcon;
#if defined(Q_OS_MAC)
	CFStringRef bundleId = NULL;
	CFURLRef bundleUrl = NULL;
	CFBundleRef bundle = NULL;
	OSStatus err = noErr;
	char buf[4096];

	bundleId = CFStringCreateWithCharacters(kCFAllocatorDefault, reinterpret_cast<const UniChar *>(identifier.unicode()), identifier.length());
	err = LSFindApplicationForInfo(kLSUnknownCreator, bundleId, NULL, NULL, &bundleUrl);
	if (err == noErr) {
		// Figure out the bundle name of the application.
		CFStringRef absBundlePath = CFURLCopyFileSystemPath(bundleUrl, kCFURLPOSIXPathStyle);
		CFStringGetCString(absBundlePath, buf, 4096, kCFStringEncodingUTF8);
		QString qsBundlePath = QString::fromUtf8(buf);
		CFRelease(absBundlePath);
		qsAppName = QFileInfo(qsBundlePath).bundleName();

		// Load the bundle's icon.
		bundle = CFBundleCreate(NULL, bundleUrl);
		if (bundle) {
			CFDictionaryRef info = CFBundleGetInfoDictionary(bundle);
			if (info) {
				CFStringRef iconFileName = reinterpret_cast<CFStringRef>(CFDictionaryGetValue(info, CFSTR("CFBundleIconFile")));
				if (iconFileName) {
					CFStringGetCString(iconFileName, buf, 4096, kCFStringEncodingUTF8);
					QString qsIconPath = QString::fromLatin1("%1/Contents/Resources/%2")
					                     .arg(qsBundlePath, QString::fromUtf8(buf));
					if (! QFile::exists(qsIconPath))
						qsIconPath += QString::fromLatin1(".icns");
					if (QFile::exists(qsIconPath))
						qiAppIcon = QIcon(qsIconPath);
				}
			}
		}
	}

	if (bundleId)
		CFRelease(bundleId);
	if (bundleUrl)
		CFRelease(bundleUrl);
	if (bundle)
		CFRelease(bundle);

#elif defined(Q_OS_WIN)
	// qWinAppInst(), whose return value we used to pass
	// to ExtractIcon below, was removed in Qt 5.8.
	//
	// It was removed via
	// https://github.com/qt/qtbase/commit/64507c7165e42c2a5029353d8f97a0d841fa6b01
	//
	// In both Qt 4 and Qt 5, the qWinAppInst() implementation
	// simply calls GetModuleHandle(0).
	//
	// To sidestep the removal of the function, we simply
	// call through to GetModuleHandle() directly.
	HINSTANCE qWinAppInstValue = GetModuleHandle(NULL);
	HICON icon = ExtractIcon(qWinAppInstValue, identifier.toStdWString().c_str(), 0);
	if (icon) {
#if QT_VERSION >= 0x050000
		extern QPixmap qt_pixmapFromWinHICON(HICON icon);
		qiAppIcon = QIcon(qt_pixmapFromWinHICON(icon));
#else
		qiAppIcon = QIcon(QPixmap::fromWinHICON(icon));
#endif
		DestroyIcon(icon);
	}
#endif
	return OverlayAppInfo(qsAppName, qiAppIcon);
}

QString OverlayConfig::applicationIdentifierForPath(const QString &path) {
#ifdef Q_OS_MAC
	QString qsIdentifier;
	CFDictionaryRef plist = NULL;
	CFDataRef data = NULL;

	QFile qfAppBundle(QString::fromLatin1("%1/Contents/Info.plist").arg(path));
	if (qfAppBundle.exists()) {
		qfAppBundle.open(QIODevice::ReadOnly);
		QByteArray qbaPlistData = qfAppBundle.readAll();

		data = CFDataCreateWithBytesNoCopy(NULL, reinterpret_cast<UInt8 *>(qbaPlistData.data()), qbaPlistData.count(), kCFAllocatorNull);
		plist = static_cast<CFDictionaryRef>(CFPropertyListCreateFromXMLData(NULL, data, kCFPropertyListImmutable, NULL));
		if (plist) {
			CFStringRef ident = static_cast<CFStringRef>(CFDictionaryGetValue(plist, CFSTR("CFBundleIdentifier")));
			if (ident) {
				char buf[4096];
				CFStringGetCString(ident, buf, 4096, kCFStringEncodingUTF8);
				qsIdentifier = QString::fromUtf8(buf);
			}
		}
	}

	if (data)
		CFRelease(data);
	if (plist)
		CFRelease(plist);

	return qsIdentifier;
#else
	return QDir::toNativeSeparators(path);
#endif
}

void OverlayConfig::load(const Settings &r) {
	s.os = r.os;

	loadCheckBox(qcbEnable, s.os.bEnable);
	qcbShowFps->setChecked(s.os.bFps);
	qcbShowTime->setChecked(s.os.bTime);
	qgpFps->setEnabled(s.os.bEnable);

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
			OverlayAppInfo oai = applicationInfoForId(str);
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

		foreach(QString str, s.os.qslLaunchers) {
			if (builtinLaunchers.contains(str)) {
				continue;
			}
			OverlayAppInfo oai = applicationInfoForId(str);
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
			OverlayAppInfo oai = applicationInfoForId(str);
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

		foreach(QString str, s.os.qslWhitelist) {
			if (builtinWhitelist.contains(str)) {
				continue;
			}
			OverlayAppInfo oai = applicationInfoForId(str);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwWhitelist);
			qlwiApplication->setData(OVERLAYCONFIG_PATH_ROLE, QVariant(str));
			qlwiApplication->setData(OVERLAYCONFIG_BUILTIN_ROLE, QVariant(false));
		}
	}

	// Paths
	{
		qlwPaths->clear();

		foreach(QString str, s.os.qslPaths) {
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
			OverlayAppInfo oai = applicationInfoForId(str);
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

		foreach(QString str, s.os.qslBlacklist) {
			if (builtinBlacklist.contains(str)) {
				continue;
			}
			OverlayAppInfo oai = applicationInfoForId(str);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwBlacklist);
			qlwiApplication->setData(OVERLAYCONFIG_PATH_ROLE, QVariant(str));
			qlwiApplication->setData(OVERLAYCONFIG_BUILTIN_ROLE, QVariant(false));
		}
	}

	initDisplay();
	resizeScene(true);
	update();
}

bool OverlayConfig::expert(bool show_expert) {
	int idx = qtwSetup->indexOf(qwExceptions);

#ifdef Q_OS_LINUX
	Q_UNUSED(show_expert);
	qtwSetup->setTabEnabled(idx, false);
#else
	qtwSetup->setTabEnabled(idx, show_expert);
#endif
	return true;
}

QString OverlayConfig::title() const {
	return tr("Overlay");
}

QIcon OverlayConfig::icon() const {
	return QIcon(QLatin1String("skin:config_osd.png"));
}

void OverlayConfig::save() const {
	s.os.bEnable = qcbEnable->isChecked();
	s.os.bFps = qcbShowFps->isChecked();
	s.os.bTime = qcbShowTime->isChecked();

	// Directly save overlay config

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

	g.qs->beginGroup(QLatin1String("overlay"));
	s.os.save();
	g.qs->endGroup();
#ifdef Q_OS_WIN
	// On MS windows force sync so the registry is updated.
	g.qs->sync();
#endif
}

void OverlayConfig::accept() const {
	g.o->forceSettings();
	g.o->setActive(s.os.bEnable);
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

	fViewScale = static_cast<float>(qgpiScreen->pixmap().height()) / static_cast<float>(qpScreen.height());
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
	QList<QListWidgetItem *> items = qlwLaunchers->selectedItems();
	if (items.isEmpty()) {
		qpbLaunchersRemove->setEnabled(false);
		return;
	}

	QListWidgetItem *item = items.at(0);
	bool isBuiltin = item->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool();

	if (isBuiltin) {
		qpbLaunchersRemove->setEnabled(false);
	} else {
		qpbLaunchersRemove->setEnabled(true);
	}
}

void OverlayConfig::on_qpbLaunchersAdd_clicked() {
#if defined(Q_OS_WIN)
	QString file = QFileDialog::getOpenFileName(this, tr("Choose executable"), QString(), QLatin1String("*.exe"));
#elif defined(Q_OS_MAC)
	QString file = QFileDialog::getOpenFileName(this, tr("Choose application"), QString(), QLatin1String("*.app"));
#else
	QString file = QString();
#endif

	if (! file.isEmpty()) {
		QString qsAppIdentifier = applicationIdentifierForPath(file);
		QStringList qslIdentifiers;
		for (int i = 0; i < qlwLaunchers->count(); i++)
			qslIdentifiers << qlwLaunchers->item(i)->data(Qt::UserRole).toString();
		if (! qslIdentifiers.contains(qsAppIdentifier)) {
			OverlayAppInfo oai = applicationInfoForId(qsAppIdentifier);
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
	QList<QListWidgetItem *> items = qlwWhitelist->selectedItems();
	if (items.isEmpty()) {
		qpbWhitelistRemove->setEnabled(false);
		return;
	}

	QListWidgetItem *item = items.at(0);
	bool isBuiltin = item->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool();

	if (isBuiltin) {
		qpbWhitelistRemove->setEnabled(false);
	} else {
		qpbWhitelistRemove->setEnabled(true);
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

	if (! file.isEmpty()) {
		QString qsAppIdentifier = applicationIdentifierForPath(file);
		QListWidget *sel = qlwWhitelist;
		QStringList qslIdentifiers;
		for (int i = 0; i < sel->count(); i++)
			qslIdentifiers << sel->item(i)->data(Qt::UserRole).toString();
		if (! qslIdentifiers.contains(qsAppIdentifier)) {
			OverlayAppInfo oai = applicationInfoForId(qsAppIdentifier);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, sel);
			qlwiApplication->setData(Qt::UserRole, QVariant(qsAppIdentifier));
			sel->setCurrentItem(qlwiApplication);
		}
	}
}

void OverlayConfig::on_qpbWhitelistRemove_clicked() {
	QListWidget *sel = qlwWhitelist;
	int row = sel->currentRow();
	if (row != -1)
		delete sel->takeItem(row);
}

void OverlayConfig::on_qlwPaths_itemSelectionChanged() {
	QList<QListWidgetItem *> items = qlwPaths->selectedItems();
	if (items.isEmpty()) {
		qpbPathsRemove->setEnabled(false);
		return;
	}

	QListWidgetItem *item = items.at(0);
	bool isBuiltin = item->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool();

	if (isBuiltin) {
		qpbPathsRemove->setEnabled(false);
	} else {
		qpbPathsRemove->setEnabled(true);
	}
}

void OverlayConfig::on_qpbPathsAdd_clicked() {
}

void OverlayConfig::on_qpbPathsRemove_clicked() {
}

void OverlayConfig::on_qlwBlacklist_itemSelectionChanged() {
	QList<QListWidgetItem *> items = qlwBlacklist->selectedItems();
	if (items.isEmpty()) {
		qpbBlacklistRemove->setEnabled(false);
		return;
	}

	QListWidgetItem *item = items.at(0);
	bool isBuiltin = item->data(OVERLAYCONFIG_BUILTIN_ROLE).toBool();

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

	if (! file.isEmpty()) {
		QString qsAppIdentifier = applicationIdentifierForPath(file);
		QListWidget *sel = qlwBlacklist;
		QStringList qslIdentifiers;
		for (int i = 0; i < sel->count(); i++)
			qslIdentifiers << sel->item(i)->data(Qt::UserRole).toString();
		if (! qslIdentifiers.contains(qsAppIdentifier)) {
			OverlayAppInfo oai = applicationInfoForId(qsAppIdentifier);
			QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, sel);
			qlwiApplication->setData(Qt::UserRole, QVariant(qsAppIdentifier));
			sel->setCurrentItem(qlwiApplication);
		}
	}
}

void OverlayConfig::on_qpbBlacklistRemove_clicked() {
	QListWidget *sel = qlwBlacklist;
	int row = sel->currentRow();
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
	QString fn = QFileDialog::getOpenFileName(this,
	             tr("Load Overlay Presets"),
	             QDir::homePath(),
	             tr("Mumble overlay presets (*.mumblelay)"));

	if (fn.isEmpty()) {
		return;
	}

	QSettings qs(fn, QSettings::IniFormat);
	OverlaySettings load_preset;

	qs.beginGroup(QLatin1String("overlay"));
	load_preset.load(&qs);
	qs.endGroup();

#ifdef Q_OS_WIN
	load_preset.qslLaunchers = s.os.qslLaunchers;
	load_preset.qslLaunchersExclude = s.os.qslLaunchersExclude;

	load_preset.qslWhitelist = s.os.qslWhitelist;
	load_preset.qslWhitelistExclude = s.os.qslWhitelistExclude;

	load_preset.qslPaths = s.os.qslPaths;
	load_preset.qslPathsExclude = s.os.qslPathsExclude;

	load_preset.qslBlacklist = s.os.qslBlacklist;
	load_preset.qslBlacklistExclude = s.os.qslBlacklistExclude;
#endif
	load_preset.bEnable = s.os.bEnable;
	s.os = load_preset;

	load(s);
}

void OverlayConfig::on_qpbSavePreset_clicked() {
	QString fn = QFileDialog::getSaveFileName(this,
	             tr("Save Overlay Presets"),
	             QDir::homePath(),
	             tr("Mumble overlay presets (*.mumblelay)"));

	if (fn.isEmpty()) {
		return;
	}

	QSettings qs(fn, QSettings::IniFormat);

	if (!qs.isWritable()) {
		qWarning() << __FUNCTION__ << "preset save file" << fn << "is not writable!";
		return;
	}

	qs.beginGroup(QLatin1String("overlay"));
	s.os.save(&qs);
	qs.remove(QLatin1String("enable"));
	qs.remove(QLatin1String("usewhitelist"));
	qs.remove(QLatin1String("blacklist"));
	qs.remove(QLatin1String("whitelist"));
	qs.remove(QLatin1String("enablelauncherfilter"));
	qs.remove(QLatin1String("launchers"));
	qs.endGroup();
}
