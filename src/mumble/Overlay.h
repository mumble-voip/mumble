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

#ifndef _OVERLAY_H
#define _OVERLAY_H

#include "../../overlay/overlay.h"
#include "ConfigDialog.h"
#include "ClientUser.h"
#include "SharedMemory.h"
#include "ui_Overlay.h"

class User;
class Overlay;

class SharedMemoryPrivate;
class SharedMemory {
	private:
		Q_DISABLE_COPY(SharedMemory)
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

class OverlayConfig : public ConfigWidget, public Ui::OverlayConfig {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayConfig)
	protected:
		QFont qfFont;
		QColor qcUser, qcWhisper, qcTalking, qcChannel, qcChannelTalking;

		static void setColorLabel(QLabel *label, QColor col);
	public:
		OverlayConfig(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void on_qsMaxHeight_valueChanged(int v);
		void on_qpbSetFont_clicked();
		void on_qpbUser_clicked();
		void on_qpbTalking_clicked();
		void on_qpbWhisper_clicked();
		void on_qpbChannel_clicked();
		void on_qpbChannelTalking_clicked();
		void accept() const;
		void save() const;
		void load(const Settings &r);
		bool expert(bool);
};

struct OverlayTextLine {
	enum Decoration { None, Muted, Deafened };
	QString qsText;
	unsigned int uiSession;
	QRgb uiColor;
	Decoration dDecor;
	int iPriority;
	QRect qrRect;
	OverlayTextLine() : uiSession(0), uiColor(0), dDecor(None), iPriority(0) { };
	OverlayTextLine(const QString &t, quint32 c, int priority = 0, unsigned int session = 0, Decoration d = None) : qsText(t), uiSession(session), uiColor(c), dDecor(d), iPriority(priority) { };
	bool operator <(const OverlayTextLine &o) const;
};

class OverlayClient : public QObject {
		friend class Overlay;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayClient);
	protected:
		typedef QPair<QByteArray, QRgb> TextImageKey;
		struct TextImage {
			QImage qiImage;
			float iOffset;
			TextImage(const QImage &img, int i) : qiImage(img), iOffset(i) {};
		};

		OverlayMsg omMsg;
		QLocalSocket *qlsSocket;
		SharedMemory2 *smMem;
		unsigned int uiWidth, uiHeight;
		float fItemHeight, fEdge, fBase;
		int iItemHeight;
		QList<OverlayTextLine> qlLines;
		QRect qrLast;
		QFont fFont;
		Timer t;
		QImage qiMuted, qiDeafened;
		QCache<TextImageKey, TextImage> qcTexts;

		void setupRender();
	protected slots:
		void readyRead();
	public:
		OverlayClient(QLocalSocket *, QObject *);
		bool setTexts(const QList<OverlayTextLine> &lines);
		void reset();
};

class OverlayPrivate : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayPrivate);
	public:
		OverlayPrivate() : QObject() {};
};

class Overlay : public QObject {
		friend class OverlayConfig;
		friend class OverlayClient;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(Overlay)
	protected:
		OverlayPrivate *d;

		QByteArray qbaMuted, qbaDeafened;
		QList<OverlayTextLine> qlCurrentTexts;
		QHash<QString, unsigned char *> qhTextures;
		QHash<QString, short> qhWidths;
		QSet<unsigned int> qsForce;
		QSet<unsigned int> qsQueried;
		QLibrary *qlOverlay;
		QTimer *qtTimer;
		QFont qfFont;
		float fFontBase;
		SharedMemory sm;
		QSvgRenderer qsrMuted, qsrDeafened;
		void platformInit();
		void setTexts(const QList<OverlayTextLine> &lines);
		void fixFont();
		void clearCache();

		QLocalServer *qlsServer;
		QList<OverlayClient *> qlClients;
	protected slots:
		void disconnected();
		void error(QLocalSocket::LocalSocketError);
		void newConnection();
	public:
		Overlay();
		~Overlay();
		bool isActive() const;
		void verifyTexture(ClientUser *cp);
	public slots:
		void on_Timer_timeout();
		void updateOverlay();
		void setActive(bool act);
		void toggleShow();
		void forceSettings();
};

#endif
