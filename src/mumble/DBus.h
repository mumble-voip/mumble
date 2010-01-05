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

#ifndef _DBUS_H
#define _DBUS_H

#include "mumble_pch.hpp"

class MumbleDBus : public QDBusAbstractAdaptor {
	private:
		Q_OBJECT
		Q_CLASSINFO("D-Bus Interface", "net.sourceforge.mumble.Mumble")
		Q_DISABLE_COPY(MumbleDBus)
		Q_PROPERTY(bool mute READ isSelfMuted WRITE setSelfMuted)
		Q_PROPERTY(bool deaf READ isSelfDeaf WRITE setSelfDeaf)
	public:
		MumbleDBus(QObject *parent);
	public slots:
		void openUrl(const QString &url, const QDBusMessage &);
		void getCurrentUrl(const QDBusMessage &);
		void focus();
		void setSelfMuted(bool mute);
		void setSelfDeaf(bool deafen);
		bool isSelfMuted();
		bool isSelfDeaf();
};

#else
class MumbleDBus;
#endif
