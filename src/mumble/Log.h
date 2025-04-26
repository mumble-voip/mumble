// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_LOG_H_
#define MUMBLE_MUMBLE_LOG_H_

#include <QSystemTrayIcon>
#include <QtCore/QDate>
#include <QtCore/QMutex>
#include <QtCore/QVector>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>

#include "ConfigDialog.h"
#include "ui_Log.h"

#ifndef USE_NO_TTS
class TextToSpeech;
#endif

class LogConfig : public ConfigWidget, public Ui::LogConfig {
private:
	Q_OBJECT
	Q_DISABLE_COPY(LogConfig)

	QTreeWidgetItem *allMessagesItem;

protected:
	void updateSelectAllButtons();

public:
	/// The unique name of this ConfigWidget
	static const QString name;
	enum Column {
		ColMessage,
		ColConsole,
		ColNotification,
		ColHighlight,
		ColTTS,
		ColMessageLimit,
		ColStaticSound,
		ColStaticSoundPath
	};
	LogConfig(Settings &st);
	QString title() const Q_DECL_OVERRIDE;
	const QString &getName() const Q_DECL_OVERRIDE;
	QIcon icon() const Q_DECL_OVERRIDE;
public slots:
	void accept() const Q_DECL_OVERRIDE;
	void save() const Q_DECL_OVERRIDE;
	void load(const Settings &) Q_DECL_OVERRIDE;

	void on_qtwMessages_itemChanged(QTreeWidgetItem *, int);
	void on_qtwMessages_itemClicked(QTreeWidgetItem *, int);
	void on_qtwMessages_itemDoubleClicked(QTreeWidgetItem *, int);
	void browseForAudioFile();

	void on_qsNotificationVolume_valueChanged(int value);
	void on_qsCueVolume_valueChanged(int value);
	void on_qsTTSVolume_valueChanged(int value);
	void on_qsbNotificationVolume_valueChanged(int value);
	void on_qsbCueVolume_valueChanged(int value);
	void on_qsbTTSVolume_valueChanged(int value);
};

class ClientUser;
class Channel;
class LogMessage;

class Log : public QObject {
	friend class LogConfig;

private:
	Q_OBJECT
	Q_DISABLE_COPY(Log)
public:
	enum MsgType {
		DebugInfo,
		CriticalError,
		Warning,
		Information,
		ServerConnected,
		ServerDisconnected,
		UserJoin,
		UserLeave,
		Recording,
		YouKicked,
		UserKicked,
		SelfMute,
		OtherSelfMute,
		YouMuted,
		YouMutedOther,
		OtherMutedOther,
		ChannelJoin,
		ChannelLeave,
		PermissionDenied,
		TextMessage,
		SelfUnmute,
		SelfDeaf,
		SelfUndeaf,
		UserRenamed,
		SelfChannelJoin,
		SelfChannelJoinOther,
		ChannelJoinConnect,
		ChannelLeaveDisconnect,
		PrivateTextMessage,
		ChannelListeningAdd,
		ChannelListeningRemove,
		PluginMessage
	};

	enum LogColorType { Time, Server, Privilege, Source, Target };
	static const MsgType firstMsgType = DebugInfo;
	static const MsgType lastMsgType  = ChannelListeningRemove;

	// Display order in settingsscreen, allows to insert new events without breaking config-compatibility with older
	// versions.
	static const MsgType msgOrder[];

protected:
	/// Mutex for qvDeferredLogs
	static QMutex qmDeferredLogs;
	/// A vector containing deferred log messages
	static QVector< LogMessage > qvDeferredLogs;

	QHash< MsgType, int > qmIgnore;
	static const char *msgNames[];
	static const char *colorClasses[];
#ifndef USE_NO_TTS
	TextToSpeech *tts;
#endif
	unsigned int uiLastId;
	QDate qdDate;
	static const QStringList allowedSchemes();

public:
	Log(QObject *p = nullptr);
	QString msgName(MsgType t) const;
	void setIgnore(MsgType t, int ignore = 1 << 30);
	void clearIgnore();
	static QString validHtml(const QString &html, QTextCursor *tc = nullptr);
	static QString imageToImg(const QByteArray &format, const QByteArray &image);
	static QString imageToImg(QImage img, int maxSize = 0);
	static QString msgColor(const QString &text, LogColorType t);
	static QString formatClientUser(ClientUser *cu, LogColorType t, const QString &displayName = QString());
	static QString formatChannel(::Channel *c);
	/// Either defers the LogMessage or defers it, depending on whether Global::l is created already
	/// (if it is, it is used to directly log the msg)
	static void logOrDefer(Log::MsgType mt, const QString &console, const QString &terse = QString(),
						   bool ownMessage = false, const QString &overrideTTS = QString(), bool ignoreTTS = false);
public slots:
	// We have to explicitly use Log::MsgType and not only MsgType in order to be able to use QMetaObject::invokeMethod
	// with this function.
	void log(Log::MsgType mt, const QString &console, const QString &terse = QString(), bool ownMessage = false,
			 const QString &overrideTTS = QString(), bool ignoreTTS = false);
	/// Logs LogMessages that have been deferred so far
	void processDeferredLogs();

signals:
	/// Signal emitted when there was a message received whose type was configured to spawn a notification
	void notificationSpawned(QString title, QString body, QSystemTrayIcon::MessageIcon icon);

	/// Signal emitted when there was a message received whose type was configured to highlight the application
	void highlightSpawned();
};

class LogMessage {
public:
	Log::MsgType mt;
	QString console;
	QString terse;
	bool ownMessage;
	QString overrideTTS;
	bool ignoreTTS;

	LogMessage() = default;
	LogMessage(Log::MsgType mt, const QString &console, const QString &terse, bool ownMessage,
			   const QString &overrideTTS, bool ignoreTTS);
};

class LogDocument : public QTextDocument {
private:
	Q_OBJECT
	Q_DISABLE_COPY(LogDocument)
public:
	LogDocument(QObject *p = nullptr);
	QVariant loadResource(int, const QUrl &) Q_DECL_OVERRIDE;
};

Q_DECLARE_METATYPE(Log::MsgType)

#endif
