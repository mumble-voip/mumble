#pragma once
#include "public_definitions.h"
#include <cmath>
#include <cstdint>
#include <vector>
#include <chrono>
#include <string_view>
#include <stdexcept>

#define M_PI_FLOAT 3.14159265358979323846f
namespace dataType {
    float fast_invsqrt(float number);


    template <typename Type>
    class TeamspeakID {
    public:
        constexpr TeamspeakID() : m_id(-1) {}
        constexpr TeamspeakID(Type id) : m_id(id) {}
        constexpr TeamspeakID(int id) : m_id(id) {}
        constexpr Type baseType() const { return m_id; }//Making this operator Type() will break operator bool in if statements... C++ Magic
        constexpr bool isValid() const noexcept { return m_id != static_cast<Type>(-1) && m_id > 0; }
        constexpr explicit operator bool() const noexcept { return isValid(); }
        constexpr bool operator!() const noexcept { return !isValid(); }
        constexpr bool operator== (const TeamspeakID& other) const noexcept { return m_id == other.m_id; }
        constexpr bool operator== (const int& other) const noexcept { return m_id == static_cast<Type>(other); }
        constexpr bool operator== (const Type& other) const noexcept { return m_id == other; }
        constexpr bool operator!= (const TeamspeakID& other) const noexcept { return m_id != other.m_id; }
        constexpr bool operator< (const TeamspeakID& other) const noexcept { return m_id < other.m_id; }
    private:
        Type m_id;
    };

    using TSClientID = TeamspeakID<anyID>;
    using TSChannelID = TeamspeakID<uint64_t>;
    using TSServerID = TeamspeakID<uint64_t>;

    // static_assert(TSClientID(5));
    static_assert(!TSClientID(-1).isValid(), "Empty TSClientID detection failed");

    class AngleDegrees;
    class AngleRadians {
        friend class AngleDegrees;
    public:
        constexpr AngleRadians(float radians) : angle(radians) {}
        //Conversions
        constexpr AngleRadians(const AngleDegrees& other);
        //Operators
        constexpr operator float() const { return angle; }
        AngleRadians operator+(const AngleRadians& other) const;
        //Functions
        float cosine() const { return cosf(angle); }
        float sine() const { return sinf(angle); }
        //Converts Angle from 0 to 360 to -180 to +180
        AngleRadians to180() const;
    protected:
        float angle;
    };
    constexpr AngleRadians operator "" _rad(long double _deg) { return AngleRadians(static_cast<float>(_deg)); }
    class AngleDegrees {
        friend class AngleRadians;
    public:
        constexpr AngleDegrees(float degrees) : angle(degrees) {}
        //Conversions
        constexpr AngleDegrees(const AngleRadians& other) : angle(other.angle * (180 / M_PI_FLOAT)) {};
        constexpr AngleRadians toRadians() const { return AngleRadians(*this); }
        //Operators
        constexpr operator float() const { return angle; }
        //Functions
        float cosine() const { return cosf(toRadians()); }
        float sine() const { return sinf(toRadians()); }
        AngleDegrees to180() const {
            AngleDegrees positive(std::fmod(angle, 360.f));
            if (positive.angle < 0) positive.angle += 360.f;
            return positive;
        }
    protected:
        float angle;
    };

    constexpr AngleRadians::AngleRadians(const AngleDegrees& other) : angle(other.angle * (M_PI_FLOAT / 180)) {}

    constexpr AngleDegrees operator "" _deg(long double _deg) { return AngleDegrees(static_cast<float>(_deg)); }

    class Vector3D {
    public:
        Vector3D() = default;
        Vector3D(float x, float y, float z);
        Vector3D(const std::vector<float>& vec);
        Vector3D(std::string_view coordinateString);
        Vector3D(Vector3D&& vec) noexcept : m_x(vec.m_x), m_y(vec.m_y), m_z(vec.m_z) {};
        Vector3D(const Vector3D& vec) = default;

        std::tuple<float, float, float> get() const; //#TODO instead of using get.. how about operator[] ?
        float& operator[](int offs) {
            switch (offs) {
                case 0: return m_x;
                case 1: return m_y;
                case 2: return m_z;
                default: throw std::out_of_range("Index out of range. A 3D vector only has 3 values.");
            }
        }
        std::string toString() const;


        float length() const;
        float lengthSqr() const;
        float dotProduct(const Vector3D& other) const;
        Vector3D crossProduct(const Vector3D& other) const;
        Vector3D normalized() const;
        bool isNull() const;
        Vector3D operator*(float multiplier) const {
            return{ m_x *multiplier,m_y *multiplier ,m_z *multiplier };
        }
        Vector3D& operator=(const Vector3D& other) = default;
        Vector3D operator-(const Vector3D& other) const;
        Vector3D operator+(const Vector3D& other) const;
        bool operator< (const Vector3D& other) const;
        bool operator== (const Vector3D& other) const;
        Vector3D operator/(float div) const;
    protected:
        float m_x = 0.f;
        float m_y = 0.f;
        float m_z = 0.f;
    };


    class Direction3D;
    class Position3D : public Vector3D {
    public:
        using Vector3D::Vector3D;
        //Initializers
        Position3D() = default;
        Position3D(const Vector3D& other) : Vector3D(other) {}
        //explicit Position3D(const Position3D &obj) = delete;
        //explicit Position3D(const TS3_VECTOR& vec) :m_x(vec.x), m_y(vec.y), m_z(vec.z) {}
        //Conversions
        operator TS3_VECTOR*();
        //Operators
        Position3D operator+(const Vector3D& other) const;
        //Functions
        float getHeight() const;
        float distanceTo(const Position3D& other) const;
        float distanceToSqr(const Position3D& other) const;
        float distanceUnderwater(const Position3D& other) const;
        Direction3D directionTo(const Position3D& other) const;
        Position3D crossProduct(const Position3D& other) const;
    };

    //Unit direction Vector
    class Direction3D : public Vector3D {
        friend class Position3D;
    public:
        template <typename ...Args>
        Direction3D(Args&&... args) : Vector3D(std::forward<Args>(args)...) { (void)"This is nuts!"; }
        explicit Direction3D(const Position3D& from, const Position3D& to);
        AngleRadians toAngle() const;
        AngleRadians toPolarAngle() const;
        Position3D getpos() const { return Position3D(m_x, m_y, m_z); }

        //Direction3D getUpVector();
        //RotationMatrix toRotationMatrix();
        operator AngleRadians() const { return toAngle(); }
        operator AngleDegrees() const { return AngleDegrees(toAngle()); }
    };


    class Velocity3D : public Vector3D {
    public:
        using Vector3D::operator*;
        //Initializers
        Velocity3D() = default;
        Velocity3D(Vector3D other) : Vector3D(std::move(other)) {};
        //Operators
        Velocity3D operator*(const std::chrono::duration<float>& time) const;

        //Functions

    };

    class RotationMatrix {
    public:
        Vector3D right;
        Vector3D up;
        Vector3D forward;
    };

    class NetID {
    public:
        NetID(std::string_view netIDStr) {
            const char *idStr = strchr(netIDStr.data(), ':');
            if (idStr) {
                creator = atoi(netIDStr.data());
                objID = atoi(idStr + 1);
            }
        }
        bool operator<(const NetID &other) const {
            return creator < other.creator || objID < other.objID;
        }
        bool operator ==(const NetID &other) const {
            return other.creator == creator && other.objID == objID;
        }
        bool operator !=(const NetID &other) const {
            return other.creator != creator || other.objID != objID;
        }
        bool isNull() const { return objID == 0; }
        int getCreator() const { return creator; }
        int getObjID() const { return objID; }
    private:
        int creator{ 0 };
        int objID{ 0 };
    };

}




namespace std {
    template <typename Type>
    struct hash<dataType::TeamspeakID<Type> > {
        size_t operator()(const dataType::TeamspeakID<Type>& id) const noexcept { return id.baseType(); }
    };
}
