// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_USERLOCALVOLUMESLIDER_H_
#define MUMBLE_MUMBLE_USERLOCALVOLUMESLIDER_H_

class ClientUser;

#include "VolumeSliderWidgetAction.h"

class UserLocalVolumeSlider : public VolumeSliderWidgetAction {
	Q_OBJECT

	/// The session ID for the user that the widget is changing the volume for.
	unsigned int m_clientSession;

public:
	UserLocalVolumeSlider(QWidget *parent = nullptr);

	/// Must be called before adding this object as an action
	void setUser(unsigned int sessionId);

private slots:
	void on_VolumeSlider_valueChanged(int value) override;
	void on_VolumeSlider_changeCompleted() override;
	void on_VolumeSlider_sliderReleased();
};

#endif
