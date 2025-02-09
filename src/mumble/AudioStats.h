// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOSTATS_H_
#	define MUMBLE_MUMBLE_AUDIOSTATS_H_

#	include <QtCore/QList>
#	include <QtCore/QTimer>
#	include <QtCore/QtGlobal>
#	include <QtWidgets/QWidget>

class AudioBar : public QWidget {
private:
	Q_OBJECT
	Q_DISABLE_COPY(AudioBar)
protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

public:
	AudioBar(QWidget *parent = nullptr);
	int iMin, iMax;
	int iBelow, iAbove;
	int iValue, iPeak;
	QColor qcBelow, qcInside, qcAbove;

	QList< QColor > qlReplacableColors;
	QList< Qt::BrushStyle > qlReplacementBrushes;
};

class AudioEchoWidget : public QWidget {
private:
	Q_OBJECT
	Q_DISABLE_COPY(AudioEchoWidget)
public:
	AudioEchoWidget(QWidget *parent);
protected slots:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

class AudioNoiseWidget : public QWidget {
private:
	Q_OBJECT
	Q_DISABLE_COPY(AudioNoiseWidget)
public:
	AudioNoiseWidget(QWidget *parent);
protected slots:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#	include "ui_AudioStats.h"

class AudioStats : public QDialog, public Ui::AudioStats {
private:
	Q_OBJECT
	Q_DISABLE_COPY(AudioStats)
protected:
	QTimer *qtTick;
	bool bTalking;

public:
	AudioStats(QWidget *parent);
	~AudioStats() Q_DECL_OVERRIDE;
public slots:
	void on_Tick_timeout();
};

#else
class AudioStats;
#endif
