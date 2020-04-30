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

class QLabel;
class QGroupBox;
class QTimer;
class QMouseEvent;

/// Smaller auxilary class for holding together two labels representing
/// the entry for a specific user in the TalkingUI<
struct Entry {
	QLabel *icon;
	QLabel *name;
	QWidget *background;
	unsigned int userSession;
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

		Entry *m_currentSelection;

		/// The icon for a talking user
		QIcon m_talkingIcon;
		/// The icon for a silent user
		QIcon m_passiveIcon;
		/// The icon for a shouting user
		QIcon m_shoutingIcon;
		/// The icon for a whispering user
		QIcon m_whisperingIcon;

		/// Sets up the UI components
		void setupUI();
		/// Removes a user from being displayed
		void removeUser(unsigned int session);

		/// Update (resize) the UI to its content
		void updateUI();

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
};

#endif // MUMBLE_MUMBLE_TALKINGUI_H_
