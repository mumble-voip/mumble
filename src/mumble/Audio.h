// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIO_H_
#define MUMBLE_MUMBLE_AUDIO_H_

#include <QtCore/QByteArray>
#include <QtCore/QElapsedTimer>
#include <QtCore/QMultiMap>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include "ClientUser.h"

#define SAMPLE_RATE 48000

// interaural delay (in samples) for a sound coming directly from the side of the head
// A Wikipedia article claims the average distance between ears is 15.2 cm for men
// (0.44 ms) and 14.4 cm for women (0.42 ms). We decided to set the delay to 0.43 ms.
// The delay is calculated from the distance and the speed of sound.
constexpr float INTERAURAL_DELAY = 0.00043 / (1 / static_cast< float >(SAMPLE_RATE));

typedef QPair< QString, QVariant > audioDevice;

class LoopUser : public ClientUser {
private:
	Q_DISABLE_COPY(LoopUser)
protected:
	QMutex qmLock;
	QElapsedTimer qetTicker;
	QElapsedTimer qetLastFetch;
	QMultiMap< float, QByteArray > qmPackets;
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
} // namespace Audio

#endif
