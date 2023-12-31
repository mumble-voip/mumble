// Copyright 2022-2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VolumeSliderWidgetAction.h"
#include "VolumeAdjustment.h"
#include "widgets/EventFilters.h"

#include <QSlider>
#include <QToolTip>

VolumeSliderWidgetAction::VolumeSliderWidgetAction(QObject *parent)
	: QWidgetAction(parent), m_volumeSlider(make_qt_unique< QSlider >(Qt::Horizontal)) {
	m_volumeSlider->setMinimum(-30);
	m_volumeSlider->setMaximum(30);
	m_volumeSlider->setAccessibleName(tr("Slider for volume adjustment"));
	m_volumeSlider->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	KeyEventObserver *keyEventFilter = new KeyEventObserver(this, QEvent::KeyRelease, false,
															{ Qt::Key_Left, Qt::Key_Right, Qt::Key_Up, Qt::Key_Down });
	m_volumeSlider->installEventFilter(keyEventFilter);

	// The list of wheel events observed seems odd at first. We have to check for multiple
	// Qt::ScrollPhase types, because Qt has a weird/non-deterministic behavior regarding
	// scroll phases. There seems to be at least 3 different states the application can
	// be in at each start-up, which all produce different scroll phases using the same
	// inputs. This might very well be a Qt bug that is fixed in future versions and this
	// list could possibly be updated to reflect that. According to official Qt docs, observing
	// Qt::ScrollUpdate should be enough. But as of Qt 5.15.8 it is not.
	MouseWheelEventObserver *wheelEventFilter =
		new MouseWheelEventObserver(this, { Qt::ScrollMomentum, Qt::ScrollUpdate, Qt::ScrollEnd }, false);
	m_volumeSlider->installEventFilter(wheelEventFilter);

	connect(m_volumeSlider.get(), &QSlider::valueChanged, this,
			&VolumeSliderWidgetAction::on_VolumeSlider_valueChanged);
	connect(m_volumeSlider.get(), &QSlider::sliderReleased, this,
			&VolumeSliderWidgetAction::on_VolumeSlider_changeCompleted);
	connect(keyEventFilter, &KeyEventObserver::keyEventObserved, this,
			&VolumeSliderWidgetAction::on_VolumeSlider_changeCompleted);
	connect(wheelEventFilter, &MouseWheelEventObserver::wheelEventObserved, this,
			&VolumeSliderWidgetAction::on_VolumeSlider_changeCompleted);

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
