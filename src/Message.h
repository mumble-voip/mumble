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

#ifndef _MESSAGE_H
#define _MESSAGE_H

#define MESSAGE_STREAM_VERSION 2

#include "ACL.h"

class Connection;
class PacketDataStream;

class Message {
	protected:
		virtual void saveStream(PacketDataStream &) const;
		virtual void restoreStream(PacketDataStream &);
	public:
		enum MessageType { ServerReject, ServerAuthenticate, Speex, ServerSync, ServerJoin, ServerLeave, ServerBanList, PlayerMute, PlayerDeaf, PlayerKick, PlayerRename, PlayerBan, PlayerMove, PlayerSelfMuteDeaf, ChannelAdd, ChannelRemove, ChannelMove, ChannelLink, PermissionDenied, EditACL, QueryUsers, Ping, TextMessage, PlayerTexture };
		unsigned int uiSession;

		Message();
		virtual ~Message();
		virtual Message::MessageType messageType() const = 0;
		virtual void process(Connection *) = 0;
		virtual bool isValid() const;

		void messageToNetwork(QByteArray &) const;
		void messageToNetwork(PacketDataStream &) const;
		static Message *networkToMessage(QByteArray &);
		static Message *networkToMessage(PacketDataStream &);
};

class MessageSpeex : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		enum { AltSpeak = 0x01, LoopBack = 0x02, EndSpeech = 0x04, FrameCountMask = 0x50 };
		unsigned int iSeq;
		// Flags is in first byte of packet.
		QByteArray qbaSpeexPacket;
		Message::MessageType messageType() const {
			return Speex;
		};
		void process(Connection *);
		bool isValid() const;
};

class MessageServerAuthenticate : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		int	iVersion;
		int iMaxBandwidth;
		QString qsUsername;
		QString qsPassword;
		MessageServerAuthenticate();
		Message::MessageType messageType() const {
			return ServerAuthenticate;
		};
		void process(Connection *);
};

class MessagePing : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		quint64 uiTimestamp;
		Message::MessageType messageType() const {
			return Ping;
		};
		void process(Connection *);
};

class MessageServerReject : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		enum RejectType { None, WrongVersion, InvalidUsername, WrongUserPW, WrongServerPW, UsernameInUse, BandwidthExceeded, ServerFull };
		QString qsReason;
		RejectType rtType;
		Message::MessageType messageType() const {
			return ServerReject;
		};
		void process(Connection *);
};

class MessageServerSync : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		int iMaxBandwidth;
		QString qsWelcomeText;
		Message::MessageType messageType() const {
			return ServerSync;
		};
		void process(Connection *);
};

class MessageServerJoin : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		QString qsPlayerName;
		int iId;
		Message::MessageType messageType() const {
			return ServerJoin;
		};
		void process(Connection *);
};

class MessageServerLeave : public Message {
	public:
		Message::MessageType messageType() const {
			return ServerLeave;
		};
		void process(Connection *);
};

class MessagePlayerMute : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		unsigned int uiVictim;
		bool bMute;
		Message::MessageType messageType() const {
			return PlayerMute;
		};
		void process(Connection *);
};

class MessagePlayerDeaf : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		unsigned int uiVictim;
		bool bDeaf;
		Message::MessageType messageType() const {
			return PlayerDeaf;
		};
		void process(Connection *);
};

class MessagePlayerSelfMuteDeaf : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		bool bMute;
		bool bDeaf;
		Message::MessageType messageType() const {
			return PlayerSelfMuteDeaf;
		};
		void process(Connection *);
};

class MessagePlayerKick : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		unsigned int  uiVictim;
		QString qsReason;
		Message::MessageType messageType() const {
			return PlayerKick;
		};
		void process(Connection *);
};

class MessagePlayerBan : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		unsigned int  uiVictim;
		QString qsReason;
		Message::MessageType messageType() const {
			return PlayerBan;
		};
		void process(Connection *);
};

class MessagePlayerMove : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		unsigned int  uiVictim;
		int iChannelId;
		Message::MessageType messageType() const {
			return PlayerMove;
		};
		void process(Connection *);
};

class MessagePlayerRename : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		QString qsName;
		Message::MessageType messageType() const {
			return PlayerRename;
		};
		void process(Connection *);
};

class MessageChannelAdd : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		int iId;
		int iParent;
		QString qsName;
		Message::MessageType messageType() const {
			return ChannelAdd;
		};
		void process(Connection *);
};

class MessageChannelRemove : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		int iId;
		Message::MessageType messageType() const {
			return ChannelRemove;
		};
		void process(Connection *);
};

class MessageChannelMove : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		int iId;
		int iParent;
		Message::MessageType messageType() const {
			return ChannelMove;
		};
		void process(Connection *);
};

class MessageChannelLink : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		enum LinkType {
			Link, Unlink, UnlinkAll, PushLink, PushUnlink
		};
		int iId;
		LinkType ltType;
		QList<int> qlTargets;
		Message::MessageType messageType() const {
			return ChannelLink;
		};
		void process(Connection *);
};

class MessageServerBanList : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		bool bQuery;
		QList<QPair<quint32, int> > qlBans;
		Message::MessageType messageType() const {
			return ServerBanList;
		};
		void process(Connection *);
};

class MessageTextMessage : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		unsigned int  uiVictim;
		QString qsMessage;
		Message::MessageType messageType() const {
			return TextMessage;
		};
		void process(Connection *);
};

class MessagePermissionDenied : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		QString qsReason;
		Message::MessageType messageType() const {
			return PermissionDenied;
		};
		void process(Connection *);
};

class MessageEditACL : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		struct GroupStruct {
			QString qsName;
			bool bInherited;
			bool bInherit;
			bool bInheritable;
			QSet<int> qsAdd;
			QSet<int> qsRemove;
			QSet<int> qsInheritedMembers;
		};

		struct ACLStruct {
			bool bApplyHere;
			bool bApplySubs;
			bool bInherited;
			int iPlayerId;
			QString qsGroup;
			ChanACL::Permissions pAllow;
			ChanACL::Permissions pDeny;
		};

		int iId;
		bool bQuery;
		bool bInheritACL;
		QList<GroupStruct> groups;
		QList<ACLStruct> acls;
		Message::MessageType messageType() const {
			return EditACL;
		};
		void process(Connection *);
};

PacketDataStream & operator<< (PacketDataStream & out, const MessageEditACL::GroupStruct &gs);
PacketDataStream & operator>> (PacketDataStream & in, MessageEditACL::GroupStruct &gs);
PacketDataStream & operator<< (PacketDataStream & out, const MessageEditACL::ACLStruct &gs);
PacketDataStream & operator>> (PacketDataStream & in, MessageEditACL::ACLStruct &gs);

class MessageQueryUsers : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		QList<int> qlIds;
		QList<QString> qlNames;
		Message::MessageType messageType() const {
			return QueryUsers;
		};
		void process(Connection *);
		bool isValid() const;
};

class MessageTexture : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		int iPlayerId;
		QByteArray qbaTexture;
		Message::MessageType messageType() const {
			return PlayerTexture;
		}
		void process(Connection *);
};

#else
class Message;
#endif
