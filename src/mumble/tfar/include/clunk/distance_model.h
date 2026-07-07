#ifndef CLUNK_DISTANCE_MODEL_H__
#define CLUNK_DISTANCE_MODEL_H__

/* clunk - cross-platform 3D audio API built on top SDL library
 * Copyright (C) 2007-2014 Netive Media Group
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <clunk/export_clunk.h>
#include <clunk/v3.h>

namespace clunk {

//!Distance model used in context.

struct CLUNKAPI DistanceModel {
	//!Type of the distance model: inversed, linear or exponent.
	enum Type { Inverse, Linear, Exponent };

	//!Type of the distance model
	Type type;
	//!Clamping means cutting off max_distance, you must set max_distance to some value then.
	bool clamped;
	
	//!Minimal distance for distance model, default == 1
	float reference_distance;
	//!Maximum distance for clamping distance models. MUST NOT BE DEFAULT
	float max_distance; 

	//!Rolloff factor for distance models
	float rolloff_factor;
	
	//!Dopple factor. 0 disables dopple effect
	float doppler_factor;
	
	//!Speed of sound
	float speed_of_sound;
	
	//! distance_divisor
	float distance_divisor;
	
	//! limit same sounds near you
	unsigned same_sounds_limit;
	
	/*!
		\brief Constructor
		\param[in] type type of the distance model: inversed, linear or exponent.
		\param[in] clamped means that max_distance will be used.
		\param[in] max_distance maximum distance for the model.
	*/ 
	DistanceModel(Type type, bool clamped, float max_distance = 0): type(type), clamped(clamped), 
	reference_distance(1), max_distance(max_distance), rolloff_factor(1), doppler_factor(0), 
	speed_of_sound(343.3f), distance_divisor(1), same_sounds_limit(2)
	{}
	
	//! Computes gain by distance. Return values is in [0-1] range.
	float gain(float distance) const;
	//! Computes doppler pitch.
	float doppler_pitch(const v3f &sl, const v3f &s_vel, const v3f &l_vel) const;
};

}

#endif

