// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ListenerLocalVolumeDialog.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "ClientUser.h"
#include "Global.h"

#include <QtWidgets/QPushButton>

#include <cmath>

ListenerLocalVolumeDialog::ListenerLocalVolumeDialog(ClientUser *user, Channel *channel, QWidget *parent)
	: QDialog(parent), m_user(user), m_channel(channel) {
	setupUi(this);

	m_initialAdjustemt = Global::get().channelListenerManager->getListenerLocalVolumeAdjustment(m_channel->iId);

	// Decibel formula: +6db = *2
	// Calculate the db-shift from the set volume-faactor
	float fdbShift = log2f(m_initialAdjustemt) * 6;
	qsUserLocalVolume->setValue(static_cast< int >(roundf(fdbShift)));

	setWindowTitle(tr("Adjusting local volume for listening to %1").arg(channel->qsName));
}

void ListenerLocalVolumeDialog::on_qsUserLocalVolume_valueChanged(int value) {
	qsbUserLocalVolume->setValue(value);
}

void ListenerLocalVolumeDialog::on_qsbUserLocalVolume_valueChanged(int value) {
	qsUserLocalVolume->setValue(value);

	// Decibel formula: +6db = *2
	// Calculate the volume-factor for the set db-shift
	Global::get().channelListenerManager->setListenerLocalVolumeAdjustment(
		m_channel->iId, static_cast< float >(pow(2.0, qsUserLocalVolume->value() / 6.0)));
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
	Global::get().channelListenerManager->setListenerLocalVolumeAdjustment(m_channel->iId, m_initialAdjustemt);

	QDialog::reject();
}
