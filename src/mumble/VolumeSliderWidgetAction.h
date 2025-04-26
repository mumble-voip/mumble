// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VOLUMEWIDGETACTION_H_
#define MUMBLE_MUMBLE_VOLUMEWIDGETACTION_H_

#include <QWidgetAction>

#include "QtUtils.h"

class QSlider;
class QLabel;

class VolumeSliderWidgetAction : public QWidgetAction {
	Q_OBJECT

public:
	VolumeSliderWidgetAction(QWidget *parent = nullptr);

protected:
	qt_unique_ptr< QWidget > m_widget;
	QSlider *m_volumeSlider;
	QLabel *m_label;

	void updateLabelValue(bool checkMouseButtons = true);
	void updateSliderValue(float value);
	void displayTooltip(int value);
	void updateTooltip(int value);

protected slots:
	virtual void on_VolumeSlider_valueChanged(int){};
	virtual void on_VolumeSlider_changeCompleted(){};
};

#endif
