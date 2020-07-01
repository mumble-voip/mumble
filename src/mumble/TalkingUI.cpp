// Copyright 2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TalkingUI.h"
#include "ClientUser.h"
#include "Channel.h"
#include "MainWindow.h"
#include "UserModel.h"

#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGuiApplication>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QScreen>
#include <QMouseEvent>
#include <QPalette>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QtCore/QStringList>
#include <QTextDocumentFragment>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

#include <algorithm>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

TalkingUI::TalkingUI(QWidget *parent)
	: QWidget(parent),
	  m_entries(),
	  m_channels(),
	  m_timers(),
	  m_currentSelection(nullptr),
	  m_talkingIcon(QIcon(QLatin1String("skin:talking_on.svg"))),
	  m_passiveIcon(QIcon(QLatin1String("skin:talking_off.svg"))),
	  m_shoutingIcon(QIcon(QLatin1String("skin:talking_alt.svg"))),
	  m_whisperingIcon(QIcon(QLatin1String("skin:talking_whisper.svg"))),
	  m_muteIcon(QIcon(QLatin1String("skin:muted_server.svg"))),
	  m_deafIcon(QIcon(QLatin1String("skin:deafened_server.svg"))),
	  m_localMuteIcon(QIcon(QLatin1String("skin:muted_local.svg"))),
	  m_selfMuteIcon(QIcon(QLatin1String("skin:muted_self.svg"))),
	  m_selfDeafIcon(QIcon(QLatin1String("skin:deafened_self.svg"))) {

	setupUI();
}

void TalkingUI::setupUI() {
	QVBoxLayout *layout = new QVBoxLayout;

	setLayout(layout);

	setWindowTitle(QObject::tr("Talking UI"));

	setAttribute(Qt::WA_ShowWithoutActivating);
	setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

	connect(g.mw->qtvUsers->selectionModel(), &QItemSelectionModel::currentChanged, this, &TalkingUI::on_mainWindowSelectionChanged);
}

void TalkingUI::setFontSize(QWidget *widget) {
	const double fontFactor = g.s.iTalkingUI_RelativeFontSize / 100.0;
	
	// We have to do this in a complicated way as Qt is very stubborn when it
	// comes to manipulating fonts.
	// We have to use stylesheets because this seems to be the only way Qt will
	// actually change the font size (setFont has no effect). However the font size
	// won't update the moment the stylesheet is applied, so we have to copy the font
	// of the widget, set the size and use that to calculate the line height for that
	// particular font (needed to size the icons appropriately).
	QFont newFont = widget->font();
	if (font().pixelSize() >= 0) {
		// font specified in pixels
		widget->setStyleSheet(QString::fromLatin1("font-size: %1px;").arg(
					static_cast<int>(std::max(fontFactor * font().pixelSize(), 1.0))));
		newFont.setPixelSize(std::max(fontFactor * font().pixelSize(), 1.0));
	} else {
		// font specified in points
		widget->setStyleSheet(QString::fromLatin1("font-size: %1pt;").arg(
					static_cast<int>(std::max(fontFactor * font().pointSize(), 1.0))));
		newFont.setPointSize(std::max(fontFactor * font().pointSize(), 1.0));
	}

	m_currentLineHeight = QFontMetrics(newFont).height();
}

void TalkingUI::setTalkingIcon(Entry &entry) const {
	const QIcon *icon = nullptr;
	switch (entry.talkingState) {
		case Settings::Talking:
			icon = &m_talkingIcon;
			break;
		case Settings::Whispering:
			icon = &m_whisperingIcon;
			break;
		case Settings::Shouting:
			icon = &m_shoutingIcon;
			break;
		default:
			icon = &m_passiveIcon;
			break;
	}

	entry.talkingIcon->setPixmap(icon->pixmap(QSize(m_currentLineHeight, m_currentLineHeight), QIcon::Normal, QIcon::On));
}

void TalkingUI::updateStatusIcons(const ClientUser *user) {
	if (!m_entries.contains(user->uiSession)) {
		return;
	}

	Entry &userEntry = m_entries[user->uiSession];

	const bool needsIcons = user->bMute || user->bDeaf || user->bLocalMute || user->bSelfMute || user->bSelfDeaf;

	if (needsIcons) {
		if (userEntry.statusIcons) {
			// Make sure the icons are visible
			userEntry.statusIcons->show();
			userEntry.background->layout()->addWidget(userEntry.statusIcons);
		} else {
			// Create the icon's label
			userEntry.statusIcons = new QLabel(userEntry.background);

			userEntry.statusIcons->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

			// Uncomment the line below if you want to debug the icons
			// userEntry.statusIcons->setStyleSheet(QLatin1String("border: 1px solid black;"));

			userEntry.background->layout()->addWidget(userEntry.statusIcons);
		}

		// Now populate with icons as needed
		QVector<QIcon *> activeIcons;

		if (user->bMute) {
			// Add mute-icon
			activeIcons << &m_muteIcon;
		}
		if (user->bSelfMute) {
			// Add self-mute-icon
			activeIcons << &m_selfMuteIcon;
		}
		if (user->bLocalMute) {
			// Add local-mute-icon
			activeIcons << &m_localMuteIcon;
		}
		if (user->bDeaf) {
			// Add deaf-icon
			activeIcons << &m_deafIcon;
		}
		if (user->bSelfDeaf) {
			// Add self-deaf-icon
			activeIcons << &m_selfDeafIcon;
		}

		// If we don't have icons to show, this branch of the if-statement shouldn't even have been chosen
		Q_ASSERT(activeIcons.size() > 0);

		// Create a Pixmap that'll hold all icons
		const QSize size(m_currentLineHeight * activeIcons.size(), m_currentLineHeight);
		QPixmap pixmap(size);
		pixmap.fill(Qt::transparent);
		
		// Draw the icons to the Pixmap
		QPainter painter(&pixmap);
		for (int i = 0; i < activeIcons.size(); i++) {
			painter.drawPixmap(i * m_currentLineHeight, 0, activeIcons[i]->pixmap(QSize(m_currentLineHeight, m_currentLineHeight),
						QIcon::Normal, QIcon::On));
		}

		userEntry.statusIcons->setPixmap(pixmap);
	} else {
		if (userEntry.statusIcons) {
			// The user has an icon container that is no longer needed -> remove it from the layout
			userEntry.background->layout()->removeWidget(userEntry.statusIcons);	
			userEntry.statusIcons->hide();
		}
	}
}

void TalkingUI::hideUser(unsigned int session) {
	QHash<unsigned int, Entry>::iterator iter = m_entries.find(session);
	if (iter == m_entries.end()) {
		qWarning("TalkingUI::hideUser: Session ID not found");
		return;
	}
	
	Entry &entry = iter.value();

	entry.background->hide();

	// In principle the user could have changed channel since it has been added to
	// the UI, so we'll iterate over all GroupBoxes and remove the labels from every
	// single one of them.
	QHashIterator<int, QGroupBox *> it(m_channels);

	while (it.hasNext()) {
		it.next();

		QGroupBox *channelBox = it.value();

		channelBox->layout()->removeWidget(entry.background);

		if (channelBox->layout()->count() == 0) {
			// There are no active speakers in this channel anymore
			channelBox->hide();
			layout()->removeWidget(channelBox);
		}
	}

	updateUI();
}

QString createChannelName(const Channel *chan, bool abbreviateName, int minPrefixChars, int minPostfixChars, int idealMaxChars, int parentLevel,
		const QString &separator, const QString &abbreviationIndicator, bool abbreviateCurrentChannel) {
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
	const int abbreviableSize = minPrefixChars + minPostfixChars + abbreviationIndicator.size();

	// Iterate over all names and check how many of them could be abbreviated
	int totalCharCount = reachedRoot ? separator.size() : 0;
	for (int i = 0; i < nameList.size(); i++) {
		totalCharCount += nameList[i].size();

		if (i + 1 < nameList.size()) {
			// Account for the separator's size as well
			totalCharCount += separator.size();
		}
	}

	QString groupName = reachedRoot ? separator : QString();

	for (int i = nameList.size() - 1; i >= 0; i--) {
		if (totalCharCount > idealMaxChars && nameList[i].size() >= abbreviableSize && (abbreviateCurrentChannel || i != 0)) {
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
	if (!m_channels.contains(channel->iId)) {
		// Create a QGroupBox for this channel
		const QString channelName = createChannelName(channel, g.s.bTalkingUI_AbbreviateChannelNames, g.s.iTalkingUI_PrefixCharCount,
				g.s.iTalkingUI_PostfixCharCount, g.s.iTalkingUI_MaxChannelNameLength, g.s.iTalkingUI_ChannelHierarchyDepth,
				g.s.qsTalkingUI_ChannelSeparator, g.s.qsTalkingUI_AbbreviationReplacement, g.s.bTalkingUI_AbbreviateCurrentChannel);

		QGroupBox *box = new QGroupBox(channelName, this);
		QVBoxLayout *layout = new QVBoxLayout();
		layout->setContentsMargins(0, 10, 0, 0);
		box->setLayout(layout);

		setFontSize(box);

		m_channels.insert(channel->iId, box);
	}
}

void TalkingUI::addUser(const ClientUser *user) {
	// In a first step, it has to be made sure that the user's channel
	// exists in this UI.
	addChannel(user->cChannel);

	bool userContained = m_entries.contains(user->uiSession);
	bool nameMatches = true;

	if (userContained) {
		// We also verify whether the name for that user matches up (if it is contained in m_entries) in case
		// a user didn't get removed from the map but its ID got reused by a new client.

		// Strip HTML from the displayed name
		const QString name = QTextDocumentFragment::fromHtml(m_entries[user->uiSession].name->text()).toPlainText();

		nameMatches = name == user->qsName;

		if (!nameMatches) {
			// Hide the stale user
			hideUser(user->uiSession);
			// Remove the old user
			m_entries.remove(user->uiSession);
		}
	}

	if (!userContained || !nameMatches) {
		bool isSelf = g.uiSession == user->uiSession;
		// Create an Entry for this user (alongside the respective labels)
		// We initially set the labels to not be visible, so that we'll
		// enter the code-block further down.

		QGroupBox *channelBox = m_channels.value(user->cChannel->iId);
		if (!channelBox) {
			qCritical("TalkingUI::addUser requesting unknown channel!");
			return;
		}

		QWidget *background = new QWidget(channelBox);
		background->setProperty("selected", false);
		QLayout *backgroundLayout = new QHBoxLayout();
		backgroundLayout->setContentsMargins(2, 3, 2, 3);
		background->setLayout(backgroundLayout);
		background->setAutoFillBackground(true);
		background->hide();

		background->setProperty("userName", user->qsName);

		QLabel *name = new QLabel(background);
		if (isSelf) {
			// Indicate self by bold name
			name->setText(QString::fromLatin1("<b>%1</b>").arg(user->qsName));
		} else {
			name->setText(user->qsName);
		}

		QLabel *icon = new QLabel(background);
		icon->setAlignment(Qt::AlignCenter);
		icon->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

		// As a default use the passive state
		Entry entry = {icon, name, background, nullptr, user->uiSession, Settings::Passive};
		setTalkingIcon(entry);
		m_entries.insert(user->uiSession, entry);


		backgroundLayout->addWidget(icon);
		backgroundLayout->addWidget(name);

		// Also create a timer for this specific user
		QTimer *timer = new QTimer(this);
		timer->setSingleShot(true);
		// * 1000 as the setting is in seconds whereas the timer expects milliseconds
		timer->setInterval(g.s.iTalkingUI_SilentUserLifeTime * 1000);
		const unsigned int session = user->uiSession;
		QObject::connect(timer, &QTimer::timeout, [this, session](){
			hideUser(session);
		});

		m_timers.insert(user->uiSession, timer);

		// If this user is currently selected, mark him/her as such
		if (g.mw && g.mw->pmModel && g.mw->pmModel->getSelectedUser() == user) {
			if (m_entries.contains(user->uiSession)) {
				setSelection(&m_entries[user->uiSession]);
			} else {
				setSelection(nullptr);
				qCritical("TalkingUI::addUser requested entry for unknown user!");
			}
		}
	}
}

void TalkingUI::ensureVisible(unsigned int userSession, int channelID) {
	if (!m_entries.contains(userSession)) {
		qCritical("TalkingUI::ensureVisible Requested visibility for unknown user");
		return;
	}
	Entry entry = m_entries[userSession];

	// Get the box we'll want to have the user in (representing the current
	// channel of the user)
	QGroupBox *channelBox = m_channels.value(channelID);
	if (!channelBox) {
		qCritical("TalkingUI::ensureVisible Can't find channel for speaker");
		return;
	}

	// Check if the user has changed channel and handle this separately in case
	// the user is currently still displayed as being in that channel
	bool changedChannel = false;
	if (channelBox->layout()->indexOf(entry.background) < 0) {
		changedChannel = true;

		hideUser(userSession);
	}

	ClientUser *self = ClientUser::get(g.uiSession);
	// In case the locals user (self) has changed channel but this method here was called before on_channelChanged
	// has been called (e.g. triggered via on_talkingStateChanged of another user), we have to make sure that the
	// channel of the local user actually exists in the TalkingUI. This is necessary because the channel update of
	// a user and the call to on_channelChanged happen asynchronously (the latter runs through the event loop and
	// thus typically (significantly) after the channel has been updated already.
	addChannel(self->cChannel);
	QGroupBox *localUserBox = m_channels.value(self->cChannel->iId);

	if (!localUserBox) {
		qCritical("TalkingUI::ensureVisible requested unknown channel!");
		return;
	}

	bool adjust = false;
	if (!channelBox->isVisible()) {
		// Make sure the channel-box is visible
		channelBox->show();

		// Sort the channels alphabetically
		bool inserted = false;
		for (int i = 0; i < layout()->count(); i++) {
			QGroupBox *currentBox = static_cast<QGroupBox *>(layout()->itemAt(i)->widget());

			if (currentBox == localUserBox) {
				// Exclude the local user's channel as that one is handled separately
				continue;
			}

			if (currentBox->title() > channelBox->title()) {
				static_cast<QVBoxLayout *>(layout())->insertWidget(i, channelBox);
				inserted = true;
				break;
			}
		}

		// The channel goes after all other channels
		if (!inserted) {
			layout()->addWidget(channelBox);
		}

		// Make sure that the user's channel is always the first channel in the list
		layout()->removeWidget(localUserBox);
		static_cast<QVBoxLayout *>(layout())->insertWidget(0, localUserBox);

		adjust = true;
	}

	if (!entry.background->isVisible() || changedChannel) {
		// Make sure the Entry for this user is visible
		entry.background->show();

		if (!m_entries.contains(self->uiSession)) {
			qCritical("TalkingUI::ensureVisible mapping for local user not present!");
			return;
		}
		Entry localUserEntry = m_entries.value(self->uiSession);

		// Sort the users alphabetically
		QString currentName = entry.background->property("userName").toString();
		bool inserted = false;
		for (int i = 0; i < channelBox->layout()->count(); i++) {
			QWidget *currentBackground = static_cast<QWidget *>(channelBox->layout()->itemAt(i)->widget());

			if (currentBackground == localUserEntry.background) {
				// Exclude the local user from sorting as that one is handled separately
				continue;
			}

			QString userName = currentBackground->property("userName").toString();

			if (userName > currentName) {
				static_cast<QVBoxLayout *>(channelBox->layout())->insertWidget(i, entry.background);
				inserted = true;
				break;
			}
		}

		// The current user goes after all other users in that channel
		if (!inserted) {
			channelBox->layout()->addWidget(entry.background);
		}

		if (channelBox == localUserBox && localUserBox->layout()->indexOf(localUserEntry.background) > 0) {
			// Make sure that the local user is always listed first 
			localUserBox->layout()->removeWidget(localUserEntry.background);
			static_cast<QVBoxLayout *>(localUserBox->layout())->insertWidget(0, localUserEntry.background);
		}

		adjust = true;
	}

	if (adjust) {
		updateUI();
	}
}

void TalkingUI::updateUI() {
	// Use timer to execute this after all other events have been processed
	QTimer::singleShot(0, [this]() { adjustSize(); });
}

void TalkingUI::setSelection(Entry *entry) {
	if (entry != m_currentSelection) {
		if (m_currentSelection) {
			// Set the selected property to false and refresh the style of the widget (letting the
			// global StyleSheet (theme) take over).
			m_currentSelection->background->setProperty("selected", false);
			m_currentSelection->background->style()->unpolish(m_currentSelection->background);
		}
		if (entry) {
			// Set the selected property to true and refresh the style of the widget (letting the
			// global StyleSheet (theme) take over).
			entry->background->setProperty("selected", true);
			entry->background->style()->unpolish(entry->background);

			if (g.mw && g.mw->pmModel) {
				g.mw->pmModel->setSelectedUser(entry->userSession);
			}
		}

		m_currentSelection = entry;
	}
}

void TalkingUI::mousePressEvent(QMouseEvent *event) {
	Q_UNUSED(event);

	QWidget *widget = qApp->widgetAt(event->globalPos());

	if (!widget) {
		setSelection(nullptr);
		return;
	}

	// iterate all available entries
	QMutableHashIterator<unsigned int, Entry> it(m_entries);

	Entry *entry = nullptr;

	while (it.hasNext()) {
		it.next();

		Entry *currentEntry = &it.value();
		if (currentEntry->talkingIcon == widget || currentEntry->name == widget || currentEntry->background == widget) {
			entry = currentEntry;
			break;
		}
	}

	setSelection(entry);

	if (entry && event->button() == Qt::RightButton && g.mw) {
		// If an entry is selected and the right mouse button was clicked, we pretend as if the user had clicked on the client in
		// the MainWindow. For this to work we map the global mouse position to the local coordinate system of the UserView in the
		// MainWindow. The function will use some internal logic to determine the user to invoke the context menu on but if that
		// fails (which in this case it will), it'll fall back to the currently selected item. This item we have updated to the
		// correct one with the setSelection() call above resulting in the proper context menu being shown at the position of the
		// mouse which in this case is in the TalkingUI.
		QMetaObject::invokeMethod(g.mw, "on_qtvUsers_customContextMenuRequested", Qt::QueuedConnection, Q_ARG(QPoint,
					g.mw->qtvUsers->mapFromGlobal(event->globalPos())));
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
	int width =  QGuiApplication::screens()[0]->availableSize().width() * 0.1;

	return {width, 0};
}

QSize TalkingUI::minimumSizeHint() const {
	return {0, 0};
}

void TalkingUI::on_talkingStateChanged() {
	ClientUser *user = qobject_cast<ClientUser *>(sender());

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

	addUser(user);

	// Get the Entry for this user
	Entry &entry = m_entries[user->uiSession];
	entry.talkingState = user->tsState;

	// Set the icon for this user according to the TalkingState
	setTalkingIcon(entry);

	if (user->tsState == Settings::Passive) {
		// User stopped talking
		// Start the timer that will remove this user if the user is not
		// the local user or if the local user is not configured to be always
		// visible.
		if (g.uiSession != user->uiSession || !g.s.bTalkingUI_LocalUserStaysVisible) {
			if (m_timers.contains(user->uiSession)) {
				m_timers[user->uiSession]->start();
			} else {
				qCritical("TalkingUI::on_talkingStateChanged Can't find timer for user (wanted to start it)!");
			}
		}
	} else {
		// User started talking
		// Stop the timer for this user in case it has been started before but the user
		// started speaking again before having been removed
		if (m_timers.contains(user->uiSession)) {
			m_timers[user->uiSession]->stop();
		} else {
			qCritical("TalkingUI::on_talkingStateChanged Can't find timer for user (wanted to stop it)!");
		}

		ensureVisible(user->uiSession, user->cChannel->iId);
	}
}

void TalkingUI::on_mainWindowSelectionChanged(const QModelIndex &current, const QModelIndex &previous) {
	Q_UNUSED(previous);

	// Sync the selection in the MainWindow to the TalkingUI
	if (g.mw && g.mw->pmModel) {
		ClientUser *user = g.mw->pmModel->getUser(current);

		if (user && m_entries.contains(user->uiSession)) {
			setSelection(&m_entries[user->uiSession]);
		} else {
			setSelection(nullptr);
		}
	}
}

void TalkingUI::on_serverSynchronized() {
	// After the synchronization is done we can also assume that the user has its name
	// assigned and is in a channel.
	// By manually adding the local user, we can make sure that the channel and the Entry
	// for it always exists.
	ClientUser *self = ClientUser::get(g.uiSession);
	addUser(self);

	if (g.s.bTalkingUI_LocalUserStaysVisible) {
		// According to the settings the local user should always be visible and as we
		// can't count on it to change its talking state right after it has connected to
		// a server, we have to add it manually.
		ensureVisible(self->uiSession, self->cChannel->iId);
	}
}

void TalkingUI::on_serverDisconnected() {
	// If we disconnect from a server, we have to clear all our users, channels, and so on
	// Note that strictly speaking we don't have to manually delete the objects as they
	// are parented to this one and thus Qt takes care of deleting them on the destruction of
	// the TalkingUI. However we do want to keep the memory footprint small as the time the
	// TalkingUI is alive, might be rather long.

	QHashIterator<unsigned int, Entry> entryIt(m_entries);
	while (entryIt.hasNext()) {
		Entry entry = entryIt.next().value();

		delete entry.talkingIcon;
		delete entry.name;
		delete entry.background;
	}
	m_entries.clear();

	QHashIterator<int, QGroupBox *> channelIt(m_channels);
	while (channelIt.hasNext()) {
		delete channelIt.next().value();
	}
	m_channels.clear();

	QHashIterator<unsigned int, QTimer *> timerIt(m_timers);
	while (timerIt.hasNext()) {
		QTimer *timer = timerIt.next().value();
		timer->stop();
		delete timer;
	}
	m_timers.clear();

	setSelection(nullptr);

	updateUI();
}

void TalkingUI::on_channelChanged(QObject *obj) {
	// According to this function's doc, the passed object must be of type ClientUser
	ClientUser *user = static_cast<ClientUser *>(obj);

	if (user && m_entries.contains(user->uiSession)) {
		if (m_entries[user->uiSession].background->isVisible()) {
			// The user is visible, so we call ensureVisible in order to update
			// the channel this particular user is being displayed in.
			// But first we have to make sure there actually exists and entry for
			// the new channel.
			addChannel(user->cChannel);
			ensureVisible(user->uiSession, user->cChannel->iId);
		}
	}
}

void TalkingUI::on_settingsChanged() {
	// The settings might have affected the way we have to display the channel names
	// thus we'll update them just in case
	QHashIterator<int, QGroupBox *> channelIt(m_channels);

	while (channelIt.hasNext()) {
		channelIt.next();

		const Channel *channel = Channel::get(channelIt.key());

		if (channel) {
			// Update
			QGroupBox *box = channelIt.value();
			box->setTitle(createChannelName(channel, g.s.bTalkingUI_AbbreviateChannelNames, g.s.iTalkingUI_PrefixCharCount,
				g.s.iTalkingUI_PostfixCharCount, g.s.iTalkingUI_MaxChannelNameLength, g.s.iTalkingUI_ChannelHierarchyDepth,
				g.s.qsTalkingUI_ChannelSeparator, g.s.qsTalkingUI_AbbreviationReplacement, g.s.bTalkingUI_AbbreviateCurrentChannel)
			);

			// The font size might have changed as well -> update it
			// As all other items are children to the channel boxes, the new font
			// size should propagate through.
			setFontSize(box);
		} else {
			qCritical("TalkingUI: Can't find channel for stored ID");
		}
	}

	// If the font has changed, we have to update the icon size as well
	QMutableHashIterator<unsigned int, Entry> entryIt(m_entries);
	while(entryIt.hasNext()) {
		Entry &entry = entryIt.next().value();
		// The new line height has already been set by setFontSize, so we only have
		// to call setTalkingIcon
		setTalkingIcon(entry);
	}

	// The time that a silent user may stick around might have changed as well
	QHashIterator<unsigned int, QTimer *> timerIt(m_timers);
	while (timerIt.hasNext()) {
		QTimer *timer = timerIt.next().value();
		// * 1000 as the setting is in seconds whereas the timer expects milliseconds
		timer->setInterval(g.s.iTalkingUI_SilentUserLifeTime * 1000);
	}

	// Whether or not the current user should always be displayed might also have changed,
	// so we'll have to update that as well.
	const ClientUser *self = ClientUser::get(g.uiSession);

	if (self) {
		if (m_timers.contains(self->uiSession)) {
			if (g.s.bTalkingUI_LocalUserStaysVisible) {
				// Stop any potentially running timers as we don't want to remove the user (anymore)
				if (m_timers.contains(self->uiSession)) {
					m_timers[self->uiSession]->stop();
				} else {
					qCritical("TalkingUI::on_settingsChanged Can't find timer for local user (wanted to stop it)");
				}

				// Make sure local user exists and is visible
				addUser(self);
				ensureVisible(self->uiSession, self->cChannel->iId);
			} else if (self->tsState == Settings::Passive) {
				// Start the timer as the user may not stay around forever and (s)he is currently not
				// talking.
				if (m_timers.contains(self->uiSession)) {
					m_timers[self->uiSession]->start();
				} else {
					qCritical("TalkingUI::on_settingsChanged Can't find timer for local user (wanted to start it)");
				}
			}
		}
	}
}

void TalkingUI::on_clientDisconnected(unsigned int userSession) {
	if (m_entries.contains(userSession)) {
		// If the user that just disconnected is contained in the TalkingUI, make sure
		// it is hidden.
		hideUser(userSession);

		// Delete the user's entry from the TalkingUI
		m_entries.remove(userSession);
	}
}

void TalkingUI::on_muteDeafStateChanged() {
	ClientUser *user = qobject_cast<ClientUser *>(sender());

	if (user) {
		// Update icons for local user only
		updateStatusIcons(user);
	}
}

