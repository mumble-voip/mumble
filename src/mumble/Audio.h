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

#ifndef AUDIO_H_
#define AUDIO_H_

#include <QtCore/QByteArray>
#include <QtCore/QMultiMap>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QVariant>

#include "ClientUser.h"

#define SAMPLE_RATE 48000

typedef QPair<QString,QVariant> audioDevice;

class LoopUser : public ClientUser {
	private:
		Q_DISABLE_COPY(LoopUser)
	protected:
		QMutex qmLock;
		QTime qtTicker;
		QTime qtLastFetch;
		QMultiMap<float, QByteArray> qmPackets;
		LoopUser();
	public:
		static LoopUser lpLoopy;
		virtual void addFrame(const QByteArray &packet);
		void fetchFrames();
};

class RecordUser : public LoopUser {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(RecordUser)
	public:
		RecordUser();
		~RecordUser();
		virtual void addFrame(const QByteArray &packet);
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
