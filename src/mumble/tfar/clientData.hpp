#pragma once
#include "common.hpp"
#include <map>
#include "RadioEffect.hpp"
#include "Clunk.h"
#include <clunk/wav_file.h>
#include "simpleSource/SimpleComp.h"
#include <memory> //shared_ptr
#include <unordered_map>
#include "Locks.hpp"
#include "antennaManager.h"
#include <set>

enum class sendingRadioType {   //Receiving FROM senders Radio.
    LISTEN_TO_SW,
    LISTEN_TO_LR,
    LISTEN_TO_DD,
    LISTEN_TO_NONE
};

enum class receivingRadioType { //Receiving TO our Radio
    LISTED_ON_SW,
    LISTED_ON_LR,
    LISTED_ON_NONE,
    LISTED_ON_GROUND,
    LISTED_ON_INTERCOM
};

struct LISTED_INFO {
    LISTED_INFO(sendingRadioType _over, receivingRadioType _on, int _volume, stereoMode _stereoMode, std::string _radio_id,
        Position3D _pos, float _waveZ, vehicleDescriptor _vehicle, AntennaConnection _antCon = AntennaConnection())
        :over(_over), on(_on), volume(_volume), stereoMode(_stereoMode), radio_id(std::move(_radio_id)),
        pos(std::move(_pos)), waveZ(_waveZ), vehicle(std::move(_vehicle)), antennaConnection(std::move(_antCon)) {}
    LISTED_INFO() = default;
    sendingRadioType over = sendingRadioType::LISTEN_TO_NONE;//What radiotype the Sender is using
    receivingRadioType on = receivingRadioType::LISTED_ON_NONE;//What radiotype we are receiving on
    int volume = 0;
    stereoMode stereoMode = stereoMode::stereo;
    std::string radio_id;
    Position3D pos;
    float waveZ = 0.f;
    vehicleDescriptor vehicle;//Only set for local_radio and is always the players vehicle
    AntennaConnection antennaConnection;
    void operator <<(std::ostream& str) const;
};

inline std::ostream& operator<<(std::ostream &os, const LISTED_INFO& w) {
    w.operator<<(os);
    return os;
}

struct unitPositionPacket {
    std::string nickname;
    Position3D position;
    Direction3D viewDirection;
    bool canSpeak;
    bool canUseSWRadio;
    bool canUseLRRadio;
    bool canUseDDRadio;
    std::string_view vehicleID;
    int terrainInterception;
    float voiceVolume;
    int objectInterception;
    bool isSpectating;
    bool isEnemyToPlayer;



    bool myData{false}; //Has to be last element
};

class clientDataEffects {
public:
    clientDataEffects() {

        compressor.setSampleRate(48000);
        compressor.setThresh(80);
        compressor.setRelease(300);
        compressor.setAttack(1);
        compressor.setRatio(0.1);
        compressor.initRuntime();

        resetRadioEffect();
    }
    ~clientDataEffects() = default;


    Dsp::SimpleFilter<Dsp::Butterworth::BandPass<2>, MAX_CHANNELS>* getSpeakerPhone(const std::string& key) {
        LockGuard_shared lock_shared(m_lock);
        if (!filtersPhone.count(key)) {
            lock_shared.unlock();
            LockGuard_exclusive lock_exclusive(m_lock);
            filtersPhone[key] = std::make_unique<Dsp::SimpleFilter<Dsp::Butterworth::BandPass<2>, MAX_CHANNELS>>();
            filtersPhone[key]->setup(2, 48000, 1850, 1550);
        }
        return filtersPhone[key].get();
    }

    Dsp::SimpleFilter<Dsp::Butterworth::BandPass<1>, MAX_CHANNELS>* getSpeakerFilter(const std::string& key) {
        LockGuard_shared lock_shared(m_lock);
        if (!filtersSpeakers.count(key)) {
            lock_shared.unlock();
            LockGuard_exclusive lock_exclusive(m_lock);
            filtersSpeakers[key] = std::make_unique<Dsp::SimpleFilter<Dsp::Butterworth::BandPass<1>, MAX_CHANNELS>>();
            filtersSpeakers[key]->setup(1, 48000, 2000, 1000);
        }
        return filtersSpeakers[key].get();
    }

    void removeSpeakerFilter(const std::string& key) {
        LockGuard_exclusive lock_shared(m_lock);
        filtersSpeakers.erase(key);
    }

    PersonalRadioEffect* getSwRadioEffect(const std::string& key) {
        LockGuard_shared lock_shared(m_lock);
        if (!swEffects.count(key)) {
            lock_shared.unlock();
            LockGuard_exclusive lock_exclusive(m_lock);
            swEffects[key] = std::make_unique<PersonalRadioEffect>();
        }
        return swEffects[key].get();
    }

    LongRangeRadioEffect* getLrRadioEffect(const std::string& key) {
        LockGuard_shared lock_shared(m_lock);
        if (!lrEffects.count(key)) {
            lock_shared.unlock();
            LockGuard_exclusive lock_exclusive(m_lock);
            lrEffects[key] = std::make_unique<LongRangeRadioEffect>();
        }
        return lrEffects[key].get();
    }

    AirborneRadioEffect* getAirborneRadioEffect(const std::string& key) {
        LockGuard_shared lock_shared(m_lock);
        if (!airborneEffects.count(key)) {
            lock_shared.unlock();
            LockGuard_exclusive lock_exclusive(m_lock);
            airborneEffects[key] = std::make_unique<AirborneRadioEffect>();
        }
        return airborneEffects[key].get();
    }


    UnderWaterRadioEffect* getUnderwaterRadioEffect(const std::string& key) {
        LockGuard_shared lock_shared(m_lock);
        if (!ddEffects.count(key)) {
            lock_shared.unlock();
            LockGuard_exclusive lock_exclusive(m_lock);
            ddEffects[key] = std::make_unique<UnderWaterRadioEffect>();
        }
        return ddEffects[key].get();
    }

    Clunk* getClunk(const std::string& key) {
        LockGuard_shared lock_shared(m_lock);
        if (!clunks.count(key)) {
            lock_shared.unlock();
            LockGuard_exclusive lock_exclusive(m_lock);
            clunks[key] = std::make_unique<Clunk>();
        }
        return clunks[key].get();
    }

    void removeClunk(const std::string& key) {
        LockGuard_exclusive lock(m_lock);
        clunks.erase(key);
    }

    Dsp::SimpleFilter<Dsp::Butterworth::LowPass<4>, MAX_CHANNELS>* getFilterCantSpeak(const std::string& key) {
        LockGuard_shared lock_shared(m_lock);
        if (!filtersCantSpeak.count(key)) {
            lock_shared.unlock();
            LockGuard_exclusive lock_exclusive(m_lock);
            filtersCantSpeak[key] = std::make_unique<Dsp::SimpleFilter<Dsp::Butterworth::LowPass<4>, MAX_CHANNELS>>();
            filtersCantSpeak[key]->setup(4, 48000, 100);
        }
        return filtersCantSpeak[key].get();
    }

    void removeFilterCantSpeak(const std::string& key) {
        LockGuard_exclusive lock(m_lock);
        filtersCantSpeak.erase(key);
    }

    Dsp::SimpleFilter<Dsp::Butterworth::LowPass<2>, MAX_CHANNELS>* getFilterVehicle(const std::string& key, float vehicleVolumeLoss) {
        const auto byKey = key + std::to_string(vehicleVolumeLoss);
        LockGuard_shared lock_shared(m_lock);
        if (!filtersVehicle.count(byKey)) {
            lock_shared.unlock();
            LockGuard_exclusive lock_exclusive(m_lock);
            filtersVehicle[byKey] = std::make_unique<Dsp::SimpleFilter<Dsp::Butterworth::LowPass<2>, MAX_CHANNELS>>();
            filtersVehicle[byKey]->setup(2, 48000, 20000 * (1.0 - vehicleVolumeLoss) / 4.0);
        }
        return filtersVehicle[byKey].get();
    }

    void removeFilterVehicle(const std::string& key) {
        LockGuard_exclusive lock(m_lock);
        filtersVehicle.erase(key);
    }

    std::map<uint8_t, std::unique_ptr<Dsp::SimpleFilter<Dsp::Butterworth::LowPass<2>, MAX_CHANNELS>>> filtersObjectInterception;

    Dsp::SimpleFilter<Dsp::Butterworth::LowPass<2>, MAX_CHANNELS>* getFilterObjectInterception(uint8_t objectCount);


    chunkware_simple::SimpleComp compressor;
    void resetRadioEffect() {
        LockGuard_exclusive lock(m_lock);
        swEffects.clear();
        lrEffects.clear();
        ddEffects.clear();
        filtersSpeakers.clear();
        filtersPhone.clear();
    }

    void resetVoices() {
        LockGuard_exclusive lock(m_lock);
        //clunks.clear();
        filtersCantSpeak.clear();
        filtersVehicle.clear();
    }
private:
    ReadWriteLock m_lock{"clientDataEffects"};

    //Filters and Effects
    template<typename T>
    using effectMap = std::map<std::string, std::unique_ptr<T>, std::less<>>;

    effectMap<PersonalRadioEffect> swEffects;
    effectMap<LongRangeRadioEffect> lrEffects;
    effectMap<AirborneRadioEffect> airborneEffects;
    effectMap<UnderWaterRadioEffect> ddEffects;
    effectMap<Clunk> clunks;

    effectMap<Dsp::SimpleFilter<Dsp::Butterworth::LowPass<4>, MAX_CHANNELS>> filtersCantSpeak;
    effectMap<Dsp::SimpleFilter<Dsp::Butterworth::LowPass<2>, MAX_CHANNELS>> filtersVehicle;
    effectMap<Dsp::SimpleFilter<Dsp::Butterworth::BandPass<1>, MAX_CHANNELS>> filtersSpeakers;
    effectMap<Dsp::SimpleFilter<Dsp::Butterworth::BandPass<2>, MAX_CHANNELS>> filtersPhone;
};






class clientData { //enable_shared_from_this doesn't work.. don't ask me
public:


    explicit clientData(TSClientID _clientID) : clientId(_clientID) {

    }


    void updatePosition(const unitPositionPacket & packet);



    auto getNickname() const { LockGuard_shared lock(m_lock); return nickname; }
    void setNickname(std::string val) { LockGuard_exclusive lock(m_lock); nickname = std::move(val); }
    Position3D getClientPosition() const;       
    Position3D getClientPositionRaw() const { LockGuard_shared lock(m_lock); return clientPosition; }
    void setClientPosition(const Position3D& val) { LockGuard_exclusive lock(m_lock); clientPosition = val; }
    auto getViewDirection() const { LockGuard_shared lock(m_lock); return viewDirection; }
    void setViewDirection(const Direction3D& val) { LockGuard_exclusive lock(m_lock); viewDirection = val; }
    auto getLastPositionUpdateTime() const { LockGuard_shared lock(m_lock); return lastPositionUpdateTime; }
    void setLastPositionUpdateTime(const std::chrono::system_clock::time_point& val) { LockGuard_exclusive lock(m_lock); lastPositionUpdateTime = val; }
    auto getCurrentTransmittingFrequency() const { LockGuard_shared lock(m_lock); return currentTransmittingFrequency; }
    void setCurrentTransmittingFrequency(std::string_view val) { LockGuard_exclusive lock(m_lock); currentTransmittingFrequency = val; }
    auto getCurrentTransmittingSubtype() const { LockGuard_shared lock(m_lock); return currentTransmittingSubtype; }
    void setCurrentTransmittingSubtype(std::string_view val) { LockGuard_exclusive lock(m_lock); currentTransmittingSubtype = val; }
    //Returns distance respecting TerrainInterception and Coefficients
    float effectiveDistanceTo(std::shared_ptr<clientData>& other) const;
    float effectiveDistanceTo(clientData* other) const;
    bool isAlive();

    operator Position3D() const { return getClientPosition(); } //convenience



    vehicleDescriptor getVehicleDescriptor() const {
        LockGuard_shared lock(m_lock);
        return vehicleId; //helpers::getVehicleDescriptor(getVehicleId());
    }




    LISTED_INFO isOverLocalRadio(std::shared_ptr<clientData>& myData, bool ignoreSwTangent, bool ignoreLrTangent, AntennaConnection& antennaConnection);
    std::vector<LISTED_INFO> isOverRadio(std::shared_ptr<clientData>& myData, bool ignoreSwTangent, bool ignoreLrTangent);



    void addModificationLog(std::string mod);
    std::vector<std::string> getModificationLog() const;
    void circularLog(const std::string& message);

    void verboseDataLog(std::ostream& str);

    std::vector <std::string> messages;
    uint32_t offset{ 0 };
    uint32_t messageCount{2048};

    //Types that are small enough not to need locks
    bool pluginEnabled = false;
    std::chrono::system_clock::time_point pluginEnabledCheck;
    const TSClientID clientId;
    sendingRadioType currentTransmittingTangentOverType = sendingRadioType::LISTEN_TO_NONE; //Radio type client is currently transmitting on
    int voiceVolume = 0;
    int range = 0;

    bool canSpeak = true;
    bool clientTalkingNow = false;
    int dataFrame = 0;

    bool canUseSWRadio = false;
    bool canUseLRRadio = false;
    bool canUseDDRadio = false;

    int terrainInterception = 0;
    int objectInterception = 0;
    float voiceVolumeMultiplifier = 1.f;
    bool isSpectating{ false };
    bool isEnemyToPlayer{ false };
    uint8_t receivingTransmission = 0; //This unit is currently receiving a transmission. Only works for local player
    std::set<std::string, std::less<>> receivingFrequencies;
    clientDataEffects effects;
    SampleBuffer OISampleBuffer;
private:
    mutable ReadWriteLock m_lock{"clientData"};

    std::vector<std::string> modificationLog;

    std::string nickname;

    Position3D clientPosition;
    Direction3D viewDirection;
    std::chrono::system_clock::time_point lastPositionUpdateTime;

    std::string currentTransmittingFrequency;//Frequency client is currently transmitting on
    std::string currentTransmittingSubtype;//subtype client is currently transmitting on

    vehicleDescriptor vehicleId;
    Vector3D velocity;

    void setVehicleId(std::string_view val) {
        //"2:390\x100.6\x10gunner"
        if (val == "no") {
            vehicleId.vehicleName = "no";
            vehicleId.vehicleIsolation = 0.f;
            vehicleId.intercomSlot = -1;
            velocity = { 0,0,0 };
            return;
        }
        auto split = helpers::split(val, '\x10');
        if (split.size() < 3) return; //I don't listen to morons!!
        vehicleId.vehicleName = split[0]; // hear vehicle
        auto& isolation = split[1];
        if (isolation == "turnout")
            vehicleId.vehicleIsolation = 0.f;
        else
            vehicleId.vehicleIsolation = helpers::parseArmaNumber(split[1]); // hear
        vehicleId.intercomSlot = helpers::parseArmaNumberToInt(split[2]);//vehicleDescriptor::stringToVehiclePosition(split[2]);
        if (split.size() > 3)
            velocity = Vector3D(split[3]);
        else
            velocity = { 0,0,0 };
    }
};

