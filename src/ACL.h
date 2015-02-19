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

#ifndef MUMBLE_ACL_H_
#define MUMBLE_ACL_H_

#include <QtCore/QHash>
#include <QtCore/QObject>

#ifdef MURMUR
#include "murmur/Mumble.pb.h"
#endif
#ifdef MUMBLE
#include "mumble/Mumble.pb.h"
#endif

class Channel;
class User;
class ServerUser;

class ChanACL : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ChanACL)
	public:
		enum Perm {
			None = MumbleProto::ACL::None,
			Write = MumbleProto::ACL::Write,
			Traverse = MumbleProto::ACL::Traverse,
			Enter = MumbleProto::ACL::Enter,
			Speak = MumbleProto::ACL::Speak,
			MuteDeafen = MumbleProto::ACL::MuteDeafen,
			Move = MumbleProto::ACL::Move,
			MakeChannel = MumbleProto::ACL::MakeChannel,
			LinkChannel = MumbleProto::ACL::LinkChannel,
			Whisper = MumbleProto::ACL::Whisper,
			TextMessage = MumbleProto::ACL::TextMessage,
			MakeTempChannel = MumbleProto::ACL::MakeTempChannel,

			// Root channel only
			Kick = MumbleProto::ACL::Kick,
			Ban = MumbleProto::ACL::Ban,
			Register = MumbleProto::ACL::Register,
			SelfRegister = MumbleProto::ACL::SelfRegister,

			Cached = MumbleProto::ACL::Cached,
			All = MumbleProto::ACL::All
		};

		Q_DECLARE_FLAGS(Permissions, Perm)

		typedef QHash<Channel *, Permissions> ChanCache;
		typedef QHash<User *, ChanCache * > ACLCache;

		Channel *c;
		bool bApplyHere;
		bool bApplySubs;

		bool bInherited;

		int iUserId;
		QString qsGroup;
		Permissions pAllow;
		Permissions pDeny;

		ChanACL(Channel *c);
#ifdef MURMUR
		static bool hasPermission(ServerUser *p, Channel *c, QFlags<Perm> perm, ACLCache *cache);
		static QFlags<Perm> effectivePermissions(ServerUser *p, Channel *c, ACLCache *cache);
#else
		static QString whatsThis(Perm p);
#endif
		static QString permName(QFlags<Perm> p);
		static QString permName(Perm p);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ChanACL::Permissions)

#endif
