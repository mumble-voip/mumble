// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VolumeSliderWidgetAction.h"
#include "VolumeAdjustment.h"

#include <QSlider>
#include <QToolTip>

VolumeSliderWidgetAction::VolumeSliderWidgetAction(QObject *parent)
	: QWidgetAction(parent), m_volumeSlider(make_qt_unique< QSlider >(Qt::Horizontal)) {
	m_volumeSlider->setMinimum(-30);
	m_volumeSlider->setMaximum(30);
	m_volumeSlider->setAccessibleName(tr("Slider for volume adjustment"));
	m_volumeSlider->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	connect(m_volumeSlider.get(), &QSlider::valueChanged, this,
			&VolumeSliderWidgetAction::on_VolumeSlider_valueChanged);
	connect(m_volumeSlider.get(), &QSlider::sliderReleased, this,
			&VolumeSliderWidgetAction::on_VolumeSlider_sliderReleased);

	setDefaultWidget(m_volumeSlider.get());

	m_volumeSlider->setProperty("mouseTracking", true);
}

void VolumeSliderWidgetAction::updateSliderValue(float value) {
	int dbShift = VolumeAdjustment::toIntegerDBAdjustment(value);
	m_volumeSlider->setValue(dbShift);
	updateTooltip(dbShift);
}

void VolumeSliderWidgetAction::updateTooltip(int value) {
	// ToolTip on hover
	QString tooltipText = QString("%1: %2%3 dB");
	m_volumeSlider->setToolTip(
		tooltipText.arg(tr("Volume Adjustment")).arg(value > 0 ? QString("+") : QString("")).arg(value));
}

void VolumeSliderWidgetAction::displayTooltip(int value) {
	// ToolTip on drag
	QString tooltipText = QString("%1%2 dB");
	QToolTip::showText(QCursor::pos(), tooltipText.arg(value > 0 ? QString("+") : QString("")).arg(value), nullptr);
}
