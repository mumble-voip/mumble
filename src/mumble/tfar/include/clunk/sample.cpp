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


#include <clunk/sample.h>
#include <clunk/context.h>
#include <clunk/locker.h>
#include <clunk/logger.h>
#include <clunk/resample.h>
#include <stdexcept>
#pragma warning( push )
#pragma warning( disable : 4101 ) // unreferenced local variable
using namespace clunk;

Sample::Sample(Context *context) : gain(1.0f), pitch(1.0f), _context(context) {}

void Sample::generateSine(const int freq, const float len) {
	AudioLocker l;

	_spec.sample_rate = _context->get_spec().sample_rate;
	_spec.channels = 1;
	_spec.format = _context->get_spec().format;

	unsigned size = ((int)(len * _spec.sample_rate)) * 2;
	_data.set_size(size);

	static double a = 0;
	double da = freq * 2 * M_PI / _spec.sample_rate;
	//LOG_DEBUG(("da = %g", da));
	
	int n = size / 2;

	s16 * stream = static_cast<s16 *>(_data.get_ptr());
	for(int i = 0; i < n; ++i) {
		*stream++ = (s16)(32767 * sin(a));
		//*stream++ = 0;
		a += da;
	}
	LOG_DEBUG(("generated %u bytes", (unsigned)_data.get_size()));
}

void Sample::init(const clunk::Buffer &src_data, const AudioSpec &spec) {
	AudioLocker l;

	_spec.sample_rate = _context->get_spec().sample_rate;
	_spec.channels = 1;
	_spec.format = _context->get_spec().format;
	Resample::resample(_spec, _data, spec, src_data);
}

Sample::~Sample() { }
#pragma warning( pop )
