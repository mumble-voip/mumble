// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OVERLAY_WIN_H_
#define MUMBLE_MUMBLE_OVERLAY_WIN_H_

#include "Overlay.h"

#include "win.h"

#include <QElapsedTimer>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QTimer>

class OverlayPrivateWin : public OverlayPrivate {
private:
	Q_OBJECT
	Q_DISABLE_COPY(OverlayPrivateWin)

public:
	void setActive(bool);
	OverlayPrivateWin(QObject *);
	~OverlayPrivateWin();

public slots:
	void onHelperProcessStarted();
	void onHelperProcessError(QProcess::ProcessError);
	void onHelperProcessExited(int exitCode, QProcess::ExitStatus exitStatus);
	void onDelayedRestartTimerTriggered();

protected:
	QProcess *m_helper_process;
	QString m_helper_exe_path;
	QStringList m_helper_exe_args;
	QElapsedTimer m_helper_start_time;
	QTimer *m_helper_restart_timer;
	bool m_helper_enabled;

	QProcess *m_helper64_process;
	QString m_helper64_exe_path;
	QStringList m_helper64_exe_args;
	QElapsedTimer m_helper64_start_time;
	QTimer *m_helper64_restart_timer;
	bool m_helper64_enabled;

	HANDLE m_mumble_handle;
	bool m_active;

	void startHelper(QProcess *helper);
};

#endif
