// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VoiceRecorder.h"

#include "AudioOutput.h"
#include "ClientUser.h"
#include "ServerHandler.h"
#include "Global.h"

#include "../Timer.h"

#include <boost/make_shared.hpp>

#include <QRegularExpression>

VoiceRecorder::RecordBuffer::RecordBuffer(int recordInfoIndex_, boost::shared_array< float > buffer_, int samples_,
										  quint64 absoluteStartSample_)

	: recordInfoIndex(recordInfoIndex_), buffer(buffer_), samples(samples_), absoluteStartSample(absoluteStartSample_) {
	// Nothing
}

VoiceRecorder::RecordInfo::RecordInfo(const QString &userName_)
	: userName(userName_), soundFile(nullptr), lastWrittenAbsoluteSample(0) {
}

VoiceRecorder::RecordInfo::~RecordInfo() {
	if (soundFile) {
		// Close libsndfile's handle if we have one.
		sf_close(soundFile);
	}
}

VoiceRecorder::VoiceRecorder(QObject *p, const Config &config)
	: QThread(p), m_recordUser(new RecordUser()), m_timestamp(new Timer()), m_config(config), m_recording(false),
	  m_abort(false), m_recordingStartTime(QDateTime::currentDateTime()), m_absoluteSampleEstimation(0) {
	// Nothing
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
	if (res.at(res.length() - 1) == QLatin1Char('.')) {
		if (res.length() == 255) { // Prevents possible infinite recursion later on
			res[254] = QLatin1Char('_');
		} else {
			res = res.append(QLatin1Char('_'));
		}
	}

	// Replace < > : " / \ | ? * as well as chr(0) to chr(31)
	res = res.replace(QRegularExpression(QLatin1String("[<>:\"/\\\\|\\?\\*\\x00-\\x1F]")), QLatin1String("_"));

	// Prepend reserved filenames CON, PRN, AUX, NUL, COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, COM9, LPT1, LPT2,
	// LPT3, LPT4, LPT5, LPT6, LPT7, LPT8, and LPT9
	res = res.replace(QRegularExpression(QLatin1String("^((CON|PRN|AUX|NUL|COM[1-9]|LPT1[1-9])(\\.|$))"),
										 QRegularExpression::CaseInsensitiveOption),
					  QLatin1String("_\\1"));

	// Make sure we do not exceed 255 characters
	if (res.length() > 255) {
		res.truncate(255);
		// Call ourselves recursively to make sure we do not end up violating any of our rules because of this
		res = sanitizeFilenameOrPathComponent(res);
	}
#else
	// For the rest just make sure the string doesn't contain a \0 or any forward-slashes
	res           = res.replace(QRegularExpression(QLatin1String("\\x00|/")), QLatin1String("_"));
#endif
	return res;
}

QString VoiceRecorder::expandTemplateVariables(const QString &path, const QString &userName) const {
	// Split path into components
	QString res;
	QStringList comp = path.split(QLatin1Char('/'));
	Q_ASSERT(!comp.isEmpty());

	// Create a readable representation of the start date.
	QString date(m_recordingStartTime.date().toString(Qt::ISODate));
	QString time(m_recordingStartTime.time().toString(QLatin1String("hh-mm-ss")));

	QString hostname(QLatin1String("Unknown"));
	if (Global::get().sh && Global::get().uiSession != 0) {
		unsigned short port;
		QString uname, pw;
		Global::get().sh->getConnectionInfo(hostname, port, uname, pw);
	}

	// Create hash which stores the names of the variables with the corresponding values.
	// Valid variables are:
	//		%user		Inserts the users name
	//		%date		Inserts the current date
	//		%time		Inserts the current time
	//		%host		Inserts the hostname
	QHash< const QString, QString > vars;
	vars.insert(QLatin1String("user"), userName);
	vars.insert(QLatin1String("date"), date);
	vars.insert(QLatin1String("time"), time);
	vars.insert(QLatin1String("host"), hostname);

	// Reassemble and expand
	bool first = true;
	foreach (QString str, comp) {
		bool replacements = false;
		QString tmp;

		tmp.reserve(str.length() * 2);
		for (decltype(str.size()) i = 0; i < str.size(); ++i) {
			bool replaced = false;
			if (str[i] == QLatin1Char('%')) {
				QHashIterator< const QString, QString > it(vars);
				while (it.hasNext()) {
					it.next();
					if (str.mid(i + 1, it.key().length()) == it.key()) {
						i += it.key().length();
						tmp += it.value();
						replaced     = true;
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

int VoiceRecorder::indexForUser(const ClientUser *clientUser) const {
	Q_ASSERT(!m_config.mixDownMode || !clientUser);

	return (m_config.mixDownMode) ? 0 : static_cast< int >(clientUser->uiSession);
}

SF_INFO VoiceRecorder::createSoundFileInfo() const {
	Q_ASSERT(m_config.sampleRate != 0);

	// When adding new formats make sure to properly configure needed additional
	// behavior after opening the file handle (e.Global::get(). to enable clipping).

	// Convert |fmFormat| to a SF_INFO structure for libsndfile.
	SF_INFO sfinfo;
	switch (m_config.recordingFormat) {
		case VoiceRecorderFormat::WAV:
		default:
			sfinfo.frames     = 0;
			sfinfo.samplerate = m_config.sampleRate;
			sfinfo.channels   = 1;
			sfinfo.format     = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
			sfinfo.sections   = 0;
			sfinfo.seekable   = 0;
			qWarning() << "VoiceRecorder: recording started to" << m_config.fileName << "@" << m_config.sampleRate
					   << "hz in WAV format";
			break;
#ifndef NO_VORBIS_RECORDING
		case VoiceRecorderFormat::VORBIS:
			sfinfo.frames     = 0;
			sfinfo.samplerate = m_config.sampleRate;
			sfinfo.channels   = 1;
			sfinfo.format     = SF_FORMAT_OGG | SF_FORMAT_VORBIS;
			sfinfo.sections   = 0;
			sfinfo.seekable   = 0;
			qWarning() << "VoiceRecorder: recording started to" << m_config.fileName << "@" << m_config.sampleRate
					   << "hz in OGG/Vorbis format";
			break;
#endif
		case VoiceRecorderFormat::AU:
			sfinfo.frames     = 0;
			sfinfo.samplerate = m_config.sampleRate;
			sfinfo.channels   = 1;
			sfinfo.format     = SF_ENDIAN_CPU | SF_FORMAT_AU | SF_FORMAT_FLOAT;
			sfinfo.sections   = 0;
			sfinfo.seekable   = 0;
			qWarning() << "VoiceRecorder: recording started to" << m_config.fileName << "@" << m_config.sampleRate
					   << "hz in AU format";
			break;
		case VoiceRecorderFormat::FLAC:
			sfinfo.frames     = 0;
			sfinfo.samplerate = m_config.sampleRate;
			sfinfo.channels   = 1;
			sfinfo.format     = SF_FORMAT_FLAC | SF_FORMAT_PCM_24;
			sfinfo.sections   = 0;
			sfinfo.seekable   = 0;
			qWarning() << "VoiceRecorder: recording started to" << m_config.fileName << "@" << m_config.sampleRate
					   << "hz in FLAC format";
			break;
#ifdef USE_SNDFILE_OPUS
		case VoiceRecorderFormat::OPUS:
			sfinfo.frames     = 0;
			sfinfo.samplerate = m_config.sampleRate;
			sfinfo.channels   = 1;
			sfinfo.format     = SF_FORMAT_OGG | SF_FORMAT_OPUS;
			sfinfo.sections   = 0;
			sfinfo.seekable   = 0;
			qWarning() << "VoiceRecorder: recording started to" << m_config.fileName << "@" << m_config.sampleRate
					   << "hz in OPUS format";
			break;
#endif
#ifdef USE_SNDFILE_MP3
		case VoiceRecorderFormat::MP3:
			sfinfo.frames     = 0;
			sfinfo.samplerate = m_config.sampleRate;
			sfinfo.channels   = 1;
			sfinfo.format     = SF_FORMAT_MPEG | SF_FORMAT_MPEG_LAYER_III;
			sfinfo.sections   = 0;
			sfinfo.seekable   = 0;
			qWarning() << "VoiceRecorder: recording started to" << m_config.fileName << "@" << m_config.sampleRate
					   << "hz in MP3 format";
			break;
#endif
	}

	Q_ASSERT(sf_format_check(&sfinfo));
	return sfinfo;
}

bool VoiceRecorder::ensureFileIsOpenedFor(SF_INFO &soundFileInfo, boost::shared_ptr< RecordInfo > &ri) {
	if (ri->soundFile) {
		// Nothing to do
		return true;
	}

	QString filename = expandTemplateVariables(m_config.fileName, ri->userName);

	// Try to find a unique filename.
	{
		int cnt = 1;
		QString nf(filename);
		QFileInfo tfi(filename);
		while (QFile::exists(nf)) {
			nf = tfi.path() + QLatin1Char('/') + tfi.completeBaseName() + QString(QLatin1String(" (%1).")).arg(cnt)
				 + tfi.suffix();

			++cnt;
		}
		filename = nf;
	}
	qWarning() << "Recorder opens file" << filename;
	QFileInfo fi(filename);

	// Create the target path.
	if (!QDir().mkpath(fi.absolutePath())) {
		qWarning() << "Failed to create target directory: " << fi.absolutePath();
		m_recording = false;
		emit error(CreateDirectoryFailed, tr("Recorder failed to create directory '%1'").arg(fi.absolutePath()));
		emit recording_stopped();
		return false;
	}

#ifdef Q_OS_WIN
	// This is needed for unicode filenames on Windows.
	ri->soundFile = sf_wchar_open(filename.toStdWString().c_str(), SFM_WRITE, &soundFileInfo);
#else
	ri->soundFile = sf_open(qPrintable(filename), SFM_WRITE, &soundFileInfo);
#endif
	if (!ri->soundFile) {
		qWarning() << "Failed to open file for recorder: " << sf_strerror(nullptr);
		m_recording = false;
		emit error(CreateFileFailed, tr("Recorder failed to open file '%1'").arg(filename));
		emit recording_stopped();
		return false;
	}

	// Store the username in the title attribute of the file (if supported by the format).
	sf_set_string(ri->soundFile, SF_STR_TITLE, qPrintable(ri->userName));

	// Enable hard-clipping for non-float formats to prevent wrapping
	if ((soundFileInfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_FLOAT
		&& (soundFileInfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_VORBIS) {
		sf_command(ri->soundFile, SFC_SET_CLIPPING, nullptr, SF_TRUE);
	}

	return true;
}

void VoiceRecorder::run() {
	Q_ASSERT(!m_recording);

	if (Global::get().sh && Global::get().sh->m_version < Version::fromComponents(1, 2, 3))
		return;

	SF_INFO soundFileInfo = createSoundFileInfo();

	m_recording = true;
	emit recording_started();

	forever {
		// Sleep until there is new data for us to process.
		m_sleepLock.lock();
		m_sleepCondition.wait(&m_sleepLock);

		if (!m_recording || m_abort
			|| (Global::get().sh && Global::get().sh->m_version < Version::fromComponents(1, 2, 3))) {
			m_sleepLock.unlock();
			break;
		}

		while (!m_abort && !m_recordBuffer.isEmpty()) {
			boost::shared_ptr< RecordBuffer > rb;
			{
				QMutexLocker l(&m_bufferLock);
				rb = m_recordBuffer.takeFirst();
			}

			// Create the file for this RecordInfo instance if it's not yet open.

			Q_ASSERT(m_recordInfo.contains(rb->recordInfoIndex));
			boost::shared_ptr< RecordInfo > ri = m_recordInfo.value(rb->recordInfoIndex);

			if (!ensureFileIsOpenedFor(soundFileInfo, ri)) {
				return;
			}

			const qint64 missingSamples =
				static_cast< qint64 >(rb->absoluteStartSample) - static_cast< qint64 >(ri->lastWrittenAbsoluteSample);

			static const qint64 heuristicSilenceThreshold = m_config.sampleRate / 10; // 100ms
			if (missingSamples > heuristicSilenceThreshold) {
				static const qint64 maxSamplesPerIteration = m_config.sampleRate * 1; // 1s

				const bool requeue = missingSamples > maxSamplesPerIteration;

				// Write |missingSamples| samples of silence up to |maxSamplesPerIteration|
				const float buffer[1024] = {};

				const qint64 silenceToWrite = std::min(missingSamples, maxSamplesPerIteration);
				qint64 rest                 = silenceToWrite;

				for (; rest > 1024; rest -= 1024)
					sf_write_float(ri->soundFile, buffer, 1024);

				if (rest > 0)
					sf_write_float(ri->soundFile, buffer, rest);

				ri->lastWrittenAbsoluteSample += static_cast< quint64 >(silenceToWrite);

				if (requeue) {
					// Requeue the writing for this buffer to keep thread responsive
					QMutexLocker l(&m_bufferLock);
					m_recordBuffer.prepend(rb);
					continue;
				}
			}

			// Write the audio buffer and update the timestamp in |ri|.
			sf_write_float(ri->soundFile, rb->buffer.get(), rb->samples);
			ri->lastWrittenAbsoluteSample += static_cast< quint64 >(rb->samples);
		}

		m_sleepLock.unlock();
	}

	m_recording = false;
	{
		QMutexLocker l(&m_bufferLock);
		m_recordInfo.clear();
		m_recordBuffer.clear();
	}

	emit recording_stopped();
	qWarning() << "VoiceRecorder: recording stopped";
}

void VoiceRecorder::stop(bool force) {
	// Tell the main loop to terminate and wake up the sleep lock.
	m_recording = false;
	m_abort     = force;

	m_sleepCondition.wakeAll();
}

void VoiceRecorder::prepareBufferAdds() {
	// Should be ms accurat
	m_absoluteSampleEstimation = (m_timestamp->elapsed() / 1000) * (static_cast< quint64 >(m_config.sampleRate) / 1000);
}

void VoiceRecorder::addBuffer(const ClientUser *clientUser, boost::shared_array< float > buffer, int samples) {
	Q_ASSERT(!m_config.mixDownMode || !clientUser);

	if (!m_recording)
		return;

	// Create a new RecordInfo object if this is a new user.
	const int index = indexForUser(clientUser);

	if (!m_recordInfo.contains(index)) {
		boost::shared_ptr< RecordInfo > ri =
			boost::make_shared< RecordInfo >(m_config.mixDownMode ? QLatin1String("Mixdown") : clientUser->qsName);

		m_recordInfo.insert(index, ri);
	}

	{
		// Save the buffer in |qlRecordBuffer|.
		QMutexLocker l(&m_bufferLock);
		boost::shared_ptr< RecordBuffer > rb =
			boost::make_shared< RecordBuffer >(index, buffer, samples, m_absoluteSampleEstimation);

		m_recordBuffer << rb;
	}

	// Tell the main loop that we have new audio data.
	m_sleepCondition.wakeAll();
}

quint64 VoiceRecorder::getElapsedTime() const {
	return m_timestamp->elapsed();
}

RecordUser &VoiceRecorder::getRecordUser() const {
	return *m_recordUser;
}

bool VoiceRecorder::isInMixDownMode() const {
	return m_config.mixDownMode;
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
#ifdef USE_SNDFILE_OPUS
		case VoiceRecorderFormat::OPUS:
			return VoiceRecorder::tr(".opus - Lossy compressed");
#endif
#ifdef USE_SNDFILE_MP3
		case VoiceRecorderFormat::MP3:
			return VoiceRecorder::tr(".mp3 - Lossy compressed");
#endif
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
#ifdef USE_SNDFILE_OPUS
		case VoiceRecorderFormat::OPUS:
			return QLatin1String("opus");
#endif
#ifdef USE_SNDFILE_MP3
		case VoiceRecorderFormat::MP3:
			return QLatin1String("mp3");
#endif
		default:
			return QString();
	}
}
