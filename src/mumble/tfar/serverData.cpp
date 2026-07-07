#include "serverData.hpp"
#include "helpers.hpp"
#include "Locks.hpp"
#include <mutex>
#include "task_force_radio.hpp"
#include "Logger.hpp"
#include "Teamspeak.hpp"


serverDataDirectory::serverDataDirectory() {
    TFAR::getInstance().onTeamspeakClientJoined.connect([this](TSServerID serverID, TSClientID clientID, const std::string& clientNickname) {
        Logger::log(LoggerTypes::pluginCommands, "clientJoined " + std::to_string(clientID.baseType()) + " " + clientNickname);
        LockGuard_shared lock(m_lock);
        auto found = data.find(serverID);
        if (found != data.end())
            found->second->clientJoined(clientID, clientNickname);
    });
    TFAR::getInstance().onTeamspeakClientLeft.connect([this](TSServerID serverID, TSClientID clientID) {
        Logger::log(LoggerTypes::pluginCommands, "clientLeft " + std::to_string(clientID.baseType()));
        LockGuard_shared lock(m_lock);
        auto found = data.find(serverID);
        if (found != data.end())
            found->second->clientLeft(clientID);
    });
    TFAR::getInstance().onTeamspeakClientUpdated.connect([this](TSServerID serverID, TSClientID clientID, const std::string& clientNickname) {
        Logger::log(LoggerTypes::pluginCommands, "clientUpdated " + std::to_string(clientID.baseType()) + " " + clientNickname);
        LockGuard_shared lock(m_lock);
        auto found = data.find(serverID);
        if (found != data.end())
            found->second->clientUpdated(clientID, clientNickname);
    });
    TFAR::getInstance().onTeamspeakServerConnect.connect([this](TSServerID serverID) {
        Logger::log(LoggerTypes::pluginCommands, "serverConnect " + std::to_string(serverID.baseType()));
        LockGuard_exclusive lock(m_lock);
        data.insert({ serverID,std::make_shared<serverData>() });
    });
    TFAR::getInstance().onTeamspeakServerDisconnect.connect([this](TSServerID serverID) {
        Logger::log(LoggerTypes::pluginCommands, "serverConnect " + std::to_string(serverID.baseType()));
        LockGuard_exclusive lock(m_lock);
        data.erase(serverID);
    });

    TFAR::getInstance().doDiagReport.connect([this](std::stringstream& diag) {
        diag << "SDD:\n";
        for (auto& it : data) {
            diag << TS_INDENT << it.first.baseType() << ":\n";
            diag << TS_INDENT << TS_INDENT << "myCD: " << it.second->myClientData << "\n";
            it.second->debugPrint(diag);
        }
    });
    TFAR::getInstance().doTypeDiagReport.connect([this](const std::string& type, std::stringstream& diag) {
        if (type != "pos") return;
        diag << "SDD:\n";
        for (auto& it : data) {
            diag << TS_INDENT << it.first.baseType() << ":\n";
            diag << TS_INDENT << TS_INDENT << "myCD: " << it.second->myClientData << "\n";
            it.second->debugPrint(diag, true);
        }
    });


}

std::shared_ptr<serverData> serverDataDirectory::getClientDataDirectory(TSServerID serverID) const {
    LockGuard_shared lock(m_lock);
    if (data.find(serverID) == data.end()) {
        Logger::log(LoggerTypes::pluginCommands, "invalid getClientDataDirectory " + std::to_string(serverID.baseType()));
        return std::shared_ptr<serverData>();
    }
    return data.at(serverID);
}

bool serverDataDirectory::hasDirectory(TSServerID serverConnectionHandlerID) const {
    LockGuard_shared lock(m_lock);
    return data.find(serverConnectionHandlerID) != data.end();
}

void serverDataDirectory::verify() {
    LockGuard_shared lock(m_lock);
    for (auto& it : data) {
        it.second->verify();
    }
}

bool serverData::hasClientData(TSClientID clientID) const {
    const auto clientData = getClientData(clientID);//using public function so we have our lock
    if (!clientData) return false;
    return abs(TFAR::getInstance().m_gameData.currentDataFrame - clientData->dataFrame) <= 1;
}

std::shared_ptr<clientData> serverData::getClientData(TSClientID clientID) const {
    LockGuard_shared lock(m_lock);

    const auto found = findClientData(clientID);
    if (found != data.end()) return std::get<2>(*found);
    return std::shared_ptr<clientData>();
}

std::shared_ptr<clientData> serverData::getClientData(std::string_view nickname) const {
    LockGuard_shared lock(m_lock);
    const auto found = findClientData(nickname);
    if (found != data.end()) return std::get<2>(*found);
    return std::shared_ptr<clientData>();
}

void serverData::forEachClient(const std::function<void(const std::shared_ptr<clientData>&)>& func) {
    LockGuard_shared lock(m_lock);
    for (auto& cl : data) {
        func(std::get<2>(cl));
    }
}

void serverData::clientJoined(TSClientID clientID, const std::string& clientNickname) {
    LockGuard_exclusive lock(m_lock);
    if (containsClientData(clientID)) {
        const auto found = findClientData(clientID);
        if (!std::get<2>(*found)) {//std::shared_ptr is nullptr
            std::get<2>(*found) = std::make_shared<clientData>(clientID);//Give it a valid clientData
        }
        auto cData = std::get<2>(*found);
        cData->addModificationLog(std::string("clientJoined MOD ") + std::to_string(clientID.baseType()) + " " + clientNickname);
        cData->setNickname(clientNickname);

        //fixup hash in case his name changed
        data.erase(found);
        insertInData(clientNickname, clientID, cData);

        return; //client already exists and is valid
    }

    if (myClientData && myClientData->clientId == clientID) { //Not needed after I added that clientLeft doesn't remove myID.. But better safe huh?
        myClientData->addModificationLog(std::string("clientJoined myData ") + std::to_string(clientID.baseType()) + " " + clientNickname);
        myClientData->setNickname(clientNickname);
    } else {
        auto newCData = std::make_shared<clientData>(clientID);
        std::get<2>(*insertInData(clientNickname, clientID, newCData))->setNickname(clientNickname);
        newCData->addModificationLog(std::string("clientJoined ") + std::to_string(clientID.baseType()) + " " + clientNickname);
    }
}

void serverData::clientLeft(TSClientID clientID) {
    LockGuard_exclusive lock(m_lock);
    if (clientID == -2) {//handle -2 clientID to remove all clients
        data.clear();
        if (myClientData) {
            insertInData(myClientData->getNickname(), myClientData->clientId, myClientData);//We will always stay on server unless we disconnect
        }
        return;
    }
    if (myClientData && myClientData->clientId == clientID) return; //Don't remove my ID... Thats removed only when it really changes.. serverreconnect
    if (!containsClientData(clientID))
        return;
    const auto clientData = findClientData(clientID);
    std::get<2>(*clientData)->addModificationLog(std::string("clientLeft ") + std::to_string(clientID.baseType()));
    data.erase(clientData);
}

void serverData::clientUpdated(TSClientID clientID, const std::string& clientNickname) {
    LockGuard_shared lock_shared(m_lock);
    if (!containsClientData(clientID))
        return;
    const auto clientDataIterator = findClientData(clientID);
    auto clientData = std::get<2>(*clientDataIterator);
    if (clientData->getNickname() == clientNickname)
        return;
    lock_shared.unlock();
    clientData->addModificationLog(std::string("clientUpdated ") + std::to_string(clientID.baseType()) + " " + clientNickname);
    LockGuard_exclusive lock_exclusive(m_lock);

    //Remove and Reinsert to keep map sorted.
    data.erase(findClientData(clientID));
    insertInData(clientNickname, clientID, clientData);
    //#TODO revert to old code commented below.. and instead of readding just change and sort
    //Don't need to care about duplicate nicknames. Teamspeak takes care of that
    //std::get<0>(*clientDataIterator) = std::hash<std::string>()(clientNickname);
    clientData->setNickname(clientNickname);
}

void serverData::debugPrint(std::stringstream& diag, bool withPos) const {
    LockGuard_shared lock_shared(m_lock);
    //Logger::log(LoggerTypes::pluginCommands, "DebugPrintStart###");
    for (auto& it : data) {
        const auto& cData = std::get<2>(it);


        diag << TS_INDENT << TS_INDENT << TS_INDENT << cData << ":\n";
        diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "NICK: " << cData->getNickname() << "\n";
        diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "SHASH: " << std::get<0>(it) << "\n";
        diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "CHASH: " << std::hash<indexedType>()(cData->getNickname()) << "\n";
        diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "SCID: " << std::get<1>(it).baseType() << "\n";
        diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "CCID: " << cData->clientId.baseType() << "\n";
        if (withPos) {
            auto [x,y,z] = cData->getClientPosition().get();
            auto [x2, y2, z2] = cData->getClientPositionRaw().get();
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "POSINTERP: " << x << "," << y << "," << z << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "POSRAW: " << x2 << "," << y2 << "," << z2 << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "VIEW: " << static_cast<float>(static_cast<AngleDegrees>(cData->getViewDirection())) << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "LPOSTIME: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - cData->getLastPositionUpdateTime()).count() << "us\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "CurTransFreq: " << cData->getCurrentTransmittingFrequency() << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "spectator: " << cData->isSpectating << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "IC: n:" << cData->getVehicleDescriptor().vehicleName << " id:"<< cData->getVehicleDescriptor().intercomSlot << "\n";
        }
        //std::string entry = "Entry " + std::to_string(std::get<0>(it)) + "=" + std::to_string(std::hash<indexedType>()(cData->getNickname()))
        //    + " " + std::to_string(std::get<1>(it).baseType()) + "=" + std::to_string(cData->clientId.baseType()) + " " + cData->getNickname();
        //Logger::log(LoggerTypes::pluginCommands, entry);
    }

    //Logger::log(LoggerTypes::pluginCommands, "DebugPrintEnd###");
}

void serverData::verify() {
    LockGuard_shared slock(m_lock);
    for (auto i = data.begin(); i < data.end(); ++i) {
        const auto cData = std::get<2>(*i);
        const auto nick = cData->getNickname();
        if (std::get<0>(*i) != std::hash<indexedType>()(nick)) {
            slock.unlock();
            LockGuard_exclusive lock(m_lock);

            auto message = "TFAR: HASH mismatch! Tell Dedmen! " + nick + "=" + std::to_string(std::hash<indexedType>()(nick)) + " IDX " + std::to_string(std::get<0>(*i));
            
            for (auto& it : cData->getModificationLog()) {
                message += "\n" + it;
            }
            
            Teamspeak::printMessageToCurrentTab(message.c_str());
            const auto cid = cData->clientId;
            data.erase(i);
            lock.unlock();
            clientJoined(cid, nick);
            return;
        }
    }
}
