// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PositionalData.h"

#include <cmath>
#include <cstdlib>
#include <stdexcept>

#include <QtCore/QReadLocker>

Vector3D::Vector3D() : x(0.0f), y(0.0f), z(0.0f) {
}

Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z) {
}

Vector3D::Vector3D(const Vector3D &other) : x(other.x), y(other.y), z(other.z) {
}

Vector3D::~Vector3D() {
}

float Vector3D::operator[](Coord coord) const {
	switch (coord) {
		case Coord::X:
			return x;
		case Coord::Y:
			return y;
		case Coord::Z:
			return z;
	}

	// invalid index
	throw std::out_of_range("May only access x, y or z");
}

Vector3D Vector3D::operator*(float factor) const {
	return { x * factor, y * factor, z * factor };
}

Vector3D Vector3D::operator/(float divisor) const {
	return { x / divisor, y / divisor, z / divisor };
}

void Vector3D::operator*=(float factor) {
	x *= factor;
	y *= factor;
	z *= factor;
}

void Vector3D::operator/=(float divisor) {
	x /= divisor;
	y /= divisor;
	z /= divisor;
}

bool Vector3D::operator==(const Vector3D &other) const {
	return equals(other, 0.0f);
}

Vector3D Vector3D::operator-(const Vector3D &other) const {
	return { x - other.x, y - other.y, z - other.z };
}

Vector3D Vector3D::operator+(const Vector3D &other) const {
	return { x + other.x, y + other.y, z + other.z };
}

float Vector3D::normSquared() const {
	return x * x + y * y + z * z;
}

float Vector3D::norm() const {
	return std::sqrt(normSquared());
}

float Vector3D::dotProduct(const Vector3D &other) const {
	return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::crossProduct(const Vector3D &other) const {
	return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
}

bool Vector3D::equals(const Vector3D &other, float threshold) const {
	if (threshold == 0.0f) {
		return x == other.x && y == other.y && z == other.z;
	} else {
		threshold = std::abs(threshold);

		return std::abs(x - other.x) < threshold && std::abs(y - other.y) < threshold
			   && std::abs(z - other.z) < threshold;
	}
}

bool Vector3D::isZero(float threshold) const {
	if (threshold == 0.0f) {
		return x == 0.0f && y == 0.0f && z == 0.0f;
	} else {
		return std::abs(x) < threshold && std::abs(y) < threshold && std::abs(z) < threshold;
	}
}

void Vector3D::normalize() {
	float len = norm();

	x /= len;
	y /= len;
	z /= len;
}

void Vector3D::toZero() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

PositionalData::PositionalData()
	: m_playerPos(), m_playerDir(), m_playerAxis(), m_cameraPos(), m_cameraDir(), m_cameraAxis(), m_context(),
	  m_identity(), m_lock(QReadWriteLock::NonRecursive) {
}

PositionalData::PositionalData(Position3D playerPos, Vector3D playerDir, Vector3D playerAxis, Position3D cameraPos,
							   Vector3D cameraDir, Vector3D cameraAxis, QString context, QString identity)
	: m_playerPos(playerPos), m_playerDir(playerDir), m_playerAxis(playerAxis), m_cameraPos(cameraPos),
	  m_cameraDir(cameraDir), m_cameraAxis(cameraAxis), m_context(context), m_identity(identity),
	  m_lock(QReadWriteLock::NonRecursive) {
}

PositionalData::~PositionalData() {
}


void PositionalData::getPlayerPos(Position3D &pos) const {
	QReadLocker lock(&m_lock);

	pos = m_playerPos;
}

Position3D PositionalData::getPlayerPos() const {
	QReadLocker lock(&m_lock);

	return m_playerPos;
}

void PositionalData::getPlayerDir(Vector3D &vec) const {
	QReadLocker lock(&m_lock);

	vec = m_playerDir;
}

Vector3D PositionalData::getPlayerDir() const {
	QReadLocker lock(&m_lock);

	return m_playerDir;
}

void PositionalData::getPlayerAxis(Vector3D &vec) const {
	QReadLocker lock(&m_lock);

	vec = m_playerAxis;
}

Vector3D PositionalData::getPlayerAxis() const {
	QReadLocker lock(&m_lock);

	return m_playerAxis;
}

void PositionalData::getCameraPos(Position3D &pos) const {
	QReadLocker lock(&m_lock);

	pos = m_cameraPos;
}

Position3D PositionalData::getCameraPos() const {
	QReadLocker lock(&m_lock);

	return m_cameraPos;
}

void PositionalData::getCameraDir(Vector3D &vec) const {
	QReadLocker lock(&m_lock);

	vec = m_cameraDir;
}

Vector3D PositionalData::getCameraDir() const {
	QReadLocker lock(&m_lock);

	return m_cameraDir;
}

void PositionalData::getCameraAxis(Vector3D &vec) const {
	QReadLocker lock(&m_lock);

	vec = m_cameraAxis;
}

Vector3D PositionalData::getCameraAxis() const {
	QReadLocker lock(&m_lock);

	return m_cameraAxis;
}

QString PositionalData::getPlayerIdentity() const {
	QReadLocker lock(&m_lock);

	return m_identity;
}

QString PositionalData::getContext() const {
	QReadLocker lock(&m_lock);

	return m_context;
}

void PositionalData::reset() {
	m_playerPos.toZero();
	m_playerDir.toZero();
	m_playerAxis.toZero();
	m_cameraPos.toZero();
	m_cameraDir.toZero();
	m_cameraAxis.toZero();
	m_context  = QString();
	m_identity = QString();
}
