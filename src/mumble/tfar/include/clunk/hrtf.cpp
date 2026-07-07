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

#include <clunk/hrtf.h>
#include <clunk/buffer.h>
#include <clunk/clunk_ex.h>
#include <stddef.h>
#include <algorithm>

#include "kemar.h"

#if defined _MSC_VER || __APPLE__ || __FreeBSD__
#	define pow10f(x) powf(10.0f, (x))
#	define log2f(x) (logf(x) / M_LN2)
#endif

namespace clunk
{

clunk_static_assert(Hrtf::WINDOW_BITS > 2);

Hrtf::Hrtf(): overlap_data()
{ }

void Hrtf::idt_iit(const v3f &position, float &idt_offset, float &angle_gr, float &left_to_right_amp) {
	float head_r = 0.093f;

	float angle = (float)(M_PI_2 - atan2f(position.y, position.x));

	angle_gr = angle * 180 / float(M_PI);
	while (angle_gr < 0)
		angle_gr += 360;

	//LOG_DEBUG(("relative position = (%g,%g,%g), angle = %g (%g)", position.x, position.y, position.z, angle, angle_gr));

	float idt_angle = fmodf(angle, 2 * (float)M_PI);

	if (idt_angle < 0)
		idt_angle += 2 * (float)M_PI;
	if (idt_angle >= float(M_PI_2) && idt_angle < (float)M_PI) {
		idt_angle = float(M_PI) - idt_angle;
	} else if (idt_angle >= float(M_PI) && idt_angle < 3 * float(M_PI_2)) {
		idt_angle = (float)M_PI - idt_angle;
	} else if (idt_angle >= 3 * (float)M_PI_2) {
		idt_angle -= (float)M_PI * 2;
	}

	//LOG_DEBUG(("idt_angle = %g (%d)", idt_angle, (int)(idt_angle * 180 / M_PI)));
	idt_offset = - head_r * (idt_angle + sin(idt_angle)) / 344;
	left_to_right_amp = pow10f(-sin(idt_angle));
	//LOG_DEBUG(("idt_offset %g, left_to_right_amp: %g", idt_offset, left_to_right_amp));
}

void Hrtf::get_kemar_data(kemar_ptr & kemar_data, int & elev_n, const v3f &pos) {
	
	kemar_data = 0;
	elev_n = 0;
	if (pos.is0())
		return;

#ifdef _WINDOWS
       float len = (float)_hypot(pos.x, pos.y);
#else
       float len = (float)hypot(pos.x, pos.y);
#endif

	int elev_gr = (int)(180 * atan2f(pos.z, len) / (float)M_PI);
	//LOG_DEBUG(("elev = %d (%g %g %g)", elev_gr, pos.x, pos.y, pos.z));

	for(size_t i = 0; i < KemarElevationCount; ++i)
	{
		const kemar_elevation_data &elev = ::kemar_data[i];
		if (elev_gr < elev.elevation + KemarElevationStep / 2)
		{
			//LOG_DEBUG(("used elevation %d", elev.elevation)); 
			kemar_data = elev.data;
			elev_n = elev.samples;
			break;
		}
	}
}

unsigned Hrtf::process(
	unsigned sample_rate, clunk::Buffer &dst_buf, unsigned dst_ch,
	const clunk::Buffer &src_buf, unsigned src_ch,
	const v3f &delta_position, float fx_volume)
{
	s16 * const dst = static_cast<s16*>(dst_buf.get_ptr());
	const unsigned dst_n = (unsigned)dst_buf.get_size() / dst_ch / 2;

	const s16 * const src = static_cast<const s16 *>(src_buf.get_ptr());
	const unsigned src_n = (unsigned)src_buf.get_size() / src_ch / 2;
	assert(dst_n <= src_n);

	kemar_ptr kemar_data;
	int angles;
	get_kemar_data(kemar_data, angles, delta_position);

	if (delta_position.is0() || kemar_data == NULL) {
		//2d stereo sound!
		if (src_ch == dst_ch) {
			memcpy(dst_buf.get_ptr(), src_buf.get_ptr(), dst_buf.get_size());
			return dst_n;
		}
		else
			throw_ex(("unsupported sample conversion"));
	}
	assert(dst_ch == 2);
	
	//LOG_DEBUG(("data: %p, angles: %d", (void *) kemar_data, angles));

	float t_idt, angle_gr, left_to_right_amp;
	idt_iit(delta_position, t_idt, angle_gr, left_to_right_amp);

	const int kemar_sector_size = 360 / angles;
	const int kemar_idx[2] = {
		((360 - (int)angle_gr + kemar_sector_size / 2) / kemar_sector_size) % angles,
		((int)angle_gr + kemar_sector_size / 2) / kemar_sector_size
	};

	float amp[2] = {
		left_to_right_amp > 1? 1: 1 / left_to_right_amp,
		left_to_right_amp > 1? left_to_right_amp: 1
	};
	//LOG_DEBUG(("%g (of %d)-> left: %d, right: %d", angle_gr, angles, kemar_idx_left, kemar_idx_right));
	
	int idt_offset = (int)(t_idt * sample_rate);

	int window = 0;
	while(sample3d[0].get_size() < dst_n * 2 || sample3d[1].get_size() < dst_n * 2) {
		int offset = window * WINDOW_SIZE / 2;
		assert(offset + WINDOW_SIZE / 2 <= src_n);
		for(unsigned c = 0; c < dst_ch; ++c) {
			sample3d[c].reserve(WINDOW_SIZE);
			s16 *dst = static_cast<s16 *>(static_cast<void *>((static_cast<u8 *>(sample3d[c].get_ptr()) + sample3d[c].get_size() - WINDOW_SIZE)));
			hrtf(c, dst, src + offset * src_ch, src_ch, src_n - offset, idt_offset, kemar_data, kemar_idx[c], amp[c]);
		}
		++window;
	}
	assert(sample3d[0].get_size() >= dst_n * 2 && sample3d[1].get_size() >= dst_n * 2);
	
	//LOG_DEBUG(("angle: %g", angle_gr));
	//LOG_DEBUG(("idt offset %d samples", idt_offset));
	s16 * src_3d[2] = { static_cast<s16 *>(sample3d[0].get_ptr()), static_cast<s16 *>(sample3d[1].get_ptr()) };
	
	//LOG_DEBUG(("size1: %u, %u, needed: %u\n%s", (unsigned)sample3d[0].get_size(), (unsigned)sample3d[1].get_size(), dst_n, sample3d[0].dump().c_str()));
	
	for(unsigned i = 0; i < dst_n; ++i) {
		for(unsigned c = 0; c < dst_ch; ++c) {
			dst[i * dst_ch + c] = src_3d[c][i];
		}
	}
	skip(dst_n);
	return window * WINDOW_SIZE / 2;
}

void Hrtf::skip(unsigned samples) {
	for(int i = 0; i < 2; ++i) {
		Buffer & buf = sample3d[i];
		buf.pop(samples * 2);
	}
}

void Hrtf::hrtf(const unsigned channel_idx, s16 *dst, const s16 *src, int src_ch, int src_n, int idt_offset, const kemar_ptr& kemar_data, int kemar_idx, float freq_decay) {
	assert(channel_idx < 2);

	//LOG_DEBUG(("channel %d: window %d: adding %d, buffer size: %u, decay: %g", channel_idx, window, WINDOW_SIZE, (unsigned)result.get_size(), freq_decay));

	if (channel_idx <= 1) {
		bool left = channel_idx == 0;
		if (!left && idt_offset > 0) {
			idt_offset = 0;
		} else if (left && idt_offset < 0) {
			idt_offset = 0;
		}
		if (idt_offset < 0)
			idt_offset = - idt_offset;
	} else 
		idt_offset = 0;

	assert(std::min(0, idt_offset) >= 0);
	assert(std::max(0, idt_offset) + WINDOW_SIZE <= src_n);

	for(int i = 0; i < WINDOW_SIZE; ++i) {
		//-1 0 1 2 3
		int p = idt_offset + i;
		assert(p >= 0 && p < src_n);
		//printf("%d of %d, ", p, src_n);
		int v = src[p * src_ch];
		_mdct.data[i] = v / 32768.0f;
		//fprintf(stderr, "%g ", _mdct.data[i]);
	}
	
	_mdct.apply_window();
	_mdct.mdct();
	{
		for(size_t i = 0; i < mdct_type::M; ++i)
		{
			const size_t kemar_sample = i * 257 / mdct_type::M;
			std::complex<float> fir(kemar_data[kemar_idx][0][kemar_sample][0], kemar_data[kemar_idx][0][kemar_sample][1]);
			//_mdct.data[i] *= std::abs(fir); TODO: fix HRTF
		}
	}

	//LOG_DEBUG(("kemar angle index: %d\n", kemar_idx));
	assert(freq_decay >= 1);
	
	_mdct.imdct();
	_mdct.apply_window();

	{
		//stupid msvc
		int i;
		for(i = 0; i < WINDOW_SIZE / 2; ++i) {
			float v = _mdct.data[i] + overlap_data[channel_idx][i];
			
			if (v < -1) {
				LOG_DEBUG(("clipping %f", v));
				v = -1;
			} else if (v > 1) {
				LOG_DEBUG(("clipping %f", v));
				v = 1;
			}
			*dst++ = (int)(v * 32767);
		}
		for(; i < WINDOW_SIZE; ++i) {
			overlap_data[channel_idx][i - WINDOW_SIZE / 2] = _mdct.data[i];
		}
	}
}

	
}
