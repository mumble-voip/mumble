// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_TALKINGUI_H_
#define MUMBLE_MUMBLE_TALKINGUI_H_

#include <QWidget>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtGui/QIcon>

#include <memory>
#include <vector>

#include "Settings.h"
#include "TalkingUIContainer.h"
#include "TalkingUIEntry.h"
#include "TalkingUISelection.h"

class QLabel;
class QGroupBox;
class QTimer;
class QMouseEvent;

class Channel;
class ClientUser;
class TalkingUIComponent;
class MultiStyleWidgetWrapper;

/// The talking UI is a widget that will display the users you are currently
/// hearing to you.
class TalkingUI : public QWidget {
	friend class TalkingUIUser;

private:
	Q_OBJECT
	Q_DISABLE_COPY(TalkingUI)

	std::vector< std::unique_ptr< TalkingUIContainer > > m_containers;
	/// The Entry corresponding to the currently selected user
	std::unique_ptr< TalkingUISelection > m_currentSelection;

	/// The current line height of an entry in the TalkingUI
	int m_currentLineHeight;

	int findContainer(int associatedChannelID, ContainerType type) const;
	std::unique_ptr< TalkingUIContainer > removeContainer(const TalkingUIContainer &container);
	std::unique_ptr< TalkingUIContainer > removeContainer(int associatedChannelID, ContainerType type);
	std::unique_ptr< TalkingUIContainer > removeIfSuperfluous(const TalkingUIContainer &container);

	void sortContainers();

	TalkingUIUser *findUser(unsigned int userSession);
	void removeUser(unsigned int userSession);

	void addListener(const ClientUser *user, const Channel *channel);
	TalkingUIChannelListener *findListener(unsigned int userSession, unsigned int channelID);
	void removeListener(unsigned int userSession, unsigned int channelID);
	void removeAllListeners();

	/// Sets up the UI components
	void setupUI();
	/// Hides an user
	///
	/// @param session The session ID of the user that shall be hidden
	void hideUser(unsigned int session);
	/// Adds an UI entry for the given Channel, if none exists yet.
	///
	/// @param channel A pointer to the channel that shall be added
	void addChannel(const Channel *channel);
	/// Adds an UI entry for the given User, if none exists yet.
	///
	/// @param channel A pointer to the user that shall be added
	/// @returns The pointer to the respective user entry in the TalkingUI
	/// (may be nullptr in case of an error)
	TalkingUIUser *findOrAddUser(const ClientUser *user);
	/// Moves the given user into the given channel
	///
	/// @paam userSession The session ID of the user
	/// @param channelID The channel ID of the channel
	void moveUserToChannel(unsigned int userSession, unsigned int channelID);

	/// Update (resize) the UI to its content
	void updateUI();

	/// Sets the font size according to the settings
	///
	/// @param widgetWrapper An instance of MultiStyleWidgetWrapper that wraps the widget whose font size
	/// 	shall be set
	void setFontSize(MultiStyleWidgetWrapper &widgetWrapper);

	/// Updates the user's status icons (reflecting e.g. its mut-state)
	///
	/// @param user A pointer to the user that shall be processed
	void updateStatusIcons(const ClientUser *user);

	/// Set the current selection
	///
	/// @param selection The new selection
	void setSelection(const TalkingUISelection &selection);

	bool isSelected(const TalkingUIComponent &component) const;

	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

public:
	TalkingUI(QWidget *parent = nullptr);

	void setVisible(bool visible) Q_DECL_OVERRIDE;
	QSize sizeHint() const Q_DECL_OVERRIDE;
	QSize minimumSizeHint() const Q_DECL_OVERRIDE;

public slots:
	void on_talkingStateChanged();
	void on_mainWindowSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void on_serverSynchronized();
	void on_serverDisconnected();
	void on_channelChanged(QObject *user);
	void on_settingsChanged();
	void on_clientDisconnected(unsigned int userSession);
	void on_muteDeafStateChanged();
	void on_userLocalVolumeAdjustmentsChanged(float newAdjustment, float oldAdjustment);
	void on_channelListenerAdded(const ClientUser *user, const Channel *channel);
	void on_channelListenerRemoved(const ClientUser *user, const Channel *channel);
	void on_channelListenerLocalVolumeAdjustmentChanged(unsigned int channelID, float newAdjustment,
														float oldAdjustment);
};

#endif // MUMBLE_MUMBLE_TALKINGUI_H_
