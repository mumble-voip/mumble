/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2010, Mikkel Krautz <mikkel@krautz.dk>

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

#import <ScriptingBridge/ScriptingBridge.h>
#include <Security/Security.h>
#import <SecurityInterface/SFCertificatePanel.h>
#include "Overlay.h"
#include "Global.h"
#include "MainWindow.h"

extern "C" {
#include <xar/xar.h>
}

// This is a temporary solution. We don't want the list of our trusted
// intermediate certificates to be tampered with, and the easiest way
// to achieve that is to embed them into the binary (since it gets
// codesigned.)
#include "../../macx/overlay-installer/intermediate.h"

static const NSString *MumbleOverlayLoaderBundle = @"/Library/ScriptingAdditions/MumbleOverlay.osax";
static const NSString *MumbleOverlayLoaderBundleIdentifier = @"net.sourceforge.mumble.OverlayScriptingAddition";

@interface OverlayInjectorMac : NSObject {
	BOOL active;
}

- (id) init;
- (void) dealloc;
- (void) appLaunched:(NSNotification *)notification;
- (void) setActive:(BOOL)flag;
- (void) eventDidFail:(const AppleEvent *)event withError:(NSError *)error;
@end

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

		if (g.s.os.bUseWhitelist) {
			if (g.s.os.qslWhitelist.contains(qsBundleIdentifier))
				overlayEnabled = YES;
		} else {
			if (! g.s.os.qslBlacklist.contains(qsBundleIdentifier))
				overlayEnabled = YES;
		}

		if (overlayEnabled) {
			pid_t pid = [[userInfo objectForKey:@"NSApplicationProcessIdentifier"] intValue];
			SBApplication *app = [SBApplication applicationWithProcessIdentifier:pid];
			[app setDelegate:self];

			// This timeout is specified in 'ticks'.
			// A tick defined as: "[...] (a tick is approximately 1/60 of a second) [...]" in the
			// Apple Event Manager Refernce documentation:
			// http://developer.apple.com/legacy/mac/library/documentation/Carbon/reference/Event_Manager/Event_Manager.pdf
			[app setTimeout:10*60];

			[app setSendMode:kAEWaitReply];
			[app sendEvent:kASAppleScriptSuite id:kGetAEUT parameters:0];

			[app setSendMode:kAENoReply];
			if (QSysInfo::MacintoshVersion == QSysInfo::MV_LEOPARD) {
				[app sendEvent:'MUOL' id:'daol' parameters:0];
			} else if (QSysInfo::MacintoshVersion >= QSysInfo::MV_SNOWLEOPARD) {
				[app sendEvent:'MUOL' id:'load' parameters:0];
			}
		}

		[pool release];
	}
}

- (void) setActive:(BOOL)flag {
	active = flag;
}

// SBApplication delegate method
- (void)eventDidFail:(const AppleEvent *)event withError:(NSError *)error {
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

void Overlay::setActive(bool act) {
	static_cast<OverlayPrivateMac *>(d)->setActive(act);
}

bool OverlayConfig::supportsInstallableOverlay() {
	return true;
}

bool OverlayConfig::supportsCertificates() {
	return true;
}

void OverlayClient::updateMouse() {
	QCursor c = qgv.viewport()->cursor();
	NSCursor *cursor = nil;
	Qt::CursorShape csShape = c.shape();

	switch (csShape) {
		case Qt::IBeamCursor:        cursor = [NSCursor IBeamCursor]; break;
		case Qt::CrossCursor:        cursor = [NSCursor crossCursor]; break;
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
	if (pm.isNull()) {
		NSImage *img = [cursor image];
		CGImageRef cgimg = NULL;
		NSArray *reps = [img representations];
		for (int i = 0; i < [reps count]; i++) {
			NSImageRep *rep = [reps objectAtIndex:i];
			if ([rep class] == [NSBitmapImageRep class]) {
				cgimg = [(NSBitmapImageRep *)rep CGImage];
			}
		}

		if (cgimg) {
			pm = QPixmap::fromMacCGImageRef(cgimg);
			qmCursors.insert(csShape, pm);
		}
	}

	NSPoint p = [cursor hotSpot];
	iOffsetX = (int) p.x;
	iOffsetY = (int) p.y;

	qgpiCursor->setPixmap(pm);
	qgpiCursor->setPos(iMouseX - iOffsetX, iMouseY - iOffsetY);
}

QByteArray preferedInstallerPath() {
	QString userinstall = g.qdBasePath.absolutePath() + QLatin1String("/Overlay/MumbleOverlay.pkg");
	if (QFile::exists(userinstall)) {
		return userinstall.toUtf8();
	}

	NSString *installerPath = [[NSBundle mainBundle] pathForResource:@"MumbleOverlay" ofType:@"pkg"];
	if (installerPath) {
		return QByteArray([installerPath UTF8String]);
	}

	return QByteArray();
}

bool OverlayConfig::isInstalled() {
	bool ret = false;

	// Determine if the installed bundle is correctly installed (i.e. it's loadable)
	NSBundle *bundle = [NSBundle bundleWithPath:MumbleOverlayLoaderBundle];
	ret = [bundle preflightAndReturnError:NULL];

	// Do the bundle identifiers match?
	if (ret) {
		ret = [[bundle bundleIdentifier] isEqualToString:MumbleOverlayLoaderBundleIdentifier];
	}

	return ret;
}

// Check whether this installer installs something 'newer' than what we already have.
// Also checks whether the new installer is compatiable with the current version of
// Mumble.
static bool isInstallerNewer(const char *path, NSUInteger curVer) {
	xar_t pkg = NULL;
	xar_iter_t iter = NULL;
	xar_file_t file = NULL;
	char *data = NULL;
	size_t size = 0;
	bool ret = false;
	QString qsMinVer, qsOverlayVer;

	pkg = xar_open(path, READ);
	if (pkg == NULL) {
		qWarning("isInstallerNewer: Unable to open pkg.");
		goto out;
	}

	iter = xar_iter_new();
	if (iter == NULL) {
		qWarning("isInstallerNewer: Unable to allocate iter");
		goto out;
	}

	file = xar_file_first(pkg, iter);
	while (file != NULL) {
		if (!strcmp(xar_get_path(file), "mumbleoverlay.pkg/PackageInfo"))
			break;
		file = xar_file_next(iter);
	}

	if (file != NULL) {
		if (xar_extract_tobuffersz(pkg, file, &data, &size) == -1) {
			goto out;
		}

		QXmlStreamReader reader(QByteArray::fromRawData(data, size));
		while (! reader.atEnd()) {
			QXmlStreamReader::TokenType tok = reader.readNext();
			if (tok == QXmlStreamReader::StartElement) {
				if (reader.name() == QLatin1String("pkg-info")) {
					qsOverlayVer = reader.attributes().value(QLatin1String("version")).toString();
				} else if (reader.name() == QLatin1String("mumble")) {
					qsMinVer = reader.attributes().value(QLatin1String("minclient")).toString();
				}
			}
		}

		if (reader.hasError() || qsMinVer.isNull() || qsOverlayVer.isNull()) {
			qWarning("isInstallerNewer: Error while parsing XML version info.");
			goto out;
		}

		NSUInteger newVer = qsOverlayVer.toUInt();

		QRegExp rx(QLatin1String("(\\d+)\\.(\\d+)\\.(\\d+)"));
		int major, minor, patch;
		int minmajor, minminor, minpatch;
		if (! rx.exactMatch(QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING))))
			goto out;
		major = rx.cap(1).toInt();
		minor = rx.cap(2).toInt();
		patch = rx.cap(3).toInt();
		if (! rx.exactMatch(qsMinVer))
			goto out;
		minmajor = rx.cap(1).toInt();
		minminor = rx.cap(2).toInt();
		minpatch = rx.cap(3).toInt();

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

		QByteArray prefer = preferedInstallerPath();
		if (prefer.isNull())
			return false;

		return isInstallerNewer(prefer.constData(), curVersion);
	}

	return false;
}

static bool authExec(AuthorizationRef ref, const char **argv) {
	OSStatus err;
	int pid, status;

	err = AuthorizationExecuteWithPrivileges(ref, argv[0], kAuthorizationFlagDefaults, const_cast<char * const *>(&argv[1]), NULL);
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

// Get the certificates form our installer XAR as a NSArray of SecCertificateRef's.
static bool getInstallerCerts(const char *path, NSArray **array) {
	bool ret = false;
	OSStatus err = noErr;
	xar_t pkg = NULL;
	xar_signature_t sig = NULL;
	int32_t ncerts = 0;
	const uint8_t *data = NULL;
	uint32_t len = 0;
	SecCertificateRef tmp = NULL;
	int cur = 0;

	if (array == NULL) {
		qWarning("getInstallerCerts: Argument error.");
		goto err;
	}

	pkg = xar_open(path, READ);
	if (pkg == NULL) {
		qWarning("getInstallerCerts: Unable to open pkg.");
		goto err;
	}

	// We're only interested in the first signature.
	sig = xar_signature_first(pkg);
	if (sig == NULL) {
		qWarning("getInstallerCerts: Unable to get first signature of XAR archive.");
		goto err;
	}

	ncerts = xar_signature_get_x509certificate_count(sig);
	*array = [[NSMutableArray alloc] init];
	for (int32_t i = 0; i < ncerts; i++) {
		if (xar_signature_get_x509certificate_data(sig, i, &data, &len) == -1) {
			qWarning("getInstallerCerts: Unable to extract certificate data.");
			goto err;
		}
		const CSSM_DATA crt = { (CSSM_SIZE) len, (uint8_t *) data };
		err = SecCertificateCreateFromData(&crt, CSSM_CERT_X_509v3, CSSM_CERT_ENCODING_DER, &tmp);
		[*array addObject:(id)tmp];
	}

	ret = true;
err:
	return ret;
}

// Validate the signature of a XAR archive (the archive format
// Apple uses for their installlers)
static bool validateInstallerSignature(const char *path) {
	xar_t pkg = NULL;
	xar_signature_t sig = NULL;
	const char *type = NULL;
	int32_t ncerts = 0;
	X509 **certs = NULL;
	const uint8_t *data = NULL;
	uint32_t len = 0;
	uint8_t *plaindata = NULL, *signdata = NULL;
	uint32_t plainlen = 0, signlen = 0;
	bool ret = false;
	int success = 0;
	RSA *rsa = NULL;
	EVP_PKEY *pkey = NULL;

	// Open installer package.
	pkg = xar_open(path, READ);
	if (pkg == NULL) {
		qWarning("validateInstallerSignature: Unable to open installer for verification.");
		goto err;
	}

	// The first signature of the file is the one we're interested in.
	sig = xar_signature_first(pkg);
	if (sig == NULL) {
		qWarning("validateInstallerSignature: Unable to get first signature.");
		goto err;
	}

	// Get the signature type
	type = xar_signature_type(sig);
	if (strcmp(type, "RSA")) {
		qWarning("validateInstallerSignature: Signature not RSA.");
		goto err;
	}

	// Extract the certificate chain
	ncerts = xar_signature_get_x509certificate_count(sig);
	if (!(ncerts > 0)) {
		qWarning("validateInstallerSignature: No certificates found in XAR.");
		goto err;
	}

	certs = (X509 **)alloca(sizeof(X509 *) * ncerts);
	for (int32_t i = 0; i < ncerts; i++) {
		// Get the certificate data...
		if (xar_signature_get_x509certificate_data(sig, i, &data, &len) == -1) {
			qWarning("validateInstallerSignature:  Could not extract DER encoded certificate from XARchive.");
			goto err;
		}
		X509 *cert = d2i_X509(NULL, &data, (int)len);
		if (cert == NULL) {
			qWarning("validateInstallerSignature: Could not parse DER data.");
			goto err;
		}
		certs[i] = cert;
	}

	// Extract the TOC signed data
	if (xar_signature_copy_signed_data(sig, &plaindata, &plainlen, &signdata, &signlen) != 0) {
		qWarning("validateInstallerSignature: Could not get signed data from XARchive.");
		goto err;
	}

	if (plainlen != 20) { /* SHA1 */
		qWarning("validateInstallerSignature: Digest of installer is not SHA1, cannot verify.");
		goto err;
	}

	pkey = X509_get_pubkey(certs[0]);
	if (! pkey) {
		qWarning("validateInstallerSignature: Unable to get pubkey from X509 struct.");
		goto err;
	}

	if (pkey->type != EVP_PKEY_RSA) {
		qWarning("validateInstallerSignature: Public key is not RSA.");
		goto err;
	}

	rsa = pkey->pkey.rsa;
	if (! rsa) {
		qWarning("validateInstallerSignature: Could not get RSA data from pkey.");
		goto err;
	}

	// Verify the signed archive...
	success = RSA_verify(NID_sha1, plaindata, plainlen, (unsigned char *)signdata, signlen, rsa);
	ret = (success == 1);

err:
	for (int32_t i = 0; i < ncerts; i++) {
		if (certs[i] != NULL)
			X509_free(certs[i]);
	}
	free(plaindata);
	free(signdata);
	if (pkg)
		xar_close(pkg);
	return ret;
}

// Get an NSArray of the system anchors + our bundled intermediate certs.
static bool getAnchorCerts(NSArray **anchors) {
	bool ret = false;
	OSStatus err = noErr;
	CFArrayRef systemAnchors = NULL;

	if (anchors == NULL) {
		qWarning("getAnchorCerts: Invalid argument.");
		goto err;
	}

	err = SecTrustCopyAnchorCertificates(&systemAnchors);
	if (err != noErr) {
		qWarning("Unable to copy system anchor certificates");
		goto err;
	}

	*anchors = [[NSMutableArray alloc] initWithArray:(NSArray *)systemAnchors];
	for (int i = 0; i < sizeof(intermediate_cas)/sizeof(intermediate_cas[0]); i++) {
		QSslCertificate cert(intermediate_cas[i]);
		QByteArray qbaIntermediate = cert.toDer();

		SecCertificateRef tmp = NULL;
		const CSSM_DATA crt = { (CSSM_SIZE) qbaIntermediate.length(), (uint8_t *) qbaIntermediate.constData() };
		err = SecCertificateCreateFromData(&crt, CSSM_CERT_X_509v3, CSSM_CERT_ENCODING_DER, &tmp);
		if (err != noErr) {
			qWarning("getAnchorCerts: Couldn't SecCertificateCreateFromData(). Skipping.");
			continue;
		}

		[(NSMutableArray *) *anchors addObject:(id)tmp];
	}

	ret = true;
err:
	return ret;
}

// First, validate the signature of the installer XAR archive. Then, check
// that the certificate chain is trusted by the system.
bool validateInstaller(const char *path) {
	bool ret = false;
	OSStatus err = noErr;
	NSArray *certs = nil;
	NSArray *anchors = nil;
	SecPolicySearchRef search = NULL;
	SecPolicyRef policy = NULL;
	SecTrustRef trust = NULL;
	SecTrustResultType result;
	CSSM_OID oid = CSSMOID_APPLE_X509_BASIC;

	// First, check that the archive signature is OK.
	if (! validateInstallerSignature(path)) {
		goto err;
	}

	// Get the certificate (and any intermediate certs) from the XAR archive.
	if (! getInstallerCerts(path, &certs)) {
		goto err;
	}

	// Create policy
	err = SecPolicySearchCreate(CSSM_CERT_X_509v3, &oid, NULL, &search);
	if (err != noErr) {
		qWarning("validateInstaller: Unable to create SecPolicySearch object.");
		goto err;
	}
	err = SecPolicySearchCopyNext(search, &policy);
	if (err != noErr) {
		qWarning("validateInstaller: Unable to fetch SecPolicyRef from search object.");
		goto err;
	}

	// Create trust
	err = SecTrustCreateWithCertificates((CFArrayRef)certs, policy, &trust);
	if (err != noErr) {
		qWarning("validateInstaller: Unable to create trust with certs...");
		goto err;
	}

	if (QSysInfo::MacintoshVersion < QSysInfo::MV_SNOWLEOPARD) {
		qWarning("validateInstaller: Non-Snow Leopard install detected. Including bundled intermediate certificates when "
		         "verifying installer.");

		// Get system anchors with our bundled intermediate certificates included.
		if (! getAnchorCerts(&anchors)) {
			qWarning("validateInstaller: Unable to fetch anchors.");
			goto err;
		}

		// Set the anchors for the trust object
		//
		// Note: By doing this, we're actually circumventing the user trust settings for the system anchors.
		//
		// There's only really a way around this for Snow Leopard, where a `SecTrustSetAnchorCertificatesOnly'
		// function was introduced. With this, you can trust the anchor certificates set for the trust object
		// AND trust the system anchors using the user's *own* trust settings for them.
		err = SecTrustSetAnchorCertificates(trust, (CFArrayRef)anchors);
		if (err != noErr) {
			qWarning("validateInstaller: Unable to set bundled anchor certificates.");
			goto err;
		}
	}

	// Do we trust this certificate?
	err = SecTrustEvaluate(trust, &result);
	if (err != noErr) {
		qWarning("validateInstaller: Unable to evaulate trust..");
		goto err;
	}

	// Good documentation on the values of SecTrustResultType:
	// http://lists.apple.com/archives/apple-cdsa/2006/Apr/msg00013.html
	switch (result) {
		case kSecTrustResultProceed:     // User trusts this certificate (as well as the system)
		case kSecTrustResultConfirm:     // Check with the user before proceeding (which we're already doing by giving them a choice).
		case kSecTrustResultUnspecified: // No user trust setting for this cert.
			ret = true;
			break;

		default:
			ret = false;
			break;
	}

err:
	[certs release];
	[anchors release];
	if (search)
		CFRelease(search);
	if (policy)
		CFRelease(policy);
	if (trust)
		CFRelease(trust);
	return ret;
}

bool OverlayConfig::installerIsValid() {
	QByteArray prefer = preferedInstallerPath();
	if (prefer.isNull())
		return false;
	return validateInstaller(prefer.constData());
}

void OverlayConfig::showCertificates() {
	QByteArray prefer = preferedInstallerPath();
	if (prefer.isNull())
		return;

	NSArray *certs;
	if (! getInstallerCerts(prefer.constData(), &certs)) {
		qWarning("OverlayConfig: could not fetch certificates");
		return;
	}

	SFCertificatePanel *certPanel = [SFCertificatePanel sharedCertificatePanel];
	[certPanel beginSheetForWindow:qt_mac_window_for(this) modalDelegate:nil didEndSelector:nil
	           contextInfo:nil certificates:certs showGroup:YES];
	[certs autorelease];
}

bool OverlayConfig::installFiles() {
	AuthorizationRef auth;
	bool ret = false;
	OSStatus err;

	QByteArray prefer = preferedInstallerPath();
	if (prefer.isNull()) {
		qWarning("OverlayConfig: No installers found in search paths.");
		return false;
	}

	if (! installerIsValid())
		return false;

	// Request an AuthorizationRef. This is a mechanism in Mac OS X that allows a parent program
	// to spawn child processes with euid = 0.
	//
	// When attempting to launch the child process, a dialog will pop up requesting the user to
	// authorize the launch by logging in with as a user with admin privileges.
	err = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &auth);
	if (err == errAuthorizationSuccess) {
		// Do the install...
		const char *installer[] = { "/usr/sbin/installer", "-pkg", prefer.constData(), "-target", "LocalSystem", NULL };
		ret = authExec(auth, installer);
	} else if (err != errAuthorizationCanceled) {
		qWarning("OverlayConfig: Failed to acquire AuthorizationRef. (err=%i)", err);
	}

	AuthorizationFree(auth, kAuthorizationFlagDefaults);
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
		err = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &auth);
		if (err == errAuthorizationSuccess) {
			const char *remove[] = { "/bin/rm", "-rf", [MumbleOverlayLoaderBundle UTF8String], NULL };
			ret = authExec(auth, remove);
		}
		AuthorizationFree(auth, kAuthorizationFlagDefaults);
	}

	return ret;
}
