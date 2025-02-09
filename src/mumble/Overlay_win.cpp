// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Overlay.h"

#include "Channel.h"
#include "MainWindow.h"
#include "MumbleApplication.h"
#include "OverlayConfig.h"
#include "User.h"
#include "Global.h"

#include "Overlay_win.h"

#include "../../overlay/overlay_exe/overlay_exe.h"

// Used by the overlay to detect whether we injected into ourselves.
extern "C" __declspec(dllexport) void mumbleSelfDetection(){};

// Determine if the current Mumble client is able to host
// x64 programs.
//
// If we're on x86, we use use the IsWoW64Process function
// to determine this.  If we're on x64, we already know we're
// capable, so we simply return TRUE.
static bool canRun64BitPrograms() {
#if defined(_M_X64)
	return TRUE;
#elif defined(_M_IX86)
	typedef BOOL(WINAPI * IsWow64ProcessPtr)(HANDLE, BOOL *);
	IsWow64ProcessPtr wow64check = (IsWow64ProcessPtr) GetProcAddress(GetModuleHandle(L"kernel32"), "IsWow64Process");
	if (wow64check) {
		BOOL isWoW64 = FALSE;
		wow64check(GetCurrentProcess(), &isWoW64);
		return isWoW64;
	}
	return FALSE;
#endif
}

OverlayPrivateWin::OverlayPrivateWin(QObject *p)
	: OverlayPrivate(p), m_helper_enabled(true), m_helper64_enabled(true), m_mumble_handle(0), m_active(false) {
	// Acquire a handle to ourselves and duplicate it. We duplicate it because
	// want it to be inheritable by our helper processes, and the handle returned
	// by GetCurrentProcess is not inheritable. Duplicating it makes it inheritable.
	// This allows our helper processes to access the handle.
	//
	// The helper processes need a handle to us, their parent, to be able to listen
	// detect when our process dies.
	//
	// The value of the handle is passed as an argument to the helper processes via
	// the command line as a number. The HANDLE type in Windows is typedef'd to LPVOID,
	// but for handles that are supposed to be shared between processes (like a process
	// handle that we are using), only the lower 32-bits of the HANDLE are considered:
	//
	//   "When sharing a handle between 32-bit and 64-bit applications, only the lower
	//    32 bits are significant [...]"
	//
	// from https://msdn.microsoft.com/en-us/library/aa384203.aspx
	HANDLE curproc = GetCurrentProcess();
	if (!DuplicateHandle(curproc, curproc, curproc, &m_mumble_handle, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
		qFatal("OverlayPrivateWin: unable to duplicate handle to the Mumble process.");
		return;
	}

	m_helper_exe_path =
		QString::fromLatin1("%1/mumble_ol_helper.exe").arg(MumbleApplication::instance()->applicationVersionRootPath());
	m_helper_exe_args << QString::number(OVERLAY_MAGIC_NUMBER)
					  << QString::number(reinterpret_cast< quintptr >(m_mumble_handle));
	m_helper_process = new QProcess(this);

	connect(m_helper_process, SIGNAL(started()), this, SLOT(onHelperProcessStarted()));

	connect(m_helper_process, SIGNAL(error(QProcess::ProcessError)), this,
			SLOT(onHelperProcessError(QProcess::ProcessError)));

	connect(m_helper_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
			SLOT(onHelperProcessExited(int, QProcess::ExitStatus)));

	m_helper_restart_timer = new QTimer(this);
	m_helper_restart_timer->setSingleShot(true);
	connect(m_helper_restart_timer, SIGNAL(timeout()), this, SLOT(onDelayedRestartTimerTriggered()));

	if (!Global::get().s.bOverlayWinHelperX86Enable) {
		qWarning("OverlayPrivateWin: mumble_ol_helper.exe (32-bit overlay helper) disabled via "
				 "'overlay_win/enable_x86_helper' config option.");
		m_helper_enabled = false;
	}

	m_helper64_exe_path = QString::fromLatin1("%1/mumble_ol_helper_x64.exe")
							  .arg(MumbleApplication::instance()->applicationVersionRootPath());
	m_helper64_exe_args = m_helper_exe_args;
	m_helper64_process  = new QProcess(this);

	connect(m_helper64_process, SIGNAL(started()), this, SLOT(onHelperProcessStarted()));

	connect(m_helper64_process, SIGNAL(error(QProcess::ProcessError)), this,
			SLOT(onHelperProcessError(QProcess::ProcessError)));

	connect(m_helper64_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
			SLOT(onHelperProcessExited(int, QProcess::ExitStatus)));

	m_helper64_restart_timer = new QTimer(this);
	m_helper64_restart_timer->setSingleShot(true);
	connect(m_helper64_restart_timer, SIGNAL(timeout()), this, SLOT(onDelayedRestartTimerTriggered()));

	if (!canRun64BitPrograms()) {
		qWarning("OverlayPrivateWin: mumble_ol_helper_x64.exe (64-bit overlay helper) disabled because the host is not "
				 "x64 capable.");
		m_helper64_enabled = false;
	} else if (!Global::get().s.bOverlayWinHelperX64Enable) {
		qWarning("OverlayPrivateWin: mumble_ol_helper_x64.exe (64-bit overlay helper) disabled via "
				 "'overlay_win/enable_x64_helper' config option.");
		m_helper64_enabled = false;
	}
}

OverlayPrivateWin::~OverlayPrivateWin() {
	m_active = false;

	if (!CloseHandle(m_mumble_handle)) {
		qFatal("OverlayPrivateWin: unable to close Mumble process handle.");
		return;
	}

	// Remove all signals, so they don't
	// interfere with our calls to waitForFinished
	// below.
	m_helper_process->disconnect();
	m_helper64_process->disconnect();

	m_helper_process->terminate();
	m_helper64_process->terminate();

	m_helper_process->waitForFinished();
	m_helper64_process->waitForFinished();
}

void OverlayPrivateWin::startHelper(QProcess *helper) {
	if (helper->state() == QProcess::NotRunning) {
		if (helper == m_helper_process) {
			helper->start(m_helper_exe_path, m_helper_exe_args);
			m_helper_start_time.restart();
		} else if (helper == m_helper64_process) {
			helper->start(m_helper64_exe_path, m_helper64_exe_args);
			m_helper64_start_time.restart();
		} else {
			qFatal("OverlayPrivateWin: invalid helper passed to startHelper().");
		}
	} else {
		qWarning("OverlayPrivateWin: startHelper() called while process is already running. skipping.");
	}
}

void OverlayPrivateWin::setActive(bool active) {
	if (m_active != active) {
		m_active = active;

		if (m_active) {
			if (m_helper_enabled) {
				startHelper(m_helper_process);
			}
			if (m_helper64_enabled) {
				startHelper(m_helper64_process);
			}
		} else {
			if (m_helper_enabled) {
				m_helper_process->terminate();
			}
			if (m_helper64_enabled) {
				m_helper64_process->terminate();
			}
		}
	}
}

static const char *exitStatusString(QProcess::ExitStatus exitStatus) {
	switch (exitStatus) {
		case QProcess::NormalExit:
			return "normal exit";
		case QProcess::CrashExit:
			return "crash";
	}

	return "unknown";
}

static const char *processErrorString(QProcess::ProcessError processError) {
	switch (processError) {
		case QProcess::FailedToStart:
			return "process failed to start";
		case QProcess::Crashed:
			return "process crashed";
		case QProcess::Timedout:
			return "process wait operation timed out";
		case QProcess::WriteError:
			return "an error occurred when attempting to write to the process";
		case QProcess::ReadError:
			return "an error occurred when attempting to read from the process";
		case QProcess::UnknownError:
			return "an unknown error occurred";
	}

	return "unknown";
}

void OverlayPrivateWin::onHelperProcessStarted() {
	QProcess *helper = qobject_cast< QProcess * >(sender());
	QString path;
	if (helper == m_helper_process) {
		path = m_helper_exe_path;
	} else if (helper == m_helper64_process) {
		path = m_helper64_exe_path;
	} else {
		qFatal("OverlayPrivateWin: unknown QProcess found in onHelperProcessStarted().");
	}

	std::uint64_t processID = helper->processId();
	qWarning("OverlayPrivateWin: overlay helper process '%s' started with PID %llu.", qPrintable(path), processID);
}

void OverlayPrivateWin::onHelperProcessError(QProcess::ProcessError processError) {
	QProcess *helper = qobject_cast< QProcess * >(sender());
	QString path;
	if (helper == m_helper_process) {
		path = m_helper_exe_path;
	} else if (helper == m_helper64_process) {
		path = m_helper64_exe_path;
	} else {
		qFatal("OverlayPrivateWin: unknown QProcess found in onHelperProcessError().");
	}

	qWarning("OverlayPrivateWin: an error occurred for overlay helper process '%s': %s", qPrintable(path),
			 processErrorString(processError));
}

void OverlayPrivateWin::onHelperProcessExited(int exitCode, QProcess::ExitStatus exitStatus) {
	QProcess *helper = qobject_cast< QProcess * >(sender());

	QString path;
	qint64 elapsedMsec   = 0;
	QTimer *restartTimer = nullptr;
	if (helper == m_helper_process) {
		path         = m_helper_exe_path;
		elapsedMsec  = m_helper_start_time.elapsed();
		restartTimer = m_helper_restart_timer;
	} else if (helper == m_helper64_process) {
		path         = m_helper64_exe_path;
		elapsedMsec  = m_helper64_start_time.elapsed();
		restartTimer = m_helper64_restart_timer;
	} else {
		qFatal("OverlayPrivateWin: unknown QProcess found in onHelperProcessExited().");
	}

	const char *helperErrString = OverlayHelperErrorToString(static_cast< OverlayHelperError >(exitCode));
	qWarning("OverlayPrivateWin: overlay helper process '%s' exited (%s) with status code %s.", qPrintable(path),
			 exitStatusString(exitStatus), helperErrString ? helperErrString : qPrintable(QString::number(exitCode)));

	// If the helper process exited while we're in 'active'
	// mode, restart it.
	if (m_active) {
		// If the helper was only recently started, be
		// a little more patient with restarting it.
		// We could be hitting a crash bug in the helper,
		// and we don't want to do too much harm in that
		// case by spawning thousands of processes.
		qint64 cooldownMsec = (qint64) Global::get().s.iOverlayWinHelperRestartCooldownMsec;
		if (elapsedMsec < cooldownMsec) {
			qint64 delayMsec = cooldownMsec - elapsedMsec;
			qWarning("OverlayPrivateWin: waiting %llu seconds until restarting helper process '%s'. last restart was "
					 "%llu seconds ago.",
					 (unsigned long long) delayMsec / 1000ULL, qPrintable(path),
					 (unsigned long long) elapsedMsec / 1000ULL);
			if (!restartTimer->isActive()) {
				restartTimer->start(delayMsec);
			}
		} else {
			startHelper(helper);
		}
	}
}

void OverlayPrivateWin::onDelayedRestartTimerTriggered() {
	if (!m_active) {
		return;
	}

	QTimer *timer = qobject_cast< QTimer * >(sender());

	QProcess *helper = nullptr;
	if (timer == m_helper_restart_timer) {
		helper = m_helper_process;
	} else if (timer == m_helper64_restart_timer) {
		helper = m_helper64_process;
	} else {
		qFatal("OverlayPrivateWin: unknown timer found in onDelayedRestartTimerTriggered().");
	}

	if (helper->state() == QProcess::NotRunning) {
		startHelper(helper);
	}
}

void Overlay::platformInit() {
	d = new OverlayPrivateWin(this);
}

void Overlay::setActiveInternal(bool act) {
	if (d) {
		// Only act if the private instance has been created already
		static_cast< OverlayPrivateWin * >(d)->setActive(act);
	}
}

bool OverlayConfig::supportsInstallableOverlay() {
	return false;
}

bool OverlayConfig::isInstalled() {
	return true;
}

bool OverlayConfig::needsUpgrade() {
	return false;
}

bool OverlayConfig::installFiles() {
	return false;
}

bool OverlayConfig::uninstallFiles() {
	return false;
}
