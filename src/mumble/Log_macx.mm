// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Log.h"

#include <QtCore/QOperatingSystemVersion>

#include <Foundation/Foundation.h>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
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

#if QT_VERSION < 0x050800
extern bool qt_mac_execute_apple_script(const QString &script, AEDesc *ret);

static bool growl_available() {
	static int isAvailable = -1;
	if (isAvailable == -1)  {
		OSStatus err = LSFindApplicationForInfo('GRRR', CFSTR("com.Growl.GrowlHelperApp"), CFSTR("GrowlHelperApp.app"), NULL, NULL);
		isAvailable = (err != kLSApplicationNotFoundErr) ? 1 : 0;
		if (isAvailable) {
			QStringList qslAllEvents;
			for (int i = Log::firstMsgType; i <= Log::lastMsgType; ++i) {
				Log::MsgType t = static_cast<Log::MsgType>(i);
				qslAllEvents << QString::fromLatin1("\"%1\"").arg(g.l->msgName(t));
			}
			QString qsScript = QString::fromLatin1(
				"tell application \"GrowlHelperApp\"\n"
				"	set the allNotificationsList to {%1}\n"
				"	set the enabledNotificationsList to {%1}\n"
				"	register as application \"Mumble\""
				"		all notifications allNotificationsList"
				"		default notifications enabledNotificationsList"
				"		icon of application \"Mumble\"\n"
				"end tell\n").arg(qslAllEvents.join(QLatin1String(",")));
			qt_mac_execute_apple_script(qsScript, NULL);
		}
	}
	return isAvailable == 1;
}
#endif // QT_VERSION

void Log::postNotification(MsgType mt, const QString &plain) {
	QString title = msgName(mt);
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1080
# if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
	const QOperatingSystemVersion current = QOperatingSystemVersion::current();
	if (current.majorVersion() > 10 || (current.majorVersion() == 10 && current.minorVersion() >= 8)) {
# else
	if (QSysInfo::MacintoshVersion >= QSysInfo::MV_MOUNTAINLION) {
# endif
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
	{
#if QT_VERSION < 0x050800
		QString qsScript = QString::fromLatin1(
			"tell application \"GrowlHelperApp\"\n"
			"	notify with name \"%1\" title \"%1\" description \"%2\" application name \"Mumble\"\n"
			"end tell\n").arg(title).arg(plain);
		if (growl_available())
			qt_mac_execute_apple_script(qsScript, NULL);
#endif
	}
}
