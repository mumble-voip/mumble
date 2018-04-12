/* Copyright (C) 2011, Benjamin Jemlich <pcgod@users.sourceforge.net>
   Copyright (C) 2011, Filipe Coelho <falktx@gmail.com>

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
}

JackAudioSystem::~JackAudioSystem() {
}

void JackAudioSystem::init_jack()
{
        client = jack_client_open("mumble", JackNullOption, 0);

        if (client) {
                in_port = jack_port_register(client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
                out_port = jack_port_register(client, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
                jack_set_process_callback(client, process_callback, this);
                jack_set_sample_rate_callback(client, srate_callback, this);
                jack_on_shutdown(client, shutdown_callback, this);

                iSampleRate = jack_get_sample_rate(client);

                if (jack_activate(client) || in_port == NULL || out_port == NULL) {
                    client = NULL;
                    return;
                }

                int port_flags;
                unsigned i = -1;
                const char** ports = jack_get_ports(client, 0, 0, JackPortIsPhysical);

                if (ports) {
                    while (ports[++i])
                    {
                        jack_port_t* port = jack_port_by_name(client, ports[i]);
                        port_flags = jack_port_flags(port);

                        if (port_flags & (JackPortIsPhysical|JackPortIsOutput) && strstr(jack_port_type(port), "audio")) {
                            jack_connect(client, ports[i], jack_port_name(in_port));
                        }
                        if (port_flags & (JackPortIsPhysical|JackPortIsInput) && strstr(jack_port_type(port), "audio")) {
                            jack_connect(client, jack_port_name(out_port), ports[i]);
                        }
                    }
                }

                jack_free(ports);

                // If we made it this far, then everything is okay
                qhInput.insert(QString(), tr("Hardware Ports"));
                qhOutput.insert(QString(), tr("Hardware Ports"));
                bJackIsGood = true;

            } else {
                bJackIsGood = false;
                client = NULL;
            }
}

void JackAudioSystem::close_jack()
{
        if (client) {
                jack_deactivate(client);
                jack_client_close(client);
                client = NULL;
        }
}

int JackAudioSystem::process_callback(jack_nframes_t nframes, void *arg)
{
        JackAudioSystem *jas = (JackAudioSystem*)arg;

        if (jas && jas->bJackIsGood) {
                AudioInputPtr ai = g.ai;
                AudioOutputPtr ao = g.ao;
                JackAudioInput *jai = (JackAudioInput*)(ai.get());
                JackAudioOutput *jao = (JackAudioOutput*)(ao.get());

                if (jai && jai->bRunning && jai->iMicChannels > 0 && !jai->isFinished()) {
                        void* input = jack_port_get_buffer(jas->in_port, nframes);
                        if ((float*)input != 0)
                            jai->addMic(input, nframes);
                }

                if (jao && jao->bRunning && jao->iChannels > 0 && !jao->isFinished()) {
                        jack_default_audio_sample_t* output = (jack_default_audio_sample_t*)jack_port_get_buffer(jas->out_port, nframes);
                        memset(output, 0, sizeof(jack_default_audio_sample_t)*nframes); //TEST
                        jao->mix(output, nframes);
                }
        }

        return 0;
}

int JackAudioSystem::srate_callback(jack_nframes_t frames, void *arg)
{
        JackAudioSystem *jas = (JackAudioSystem*)arg;
        jas->iSampleRate = frames;
        return 0;
}

void JackAudioSystem::shutdown_callback(void *arg)
{
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
            iMixerFreq = jasys->iSampleRate;
            iChannels = 1;
            initializeMixer(chanmasks);
        }

        qmMutex.lock();
        while (bRunning)
                qwcWait.wait(&qmMutex);
        qmMutex.unlock();
}
