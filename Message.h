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
		enum MessageType { M_SPEEX, M_SERVER_JOIN, M_SERVER_LEAVE, M_PLAYER_MUTE, M_PLAYER_MUTE_ALL, M_PLAYER_KICK };
		virtual void saveStream(QDataStream &);
		virtual void restoreStream(QDataStream &);
	public:
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
		QByteArray m_qbaSpeexPacket;
		MessageSpeex();
		Message::MessageType messageType() { return M_SPEEX; };
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
		Message::MessageType messageType() { return M_SERVER_JOIN; };
		void process(Connection *);
		bool isValid();
};

class MessageServerLeave : public Message {
	public:
		Message::MessageType messageType() { return M_SERVER_LEAVE; };
		void process(Connection *);
};

#else
class Message;
#endif
