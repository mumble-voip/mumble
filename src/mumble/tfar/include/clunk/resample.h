#ifndef CLUNK_RESAMPLE_H
#define CLUNK_RESAMPLE_H

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
#include <clunk/buffer.h>
#include <clunk/logger.h>
#include <stdexcept>

namespace clunk {
	namespace impl {
		template<typename DstType, typename SrcType>
		struct SampleResampler;

		//remove this
		template<> struct SampleResampler<u8, u8>	{ static void Write(u8 &dst, const u8 &src)		{ dst = src; } };
		template<> struct SampleResampler<u8, s8>	{ static void Write(u8 &dst, const s8 &src)		{ dst = src + 128; } };
		template<> struct SampleResampler<u8, u16>	{ static void Write(u8 &dst, const u16 &src)	{ dst = src >> 8; } };
		template<> struct SampleResampler<u8, s16>	{ static void Write(u8 &dst, const s16 &src)	{ dst = (src + (u16)32768) >> 8; } };

		template<> struct SampleResampler<s8, u8>	{ static void Write(s8 &dst, const u8 &src)		{ dst = src - 128; } };
		template<> struct SampleResampler<s8, s8>	{ static void Write(s8 &dst, const s8 &src)		{ dst = src; } };
		template<> struct SampleResampler<s8, u16>	{ static void Write(s8 &dst, const u16 &src)	{ dst = (src - (u16)32768) >> 8; } };
		template<> struct SampleResampler<s8, s16>	{ static void Write(s8 &dst, const s16 &src)	{ dst = src >> 8; } };

		template<> struct SampleResampler<u16, u8>	{ static void Write(u16 &dst, const u8 &src)	{ dst = src << 8; } };
		template<> struct SampleResampler<u16, s8>	{ static void Write(u16 &dst, const s8 &src)	{ dst = (src + (u16)128) << 8; } };
		template<> struct SampleResampler<u16, u16>	{ static void Write(u16 &dst, const u16 &src)	{ dst = src; } };
		template<> struct SampleResampler<u16, s16>	{ static void Write(u16 &dst, const s16 &src)	{ dst = src + (u16)32768; } };

		template<> struct SampleResampler<s16, u8>	{ static void Write(s16 &dst, const u8 &src)	{ dst = ((s16)src - (s16)128) << 8; } };
		template<> struct SampleResampler<s16, s8>	{ static void Write(s16 &dst, const s8 &src)	{ dst = src << 8; } };
		template<> struct SampleResampler<s16, u16>	{ static void Write(s16 &dst, const u16 &src)	{ dst = src - (u16)32768; } };
		template<> struct SampleResampler<s16, s16>	{ static void Write(s16 &dst, const s16 &src)	{ dst = src; } };

		template<int DstChannels, int SrcChannels>
		struct ChannelResampler {
			template<typename DstType, typename SrcType>
			static void resample(DstType * &dst, const SrcType *src);
		};

		template<>
		struct ChannelResampler<1, 1> {
			template<typename DstType, typename SrcType>
			static void resample(DstType * &dst, const SrcType *src) {
				SampleResampler<DstType, SrcType>::Write(*dst++, *src);
			}
		};
		template<>
		struct ChannelResampler<2, 2> {
			template<typename DstType, typename SrcType>
			static void resample(DstType * &dst, const SrcType *src) {
				SampleResampler<DstType, SrcType>::Write(*dst++, *src++);
				SampleResampler<DstType, SrcType>::Write(*dst++, *src++);
			}
		};
		template<>
		struct ChannelResampler<1, 2> {
			template<typename DstType, typename SrcType>
			static void resample(DstType * &dst, const SrcType *src) {
				SrcType v = (*src++ >> 1);
				v += (*src++ >> 1);
				SampleResampler<DstType, SrcType>::Write(*dst++, v);
			}
		};
		template<>
		struct ChannelResampler<2, 1> {
			template<typename DstType, typename SrcType>
			static void resample(DstType * &dst, const SrcType *src) {
				SampleResampler<DstType, SrcType>::Write(*dst++, *src);
				SampleResampler<DstType, SrcType>::Write(*dst++, *src);
			}
		};

		template<typename DstAudioFormat, int DstChannels, typename SrcAudioFormat, int SrcChannels>
		struct Resampler4 {
			typedef impl::ChannelResampler<DstChannels, SrcChannels> ChannelResampler;

			static void resample(AudioSpec dst_format, Buffer &dst_data, AudioSpec src_format, const Buffer &src_data) {
				const typename SrcAudioFormat::Type *src = static_cast<typename SrcAudioFormat::Type *>(src_data.get_ptr());
				size_t src_size = src_data.get_size() / sizeof(typename SrcAudioFormat::Type) / src_format.channels;
				size_t dst_size = (size_t)(src_size * (1.0f * dst_format.sample_rate / src_format.sample_rate));

				dst_data.set_size(dst_size * dst_format.channels * sizeof(typename DstAudioFormat::Type));
				typename DstAudioFormat::Type *dst = static_cast<typename DstAudioFormat::Type *>(dst_data.get_ptr());
				LOG_DEBUG(("converting %d:%u:%u (%u) -> %d:%u:%u (%u)",
					src_format.sample_rate, 8 * (unsigned)sizeof(typename SrcAudioFormat::Type), src_format.channels, (unsigned)src_data.get_size(),
					dst_format.sample_rate, 8 * (unsigned)sizeof(typename DstAudioFormat::Type), dst_format.channels, (unsigned)dst_data.get_size()
				));
				if (src_format.channels == 0)
					throw std::runtime_error("invalid src channel number");
				if (dst_format.channels == 0)
					throw std::runtime_error("invalid dst channel number");

				if (dst_size == 0)
					return;

				size_t error = dst_size / 2;
				for(size_t i = 0; i < dst_size; ++i) {
					ChannelResampler::resample(dst, src);
					error -= src_size;
					if (error < 0) {
						src += src_format.channels;
						error += dst_size;
					}
				}
			}
		};

		template<typename DstAudioFormat, int DstChannels, typename SrcAudioFormat>
		struct Resampler3 {
			static void resample(AudioSpec dst_format, Buffer &dst_data, AudioSpec src_format, const Buffer &src_data) {
				switch(src_format.channels) {
					case 1: Resampler4<DstAudioFormat, DstChannels, SrcAudioFormat, 1>::resample(dst_format, dst_data, src_format, src_data); break;
					case 2: Resampler4<DstAudioFormat, DstChannels, SrcAudioFormat, 2>::resample(dst_format, dst_data, src_format, src_data); break;
					default: throw std::runtime_error("invalid dst channel count");
				}
			}
		};

		template<typename DstAudioFormat, typename SrcAudioFormat>
		struct Resampler2 {
			static void resample(AudioSpec dst_format, Buffer &dst_data, AudioSpec src_format, const Buffer &src_data) {
				switch(dst_format.channels) {
					case 1: Resampler3<DstAudioFormat, 1, SrcAudioFormat>::resample(dst_format, dst_data, src_format, src_data); break;
					case 2: Resampler3<DstAudioFormat, 2, SrcAudioFormat>::resample(dst_format, dst_data, src_format, src_data); break;
					default: throw std::runtime_error("invalid dst channel count");
				}
			}
		};

		template<typename DstAudioFormat>
		struct Resampler1 {
			static void resample(AudioSpec dst_format, Buffer &dst, AudioSpec src_format, const Buffer &src)
			{
				switch(src_format.format) {
					case AudioSpec::S8:		Resampler2<DstAudioFormat, AudioFormat<AudioSpec::S8> >::resample(dst_format, dst, src_format, src); break;
					case AudioSpec::S16:	Resampler2<DstAudioFormat, AudioFormat<AudioSpec::S16> >::resample(dst_format, dst, src_format, src); break;
					case AudioSpec::U8:		Resampler2<DstAudioFormat, AudioFormat<AudioSpec::U8> >::resample(dst_format, dst, src_format, src); break;
					case AudioSpec::U16:	Resampler2<DstAudioFormat, AudioFormat<AudioSpec::U16> >::resample(dst_format, dst, src_format, src); break;
					default: throw std::runtime_error("invalid src format");
				}
			}
		};
	}

	struct Resample {
		static void resample(AudioSpec dst_format, Buffer &dst, AudioSpec src_format, const Buffer &src)
		{
			switch(dst_format.format) {
				case AudioSpec::S8:		impl::Resampler1<AudioFormat<AudioSpec::S8> >::resample(dst_format, dst, src_format, src); break;
				case AudioSpec::S16:	impl::Resampler1<AudioFormat<AudioSpec::S16> >::resample(dst_format, dst, src_format, src); break;
				case AudioSpec::U8:		impl::Resampler1<AudioFormat<AudioSpec::U8> >::resample(dst_format, dst, src_format, src); break;
				case AudioSpec::U16:	impl::Resampler1<AudioFormat<AudioSpec::U16> >::resample(dst_format, dst, src_format, src); break;
				default: throw std::runtime_error("invalid dst format");
			}
		}
	};
}

#endif
