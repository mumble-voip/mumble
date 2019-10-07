// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "JackAudio.h"

#include "Global.h"


static JackAudioSystem *jasys = NULL;

// jackStatusToStringList converts a jack_status_t (a flag type
// that can contain multiple Jack statuses) to a QStringList.
QStringList jackStatusToStringList(jack_status_t status) {
	QStringList statusList;

	if ((status & JackFailure) != 0) {
		statusList << QLatin1String("JackFailure - overall operation failed");
	}
	if ((status & JackInvalidOption) != 0) {
		statusList << QLatin1String("JackInvalidOption - the operation contained an invalid or unsupported option");
	}
	if ((status & JackNameNotUnique) != 0)  {
		statusList << QLatin1String("JackNameNotUnique - the desired client name is not unique");
	}
	if ((status & JackServerStarted) != 0) {
		statusList << QLatin1String("JackServerStarted - the server was started as a result of this operation");
	}
	if ((status & JackServerFailed) != 0) {
		statusList << QLatin1String("JackServerFailed - unable to connect to the JACK server");
	}
	if ((status & JackServerError) != 0) {
		statusList << QLatin1String("JackServerError - communication error with the JACK server");
	}
	if ((status & JackNoSuchClient) != 0) {
		statusList << QLatin1String("JackNoSuchClient - requested client does not exist");
	}
	if ((status & JackLoadFailure) != 0) {
		statusList << QLatin1String("JackLoadFailure - unable to load initial client");
	}
	if ((status & JackInitFailure) != 0) {
		statusList << QLatin1String("JackInitFailure - unable to initialize client");
	}
	if ((status & JackShmFailure) != 0)  {
		statusList << QLatin1String("JackShmFailure - unable to access shared memory");
	}
	if ((status & JackVersionError) != 0) {
		statusList << QLatin1String("JackVersionError - client's protocol version does not match");
	}
	if ((status & JackBackendError) != 0) {
		statusList << QLatin1String("JackBackendError - a backend error occurred");
	}
	if ((status & JackClientZombie) != 0) {
		statusList << QLatin1String("JackClientZombie - client zombified");
	}

	return statusList;
}

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
		}

		void destroy() {
			delete airJackAudio;
			delete aorJackAudio;
			if (jasys) {
				delete jasys;
				jasys = NULL;
			}
		}
};

static JackAudioInit jackinit; // To instantiate the classes (JackAudioSystem, JackAudioInputRegistrar and JackAudioOutputRegistrar).

JackAudioSystem::JackAudioSystem()
	: bActive(false)
	, client(NULL)
	, in_port(NULL)
	, output_buffer(NULL)
	, iBufferSize(0)
	, bJackIsGood(false)
	, bInputIsGood(false)
	, bOutputIsGood(false)
	, iSampleRate(0)
{
	if (g.s.qsJackAudioOutput.isEmpty()) {
		iOutPorts = 1;
	} else {
		iOutPorts = g.s.qsJackAudioOutput.toInt();
	}
	memset(reinterpret_cast<void *>(&out_ports), 0, sizeof(out_ports));

	qhInput.insert(QString(), tr("Hardware Ports"));
	qhOutput.insert(QString::number(1), tr("Mono"));
	qhOutput.insert(QString::number(2), tr("Stereo"));

	jack_status_t status = static_cast<jack_status_t>(0);
	int err = 0;

	jack_options_t jack_option = g.s.bJackStartServer ? JackNullOption : JackNoStartServer;
	client = jack_client_open(g.s.qsJackClientName.toStdString().c_str(), jack_option, &status);

	if (!client) {
		QStringList errors = jackStatusToStringList(status);
		qWarning("JackAudioSystem: unable to open client due to %i errors:", errors.count());
		for (int i = 0; i < errors.count(); ++i) {
			qWarning("JackAudioSystem: %s", qPrintable(errors.at(i)));
		}

		return;
	}

	qWarning("JackAudioSystem: client \"%s\" opened successfully", jack_get_client_name(client));
	iBufferSize = jack_get_buffer_size(client);
	iSampleRate = jack_get_sample_rate(client);

	err = jack_set_process_callback(client, process_callback, this);
	if (err != 0) {
		qWarning("JackAudioSystem: unable to set process callback - jack_set_process_callback() returned %i", err);
		return;
	}

	err = jack_set_sample_rate_callback(client, srate_callback, this);
	if (err != 0) {
		qWarning("JackAudioSystem: unable to set sample rate callback - jack_set_sample_rate_callback() returned %i", err);
		return;
	}

	err = jack_set_buffer_size_callback(client, buffer_size_callback, this);
	if (err != 0) {
		qWarning("JackAudioSystem: unable to set buffer size callback - jack_set_buffer_size_callback() returned %i", err);
		return;
	}

	jack_on_shutdown(client, shutdown_callback, this);

	// If we made it this far, then everything is okay
	bJackIsGood = true;
}

JackAudioSystem::~JackAudioSystem() {
	QMutexLocker lock(&qmWait);

	if (client) {
		int err = 0;
		err = jack_deactivate(client);
		if (err != 0)  {
			qWarning("JackAudioSystem: unable to remove client from the process graph - jack_deactivate() returned %i", err);
		}

		bActive = false;

		err = jack_client_close(client);
		if (err != 0) {
			qWarning("JackAudioSystem: unable to disconnect from the server - jack_client_close() returned %i", err);
		}

		delete [] output_buffer;
		output_buffer = NULL;

		client = NULL;
	}

	bJackIsGood = false;
}

void JackAudioSystem::auto_connect_ports() {
	if (!(client && g.s.bJackAutoConnect)) {
		return;
	}

	disconnect_ports();

	const char **ports = NULL;
	const int wanted_out_flags = JackPortIsPhysical | JackPortIsOutput;
	const int wanted_in_flags = JackPortIsPhysical | JackPortIsInput;
	int err;
	unsigned int connected_out_ports = 0;
	unsigned int connected_in_ports = 0;

	ports = jack_get_ports(client, 0, "audio", JackPortIsPhysical);
	if (ports != NULL) {
		int i = 0;
		while (ports[i] != NULL) {
			jack_port_t * const port = jack_port_by_name(client, ports[i]);
			if (port == NULL)  {
				qWarning("JackAudioSystem: jack_port_by_name() returned an invalid port - skipping it");
				continue;
			}

			const int port_flags = jack_port_flags(port);

			if (bInputIsGood && (port_flags & wanted_out_flags) == wanted_out_flags && connected_in_ports < 1) {
				err = jack_connect(client, ports[i], jack_port_name(in_port));
				if (err != 0) {
					qWarning("JackAudioSystem: unable to connect port '%s' to '%s' - jack_connect() returned %i", ports[i], jack_port_name(in_port), err);
				} else {
					connected_in_ports++;
				}
			} else if (bOutputIsGood && (port_flags & wanted_in_flags) == wanted_in_flags && connected_out_ports < iOutPorts) {
				err = jack_connect(client, jack_port_name(out_ports[connected_out_ports]), ports[i]);
				if (err != 0) {
					qWarning("JackAudioSystem: unable to connect port '%s' to '%s' - jack_connect() returned %i", jack_port_name(out_ports[connected_out_ports]), ports[i], err);
				} else {
					connected_out_ports++;
				}
			}

			++i;
		}
	}
}

void JackAudioSystem::disconnect_ports() {
	if (!client) {
		return;
	}

	// Disconnect the input port
	if (in_port != NULL) {
		int err = jack_port_disconnect(client, in_port);
		if (err != 0)  {
			qWarning("JackAudioSystem: unable to disconnect in port - jack_port_disconnect() returned %i", err);
		}
	}

	// Disconnect the output ports
	for (unsigned int i = 0; i < iOutPorts; ++i) {
		if (out_ports[i] != NULL) {
			int err = jack_port_disconnect(client, out_ports[i]);
			if (err != 0)  {
				qWarning("JackAudioSystem: unable to disconnect out port - jack_port_disconnect() returned %i", err);
			}
		}
	}
}

void JackAudioSystem::activate() {
	QMutexLocker lock(&qmWait);
	if (client) {
		if (bActive) {
			auto_connect_ports();
			return;
		}

		int err = jack_activate(client);
		if (err != 0) {
			qWarning("JackAudioSystem: unable to activate client - jack_activate() returned %i", err);
			bJackIsGood = false;
			return;
		}
		bActive = true;

		auto_connect_ports();
	}
}

int JackAudioSystem::process_callback(jack_nframes_t nframes, void *arg) {
	JackAudioSystem * const jas = static_cast<JackAudioSystem*>(arg);

	if (jas && jas->bJackIsGood) {
		AudioInputPtr ai = g.ai;
		AudioOutputPtr ao = g.ao;
		JackAudioInput * const jai = dynamic_cast<JackAudioInput *>(ai.get());
		JackAudioOutput * const jao = dynamic_cast<JackAudioOutput *>(ao.get());

		if (jai && jai->isRunning() && jai->iMicChannels > 0 && !jai->isFinished()) {
			QMutexLocker(&jai->qmMutex);
			void *input = jack_port_get_buffer(jas->in_port, nframes);
			if (input != NULL) {
				jai->addMic(input, nframes);
			}
		}

		if (jao && jao->isRunning() && jao->iChannels > 0 && !jao->isFinished()) {
			QMutexLocker(&jao->qmMutex);

			jack_default_audio_sample_t *port_buffers[JACK_MAX_OUTPUT_PORTS];
			for (unsigned int i = 0; i < jao->iChannels; ++i) {

				port_buffers[i] = (jack_default_audio_sample_t*)jack_port_get_buffer(jas->out_ports[i], nframes);
				if (port_buffers[i] == NULL) {
					return 1;
				}
			}

			jack_default_audio_sample_t * const buffer = jas->output_buffer;
			memset(buffer, 0, sizeof(jack_default_audio_sample_t) * nframes * jao->iChannels);

			jao->mix(buffer, nframes);

			if (jao->iChannels == 1) {

				memcpy(port_buffers[0], buffer, sizeof(jack_default_audio_sample_t) * nframes);
			} else {

				// de-interleave channels
				for (unsigned int i = 0; i < nframes * jao->iChannels; ++i) {
					port_buffers[i % jao->iChannels][i / jao->iChannels] = buffer[i];
				}
			}
		}
	}

	return 0;
}

int JackAudioSystem::srate_callback(jack_nframes_t frames, void *arg) {
	JackAudioSystem * const jas = static_cast<JackAudioSystem*>(arg);
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
	output_buffer = new jack_default_audio_sample_t[frames * iOutPorts];
	if (output_buffer == NULL) {
		bJackIsGood = false;
	}

	if (jao) {
		jao->qmMutex.unlock();
	}
}

void JackAudioSystem::initializeInput() {
	QMutexLocker lock(&qmWait);

	if (!jasys->bJackIsGood) {
		return;
	}

	AudioInputPtr ai = g.ai;
	JackAudioInput * const jai = dynamic_cast<JackAudioInput *>(ai.get());

	if (jai) {
		jai->qmMutex.lock();
	}

	in_port = jack_port_register(client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	if (in_port == NULL) {
		qWarning("JackAudioSystem: unable to register 'input' port");
		return;
	}

	bInputIsGood = true;

	if (jai) {
		jai->qmMutex.unlock();
	}
}

void JackAudioSystem::destroyInput() {
	AudioInputPtr ai = g.ai;
	JackAudioInput * const jai = dynamic_cast<JackAudioInput *>(ai.get());

	if (jai) {
		jai->qmMutex.lock();
	}

	if (in_port != NULL) {
		int err = jack_port_unregister(client, in_port);
		if (err != 0)  {
			qWarning("JackAudioSystem: unable to unregister in port - jack_port_unregister() returned %i", err);
			return;
		}
	}

	bInputIsGood = false;

	if (jai) {
		jai->qmMutex.unlock();
	}
}

void JackAudioSystem::initializeOutput() {
	QMutexLocker lock(&qmWait);

	if (!jasys->bJackIsGood) {
		return;
	}

	AudioOutputPtr ao = g.ao;
	JackAudioOutput * const jao = dynamic_cast<JackAudioOutput *>(ao.get());

	allocOutputBuffer(iBufferSize);

	if (jao) {
		jao->qmMutex.lock();
	}

	for (unsigned int i = 0; i < iOutPorts; ++i) {
		char name[10];
		snprintf(name, 10, "output_%d", i + 1);

		out_ports[i] = jack_port_register(client, name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
		if (out_ports[i] == NULL) {
			qWarning("JackAudioSystem: unable to register 'output' port");
			break;
		}
	}

	bOutputIsGood = true;

	if (jao) {
		jao->qmMutex.unlock();
	}
}

void JackAudioSystem::destroyOutput() {
	AudioOutputPtr ao = g.ao;
	JackAudioOutput * const jao = dynamic_cast<JackAudioOutput *>(ao.get());

	if (jao) {
		jao->qmMutex.lock();
	}

	delete [] output_buffer;
	output_buffer = NULL;

	for (unsigned int i = 0; i < iOutPorts; ++i) {
		if (out_ports[i] != NULL) {
			int err = jack_port_unregister(client, out_ports[i]);
			if (err != 0)  {
				qWarning("JackAudioSystem: unable to unregister out port - jack_port_unregister() returned %i", err);
			}
			out_ports[i] = NULL;
		}
	}

	bOutputIsGood = false;

	if (jao) {
		jao->qmMutex.unlock();
	}
}

int JackAudioSystem::buffer_size_callback(jack_nframes_t frames, void *arg) {
	JackAudioSystem * const jas = static_cast<JackAudioSystem*>(arg);
	jas->allocOutputBuffer(frames);
	return 0;
}

void JackAudioSystem::shutdown_callback(void *arg) {
	JackAudioSystem * const jas = static_cast<JackAudioSystem*>(arg);
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
	std::sort(qlInputDevs.begin(), qlInputDevs.end());

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
	std::sort(qlOutputDevs.begin(), qlOutputDevs.end());

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
	jasys->iOutPorts = qBound<unsigned>(1, choice.toInt(), JACK_MAX_OUTPUT_PORTS);
}

JackAudioInput::JackAudioInput() {
	bRunning = true;
	iMicChannels = 0;
}

JackAudioInput::~JackAudioInput() {
	bRunning = false;
	iMicChannels = 0;
	qmMutex.lock();
	qwcWait.wakeAll();
	qmMutex.unlock();
	wait();
}

void JackAudioInput::run() {
	if (!jasys) {
		exit(1);
	}

	jasys->initializeInput();

	if (!jasys->bInputIsGood) {
		exit(1);
	}

	iMicFreq = jasys->iSampleRate;
	iMicChannels = 1;
	eMicFormat = SampleFloat;
	initializeMixer();
	jasys->activate();

	qmMutex.lock();
	while (bRunning)
		qwcWait.wait(&qmMutex);
	qmMutex.unlock();

	jasys->destroyInput();
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
	if (!jasys) {
		exit(1);
	}

	jasys->initializeOutput();

	if (!jasys->bOutputIsGood) {
		exit(1);
	}

	unsigned int chanmasks[32];

	chanmasks[0] = SPEAKER_FRONT_LEFT;
	chanmasks[1] = SPEAKER_FRONT_RIGHT;

	eSampleFormat = SampleFloat;
	iChannels = jasys->iOutPorts;
	iMixerFreq = jasys->iSampleRate;
	initializeMixer(chanmasks);
	jasys->activate();

	qmMutex.lock();
	while (bRunning)
		qwcWait.wait(&qmMutex);
	qmMutex.unlock();

	jasys->destroyOutput();
}
