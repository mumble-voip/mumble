/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#include "../../overlay/overlay.h"
#include "ConfigDialog.h"

class Player;

class SharedMemoryPrivate;
class SharedMemory {
    protected:
    	SharedMemoryPrivate *d;
    public:
	SharedMem *sm;
	SharedMemory();
	~SharedMemory();
	void resolve(QLibrary *lib);
	bool tryLock();
	void unlock();
};

class OverlayConfig : public ConfigWidget {
	Q_OBJECT
	protected:
		QCheckBox *qcbEnable, *qcbAlwaysSelf;
		QComboBox *qcbShow;
		QCheckBox *qcbUserTextures;

		QSlider *qsX, *qsY;
		QCheckBox *qcbLeft, *qcbRight, *qcbTop, *qcbBottom;
		QLabel *qlCurrentFont;

		QFont qfFont;
		QPushButton *qpbSetFont;
		QSlider *qsMaxHeight;
		QLabel *qlMaxHeight;

		QColor qcPlayer, qcAltTalking, qcTalking, qcChannel, qcChannelTalking;

		QLabel *qlPlayer, *qlTalking, *qlAltTalking, *qlChannel, *qlChannelTalking;
		QPushButton *qpbPlayer, *qpbTalking, *qpbAltTalking, *qpbChannel, *qpbChannelTalking;

		static void setColorLabel(QLabel *label, QColor col);
	public:
		OverlayConfig(QWidget *p = NULL);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void on_MaxHeight_valueChanged(int v);
		void on_SetFont_clicked();
		void on_Player_clicked();
		void on_Talking_clicked();
		void on_AltTalking_clicked();
		void on_Channel_clicked();
		void on_ChannelTalking_clicked();
		void accept();
};


class OverlayPrivate;
class Overlay : public QObject {
	friend class OverlayConfig;
	Q_OBJECT
	protected:
		OverlayPrivate *d;

		enum Decoration { None, Muted, Deafened };

		struct TextLine {
		    QString qsText;
		    int iPlayer;
		    quint32 uiColor;
		    Decoration dDecor;
		    TextLine(const QString &t, quint32 c, int p = -1, Decoration d = None) : qsText(t), iPlayer(p), uiColor(c), dDecor(d) { };
		};

		typedef QPair<short, QByteArray> UserTexture;
		QByteArray qbaMuted, qbaDeafened;
		QList<TextLine> qlCurrentTexts;
		QHash<QString, unsigned char *> qhTextures;
		QHash<int, UserTexture> qhUserTextures;
		QHash<QString, short> qhWidths;
		QHash<int, QString> qhQueried;
		QSet<int> qsForce;
		QLibrary *qlOverlay;
		QTimer *qtTimer;
		float fFontBase;
		SharedMemory sm;
		void platformInit();
		void setTexts(const QList<TextLine> &lines);
		void fixFont();
		void clearCache();
	public:
		Overlay();
		~Overlay();
		bool isActive() const;
		void textureResponse(int id, const QByteArray &texture);
	public slots:
		void on_Timer_timeout();
		void updateOverlay();
		void setActive(bool act);
		void toggleShow();
		void forceSettings();
};

#endif
