// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_VOLUMEADJUSTMENT_H_
#define MUMBLE_VOLUMEADJUSTMENT_H_

#include <limits>

class VolumeAdjustment {
public:
	static constexpr const int INVALID_DB_ADJUSTMENT = std::numeric_limits< int >::max();

	explicit VolumeAdjustment(float factor = 1.0f, int dbAdjustment = INVALID_DB_ADJUSTMENT);

	float factor;
	int dbAdjustment;

	static float toFactor(int dbAdjustment);
	static float toDBAdjustment(float factor);
	static int toIntegerDBAdjustment(float factor);

	static VolumeAdjustment fromFactor(float factor);
	static VolumeAdjustment fromDBAdjustment(int dbAdjustment);

	friend bool operator==(const VolumeAdjustment &lhs, const VolumeAdjustment &rhs);
	friend bool operator!=(const VolumeAdjustment &lhs, const VolumeAdjustment &rhs);
};


#endif
