#define _USE_MATH_DEFINES
#include <cmath>
#include <QtCore>

#include <speex/speex_resampler.h>

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);
	
	SpeexResamplerState *srs;
	
	int iMicFreq = 44100;
	int iSampleRate = 48000;
	int iFrameSize = iSampleRate / 100;
	int err = 0;
	
	srs = speex_resampler_init(1, iMicFreq, iSampleRate, 3, &err);

	int iMicLength = (iFrameSize * iMicFreq) / iSampleRate;
	
	float *pfInput = new float[iMicLength];
	float *pfOutput = new float[iFrameSize];
	
	
	for(int i=0;i<iMicLength;++i) {
		pfInput[i] = sinf((M_PI * i * 20) / iMicLength);
		pfOutput[i] = 0;
	}
	
	spx_uint32_t inlen = iMicLength;
	spx_uint32_t outlen = iFrameSize;
	speex_resampler_process_float(srs, 0, pfInput, &inlen, pfOutput, &outlen);
	
	float min = 0.0;
	float max = 0.0;
	
	for(int i=0;i<iFrameSize;++i) {
		min = qMin(min, pfOutput[i]);
		max = qMax(max, pfOutput[i]);
	}
	
	qWarning() << min << max;
	
	return 0;
}
