// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MumbleApplication.h"

#include <Foundation/Foundation.h>

static bool appNapSuppressed = false;

void MUSuppressAppNap(bool suppress) {
	if (suppress == appNapSuppressed) {
		qWarning("AppNap: attempt to set AppNap suppression state to %s while already in that state.", suppress ? "true" : "false");
		return;
	}

	QString translatedReason = QApplication::tr("Mumble is currently connected to a server");
	NSString *reason = const_cast<NSString *>(reinterpret_cast<const NSString *>(CFStringCreateWithCharacters(kCFAllocatorDefault, reinterpret_cast<const UniChar *>(translatedReason.unicode()), translatedReason.length())));

	if (suppress) {
		[[NSProcessInfo processInfo] disableAutomaticTermination:reason];
		qWarning("AppNap: suppressed with reason: '%s'", qPrintable(translatedReason));
	} else {
		[[NSProcessInfo processInfo] enableAutomaticTermination:reason];
		qWarning("AppNap: re-enabled, was suppressed with reason: '%s'", qPrintable(translatedReason));
	}

	appNapSuppressed = suppress;

	[reason release];
}
