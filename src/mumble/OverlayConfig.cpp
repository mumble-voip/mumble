/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "Overlay.h"
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

static ConfigWidget *OverlayConfigDialogNew(Settings &st) {
	return new OverlayConfig(st);
}

// Hide overlay config for Mac OS X universal builds
#if !(defined(Q_OS_MAC) && !defined(QT_MAC_USE_COCOA))
static ConfigRegistrar registrar(6000, OverlayConfigDialogNew);
#endif

void OverlayConfig::initDisplay() {
	// set up FPS preview
	qgsFpsPreview.clear();
	qgsFpsPreview.setBackgroundBrush(qgvFpsPreview->backgroundBrush());

	qgpiFpsDemo = new QGraphicsPixmapItem();
	refreshFpsDemo();

	qgsFpsPreview.addItem(qgpiFpsDemo);
	qgpiFpsDemo->show();

	qgvFpsPreview->setScene(&qgsFpsPreview);
	qgvFpsPreview->centerOn(qgpiFpsDemo);

	// set up overlay preview
	qgpiScreen = new QGraphicsPixmapItem();
	qgpiScreen->setPixmap(qpScreen);
	qgpiScreen->setOpacity(0.5f);
	qgpiScreen->setZValue(-10.0f);

	qgpiFpsLive = new QGraphicsPixmapItem();
	qgpiFpsLive->setZValue(-2.0f);
	refreshFpsLive();

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
	bpFpsDemo = OverlayTextLine(tr("FPS: %1").arg(42), s.os.qfFps).createPixmap(s.os.qcFps);
	qgpiFpsDemo->setPixmap(bpFpsDemo);
	qgvFpsPreview->centerOn(qgpiFpsDemo);
}

void OverlayConfig::refreshFpsLive() {
	if (s.os.bFps) {
		qgpiFpsLive->setPos(s.os.qrfFps.topLeft() * fViewScale);
		qgpiFpsLive->setPixmap(bpFpsDemo.scaled(bpFpsDemo.size() * fViewScale));
		qgpiFpsLive->setOffset((-bpFpsDemo.qpBasePoint + QPoint(0, bpFpsDemo.iAscent)) * fViewScale);
	} else {
		qgpiFpsLive->setPixmap(QPixmap());
	}
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
	}
	on_qswOverlayPage_currentChanged(qswOverlayPage->currentIndex());

	// grab a desktop screenshot as background
	QRect dsg = QApplication::desktop()->screenGeometry();
	qpScreen = QPixmap::grabWindow(QApplication::desktop()->winId(), dsg.x(), dsg.y(), dsg.width(), dsg.height());
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
	connect(qpbUpgradeShowCerts, SIGNAL(clicked()), this, SLOT(on_qpbShowCerts_clicked()));
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
	HICON icon = ExtractIcon(qWinAppInst(), identifier.utf16(), 0);
	if (icon) {
		qiAppIcon = QIcon(QPixmap::fromWinHICON(icon));
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

	QFile qfAppBundle(QString("%1/Contents/Info.plist").arg(path));
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
	qgpFps->setEnabled(s.os.bEnable);

	qlwBlacklist->clear();
	qlwWhitelist->clear();

	qrbWhitelist->setChecked(s.os.bUseWhitelist);
	qswBlackWhiteList->setCurrentWidget(s.os.bUseWhitelist ? qwWhite : qwBlack);

	foreach(QString str, s.os.qslWhitelist) {
		OverlayAppInfo oai = applicationInfoForId(str);
		QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwWhitelist);
		qlwiApplication->setData(Qt::UserRole, QVariant(str));
	}

	foreach(QString str, s.os.qslBlacklist) {
		OverlayAppInfo oai = applicationInfoForId(str);
		QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, qlwBlacklist);
		qlwiApplication->setData(Qt::UserRole, QVariant(str));
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

	// Directly save overlay config
	s.os.qslBlacklist.clear();
	for (int i=0;i<qlwBlacklist->count();++i) {
		QVariant qvUserData = qlwBlacklist->item(i)->data(Qt::UserRole);
		s.os.qslBlacklist << qvUserData.toString();
	}

	s.os.qslWhitelist.clear();
	for (int i=0;i<qlwWhitelist->count();++i) {
		QVariant qvUserData = qlwWhitelist->item(i)->data(Qt::UserRole);
		s.os.qslWhitelist << qvUserData.toString();
	}

	s.os.bUseWhitelist = qrbWhitelist->isChecked();

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

	QFont f = qgtiInstructions->font();
	f.setPointSizeF(qgs.sceneRect().height() / 20.0f);
	qgtiInstructions->setFont(f);

	qgtiInstructions->setPos(qgs.sceneRect().width() / 20.0f, qgs.sceneRect().height() / 10.0f);
	qgtiInstructions->setTextWidth(qgs.sceneRect().width() * 18.0f / 20.0f);

	qgvView->fitInView(qgs.sceneRect(), Qt::KeepAspectRatio);
	oug->updateLayout();
	oug->updateUsers();
}

void OverlayConfig::on_qpbAdd_clicked() {
#if defined(Q_OS_WIN)
	QString file = QFileDialog::getOpenFileName(this, tr("Choose executable"), QString(), QLatin1String("*.exe"));
#elif defined(Q_OS_MAC)
	QString file = QFileDialog::getOpenFileName(this, tr("Choose application"), QString(), QLatin1String("*.app"));
#else
	QString file = QString();
#endif

	if (! file.isEmpty()) {
		QString qsAppIdentifier = applicationIdentifierForPath(file);
		QListWidget *sel = qrbBlacklist->isChecked() ? qlwBlacklist : qlwWhitelist;
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

void OverlayConfig::on_qpbRemove_clicked() {
	QListWidget *sel = qrbBlacklist->isChecked() ? qlwBlacklist : qlwWhitelist;
	int row = sel->currentRow();
	if (row != -1)
		delete sel->takeItem(row);
}

void OverlayConfig::on_qrbBlacklist_toggled(bool checked) {
	qswBlackWhiteList->setCurrentWidget(checked ? qwBlack : qwWhite);
}

void OverlayConfig::on_qcbEnable_stateChanged(int state) {
	qgpFps->setEnabled(state == Qt::Checked);
}

void OverlayConfig::on_qswOverlayPage_currentChanged(int) {
	QLatin1String qsStyleSheetInvalid("QLabel { color: red; }");
	QLatin1String qsStyleSheetValid("QLabel { color: green; }");
	QString qsValidInstaller = tr("Mumble has deemed the installer valid.");
	QString qsInvalidInstaller = tr("Mumble was unable to verify the authenticity of the installer.");

	if (qswOverlayPage->currentWidget() == qwOverlayInstall) {
		qpbShowCerts->setVisible(supportsCertificates());
		qlInstallValidityText->setVisible(supportsCertificates());
		if (! installerIsValid()) {
			qlInstallValidityText->setStyleSheet(qsStyleSheetInvalid);
			qlInstallValidityText->setText(QString::fromLatin1("<p>%1</p>").arg(qsInvalidInstaller));
			qpbInstall->setEnabled(false);
		} else {
			qlInstallValidityText->setStyleSheet(qsStyleSheetValid);
			qlInstallValidityText->setText(QString::fromLatin1("<p>%1</p>").arg(qsValidInstaller));
			qpbInstall->setEnabled(true);
		}
	} else if (qswOverlayPage->currentWidget() == qwOverlayUpgrade) {
		qpbUpgradeShowCerts->setVisible(supportsCertificates());
		qlUpgradeValidityText->setVisible(supportsCertificates());
		if (! installerIsValid()) {
			qlUpgradeValidityText->setStyleSheet(qsStyleSheetInvalid);
			qlUpgradeValidityText->setText(QString::fromLatin1("<p>%1</p>").arg(qsInvalidInstaller));
			qpbUpgrade->setEnabled(false);
		} else {
			qlUpgradeValidityText->setStyleSheet(qsStyleSheetValid);
			qlUpgradeValidityText->setText(QString::fromLatin1("<p>%1</p>").arg(qsValidInstaller));
			qpbUpgrade->setEnabled(true);
		}
	} else if (qswOverlayPage->currentWidget() == qwOverlayConfig) {
		qpbUninstall->setVisible(supportsInstallableOverlay());
	}
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

void OverlayConfig::on_qpbShowCerts_clicked() {
	showCertificates();
}

void OverlayConfig::on_qcbShowFps_stateChanged(int state) {
	Q_UNUSED(state);
	s.os.bFps = qcbShowFps->isChecked();
	refreshFpsLive();
}

void OverlayConfig::on_qpbFpsFont_clicked() {
	bool ok;
	QFont new_font = QFontDialog::getFont(&ok, s.os.qfFps);

	if (ok) {
		s.os.qfFps = new_font;

		refreshFpsDemo();
		refreshFpsLive();
	}
}

void OverlayConfig::on_qpbFpsColor_clicked() {
	QColor color = QColorDialog::getColor(s.os.qcFps);

	if (color.isValid()) {
		s.os.qcFps = color;

		refreshFpsDemo();
		refreshFpsLive();
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
	load_preset.qslBlacklist = s.os.qslBlacklist;
	load_preset.qslWhitelist = s.os.qslWhitelist;
#endif
	load_preset.bUseWhitelist = s.os.bUseWhitelist;
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
	qs.endGroup();
}
