/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "Global.h"

Global *Global::g_global_struct;

Global::Global() {
	mw = 0;
	sh = 0;
	db = 0;
	p = 0;
	nam = 0;
	uiSession = 0;
	uiDoublePush = 1000000;
	iPushToTalk = 0;
	iAltSpeak = 0;
	bPushToMute = false;
	bCenterPosition = false;
	bEchoTest = false;
	bPosTest = false;
	iAudioPathTime = 0;
	qsRegionalHost = QLatin1String("mumble.info");
	bAttenuateOthers = false;
}

QMultiMap<int, DeferInit *> *DeferInit::qmDeferers = NULL;

void DeferInit::add(int priority) {
	if (qmDeferers == NULL) {
		qmDeferers = new QMultiMap<int, DeferInit *>();
	}
	qmDeferers->insert(priority, this);
}

DeferInit::~DeferInit() {
}

void DeferInit::run_initializers() {
	if (! qmDeferers)
		return;
	foreach(DeferInit *d, *qmDeferers) {
		d->initialize();
	}
}

void DeferInit::run_destroyers() {
	if (! qmDeferers)
		return;
	foreach(DeferInit *d, *qmDeferers) {
		d->destroy();
	}
}
