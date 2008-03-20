/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008, Mikkel Krautz <mikkel@krautz.dk>

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

#include "GlobalShortcut_macx.h"

#define MOD_OFFSET   0x10000
#define MOUSE_OFFSET 0x20000

GlobalShortcutEngine *GlobalShortcutEngine::platformInit() {
	return new GlobalShortcutMac();
}

// ---

static OSStatus MonitorHandler(EventHandlerCallRef caller,
                               EventRef event,
                               void *udata) {
	GlobalShortcutMac *gs = reinterpret_cast<GlobalShortcutMac *>(udata);
	OSType type = GetEventClass(event);
	UInt32 kind = GetEventKind(event);
	unsigned int keycode;
	EventMouseButton mb;
	UInt32 ch;
	bool down;

	Q_UNUSED(caller);
	Q_ASSERT(udata != NULL);

	/* Keyboard events */
	if (type == kEventClassKeyboard) {

		/* Modifiers are special, of course. */
		if (kind == kEventRawKeyModifiersChanged) {
			GetEventParameter(event, kEventParamKeyModifiers, typeUInt32,
			                  NULL, sizeof(UInt32), NULL, &ch);
			gs->handleModButton(ch);

			/* Regular keypresses. */
		} else {
			GetEventParameter(event, kEventParamKeyCode, typeUInt32,
			                  NULL, sizeof(UInt32), NULL, &ch);
			keycode = static_cast<unsigned int>(ch);
			down = (kind == kEventRawKeyDown);
			gs->handleButton(keycode, down);
		}

		/* Mouse events */
	} else if (type == kEventClassMouse) {
		GetEventParameter(event, kEventParamMouseButton, typeMouseButton,
		                  NULL, sizeof(EventMouseButton), NULL, &mb);
		keycode = static_cast<unsigned int>(mb);
		down = (kind == kEventMouseDown);
		gs->handleButton(MOUSE_OFFSET+keycode, down);
	}

	return eventNotHandledErr;
}

static OSStatus CmdHandler(EventHandlerCallRef caller,
                           EventRef event,
                           void *udata) {
	OSStatus err;
	UInt32 klass = GetEventClass(event);

	Q_ASSERT(udata != NULL);

	if (klass != kEventClassCommand) {
		err = MonitorHandler(caller, event, udata);
		if (err)
			return err;
	}

	return eventNotHandledErr;
}

GlobalShortcutMac::GlobalShortcutMac() : modmask(0) {

	static const EventTypeSpec kEvents[] = {
		{ kEventClassCommand,   kEventCommandUpdateStatus },
		{ kEventClassKeyboard,  kEventRawKeyDown },
		{ kEventClassKeyboard,  kEventRawKeyUp },
		{ kEventClassKeyboard,  kEventRawKeyModifiersChanged },
		{ kEventClassMouse,     kEventMouseDown },
		{ kEventClassMouse,     kEventMouseUp },
	};

	static const EventTypeSpec kCmdEvents[] = {
		{ kEventClassCommand,   kEventCommandProcess },
		{ kEventClassKeyboard,  kEventRawKeyDown },
		{ kEventClassKeyboard,  kEventRawKeyUp },
		{ kEventClassKeyboard,  kEventRawKeyModifiersChanged },
		{ kEventClassMouse,     kEventMouseDown },
		{ kEventClassMouse,     kEventMouseUp },
	};

	/*
	 * The CmdHandler handles events when the program is in the foreground.
	 * In a usual Carbon application, this would handle all sorts of internal
	 * events related to the program.
	 */
	InstallApplicationEventHandler(CmdHandler, GetEventTypeCount(kCmdEvents),
	                               kCmdEvents, this, NULL);

	/*
	 * The MonitorHandler handles events when the program is in the background.
	 * This makes it possible for us to receive keystrokes even when Mumble isn't
	 * the foremost program.
	 */
	InstallEventHandler(GetEventMonitorTarget(), MonitorHandler,
	                    GetEventTypeCount(kEvents), kEvents,
	                    this, NULL);
}

GlobalShortcutMac::~GlobalShortcutMac() {
}

void GlobalShortcutMac::run() {
}

void GlobalShortcutMac::needRemap() {
	remap();
}

void GlobalShortcutMac::handleModButton(UInt32 newmask) {
	bool down;

#define MOD_CHANGED(mask, btn) do { \
		if ((newmask & mask) != (modmask & mask)) { \
			down = newmask & mask; \
			handleButton(MOD_OFFSET+btn, down); \
		}} while (0)

	MOD_CHANGED(cmdKey, 0);
	MOD_CHANGED(shiftKey, 1);
	MOD_CHANGED(alphaLock, 2);
	MOD_CHANGED(optionKey, 3);
	MOD_CHANGED(controlKey, 4);
	MOD_CHANGED(kEventKeyModifierNumLockMask, 5);
	MOD_CHANGED(kEventKeyModifierFnMask, 6);

	modmask = newmask;
}

QString GlobalShortcutMac::buttonName(const QVariant &v) {
	bool ok;
	unsigned int key=v.toUInt(&ok);
	if (!ok)
		return QString();

	if (key >= MOUSE_OFFSET)
		return QString("MOUSE_%1").arg(key-MOUSE_OFFSET);
	else if (key >= MOD_OFFSET)
		return QString("MOD_%1").arg(key-MOD_OFFSET);
	else
		return QString("KEY_%1").arg(key);
}
