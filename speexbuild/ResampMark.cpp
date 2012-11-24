#include <QtCore>

static const float tfreq1 = 48000.f;
static const float tfreq2 = 44100.f;
static const int qual = 3;
static const int loops = 2000 / qual;
// static const int loops = 1;
#define SM_VERIFY
// #define EXACT

#ifdef Q_OS_WIN
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#include <mathimf.h>
#define CALLGRIND_START_INSTRUMENTATION
#define CALLGRIND_STOP_INSTRUMENTATION
#define CALLGRIND_ZERO_STATS
#define RUNNING_ON_VALGRIND 0
#else
#include <sched.h>
#include <sys/mman.h>
#include <valgrind/callgrind.h>
#endif

#include <math.h>
#include <speex/speex_resampler.h>

#include <emmintrin.h>


#include "Timer.h"

template<class T>
static inline double veccomp(const QVector<T> &a, const QVector<T> &b, const char *n) {
	long double gdiff = 0.0;
	if (a.size() != b.size()) {
		qFatal("%s: %d <=> %d", n, a.size(), b.size());
	}
	for (int i=0;i<a.size();++i) {
		double diff = fabs(a[i] - b[i]);
		if (diff > gdiff)
			gdiff = diff;
#ifdef EXACT

		if (a[i] != b[i]) {
#else
		union { T tv;
			quint32 uv;
		} v1, v2;
		v1.uv = v2.uv = 0;
		v1.tv = a[i];
		v2.tv = b[i];
		if (fabsf(a[i] - b[i]) > 1) {
			qWarning("%08x %08x %08x", v1.uv, v2.uv, v1.uv ^ v2.uv);
#endif
			qFatal("%s: Offset %d: %.10g <=> %.10g", n, i, static_cast<double>(a[i]), static_cast<double>(b[i]));
		}
	}
	return gdiff;
}

template<class T>
QPair<T, T> confint(const QVector<T> &vecin) {
	long double avg = 0.0;
	long double stddev = 0.0;
	QVector<T> vec = vecin;
	qSort(vec.begin(), vec.end());
	for (int i=0;i<vec.count() / 20;++i) {
		vec.pop_front();
		vec.pop_back();
	}

	foreach(T v, vec)
		avg += v;
	avg /= vec.count();

	foreach(T v, vec)
		stddev += (v-avg)*(v-avg);
	stddev = sqrtl(stddev / vec.count());
	return QPair<T,T>(static_cast<T>(avg), static_cast<T>(1.96 * stddev));
}

int main(int argc, char **argv) {

	CALLGRIND_STOP_INSTRUMENTATION;
	CALLGRIND_ZERO_STATS;

	QCoreApplication a(argc, argv);

	QFile f((argc >= 2) ? argv[1] : "wb_male.wav");
	if (! f.open(QIODevice::ReadOnly)) {
		qFatal("Failed to open file!");
	}
	f.seek(36 + 8);

	QFile o("output.raw");
	if (!(RUNNING_ON_VALGRIND))
		if (! o.open(QIODevice::WriteOnly))
			qFatal("Failed to open output!");

	QFile vf((argc >= 3) ? argv[2] : "verify.raw");
	if (! vf.open(QIODevice::ReadOnly)) {
		qWarning("Failed to open validate file!");
	}

	QDataStream out(&o);
	QDataStream verify(&vf);

	const int iFrameSize = 320;

	QVector<QByteArray> v;
	while (1) {
		QByteArray qba = f.read(iFrameSize * 2);
		if (qba.size() != iFrameSize * 2)
			break;
		v.append(qba);
	}

	int nframes = v.size();


	qWarning("Ready to process %d frames of %d samples", nframes, iFrameSize);

	QVector<short *> qvInShort;
	QVector<float *> qvIn;
	QVector<float *> qvDirect;
	QVector<float *> qvInterpolate;
	QVector<float *> qvInterpolateMC;
	QVector<short *> qvInterpolateShort;
	QVector<float *> qv8;
	QVector<float *> qv96;

	const float sfraq1 = tfreq1 / 16000.0f;
	float fOutSize1 = iFrameSize * sfraq1;
	int iOutSize1 = lroundf(fOutSize1);

	const float sfraq2 = tfreq2 / 16000.0f;
	float fOutSize2 = iFrameSize * sfraq2;
	int iOutSize2 = lroundf(fOutSize2);

	int iOutSize8 = iFrameSize / 2;
	int iOutSize96 = iFrameSize * 6;

	if (RUNNING_ON_VALGRIND)
		nframes = qMin(nframes, 10);

	QVector<float> fInput(nframes * iFrameSize);
	QVector<float> fDirect(nframes * iOutSize1);
	QVector<float> fInterpolate(nframes * iOutSize2);
	QVector<float> fInterpolateMC(nframes * iOutSize2);
	QVector<short> sInterpolate(nframes * iOutSize2);
	QVector<float> f96(nframes * iOutSize96);
	QVector<float> f8(nframes *iOutSize8);

	for (int i=0;i<nframes;i++) {
		short *s = reinterpret_cast<short *>(v[i].data());
		float *f = fInput.data() + i * iFrameSize;

		for (int j=0;j<iFrameSize;j++)
			f[j]=s[j]+20;

		qvInShort.append(s);
		qvIn.append(f);
		qvDirect.append(fDirect.data() + i * iOutSize1);
		qvInterpolate.append(fInterpolate.data() + i * iOutSize2);
		qvInterpolateMC.append(fInterpolateMC.data() + i * iOutSize2);
		qvInterpolateShort.append(sInterpolate.data() + i * iOutSize2);
		qv8.append(f8.data() + i * iOutSize8);
		qv96.append(f96.data() + i * iOutSize96);
	}

	int err;
	SpeexResamplerState *srs1 = speex_resampler_init(1, 16000, lroundf(tfreq1), qual, &err);
	SpeexResamplerState *srs2 = speex_resampler_init(1, 16000, lroundf(tfreq2), qual, &err);
	SpeexResamplerState *srs2i = speex_resampler_init(1, 16000, lroundf(tfreq2), qual, &err);
	SpeexResamplerState *srss = speex_resampler_init(3, 16000, lroundf(tfreq2), qual, &err);

	SpeexResamplerState *srsto96 = speex_resampler_init(1, 16000, 96000, 5, &err);
	SpeexResamplerState *srs8to96 = speex_resampler_init(1, 8000, 96000, qual, &err);
	SpeexResamplerState *srs96to8 = speex_resampler_init(1, 96000, 8000, qual, &err);


#ifdef Q_OS_WIN
	if (!SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS))
		qWarning("Application: Failed to set priority!");
#endif

	spx_uint32_t inlen;
	spx_uint32_t outlen;

	Timer t;
	quint64 e;

	if (! RUNNING_ON_VALGRIND) {
#ifndef Q_OS_WIN
		struct sched_param sp;
		sp.sched_priority = sched_get_priority_max(SCHED_FIFO);

		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(1, &cpuset);

		if (sched_setscheduler(getpid(), SCHED_FIFO, &sp) != 0)
			qWarning("No realtime.");
		if (mlockall(MCL_CURRENT | MCL_FUTURE) != 0)
			qWarning("No mlock.");
		if (sched_setaffinity(0, sizeof(cpuset), &cpuset) != 0)
			qWarning("No affinity");

		sched_yield();
#endif

		for (int i=0;i<nframes;++i) {
			inlen = iFrameSize;
			outlen = iOutSize96;
			speex_resampler_process_float(srsto96, 0, qvIn[i], &inlen, qv96[i], &outlen);
		}

		QVector<unsigned long long> qvTimes;
		QPair<unsigned long long, unsigned long long> ci;

		for (int j=0;j<loops;j++) {
			t.restart();
			for (int i=0;i<nframes;i++) {
				inlen = iFrameSize;
				outlen = iOutSize1;
				speex_resampler_process_float(srs1, 0, qvIn[i], &inlen, qvDirect[i], &outlen);
			}
			e = t.elapsed();
			qvTimes.append(e);
		}
		ci = confint(qvTimes);
		qWarning("Direct:      %8llu +/- %3llu usec (%d)", ci.first, ci.second, qvTimes.count(), qvTimes.count());

		qvTimes.clear();

		for (int j=0;j<loops;j++) {
			t.restart();
			for (int i=0;i<nframes;i++) {
				inlen = iFrameSize;
				outlen = iOutSize2;
				speex_resampler_process_float(srs2, 0, qvIn[i], &inlen, qvInterpolate[i], &outlen);
			}
			e = t.elapsed();
			qvTimes.append(e);
		}
		ci = confint(qvTimes);
		qWarning("Interpolate: %8llu +/- %3llu usec (%d)", ci.first, ci.second, qvTimes.count());

		qvTimes.clear();
		for (int j=0;j<loops;j++) {
			t.restart();
			for (int i=0;i<nframes;i++) {
				inlen = iOutSize96;
				outlen = iOutSize8;
				speex_resampler_process_float(srs96to8, 0, qv96[i], &inlen, qv8[i], &outlen);
			}
			e = t.elapsed();
			qvTimes.append(e);
		}
		ci = confint(qvTimes);
		qWarning("96 => 8:     %8llu +/- %3llu usec (%d)", ci.first, ci.second, qvTimes.count());

		qvTimes.clear();
		t.restart();
		for (int j=0;j<loops;j++) {
			t.restart();
			for (int i=0;i<nframes;i++) {
				inlen = iOutSize8;
				outlen = iOutSize96;
				speex_resampler_process_float(srs8to96, 0, qv8[i], &inlen, qv96[i], &outlen);
			}
			e = t.elapsed();
			qvTimes.append(e);
		}
		ci = confint(qvTimes);
		qWarning("8 => 96:     %8llu +/- %3llu usec (%d)", ci.first, ci.second, qvTimes.count());

		speex_resampler_reset_mem(srs1);
		speex_resampler_reset_mem(srs2);
	}

	t.restart();
	CALLGRIND_START_INSTRUMENTATION;

	for (int i=0;i<nframes;i++) {
		inlen = iFrameSize;
		outlen = iOutSize1;
		speex_resampler_process_float(srs1, 0, qvIn[i], &inlen, qvDirect[i], &outlen);

		inlen = iFrameSize;
		outlen = iOutSize2;
		speex_resampler_process_float(srs2, 0, qvIn[i], &inlen, qvInterpolate[i], &outlen);

		inlen = iFrameSize;
		outlen = iOutSize2;
		speex_resampler_process_int(srs2i, 0, qvInShort[i], &inlen, qvInterpolateShort[i], &outlen);

		inlen = iFrameSize / 4;
		outlen = iOutSize2 / 4;
		speex_resampler_process_interleaved_float(srss, qvIn[i], &inlen, qvInterpolateMC[i], &outlen);
	}
	e = t.elapsed();

#ifdef Q_OS_WIN
	if (!SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS))
		qWarning("Application: Failed to reset priority!");
#endif

	const int freq[10] = { 22050, 32000, 11025, 16000, 48000, 41000, 8000, 96000, 11025, 1600 };

	QVector<float> fMagic;

	for (int f=0;f<10;f++) {
		float fbuff[32767];
		speex_resampler_set_rate(srs1, 16000, freq[f]);
		for (int q = 0;q < 10;q++) {
			speex_resampler_set_quality(srs1, (3*q) % 7);
			inlen = iFrameSize;
			outlen = 32767;
			speex_resampler_process_float(srs1, 0, qvIn[(f*10+q) % nframes], &inlen, fbuff, &outlen);
			for (int j=0;j<outlen;j++)
				fMagic.append(fbuff[j]);
		}
		inlen = iFrameSize;
		outlen = 32767;
		speex_resampler_process_float(srs1, 0, NULL, &inlen, fbuff, &outlen);
		for (int j=0;j<outlen;j++)
			fMagic.append(fbuff[j]);
	}

	// Cropped magic test
	for (int f=0;f<10;f++) {
		float fbuff[32767];
		speex_resampler_set_rate(srs1, 16000, freq[f]);
		for (int q = 0;q < 10;q++) {
			speex_resampler_set_quality(srs1, (3*q) % 7);
			inlen = iFrameSize;
			outlen = 16;
			speex_resampler_process_float(srs1, 0, qvIn[(f*10+q) % nframes], &inlen, fbuff, &outlen);
			for (int j=0;j<outlen;j++)
				fMagic.append(fbuff[j]);
		}
		inlen = iFrameSize;
		outlen = 32767;
		speex_resampler_process_float(srs1, 0, NULL, &inlen, fbuff, &outlen);
		for (int j=0;j<outlen;j++)
			fMagic.append(fbuff[j]);
	}


	CALLGRIND_STOP_INSTRUMENTATION;

	qWarning("Used %llu usec", e);
	qWarning("%.2f times realtime", (20000ULL * nframes) / (e * 1.0));

	if (! RUNNING_ON_VALGRIND) {
		QVector<float> vDirect;
		QVector<float> vInterpolate;
		QVector<short> vsInterpolate;
		QVector<float> vMagic;
		QVector<float> vInterpolateMC;

		out << fDirect << fInterpolate << sInterpolate << fMagic << fInterpolateMC;

		if (vf.isOpen()) {
			verify >> vDirect >> vInterpolate >> vsInterpolate >> vMagic >> vInterpolateMC;

			double rmsd = veccomp(vDirect, fDirect, "SRS1");
			double rmsi = veccomp(vInterpolate, fInterpolate, "SRS2");
			veccomp(vsInterpolate, sInterpolate, "SRS2i");
			veccomp(vMagic, fMagic, "Magic");
			veccomp(vInterpolateMC, fInterpolateMC, "MC");
			qWarning("Direct: %g", rmsd);
			qWarning("Interp: %g", rmsi);
		} else {
			qWarning("No verification!");
		}
	}

	return 0;
}
