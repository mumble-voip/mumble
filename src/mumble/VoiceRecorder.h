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

enum Format {
	WAV = 0,
	VORBIS,
	AU,
	FLAC,
	kEnd
};

QString getFormatDescription(VoiceRecorderFormat::Format fm);
QString getFormatDefaultExtension(VoiceRecorderFormat::Format fm);

};

class VoiceRecorder : public QThread {
  private:
	struct RecordBuffer {
		const ClientUser *cuUser;
		boost::shared_array<float> fBuffer;
		int iSamples;
		quint64 uiTimestamp;

		explicit RecordBuffer(const ClientUser *cu, boost::shared_array<float> buffer, int samples, quint64 timestamp);
	};

	struct RecordInfo {
		SNDFILE *sf;
		quint64 uiLastPosition;

		explicit RecordInfo();
		~RecordInfo();
	};

	QHash< int, boost::shared_ptr<RecordInfo> > qhRecordInfo;
	QList< boost::shared_ptr<RecordBuffer> > qlRecordBuffer;

	boost::scoped_ptr<RecordUser> recordUser;
	boost::scoped_ptr<Timer> tTimestamp;

	QMutex qmBufferLock;
	QMutex qmSleepLock;
	QWaitCondition qwcSleep;

	int iSampleRate;
	bool bRecording;
	QString qsFileName;
	bool bMixDown;
	VoiceRecorderFormat::Format fmFormat;
	const QDateTime qdtRecordingStart;

	QString sanitizeFilenameOrPathComponent(const QString &str) const;
	QString expandTemplateVariables(const QString &path, boost::shared_ptr<RecordBuffer> rb) const;

  public:
	explicit VoiceRecorder(QObject *p);
	~VoiceRecorder();

	void run();
	void stop();
	void addBuffer(const ClientUser *cu, boost::shared_array<float> buffer, int samples);
	void setSampleRate(int sampleRate);
	int getSampleRate() const;
	void setFileName(QString fn);
	void setMixDown(bool mixDown);
	bool getMixDown() const;
	quint64 getElapsedTime() const;
	RecordUser &getRecordUser() const;

	void setFormat(VoiceRecorderFormat::Format fm);
	VoiceRecorderFormat::Format getFormat() const;
};

typedef boost::shared_ptr<VoiceRecorder> VoiceRecorderPtr;

#endif
