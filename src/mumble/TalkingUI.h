// Copyright 2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_TALKINGUI_H_
#define MUMBLE_MUMBLE_TALKINGUI_H_

#include <QWidget>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtGui/QIcon>

#include "Settings.h"

class QLabel;
class QGroupBox;
class QTimer;
class QMouseEvent;

class Channel;
class ClientUser;

/// Smaller auxilary class for holding together two labels representing
/// the entry for a specific user in the TalkingUI
struct Entry {
	QLabel *talkingIcon;
	QLabel *name;
	QWidget *background;
	/// A label that'll show a user's status icons. If no icons are displayed,
	/// this may be null!
	QLabel *statusIcons;
	unsigned int userSession;
	Settings::TalkState talkingState;
};

/// The talking UI is a widget that will display the users you are currently
/// hearing to you.
class TalkingUI : public QWidget {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(TalkingUI);

		/// A map between user session ID and the corresponding Entry
		QHash<unsigned int, Entry> m_entries;
		/// A map between channel ID and the QGroupBox used to represent it
		QHash<int, QGroupBox *> m_channels;
		/// A map betweem user session IDs and the timer used to remove users
		/// that have stopped speaking for a certain amount of time.
		QHash<unsigned int, QTimer *> m_timers;
		/// The Entry corresponding to the currently selected user
		Entry *m_currentSelection;

		/// The icon for a talking user
		QIcon m_talkingIcon;
		/// The icon for a silent user
		QIcon m_passiveIcon;
		/// The icon for a shouting user
		QIcon m_shoutingIcon;
		/// The icon for a whispering user
		QIcon m_whisperingIcon;

		/// The icon for a muted user
		QIcon m_muteIcon;
		/// The icon for a deafened user
		QIcon m_deafIcon;
		/// The icon for a locally muted user
		QIcon m_localMuteIcon;
		/// The icon for the local user if muted
		QIcon m_selfMuteIcon;
		/// The icon for the local user if deafened
		QIcon m_selfDeafIcon;

		/// The current line height of an entry in the TalkingUI
		int m_currentLineHeight;

		/// Sets up the UI components
		void setupUI();
		/// Hides an user
		///
		/// @param session The session ID of the user that shall be hidden
		void hideUser(unsigned int session);
		/// Adds an UI entry for the given Channel, if none exists yet.
		///
		/// @param channel A pointer to the channel that shall be added
		void addChannel(const Channel *channel);;
		/// Adds an UI entry for the given User, if none exists yet.
		///
		/// @param channel A pointer to the user that shall be added
		void addUser(const ClientUser *user);
		/// Makes sure the user with the given session is visible and
		/// placed in the channel with the given ID. Note that both the
		/// user and the channel are expected to have an UI entry already.
		///
		/// @paam userSession The session ID of the user
		/// @param channelID The channel ID of the channel
		void ensureVisible(unsigned int userSession, int channelID);

		/// Update (resize) the UI to its content
		void updateUI();

		/// Sets the font size according to the settings
		///
		/// @param widget a pointer to the widget to set the font size for
		void setFontSize(QWidget *widget);

		/// Sets the icon for the given entry based on its TalkingState
		///
		/// @param entry A reference to the Entry to process
		void setTalkingIcon(Entry &entry) const;

		/// Updates the user's status icons (reflecting e.g. its mut-state)
		///
		/// @param user A pointer to the user that shall be processed
		void updateStatusIcons(const ClientUser *user);

		/// Set the current selection
		///
		/// @param entry A pointer to the Entry that shall be selected or nullptr to clear the selection
		void setSelection(Entry *entry);

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
};

#endif // MUMBLE_MUMBLE_TALKINGUI_H_
