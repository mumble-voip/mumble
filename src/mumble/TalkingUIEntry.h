// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_TALKINGUIENTRY_H_
#define MUMBLE_MUMBLE_TALKINGUIENTRY_H_

#include "Settings.h"
#include "TalkingUIComponent.h"
#include "widgets/MultiStyleWidgetWrapper.h"

#include <QIcon>
#include <QLatin1String>
#include <QString>
#include <QTimer>

#include <vector>

class QWidget;
class QLabel;
class TalkingUIContainer;
class ClientUser;
class Channel;

enum class EntryType { USER, LISTENER };

enum class EntryPriority { LOWEST, LOW, DEFAULT, HIGH };


class TalkingUIEntry : public TalkingUIComponent {
	// Needed in order for the container class to be able to modify m_container
	friend class TalkingUIContainer;

protected:
	unsigned int m_associatedUserSession;

	TalkingUIContainer *m_container = nullptr;

	EntryPriority m_priority = EntryPriority::DEFAULT;

public:
	TalkingUIEntry(unsigned int associatedUserSession);
	virtual ~TalkingUIEntry() = default;

	virtual EntryType getType() const = 0;

	virtual unsigned int getAssociatedUserSession() const;

	virtual TalkingUIContainer *getContainer();
	virtual const TalkingUIContainer *getContainer() const;

	virtual void setPriority(EntryPriority priority);
	virtual EntryPriority getPriority() const;

	virtual void setIconSize(int size) = 0;

	virtual void setDisplayString(const QString &displayString) = 0;

	virtual int compare(const TalkingUIEntry &other) const;

	bool operator==(const TalkingUIEntry &other) const;
	bool operator!=(const TalkingUIEntry &other) const;
	bool operator<(const TalkingUIEntry &other) const;
	bool operator<=(const TalkingUIEntry &other) const;
	bool operator>(const TalkingUIEntry &other) const;
	bool operator>=(const TalkingUIEntry &other) const;
};


class TalkingUIUser : public TalkingUIEntry {
protected:
	QWidget *m_backgroundWidget = nullptr;
	MultiStyleWidgetWrapper m_backgroundWidgetStyleWrapper;

	QLabel *m_talkingIcon = nullptr;
	QLabel *m_nameLabel   = nullptr;
	QLabel *m_statusIcons = nullptr;

	QString m_name;

	int m_iconSize                     = 5;
	Settings::TalkState m_talkingState = Settings::Passive;

	QTimer m_timer;
	bool m_restrictLifetime = false;

	const QIcon &getTalkingIcon(Settings::TalkState talkState) const;

	virtual void updateTalkingIcon();

public:
	struct UserStatus {
		bool muted, selfMuted, localMuted, deafened, selfDeafened;
	};

	TalkingUIUser(const ClientUser &user);
	virtual ~TalkingUIUser() override;

	virtual QWidget *getWidget() override;
	virtual const QWidget *getWidget() const override;
	virtual MultiStyleWidgetWrapper &getStylableWidget() override;

	virtual EntryType getType() const override;

	virtual QString getName() const;

	virtual int compare(const TalkingUIEntry &other) const override;

	virtual void setTalkingState(Settings::TalkState talkState);

	virtual void setIconSize(int size) override;

	virtual void setDisplayString(const QString &displayString) override;

	virtual void setLifeTime(unsigned int time);
	virtual void restrictLifetime(bool restrict);

	virtual void setStatus(UserStatus status);
};


class TalkingUIChannelListener : public TalkingUIEntry {
protected:
	QWidget *m_backgroundWidget = nullptr;
	MultiStyleWidgetWrapper m_backgroundWidgetStyleWrapper;

	QLabel *m_icon      = nullptr;
	QLabel *m_nameLabel = nullptr;

	unsigned int m_channelID;
	QString m_name;

public:
	TalkingUIChannelListener(const ClientUser &user, const Channel &channel);
	virtual ~TalkingUIChannelListener() override;

	virtual EntryType getType() const override;

	virtual QWidget *getWidget() override;
	virtual const QWidget *getWidget() const override;
	MultiStyleWidgetWrapper &getStylableWidget() override;

	virtual void setIconSize(int size) override;

	virtual void setDisplayString(const QString &displayString) override;

	virtual int getAssociatedChannelID() const;
};

#endif // MUMBLE_MUMBLE_TALKINGUIENTRY_H_
