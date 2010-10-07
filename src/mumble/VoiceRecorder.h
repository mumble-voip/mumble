/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2010, Benjamin Jemlich <pcgod@users.sourceforge.net>
   Copyright (C) 2010, Stefan Hacker <dd0t@users.sourceforge.net>

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

#ifndef _VOICERECORDER_H
#define _VOICERECORDER_H

#pragma once

#include "mumble_pch.hpp"

class ClientUser;
class RecordUser;
struct Timer;

namespace VoiceRecorderFormat {

// List of all formats currently supported by the recorder.
	enum Format {
		WAV = 0,	// WAVE Format
#ifndef NO_VORBIS_RECORDING		// When switching between a non vorbis capable lib and a vorbis capable one this can mess up the selection stored in the config
		VORBIS,		// Ogg Vorbis Format
#endif
		AU,			// AU Format
		FLAC,		// FLAC Format
		kEnd
	};

// Returns a human readable description of the format id.
	QString getFormatDescription(VoiceRecorderFormat::Format fm);

// Returns the default extension for the given format.
	QString getFormatDefaultExtension(VoiceRecorderFormat::Format fm);

};

class VoiceRecorder : public QThread {
		Q_OBJECT
	private:
		// Stores information about a recording buffer.
		struct RecordBuffer {
			// Constructs a new RecordBuffer object.
			explicit RecordBuffer(const ClientUser *cu, boost::shared_array<float> buffer, int samples, quint64 timestamp);

			// The user to which this buffer belongs.
			const ClientUser *cuUser;

			// The buffer.
			boost::shared_array<float> fBuffer;

			// The number of samples in the buffer.
			int iSamples;

			// Timestamp for the buffer.
			quint64 uiTimestamp;
		};

		// Keep the recording state for one user.
		struct RecordInfo {
			explicit RecordInfo();
			~RecordInfo();

			// libsndfile's handle.
			SNDFILE *sf;

			// The timestamp where we last wrote audio data for this user.
			quint64 uiLastPosition;
		};

		// Hash which maps the |uiSession| of all users for which we have to keep a recording state to the corresponding RecordInfo object.
		QHash< int, boost::shared_ptr<RecordInfo> > qhRecordInfo;

		// List containing all unprocessed RecordBuffer objects.
		QList< boost::shared_ptr<RecordBuffer> > qlRecordBuffer;

		// The user which is used to record local audio.
		boost::scoped_ptr<RecordUser> recordUser;

		// High precision timer for buffer timestamps.
		boost::scoped_ptr<Timer> tTimestamp;

		// Protects the buffer list |qlRecordBuffer|.
		QMutex qmBufferLock;

		// Wait condition and mutex to block until there is new data.
		QMutex qmSleepLock;
		QWaitCondition qwcSleep;

		// The current sample rate of the recorder.
		int iSampleRate;

		// True if the main loop is active.
		bool bRecording;

		// The path to store recordings.
		QString qsFileName;

		// True if multi channel recording is disabled.
		bool bMixDown;

		// The current recording format.
		VoiceRecorderFormat::Format fmFormat;

		// The timestamp where the recording started.
		const QDateTime qdtRecordingStart;


		// Removes invalid characters in a path component.
		QString sanitizeFilenameOrPathComponent(const QString &str) const;

		// Expands the template variables in |path| using the information contained in |rb|.
		QString expandTemplateVariables(const QString &path, boost::shared_ptr<RecordBuffer> rb) const;

	public:
		// Error enum
		enum Error { Unspecified, CreateDirectoryFailed, CreateFileFailed, InvalidSampleRate };

		// Creates a new VoiceRecorder instance.
		explicit VoiceRecorder(QObject *p);
		~VoiceRecorder();

		// The main event loop of the thread, which writes all buffers to files.
		void run();

		// Stops the main loop.
		void stop();

		// Adds an audio buffer which contains |samples| audio samples to the recorder.
		void addBuffer(const ClientUser *cu, boost::shared_array<float> buffer, int samples);

		// Sets the sample rate of the recorder. The sample rate can't change while the recoder is active.
		void setSampleRate(int sampleRate);

		// Returns the current sample rate of the encoder.
		int getSampleRate() const;

		// Sets the path and filename for recordings.
		void setFileName(QString fn);

		// Sets the state of multi channel recording. This can't change while the recorder is active.
		void setMixDown(bool mixDown);

		// Returns the multi channel recording state of the recorder.
		bool getMixDown() const;

		// Returns the elapsed time since the recording started.
		quint64 getElapsedTime() const;

		// Returns a refence to the record user which is used to record local audio.
		RecordUser &getRecordUser() const;

		// Sets the storage format for recordings. Can't change while the recorder is active.
		void setFormat(VoiceRecorderFormat::Format fm);

		// Returns the current recording format.
		VoiceRecorderFormat::Format getFormat() const;
	signals:
		void error(int err, QString strerr);
		void recording_started();
		void recording_stopped();
};

typedef boost::shared_ptr<VoiceRecorder> VoiceRecorderPtr;

#endif
