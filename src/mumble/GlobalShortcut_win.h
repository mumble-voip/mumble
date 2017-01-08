// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_WIN_H_
#define MUMBLE_MUMBLE_GLOBALSHORTCUT_WIN_H_

#include "GlobalShortcut.h"
#include "Timer.h"

#ifdef USE_GKEY
#include "GKey.h"
#endif

#ifdef USE_XBOXINPUT
#include "XboxInput.h"
#endif

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

typedef QPair<GUID, DWORD> qpButton;

struct InputDevice {
	LPDIRECTINPUTDEVICE8 pDID;

	QString name;

	GUID guid;
	QVariant vguid;

	GUID guidproduct;
	QVariant vguidproduct;

	uint16_t vendor_id;
	uint16_t product_id;

	// dwType to name
	QHash<DWORD, QString> qhNames;

	// Map dwType to dwOfs in our structure
	QHash<DWORD, DWORD> qhTypeToOfs;

	// Map dwOfs in our structure to dwType
	QHash<DWORD, DWORD> qhOfsToType;

	// Buttons active since last reset
	QSet<DWORD> activeMap;
};

class GlobalShortcutWin : public GlobalShortcutEngine {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(GlobalShortcutWin)
	public:
		BYTE ucKeyState[256];
		LPDIRECTINPUT8 pDI;
		QHash<GUID, InputDevice *> qhInputDevices;
		HHOOK hhMouse, hhKeyboard;
		unsigned int uiHardwareDevices;
		Timer tDoubleClick;
		bool bHook;
#ifdef USE_GKEY
		GKeyLibrary *gkey;
#endif
#ifdef USE_XBOXINPUT
		/// xboxinputLastPacket holds the last packet number
		/// that was processed. Any new data queried for a
		/// device is only valid if the packet number is
		/// different than last time we queried it.
		uint32_t   xboxinputLastPacket[XBOXINPUT_MAX_DEVICES];
		XboxInput *xboxinput;
		/// nxboxinput holds the number of XInput devices
		/// available on the system. It is filled out by
		/// our EnumDevices callback.
		int nxboxinput;
#endif

		static BOOL CALLBACK EnumSuitableDevicesCB(LPCDIDEVICEINSTANCE, LPDIRECTINPUTDEVICE8, DWORD, DWORD, LPVOID);
		static BOOL CALLBACK EnumDevicesCB(LPCDIDEVICEINSTANCE, LPVOID);
		static BOOL CALLBACK EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
		static LRESULT CALLBACK HookKeyboard(int, WPARAM, LPARAM);
		static LRESULT CALLBACK HookMouse(int, WPARAM, LPARAM);

		virtual bool canSuppress() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
	public slots:
		void timeTicked();
	public:
		GlobalShortcutWin();
		~GlobalShortcutWin() Q_DECL_OVERRIDE;
		void unacquire();
		QString buttonName(const QVariant &) Q_DECL_OVERRIDE;

		virtual void prepareInput() Q_DECL_OVERRIDE;
};

uint qHash(const GUID &);

#endif
