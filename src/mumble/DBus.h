// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DBUS_H_
#define MUMBLE_MUMBLE_DBUS_H_

#include <QtDBus/QDBusAbstractAdaptor>

class QDBusMessage;

class MumbleDBus : public QDBusAbstractAdaptor {
private:
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "net.sourceforge.mumble.Mumble")
	Q_DISABLE_COPY(MumbleDBus)
	Q_PROPERTY(bool mute READ isSelfMuted WRITE setSelfMuted)
	Q_PROPERTY(bool deaf READ isSelfDeaf WRITE setSelfDeaf)
public:
	MumbleDBus(QObject *parent);
public slots:
	void openUrl(const QString &url, const QDBusMessage &);
	void getCurrentUrl(const QDBusMessage &);
	void getTalkingUsers(const QDBusMessage &);
	void focus();

	/// Change when Mumble transmits voice.
	///
	/// @param mode The new transmit mode (0 = continuous, 1 = voice activity, 2 = push-to-talk)
	void setTransmitMode(unsigned int mode, const QDBusMessage &);

	/// Get the current transmit mode.
	///
	/// @return The current transmit mode (0 = continuous, 1 = voice activity, 2 = push-to-talk)
	unsigned int getTransmitMode();

	void toggleSelfMuted();
	void toggleSelfDeaf();
	void setSelfMuted(bool mute);
	void setSelfDeaf(bool deafen);
	bool isSelfMuted();
	bool isSelfDeaf();
	void startTalking();
	void stopTalking();
};

#endif
