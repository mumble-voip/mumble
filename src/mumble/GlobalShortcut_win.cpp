// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// For detailed info about RAWKEYBOARD handling:
// https://blog.molecular-matters.com/2011/09/05/properly-handling-keyboard-input

#include "GlobalShortcut_win.h"

#include "Global.h"

#ifdef USE_GKEY
#	include "GKey.h"
#endif

#include <codecvt>
#include <iomanip>

#include <QTimer>

#include <sstream>

extern "C" {
// clang-format off
// Do NOT change the order of the includes below or compile errors will occur!
#include <hidsdi.h>
#include <hidpi.h>
// clang-format on
}

// From os_win.cpp
extern HWND mumble_mw_hwnd;

struct InputHid {
	uint32_t button = 0;
	std::string deviceName;
	std::string devicePrefix;

	friend QDataStream &operator<<(QDataStream &stream, const InputHid &output) {
		stream << static_cast< quint32 >(output.button);
		stream << QString::fromStdString(output.deviceName);
		stream << QString::fromStdString(output.devicePrefix);

		return stream;
	}

	friend QDataStream &operator>>(QDataStream &stream, InputHid &input) {
		quint32 button;
		QString deviceName;
		QString devicePrefix;

		stream >> button;
		stream >> deviceName;
		stream >> devicePrefix;

		input.button       = static_cast< uint32_t >(button);
		input.deviceName   = deviceName.toStdString();
		input.devicePrefix = devicePrefix.toStdString();

		return stream;
	}

	bool operator==(const InputHid &rhs) const {
		return this->button == rhs.button && this->deviceName == rhs.deviceName
			   && this->devicePrefix == rhs.devicePrefix;
	}

	bool operator<(const InputHid &rhs) const {
		return this->button < rhs.button && this->deviceName < rhs.deviceName && this->devicePrefix < rhs.devicePrefix;
	}
};
Q_DECLARE_METATYPE(InputHid)

struct InputKeyboard {
	bool e0;
	uint16_t code;

	friend QDataStream &operator<<(QDataStream &stream, const InputKeyboard &output) {
		stream << output.e0;
		stream << static_cast< quint16 >(output.code);

		return stream;
	}

	friend QDataStream &operator>>(QDataStream &stream, InputKeyboard &input) {
		quint16 code;

		stream >> input.e0;
		stream >> code;

		input.code = static_cast< uint16_t >(code);

		return stream;
	}

	bool operator==(const InputKeyboard &rhs) const { return this->e0 == rhs.e0 && this->code == rhs.code; }

	bool operator<(const InputKeyboard &rhs) const { return this->e0 < rhs.e0 && this->code < rhs.code; }
};
Q_DECLARE_METATYPE(InputKeyboard)

enum class InputMouse { Left = 1, Right, Middle, Side_1, Side_2 };
Q_DECLARE_METATYPE(InputMouse)

#ifdef USE_XBOXINPUT
struct InputXinput {
	uint8_t device;
	uint8_t code;

	friend QDataStream &operator<<(QDataStream &stream, const InputXinput &output) {
		stream << static_cast< qint8 >(output.device);
		stream << static_cast< qint8 >(output.code);

		return stream;
	}

	friend QDataStream &operator>>(QDataStream &stream, InputXinput &input) {
		quint8 device;
		quint8 code;

		stream >> device;
		stream >> code;

		input.device = static_cast< uint8_t >(device);
		input.code   = static_cast< uint8_t >(code);

		return stream;
	}

	bool operator==(const InputXinput &rhs) const { return this->device == rhs.device && this->code == rhs.code; }

	bool operator<(const InputXinput &rhs) const { return this->device < rhs.device && this->code < rhs.code; }
};
Q_DECLARE_METATYPE(InputXinput)
#endif
#ifdef USE_GKEY
struct InputGkey {
	bool keyboard;
	uint8_t button;
	uint8_t mode;

	friend QDataStream &operator<<(QDataStream &stream, const InputGkey &output) {
		stream << output.keyboard;
		stream << static_cast< qint8 >(output.button);
		stream << static_cast< qint8 >(output.mode);

		return stream;
	}

	friend QDataStream &operator>>(QDataStream &stream, InputGkey &input) {
		quint8 button;
		quint8 mode;

		stream >> input.keyboard;
		stream >> button;
		stream >> mode;

		input.button = static_cast< uint8_t >(button);
		input.mode   = static_cast< uint8_t >(mode);

		return stream;
	}

	bool operator==(const InputGkey &rhs) const {
		return this->keyboard == rhs.keyboard && this->button == rhs.button && this->mode == rhs.mode;
	}

	bool operator<(const InputGkey &rhs) const {
		return this->keyboard < rhs.keyboard && this->button < rhs.button && this->mode < rhs.mode;
	}
};
Q_DECLARE_METATYPE(InputGkey)
#endif

GlobalShortcutEngine *GlobalShortcutEngine::platformInit() {
	return new GlobalShortcutWin();
}

void GlobalShortcutWin::registerMetaTypes() {
	static bool registered = false;

	// Only register the MetaTypes once.
	if (!registered) {
		registered = true;

		qRegisterMetaType< InputHid >();
		qRegisterMetaTypeStreamOperators< InputHid >();
		QMetaType::registerComparators< InputHid >();

		qRegisterMetaType< InputKeyboard >();
		qRegisterMetaTypeStreamOperators< InputKeyboard >();
		QMetaType::registerComparators< InputKeyboard >();

		qRegisterMetaType< InputMouse >();
		qRegisterMetaTypeStreamOperators< InputMouse >();
		QMetaType::registerComparators< InputMouse >();
#ifdef USE_XBOXINPUT
		qRegisterMetaType< InputXinput >();
		qRegisterMetaTypeStreamOperators< InputXinput >();
		QMetaType::registerComparators< InputXinput >();
#endif
#ifdef USE_GKEY
		qRegisterMetaType< InputGkey >();
		qRegisterMetaTypeStreamOperators< InputGkey >();
		QMetaType::registerComparators< InputGkey >();
#endif
	}
}

QList< Shortcut > GlobalShortcutWin::migrateSettings(const QList< Shortcut > &oldShortcuts) {
	constexpr QUuid keyboardUuid(0x6F1D2B61, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);
	constexpr QUuid mouseUuid(0x6F1D2B60, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);
	constexpr QUuid xinputUuid(0xCA3937E3, 0x640C, 0x4D9E, 0x9E, 0xF3, 0x90, 0x3F, 0x8B, 0x4F, 0xBC, 0xAB);
	constexpr QUuid gkeyKeyboardUuid(0x153E64E6, 0x98C8, 0x4E, 0x03, 0x80EF, 0x5F, 0xFD, 0x33, 0xD2, 0x5B, 0x8A);
	constexpr QUuid gkeyMouseUuid(0xC41E60AF, 0x9022, 0x46CF, 0xBC, 0x39, 0x37, 0x98, 0x10, 0x82, 0xD7, 0x16);

	QList< Shortcut > newShortcuts;

	for (auto shortcut : oldShortcuts) {
		bool ok = true;

		for (auto &button : shortcut.qlButtons) {
			if (!button.isValid()) {
				// This happens when the user runs a version that uses
				// the old format after the migration is performed.
				ok = false;
				break;
			}

			if (button.userType() == qMetaTypeId< InputHid >() || button.userType() == qMetaTypeId< InputKeyboard >()
				|| button.userType() == qMetaTypeId< InputMouse >() || button.userType() == qMetaTypeId< InputXinput >()
				|| button.userType() == qMetaTypeId< InputGkey >()) {
				// Already in the new format.
				continue;
			}

			const auto entries = button.toList();
			if (entries.size() < 2) {
				ok = false;
				break;
			}

			auto value = entries.at(0).toUInt(&ok);
			if (!ok) {
				break;
			}

			const auto uuid = entries.at(1).toUuid();
			if (uuid == keyboardUuid) {
				InputKeyboard input;
				input.code = (value & ~0x8000U) >> 8;
				input.e0   = value & 0x8000U;

				// With DirectInput the extended bit is:
				// - Set for the Pause key.
				// - Unset for the Numlock key.
				// With raw input it's the opposite.
				if (input.code == 0x45) {
					input.e0 = !input.e0;
				}

				button = QVariant::fromValue(input);
			} else if (uuid == mouseUuid) {
				value >>= 8;
				if (value < 3 || value > 7) {
					ok = false;
					break;
				}

				button = QVariant::fromValue(static_cast< InputMouse >(value - 2));
#ifdef USE_XBOXINPUT
			} else if (uuid == xinputUuid) {
				InputXinput input;
				input.device = (value >> 24) & 0xFF;
				input.code   = value & 0x00FFFFFF;

				button = QVariant::fromValue(input);
#endif
#ifdef USE_GKEY
			} else if (uuid == gkeyKeyboardUuid) {
				InputGkey input = {};
				input.keyboard  = true;
				input.mode      = value >> 16;
				input.button    = value & 0xFFFF;

				button = QVariant::fromValue(input);
			} else if (uuid == gkeyMouseUuid) {
				InputGkey input = {};
				input.button    = value;

				button = QVariant::fromValue(input);
#endif
			} else {
				ok = false;
				break;
			}
		}

		if (ok) {
			newShortcuts << shortcut;
		}
	}

	return newShortcuts;
}

GlobalShortcutWin::GlobalShortcutWin()
#ifdef USE_XBOXINPUT
	: m_xinputDevices(0), m_xinputLastPacket()
#endif
{
	// Register the MetaTypes if they have not already been registered (e.g in Settings)
	registerMetaTypes();

	connect(this, &GlobalShortcutWin::hidMessage, this, &GlobalShortcutWin::on_hidMessage);
	connect(this, &GlobalShortcutWin::keyboardMessage, this, &GlobalShortcutWin::on_keyboardMessage);
	connect(this, &GlobalShortcutWin::mouseMessage, this, &GlobalShortcutWin::on_mouseMessage);

	start(QThread::LowestPriority);
}

GlobalShortcutWin::~GlobalShortcutWin() {
	quit();
	wait();
}

void GlobalShortcutWin::run() {
#ifdef USE_XBOXINPUT
	if (Global::get().s.bEnableXboxInput) {
		static_assert(std::numeric_limits< uint8_t >::max() >= XBOXINPUT_MAX_DEVICES,
					  "Higher XBOXINPUT_MAX_DEVICES than anticipated.");

		m_xinput = std::make_unique< XboxInput >();
		if (!m_xinput->isValid()) {
			qWarning("GlobalShortcutWin: Failed to initialize XboxInput!");
			m_xinput.reset();
		}
	}
#endif
#ifdef USE_GKEY
	if (Global::get().s.bEnableGKey) {
		static_assert(std::numeric_limits< uint8_t >::max() >= GKEY_MAX_KEYBOARD_MODE,
					  "GKEY_MAX_KEYBOARD_MODE is higher than anticipated.");
		static_assert(std::numeric_limits< uint8_t >::max() >= GKEY_MAX_MOUSE_BUTTON,
					  "GKEY_MAX_MOUSE_BUTTON is higher than anticipated.");

		m_gkey = std::make_unique< GKeyLibrary >();
		if (!m_gkey->isValid()) {
			qWarning("GlobalShortcutWin: Failed to initialize GKey!");
			m_gkey.reset();
		}
	}
#endif
	// Wait for MainWindow's constructor to finish, so that we can get the window's handle.
	while (!mumble_mw_hwnd) {
		yieldCurrentThread();
	}

	constexpr uint8_t nRid   = 5;
	RAWINPUTDEVICE rid[nRid] = {};

	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage     = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags     = RIDEV_INPUTSINK;
	rid[0].hwndTarget  = mumble_mw_hwnd;

	rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[1].usUsage     = HID_USAGE_GENERIC_JOYSTICK;
	rid[1].dwFlags     = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
	rid[1].hwndTarget  = mumble_mw_hwnd;

	rid[2].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[2].usUsage     = HID_USAGE_GENERIC_GAMEPAD;
	rid[2].dwFlags     = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
	rid[2].hwndTarget  = mumble_mw_hwnd;

	rid[3].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[3].usUsage     = HID_USAGE_GENERIC_KEYBOARD;
	rid[3].dwFlags     = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
	rid[3].hwndTarget  = mumble_mw_hwnd;

	rid[4].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[4].usUsage     = HID_USAGE_GENERIC_MULTI_AXIS_CONTROLLER;
	rid[4].dwFlags     = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
	rid[4].hwndTarget  = mumble_mw_hwnd;

	if (!RegisterRawInputDevices(rid, nRid, sizeof(RAWINPUTDEVICE))) {
		qWarning("GlobalShortcutWindows: RegisterRawInputDevices() failed with error %u!", GetLastError());
	}

	QTimer timer;
	connect(&timer, &QTimer::timeout, this, &GlobalShortcutWin::timeTicked);
	timer.start(20);

	exec();
}

void GlobalShortcutWin::injectRawInputMessage(HRAWINPUT handle) {
	UINT size = 0;
	if (GetRawInputData(handle, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) != 0) {
		return;
	}

	auto buffer = std::make_unique< uint8_t[] >(size);
	if (GetRawInputData(handle, RID_INPUT, buffer.get(), &size, sizeof(RAWINPUTHEADER)) <= 0) {
		return;
	}

	auto input = reinterpret_cast< const PRAWINPUT >(buffer.get());
	switch (input->header.dwType) {
		case RIM_TYPEMOUSE: {
			const RAWMOUSE &mouse = input->data.mouse;
			emit mouseMessage(mouse.usButtonFlags, mouse.usButtonData);
			break;
		}
		case RIM_TYPEKEYBOARD: {
			const RAWKEYBOARD &keyboard = input->data.keyboard;
			if (keyboard.MakeCode == KEYBOARD_OVERRUN_MAKE_CODE) {
				// Invalid or unrecognizable combination of keys is pressed or
				// the number of keys pressed exceeds the limit for this keyboard.
				return;
			}

			if (keyboard.VKey == 0xFF) {
				// Discard "fake keys" which are part of an escaped sequence.
				return;
			}

			emit keyboardMessage(keyboard.Flags, keyboard.MakeCode, keyboard.VKey);
			break;
		}
		case RIM_TYPEHID: {
			const RAWHID &hid = input->data.hid;
			std::vector< char > reports(hid.dwSizeHid * hid.dwCount);
			memcpy(reports.data(), hid.bRawData, reports.size());

			emit hidMessage(input->header.hDevice, std::move(reports), hid.dwSizeHid);
		}
	}
}

void GlobalShortcutWin::on_hidMessage(const HANDLE deviceHandle, std::vector< char > reports,
									  const uint32_t reportSize) {
	auto iter = m_devices.find(deviceHandle);
	if (iter == m_devices.end()) {
		iter = addDevice(deviceHandle);
		if (iter == m_devices.cend()) {
			return;
		}
	}

	auto &device = (*iter).second;
#ifdef USE_XBOXINPUT
	if (device.xinput && m_xinput) {
		return;
	}
#endif
	auto data = reinterpret_cast< PHIDP_PREPARSED_DATA >(&device.data[0]);

	ULONG nUsages = device.buttons.size();
	std::vector< USAGE > usages(nUsages);
	if (HidP_GetUsages(HidP_Input, HID_USAGE_PAGE_BUTTON, 0, &usages[0], &nUsages, data, &reports[0], reportSize)
		!= HIDP_STATUS_SUCCESS) {
		return;
	}

	device.buttons.assign(device.buttons.size(), false);
	for (ULONG i = 0; i < nUsages; ++i) {
		device.buttons[usages[i] - device.usageRange.first] = true;
	}

	for (uint32_t i = 0; i < device.buttons.size(); ++i) {
		InputHid input;
		input.button       = i;
		input.deviceName   = device.name;
		input.devicePrefix = device.prefix;

		handleButton(QVariant::fromValue(input), device.buttons[i]);
	}
}

void GlobalShortcutWin::on_keyboardMessage(const uint16_t flags, uint16_t scanCode, const uint16_t virtualKey) {
	if (virtualKey == VK_NUMLOCK) {
		// Keys like “Pause / Break” and “Numlock” act strangely,
		// sometimes like they are not even the same physical key.
		// They use the so called escaped sequences, which we have to decipher.
		scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) | 0x100;
	}

	// E0 and E1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
	// See https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html.
	if (flags & RI_KEY_E1) {
		// For escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey().
		// MapVirtualKey() is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
		scanCode = virtualKey != VK_PAUSE ? MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) : 0x45;
	}

	InputKeyboard input = {};
	input.code          = scanCode;
	input.e0            = flags & RI_KEY_E0;

	handleButton(QVariant::fromValue(input), !(flags & RI_KEY_BREAK));
}

void GlobalShortcutWin::on_mouseMessage(const uint16_t flags, const uint16_t data) {
	InputMouse input;
	bool down;

	switch (flags) {
		case RI_MOUSE_BUTTON_1_DOWN:
			input = InputMouse::Left;
			down  = true;
			break;
		case RI_MOUSE_BUTTON_1_UP:
			input = InputMouse::Left;
			down  = false;
			break;
		case RI_MOUSE_BUTTON_2_DOWN:
			input = InputMouse::Right;
			down  = true;
			break;
		case RI_MOUSE_BUTTON_2_UP:
			input = InputMouse::Right;
			down  = false;
			break;
		case RI_MOUSE_BUTTON_3_DOWN:
			input = InputMouse::Middle;
			down  = true;
			break;
		case RI_MOUSE_BUTTON_3_UP:
			input = InputMouse::Middle;
			down  = false;
			break;
		case RI_MOUSE_BUTTON_4_DOWN:
			input = InputMouse::Side_1;
			down  = true;
			break;
		case RI_MOUSE_BUTTON_4_UP:
			input = InputMouse::Side_1;
			down  = false;
			break;
		case RI_MOUSE_BUTTON_5_DOWN:
			input = InputMouse::Side_2;
			down  = true;
			break;
		case RI_MOUSE_BUTTON_5_UP:
			input = InputMouse::Side_2;
			down  = false;
			break;
		default:
			return;
	}

	handleButton(QVariant::fromValue(input), down);
}

GlobalShortcutWin::DeviceMap::iterator GlobalShortcutWin::addDevice(const HANDLE deviceHandle) {
	RID_DEVICE_INFO deviceInfo = {};
	UINT size                  = sizeof(deviceInfo);
	if (GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICEINFO, &deviceInfo, &size) <= 0) {
		return m_devices.end();
	}

	Device device;

	// E.g. "[045E:02EA]"
	std::stringstream nameStream;
	nameStream << std::uppercase << std::hex << std::setfill('0');
	nameStream << '[';
	nameStream << std::setw(4) << deviceInfo.hid.dwVendorId;
	nameStream << ':';
	nameStream << std::setw(4) << deviceInfo.hid.dwProductId;
	nameStream << ']';

	device.prefix = nameStream.str() + ':';

	if (GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICENAME, nullptr, &size) == 0) {
		std::wstring name;
		name.resize(size);
		// E.g. "\\?\HID#VID_045E&PID_02FF&IG_00#8&15c9c520&0&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}"
		if (GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICENAME, &name[0], &size) > 0) {
#ifdef USE_XBOXINPUT
			if (name.find(L"IG_") != name.npos) {
				device.xinput = true;
			}
#endif
			auto handle = CreateFile(name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
									 nullptr, OPEN_EXISTING, 0, nullptr);
			if (handle != INVALID_HANDLE_VALUE) {
				// From
				// https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/hidsdi/nf-hidsdi-hidd_getproductstring:
				// "For USB devices, the maximum string length is 126 wide characters (not including the terminating
				// NULL character)."
				//
				// std::string::resize() doesn't take the NULL character into account, but an extra byte doesn't hurt.
				name.clear();
				name.resize(127);

				std::wstring_convert< std::codecvt_utf8_utf16< wchar_t > > conv;

				if (HidD_GetManufacturerString(handle, &name[0], sizeof(wchar_t) * name.size())) {
					nameStream << ' ' << conv.to_bytes(name);
					name.clear();
				}

				if (HidD_GetProductString(handle, &name[0], sizeof(wchar_t) * name.size())) {
					nameStream << ' ' << conv.to_bytes(name);
				}

				CloseHandle(handle);
			}
		}
	}

	device.name = nameStream.str();

	if (GetRawInputDeviceInfo(deviceHandle, RIDI_PREPARSEDDATA, nullptr, &size) != 0) {
		return m_devices.end();
	}

	device.data.resize(size);
	auto data = reinterpret_cast< PHIDP_PREPARSED_DATA >(&device.data[0]);
	if (GetRawInputDeviceInfo(deviceHandle, RIDI_PREPARSEDDATA, data, &size) <= 0) {
		return m_devices.end();
	}

	HIDP_CAPS caps;
	if (HidP_GetCaps(data, &caps) != HIDP_STATUS_SUCCESS) {
		return m_devices.end();
	}

	USHORT nCaps = caps.NumberInputButtonCaps;
	std::vector< HIDP_BUTTON_CAPS > buttonCaps(nCaps);
	if (HidP_GetSpecificButtonCaps(HidP_Input, HID_USAGE_PAGE_BUTTON, 0, 0, &buttonCaps[0], &nCaps, data)
		!= HIDP_STATUS_SUCCESS) {
		return m_devices.end();
	}

	device.usageRange.first  = buttonCaps[0].Range.UsageMin;
	device.usageRange.second = buttonCaps[0].Range.UsageMax;
	device.buttons.resize(device.usageRange.second - device.usageRange.first + 1);

	if (device.xinput) {
		++m_xinputDevices;
	}

	qInfo("GlobalShortcutWin: \"%s\" added", device.name.c_str());

	return m_devices.insert({ deviceHandle, device }).first;
}

void GlobalShortcutWin::deviceRemoved(const HANDLE deviceHandle) {
	const auto iter = m_devices.find(deviceHandle);
	if (iter == m_devices.cend()) {
		return;
	}

	const auto &device = (*iter).second;
	if (device.xinput) {
		--m_xinputDevices;
	}

	qInfo("GlobalShortcutWin: \"%s\" removed", device.name.c_str());

	m_devices.erase(iter);
}
#ifdef USE_XBOXINPUT
bool GlobalShortcutWin::xinputIsPressed(const uint8_t bit, const XboxInputState &state) {
	// The buttons field of XboxInputState contains a bit
	// for each button on the Xbox controller. The official
	// headers enumerate the bits via XINPUT_GAMEPAD_*.
	// The official mapping uses all 16-bits, but leaves
	// bit 10 and 11 (counting from 0) undocumented.
	//
	// It turns out that bit 10 is the guide button,
	// which can be queried using the non-public
	// XInputGetStateEx() function.
	//
	// Our mapping uses the bit number as a button index.
	// So 0x1 -> 0, 0x2 -> 1, 0x4 -> 2, and so on...
	//
	// However, since the buttons field is only a 16-bit value,
	// and we also want to use the left and right triggers as
	// buttons, we assign them the button indexes 16 and 17.
	switch (bit) {
		case 16:
			return state.leftTrigger > XBOXINPUT_TRIGGER_THRESHOLD;
		case 17:
			return state.rightTrigger > XBOXINPUT_TRIGGER_THRESHOLD;
		default:
			return state.buttons & (1 << bit);
	}
}
#endif
void GlobalShortcutWin::timeTicked() {
	if (bNeedRemap) {
		remap();
	}
#ifdef USE_XBOXINPUT
	if (m_xinput && m_xinputDevices > 0) {
		for (uint8_t i = 0; i < XBOXINPUT_MAX_DEVICES; ++i) {
			XboxInputState state;
			if (m_xinput->GetState(i, &state) != ERROR_SUCCESS) {
				continue;
			}

			// Skip the result of GetState() if the packet number hasn't changed,
			// or if we're at the first packet.
			if (m_xinputLastPacket[i] != 0 && state.packetNumber == m_xinputLastPacket[i]) {
				continue;
			}

			InputXinput input = {};
			input.device      = i;

			for (uint8_t j = 0; j < 18; ++j) {
				input.code = j;
				handleButton(QVariant::fromValue(input), xinputIsPressed(j, state));
			}

			m_xinputLastPacket[i] = state.packetNumber;
		}
	}
#endif
#ifdef USE_GKEY
	if (m_gkey) {
		InputGkey input = {};
		input.keyboard  = false;

		for (uint8_t button = GKEY_MIN_MOUSE_BUTTON; button <= GKEY_MAX_MOUSE_BUTTON; ++button) {
			input.button = button;
			handleButton(QVariant::fromValue(input), m_gkey->isMouseButtonPressed(button));
		}

		input.keyboard = true;

		for (uint8_t mode = GKEY_MIN_KEYBOARD_MODE; mode <= GKEY_MAX_KEYBOARD_MODE; ++mode) {
			input.mode = mode;

			for (uint8_t button = GKEY_MIN_KEYBOARD_BUTTON; button <= GKEY_MAX_KEYBOARD_BUTTON; ++button) {
				input.button = button;
				handleButton(QVariant::fromValue(input), m_gkey->isKeyboardGkeyPressed(button, mode));
			}
		}
	}
#endif
}

GlobalShortcutWin::ButtonInfo GlobalShortcutWin::buttonInfo(const QVariant &button) {
	ButtonInfo info;

	// Unfortunately qMetaTypeId() is not a constexpr function.
	// As a result, we cannot use a switch-case.
	if (button.userType() == qMetaTypeId< InputHid >()) {
		const auto input  = button.value< InputHid >();
		info.device       = QString::fromStdString(input.deviceName);
		info.devicePrefix = QString::fromStdString(input.devicePrefix);
		info.name         = QString::number(input.button);
	} else if (button.userType() == qMetaTypeId< InputKeyboard >()) {
		info.device = tr("Keyboard");

		const auto input = button.value< InputKeyboard >();
		wchar_t buffer[MAX_PATH];
		if (GetKeyNameText((input.code << 16) | (input.e0 << 24), buffer, MAX_PATH)) {
			info.name = QString::fromWCharArray(buffer);
		} else {
			// If GetKeyNameText() cannot find the name. Show a K prefix and the scan code instead of Unknown.
			// For keys like F13 - F24
			info.devicePrefix = QStringLiteral("K");
			info.name         = QString::number(static_cast< uint >(input.code));
		}
	} else if (button.userType() == qMetaTypeId< InputMouse >()) {
		info.device       = tr("Mouse");
		info.devicePrefix = QStringLiteral("M");

		const auto input = button.value< InputMouse >();
		info.name        = QString::number(static_cast< uint >(input));
	}
#ifdef USE_XBOXINPUT
	else if (button.userType() == qMetaTypeId< InputXinput >()) {
		const auto input  = button.value< InputXinput >();
		info.device       = QString::fromLatin1("Xbox controller #%1").arg(input.device + 1);
		info.devicePrefix = QString::fromLatin1("Xbox%1:").arg(input.device + 1);

		// Translate from our own button index mapping to
		// the actual Xbox controller button names.
		// For a description of the mapping, see the state
		// querying code in GlobalShortcutWin::timeTicked().
		switch (input.code) {
			case 0:
				info.name = QStringLiteral("Up");
				break;
			case 1:
				info.name = QStringLiteral("Down");
				break;
			case 2:
				info.name = QStringLiteral("Left");
				break;
			case 3:
				info.name = QStringLiteral("Right");
				break;
			case 4:
				info.name = QStringLiteral("Start");
				break;
			case 5:
				info.name = QStringLiteral("Back");
				break;
			case 6:
				info.name = QStringLiteral("LeftThumb");
				break;
			case 7:
				info.name = QStringLiteral("RightThumb");
				break;
			case 8:
				info.name = QStringLiteral("LeftShoulder");
				break;
			case 9:
				info.name = QStringLiteral("RightShoulder");
				break;
			case 10:
				info.name = QStringLiteral("Guide");
				break;
			case 11:
				info.name = QStringLiteral("11");
				break;
			case 12:
				info.name = QStringLiteral("A");
				break;
			case 13:
				info.name = QStringLiteral("B");
				break;
			case 14:
				info.name = QStringLiteral("X");
				break;
			case 15:
				info.name = QStringLiteral("Y");
				break;
			case 16:
				info.name = QStringLiteral("LeftTrigger");
				break;
			case 17:
				info.name = QStringLiteral("RightTrigger");
		}
	}
#endif
#ifdef USE_GKEY
	else if (button.userType() == qMetaTypeId< InputGkey >()) {
		info.device       = QStringLiteral("Logitech G-keys");
		info.devicePrefix = QStringLiteral("GKey:");

		const auto input = button.value< InputGkey >();
		if (input.keyboard) {
			info.name = m_gkey->getKeyboardGkeyString(input.button, input.mode);
		} else {
			info.name = m_gkey->getMouseButtonString(input.button);
		}
	}
#endif

	return info;
}
