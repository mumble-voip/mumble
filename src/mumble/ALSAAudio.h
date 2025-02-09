// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ALSAAUDIO_H_
#	define MUMBLE_MUMBLE_ALSAAUDIO_H_

#	include "AudioInput.h"
#	include "AudioOutput.h"

class ALSAAudioOutput;
class ALSAAudioInput;

class ALSAAudioInput : public AudioInput {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ALSAAudioInput)
public:
	ALSAAudioInput();
	~ALSAAudioInput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
};

class ALSAAudioOutput : public AudioOutput {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ALSAAudioOutput)
protected:
public:
	ALSAAudioOutput();
	~ALSAAudioOutput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
};

#else
class ALSAAudioInput;
class ALSAAudioOutput;
#endif
