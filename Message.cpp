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
#include <QBuffer>

Message::Message() {
	m_sPlayerId = 0;
}

Message::~Message() {
}

void Message::messageToNetwork(QByteArray &qbaOut) {
	QDataStream qdsOut(qbaOut);
	qdsOut << messageType();
	qdsOut << m_sPlayerId;
	saveStream(qdsOut);
}

Message *Message::networkToMessage(QByteArray &qbaIn) {
	QDataStream qdsIn(qbaIn);
	Message *mMsg = NULL;
	int iMessageType;
	short sPlayerId;
	qdsIn >> iMessageType;
	qdsIn >> sPlayerId;
	switch(iMessageType) {
		case M_SPEEX:
			mMsg = new MessageSpeex();
			break;
		case M_SERVER_JOIN:
			mMsg = new MessageServerJoin();
			break;
		default:
			qWarning("Message: %d[%d] is unknown type", iMessageType, sPlayerId);
	}
	if (mMsg) {
		mMsg->m_sPlayerId=sPlayerId;
		mMsg->restoreStream(qdsIn);
		if (qdsIn.status() != QDataStream::Ok) {
			delete mMsg;
			mMsg = NULL;
			qWarning("Message: %d[%d] Corrput or short packet", iMessageType, sPlayerId);
		} else if (qdsIn.device()->bytesAvailable() != 0) {
			delete mMsg;
			mMsg = NULL;
			qWarning("Message: %d[%d] Long packet", iMessageType, sPlayerId);
		} else if (! mMsg->isValid()) {
			delete mMsg;
			mMsg = NULL;
			qWarning("Message: %d[%d] Failed to validate", iMessageType, sPlayerId);
		}
	}

	return mMsg;
}

bool Message::isValid() {
	qWarning("Message: MessageType %d doesn't have a validator", messageType());
	return TRUE;
}

void Message::saveStream(QDataStream &) {
}

void Message::restoreStream(QDataStream &) {
}


MessageServerJoin::MessageServerJoin() {
	m_qsPlayerName = QString();
}

void MessageServerJoin::saveStream(QDataStream &qdsOut) {
	qdsOut << m_qsPlayerName;
}

void MessageServerJoin::restoreStream(QDataStream &qdsIn) {
	qdsIn >> m_qsPlayerName;
}

bool MessageServerJoin::isValid() {
	bool ok = true;
	for(int i=0;i<m_qsPlayerName.length();i++) {
		QChar c=m_qsPlayerName[i];
		if (! c.isLetterOrNumber() && (c != ' '))
			ok = false;
	}
	return ok;
}


MessageSpeex::MessageSpeex() {
	m_qbaSpeexPacket = QByteArray();
}

void MessageSpeex::saveStream(QDataStream &qdsOut) {
	QBuffer *qbBuffer = static_cast<QBuffer *>(qdsOut.device());
	qbBuffer->buffer().append(m_qbaSpeexPacket);
}

void MessageSpeex::restoreStream(QDataStream &qdsIn) {
	QBuffer *qbBuffer = static_cast<QBuffer *>(qdsIn.device());
	m_qbaSpeexPacket = qbBuffer->buffer().right(qbBuffer->bytesAvailable());
	qbBuffer->seek(qbBuffer->size());
}

bool MessageSpeex::isValid() {
	return ! m_qbaSpeexPacket.isEmpty();
}
