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

#ifndef _AUDIO_H
#define _AUDIO_H

#include "mumble_pch.hpp"
#include "Player.h"

#define SAMPLE_RATE 16000

typedef QPair<QString,QVariant> audioDevice;

class LoopPlayer : public ClientPlayer {
	private:
		Q_DISABLE_COPY(LoopPlayer)
	protected:
		typedef QPair<quint32, QByteArray> Packet;
		QMutex qmLock;
		QTime qtTicker;
		QTime qtLastFetch;
		QMultiMap<float, Packet> qmPackets;
		LoopPlayer();
	public:
		static LoopPlayer lpLoopy;
		void addFrame(const QByteArray &packet, int seq);
		void fetchFrames();
};

namespace Audio {
	void startInput(const QString &input = QString());
	void stopInput();

	void startOutput(const QString &output = QString());
	void stopOutput();

	void start(const QString &input = QString(), const QString &output = QString());
	void stop();
}

#endif
