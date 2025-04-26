// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TalkingUI.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "ClientUser.h"
#include "MainWindow.h"
#include "TalkingUIComponent.h"
#include "UserModel.h"
#include "widgets/MultiStyleWidgetWrapper.h"
#include "Global.h"

#include <QGroupBox>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPalette>
#include <QScreen>
#include <QTextDocumentFragment>
#include <QVBoxLayout>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

#include <algorithm>
#include <cassert>

TalkingUI::TalkingUI(QWidget *parent) : QWidget(parent), m_containers(), m_currentSelection(nullptr) {
	setupUI();
}

int TalkingUI::findContainer(int associatedChannelID, ContainerType type) const {
	for (std::size_t i = 0; i < m_containers.size(); i++) {
		const std::unique_ptr< TalkingUIContainer > &currentContainer = m_containers[i];

		if (currentContainer->getType() == type && currentContainer->getAssociatedChannelID() == associatedChannelID) {
			return static_cast< int >(i);
		}
	}

	return -1;
}

std::unique_ptr< TalkingUIContainer > TalkingUI::removeContainer(const TalkingUIContainer &container) {
	return removeContainer(container.getAssociatedChannelID(), container.getType());
}

std::unique_ptr< TalkingUIContainer > TalkingUI::removeContainer(int associatedChannelID, ContainerType type) {
	int index = findContainer(associatedChannelID, type);

	std::unique_ptr< TalkingUIContainer > container(nullptr);

	if (index >= 0) {
		// Move the container out of the vector
		container = std::move(m_containers[static_cast< std::size_t >(index)]);
		m_containers.erase(m_containers.begin() + index);

		// If the container is currently selected, clear the selection
		if (isSelected(*container)) {
			setSelection(EmptySelection());
		}
	}

	return container;
}

std::unique_ptr< TalkingUIContainer > TalkingUI::removeIfSuperfluous(const TalkingUIContainer &container) {
	if (container.isEmpty() && !container.isPermanent()) {
		return removeContainer(container);
	}

	return nullptr;
}

struct container_ptr_less {
	bool operator()(const std::unique_ptr< TalkingUIContainer > &first,
					const std::unique_ptr< TalkingUIContainer > &second) {
		return *first < *second;
	}
};

void TalkingUI::sortContainers() {
	// Remove all containers from the UI
	for (auto &currentContainer : m_containers) {
		layout()->removeWidget(currentContainer->getWidget());
	}

	// Sort the containers
	std::sort(m_containers.begin(), m_containers.end(), container_ptr_less());

	// Add them again in the order they appear in the vector
	for (auto &currentContainer : m_containers) {
		layout()->addWidget(currentContainer->getWidget());
	}
}

TalkingUIUser *TalkingUI::findUser(unsigned int userSession) {
	for (auto &currentContainer : m_containers) {
		TalkingUIEntry *entry = currentContainer->get(userSession, EntryType::USER);

		if (entry) {
			// We know that it must be a TalkingUIUser since that is what we searched for
			return static_cast< TalkingUIUser * >(entry);
		}
	}

	return nullptr;
}

void TalkingUI::removeUser(unsigned int userSession) {
	TalkingUIUser *userEntry = findUser(userSession);

	if (userEntry) {
		// If the user that is going to be deleted is currently selected, clear the selection
		if (isSelected(*userEntry)) {
			setSelection(EmptySelection());
		}

		TalkingUIContainer *userContainer = userEntry->getContainer();

		userContainer->removeEntry(userEntry);

		removeIfSuperfluous(*userContainer);

		updateUI();
	}
}

void TalkingUI::addListener(const ClientUser *user, const Channel *channel) {
	TalkingUIChannelListener *existingEntry = findListener(user->uiSession, channel->iId);

	if (!existingEntry) {
		// Only create entry if it doesn't exist yet

		// First make sure the channel exists
		addChannel(channel);

		std::unique_ptr< TalkingUIContainer > &channelContainer = m_containers[static_cast< std::size_t >(
			findContainer(static_cast< int >(channel->iId), ContainerType::CHANNEL))];

		std::unique_ptr< TalkingUIChannelListener > listenerEntry =
			std::make_unique< TalkingUIChannelListener >(*user, *channel);

		channelContainer->addEntry(std::move(listenerEntry));

		sortContainers();
	}
}

TalkingUIChannelListener *TalkingUI::findListener(unsigned int userSession, unsigned int channelID) {
	int channelIndex = findContainer(static_cast< int >(channelID), ContainerType::CHANNEL);

	if (channelIndex >= 0) {
		std::unique_ptr< TalkingUIContainer > &channelContainer =
			m_containers[static_cast< std::size_t >(channelIndex)];

		TalkingUIEntry *entry = channelContainer->get(userSession, EntryType::LISTENER);

		if (entry) {
			return static_cast< TalkingUIChannelListener * >(entry);
		}
	}

	return nullptr;
}

void TalkingUI::removeListener(unsigned int userSession, unsigned int channelID) {
	TalkingUIChannelListener *listenerEntry = findListener(userSession, channelID);

	if (listenerEntry) {
		// If the user that is going to be deleted is currently selected, clear the selection
		if (isSelected(*listenerEntry)) {
			setSelection(EmptySelection());
		}

		TalkingUIContainer *userContainer = listenerEntry->getContainer();

		userContainer->removeEntry(listenerEntry);

		removeIfSuperfluous(*userContainer);

		updateUI();
	}
}

void TalkingUI::removeAllListeners() {
	// Find all listener entries
	std::vector< TalkingUIEntry * > entriesToBeRemoved;
	for (auto &currentContainer : m_containers) {
		for (auto &currentEntry : currentContainer->getEntries()) {
			if (currentEntry->getType() == EntryType::LISTENER) {
				entriesToBeRemoved.push_back(currentEntry.get());
			}
		}
	}

	// remove the individual entries
	for (auto currentEntry : entriesToBeRemoved) {
		TalkingUIContainer *container = currentEntry->getContainer();

		container->removeEntry(currentEntry);

		removeIfSuperfluous(*container);
	}

	// if we removed something, update the UI
	if (entriesToBeRemoved.size() > 0) {
		updateUI();
	}
}

void TalkingUI::setupUI() {
	QVBoxLayout *layout = new QVBoxLayout;

	setLayout(layout);

	setWindowTitle(QObject::tr("Talking UI"));

	setAttribute(Qt::WA_ShowWithoutActivating);
	setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

	// Hide the "?" (context help) button in the title bar of the widget as we don't want
	// that due to it taking valuable screen space so that the title can't be displayed
	// properly and as the TalkingUI doesn't provide context help anyways, this is not a big loss.
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(Global::get().mw->qtvUsers->selectionModel(), &QItemSelectionModel::currentChanged, this,
			&TalkingUI::on_mainWindowSelectionChanged);
}

void TalkingUI::setFontSize(MultiStyleWidgetWrapper &widgetWrapper) {
	const double fontFactor  = Global::get().s.iTalkingUI_RelativeFontSize / 100.0;
	const int origLineHeight = QFontMetrics(font()).height();

	if (font().pixelSize() >= 0) {
		// font specified in pixels
		uint32_t pixelSize = static_cast< uint32_t >(std::max(fontFactor * font().pixelSize(), 1.0));
		widgetWrapper.setFontSize(pixelSize, true);
	} else {
		// font specified in points
		uint32_t pointSize = static_cast< uint32_t >(std::max(fontFactor * font().pointSize(), 1.0));
		widgetWrapper.setFontSize(pointSize, false);
	}

	m_currentLineHeight = static_cast< int >(std::max(origLineHeight * fontFactor, 1.0));
}

void TalkingUI::updateStatusIcons(const ClientUser *user) {
	TalkingUIUser *userEntry = findUser(user->uiSession);

	if (!userEntry) {
		return;
	}

	TalkingUIUser::UserStatus status;
	status.muted        = user->bMute;
	status.selfMuted    = user->bSelfMute;
	status.localMuted   = user->bLocalMute;
	status.deafened     = user->bDeaf;
	status.selfDeafened = user->bSelfDeaf;

	userEntry->setStatus(status);
}

void TalkingUI::hideUser(unsigned int session) {
	removeUser(session);

	updateUI();
}

QString createChannelName(const Channel *chan, bool abbreviateName, int minPrefixChars, int minPostfixChars,
						  int idealMaxChars, int parentLevel, const QString &separator,
						  const QString &abbreviationIndicator, bool abbreviateCurrentChannel) {
	if (!abbreviateName) {
		return chan->qsName;
	}

	// Assemble list of relevant channel names (representing the channel hierarchy
	QStringList nameList;
	do {
		nameList << chan->qsName;

		chan = chan->cParent;
	} while (chan && nameList.size() < (parentLevel + 1));

	const bool reachedRoot = !chan;

	// We also want to abbreviate names that nominally have the same amount of characters before and
	// after abbreviation. However as we're typically not using mono-spaced fonts, the abbreviation
	// indicator might still occupy less space than the original text.
	const auto abbreviableSize = minPrefixChars + minPostfixChars + abbreviationIndicator.size();

	// Iterate over all names and check how many of them could be abbreviated
	auto totalCharCount = reachedRoot ? separator.size() : 0;
	for (int i = 0; i < nameList.size(); i++) {
		totalCharCount += nameList[i].size();

		if (i + 1 < nameList.size()) {
			// Account for the separator's size as well
			totalCharCount += separator.size();
		}
	}

	QString groupName = reachedRoot ? separator : QString();

	for (decltype(nameList.size()) i = nameList.size() - 1; i >= 0; i--) {
		if (totalCharCount > idealMaxChars && nameList[i].size() >= abbreviableSize
			&& (abbreviateCurrentChannel || i != 0)) {
			// Abbreviate the names as much as possible
			groupName += nameList[i].left(minPrefixChars) + abbreviationIndicator + nameList[i].right(minPostfixChars);
		} else {
			groupName += nameList[i];
		}

		if (i != 0) {
			groupName += separator;
		}
	}

	return groupName;
}

void TalkingUI::addChannel(const Channel *channel) {
	if (findContainer(static_cast< int >(channel->iId), ContainerType::CHANNEL) < 0) {
		// Create a QGroupBox for this channel
		const QString channelName = createChannelName(
			channel, Global::get().s.bTalkingUI_AbbreviateChannelNames, Global::get().s.iTalkingUI_PrefixCharCount,
			Global::get().s.iTalkingUI_PostfixCharCount, Global::get().s.iTalkingUI_MaxChannelNameLength,
			Global::get().s.iTalkingUI_ChannelHierarchyDepth, Global::get().s.qsHierarchyChannelSeparator,
			Global::get().s.qsTalkingUI_AbbreviationReplacement, Global::get().s.bTalkingUI_AbbreviateCurrentChannel);

		std::unique_ptr< TalkingUIChannel > channelContainer =
			std::make_unique< TalkingUIChannel >(channel->iId, channelName, *this);

		QWidget *channelWidget = channelContainer->getWidget();

		setFontSize(channelContainer->getStylableWidget());

		layout()->addWidget(channelWidget);

		m_containers.push_back(std::move(channelContainer));
	}
}

TalkingUIUser *TalkingUI::findOrAddUser(const ClientUser *user) {
	TalkingUIUser *oldUserEntry = findUser(user->uiSession);
	bool nameMatches            = true;

	if (oldUserEntry) {
		// We also verify whether the name for that user matches up (if it is contained in m_entries) in case
		// a user didn't get removed from the map but its ID got reused by a new client.

		nameMatches = oldUserEntry->getName() == user->qsName;

		if (!nameMatches) {
			// Hide and remove the stale user
			hideUser(user->uiSession);

			// reset pointer
			oldUserEntry = nullptr;
		}
	}

	// Make sure that the user's channel exists in this UI.
	// Note that this has to be done **after** the name check above as
	// the user might have been renamed in which case the abovementioned
	// code block removes the old user entry. If that user was the only
	// client in its channel, the channel gets removed as well. However
	// the code below expects the user's channel to exist.
	addChannel(user->cChannel);

	if (!oldUserEntry || !nameMatches) {
		// Create an entry for this user
		bool isSelf = Global::get().uiSession == user->uiSession;

		int channelIndex = findContainer(static_cast< int >(user->cChannel->iId), ContainerType::CHANNEL);
		if (channelIndex < 0) {
			qCritical("TalkingUI::findOrAddUser User's channel does not exist!");
			return nullptr;
		}
		std::unique_ptr< TalkingUIContainer > &channelContainer =
			m_containers[static_cast< std::size_t >(channelIndex)];
		if (!channelContainer) {
			qCritical("TalkingUI::findOrAddUser requesting unknown channel!");
			return nullptr;
		}

		std::unique_ptr< TalkingUIUser > userEntry = std::make_unique< TalkingUIUser >(*user);
		TalkingUIUser *newUserEntry                = userEntry.get();

		// * 1000 as the setting is in seconds whereas the timer expects milliseconds
		userEntry->setLifeTime(static_cast< unsigned int >(Global::get().s.iTalkingUI_SilentUserLifeTime) * 1000);

		bool isLocalUserAndLocalUserAlwaysVisible = isSelf && Global::get().s.bTalkingUI_LocalUserStaysVisible;
		bool usersAlwaysVisible                   = Global::get().s.talkingUI_UsersAlwaysVisible;
		bool isUserAlwaysVisible                  = usersAlwaysVisible || isLocalUserAndLocalUserAlwaysVisible;
		userEntry->restrictLifetime(!isUserAlwaysVisible);

		userEntry->setPriority(isSelf ? EntryPriority::HIGH : EntryPriority::DEFAULT);

		QObject::connect(user, &ClientUser::localVolumeAdjustmentsChanged, this,
						 &TalkingUI::on_userLocalVolumeAdjustmentsChanged);

		// If this user is currently selected, mark him/her as such
		if (Global::get().mw && Global::get().mw->pmModel && Global::get().mw->pmModel->getSelectedUser() == user) {
			setSelection(UserSelection(userEntry->getWidget(), userEntry->getAssociatedUserSession()));
		}

		// As the font size of the newly created widget did not adapt to any StyleSheet it might have inherited
		// from its parents (the channel box), the size the talking icon is initialized to in the entry's
		// constructor is incorrect. Therefore we have to explicitly update it here.
		userEntry->setIconSize(m_currentLineHeight);

		// Actually add the user to the respective channel
		channelContainer->addEntry(std::move(userEntry));

		sortContainers();

		return newUserEntry;
	} else {
		return oldUserEntry;
	}
}

void TalkingUI::moveUserToChannel(unsigned int userSession, unsigned int channelID) {
	int targetChanIndex = findContainer(static_cast< int >(channelID), ContainerType::CHANNEL);

	if (targetChanIndex < 0) {
		qCritical("TalkingUI::moveUserToChannel Can't find channel for speaker");
		return;
	}

	std::unique_ptr< TalkingUIContainer > &targetChannel = m_containers[static_cast< std::size_t >(targetChanIndex)];

	if (targetChannel->contains(userSession, EntryType::USER)) {
		// The given user is already in the target channel - nothing to do
		return;
	}

	// Iterate all containers in order to find the one the user is currently in
	TalkingUIUser *userEntry = findUser(userSession);

	if (userEntry) {
		TalkingUIContainer *oldContainer = userEntry->getContainer();

		targetChannel->addEntry(oldContainer->removeEntry(userEntry));

		removeIfSuperfluous(*oldContainer);

		sortContainers();
	} else {
		qCritical("TalkingUI::moveUserToChannel Unable to locate user");
		return;
	}

	updateUI();
}

void TalkingUI::updateUI() {
	// Use timer to execute this after all other events have been processed
	QTimer::singleShot(0, [this]() { adjustSize(); });
}

void TalkingUI::setSelection(const TalkingUISelection &selection) {
	if (dynamic_cast< const EmptySelection * >(&selection)) {
		// The selection is set to an empty selection
		if (m_currentSelection) {
			// There currently is a selection -> clear and remove it
			m_currentSelection->discard();
			m_currentSelection.reset();
		}
	} else {
		if (m_currentSelection) {
			if (selection == *m_currentSelection) {
				// Selection hasn't actually changed
				return;
			}

			// Discard old selection (it'll get deleted on re-assignment below)
			m_currentSelection->discard();
		}

		// Use the new selection (which at this point we know is not the empty selection)
		m_currentSelection = selection.cloneToHeap();

		m_currentSelection->apply();
		m_currentSelection->syncToMainWindow();
	}
}

bool TalkingUI::isSelected(const TalkingUIComponent &component) const {
	if (m_currentSelection) {
		return *m_currentSelection == component.getWidget();
	}

	return false;
}

void TalkingUI::mousePressEvent(QMouseEvent *event) {
	bool foundTarget = false;

	for (auto &currentContainer : m_containers) {
		const QRect containerArea(currentContainer->getWidget()->mapToGlobal(QPoint(0, 0)),
								  currentContainer->getWidget()->size());
		if (containerArea.contains(event->globalPosition().toPoint())) {
			for (auto &currentEntry : currentContainer->getEntries()) {
				const QRect entryArea(currentEntry->getWidget()->mapToGlobal(QPoint(0, 0)),
									  currentEntry->getWidget()->size());
				if (entryArea.contains(event->globalPosition().toPoint())) {
					switch (currentEntry->getType()) {
						case EntryType::USER:
							setSelection(
								UserSelection(currentEntry->getWidget(), currentEntry->getAssociatedUserSession()));
							break;
						case EntryType::LISTENER:
							TalkingUIChannelListener *listenerEntry =
								static_cast< TalkingUIChannelListener * >(currentEntry.get());
							setSelection(ListenerSelection(
								listenerEntry->getWidget(), listenerEntry->getAssociatedUserSession(),
								static_cast< unsigned int >(listenerEntry->getAssociatedChannelID())));
							break;
					}

					foundTarget = true;

					break;
				}
			}

			if (!foundTarget) {
				// Select channel itself
				setSelection(ChannelSelection(currentContainer->getWidget(),
											  static_cast< unsigned int >(currentContainer->getAssociatedChannelID())));

				foundTarget = true;
			}

			break;
		}
	}

	if (foundTarget) {
		if (event->button() == Qt::RightButton && Global::get().mw) {
			// If an entry is selected and the right mouse button was clicked, we pretend as if the user had clicked on
			// the client in the MainWindow. For this to work we map the global mouse position to the local coordinate
			// system of the UserView in the MainWindow. The function will use some internal logic to determine the user
			// to invoke the context menu on but if that fails (which in this case it will), it'll fall back to the
			// currently selected item. This item we have updated to the correct one with the setSelection() call above
			// resulting in the proper context menu being shown at the position of the mouse which in this case is in
			// the TalkingUI.
			QMetaObject::invokeMethod(
				Global::get().mw, "on_qtvUsers_customContextMenuRequested", Qt::QueuedConnection,
				Q_ARG(QPoint, Global::get().mw->qtvUsers->mapFromGlobal(event->globalPosition().toPoint())),
				Q_ARG(bool, false));
		}
	} else {
		// Clear selection
		setSelection(EmptySelection());
	}

	updateUI();
}

void TalkingUI::setVisible(bool visible) {
	if (visible) {
		adjustSize();
	}

	QWidget::setVisible(visible);
}

QSize TalkingUI::sizeHint() const {
	// Prefer to occupy at least 10% of the screen's size
	// This aims to be a good compromise between not being in the way and not
	// being too small to being handled properly.
	int width = static_cast< int >(QGuiApplication::screens()[0]->availableSize().width() * 0.1);

	return { width, 0 };
}

QSize TalkingUI::minimumSizeHint() const {
	return { 0, 0 };
}

void TalkingUI::on_talkingStateChanged() {
	ClientUser *user = qobject_cast< ClientUser * >(sender());

	if (!user) {
		// If the user that caused this event doesn't exist anymore, it means that it
		// got deleted in the meantime. This in turn means that the user disconnected
		// from the server. In that case it has been removed via on_clientDisconnected
		// already (or shortly will be), so it is safe to silently ignore this case
		// here.
		return;
	}

	if (!user->cChannel) {
		// If the user doesn't have an associated channel, something's either wrong
		// or that user has just disconnected. In either way, we want to make sure
		// that this user won't stick around in the UI.
		hideUser(user->uiSession);
		return;
	}

	TalkingUIUser *userEntry = findOrAddUser(user);

	if (userEntry) {
		userEntry->setTalkingState(user->tsState);
	}

	updateUI();
}

void TalkingUI::on_mainWindowSelectionChanged(const QModelIndex &current, const QModelIndex &previous) {
	Q_UNUSED(previous);

	// Sync the selection in the MainWindow to the TalkingUI
	if (Global::get().mw && Global::get().mw->pmModel) {
		bool clearSelection = true;

		const ClientUser *user = Global::get().mw->pmModel->getUser(current);
		const Channel *channel = Global::get().mw->pmModel->getChannel(current);

		if (Global::get().mw->pmModel->isChannelListener(current)) {
			TalkingUIChannelListener *listenerEntry = findListener(user->uiSession, channel->iId);

			if (listenerEntry) {
				setSelection(ListenerSelection(listenerEntry->getWidget(), user->uiSession, channel->iId));

				clearSelection = false;
			}
		} else {
			if (user) {
				TalkingUIUser *userEntry = findUser(user->uiSession);

				if (userEntry) {
					// Only select the user if there is an actual entry for it in the TalkingUI
					setSelection(UserSelection(userEntry->getWidget(), userEntry->getAssociatedUserSession()));

					clearSelection = false;
				}
			} else if (!user && channel) {
				// if user != nullptr, the selection is actually a user, but UserModel::getChannel still returns
				// the channel of that user. However we only want to select the channel if the user has indeed
				// selected the channel and not just one of the users in it.
				int index = findContainer(static_cast< int >(channel->iId), ContainerType::CHANNEL);

				if (index >= 0) {
					// Only select the channel if there is present in the TalkingUI
					std::unique_ptr< TalkingUIContainer > &targetContainer =
						m_containers[static_cast< std::size_t >(index)];

					setSelection(
						ChannelSelection(targetContainer->getWidget(),
										 static_cast< unsigned int >(targetContainer->getAssociatedChannelID())));

					clearSelection = false;
				}
			}
		}

		if (clearSelection) {
			setSelection(EmptySelection());
		}
	}
}

void TalkingUI::on_serverSynchronized() {
	ClientUser *self = ClientUser::get(Global::get().uiSession);

	assert(self);

	if (Global::get().s.bTalkingUI_LocalUserStaysVisible) {
		// According to the settings the local user should always be visible and as we
		// can't count on it to change its talking state right after it has connected to
		// a server, we have to add it manually.
		findOrAddUser(self);
	}

	// According to the settings the all users should always be visible and as we
	// can't count on users to change their talking state right after our user has connected
	// to a server, we have to add them manually.
	if (Global::get().s.talkingUI_UsersAlwaysVisible) {
		for (auto &user : ClientUser::c_qmUsers) {
			findOrAddUser(user);
		}
	}
	// The client may have received add listener messages for the user before the
	// sync was complete. So we do this to ensure that they appear in the
	// TalkingUI. Removing all listeners is probably not necessary but could
	// prevent duplicates appearing in the case of a race.
	removeAllListeners();
	if (Global::get().s.bTalkingUI_ShowLocalListeners) {
		if (self) {
			const QSet< unsigned int > channels =
				Global::get().channelListenerManager->getListenedChannelsForUser(self->uiSession);

			for (unsigned int currentChannelID : channels) {
				const Channel *channel = Channel::get(currentChannelID);

				if (channel) {
					addListener(self, channel);
				}
			}
		}
	}
}

void TalkingUI::on_serverDisconnected() {
	setSelection(EmptySelection());

	// If we disconnect from a server, we have to clear all our users, channels, and so on
	// Since all entries are owned by their respective containers, we only have to delete
	// all containers. These in turn are managed by smart-pointers and therefore it is
	// enough to let those go out of scope.
	m_containers.clear();

	updateUI();
}

void TalkingUI::on_channelChanged(QObject *obj) {
	// According to this function's doc, the passed object must be of type ClientUser
	ClientUser *user = static_cast< ClientUser * >(obj);

	if (!user) {
		return;
	}

	TalkingUIUser *userEntry = findUser(user->uiSession);

	if (userEntry) {
		// The user is visible, so we call moveUserToChannel in order to update
		// the channel this particular user is being displayed in.
		// But first we have to make sure there actually exists and entry for
		// the new channel.
		addChannel(user->cChannel);
		moveUserToChannel(user->uiSession, user->cChannel->iId);
	}
}

void TalkingUI::on_settingsChanged() {
	// The settings might have affected the way we have to display the channel names
	// thus we'll update them just in case
	for (auto &currentContainer : m_containers) {
		// The font size might have changed as well -> update it
		// By the hierarchy in the UI the font-size should propagate to all
		// sub-elements (entries) as well.
		setFontSize(currentContainer->getStylableWidget());

		if (currentContainer->getType() != ContainerType::CHANNEL) {
			continue;
		}

		TalkingUIChannel *channelContainer = static_cast< TalkingUIChannel * >(currentContainer.get());

		const Channel *channel = Channel::get(static_cast< unsigned int >(currentContainer->getAssociatedChannelID()));

		if (channel) {
			// Update
			channelContainer->setName(createChannelName(
				channel, Global::get().s.bTalkingUI_AbbreviateChannelNames, Global::get().s.iTalkingUI_PrefixCharCount,
				Global::get().s.iTalkingUI_PostfixCharCount, Global::get().s.iTalkingUI_MaxChannelNameLength,
				Global::get().s.iTalkingUI_ChannelHierarchyDepth, Global::get().s.qsHierarchyChannelSeparator,
				Global::get().s.qsTalkingUI_AbbreviationReplacement,
				Global::get().s.bTalkingUI_AbbreviateCurrentChannel));
		} else {
			qCritical("TalkingUI: Can't find channel for stored ID");
		}
	}

	int silentUserLifeTime  = Global::get().s.iTalkingUI_SilentUserLifeTime;
	bool usersAlwaysVisible = Global::get().s.talkingUI_UsersAlwaysVisible;
	// If the font has changed, we have to update the icon size as well
	for (auto &currentContainer : m_containers) {
		for (auto &currentEntry : currentContainer->getEntries()) {
			currentEntry->setIconSize(m_currentLineHeight);

			if (currentEntry->getType() == EntryType::USER) {
				TalkingUIUser *userEntry = static_cast< TalkingUIUser * >(currentEntry.get());

				// The time that a silent user may stick around might have changed as well
				// * 1000 as the setting is in seconds whereas the timer expects milliseconds
				userEntry->setLifeTime(static_cast< unsigned int >(silentUserLifeTime) * 1000);
				userEntry->restrictLifetime(!usersAlwaysVisible);
			}
		}
	}

	// According to the settings the all users should always be visible and as we
	// can't count on users to change their talking state right after settings where changed
	// we have to add them manually.
	if (usersAlwaysVisible) {
		for (auto &user : ClientUser::c_qmUsers) {
			findOrAddUser(user);
		}
	}

	const ClientUser *self = ClientUser::get(Global::get().uiSession);

	// Whether or not the current user should always be displayed might also have changed,
	// so we'll have to update that as well.
	TalkingUIUser *localUserEntry = findUser(Global::get().uiSession);
	if (localUserEntry) {
		bool localUserAlwaysVisible = usersAlwaysVisible || Global::get().s.bTalkingUI_LocalUserStaysVisible;
		localUserEntry->restrictLifetime(!localUserAlwaysVisible);
	} else {
		if (self && (Global::get().s.bTalkingUI_LocalUserStaysVisible || usersAlwaysVisible)) {
			// Add the local user as it is requested to be displayed
			findOrAddUser(self);
		}
	}


	// Furthermore whether or not to display the local user's listeners might have changed -> clear all
	// listeners from the TalkingUI and add them again if appropriate
	removeAllListeners();
	if (Global::get().s.bTalkingUI_ShowLocalListeners) {
		if (self) {
			const QSet< unsigned int > channels =
				Global::get().channelListenerManager->getListenedChannelsForUser(self->uiSession);

			for (unsigned int currentChannelID : channels) {
				const Channel *channel = Channel::get(currentChannelID);

				if (channel) {
					addListener(self, channel);
				}
			}
		}
	}
}

void TalkingUI::on_clientDisconnected(unsigned int userSession) {
	removeUser(userSession);
}

void TalkingUI::on_muteDeafStateChanged() {
	ClientUser *user = qobject_cast< ClientUser * >(sender());

	if (user) {
		// Update icons for local user only
		updateStatusIcons(user);
	}
}

void TalkingUI::on_userLocalVolumeAdjustmentsChanged(float, float) {
	ClientUser *user = qobject_cast< ClientUser * >(sender());

	if (user) {
		TalkingUIUser *userEntry = findUser(user->uiSession);
		if (userEntry) {
			userEntry->setDisplayString(UserModel::createDisplayString(*user, false, nullptr));
		}
	}
}

void TalkingUI::on_channelListenerAdded(const ClientUser *user, const Channel *channel) {
	if (user->uiSession == Global::get().uiSession && Global::get().s.bTalkingUI_ShowLocalListeners) {
		addListener(user, channel);
	}
}

void TalkingUI::on_channelListenerRemoved(const ClientUser *user, const Channel *channel) {
	removeListener(user->uiSession, channel->iId);
}

void TalkingUI::on_channelListenerLocalVolumeAdjustmentChanged(unsigned int channelID, float, float) {
	TalkingUIChannelListener *listenerEntry = findListener(Global::get().uiSession, channelID);

	const Channel *channel = Channel::get(channelID);
	const ClientUser *self = ClientUser::get(Global::get().uiSession);

	if (listenerEntry && channel && self) {
		listenerEntry->setDisplayString(UserModel::createDisplayString(*self, true, channel));
	}
}
