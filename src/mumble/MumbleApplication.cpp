// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "MumbleApplication.h"

#include "MainWindow.h"
#include "GlobalShortcut.h"
#include "Global.h"
#include "EnvUtils.h"

#ifdef Q_OS_WIN
# include "GlobalShortcut_win.h"
#endif

MumbleApplication *MumbleApplication::instance() {
	return static_cast<MumbleApplication *>(QCoreApplication::instance());
}

MumbleApplication::MumbleApplication(int &pargc, char **pargv)
    : QApplication(pargc, pargv) {
	
	connect(this,
	        SIGNAL(commitDataRequest(QSessionManager&)),
	        SLOT(onCommitDataRequest(QSessionManager&)),
	        Qt::DirectConnection);
}

QString MumbleApplication::applicationVersionRootPath() {
	QString versionRoot = EnvUtils::getenv(QLatin1String("MUMBLE_VERSION_ROOT"));
	if (versionRoot.count() > 0) {
		return versionRoot;
	}
	return this->applicationDirPath();
}

void MumbleApplication::onCommitDataRequest(QSessionManager &) {
	// Make sure the config is saved and supress the ask on quit message
	if (g.mw) {
		g.s.save();
		g.mw->bSuppressAskOnQuit = true;
		qWarning() << "Session likely ending. Suppressing ask on quit";
	}
}

bool MumbleApplication::event(QEvent *e) {
	if (e->type() == QEvent::FileOpen) {
		QFileOpenEvent *foe = static_cast<QFileOpenEvent *>(e);
		if (! g.mw) {
			this->quLaunchURL = foe->url();
		} else {
			g.mw->openUrl(foe->url());
		}
		return true;
	}
	return QApplication::event(e);
}

#ifdef Q_OS_WIN

static bool handleWMKeyMessage(MSG *msg) {
	GlobalShortcutWin *gsw = static_cast<GlobalShortcutWin *>(GlobalShortcutEngine::engine);
	if (gsw == NULL) {
		return false;
	}

	DWORD scancode = (msg->lParam >> 16) & 0xff;
	DWORD vkcode = msg->wParam;
	bool extended = !!(msg->lParam & 0x01000000);
	bool up = !!(msg->lParam & 0x80000000);

	return gsw->injectKeyMessage(scancode, vkcode, extended, up);
}

bool handleWMMouseMessage(MSG *msg) {
	GlobalShortcutWin *gsw = static_cast<GlobalShortcutWin *>(GlobalShortcutEngine::engine);
	if (gsw == NULL) {
		return false;
	}

	bool down = false;
	unsigned int btn = 0;

	switch (msg->message) {
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
		case WM_XBUTTONUP: {
			unsigned int offset = (msg->wParam >> 16) & 0xffff;
			btn = 5 + offset;
		}
		default:
			// Non-mouse event. Return early.
			return false;
	}

	return gsw->injectMouseMessage(btn, down);
}

# if QT_VERSION >= 0x050000
bool MumbleApplication::nativeEventFilter(const QByteArray &, void *message, long *) {
	MSG *msg = reinterpret_cast<MSG *>(message);

	if (QThread::currentThread() == thread()) {
		if (Global::g_global_struct) {
			switch (msg->message) {
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
				case WM_MBUTTONDOWN:
				case WM_MBUTTONUP:
				case WM_XBUTTONDOWN:
				case WM_XBUTTONUP:
					if (handleWMMouseMessage(msg)) {
						// Suppress
						return true;
					}
					break;
				case WM_KEYDOWN:
				case WM_KEYUP:
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
					if (handleWMKeyMessage(msg)) {
						// Suppress
						return true;
					}
					break;
				default:
					break;
			}
		}
	}
	return false;
}
# else
bool MumbleApplication::winEventFilter(MSG *msg, long *result) {
	if (QThread::currentThread() == thread()) {
		if (Global::g_global_struct) {
			switch (msg->message) {
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
				case WM_MBUTTONDOWN:
				case WM_MBUTTONUP:
				case WM_XBUTTONDOWN:
				case WM_XBUTTONUP:
					if (handleWMMouseMessage(msg)) {
						// Suppress
						return true;
					}
					break;
				case WM_KEYDOWN:
				case WM_KEYUP:
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
					if (handleWMKeyMessage(msg)) {
						// Suppress
						return true;
					}
					break;
				default:
					break;
			}
		}
	}
	return QApplication::winEventFilter(msg, result);
}
# endif
#endif
