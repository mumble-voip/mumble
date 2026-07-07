#include "server_radio_data.hpp"
#include "helpers.h"


void SERVER_RADIO_DATA::setFreqInfos(const std::vector<std::string>& tokens) {
	CriticalSectionLock lock(&serverDataCriticalSection);
	mySwFrequencies = helpers::parseFrequencies(tokens[1]);
	myLrFrequencies = helpers::parseFrequencies(tokens[2]);
	myDdFrequency = tokens[3];
	alive = tokens[4] == "true";
	myVoiceVolume = helpers::parseArmaNumberToInt(tokens[5]);
	ddVolumeLevel = helpers::parseArmaNumberToInt(tokens[6]);
	wavesLevel = helpers::parseArmaNumber(tokens[8]);
	terrainIntersectionCoefficient = helpers::parseArmaNumber(tokens[9]);
	globalVolume = helpers::parseArmaNumber(tokens[10]);
	receivingDistanceMultiplicator = helpers::parseArmaNumber(tokens[12]);
	speakerDistance = helpers::parseArmaNumber(tokens[13]);
}

std::map<uint64_t, SERVER_RADIO_DATA>::iterator SERVER_ID_TO_SERVER_DATA::begin() {
	return data.begin();
}

std::map<uint64_t, SERVER_RADIO_DATA>::iterator SERVER_ID_TO_SERVER_DATA::end() {
	return data.end();
}

size_t SERVER_ID_TO_SERVER_DATA::count(uint64_t const& serverConnectionHandlerID) const {
	CriticalSectionLock lock(&serverDataCriticalSection);
	return data.count(serverConnectionHandlerID);
}

SERVER_RADIO_DATA& SERVER_ID_TO_SERVER_DATA::operator[](uint64_t const& serverConnectionHandlerID) {
	return data[serverConnectionHandlerID];
}

void SERVER_ID_TO_SERVER_DATA::setMyNickname(uint64_t const& serverConnectionHandlerID, const std::string& nickname) {
	CriticalSectionLock lock(&serverDataCriticalSection);
	data[serverConnectionHandlerID].setMyNicknamex(nickname);
}

std::string SERVER_ID_TO_SERVER_DATA::getMyNickname(uint64_t const& serverConnectionHandlerID) {
	CriticalSectionLock lock(&serverDataCriticalSection);
	return data[serverConnectionHandlerID].getMyNickname();
}

void SERVER_ID_TO_SERVER_DATA::resetAndSetMyNickname(uint64_t const& serverConnectionHandlerID, const std::string& nickname) {
	CriticalSectionLock lock(&serverDataCriticalSection);
	data[serverConnectionHandlerID] = SERVER_RADIO_DATA();
	data[serverConnectionHandlerID].setMyNicknamex(nickname);
}

std::vector<std::shared_ptr<CLIENT_DATA>> SERVER_ID_TO_SERVER_DATA::getClientDataByClientID(uint64_t const& serverConnectionHandlerID, anyID clientID) {
	CriticalSectionLock lock(&serverDataCriticalSection);
	return data[serverConnectionHandlerID].nicknameToClientData.getClientDataByClientID(clientID);
}

float SERVER_ID_TO_SERVER_DATA::getWavesLevel(uint64_t const& serverConnectionHandlerID) {
	CriticalSectionLock lock(&serverDataCriticalSection);
	return data[serverConnectionHandlerID].wavesLevel;
}

size_t SERVER_ID_TO_SERVER_DATA::clientDataCount(uint64_t const& serverConnectionHandlerID, const std::string & nickname) {
	CriticalSectionLock lock(&serverDataCriticalSection);
	return data[serverConnectionHandlerID].nicknameToClientData.count(nickname);
}

void SERVER_ID_TO_SERVER_DATA::setFreqInfos(const uint64_t &serverConnectionHandlerID, const std::vector<std::string> &tokens) {
	if (data.count(serverConnectionHandlerID))
		data[serverConnectionHandlerID].setFreqInfos(tokens);
}
