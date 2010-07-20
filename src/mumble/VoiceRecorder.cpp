/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2010, Benjamin Jemlich <pcgod@users.sourceforge.net>

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

#include "VoiceRecorder.h"

#include "Audio.h"
#include "ClientUser.h"

VoiceRecorder::VoiceRecorder(QObject *p) : QThread(p), iSampleRate(0),
	bRecording(false), bMixDown(false), uiRecordedSamples(0), recordUser(new RecordUser()) {
}

VoiceRecorder::~VoiceRecorder() {
	stop();
	wait();
	clearLists();
	delete recordUser;
}

void VoiceRecorder::clearLists() {
	while (!qhRecordBuffer.isEmpty()) {
		delete qhRecordBuffer.takeFirst();
	}

	QHash<int, RecordInfo *>::iterator it = qhRecordInfo.begin();
	while (it != qhRecordInfo.end()) {
		RecordInfo *ri = it.value();
		if (ri->sf) {
			sf_close(ri->sf);
		}
		delete ri;
		it = qhRecordInfo.erase(it);
	}
}

void VoiceRecorder::run() {
	Q_ASSERT(iSampleRate != 0);

	if (iSampleRate == 0)
		return;

	qWarning() << "VoiceRecorder: recording started to" << qsFileName << "@" << iSampleRate << "hz";

	//	SF_INFO sfinfo = {0, sampleRate, 1, SF_FORMAT_FLAC | SF_FORMAT_PCM_24, 0, 0};
	SF_INFO sfinfo = {0, iSampleRate, 1, SF_ENDIAN_CPU | SF_FORMAT_AU | SF_FORMAT_FLOAT, 0, 0};
	Q_ASSERT(sf_format_check(&sfinfo));

	bRecording = true;
	forever {
		qmSleepLock.lock();
		qwcSleep.wait(&qmSleepLock);

		if (!bRecording)  {
			qmSleepLock.unlock();
			break;
		}

		while (!qlRecordBuffer.isEmpty()) {
			RecordBuffer *rb;
			{
				QMutexLocker l(&qmBufferLock);
				rb = qlRecordBuffer.takeFirst();
			}

			int index = bMixDown ? 0 : rb->cuUser->uiSession;
			Q_ASSERT(qhRecordInfo.contains(index));

			RecordInfo *ri = qhRecordInfo.value(index);
			if (!ri->sf) {
				ri->sf = sf_open(qPrintable(qsFileName.arg(index)), SFM_WRITE, &sfinfo);
				//sf_command(ri->sf, SFC_SET_UPDATE_HEADER_AUTO, NULL, SF_TRUE);
				if (rb->cuUser)
					sf_set_string(ri->sf, SF_STR_TITLE, qPrintable(rb->cuUser->qsName));
			}

			if (ri->uiLastPosition != uiRecordedSamples) {
				// write silence until we reach our current sample value
				float *buffer = new float[1024];
				memset(buffer, 0, sizeof(float) * 1024);
				int rest = (uiRecordedSamples - ri->uiLastPosition) % 1024;
				quint64 steps = (uiRecordedSamples - ri->uiLastPosition) / 1024;
				for (quint64 i = 0; i < steps; ++i) {
					sf_write_float(ri->sf, buffer, 1024);
				}
				if (rest > 0)
					sf_write_float(ri->sf, buffer, rest);
				delete [] buffer;
			}

			sf_write_float(ri->sf, rb->fBuffer.get(), rb->iSamples);
			uiRecordedSamples += rb->iSamples;
			ri->uiLastPosition = uiRecordedSamples;
			delete rb;
		}

		qmSleepLock.unlock();
	}
	qWarning() << "VoiceRecorder: recording stopped";
}

void VoiceRecorder::stop() {
	bRecording = false;
	qwcSleep.wakeAll();
}

void VoiceRecorder::addBuffer(const ClientUser *cu, boost::shared_array<float> buffer, int samples) {
	Q_ASSERT(!bMixDown || cu == NULL);

	{
		QMutexLocker l(&qmBufferLock);
		qlRecordBuffer << new RecordBuffer(cu, buffer, samples);
	}
	int index = bMixDown ? 0 : cu->uiSession;
	if (!qhRecordInfo.contains(index)) {
		qhRecordInfo.insert(index, new RecordInfo());
	}
	qwcSleep.wakeAll();
}

void VoiceRecorder::addSilence(int samples) {
	// FIXME: locking?
	uiRecordedSamples += samples;
}

void VoiceRecorder::setSampleRate(int sampleRate) {
	Q_ASSERT(!bRecording);

	iSampleRate = sampleRate;
}

void VoiceRecorder::setFileName(QString fn) {
	Q_ASSERT(!bRecording);
	Q_ASSERT(fn.indexOf(QLatin1String("%1"))!=-1);

	qsFileName = fn;
}

void VoiceRecorder::setMixDown(bool mixDown) {
	Q_ASSERT(!bRecording);

	bMixDown = mixDown;
}

bool VoiceRecorder::getMixDown() {
	return bMixDown;
}
