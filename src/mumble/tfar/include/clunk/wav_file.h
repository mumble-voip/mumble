#ifndef CLUNK_WAV_FILE_H
#define CLUNK_WAV_FILE_H

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

#include <string>
#include <stdio.h>
#include <clunk/types.h>
#include <clunk/buffer.h>
#include <clunk/audio_spec.h>

namespace clunk {
	class Context;
	class Sample;

	class CLUNKAPI WavFile {
	public:
		FILE *		_f;
		AudioSpec	_spec;
		Buffer		_data;

	public:
		WavFile()
		{
		}
		WavFile(FILE *f);
		~WavFile();

		void read();
		u32 read_32le();
		void read(Buffer &buffer, size_t size);
		void read_format(const Buffer &data);
		bool ok() const
		{ return _spec.channels != 0 && !_data.empty(); }

	public:
		static Sample * load(Context &context, const std::string &fname);
		static void save(Sample &sample, const std::string &fname);
	};
}

#endif
