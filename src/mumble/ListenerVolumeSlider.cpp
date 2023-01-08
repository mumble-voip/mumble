// Copyright 2022-2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ListenerVolumeSlider.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "ServerHandler.h"
#include "VolumeAdjustment.h"
#include "Global.h"

ListenerVolumeSlider::ListenerVolumeSlider(QWidget *parent) : VolumeSliderWidgetAction(parent) {
}

void ListenerVolumeSlider::setListenedChannel(const Channel &channel) {
	m_channel = &channel;

	float initialAdjustment =
		Global::get()
			.channelListenerManager->getListenerVolumeAdjustment(Global::get().uiSession, m_channel->iId)
			.factor;
	updateSliderValue(initialAdjustment);
}

void ListenerVolumeSlider::on_VolumeSlider_valueChanged(int value) {
	updateTooltip(value);
	displayTooltip(value);
}

void ListenerVolumeSlider::on_VolumeSlider_sliderReleased() {
	ServerHandlerPtr handler = Global::get().sh;

	if (!handler || !m_channel || !m_volumeSlider) {
		return;
	}

	VolumeAdjustment adjustment = VolumeAdjustment::fromDBAdjustment(m_volumeSlider->value());

	if (handler->m_version >= Mumble::Protocol::PROTOBUF_INTRODUCTION_VERSION) {
		// With the new audio protocol, volume adjustments for listeners are handled on the server and thus we want
		// to avoid spamming updates to the adjustments, which is why we only update them once the slider is released.
		MumbleProto::UserState mpus;
		mpus.set_session(Global::get().uiSession);

		MumbleProto::UserState::VolumeAdjustment *adjustmentMsg = mpus.add_listening_volume_adjustment();
		adjustmentMsg->set_listening_channel(m_channel->iId);
		adjustmentMsg->set_volume_adjustment(adjustment.factor);

		handler->sendMessage(mpus);
	} else {
		// Before the new audio protocol, volume adjustments for listeners are handled locally
		Global::get().channelListenerManager->setListenerVolumeAdjustment(Global::get().uiSession, m_channel->iId,
																		  adjustment);
	}
}
