// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VolumeSliderWidgetAction.h"

#include "MumbleApplication.h"
#include "VolumeAdjustment.h"
#include "widgets/EventFilters.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QToolTip>

VolumeSliderWidgetAction::VolumeSliderWidgetAction(QWidget *parent)
	: QWidgetAction(parent), m_widget(make_qt_unique< QWidget >(parent)),
	  m_volumeSlider(new QSlider(Qt::Horizontal, parent)), m_label(new QLabel("0 db", parent)) {
	m_volumeSlider->setMinimum(-30);
	m_volumeSlider->setMaximum(30);
	m_volumeSlider->setAccessibleName(tr("Local volume adjustment"));

	m_label->setStyleSheet("QLabel { margin-left: 0px; padding: 0px; }");
	m_volumeSlider->setStyleSheet("QSlider { margin-right: 0px; }");

	KeyEventObserver *keyEventFilter =
		new KeyEventObserver(this, QEvent::KeyRelease, false, { Qt::Key_Left, Qt::Key_Right });
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


	// Since we do not want the inline label to update when we drag the mouse, we
	// must install a click event observer to update the label when the user just randomly
	// clicks on the slider bar.
	MouseClickEventObserver *mouseEventFilter = new MouseClickEventObserver(this, false);
	m_volumeSlider->installEventFilter(mouseEventFilter);
	connect(mouseEventFilter, &MouseClickEventObserver::clickEventObserved, this, [this]() {
		m_volumeSlider->setFocus(Qt::TabFocusReason);
		updateLabelValue(false);
	});

	// Also update the label explicitly when the slider body is released.
	connect(m_volumeSlider, &QSlider::sliderReleased, this, [this]() {
		m_volumeSlider->setFocus(Qt::TabFocusReason);
		updateLabelValue(false);
	});

	connect(m_volumeSlider, &QSlider::valueChanged, this, &VolumeSliderWidgetAction::on_VolumeSlider_valueChanged);

	connect(m_volumeSlider, &QSlider::sliderReleased, this, &VolumeSliderWidgetAction::on_VolumeSlider_changeCompleted);
	connect(keyEventFilter, &KeyEventObserver::keyEventObserved, this,
			&VolumeSliderWidgetAction::on_VolumeSlider_changeCompleted);
	connect(wheelEventFilter, &MouseWheelEventObserver::wheelEventObserved, this,
			&VolumeSliderWidgetAction::on_VolumeSlider_changeCompleted);
	connect(mouseEventFilter, &MouseClickEventObserver::clickEventObserved, this,
			&VolumeSliderWidgetAction::on_VolumeSlider_changeCompleted);

	UpDownKeyEventFilter *eventFilter = new UpDownKeyEventFilter(this);
	m_volumeSlider->installEventFilter(eventFilter);

	// Used to display the drag tooltip at the mouse position
	m_volumeSlider->setProperty("mouseTracking", true);

	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(m_volumeSlider);
	layout->addWidget(m_label);
	layout->setContentsMargins(0, 0, -1, 0);
	layout->setSpacing(3);
	m_widget->setLayout(layout);

	m_widget->setFocusProxy(m_volumeSlider);
	m_widget->setFocusPolicy(Qt::TabFocus);

	setDefaultWidget(m_widget.get());
}

void VolumeSliderWidgetAction::updateLabelValue(bool checkMouseButtons) {
	if (checkMouseButtons && MumbleApplication::instance()->mouseButtons() != Qt::NoButton) {
		// Do not update the label while the user is dragging the slider.
		// This will otherwise cause a glitchy experience.
		return;
	}

	m_label->setText(QString("%01dB").arg(m_volumeSlider->value()));
}

void VolumeSliderWidgetAction::updateSliderValue(float value) {
	int dbShift = VolumeAdjustment::toIntegerDBAdjustment(value);
	m_volumeSlider->setValue(dbShift);
	updateTooltip(dbShift);
	updateLabelValue(false);
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
