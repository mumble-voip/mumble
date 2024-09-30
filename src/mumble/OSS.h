// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OSS_H_
#define MUMBLE_MUMBLE_OSS_H_

#include "AudioInput.h"
#include "AudioOutput.h"

class OSSInput : public AudioInput {
private:
	Q_OBJECT
	Q_DISABLE_COPY(OSSInput)
protected:
	void release();

public:
	OSSInput();
	~OSSInput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
};

class OSSOutput : public AudioOutput {
	friend class OSSUser;

private:
	Q_OBJECT
	Q_DISABLE_COPY(OSSOutput)
public:
	OSSOutput();
	~OSSOutput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
};

#endif
