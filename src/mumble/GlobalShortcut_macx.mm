// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#import <AppKit/AppKit.h>
#import <Carbon/Carbon.h>

#include "GlobalShortcut_macx.h"
#ifdef USE_OVERLAY
#	include "OverlayClient.h"
#endif

#define MOD_OFFSET   0x10000
#define MOUSE_OFFSET 0x20000

GlobalShortcutEngine *GlobalShortcutEngine::platformInit() {
	return new GlobalShortcutMac();
}

CGEventRef GlobalShortcutMac::callback(CGEventTapProxy proxy, CGEventType type,
                                       CGEventRef event, void *udata) {
	GlobalShortcutMac *gs = reinterpret_cast<GlobalShortcutMac *>(udata);
	unsigned int keycode;
	bool suppress = false;
	bool forward = false;
	bool down = false;
	int64_t repeat = 0;

	Q_UNUSED(proxy);

	switch (type) {
		case kCGEventLeftMouseDown:
		case kCGEventRightMouseDown:
		case kCGEventOtherMouseDown:
			down = true;
		case kCGEventLeftMouseUp:
		case kCGEventRightMouseUp:
		case kCGEventOtherMouseUp: {
			keycode = static_cast<unsigned int>(CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber));
			suppress = gs->handleButton(MOUSE_OFFSET+keycode, down);
			/* Suppressing "the" mouse button is probably not a good idea :-) */
			if (keycode == 0)
				suppress = false;
			forward = !suppress;
			break;
		}

		case kCGEventMouseMoved:
		case kCGEventLeftMouseDragged:
		case kCGEventRightMouseDragged:
		case kCGEventOtherMouseDragged: {
#ifdef USE_OVERLAY
			if (Global::get().ocIntercept) {
				int64_t dx = CGEventGetIntegerValueField(event, kCGMouseEventDeltaX);
				int64_t dy = CGEventGetIntegerValueField(event, kCGMouseEventDeltaY);
				Global::get().ocIntercept->iMouseX = qBound<int>(0, Global::get().ocIntercept->iMouseX + static_cast<int>(dx), Global::get().ocIntercept->iWidth - 1);
				Global::get().ocIntercept->iMouseY = qBound<int>(0, Global::get().ocIntercept->iMouseY + static_cast<int>(dy), Global::get().ocIntercept->iHeight - 1);
				QMetaObject::invokeMethod(Global::get().ocIntercept, "updateMouse", Qt::QueuedConnection);
				forward = true;
			}
#endif
			break;
		}

		case kCGEventScrollWheel:
			forward = true;
			break;

		case kCGEventKeyDown:
			down = true;
		case kCGEventKeyUp:
			repeat = CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat);
			if (! repeat) {
				keycode = static_cast<unsigned int>(CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode));
				suppress = gs->handleButton(keycode, down);
			}
			forward = true;
			break;

		case kCGEventFlagsChanged: {
			CGEventFlags f = CGEventGetFlags(event);

			// Dump active event taps on Ctrl+Alt+Cmd.
			CGEventFlags ctrlAltCmd = static_cast<CGEventFlags>(kCGEventFlagMaskControl|kCGEventFlagMaskAlternate|kCGEventFlagMaskCommand);
			if ((f & ctrlAltCmd) == ctrlAltCmd)
				gs->dumpEventTaps();

			suppress = gs->handleModButton(f);
			forward = !suppress;
			break;
		}

		case kCGEventTapDisabledByTimeout:
			qWarning("GlobalShortcutMac: EventTap disabled by timeout. Re-enabling.");
			/*
			 * On Snow Leopard, we get this event type quite often. It disables our event
			 * tap completely. Possible Apple bug.
			 *
			 * For now, simply call CGEventTapEnable() to enable our event tap again.
			 *
			 * See: http://lists.apple.com/archives/quartz-dev/2009/Sep/msg00007.html
			 */
			CGEventTapEnable(gs->port, true);
			break;

		case kCGEventTapDisabledByUserInput:
			break;

		default:
			break;
	}

#ifdef USE_OVERLAY
		if (forward && Global::get().ocIntercept) {
			NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
			NSEvent *evt = [[NSEvent eventWithCGEvent:event] retain];
			QMetaObject::invokeMethod(gs, "forwardEvent", Qt::QueuedConnection, Q_ARG(void *, evt));
			[pool release];
			return nullptr;
		}
#else
	// Mark forward as unused in this case
	(void) forward;
#endif
	return suppress ? nullptr : event;
}

GlobalShortcutMac::GlobalShortcutMac()
    : loop(nullptr)
    , port(nullptr)
    , modmask(static_cast<CGEventFlags>(0)) {
#ifndef QT_NO_DEBUG
	qWarning("GlobalShortcutMac: Debug build detected. Disabling shortcut engine.");
	return;
#endif

	CGEventMask evmask = CGEventMaskBit(kCGEventLeftMouseDown) |
	                     CGEventMaskBit(kCGEventLeftMouseUp) |
	                     CGEventMaskBit(kCGEventRightMouseDown) |
	                     CGEventMaskBit(kCGEventRightMouseUp) |
	                     CGEventMaskBit(kCGEventOtherMouseDown) |
	                     CGEventMaskBit(kCGEventOtherMouseUp) |
	                     CGEventMaskBit(kCGEventKeyDown) |
	                     CGEventMaskBit(kCGEventKeyUp) |
	                     CGEventMaskBit(kCGEventFlagsChanged) |
	                     CGEventMaskBit(kCGEventMouseMoved) |
	                     CGEventMaskBit(kCGEventLeftMouseDragged) |
	                     CGEventMaskBit(kCGEventRightMouseDragged) |
	                     CGEventMaskBit(kCGEventOtherMouseDragged) |
	                     CGEventMaskBit(kCGEventScrollWheel);
	port = CGEventTapCreate(kCGSessionEventTap,
	                        kCGTailAppendEventTap,
	                        kCGEventTapOptionDefault, // active filter (not only a listener)
	                        evmask,
	                        GlobalShortcutMac::callback,
	                        this);

	if (! port) {
		qWarning("GlobalShortcutMac: Unable to create EventTap. Global Shortcuts will not be available.");
		return;
	}

	kbdLayout = nullptr;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1050
# if MAC_OS_X_VERSION_MIN_REQUIRED < 1050
	if (TISCopyCurrentKeyboardInputSource && TISGetInputSourceProperty)
# endif
	{
		TISInputSourceRef inputSource = TISCopyCurrentKeyboardInputSource();
		if (inputSource) {
			CFDataRef data = static_cast<CFDataRef>(TISGetInputSourceProperty(inputSource, kTISPropertyUnicodeKeyLayoutData));
			if (data)
				kbdLayout = reinterpret_cast<UCKeyboardLayout *>(const_cast<UInt8 *>(CFDataGetBytePtr(data)));
		}
	}
#endif
#ifndef __LP64__
	if (! kbdLayout) {
		SInt16 currentKeyScript = GetScriptManagerVariable(smKeyScript);
		SInt16 lastKeyLayoutID = GetScriptVariable(currentKeyScript, smScriptKeys);
		Handle handle = GetResource('uchr', lastKeyLayoutID);
		if (handle)
			kbdLayout = reinterpret_cast<UCKeyboardLayout *>(*handle);
	}
#endif
	if (! kbdLayout)
		qWarning("GlobalShortcutMac: No keyboard layout mapping available. Unable to perform key translation.");

	start(QThread::TimeCriticalPriority);
}

GlobalShortcutMac::~GlobalShortcutMac() {
#ifndef QT_NO_DEBUG
	return;
#endif
	if (loop) {
		CFRunLoopStop(loop);
		loop = nullptr;
		wait();
	}
}

void GlobalShortcutMac::dumpEventTaps() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	uint32_t ntaps = 0;
	CGEventTapInformation table[64];
	if (CGGetEventTapList(20, table, &ntaps) == kCGErrorSuccess) {
		qWarning("--- Installed Event Taps ---");
		for (uint32_t i = 0; i < ntaps; i++) {
			CGEventTapInformation *info = &table[i];

			NSString *processName = nil;
			NSRunningApplication *app = [NSRunningApplication runningApplicationWithProcessIdentifier: info->processBeingTapped];
			if (app) {
				processName = [app localizedName];
			}

			qWarning("{");
			qWarning("  eventTapID: %u", info->eventTapID);
			qWarning("  tapPoint: 0x%x", info->tapPoint);
			qWarning("  options = 0x%x", info->options);
			qWarning("  eventsOfInterest = 0x%llx", info->eventsOfInterest);
			qWarning("  tappingProcess = %i (%s)", info->tappingProcess, [processName UTF8String]);
			qWarning("  processBeingTapped = %i", info->processBeingTapped);
			qWarning("  enabled = %s", info->enabled ? "true":"false");
			qWarning("  minUsecLatency = %.2f", info->minUsecLatency);
			qWarning("  avgUsecLatency = %.2f", info->avgUsecLatency);
			qWarning("  maxUsecLatency = %.2f", info->maxUsecLatency);
			qWarning("}");
		}
		qWarning("--- End of Event Taps ---");
	}
	[pool release];
}

void GlobalShortcutMac::forwardEvent(void *evt) {
	NSEvent *event = (NSEvent *) evt;
#ifdef USE_OVERLAY
	SEL sel = nil;

	if (!Global::get().ocIntercept) {
		[event release];
		return;
	}

	QWidget *vp  = Global::get().ocIntercept->qgv.viewport();
	NSView *view = (NSView *) vp->winId();

	switch ([event type]) {
		case NSEventTypeLeftMouseDown:
			sel = @selector(mouseDown:);
			break;
		case NSEventTypeLeftMouseUp:
			sel = @selector(mouseUp:);
			break;
		case NSEventTypeLeftMouseDragged:
			sel = @selector(mouseDragged:);
			break;
		case NSEventTypeRightMouseDown:
			sel = @selector(rightMouseDown:);
			break;
		case NSEventTypeRightMouseUp:
			sel = @selector(rightMouseUp:);
			break;
		case NSEventTypeRightMouseDragged:
			sel = @selector(rightMouseDragged:);
			break;
		case NSEventTypeOtherMouseDown:
			sel = @selector(otherMouseDown:);
			break;
		case NSEventTypeOtherMouseUp:
			sel = @selector(otherMouseUp:);
			break;
		case NSEventTypeOtherMouseDragged:
			sel = @selector(otherMouseDragged:);
			break;
		case NSEventTypeMouseEntered:
			sel = @selector(mouseEntered:);
			break;
		case NSEventTypeMouseExited:
			sel = @selector(mouseExited:);
			break;
		case NSEventTypeMouseMoved:
			sel = @selector(mouseMoved:);
			break;
		default:
			// Ignore the rest. We only care about mouse events.
			break;
	}

	if (sel) {
		NSPoint p; p.x = (CGFloat) Global::get().ocIntercept->iMouseX;
		p.y = (CGFloat) (Global::get().ocIntercept->iHeight - Global::get().ocIntercept->iMouseY);
		NSEvent *mouseEvent = [NSEvent mouseEventWithType:[event type] location:p modifierFlags:[event modifierFlags] timestamp:[event timestamp]
		                               windowNumber:0 context:nil eventNumber:[event eventNumber] clickCount:[event clickCount]
		                               pressure:[event pressure]];
		if ([view respondsToSelector:sel])
				[view performSelector:sel withObject:mouseEvent];
		[event release];
		return;
	}

	switch ([event type]) {
		case NSEventTypeKeyDown:
			sel = @selector(keyDown:);
			break;
		case NSEventTypeKeyUp:
			sel = @selector(keyUp:);
			break;
		case NSEventTypeFlagsChanged:
			sel = @selector(flagsChanged:);
			break;
		case NSEventTypeScrollWheel:
			sel = @selector(scrollWheel:);
			break;
		default:
			break;
	}

	if (sel) {
		if ([view respondsToSelector:sel])
				[view performSelector:sel withObject:event];
	}
#endif
	[event release];
}

void GlobalShortcutMac::run() {
	loop = CFRunLoopGetCurrent();
	CFRunLoopSourceRef src = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, port, 0);
	CFRunLoopAddSource(loop, src, kCFRunLoopCommonModes);
	CFRunLoopRun();
}

void GlobalShortcutMac::needRemap() {
	remap();
}

bool GlobalShortcutMac::handleModButton(const CGEventFlags newmask) {
	bool down;
	bool suppress = false;

#define MOD_CHANGED(mask, btn) do { \
	    if ((newmask & mask) != (modmask & mask)) { \
	        down = newmask & mask; \
	        suppress = handleButton(MOD_OFFSET+btn, down); \
	        modmask = newmask; \
	        return suppress; \
	    }} while (0)

	MOD_CHANGED(kCGEventFlagMaskAlphaShift, 0);
	MOD_CHANGED(kCGEventFlagMaskShift, 1);
	MOD_CHANGED(kCGEventFlagMaskControl, 2);
	MOD_CHANGED(kCGEventFlagMaskAlternate, 3);
	MOD_CHANGED(kCGEventFlagMaskCommand, 4);
	MOD_CHANGED(kCGEventFlagMaskHelp, 5);
	MOD_CHANGED(kCGEventFlagMaskSecondaryFn, 6);
	MOD_CHANGED(kCGEventFlagMaskNumericPad, 7);

	return false;
}

QString GlobalShortcutMac::translateMouseButton(const unsigned int keycode) const {
	return QString::fromLatin1("%1").arg(keycode - MOUSE_OFFSET + 1);
}

QString GlobalShortcutMac::translateModifierKey(const unsigned int keycode) const {
	unsigned int key = keycode - MOD_OFFSET;
	switch (key) {
		case 0:
			return QLatin1String("Caps Lock");
		case 1:
			return QLatin1String("Shift");
		case 2:
			return QLatin1String("Control");
		case 3:
			return QLatin1String("Alt/Option");
		case 4:
			return QLatin1String("Command");
		case 5:
			return QLatin1String("Help");
		case 6:
			return QLatin1String("Fn");
		case 7:
			return QLatin1String("Num Lock");
	}
	return QString::fromLatin1("Modifier %1").arg(key);
}

QString GlobalShortcutMac::translateKeyName(const unsigned int keycode) const {
	UInt32 junk = 0;
	UniCharCount len = 64;
	std::vector<UniChar> unicodeString;
	unicodeString.resize(static_cast<std::size_t>(len));

	if (! kbdLayout)
		return QString();

	OSStatus err = UCKeyTranslate(kbdLayout, static_cast<UInt16>(keycode),
	                              kUCKeyActionDisplay, 0, LMGetKbdType(),
	                              kUCKeyTranslateNoDeadKeysBit, &junk,
	                              len, &len, unicodeString.data());
	if (err != noErr)
		return QString();

	if (len == 1) {
		switch (unicodeString[0]) {
			case '\t':
				return QLatin1String("Tab");
			case '\r':
				return QLatin1String("Enter");
			case '\b':
				return QLatin1String("Backspace");
			case 27:
				return QLatin1String("Escape");
			case ' ':
				return QLatin1String("Space");
			case 28:
				return QLatin1String("Left");
			case 29:
				return QLatin1String("Right");
			case 30:
				return QLatin1String("Up");
			case 31:
				return QLatin1String("Down");
		}

		if (unicodeString[0] < ' ') {
			qWarning("GlobalShortcutMac: Unknown translation for keycode %u: %u", keycode, unicodeString[0]);
			return QString();
		}
	}

	return QString(reinterpret_cast<const QChar *>(unicodeString.data()), static_cast<int>(len)).toUpper();
}

GlobalShortcutMac::ButtonInfo GlobalShortcutMac::buttonInfo(const QVariant &v) {
	bool ok;
	unsigned int key = v.toUInt(&ok);
	if (!ok) {
		return ButtonInfo();
	}

	ButtonInfo info;

	if (key >= MOUSE_OFFSET) {
		info.device = tr("Mouse");
		info.devicePrefix = QLatin1String("M");
		info.name = translateMouseButton(key);
		return info;
	} else {
		info.device = tr("Keyboard");
		info.name = key >= MOD_OFFSET ? translateModifierKey(key) : translateKeyName(key);
		if (!info.name.isEmpty()) {
			return info;
		}
	}

	info.name = QString::number(key);
	return info;
}

void GlobalShortcutMac::setEnabled(bool b) {
	// Since Mojave, passing nullptr to CGEventTapEnable() segfaults.
	if (port) {
		CGEventTapEnable(port, b);
	}
}

bool GlobalShortcutMac::enabled() {
	if (!port) {
		return false;
	}

	return CGEventTapIsEnabled(port);
}

bool GlobalShortcutMac::canSuppress() {
	return true;
}

bool GlobalShortcutMac::canDisable() {
	return true;
}

#undef MOD_OFFSET
#undef MOUSE_OFFSET
#undef MOD_CHANGED
