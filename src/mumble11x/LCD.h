/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

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

#ifndef _LCD_H
#define _LCD_H

#include "ConfigDialog.h"
#include "Timer.h"
#include "ui_LCD.h"

class Player;
class LCDDevice;

class LCDConfig : public ConfigWidget, public Ui::LCDConfig {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LCDConfig)
	public:
		LCDConfig(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void on_qsMinColWidth_valueChanged(int v);
		void on_qsSplitterWidth_valueChanged(int v);
		void accept() const;
		void save() const;
		void load(const Settings &r);
		bool expert(bool);
};

class LCDEngine : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LCDEngine)
	protected:
		QList<LCDDevice *> qlDevices;
	public:
		LCDEngine();
		virtual ~LCDEngine();
		virtual QList<LCDDevice *> devices() const = 0;
};

class LCDDevice {
	public:
		enum Type { GraphicLCD, CharacterLCD };
		LCDDevice();
		virtual ~LCDDevice();
		virtual bool enabled() = 0;
		virtual void setEnabled(bool e) = 0;
		virtual void blitImage(QImage *img, bool alert = false) = 0;
		virtual QString name() const = 0;
		virtual Type type() const = 0;
		virtual QSize size() const = 0;
};

typedef LCDEngine *(*LCDEngineNew)(void);

class LCDEngineRegistrar {
	protected:
		LCDEngineNew n;
	public:
		static QList<LCDEngineNew> *qlInitializers;
		LCDEngineRegistrar(LCDEngineNew n);
		~LCDEngineRegistrar();
};

class LCD : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LCD)
	protected:
		QFont qfNormal, qfBold, qfItalic, qfItalicBold;
		QMap<unsigned int, Timer> qmSpeaking;
		QMap<unsigned int, Timer> qmNew;
		QMap<unsigned int, Timer> qmOld;
		QMap<unsigned int, QString> qmNameCache;

		int iFontHeight;
		int iFrameIndex;
		QHash<QSize, unsigned char *> qhImageBuffers;
		QHash<QSize, QImage *> qhImages;
		void initBuffers();
		void destroyBuffers();
		QImage qiLogo;
		QTimer *qtTimer;
	public slots:
		void tick();
	public:
		LCD();
		~LCD();
		void updatePlayerView();
		bool hasDevices();
};

uint qHash(const QSize &size);

#else
class LCDConfig;
class LCDDevice;
class LCDEngine;
class LCDEngineRegistrar;
class LCD;
#endif
