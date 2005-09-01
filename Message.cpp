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

#include "Message.h"

Message::Message() {
}

void Message::messageToNetwork(QByteArray &qbaOut) {
	QDataStream qdsOut(qbaOut);
	qdsOut << messageType();
	saveStream(qdsOut);
}

Message *Message::networkToMessage(QByteArray &qbaIn) {
	QDataStream qdsIn(qbaIn);
	Message *mMsg = NULL;
	int id;
	qdsIn >> id;
	switch(id) {
		case M_SERVER_JOIN:
			mMsg = new MessageServerJoin();
			break;
		default:
			qWarning("Message: Message ID %d couldn't be read", id);
	}
	if (mMsg)
		mMsg->restoreStream(qdsIn);

	return mMsg;
}


MessageServerJoin::MessageServerJoin() {
	m_iId = 0;
	m_qsPlayerName = QString();
}

void MessageServerJoin::saveStream(QDataStream &qdsOut) {
	qdsOut << m_iId;
	qdsOut << m_qsPlayerName;
}

void MessageServerJoin::restoreStream(QDataStream &qdsIn) {
	qdsIn >> m_iId;
	qdsIn >> m_qsPlayerName;
}

// This will be moved to the actual handlers for client/server stuff
void MessageServerJoin::process()
{
}
