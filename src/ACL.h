// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_ACL_H_
#define MUMBLE_ACL_H_

#include <QtCore/QHash>
#include <QtCore/QObject>

class Channel;
class User;
class ServerUser;

class ChanACL : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ChanACL)
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
			Whisper = 0x100,
			TextMessage = 0x200,
			MakeTempChannel = 0x400,

			// Root channel only
			Kick = 0x10000,
			Ban = 0x20000,
			ResetCommentTexture = 0x40000,
			Register = 0x80000,
			SelfRegister = 0x100000,

			Cached = 0x8000000,
			All = 0x1F07FF
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
