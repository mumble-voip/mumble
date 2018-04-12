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

#ifndef _JACKAUDIO_H
#define _JACKAUDIO_H

#include "AudioInput.h"
#include "AudioOutput.h"
#include <jack/jack.h>

class JackAudioOutput;
class JackAudioInput;

class JackAudioSystem : public QObject {
        private:
                Q_OBJECT
                Q_DISABLE_COPY(JackAudioSystem)
        protected:
                jack_client_t* client;
                jack_port_t* in_port;
                jack_port_t* out_port;

                static int process_callback(jack_nframes_t nframes, void *arg);
                static int srate_callback(jack_nframes_t frames, void *arg);
                static void shutdown_callback(void *arg);
        public:
                QHash<QString, QString> qhInput;
                QHash<QString, QString> qhOutput;
                bool bJackIsGood;
                int iSampleRate;
                QMutex qmWait;
                QWaitCondition qwcWait;

                void init_jack();
                void close_jack();

                JackAudioSystem();
                ~JackAudioSystem();
};

class JackAudioInput : public AudioInput {
                friend class JackAudioSystem;
        private:
                Q_OBJECT
                Q_DISABLE_COPY(JackAudioInput)
        protected:
                QMutex qmMutex;
                QWaitCondition qwcWait;
        public:
                JackAudioInput();
                ~JackAudioInput();
                void run();
};

class JackAudioOutput : public AudioOutput {
                friend class JackAudioSystem;
        private:
                Q_OBJECT
                Q_DISABLE_COPY(JackAudioOutput)
        protected:
                QMutex qmMutex;
                QWaitCondition qwcWait;
        public:
                JackAudioOutput();
                ~JackAudioOutput();
                void run();
};

#endif
