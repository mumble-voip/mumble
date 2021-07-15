// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_WIN_H_
#define MUMBLE_MUMBLE_GLOBALSHORTCUT_WIN_H_

#include "GlobalShortcut.h"

#ifdef USE_XBOXINPUT
#	include "XboxInput.h"
#endif

#include <memory>
#include <unordered_map>

#ifdef USE_GKEY
class GKeyLibrary;
#endif

class GlobalShortcutWin : public GlobalShortcutEngine {
	Q_OBJECT
	Q_DISABLE_COPY(GlobalShortcutWin)

public:
	static void registerMetaTypes();

	/// @param oldShortcuts List of shortcuts to migrate.
	/// @returns List of shortcuts in the new format.
	static QList< Shortcut > migrateSettings(const QList< Shortcut > &oldShortcuts);

	/// Inject a native Windows raw input message into GlobalShortcutWin's
	/// thread. This method is meant to be called from the main thread
	/// to pass native Windows keyboard messages to GlobalShortcutWin.
	///
	/// @param msg The raw input handle to inject into GlobalShortcutWin.
	void injectRawInputMessage(HRAWINPUT handle);

	ButtonInfo buttonInfo(const QVariant &button) override;
	void run() override;

	GlobalShortcutWin();
	~GlobalShortcutWin() override;
public slots:
	void deviceRemoved(const HANDLE deviceHandle);
	void timeTicked();

	void on_hidMessage(const HANDLE deviceHandle, std::vector< char > reports, const uint32_t reportSize);
	void on_keyboardMessage(const uint16_t flags, uint16_t scanCode, const uint16_t virtualKey);
	void on_mouseMessage(const uint16_t flags, const uint16_t data);
signals:
	void hidMessage(const HANDLE deviceHandle, std::vector< char > reports, const uint32_t reportSize);
	void keyboardMessage(const uint16_t flags, uint16_t scanCode, const uint16_t virtualKey);
	void mouseMessage(const uint16_t flags, const uint16_t data);

protected:
	struct Device {
		std::string name;
		std::string prefix;
		std::vector< uint8_t > data;
		std::vector< bool > buttons;
		std::pair< uint16_t, uint16_t > usageRange;
#ifdef USE_XBOXINPUT
		bool xinput;
		Device() : xinput(false){};
#endif
	};

	typedef std::unordered_map< HANDLE, Device > DeviceMap;
	DeviceMap m_devices;
#ifdef USE_XBOXINPUT
	std::unique_ptr< XboxInput > m_xinput;
	/// Holds the number of XInput devices available on the system.
	/// It is filled out by addDevice().
	uint8_t m_xinputDevices;
	/// Holds the last packet number that was processed.
	/// Any new data queried for a device is only valid
	/// if the packet number is different than last time we queried it.
	uint32_t m_xinputLastPacket[XBOXINPUT_MAX_DEVICES];

	static bool xinputIsPressed(const uint8_t bit, const XboxInputState &state);
#endif
#ifdef USE_GKEY
	std::unique_ptr< GKeyLibrary > m_gkey;
#endif
	DeviceMap::iterator addDevice(const HANDLE deviceHandle);
};

#endif
