// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OSS.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

#include "MainWindow.h"
#include "User.h"
#include "Global.h"

#include <memory>
#include <vector>

#define NBLOCKS 8

class OSSEnumerator {
public:
	QHash< QString, QString > qhInput;
	QHash< QString, QString > qhOutput;
	QHash< QString, QString > qhDevices;
	OSSEnumerator();
};

static OSSEnumerator *cards = nullptr;

class OSSInit : public DeferInit {
	void initialize() { cards = new OSSEnumerator(); };
	void destroy() {
		delete cards;
		cards = nullptr;
	};
};

static OSSInit ossi;

class OSSInputRegistrar : public AudioInputRegistrar {
public:
	OSSInputRegistrar();
	virtual AudioInput *create();
	virtual const QVariant getDeviceChoice();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
	virtual bool canEcho(EchoCancelOptionID echoCancelID, const QString &outputSystem) const;
	virtual bool isMicrophoneAccessDeniedByOS() { return false; };
};


class OSSOutputRegistrar : public AudioOutputRegistrar {
public:
	OSSOutputRegistrar();
	virtual AudioOutput *create();
	virtual const QVariant getDeviceChoice();
	virtual const QList< audioDevice > getDeviceChoices();
	virtual void setDeviceChoice(const QVariant &, Settings &);
};

static OSSInputRegistrar airOSS;
static OSSOutputRegistrar aorOSS;

OSSInputRegistrar::OSSInputRegistrar() : AudioInputRegistrar(QLatin1String("OSS")) {
}

AudioInput *OSSInputRegistrar::create() {
	return new OSSInput();
}

const QVariant OSSInputRegistrar::getDeviceChoice() {
	return Global::get().s.qsOSSInput;
}

const QList< audioDevice > OSSInputRegistrar::getDeviceChoices() {
	QList< audioDevice > choices;

	QStringList keys = cards->qhInput.keys();
	std::sort(keys.begin(), keys.end());

	for (const auto &key : keys) {
		choices << audioDevice(cards->qhInput.value(key), key);
	}

	return choices;
}

void OSSInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	s.qsOSSInput = choice.toString();
}

bool OSSInputRegistrar::canEcho(EchoCancelOptionID, const QString &) const {
	return false;
}

OSSOutputRegistrar::OSSOutputRegistrar() : AudioOutputRegistrar(QLatin1String("OSS")) {
}

AudioOutput *OSSOutputRegistrar::create() {
	return new OSSOutput();
}

const QVariant OSSOutputRegistrar::getDeviceChoice() {
	return Global::get().s.qsOSSOutput;
}

const QList< audioDevice > OSSOutputRegistrar::getDeviceChoices() {
	QList< audioDevice > choices;

	QStringList keys = cards->qhOutput.keys();
	std::sort(keys.begin(), keys.end());

	for (const auto &key : keys) {
		choices << audioDevice(cards->qhOutput.value(key), key);
	}

	return choices;
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

	for (int i = 0; i < sysinfo.numaudios; i++) {
		oss_audioinfo ainfo;
		ainfo.dev = i;
		if (ioctl(mixerfd, SNDCTL_AUDIOINFO, &ainfo) == -1) {
			qWarning("OSSEnumerator: SNDCTL_AUDIOINFO failed for device %d", i);
			continue;
		}

		QString handle = QLatin1String(ainfo.handle);
		QString name   = QLatin1String(ainfo.name);
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

class FileDescriptor {
public:
	FileDescriptor(int fd = -1) : m_fd(fd) {}
	~FileDescriptor() {
		if (m_fd != -1) {
			close(m_fd);
			m_fd = -1;
		}
	}

	operator int() const { return m_fd; }

private:
	int m_fd = -1;
};

void OSSInput::run() {
	QByteArray device = cards->qhDevices.value(Global::get().s.qsOSSInput).toLatin1();
	if (device.isEmpty()) {
		qWarning("OSSInput: Stored device not found, falling back to default");
		device = cards->qhDevices.value(QString()).toLatin1();
	}

	FileDescriptor fd(open(device.constData(), O_RDONLY, 0));
	if (fd == -1) {
		qWarning("OSSInput: Failed to open %s", device.constData());
		return;
	}

	int ival;

	ival = AFMT_S16_NE;
	if ((ioctl(fd, SNDCTL_DSP_SETFMT, &ival) == -1) || (ival != AFMT_S16_NE)) {
		qWarning("OSSInput: Failed to set sound format");
		return;
	}

	ival = 1;
	if ((ioctl(fd, SNDCTL_DSP_CHANNELS, &ival) == -1)) {
		qWarning("OSSInput: Failed to set mono mode");
		return;
	}
	iMicChannels = static_cast< unsigned int >(ival);

	ival = SAMPLE_RATE;
	if (ioctl(fd, SNDCTL_DSP_SPEED, &ival) == -1) {
		qWarning("OSSInput: Failed to set speed");
		return;
	}
	iMicFreq = static_cast< unsigned int >(ival);

	qWarning("OSSInput: Starting audio capture from %s", device.constData());

	eMicFormat = SampleShort;
	initializeMixer();

	std::vector< short > buffer;
	buffer.resize(iMicLength);
	while (bRunning) {
		std::size_t len = iMicLength * iMicChannels * sizeof(short);
		ssize_t l       = read(fd, buffer.data(), len);
		if (l != static_cast< ssize_t >(len)) {
			qWarning("OSSInput: Read %zd", l);
			break;
		}
		addMic(buffer.data(), iMicLength);
	}

	qWarning("OSSInput: Releasing.");
	ioctl(fd, SNDCTL_DSP_RESET, nullptr);
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
	QByteArray device = cards->qhDevices.value(Global::get().s.qsOSSOutput).toLatin1();
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

	ival = (Global::get().s.iOutputDelay + 1) << 16 | 11;

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

	iChannels = 2;

	ival = static_cast< int >(iChannels);
	if ((ioctl(fd, SNDCTL_DSP_CHANNELS, &ival) == -1) && (ival == static_cast< int >(iChannels))) {
		qWarning("OSSOutput: Failed to set channels");
		return;
	}
	iChannels = static_cast< unsigned int >(ival);

	ival = SAMPLE_RATE;
	if (ioctl(fd, SNDCTL_DSP_SPEED, &ival) == -1) {
		qWarning("OSSOutput: Failed to set speed");
		return;
	}
	iMixerFreq = static_cast< unsigned int >(ival);

	const unsigned int chanmasks[32] = { SPEAKER_FRONT_LEFT,    SPEAKER_FRONT_RIGHT, SPEAKER_FRONT_CENTER,
										 SPEAKER_LOW_FREQUENCY, SPEAKER_BACK_LEFT,   SPEAKER_BACK_RIGHT,
										 SPEAKER_SIDE_LEFT,     SPEAKER_SIDE_RIGHT,  SPEAKER_BACK_CENTER };

	eSampleFormat = SampleShort;

	initializeMixer(chanmasks);

	unsigned int iOutputBlock = (iFrameSize * iMixerFreq) / SAMPLE_RATE;

	qWarning("OSSOutput: Starting audio playback to %s", device.constData());

	std::size_t blocklen = iOutputBlock * iChannels * sizeof(short);
	static std::vector< short > mbuffer;
	mbuffer.resize(iOutputBlock * iChannels);

	while (bRunning) {
		bool stillRun = mix(mbuffer.data(), iOutputBlock);
		if (stillRun) {
			ssize_t l = write(fd, mbuffer.data(), blocklen);
			if (l != static_cast< ssize_t >(blocklen)) {
				qWarning("OSSOutput: Write %zd != %zd", l, blocklen);
				break;
			}
		} else {
			while (!mix(mbuffer.data(), iOutputBlock) && bRunning)
				this->msleep(20);
			ssize_t l = write(fd, mbuffer.data(), blocklen);
			if (l != static_cast< ssize_t >(blocklen)) {
				qWarning("OSSOutput: Write %zd != %zd", l, blocklen);
				break;
			}
		}
	}
	qWarning("OSSOutput: Releasing device");
	ioctl(fd, SNDCTL_DSP_RESET, nullptr);
	close(fd);
}

#undef NBLOCKS
