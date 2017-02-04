// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "MumbleApplication.h"

#include "MainWindow.h"
#include "GlobalShortcut.h"
#include "Global.h"

// getenvQString is a wrapper around _wgetenv_s (on Windows)
// and getenv (on everything else).
//
// On Windows, it expects a Unicode environment -- so variables
// are expected to be UTF16.
// On everthing else, it expects the environment variables to be
// UTF-8 encoded.
static QString getenvQString(QString name) {
#ifdef Q_OS_WIN
	QByteArray buf;
	size_t requiredSize = 0;

	static_assert(sizeof(wchar_t) == sizeof(ushort), "expected 2-byte wchar_t");

	const wchar_t *wname = reinterpret_cast<const wchar_t *>(name.utf16());

	// Query the required buffer size (in elements).
	_wgetenv_s(&requiredSize, 0, 0, wname);
	if (requiredSize == 0) {
		return QString();
	}

	// Resize buf to fit the value and put it there.
	buf.resize(static_cast<int>(requiredSize * sizeof(wchar_t)));
	_wgetenv_s(&requiredSize, reinterpret_cast<wchar_t *>(buf.data()), requiredSize, wname);

	// Convert the value to QString and return it.
	const wchar_t *wbuf = reinterpret_cast<const wchar_t *>(buf.constData());
	return QString::fromWCharArray(wbuf);
#else
	QByteArray nameU8 = name.toUtf8();
	char *val = ::getenv(nameU8.constData());
	if (val == NULL) {
		return QString();
	}
	return QString::fromUtf8(val);
#endif
}

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
	QString versionRoot = getenvQString(QLatin1String("MUMBLE_VERSION_ROOT"));
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
bool MumbleApplication::nativeEventFilter(const QByteArray &eventType, void *message, long *result) {
	Q_UNUSED(eventType);
	MSG *msg = reinterpret_cast<MSG *>(message);

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
