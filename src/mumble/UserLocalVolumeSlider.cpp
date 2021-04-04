// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserLocalVolumeSlider.h"
#include "ClientUser.h"
#include "Database.h"
#include "MainWindow.h"

#include <cmath>

#include "Global.h"

UserLocalVolumeSlider::UserLocalVolumeSlider(QWidget *parent) 
	: VolumeSliderWidgetAction(parent) {}

void UserLocalVolumeSlider::setUser(unsigned int sessionId) {

	m_clientSession = sessionId;

	ClientUser *user = ClientUser::get(sessionId);

	blockSliderSignals(false);

	if (user) {
		updateSliderValue(user->getLocalVolumeAdjustments());
	}

}


void UserLocalVolumeSlider::on_VolumeSlider_valueChanged(int value) {

	ClientUser *user = ClientUser::get(m_clientSession);

	displayTooltip(value);
	
	if (user) {
		// Decibel formula: +6db = *2
		user->setLocalVolumeAdjustment(static_cast< float >(pow(2.0, value / 6.0)));
	}
}

void UserLocalVolumeSlider::on_VolumeSlider_sliderReleased() {
	ClientUser *user = ClientUser::get(m_clientSession);
	if (user) {
		if (!user->qsHash.isEmpty()) {
			Global::get().db->setUserLocalVolume(user->qsHash, user->getLocalVolumeAdjustments());
		} else {
			Global::get().mw->logChangeNotPermanent(QObject::tr("Local Volume Adjustment..."), user);
		}
	}
}



