/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2010-2011, Benjamin Jemlich <pcgod@users.sourceforge.net>
   Copyright (C) 2010-2011, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "mumble_pch.hpp"

#include "VoiceRecorder.h"

#include "AudioOutput.h"
#include "ClientUser.h"
#include "Global.h"
#include "ServerHandler.h"

#include "../Timer.h"

VoiceRecorder::RecordBuffer::RecordBuffer(const ClientUser *cu,
        boost::shared_array<float> buffer, int samples, quint64 timestamp) :
		cuUser(cu), fBuffer(buffer), iSamples(samples), uiTimestamp(timestamp) {
}

VoiceRecorder::RecordInfo::RecordInfo() : sf(NULL), uiLastPosition(0) {
}

VoiceRecorder::RecordInfo::~RecordInfo() {
	if (sf) {
		// Close libsndfile's handle if we have one.
		sf_close(sf);
	}
}

VoiceRecorder::VoiceRecorder(QObject *p) : QThread(p), recordUser(new RecordUser()),
		tTimestamp(new Timer()), iSampleRate(0), bRecording(false), bMixDown(false),
		fmFormat(VoiceRecorderFormat::WAV), qdtRecordingStart(QDateTime::currentDateTime()) {
}

VoiceRecorder::~VoiceRecorder() {
	stop();
	wait();
}

QString VoiceRecorder::sanitizeFilenameOrPathComponent(const QString &str) const {
	// Trim leading/trailing whitespaces
	QString res = str.trimmed();
	if (res.isEmpty())
		return QLatin1String("_");

#ifdef Q_OS_WIN
	// Rules according to http://en.wikipedia.org/wiki/Filename#Comparison_of_file_name_limitations
	// and http://msdn.microsoft.com/en-us/library/aa365247(VS.85).aspx

	// Make sure name doesn't end in "."
	if (res.at(res.length() - 1) == QLatin1Char('.'))
		if (res.length() == 255) // Prevents possible infinite recursion later on
			res[254] = QLatin1Char('_');
		else
			res = res.append(QLatin1Char('_'));

	// Replace < > : " / \ | ? * as well as chr(0) to chr(31)
	res = res.replace(QRegExp(QLatin1String("[<>:\"/\\\\|\\?\\*\\x00-\\x1F]")), QLatin1String("_"));

	// Prepend reserved filenames CON, PRN, AUX, NUL, COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, COM9, LPT1, LPT2, LPT3, LPT4, LPT5, LPT6, LPT7, LPT8, and LPT9
	res = res.replace(QRegExp(QLatin1String("^((CON|PRN|AUX|NUL|COM[1-9]|LPT1[1-9])(\\.|$))"), Qt::CaseInsensitive), QLatin1String("_\\1"));

	// Make sure we do not exceed 255 characters
	if (res.length() > 255) {
		res.truncate(255);
		// Call ourselves recursively to make sure we do not end up violating any of our rules because of this
		res = sanitizeFilenameOrPathComponent(res);
	}
#else
	// For the rest just make sure the string doesn't contain a \0 or any forward-slashes
	res = res.replace(QRegExp(QLatin1String("\\x00|/")), QLatin1String("_"));
#endif
	return res;
}

QString VoiceRecorder::expandTemplateVariables(const QString &path, boost::shared_ptr<RecordBuffer> rb) const {
	// Split path into components
	QString res;
	QStringList comp = path.split(QLatin1Char('/'));
	Q_ASSERT(!comp.isEmpty());

	QString username(QLatin1String("Mixdown"));
	// In mixdown mode |cuUser| is always NULL.
	if (rb->cuUser)
		username = rb->cuUser->qsName;

	// Create a readable representation of the start date.
	QString date(qdtRecordingStart.date().toString(Qt::ISODate));
	QString time(qdtRecordingStart.time().toString(QLatin1String("hh-mm-ss")));

	QString hostname(QLatin1String("Unknown"));
	if (g.sh && g.uiSession != 0) {
		unsigned short port;
		QString uname, pw;
		g.sh->getConnectionInfo(hostname, port, uname, pw);
	}

	// Create hash which stores the names of the variables with the corresponding values.
	// Valid variables are:
	//		%user		Inserts the users name
	//		%date		Inserts the current date
	//		%time		Inserts the current time
	//		%host		Inserts the hostname
	QHash<const QString, QString> vars;
	vars.insert(QLatin1String("user"), username);
	vars.insert(QLatin1String("date"), date);
	vars.insert(QLatin1String("time"), time);
	vars.insert(QLatin1String("host"), hostname);

	// Reassemble and expand
	bool first = true;
	foreach(QString str, comp) {
		bool replacements = false;
		QString tmp;

		tmp.reserve(str.length() * 2);
		for (int i = 0; i < str.size(); ++i) {
			bool replaced = false;
			if (str[i] == QLatin1Char('%')) {
				QHashIterator<const QString, QString> it(vars);
				while (it.hasNext()) {
					it.next();
					if (str.midRef(i + 1, it.key().length()) == it.key()) {
						i += it.key().length();
						tmp += it.value();
						replaced = true;
						replacements = true;
						break;
					}
				}
			}

			if (!replaced)
				tmp += str[i];
		}

		str = tmp;

		if (replacements)
			str = sanitizeFilenameOrPathComponent(str);

		if (first) {
			first = false;
			res.append(str);
		} else {
			res.append(QLatin1Char('/') + str);
		}
	}
	return res;
}

void VoiceRecorder::run() {
	Q_ASSERT(!bRecording);
	Q_ASSERT(iSampleRate != 0);

	if (iSampleRate == 0) {
		emit error(InvalidSampleRate, tr("Invalid sample rate given to recorder"));
		return;
	}

	// Convert |fmFormat| to a SF_INFO structure for libsndfile.
	SF_INFO sfinfo;
	switch (fmFormat) {
		case VoiceRecorderFormat::WAV:
		default:
			sfinfo.frames = 0;
			sfinfo.samplerate = iSampleRate;
			sfinfo.channels = 1;
			sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
			sfinfo.sections = 0;
			sfinfo.seekable = 0;
			qWarning() << "VoiceRecorder: recording started to" << qsFileName << "@" << iSampleRate << "hz in WAV format";
			break;
#ifndef NO_VORBIS_RECORDING
		case VoiceRecorderFormat::VORBIS:
			sfinfo.frames = 0;
			sfinfo.samplerate = iSampleRate;
			sfinfo.channels = 1;
			sfinfo.format = SF_FORMAT_OGG | SF_FORMAT_VORBIS;
			sfinfo.sections = 0;
			sfinfo.seekable = 0;
			qWarning() << "VoiceRecorder: recording started to" << qsFileName << "@" << iSampleRate << "hz in OGG/Vorbis format";
			break;
#endif
		case VoiceRecorderFormat::AU:
			sfinfo.frames = 0;
			sfinfo.samplerate = iSampleRate;
			sfinfo.channels = 1;
			sfinfo.format = SF_ENDIAN_CPU | SF_FORMAT_AU | SF_FORMAT_FLOAT;
			sfinfo.sections = 0;
			sfinfo.seekable = 0;
			qWarning() << "VoiceRecorder: recording started to" << qsFileName << "@" << iSampleRate << "hz in AU format";
			break;
		case VoiceRecorderFormat::FLAC:
			sfinfo.frames = 0;
			sfinfo.samplerate = iSampleRate;
			sfinfo.channels = 1;
			sfinfo.format = SF_FORMAT_FLAC | SF_FORMAT_PCM_24;
			sfinfo.sections = 0;
			sfinfo.seekable = 0;
			qWarning() << "VoiceRecorder: recording started to" << qsFileName << "@" << iSampleRate << "hz in FLAC format";
			break;
	}

	Q_ASSERT(sf_format_check(&sfinfo));
	if (g.sh && g.sh->uiVersion < 0201003)
		return;

	bRecording = true;
	emit recording_started();
	forever {
		// Sleep until there is new data for us to process.
		qmSleepLock.lock();
		qwcSleep.wait(&qmSleepLock);

		if (!bRecording || (g.sh && g.sh->uiVersion < 0201003)) {
			qmSleepLock.unlock();
			break;
		}

		while (!qlRecordBuffer.isEmpty()) {
			boost::shared_ptr<RecordBuffer> rb;
			{
				QMutexLocker l(&qmBufferLock);
				rb = qlRecordBuffer.takeFirst();
			}

			// Use 0 as the |index| if multi channel recording is disabled.
			int index = bMixDown ? 0 : rb->cuUser->uiSession;
			Q_ASSERT(qhRecordInfo.contains(index));

			// Create the file for this RecordInfo instance if it's not yet open.
			boost::shared_ptr<RecordInfo> ri = qhRecordInfo.value(index);
			if (!ri->sf) {
				QString filename = expandTemplateVariables(qsFileName, rb);

				// Try to find a unique filename.
				{
					int cnt = 1;
					QString nf(filename);
					QFileInfo tfi(filename);
					while (QFile::exists(nf)) {
						nf = tfi.path() + QLatin1Char('/') + tfi.completeBaseName() + QString(QLatin1String(" (%1).")).arg(cnt) +  tfi.suffix();
						++cnt;
					}
					filename = nf;
				}
				qWarning() << "Recorder opens file" << filename;
				QFileInfo fi(filename);

				// Create the target path.
				if (!QDir().mkpath(fi.absolutePath())) {
					qWarning() << "Failed to create target directory: " << fi.absolutePath();
					bRecording = false;
					emit error(CreateDirectoryFailed, tr("Recorder failed to create directory '%1'").arg(fi.absolutePath()));
					emit recording_stopped();
					return;
				}

#ifdef Q_OS_WIN
				// This is needed for unicode filenames on Windows.
				ri->sf = sf_wchar_open(filename.toStdWString().c_str(), SFM_WRITE, &sfinfo);
#else
				ri->sf = sf_open(qPrintable(filename), SFM_WRITE, &sfinfo);
#endif
				if (ri->sf == NULL) {
					qWarning() << "Failed to open file for recorder: "<< sf_strerror(NULL);
					bRecording = false;
					emit error(CreateFileFailed, tr("Recorder failed to open file '%1'").arg(filename));
					emit recording_stopped();
					return;
				}

				// Store the username in the title attribute of the file (if supported by the format).
				if (rb->cuUser)
					sf_set_string(ri->sf, SF_STR_TITLE, qPrintable(rb->cuUser->qsName));
			}

			// Calculate the difference between the time of the current buffer and the time where we last wrote audio data for that user.
			// Writes silence if the number of |missingSamples| is larger than a threshold of 100ms (to account for processing delay).
			qint64 missingSamples = ((rb->uiTimestamp - ri->uiLastPosition) * iSampleRate) / 1000000 - rb->iSamples;
			if (missingSamples > iSampleRate / 10) {
				// Write |missingSamples| samples of silence.
				boost::scoped_array<float> buffer(new float[1024]);
				memset(buffer.get(), 0, sizeof(float) * 1024);
				qint64 rest = missingSamples;
				for (; rest > 1024; rest -= 1024)
					sf_write_float(ri->sf, buffer.get(), 1024);

				if (rest > 0)
					sf_write_float(ri->sf, buffer.get(), rest);
			}

			// Write the audio buffer and update the timestamp in |ri|.
			sf_write_float(ri->sf, rb->fBuffer.get(), rb->iSamples);
			ri->uiLastPosition = rb->uiTimestamp;
		}

		qmSleepLock.unlock();
	}
	bRecording = false;
	emit recording_stopped();
	qWarning() << "VoiceRecorder: recording stopped";
}

void VoiceRecorder::stop() {
	// Tell the main loop to terminate and wake up the sleep lock.
	bRecording = false;
	qwcSleep.wakeAll();
}

void VoiceRecorder::addBuffer(const ClientUser *cu, boost::shared_array<float> buffer, int samples) {
	Q_ASSERT(!bMixDown || cu == NULL);

	// Create a new RecordInfo object if this is a new user.
	int index = bMixDown ? 0 : cu->uiSession;
	if (!qhRecordInfo.contains(index)) {
		boost::shared_ptr<RecordInfo> ri = boost::make_shared<RecordInfo>();
		qhRecordInfo.insert(index, ri);
	}

	{
		// Save the buffer in |qlRecordBuffer|.
		QMutexLocker l(&qmBufferLock);
		boost::shared_ptr<RecordBuffer> rb = boost::make_shared<RecordBuffer>(cu, buffer, samples, tTimestamp->elapsed());
		qlRecordBuffer << rb;
	}

	// Tell the main loop that we have new audio data.
	qwcSleep.wakeAll();
}

void VoiceRecorder::setSampleRate(int sampleRate) {
	Q_ASSERT(!bRecording);

	iSampleRate = sampleRate;
}

int VoiceRecorder::getSampleRate() const {
	return iSampleRate;
}

void VoiceRecorder::setFileName(QString fn) {
	Q_ASSERT(!bRecording);
	qsFileName = QDir::fromNativeSeparators(fn);
}

void VoiceRecorder::setMixDown(bool mixDown) {
	Q_ASSERT(!bRecording);

	bMixDown = mixDown;
}

bool VoiceRecorder::getMixDown() const {
	return bMixDown;
}

quint64 VoiceRecorder::getElapsedTime() const {
	return tTimestamp->elapsed();
}

RecordUser &VoiceRecorder::getRecordUser() const {
	return *recordUser;
}

void VoiceRecorder::setFormat(VoiceRecorderFormat::Format fm) {
	Q_ASSERT(!bRecording);
	fmFormat = fm;
}

VoiceRecorderFormat::Format VoiceRecorder::getFormat() const {
	return fmFormat;
}

QString VoiceRecorderFormat::getFormatDescription(VoiceRecorderFormat::Format fm) {
	switch (fm) {
		case VoiceRecorderFormat::WAV:
			return VoiceRecorder::tr(".wav - Uncompressed");
#ifndef NO_VORBIS_RECORDING
		case VoiceRecorderFormat::VORBIS:
			return VoiceRecorder::tr(".ogg (Vorbis) - Compressed");
#endif
		case VoiceRecorderFormat::AU:
			return VoiceRecorder::tr(".au - Uncompressed");
		case VoiceRecorderFormat::FLAC:
			return VoiceRecorder::tr(".flac - Lossless compressed");
		default:
			return QString();
	}
}

QString VoiceRecorderFormat::getFormatDefaultExtension(VoiceRecorderFormat::Format fm) {
	switch (fm) {
		case VoiceRecorderFormat::WAV:
			return QLatin1String("wav");
#ifndef NO_VORBIS_RECORDING
		case VoiceRecorderFormat::VORBIS:
			return QLatin1String("ogg");
#endif
		case VoiceRecorderFormat::AU:
			return QLatin1String("au");
		case VoiceRecorderFormat::FLAC:
			return QLatin1String("flac");
		default:
			return QString();
	}
}
