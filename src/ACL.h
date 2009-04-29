/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _ACL_H
#define _ACL_H

#include "murmur_pch.h"

class Channel;
class Player;

class ChanACL : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ChanACL);
	public:
		enum Perm {
			None = 0x0,
			Write = 0x1,
			Traverse = 0x2,
			Enter = 0x4,
			Speak = 0x8,
			MuteDeafen = 0x10,
			Move = 0x20,
			MakeChannel = 0x40,
			LinkChannel = 0x80,
			AltSpeak = 0x100,
			TextMessage = 0x200,
			
			// Root channel only
			Kick = 0x1000,
			Ban = 0x2000,
			Register = 0x4000,
			
			Cached = 0x8000000,
			All = 0x703ff
		};

		Q_DECLARE_FLAGS(Permissions, Perm)

		typedef QHash<Channel *, Permissions> ChanCache;
		typedef QHash<Player *, ChanCache * > ACLCache;

		Channel *c;
		bool bApplyHere;
		bool bApplySubs;

		bool bInherited;

		int iPlayerId;
		QString qsGroup;
		Permissions pAllow;
		Permissions pDeny;

		ChanACL(Channel *c);
		static bool hasPermission(Player *p, Channel *c, QFlags<Perm> perm, ACLCache &cache);
		static QString permName(QFlags<Perm> p);
		static QString permName(Perm p);
		static QString whatsThis(Perm p);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ChanACL::Permissions)

#else
class ChanACL;
#endif
