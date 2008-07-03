module Murmur
{
	struct Player {
		int session;
		int playerid;
		bool mute;
		bool deaf;
		bool suppressed;
		bool selfMute;
		bool selfDeaf;
		int channel;
		string name;
		int onlinesecs;
		int bytespersec;
	};

	sequence<int> IntList;

	struct Channel {
		int id;
		string name;
		int parent;
		IntList links;
	};

	struct Group {
		string name;
		bool inherited;
		bool inherit;
		bool inheritable;
		IntList add;
		IntList remove;
		IntList members;
	};

	struct ACL {
		bool applyHere;
		bool applySubs;
		bool inherited;
		int playerid;
		string group;
		int allow;
		int deny;
	};

	struct Ban {
		int address;
		int bits;
	};

	struct RegisteredPlayer {
		int playerid;
		string name;
		string email;
		string pw;
	};

	struct LogEntry {
		int timestamp;
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

	class Tree {
		Channel c;
		TreeList children;
		PlayerList players;
	};


	exception MurmurException {};
	exception InvalidSessionException extends MurmurException {};
	exception InvalidChannelException extends MurmurException {};
	exception InvalidServerException extends MurmurException {};
	exception ServerBootedException extends MurmurException {};
	exception ServerFailureException extends MurmurException {};
	exception InvalidPlayerException extends MurmurException {};

	interface Server {
		idempotent bool isRunning();
		void start() throws ServerBootedException, ServerFailureException;
		void stop() throws ServerBootedException;
		void delete() throws ServerBootedException;
		idempotent int id();

		idempotent string getConf(string key);
		idempotent ConfigMap getAllConf();
		idempotent void setConf(string key, string value);
		idempotent void setSuperuserPasssword(string pw);
		idempotent LogList getLog(int min, int max);

		idempotent PlayerMap getPlayers() throws ServerBootedException;
		idempotent ChannelMap getChannels() throws ServerBootedException;
		idempotent Tree getTree() throws ServerBootedException;

		idempotent BanList getBans() throws ServerBootedException;
		idempotent void setBans(BanList bans) throws ServerBootedException;

		void kickPlayer(int session, string reason) throws ServerBootedException, InvalidSessionException;
		idempotent Player getState(int session) throws ServerBootedException, InvalidSessionException;
		idempotent void setState(Player state) throws ServerBootedException, InvalidSessionException, InvalidChannelException;
		
		idempotent Channel getChannelState(int channelid) throws ServerBootedException, InvalidChannelException;
		idempotent void setChannelState(Channel state) throws ServerBootedException, InvalidChannelException;

		void removeChannel(int channelid) throws ServerBootedException, InvalidChannelException;
		int addChannel(string name, int parent) throws ServerBootedException, InvalidChannelException;

		idempotent void getACL(int channelid, out ACLList acls, out GroupList groups, out bool inherit) throws ServerBootedException, InvalidChannelException;
		idempotent void setACL(int channelid, ACLList acls, GroupList groups, bool inherit) throws ServerBootedException, InvalidChannelException;

		idempotent NameMap getPlayerNames(IdList ids) throws ServerBootedException;
		idempotent IdMap getPlayerIds(NameList names) throws ServerBootedException;

		int registerPlayer(string name) throws ServerBootedException;
		void unregisterPlayer(int playerid) throws ServerBootedException, InvalidPlayerException;
		idempotent void updateregistration(RegisteredPlayer registration) throws ServerBootedException, InvalidPlayerException;
		idempotent RegisteredPlayer getRegistration(int playerid) throws ServerBootedException, InvalidPlayerException;
		idempotent RegisteredPlayerList getRegisteredPlayers(string filter) throws ServerBootedException;
		idempotent int verifyPassword(string playerid, string pw) throws ServerBootedException;
		idempotent Texture getTexture(int playerid) throws ServerBootedException, InvalidPlayerException;
		idempotent void setTexture(int playerid, Texture tex) throws ServerBootedException, InvalidPlayerException;
	};

	sequence<Server *> ServerList;

	interface Meta {
		idempotent Server *getServer(int id) throws InvalidServerException;
		Server *newServer();
		idempotent ServerList getBootedServers();
		idempotent ServerList getAllServers();
		idempotent ConfigMap getDefaultConf();
	};
};
