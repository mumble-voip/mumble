// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#import <AppKit/AppKit.h>

@interface CompatApp : NSObject
@end

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
@interface CompatApp (NSApplicationDelegateExtension) <NSApplicationDelegate>
@end
#endif

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
@interface CompatApp (NSFileManagerDelegateExtension) <NSFileManagerDelegate>
@end
#endif

@implementation CompatApp

- (void) applicationDidFinishLaunching:(NSNotification *)notification {
	(void)notification;

	NSAlert *alert = [NSAlert alertWithMessageText:@"Mumble" defaultButton:@"OK" alternateButton:nil otherButton:@"Visit Website"
	                          informativeTextWithFormat:@"This version of Mumble only runs on 64-bit Intel Macs.\n\n"
	                                                     "Please download the Universal version from the Mumble website instead.\n"];
	[[alert window] makeKeyAndOrderFront:self];
	[[alert window] center];
	NSInteger button = [alert runModal];
	if (button == NSAlertOtherReturn) {
		[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://mumble.info/"]];
	}

	[NSApp terminate:self];
	[alert release];
}

@end

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	CompatApp *app = [[CompatApp alloc] init];	
	[NSApplication sharedApplication];
	[NSApp setDelegate:app];
	[NSApp run];
        [pool release];
	return 0;
}
