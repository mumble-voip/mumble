#ifndef CLUNK_SAMPLE_H__
#define CLUNK_SAMPLE_H__

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
#include <clunk/buffer.h>
#include <clunk/audio_spec.h>

namespace clunk {
class Context;

//!Holds raw wave data. 
class CLUNKAPI Sample {
public: 
	///name - for general purpose
	std::string name;
	///gain
	float gain;
	///pitch, 2.0f - pitching up one octave
	float pitch;

	~Sample();

	/*!
		\brief initializes sample
		\param[in] data raw audio data
		\param[in] spec audio format specification
	*/	
	void init(const clunk::Buffer &data, const AudioSpec &spec);

	/*! 
		\brief generate sine wave with given length (seconds)
		\param[in] freq frequency
		\param[in] len of the sample in seconds
	*/
	void generateSine(int freq, float len);

	float length() const {
		return 1.0f * _data.get_size() / _spec.sample_rate / _spec.channels / 2;
	}

	const clunk::Buffer & get_data() const	{ return _data; }
	const AudioSpec get_spec() const		{ return _spec; }

private:
	friend class Context;

	Sample(Context *context);

	Sample(const Sample &);
	const Sample& operator=(const Sample &);

	Context *		_context;
	AudioSpec		_spec;
	clunk::Buffer	_data;
};
}

#endif

