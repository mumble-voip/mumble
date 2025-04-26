// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOOUTPUTBUFFER_H_
#define MUMBLE_MUMBLE_AUDIOOUTPUTBUFFER_H_

#include <QtCore/QObject>

#include <array>
#include <memory>

class AudioOutputBuffer : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(AudioOutputBuffer)
protected:
	unsigned int iBufferSize;

	/// Used to resize the buffer.
	/// WARNING:
	///          Audio callback is a dedicated place that can be executed
	///          in a special thread or interrupt handler. Allocating
	///          memory will probably crash the program!
	void resizeBuffer(unsigned int newsize);

public:
	AudioOutputBuffer(){};
	~AudioOutputBuffer() Q_DECL_OVERRIDE;
	float *pfBuffer                   = nullptr;
	float *pfVolume                   = nullptr;
	float m_suggestedVolumeAdjustment = 1.0f;
	std::unique_ptr< unsigned int[] > piOffset;
	std::array< float, 3 > fPos = { 0.0, 0.0, 0.0 };
	bool bStereo;
	virtual bool prepareSampleBuffer(unsigned int snum) = 0;
};

#endif // AUDIOOUTPUTBUFFER_H_
