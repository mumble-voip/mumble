/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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

#include "murmur_pch.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "Server.h"
#include "ServerDB.h"

extern Server *g_sServer;

int main(int argc, char **argv)
{
	// Check for SSE and MMX, but only in the windows binaries
#ifdef Q_OS_WIN
#define cpuid(func,ax,bx,cx,dx) __asm__ __volatile__ ("cpuid": "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func));
#define MMXSSE 0x02800000
	unsigned int ax, bx, cx, dx;
	cpuid(1,ax,bx,cx,dx);
	if ((dx & MMXSSE) != MMXSSE) {
		::MessageBoxA(NULL, "Mumble requires a SSE capable processor (Pentium 3 / Ahtlon-XP)", "Mumble", MB_OK | MB_ICONERROR);
		exit(0);
	}
#endif

	int res;

	QCoreApplication a(argc, argv);

	a.setApplicationName("Murmur");
	a.setOrganizationName("Mumble");
	a.setOrganizationDomain("mumble.sourceforge.net");

	argc = a.argc();
	argv = a.argv();

	QString inifile;
	QString supw;

	for(int i=1;i<argc;i++) {
		QString arg = QString(argv[i]).toLower();
		if ((arg == "-supw") && ( i+1 < argc )) {
			i++;
			supw = argv[i];
		} else if ((arg == "-ini") && ( i+1 < argc )) {
			i++;
			inifile=argv[i];
		} else {
			qWarning("Unknown argument %s", argv[i]);
		}
	}

	g_sp.read(inifile);

	ServerDB db;

	if (! supw.isEmpty()) {
		ServerDB::setPW(0, supw);
		qFatal("Superuser password set");
	}

	db.readChannels();

	g_sServer = new Server();
	res=a.exec();

	return res;
}

