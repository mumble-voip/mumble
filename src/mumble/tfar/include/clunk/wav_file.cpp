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

#include <clunk/wav_file.h>
#include <clunk/sample.h>
#include <clunk/context.h>
#include <clunk/logger.h>
#include <stdio.h>
#include <stdexcept>
#include <memory>

namespace clunk {
	WavFile::WavFile(FILE *f) : _f(f) {}
	WavFile::~WavFile() { fclose(_f); }

	u32 WavFile::read_32le()
	{
		u8 data[4];
		if (fread(data, 1, 4, _f) != 4)
			throw std::runtime_error("fread failed");
		return (data[0]) | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
	}

	void WavFile::read_format(const Buffer &data) {
		const u8 *src = static_cast<u8 *>(data.get_ptr());
		if (data.get_size() < 16)
			throw std::runtime_error("invalid header size");

		u16 format = src[0] | (src[1] << 8);
		if (format != 1)
			throw std::runtime_error("only PCM format supported");
		_spec.channels		= src[2] | (src[3] << 8);
		_spec.sample_rate	= src[4] | (src[5] << 8) | (src[6] << 16) | (src[7] << 24);
		format				= src[14] | (src[15] << 8);
		if (format != 8 && format != 16)
			throw std::runtime_error("invalid bits per sample size");
		LOG_DEBUG(("channels: %u, sample rate: %u, format: %u", _spec.channels, _spec.sample_rate, _spec.format));
	}

	void WavFile::read(Buffer &buffer, size_t size)
	{
		buffer.set_size(size);
		if (fread(buffer.get_ptr(), 1, size, _f) != size)
			throw std::runtime_error("read failed");
	}

	void WavFile::read() {
		fseek(_f, 0, SEEK_SET);
		u32 riff = read_32le();
		read_32le();
		if (riff != 0x46464952)
			throw std::runtime_error("invalid riff file signature");
		u32 format = read_32le();
		if (format != 0x45564157)
			throw std::runtime_error("only wave format supported");

		while(!ok())
		{
			u32 id = read_32le();
			u32 size = read_32le();
			LOG_DEBUG(("id: 0x%08x, size: %u", id, size));
			if (id == 0x20746d66)
			{
				Buffer fmt;
				read(fmt, size);
				read_format(fmt);
			} else if (id == 0x61746164) {
				read(_data, size);

#if !defined(_WINDOWS) && __BYTE_ORDER == __BIG_ENDIAN
				if (_spec.bytes_per_sample() == 2)
				{
					u8 *ptr = static_cast<u8 *>(_data.get_ptr());
					for(size_t i = 0; i + 1 < _data.get_size(); i += 2, ptr += 2)
						std::swap(ptr[0], ptr[1]);
				}
#endif
			} else
				fseek(_f, size, SEEK_CUR);
		}
	}

	Sample * WavFile::load(Context &context, const std::string &fname) {
		FILE *f = fopen(fname.c_str(), "rb");
		if (!f)
			throw std::runtime_error("cannot open file: " + fname);
		WavFile wav(f);
		wav.read();
		std::unique_ptr<Sample> sample(context.create_sample());
		sample->init(wav._data, wav._spec);
		sample->name = fname;
		return sample.release();
	}
}
