// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OverlayConfig.h"
#include "OverlayClient.h"
#include "MainWindow.h"
#include "Global.h"
#include "Version.h"

#include <QtCore/QProcess>
#include <QtCore/QXmlStreamReader>

#import <ScriptingBridge/ScriptingBridge.h>
#import <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>

extern "C" {
#include <xar/xar.h>
}

// Ignore deprecation warnings for the whole file, for now.
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static NSString *MumbleOverlayLoaderBundle = @"/Library/ScriptingAdditions/MumbleOverlay.osax";
static NSString *MumbleOverlayLoaderBundleIdentifier = @"net.sourceforge.mumble.OverlayScriptingAddition";

pid_t getForegroundProcessId() {
	NSRunningApplication *app = [[NSWorkspace sharedWorkspace] frontmostApplication];
	if (app) {
		return [app processIdentifier];
	}

	return 0;
}

@interface OverlayInjectorMac : NSObject {
	BOOL active;
}
- (id) init;
- (void) dealloc;
- (void) appLaunched:(NSNotification *)notification;
- (void) setActive:(BOOL)flag;
- (void) eventDidFail:(const AppleEvent *)event withError:(NSError *)error;
@end

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
@interface OverlayInjectorMac () <SBApplicationDelegate>
@end
#endif

@implementation OverlayInjectorMac

- (id) init {
	self = [super init];

	if (self) {
		active = NO;
		NSWorkspace *workspace = [NSWorkspace sharedWorkspace];
		[[workspace notificationCenter] addObserver:self
		                                selector:@selector(appLaunched:)
		                                name:NSWorkspaceDidLaunchApplicationNotification
		                                object:workspace];
		return self;
	}

	return nil;
}

- (void) dealloc {
	NSWorkspace *workspace = [NSWorkspace sharedWorkspace];
	[[workspace notificationCenter] removeObserver:self
		                                name:NSWorkspaceDidLaunchApplicationNotification
		                                object:workspace];

	[super dealloc];
}

- (void) appLaunched:(NSNotification *)notification {
	if (active) {
		BOOL overlayEnabled = NO;
		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
		NSDictionary *userInfo = [notification userInfo];

		NSString *bundleId = [userInfo objectForKey:@"NSApplicationBundleIdentifier"];
		if ([bundleId isEqualToString:[[NSBundle mainBundle] bundleIdentifier]])
			return;

		QString qsBundleIdentifier = QString::fromUtf8([bundleId UTF8String]);

		switch (Global::get().s.os.oemOverlayExcludeMode) {
			case OverlaySettings::LauncherFilterExclusionMode: {
				qWarning("Overlay_macx: launcher filter mode not implemented on macOS, allowing everything");
				overlayEnabled = YES;
				break;
			}
			case OverlaySettings::WhitelistExclusionMode: {
				if (Global::get().s.os.qslWhitelist.contains(qsBundleIdentifier)) {
					overlayEnabled = YES;
				}
				break;
			}
			case OverlaySettings::BlacklistExclusionMode: {
				if (! Global::get().s.os.qslBlacklist.contains(qsBundleIdentifier)) {
					overlayEnabled = YES;
				}
				break;
			}
		}

		if (overlayEnabled) {
			pid_t pid = [[userInfo objectForKey:@"NSApplicationProcessIdentifier"] intValue];
			SBApplication *app = [SBApplication applicationWithProcessIdentifier:pid];
			[app setDelegate:self];

			// This timeout is specified in 'ticks'.
			// A tick defined as: "[...] (a tick is approximately 1/60 of a second) [...]" in the
			// Apple Event Manager Reference documentation:
			// http://developer.apple.com/legacy/mac/library/documentation/Carbon/reference/Event_Manager/Event_Manager.pdf
			[app setTimeout:10*60];

			[app setSendMode:kAEWaitReply];
			[app sendEvent:kASAppleScriptSuite id:kGetAEUT parameters:0];

			[app setSendMode:kAENoReply];
		}

		[pool release];
	}
}

- (void) setActive:(BOOL)flag {
	active = flag;
}

// SBApplication delegate method
- (void)eventDidFail:(const AppleEvent *)event withError:(NSError *)error {
	Q_UNUSED(event);
	Q_UNUSED(error);

	// Do nothing. This method is only here to avoid an exception.
}

@end

class OverlayPrivateMac : public OverlayPrivate {
	protected:
		OverlayInjectorMac *olm;
	public:
		void setActive(bool);
		OverlayPrivateMac(QObject *);
		~OverlayPrivateMac();
};

OverlayPrivateMac::OverlayPrivateMac(QObject *p) : OverlayPrivate(p) {
	olm = [[OverlayInjectorMac alloc] init];
}

OverlayPrivateMac::~OverlayPrivateMac() {
	[olm release];
}

void OverlayPrivateMac::setActive(bool act) {
	[olm setActive:act];
}

void Overlay::platformInit() {
	d = new OverlayPrivateMac(this);
}

void Overlay::setActiveInternal(bool act) {
	if (d) {
		/// Only act if the private instance has been created already
		static_cast<OverlayPrivateMac *>(d)->setActive(act);
	}
}

bool OverlayConfig::supportsInstallableOverlay() {
	return true;
}

void OverlayClient::updateMouse() {
	QCursor c = qgv.viewport()->cursor();
	NSCursor *cursor = nil;
	Qt::CursorShape csShape = c.shape();

	switch (csShape) {
		case Qt::IBeamCursor:        cursor = [NSCursor IBeamCursor]; break;
		case Qt::CrossCursor:        cursor = [NSCursor crosshairCursor]; break;
		case Qt::ClosedHandCursor:   cursor = [NSCursor closedHandCursor]; break;
		case Qt::OpenHandCursor:     cursor = [NSCursor openHandCursor]; break;
		case Qt::PointingHandCursor: cursor = [NSCursor pointingHandCursor]; break;
		case Qt::SizeVerCursor:      cursor = [NSCursor resizeUpDownCursor]; break;
		case Qt::SplitVCursor:       cursor = [NSCursor resizeUpDownCursor]; break;
		case Qt::SizeHorCursor:      cursor = [NSCursor resizeLeftRightCursor]; break;
		case Qt::SplitHCursor:       cursor = [NSCursor resizeLeftRightCursor]; break;
		default:                     cursor = [NSCursor arrowCursor]; break;
	}

	QPixmap pm = qmCursors.value(csShape);

	NSPoint p = [cursor hotSpot];
	iOffsetX = (int) p.x;
	iOffsetY = (int) p.y;

	qgpiCursor->setPixmap(pm);
	qgpiCursor->setPos(iMouseX - iOffsetX, iMouseY - iOffsetY);
}

QString installerPath() {
	NSString *installerPath = [[NSBundle mainBundle] pathForResource:@"MumbleOverlay" ofType:@"pkg"];
	if (installerPath) {
		return QString::fromUtf8([installerPath UTF8String]);
	}
	return QString();
}

bool OverlayConfig::isInstalled() {
	bool ret = false;

	// Determine if the installed bundle is correctly installed (i.e. it's loadable)
	NSBundle *bundle = [NSBundle bundleWithPath:MumbleOverlayLoaderBundle];
	ret = [bundle preflightAndReturnError:nullptr];

	// Do the bundle identifiers match?
	if (ret) {
		ret = [[bundle bundleIdentifier] isEqualToString:MumbleOverlayLoaderBundleIdentifier];
	}

	return ret;
}

// Check whether this installer installs something 'newer' than what we already have.
// Also checks whether the new installer is compatible with the current version of
// Mumble.
static bool isInstallerNewer(QString path, NSUInteger curVer) {
	xar_t pkg = nullptr;
	xar_iter_t iter = nullptr;
	xar_file_t file = nullptr;
	char *data = nullptr;
	size_t size = 0;
	bool ret = false;
	QString qsMinVer, qsOverlayVer;

	pkg = xar_open(path.toUtf8().constData(), READ);
	if (!pkg) {
		qWarning("isInstallerNewer: Unable to open pkg.");
		goto out;
	}

	iter = xar_iter_new();
	if (!iter) {
		qWarning("isInstallerNewer: Unable to allocate iter");
		goto out;
	}

	file = xar_file_first(pkg, iter);
	while (file) {
		if (!strcmp(xar_get_path(file), "upgrade.xml"))
			break;
		file = xar_file_next(iter);
	}

	if (file) {
		if (xar_extract_tobuffersz(pkg, file, &data, &size) == -1) {
			goto out;
		}

		QXmlStreamReader reader(QByteArray::fromRawData(data, static_cast<int>(size)));
		while (! reader.atEnd()) {
			QXmlStreamReader::TokenType tok = reader.readNext();
			if (tok == QXmlStreamReader::StartElement) {
				if (reader.name() == QLatin1String("upgrade")) {
					qsOverlayVer = reader.attributes().value(QLatin1String("version")).toString();
					qsMinVer = reader.attributes().value(QLatin1String("minclient")).toString();
				}
			}
		}

		if (reader.hasError() || qsMinVer.isNull() || qsOverlayVer.isNull()) {
			qWarning("isInstallerNewer: Error while parsing XML version info.");
			goto out;
		}

		NSUInteger newVer = qsOverlayVer.toUInt();

		Version::component_t major, minor, patch;
		if (!Version::getComponents(major, minor, patch, QLatin1String(MUMTEXT(MUMBLE_VERSION)))) {
			goto out;
		}

		Version::component_t minmajor, minminor, minpatch;
		if (!Version::getComponents(minmajor, minminor, minpatch, qsMinVer)) {
			goto out;
		}

		ret = (major >= minmajor) && (minor >= minminor) && (patch >= minpatch) && (newVer > curVer);
	}

out:
	xar_close(pkg);
	xar_iter_free(iter);
	free(data);
	return ret;
}

bool OverlayConfig::needsUpgrade() {
	NSDictionary *infoPlist = [NSDictionary dictionaryWithContentsOfFile:[NSString stringWithFormat:@"%@/Contents/Info.plist", MumbleOverlayLoaderBundle]];
	if (infoPlist) {
		NSUInteger curVersion = [[infoPlist objectForKey:@"MumbleOverlayVersion"] unsignedIntegerValue];

		QString path = installerPath();
		if (path.isEmpty())
			return false;

		return isInstallerNewer(path, curVersion);
	}

	return false;
}

static bool authExec(AuthorizationRef ref, const char **argv) {
	OSStatus err = noErr;
	int pid = 0, status = 0;

	err = AuthorizationExecuteWithPrivileges(ref, argv[0], kAuthorizationFlagDefaults, const_cast<char * const *>(&argv[1]), nullptr);
	if (err == errAuthorizationSuccess) {
		do {
			pid = wait(&status);
		} while (pid == -1 && errno == EINTR);
		return (pid != -1 && WIFEXITED(status) && WEXITSTATUS(status) == 0);
	}

	qWarning("Overlay_macx: Failed to AuthorizeExecuteWithPrivileges. (err=%i)", err);
	qWarning("Overlay_macx: Status: (pid=%i, exited=%u, exitStatus=%u)", pid, WIFEXITED(status), WEXITSTATUS(status));

	return false;
}

bool OverlayConfig::installFiles() {
	bool ret = false;

	QString path = installerPath();
	if (path.isEmpty()) {
		qWarning("OverlayConfig: No installers found in search paths.");
		return false;
	}

	QProcess installer(this);
	QStringList args;
	args << QString::fromLatin1("-W");
	args << path;
	installer.start(QLatin1String("/usr/bin/open"), args, QIODevice::ReadOnly);

	while (!installer.waitForFinished(1000)) {
		qApp->processEvents();
	}

	return ret;
}

bool OverlayConfig::uninstallFiles() {
	AuthorizationRef auth;
	NSBundle *loaderBundle;
	bool ret = false, bundleOk = false;
	OSStatus err;

	// Load the installed loader bundle and check if it's something we're willing to uninstall.
	loaderBundle = [NSBundle bundleWithPath:MumbleOverlayLoaderBundle];
	bundleOk = [[loaderBundle bundleIdentifier] isEqualToString:MumbleOverlayLoaderBundleIdentifier];

	// Perform uninstallation using Authorization Services. (Pops up a dialog asking for admin privileges)
	if (bundleOk) {
		err = AuthorizationCreate(nullptr, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &auth);
		if (err == errAuthorizationSuccess) {
			QByteArray tmp = QString::fromLatin1("/tmp/%1_Uninstalled_MumbleOverlay.osax").arg(QDateTime::currentMSecsSinceEpoch()).toLocal8Bit();
			const char *remove[] = { "/bin/mv", [MumbleOverlayLoaderBundle UTF8String], tmp.constData(), nullptr };
			ret = authExec(auth, remove);
		}
		AuthorizationFree(auth, kAuthorizationFlagDefaults);
	}

	return ret;
}
