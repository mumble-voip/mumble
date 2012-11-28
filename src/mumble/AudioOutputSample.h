/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net>

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

#ifndef AUDIOOUTPUTSAMPLE_H_
#define AUDIOOUTPUTSAMPLE_H_

#include <sndfile.h>
#include <speex/speex_resampler.h>
#include <QtCore/QObject>
#include <QtCore/QFile>

#include "AudioOutputUser.h"

class SoundFile : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(SoundFile)
	protected:
		SNDFILE *sfFile;
		SF_INFO siInfo;
		QFile qfFile;
		static sf_count_t vio_get_filelen(void *user_data);
		static sf_count_t vio_seek(sf_count_t offset, int whence, void *user_data);
		static sf_count_t vio_read(void *ptr, sf_count_t count, void *user_data);
		static sf_count_t vio_write(const void *ptr, sf_count_t count, void *user_data);
		static sf_count_t vio_tell(void *user_data);
	public:
		SoundFile(const QString &fname);
		virtual ~SoundFile();

		int channels() const;
		int samplerate() const;
		int error() const ;
		QString strError() const;
		bool isOpen() const;

		sf_count_t seek(sf_count_t frames, int whence);
		sf_count_t read(float *ptr, sf_count_t items);
};

class AudioOutputSample : public AudioOutputUser {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutputSample)
	protected:
		unsigned int iLastConsume;
		unsigned int iBufferFilled;
		unsigned int iOutSampleRate;
		SpeexResamplerState *srs;

		SoundFile *sfHandle;

		bool bLoop;
		bool bEof;
	signals:
		void playbackFinished();
	public:
		static SoundFile* loadSndfile(const QString &filename);
		static QString browseForSndfile();
		virtual bool needSamples(unsigned int snum);
		AudioOutputSample(const QString &name, SoundFile *psndfile, bool repeat, unsigned int freq);
		~AudioOutputSample();
};

#endif  // AUDIOOUTPUTSAMPLE_H_
