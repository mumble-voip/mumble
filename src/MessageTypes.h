/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MESSAGE_TYPES_H_
#define MESSAGE_TYPES_H_

/**
  Protobuf packet type enumeration for message handler generation.

  Warning: Only append to the end.
 */
#define MUMBLE_MH_ALL \
	MUMBLE_MH_MSG(Version) \
	MUMBLE_MH_MSG(UDPTunnel) \
	MUMBLE_MH_MSG(Authenticate) \
	MUMBLE_MH_MSG(Ping) \
	MUMBLE_MH_MSG(Reject) \
	MUMBLE_MH_MSG(ServerSync) \
	MUMBLE_MH_MSG(ChannelRemove) \
	MUMBLE_MH_MSG(ChannelState) \
	MUMBLE_MH_MSG(UserRemove) \
	MUMBLE_MH_MSG(UserState) \
	MUMBLE_MH_MSG(BanList) \
	MUMBLE_MH_MSG(TextMessage) \
	MUMBLE_MH_MSG(PermissionDenied) \
	MUMBLE_MH_MSG(ACL) \
	MUMBLE_MH_MSG(QueryUsers) \
	MUMBLE_MH_MSG(CryptSetup) \
	MUMBLE_MH_MSG(ContextActionModify) \
	MUMBLE_MH_MSG(ContextAction) \
	MUMBLE_MH_MSG(UserList) \
	MUMBLE_MH_MSG(VoiceTarget) \
	MUMBLE_MH_MSG(PermissionQuery) \
	MUMBLE_MH_MSG(CodecVersion) \
	MUMBLE_MH_MSG(UserStats) \
	MUMBLE_MH_MSG(RequestBlob) \
	MUMBLE_MH_MSG(ServerConfig) \
	MUMBLE_MH_MSG(SuggestConfig)

struct MessageTypes {
	enum UDPMessageType {
		UDPVoiceCELTAlpha,
		UDPPing,
		UDPVoiceSpeex,
		UDPVoiceCELTBeta,
		UDPVoiceOpus
	};

#define MUMBLE_MH_MSG(x) x,
	enum MessageType {
		MUMBLE_MH_ALL
	};
#undef MUMBLE_MH_MSG
};

#endif // MESSAGE_TYPES_H_
