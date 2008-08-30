/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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

#include "GlobalShortcut_win.h"
#include "MainWindow.h"
#include "Global.h"

#undef FAILED
#define FAILED(Status) (static_cast<HRESULT>(Status)<0)

#define DX_SAMPLE_BUFFER_SIZE 512

uint qHash(const GUID &a) {
	uint val = a.Data1 ^ a.Data2 ^ a.Data3;
	for (int i=0;i<8;i++)
		val += a.Data4[i];
	return val;
}

GlobalShortcutEngine *GlobalShortcutEngine::platformInit() {
	return new GlobalShortcutWin();
}

GlobalShortcutWin::GlobalShortcutWin() {
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeTicked()));

	HRESULT hr;

	pDI = NULL;

	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void **>(&pDI), NULL))) {
		qFatal("GlobalShortcutWin: Failed to create d8input");
		return;
	}

	timer->start(20);
	start(QThread::TimeCriticalPriority);
}

GlobalShortcutWin::~GlobalShortcutWin() {
	quit();
	wait();
	foreach(InputDevice *id, qhInputDevices) {
		if (id->pDID) {
			id->pDID->Unacquire();
			id->pDID->Release();
		}
	}
	pDI->Release();
}

void GlobalShortcutWin::run() {
	HMODULE hSelf;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (wchar_t *) &HookKeyboard, &hSelf);
	hhKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, HookKeyboard, hSelf, 0);
	hhMouse = SetWindowsHookEx(WH_MOUSE_LL, HookMouse, hSelf, 0);

	exec();

	UnhookWindowsHookEx(hhKeyboard);
	UnhookWindowsHookEx(hhMouse);
}

LRESULT CALLBACK GlobalShortcutWin::HookKeyboard(int nCode, WPARAM wParam, LPARAM lParam) {
	GlobalShortcutWin *gsw=static_cast<GlobalShortcutWin *>(engine);
	KBDLLHOOKSTRUCT *key=reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
	if (nCode >= 0) {
		QList<QVariant> ql;
		ql << static_cast<unsigned int>((key->scanCode << 8) | 0x4);
		ql << QVariant(QUuid(GUID_SysKeyboard));
		if (gsw->handleButton(ql, !(key->flags & LLKHF_UP)))
			return 1;
	}
	return CallNextHookEx(gsw->hhKeyboard, nCode, wParam, lParam);
}

LRESULT CALLBACK GlobalShortcutWin::HookMouse(int nCode, WPARAM wParam, LPARAM lParam) {
	GlobalShortcutWin *gsw=static_cast<GlobalShortcutWin *>(engine);
	MSLLHOOKSTRUCT *mouse=reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);
	if (nCode >= 0) {
		bool down = false;
		unsigned int btn = 0;
		switch (wParam) {
			case WM_LBUTTONDOWN:
				down = true;
			case WM_LBUTTONUP:
				btn = 3;
				break;
			case WM_RBUTTONDOWN:
				down = true;
			case WM_RBUTTONUP:
				btn = 4;
				break;
			case WM_MBUTTONDOWN:
				down = true;
			case WM_MBUTTONUP:
				btn = 5;
				break;
			case WM_XBUTTONDOWN:
				down = true;
			case WM_XBUTTONUP:
				btn = 5 + (mouse->mouseData >> 16);
			default:
				break;
		}
		if (btn) {
			QList<QVariant> ql;
			ql << static_cast<unsigned int>((btn << 8) | 0x4);
			ql << QVariant(QUuid(GUID_SysMouse));
			if (gsw->handleButton(ql, down))
				return 1;
		}
	}
	return CallNextHookEx(gsw->hhMouse, nCode, wParam, lParam);
}



BOOL CALLBACK GlobalShortcutWin::EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef) {
	InputDevice *id=static_cast<InputDevice *>(pvRef);
	QString name = QString::fromUtf16(reinterpret_cast<const ushort *>(lpddoi->tszName));
	id->qhNames[lpddoi->dwType] = name;

	return DIENUM_CONTINUE;
}

BOOL GlobalShortcutWin::EnumDevicesCB(LPCDIDEVICEINSTANCE pdidi, LPVOID pContext) {
	GlobalShortcutWin *cbgsw=static_cast<GlobalShortcutWin *>(pContext);
	HRESULT hr;

	QString name = QString::fromUtf16(reinterpret_cast<const ushort *>(pdidi->tszProductName));
	QString sname = QString::fromUtf16(reinterpret_cast<const ushort *>(pdidi->tszInstanceName));

	InputDevice *id = new InputDevice;
	id->name = name;
	id->guid = pdidi->guidInstance;
	id->vguid = QVariant(QUuid(id->guid).toString());

	if (FAILED(hr = cbgsw->pDI->CreateDevice(pdidi->guidInstance, &id->pDID, NULL)))
		qFatal("GlobalShortcutWin: CreateDevice: %lx", hr);

	if (FAILED(hr = id->pDID->EnumObjects(EnumDeviceObjectsCallback, static_cast<void *>(id), DIDFT_BUTTON)))
		qFatal("GlobalShortcutWin: EnumObjects: %lx", hr);

	if (id->qhNames.count() > 0) {
		QList<DWORD> types = id->qhNames.keys();
		qSort(types);

		int nbuttons = types.count();
		DIOBJECTDATAFORMAT *rgodf = new DIOBJECTDATAFORMAT[nbuttons];
		DIDATAFORMAT df;
		ZeroMemory(&df, sizeof(df));
		df.dwSize = sizeof(df);
		df.dwObjSize = sizeof(DIOBJECTDATAFORMAT);
		df.dwFlags=DIDF_ABSAXIS;
		df.dwDataSize = (nbuttons + 3) & (~0x3);
		df.dwNumObjs = nbuttons;
		df.rgodf = rgodf;
		for (int i=0;i<nbuttons;i++) {
			ZeroMemory(& rgodf[i], sizeof(DIOBJECTDATAFORMAT));
			DWORD dwType = types[i];
			DWORD dwOfs = i;
			rgodf[i].dwOfs = dwOfs;
			rgodf[i].dwType = dwType;
			id->qhOfsToType[dwOfs] = dwType;
			id->qhTypeToOfs[dwType] = dwOfs;
		}

		if (FAILED(hr = id->pDID->SetCooperativeLevel(g.mw->winId(), DISCL_NONEXCLUSIVE|DISCL_BACKGROUND)))
			qFatal("GlobalShortcutWin: SetCooperativeLevel: %lx", hr);

		if (FAILED(hr = id->pDID->SetDataFormat(&df)))
			qFatal("GlobalShortcutWin: SetDataFormat: %lx", hr);

		DIPROPDWORD dipdw;

		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = DX_SAMPLE_BUFFER_SIZE;

		if (FAILED(hr = id->pDID->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
			qFatal("GlobalShortcutWin::SetProperty");

		qWarning("Adding device %s %s %s:%d", qPrintable(QUuid(id->guid).toString()),qPrintable(name),qPrintable(sname),id->qhNames.count());

		cbgsw->qhInputDevices[id->guid] = id;
	} else {
		id->pDID->Release();
		delete(id);
	}

	return DIENUM_CONTINUE;
}

void GlobalShortcutWin::timeTicked() {
	if (bNeedRemap)
		remap();

	if (qhInputDevices.isEmpty())
		pDI->EnumDevices(DI8DEVCLASS_ALL, EnumDevicesCB, static_cast<void *>(this), DIEDFL_ALLDEVICES);

	foreach(InputDevice *id, qhInputDevices) {
		DIDEVICEOBJECTDATA rgdod[DX_SAMPLE_BUFFER_SIZE];
		DWORD   dwItems = DX_SAMPLE_BUFFER_SIZE;
		HRESULT hr;

		id->pDID->Acquire();
		id->pDID->Poll();

		hr = id->pDID->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), rgdod, &dwItems, 0);
		if (FAILED(hr))
			continue;

		if (dwItems <= 0)
			continue;

		for (DWORD j=0; j<dwItems; j++) {
			QList<QVariant> ql;

			quint32 uiType = id->qhOfsToType.value(rgdod[j].dwOfs);
			ql << uiType;
			ql << id->vguid;
			handleButton(ql, rgdod[j].dwData & 0x80);
		}
	}
}

QString GlobalShortcutWin::buttonName(const QVariant &v) {
	GlobalShortcutWin *gsw = static_cast<GlobalShortcutWin *>(GlobalShortcutEngine::engine);

	const QList<QVariant> &sublist = v.toList();
	if (sublist.count() != 2)
		return QString();

	bool ok = false;
	DWORD type = sublist.at(0).toUInt(&ok);
	QUuid guid(sublist.at(1).toString());

	if (guid.isNull() || (!ok))
		return QString();

	QString device=guid.toString();
	QString name=QLatin1String("Unknown");
	InputDevice *id = gsw->qhInputDevices.value(guid);
	if (guid == GUID_SysMouse)
		device=QLatin1String("M:");
	else if (guid == GUID_SysKeyboard)
		device=QLatin1String("K:");
	else if (id)
		device=id->name+QLatin1String(":");
	if (id) {
		name=id->qhNames.value(type);
	}
	return device+name;
}

bool GlobalShortcutWin::canSuppress() {
	return true;
}
