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

#ifndef _MESSAGE_H
#define _MESSAGE_H

#define MESSAGE_STREAM_VERSION 4

#include "ACL.h"

class Connection;
class PacketDataStream;

class Message {
	protected:
		virtual void saveStream(PacketDataStream &) const;
		virtual void restoreStream(PacketDataStream &);
	public:
		enum MessageType { ServerReject, ServerAuthenticate, Speex, ServerSync, ServerJoin, ServerLeave, ServerBanList, PlayerMute, PlayerDeaf, PlayerKick, PlayerRename, PlayerBan, PlayerMove, PlayerSelfMuteDeaf, ChannelAdd, ChannelRemove, ChannelMove, ChannelLink, ChannelRename, PermissionDenied, EditACL, QueryUsers, Ping, TextMessage, PlayerTexture, CryptSetup, CryptSync, PingStats, ContextAction, ContextAddAction, ChannelDescUpdate };
		unsigned int uiSession;

		Message();
		virtual ~Message();
		virtual Message::MessageType messageType() const = 0;
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
		enum { AltSpeak = 0x01, LoopBack = 0x02, EndSpeech = 0x04, FrameCountMask = 0x30 };
		unsigned int iSeq;
		// Flags is in first byte of packet.
		QByteArray qbaSpeexPacket;
		Message::MessageType messageType() const {
			return Speex;
		};
		bool isValid() const;
};

class MessageServerAuthenticate : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		int	iVersion;
		QString qsUsername;
		QString qsPassword;
		MessageServerAuthenticate();
		Message::MessageType messageType() const {
			return ServerAuthenticate;
		};
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
};

class MessagePingStats : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		quint64 uiTimestamp;
		quint32 uiGood;
		quint32 uiLate;
		quint32 uiLost;
		quint32 uiResync;
		double dUDPPingAvg;
		double dUDPPingVar;
		quint32 uiUDPPackets;
		double dTCPPingAvg;
		double dTCPPingVar;
		quint32 uiTCPPackets;
		Message::MessageType messageType() const {
			return PingStats;
		};
};

class MessageServerReject : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		enum RejectType { None, WrongVersion, InvalidUsername, WrongUserPW, WrongServerPW, UsernameInUse, ServerFull };
		QString qsReason;
		RejectType rtType;
		Message::MessageType messageType() const {
			return ServerReject;
		};
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
};

class MessageServerLeave : public Message {
	public:
		Message::MessageType messageType() const {
			return ServerLeave;
		};
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
};

class MessageChannelRename : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		int iId;
		QString qsName;
		Message::MessageType messageType() const {
			return ChannelRename;
		};
};

class MessageChannelDescUpdate : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		int iId;
		QString qsDesc;
		Message::MessageType messageType() const {
			return ChannelDescUpdate;
		};
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
};

class MessageTextMessage : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		unsigned int  uiVictim;
		int iChannel;
		bool bTree;
		QString qsMessage;
		Message::MessageType messageType() const {
			return TextMessage;
		};
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
};

class MessageCryptSetup : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		QByteArray qbaKey, qbaClientNonce, qbaServerNonce;
		Message::MessageType messageType() const {
			return CryptSetup;
		}
		bool isValid() const;
};

class MessageCryptSync : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		QByteArray qbaNonce;
		Message::MessageType messageType() const {
			return CryptSync;
		}
		bool isValid() const;
};

class MessageContextAddAction : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		enum Context { CtxServer = 0x01, CtxChannel = 0x02, CtxPlayer = 0x04 };
		QString qsAction;
		QString qsText;
		Context ctx;
		Message::MessageType messageType() const {
			return ContextAddAction;
		}
		bool isValid() const;
};

class MessageContextAction : public Message {
	protected:
		void saveStream(PacketDataStream &) const;
		void restoreStream(PacketDataStream &);
	public:
		QString qsAction;
		unsigned int uiVictim;
		int iChannel;
		Message::MessageType messageType() const {
			return ContextAction;
		}
		bool isValid() const;
};

class MessageHandler {
	protected:
		virtual void msgSpeex(Connection *, MessageSpeex *) = 0;
		virtual void msgServerAuthenticate(Connection *, MessageServerAuthenticate *) = 0;
		virtual void msgPing(Connection *, MessagePing *) = 0;
		virtual void msgPingStats(Connection *, MessagePingStats *) = 0;
		virtual void msgServerReject(Connection *, MessageServerReject *) = 0;
		virtual void msgServerSync(Connection *, MessageServerSync *) = 0;
		virtual void msgServerJoin(Connection *, MessageServerJoin *) = 0;
		virtual void msgServerLeave(Connection *, MessageServerLeave *) = 0;
		virtual void msgPlayerMute(Connection *, MessagePlayerMute *) = 0;
		virtual void msgPlayerDeaf(Connection *, MessagePlayerDeaf *) = 0;
		virtual void msgPlayerSelfMuteDeaf(Connection *, MessagePlayerSelfMuteDeaf *) = 0;
		virtual void msgPlayerKick(Connection *, MessagePlayerKick *) = 0;
		virtual void msgPlayerBan(Connection *, MessagePlayerBan *) = 0;
		virtual void msgPlayerMove(Connection *, MessagePlayerMove *) = 0;
		virtual void msgPlayerRename(Connection *, MessagePlayerRename *) = 0;
		virtual void msgChannelAdd(Connection *, MessageChannelAdd *) = 0;
		virtual void msgChannelRemove(Connection *, MessageChannelRemove *) = 0;
		virtual void msgChannelMove(Connection *, MessageChannelMove *) = 0;
		virtual void msgChannelLink(Connection *, MessageChannelLink *) = 0;
		virtual void msgChannelRename(Connection *, MessageChannelRename *) = 0;
		virtual void msgChannelDescUpdate(Connection *, MessageChannelDescUpdate *) = 0;
		virtual void msgServerBanList(Connection *, MessageServerBanList *) = 0;
		virtual void msgTextMessage(Connection *, MessageTextMessage *) = 0;
		virtual void msgPermissionDenied(Connection *, MessagePermissionDenied *) = 0;
		virtual void msgEditACL(Connection *, MessageEditACL *) = 0;
		virtual void msgQueryUsers(Connection *, MessageQueryUsers *) = 0;
		virtual void msgTexture(Connection *, MessageTexture *) = 0;
		virtual void msgCryptSetup(Connection *, MessageCryptSetup *) = 0;
		virtual void msgCryptSync(Connection *, MessageCryptSync *) = 0;
		virtual void msgContextAddAction(Connection *, MessageContextAddAction *) = 0;
		virtual void msgContextAction(Connection *, MessageContextAction *) = 0;
		void dispatch(Connection *, Message *);
	public:
		virtual ~MessageHandler() { };
};

#else
class Message;
#endif
