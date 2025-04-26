// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_TALKINGUISELECTION_H_
#define MUMBLE_MUMBLE_TALKINGUISELECTION_H_

#include <memory>

class QWidget;

/// Base class of all selections within the TalkingUI
class TalkingUISelection {
protected:
	/// The widget that is used to represent this selection (it'll be marked
	/// as selected).
	QWidget *m_widget;

	explicit TalkingUISelection() = default;

public:
	explicit TalkingUISelection(QWidget *widget);
	virtual ~TalkingUISelection() = default;

	/// Turns this selection on or off. Turning it on usually involves marking the
	/// associated Widget in a certain way while deactivating the selection reverts this effect.
	///
	/// @param active Whether to activate this selection
	virtual void setActive(bool active);

	/// Applies this selection. This is a shortcut for setActive(true).
	virtual void apply() final;

	/// Discards this selection. This is a shortcut for setActive(false).
	virtual void discard() final;

	/// Synchronizes this selection to the MainWindow
	virtual void syncToMainWindow() const = 0;

	bool operator==(const TalkingUISelection &other) const;
	bool operator!=(const TalkingUISelection &other) const;

	bool operator==(const QWidget *widget) const;
	bool operator!=(const QWidget *widget) const;

	virtual std::unique_ptr< TalkingUISelection > cloneToHeap() const = 0;
};

/// A class representing the selection of a user in the TalkingUI
class UserSelection : public TalkingUISelection {
protected:
	const unsigned int m_userSession;

public:
	explicit UserSelection(QWidget *widget, unsigned int userSession);
	explicit UserSelection(const UserSelection &) = default;

	virtual void syncToMainWindow() const override;

	virtual std::unique_ptr< TalkingUISelection > cloneToHeap() const override;
};

/// A class representing the selection of a channel in the TalkingUI
class ChannelSelection : public TalkingUISelection {
protected:
	const unsigned int m_channelID;

public:
	explicit ChannelSelection(QWidget *widget, unsigned int channelID);
	explicit ChannelSelection(const ChannelSelection &) = default;

	virtual void syncToMainWindow() const override;

	virtual std::unique_ptr< TalkingUISelection > cloneToHeap() const override;
};

class ListenerSelection : public TalkingUISelection {
protected:
	unsigned int m_userSession;
	const unsigned int m_channelID;

public:
	explicit ListenerSelection(QWidget *widget, unsigned int userSession, unsigned int channelID);
	explicit ListenerSelection(const ListenerSelection &) = default;

	virtual void syncToMainWindow() const override;

	virtual std::unique_ptr< TalkingUISelection > cloneToHeap() const override;
};

/// A class representing an empty selection in the TalkingUI
class EmptySelection : public TalkingUISelection {
public:
	explicit EmptySelection()                       = default;
	explicit EmptySelection(const EmptySelection &) = default;

	virtual void syncToMainWindow() const override;

	virtual std::unique_ptr< TalkingUISelection > cloneToHeap() const override;
};

#endif // MUMBLE_MUMBLE_TALKINGUISELECTION_H_
