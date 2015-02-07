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

#ifndef MUMBLE_MUMBLE_OVERLAY_WIN_H_
#define MUMBLE_MUMBLE_OVERLAY_WIN_H_

#include <QString>
#include <QStringList>
#include <QProcess>

#include "Overlay.h"

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
		void onHelperProcessExited(int exitCode, QProcess::ExitStatus exitStatus);

	protected:
		QProcess *m_helper_process;
		QString m_helper_exe_path;
		QStringList m_helper_exe_args;

		QProcess *m_helper64_process;
		QString m_helper64_exe_path;
		QStringList m_helper64_exe_args;

		bool m_active;
		bool m_allow64bit;

		void startHelper(QProcess *helper);
};

#endif
