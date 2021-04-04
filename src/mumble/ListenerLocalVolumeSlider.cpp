// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ListenerLocalVolumeSlider.h"
#include "ChannelListener.h"

#include <cmath>


ListenerLocalVolumeSlider::ListenerLocalVolumeSlider(QWidget *parent)
	: VolumeSliderWidgetAction(parent) {}

void ListenerLocalVolumeSlider::setListenedChannel(const Channel &channel) {

	m_channel = &channel;

	blockSliderSignals(false);


	float initialAdjustment = ChannelListener::getListenerLocalVolumeAdjustment(m_channel);
	updateSliderValue(initialAdjustment);

}


void ListenerLocalVolumeSlider::on_VolumeSlider_valueChanged(int value) {

	displayTooltip(value);

	// Decibel formula: +6db = *2
	if (m_channel) {
		ChannelListener::setListenerLocalVolumeAdjustment(m_channel, static_cast< float >(pow(2.0, value / 6.0)));
	}


}