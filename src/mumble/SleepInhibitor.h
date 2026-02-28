// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SLEEPINHIBITOR_H_
#define MUMBLE_MUMBLE_SLEEPINHIBITOR_H_

#include <cstdint>

#include <QtCore/QObject>
#if defined(USE_DBUS)
#	include <QtDBus/QDBusUnixFileDescriptor>
#endif

class SleepInhibitor : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(SleepInhibitor)

public:
	explicit SleepInhibitor(QObject *parent = nullptr);
	~SleepInhibitor() override;

	void setInhibit(bool inhibit);

private:
	bool m_inhibited = false;
#if defined(Q_OS_WIN)
	// No extra state needed for Windows (ES_CONTINUOUS)
#elif defined(Q_OS_MAC)
	uint32_t m_assertionID = 0;
#elif defined(USE_DBUS)
	uint32_t m_cookie = 0;
	QDBusUnixFileDescriptor m_logindFD;
#endif

	void platformInhibit();
	void platformDeinhibit();
};

#endif
