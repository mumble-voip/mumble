// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include <sndio.h>

#include "SndioAudio.h"

#include "User.h"
#include "Global.h"
#include "MainWindow.h"

class SndioInputRegistrar : public AudioInputRegistrar {
	public:
		SndioInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		virtual bool canEcho(const QString &) const;
};


class SndioOutputRegistrar : public AudioOutputRegistrar {
	public:
		SndioOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
};

static SndioInputRegistrar airSndio;
static SndioOutputRegistrar aorSndio;

SndioInputRegistrar::SndioInputRegistrar() : AudioInputRegistrar(QLatin1String("sndio")) {
}

AudioInput *SndioInputRegistrar::create() {
	return new SndioInput();
}

const QList<audioDevice> SndioInputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;
	qlReturn << audioDevice(QLatin1String("Default"), QVariant());
	return qlReturn;
}

void SndioInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsSndioInput = choice.toString();
}

bool SndioInputRegistrar::canEcho(const QString &) const {
	return false;
}

SndioOutputRegistrar::SndioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("sndio")) {
}

AudioOutput *SndioOutputRegistrar::create() {
	return new SndioOutput();
}

const QList<audioDevice> SndioOutputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;
	qlReturn << audioDevice(QLatin1String("Default"), QVariant());
	return qlReturn;
}

void SndioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsSndioOutput = choice.toString();
}

SndioInput::SndioInput() {
	udata = NULL;
}

SndioInput::~SndioInput() {
}

void SndioInput::run() {
}

SndioOutput::SndioOutput() {
	bRunning = true;
	qWarning("SndioOutput: Initialized");
}

SndioOutput::~SndioOutput() {
	bRunning = false;
	// Call destructor of all children
	wipe();
	// Wait for terminate
	wait();
	qWarning("SndioOutput: Destroyed");
}

void SndioOutput::run() {
	struct sio_hdl *handle = NULL;
	struct sio_par par;
	int prime = 0;

	const unsigned int chanmasks[32] = {
		SPEAKER_FRONT_LEFT,
		SPEAKER_FRONT_RIGHT,
		SPEAKER_BACK_LEFT,
		SPEAKER_BACK_RIGHT,
		SPEAKER_FRONT_CENTER,
		SPEAKER_LOW_FREQUENCY,
	};

	handle = sio_open(SIO_DEVANY, SIO_PLAY|SIO_REC, 0);
	if (handle == NULL) {
		qWarning("SndioOutput: failed to sio_open()");
		goto out;
	}

	sio_initpar(&par);

	par.bits = 16;
	par.bps = 2;
	par.sig = 1;
	par.le = SIO_LE_NATIVE;
	// Don't set par.msb, since it is not important due to
	// the fact that our par.bits/par.bps does not incur
	// any padding.
	par.msb = 1;
	// Don't set par.rchan or par.pchan. Let sndio decide.
	par.rate = SAMPLE_RATE;
	par.appbufsz = (SAMPLE_RATE / 100);
	par.xrun = SIO_IGNORE;

	if (sio_setpar(handle, &par) == 0) {
		qWarning("SndioOutput: sio_setpar() failed");
		goto out;
	}

	if (sio_getpar(handle, &par) == 0) {
		qWarning("SndioOutput: sio_getpar() failed");
		goto out;
	}

	if (par.bits != 16 || par.bps != 2 || par.sig != 1) {
		qWarning("SndioOutput: unable to configure sndio device: bits=%i, bps=%i, sig=%i", par.bits, par.bps, par.sig);
	}

	iChannels = static_cast<int>(par.pchan);
	iMixerFreq = static_cast<int>(par.rate);
	eSampleFormat = SampleShort;

	initializeMixer(chanmasks);

	if (sio_start(handle) == 0) {
		qWarning("SndioOutput: sio_start() failed");
		goto out;
	}

	prime = 20; // Why?

	while (bRunning) {
		AudioInputPtr aisp = g.ai;
		AudioInput *ai = aisp.data();
		SndioInput *si = qobject_cast<SndioInput *>(ai);

		if (si == NULL) {
			aisp.clear();
			ai = NULL;
		}

		// Initialize input
		if (si != NULL && si->udata == NULL) {
			si->udata = handle;

			si->iMicChannels = par.rchan;
			si->iMicFreq = par.rate;
			si->eMicFormat = AudioInput::SampleShort; //XXX
			si->initializeMixer();
		}

		if (prime == 0) {
			if (si != NULL && si->udata == handle) {
				int iMicLength = si->iMicLength;
				int iMicChannels = si->iMicChannels;

				short inbuf[iMicLength * iMicChannels];
				size_t len = iMicLength * iMicChannels * sizeof(short);
				size_t nread = sio_read(handle, inbuf, len);
				if (nread > 0) {
					size_t nreadFrames = nread / (sizeof(short) * iMicChannels);
					si->addMic(inbuf, nreadFrames);
				}
			}
		}

		// Done with AudioInput...
		aisp.clear();
		ai = NULL;
		si = NULL;

		int iOutputBlock = (iFrameSize * iMixerFreq) / SAMPLE_RATE;
		short buf[iOutputBlock * iChannels];
		bool ok = mix(buf, iOutputBlock);
		if (!ok) {
			memset(&buf, 0, sizeof(buf));
		}
		size_t nwritten = sio_write(handle, buf, sizeof(buf));
		if (nwritten != sizeof(buf)) {
			qWarning("SndioOutput: sio_write() short write");
			break;
		}
		if (prime > 0) {
			prime -= 1;
		}
	}

out:
	if (handle != NULL) {
		sio_close(handle);
	}
}
