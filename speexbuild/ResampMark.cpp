#include <QtCore>

static const float tfreq1 = 48000.f;
static const float tfreq2 = 44100.f;
#define SM_VERIFY
// #define EXACT

#ifdef Q_OS_WIN
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#define CALLGRIND_START_INSTRUMENTATION
#define CALLGRIND_STOP_INSTRUMENTATION
#define CALLGRIND_ZERO_STATS
#else
#include <valgrind/callgrind.h>
#endif

#include <math.h>
#include <speex/speex_resampler.h>

#include "Timer.h"

int main(int argc, char **argv) {

	CALLGRIND_STOP_INSTRUMENTATION;
	CALLGRIND_ZERO_STATS;

	QCoreApplication a(argc, argv);

	QFile f((argc >= 2) ? argv[1] : "wb_male.wav");
	if (! f.open(QIODevice::ReadOnly)) {
		qFatal("Failed to open file!");
	}
	f.seek(36 + 8);

#ifdef SM_OUTPUT
	QFile o("output.raw");
	if (! o.open(QIODevice::WriteOnly))
		qFatal("Failed to open output!");
#endif
#ifdef SM_VERIFY
	QFile vf((argc >= 3) ? argv[2] : "verify.raw");
	if (! vf.open(QIODevice::ReadOnly)) {
		qWarning("Failed to open validate file!");
	}
#endif

	const int iFrameSize = 320;

	QVector<QByteArray> v;
	while(1) {
		QByteArray qba = f.read(iFrameSize * 2);
		if (qba.size() != iFrameSize * 2)
			break;
		v.append(qba);
	}

	int nframes = v.size();
	

	qWarning("Ready to process %d frames of %d samples", nframes, iFrameSize);

	QVector<float *> sv;



	short tframe[2048];
	for(int i=0;i<iFrameSize;i++)
		tframe[i] = 0;

	for(int i=0;i<nframes;i++) {
		float *f = new float[iFrameSize];
		short *s = reinterpret_cast<short *>(v[i].data());
		for(int j=0;j<iFrameSize;j++)
			f[j]=s[j];
		sv.append(f);
	}
	
	float resampframe[32768];
	float verifyframe[32768];
	
	const float sfraq1 = tfreq1 / 16000.0f;
	float fOutSize1 = iFrameSize * sfraq1;
	int iOutSize1 = lroundf(fOutSize1);
		
	const float sfraq2 = tfreq2 / 16000.0f;
	float fOutSize2 = iFrameSize * sfraq2;
	int iOutSize2 = lroundf(fOutSize2);
		
	int err;
	SpeexResamplerState *srs1 = speex_resampler_init(1, 16000, lroundf(tfreq1), 3, &err);
	SpeexResamplerState *srs2 = speex_resampler_init(1, 16000, lroundf(tfreq2), 3, &err);
	
#ifdef Q_OS_WIN
    if (!SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS))
             qWarning("Application: Failed to set priority!");
#endif

	int len;
	spx_uint32_t inlen;
	spx_uint32_t outlen;

	Timer t;
	quint64 e;

	if (RUNNING_ON_VALGRIND) {
		nframes = qMin(nframes, 10);
	} else {
		t.restart();
		for(int j=0;j<100;j++) {
			for(int i=0;i<100;i++) {
				float *in = sv[i];
				inlen = iFrameSize;
				outlen = iOutSize1;
				speex_resampler_process_float(srs1, 0, in, &inlen, resampframe, &outlen);
			}
		}
		e = t.elapsed();
		qWarning("Direct:      %10llu usec", e);

		t.restart();
		for(int j=0;j<100;j++) {
			for(int i=0;i<100;i++) {
				float *in = sv[i];
				inlen = iFrameSize;
				outlen = iOutSize2;
				speex_resampler_process_float(srs2, 0, in, &inlen, resampframe, &outlen);
			}
		}
		e = t.elapsed();
		qWarning("Interpolate: %10llu usec", e);
		speex_resampler_reset_mem(srs1);
		speex_resampler_reset_mem(srs2);
	}
	
	t.restart();
	CALLGRIND_START_INSTRUMENTATION;

	for(int i=0;i<nframes;i++) {
		float *in = sv[i];
		
		inlen = iFrameSize;
		outlen = iOutSize1;
		speex_resampler_process_float(srs1, 0, in, &inlen, resampframe, &outlen);

		if (! RUNNING_ON_VALGRIND) {
#ifdef SM_OUTPUT
			o.write(reinterpret_cast<const char *>(resampframe), outlen * sizeof(float));
#endif
#ifdef SM_VERIFY
			if (vf.read(reinterpret_cast<char *>(verifyframe), outlen * sizeof(float)) == outlen * sizeof(float)) {
				for(int j=0;j<outlen;j++)
#ifdef EXACT
					if (verifyframe[j] != resampframe[j])
#else
					if (fabs(verifyframe[j]-resampframe[j]) > fabs(verifyframe[j])*0.2f)
#endif
						qFatal("Frame %d, Pos %d: SRS1 %.10g <=> %.10g", i, j, verifyframe[j], resampframe[j]);
			}
#endif
		}
		
		inlen = iFrameSize;
		outlen = iOutSize2;
		speex_resampler_process_float(srs2, 0, in, &inlen, resampframe, &outlen);

		if (! RUNNING_ON_VALGRIND) {
#ifdef SM_OUTPUT
		o.write(reinterpret_cast<const char *>(resampframe), outlen * sizeof(float));
#endif

#ifdef SM_VERIFY
		if (vf.read(reinterpret_cast<char *>(verifyframe), outlen * sizeof(float)) == outlen * sizeof(float))
			for(int j=0;j<outlen;j++)
#ifdef EXACT
				if (verifyframe[j] != resampframe[j])
#else
				if (fabs(verifyframe[j]-resampframe[j]) > fabs(verifyframe[j])*0.2f)
#endif
					qFatal("Frame %d, Pos %d: SRS2 %.10g <=> %.10g", i, j, verifyframe[j], resampframe[j]);
#endif

#ifdef SM_VERIFY
		if ((inlen != iFrameSize) || (outlen != iOutSize2)) 
			qWarning("%d!=%d %d!=%d (%f)", inlen, iFrameSize, outlen, iOutSize2, fOutSize2);
		} else {
    			qWarning("%d / %d", i, nframes);
		}
#endif
	}
	CALLGRIND_STOP_INSTRUMENTATION;

	e = t.elapsed();

#ifdef Q_OS_WIN
    if (!SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS))
             qWarning("Application: Failed to reset priority!");
#endif

	qWarning("Used %llu usec", e);
	qWarning("%.2f times realtime", (20000ULL * nframes) / (e * 1.0));
	return 0;
}

#include "Timer.cpp"
