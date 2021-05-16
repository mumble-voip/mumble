// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CHANNELLISTENERMANAGER_H_
#define MUMBLE_CHANNELLISTENERMANAGER_H_

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>
#include <QtCore/QSet>

#include <atomic>

class User;
class Channel;

/// This class serves as a namespace for storing information about ChannelListeners. This is a feature
/// that allows a user to listen to a channel without being in it. Kinda similar to linked channels
/// except that this is something each user can do individually.
class ChannelListenerManager : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ChannelListenerManager)

protected:
	/// A lock for guarding m_listeningUsers as well as m_listenedChannels
	mutable QReadWriteLock m_listenerLock;
	/// A map between a user's session and a list of IDs of all channels the user is listening to
	QHash< unsigned int, QSet< int > > m_listeningUsers;
	/// A map between a channel's ID and a list of all user-sessions of users listening to that channel
	QHash< int, QSet< unsigned int > > m_listenedChannels;
#ifdef MUMBLE
	/// A lock for guarding m_listenerVolumeAdjustments
	mutable QReadWriteLock m_volumeLock;
	/// A map between channel IDs and local volume adjustments to be made for ChannelListeners
	/// in that channel
	QHash< int, float > m_listenerVolumeAdjustments;

	/// A flag indicating whether the initial synchronization with the server has finished yet
	std::atomic< bool > m_initialSyncDone;
#endif

public:
	/// Constructor
	explicit ChannelListenerManager();

	/// Adds a listener to the channel.
	///
	/// @param userSession The session ID of the user
	/// @param channelID The ID of the channel
	void addListener(unsigned int userSession, int channelID);

	/// Removes a listener from the channel.
	///
	/// @param userSession The session ID of the user
	/// @param channelID The ID of the channel
	void removeListener(unsigned int userSession, int channelID);

	/// @param userSession The session ID of the user
	/// @param channelID The ID of the channel
	/// @returns Whether the given user is listening to the given channel
	bool isListening(unsigned int userSession, int channelID) const;

	/// @param userSession The session ID of the user
	/// @returns Whether this user is listening to any channel via the ChannelListener feature
	bool isListeningToAny(unsigned int userSession) const;

	/// @param channelID The ID of the channel
	/// @returns Whether any user listens to this channel via the ChannelListener feature
	bool isListenedByAny(int channelID) const;

	/// @param channelID The ID of the channel
	/// @returns A set of user sessions of users listening to the given channel
	const QSet< unsigned int > getListenersForChannel(int channelID) const;

	/// @param userSession The session ID of the user
	/// @returns A set of channel IDs of channels the given user is listening to
	const QSet< int > getListenedChannelsForUser(unsigned int userSession) const;

	/// @param channelID The ID of the channel
	/// @returns The amount of users that are listening to the given channel
	int getListenerCountForChannel(int channelID) const;

	/// @param userSession The session ID of the user
	/// @returns The amount of channels the given user is listening to
	int getListenedChannelCountForUser(unsigned int userSession) const;

#ifdef MUMBLE
	/// Sets the local volume adjustment for any channelListener in the given channel.
	///
	/// @param channelID The ID of the channel
	/// @param volumeAdjustment The volume adjustment to apply
	void setListenerLocalVolumeAdjustment(int channelID, float volumeAdjustment);

	/// @param channelID The ID of the channel
	/// @param The local volume adjustment for the given channel. If none has been set,
	/// 	1.0f is being returned.
	float getListenerLocalVolumeAdjustment(int channelID) const;

	/// @param filter Whether to filter out adjustments of 1 (which have no effect)
	/// @returns A map between channel IDs and the currently set volume adjustment
	QHash< int, float > getAllListenerLocalVolumeAdjustments(bool filter = false) const;

	/// @done Whether the initial synchronization with the server is done yet
	void setInitialServerSyncDone(bool done);

	/// Saves the current ChannelListener state to the database.
	/// NOTE: This function may only be called from the main thread!
	void saveToDB() const;
#endif

	/// Clears all ChannelListeners and volume adjustments
	void clear();
signals:
#ifdef MUMBLE
	void localVolumeAdjustmentsChanged(int channelID, float newAdjustment, float oldAdjustment);
#endif
};

#endif // MUMBLE_CHANNELLISTENERMANAGER_H_
