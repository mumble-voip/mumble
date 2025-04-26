// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TalkingUIEntry.h"
#include "Channel.h"
#include "ClientUser.h"
#include "TalkingUI.h"
#include "TalkingUIContainer.h"
#include "UserModel.h"
#include "Global.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QVariant>
#include <QWidget>

TalkingUIEntry::TalkingUIEntry(unsigned int associatedUserSession) : m_associatedUserSession(associatedUserSession) {
}

unsigned int TalkingUIEntry::getAssociatedUserSession() const {
	return m_associatedUserSession;
}

TalkingUIContainer *TalkingUIEntry::getContainer() {
	return m_container;
}

const TalkingUIContainer *TalkingUIEntry::getContainer() const {
	return m_container;
}

int TalkingUIEntry::compare(const TalkingUIEntry &other) const {
	if (getPriority() != other.getPriority()) {
		return static_cast< int >(getPriority()) - static_cast< int >(other.getPriority());
	}

	if (getType() != other.getType()) {
		return other.getType() > getType() ? -1 : 1;
	}

	if (other.getAssociatedUserSession() == getAssociatedUserSession()) {
		return 0;
	}

	return other.getAssociatedUserSession() > m_associatedUserSession ? -1 : 1;
}

void TalkingUIEntry::setPriority(EntryPriority priority) {
	m_priority = priority;
}

EntryPriority TalkingUIEntry::getPriority() const {
	return m_priority;
}

bool TalkingUIEntry::operator==(const TalkingUIEntry &other) const {
	return compare(other) == 0;
}

bool TalkingUIEntry::operator!=(const TalkingUIEntry &other) const {
	return compare(other) != 0;
}

bool TalkingUIEntry::operator<(const TalkingUIEntry &other) const {
	return compare(other) < 0;
}

bool TalkingUIEntry::operator<=(const TalkingUIEntry &other) const {
	return compare(other) <= 0;
}

bool TalkingUIEntry::operator>(const TalkingUIEntry &other) const {
	return compare(other) > 0;
}

bool TalkingUIEntry::operator>=(const TalkingUIEntry &other) const {
	return compare(other) >= 0;
}



static const int USER_CONTAINER_HORIZONTAL_MARGIN = 2;
static const int USER_CONTAINER_VERTICAL_MARGIN   = 3;


TalkingUIUser::TalkingUIUser(const ClientUser &user)
	: TalkingUIEntry(user.uiSession), m_backgroundWidget(new QWidget()),
	  m_backgroundWidgetStyleWrapper(m_backgroundWidget), m_name(user.qsName), m_timer() {
	// Create background widget and its layout that we'll use to place all other
	// components on
	m_backgroundWidget->setProperty("selected", false);
	QLayout *backgroundLayout = new QHBoxLayout();
	backgroundLayout->setContentsMargins(USER_CONTAINER_HORIZONTAL_MARGIN, USER_CONTAINER_VERTICAL_MARGIN,
										 USER_CONTAINER_HORIZONTAL_MARGIN, USER_CONTAINER_VERTICAL_MARGIN);
	m_backgroundWidget->setLayout(backgroundLayout);
	m_backgroundWidget->setAutoFillBackground(true);

	// Create the label we use to display the user's name
	m_nameLabel                 = new QLabel(m_backgroundWidget);
	const QString displayString = UserModel::createDisplayString(user, false, nullptr);
	setDisplayString(displayString);

	// Create the label we'll use to display the user's TalkingIcon
	m_talkingIcon = new QLabel(m_backgroundWidget);
	m_talkingIcon->setAlignment(Qt::AlignCenter);
	m_talkingIcon->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	// Create the label we'll use to display any status icons for the user
	m_statusIcons = new QLabel(m_backgroundWidget);
	m_statusIcons->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_statusIcons->hide(); // hide by default
	// Uncomment the line below if you want to debug the icons
	// m_statusIcons->setStyleSheet(QLatin1String("border: 1px solid black;"));


	// By default we assume that there are no status icons to display and thus we only add
	// the label for the TalkingIcon and the name
	backgroundLayout->addWidget(m_talkingIcon);
	backgroundLayout->addWidget(m_nameLabel);

	// init size for talking icon to FontSize
	m_iconSize = QFontMetrics(m_backgroundWidget->font()).height();

	updateTalkingIcon();

	// If the timer runs out, we remove this entry from its container (if any)
	// and thereby this will cause its deletion (smart-pointer goes out of scope)
	QObject::connect(&m_timer, &QTimer::timeout, [this]() {
		if (getContainer()) {
			// We let the TalkingUI handle the removing in order for it to do the necessary
			// housekeeping (e.Global::get(). making sure there is no active selection for this user).
			getContainer()->m_talkingUI.removeUser(getAssociatedUserSession());
		}
	});
}

TalkingUIUser::~TalkingUIUser() {
	// Note that we have to use deleteLater instead of directly deleting the objects since
	// the latter can easily cause segmentation faults due to Qt's automatic memory
	// management model (parent-child-hierarchy)
	m_backgroundWidget->deleteLater();
	m_talkingIcon->deleteLater();
	m_nameLabel->deleteLater();
	m_statusIcons->deleteLater();
}

const QIcon &TalkingUIUser::getTalkingIcon(Settings::TalkState talkState) const {
	static const QIcon s_talkingIcon      = QIcon(QLatin1String("skin:talking_on.svg"));
	static const QIcon s_mutedTalkingIcon = QIcon(QLatin1String("skin:talking_muted.svg"));
	static const QIcon s_passiveIcon      = QIcon(QLatin1String("skin:talking_off.svg"));
	static const QIcon s_shoutingIcon     = QIcon(QLatin1String("skin:talking_alt.svg"));
	static const QIcon s_whisperingIcon   = QIcon(QLatin1String("skin:talking_whisper.svg"));

	switch (talkState) {
		case Settings::MutedTalking:
			return s_mutedTalkingIcon;
		case Settings::Talking:
			return s_talkingIcon;
		case Settings::Whispering:
			return s_whisperingIcon;
		case Settings::Shouting:
			return s_shoutingIcon;
		case Settings::Passive:
			return s_passiveIcon;
	}

	// switch above should cover all possible TalkStates
	// Return s_passiveIcon as a fallback
	qCritical("TalkingUIUser: Encountered invalid TalkState");
	return s_passiveIcon;
}

QWidget *TalkingUIUser::getWidget() {
	return m_backgroundWidget;
}

const QWidget *TalkingUIUser::getWidget() const {
	return m_backgroundWidget;
}

MultiStyleWidgetWrapper &TalkingUIUser::getStylableWidget() {
	return m_backgroundWidgetStyleWrapper;
}

EntryType TalkingUIUser::getType() const {
	return EntryType::USER;
}

QString TalkingUIUser::getName() const {
	return m_name;
}

int TalkingUIUser::compare(const TalkingUIEntry &other) const {
	if (other.getType() != getType()) {
		return static_cast< int >(other.getType()) - static_cast< int >(getType());
	}

	const TalkingUIUser &otherUser = static_cast< const TalkingUIUser & >(other);

	if (getPriority() != otherUser.getPriority()) {
		return getPriority() < other.getPriority() ? 1 : -1;
	}

	int res = getName().localeAwareCompare(otherUser.getName());

	if (res == 0) {
		// Make sure that we only consider users the same, if they have the same ID and
		// not simply because they happen to have the same name
		res = static_cast< int >(otherUser.getAssociatedUserSession()) - static_cast< int >(getAssociatedUserSession());
	}

	return res;
}

void TalkingUIUser::updateTalkingIcon() {
	m_talkingIcon->setPixmap(
		getTalkingIcon(m_talkingState).pixmap(QSize(m_iconSize, m_iconSize), QIcon::Normal, QIcon::On));
}

void TalkingUIUser::setTalkingState(Settings::TalkState talkState) {
	if (m_talkingState == talkState) {
		return;
	}

	m_talkingState = talkState;

	switch (talkState) {
		case Settings::Passive:
			if (m_restrictLifetime) {
				// Start timer as soon as the user has become passive
				m_timer.start();
			}
			break;
		case Settings::Talking:
		case Settings::Whispering:
		case Settings::Shouting:
		case Settings::MutedTalking:
			// Stop timer as the user is no longer inactive
			m_timer.stop();
	}

	updateTalkingIcon();
}

void TalkingUIUser::setIconSize(int size) {
	m_iconSize = size;

	updateTalkingIcon();
}

void TalkingUIUser::setDisplayString(const QString &displayString) {
	if (Global::get().uiSession == getAssociatedUserSession()) {
		// Display own name in bold
		m_nameLabel->setText(QString::fromLatin1("<b>%1</b>").arg(displayString));
	} else {
		m_nameLabel->setText(displayString);
	}
}

void TalkingUIUser::setLifeTime(unsigned int time) {
	m_timer.setInterval(static_cast< int >(time));
}

void TalkingUIUser::restrictLifetime(bool restrict) {
	if (restrict && !m_timer.isActive()) {
		// Start timer
		m_timer.start();
	} else if (!restrict && m_timer.isActive()) {
		// Stop timer
		m_timer.stop();
	}

	m_restrictLifetime = restrict;
}

void TalkingUIUser::setStatus(UserStatus status) {
	static const QIcon s_muteIcon      = QIcon(QLatin1String("skin:muted_server.svg"));
	static const QIcon s_deafIcon      = QIcon(QLatin1String("skin:deafened_server.svg"));
	static const QIcon s_localMuteIcon = QIcon(QLatin1String("skin:muted_local.svg"));
	static const QIcon s_selfMuteIcon  = QIcon(QLatin1String("skin:muted_self.svg"));
	static const QIcon s_selfDeafIcon  = QIcon(QLatin1String("skin:deafened_self.svg"));


	std::vector< std::reference_wrapper< const QIcon > > icons;

	if (status.muted) {
		icons.push_back(s_muteIcon);
	}
	if (status.selfMuted) {
		icons.push_back(s_selfMuteIcon);
	}
	if (status.localMuted) {
		icons.push_back(s_localMuteIcon);
	}
	if (status.deafened) {
		icons.push_back(s_deafIcon);
	}
	if (status.selfDeafened) {
		icons.push_back(s_selfDeafIcon);
	}

	if (icons.size() == 0) {
		// No status icons to be shown -> hide the widget
		m_statusIcons->hide();
		m_backgroundWidget->layout()->removeWidget(m_statusIcons);
	} else {
		// Create a Pixmap that'll hold all icons
		const QSize size(m_iconSize * static_cast< int >(icons.size()), m_iconSize);
		QPixmap pixmap(size);
		pixmap.fill(Qt::transparent);

		// Draw the icons to the Pixmap
		QPainter painter(&pixmap);
		for (unsigned int i = 0; i < icons.size(); i++) {
			painter.drawPixmap(static_cast< int >(i) * m_iconSize, 0,
							   icons[i].get().pixmap(QSize(m_iconSize, m_iconSize), QIcon::Normal, QIcon::On));
		}

		m_statusIcons->setPixmap(pixmap);

		if (m_backgroundWidget->layout()->indexOf(m_statusIcons) < 0) {
			// Add the status icons into the scene
			m_statusIcons->show();
			m_backgroundWidget->layout()->addWidget(m_statusIcons);
		}
	}
}



TalkingUIChannelListener::TalkingUIChannelListener(const ClientUser &user, const Channel &channel)
	: TalkingUIEntry(user.uiSession), m_backgroundWidget(new QWidget()),
	  m_backgroundWidgetStyleWrapper(m_backgroundWidget), m_channelID(channel.iId), m_name(user.qsName) {
	// Create background widget and its layout that we'll use to place all other
	// components on
	m_backgroundWidget->setProperty("selected", false);
	QLayout *backgroundLayout = new QHBoxLayout();
	backgroundLayout->setContentsMargins(USER_CONTAINER_HORIZONTAL_MARGIN, USER_CONTAINER_VERTICAL_MARGIN,
										 USER_CONTAINER_HORIZONTAL_MARGIN, USER_CONTAINER_VERTICAL_MARGIN);
	m_backgroundWidget->setLayout(backgroundLayout);
	m_backgroundWidget->setAutoFillBackground(true);


	// Create the label we use to display the icon
	m_icon = new QLabel(m_backgroundWidget);
	m_icon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	int iconSize = QFontMetrics(m_backgroundWidget->font()).height();
	// setIconSize will not only set the size but also the icon itself
	setIconSize(iconSize);

	// Create the label we use to display the user's name
	m_nameLabel                 = new QLabel(m_backgroundWidget);
	const QString displayString = UserModel::createDisplayString(user, true, &channel);
	setDisplayString(displayString);


	backgroundLayout->addWidget(m_icon);
	backgroundLayout->addWidget(m_nameLabel);
}

TalkingUIChannelListener::~TalkingUIChannelListener() {
	m_backgroundWidget->deleteLater();
	m_icon->deleteLater();
	m_nameLabel->deleteLater();
}

EntryType TalkingUIChannelListener::getType() const {
	return EntryType::LISTENER;
}

QWidget *TalkingUIChannelListener::getWidget() {
	return m_backgroundWidget;
}

const QWidget *TalkingUIChannelListener::getWidget() const {
	return m_backgroundWidget;
}

MultiStyleWidgetWrapper &TalkingUIChannelListener::getStylableWidget() {
	return m_backgroundWidgetStyleWrapper;
}

void TalkingUIChannelListener::setIconSize(int size) {
	static const QIcon s_earIcon = QIcon(QLatin1String("skin:ear.svg"));

	m_icon->setPixmap(s_earIcon.pixmap(QSize(size, size), QIcon::Normal, QIcon::On));
}

void TalkingUIChannelListener::setDisplayString(const QString &displayString) {
	m_nameLabel->setText(QString::fromLatin1("<i>%1</i>").arg(displayString));
}

int TalkingUIChannelListener::getAssociatedChannelID() const {
	return static_cast< int >(m_channelID);
}
