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
# if QT_VERSION >= 0x050000
bool MumbleApplication::nativeEventFilter(const QByteArray &, void *message, long *) {
	MSG *msg = reinterpret_cast<MSG *>(message);

	if (QThread::currentThread() == thread()) {
		if (Global::g_global_struct) {
			switch (msg->message) {
				case WM_KEYDOWN:
				case WM_KEYUP:
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP: {
						GlobalShortcutWin *gsw = reinterpret_cast<GlobalShortcutWin *>(GlobalShortcutEngine::engine);
						DWORD scancode = (msg->lParam >> 16) & 0xff;
						DWORD vkcode = msg->wParam;
						bool extended = !!(msg->lParam & 0x01000000);
						bool up = !!(msg->lParam & 0x80000000);

						gsw->injectKeyPress(scancode, vkcode, extended, up);
					}
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
		if (Global::g_global_struct && g.ocIntercept) {
			switch (msg->message) {
				case WM_MOUSELEAVE:
					*result = 0;
					return true;
				case WM_KEYDOWN:
				case WM_KEYUP:
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
					GlobalShortcutEngine::engine->prepareInput();
				default:
					break;
			}
		}
	}
	return QApplication::winEventFilter(msg, result);
}
# endif
#endif
