// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SleepInhibitor.h"
#include "Global.h"

#if defined(Q_OS_WIN)
#	include <windows.h>
#elif defined(Q_OS_MAC)
#	include <IOKit/pwr_mgt/IOPMLib.h>
#elif defined(USE_DBUS)
#	include <QtDBus/QtDBus>
#endif

SleepInhibitor::SleepInhibitor(QObject *parent) : QObject(parent) {
}

SleepInhibitor::~SleepInhibitor() {
	setInhibit(false);
}

void SleepInhibitor::setInhibit(bool inhibit) {
	if (inhibit == m_inhibited) {
		return;
	}

	if (inhibit) {
		platformInhibit();
	} else {
		platformDeinhibit();
	}

	m_inhibited = inhibit;
}

void SleepInhibitor::platformInhibit() {
#if defined(Q_OS_WIN)
	SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
#elif defined(Q_OS_MAC)
	CFStringRef reasonForActivity = CFStringCreateWithCString(kCFAllocatorDefault, "Mumble is in a call", kCFStringEncodingUTF8);
	IOReturn success = IOPMAssertionCreateWithDescription(kIOPMAssertionTypeNoDisplaySleep,
	                                                     reasonForActivity,
	                                                     NULL, NULL, NULL, 0, NULL,
	                                                     &m_assertionID);
	CFRelease(reasonForActivity);
	if (success != kIOReturnSuccess) {
		qWarning("SleepInhibitor: Failed to create IOPMAssertion");
		m_assertionID = 0;
	}
#elif defined(USE_DBUS)
	QDBusInterface screensaver("org.freedesktop.ScreenSaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
	if (screensaver.isValid()) {
		QDBusReply<uint32_t> reply = screensaver.call("Inhibit", "Mumble", "In a call");
		if (reply.isValid()) {
			m_cookie = reply.value();
		} else {
			qWarning() << "SleepInhibitor: ScreenSaver Inhibit call failed:" << reply.error().message();
		}
	}

	QDBusInterface logind("org.freedesktop.login1", "/org/freedesktop/login1", "org.freedesktop.login1.Manager", QDBusConnection::systemBus());
	if (logind.isValid()) {
		QDBusReply<QDBusUnixFileDescriptor> reply = logind.call("Inhibit", "sleep:idle", "Mumble", "In a call", "block");
		if (reply.isValid()) {
			m_logindFD = reply.value();
		} else {
			qWarning() << "SleepInhibitor: Logind Inhibit call failed:" << reply.error().message();
		}
	}
#endif
}

void SleepInhibitor::platformDeinhibit() {
#if defined(Q_OS_WIN)
	SetThreadExecutionState(ES_CONTINUOUS);
#elif defined(Q_OS_MAC)
	if (m_assertionID != 0) {
		IOPMAssertionRelease(m_assertionID);
		m_assertionID = 0;
	}
#elif defined(USE_DBUS)
	if (m_cookie != 0) {
		QDBusInterface screensaver("org.freedesktop.ScreenSaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
		if (screensaver.isValid()) {
			screensaver.call("UnInhibit", m_cookie);
		}
		m_cookie = 0;
	}
	if (m_logindFD.isValid()) {
		m_logindFD = QDBusUnixFileDescriptor();
	}
#endif
}
