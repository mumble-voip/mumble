/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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
#include "murmur_pch.h"

void MessageHandler::dispatch(Connection *cCon, const QByteArray &msg) {
	/*
		switch (type) {
			case MessageHandler::Version: {
					MumbleProto::Version mpv;
					mpv.ParseFromArray(msg.constData(), msg.size());
					msgVersion(cCon, &mpv);
				}
				break;
			default:
				break;
		}
	*/
}

void MessageHandler::messageToNetwork(const ::google::protobuf::Message &msg, unsigned int msgType, QByteArray &cache) {
	int len = msg.ByteSize();
	if (len > 0x7fffff)
		return;
	cache.resize(len + 4);
	unsigned char *uc = reinterpret_cast<unsigned char *>(cache.data());
	uc[0] = msgType;
	uc[1] = (len >> 16) & 0xFF;
	uc[2] = (len >> 8) & 0xFF;
	uc[3] = len & 0xFF;

	msg.SerializeToArray(uc + 4, len);
}
