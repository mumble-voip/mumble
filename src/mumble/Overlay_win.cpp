/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#include "User.h"
#include "Channel.h"
#include "Overlay.h"
#include "MainWindow.h"
#include "Global.h"

typedef unsigned int (__cdecl *GetOverlayMagicVersionProc)();
typedef void (__cdecl *PrepProc)();

// Used by the overlay to detect whether we injected into ourselves.
//
// A similar declaration can be found in mumble_exe's Overlay.cpp,
// for the overlay's self-detection checks to continue working in a
// mumble_app.dll world.
extern "C" __declspec(dllexport) void mumbleSelfDetection() {};

OverlayPrivateWin::OverlayPrivateWin(QObject *p) : OverlayPrivate(p) {

	hpInstall = NULL;
	hpRemove = NULL;
	qlOverlay = new QLibrary(this);

	QString path = QString::fromLatin1("%1/mumble_ol.dll").arg(qApp->applicationDirPath());
	qlOverlay->setFileName(path);
	if (! qlOverlay->load()) {
		QMessageBox::critical(NULL, QLatin1String("Mumble"), tr("Failed to load overlay library. This means either that:\n"
		                      "- the library (mumble_ol.dll) wasn't found in the directory you ran Mumble from\n"
		                      "- you're on an OS earlier than WinXP SP2"), QMessageBox::Ok, QMessageBox::NoButton);
		qWarning("Overlay failure");
		return;
	}

	GetOverlayMagicVersionProc gompvp = (GetOverlayMagicVersionProc)qlOverlay->resolve("GetOverlayMagicVersion");
	if (! gompvp) {
		qWarning("The overlay librarys overlay protocol version could not be verified. Overlay will not be enabled.");
		return;
	}

	if (gompvp() != OVERLAY_MAGIC_NUMBER) {
		qWarning("Client overlay protocol version does not match the overlay library one. Overlay will not be enabled.");
		return;
	}

	hpInstall = (HooksProc)qlOverlay->resolve("InstallHooks");
	hpRemove = (HooksProc)qlOverlay->resolve("RemoveHooks");
	PrepProc prepareProc9 = (PrepProc) qlOverlay->resolve("PrepareD3D9");
	PrepProc prepareProcDXGI = (PrepProc) qlOverlay->resolve("PrepareDXGI");

	if (prepareProc9)
		prepareProc9();

	if (prepareProcDXGI)
		prepareProcDXGI();
}

OverlayPrivateWin::~OverlayPrivateWin() {
	qlOverlay->unload();
}

void OverlayPrivateWin::setActive(bool act) {
	if (act && hpInstall)
		hpInstall();
	else if (! act && hpRemove)
		hpRemove();
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
