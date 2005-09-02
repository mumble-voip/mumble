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

#include <QDataStream>
#include <QByteArray>
#include <QString>
#include "Connection.h"

class Message {
	protected:
		virtual void saveStream(QDataStream &);
		virtual void restoreStream(QDataStream &);
	public:
		enum MessageType { Speex, ServerJoin, ServerLeave, PlayerMute, PlayerDeaf, PlayerKick };

		short m_sPlayerId;

		Message();
		virtual ~Message();
		virtual Message::MessageType messageType() = 0;
		virtual void process(Connection *) = 0;
		virtual bool isValid();

		void messageToNetwork(QByteArray &);
		static Message *networkToMessage(QByteArray &);
};

class MessageSpeex : public Message {
	protected:
		void saveStream(QDataStream &);
		void restoreStream(QDataStream &);
	public:
		int m_iSeq;
		QByteArray m_qbaSpeexPacket;
		MessageSpeex();
		Message::MessageType messageType() { return Speex; };
		void process(Connection *);
		bool isValid();
};


class MessageServerJoin : public Message {
	protected:
		void saveStream(QDataStream &);
		void restoreStream(QDataStream &);
	public:
		QString m_qsPlayerName;
		MessageServerJoin();
		Message::MessageType messageType() { return ServerJoin; };
		void process(Connection *);
		bool isValid();
};

class MessageServerLeave : public Message {
	public:
		Message::MessageType messageType() { return ServerLeave; };
		void process(Connection *);
};

class MessagePlayerMute : public Message {
	protected:
		void saveStream(QDataStream &);
		void restoreStream(QDataStream &);
	public:
		bool m_bMute;
		MessagePlayerMute();
		Message::MessageType messageType() { return PlayerMute; };
		void process(Connection *);
};

class MessagePlayerDeaf : public Message {
	protected:
		void saveStream(QDataStream &);
		void restoreStream(QDataStream &);
	public:
		bool m_bDeaf;
		MessagePlayerDeaf();
		Message::MessageType messageType() { return PlayerDeaf; };
		void process(Connection *);
};

class MessagePlayerKick : public Message {
	protected:
		void saveStream(QDataStream &);
		void restoreStream(QDataStream &);
	public:
		QString m_qsReason;
		MessagePlayerKick();
		Message::MessageType messageType() { return PlayerKick; };
		void process(Connection *);
};

#else
class Message;
#endif
