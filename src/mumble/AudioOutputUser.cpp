// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioOutputUser.h"

AudioOutputUser::AudioOutputUser(const QString& name) : qsName(name) {
	iBufferSize = 0;
	pfBuffer = NULL;
	pfVolume = NULL;
	fPos[0]=fPos[1]=fPos[2]=0.0;
}

AudioOutputUser::~AudioOutputUser() {
	delete [] pfBuffer;
	delete [] pfVolume;
}

void AudioOutputUser::resizeBuffer(unsigned int newsize) {
	if (newsize > iBufferSize) {
		float *n = new float[newsize];
		if (pfBuffer) {
			memcpy(n, pfBuffer, sizeof(float) * iBufferSize);
			delete [] pfBuffer;
		}
		pfBuffer = n;
		iBufferSize = newsize;
	}
}
