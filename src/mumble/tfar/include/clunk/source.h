#ifndef CLUNK_SOURCE_H__
#define CLUNK_SOURCE_H__

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
#include <clunk/buffer.h>
#include <clunk/hrtf.h>

namespace clunk {


class Sample;
class Buffer;

//!class holding information about source. 
class CLUNKAPI Source {
public:
	///pointer to the sample holding audio data
	const Sample * const sample;
	
	///loop flag
	bool loop;
	///delta position from the object's center
	v3f delta_position; //0 - from the center of the object. 
	///gain
	float gain;
	///pitch, 2.0f - pitching up one octave
	float pitch;
	/*!
		panning. -1.0f = 2 * vol in left channel, 0 in right. +1.0f = no sound in left channel, 2 * vol in right.
		note: panning is actually applied on mono samples in center(listener) position.
	*/
	float panning;
	/*! 
		\brief constructs new source
		\param[in] sample audio data
		\param[in] loop loops sample
		\param[in] delta delta position. (0, 0) is the object's center.
		\param[in] gain gain
		\param[in] pitch pitch
	*/
	Source(const Sample * sample, const bool loop = false, const v3f &delta = v3f(), float gain = 1, float pitch = 1, float panning = 0);
	///returns current source's status.
	bool playing() const;

	///fades out source. usually you do not need this method
	void fade_out(float sec);
	
	~Source();

	/*! 
		\brief for the internal use only. DO NOT USE IT. 
		\internal for the internal use only. 
	*/
	void _update_position(int dp);

	/*! 
		\brief for the internal use only. DO NOT USE IT. 
		\internal for the internal use only. 
	*/
	float _process(clunk::Buffer &buffer, unsigned ch, const v3f &position, float fx_volume, float pitch);

private: 
	int position, fadeout, fadeout_total;
	Hrtf _hrtf;
};
}

#endif
