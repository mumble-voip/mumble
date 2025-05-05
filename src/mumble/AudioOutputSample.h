// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOOUTPUTSAMPLE_H_
#define MUMBLE_MUMBLE_AUDIOOUTPUTSAMPLE_H_

#include <QtCore/QFile>
#include <QtCore/QObject>
#include <sndfile.h>
#include <speex/speex_resampler.h>

#include "AudioOutputBuffer.h"

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
	~SoundFile();

	int channels() const;
	int samplerate() const;
	int error() const;
	QString strError() const;
	bool isOpen() const;

	sf_count_t seek(sf_count_t frames, int whence);
	sf_count_t read(float *ptr, sf_count_t items);
};

class AudioOutputSample : public AudioOutputBuffer {
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

	float m_volume;
signals:
	void playbackFinished();

public:
	static SoundFile *loadSndfile(const QString &filename);
	static QString browseForSndfile(QString defaultpath = QString());
	virtual bool prepareSampleBuffer(unsigned int frameCount) Q_DECL_OVERRIDE;
	float getVolume() const;
	AudioOutputSample(SoundFile *psndfile, float volume, bool repeat, unsigned int freq, unsigned int bufferSize);
	~AudioOutputSample() Q_DECL_OVERRIDE;
};

#endif // AUDIOOUTPUTSAMPLE_H_
