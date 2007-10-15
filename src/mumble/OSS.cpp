/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#include "OSS.h"
#include "Player.h"
#include "Global.h"
#include "MainWindow.h"
#include <sys/soundcard.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#define NBLOCKS 8

#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

class OSSEnumerator {
	public:
		QHash<QString,QString> qhInput;
		QHash<QString,QString> qhOutput;
		QHash<QString,QString> qhDevices;
		OSSEnumerator();
};

static OSSEnumerator cards;

class OSSInputRegistrar : public AudioInputRegistrar {
	public:
		OSSInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &);
};


class OSSOutputRegistrar : public AudioOutputRegistrar {
	public:
		OSSOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &);
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

	QHash<QString,QString>::const_iterator i;

	for (i=cards.qhInput.constBegin();i!=cards.qhInput.constEnd();++i) {
		qlReturn << audioDevice(i.value(), i.key());
	}
	return qlReturn;
}

void OSSInputRegistrar::setDeviceChoice(const QVariant &choice) {
	g.s.qsOSSInput = choice.toString();
}

OSSOutputRegistrar::OSSOutputRegistrar() : AudioOutputRegistrar(QLatin1String("OSS")) {
}

AudioOutput *OSSOutputRegistrar::create() {
	return new OSSOutput();
}

const QList<audioDevice> OSSOutputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;

	QHash<QString,QString>::const_iterator i;

	for (i=cards.qhOutput.constBegin();i!=cards.qhOutput.constEnd();++i) {
		qlReturn << audioDevice(i.value(), i.key());
	}
	return qlReturn;
}

void OSSOutputRegistrar::setDeviceChoice(const QVariant &choice) {
	g.s.qsOSSOutput = choice.toString();
}

static ConfigWidget *OSSConfigDialogNew(Settings &st) {
	return new OSSConfig(st);
}

static ConfigRegistrar registrar(21, OSSConfigDialogNew);


OSSEnumerator::OSSEnumerator() {
	qhInput.insert(QString(), QLatin1String("Default OSS Device"));
	qhOutput.insert(QString(), QLatin1String("Default OSS Device"));
	qhDevices.insert(QString(), QLatin1String("/dev/dsp"));
	
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
	
	for(int i=0;i< sysinfo.numaudios;i++) {
		oss_audioinfo ainfo;
		ainfo.dev = i;
		if(ioctl(mixerfd, SNDCTL_AUDIOINFO, &ainfo) == -1) {
			qWarning("OSSEnumerator: SNDCTL_AUDIOINFO failed for device %d", i);
			continue;
		}
		qWarning("DeviceNode: %s", ainfo.devnode);
		qWarning("DeviceName: %s", ainfo.name);
		
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
}

OSSConfig::OSSConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	QList<QString> qlOutputDevs = cards.qhOutput.keys();
	qSort(qlOutputDevs);
	QList<QString> qlInputDevs = cards.qhInput.keys();
	qSort(qlInputDevs);

	bool found;

	found = false;
	foreach(QString dev, qlInputDevs) {
		qcbInputDevice->addItem(cards.qhInput.value(dev), dev);
		if (dev == g.s.qsOSSInput) {
			found = true;
			qcbInputDevice->setCurrentIndex(qcbInputDevice->count() - 1);
		}
	}

	found = false;
	foreach(QString dev, qlOutputDevs) {
		qcbOutputDevice->addItem(cards.qhOutput.value(dev), dev);
		if (dev == g.s.qsOSSOutput) {
			found = true;
			qcbOutputDevice->setCurrentIndex(qcbOutputDevice->count() - 1);
		}
	}

	qcbOutputDevice->setWhatsThis(qcbInputDevice->whatsThis());
	qsOutputDelay->setValue(g.s.iDXOutputDelay);
	on_qsOutputDelay_valueChanged(qsOutputDelay->value());
}

QString OSSConfig::title() const {
	return tr("OSS");
}

QIcon OSSConfig::icon() const {
	return QIcon(QLatin1String("skin:config_dsound.png"));
}

void OSSConfig::save() const {
	s.iDXOutputDelay = qsOutputDelay->value();
	s.qsOSSInput = qcbInputDevice->itemData(qcbInputDevice->currentIndex()).toString();
	s.qsOSSOutput = qcbOutputDevice->itemData(qcbOutputDevice->currentIndex()).toString();
}

void OSSConfig::load(const Settings &r) {
	for(int i=0;i<qcbInputDevice->count();i++) {
		if (qcbInputDevice->itemData(i).toString() == r.qsOSSInput) {
			qcbInputDevice->setCurrentIndex(i);
			break;
		}
	}

	for(int i=0;i<qcbOutputDevice->count();i++) {
		if (qcbOutputDevice->itemData(i).toString() == r.qsOSSOutput) {
			qcbOutputDevice->setCurrentIndex(i);
			break;
		}
	}

	qsOutputDelay->setValue(r.iDXOutputDelay);
}

bool OSSConfig::expert(bool b) {
	qcbOutput->setVisible(b);
	return true;
}

void OSSConfig::on_qsOutputDelay_valueChanged(int v) {
	qlOutputDelay->setText(tr("%1ms").arg(v*20));
}

OSSInput::OSSInput() {
}

OSSInput::~OSSInput() {
	// Signal input thread to end
	bRunning = false;
	wait();
}

void OSSInput::run() {
	bRunning = true;

	QByteArray device = cards.qhDevices.value(g.s.qsOSSInput).toLatin1();
	if (device.isEmpty()) {
		qWarning("OSSInput: Stored device not found, falling back to default");
		device = cards.qhDevices.value(QString()).toLatin1();
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
		return;
	}
	
	ival = 1;
	if ((ioctl(fd, SNDCTL_DSP_CHANNELS, &ival) == -1) || (ival != 1)) {
		qWarning("OSSInput: Failed to set mono mode");
		return;
	}
	
	ival = SAMPLE_RATE;
	if (ioctl(fd, SNDCTL_DSP_SPEED, &ival) == -1) {
		qWarning("OSSInput: Failed to set speed");
		return;
	}
	
	qWarning("OSSInput: Staring audio capture from %s", device.constData());
	
	while (bRunning) {
		int l = read(fd, psMic, iFrameSize * 2);
		if (l != iFrameSize * 2) {
			qWarning("OSSInput: Read %d", l);
			break;
		}
		encodeAudioFrame();
	}
	
	qWarning("OSSInput: Releasing.");
	ioctl(fd, SNDCTL_DSP_HALT, NULL);
	close(fd);
}

OSSOutput::OSSOutput() {
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
	bRunning = true;

	QByteArray device = cards.qhDevices.value(g.s.qsOSSOutput).toLatin1();
	if (device.isEmpty()) {
		qWarning("OSSOutput: Stored device not found, falling back to default");
		device = cards.qhDevices.value(QString()).toLatin1();
	}
	
	int fd = open(device.constData(), O_WRONLY, 0);
	if (fd == -1) {
		qWarning("OSSOutput: Failed to open %s", device.constData());
		return;
	}
	
	int ival;
	
	ival = (g.s.iDXOutputDelay+1) << 16 | 10;
	
	if (ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &ival) == -1) {
		qWarning("OSSOutput: Failed to set fragment");
	}
	
	ival = AFMT_S16_NE;
	if ((ioctl(fd, SNDCTL_DSP_SETFMT, &ival) == -1) || (ival != AFMT_S16_NE)) {
		qWarning("OSSOutput: Failed to set sound format");
		return;
	}
	
	ival = 1;
	if ((ioctl(fd, SNDCTL_DSP_CHANNELS, &ival) == -1) || (ival != 1)) {
		qWarning("OSSOutput: Failed to set mono mode");
		return;
	}
	
	ival = SAMPLE_RATE;
	if (ioctl(fd, SNDCTL_DSP_SPEED, &ival) == -1) {
		qWarning("OSSOutput: Failed to set speed");
		return;
	}
	
	qWarning("OSSOutput: Staring audio playback to %s", device.constData());

	short buffer[iFrameSize] __attribute__((aligned(16)));

	while (bRunning) {
		bool stillRun = mixAudio(buffer);
		int l = write(fd, buffer, iFrameSize * 2);
		if (l != iFrameSize * 2) {
			qWarning("OSSOutput: Write %d", l);
			break;
		}
/*		
		ioctl(fd, SNDCTL_DSP_GETODELAY, &ival);
		qWarning("Delay %d", ival / 2);
*/
		if (! stillRun) {
			while (! mixAudio(buffer) && bRunning)
				this->usleep(20);

			if (! bRunning)
				break;
		}
	}
	qWarning("OSSOutput: Releasing device");
	ioctl(fd, SNDCTL_DSP_HALT, NULL);
	close(fd);
}
