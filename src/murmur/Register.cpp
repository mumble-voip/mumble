/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#include <QtXml>

#include "Server.h"
#include "Register.h"

Register::Register() {
  http = NULL;
  connect(&qtTick, SIGNAL(timeout()), this, SLOT(update()));
  
  if (! g_sp.qsRegName.isEmpty()) {
    if ((! g_sp.qsRegName.isEmpty()) && (! g_sp.qsRegPassword.isEmpty()) && (g_sp.qurlRegWeb.isValid()) && (g_sp.qsPassword.isEmpty()))
      qtTick.start(60000);
    else
      qWarning("Registration needs nonempty name, password and url, and the server must not be password protected.");
  } else {
      qWarning("Not registering server as public");
  }
}

void Register::abort() {
  if (http) {
    http->deleteLater();
    http = NULL;
  }
}

void Register::update() {
  abort();
  qtTick.start(1000 * 60 * 60);

  QDomDocument doc;
  QDomElement root=doc.createElement(QLatin1String("server"));
  doc.appendChild(root);
  
  QDomElement tag;
  QDomText t;
  
  tag=doc.createElement(QLatin1String("name"));
  root.appendChild(tag);
  
  t=doc.createTextNode(g_sp.qsRegName);
  tag.appendChild(t);

  tag=doc.createElement(QLatin1String("password"));
  root.appendChild(tag);
  
  t=doc.createTextNode(g_sp.qsRegPassword);
  tag.appendChild(t);

  tag=doc.createElement(QLatin1String("port"));
  root.appendChild(tag);
  
  t=doc.createTextNode(QString::number(g_sp.iPort));
  tag.appendChild(t);

  tag=doc.createElement(QLatin1String("url"));
  root.appendChild(tag);
  
  t=doc.createTextNode(g_sp.qurlRegWeb.toString());
  tag.appendChild(t);
  
  http = new QHttp(this);
  connect(http, SIGNAL(done(bool)), this, SLOT(done(bool)));
  http->setHost(QLatin1String("mumble.hive.no"), 80);
  
  QHttpRequestHeader h(QLatin1String("POST"), QLatin1String("/register.cgi"));
  h.setValue(QLatin1String("Connection"), QLatin1String("Keep-Alive"));
  h.setValue(QLatin1String("Host"), QLatin1String("mumble.hive.no"));
  h.setContentType(QLatin1String("text/xml"));
  http->request(h, doc.toString().toUtf8());
}

void Register::done(bool err) {
  if (err) {
    qWarning("Regstration failed: %s", qPrintable(http->errorString()));
  } else {
    QByteArray qba = http->readAll();
    qWarning("Registration: %s", qPrintable(QString(QLatin1String(qba))));
  }
  abort();
}
