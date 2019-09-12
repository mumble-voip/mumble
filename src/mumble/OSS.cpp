// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OSS.h"

#include <sys/soundcard.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "User.h"
#include "MainWindow.h"

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

#define NBLOCKS 8

class OSSEnumerator {
	public:
		QHash<QString,QString> qhInput;
		QHash<QString,QString> qhOutput;
		QHash<QString,QString> qhDevices;
		OSSEnumerator();
};

static OSSEnumerator *cards = NULL;

class OSSInit : public DeferInit {
		void initialize() {
			cards = new OSSEnumerator();
		};
		void destroy() {
			delete cards;
			cards = NULL;
		};
};

static OSSInit ossi;

class OSSInputRegistrar : public AudioInputRegistrar {
	public:
		OSSInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		virtual bool canEcho(const QString &) const;
};


class OSSOutputRegistrar : public AudioOutputRegistrar {
	public:
		OSSOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
};

static OSSInputRegistrar airOSS;
static OSSOutputRegistrar aorOSS;

OSSInputRegistrar::OSSInputRegistrar() : AudioInputRegistrar(QLatin1String("OSS")) {
}

AudioInput *OSSInputRegistrar::create() {
	return new OSSInput();
}

const QList<audioDevice> OSSInputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;

	QStringList qlInputDevs = cards->qhInput.keys();
	qSort(qlInputDevs);

	if (qlInputDevs.contains(g.s.qsOSSInput)) {
		qlInputDevs.removeAll(g.s.qsOSSInput);
		qlInputDevs.prepend(g.s.qsOSSInput);
	}

	foreach(const QString &dev, qlInputDevs) {
		qlReturn << audioDevice(cards->qhInput.value(dev), dev);
	}

	return qlReturn;
}

void OSSInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsOSSInput = choice.toString();
}

bool OSSInputRegistrar::canEcho(const QString &) const {
	return false;
}

OSSOutputRegistrar::OSSOutputRegistrar() : AudioOutputRegistrar(QLatin1String("OSS")) {
}

AudioOutput *OSSOutputRegistrar::create() {
	return new OSSOutput();
}

const QList<audioDevice> OSSOutputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;

	QStringList qlOutputDevs = cards->qhOutput.keys();
	qSort(qlOutputDevs);

	if (qlOutputDevs.contains(g.s.qsOSSOutput)) {
		qlOutputDevs.removeAll(g.s.qsOSSOutput);
		qlOutputDevs.prepend(g.s.qsOSSOutput);
	}

	foreach(const QString &dev, qlOutputDevs) {
		qlReturn << audioDevice(cards->qhOutput.value(dev), dev);
	}

	return qlReturn;
}

void OSSOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsOSSOutput = choice.toString();
}

OSSEnumerator::OSSEnumerator() {
	qhInput.insert(QString(), QLatin1String("Default OSS Device"));
	qhOutput.insert(QString(), QLatin1String("Default OSS Device"));
	qhDevices.insert(QString(), QLatin1String("/dev/dsp"));

#if (SOUND_VERSION >= 0x040002)
	int mixerfd = open("/dev/mixer", O_RDWR, 0);
	if (mixerfd == -1) {
		qWarning("OSSEnumerator: Failed to open /dev/mixer");
		return;
	}

	oss_sysinfo sysinfo;

	if (ioctl(mixerfd, SNDCTL_SYSINFO, &sysinfo) == -1) {
		qWarning("OSSEnumerator: Failed SNDCTL_SYSINFO");
		return;
	}

	for (int i=0;i< sysinfo.numaudios;i++) {
		oss_audioinfo ainfo;
		ainfo.dev = i;
		if (ioctl(mixerfd, SNDCTL_AUDIOINFO, &ainfo) == -1) {
			qWarning("OSSEnumerator: SNDCTL_AUDIOINFO failed for device %d", i);
			continue;
		}

		QString handle = QLatin1String(ainfo.handle);
		QString name = QLatin1String(ainfo.name);
		QString device = QLatin1String(ainfo.devnode);

		if (ainfo.caps & PCM_CAP_HIDDEN)
			continue;

		qhDevices.insert(handle, device);

		if (ainfo.caps & PCM_CAP_INPUT)
			qhInput.insert(handle, name);
		if (ainfo.caps & PCM_CAP_OUTPUT)
			qhOutput.insert(handle, name);
	}
	close(mixerfd);
#endif
}

OSSInput::OSSInput() {
	bRunning = true;
}

OSSInput::~OSSInput() {
	// Signal input thread to end
	bRunning = false;
	wait();
}

void OSSInput::run() {
	QByteArray device = cards->qhDevices.value(g.s.qsOSSInput).toLatin1();
	if (device.isEmpty()) {
		qWarning("OSSInput: Stored device not found, falling back to default");
		device = cards->qhDevices.value(QString()).toLatin1();
	}

	int fd = open(device.constData(), O_RDONLY, 0);
	if (fd == -1) {
		qWarning("OSSInput: Failed to open %s", device.constData());
		return;
	}

	int ival;

	ival = AFMT_S16_NE;
	if ((ioctl(fd, SNDCTL_DSP_SETFMT, &ival) == -1) || (ival != AFMT_S16_NE)) {
		qWarning("OSSInput: Failed to set sound format");
		goto out;
	}

	ival = 1;
	if ((ioctl(fd, SNDCTL_DSP_CHANNELS, &ival) == -1)) {
		qWarning("OSSInput: Failed to set mono mode");
		goto out;
	}
	iMicChannels = ival;

	ival = SAMPLE_RATE;
	if (ioctl(fd, SNDCTL_DSP_SPEED, &ival) == -1) {
		qWarning("OSSInput: Failed to set speed");
		goto out;
	}
	iMicFreq = ival;

	qWarning("OSSInput: Starting audio capture from %s", device.constData());

	eMicFormat = SampleShort;
	initializeMixer();

	while (bRunning) {
		short buffer[iMicLength];

		int len = static_cast<int>(iMicLength * iMicChannels * sizeof(short));
		ssize_t l = read(fd, buffer, len);
		if (l != len) {
			qWarning("OSSInput: Read %zd", l);
			break;
		}
		addMic(buffer, iMicLength);
	}

	qWarning("OSSInput: Releasing.");
	ioctl(fd, SNDCTL_DSP_RESET, NULL);

out:
	close(fd);
}

OSSOutput::OSSOutput() {
	bRunning = true;

	qWarning("OSSOutput: Initialized");
}

OSSOutput::~OSSOutput() {
	bRunning = false;
	// Call destructor of all children
	wipe();
	// Wait for terminate
	wait();
	qWarning("OSSOutput: Destroyed");
}

void OSSOutput::run() {
	QByteArray device = cards->qhDevices.value(g.s.qsOSSOutput).toLatin1();
	if (device.isEmpty()) {
		qWarning("OSSOutput: Stored device not found, falling back to default");
		device = cards->qhDevices.value(QString()).toLatin1();
	}

	int fd = open(device.constData(), O_WRONLY, 0);
	if (fd == -1) {
		qWarning("OSSOutput: Failed to open %s", device.constData());
		return;
	}

	int ival;

	ival = (g.s.iOutputDelay+1) << 16 | 11;

	if (ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &ival) == -1) {
		qWarning("OSSOutput: Failed to set fragment");
	}

	ival = AFMT_S16_NE;
	if ((ioctl(fd, SNDCTL_DSP_SETFMT, &ival) == -1) || (ival != AFMT_S16_NE)) {
		qWarning("OSSOutput: Failed to set sound format");
		if ((ival != AFMT_S16_NE))
			close(fd);
		return;
	}

	iChannels = 0;

	if (g.s.doPositionalAudio())
		iChannels = 2;
	else
		iChannels = 1;

	ival = iChannels;
	if ((ioctl(fd, SNDCTL_DSP_CHANNELS, &ival) == -1) && (ival == static_cast<int>(iChannels))) {
		qWarning("OSSOutput: Failed to set channels");
		return;
	}
	iChannels = ival;

	ival = SAMPLE_RATE;
	if (ioctl(fd, SNDCTL_DSP_SPEED, &ival) == -1) {
		qWarning("OSSOutput: Failed to set speed");
		return;
	}
	iMixerFreq = ival;

	const unsigned int chanmasks[32] = {
		SPEAKER_FRONT_LEFT,
		SPEAKER_FRONT_RIGHT,
		SPEAKER_FRONT_CENTER,
		SPEAKER_LOW_FREQUENCY,
		SPEAKER_BACK_LEFT,
		SPEAKER_BACK_RIGHT,
		SPEAKER_SIDE_LEFT,
		SPEAKER_SIDE_RIGHT,
		SPEAKER_BACK_CENTER
	};

	eSampleFormat = SampleShort;

	initializeMixer(chanmasks);

	int iOutputBlock = (iFrameSize * iMixerFreq) / SAMPLE_RATE;

	qWarning("OSSOutput: Starting audio playback to %s", device.constData());

	ssize_t blocklen = iOutputBlock * iChannels * sizeof(short);
	short mbuffer[iOutputBlock * iChannels];

	while (bRunning) {
		bool stillRun = mix(mbuffer, iOutputBlock);
		if (stillRun) {
			ssize_t l = write(fd, mbuffer, blocklen);
			if (l != blocklen) {
				qWarning("OSSOutput: Write %zd != %zd", l, blocklen);
				break;
			}
		} else {
			while (! mix(mbuffer, iOutputBlock) && bRunning)
				this->msleep(20);
			ssize_t l = write(fd, mbuffer, blocklen);
			if (l != blocklen) {
				qWarning("OSSOutput: Write %zd != %zd", l, blocklen);
				break;
			}
		}
	}
	qWarning("OSSOutput: Releasing device");
	ioctl(fd, SNDCTL_DSP_RESET, NULL);
	close(fd);
}
