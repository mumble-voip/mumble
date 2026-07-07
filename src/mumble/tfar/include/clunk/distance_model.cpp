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

#include <clunk/distance_model.h>

float clunk::DistanceModel::gain(float distance) const {
	float gain = 0;
	distance /= distance_divisor;
	switch(type) {
	case Inverse: 
		if (clamped) {
			if (distance < reference_distance)
				distance = reference_distance;
			if (distance > max_distance)
				distance = max_distance;
		}
		gain = reference_distance / (reference_distance + rolloff_factor * (distance - reference_distance));
		break;
		
	case Linear: 
		if (clamped) {
			if (distance < reference_distance)
				distance = reference_distance;
			if (distance > max_distance)
				distance = max_distance;			
		} 
		
		gain = 1 - rolloff_factor * (distance - reference_distance) / (max_distance - reference_distance);
		break;
	
	case Exponent: 
		if (clamped) {
			if (distance < reference_distance)
				distance = reference_distance;
			if (distance > max_distance)
				distance = max_distance;
		}
		gain = powf(distance / reference_distance, - rolloff_factor);
		
		break;	
	}

	if (gain < 0)
		gain = 0;
	else if (gain > 1)
		gain = 1;

	return gain;
}


float clunk::DistanceModel::doppler_pitch(const v3f &sl, const v3f &s_vel, const v3f &l_vel) const {
	if (doppler_factor <= 0)
		return 1.0f;
	
	float len = sl.length();
	if (len <= 0)
		return 1.0f;
	
	float max_speed = speed_of_sound / doppler_factor;
	
	float vls = sl.dot_product(l_vel) / len;
	if (vls > max_speed)
		vls = max_speed;

	float vss = sl.dot_product(s_vel) / len;
	if (vss > max_speed)
		vss = max_speed;
	
	return (speed_of_sound - doppler_factor * vls) / (speed_of_sound - doppler_factor * vss);
}
