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
		bool isRunning();
		void start() throws ServerBootedException;
		void stop() throws ServerBootedException;
		void delete() throws ServerBootedException;
		int id();

		string getConf(string key);
		ConfigMap getAllConf();
		void setConf(string key, string value);
		void setSuperuserPasssword(string pw);
		LogList getLog(int min, int max);

		PlayerMap getPlayers();
		ChannelMap getChannels();
		Tree getTree();

		BanList getBans();
		void setBans(BanList bans);

		void kickPlayer(int session, string reason) throws InvalidSessionException;
		Player getState(int session) throws InvalidSessionException;
		void setState(Player state) throws InvalidSessionException;
		
		Channel getChannelState(int channelid) throws InvalidChannelException;
		void setChannelState(Channel state) throws InvalidChannelException;

		void removeChannel(int channelid) throws InvalidChannelException;
		int addChannel(string name, int parent) throws InvalidChannelException;

		void getACL(int channelid, out ACLList acls, out GroupList groups, out bool inherit) throws InvalidChannelException;
		void setACL(int channelid, ACLList acls, GroupList groups, bool inherit) throws InvalidChannelException;

		NameMap getPlayerNames(IdList ids);
		IdMap getPlayerIds(NameList names);

		int registerPlayer(string name);
		void unregisterPlayer(int playerid);
		void updateregistration(RegisteredPlayer registration);
		RegisteredPlayer getRegistration(int playerid);
		RegisteredPlayerList getRegisteredPlayers(string filter);
		int verifyPassword(string playerid, string pw);
		Texture getTexture(int playerid);
		void setTexture(int playerid, Texture tex);
	};

	sequence<Server *> ServerList;

	interface Meta {
		Server *getServer(int id) throws InvalidServerException;
		Server *newServer();
		ServerList getBootedServers();
		ServerList getAllServers();
		ConfigMap getDefaultConf();
	};
};
