// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserLocalVolumeSlider.h"
#include "ClientUser.h"
#include "Database.h"
#include "MainWindow.h"

#include "Global.h"

UserLocalVolumeSlider::UserLocalVolumeSlider(QWidget *parent) : VolumeSliderWidgetAction(parent) {
	connect(m_volumeSlider, &QSlider::sliderReleased, this, &UserLocalVolumeSlider::on_VolumeSlider_sliderReleased);
}

void UserLocalVolumeSlider::setUser(unsigned int sessionId) {
	m_clientSession = sessionId;

	ClientUser *user = ClientUser::get(sessionId);

	if (user) {
		updateSliderValue(user->getLocalVolumeAdjustments());
	}
}

void UserLocalVolumeSlider::on_VolumeSlider_valueChanged(int value) {
	ClientUser *user = ClientUser::get(m_clientSession);

	updateTooltip(value);
	displayTooltip(value);

	if (user) {
		float factor = VolumeAdjustment::toFactor(value);
		user->setLocalVolumeAdjustment(factor);
	}
}

void UserLocalVolumeSlider::on_VolumeSlider_changeCompleted() {
	ClientUser *user = ClientUser::get(m_clientSession);
	if (user) {
		if (!user->qsHash.isEmpty()) {
			Global::get().db->setUserLocalVolume(user->qsHash, user->getLocalVolumeAdjustments());
		}

		updateLabelValue();
	}
}

void UserLocalVolumeSlider::on_VolumeSlider_sliderReleased() {
	ClientUser *user = ClientUser::get(m_clientSession);
	if (user && user->qsHash.isEmpty()) {
		Global::get().mw->logChangeNotPermanent(QObject::tr("Local Volume Adjustment..."), user);
	}
}
