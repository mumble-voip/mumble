// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_LOG_H_
#define MUMBLE_MUMBLE_LOG_H_

#include <QtCore/QDate>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>

#include "ConfigDialog.h"
#include "ui_Log.h"

class TextToSpeech;

class LogConfig : public ConfigWidget, public Ui::LogConfig {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LogConfig)
	public:
		enum Column { ColMessage, ColConsole, ColNotification, ColHighlight, ColTTS, ColStaticSound, ColStaticSoundPath };
		LogConfig(Settings &st);
		QString title() const Q_DECL_OVERRIDE;
		QIcon icon() const Q_DECL_OVERRIDE;
	public slots:
		void accept() const Q_DECL_OVERRIDE;
		void save() const Q_DECL_OVERRIDE;
		void load(const Settings &) Q_DECL_OVERRIDE;

		void on_qtwMessages_itemChanged(QTreeWidgetItem*, int);
		void on_qtwMessages_itemClicked(QTreeWidgetItem*, int);
		void on_qtwMessages_itemDoubleClicked(QTreeWidgetItem*, int);
		void browseForAudioFile();
};

class ClientUser;
class Channel;

class Log : public QObject {
		friend class LogConfig;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(Log)
	public:
		enum MsgType { DebugInfo, CriticalError, Warning, Information, ServerConnected, ServerDisconnected, UserJoin, UserLeave, Recording, YouKicked, UserKicked, SelfMute, OtherSelfMute, YouMuted, YouMutedOther, OtherMutedOther, ChannelJoin, ChannelLeave, PermissionDenied, TextMessage, SelfUnmute, SelfDeaf, SelfUndeaf, UserRenamed, SelfChannelJoin, SelfChannelJoinOther, ChannelJoinConnect, ChannelLeaveDisconnect, PrivateTextMessage };
		enum LogColorType { Time, Server, Privilege, Source, Target };
		static const MsgType firstMsgType = DebugInfo;
		static const MsgType lastMsgType = PrivateTextMessage;
		
		// Display order in settingsscreen, allows to insert new events without breaking config-compatibility with older versions.
		static const MsgType msgOrder[];
	
	protected:
		QHash<MsgType, int> qmIgnore;
		static const char *msgNames[];
		static const char *colorClasses[];
		TextToSpeech *tts;
		unsigned int uiLastId;
		QDate qdDate;
		static const QStringList allowedSchemes();
		void postNotification(MsgType mt, const QString &plain);
		void postQtNotification(MsgType mt, const QString &plain);
	public:
		Log(QObject *p = NULL);
		QString msgName(MsgType t) const;
		void setIgnore(MsgType t, int ignore = 1 << 30);
		void clearIgnore();
		static QString validHtml(const QString &html, QTextCursor *tc = NULL);
		static QString imageToImg(const QByteArray &format, const QByteArray &image);
		static QString imageToImg(QImage img);
		static QString msgColor(const QString &text, LogColorType t);
		static QString formatClientUser(ClientUser *cu, LogColorType t, const QString &displayName=QString());
		static QString formatChannel(::Channel *c);
	public slots:
		void log(MsgType t, const QString &console, const QString &terse=QString(), bool ownMessage = false, const QString &overrideTTS=QString());
};

class LogDocument : public QTextDocument {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LogDocument)
	public:
		LogDocument(QObject *p = NULL);
		QVariant loadResource(int, const QUrl &) Q_DECL_OVERRIDE;
	public slots:
		void finished();
};

class LogDocumentResourceAddedEvent : public QEvent {
	public:
		static const QEvent::Type Type = static_cast<QEvent::Type>(20145);

		LogDocumentResourceAddedEvent();
};

#endif
