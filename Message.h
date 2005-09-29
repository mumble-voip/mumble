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

#ifndef _MESSAGE_H
#define _MESSAGE_H

#define MESSAGE_STREAM_VERSION 4

#include <QDataStream>
#include <QByteArray>
#include <QString>
#include <QSet>
#include "ACL.h"

class Connection;

class Message {
	protected:
		virtual void saveStream(QDataStream &) const;
		virtual void restoreStream(QDataStream &);
	public:
		enum MessageType { Speex, ServerAuthenticate, ServerReject, ServerSync, ServerJoin, ServerLeave, PlayerMute, PlayerDeaf, PlayerKick, PlayerSelfMuteDeaf, ChannelAdd, ChannelRemove, ChannelMove, PlayerMove, PermissionDenied, EditACL, QueryUsers, ChannelLink };
		short sPlayerId;

		Message();
		virtual ~Message();
		virtual Message::MessageType messageType() const = 0;
		virtual void process(Connection *) = 0;
		virtual bool isValid() const;

		void messageToNetwork(QByteArray &) const;
		static Message *networkToMessage(QByteArray &);
};

class MessageSpeex : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		int iSeq;
		QByteArray qbaSpeexPacket;
		MessageSpeex();
		Message::MessageType messageType() const { return Speex; };
		void process(Connection *);
		bool isValid() const;
};


class MessageServerAuthenticate : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		int	iVersion;
		QString qsUsername;
		QString qsPassword;
		MessageServerAuthenticate();
		Message::MessageType messageType() const { return ServerAuthenticate; };
		void process(Connection *);
};

class MessageServerReject : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		QString qsReason;
		MessageServerReject();
		Message::MessageType messageType() const { return ServerReject; };
		void process(Connection *);
};

class MessageServerSync : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		QString qsWelcomeText;
		MessageServerSync();
		Message::MessageType messageType() const { return ServerSync; };
		void process(Connection *);
};

class MessageServerJoin : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		QString qsPlayerName;
		int iId;
		MessageServerJoin();
		Message::MessageType messageType() const { return ServerJoin; };
		void process(Connection *);
};

class MessageServerLeave : public Message {
	public:
		Message::MessageType messageType() const { return ServerLeave; };
		void process(Connection *);
};

class MessagePlayerMute : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		short sVictim;
		bool bMute;
		MessagePlayerMute();
		Message::MessageType messageType() const { return PlayerMute; };
		void process(Connection *);
};

class MessagePlayerDeaf : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		short sVictim;
		bool bDeaf;
		MessagePlayerDeaf();
		Message::MessageType messageType() const { return PlayerDeaf; };
		void process(Connection *);
};

class MessagePlayerSelfMuteDeaf : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		bool bMute;
		bool bDeaf;
		MessagePlayerSelfMuteDeaf();
		Message::MessageType messageType() const { return PlayerSelfMuteDeaf; };
		void process(Connection *);
};

class MessagePlayerKick : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		short sVictim;
		QString qsReason;
		MessagePlayerKick();
		Message::MessageType messageType() const { return PlayerKick; };
		void process(Connection *);
};

class MessagePlayerMove : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		short sVictim;
		int iChannelId;
		MessagePlayerMove();
		Message::MessageType messageType() const { return PlayerMove; };
		void process(Connection *);
};

class MessageChannelAdd : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		int iId;
		int iParent;
		QString qsName;
		MessageChannelAdd();
		Message::MessageType messageType() const { return ChannelAdd; };
		void process(Connection *);
};

class MessageChannelRemove : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		int iId;
		MessageChannelRemove();
		Message::MessageType messageType() const { return ChannelRemove; };
		void process(Connection *);
};

class MessageChannelMove : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		int iId;
		int iParent;
		MessageChannelMove();
		Message::MessageType messageType() const { return ChannelMove; };
		void process(Connection *);
};

class MessageChannelLink : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		int iId;
		bool bCreate;
		int iTarget;
		MessageChannelLink();
		Message::MessageType messageType() const { return ChannelLink; };
		void process(Connection *);
};

class MessagePermissionDenied : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		QString qsReason;
		MessagePermissionDenied();
		Message::MessageType messageType() const { return PermissionDenied; };
		void process(Connection *);
};


class MessageEditACL : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
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
		MessageEditACL();
		Message::MessageType messageType() const { return EditACL; };
		void process(Connection *);
};

QDataStream & operator<< ( QDataStream & out, const MessageEditACL::GroupStruct &gs );
QDataStream & operator>> ( QDataStream & in, MessageEditACL::GroupStruct &gs );
QDataStream & operator<< ( QDataStream & out, const MessageEditACL::ACLStruct &gs );
QDataStream & operator>> ( QDataStream & in, MessageEditACL::ACLStruct &gs );

class MessageQueryUsers : public Message {
	protected:
		void saveStream(QDataStream &) const;
		void restoreStream(QDataStream &);
	public:
		QList<int> qlIds;
		QList<QString> qlNames;
		MessageQueryUsers();
		Message::MessageType messageType() const { return QueryUsers; };
		void process(Connection *);
		bool isValid() const;
};

#else
class Message;
#endif
