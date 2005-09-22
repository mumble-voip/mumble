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

#include <QCoreApplication>
#include "Server.h"
#include "ServerDB.h"

extern Server *g_sServer;

int main(int argc, char **argv)
{
	int res;

	QCoreApplication a(argc, argv);

	a.setApplicationName("Murmur");
	a.setOrganizationName("Mumble");
	a.setOrganizationDomain("mumble.sourceforge.net");

	argc = a.argc();
	argv = a.argv();

	for(int i=1;i<argc;i++) {
		QString arg = QString(argv[i]).toLower();
		if (arg == "-loop") {
			qWarning("Enabling loop mode");
			g_sp.bTestloop = true;
		} else if ((arg == "-pw") && ( i+1 < argc )) {
			i++;
			g_sp.qsPassword = argv[i];
			qWarning("Setting password to %s", g_sp.qsPassword.toLatin1().constData());
		} else if ((arg == "-text") && ( i+1 < argc )) {
			i++;
			g_sp.qsWelcomeText = argv[i];
			qWarning("Setting welcome text to \"%s\"", g_sp.qsWelcomeText.toLatin1().constData());
		} else if ((arg == "-port") && ( i+1 < argc )) {
			i++;
			int p = QString(argv[i]).toInt();
			if ((p>0) && (p<65536)) {
				qWarning("Setting listen port to %d", p);
				g_sp.iPort = p;
			}
		} else {
			qWarning("Unknown argument %s", argv[i]);
		}
	}

	ServerDB db;
	db.readChannels();

	g_sServer = new Server();
	res=a.exec();

	return res;
}

