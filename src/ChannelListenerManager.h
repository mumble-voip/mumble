// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CHANNELLISTENERMANAGER_H_
#define MUMBLE_CHANNELLISTENERMANAGER_H_

#include "VolumeAdjustment.h"

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>
#include <QtCore/QSet>

#include <unordered_map>

class User;
class Channel;

struct ChannelListener {
	/// The session ID of the owning user
	unsigned int userSession;
	/// The ID of the channel this listener is placed in
	unsigned int channelID;
};

// Make ChannelListener hashable and comparable
template<> struct std::hash< ChannelListener > {
	std::size_t operator()(const ChannelListener &val) const {
		return std::hash< unsigned int >()(val.userSession) ^ (std::hash< unsigned int >()(val.channelID) << 2);
	}
};
std::size_t qHash(const ChannelListener &listener);
bool operator==(const ChannelListener &lhs, const ChannelListener &rhs);


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
	QHash< unsigned int, QSet< unsigned int > > m_listeningUsers;
	/// A map between a channel's ID and a list of all user-sessions of users listening to that channel
	QHash< unsigned int, QSet< unsigned int > > m_listenedChannels;
	/// A lock for guarding m_listenerVolumeAdjustments
	mutable QReadWriteLock m_volumeLock;
	/// A map between channel IDs and local volume adjustments to be made for ChannelListeners
	/// in that channel
	std::unordered_map< ChannelListener, VolumeAdjustment > m_listenerVolumeAdjustments;

public:
	/// Constructor
	explicit ChannelListenerManager();

	/// Adds a listener to the channel.
	///
	/// @param userSession The session ID of the user
	/// @param channelID The ID of the channel
	void addListener(unsigned int userSession, unsigned int channelID);

	/// Removes a listener from the channel.
	///
	/// @param userSession The session ID of the user
	/// @param channelID The ID of the channel
	void removeListener(unsigned int userSession, unsigned int channelID);

	/// @param userSession The session ID of the user
	/// @param channelID The ID of the channel
	/// @returns Whether the given user is listening to the given channel
	bool isListening(unsigned int userSession, unsigned int channelID) const;

	/// @param userSession The session ID of the user
	/// @returns Whether this user is listening to any channel via the ChannelListener feature
	bool isListeningToAny(unsigned int userSession) const;

	/// @param channelID The ID of the channel
	/// @returns Whether any user listens to this channel via the ChannelListener feature
	bool isListenedByAny(unsigned int channelID) const;

	/// @param channelID The ID of the channel
	/// @returns A set of user sessions of users listening to the given channel
	const QSet< unsigned int > getListenersForChannel(unsigned int channelID) const;

	/// @param userSession The session ID of the user
	/// @returns A set of channel IDs of channels the given user is listening to
	const QSet< unsigned int > getListenedChannelsForUser(unsigned int userSession) const;

	/// @param channelID The ID of the channel
	/// @returns The amount of users that are listening to the given channel
	int getListenerCountForChannel(unsigned int channelID) const;

	/// @param userSession The session ID of the user
	/// @returns The amount of channels the given user is listening to
	int getListenedChannelCountForUser(unsigned int userSession) const;

	/// Sets the volume adjustment for the channelListener of the given user in the given channel.
	///
	/// @param userSession The session ID of the user
	/// @param channelID The ID of the channel
	/// @param volumeAdjustment The volume adjustment to apply
	void setListenerVolumeAdjustment(unsigned int userSession, unsigned int channelID,
									 const VolumeAdjustment &volumeAdjustment);

	/// @param userSession The session ID of the user
	/// @param channelID The ID of the channel
	/// @returns The volume adjustment for the listener of the given user in the given channel.
	const VolumeAdjustment &getListenerVolumeAdjustment(unsigned int userSession, unsigned int channelID) const;

	/// @param userSession The session ID of the user whose listener's volume adjustments to obtain
	/// @returns A map between channel IDs and the currently set volume adjustment
	std::unordered_map< unsigned int, VolumeAdjustment >
		getAllListenerVolumeAdjustments(unsigned int userSession) const;

	/// Clears all ChannelListeners and volume adjustments
	void clear();
signals:
	void localVolumeAdjustmentsChanged(unsigned int channelID, float newAdjustment, float oldAdjustment);
};

#endif // MUMBLE_CHANNELLISTENERMANAGER_H_
