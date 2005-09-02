/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QAction>
#include <QMap>
#include "Player.h"
#include "Connection.h"
#include "ServerHandler.h"
#include "About.h"

class MainWindow : public QMainWindow {
	Q_OBJECT
	public:
		QListWidget *m_qlwPlayers;
		QAction *m_qaServerConnect, *m_qaServerDisconnect, *m_qaServerStats;
		QAction *m_qaPlayerKick, *m_qaPlayerMute, *m_qaPlayerDeaf;
		QAction *m_qaAudioConfig, *m_qaAudioReset;
		QAction *m_qaHelpAbout, *m_qaHelpAboutQt;

		QMap<short, QListWidgetItem *> m_qmPlayers;
		QMap<QListWidgetItem *, Player *> m_qmPlayerWidgets;

		void setupGui();
		void customEvent(QEvent *evt);

	public slots:
		void on_ServerConnect_triggered();
		void on_ServerDisconnect_triggered();
		void on_HelpAbout_triggered();
		void on_HelpAboutQt_triggered();
		void serverConnected();
		void serverDisconnected();
	public:
		MainWindow(QWidget *parent);
};

extern MainWindow *g_mwMainWindow;

#else
class MainWindow;
#endif
