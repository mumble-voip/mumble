// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MumbleApplication.h"

#include <Foundation/Foundation.h>

static bool appNapSuppressed = false;

void MUSuppressAppNap(bool suppress) {
	NSProcessInfo *processInfo = [NSProcessInfo processInfo];

	if (suppress == appNapSuppressed) {
		qWarning("AppNap: attempt to set AppNap suppression state to %s while already in that state.", suppress ? "true" : "false");
		return;
	}

	QString translatedReason = QApplication::tr("Mumble is currently connected to a server");
	NSString *reason = [[NSString alloc] initWithCharacters:reinterpret_cast<const unichar *>(translatedReason.unicode())
	                                                  length:static_cast<NSUInteger>(translatedReason.length())];

	if (suppress) {
		[processInfo disableAutomaticTermination:reason];
		qWarning("AppNap: suppressed with reason: '%s'", qPrintable(translatedReason));
	} else {
		[processInfo enableAutomaticTermination:reason];
		qWarning("AppNap: re-enabled, was suppressed with reason: '%s'", qPrintable(translatedReason));
	}

	appNapSuppressed = suppress;

	[reason release];

}
