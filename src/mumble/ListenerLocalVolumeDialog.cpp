// Copyright 2020-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ListenerLocalVolumeDialog.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "ClientUser.h"
#include "VolumeAdjustment.h"
#include "Global.h"

#include <QtWidgets/QPushButton>

#include <cmath>

ListenerLocalVolumeDialog::ListenerLocalVolumeDialog(ClientUser *user, Channel *channel, QWidget *parent)
	: QDialog(parent), m_user(user), m_channel(channel) {
	setupUi(this);

	m_initialAdjustment = Global::get().channelListenerManager->getListenerLocalVolumeAdjustment(m_channel->iId);

	// Calculate the db-shift from the set volume-factor
	int dbShift = VolumeAdjustment::toIntegerDBAdjustment(m_initialAdjustment);
	qsUserLocalVolume->setValue(dbShift);

	setWindowTitle(tr("Adjusting local volume for listening to %1").arg(channel->qsName));
}

void ListenerLocalVolumeDialog::on_qsUserLocalVolume_valueChanged(int value) {
	qsbUserLocalVolume->setValue(value);
}

void ListenerLocalVolumeDialog::on_qsbUserLocalVolume_valueChanged(int value) {
	qsUserLocalVolume->setValue(value);

	// Calculate the volume-factor for the set db-shift
	float factor = VolumeAdjustment::toFactor(qsUserLocalVolume->value());
	Global::get().channelListenerManager->setListenerLocalVolumeAdjustment(m_channel->iId, factor);
}

void ListenerLocalVolumeDialog::on_qbbUserLocalVolume_clicked(QAbstractButton *button) {
	if (button == qbbUserLocalVolume->button(QDialogButtonBox::Reset)) {
		qsUserLocalVolume->setValue(0);
	}
	if (button == qbbUserLocalVolume->button(QDialogButtonBox::Ok)) {
		ListenerLocalVolumeDialog::accept();
	}
	if (button == qbbUserLocalVolume->button(QDialogButtonBox::Cancel)) {
		ListenerLocalVolumeDialog::close();
	}
}

void ListenerLocalVolumeDialog::reject() {
	// Restore to what has been set before the dialog
	Global::get().channelListenerManager->setListenerLocalVolumeAdjustment(m_channel->iId, m_initialAdjustment);

	QDialog::reject();
}
