// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ListenerVolumeSlider.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "ServerHandler.h"
#include "Global.h"

ListenerVolumeSlider::ListenerVolumeSlider(QWidget *parent) : VolumeSliderWidgetAction(parent), m_currentSendDelay(0) {
	connect(&m_sendTimer, &QTimer::timeout, this, &ListenerVolumeSlider::sendToServer);
	connect(&m_resetTimer, &QTimer::timeout, this, [this]() { m_currentSendDelay = 0; });

	m_sendTimer.setSingleShot(true);
	m_resetTimer.setSingleShot(true);
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

void ListenerVolumeSlider::on_VolumeSlider_changeCompleted() {
	ServerHandlerPtr handler = Global::get().sh;

	if (!handler || !m_channel || !m_volumeSlider) {
		return;
	}

	VolumeAdjustment adjustment = VolumeAdjustment::fromDBAdjustment(m_volumeSlider->value());
	updateLabelValue();

	if (handler->m_version >= Mumble::Protocol::PROTOBUF_INTRODUCTION_VERSION) {
		// With the new audio protocol, volume adjustments for listeners are handled on the server and thus we want
		// to avoid spamming updates to the adjustments, which is why we only update them with a delay.

		if (m_cachedChannelID == m_channel->iId && m_cachedAdjustment == adjustment) {
			return;
		}

		m_cachedChannelID  = m_channel->iId;
		m_cachedAdjustment = adjustment;

		// Timer values: 0, 50, 150, 350, 750, 1000 (ms)
		m_resetTimer.stop();
		m_sendTimer.start(static_cast< int >(m_currentSendDelay));
		m_currentSendDelay = std::min(1000u, (m_currentSendDelay + 25) * 2);

	} else {
		// Before the new audio protocol, volume adjustments for listeners are handled locally
		Global::get().channelListenerManager->setListenerVolumeAdjustment(Global::get().uiSession, m_channel->iId,
																		  adjustment);
	}
}

void ListenerVolumeSlider::sendToServer() {
	ServerHandlerPtr handler = Global::get().sh;

	m_resetTimer.start(3000);

	if (!handler) {
		return;
	}

	MumbleProto::UserState mpus;
	mpus.set_session(Global::get().uiSession);

	MumbleProto::UserState::VolumeAdjustment *adjustmentMsg = mpus.add_listening_volume_adjustment();
	adjustmentMsg->set_listening_channel(m_cachedChannelID);
	adjustmentMsg->set_volume_adjustment(m_cachedAdjustment.factor);

	handler->sendMessage(mpus);
}
