// Copyright 2011-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOOUTPUTUSER_H_
#define MUMBLE_MUMBLE_AUDIOOUTPUTUSER_H_

#include <QtCore/QObject>
#include <memory>

class AudioOutputUser : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(AudioOutputUser)
protected:
	unsigned int iBufferSize;

	/// Used to resize the buffer.
	/// WARNING:
	///          Audio callback is a dedicated place that can be executed
	///          in a special thread or interrupt handler. Allocating
	///          memory will probably crash the program!
	void resizeBuffer(unsigned int newsize);

public:
	AudioOutputUser(const QString &name);
	~AudioOutputUser() Q_DECL_OVERRIDE;
	const QString qsName;
	float *pfBuffer = nullptr;
	float *pfVolume = nullptr;
	std::unique_ptr< unsigned int[] > piOffset;
	float fPos[3] = { 0.0, 0.0, 0.0 };
	bool bStereo;
	virtual bool prepareSampleBuffer(unsigned int snum) = 0;
};

#endif // AUDIOOUTPUTUSER_H_
