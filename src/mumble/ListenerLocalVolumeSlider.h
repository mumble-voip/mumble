// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_LISTENERLOCALVOLUMESLIDER_H_
#define MUMBLE_MUMBLE_LISTENERLOCALVOLUMESLIDER_H_

class Channel;

#include "VolumeSliderWidgetAction.h"

class ListenerLocalVolumeSlider : public VolumeSliderWidgetAction {
	Q_OBJECT

	/// The channel of the listener proxy this dialog is operating on
	const Channel *m_channel;

public:
	ListenerLocalVolumeSlider(QWidget *parent = nullptr);

	/// Must be called before adding this object as an action
	void setListenedChannel(const Channel &channel);

private slots:
	void on_VolumeSlider_valueChanged(int value);

};


#endif