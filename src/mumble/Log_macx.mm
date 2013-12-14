/* Copyright (C) 2012, Mikkel Krautz <mikkel@krautz.dk>

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

#include "mumble_pch.hpp"

#include "Log.h"
#include "Global.h"

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1080

@interface MUUserNotificationCenterDelegate : NSObject
@end

@implementation MUUserNotificationCenterDelegate
- (void) userNotificationCenter:(NSUserNotificationCenter *)center didDeliverNotification:(NSUserNotification *)notification {
}

- (void) userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification {
	[center removeDeliveredNotification:notification];
}

- (BOOL) userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification {
	return NO;
}
@end

static NSString *Log_QString_to_NSString(const QString& string) {
	return const_cast<NSString *>(reinterpret_cast<const NSString *>(CFStringCreateWithCharacters(kCFAllocatorDefault,
	                                reinterpret_cast<const UniChar *>(string.unicode()), string.length())));
}

#endif

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

void Log::postNotification(MsgType mt, const QString &console, const QString &plain) {
	QString title = msgName(mt);
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1080
	if (QSysInfo::MacintoshVersion >= QSysInfo::MV_MOUNTAINLION) {
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
		QString qsScript = QString::fromLatin1(
			"tell application \"GrowlHelperApp\"\n"
			"	notify with name \"%1\" title \"%1\" description \"%2\" application name \"Mumble\"\n"
			"end tell\n").arg(title).arg(plain);
		if (growl_available())
			qt_mac_execute_apple_script(qsScript, NULL);
	}
}
