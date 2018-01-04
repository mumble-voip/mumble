// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DIRECTSOUND_H_
#define MUMBLE_MUMBLE_DIRECTSOUND_H_

#include "AudioInput.h"
#include "AudioOutput.h"

class DXAudioOutput : public AudioOutput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(DXAudioOutput)
	public:
		DXAudioOutput();
		~DXAudioOutput() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
};

class DXAudioInput : public AudioInput {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(DXAudioInput)
	public:
		DXAudioInput();
		~DXAudioInput() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
};

#endif
