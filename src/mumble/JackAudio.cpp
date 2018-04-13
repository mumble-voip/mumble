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
#include "User.h"
#include "Global.h"
#include "MainWindow.h"
#include "Timer.h"

#include <cstring>

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
	active = false;
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
		out_ports[0] = jack_port_register(client, "output_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
		out_ports[1] = jack_port_register(client, "output_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

		bJackIsGood = true;
		allocate_output_buffer(jack_get_buffer_size(client));
		if (bJackIsGood == false) {
			close_jack();
			return;
		}

		jack_set_process_callback(client, process_callback, this);
		jack_set_sample_rate_callback(client, srate_callback, this);
		jack_set_buffer_size_callback(client, buffer_size_callback, this);
		jack_on_shutdown(client, shutdown_callback, this);

		iSampleRate = jack_get_sample_rate(client);

		if (in_port == NULL || out_ports[0] == NULL || out_ports[1] == NULL) {
			close_jack();
			return;
		}

		// If we made it this far, then everything is okay
		qhInput.insert(QString(), tr("Hardware Ports"));
		qhOutput.insert(QString(), tr("Hardware Ports"));
		bJackIsGood = true;

	} else {
		bJackIsGood = false;
		client = NULL;
	}
}

void JackAudioSystem::close_jack() {

	if (client) {
		jack_deactivate(client);
		jack_client_close(client);

		delete [] output_buffer;
		output_buffer = NULL;

		client = NULL;
	}
	bJackIsGood = false;
}

void JackAudioSystem::activate()
{
	if (active) {
		return;
	}

	if (jack_activate(client)) {
		close_jack();
		return;
	}
	active = true;
}

int JackAudioSystem::process_callback(jack_nframes_t nframes, void *arg) {

	JackAudioSystem *jas = (JackAudioSystem*)arg;

	if (jas && jas->bJackIsGood) {
		AudioInputPtr ai = g.ai;
		AudioOutputPtr ao = g.ao;
		AudioInput *raw_ai = ai.get();
		AudioOutput *raw_ao = ao.get();
		JackAudioInput *jai = dynamic_cast<JackAudioInput *>(raw_ai);
		JackAudioOutput *jao = dynamic_cast<JackAudioOutput *>(raw_ao);

		if (jai && jai->isRunning() && jai->iMicChannels > 0 && !jai->isFinished()) {
			jai->qmMutex.lock();
			void* input = jack_port_get_buffer(jas->in_port, nframes);
			jai->addMic(input, nframes);
			jai->qmMutex.unlock();
		}

		if (jao && jao->isRunning() && jao->iChannels > 0 && !jao->isFinished()) {
			jao->qmMutex.lock();

			jack_default_audio_sample_t* port_buffers[JACK_OUTPUT_CHANNELS];
			for (unsigned int i = 0; i < jao->iChannels; ++i) {

				port_buffers[i] = (jack_default_audio_sample_t*)jack_port_get_buffer(jas->out_ports[i], nframes);
			}

			jack_default_audio_sample_t * const buffer = jas->output_buffer;
			memset(buffer, 0, sizeof(jack_default_audio_sample_t) * nframes * JACK_OUTPUT_CHANNELS);

			jao->mix(buffer, nframes);

			for (unsigned int i = 0; i < nframes * jao->iChannels; i += JACK_OUTPUT_CHANNELS) {

				const unsigned int buffer_pos = i / JACK_OUTPUT_CHANNELS;
				port_buffers[0][buffer_pos] = buffer[i];
				port_buffers[1][buffer_pos] = buffer[i+1];
			}
			jao->qmMutex.unlock();
		}
	}

	return 0;
}

int JackAudioSystem::srate_callback(jack_nframes_t frames, void *arg) {

	JackAudioSystem *jas = (JackAudioSystem*)arg;
	jas->iSampleRate = frames;
	return 0;
}

void JackAudioSystem::allocate_output_buffer(jack_nframes_t frames) {

	AudioOutputPtr ao = g.ao;
	JackAudioOutput * const jao = dynamic_cast<JackAudioOutput *>(ao.get());

	if (jao) {
		jao->qmMutex.lock();
	}
	if (output_buffer) {
		delete [] output_buffer;
		output_buffer = NULL;
	}
	output_buffer = new jack_default_audio_sample_t[frames * JACK_OUTPUT_CHANNELS];
	if (output_buffer) {
		memset(output_buffer, 0, sizeof(jack_default_audio_sample_t) * frames * JACK_OUTPUT_CHANNELS);
	} else {
		bJackIsGood = false;
	}

	if (jao) {
		jao->qmMutex.unlock();
	}
}

int JackAudioSystem::buffer_size_callback(jack_nframes_t frames, void *arg) {

	JackAudioSystem *jas = (JackAudioSystem*)arg;
	jas->allocate_output_buffer(frames);
	return 0;
}

void JackAudioSystem::shutdown_callback(void *arg) {

	JackAudioSystem *jas = (JackAudioSystem*)arg;
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

	foreach(const QString &dev, qlOutputDevs) {
		qlReturn << audioDevice(jasys->qhOutput.value(dev), dev);
	}

	return qlReturn;
}

void JackAudioOutputRegistrar::setDeviceChoice(const QVariant &choice, Settings &s) {
	Q_UNUSED(choice);
	Q_UNUSED(s);
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
		iChannels = JACK_OUTPUT_CHANNELS;
		iMixerFreq = jasys->iSampleRate;
		initializeMixer(chanmasks);
		jasys->activate();
	}

	qmMutex.lock();
	while (bRunning)
		qwcWait.wait(&qmMutex);
	qmMutex.unlock();
}
