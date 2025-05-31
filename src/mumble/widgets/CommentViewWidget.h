// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_COMMENTVIEWWIDGET_H_
#define MUMBLE_MUMBLE_WIDGETS_COMMENTVIEWWIDGET_H_

#include <QtCore/QMetaObject>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/qcompilerdetection.h>
#include <memory>
#include <ui_CommentViewWidget.h>

#include "ClientUser.h"
#include "ConfigWidget.h"
#include "UserInformation.h"


class CommentViewWidget final : public QDockWidget, Ui_CommentViewWidget {
private:
	Q_OBJECT
	Q_DISABLE_COPY(CommentViewWidget)

	void updateCommentContent(const ClientUser *user, const Channel *channel) const;

public:
	explicit CommentViewWidget(QWidget *parent = nullptr);

	void updateUserStats(const MumbleProto::UserStats &stats) const;

	void updateCommentView(ClientUser *user, Channel *channel);

protected:
	std::unique_ptr< QObject > updateCommentSubscription = nullptr;
	uint32_t m_currentSession;
};

#endif // MUMBLE_MUMBLE_WIDGETS_COMMENTVIEWWIDGET_H_