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
	sPlayerId = 0;
}

Message::~Message() {
}

void Message::messageToNetwork(QByteArray &qbaOut) const {
	QDataStream qdsOut(&qbaOut, QIODevice::WriteOnly);
	qdsOut << static_cast<unsigned char>(messageType());
	qdsOut << static_cast<unsigned char>(sPlayerId);
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
		case ServerSync:
			mMsg = new MessageServerSync();
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
		case PlayerSelfMuteDeaf:
			mMsg = new MessagePlayerSelfMuteDeaf();
			break;
		case PlayerKick:
			mMsg = new MessagePlayerKick();
			break;
		default:
			qWarning("Message: %d[%d] is unknown type", iMessageType, sPlayerId);
	}
	if (mMsg) {
		mMsg->sPlayerId=sPlayerId;
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

bool Message::isValid() const {
	return true;
}

void Message::saveStream(QDataStream &) const {
}

void Message::restoreStream(QDataStream &) {
}


MessageServerAuthenticate::MessageServerAuthenticate() {
	iVersion = MESSAGE_STREAM_VERSION;
}

void MessageServerAuthenticate::saveStream(QDataStream &qdsOut) const{
	qdsOut << iVersion;
	qdsOut << qsUsername;
	qdsOut << qsPassword;
}

void MessageServerAuthenticate::restoreStream(QDataStream &qdsIn) {
	qdsIn >> iVersion;
	qdsIn >> qsUsername;
	qdsIn >> qsPassword;
}


MessageServerReject::MessageServerReject() {
	qsReason = QString();
}

void MessageServerReject::saveStream(QDataStream &qdsOut) const {
	qdsOut << qsReason;
}

void MessageServerReject::restoreStream(QDataStream &qdsIn) {
	qdsIn >> qsReason;
}


MessageServerJoin::MessageServerJoin() {
	qsPlayerName = QString();
}

void MessageServerJoin::saveStream(QDataStream &qdsOut) const {
	qdsOut << qsPlayerName;
}

void MessageServerJoin::restoreStream(QDataStream &qdsIn) {
	qdsIn >> qsPlayerName;
}

MessageSpeex::MessageSpeex() {
	qbaSpeexPacket = QByteArray();
}

void MessageSpeex::saveStream(QDataStream &qdsOut) const {
	qdsOut << iSeq;
	QBuffer *qbBuffer = static_cast<QBuffer *>(qdsOut.device());
	qbBuffer->buffer().append(qbaSpeexPacket);
}

void MessageSpeex::restoreStream(QDataStream &qdsIn) {
	qdsIn >> iSeq;
	QBuffer *qbBuffer = static_cast<QBuffer *>(qdsIn.device());
	qbaSpeexPacket = qbBuffer->buffer().right(qbBuffer->bytesAvailable());
	qbBuffer->seek(qbBuffer->size());
}

bool MessageSpeex::isValid() const {
	return ! qbaSpeexPacket.isEmpty();
}


MessagePlayerMute::MessagePlayerMute() {
	bMute = false;
}

void MessagePlayerMute::saveStream(QDataStream &qdsOut) const {
	qdsOut << sVictim;
	qdsOut << bMute;
}

void MessagePlayerMute::restoreStream(QDataStream &qdsIn) {
	qdsIn >> sVictim;
	qdsIn >> bMute;
}


MessagePlayerDeaf::MessagePlayerDeaf() {
	bDeaf = false;
}

void MessagePlayerDeaf::saveStream(QDataStream &qdsOut) const {
	qdsOut << sVictim;
	qdsOut << bDeaf;
}

void MessagePlayerDeaf::restoreStream(QDataStream &qdsIn) {
	qdsIn >> sVictim;
	qdsIn >> bDeaf;
}

MessagePlayerKick::MessagePlayerKick() {
	qsReason = QString();
}

void MessagePlayerKick::saveStream(QDataStream &qdsOut) const {
	qdsOut << sVictim;
	qdsOut << qsReason;
}

void MessagePlayerKick::restoreStream(QDataStream &qdsIn) {
	qdsIn >> sVictim;
	qdsIn >> qsReason;
}

MessagePlayerSelfMuteDeaf::MessagePlayerSelfMuteDeaf() {
	bMute = false;
	bDeaf = false;
}

void MessagePlayerSelfMuteDeaf::saveStream(QDataStream &qdsOut) const {
	qdsOut << bMute;
	qdsOut << bDeaf;
}

void MessagePlayerSelfMuteDeaf::restoreStream(QDataStream &qdsIn) {
	qdsIn >> bMute;
	qdsIn >> bDeaf;
}

