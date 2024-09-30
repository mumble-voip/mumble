// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VolumeAdjustment.h"

#include <cassert>
#include <cmath>

constexpr float DB_THRESHOLD = 0.1f;

VolumeAdjustment::VolumeAdjustment(float factor, int dbAdjustment) : factor(factor), dbAdjustment(dbAdjustment) {
	assert(dbAdjustment == INVALID_DB_ADJUSTMENT
		   // Verify that the used dbAdjustment is a reasonable representation of the given factor. Particularly, we
		   // want to make sure that the deviation of the true dB representation of the factor (usually a floating point
		   // value) doesn't deviate more than dbThreshold from the passed integer dB value. For all cases, where this
		   // would be the case, we expect INVALID_DB_ADJUSTMENT to be passed instead.
		   //
		   // If dB is the dB-representation of a loudness change factor f, we have
		   // dB = log2(f) * 6    <=>    f = 2^{dB/6}
		   // (+6dB equals a doubling in loudness)
		   || DB_THRESHOLD >= std::abs(static_cast< float >(dbAdjustment) - VolumeAdjustment::toDBAdjustment(factor)));
}

// Decibel formula: +6db = *2
float VolumeAdjustment::toFactor(int dbAdjustment) {
	return static_cast< float >(std::pow(2.0, dbAdjustment / 6.0));
}

float VolumeAdjustment::toDBAdjustment(float factor) {
	return static_cast< float >(std::log2(factor) * 6.0);
}

int VolumeAdjustment::toIntegerDBAdjustment(float factor) {
	return static_cast< int >(std::roundf(VolumeAdjustment::toDBAdjustment(factor)));
}

VolumeAdjustment VolumeAdjustment::fromFactor(float factor) {
	if (factor > 0) {
		float dB = VolumeAdjustment::toDBAdjustment(factor);

		if (std::abs(dB - static_cast< float >(static_cast< int >(dB))) < DB_THRESHOLD) {
			// Close-enough
			return VolumeAdjustment(factor, static_cast< int >(std::round(dB)));
		} else {
			return VolumeAdjustment(factor, INVALID_DB_ADJUSTMENT);
		}
	} else {
		return VolumeAdjustment(factor, INVALID_DB_ADJUSTMENT);
	}
}

VolumeAdjustment VolumeAdjustment::fromDBAdjustment(int dbAdjustment) {
	float factor = VolumeAdjustment::toFactor(dbAdjustment);

	return VolumeAdjustment(factor, dbAdjustment);
}

bool operator==(const VolumeAdjustment &lhs, const VolumeAdjustment &rhs) {
	return lhs.dbAdjustment == rhs.dbAdjustment && std::abs(lhs.factor - rhs.factor) < 0.1f;
}

bool operator!=(const VolumeAdjustment &lhs, const VolumeAdjustment &rhs) {
	return !(lhs == rhs);
}
