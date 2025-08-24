// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CommentViewWidget.h"

#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <memory>

#include "../../Channel.h"
#include "../../User.h"
#include "../ClientUser.h"
#include "../Database.h"
#ifdef USE_OVERLAY
#	include "Overlay.h"
#endif
#include "ProtoUtils.h"
#include "QtUtils.h"
#include "ServerHandler.h"
#include "TextureManager.h"
#include "../Global.h"


CommentViewWidget::CommentViewWidget(QWidget *parent)
	: QDockWidget(parent), Ui_CommentViewWidget(), m_currentSession(0) {
	setupUi(this);

	setTitleBarWidget(qlTitle);
}

void CommentViewWidget::updateCommentContent(const ClientUser *user, const Channel *channel) const {
	QString comment;
	if (user) {
		qlTitle->setText(user->qsName);
		qlDescriptionTitle->setText(tr("Comment"));
		comment = user->qsComment();

		QPixmap pix = QPixmap();
		// pretty sure this is not hardcoded

		pix.loadFromData(user->qbaTexture, user->qbaTextureFormat);

		qlAvatar->setImage(pix);
		qlAvatar->setVisible(!user->qbaTexture.isEmpty());

		qfUserView->setVisible(true);
		qfChannelView->setVisible(false);
	} else if (channel) {
		qlTitle->setText(channel->qsName);

		qlChannelUserCount->setText(QString::number(channel->qlUsers.count()));
		qlChannelProperties->setText(tr(channel->bTemporary ? "Temporary" : "Permanent"));
		qlDescriptionTitle->setText(tr("Description"));
		comment = channel->qsDesc;

		qfUserView->setVisible(false);
		qfChannelView->setVisible(true);
	} else {
		return;
	}

	qlDescription->setText(comment);
}

void CommentViewWidget::updateUserStats(const MumbleProto::UserStats &stats) const {
	if (m_currentSession == stats.session()) {
		qlUserVersion->setVisible(stats.has_version());
		qlUserVersionCaption->setVisible(stats.has_version());
		qlUserOs->setVisible(stats.has_version());
		qlUserOsCaption->setVisible(stats.has_version());
		if (stats.has_version()) {
			const MumbleProto::Version &mpv = stats.version();
			Version::full_t version         = MumbleProto::getVersion(mpv);
			qlUserVersion->setText(tr("%1 (%2)").arg(Version::toString(version), u8(mpv.release())));
			qlUserOs->setText(tr("%1 (%2)").arg(u8(mpv.os()), u8(mpv.os_version())));
		}
	}
}

void CommentViewWidget::updateCommentView(ClientUser *user, Channel *channel) {
	if (user) {
		m_currentSession = user->uiSession;
		Global::get().sh->requestUserStats(user->uiSession, false);

		if (!user->qbaCommentHash.isEmpty() && user->qsComment().isEmpty()) {
			user->setComment(QString::fromUtf8(Global::get().db->blob(user->qbaCommentHash)));
		}
		if (!user->qbaTextureHash.isEmpty() && user->qbaTexture.isEmpty()) {
			user->qbaTexture = Global::get().db->blob(user->qbaTextureHash);
			if (!user->qbaTexture.isEmpty()) {
#ifdef USE_OVERLAY
				Global::get().o->updateOverlay();
#endif
			}
		}
		updateCommentContent(user, nullptr);

		connect(user, &ClientUser::localNicknameChanged, this, [=, this]() { updateCommentContent(user, nullptr); });
		connect(user, &ClientUser::commentChanged, this, [=, this]() { updateCommentContent(user, nullptr); });

	} else {
		m_currentSession = 0;
		// Get root channel if neither user nor channel are selected
		if (!channel) {
			const ClientUser *clientUser = ClientUser::get(Global::get().uiSession);
			if (!clientUser) {
				return; // no channels yet
			}
			channel = clientUser->cChannel;
			while (channel->cParent != nullptr) {
				channel = channel->cParent;
			}
		}
		if (!channel->qbaDescHash.isEmpty() && channel->qsDesc.isEmpty()) {
			channel->qsDesc = QString::fromUtf8(Global::get().db->blob(channel->qbaDescHash));
		}
		updateCommentContent(nullptr, channel);

		updateCommentSubscription                   = std::make_unique< QObject >();
		const QObject *updateCommentSubscriptionRaw = updateCommentSubscription.get();
		connect(channel, &Channel::channelEntered, updateCommentSubscriptionRaw,
				[=, this](const Channel *newChannel, [[maybe_unused]] const Channel *prevChannel,
						  [[maybe_unused]] const User *u) { updateCommentContent(nullptr, newChannel); });
		connect(channel, &Channel::channelExited, updateCommentSubscriptionRaw,
				[=, this](const Channel *c, [[maybe_unused]] const User *u) { updateCommentContent(nullptr, c); });
	}
}
