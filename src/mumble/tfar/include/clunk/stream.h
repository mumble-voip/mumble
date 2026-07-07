#ifndef CLUNK_STREAM_H__
#define CLUNK_STREAM_H__

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
#include <clunk/types.h>
#include <clunk/audio_spec.h>

namespace clunk {
class Context;
class Buffer;

/*! 
	\brief Music/Ambient stream.
	simple abstract class allowing you to play audio streams. Note that stream's methods will be called from the audio callback. 
	Usually it's the different thread context and if you're using any global variables from the stream code, you need to protect it with mutex
	or clunk::AudioLocker. 
*/

class CLUNKAPI Stream {
public: 
	Stream();
	///rewinds stream
	/*!
		Rewind your stream in your function
	*/
	virtual void rewind() = 0;
	
	/*! 
		\brief reads some data from the stream. 
		\param[out] data destination buffer
		\param[in] hint points out for the recommented data size. You could read less or more hint. 
	*/
	virtual bool read(clunk::Buffer &data, unsigned hint) = 0;
	virtual ~Stream();

protected: 
	///stream sample rate
	/*! initialize it from your open() method */
	AudioSpec _spec;

	friend class Context;
};
}

#endif

