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
	QDataStream qdsOut(&qbaOut, QIODevice::WriteOnly);
	qdsOut << ((unsigned char) messageType());
	qdsOut << ((unsigned char) m_sPlayerId);
	saveStream(qdsOut);
}

Message *Message::networkToMessage(QByteArray &qbaIn) {
	QDataStream qdsIn(qbaIn);
	Message *mMsg = NULL;
	unsigned char iMessageType;
	unsigned char sPlayerId;
	qdsIn >> iMessageType;
	qdsIn >> sPlayerId;
	switch(iMessageType) {
		case Speex:
			mMsg = new MessageSpeex();
			break;
		case ServerAuthenticate:
			mMsg = new MessageServerAuthenticate();
			break;
		case ServerReject:
			mMsg = new MessageServerReject();
			break;
		case ServerJoin:
			mMsg = new MessageServerJoin();
			break;
		case ServerLeave:
			mMsg = new MessageServerLeave();
			break;
		case PlayerMute:
			mMsg = new MessagePlayerMute();
			break;
		case PlayerDeaf:
			mMsg = new MessagePlayerDeaf();
			break;
		case PlayerKick:
			mMsg = new MessagePlayerKick();
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
	return true;
}

void Message::saveStream(QDataStream &) {
}

void Message::restoreStream(QDataStream &) {
}


MessageServerAuthenticate::MessageServerAuthenticate() {
	m_iVersion = MESSAGE_STREAM_VERSION;
}

void MessageServerAuthenticate::saveStream(QDataStream &qdsOut) {
	qdsOut << m_iVersion;
	qdsOut << m_qsUsername;
	qdsOut << m_qsPassword;
}

void MessageServerAuthenticate::restoreStream(QDataStream &qdsIn) {
	qdsIn >> m_iVersion;
	qdsIn >> m_qsUsername;
	qdsIn >> m_qsPassword;
}


MessageServerReject::MessageServerReject() {
	m_qsReason = QString();
}

void MessageServerReject::saveStream(QDataStream &qdsOut) {
	qdsOut << m_qsReason;
}

void MessageServerReject::restoreStream(QDataStream &qdsIn) {
	qdsIn >> m_qsReason;
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

MessageSpeex::MessageSpeex() {
	m_qbaSpeexPacket = QByteArray();
}

void MessageSpeex::saveStream(QDataStream &qdsOut) {
	qdsOut << m_iSeq;
	QBuffer *qbBuffer = static_cast<QBuffer *>(qdsOut.device());
	qbBuffer->buffer().append(m_qbaSpeexPacket);
}

void MessageSpeex::restoreStream(QDataStream &qdsIn) {
	qdsIn >> m_iSeq;
	QBuffer *qbBuffer = static_cast<QBuffer *>(qdsIn.device());
	m_qbaSpeexPacket = qbBuffer->buffer().right(qbBuffer->bytesAvailable());
	qbBuffer->seek(qbBuffer->size());
}

bool MessageSpeex::isValid() {
	return ! m_qbaSpeexPacket.isEmpty();
}


MessagePlayerMute::MessagePlayerMute() {
	m_bMute = false;
}

void MessagePlayerMute::saveStream(QDataStream &qdsOut) {
	qdsOut << m_bMute;
}

void MessagePlayerMute::restoreStream(QDataStream &qdsIn) {
	qdsIn >> m_bMute;
}


MessagePlayerDeaf::MessagePlayerDeaf() {
	m_bDeaf = false;
}

void MessagePlayerDeaf::saveStream(QDataStream &qdsOut) {
	qdsOut << m_bDeaf;
}

void MessagePlayerDeaf::restoreStream(QDataStream &qdsIn) {
	qdsIn >> m_bDeaf;
}


MessagePlayerKick::MessagePlayerKick() {
	m_qsReason = QString();
}

void MessagePlayerKick::saveStream(QDataStream &qdsOut) {
	qdsOut << m_qsReason;
}

void MessagePlayerKick::restoreStream(QDataStream &qdsIn) {
	qdsIn >> m_qsReason;
}
