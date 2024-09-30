// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VOICERECORDER_H_
#define MUMBLE_MUMBLE_VOICERECORDER_H_

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#ifndef Q_MOC_RUN
#	include <boost/scoped_ptr.hpp>
#	include <boost/shared_array.hpp>
#endif

#include <QtCore/QDateTime>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QWaitCondition>

#ifdef Q_OS_WIN
#	define ENABLE_SNDFILE_WINDOWS_PROTOTYPES 1
#endif

#include <sndfile.h>

class ClientUser;
class RecordUser;
class Timer;

/// Utilities and enums for voice recorder format handling
namespace VoiceRecorderFormat {

/// List of all formats currently supported by the recorder.
enum Format {
	/// WAVE Format
	WAV = 0,
// When switching between a non vorbis capable lib and a vorbis capable one
// this can mess up the selection stored in the config
#ifndef NO_VORBIS_RECORDING
	/// Ogg Vorbis Format
	VORBIS,
#endif
	/// AU Format
	AU,
	/// FLAC Format
	FLAC,
#ifdef USE_SNDFILE_OPUS
	// OPUS Format
	OPUS,
#endif
#ifdef USE_SNDFILE_MP3
	MP3,
#endif
	kEnd
};

/// Returns a human readable description of the format id.
QString getFormatDescription(VoiceRecorderFormat::Format fm);

/// Returns the default extension for the given format.
QString getFormatDefaultExtension(VoiceRecorderFormat::Format fm);

} // namespace VoiceRecorderFormat

/// Class for recording audio data.
///
/// Runs as a separate thread accepting audio data through the addBuffer method
/// which is then encoded using one of the formats of VoiceRecordingFormat::Format
/// and written to disk.
///
class VoiceRecorder : public QThread {
	Q_OBJECT
public:
	/// Possible error conditions inside the recorder
	enum Error { Unspecified, CreateDirectoryFailed, CreateFileFailed, InvalidSampleRate };

	/// Structure for holding configuration of VoiceRecorder object
	struct Config {
		/// The current sample rate of the recorder.
		int sampleRate;

		/// The path to store recordings.
		QString fileName;

		/// True if multi channel recording is disabled.
		bool mixDownMode;

		/// The current recording format.
		VoiceRecorderFormat::Format recordingFormat;
	};

	/// Creates a new VoiceRecorder instance.
	VoiceRecorder(QObject *p, const Config &config);
	~VoiceRecorder() Q_DECL_OVERRIDE;

	/// The main event loop of the thread, which writes all buffers to files.
	void run() Q_DECL_OVERRIDE;

	/// Stops the main loop.
	/// @param force If true buffers are discarded. Otherwise the thread will not stop before writing everything.
	void stop(bool force = false);

	/// Remembers the current time for a set of coming addBuffer calls
	void prepareBufferAdds();

	/// Adds an audio buffer which contains |samples| audio samples to the recorder.
	/// The audio data will be assumed to be recorded at the time
	/// prepareBufferAdds was last called.
	/// @param clientUser User for which to add the audio data. nullptr in mixdown mode.
	void addBuffer(const ClientUser *clientUser, boost::shared_array< float > buffer, int samples);

	/// Returns the elapsed time since the recording started.
	quint64 getElapsedTime() const;

	/// Returns a reference to the record user which is used to record local audio.
	RecordUser &getRecordUser() const;

	/// Returns true if the recorder is recording mixed down data instead of multichannel
	bool isInMixDownMode() const;
signals:
	/// Emitted if an error is encountered
	void error(int err, QString strerr);

	/// Emitted when recording is started
	void recording_started();
	/// Emitted when recording is stopped
	void recording_stopped();

private:
	/// Stores information about a recording buffer.
	struct RecordBuffer {
		/// Constructs a new RecordBuffer object.
		RecordBuffer(int recordInfoIndex_, boost::shared_array< float > buffer_, int samples_,
					 quint64 absoluteStartSample_);

		/// Hashmap index for the user
		const int recordInfoIndex;

		/// The buffer.
		boost::shared_array< float > buffer;

		/// The number of samples in the buffer.
		int samples;

		/// Absolute sample number at the start of this buffer
		quint64 absoluteStartSample;
	};

	/// Stores the recording state for one user.
	struct RecordInfo {
		RecordInfo(const QString &userName_);
		~RecordInfo();

		/// Name of the user being recorded
		const QString userName;

		/// libsndfile's handle.
		SNDFILE *soundFile;

		/// The last absolute sample we wrote for this users
		quint64 lastWrittenAbsoluteSample;
	};

	typedef QHash< int, boost::shared_ptr< RecordInfo > > RecordInfoMap;

	/// Removes invalid characters in a path component.
	QString sanitizeFilenameOrPathComponent(const QString &str) const;

	/// Expands the template variables in |path| for the given |userName|.
	QString expandTemplateVariables(const QString &path, const QString &userName) const;

	/// Returns the RecordInfo hashmap index for the given user
	int indexForUser(const ClientUser *clientUser) const;

	/// Create a sndfile SF_INFO structure describing the currently configured recording format
	SF_INFO createSoundFileInfo() const;

	/// Opens the file for the given recording information
	/// Helper function for run method. Will abort recording on failure.
	bool ensureFileIsOpenedFor(SF_INFO &soundFileInfo, boost::shared_ptr< RecordInfo > &ri);

	/// Hash which maps the |uiSession| of all users for which we have to keep a recording state to the corresponding
	/// RecordInfo object.
	RecordInfoMap m_recordInfo;

	/// List containing all unprocessed RecordBuffer objects.
	QList< boost::shared_ptr< RecordBuffer > > m_recordBuffer;

	/// The user which is used to record local audio.
	boost::scoped_ptr< RecordUser > m_recordUser;

	/// High precision timer for buffer timestamps.
	boost::scoped_ptr< Timer > m_timestamp;

	/// Protects the buffer list |qlRecordBuffer|.
	QMutex m_bufferLock;

	/// Wait condition and mutex to block until there is new data.
	QMutex m_sleepLock;
	QWaitCondition m_sleepCondition;

	/// Configuration for this instance
	const Config m_config;

	/// True if the main loop is active.
	bool m_recording;

	/// Tells the recorder to not finish writing its buffers before returning
	bool m_abort;

	/// The timestamp where the recording started.
	const QDateTime m_recordingStartTime;

	/// Absolute sample position to assume for buffer adds
	quint64 m_absoluteSampleEstimation;
};

typedef boost::shared_ptr< VoiceRecorder > VoiceRecorderPtr;

#endif
