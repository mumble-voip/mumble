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
#include "Overlay.h"

@interface OverlayInjectorMac : NSObject
 - (void) appLaunched:(NSNotification *)notification;
 - (void) setActive:(BOOL)flag;
@end

@implementation OverlayInjectorMac
- (void) appLaunched:(NSNotification *)notification {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSDictionary *userInfo = [notification userInfo];
	
	pid_t pid = [[userInfo objectForKey:@"NSApplicationProcessIdentifier"] intValue];
	SBApplication *app = [SBApplication applicationWithProcessIdentifier:pid];

	[app setSendMode:kAENoReply];
	[app sendEvent:kASAppleScriptSuite id:kGetAEUT parameters:0];
	[app sendEvent:'MUOL' id:'load' parameters:0];
	[pool release];
}

- (void) setActive:(BOOL)flag {
	NSWorkspace *workspace = [NSWorkspace sharedWorkspace];

	if (flag == YES) {
		[[workspace notificationCenter] addObserver:self
		                                selector:@selector(appLaunched:)
		                                name:NSWorkspaceDidLaunchApplicationNotification
		                                object:workspace];
	} else {
		[[workspace notificationCenter] removeObserver:self
		                                name:NSWorkspaceDidLaunchApplicationNotification
		                                object:workspace];
	}
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

bool Overlay::supportsInstallableOverlay() {
	return true;
}

bool Overlay::isInstalled() {
	bool ret = false;

	// Get the path where we expect the overlay loader to be installed.
	NSString *path = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"MumbleOverlayLoaderBundle"];
	if (! path) {
		qWarning("Overlay_macx: No MumbleOverlayLoaderPath specified in Info.plist. "
			 "Pretending overlay is already installed.");
		return false;
	}

	// Determine if the installed bundle is correctly installed (i.e. it's loadable)
	NSBundle *bundle = [NSBundle bundleWithPath:path];
	return [bundle preflightAndReturnError:NULL];
}

bool Overlay::needsUpgrade() {
	// Get required version from our own Info.plist
	NSUInteger reqVersion = [[[NSBundle mainBundle] objectForInfoDictionaryKey:@"MumbleOverlayLoaderRequiredVersion"] unsignedIntegerValue];

	// Load the overlay loader bundle.
	NSBundle *bundle = [NSBundle bundleWithPath:[[NSBundle mainBundle] objectForInfoDictionaryKey:@"MumbleOverlayLoaderBundle"]];
	if (! bundle) {
		qWarning("Overlay_macx: Unable to load the installed OSAX bundle. This shouldn't happen.");
		return false;
	}

	// Get its version.
	NSUInteger curVersion = [[bundle objectForInfoDictionaryKey:@"MumbleOverlayLoaderVersion"] unsignedIntegerValue];

	// If the two versions do not match up, we need to upgrade.
	return curVersion != reqVersion;
}

bool Overlay::installFiles() {
	AuthorizationRef auth;
	bool ret = false;
	int pid, status;
	OSStatus err;

	// Get the tarball that we should install.
	NSString *tarballPath = [NSString stringWithFormat:@"%@/%@", [[NSBundle mainBundle] bundlePath],
					[[NSBundle mainBundle] objectForInfoDictionaryKey:@"MumbleOverlayInstallPayload"]];

	// And the destination we should install it to.
	NSString *destination = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"MumbleOverlayInstallDestintaion"];

	if (! tarballPath || ! destination) {
		qWarning("Overlay_macx: Info.plist does not specify installation parameters.");
		return false;
	}

	// Request an AuthorizationRef. This is a mechanism in Mac OS X that allows a parent program
	// to spawn child processes with euid = 0.
	//
	// When attempting to launch the child process, a dialog will pop up requesting the user to
	// authorize the launch by logging in with as a user with admin privileges.
	err = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &auth);
	if (err == errAuthorizationSuccess) {
		// This is the tar command that we execute to install our support files.
		const char *argv[] = { "/usr/bin/tar", "-jxf", [tarballPath UTF8String], "-C", [destination UTF8String], NULL };
		// Launch the child process.
		err = AuthorizationExecuteWithPrivileges(auth, argv[0], kAuthorizationFlagDefaults, const_cast<char * const *>(&argv[1]), NULL);
		if (err == errAuthorizationSuccess) {
			// And wait until it is dead.
			do {
				pid = wait(&status);
			} while (pid == -1 && errno == EINTR);
			ret = (pid != -1 && WIFEXITED(status) && WEXITSTATUS(status) == 0);
		} else
			qWarning("Overlay_macx: Failed to AuthorizeExecuteWithPrivileges. (err=%i)", err);
	} else
		qWarning("Overlay_macx: Failed to acquire AuthorizationRef. (err=%i)", err);

	if (! ret) {
		qWarning("Overlay_macx: Failure in installer process. (pid=%i, exited=%u, exitStatus=%u)",
				pid, WIFEXITED(status), WEXITSTATUS(status));
	}

	// Free the AuthorizationRef that we acquired earlier. We're done launching priviledged children.
	AuthorizationFree(auth, kAuthorizationFlagDefaults);
	return ret;
}

bool Overlay::uninstallFiles() {
	// Get the absolute path of our overlay loader bundle.
	NSString *path = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"MumbleOverlayLoaderBundle"];
	// Make sure it is unloaded, at least from ourselves, before we trash it.
	[[NSBundle bundleWithPath:path] unload];

	// This is a tiny AppleScript that kindly asks the Finder to throw
	// the currently installed loader bundle into the trash.
	//
	// This is nice because we don't need to have any privileged code in
	// Mumble itself that does it, and because it goes into the user's
	// trash.
	NSString *uninstallScript = [NSString stringWithFormat:@""
		"set bundlePath to POSIX file \"%@\"\n"
		"tell application \"Finder\" to delete bundlePath\n", path];

	// Execute the script, and hope for the best.
	NSError *error = nil;
	NSString *script = [[NSAppleScript alloc] initWithSource:uninstallScript];
	[script executeAndReturnError:&error];
	[script release];

	if (error) {
		NSLog(@"%@", error);
		return false;
	}

	return true;
}
