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
		case PlayerMove:
			mMsg = new MessagePlayerMove();
			break;
		case ChannelAdd:
			mMsg = new MessageChannelAdd();
			break;
		case ChannelRemove:
			mMsg = new MessageChannelRemove();
			break;
		case ChannelMove:
			mMsg = new MessageChannelMove();
			break;
		case ChannelLink:
			mMsg = new MessageChannelLink();
			break;
		case PermissionDenied:
			mMsg = new MessagePermissionDenied();
			break;
		case EditACL:
			mMsg = new MessageEditACL();
			break;
		case QueryUsers:
			mMsg = new MessageQueryUsers();
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

MessageServerSync::MessageServerSync() {
	qsWelcomeText = QString();
}

void MessageServerSync::saveStream(QDataStream &qdsOut) const {
	qdsOut << qsWelcomeText;
}

void MessageServerSync::restoreStream(QDataStream &qdsIn) {
	qdsIn >> qsWelcomeText;
}

MessageServerJoin::MessageServerJoin() {
	qsPlayerName = QString();
	iId = -2;
}

void MessageServerJoin::saveStream(QDataStream &qdsOut) const {
	qdsOut << qsPlayerName;
	qdsOut << iId;
}

void MessageServerJoin::restoreStream(QDataStream &qdsIn) {
	qdsIn >> qsPlayerName;
	qdsIn >> iId;
}

MessagePermissionDenied::MessagePermissionDenied() {
	qsReason = QString();
}

void MessagePermissionDenied::saveStream(QDataStream &qdsOut) const {
	qdsOut << qsReason;
}

void MessagePermissionDenied::restoreStream(QDataStream &qdsIn) {
	qdsIn >> qsReason;
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

MessagePlayerMove::MessagePlayerMove() {
	sVictim = -1;
	iChannelId = 0;
}

void MessagePlayerMove::saveStream(QDataStream &qdsOut) const {
	qdsOut << sVictim;
	qdsOut << iChannelId;
}

void MessagePlayerMove::restoreStream(QDataStream &qdsIn) {
	qdsIn >> sVictim;
	qdsIn >> iChannelId;
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

MessageChannelAdd::MessageChannelAdd() {
	iId = 0;
	iParent = -1;
}

void MessageChannelAdd::saveStream(QDataStream &qdsOut) const {
	qdsOut << iId;
	qdsOut << iParent;
	qdsOut << qsName;
}

void MessageChannelAdd::restoreStream(QDataStream &qdsIn) {
	qdsIn >> iId;
	qdsIn >> iParent;
	qdsIn >> qsName;
}

MessageChannelRemove::MessageChannelRemove() {
	iId = 0;
}

void MessageChannelRemove::saveStream(QDataStream &qdsOut) const {
	qdsOut << iId;
}

void MessageChannelRemove::restoreStream(QDataStream &qdsIn) {
	qdsIn >> iId;
}

MessageChannelLink::MessageChannelLink() {
	iId = 0;
	ltType = UnlinkAll;
}

void MessageChannelLink::saveStream(QDataStream &qdsOut) const {
	qdsOut << iId;
	qdsOut << static_cast<int>(ltType);
	qdsOut << qlTargets;
}

void MessageChannelLink::restoreStream(QDataStream &qdsIn) {
	qdsIn >> iId;
	int v;
	qdsIn >> v;
	ltType = static_cast<LinkType>(v);
	qdsIn >> qlTargets;
}

MessageChannelMove::MessageChannelMove() {
	iId = 0;
	iParent = 0;
}

void MessageChannelMove::saveStream(QDataStream &qdsOut) const {
	qdsOut << iId;
	qdsOut << iParent;
}

void MessageChannelMove::restoreStream(QDataStream &qdsIn) {
	qdsIn >> iId;
	qdsIn >> iParent;
}

MessageEditACL::MessageEditACL() {
	bQuery = true;
	iId = 0;
	bInheritACL = true;
}

void MessageEditACL::saveStream(QDataStream &qdsOut) const {
	qdsOut << iId;
	qdsOut << bQuery;
	if (bQuery)
		return;

	qdsOut << bInheritACL;
	qdsOut << groups;
	qdsOut << acls;
}

void MessageEditACL::restoreStream(QDataStream &qdsIn) {
	qdsIn >> iId;
	qdsIn >> bQuery;
	if (bQuery)
		return;

	qdsIn >> bInheritACL;
	qdsIn >> groups;
	qdsIn >> acls;
}

MessageQueryUsers::MessageQueryUsers() {
}

void MessageQueryUsers::saveStream(QDataStream &qdsOut) const {
	qdsOut << qlIds;
	qdsOut << qlNames;
}

void MessageQueryUsers::restoreStream(QDataStream &qdsIn) {
	qdsIn >> qlIds;
	qdsIn >> qlNames;
}

bool MessageQueryUsers::isValid() const {
	if (qlIds.count() != qlNames.count())
		return false;

	int i;
	for(i=0;i<qlIds.count();i++) {
		if ((qlIds[i] == -1) && (qlNames[i].isEmpty()))
			return false;
	}
	return true;
}

QDataStream & operator<< ( QDataStream & out, const MessageEditACL::GroupStruct &gs ) {
	out << gs.qsName;
	out << gs.bInherited;
	out << gs.bInherit;
	out << gs.bInheritable;
	out << gs.qsAdd;
	out << gs.qsRemove;
	out << gs.qsInheritedMembers;
	return out;
}

QDataStream & operator>> ( QDataStream & in, MessageEditACL::GroupStruct &gs ) {
	in >> gs.qsName;
	in >> gs.bInherited;
	in >> gs.bInherit;
	in >> gs.bInheritable;
	in >> gs.qsAdd;
	in >> gs.qsRemove;
	in >> gs.qsInheritedMembers;
	return in;
}

QDataStream & operator<< ( QDataStream & out, const MessageEditACL::ACLStruct &as ) {
	out << as.bApplyHere;
	out << as.bApplySubs;
	out << as.bInherited;
	out << as.iPlayerId;
	out << as.qsGroup;
	out << static_cast<int>(as.pAllow);
	out << static_cast<int>(as.pDeny);
	return out;
}

QDataStream & operator>> ( QDataStream & in, MessageEditACL::ACLStruct &as ) {
	int v;

	in >> as.bApplyHere;
	in >> as.bApplySubs;
	in >> as.bInherited;
	in >> as.iPlayerId;
	in >> as.qsGroup;
	in >> v;
	as.pAllow = static_cast<ChanACL::Permissions>(v);
	in >> v;
	as.pDeny = static_cast<ChanACL::Permissions>(v);
	return in;
}
