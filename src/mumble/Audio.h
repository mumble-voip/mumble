// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIO_H_
#define MUMBLE_MUMBLE_AUDIO_H_

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
		~RecordUser() Q_DECL_OVERRIDE;
		void addFrame(const QByteArray &packet) Q_DECL_OVERRIDE;
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
