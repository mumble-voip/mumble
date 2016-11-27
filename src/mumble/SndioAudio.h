// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SNDIOAUDIO_H_
#define MUMBLE_MUMBLE_SNDIOAUDIO_H_

#include "AudioInput.h"
#include "AudioOutput.h"

class SndioInput : public AudioInput {
		friend class SndioOutput;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(SndioInput)
	protected:
		void *udata;
	public:
		SndioInput();
		~SndioInput() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
};

class SndioOutput : public AudioOutput {
		friend class SndioInput;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(SndioOutput)
	public:
		SndioOutput();
		~SndioOutput() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
};

#endif
