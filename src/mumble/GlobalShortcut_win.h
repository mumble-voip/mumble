/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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

#include "GlobalShortcut.h"
#include "ConfigDialog.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

typedef QPair<GUID, DWORD> qpButton;

struct Shortcut {
	GlobalShortcut *gs;
	bool bActive;
	int iNumDown;
	QList<qpButton> qlButtons;
	QList<bool> qlActive;
};

struct InputDevice {
	LPDIRECTINPUTDEVICE8 pDID;
	GUID guid;
	QString name;

	// dwType to name
	QHash<DWORD, QString> qhNames;

	// Map dwType to dwOfs in our structure
	QHash<DWORD, DWORD> qhTypeToOfs;

	// Map dwOfs in our structure to dwType
	QHash<DWORD, DWORD> qhOfsToType;

	// Map dwOfs for this device to possible matching shortcut
	QMultiHash<DWORD, Shortcut *> qmhOfsToShortcut;

	// Buttons active since last reset
	QSet<DWORD> activeMap;
};

class DirectInputKeyWidget : public QLineEdit {
	Q_OBJECT
	protected:
		virtual void focusInEvent(QFocusEvent *event);
		virtual void focusOutEvent(QFocusEvent *event);
		virtual void mouseDoubleClickEvent(QMouseEvent *e);
	public:
		QList<qpButton> qlButtons;
		bool bModified;
		DirectInputKeyWidget(QWidget *p = NULL);
		void setShortcut(GlobalShortcut *gs);
	public slots:
		void updateKeys(bool last);
		void displayKeys();
};

class GlobalShortcutWinConfig : public ConfigWidget {
	Q_OBJECT
	protected:
		QHash<GlobalShortcut *,DirectInputKeyWidget *> qhKeys;
	public:
		GlobalShortcutWinConfig(QWidget *p = NULL);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void accept();
};

class GlobalShortcutWin : public QObject {
	Q_OBJECT
	public:
		QTimer *timer;
		int ref;

		LPDIRECTINPUT8 pDI;
		QHash<int, GlobalShortcut *> qmShortcuts;
		QHash<GlobalShortcut *, Shortcut *> qhGlobalToWin;
		QHash<GUID, InputDevice *> qhInputDevices;
		static BOOL CALLBACK EnumSuitableDevicesCB(LPCDIDEVICEINSTANCE, LPDIRECTINPUTDEVICE8, DWORD, DWORD, LPVOID);
		static BOOL CALLBACK EnumDevicesCB(LPCDIDEVICEINSTANCE, LPVOID);
		static BOOL CALLBACK EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
		bool bNeedRemap;
		bool bIgnoreActive;
		int iButtonsDown;
	public slots:
		void timeTicked();
	signals:
		void buttonPressed(bool last);
	public:
		GlobalShortcutWin();
		~GlobalShortcutWin();
		void unacquire();
		void remap();
		void add(GlobalShortcut *);
		void remove(GlobalShortcut *);
		void resetMap();
		QList<qpButton> getCurrentButtons();
};

uint qHash(const GUID &);
