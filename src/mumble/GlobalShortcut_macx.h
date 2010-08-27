/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

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

#include <stdlib.h>
#include "GlobalShortcut.h"
#include "Global.h"

class GlobalShortcutMacInit : public QObject, public DeferInit {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(GlobalShortcutMacInit)
	public:
		GlobalShortcutMacInit();
		void initialize();
	public slots:
		void openPrefsPane(const QString &) const;
	protected:
		bool accessibilityApiEnabled() const;
		void accessibilityDialog() const;
};

class GlobalShortcutMac : public GlobalShortcutEngine {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(GlobalShortcutMac)
	public:
		GlobalShortcutMac();
		~GlobalShortcutMac();
		QString buttonName(const QVariant &);
		void needRemap();
		bool handleModButton(CGEventFlags newmask);
		virtual bool canSuppress();

#ifndef COMPAT_CLIENT
	public slots:
		void forwardEvent(void *evt);
#endif

	protected:
		CFRunLoopRef loop;
		CFMachPortRef port;
		CGEventFlags modmask;
		UCKeyboardLayout *kbdLayout;

		void run();

		static CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *udata);
		QString translateMouseButton(const unsigned int keycode) const;
		QString translateModifierKey(const unsigned int keycode) const;
		QString translateKeyName(const unsigned int keycode) const;
};
