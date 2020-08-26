// Copyright 2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CHANNEL_LISTENER_H_
#define MUMBLE_CHANNEL_LISTENER_H_

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QReadWriteLock>

#include <atomic>

class User;
class Channel;

/// This class serves as a namespace for storing information about ChannelListeners. This is a feature
/// that allows a user to listen to a channel without being in it. Kinda similar to linked channels
/// except that this is something each user can do individually.
class ChannelListener : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ChannelListener)

	protected:
		/// A lock for guarding m_listeningUsers as well as m_listenedChannels
		mutable QReadWriteLock m_listenerLock;
		/// A map between a user's session and a list of IDs of all channels the user is listening to
		QHash<unsigned int, QSet<int>> m_listeningUsers;
		/// A map between a channel's ID and a list of all user-sessions of users listening to that channel
		QHash<int, QSet<unsigned int>> m_listenedChannels;
#ifdef MUMBLE
		/// A lock for guarding m_listenerVolumeAdjustments
		mutable QReadWriteLock m_volumeLock;
		/// A map between channel IDs and local volume adjustments to be made for ChannelListeners
		/// in that channel
		QHash<int, float> m_listenerVolumeAdjustments;

		/// A flag indicating whether the initial synchronization with the server has finished yet
		std::atomic<bool> m_initialSyncDone;
#endif

		/// The static ChannelListener instance returned by ChannelListener::get()
		static ChannelListener s_instance;

		/// Constructor
		ChannelListener();

		/// Adds a listener to the channel.
		///
		/// @param userSession The session ID of the user
		/// @param channelID The ID of the channel
		void addListenerImpl(unsigned int userSession, int channelID);

		/// Removes a listener from the channel.
		///
		/// @param userSession The session ID of the user
		/// @param channelID The ID of the channel
		void removeListenerImpl(unsigned int userSession, int channelID);

		/// @param userSession The session ID of the user
		/// @param channelID The ID of the channel
		/// @returns Whether the given user is listening to the given channel
		bool isListeningImpl(unsigned int userSession, int channelID) const;

		/// @param userSession The session ID of the user
		/// @returns Whether this user is listening to any channel via the ChannelListener feature
		bool isListeningToAnyImpl(unsigned int userSession) const;

		/// @param channelID The ID of the channel
		/// @returns Whether any user listens to this channel via the ChannelListener feature
		bool isListenedByAnyImpl(int channelID) const;

		/// @param channelID The ID of the channel
		/// @returns A set of user sessions of users listening to the given channel
		const QSet<unsigned int> getListenersForChannelImpl(int channelID) const;

		/// @param userSession The session ID of the user
		/// @returns A set of channel IDs of channels the given user is listening to
		const QSet<int> getListenedChannelsForUserImpl(unsigned int userSession) const;

		/// @param channelID The ID of the channel
		/// @returns The amount of users that are listening to the given channel
		int getListenerCountForChannelImpl(int channelID) const;

		/// @param userSession The session ID of the user
		/// @returns The amount of channels the given user is listening to
		int getListenedChannelCountForUserImpl(unsigned int userSession) const;

#ifdef MUMBLE
		/// Sets the local volume adjustment for any channelListener in the given channel.
		///
		/// @param channelID The ID of the channel
		/// @param volumeAdjustment The volume adjustment to apply
		void setListenerLocalVolumeAdjustmentImpl(int channelID, float volumeAdjustment);

		/// @param channelID The ID of the channel
		/// @param The local volume adjustment for the given channel. If none has been set,
		/// 	1.0f is being returned.
		float getListenerLocalVolumeAdjustmentImpl(int channelID) const;

		/// @param filter Whether to filter out adjustments of 1 (which have no effect)
		/// @returns A map between channel IDs and the currently set volume adjustment
		QHash<int, float> getAllListenerLocalVolumeAdjustmentsImpl(bool filter = false) const;

		/// @done Whether the initial synchronization with the server is done yet
		void setInitialServerSyncDoneImpl(bool done);
#endif

		/// Clears all ChannelListeners and volume adjustments
		void clearImpl();

	public:
		/// @returns The static ChannelListener instance
		static ChannelListener& get();

		/// Adds a listener to the channel.
		///
		/// @param userSession The session ID of the user
		/// @param channelID The ID of the channel
		static void addListener(unsigned int userSession, int channelID);
		
		/// Adds a listener to the channel.
		///
		/// @param userSession The session ID of the user
		/// @param channelID The ID of the channel
		static void addListener(const User *user, const Channel *channel);

		 
		/// Removes a listener from the channel.
		///
		/// @param userSession The session ID of the user
		/// @param channelID The ID of the channel
		static void removeListener(unsigned int userSession, int channelID);

		/// Removes a listener from the channel.
		///
		/// @param userSession The session ID of the user
		/// @param channelID The ID of the channel
		static void removeListener(const User *user, const Channel *channel);


		/// @param userSession The session ID of the user
		/// @param channelID The ID of the channel
		/// @returns Whether the given user is listening to the given channel
		static bool isListening(unsigned int userSession, int channelID);

		/// @param channel A pointer to the channel object
		/// @param user A pointer to the user object
		/// @returns Whether the given user is listening to the given channel
		static bool isListening(const User *user, const Channel *channel);


		/// @param userSession The session ID of the user
		/// @returns Whether this user is listening to any channel via the ChannelListener feature
		static bool isListeningToAny(unsigned int userSession);

		/// @param user A pointer to the user object
		/// @returns Whether this user is listening to any channel via the ChannelListener feature
		static bool isListeningToAny(const User *user);


		/// @param channelID The ID of the channel
		/// @returns Whether any user listens to this channel via the ChannelListener feature
		static bool isListenedByAny(int channelID);

		/// @param channel A pointer to the channel object
		/// @returns Whether any user listens to this channel via the ChannelListener feature
		static bool isListenedByAny(const Channel *channel);


		/// @param channelID The ID of the channel
		/// @returns A set of user sessions of users listening to the given channel
		static const QSet<unsigned int> getListenersForChannel(int channelID);

		/// @param channel A pointer to the channel object
		/// @returns A set of user sessions of users listening to the given channel
		static const QSet<unsigned int> getListenersForChannel(const Channel *channel);


		/// @param userSession The session ID of the user
		/// @returns A set of channel IDs of channels the given user is listening to
		static const QSet<int> getListenedChannelsForUser(unsigned int userSession);

		/// @param user A pointer to the user object
		/// @returns A set of channel IDs of channels the given user is listening to
		static const QSet<int> getListenedChannelsForUser(const User *user);


		/// @param channelID The ID of the channel
		/// @returns The amount of users that are listening to the given channel
		static int getListenerCountForChannel(int channelID);

		/// @param channel A pointer to the channel object
		/// @returns The amount of users that are listening to the given channel
		static int getListenerCountForChannel(const Channel *channel);


		/// @param userSession The session ID of the user
		/// @returns The amount of channels the given user is listening to
		static int getListenedChannelCountForUser(unsigned int userSession);

		/// @param user A pointer to the user object
		/// @returns The amount of channels the given user is listening to
		static int getListenedChannelCountForUser(const User *user);


#ifdef MUMBLE
		/// Sets the local volume adjustment for any channelListener in the given channel.
		///
		/// @param channelID The ID of the channel
		/// @param volumeAdjustment The volume adjustment to apply
		static void setListenerLocalVolumeAdjustment(int channelID, float volumeAdjustment);

		/// Sets the local volume adjustment for any channelListener in the given channel.
		///
		/// @param channel A pointer to the channel object
		/// @param volumeAdjustment The volume adjustment to apply
		static void setListenerLocalVolumeAdjustment(const Channel *channel, float volumeAdjustment);


		/// @param channelID The ID of the channel
		/// @param The local volume adjustment for the given channel. If none has been set,
		/// 	1.0f is being returned.
		static float getListenerLocalVolumeAdjustment(int channelID);

		/// @param channel A pointer to the channel object
		/// @param The local volume adjustment for the given channel. If none has been set,
		/// 	1.0f is being returned.
		static float getListenerLocalVolumeAdjustment(const Channel *channel);

		/// @param filter Whether to filter out adjustments of 1 (which have no effect)
		/// @returns A map between channel IDs and the currently set volume adjustment
		static QHash<int, float> getAllListenerLocalVolumeAdjustments(bool filter = false);

		/// @done Whether the initial synchronization with the server is done yet
		static void setInitialServerSyncDone(bool done);

		/// Saves the current ChannelListener state to the database.
		/// NOTE: This function may only be called from the main thread!
		static void saveToDB();
#endif

		/// Clears all ChannelListeners and volume adjustments
		static void clear();

	signals:
#ifdef MUMBLE
		void localVolumeAdjustmentsChanged(int channelID, float newAdjustment, float oldAdjustment);
#endif
};

#endif // MUMBLE_CHANNEL_LISTENER_H_
