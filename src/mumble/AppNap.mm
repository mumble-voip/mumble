/* Copyright (C) 2015, Mikkel Krautz <mikkel@krautz.dk>

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

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
static bool appNapSuppressed = false;
#endif

void MUSuppressAppNap(bool suppress) {
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	NSProcessInfo *processInfo = [NSProcessInfo processInfo];
	if (![processInfo respondsToSelector:@selector(disableAutomaticTermination:)]) {
		return;
	}

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
#else
	Q_UNUSED(suppress);
#endif
}
