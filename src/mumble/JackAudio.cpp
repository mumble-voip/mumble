/* Copyright (C) 2011, Benjamin Jemlich <pcgod@users.sourceforge.net>
   Copyright (C) 2011, Filipe Coelho <falktx@gmail.com>
   Copyright (C) 2018, Bernd Buschinski <b.buschinski@gmail.com>

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

#include "JackAudio.h"

#include "Global.h"


static JackAudioSystem *jasys = NULL;

class JackAudioInputRegistrar : public AudioInputRegistrar {
	public:
		JackAudioInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
		virtual bool canEcho(const QString &) const;
};

class JackAudioOutputRegistrar : public AudioOutputRegistrar {
	public:
		JackAudioOutputRegistrar();
		virtual AudioOutput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &, Settings &);
};

class JackAudioInit : public DeferInit {
	public:
		JackAudioInputRegistrar *airJackAudio;
		JackAudioOutputRegistrar *aorJackAudio;
		void initialize() {
			jasys = new JackAudioSystem();
			jasys->init_jack();
			jasys->qmWait.lock();
			jasys->qwcWait.wait(&jasys->qmWait, 1000);
			jasys->qmWait.unlock();
			if (jasys->bJackIsGood) {
				airJackAudio = new JackAudioInputRegistrar();
				aorJackAudio = new JackAudioOutputRegistrar();
			} else {
				airJackAudio = NULL;
				aorJackAudio = NULL;
				delete jasys;
				jasys = NULL;
			}
		};
		void destroy() {
			if (airJackAudio)
				delete airJackAudio;
			if (aorJackAudio)
				delete aorJackAudio;
			if (jasys) {
				jasys->close_jack();
				delete jasys;
				jasys = NULL;
			}
		};
};

static JackAudioInit jackinit; //unused

JackAudioSystem::JackAudioSystem() {
	bJackIsGood = false;
	iSampleRate = 0;
	output_buffer = NULL;
	bActive = false;
	if (g.s.qsJackAudioOutput.isEmpty()) {
		iOutPorts = 1;
	} else {
		iOutPorts = g.s.qsJackAudioOutput.toInt();
	}
	memset((void*)&out_ports, 0, sizeof(out_ports));
}

JackAudioSystem::~JackAudioSystem() {
}

void JackAudioSystem::init_jack() {

	output_buffer = NULL;

	/* TODO make option */
	jack_options_t jack_option = false ? JackNullOption : JackNoStartServer;
	client = jack_client_open("mumble", jack_option, 0);

	if (client) {
		in_port = jack_port_register(client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

		bJackIsGood = true;
		iBufferSize = jack_get_buffer_size(client);
		iSampleRate = jack_get_sample_rate(client);

		setNumberOfOutPorts(iOutPorts);

		if (bJackIsGood == false) {
			close_jack();
			return;
		}

		jack_set_process_callback(client, process_callback, this);
		jack_set_sample_rate_callback(client, srate_callback, this);
		jack_set_buffer_size_callback(client, buffer_size_callback, this);
		jack_on_shutdown(client, shutdown_callback, this);

		if (in_port == NULL) {
			close_jack();
			return;
		}

		// If we made it this far, then everything is okay
		qhInput.insert(QString(), tr("Hardware Ports"));
		qhOutput.insert(QString::number(1), tr("Mono"));
		qhOutput.insert(QString::number(2), tr("Stereo"));
		bJackIsGood = true;

	} else {
		bJackIsGood = false;
		client = NULL;
	}
}

void JackAudioSystem::close_jack() {

	QMutexLocker lock(&qmWait);
	if (client) {
		jack_deactivate(client);
		bActive = false;

		if (in_port != NULL) {
			jack_port_unregister(client, in_port);
		}

		for (unsigned i = 0; i < iOutPorts; ++i) {
			if (out_ports[i] != NULL) {
				jack_port_unregister(client, out_ports[0]);
			}
		}

		jack_client_close(client);

		delete [] output_buffer;
		output_buffer = NULL;

		client = NULL;
	}
	bJackIsGood = false;
}

void JackAudioSystem::activate()
{
	QMutexLocker lock(&qmWait);
	if (client) {
		if (bActive) {
			return;
		}

		if (jack_activate(client) != 0) {
			close_jack();
			return;
		}
		bActive = true;
	}
}

int JackAudioSystem::process_callback(jack_nframes_t nframes, void *arg) {

	JackAudioSystem * const jas = (JackAudioSystem*)arg;

	if (jas && jas->bJackIsGood) {
		AudioInputPtr ai = g.ai;
		AudioOutputPtr ao = g.ao;
		JackAudioInput * const jai = dynamic_cast<JackAudioInput *>(ai.get());
		JackAudioOutput * const jao = dynamic_cast<JackAudioOutput *>(ao.get());

		if (jai && jai->isRunning() && jai->iMicChannels > 0 && !jai->isFinished()) {
			jai->qmMutex.lock();
			void* input = jack_port_get_buffer(jas->in_port, nframes);
			jai->addMic(input, nframes);
			jai->qmMutex.unlock();
		}

		if (jao && jao->isRunning() && jao->iChannels > 0 && !jao->isFinished()) {
			jao->qmMutex.lock();

			jack_default_audio_sample_t* port_buffers[JACK_MAX_OUTPUT_PORTS];
			for (unsigned int i = 0; i < jao->iChannels; ++i) {

				port_buffers[i] = (jack_default_audio_sample_t*)jack_port_get_buffer(jas->out_ports[i], nframes);
			}

			jack_default_audio_sample_t * const buffer = jas->output_buffer;
			memset(buffer, 0, sizeof(jack_default_audio_sample_t) * nframes * jao->iChannels);

			jao->mix(buffer, nframes);

			if (jao->iChannels == 1) {

				memcpy(port_buffers[0], buffer, sizeof(jack_default_audio_sample_t) * nframes);
			} else {

				// de-interleave channels
				for (unsigned int i = 0; i < nframes * jao->iChannels; ++i) {
					port_buffers[i % jao->iChannels][i/jao->iChannels] = buffer[i];
				}
			}
			jao->qmMutex.unlock();
		}
	}

	return 0;
}

int JackAudioSystem::srate_callback(jack_nframes_t frames, void *arg) {

	JackAudioSystem * const jas = (JackAudioSystem*)arg;
	jas->iSampleRate = frames;
	return 0;
}

void JackAudioSystem::allocOutputBuffer(jack_nframes_t frames) {

	iBufferSize = frames;
	AudioOutputPtr ao = g.ao;
	JackAudioOutput * const jao = dynamic_cast<JackAudioOutput *>(ao.get());

	if (jao) {
		jao->qmMutex.lock();
	}
	if (output_buffer) {
		delete [] output_buffer;
		output_buffer = NULL;
	}
	output_buffer = new jack_default_audio_sample_t[frames * numberOfOutPorts()];
	if (output_buffer == NULL) {
		bJackIsGood = false;
	}

	if (jao) {
		jao->qmMutex.unlock();
	}
}

void JackAudioSystem::setNumberOfOutPorts(unsigned int ports) {

	AudioOutputPtr ao = g.ao;
	JackAudioOutput * const jao = dynamic_cast<JackAudioOutput *>(ao.get());
	unsigned int oldSize = iOutPorts;

	iOutPorts = qBound<unsigned>(1, ports, JACK_MAX_OUTPUT_PORTS);

	allocOutputBuffer(iBufferSize);

	if (jao) {
		jao->qmMutex.lock();
	}

	if (bActive)
		jack_deactivate(client);

	for (unsigned int i = 0; i < oldSize; ++i) {
		if (out_ports[i] != NULL) {
			jack_port_unregister(client, out_ports[i]);
			out_ports[i] = NULL;
		}
	}

	for (unsigned int i = 0; i < iOutPorts; ++i) {

		char name[10];
		snprintf(name, 10, "output_%d", i + 1);

		out_ports[i] = jack_port_register(client, name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
		if (out_ports[i] == NULL) {
			bJackIsGood = false;
			break;
		}
	}

	if (bActive)
		jack_activate(client);

	if (jao) {
		jao->qmMutex.unlock();
	}
}

unsigned int JackAudioSystem::numberOfOutPorts() const {

	return iOutPorts;
}

int JackAudioSystem::buffer_size_callback(jack_nframes_t frames, void *arg) {

	JackAudioSystem * const jas = (JackAudioSystem*)arg;
	jas->allocOutputBuffer(frames);
	return 0;
}

void JackAudioSystem::shutdown_callback(void *arg) {

	JackAudioSystem * const jas = (JackAudioSystem*)arg;
	jas->bJackIsGood = false;
}

JackAudioInputRegistrar::JackAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("JACK"), 10) {
}

AudioInput *JackAudioInputRegistrar::create() {
	return new JackAudioInput();
}

const QList<audioDevice> JackAudioInputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;

	QStringList qlInputDevs = jasys->qhInput.keys();
	qSort(qlInputDevs);

	foreach(const QString &dev, qlInputDevs) {
		qlReturn << audioDevice(jasys->qhInput.value(dev), dev);
	}

	return qlReturn;
}

void JackAudioInputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	Q_UNUSED(choice);
	Q_UNUSED(s);
}

bool JackAudioInputRegistrar::canEcho(const QString &osys) const {
	Q_UNUSED(osys);
	return false;
}

JackAudioOutputRegistrar::JackAudioOutputRegistrar() : AudioOutputRegistrar(QLatin1String("JACK"), 10) {
}

AudioOutput *JackAudioOutputRegistrar::create() {
	return new JackAudioOutput();
}

const QList<audioDevice> JackAudioOutputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;

	QStringList qlOutputDevs = jasys->qhOutput.keys();
	qSort(qlOutputDevs);

	if (qlOutputDevs.contains(g.s.qsJackAudioOutput)) {
		qlOutputDevs.removeAll(g.s.qsJackAudioOutput);
		qlOutputDevs.prepend(g.s.qsJackAudioOutput);
	}

	foreach(const QString &dev, qlOutputDevs) {
		qlReturn << audioDevice(jasys->qhOutput.value(dev), dev);
	}

	return qlReturn;
}

void JackAudioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {

	s.qsJackAudioOutput = choice.toString();
	jasys->setNumberOfOutPorts(choice.toInt());
}

JackAudioInput::JackAudioInput() {
	bRunning = true;
	iMicChannels = 0;
};

JackAudioInput::~JackAudioInput() {
	bRunning = false;
	iMicChannels = 0;
	qmMutex.lock();
	qwcWait.wakeAll();
	qmMutex.unlock();
	wait();
}

void JackAudioInput::run() {
	if (jasys && jasys->bJackIsGood) {
		iMicFreq = jasys->iSampleRate;
		iMicChannels = 1;
		eMicFormat = SampleFloat;
		initializeMixer();
		jasys->activate();
	}

	qmMutex.lock();
	while (bRunning)
		qwcWait.wait(&qmMutex);
	qmMutex.unlock();
}

JackAudioOutput::JackAudioOutput() {
	bRunning = true;
	iChannels = 0;
}

JackAudioOutput::~JackAudioOutput() {
	bRunning = false;
	iChannels = 0;
	qmMutex.lock();
	qwcWait.wakeAll();
	qmMutex.unlock();
	wait();
}

void JackAudioOutput::run() {
	if (jasys && jasys->bJackIsGood) {
		unsigned int chanmasks[32];

		chanmasks[0] = SPEAKER_FRONT_LEFT;
		chanmasks[1] = SPEAKER_FRONT_RIGHT;

		eSampleFormat = SampleFloat;
		iChannels = jasys->numberOfOutPorts();
		iMixerFreq = jasys->iSampleRate;
		initializeMixer(chanmasks);
		jasys->activate();
	}

	qmMutex.lock();
	while (bRunning)
		qwcWait.wait(&qmMutex);
	qmMutex.unlock();
}
