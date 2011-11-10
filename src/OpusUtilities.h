/* Copyright (C) 2011, Benjamin Jemlich <pcgod@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef OPUS_UTILITIES_H_
#define OPUS_UTILITIES_H_

#include "PacketDataStream.h"

class OpusUtilities {
  public:
	static int EncodeSize(int size, unsigned char *data) {
		if (size < 252) {
			data[0] = size;
			return 1;
		} else {
			data[0] = 252 + (size & 0x3);
			data[1] = (size - static_cast<int>(data[0])) >> 2;
			return 2;
		}
	}

	static int ParseSize(PacketDataStream *pds) {
		if (pds->left() < 2)
			return -1;
		int tmp = pds->next();
		if (tmp < 252)
			return tmp;
		return 4 * pds->next() + tmp;
	}

	static int ParseToc(PacketDataStream *pds) {
		// The last 2 bits contain the number of frames in the packet as described
		// in the Opus RFC.
		switch (pds->next() & 0x3) {
		case 0:
			return ParseSize(pds);
		case 1:
			return 2 * ParseSize(pds);
		case 2:
			return ParseSize(pds) + ParseSize(pds);
		case 3:
			int tmp = pds->next();
			int count = tmp & 0x3F;
			int length = 0;
			if (tmp & 0x40) {
				int x;
				do {
					if (!pds->left())
						return -1;
					x = pds->next();
					length += x == 255 ? 254 : x;
				} while (x == 255);
			}
			if (tmp & 0x80) {
				length += count * ParseSize(pds);
			} else {
				for (int i = count; i; --i) {
					length += ParseSize(pds);
				}
			}
			return length;
		}
		// Silence the compiler warning...
		return -1;
	}
};

#endif  // OPUS_UTILITIES_H_

