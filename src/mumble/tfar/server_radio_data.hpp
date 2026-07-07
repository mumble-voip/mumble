#pragma once
#include "client_data.hpp"
#include <Windows.h>
extern CRITICAL_SECTION serverDataCriticalSection;
struct FREQ_SETTINGS {
	int volume;
	stereoMode stereoMode;
	std::string radioClassname;
};

struct SPEAKER_DATA {
	std::string radio_id;
	std::string nickname;
	std::vector<float> pos;
	int volume;
	std::pair<std::string, float> vehicle;
	float waveZ;
};

struct SERVER_RADIO_DATA {
	std::string getMyNickname() const { return myNickname; }
	void setMyNicknamex(std::string val) {//#TODO remove x
		myNickname = CLIENT_DATA::convertNickname(val);
	}






	void setFreqInfos(const std::vector<std::string>& tokens);


	std::string myOriginalNickname;
	std::string getMyOriginalNickname() const { return myOriginalNickname; }
	void setMyOriginalNickname(std::string val) {
		EnterCriticalSection(&serverDataCriticalSection);
		myOriginalNickname = val;
		LeaveCriticalSection(&serverDataCriticalSection);
	}
	bool tangentPressed;
	TS3_VECTOR myPosition;
	STRING_TO_CLIENT_DATA_MAP nicknameToClientData;
#ifndef unmuteAllClients
	std::vector<anyID> mutedClients; //Access is guarded by serverDataCriticalSection 
	void sortMutedClients() {
		std::sort(mutedClients.begin(), mutedClients.end());
		mutedClients.erase(std::unique(mutedClients.begin(), mutedClients.end()), mutedClients.end());
	}
#endif

	std::map<std::string, FREQ_SETTINGS> mySwFrequencies;
	std::map<std::string, FREQ_SETTINGS> myLrFrequencies;

	std::string myDdFrequency;
	std::multimap<std::string, SPEAKER_DATA> speakers;
	std::map<std::string, clunk::WavFile*> waves;

	int ddVolumeLevel;
	int myVoiceVolume;
	bool alive;
	bool canSpeak;
	float wavesLevel;
	float terrainIntersectionCoefficient;
	float globalVolume;
	float receivingDistanceMultiplicator;
	float speakerDistance;

	int currentDataFrame;

	std::string currentTransmittingRadio;//Used for half-duplex mode

	SERVER_RADIO_DATA(): tangentPressed(false), ddVolumeLevel(0), myVoiceVolume(0), alive(false), canSpeak(false),
		wavesLevel(0), terrainIntersectionCoefficient(7.0f), globalVolume(1.0f),
		receivingDistanceMultiplicator(1.0f), speakerDistance(20.0f), currentDataFrame(INVALID_DATA_FRAME), currentTransmittingRadio("")
	{

	}

private:
	std::string myNickname;

};


//typedef std::map<uint64, SERVER_RADIO_DATA> SERVER_ID_TO_SERVER_DATA;





class SERVER_ID_TO_SERVER_DATA {
public:
	typedef std::map<uint64_t, SERVER_RADIO_DATA>::iterator iterator;
	std::map<uint64_t, SERVER_RADIO_DATA>::iterator begin();
	std::map<uint64_t, SERVER_RADIO_DATA>::iterator end();
	size_t count(const uint64_t &serverConnectionHandlerID) const;
	SERVER_RADIO_DATA& operator[](const uint64_t &serverConnectionHandlerID);
	auto erase(const uint64_t &serverConnectionHandlerID) {
		return data.erase(serverConnectionHandlerID);
	}
	void setMyNickname(const uint64_t &serverConnectionHandlerID, const std::string& nickname);
	std::string getMyNickname(const uint64_t &serverConnectionHandlerID);
	//convenience function to keep CriticalSection interaction low
	void resetAndSetMyNickname(const uint64_t &serverConnectionHandlerID, const std::string& nickname);
	std::vector<std::shared_ptr<CLIENT_DATA>> getClientDataByClientID(const uint64_t &serverConnectionHandlerID, anyID clientID);
	float getWavesLevel(uint64_t const& serverConnectionHandlerID);
	//convenience function for serverIdToData[serverConnectionHandlerID].nicknameToClientData.count(nickname) with CriticalSectionLock
	size_t clientDataCount(const uint64_t &serverConnectionHandlerID, const std::string & nickname);
	void setFreqInfos(const uint64_t &serverConnectionHandlerID, const std::vector<std::string> &tokens);
private:
	std::map<uint64, SERVER_RADIO_DATA> data;
};












