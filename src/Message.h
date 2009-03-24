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

#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "Connection.h"
#include "Mumble.pb.h"

class MessageHandler {
	public:
		enum UDPMessageType { UDPVoice, UDPPing };
		enum MessageType { Version, UDPTunnel, Authenticate, Ping, Reject, ServerSync, ChannelRemove, ChannelState, UserRemove, UserState, BanList, TextMessage, PermissionDenied, ACL, QueryUsers, CryptSetup, ContextActionAdd, ContextAction };
	protected:
		virtual void msgVersion(Connection *, MumbleProto::Version *) = 0;
		virtual void msgUDPTunnel(Connection *, MumbleProto::UDPTunnel *) = 0;
		virtual void msgAuthenticate(Connection *, MumbleProto::Authenticate *) = 0;
		virtual void msgPing(Connection *, MumbleProto::Ping *) = 0;
		virtual void msgReject(Connection *, MumbleProto::Reject *) = 0;
		virtual void msgServerSync(Connection *, MumbleProto::ServerSync *) = 0;
		virtual void msgChannelRemove(Connection *, MumbleProto::ChannelRemove *) = 0;
		virtual void msgChannelState(Connection *, MumbleProto::ChannelState *) = 0;
		virtual void msgUserRemove(Connection *, MumbleProto::UserRemove *) = 0;
		virtual void msgUserState(Connection *, MumbleProto::UserState *) = 0;
		virtual void msgBanList(Connection *, MumbleProto::BanList *) = 0;
		virtual void msgTextMessage(Connection *, MumbleProto::TextMessage *) = 0;
		virtual void msgPermissionDenied(Connection *, MumbleProto::PermissionDenied *) = 0;
		virtual void msgACL(Connection *, MumbleProto::ACL *) = 0;
		virtual void msgQueryUsers(Connection *, MumbleProto::QueryUsers *) = 0;
		virtual void msgCryptSetup(Connection *, MumbleProto::CryptSetup *) = 0;
		virtual void msgContextActionAdd(Connection *, MumbleProto::ContextActionAdd *) = 0;
		virtual void msgContextAction(Connection *, MumbleProto::ContextAction *) = 0;
		void dispatch(Connection *, int type, const QByteArray &);
		virtual ~MessageHandler() { };
};

inline QString u8(const ::std::string &str) {
	return QString::fromUtf8(str.data(), str.length());
}

inline ::std::string u8(const QString &str) {
	const QByteArray &qba = str.toUtf8();
	return ::std::string(qba.constData(), qba.length());
}

#else
class Message;
#endif
