// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#import <AppKit/AppKit.h>
#import <Carbon/Carbon.h>

#include "GlobalShortcut_macx.h"

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
			break;
		}

		case kCGEventMouseMoved:
		case kCGEventLeftMouseDragged:
		case kCGEventRightMouseDragged:
		case kCGEventOtherMouseDragged:
		case kCGEventScrollWheel:
			break;

		case kCGEventKeyDown:
			down = true;
		case kCGEventKeyUp:
			repeat = CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat);
			if (! repeat) {
				keycode = static_cast<unsigned int>(CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode));
				suppress = gs->handleButton(keycode, down);
			}
			break;

		case kCGEventFlagsChanged: {
			CGEventFlags f = CGEventGetFlags(event);

			// Dump active event taps on Ctrl+Alt+Cmd.
			CGEventFlags ctrlAltCmd = static_cast<CGEventFlags>(kCGEventFlagMaskControl|kCGEventFlagMaskAlternate|kCGEventFlagMaskCommand);
			if ((f & ctrlAltCmd) == ctrlAltCmd)
				gs->dumpEventTaps();

			suppress = gs->handleModButton(f);
			break;
		}

		case kCGEventTapDisabledByTimeout:
			qWarning("GlobalShortcutMac: EventTap disabled by timeout. Re-enabling.");
			/*
			 * Re-enable the event tap if it gets disabled by timeout.
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

	{
		TISInputSourceRef inputSource = TISCopyCurrentKeyboardInputSource();
		if (inputSource) {
			CFDataRef data = static_cast<CFDataRef>(TISGetInputSourceProperty(inputSource, kTISPropertyUnicodeKeyLayoutData));
			if (data)
				kbdLayout = reinterpret_cast<UCKeyboardLayout *>(const_cast<UInt8 *>(CFDataGetBytePtr(data)));
		}
	}
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
	@autoreleasepool {
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
	}
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
