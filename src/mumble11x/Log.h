/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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
		void browseForSpxFile();
};

class Log : public QObject {
		friend class LogConfig;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(Log)
	public:
		enum MsgType { DebugInfo, CriticalError, Warning, Information, ServerConnected, ServerDisconnected, PlayerJoin, PlayerLeave, YouKicked, PlayerKicked, SelfMute, OtherSelfMute, YouMuted, YouMutedOther, OtherMutedOther, ChannelJoin, ChannelLeave, PermissionDenied, TextMessage };
		static const MsgType firstMsgType = DebugInfo;
		static const MsgType lastMsgType = TextMessage;
	protected:
		QHash<MsgType, int> qmIgnore;
		static const char *msgNames[];
		TextToSpeech *tts;
		unsigned int uiLastId;
	public:
		Log(QObject *p = NULL);
		QString msgName(MsgType t) const;
		void setIgnore(MsgType t, int ignore = 1 << 30);
		void clearIgnore();
	public slots:
		void log(MsgType t, const QString &console, const QString &terse=QString());
};

class LogDocument : public QTextDocument {
		Q_OBJECT
	public:
		QVariant qvVariant;
		int iType;
		QUrl quUrl;
		LogDocument(QObject *p);
		QVariant loadResource(int, const QUrl &);
	public slots:
		void finished();
};

#else
class Log;
#endif
