#include "clientData.hpp"
#include "Logger.hpp"
#include "task_force_radio.hpp"
#include "antennaManager.h"
#include <iomanip>

#define logParam(x) str << #x << " " << (x) << "\n"
#define logParamN(n,x) str << #n << " " << (x) << "\n"

void LISTED_INFO::operator<<(std::ostream& str) const {

    logParamN(over, static_cast<uint32_t>(over));
    logParamN(on, static_cast<uint32_t>(on));
    logParam(volume);
    logParamN(stereoMode, static_cast<uint32_t>(stereoMode));
    logParam(radio_id);
    logParamN(pos, pos.toString());
    logParam(waveZ);

    str << "vehicle:\n";
    str << "\tvehicleName " << vehicle.vehicleName << "\n";
    str << "\tvehicleIsolation " << vehicle.vehicleIsolation << "\n";
    str << "\tintercomSlot " << vehicle.intercomSlot << "\n";

    str << "antennaConnection:\n" << antennaConnection << "\n";
}

extern float debugDisplayThing;
extern float debugDisplayThing2;

//double interpTo(double target) {
//    static double lastFreq;
//    static std::chrono::system_clock::time_point lastTick;
//
//    auto ms = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(std::chrono::system_clock::now() - lastTick).count();
//    constexpr double freqPerMS = 10;
//    lastTick = std::chrono::system_clock::now();
//    if (lastFreq == target) return target;
//
//    double distance = std::abs(lastFreq - target);
//    double maxDistance = ms * freqPerMS;
//
//    if (maxDistance > distance) {
//        lastFreq = target;
//    } else if (target < lastFreq) {
//        lastFreq -= maxDistance;
//    } else if (target > lastFreq) {
//        lastFreq += maxDistance;
//    }
//    return lastFreq;
//}

Dsp::SimpleFilter<Dsp::Butterworth::LowPass<2>, MAX_CHANNELS>*
 clientDataEffects::getFilterObjectInterception(uint8_t objectCount) {
    objectCount = std::min(objectCount, static_cast<uint8_t>(5));
    static float lastStrength = 400;
    if (auto newStrength = TFAR::config.get<float>(Setting::objectInterceptionStrength); newStrength != lastStrength) {
        LockGuard_exclusive lock_exclusive(m_lock);
        filtersObjectInterception.clear();
        lastStrength = newStrength;
    }

    debugDisplayThing = 2000 - (objectCount * lastStrength);
    debugDisplayThing2 = objectCount;
    LockGuard_shared lock_shared(m_lock);
    if (!filtersObjectInterception.count(objectCount)) {
        lock_shared.unlock();
        LockGuard_exclusive lock_exclusive(m_lock);
        filtersObjectInterception[objectCount] = std::make_unique<Dsp::SimpleFilter<
            Dsp::Butterworth::LowPass<2>, MAX_CHANNELS>>();

        filtersObjectInterception[objectCount]->setup(2, 48000, 2000 - (objectCount * lastStrength));
        //#TODO not happy with that..
    }
    return filtersObjectInterception[objectCount].get();
}

void clientData::updatePosition(const unitPositionPacket & packet) {
    LockGuard_exclusive lock(m_lock);

    clientPosition = packet.position;//Could move assign if more performance is needed
    viewDirection = packet.viewDirection;
    canSpeak = packet.canSpeak;
    if (packet.myData) {
        //Ugly hack because the canUsees of other people don't respect if they even have that radio type
        canUseSWRadio = packet.canUseSWRadio;
        canUseLRRadio = packet.canUseLRRadio;
        canUseDDRadio = packet.canUseDDRadio;
    }
    setVehicleId(packet.vehicleID);
    terrainInterception = packet.terrainInterception;
    voiceVolumeMultiplifier = packet.voiceVolume;
    objectInterception = packet.objectInterception;
    isSpectating = packet.isSpectating;
    isEnemyToPlayer = packet.isEnemyToPlayer;

    //OutputDebugStringA(std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - lastPositionUpdateTime).count()).c_str());
    //OutputDebugStringA("\n");

    lastPositionUpdateTime = std::chrono::system_clock::now();
    dataFrame = TFAR::getInstance().m_gameData.currentDataFrame;
}

Position3D clientData::getClientPosition() const {
    LockGuard_shared lock(m_lock);

    if (velocity.isNull())
        return clientPosition;

    const auto offsetTime = std::chrono::duration<float>(std::chrono::system_clock::now() - lastPositionUpdateTime).count();
    const auto offset = velocity * offsetTime;

    //float x, y, z;
    //std::tie(x, y, z) = offset.get();
    //OutputDebugStringA((std::to_string(offsetTime) + "o " + std::to_string(x) + "," + std::to_string(x) + ","+ std::to_string(z)).c_str());
    //OutputDebugStringA("\n");

    return clientPosition + offset;
}

float clientData::effectiveDistanceTo(std::shared_ptr<clientData>& other) const {
    return effectiveDistanceTo(other.get());
}

float clientData::effectiveDistanceTo(clientData* other) const {
    float d = getClientPosition().distanceTo(other->getClientPosition());
    // (bob distance player) + (bob call TFAR_fnc_calcTerrainInterception) * 7 + (bob call TFAR_fnc_calcTerrainInterception) * 7 * ((bob distance player) / 2000.0)
    float result = d +
        other->terrainInterception * TFAR::getInstance().m_gameData.terrainIntersectionCoefficient
        +
        other->terrainInterception * TFAR::getInstance().m_gameData.terrainIntersectionCoefficient * (d / 2000.0f);
    result *= TFAR::getInstance().m_gameData.receivingDistanceMultiplicator;
    return result;
}

bool clientData::isAlive() {
    if (dataFrame == INVALID_DATA_FRAME) return false;
    const bool timeout = (std::chrono::system_clock::now() - getLastPositionUpdateTime() > (MILLIS_TO_EXPIRE * 5)) || (abs(TFAR::getInstance().m_gameData.currentDataFrame - dataFrame) > 1);
    if (timeout)
        dataFrame = INVALID_DATA_FRAME;
    return !timeout;
}

clientData* lastORCheck = nullptr;
#include "Teamspeak.hpp"
#define DIAGLOG(x) if (lastORCheck != this) circularLog(x)

LISTED_INFO clientData::isOverLocalRadio(std::shared_ptr<clientData>& myData, bool ignoreSwTangent, bool ignoreLrTangent, AntennaConnection& antennaConnection) {
    ProfileFunction;
    //Sender is this
    LISTED_INFO result;
    result.over = sendingRadioType::LISTEN_TO_NONE;
    result.volume = 0;
    result.on = receivingRadioType::LISTED_ON_NONE;
    result.waveZ = 1.0f;
    if (!myData) return result;

    const auto senderNickname = getNickname();

    const auto myPosition = myData->getClientPosition();
    const auto clientPosition = getClientPosition();

    //If we are underwater and can still use SWRadio then we are in a vehicle
    const bool receiverUnderwater = myPosition.getHeight() < 0 && !myData->canUseSWRadio;
    const bool senderUnderwater = clientPosition.getHeight() < 0 && !canUseSWRadio;

    //If we didn't find anything result.on has to be LISTED_ON_NONE so we can set result.over
    //even if we won't find a valid path on the receiver side

    if ((currentTransmittingTangentOverType == sendingRadioType::LISTEN_TO_SW || ignoreSwTangent) && (canUseSWRadio || canUseDDRadio)) {//Sending from SW
        result.over = senderUnderwater ? sendingRadioType::LISTEN_TO_DD : sendingRadioType::LISTEN_TO_SW;
    } else if ((currentTransmittingTangentOverType == sendingRadioType::LISTEN_TO_LR || ignoreLrTangent) && canUseLRRadio) {//Sending from LR
        result.over = sendingRadioType::LISTEN_TO_LR;
    } else {
        DIAGLOG("IOLR No Send? ttot="+std::to_string(static_cast<int>(currentTransmittingTangentOverType))+" CURF="+
            std::to_string(canUseSWRadio) +
            std::to_string(canUseDDRadio) +
            std::to_string(canUseLRRadio));
        //He isn't actually sending on anything... 
        return result;
    }

    result.radio_id = "local_radio";
    result.vehicle = myData->getVehicleDescriptor();
    const auto senderFrequency = getCurrentTransmittingFrequency();

    LockGuard_shared countLock(TFAR::getInstance().m_gameData.m_lock);
    //Sender is sending on a Frequency we are listening to on our LR Radio
    const bool senderOnLRFrequency = TFAR::getInstance().m_gameData.myLrFrequencies.count(senderFrequency) != 0;
    //Sender is sending on a Frequency we are listening to on our SW Radio
    const bool senderOnSWFrequency = TFAR::getInstance().m_gameData.mySwFrequencies.count(senderFrequency) != 0;
    countLock.unlock();
    if (!senderOnSWFrequency && !senderOnLRFrequency) {
        DIAGLOG("IOLR No freq sf=" + senderFrequency);
        return result; //He's not on any frequency we can receive on
    }

    auto effectiveDist = myData->effectiveDistanceTo(this);
    const bool isUnderwater = receiverUnderwater || senderUnderwater;

    if (isUnderwater) {
        DIAGLOG("IOLR underwater");
        const auto underwaterDist = myPosition.distanceUnderwater(clientPosition);
        //Seperate distance underwater and distance overwater.
        effectiveDist = underwaterDist * (range / helpers::distanceForDiverRadio()) + (effectiveDist - underwaterDist);

        if (effectiveDist > range && !antennaConnection.isNull()) {
           
                effectiveDist = 0.f; //just use make the range check succeed
        }
    }
    
    if (effectiveDist > range) {    //Out of range
        if (!antennaConnection.isNull()) {
            //Normal range not sufficient. Check if Antenna reaches.
            const auto antennaUnderwater = clientPosition.distanceUnderwater(antennaConnection.getAntenna()->getPos());
            const auto antennaTotal = clientPosition.distanceTo(antennaConnection.getAntenna()->getPos());
            const auto effectiveRangeToAntenna = antennaUnderwater * (range / helpers::distanceForDiverRadio())
                + (antennaTotal - antennaUnderwater);
            if (effectiveRangeToAntenna > range) {//Antenna doesn't reach
                DIAGLOG("IOLR No reach ed=" + std::to_string(effectiveDist) + " rng=" + std::to_string(range));
                return result;
            }
        } else {
            DIAGLOG("IOLR No reach ed=" + std::to_string(effectiveDist) + " rng=" + std::to_string(range));
            return result;
        }
    }
        
    //#TODO always have a "valid" antenna connection in the end result. Just to transmit the connectionLoss to not have to calculate it again
    result.antennaConnection = antennaConnection;

    std::string currentTransmittingRadio;
    if (!TFAR::config.get<bool>(Setting::full_duplex)) {
        //We have to get it here because we can't while m_gameData is Locked
        currentTransmittingRadio = TFAR::getInstance().m_gameData.getCurrentTransmittingRadio();
    }

    if (senderOnLRFrequency && myData->canUseLRRadio) {//to our LR
        LockGuard_shared lock(TFAR::getInstance().m_gameData.m_lock);
        auto &frequencyInfo = TFAR::getInstance().m_gameData.myLrFrequencies[senderFrequency];
        if (!TFAR::config.get<bool>(Setting::full_duplex) && //If we are currently transmitting on that Radio we can't hear so we return before result gets valid
            frequencyInfo.radioClassname == currentTransmittingRadio) {
            DIAGLOG("IOLR No duplex LR RECV");
            return result;
        }
        result.on = receivingRadioType::LISTED_ON_LR;
        result.volume = frequencyInfo.volume;
        result.stereoMode = frequencyInfo.stereoMode;
        DIAGLOG("IOLR RECV! RC=" + frequencyInfo.radioClassname);
    } else if (senderOnSWFrequency && myData->canUseSWRadio) {//to our SW
        LockGuard_shared lock(TFAR::getInstance().m_gameData.m_lock);
        auto &frequencyInfo = TFAR::getInstance().m_gameData.mySwFrequencies[senderFrequency];
        if (!TFAR::config.get<bool>(Setting::full_duplex) && //If we are currently transmitting on that Radio we can't hear so we return before result gets valid
            frequencyInfo.radioClassname == currentTransmittingRadio) {
            DIAGLOG("IOLR No duplex SR RECV");
            return result;
        }
        result.on = receivingRadioType::LISTED_ON_SW;
        result.volume = frequencyInfo.volume;
        result.stereoMode = frequencyInfo.stereoMode;
        DIAGLOG("IOLR RECV! RC="+ frequencyInfo.radioClassname);
    }
    return result;
}

std::vector<LISTED_INFO> clientData::isOverRadio(std::shared_ptr<clientData>& myData, bool ignoreSwTangent, bool ignoreLrTangent) {
    ProfileFunction;
    execAtReturn setLastOr([this](){
        lastORCheck = this;
    });
    std::string senderNickname = getNickname();
    std::vector<LISTED_INFO> result;
    if (!myData) {
        DIAGLOG("IOR No Data");
        return result;
    }
    //Intercom has to be here because it has to be before range==0 check
    //vehicle intercom
    const auto vecDescriptor = getVehicleDescriptor();
    const auto myVecDescriptor = myData->getVehicleDescriptor();
    if (TFAR::config.get<bool>(Setting::intercomEnabled) &&
        currentTransmittingTangentOverType == sendingRadioType::LISTEN_TO_NONE && //Not currently transmitting on a Radio. If transmitting only direct speech.
        vecDescriptor.vehicleName != "no" && vecDescriptor.vehicleName == myVecDescriptor.vehicleName //In same vehicle 
        && vecDescriptor.intercomSlot != -1 && vecDescriptor.intercomSlot == myVecDescriptor.intercomSlot) { //On same Intercom Channel
        result.emplace_back(
            sendingRadioType::LISTEN_TO_SW, //unused
            receivingRadioType::LISTED_ON_INTERCOM,
            7,  //unused
            stereoMode::stereo,  //unused
            "intercom",   //unused
            Position3D(0, 0, 0), //unused
            0.f,     //unused
            getVehicleDescriptor());  //unused
    }



    if (range == 0) {
        DIAGLOG("IOR No Radio Transmit");
        return result; //If we are sending range is set to Radio's range. Always!
    }
    auto antennaConnection = (clientId != myData->clientId) ? TFAR::getAntennaManager()->findConnection(getClientPosition(), static_cast<float>(range), myData->getClientPosition()) : AntennaConnection();

    //check if we receive him over a radio we have on us
    if (clientId != myData->clientId) {//We don't hear ourselves over our Radio ^^
        const auto local = isOverLocalRadio(myData, ignoreSwTangent, ignoreLrTangent, antennaConnection);
        if (local.on != receivingRadioType::LISTED_ON_NONE && local.over != sendingRadioType::LISTEN_TO_NONE) {
            result.push_back(local);
        } else {
            DIAGLOG("IOR No Local Radio");
        }
    }

    const auto effectiveDistance = myData->effectiveDistanceTo(this);
    //check if we receive him over a radio laying on ground

    //We reuse the antennaConnection for a nearby speaker. Technically antennaConnection measures connection to our body. But a speaker Radio is not that far away from us anyway
    if (effectiveDistance > range && antennaConnection.isNull()) { //does senders range reach to us?
        DIAGLOG("IOR No Range & no Ant efDist="+std::to_string(effectiveDistance)+" rng="+std::to_string(range));
        return result; //His distance > range and no suitable Antenna
    }


    if (
        (canUseSWRadio && (currentTransmittingTangentOverType == sendingRadioType::LISTEN_TO_SW || ignoreSwTangent)) || //Sending over SW
        (canUseLRRadio && (currentTransmittingTangentOverType == sendingRadioType::LISTEN_TO_LR || ignoreLrTangent))) { //Sending over LR
        auto currentFrequency = getCurrentTransmittingFrequency();
        LockGuard_shared lock(TFAR::getInstance().m_gameData.m_lock);
        for (const auto& it : TFAR::getInstance().m_gameData.speakers) {
            if (it.first != currentFrequency) continue;
            auto& speaker = it.second;
            auto speakerOwner = speaker.client.lock();
            //If the speaker is Senders backpack we don't hear it.. Because he is sending with his Backpack so it can't receive
            //Also because we would normally hear him twice
            if (speakerOwner && speakerOwner->clientId == clientId) continue;
            auto speakerPosition = speaker.getPos(speakerOwner);
            if (speakerPosition.isNull()) continue;//Don't know its position
            if (speakerPosition.getHeight() < 0) continue;//Speakers don't work underwater.. duh
            result.emplace_back(
                (currentTransmittingTangentOverType == sendingRadioType::LISTEN_TO_SW || ignoreSwTangent) ? sendingRadioType::LISTEN_TO_SW : sendingRadioType::LISTEN_TO_LR,
                receivingRadioType::LISTED_ON_GROUND,
                speaker.volume,
                stereoMode::stereo,
                speaker.radio_id,
                speakerPosition,
                speaker.waveZ,
                speaker.vehicle,
                antennaConnection);

        }

    } else {
        DIAGLOG("IOR No spk? cusw=" + std::to_string(canUseSWRadio) + " culr=" + std::to_string(canUseLRRadio)+
            " ttot="+std::to_string(static_cast<int>(currentTransmittingTangentOverType))+
            " tfrq="+ getCurrentTransmittingFrequency());
    }
    return result;



}

void clientData::addModificationLog(std::string mod) {
    LockGuard_exclusive lock(m_lock);
    //Logger::log(LoggerTypes::pluginCommands, "mod " + mod);
    modificationLog.emplace_back(std::move(mod));
}
std::vector<std::string> clientData::getModificationLog() const {
    LockGuard_shared lock(m_lock);
    return modificationLog;
}

void clientData::circularLog(const std::string& message) {
    std::stringstream msg;
    const auto now = std::chrono::system_clock::now();
    const auto in_time_t = std::chrono::system_clock::to_time_t(now);
    msg << std::put_time(std::localtime(&in_time_t), "%H:%M:%S") << " " << message;
    LockGuard_exclusive lock(m_lock);
    messages.push_back(msg.str());
    if (++offset > messageCount) offset = 0;

}

void clientData::verboseDataLog(std::ostream& str) {
    LockGuard_shared lock(m_lock);

    logParam(pluginEnabled);
    logParamN(clientId,clientId.baseType());
    logParamN(currentTransmittingTangentOverType,static_cast<uint32_t>(currentTransmittingTangentOverType));
    logParam(voiceVolume);
    logParam(range);
    logParam(canSpeak);
    logParam(clientTalkingNow);
    logParam(dataFrame);
    logParam(canUseSWRadio);
    logParam(canUseLRRadio);
    logParam(canUseDDRadio);
    logParam(terrainInterception);
    logParam(objectInterception);
    logParam(voiceVolumeMultiplifier);
    logParam(isSpectating);
    logParam(isEnemyToPlayer);
    logParamN(receivingTransmission,static_cast<uint32_t>(receivingTransmission));

    str << "receivingFrequencies:\n";

    for (auto& it : receivingFrequencies) {
        str << "\t" << it << "\n";
    }
    str << "modificationLog:\n";

    for (auto& it : modificationLog) {
        str << "\t" << it << "\n";
    }
    logParam(nickname);
    logParamN(clientPosition, clientPosition.toString());
    logParamN(viewDirection, viewDirection.toString());


    auto in_time_t = std::chrono::system_clock::to_time_t(lastPositionUpdateTime);
    str << "lastPositionUpdateTime " << std::put_time(std::localtime(&in_time_t), "%H:%M:%S") << "\n";

    logParam(currentTransmittingFrequency);
    logParam(currentTransmittingSubtype);

    str << "vehicleId:\n";
    str << "\tvehicleName " << vehicleId.vehicleName << "\n";
    str << "\tvehicleIsolation " << vehicleId.vehicleIsolation << "\n";
    str << "\tintercomSlot " << vehicleId.intercomSlot << "\n";

    logParamN(velocity,velocity.toString());
}
