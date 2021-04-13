// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VolumeSliderWidgetAction.h"

#include <cmath>

#include <QtWidgets/qtooltip.h>
#include <QtWidgets/QSlider>

VolumeSliderWidgetAction::VolumeSliderWidgetAction(QObject *parent)
	: QWidgetAction(parent) {

	qsVolumeSliderWidgetAction = new QSlider(Qt::Horizontal);

	qsVolumeSliderWidgetAction->setMinimum(-60);
	qsVolumeSliderWidgetAction->setMaximum(30);
	qsVolumeSliderWidgetAction->setAccessibleName(tr("Volume"));
	qsVolumeSliderWidgetAction->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	blockSliderSignals(true);

	connect(qsVolumeSliderWidgetAction, SIGNAL(valueChanged(int)), this, SLOT(on_VolumeSlider_valueChanged(int)));
	connect(qsVolumeSliderWidgetAction, SIGNAL(sliderReleased()), this, SLOT(on_VolumeSlider_sliderReleased()));

	setDefaultWidget(qsVolumeSliderWidgetAction);

	qsVolumeSliderWidgetAction->setProperty("mouseTracking", true);

}

void VolumeSliderWidgetAction::blockSliderSignals(bool block) {
	qsVolumeSliderWidgetAction->blockSignals(block);
}

void VolumeSliderWidgetAction::updateSliderValue(float value) {

	qsVolumeSliderWidgetAction->setValue(std::round(log2(value) * 6.0));

}

void VolumeSliderWidgetAction::displayTooltip(int value) {
	
	QString tooltipText;
	if (value > 0) {
		tooltipText = QString("+%1").arg(value);
	} else {
		tooltipText = QString("%1").arg(value);
	}

	QToolTip::showText(QCursor::pos(), tooltipText, nullptr);
}
