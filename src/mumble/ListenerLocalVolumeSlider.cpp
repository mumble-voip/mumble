// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ListenerLocalVolumeSlider.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "VolumeAdjustment.h"
#include "Global.h"

ListenerLocalVolumeSlider::ListenerLocalVolumeSlider(QWidget *parent) : VolumeSliderWidgetAction(parent) {
}

void ListenerLocalVolumeSlider::setListenedChannel(const Channel &channel) {
	m_channel = &channel;

	float initialAdjustment = Global::get().channelListenerManager->getListenerLocalVolumeAdjustment(m_channel->iId);
	updateSliderValue(initialAdjustment);
}

void ListenerLocalVolumeSlider::on_VolumeSlider_valueChanged(int value) {
	updateTooltip(value);
	displayTooltip(value);

	if (m_channel) {
		float factor = VolumeAdjustment::toFactor(value);
		Global::get().channelListenerManager->setListenerLocalVolumeAdjustment(m_channel->iId, factor);
	}
}
