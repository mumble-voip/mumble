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

#include "GlobalShortcut.h"
#include "ConfigDialog.h"

typedef QPair<int, unsigned int> qpButton;

struct Shortcut {
	GlobalShortcut *gs;
	int iNumDown;
	bool bActive;
	QList<int> qlButtons;
};

class XInputKeyWidget : public QLineEdit {
		Q_OBJECT
	protected:
		virtual void focusInEvent(QFocusEvent *event);
		virtual void focusOutEvent(QFocusEvent *event);
		virtual void mouseDoubleClickEvent(QMouseEvent *e);
	public:
		QList<int> qlButtons;
		bool bModified;
		XInputKeyWidget(QWidget *p = NULL);
		void setShortcut(QList<int> ql);
	public slots:
		void setButton(bool);
		void displayKeys();
};

class GlobalShortcutXConfig : public ConfigWidget {
		Q_OBJECT
	protected:
		QHash<GlobalShortcut *,XInputKeyWidget *> qhKeys;
	public:
		GlobalShortcutXConfig(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void save() const;
		void load(const Settings &r);
		bool expert(bool);
};

#define NUM_BUTTONS 0x2ff

class GlobalShortcutX : public QThread {
		Q_OBJECT
	public:
		Display *display;
		int ref;
		bool bRunning;
		bool activeMap[NUM_BUTTONS];
		bool touchMap[NUM_BUTTONS];
		bool bFirstMouseReleased;
		QHash<int, GlobalShortcut *> qmShortcuts;
		QHash<GlobalShortcut *, Shortcut *> qhGlobalToX;
		QList<QFile *> qlInputDevices;
		bool globalEvent(XEvent *);
		bool bGrabbing;
		bool bNeedRemap;
		void grab();
		void release();
		QMultiHash<int, Shortcut *> qmhKeyToShortcut;
		void handleEvent(int evtcode, bool down);

		GlobalShortcutX();
		~GlobalShortcutX();
		void run();
		void remap();
		void resetMap();
		void add(GlobalShortcut *);
		void remove(GlobalShortcut *);
		QList<int> getCurrentButtons();
	public slots:
		void inputReadyRead(int);
	signals:
		void buttonPressed(bool);
};
