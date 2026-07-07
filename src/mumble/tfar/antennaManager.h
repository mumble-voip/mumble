#pragma once
#include "datatypes.hpp"
#include "Locks.hpp"
#include <memory>
#include <string>
#include <iostream>

using namespace dataType;
class Antenna {
public:
    Antenna(NetID _id, Position3D _pos, float _range) : id(_id), pos(std::move(_pos)), range(_range), rangeSquared(_range*_range) {}
    Antenna(Antenna&& other) noexcept : id(other.id), pos(std::move(other.pos)), range(other.range), rangeSquared(other.rangeSquared) { }

    float distanceTo(const Position3D& other) const { return pos.distanceTo(other); }
    float lossTo(const Position3D& other) const {
        if (!range) return 1.f;
        const auto dist = pos.distanceTo(other);
        if (dist > range) return 1.f;
        return dist / range;
    }
    float lossFrom(const Position3D& from, float maxDistance) const {
        if (!maxDistance) return 1.f;
        const auto dist = pos.distanceTo(from);
        if (dist > maxDistance) return 1.f;
        return dist / maxDistance;
    }
    bool canBeReachedBy(const Position3D& from, float maxDistanceSquared) const {
        return pos.distanceToSqr(from) < maxDistanceSquared;
    }
    bool canReach(const Position3D& to) const {
        return pos.distanceToSqr(to) < rangeSquared;
    }
    float connectionLoss(const Position3D& from, float maxDistanceToAnt, const Position3D& to) const {
        return lossTo(to) + lossFrom(from, maxDistanceToAnt);
    }
    //Operators
    Antenna& operator=(Antenna&& ant) noexcept {
        id = ant.id;
        pos = std::move(ant.pos);
        range = ant.range;
        rangeSquared = ant.rangeSquared;
        satUplink = ant.satUplink;
        return *this;
    }
    bool operator==(const NetID& antID) const {
        return id == antID;
    }

    std::ostream& operator<<(std::ostream& str) const {
        str << "id" << id.getCreator() << ":" << id.getObjID() << "\n";
        str << "pos" << pos.toString() << "\n";
        str << "range" << range << "\n";
        str << "rangeSquared" << rangeSquared << "\n";
        str << "satUplink" << satUplink << "\n";
        return str;
    }


    //Conversions
    operator Position3D&() { return pos; }
    operator NetID&() { return id; }

    //Setters
    void setID(NetID _id) { id = _id; }
    void setPos(Position3D _pos) { pos = std::move(_pos); }
    void setRange(float _range) { range = _range; }
    void setSatelliteUplink(bool hasUplink) { satUplink = hasUplink; }
    //Getters
    NetID getID() const { return id; }
    Position3D getPos() const { return pos; }
    float getRange() const { return range; }
    bool hasSatelliteUplink() const { return satUplink; }

private:
    NetID id;
    Position3D pos;
    float range{ 0 };
    float rangeSquared{ 0 };
    bool satUplink{ false }; //future functionality
};

inline std::ostream& operator<<(std::ostream &os, const Antenna& w) {
    return w.operator<<(os);
}

class AntennaConnection {
public:
    AntennaConnection() = default;
    AntennaConnection(Position3D _from, std::shared_ptr<Antenna> _overAntenna,Position3D _to,float _loss) : 
        from(std::move(_from)), overAntenna(std::move(_overAntenna)), to(std::move(_to)), loss(_loss) {}
    float getLoss() const { return loss; }
    bool isNull() const { return loss == 7.f; }//Magic number. Loss will never be >1
    std::shared_ptr<Antenna> getAntenna() const { return overAntenna; }

    std::ostream&  operator<<(std::ostream& str) const {
        if (overAntenna)
            str << "overAntenna " << *overAntenna<< "\n";
        else
            str << "overAntenna <null> \n";
        str << "from" << from.toString() << "\n";
        str << "to" << to.toString() << "\n";
        str << "loss" << loss << "\n";
        return str;
    }

private:
    Position3D from;
    std::shared_ptr<Antenna> overAntenna;
    Position3D to;
    float loss {7.f};
};

inline std::ostream& operator<<(std::ostream &os, const AntennaConnection& w) {
    return w.operator<<(os);
}

class AntennaManager {
public:
    AntennaManager();
    ~AntennaManager() = default;

    AntennaConnection findConnection(const Position3D& from, float maxDistanceToAnt, const Position3D& to);
    void addAntenna(Antenna ant);
    void removeAntenna(const NetID& antennaID);
private:
    std::vector<std::shared_ptr<Antenna>>::iterator findAntenna(const NetID& antennaID);


    //There are better containers for this. May rework this later
    std::vector<std::shared_ptr<Antenna>> antennas;

    ReadWriteLock m_lock{"AntennaManager"};
};

