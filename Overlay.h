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

#ifndef _OVERLAY_H
#define _OVERLAY_H

#include "overlay/overlay.h"
#include "ConfigDialog.h"

class Player;

class OverlayConfig : public ConfigWidget {
	Q_OBJECT
	protected:
		QCheckBox *qcbEnable;

		QSlider *qsX, *qsY;
		QCheckBox *qcbLeft, *qcbRight, *qcbTop, *qcbBottom;
		QLabel *qlCurrentFont;

		QFont qfFont;
		QPushButton *qpbSetFont;
		QSlider *qsMaxWidth;
		QLabel *qlMaxWidth;

		QColor qcPlayer, qcTalking, qcChannel, qcChannelTalking;

		QLabel *qlPlayer, *qlTalking, *qlChannel, *qlChannelTalking;
		QPushButton *qpbPlayer, *qpbTalking, *qpbChannel, *qpbChannelTalking;

		static void setColorLabel(QLabel *label, QColor col);
	public:
		OverlayConfig(QWidget *p = NULL);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void on_MaxWidth_valueChanged(int v);
		void on_SetFont_clicked();
		void on_Player_clicked();
		void on_Talking_clicked();
		void on_Channel_clicked();
		void on_ChannelTalking_clicked();
		void accept();
};

class Overlay : public QObject {
	friend class OverlayConfig;
	Q_OBJECT
	protected:
		typedef void (__cdecl *HooksProc)();
		QLibrary *qlOverlay;
		QTimer *qtTimer;
		HANDLE hMutex;
		HooksProc hpInstall, hpRemove;
		SharedMem *sm;
		bool bShowAll;
	public:
		Overlay();
		~Overlay();
		bool isActive() const;
	public slots:
		void on_Timer_timeout();
		void setPlayers(QList<Player *> players);
		void setActive(bool act);
		void toggleShow();
		void forceSettings();
};

#endif
