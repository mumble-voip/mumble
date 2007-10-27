/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

GlobalShortcutWin::GlobalShortcutWin() {
	engine = this;
	ref = 0;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeTicked()));

	HRESULT hr;

	pDI = NULL;

	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void **>(&pDI), NULL))) {
		qFatal("GlobalShortcutWin: Failed to create d8input");
		return;
	}

	remap();

	timer->start(20);
}

BOOL CALLBACK GlobalShortcutWin::EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef) {
	InputDevice *id=static_cast<InputDevice *>(pvRef);
	QString name = QString::fromUtf16(reinterpret_cast<const ushort *>(lpddoi->tszName));
//	qWarning("%08lx %08lx %s",lpddoi->dwOfs,lpddoi->dwType, qPrintable(name));
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
//			qWarning("%4x %4x %s", dwType, dwOfs, qPrintable(id->qhNames[dwType]));
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

void GlobalShortcutWin::add(GlobalShortcut *gs) {
	qmShortcuts[gs->idx] = gs;
	bNeedRemap = true;
}

void GlobalShortcutWin::remove(GlobalShortcut *gs) {
	qmShortcuts.remove(gs->idx);
	bNeedRemap = true;
}

void GlobalShortcutWin::remap() {
	bNeedRemap = false;

	unacquire();

	if (qmShortcuts.isEmpty())
		return;

	foreach(InputDevice *id, qhInputDevices) {
		if (id->pDID) {
			id->pDID->Release();
		}
		delete(id);
	}
	qhInputDevices.clear();

	pDI->EnumDevices(DI8DEVCLASS_ALL, EnumDevicesCB, static_cast<void *>(this), DIEDFL_ALLDEVICES);

	foreach(Shortcut *s, qhGlobalToWin) {
		delete(s);
	}
	qhGlobalToWin.clear();

	foreach(GlobalShortcut *gs, qmShortcuts) {
		const QList<QVariant> &ql = g.s.qmShortcuts.value(gs->idx);
		QList<QUuid> guids;
		QList<DWORD> types;
		foreach(QVariant b, ql) {
			const QList<QVariant> &l = b.toList();
			if (l.count() != 2)
				continue;
			QUuid guid(l.at(0).toString());
			DWORD type = l.at(1).toUInt();
			if (! guid.isNull()) {
				guids << guid;
				types << type;
			}
		}
		if (guids.count() > 0) {
			Shortcut *s = new Shortcut();
			s->gs = gs;
			s->bActive = false;
			s->iNumDown = 0;
			for (int i=0;i<guids.count();i++) {
				s->qlButtons << qpButton(guids[i], types[i]);
				s->qlActive << false;
				if (qhInputDevices.contains(guids[i])) {
					InputDevice *id = qhInputDevices[guids[i]];
					id->qmhOfsToShortcut.insert(id->qhTypeToOfs[types[i]], s);
				}
			}
			qhGlobalToWin[gs] = s;
		}
	}
}

GlobalShortcutWin::~GlobalShortcutWin() {
	unacquire();

	foreach(InputDevice *id, qhInputDevices) {
		if (id->pDID) {
			id->pDID->Release();
		}
	}
	pDI->Release();
}

void GlobalShortcutWin::unacquire() {
	foreach(InputDevice *id, qhInputDevices) {
		if (id->pDID) {
			id->pDID->Unacquire();
		}
	}
}

void GlobalShortcutWin::timeTicked() {
	bool pressed = false;

	if (bNeedRemap)
		remap();


	foreach(InputDevice *id, qhInputDevices) {
		DIDEVICEOBJECTDATA rgdod[DX_SAMPLE_BUFFER_SIZE];
		DWORD   dwItems = DX_SAMPLE_BUFFER_SIZE;
		HRESULT hr;

		id->pDID->Acquire();
		id->pDID->Poll();

		hr = id->pDID->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), rgdod, &dwItems, 0);
		if (FAILED(hr))
			continue;

		for (DWORD j=0; j<dwItems; j++) {
//			qWarning("%4x %4x", rgdod[j].dwOfs, rgdod[j].dwData);

			if (!bIgnoreActive) {
				if (rgdod[j].dwData & 0x80) {
					iButtonsDown++;
					id->activeMap.insert(rgdod[j].dwOfs);
					pressed = true;
				} else if (id->activeMap.contains(rgdod[j].dwOfs)) {
					iButtonsDown--;
					if (iButtonsDown == 0)
						pressed = true;
				}
			}

			foreach(Shortcut *s, id->qmhOfsToShortcut.values(rgdod[j].dwOfs)) {
				qpButton button(id->guid, id->qhOfsToType[rgdod[j].dwOfs]);
				int idx;
				for (idx=0;idx<s->qlButtons.count();idx++) {
					if (s->qlButtons[idx] == button) {
						bool a = (rgdod[j].dwData & 0x80);
						if (a != s->qlActive[idx]) {
							s->qlActive[idx] = a;
							if (!a && s->bActive) {
								s->iNumDown--;
								s->bActive = false;
								emit s->gs->triggered(a);
								s->gs->act = false;
								emit s->gs->up();
							} else if (!a) {
								s->iNumDown--;
							} else if (a) {
								s->iNumDown++;
								if (s->iNumDown == s->qlActive.count()) {
									s->bActive = true;
									emit s->gs->triggered(a);
									s->gs->act = true;
									emit s->gs->down();
								}
							}
						}
					}
				}
			}
		}
	}

	if (pressed)
		emit buttonPressed(iButtonsDown == 0);
	bIgnoreActive = false;
}

void GlobalShortcutWin::resetMap() {
	foreach(InputDevice *id, qhInputDevices) {
		id->activeMap.clear();
	}
	bIgnoreActive = true;
	iButtonsDown = 0;
}

QList<QVariant> GlobalShortcutWin::getCurrentButtons() {
	QList<QVariant> buttons;
	foreach(InputDevice *id, qhInputDevices) {
		foreach(DWORD ofs, id->activeMap) {
			QList<QVariant> sublist;
			sublist << QUuid(id->guid).toString();
			sublist << static_cast<unsigned int>(id->qhOfsToType[ofs]);
			buttons << QVariant(sublist);
		}
	}
	return buttons;
}

QString GlobalShortcutWin::buttonName(const QVariant &v) {
	GlobalShortcutWin *gsw = static_cast<GlobalShortcutWin *>(GlobalShortcutEngine::engine);

	const QList<QVariant> &sublist = v.toList();
	if (sublist.count() != 2)
		return QString();

	QUuid guid(sublist.at(0).toString());
	bool ok = false;
	DWORD type = sublist.at(1).toUInt(&ok);

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


GlobalShortcut::GlobalShortcut(QObject *p, int index, QString qsName) : QObject(p) {
	if (! GlobalShortcutEngine::engine)
		GlobalShortcutEngine::engine = new GlobalShortcutWin();

	GlobalShortcutWin *gsw = static_cast<GlobalShortcutWin *>(GlobalShortcutEngine::engine);
	gsw->ref++;
	idx = index;
	name=qsName;
	act = false;
	gsw->add(this);
}

GlobalShortcut::~GlobalShortcut() {
	GlobalShortcutWin *gsw = static_cast<GlobalShortcutWin *>(GlobalShortcutEngine::engine);

	gsw->remove(this);
	gsw->ref--;
	if (gsw->ref == 0) {
		delete gsw;
		GlobalShortcutEngine::engine = NULL;
	}
}
