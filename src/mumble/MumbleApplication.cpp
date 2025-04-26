// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MumbleApplication.h"

#include "EnvUtils.h"
#include "MainWindow.h"
#include "Global.h"
#include "GlobalShortcut.h"

#if defined(Q_OS_WIN)
#	include "GlobalShortcut_win.h"
#endif

#include <QtGui/QFileOpenEvent>

MumbleApplication *MumbleApplication::instance() {
	return static_cast< MumbleApplication * >(QCoreApplication::instance());
}

MumbleApplication::MumbleApplication(int &pargc, char **pargv) : QApplication(pargc, pargv) {
	connect(this, SIGNAL(commitDataRequest(QSessionManager &)), SLOT(onCommitDataRequest(QSessionManager &)),
			Qt::DirectConnection);
}

QString MumbleApplication::applicationVersionRootPath() {
	QString versionRoot = EnvUtils::getenv(QLatin1String("MUMBLE_VERSION_ROOT"));
	if (!versionRoot.isEmpty()) {
		return versionRoot;
	}
	return this->applicationDirPath();
}

void MumbleApplication::onCommitDataRequest(QSessionManager &) {
	// Make sure the config is saved and suppress the ask on quit message
	if (Global::get().mw) {
		Global::get().s.mumbleQuitNormally = true;
		Global::get().s.save();
		Global::get().mw->forceQuit = true;
		qWarning() << "Session likely ending. Suppressing ask on quit";
	}
}

bool MumbleApplication::event(QEvent *e) {
	if (e->type() == QEvent::FileOpen) {
		QFileOpenEvent *foe = static_cast< QFileOpenEvent * >(e);
		if (!Global::get().mw) {
			this->quLaunchURL = foe->url();
		} else {
			Global::get().mw->openUrl(foe->url());
		}
		return true;
	}
	return QApplication::event(e);
}

#ifdef Q_OS_WIN
bool MumbleApplication::nativeEventFilter(const QByteArray &, void *message, qintptr *) {
	auto gsw = static_cast< GlobalShortcutWin * >(GlobalShortcutEngine::engine);
	if (!gsw) {
		return false;
	}

	auto msg = reinterpret_cast< const MSG * >(message);
	switch (msg->message) {
		case WM_INPUT:
			gsw->injectRawInputMessage(reinterpret_cast< HRAWINPUT >(msg->lParam));
			break;
		case WM_INPUT_DEVICE_CHANGE:
			// We don't care about GIDC_ARRIVAL because we add a device only when we receive input from it.
			if (msg->wParam == GIDC_REMOVAL) {
				// The device is not available anymore, free resources allocated for it.
				gsw->deviceRemoved(reinterpret_cast< const HANDLE >(msg->lParam));
			}
	}

	return false;
}
#endif
