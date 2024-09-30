// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioOutputBuffer.h"

AudioOutputBuffer::~AudioOutputBuffer() {
	delete[] pfBuffer;
	delete[] pfVolume;
}

void AudioOutputBuffer::resizeBuffer(unsigned int newsize) {
	if (newsize > iBufferSize) {
		float *n = new float[newsize];
		if (pfBuffer) {
			memcpy(n, pfBuffer, sizeof(float) * iBufferSize);
			delete[] pfBuffer;
		}
		pfBuffer    = n;
		iBufferSize = newsize;
	}
}
