// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_POSITIONAL_AUDIO_CONTEXT_H_
#define MUMBLE_MUMBLE_POSITIONAL_AUDIO_CONTEXT_H_

#include <QtCore/QReadWriteLock>
#include <QtCore/QString>

/// An enum for the three cartesian coordinate axes x, y and z
enum class Coord { X = 0, Y, Z };

/// A 3D vector class holding an x-, y- and z-coordinate
struct Vector3D {
	/// The vector's x-coordinate
	float x;
	/// The vector's y-coordinate
	float y;
	/// The vector's z-coordinate
	float z;

	/// Access the respective coordinate in an array-like fashion
	///
	/// @param coord The Coord to access
	/// @returns The value of the respective coordinate
	float operator[](Coord coord) const;
	/// @param factor The factor to scale by
	/// @returns A new vector that has been created by scaling this vector by the given factor
	Vector3D operator*(float factor) const;
	/// @param divisor The divisor to apply to all coordinates
	/// @returns A new vector obtained from this one by applying the divisor to all coordinates
	Vector3D operator/(float divisor) const;
	/// Scales this vector by the given factor
	///
	/// @param factor The factor to use
	void operator*=(float factor);
	/// Divides all of this vector's coordinates by the given divisor
	///
	/// @param divisor The divisor to use
	void operator/=(float divisor);
	/// @param other The vector to compare this one to
	/// @returns Whether the given vector is equal to this one (their coordinates are the same)
	bool operator==(const Vector3D &other) const;
	/// @param other The vector to subtract from this one
	/// @returns A new vector representing the difference of this vector and the other one
	Vector3D operator-(const Vector3D &other) const;
	/// @param other The vector to add to this one
	/// @returns A new vector representing the sum of this vector and the other one
	Vector3D operator+(const Vector3D &other) const;
	/// @param other The vector to copy
	/// @returns A copy of the other vector
	Vector3D &operator=(const Vector3D &other) = default;

	// allow explicit conversions from this struct to a float-array / float-pointer
	/// Explicit conversion to a float-array (of length 3) containing the coordinates of this vector
	explicit operator const float *() const { return &x; };
	/// Explicit conversion to a float-array (of length 3) containing the coordinates of this vector
	explicit operator float *() { return &x; };

	/// Default constructor - sets all coordinates to 0
	Vector3D();
	/// @param x The x-coordinate
	/// @param y The y-coordinate
	/// @param z The z-coordinate
	Vector3D(float x, float y, float z);
	/// Copy constructor
	///
	/// @param other The vector to copy
	Vector3D(const Vector3D &other);
	/// Destructor
	~Vector3D();
	/// @returns The squared euclidean norm (length of the vector)
	float normSquared() const;
	/// If possible normSquared() should be preferred as this doesn't require a square-root operator
	///
	/// @returns The euclidean norm (length of the vector)
	float norm() const;
	/// @param other The vector to calculate the dot-product with
	/// @returns The dot-product between this vector an the other one
	float dotProduct(const Vector3D &other) const;
	/// @param other The vector to calculate the cross-product (vector-product) with
	/// @returns The vector resulting from the cross-product (vector-product)
	Vector3D crossProduct(const Vector3D &other) const;
	/// @param other The vector to compare this one to
	/// @param threshold The maximum absolute difference for coordinates to still be considered equal
	/// @returns Whether this and the given vector are equal
	bool equals(const Vector3D &other, float threshold = 0.0f) const;
	/// @param threshold The maximum absolute value a coordinate may have to still be considered zero
	/// @returns Whether this vector is the zero-vector
	bool isZero(float threshold = 0.0f) const;
	/// Normalizes this vector to a unit-vector. Callin this function on a zero-vector results in undefined behaviour!
	void normalize();
	/// Transforms this vector to a zero-vector by setting all coordinates to zero
	void toZero();
};

// As we're casting the vector struct to float-arrays, we have to make sure that the compiler won't introduce any
// padding into the structure
static_assert(sizeof(Vector3D) == 3 * sizeof(float),
			  "The compiler added padding to the Vector3D structure so it can't be cast to a float-array!");

/// A convenient alias as a position can be treated the same way a vector can
typedef Vector3D Position3D;


/// A class holding positional data used in the positional audio feature
class PositionalData {
	friend class PluginManager; // needed in order for PluginManager::fetch to write to the contained fields
protected:
	/// The player's position in the 3D world
	Position3D m_playerPos;
	/// The direction in which the player is looking
	Vector3D m_playerDir;
	/// The connection vector between the player's feet and his/her head
	Vector3D m_playerAxis;
	/// The camera's position un the 3D world
	Position3D m_cameraPos;
	/// The direction in which the camera is looking
	Vector3D m_cameraDir;
	/// The connection from the camera's bottom to its top
	Vector3D m_cameraAxis;
	/// The context of this positional data. This might include the game's name, the server currently connected to, etc.
	/// and is used to determine which players can hear one another
	QString m_context;
	/// The player's ingame identity (name)
	QString m_identity;
	/// The lock guarding all fields of this class
	mutable QReadWriteLock m_lock;

public:
	/// Default constructor
	PositionalData();
	/// Constructor initializing all fields to a specific value
	PositionalData(Position3D playerPos, Vector3D playerDir, Vector3D playerAxis, Position3D cameraPos,
				   Vector3D cameraDir, Vector3D cameraAxis, QString context, QString identity);
	/// Destructor
	~PositionalData();
	/// @param[out] pos The player's 3D position
	void getPlayerPos(Position3D &pos) const;
	/// @returns The player's 3D position
	Position3D getPlayerPos() const;
	/// @param[out] vec The direction in which the player is currently looking
	void getPlayerDir(Vector3D &vec) const;
	/// @returns The direction in which the player is currently looking
	Vector3D getPlayerDir() const;
	/// @param[out] axis The connection between the player's feet and his/her head
	void getPlayerAxis(Vector3D &axis) const;
	/// @returns The connection between the player's feet and his/her head
	Vector3D getPlayerAxis() const;
	/// @param[out] pos The camera's 3D position
	void getCameraPos(Position3D &pos) const;
	/// @returns The camera's 3D position
	Position3D getCameraPos() const;
	/// @param[out] vec The direction in which the camera is currently looking
	void getCameraDir(Vector3D &vec) const;
	/// @returns The direction in which the camera is currently looking
	Vector3D getCameraDir() const;
	/// @param[out] axis The connection between the player's feet and his/her head
	void getCameraAxis(Vector3D &axis) const;
	/// @returns The connection between the player's feet and his/her head
	Vector3D getCameraAxis() const;
	/// @returns The player's identity
	QString getPlayerIdentity() const;
	/// @returns The current context
	QString getContext() const;
	/// Resets all fields in this object
	void reset();
};

#endif
