#ifndef CLUNK_MIXER_H
#define CLUNK_MIXER_H

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


#include <clunk/audio_spec.h>
#include <limits>

namespace clunk {

	static const int MaxMixVolumeShift = 7;
	static const u8 MaxMixVolume = (1 << MaxMixVolumeShift);

	namespace impl {
		template<typename Format> struct Mixer {

			typedef typename Format::Type			Type;
			typedef typename Format::DoubleType		DoubleType;

			static void mix(void *dst_, const void *src_, size_t size, int volume) {
				size /= sizeof(Type);
				Type *dst = static_cast<Type *>(dst_);
				const Type *src = static_cast<const Type *>(src_);
				while(size--)
				{
					DoubleType value = (((DoubleType)*dst << MaxMixVolumeShift) + (volume * (DoubleType)*src++ )) >> MaxMixVolumeShift;
					if (value > std::numeric_limits<Type>::max())
						value = std::numeric_limits<Type>::max();
					if (value < std::numeric_limits<Type>::min())
						value = std::numeric_limits<Type>::min();
					*dst++ = Type(value);
				}
			}
		};
	}
	struct Mixer {
		static void mix(AudioSpec::Format format, void *dst, const void *src, size_t size, int volume)
		{
			switch(format)
			{
				case AudioSpec::S8:		impl::Mixer<AudioFormat<AudioSpec::S8> >::mix(dst, src, size, volume); break;
				case AudioSpec::S16:	impl::Mixer<AudioFormat<AudioSpec::S16> >::mix(dst, src, size, volume); break;
				case AudioSpec::U8:		impl::Mixer<AudioFormat<AudioSpec::U8> >::mix(dst, src, size, volume); break;
				case AudioSpec::U16:	impl::Mixer<AudioFormat<AudioSpec::U16> >::mix(dst, src, size, volume); break;
			}
		}
	};
}

#endif
