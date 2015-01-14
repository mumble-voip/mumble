/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2015, Mikkel Krautz <mikkel@krautz.dk>

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

#include "mumble_pch.hpp"

#include "Overlay.h"

#include "User.h"
#include "Channel.h"
#include "OverlayConfig.h"
#include "MainWindow.h"
#include "Global.h"

#include "Overlay_win.h"

// Used by the overlay to detect whether we injected into ourselves.
//
// A similar declaration can be found in mumble_exe's Overlay.cpp,
// for the overlay's self-detection checks to continue working in a
// mumble_app.dll world.
extern "C" __declspec(dllexport) void mumbleSelfDetection() {};

OverlayPrivateWin::OverlayPrivateWin(QObject *p) : OverlayPrivate(p) {
	m_helper_exe_path = QString::fromLatin1("%1/mumble_ol.exe").arg(qApp->applicationDirPath());
	m_helper_exe_args = QStringList(QString::number(OVERLAY_MAGIC_NUMBER));
	m_helper_process = new QProcess(this);

	connect(m_helper_process, SIGNAL(started()),
	        this, SLOT(helperProcessStarted()));

	connect(m_helper_process, SIGNAL(finished(int, QProcess::ExitStatus)),
	        this, SLOT(helperProcessExited(int, QProcess::ExitStatus)));
}

OverlayPrivateWin::~OverlayPrivateWin() {
	m_active = false;
}

void OverlayPrivateWin::startHelper(QProcess *helper) {
	if (helper->state() == QProcess::NotRunning) {
		helper->start(m_helper_exe_path, m_helper_exe_args);
	} else {
		qWarning("OverlayPrivateWin: startHelper() called while process is already running. skipping.");
	}
}

void OverlayPrivateWin::setActive(bool active) {
	if (m_active != active) {
		m_active = active;

		if (m_active) {
			startHelper(m_helper_process);
		} else {
			m_helper_process->terminate();
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

void OverlayPrivateWin::helperProcessStarted() {
	QProcess *helper = qobject_cast<QProcess *>(sender());

	PROCESS_INFORMATION *pi = helper->pid();
	qWarning("OverlayPrivateWin: overlay helper process '%s' started with PID %llu.",
	         qPrintable(m_helper_exe_path), static_cast<unsigned long long>(pi->dwProcessId));
}

void OverlayPrivateWin::helperProcessExited(int exitCode, QProcess::ExitStatus exitStatus) {
	QProcess *helper = qobject_cast<QProcess *>(sender());

	qWarning("OverlayPrivateWin: overlay helper process exited (%s) with status code %i.",
	         exitStatusString(exitStatus),
	         exitCode);

	// If the helper process exited while we're in 'active'
	// mode, restart it.
	if (m_active) {
			startHelper(helper);
	}
}

void Overlay::platformInit() {
	d = new OverlayPrivateWin(this);
}

void Overlay::setActive(bool act) {
	static_cast<OverlayPrivateWin *>(d)->setActive(act);
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