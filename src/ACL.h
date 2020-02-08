// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

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
