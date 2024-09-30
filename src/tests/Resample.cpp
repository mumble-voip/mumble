// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/**
 * Speex resampling test / information.
 */

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#include "Timer.h"

#include <QtCore>

#include <cmath>

#include <speex/speex_resampler.h>

#include <ipp.h>

#define ITER 1000

int main(int argc, char **argv) {
	ippStaticInit();
	ippSetNumThreads(1);

#ifdef Q_OS_WIN
	if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
		qWarning("Application: Failed to set priority!");
#endif

	QCoreApplication a(argc, argv);

	SpeexResamplerState *srs;

	int iMicFreq    = 44100;
	int iSampleRate = 48000;
	int iFrameSize  = iSampleRate / 100;
	int err         = 0;

	srs = speex_resampler_init(1, iMicFreq, iSampleRate, 3, &err);

	int iMicLength = (iFrameSize * iMicFreq) / iSampleRate;

	qWarning() << iMicFreq << iSampleRate << iFrameSize << iMicLength;

	float *pfInput  = new float[iMicLength];
	float *pfOutput = new float[iFrameSize];

	for (int i = 0; i < iMicLength; ++i) {
		pfInput[i] = sinf((M_PI * i * 20) / iMicLength);
	}

	for (int i = 0; i < iFrameSize; ++i)
		pfOutput[i] = 0;

	qWarning() << "speex resampler latency: " << speex_resampler_get_input_latency(srs);


	Timer t;

	for (int i = 0; i < ITER; ++i) {
		spx_uint32_t inlen  = iMicLength;
		spx_uint32_t outlen = iFrameSize;
		speex_resampler_process_float(srs, 0, pfInput, &inlen, pfOutput, &outlen);
	}

	quint64 e = t.elapsed();

	qWarning() << "speex us per iteration: " << (e / ITER);

	float min = 0.0;
	float max = 0.0;

	for (int i = 0; i < iFrameSize; ++i) {
		min = qMin(min, pfOutput[i]);
		max = qMax(max, pfOutput[i]);
	}

	qWarning() << "Speex bounds" << min << max;

	delete[] pfOutput;

	int history  = 24;
	int lastread = history;
	double time  = history;
	int olen     = 0;

	float *inBuf = ippsMalloc_32f(iMicLength + history * 2 + 2);
	ippsZero_32f(inBuf, iMicLength + history * 2 + 2);

	pfOutput = ippsMalloc_32f(iFrameSize + 2);

	for (int i = 0; i < iFrameSize; ++i)
		pfOutput[i] = 0;


	IppsResamplingPolyphaseFixed_32f *pSpec = nullptr;
	ippsResamplePolyphaseFixedInitAlloc_32f(&pSpec, iMicFreq, iSampleRate, 2 * history, 0.90f, 8.0f, ippAlgHintFast);

	t.restart();

	for (int i = 0; i < ITER; ++i) {
		ippsCopy_32f(pfInput, inBuf + history, iMicLength);

		ippsResamplePolyphaseFixed_32f(pSpec, inBuf, iMicLength, pfOutput, .99f, &time, &olen);

		time -= iMicLength;

		ippsMove_32f(inBuf + history, inBuf, history);
	}

	e = t.elapsed();
	qWarning() << "ipp us per iteration: " << (e / ITER);

	min = 0.0;
	max = 0.0;

	for (int i = 0; i < iFrameSize; ++i) {
		min = qMin(min, pfOutput[i]);
		max = qMax(max, pfOutput[i]);
	}

	qWarning() << "IPP bounds" << min << max;

	delete[] pfInput;

	return 0;
}

#undef ITER
