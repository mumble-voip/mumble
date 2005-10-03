/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#include <QMessageBox>
#include <QApplication>
#include "Player.h"
#include "Overlay.h"

Overlay::Overlay() : QObject() {
	qlOverlay = new QLibrary(this);

#ifndef QT_NO_DEBUG
	QString path="../overlay/mumble_ol.dll";
#else
	QString path=QString("%1/mumble_ol.dll").arg(qApp->applicationDirPath());
#endif

	qlOverlay->setFileName(path);
	if (! qlOverlay->load()) {
		QMessageBox::critical(NULL, tr("Mumble"), tr("Failed to load overlay library."), QMessageBox::Ok, QMessageBox::NoButton);
		qFatal("Overlay failure");
	}

	sm=reinterpret_cast<SharedMem *>(qlOverlay->resolve("sm"));

	hpInstall = (HooksProc)qlOverlay->resolve("InstallHooks");
	hpRemove = (HooksProc)qlOverlay->resolve("RemoveHooks");

	qtTimer=new QTimer(this);
	qtTimer->setObjectName("Timer");
	qtTimer->start();

	hMutex = CreateMutex(NULL, false, L"MumbleSharedMutex");

	QMetaObject::connectSlotsByName(this);
}

Overlay::~Overlay() {
	setActive(false);
	qlOverlay->unload();
}

bool Overlay::isActive() const {
	return sm->bHooked;
}

void Overlay::setActive(bool act) {
	if (act)
		hpInstall();
	else
		hpRemove();
}

void Overlay::on_Timer_timeout() {
	sm->lastAppAlive = GetTickCount();
}

void Overlay::setPlayers(QList<Player *> players) {
	DWORD dwWaitResult = WaitForSingleObject(hMutex, 500L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		int idx = 0;
		foreach(Player *p, players) {
			sm->players[idx].bTalking = p->bTalking;
			QString str = p->qsName.left(127);
			const wchar_t *wstr = reinterpret_cast<const wchar_t *>(str.utf16());
			wcscpy(sm->players[idx].name, wstr);
			idx++;
		}
		for(; idx<32;idx++)
			sm->players[idx].name[0] = 0;
		ReleaseMutex(hMutex);
	}
}
