/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008, Andreas Messer <andi@bupfen.de>

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

#ifndef _POSITIONALSOUND_H
#define _POSITIONALSOUND_H

#include "ConfigDialog.h"
#include "Global.h"
#include "Settings.h"
#include "ui_PositionalSound.h"

class PlotWidget : public QWidget {
		Q_OBJECT
	protected:
		const QList<float> &xdata;
		const QList<float> &ydata;

		void paintEvent(QPaintEvent*);

	public:
		PlotWidget(QWidget *parent, const QList<float> &x, const QList<float> &y);
		int heightForWidth(int) const;
};

class PositionalSoundConfig : public ConfigWidget,
			public Ui::PositionalSoundConfig {
		Q_OBJECT
	protected:
		QList<float> xdata;
		QList<float> ydata;

		PlotWidget* plot;
	public:
		PositionalSoundConfig(Settings &st);
		~PositionalSoundConfig();
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void save() const;
		void load(const Settings &r);
		bool expert(bool);
		void on_qcbEnable_stateChanged(int);
		void on_qcbModel_currentIndexChanged(int);
		void on_qdsbDistance_valueChanged(double);
		void on_qdsbPreGain_valueChanged(double);
		void on_qdsbMaxAtt_valueChanged(double);

		void update();
};

class PositionalSound {
	public:
		static inline float todB(const float ratio) {
			return 20 * log10f(ratio);
		}

		static inline float toRatio(const float dB) {
			return powf(10, dB/20);
		}

		static inline float ModelConstant(const float pregain) {
			return pregain;
		}

		static inline float ModelLinear(const float pregain, const float maxatt, const float distance, const float d) {
			const float att = 10 * d/distance;


			return pregain - (att < maxatt ? att : maxatt);
		}

		static inline float calcdB(const float d) {
			switch (g.s.ePositionalSoundModel) {
				case Settings::CONSTANT:
					return ModelConstant(g.s.fPositionalSoundPreGain);
				case Settings::LINEAR:
					return ModelLinear(g.s.fPositionalSoundPreGain, g.s.fPositionalSoundMaxAtt, g.s.fPositionalSoundDistance, d);
			}

			return 0;
		}


};

#else
class PositionalSoundConfig;
#endif
