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

#include "Message.h"
#include "PacketDataStream.h"
#include "murmur_pch.h"

Message::Message() {
	uiSession = 0;
}

Message::~Message() {
}

void Message::messageToNetwork(QByteArray &qbaOut) const {
	char buffer[8192];
	PacketDataStream qdsOut(buffer, 8192);
	qdsOut << messageType();
	qdsOut << uiSession;
	saveStream(qdsOut);

	if (qdsOut.undersize() == 0) {
		qbaOut = QByteArray(buffer, qdsOut.size());
		return;
	}

	unsigned int size = 8192 + qdsOut.undersize();
	STACKVAR(char, b, size);
	PacketDataStream pdsResized(b, size);
	pdsResized << messageType();
	pdsResized << uiSession;
	saveStream(pdsResized);
	qbaOut = QByteArray(b, pdsResized.size());
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
		case ChannelRename:
			mMsg = new MessageChannelRename();
			break;
		case ChannelDescUpdate:
			mMsg = new MessageChannelDescUpdate();
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
		case PingStats:
			mMsg = new MessagePingStats();
			break;
		case PlayerTexture:
			mMsg = new MessageTexture();
			break;
		case CryptSetup:
			mMsg = new MessageCryptSetup();
			break;
		case CryptSync:
			mMsg = new MessageCryptSync();
			break;
		case ContextAction:
			mMsg = new MessageContextAction();
			break;
		case ContextAddAction:
			mMsg = new MessageContextAddAction();
			break;
		default:
			qWarning("Message: Type %d (session %d, size %d) is unknown type", iMessageType, uiSession, qdsIn.capacity());
	}
	if (mMsg) {
		mMsg->uiSession=uiSession;
		mMsg->restoreStream(qdsIn);
		if (! qdsIn.isValid()) {
			delete mMsg;
			mMsg = NULL;
			qWarning("Message: Type %d (session %d, size %d) corrupt or short packet", iMessageType, uiSession, qdsIn.capacity());
		} else if (qdsIn.left() != 0) {
			delete mMsg;
			mMsg = NULL;
			qWarning("Message: Type %d (session %d) Long packet: %d/%d leftover bytes", iMessageType, uiSession, qdsIn.left(), qdsIn.size());
		} else if (! mMsg->isValid()) {
			delete mMsg;
			mMsg = NULL;
			qWarning("Message: Type %d (session %d, size %d) failed to validate", iMessageType, uiSession, qdsIn.capacity());
		}
	}

	return mMsg;
}

void MessageHandler::dispatch(Connection *cCon, Message *msg) {
	switch (msg->messageType()) {
		case Message::Speex:
			msgSpeex(cCon, static_cast<MessageSpeex *>(msg));
			break;
		case Message::ServerAuthenticate:
			msgServerAuthenticate(cCon, static_cast<MessageServerAuthenticate *>(msg));
			break;
		case Message::ServerReject:
			msgServerReject(cCon, static_cast<MessageServerReject *>(msg));
			break;
		case Message::ServerSync:
			msgServerSync(cCon, static_cast<MessageServerSync *>(msg));
			break;
		case Message::ServerJoin:
			msgServerJoin(cCon, static_cast<MessageServerJoin *>(msg));
			break;
		case Message::ServerLeave:
			msgServerLeave(cCon, static_cast<MessageServerLeave *>(msg));
			break;
		case Message::ServerBanList:
			msgServerBanList(cCon, static_cast<MessageServerBanList *>(msg));
			break;
		case Message::PlayerMute:
			msgPlayerMute(cCon, static_cast<MessagePlayerMute *>(msg));
			break;
		case Message::PlayerDeaf:
			msgPlayerDeaf(cCon, static_cast<MessagePlayerDeaf *>(msg));
			break;
		case Message::PlayerSelfMuteDeaf:
			msgPlayerSelfMuteDeaf(cCon, static_cast<MessagePlayerSelfMuteDeaf *>(msg));
			break;
		case Message::PlayerKick:
			msgPlayerKick(cCon, static_cast<MessagePlayerKick *>(msg));
			break;
		case Message::PlayerBan:
			msgPlayerBan(cCon, static_cast<MessagePlayerBan *>(msg));
			break;
		case Message::PlayerMove:
			msgPlayerMove(cCon, static_cast<MessagePlayerMove *>(msg));
			break;
		case Message::PlayerRename:
			msgPlayerRename(cCon, static_cast<MessagePlayerRename *>(msg));
			break;
		case Message::ChannelAdd:
			msgChannelAdd(cCon, static_cast<MessageChannelAdd *>(msg));
			break;
		case Message::ChannelRemove:
			msgChannelRemove(cCon, static_cast<MessageChannelRemove *>(msg));
			break;
		case Message::ChannelMove:
			msgChannelMove(cCon, static_cast<MessageChannelMove *>(msg));
			break;
		case Message::ChannelLink:
			msgChannelLink(cCon, static_cast<MessageChannelLink *>(msg));
			break;
		case Message::ChannelRename:
			msgChannelRename(cCon, static_cast<MessageChannelRename *>(msg));
			break;
		case Message::ChannelDescUpdate:
			msgChannelDescUpdate(cCon, static_cast<MessageChannelDescUpdate *>(msg));
			break;
		case Message::TextMessage:
			msgTextMessage(cCon, static_cast<MessageTextMessage *>(msg));
			break;
		case Message::PermissionDenied:
			msgPermissionDenied(cCon, static_cast<MessagePermissionDenied *>(msg));
			break;
		case Message::EditACL:
			msgEditACL(cCon, static_cast<MessageEditACL *>(msg));
			break;
		case Message::QueryUsers:
			msgQueryUsers(cCon, static_cast<MessageQueryUsers *>(msg));
			break;
		case Message::Ping:
			msgPing(cCon, static_cast<MessagePing *>(msg));
			break;
		case Message::PingStats:
			msgPingStats(cCon, static_cast<MessagePingStats *>(msg));
			break;
		case Message::PlayerTexture:
			msgTexture(cCon, static_cast<MessageTexture *>(msg));
			break;
		case Message::CryptSetup:
			msgCryptSetup(cCon, static_cast<MessageCryptSetup *>(msg));
			break;
		case Message::CryptSync:
			msgCryptSync(cCon, static_cast<MessageCryptSync *>(msg));
			break;
		case Message::ContextAction:
			msgContextAction(cCon, static_cast<MessageContextAction *>(msg));
			break;
		case Message::ContextAddAction:
			msgContextAddAction(cCon, static_cast<MessageContextAddAction *>(msg));
			break;
		default:
			qFatal("MessageHandler called with unknown message type %d", msg->messageType());
	}
}

bool Message::isValid() const {
	return true;
}

void Message::saveStream(PacketDataStream &) const {
}

void Message::restoreStream(PacketDataStream &) {
}

void MessagePing::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiTimestamp;
}

void MessagePing::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiTimestamp;
}

void MessagePingStats::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiTimestamp << uiGood << uiLate << uiLost << uiResync << dUDPPingAvg << dUDPPingVar << uiUDPPackets << dTCPPingAvg << dTCPPingVar << uiTCPPackets;
}

void MessagePingStats::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiTimestamp >> uiGood >> uiLate >> uiLost >> uiResync >> dUDPPingAvg >> dUDPPingVar >> uiUDPPackets >> dTCPPingAvg >> dTCPPingVar >> uiTCPPackets;
}

MessageServerAuthenticate::MessageServerAuthenticate() {
	iVersion = MESSAGE_STREAM_VERSION;
}

void MessageServerAuthenticate::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iVersion;
	qdsOut << qsUsername;
	qdsOut << qsPassword;
}

void MessageServerAuthenticate::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iVersion;
	qdsIn >> qsUsername;
	qdsIn >> qsPassword;
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

void MessageServerSync::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iMaxBandwidth;
	qdsOut << qsWelcomeText;
}

void MessageServerSync::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iMaxBandwidth;
	qdsIn >> qsWelcomeText;
}

void MessageServerJoin::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qsPlayerName;
	qdsOut << iId;
}

void MessageServerJoin::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qsPlayerName;
	qdsIn >> iId;
}

void MessageServerBanList::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << bQuery;
	qdsOut << qlBans;
}

void MessageServerBanList::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> bQuery;
	qdsIn >> qlBans;
}

void MessagePermissionDenied::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qsReason;
}

void MessagePermissionDenied::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qsReason;
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

void MessagePlayerMute::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << bMute;
}

void MessagePlayerMute::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> bMute;
}

void MessagePlayerDeaf::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << bDeaf;
}

void MessagePlayerDeaf::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> bDeaf;
}

void MessagePlayerKick::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << qsReason;
}

void MessagePlayerKick::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> qsReason;
}

void MessagePlayerBan::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << qsReason;
}

void MessagePlayerBan::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> qsReason;
}

void MessagePlayerMove::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << iChannelId;
}

void MessagePlayerMove::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> iChannelId;
}

void MessagePlayerRename::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qsName;
}

void MessagePlayerRename::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qsName;
}

void MessagePlayerSelfMuteDeaf::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << bMute;
	qdsOut << bDeaf;
}

void MessagePlayerSelfMuteDeaf::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> bMute;
	qdsIn >> bDeaf;
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

void MessageChannelRemove::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iId;
}

void MessageChannelRemove::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iId;
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

void MessageChannelMove::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iId;
	qdsOut << iParent;
}

void MessageChannelMove::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iId;
	qdsIn >> iParent;
}

void MessageChannelRename::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iId << qsName;
}

void MessageChannelRename::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iId >> qsName;
}

void MessageChannelDescUpdate::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iId << qsDesc;
}

void MessageChannelDescUpdate::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iId >> qsDesc;
}

void MessageTextMessage::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << uiVictim;
	qdsOut << iChannel;
	qdsOut << bTree;
	qdsOut << qsMessage;
}

void MessageTextMessage::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> uiVictim;
	qdsIn >> iChannel;
	qdsIn >> bTree;
	qdsIn >> qsMessage;
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

void MessageTexture::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << iPlayerId;
	qdsOut << qbaTexture;
}

void MessageTexture::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> iPlayerId;
	qdsIn >> qbaTexture;
}

void MessageCryptSetup::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qbaKey << qbaServerNonce << qbaClientNonce;
}

void MessageCryptSetup::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qbaKey >> qbaServerNonce >> qbaClientNonce;
}

bool MessageCryptSetup::isValid() const {
	return ((qbaKey.size() == AES_BLOCK_SIZE) &&
	        (qbaClientNonce.size() == AES_BLOCK_SIZE) &&
	        (qbaServerNonce.size() == AES_BLOCK_SIZE));
}


void MessageCryptSync::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qbaNonce;
}

void MessageCryptSync::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qbaNonce;
}

bool MessageCryptSync::isValid() const {
	return (qbaNonce.isEmpty() || (qbaNonce.size() == AES_BLOCK_SIZE));
}

void MessageContextAddAction::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qsAction;
	qdsOut << qsText;
	qdsOut << static_cast<int>(ctx);
}

void MessageContextAddAction::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qsAction;
	qdsIn >> qsText;
	int c;
	qdsIn >> c;
	ctx = static_cast<Context>(c);
}

bool MessageContextAddAction::isValid() const {
	return (! qsAction.isEmpty() && (ctx & (CtxServer | CtxPlayer | CtxChannel)));
}

void MessageContextAction::saveStream(PacketDataStream &qdsOut) const {
	qdsOut << qsAction;
	qdsOut << uiVictim;
	qdsOut << iChannel;
}

void MessageContextAction::restoreStream(PacketDataStream &qdsIn) {
	qdsIn >> qsAction;
	qdsIn >> uiVictim;
	qdsIn >> iChannel;
}

bool MessageContextAction::isValid() const {
	return (! qsAction.isEmpty());
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
