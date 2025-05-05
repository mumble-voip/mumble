// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_H_
#define MUMBLE_MUMBLE_PLUGIN_H_

#define MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS
#include "MumblePlugin.h"
#undef MUMBLE_PLUGIN_NO_DEFAULT_FUNCTION_DEFINITIONS

#include "MumbleAPI_structs.h"
#include "PositionalData.h"

#include <QLibrary>
#include <QMutex>
#include <QObject>
#include <QReadWriteLock>
#include <QString>
#include <QUrl>

#include <memory>
#include <stdexcept>

/// A struct for holding the function pointers to the functions inside the plugin's library
/// For the documentation of those functions, see the plugin's header file (the one used when developing a plugin)
struct MumblePluginFunctions {
	decltype(&mumble_init) init;
	decltype(&mumble_shutdown) shutdown;
	decltype(&mumble_getName) getName;
	decltype(&mumble_getAPIVersion) getAPIVersion;
	decltype(&mumble_registerAPIFunctions) registerAPIFunctions;
	decltype(&mumble_releaseResource) releaseResource;

	decltype(&mumble_getPluginFunctionsVersion) getPluginFunctionsVersion;

	// Further utility functions the plugin may implement
	decltype(&mumble_setMumbleInfo) setMumbleInfo;
	decltype(&mumble_getVersion) getVersion;
	decltype(&mumble_getAuthor) getAuthor;
	decltype(&mumble_getDescription) getDescription;
	decltype(&mumble_getFeatures) getFeatures;
	decltype(&mumble_deactivateFeatures) deactivateFeatures;

	// Functions for dealing with positional audio (or rather the fetching of the needed data)
	decltype(&mumble_initPositionalData) initPositionalData;
	decltype(&mumble_fetchPositionalData) fetchPositionalData;
	decltype(&mumble_shutdownPositionalData) shutdownPositionalData;
	decltype(&mumble_getPositionalDataContextPrefix) getPositionalDataContextPrefix;

	// Callback functions and EventHandlers
	decltype(&mumble_onServerConnected) onServerConnected;
	decltype(&mumble_onServerDisconnected) onServerDisconnected;
	decltype(&mumble_onChannelEntered) onChannelEntered;
	decltype(&mumble_onChannelExited) onChannelExited;
	decltype(&mumble_onUserTalkingStateChanged) onUserTalkingStateChanged;
	decltype(&mumble_onReceiveData) onReceiveData;
	decltype(&mumble_onAudioInput) onAudioInput;
	decltype(&mumble_onAudioSourceFetched) onAudioSourceFetched;
	decltype(&mumble_onAudioOutputAboutToPlay) onAudioOutputAboutToPlay;
	decltype(&mumble_onServerSynchronized) onServerSynchronized;
	decltype(&mumble_onUserAdded) onUserAdded;
	decltype(&mumble_onUserRemoved) onUserRemoved;
	decltype(&mumble_onChannelAdded) onChannelAdded;
	decltype(&mumble_onChannelRemoved) onChannelRemoved;
	decltype(&mumble_onChannelRenamed) onChannelRenamed;
	decltype(&mumble_onKeyEvent) onKeyEvent;

	// Plugin updates
	decltype(&mumble_hasUpdate) hasUpdate;
	decltype(&mumble_getUpdateDownloadURL) getUpdateDownloadURL;
};


/// An exception that is being thrown by a plugin whenever it encounters an error
class PluginError : public std::runtime_error {
public:
	// inherit constructors of runtime_error
	using std::runtime_error::runtime_error;
};


/// An implementation similar to QReadLocker except that this one allows to lock on a lock the same thread is already
/// holding a write-lock on. This could also result in obtaining a write-lock though so it shouldn't be used for code
/// regions that take quite some time and rely on other readers still having access to the locked object.
class PluginReadLocker {
protected:
	/// The lock this lock-guard is acting upon
	QReadWriteLock *m_lock;
	/// A flag indicating whether the lock has been unlocked (manually) and thus doesn't have to be unlocked
	/// in the destructor.
	bool m_unlocked;

public:
	/// Constructor of the PluginReadLocker. If the passed lock-pointer is not nullptr, the constructor will
	/// already lock the provided lock.
	///
	/// @param lock A pointer to the QReadWriteLock that shall be managed by this object. May be nullptr
	PluginReadLocker(QReadWriteLock *lock);
	/// Locks this lock again after it has been unlocked before (Locking a locked lock results in a runtime error)
	void relock();
	/// Unlocks this lock
	void unlock();
	~PluginReadLocker();
};

class Plugin;

/// Typedef for the plugin ID
typedef uint32_t plugin_id_t;
/// Typedef for a plugin pointer
typedef std::shared_ptr< Plugin > plugin_ptr_t;
/// Typedef for a const plugin pointer
typedef std::shared_ptr< const Plugin > const_plugin_ptr_t;

/// A class representing a plugin library attached to Mumble. It can be used to manage (load/unload) and access plugin
/// libraries.
class Plugin : public QObject {
	friend class PluginManager;
	friend class PluginConfig;

private:
	Q_OBJECT
	Q_DISABLE_COPY(Plugin)
protected:
	/// A mutex guarding Plugin::nextID
	static QMutex s_idLock;
	/// The ID of the plugin that will be loaded next. Whenever accessing this field, Plugin::idLock should be locked.
	static plugin_id_t s_nextID;

	/// Constructor of the Plugin.
	///
	/// @param path The path to the plugin's shared library file. This path has to exist unless isBuiltIn is true
	/// @param isBuiltIn A flag indicating that this is a plugin built into Mumble itself and is does not backed by a
	/// shared library
	/// @param p A pointer to a QObject representing the parent of this object or nullptr if there is no parent
	Plugin(QString path, bool isBuiltIn = false, QObject *p = nullptr);

	/// A flag indicating whether this plugin is valid. It is mainly used throughout the plugin's initialization.
	bool m_pluginIsValid;
	/// The QLibrary representing the shared library of this plugin
	QLibrary m_lib;
	/// The path to the shared library file in the host's filesystem
	QString m_pluginPath;
	/// The unique ID of this plugin. Note though that this ID is not suitable for uniquely identifying this plugin
	/// between restarts of Mumble (not even between rescans of the plugins) let alone across clients.
	plugin_id_t m_pluginID;
	// a flag indicating whether this plugin has been loaded by calling its init function.
	bool m_pluginIsLoaded;
	/// The lock guarding this plugin object. Every time a member is accessed this lock should be locked accordingly.
	/// After successful construction and initialization (doInitilize()), this member variable is effectively const
	/// and therefore no locking is required in order to read from it!
	/// In fact protecting read-accesses by a non-recursive lock can introduce deadlocks by plugins using certain
	/// API functions.
	mutable QReadWriteLock m_pluginLock;
	/// The struct holding the function pointers to the functions in the shared library.
	MumblePluginFunctions m_pluginFnc;
	/// A flag indicating whether this plugin is built into Mumble and is thus not represented by a shared library.
	bool m_isBuiltIn;
	/// A flag indicating whether positional data gathering is enabled for this plugin (Enabled as in allowed via
	/// preferences).
	bool m_positionalDataIsEnabled;
	/// A flag indicating whether positional data gathering is currently active (Active as in running)
	bool m_positionalDataIsActive;
	/// A flag indicating whether this plugin has permission to monitor keyboard events that occur while
	/// Mumble has the keyboard focus.
	bool m_mayMonitorKeyboard;


	QString extractWrappedString(MumbleStringWrapper wrapper) const;


	// Most of this class's functions are protected in order to only allow access to them via the PluginManager
	// as some require additional handling before/after calling them.

	/// Initializes this plugin. This function must be called directly after construction. This is guaranteed when the
	/// plugin is created via Plugin::createNew
	virtual bool doInitialize();
	/// Resolves the function pointers in the shared library and sets the respective fields in Plugin::apiFnc
	virtual void resolveFunctionPointers();
	/// Enables positional data gathering for this plugin (as in allowing)
	///
	/// @param enable Whether to enable the data gathering
	virtual void enablePositionalData(bool enable = true);
	/// Allows or forbids the monitoring of keyboard events for this plugin.
	///
	/// @param allow Whether to allow or forbid it
	virtual void allowKeyboardMonitoring(bool allow);


	/// Initializes this plugin
	virtual mumble_error_t init();
	/// Shuts this plugin down
	virtual void shutdown();
	/// Delegates the struct of API function pointers to the plugin backend
	///
	/// @param api The pointer to the API struct
	virtual void registerAPIFunctions(void *api) const;
	/// Asks the plugin to release (free/delete) the resource pointed to by the given pointer
	///
	/// @param pointer Pointer to the resource
	virtual void releaseResource(const void *pointer) const;
	/// Provides the plugin backend with some version information about Mumble
	///
	/// @param mumbleVersion The version of the Mumble client
	/// @param mumbleAPIVersion The API version used by the Mumble client
	/// @param minimalExpectedAPIVersion The minimal API version expected to be used by the plugin backend
	virtual void setMumbleInfo(mumble_version_t mumbleVersion, mumble_version_t mumbleAPIVersion,
							   mumble_version_t minimalExpectedAPIVersion) const;
	/// Asks the plugin to deactivate certain features
	///
	/// @param features The feature list or'ed together
	/// @returns The list of features that couldn't be deactivated or'ed together
	virtual uint32_t deactivateFeatures(uint32_t features) const;
	/// Shows an about-dialog
	///
	/// @parent A pointer to the QWidget that should be used as a parent
	/// @returns Whether the dialog could be shown successfully
	virtual bool showAboutDialog(QWidget *parent) const;
	/// Shows a config-dialog
	///
	/// @parent A pointer to the QWidget that should be used as a parent
	/// @returns Whether the dialog could be shown successfully
	virtual bool showConfigDialog(QWidget *parent) const;
	/// Initializes the positional data gathering
	///
	/// @params programNames A pointer to an array of const char* representing the program names
	/// @params programCount A pointer to an array of PIDs corresponding to the program names
	/// @params programCount The length of the two previous arrays
	virtual uint8_t initPositionalData(const char *const *programNames, const uint64_t *programPIDs,
									   size_t programCount);
	/// Fetches the positional data
	///
	/// @param[out] avatarPos The position of the ingame avatar (player)
	/// @param[out] avatarDir The directiion in which the avatar (player) is looking/facing
	/// @param[out] avatarAxis The vector from the avatar's toes to its head
	/// @param[out] cameraPos The position of the ingame camera
	/// @param[out] cameraDir The direction in which the camera is looking/facing
	/// @param[out] cameraAxis The vector from the camera's bottom to its top
	/// @param[out] context The context of the current game-session (includes server/squad info)
	/// @param[out] identity The ingame identity of the player (name)
	virtual bool fetchPositionalData(Position3D &avatarPos, Vector3D &avatarDir, Vector3D &avatarAxis,
									 Position3D &cameraPos, Vector3D &cameraDir, Vector3D &cameraAxis, QString &context,
									 QString &identity) const;
	/// Shuts down positional data gathering
	virtual void shutdownPositionalData();
	/// @returns The positional data context prefix to be used for this plugin
	virtual QString getPositionalDataContextPrefix() const;
	/// Called to indicate that the client has connected to a server
	///
	/// @param connection An object used to identify the current connection
	virtual void onServerConnected(mumble_connection_t connection) const;
	/// Called to indicate that the client disconnected from a server
	///
	/// @param connection An object used to identify the connection that has been disconnected
	virtual void onServerDisconnected(mumble_connection_t connection) const;
	/// Called to indicate that a user has switched its channel
	///
	/// @param connection An object used to identify the current connection
	/// @param userID The ID of the user that switched channel
	/// @param previousChannelID The ID of the channel the user came from (-1 if there is no previous channel)
	/// æparam newChannelID The ID of the channel the user has switched to
	virtual void onChannelEntered(mumble_connection_t connection, mumble_userid_t userID,
								  mumble_channelid_t previousChannelID, mumble_channelid_t newChannelID) const;
	/// Called to indicate that a user exited a channel.
	///
	/// @param connection An object used to identify the current connection
	/// @param userID The ID of the user that switched channel
	/// @param channelID The ID of the channel the user exited
	virtual void onChannelExited(mumble_connection_t connection, mumble_userid_t userID,
								 mumble_channelid_t channelID) const;
	/// Called to indicate that a user has changed its talking state
	///
	/// @param connection An object used to identify the current connection
	/// @param userID The ID of the user that switched channel
	/// @param talkingState The new talking state of the user
	virtual void onUserTalkingStateChanged(mumble_connection_t connection, mumble_userid_t userID,
										   mumble_talking_state_t talkingState) const;
	/// Called to indicate that a data packet has been received
	///
	/// @param connection An object used to identify the current connection
	/// @param sender The ID of the user whose client sent the data
	/// @param data The actual data
	/// @param dataLength The length of the data array
	/// @param datID The ID of the data used to determine whether this plugin handles this data or not
	/// @returns Whether this plugin handled the data
	virtual bool onReceiveData(mumble_connection_t connection, mumble_userid_t sender, const uint8_t *data,
							   size_t dataLength, const char *dataID) const;
	/// Called to indicate that there is audio input
	///
	/// @param inputPCM A pointer to a short array representing the input PCM
	/// @param sampleCount The amount of samples per channel
	/// @param channelCount The amount of channels in the PCM
	/// @param sampleRate The used sample rate in Hz
	/// @param isSpeech Whether Mumble considers the input as speech
	/// @returns Whether this plugin has modified the audio
	virtual bool onAudioInput(short *inputPCM, uint32_t sampleCount, uint16_t channelCount, uint32_t sampleRate,
							  bool isSpeech) const;
	/// Called to indicate that an audio source has been fetched
	///
	/// @param outputPCM A pointer to a short array representing the output PCM
	/// @param sampleCount The amount of samples per channel
	/// @param channelCount The amount of channels in the PCM
	/// @param sampleRate The used sample rate in Hz
	/// @param isSpeech Whether Mumble considers the output as speech
	/// @param userID The ID of the user responsible for the output (only relevant if isSpeech == true)
	/// @returns Whether this plugin has modified the audio
	virtual bool onAudioSourceFetched(float *outputPCM, uint32_t sampleCount, uint16_t channelCount,
									  uint32_t sampleRate, bool isSpeech, mumble_userid_t userID) const;
	/// Called to indicate that audio is about to be played
	///
	/// @param outputPCM A pointer to a short array representing the output PCM
	/// @param sampleCount The amount of samples per channel
	/// @param channelCount The amount of channels in the PCM
	/// @param sampleRate The used sample rate in Hz
	/// @returns Whether this plugin has modified the audio
	virtual bool onAudioOutputAboutToPlay(float *outputPCM, uint32_t sampleCount, uint16_t channelCount,
										  uint32_t sampleRate) const;
	/// Called when the server has synchronized with the client
	///
	/// @param connection An object used to identify the current connection
	virtual void onServerSynchronized(mumble_connection_t connection) const;
	/// Called when a new user gets added to the user model. This is the case when that new user freshly connects to the
	/// server the local user is on but also when the local user connects to a server other clients are already
	/// connected to (in this case this method will be called for every client already on that server).
	///
	/// @param connection An object used to identify the current connection
	/// @param userID The ID of the user that has been added
	virtual void onUserAdded(mumble_connection_t connection, mumble_userid_t userID) const;
	/// Called when a user gets removed from the user model. This is the case when that user disconnects from the server
	/// the local user is on but also when the local user disconnects from a server other clients are connected to (in
	/// this case this method will be called for every client on that server).
	///
	/// @param connection An object used to identify the current connection
	/// @param userID The ID of the user that has been removed
	virtual void onUserRemoved(mumble_connection_t connection, mumble_userid_t userID) const;
	/// Called when a new channel gets added to the user model. This is the case when a new channel is created on the
	/// server the local user is on but also when the local user connects to a server that contains channels other than
	/// the root-channel (in this case this method will be called for ever non-root channel on that server).
	///
	/// @param connection An object used to identify the current connection
	/// @param channelID The ID of the channel that has been added
	virtual void onChannelAdded(mumble_connection_t connection, mumble_channelid_t channelID) const;
	/// Called when a channel gets removed from the user model. This is the case when a channel is removed on the server
	/// the local user is on but also when the local user disconnects from a server that contains channels other than
	/// the root-channel (in this case this method will be called for ever non-root channel on that server).
	///
	/// @param connection An object used to identify the current connection
	/// @param channelID The ID of the channel that has been removed
	virtual void onChannelRemoved(mumble_connection_t connection, mumble_channelid_t channelID) const;
	/// Called when a channel gets renamed. This also applies when a new channel is created (thus assigning it an
	/// initial name is also considered renaming).
	///
	/// @param connection An object used to identify the current connection
	/// @param channelID The ID of the channel that has been renamed
	virtual void onChannelRenamed(mumble_connection_t connection, mumble_channelid_t channelID) const;
	/// Called when a key has been pressed or released while Mumble has keyboard focus.
	///
	/// @param keyCode The key code of the respective key. The character codes are defined
	/// 	via the KeyCode enum. For printable 7-bit ASCII characters these codes conform
	/// 	to the ASCII code-page with the only difference that case is not distinguished. Therefore
	/// 	always the upper-case letter code will be used for letters.
	/// @param wasPress Whether the key has been pressed (instead of released)
	virtual void onKeyEvent(mumble_keycode_t keyCode, bool wasPress) const;


public:
	/// A template function for instantiating new plugin objects and initializing them. The plugin will be allocated on
	/// the heap and has thus to be deleted via the delete instruction.
	///
	/// @tparam T The type of the plugin to be instantiated
	/// @tparam Ts The types of the constructor arguments
	/// @param args A list of args passed to the constructor of the plugin object
	/// @returns A pointer to the allocated plugin
	///
	/// @throws PluginError if the plugin could not be loaded
	template< typename T, typename... Ts > static T *createNew(Ts &&... args) {
		static_assert(std::is_base_of< Plugin, T >::value,
					  "The Plugin::create() can only be used to instantiate objects of base-type Plugin");
		static_assert(!std::is_pointer< T >::value,
					  "Plugin::create() can't be used to instantiate pointers. It will return a pointer automatically");

		T *instancePtr = new T(std::forward< Ts >(args)...);

		// call the initialize-method and throw an exception of it doesn't succeed
		if (!instancePtr->doInitialize()) {
			delete instancePtr;
			// Delete the constructed object to prevent a memory leak
			throw PluginError("Failed to initialize plugin");
		}

		return instancePtr;
	}

	/// Destructor
	virtual ~Plugin() Q_DECL_OVERRIDE;
	/// @returns Whether this plugin is in a valid state
	virtual bool isValid() const;
	/// @returns Whether this plugin is loaded (has been initialized via Plugin::init())
	virtual bool isLoaded() const Q_DECL_FINAL;
	/// @returns The unique ID of this plugin. This ID holds only as long as this plugin isn't "reconstructed".
	virtual plugin_id_t getID() const Q_DECL_FINAL;
	/// @returns Whether this plugin is built into Mumble (thus not backed by a shared library)
	virtual bool isBuiltInPlugin() const Q_DECL_FINAL;
	/// @returns The path to the shared library in the host's filesystem
	virtual QString getFilePath() const;
	/// @returns Whether positional data gathering is enabled (as in allowed via preferences)
	virtual bool isPositionalDataEnabled() const Q_DECL_FINAL;
	/// @returns Whether positional data gathering is currently active (as in running)
	virtual bool isPositionalDataActive() const Q_DECL_FINAL;
	/// @returns Whether this plugin is currently allowed to monitor keyboard events
	virtual bool isKeyboardMonitoringAllowed() const Q_DECL_FINAL;


	/// @returns Whether this plugin provides an about-dialog
	virtual bool providesAboutDialog() const;
	/// @returns Whether this plugin provides an config-dialog
	virtual bool providesConfigDialog() const;
	/// @returns The name of this plugin
	virtual QString getName() const;
	/// @returns The API version this plugin intends to use
	virtual mumble_version_t getAPIVersion() const;
	/// @returns The version of the plugins function scheme this plugin is using
	virtual mumble_version_t getPluginFunctionsVersion() const;
	/// @returns The version of this plugin
	virtual mumble_version_t getVersion() const;
	/// @returns The author of this plugin
	virtual QString getAuthor() const;
	/// @returns The plugin's description
	virtual QString getDescription() const;
	/// @returns The plugin's features or'ed together (See the PluginFeature enum in MumblePlugin.h for what features
	/// are available)
	virtual uint32_t getFeatures() const;
	/// @return Whether the plugin has found a new/updated version of itself available for download
	virtual bool hasUpdate() const;
	/// @return The URL to download the updated plugin. May be empty
	virtual QUrl getUpdateDownloadURL() const;
};

#endif
