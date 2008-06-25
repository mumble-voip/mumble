module Murmur
{
	struct Player {
		int session;
		bool mute;
		bool deaf;
		bool suppressed;
		bool selfMute;
		bool selfDeaf;
		int channel;
		string name;
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
		int id;
		string name;
		string email;
		string pw;
	};

	struct LogEntry {
		int timestamp;
		string txt;
	};

	sequence<Channel> ChannelList;
	sequence<Player> PlayerList;
	sequence<Group> GroupList;
	sequence<ACL> ACLList;
	sequence<int> ServerList;
	sequence<LogEntry> LogList;
	sequence<Ban> BanList;
	sequence<int> IdList;
	sequence<string> NameList;
	sequence<RegisteredPlayer> RegisteredPlayerList;
	sequence<byte> Texture;
	dictionary<string, string> ConfigMap;

	exception PlayerError {
		int session;
	};

	exception ChannelError {
		int channelid;
	};

	exception NoSuchSession extends PlayerError {};
	exception NoSuchChannel extends ChannelError {};

	interface Server {
		PlayerList getPlayers();
		ChannelList getChannels();

		void getACL(int channelid, out ACLList acls, out GroupList groups, out bool inherit);
		void setACL(int channelid, ACLList acls, GroupList groups, bool inherit);

		BanList getBans();
		void setBans(BanList bans);

		void kickPlayer(int session, string reason) throws NoSuchSession;
		Player getState(int session) throws NoSuchSession;
		void setState(int session, Player state) throws NoSuchSession;
		
		Channel getChannelState(int channelid) throws NoSuchChannel;
		void setChanelState(int channelid, Channel state) throws NoSuchChannel;

		void removeChannel(int channelid) throws NoSuchChannel;
		int addChannel(string name, int parent) throws NoSuchChannel;

		NameList getPlayerNames(IdList ids);
		IdList getPlayerIds(NameList names);

		int registerPlayer(string name);
		void unregisterPlayer(int id);
		void updateregistration(RegisteredPlayer registration);
		RegisteredPlayer getRegistration(int id);
		RegisteredPlayerList getRegisteredPlayers(string filter);
		bool verifyPassword(int id, string pw);
		Texture getTexture(int id);
		void setTexture(int id, Texture tex);
	};


	exception ServerError {
		int id;
	};

	exception NoSuchServer extends ServerError {};
	exception ServerAlreadyBooted extends ServerError {};
	exception ServerNotBooted extends ServerError {};

	interface Meta {
		Server *getServer(int id) throws NoSuchServer;
		void start(int id) throws NoSuchServer, ServerAlreadyBooted;
		int newServer();
		void deleteServer(int id) throws NoSuchServer, ServerAlreadyBooted;
		ServerList getBootedServers();
		ServerList getAllServers();
		bool isBooted(int id) throws NoSuchServer;
		string getConf(int id, string key) throws NoSuchServer;
		ConfigMap getAllConf(int id) throws NoSuchServer;
		ConfigMap getDefaultConf();
		void setConf(int id, string key, string value) throws NoSuchServer;
		void setSuperuserPassword(int id, string pw) throws NoSuchServer;
		LogList getLog(int id, int min, int max) throws NoSuchServer;
	};
};
