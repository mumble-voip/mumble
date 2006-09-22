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


#include "TrayIcon.h"

#define TI_EVENT (WM_USER + 938)
#define TI_QEVENT (QEvent::User + 938)

class TrayIconPrivate {
	public:
		TrayIcon *q;

		TrayIconPrivate(TrayIcon *ti);
		void showIcon();
		void hideIcon();
};

TrayIconPrivate::TrayIconPrivate(TrayIcon *ti) {
	q = ti;
}

void TrayIconPrivate::showIcon() {
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = q->winId();
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_STATE;
	nid.uCallbackMessage = TI_EVENT;
	nid.hIcon = reinterpret_cast<HICON>(LoadImage(GetModuleHandle(NULL), L"IDI_ICON1", IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
	wcscpy(nid.szTip, L"Mumble");
	nid.dwState = 0;
	nid.dwStateMask = NIS_HIDDEN | NIS_SHAREDICON;

	// Ballon Info.. Unused though, as it is annoying to see every time.
	wcscpy(nid.szInfo, L"Mumble is now minimized to the tray");
	nid.uTimeout = 1000;
	wcscpy(nid.szInfoTitle, L"Mumble");
	nid.dwInfoFlags = NIIF_INFO;

	Shell_NotifyIcon(NIM_ADD, &nid);

	nid.uVersion = NOTIFYICON_VERSION;
	Shell_NotifyIcon(NIM_SETVERSION, &nid);
}

void TrayIconPrivate::hideIcon() {
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = q->winId();
	nid.uID = 1;

	Shell_NotifyIcon(NIM_DELETE, &nid);
}

TrayIcon::TrayIcon(QMainWindow *mw) : QWidget(mw) {
	qmwWindow = mw;
	mw->installEventFilter(this);
	bActive = false;
	setVisible(false);

	d = new TrayIconPrivate(this);

	setActive(true);
}

TrayIcon::~TrayIcon() {
	d->hideIcon();
}

// If we just hide in the Hide event, we'll be shown
// minimized immediately.. So postpone until all QT processing
// is done, then just HIDE.

bool TrayIcon::eventFilter(QObject *watched, QEvent *evt) {
	if (watched != qmwWindow)
		return false;

	if (! bActive)
		return false;

	switch (evt->type()) {
		case QEvent::Hide:
			d->showIcon();
			qApp->postEvent(qmwWindow, new QEvent(static_cast<QEvent::Type>(TI_QEVENT)));
			break;
		case TI_QEVENT:
			ShowWindow(qmwWindow->winId(), SW_HIDE);
			return true;
			break;
		case QEvent::Show:
			d->hideIcon();
			qmwWindow->setVisible(true);
			return true;
			break;
		default:
			break;
	}

	return false;
}

bool TrayIcon::isActive() const {
	return bActive;
}

void TrayIcon::setActive(bool active) {
	if (active == bActive)
		return;

	bActive = active;
}

bool TrayIcon::winEvent(MSG *message, long *result) {
	if (message->message != TI_EVENT)
		return false;

	*result = 0;

	switch(message->lParam) {
		case WM_LBUTTONUP:
			qmwWindow->show();
			ShowWindow(qmwWindow->winId(), SW_RESTORE);
			SetForegroundWindow(qmwWindow->winId());
			break;
		default:
			break;
	}

	return true;
}
