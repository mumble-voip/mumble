/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>

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

#ifndef _LOG_H
#define _LOG_H

#include "mumble_pch.hpp"
#include "ConfigDialog.h"
#include "AudioOutput.h"
#include "ui_Log.h"

class TextToSpeech;

class LogConfig : public ConfigWidget, public Ui::LogConfig {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LogConfig)
	public:
		enum Column { ColMessage, ColConsole, ColNotification, ColTTS, ColStaticSound, ColStaticSoundPath };
		LogConfig(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void accept() const;
		virtual void save() const;
		virtual void load(const Settings &);
		virtual bool expert(bool);

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
		enum MsgType { DebugInfo, CriticalError, Warning, Information, ServerConnected, ServerDisconnected, UserJoin, UserLeave, Recording, YouKicked, UserKicked, SelfMute, OtherSelfMute, YouMuted, YouMutedOther, OtherMutedOther, ChannelJoin, ChannelLeave, PermissionDenied, TextMessage };
		enum LogColorType { Time, Server, Privilege, Source, Target };
		static const MsgType firstMsgType = DebugInfo;
		static const MsgType lastMsgType = TextMessage;
	protected:
		QHash<MsgType, int> qmIgnore;
		static const char *msgNames[];
		static const char *colorClasses[];
		TextToSpeech *tts;
		unsigned int uiLastId;
		QDate qdDate;
		static const QStringList allowedSchemes();
	public:
		Log(QObject *p = NULL);
		QString msgName(MsgType t) const;
		void setIgnore(MsgType t, int ignore = 1 << 30);
		void clearIgnore();
		static QString validHtml(const QString &html, bool allowReplacement = false, QTextCursor *tc = NULL);
		static QString imageToImg(const QByteArray &format, const QByteArray &image);
		static QString imageToImg(QImage img);
		static QString msgColor(const QString &text, LogColorType t);
		static QString formatClientUser(ClientUser *cu, LogColorType t);
		static QString formatChannel(::Channel *c);
	public slots:
		void log(MsgType t, const QString &console, const QString &terse=QString(), bool console_only = false);
};

class ValidDocument : public QTextDocument {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ValidDocument)
	protected:
		QStringList qslValidImage;
		bool bValid;
		QVariant loadResource(int, const QUrl &);
	public:
		ValidDocument(bool httpimages, QObject *p = NULL);
		bool isValid() const;
};

class LogDocument : public QTextDocument {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LogDocument)
	public:
		LogDocument(QObject *p = NULL);
		QVariant loadResource(int, const QUrl &);
	public slots:
		void receivedHead();
		void finished();
};

#else
class Log;
#endif
