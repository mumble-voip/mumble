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

#include "Message.h"
#include "PacketDataStream.h"

Message::Message() {
	uiSession = 0;
}

Message::~Message() {
}

void Message::messageToNetwork(QByteArray &qbaOut) const {
	char buffer[65535];
	PacketDataStream qdsOut(buffer, 65535);
	qdsOut << messageType();
	qdsOut << uiSession;
	saveStream(qdsOut);

	qbaOut = QByteArray(buffer, qdsOut.size());
}

void Message::messageToNetwork(PacketDataStream &pds) const {
	pds << messageType();
	pds << uiSession;
	saveStream(pds);
	pds.truncate();
}

Message *Message::networkToMessage(QByteArray &qbaIn) {
	PacketDataStream qdsIn(qbaIn.constData(), qbaIn.size());
	return networkToMessage(qdsIn);
}

Message *Message::networkToMessage(PacketDataStream &qdsIn) {
	Message *mMsg = NULL;
	unsigned char iMessageType;
	short uiSession;
	qdsIn >> iMessageType;
	qdsIn >> uiSession;
	switch (iMessageType) {
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
		case ServerBanList:
			mMsg = new MessageServerBanList();
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
		case PlayerBan:
			mMsg = new MessagePlayerBan();
			break;
		case PlayerMove:
			mMsg = new MessagePlayerMove();
			break;
		case PlayerRename:
			mMsg = new MessagePlayerRename();
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
		case TextMessage:
			mMsg = new MessageTextMessage();
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
		case Ping:
			mMsg = new MessagePing();
			break;
		case PlayerTexture:
			mMsg = new MessageTexture();
			break;
		default:
			qWarning("Message: %d[%d] is unknown type", iMessageType, uiSession);
	}
	if (mMsg) {
		mMsg->uiSession=uiSession;
		mMsg->restoreStream(qdsIn);
		if (! qdsIn.isValid()) {
			delete mMsg;
			mMsg = NULL;
			qWarning("Message: %d[%d] Corrupt or short packet", iMessageType, uiSession);
		} else if (qdsIn.left() != 0) {
			delete mMsg;
			mMsg = NULL;
			qWarning("Message: %d[%d] Long packet: %d leftover bytes", iMessageType, uiSession, qdsIn.left());
		} else if (! mMsg->isValid()) {
			delete mMsg;
			mMsg = NULL;
			qWarning("Message: %d[%d] Failed to validate", iMessageType, uiSession);
		}
	}

	return mMsg;
}

bool Message::isValid() const {
	return true;
}

void Message::saveStream(PacketDataStream &) const {
}

void Message::restoreStream(PacketDataStream &) {
}

MessagePing::MessagePing() {
}

void MessagePing::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiTimestamp;
}

void MessagePing::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiTimestamp;
}

MessageServerAuthenticate::MessageServerAuthenticate() {
	iVersion = MESSAGE_STREAM_VERSION;
	iMaxBandwidth = 100000;
}

void MessageServerAuthenticate::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iVersion;
	qdsOut << iMaxBandwidth;
	qdsOut << qsUsername;
	qdsOut << qsPassword;
}

void MessageServerAuthenticate::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iVersion;
	qdsIn >> iMaxBandwidth;
	qdsIn >> qsUsername;
	qdsIn >> qsPassword;
}


MessageServerReject::MessageServerReject() {
}

void MessageServerReject::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qsReason;
	qdsOut << static_cast<int>(rtType);
}

void MessageServerReject::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qsReason;
	int v;
	qdsIn >> v;
	rtType = static_cast<RejectType>(v);
}

MessageServerSync::MessageServerSync() {
}

void MessageServerSync::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iMaxBandwidth;
	qdsOut << qsWelcomeText;
}

void MessageServerSync::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iMaxBandwidth;
	qdsIn >> qsWelcomeText;
}

MessageServerJoin::MessageServerJoin() {
}

void MessageServerJoin::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qsPlayerName;
	qdsOut << iId;
}

void MessageServerJoin::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qsPlayerName;
	qdsIn >> iId;
}

MessageServerBanList::MessageServerBanList() {
	bQuery = true;
}

void MessageServerBanList::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << bQuery;
	qdsOut << qlBans;
}

void MessageServerBanList::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> bQuery;
	qdsIn >> qlBans;
}

MessagePermissionDenied::MessagePermissionDenied() {
}

void MessagePermissionDenied::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qsReason;
}

void MessagePermissionDenied::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qsReason;
}

MessageSpeex::MessageSpeex() {
}

void MessageSpeex::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << static_cast<unsigned short>(iSeq);
	qdsOut.append(qbaSpeexPacket.constData(), qbaSpeexPacket.size());
}

void MessageSpeex::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iSeq;
	qbaSpeexPacket = qdsIn.dataBlock(qdsIn.left());
}

bool MessageSpeex::isValid() const {
	return ! qbaSpeexPacket.isEmpty();
}

MessagePlayerMute::MessagePlayerMute() {
}

void MessagePlayerMute::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << bMute;
}

void MessagePlayerMute::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> bMute;
}


MessagePlayerDeaf::MessagePlayerDeaf() {
}

void MessagePlayerDeaf::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << bDeaf;
}

void MessagePlayerDeaf::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> bDeaf;
}

MessagePlayerKick::MessagePlayerKick() {
}

void MessagePlayerKick::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << qsReason;
}

void MessagePlayerKick::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> qsReason;
}

MessagePlayerBan::MessagePlayerBan() {
}

void MessagePlayerBan::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << qsReason;
}

void MessagePlayerBan::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> qsReason;
}

MessagePlayerMove::MessagePlayerMove() {
}

void MessagePlayerMove::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << iChannelId;
}

void MessagePlayerMove::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> iChannelId;
}

MessagePlayerRename::MessagePlayerRename() {
}

void MessagePlayerRename::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qsName;
}

void MessagePlayerRename::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qsName;
}

MessagePlayerSelfMuteDeaf::MessagePlayerSelfMuteDeaf() {
}

void MessagePlayerSelfMuteDeaf::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << bMute;
	qdsOut << bDeaf;
}

void MessagePlayerSelfMuteDeaf::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> bMute;
	qdsIn >> bDeaf;
}

MessageChannelAdd::MessageChannelAdd() {
}

void MessageChannelAdd::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iId;
	qdsOut << iParent;
	qdsOut << qsName;
}

void MessageChannelAdd::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iId;
	qdsIn >> iParent;
	qdsIn >> qsName;
}

MessageChannelRemove::MessageChannelRemove() {
}

void MessageChannelRemove::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iId;
}

void MessageChannelRemove::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iId;
}

MessageChannelLink::MessageChannelLink() {
}

void MessageChannelLink::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iId;
	qdsOut << static_cast<int>(ltType);
	qdsOut << qlTargets;
}

void MessageChannelLink::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iId;
	int v;
	qdsIn >> v;
	ltType = static_cast<LinkType>(v);
	qdsIn >> qlTargets;
}

MessageChannelMove::MessageChannelMove() {
}

void MessageChannelMove::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iId;
	qdsOut << iParent;
}

void MessageChannelMove::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iId;
	qdsIn >> iParent;
}

MessageTextMessage::MessageTextMessage() {
}

void MessageTextMessage::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << qsMessage;
}

void MessageTextMessage::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> qsMessage;
}

MessageEditACL::MessageEditACL() {
}

void MessageEditACL::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iId;
	qdsOut << bQuery;
	if (bQuery)
		return;

	qdsOut << bInheritACL;
	qdsOut << groups;
	qdsOut << acls;
}

void MessageEditACL::restoreStream(PacketDataStream &qdsIn) {
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

void MessageQueryUsers::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qlIds;
	qdsOut << qlNames;
}

void MessageQueryUsers::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qlIds;
	qdsIn >> qlNames;
}

bool MessageQueryUsers::isValid() const {
	if (qlIds.count() != qlNames.count())
		return false;

	int i;
	for (i=0;i<qlIds.count();i++) {
		if ((qlIds[i] == -1) && (qlNames[i].isEmpty()))
			return false;
	}
	return true;
}

MessageTexture::MessageTexture() {
}

void MessageTexture::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iPlayerId;
	qdsOut << qbaTexture;
}

void MessageTexture::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iPlayerId;
	qdsIn >> qbaTexture;
}

PacketDataStream & operator<< (PacketDataStream & out, const MessageEditACL::GroupStruct &gs) {
	out << gs.qsName;
	out << gs.bInherited;
	out << gs.bInherit;
	out << gs.bInheritable;
	out << gs.qsAdd;
	out << gs.qsRemove;
	out << gs.qsInheritedMembers;
	return out;
}

PacketDataStream & operator>> (PacketDataStream & in, MessageEditACL::GroupStruct &gs) {
	in >> gs.qsName;
	in >> gs.bInherited;
	in >> gs.bInherit;
	in >> gs.bInheritable;
	in >> gs.qsAdd;
	in >> gs.qsRemove;
	in >> gs.qsInheritedMembers;
	return in;
}

PacketDataStream & operator<< (PacketDataStream & out, const MessageEditACL::ACLStruct &as) {
	out << as.bApplyHere;
	out << as.bApplySubs;
	out << as.bInherited;
	out << as.iPlayerId;
	out << as.qsGroup;
	out << static_cast<int>(as.pAllow);
	out << static_cast<int>(as.pDeny);
	return out;
}

PacketDataStream & operator>> (PacketDataStream & in, MessageEditACL::ACLStruct &as) {
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
