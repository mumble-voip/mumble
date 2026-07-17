// Copyright 2026 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.
//
// 版本号: 1.0.2
// 修订日期: 2026-07-17
// 作者/生成者: Auto (Composer)
// 最后修订人: Auto (Composer)
// 变更说明:
//   - 1.0.0 (2026-07-17): 初版。多群组（Channel Listener）管理对话框实现。
//   - 1.0.1 (2026-07-17): 合入 yqwer/mumble 最新 master（约 1.7.x）时同步保留。
//   - 1.0.2 (2026-07-17): 适配最新 API：Channel::ROOT_ID → Mumble::ROOT_CHANNEL_ID。

#include "MultiGroupDialog.h"

#include "Channel.h"
#include "ChannelListenerManager.h"
#include "ClientUser.h"
#include "Global.h"
#include "MumbleConstants.h"
#include "ServerHandler.h"
#include "Version.h"

#include <QtGui/QShowEvent>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QMessageBox>

MultiGroupDialog::MultiGroupDialog(QWidget *parent) : QDialog(parent) {
	setupUi(this);
}

void MultiGroupDialog::showEvent(QShowEvent *event) {
	QDialog::showEvent(event);
	refreshLists();
}

void MultiGroupDialog::collectChannels(const Channel *channel, QList< const Channel * > &out) const {
	if (!channel) {
		return;
	}
	out << channel;
	for (Channel *child : channel->qlChannels) {
		collectChannels(child, out);
	}
}

void MultiGroupDialog::refreshLists() {
	qlwAvailable->clear();
	qlwListening->clear();

	ClientUser *self = ClientUser::get(Global::get().uiSession);
	if (!self || !self->cChannel || !Global::get().sh || !Global::get().sh->isConnected()) {
		qlCurrentChannel->setText(tr("Current speak channel: (not connected)"));
		qpbJoin->setEnabled(false);
		qpbLeave->setEnabled(false);
		return;
	}

	qpbJoin->setEnabled(true);
	qpbLeave->setEnabled(true);

	const Channel *current = self->cChannel;
	qlCurrentChannel->setText(tr("Current speak channel: %1 (PTT goes here only)")
								  .arg(current->getPath().isEmpty() ? current->qsName : current->getPath()));

	if (Global::get().sh->m_version < Version::fromComponents(1, 4, 0)) {
		QMessageBox::information(this, tr("Multi-Group Listening"),
								 tr("This server does not support Channel Listeners (requires Mumble 1.4+)."));
		qpbJoin->setEnabled(false);
		qpbLeave->setEnabled(false);
		return;
	}

	const QSet< unsigned int > listening =
		Global::get().channelListenerManager->getListenedChannelsForUser(Global::get().uiSession);

	QList< const Channel * > allChannels;
	collectChannels(Channel::get(Mumble::ROOT_CHANNEL_ID), allChannels);

	for (const Channel *channel : allChannels) {
		if (!channel) {
			continue;
		}
		if (listening.contains(channel->iId)) {
			auto *item = new QListWidgetItem(channel->getPath().isEmpty() ? channel->qsName : channel->getPath());
			item->setData(Qt::UserRole, channel->iId);
			if (channel->iId == current->iId) {
				item->setText(item->text() + tr(" [current]"));
			}
			qlwListening->addItem(item);
		} else if (channel->iId != current->iId) {
			// Current channel membership already receives audio; listing it as "available" is confusing.
			auto *item = new QListWidgetItem(channel->getPath().isEmpty() ? channel->qsName : channel->getPath());
			item->setData(Qt::UserRole, channel->iId);
			qlwAvailable->addItem(item);
		}
	}
}

void MultiGroupDialog::on_qpbJoin_clicked() {
	if (!Global::get().sh || !Global::get().sh->isConnected()) {
		return;
	}

	QList< unsigned int > ids;
	const QList< QListWidgetItem * > selected = qlwAvailable->selectedItems();
	for (QListWidgetItem *item : selected) {
		ids << item->data(Qt::UserRole).toUInt();
	}

	if (ids.isEmpty()) {
		return;
	}

	Global::get().sh->startListeningToChannels(ids);
	// Server ack updates ChannelListenerManager; refresh shortly via user state.
	refreshLists();
}

void MultiGroupDialog::on_qpbLeave_clicked() {
	if (!Global::get().sh || !Global::get().sh->isConnected()) {
		return;
	}

	QList< unsigned int > ids;
	const QList< QListWidgetItem * > selected = qlwListening->selectedItems();
	for (QListWidgetItem *item : selected) {
		ids << item->data(Qt::UserRole).toUInt();
	}

	if (ids.isEmpty()) {
		return;
	}

	Global::get().sh->stopListeningToChannels(ids);
	refreshLists();
}

void MultiGroupDialog::on_qpbRefresh_clicked() {
	refreshLists();
}
