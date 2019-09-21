// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioOutputSample.h"

#include "Audio.h"
#include "Utils.h"

#include <QtCore/QDebug>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <cmath>

SoundFile::SoundFile(const QString &fname) {
	siInfo.frames = 0;
	siInfo.channels = 1;
	siInfo.samplerate = 0;
	siInfo.sections = 0;
	siInfo.seekable = 0;
	siInfo.format = 0;

	sfFile = NULL;

	qfFile.setFileName(fname);

	if (qfFile.open(QIODevice::ReadOnly)) {
		static SF_VIRTUAL_IO svi = {&SoundFile::vio_get_filelen, &SoundFile::vio_seek, &SoundFile::vio_read, &SoundFile::vio_write, &SoundFile::vio_tell};

		sfFile = sf_open_virtual(&svi, SFM_READ, &siInfo, this);
	}
}

SoundFile::~SoundFile() {
	if (sfFile)
		sf_close(sfFile);
}

bool SoundFile::isOpen() const {
	return (sfFile != NULL) && qfFile.isOpen();
}

int SoundFile::channels() const {
	return siInfo.channels;
}

int SoundFile::samplerate() const {
	return siInfo.samplerate;
}

int SoundFile::error() const {
	return sf_error(sfFile);
}

QString SoundFile::strError() const {
	return QLatin1String(sf_strerror(sfFile));
}

sf_count_t SoundFile::seek(sf_count_t frames, int whence) {
	return sf_seek(sfFile, frames, whence);
}

sf_count_t SoundFile::read(float *ptr, sf_count_t items) {
	return sf_read_float(sfFile, ptr, items);
}

sf_count_t SoundFile::vio_get_filelen(void *user_data) {
	SoundFile *sf = reinterpret_cast<SoundFile *>(user_data);

	if (! sf->qfFile.isOpen())
		return -1;

	return (sf->qfFile.size());
}

sf_count_t SoundFile::vio_seek(sf_count_t offset, int whence, void *user_data) {
	SoundFile *sf = reinterpret_cast<SoundFile *>(user_data);

	if (! sf->qfFile.isOpen())
		return -1;

	if (whence == SEEK_SET) {
		sf->qfFile.seek(offset);
	} else if (whence == SEEK_END) {
		sf->qfFile.seek(sf->qfFile.size() - offset);
	} else {
		sf->qfFile.seek(sf->qfFile.pos() + offset);
	}
	return sf->qfFile.pos();
}

sf_count_t SoundFile::vio_read(void *ptr, sf_count_t count, void *user_data) {
	SoundFile *sf = reinterpret_cast<SoundFile *>(user_data);

	if (! sf->qfFile.isOpen())
		return -1;

	return sf->qfFile.read(reinterpret_cast<char *>(ptr), count);
}

sf_count_t SoundFile::vio_write(const void *ptr, sf_count_t count, void *user_data) {
	SoundFile *sf = reinterpret_cast<SoundFile *>(user_data);

	if (! sf->qfFile.isOpen())
		return -1;

	return sf->qfFile.write(reinterpret_cast<const char *>(ptr), count);
}

sf_count_t SoundFile::vio_tell(void *user_data) {
	SoundFile *sf = reinterpret_cast<SoundFile *>(user_data);

	if (! sf->qfFile.isOpen())
		return -1;

	return sf->qfFile.pos();
}

AudioOutputSample::AudioOutputSample(const QString &name, SoundFile *psndfile, bool loop, unsigned int freq) : AudioOutputUser(name) {
	int err;

	sfHandle = psndfile;
	iOutSampleRate = freq;

	// Check if the file is good
	if (sfHandle->channels() <= 0 || sfHandle->channels() > 2) {
		sfHandle = NULL;
		return;
	}

	/* qWarning() << "Channels: " << sfHandle->channels();
	qWarning() << "Samplerate: " << sfHandle->samplerate();
	qWarning() << "Target Sr.: " << freq;
	qWarning() << "Format: " << sfHandle->format() << endl; */

	// If the frequencies don't match initialize the resampler
	if (sfHandle->samplerate() != static_cast<int>(freq)) {
		srs = speex_resampler_init(1, sfHandle->samplerate(), iOutSampleRate, 3, &err);
		if (err != RESAMPLER_ERR_SUCCESS) {
			qWarning() << "Initialize " << sfHandle->samplerate() << " to " << iOutSampleRate << " resampler failed!";
			srs = NULL;
			sfHandle = NULL;
			return;
		}
	} else {
		srs = NULL;
	}

	iLastConsume = iBufferFilled = 0;
	bLoop = loop;
	bEof = false;
}

AudioOutputSample::~AudioOutputSample() {
	if (srs)
		speex_resampler_destroy(srs);

	delete sfHandle;
	sfHandle = NULL;
}

SoundFile* AudioOutputSample::loadSndfile(const QString &filename) {
	SoundFile *sf;

	// Create the filehandle and do a quick check if everything is ok
	sf = new SoundFile(filename);

	if (! sf->isOpen()) {
		qWarning() << "File " << filename << " failed to open";
		delete sf;
		return NULL;
	}

	if (sf->error() != SF_ERR_NO_ERROR) {
		qWarning() << "File " << filename << " couldn't be loaded: " << sf->strError();
		delete sf;
		return NULL;
	}

	if (sf->channels() <= 0 || sf->channels() > 2) {
		qWarning() << "File " << filename << " contains " << sf->channels() << " Channels, only 1 or 2 are supported.";
		delete sf;
		return NULL;
	}
	return sf;
}

QString AudioOutputSample::browseForSndfile(QString defaultpath) {
	QString file = QFileDialog::getOpenFileName(NULL, tr("Choose sound file"), defaultpath, QLatin1String("*.wav *.ogg *.ogv *.oga *.flac"));
	if (! file.isEmpty()) {
		SoundFile *sf = AudioOutputSample::loadSndfile(file);
		if (sf == NULL) {
			QMessageBox::critical(NULL,
			                      tr("Invalid sound file"),
			                      tr("The file '%1' cannot be used by Mumble. Please select a file with a compatible format and encoding.").arg(Qt::escape(file)));
			return QString();
		}
		delete sf;
	}
	return file;
}

bool AudioOutputSample::prepareSampleBuffer(unsigned int snum) {
	// Forward the buffer
	for (unsigned int i=iLastConsume;i<iBufferFilled;++i)
		pfBuffer[i-iLastConsume]=pfBuffer[i];
	iBufferFilled -= iLastConsume;
	iLastConsume = snum;

	// Check if we can satisfy request with current buffer
	if (iBufferFilled >= snum)
		return true;

	// Calculate the required buffersize to hold the results
	unsigned int iInputFrames = static_cast<unsigned int>(ceilf(static_cast<float>(snum * sfHandle->samplerate()) / static_cast<float>(iOutSampleRate)));
	unsigned int iInputSamples = iInputFrames * sfHandle->channels();

	bool mix = sfHandle->channels() > 1;
	STACKVAR(float, fOut, iInputSamples);
	STACKVAR(float, fMix, iInputFrames);

	bool eof = false;
	sf_count_t read;
	do {
		resizeBuffer(iBufferFilled + snum);

		// If we need to resample or mix write to the buffer on stack
		float *pOut = (srs || mix) ? fOut : pfBuffer + iBufferFilled;

		// Try to read all samples needed to satifsy this request
		if ((read = sfHandle->read(pOut, iInputSamples)) < iInputSamples) {
			if (sfHandle->error() != SF_ERR_NO_ERROR || !bLoop) {
				// We reached the eof or encountered an error, stuff with zeroes
				memset(pOut, 0, sizeof(float) * (iInputSamples - read));
				read = iInputSamples;
				eof = true;
			} else {
				sfHandle->seek(SEEK_SET, 0);
			}
		}

		if (mix) { // Mix the channels (only two channels)
			read /= 2;
			// If we need to resample after this write to extra buffer
			pOut = srs ? fMix : pfBuffer + iBufferFilled;
			for (unsigned int i = 0; i < read; i++)
				pOut[i] = (fOut[i*2] + fOut[i*2 + 1]) * 0.5f;

		}

		spx_uint32_t inlen = static_cast<unsigned int>(read);
		spx_uint32_t outlen = snum;
		if (srs) // If necessary resample
			speex_resampler_process_float(srs, 0, pOut, &inlen, pfBuffer + iBufferFilled, &outlen);

		iBufferFilled += outlen;
	} while (iBufferFilled < snum);

	if (eof && !bEof) {
		emit playbackFinished();
		bEof = true;
	}

	return !eof;
}
