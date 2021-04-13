// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VOLUMEWIDGETACTION_H_
#define MUMBLE_MUMBLE_VOLUMEWIDGETACTION_H_

#include <QtWidgets/QWidgetAction>

class QSlider;

class VolumeSliderWidgetAction : public QWidgetAction {
	Q_OBJECT


public:
	VolumeSliderWidgetAction(QObject *parent = nullptr);


protected:
	QSlider *qsVolumeSliderWidgetAction;

	/// Converts the value in dB before updating the slider
	void updateSliderValue(float value);
	void displayTooltip(int value);
	void blockSliderSignals(bool block);

protected slots:
	virtual void on_VolumeSlider_valueChanged(int value) = 0;
	virtual void on_VolumeSlider_sliderReleased(){};
};


#endif