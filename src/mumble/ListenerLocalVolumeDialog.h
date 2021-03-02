// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_LISTENERVOLUME_H_
#define MUMBLE_MUMBLE_LISTENERVOLUME_H_

#include "ui_UserLocalVolumeDialog.h"

class ClientUser;
class Channel;

/// The dialog to configure the local volume adjustment for a "listener proxy". Therefore
/// this dialog can be used to tune the volume of audio streams one receivs via the listening
/// feature.
/// It actually uses the same UI frontend as UserLocalVolumeDialog as it only needs backend
/// changes.
class ListenerLocalVolumeDialog : public QDialog, private Ui::UserLocalVolumeDialog {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ListenerLocalVolumeDialog);

protected:
	/// The user belonging to the listener proxy this dialog has been invoked on
	ClientUser *m_user;
	/// The channel of the listener proxy this dialog has been invoked on
	Channel *m_channel;
	/// The volume adjustment that was set before this dialog opened
	float m_initialAdjustemt;

public slots:
	void on_qsUserLocalVolume_valueChanged(int value);
	void on_qsbUserLocalVolume_valueChanged(int value);
	void on_qbbUserLocalVolume_clicked(QAbstractButton *b);
	void reject();

public:
	ListenerLocalVolumeDialog(ClientUser *user, Channel *channel, QWidget *parent = nullptr);
};

#endif
