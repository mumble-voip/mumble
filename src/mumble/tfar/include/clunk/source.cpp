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


#include <clunk/source.h>
#include <clunk/clunk_ex.h>
#include <clunk/buffer.h>
#include <clunk/sample.h>
#include <assert.h>
#include <clunk/clunk_assert.h>
#include <clunk/mixer.h>

#if defined _MSC_VER || __APPLE__ || __FreeBSD__
#	define pow10f(x) powf(10.0f, (x))
#	define log2f(x) (logf(x) / M_LN2)
#endif

using namespace clunk;

Source::Source(const Sample * sample, const bool loop, const v3f &delta, float gain, float pitch, float panning):
	sample(sample), loop(loop), delta_position(delta), gain(gain), pitch(pitch), panning(panning), 
	position(0), fadeout(0), fadeout_total(0)
{	
	if (sample == NULL)
		throw_ex(("sample for source cannot be NULL"));
}
	
bool Source::playing() const {
	if (fadeout_total > 0 && fadeout <= 0)
		return false;

	if (loop) 
		return true;
	
	//if (!sample3d[0].empty() || !sample3d[1].empty())
	//	return true;
	
	return position < (int)(sample->get_data().get_size() / sample->get_spec().channels / 2);
}
	
float Source::_process(clunk::Buffer &dst_buf, unsigned dst_ch, const v3f &delta_position, float fx_volume, float pitch) {
	
	const s16 * src = static_cast<const s16 *>(sample->get_data().get_ptr());
	if (src == NULL)
			throw_ex(("uninitialized sample used (%p)", (void *)sample));

	pitch *= this->pitch * sample->pitch;
	if (pitch <= 0)
		throw_ex(("pitch %g could not be negative or zero", pitch));

	unsigned src_ch = sample->get_spec().channels;
	unsigned src_n = (unsigned)sample->get_data().get_size() / src_ch / 2;
	unsigned dst_n = (unsigned)dst_buf.get_size() / dst_ch / 2;

	float vol = fx_volume * gain * sample->gain;
	
	if (vol > 1)
		vol = 1;

	Buffer src_buf;
	unsigned dst_n_plus_overlap = dst_n + Hrtf::WINDOW_SIZE;
	src_buf.set_size(dst_ch * dst_n_plus_overlap * 2);
	s16 * src_buf_ptr = static_cast<s16 *>(src_buf.get_ptr());
	for(unsigned i = 0; i < dst_n_plus_overlap; ++i) {
		for(unsigned c = 0; c < dst_ch; ++c) {
			int p = position + (int)(i * pitch);

			s16 v = 0;
			if (loop || (p >= 0 && p < (int)src_n)) {
				p %= src_n;
				if (p < 0)
					p += src_n;

				if (c < src_ch) {
					v = src[p * src_ch + c];
				} else {
					v = src[p * src_ch];//expand mono channel if needed
				}

				if (panning != 0 && c < 2) {
					bool left = c == 0;
					int v0 = (int)((1.0f + panning * (left? -1: 1)) * v);
					if (v0 > 32767) {
						v = 32767;
					} else if (v0 < -32767) {
						v = -32767;
					} else {
						v = (s16)v0;
					}
				}
				if (fadeout_total > 0 && fadeout - i <= 0) {
					v = 0;
				}
				if (fadeout_total > 0 && fadeout - i > 0) {
					//LOG_DEBUG(("fadeout %d: %d -> %d", fadeout - i, v, v * (fadeout - i) / fadeout_total));
					v *= (fadeout - i) / fadeout_total;
				}
			}
			src_buf_ptr[i * dst_ch + c] = v;
		}
	}

	if (vol < 0 || (int)floor(MaxMixVolume * vol + 0.5f) <= 0) {
		_update_position((int)(dst_n * pitch));
		return 0;
	}
	
	unsigned used_samples = _hrtf.process(sample->get_spec().sample_rate, dst_buf, dst_ch, src_buf, dst_ch, delta_position, vol);
	_update_position((int)(used_samples * pitch));

	//LOG_DEBUG(("size2: %u, %u, needed: %u", (unsigned)sample3d[0].get_size(), (unsigned)sample3d[1].get_size(), dst_n));
	return vol;
}

void Source::_update_position(const int dp) {
	//LOG_DEBUG(("update_position(%d)", dp));
	position += dp;

	int src_n = (int)sample->get_data().get_size() / sample->get_spec().channels / 2;
	if (loop) {
		position %= src_n;
		//LOG_DEBUG(("position %d", position));
		if (position < 0)
			position += src_n;
	}
	if (fadeout_total > 0) {
		fadeout -= dp;
		if (fadeout <= 0) {
			fadeout = 0;
			loop = false;
		}
	}
}

Source::~Source() {}

void Source::fade_out(const float sec) {
	fadeout = fadeout_total = (int)(sample->get_spec().sample_rate * sec);
}
