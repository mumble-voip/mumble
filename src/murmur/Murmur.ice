/**
 *
 * Information and control of the murmur server. Each server has
 * one [Meta] interface that controls global information, and
 * each virtual server has a [Server] interface.
 *
 **/

module Murmur
{
	/** A connected player.
         **/
	struct Player {
		/** Session ID. This identifies the connection to the server. */
		int session;
		/** Player ID. Matches [RegisteredPlayer::playerid] or -1 if the player is anonymous. */
		int playerid;
		/** Is player muted by the server? */
		bool mute;
		/** Is player deafened by the server? If true, this implies mute. */
		bool deaf;
		/** Is the player suppressed by the server? This means the user is not muted, but does not have speech privileges in the current channel. */
		bool suppressed;
		/** Is the player self-muted? */
		bool selfMute;
		/** Is the player self-deafened? If true, this implies mute. */
		bool selfDeaf;
		/** Channel ID the player is in. Matches [Channel::id]. */
		int channel;
		/** The name of the player. */
		string name;
		/** Seconds player has been online. */
		int onlinesecs;
		/** Average transmission rate in bytes per second over the last few seconds. */
		int bytespersec;
	};

	sequence<int> IntList;

	/** A channel.
	 **/
	struct Channel {
		/** Channel ID. This is unique per channel, and the root channel is always id 0. */
		int id;
		/** Name of the channel. There can not be two channels with the same parent that has the same name. */
		string name;
		/** ID of parent channel, or -1 if this is the root channel. */
		int parent;
		/** List of id of linked channels. */
		IntList links;
	};

	/** A group. Groups are defined per channel, and can inherit members from parent channels.
	 **/
	struct Group {
		/** Group name */
		string name;
		/** Is this group inherited from a parent channel? Read-only. */
		bool inherited;
		/** Does this group inherit members from parent channels? */
		bool inherit;
		/** Can subchannels inherit members from this group? */
		bool inheritable;
		/** List of players to add to the group. */
		IntList add;
		/** List of inherited players to remove from the group. */
		IntList remove;
		/** Current members of the group, including inherited members. Read-only. */
		IntList members;
	};

	/** Write access to channel control. Implies all other permissions (except Speak). */
	const int PermissionWrite = 0x01;
	/** Traverse channel. Without this, a client cannot reach subchannels, no matter which privileges he has there. */
	const int PermissionTraverse = 0x02;
	/** Enter channel. */
	const int PermissionEnter = 0x04;
	/** Speak in channel. */
	const int PermissionSpeak = 0x08;
	/** Alt-speak in channel. This is different from Speak, so you can set up different permissions. */
	const int PermissionAltSpeak = 0x100;
	/** Mute and deafen other players in this channel. */
	const int PermissionMuteDeafen = 0x10;
	/** Move and Kick players from channel. You need this permission in both the source and destination channel to move another player.
         *  If you have this privilege on the root channel, you can ban players.
	 */
        const int PermissionMoveKick = 0x20;
	/** Make new channel as a subchannel of this channel. */
	const int PermissionMakeChannel = 0x40;
	/** Link this channel. You need this permission in both the source and destination channel to link channels, or in either channel to unlink them. */
	const int PermissionLinkChannel = 0x80;

	/** Access Control List for a channel. ACLs are defined per channel, and can be inherited from parent channels.
	 **/
	struct ACL {
		/** Does the ACL apply to this channel? */
		bool applyHere;
		/** Does the ACL apply to subchannels? */
		bool applySubs;
		/** Is this ACL inherited from a parent channel? Read-only. */
		bool inherited;
		/** ID of player this ACL applies to. -1 if using a group name. */
		int playerid;
		/** Group this ACL applies to. Blank if using playerid. */
		string group;
		/** Binary mask of privileges to allow. */
		int allow;
		/** Binary mask of privileges to deny. */
		int deny;
	};

	/** A single ip mask for a ban.
         **/
	struct Ban {
		/** IPv4 address to ban, represented as a single integer. */
		int address;
		/** Number of bits in ban to apply. This uses CIDR notation, so putting 24 here is the equivalent of netmask 255.255.255.0. */
		int bits;
	};

	/** A registered player.
	 **/
	struct RegisteredPlayer {
		/** Player id. Matches the playerid in [Player::playerid]. */
		int playerid;
		/** Player name. */
		string name;
		/** Player email. */
		string email;
		/** Player password. Write-only. */
		string pw;
	};

	/** A entry in the log.
	 **/
	struct LogEntry {
		/** Timestamp in UNIX time_t */
		int timestamp;
		/** The log message. */
		string txt;
	};

	class Tree;
	sequence<Tree> TreeList;

	dictionary<int, Player> PlayerMap;
	dictionary<int, Channel> ChannelMap;
	sequence<Channel> ChannelList;
	sequence<Player> PlayerList;
	sequence<Group> GroupList;
	sequence<ACL> ACLList;
	sequence<LogEntry> LogList;
	sequence<Ban> BanList;
	sequence<int> IdList;
	sequence<string> NameList;
	dictionary<int, string> NameMap;
	dictionary<string, int> IdMap;
	sequence<RegisteredPlayer> RegisteredPlayerList;
	sequence<byte> Texture;
	dictionary<string, string> ConfigMap;
	sequence<string> GroupNameList;

	/** Player and subchannel state. Read-only.
         **/
	class Tree {
		/** Channel definition of current channel. */
		Channel c;
		/** List of subchannels. */
		TreeList children;
		/** Players in this channel. */
		PlayerList players;
	};

	exception MurmurException {};
	/** This is thrown when you specify an invalid session. This may happen if the player has disconnected since your last call to [Server::getPlayers]. See [Player::session] */
	exception InvalidSessionException extends MurmurException {};
	/** This is thrown when you specify an invalid channel id. This may happen if the channel was removed by another provess. It can also be thrown if you try to add an invalid channel. */
	exception InvalidChannelException extends MurmurException {};
	/** This is thrown when you try to do an operation on a server that does not exist. This may happen if someone has removed the server. */
	exception InvalidServerException extends MurmurException {};
	/** This happens if you try to fetch player or channel state on a stopped server, if you try to stop an already stopped server or start an already started server. */
	exception ServerBootedException extends MurmurException {};
	/** This is thrown if [Server::start] fails, and should generally be the cause for some concern. */
	exception ServerFailureException extends MurmurException {};
	/** This is thrown when you specify an invalid playerid. See [RegisteredPlayer::playerid]. */
	exception InvalidPlayerException extends MurmurException {};
	/** This is thrown when you try to set an invalid texture. */
	exception InvalidTextureException extends MurmurException {};
	/** This is thrown when you supply an invalid callback. */
	exception InvalidCallbackException extends MurmurException {};

	/** Callback interface for servers. You can supply an implementation of this to receive notification
	 *  messages from the server.
	 *  If an added callback ever throws an exception or goes away, it will be automatically removed.
	 *  Please note that all callbacks are done asynchronously; murmur does not wait for the callback to
	 *  complete before continuing processing.
	 *  Note that callbacks are removed when a server is stopped, so you should have a callback for
	 *  [MetaCallback::started] which calls [Server::addCallback].
	 *  @see MetaCallback
	 *  @see Server::addCallback
	 */
	interface ServerCallback {
		/** Called when a player connects to the server. 
		 *  @param state State of connected player.
		 */ 
		idempotent void playerConnected(Player state);
		/** Called when a player disconnects from the server. The player has already been removed, so you can no longer use methods like [Server::getState]
		 *  to retrieve the player's state.
		 *  @param state State of disconnected player.
		 */ 
		idempotent void playerDisconnected(Player state);
		/** Called when a player state changes. This is called if the player moves, is renamed, is muted, deafened etc.
		 *  @param state New state of player.
		 */ 
		idempotent void playerStateChanged(Player state);
		/** Called when a new channel is created. 
		 *  @param state State of new channel.
		 */ 
		idempotent void channelCreated(Channel state);
		/** Called when a channel is removed. The channel has already been removed, you can no longer use methods like [Server::getChannelState]
		 *  @param state State of removed channel.
		 */ 
		idempotent void channelRemoved(Channel state);
		/** Called when a new channel state changes. This is called if the channel is moved, renamed or if new links are added.
		 *  @param state New state of channel.
		 */ 
		idempotent void channelStateChanged(Channel state);
	};

	/** Context for actions in the Server menu. */
	const int ContextServer = 0x01;
	/** Context for actions in the Channel menu. */
	const int ContextChannel = 0x02;
	/** Context for actions in the Player menu. */
	const int ContextPlayer = 0x04;

	/** Callback interface for context actions. You need to supply one of these for [Server::addContext]. 
	 *  If an added callback ever throws an exception or goes away, it will be automatically removed.
	 *  Please note that all callbacks are done asynchronously; murmur does not wait for the callback to
	 *  complete before continuing processing.
	 */
	interface ServerContextCallback {
		/** Called when a context action is performed.
		 *  @param action Action to be performed.
		 *  @param user User which initiated the action.
		 *  @param session If nonzero, session of target player.
		 *  @param channelid If nonzero, session of target channel.
		 */
		idempotent void contextAction(string action, Player user, int session, int channelid);
	};

	/** Callback interface for server authentication. You need to supply one of these for [Server::setAuthenticator].
	 *  If an added callback ever throws an exception or goes away, it will be automatically removed.
	 *  Please note that unlike [ServerCallback] and [ServerContextCallback], these methods are called
	 *  synchronously. If the response lags, the entire murmur server will lag.
	 *  Also note that, as the method calls are synchronous, calling a function from [Server] or [Meta] will
	 *  deadlock the server.
	 */
	interface ServerAuthenticator {
		/** Called to authenticate a player. If you do not know the playername in question, always return -2 from this
		 *  method to fall through to normal database authentication.
		 *  Note that if authentication succeeds, murmur will create a record of the player in it's database, reserving
		 *  the playername and id so it cannot be used for normal database authentication.
		 *
		 *  @param name Playername to authenticate.
		 *  @param pw Password to authenticate with.
		 *  @param newname Set this to change the playername from the supplied one.
		 *  @param groups List of groups on the root channel that the player will be added to for the duration of the connection.
		 *  @return PlayerID of authenticated player, -1 for authentication failures and -2 for unknown player (fallthrough).
		 */
		idempotent int authenticate(string name, string pw, out string newname, out GroupNameList groups);

		/** Map a name to a player id.
		 *  @param name Playername to map.
		 *  @return Player id or -2 for unknown name.
		 */
		idempotent int nameToId(string name);

		/** Map a player to a Player id.
		 *  @param id Player id to map.
		 *  @return Name of player or empty string for unknown id.
		 */
		idempotent string idToName(int id);

		/** Map a player to a custom Texture.
		 *  @param id Player id to map.
		 *  @return User texture or an empty texture for unknwon players or players without textures.
		 */
		idempotent Texture idToTexture(int id);
	};

	/** Callback interface for server authentication and registration. This allows you to support both authentication
	 *  and account updating.
	 *  You do not need to implement this if all you want is authentication, you only need this if other scripts
	 *  connected to the same server calls e.g. [Server::setTexture].
	 *  Almost all of these methods all fall through, meaning murmur should continue the operation against its
	 *  own database.
	 */
	interface ServerUpdatingAuthenticator extends ServerAuthenticator {
		/** Register a new player.
		 *  @param name Playername to register.
		 *  @return Player id of new player, -1 for registration failure, or -2 to fall through.
		 */
		int registerPlayer(string name);

		/** Unregister a player.
		 *  @param id Playerid to unregister.
		 *  @return 1 for successfull unregistration, 0 for unsuccessfull unregistration, -1 to fall through.
		 */
		int unregisterPlayer(int id);

		/** Get a list of registered players matching filter.
		 *  @param filter Substring playernames must contain. If empty, return all registered players.
		 *  @return List of matching registered players.
		 */
		idempotent RegisteredPlayerList getRegisteredPlayers(string filter);

		/** Get single player registration.
		 *  @param id Playerid of registered player.
		 *  @param name Name of registered player.
		 *  @param email Email address of registered player.
		 *  @return 1 for successfull fetch, 0 for unsuccessfull fetch, -1 to fall through.
		 */
		idempotent int getRegistration(int id, out string name, out string email);

		/** Set password of player registration.
		 *  @param id Playerid of registered player.
		 *  @param pw New password.
		 *  @return 1 for successfull update, 0 for unsuccessfull update, -1 to fall through.
		 */
		idempotent int setPassword(int id, string pw);

		/** Set Email address of player registration.
		 *  @param id Playerid of registered player.
		 *  @param pw email New Email address.
		 *  @return 1 for successfull update, 0 for unsuccessfull update, -1 to fall through.
		 */
		idempotent int setEmail(int id, string email);

		/** Set playername of player registration.
		 *  @param id Playerid of registered player.
		 *  @param name New Playername.
		 *  @return 1 for successfull update, 0 for unsuccessfull update, -1 to fall through.
		 */
		idempotent int setName(int id, string name);

		/** Set texture of player registration.
		 *  @param id Playerid of registered player.
		 *  @param tex New texture.
		 *  @return 1 for successfull update, 0 for unsuccessfull update, -1 to fall through.
		 */
		idempotent int setTexture(int id, Texture tex);
	};

	/** Per-server interface. This includes all methods for configuring and altering
         * the state of a single virtual server. You can retrieve a pointer to this interface
         * from one of the methods in [Meta].
         **/
	["amd"] interface Server {
		/** Shows if the server currently running (accepting players).
		 *
		 * @return Run-state of server.
		 */
		idempotent bool isRunning();

		/** Start server. */
		void start() throws ServerBootedException, ServerFailureException;

		/** Stop server. */
		void stop() throws ServerBootedException;

		/** Delete server and all it's configuration. */
		void delete() throws ServerBootedException;

		/** Fetch the server id.
		 *
		 * @return Unique server id.
		 */
		idempotent int id();

		/** Add a callback. The callback will receive notifications about changes to players and channels.
		 *
		 * @param cb Callback interface which will receive notifications.
		 * @see removeCallback
		 */
		void addCallback(ServerCallback *cb) throws ServerBootedException, InvalidCallbackException;

		/** Remove a callback.
		 *
		 * @param cb Callback interface to be removed.
		 * @see addCallback
		 */
		void removeCallback(ServerCallback *cb) throws ServerBootedException, InvalidCallbackException;

		/** Set external authenticator. If set, all authentications from clients are forwarded to this
		 *  proxy.
		 *
		 * @param auth Authenticator object to perform subsequent authentications.
		 */
		void setAuthenticator(ServerAuthenticator *auth) throws ServerBootedException, InvalidCallbackException;

		/** Retrieve configuration item.
		 * @param key Configuration key.
		 * @return Configuration value. If this is empty, see [Meta::getDefaultConf]
		 */
		idempotent string getConf(string key);

		/** Retrieve all configuration items.
		 * @return All configured values. If a value isn't set here, the value from [Meta::getDefaultConf] is used.
		 */
		idempotent ConfigMap getAllConf();

		/** Set a configuration item.
		 * @param key Configuration key.
		 * @param value Configuration value.
		 */
		idempotent void setConf(string key, string value);

		/** Set superuser password. This is just a convenience for using [updateregistration] on player id 0.
		 * @param pw Password.
		 */
		idempotent void setSuperuserPassword(string pw);

		/** Fetch log entries.
		 * @param first Lowest numbered entry to fetch. 0 is the most recent item.
		 * @param last Last entry to fetch.
		 * @return List of log entries.
		 */
		idempotent LogList getLog(int first, int last);

		/** Fetch all players. This returns all currently connected players on the server.
		 * @return List of connected players.
		 * @see getState
		 */
		idempotent PlayerMap getPlayers() throws ServerBootedException;

		/** Fetch all channels. This returns all defined channels on the server. The root channel is always channel 0.
		 * @return List of defined channels.
		 * @see getChannelState
		 */
		idempotent ChannelMap getChannels() throws ServerBootedException;

		/** Fetch all channels and connected players as a tree. This retrieves an easy-to-use representation of the server
		 *  as a tree. This is primarily used for viewing the state of the server on a webpage.
		 * @return Recursive tree of all channels and connected players.
		 */
		idempotent Tree getTree() throws ServerBootedException;

		/** Fetch all current IP bans on the server.
		 * @return List of bans.
		 */
		idempotent BanList getBans() throws ServerBootedException;

		/** Set all current IP bans on the server. This will replace any bans already present, so if you want to add a ban, be sure to call [getBans] and then
		 *  append to the returned list before calling this method.
		 * @param bans List of bans.
		 */
		idempotent void setBans(BanList bans) throws ServerBootedException;

		/** Kick a player. The player is not banned, and is free to rejoin the server.
		 * @param session Connection ID of player. See [Player::session].
		 * @param reason Text message to show when player is kicked.
		 */
		void kickPlayer(int session, string reason) throws ServerBootedException, InvalidSessionException;

		/** Get state of a single connected player. 
		 * @param session Connection ID of player. See [Player::session].
		 * @return State of connected player.
		 * @see setState
		 * @see getPlayers
		 */
		idempotent Player getState(int session) throws ServerBootedException, InvalidSessionException;

		/** Set player state. You can use this to move, mute and deafen players.
		 * @param state Player state to set.
		 * @see getState
		 */
		idempotent void setState(Player state) throws ServerBootedException, InvalidSessionException, InvalidChannelException;

		/** Send text message to a single player.
		 * @param session Connection ID of player. See [Player::session].
		 * @param text Message to send.
		 * @see sendMessageChannel
		 */
		void sendMessage(int session, string text) throws ServerBootedException, InvalidSessionException;

		/** Check if player is permitted to perform action.
		 * @param session Connection ID of player. See [Player::session].
		 * @param channelid ID of Channel. See [Channel::id].
		 * @param perm Permission bits to check.
		 * @return true if any of the permissions in perm were set for the user.
		 */
		bool hasPermission(int session, int channelid, int perm) throws ServerBootedException, InvalidSessionException, InvalidChannelException;

		/** Add a context callback. This is done per player, and will add a context menu action for the player.
		 *
		 * @param session Session of user which should receive context entry.
		 * @param action Action string, a unique name to associate with the action.
		 * @param text Name of action shown to user.
		 * @param cb Callback interface which will receive notifications.
		 * @param ctx Context this should be used in. Needs to be one or a combination of [ContextServer], [ContextChannel] and [ContextPlayer].
		 * @see removeContextCallback
		 */
		void addContextCallback(int session, string action, string text, ServerContextCallback *cb, int ctx) throws ServerBootedException, InvalidCallbackException;

		/** Remove a callback.
		 *
		 * @param cb Callback interface to be removed. This callback will be removed from all from all players.
		 * @see addContextCallback
		 */
		void removeContextCallback(ServerContextCallback *cb) throws ServerBootedException, InvalidCallbackException;
		
		/** Get state of single channel.
		 * @param channelid ID of Channel. See [Channel::id].
		 * @return State of channel.
		 * @see setChannelState
		 * @see getChannels
		 */
		idempotent Channel getChannelState(int channelid) throws ServerBootedException, InvalidChannelException;

		/** Set state of a single channel. You can use this to move or relink channels.
		 * @param state Channel state to set.
		 * @see getChannelState
		 */
		idempotent void setChannelState(Channel state) throws ServerBootedException, InvalidChannelException;

		/** Remove a channel and all its subchannels.
		 * @param channelid ID of Channel. See [Channel::id].
		 */
		void removeChannel(int channelid) throws ServerBootedException, InvalidChannelException;

		/** Add a new channel.
		 * @param name Name of new channel.
		 * @param parent Channel ID of parent channel. See [Channel::id].
		 * @return ID of newly created channel.
		 */
		int addChannel(string name, int parent) throws ServerBootedException, InvalidChannelException;

		/** Send text message to channel or a tree of channels.
		 * @param channelid Channel ID of channel to send to. See [Channel::id].
		 * @param tree If true, the message will be sent to the channel and all its subchannels.
		 * @param text Message to send.
		 * @see sendMessage
		 */
		void sendMessageChannel(int channelid, bool tree, string text) throws ServerBootedException, InvalidChannelException;

		/** Retrieve ACLs and Groups on a channel.
		 * @param channelid Channel ID of channel to fetch from. See [Channel::id].
		 * @param acls List of ACLs on the channel. This will include inherited ACLs.
		 * @param groups List of groups on the channel. This will include inherited groups.
		 * @param inherit Does this channel inherit ACLs from the parent channel?
		 */
		idempotent void getACL(int channelid, out ACLList acls, out GroupList groups, out bool inherit) throws ServerBootedException, InvalidChannelException;

		/** Set ACLs and Groups on a channel. Note that this will replace all existing ACLs and groups on the channel.
		 * @param channelid Channel ID of channel to fetch from. See [Channel::id].
		 * @param acls List of ACLs on the channel.
		 * @param groups List of groups on the channel.
		 * @param inherit Should this channel inherit ACLs from the parent channel?
		 */
		idempotent void setACL(int channelid, ACLList acls, GroupList groups, bool inherit) throws ServerBootedException, InvalidChannelException;

		/** Map a list of [Player::playerid] or [RegisteredPlayer::playerid] to a matching name.
		 * @param List of ids.
		 * @return Matching list of names, with an empty string representing invalid or unknown ids.
		 */
		idempotent NameMap getPlayerNames(IdList ids) throws ServerBootedException;

		/** Map a list of player names to a matching [RegisteredPlayer::playerid].
		 * @param List of names.
		 * @reuturn List of matching ids, with -1 representing invalid or unknown player names.
		 */
		idempotent IdMap getPlayerIds(NameList names) throws ServerBootedException;

		/** Register a new player.
		 * @param name Name of player to register.
		 * @return The ID of the player. See [RegisteredPlayer::playerid].
		 */
		int registerPlayer(string name) throws ServerBootedException, InvalidPlayerException;
		
		/** Remove a player registration.
		 * @param playerid ID of registered player. See [RegisteredPlayer::playerid].
		 */
		void unregisterPlayer(int playerid) throws ServerBootedException, InvalidPlayerException;

		/** Update the registration for a player. You can use this to set the email or password of a player,
		 * and can also use it to change the player's name.
		 * @param registration Updated registration record.
		 */
		idempotent void updateregistration(RegisteredPlayer registration) throws ServerBootedException, InvalidPlayerException;

		/** Fetch registration for a single player.
		 * @param playerid ID of registered player. See [RegisteredPlayer::playerid].
		 * @return Registration record.
		 */
		idempotent RegisteredPlayer getRegistration(int playerid) throws ServerBootedException, InvalidPlayerException;

		/** Fetch a group of registered players.
		 * @param filter Substring of player name. If blank, will retrieve all registered players.
		 * @return List of registration records.
		 */
		idempotent RegisteredPlayerList getRegisteredPlayers(string filter) throws ServerBootedException;

		/** Verify the password of a player. You can use this to verify a player's credentials.
		 * @param name Player name. See [RegisteredPlayer::name].
		 * @param pw Player password.
		 * @return Player ID of registered player (See [RegisteredPlayer::playerid]), -1 for failed authentication or -2 for unknown usernames.
		 */
		idempotent int verifyPassword(string name, string pw) throws ServerBootedException;

		/** Fetch user texture. Textures are stored as zlib compress()ed 600x60 32-bit RGBA data.
		 * @param playerid ID of registered player. See [RegisteredPlayer::playerid].
		 * @return Custom texture associated with player or an empty texture.
		 */
		idempotent Texture getTexture(int playerid) throws ServerBootedException, InvalidPlayerException;

		/** Set user texture. The texture is a 600x60 32-bit RGBA raw texture, optionally zlib compress()ed.
		 * @param playerid ID of registered player. See [RegisteredPlayer::playerid].
		 * @param tex Texture to set for the player, or an empty texture to remove the existing texture.
		 */
		idempotent void setTexture(int playerid, Texture tex) throws ServerBootedException, InvalidPlayerException, InvalidTextureException;
	};

	/** Callback interface for Meta. You can supply an implementation of this to recieve notifications
	 *  when servers are stopped or started.
	 *  If an added callback ever throws an exception or goes away, it will be automatically removed.
	 *  Please note that all callbacks are done asynchronously; murmur does not wait for the callback to
	 *  complete before continuing processing.
	 *  @see ServerCallback
	 *  @see Meta::addCallback
	 */
	interface MetaCallback {
		/** Called when a server is started. The server is up and running when this event is sent, so all methods that 
		 *  need a running server will work.
		 *  @param srv Interface for started server.
		 */
		void started(Server *srv);

		/** Called when a server is stopped. The server is already stopped when this event is sent, so no methods that
		 *  need a running server will work.
		 *  @param srv Interface for started server.
		 */
		void stopped(Server *srv);
	};

	sequence<Server *> ServerList;

	/** This is the meta interface. It is primarily used for retrieving the [Server] interfaces for each individual server.
	 **/
	["amd"] interface Meta {
		/** Fetch interface to specific server.
		 * @param id Server ID. See [Server::getId].
		 * @return Interface for specified server, or a null proxy if id is invalid.
		 */
		idempotent Server *getServer(int id);

		/** Create a new server. Call [Server::getId] on the returned interface to find it's ID.
		 * @return Interface for new server.
		 */
		Server *newServer();

		/** Fetch list of all currently running servers.
		 * @return List of interfaces for running servers.
		 */
		idempotent ServerList getBootedServers();

		/** Fetch list of all defined servers.
		 * @return List of interfaces for all servers.
		 */
		idempotent ServerList getAllServers();

		/** Fetch default configuraion. This returns the configuration items that were set in the configuration file, or
		 * the built-in default. The individual servers will use these values unless they have been overridden in the
		 * server specific configuration. The only special case is the port, which defaults to the value defined here +
		 * the servers ID - 1 (so that virtual server #1 uses the defined port, server #2 uses port+1 etc).
		 * @return Default configuration of the servers.
		 */
		idempotent ConfigMap getDefaultConf();

		/** Fetch version of Murmur. 
		 * @param major Major version.
		 * @param minor Minor version.
		 * @param patch Patchlevel.
		 * @param text Textual representation of version. Note that this may not match the [major], [minor] and [patch] levels, as it
		 *   may be simply the compile date or the SVN revision. This is usually the text you want to present to users.
		 */
		idempotent void getVersion(out int major, out int minor, out int patch, out string text);

		/** Add a callback. The callback will receive notifications when servers are started or stopped.
		 *
		 * @param cb Callback interface which will receive notifications.
		 */
		void addCallback(MetaCallback *cb) throws InvalidCallbackException;

		/** Remove a callback.
		 *
		 * @param cb Callback interface to be removed.
		 */
		void removeCallback(MetaCallback *cb) throws InvalidCallbackException;
	};
};
