// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOOUTPUTUSER_H_
#define MUMBLE_MUMBLE_AUDIOOUTPUTUSER_H_

#include <QtCore/QObject>

class AudioOutputUser : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AudioOutputUser)
	protected:
		unsigned int iBufferSize;
		void resizeBuffer(unsigned int newsize);
	public:
		AudioOutputUser(const QString& name);
		~AudioOutputUser() Q_DECL_OVERRIDE;
		const QString qsName;
		float *pfBuffer;
		float *pfVolume;
		float fPos[3];
		virtual bool prepareSampleBuffer(unsigned int snum) = 0;
};

#endif  // AUDIOOUTPUTUSER_H_
