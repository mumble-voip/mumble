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

#ifndef _UNIXMURMUR_H
#define _UNIXMURMUR_H

#include "murmur_pch.h"

class LimitTest : public QThread {
		Q_OBJECT
		Q_DISABLE_COPY(LimitTest)
	protected:
		static QMutex *qm;
		static QWaitCondition *qw, *qstartw;
		LimitTest();
	public:
		int tid;
		void run();
		static void testLimits(QCoreApplication &);
};

class UnixMurmur : public QObject {
		Q_OBJECT
		Q_DISABLE_COPY(UnixMurmur)
	protected:
		bool bRoot;
		static int iHupFd[2], iTermFd[2];
		QSocketNotifier *qsnHup, *qsnTerm;

		static void hupSignalHandler(int);
		static void termSignalHandler(int);
	public slots:
		void handleSigHup();
		void handleSigTerm();
	public:
		void setuid();
		void initialcap();
		void finalcap();

		UnixMurmur();
		~UnixMurmur();
};

#endif
