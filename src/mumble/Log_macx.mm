// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Log.h"

#include <QtCore/QOperatingSystemVersion>

#include <Foundation/Foundation.h>

#include "Global.h"

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1080

@interface MUUserNotificationCenterDelegate : NSObject <NSUserNotificationCenterDelegate>
@end

@implementation MUUserNotificationCenterDelegate
- (void) userNotificationCenter:(NSUserNotificationCenter *)center didDeliverNotification:(NSUserNotification *)notification {
	Q_UNUSED(center);
	Q_UNUSED(notification);
}

- (void) userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification {
	[center removeDeliveredNotification:notification];
}

- (BOOL) userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification {
	Q_UNUSED(center);
	Q_UNUSED(notification);

	return NO;
}
@end

static NSString *Log_QString_to_NSString(const QString& string) {
	return const_cast<NSString *>(reinterpret_cast<const NSString *>(CFStringCreateWithCharacters(kCFAllocatorDefault,
	                                reinterpret_cast<const UniChar *>(string.unicode()), string.length())));
}

#endif


void Log::postNotification(MsgType mt, const QString &plain) {
	QString title = msgName(mt);
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1080
	const QOperatingSystemVersion current = QOperatingSystemVersion::current();
	if (current.majorVersion() > 10 || (current.majorVersion() == 10 && current.minorVersion() >= 8)) {
		NSUserNotificationCenter *userNotificationCenter = [NSUserNotificationCenter defaultUserNotificationCenter];
		if (userNotificationCenter.delegate == nil) {
			// We hand the delegate property a delegate with a retain count of 1.  We don't keep
			// a reference to the delegate anywhere else, so it's not really a leak.
			userNotificationCenter.delegate = [[MUUserNotificationCenterDelegate alloc] init];
		}
		NSUserNotification *userNotification = [[[NSUserNotification alloc] init] autorelease];
		userNotification.title = [Log_QString_to_NSString(title) autorelease];
		userNotification.informativeText = [Log_QString_to_NSString(plain) autorelease];
		[userNotificationCenter scheduleNotification:userNotification];
	} else
#endif
	{}
}
